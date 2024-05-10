/*
   Futura Academy: https://academy.futuranet.it/
   Webinar 2: Esp32 Futura Demoboard
   Dario Ciceri
   Canale YouTube: https://www.youtube.com/channel/UCuPuHsNjWX7huiztYu9ROQA
   Profilo Instagram: https://www.instagram.com/_dario.ciceri_/
*/

#include <Arduino.h>
#include <esp_now.h>
#include <WiFi.h>
#include <esp_wifi.h>  // solo per esp_wifi_set_channel()

// Copia globale dello slave
esp_now_peer_info_t slave;
#define CHANNEL 1
#define PRINT_SCAN_RESULTS 0
#define DELETE_BEFORE_PAIR 0

// Inizializza ESP Now con fallback
void initEspNow() {
  WiFi.disconnect();
  if (esp_now_init() == ESP_OK) {
    Serial.println("Inizializzazione ESPNow riuscita");
  } else {
    Serial.println("Inizializzazione ESPNow fallita");
    // Riprova l'inizializzazione di EspNow, aggiungi un conteggio e quindi riavvia?
    // initEspNow();
    // o semplicemente riavvia
    ESP.restart();
  }
}

// Scansiona gli slave in modalità AP
void scanForSlave() {
  int16_t scanResults = WiFi.scanNetworks(false, false, false, 300, CHANNEL);  // Scansione solo su un canale
  // resetta su ogni scansione
  bool slaveFound = 0;
  memset(&slave, 0, sizeof(slave));

  Serial.println("");
  if (scanResults == 0) {
    Serial.println("Nessun dispositivo WiFi in modalità AP trovato");
  } else {
    Serial.print("Trovati ");
    Serial.print(scanResults);
    Serial.println(" dispositivi ");
    for (int i = 0; i < scanResults; ++i) {
      // Stampa SSID e RSSI per ogni dispositivo trovato
      String SSID = WiFi.SSID(i);
      int32_t RSSI = WiFi.RSSI(i);
      String BSSIDstr = WiFi.BSSIDstr(i);

      if (PRINT_SCAN_RESULTS) {
        Serial.print(i + 1);
        Serial.print(": ");
        Serial.print(SSID);
        Serial.print(" (");
        Serial.print(RSSI);
        Serial.print(")");
        Serial.println("");
      }
      delay(10);
      // Controlla se il dispositivo corrente inizia con `Slave`
      if (SSID.indexOf("Slave") == 0) {
        // SSID di interesse
        Serial.println("Trovato uno Slave.");
        Serial.print(i + 1);
        Serial.print(": ");
        Serial.print(SSID);
        Serial.print(" [");
        Serial.print(BSSIDstr);
        Serial.print("]");
        Serial.print(" (");
        Serial.print(RSSI);
        Serial.print(")");
        Serial.println("");
        // Ottieni BSSID => Indirizzo Mac dello Slave
        int mac[6];
        if (6 == sscanf(BSSIDstr.c_str(), "%x:%x:%x:%x:%x:%x", &mac[0], &mac[1], &mac[2], &mac[3], &mac[4], &mac[5])) {
          for (int ii = 0; ii < 6; ++ii) {
            slave.peer_addr[ii] = (uint8_t)mac[ii];
          }
        }

        slave.channel = CHANNEL;  // scegli un canale
        slave.encrypt = 0;        // nessuna crittografia

        slaveFound = 1;
        // stiamo pianificando di avere solo uno slave in questo esempio;
        // Pertanto, interrompi dopo averne trovato uno, per essere un po' efficienti
        break;
      }
    }
  }

  if (slaveFound) {
    Serial.println("Slave trovato, elaborazione..");
  } else {
    Serial.println("Slave non trovato, riprovare.");
  }

  // pulizia della memoria RAM
  WiFi.scanDelete();
}

// Controlla se lo slave è già accoppiato con il master.
// Se no, accoppia lo slave con il master
bool manageSlave() {
  if (slave.channel == CHANNEL) {
    if (DELETE_BEFORE_PAIR) {
      deletePeer();
    }

    Serial.print("Stato Slave: ");
    // controlla se il peer esiste
    bool exists = esp_now_is_peer_exist(slave.peer_addr);
    if (exists) {
      // Slave già accoppiato.
      Serial.println("Già accoppiato");
      return true;
    } else {
      // Slave non accoppiato, tentativo di accoppiamento
      esp_err_t addStatus = esp_now_add_peer(&slave);
      if (addStatus == ESP_OK) {
        // Accoppiamento riuscito
        Serial.println("Accoppiamento riuscito");
        return true;
      } else if (addStatus == ESP_ERR_ESPNOW_NOT_INIT) {
        // Come siamo arrivati fin qui!!
        Serial.println("ESPNow Non Inizializzato");
        return false;
      } else if (addStatus == ESP_ERR_ESPNOW_ARG) {
        Serial.println("Argomento non valido");
        return false;
      } else if (addStatus == ESP_ERR_ESPNOW_FULL) {
        Serial.println("Lista peer piena");
        return false;
      } else if (addStatus == ESP_ERR_ESPNOW_NO_MEM) {
        Serial.println("Memoria esaurita");
        return false;
      } else if (addStatus == ESP_ERR_ESPNOW_EXIST) {
        Serial.println("Peer Esistente");
        return true;
      } else {
        Serial.println("Non si sa cosa sia successo");
        return false;
      }
    }
  } else {
    // Nessuno slave trovato da elaborare
    Serial.println("Nessuno slave trovato da elaborare");
    return false;
  }
}

void deletePeer() {
  esp_err_t delStatus = esp_now_del_peer(slave.peer_addr);
  Serial.print("Stato Eliminazione Slave: ");
  if (delStatus == ESP_OK) {
    // Eliminazione riuscita
    Serial.println("Successo");
  } else if (delStatus == ESP_ERR_ESPNOW_NOT_INIT) {
    // Come siamo arrivati fin qui!!
    Serial.println("ESPNow Non Inizializzato");
  } else if (delStatus == ESP_ERR_ESPNOW_ARG) {
    Serial.println("Argomento non valido");
  } else if (delStatus == ESP_ERR_ESPNOW_NOT_FOUND) {
    Serial.println("Peer non trovato.");
  } else {
    Serial.println("Non si sa cosa sia successo");
  }
}

#include "src/FuturaBoard.h"

#define LIGHT_INT_HIGH  1000
#define LIGHT_INT_LOW   10

#define PROX_INT_HIGH   50
#define PROX_INT_LOW    0

OLED display;
AQS sensoreAria;
Gesture sensoreGesture(LIGHT_INT_HIGH, LIGHT_INT_LOW, PROX_INT_HIGH, PROX_INT_LOW, &display);
Lightning sensoreFulmini;
Pressure sensorePressione;
RFID sensoreNfc;

void interruptRoutine() {
  sensoreGesture.isr_flag = 1;
}

// invia dati
void sendData() {
  // Leggi i dati dal sensore di aria
  SensorData dataAria = sensoreAria.read();

  // Leggi i dati dal sensore di pressione
  PressureData dataPressione = sensorePressione.read();

  // Leggi il messaggio dal sensore NFC
  String message = sensoreNfc.extendedRead();
  Serial.println(message);

  // Calcola la lunghezza totale dei dati
  size_t totalLength = sizeof(float) * 7 + message.length() + 1;

  // Crea un array per contenere tutti i dati
  uint8_t dataArray[totalLength];

  // Converti i valori float in byte per i dati del sensore di aria
  memcpy(dataArray, &dataAria.co2, sizeof(float));
  memcpy(dataArray + sizeof(float), &dataAria.tvoc, sizeof(float));
  memcpy(dataArray + 2 * sizeof(float), &dataAria.h2, sizeof(float));
  memcpy(dataArray + 3 * sizeof(float), &dataAria.ethanol, sizeof(float));

  // Converti i valori float in byte per i dati del sensore di pressione
  memcpy(dataArray + 4 * sizeof(float), &dataPressione.temperature, sizeof(float));
  memcpy(dataArray + 5 * sizeof(float), &dataPressione.pressure, sizeof(float));
  memcpy(dataArray + 6 * sizeof(float), &dataPressione.altitude, sizeof(float));

  // Copia il messaggio NFC come stringa
  message.toCharArray((char *)(dataArray + sizeof(float) * 7), message.length() + 1);

  const uint8_t *peerAddr = slave.peer_addr;

  // Invia i dati tramite ESP Now
  esp_err_t result = esp_now_send(peerAddr, dataArray, totalLength);

  Serial.print("Stato Invio: ");
  if (result == ESP_OK) {
    Serial.println("Successo");
  } else if (result == ESP_ERR_ESPNOW_NOT_INIT) {
    Serial.println("ESPNow non Inizializzato.");
  } else if (result == ESP_ERR_ESPNOW_ARG) {
    Serial.println("Argomento non valido");
  } else if (result == ESP_ERR_ESPNOW_INTERNAL) {
    Serial.println("Errore Interno");
  } else if (result == ESP_ERR_ESPNOW_NO_MEM) {
    Serial.println("ESP_ERR_ESPNOW_NO_MEM");
  } else if (result == ESP_ERR_ESPNOW_NOT_FOUND) {
    Serial.println("Peer non trovato.");
  } else {
    Serial.println("Non si sa cosa sia successo");
  }
}



// callback quando i dati vengono inviati da Master a Slave
void onDataSent(const uint8_t *macAddr, esp_now_send_status_t status) {
  char macStr[18];
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
           macAddr[0], macAddr[1], macAddr[2], macAddr[3], macAddr[4], macAddr[5]);
  Serial.print("Ultimo Pacchetto Inviato a: ");
  Serial.println(macStr);
  Serial.print("Ultimo Stato Invio Pacchetto: ");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Consegna Riuscita" : "Consegna Fallita");
}

void setup() {
  Serial.begin(115200);
  attachInterrupt(sensoreGesture.intPin, interruptRoutine, FALLING);
  printLogo();
  display.init();
  display.show3dCube();
  display.logo();
  display.clear();
  display.displayText(0, 15, "Caricamento...");
  display.clear();
  display.displayText(0, 15, "pressione...");
  sensorePressione.init();
  display.clear();
  display.displayText(0, 15, "AQS...");
  sensoreAria.init();
  display.clear();
  display.displayText(0, 15, "Gestures...");
  sensoreGesture.init();
  display.clear();
  display.displayText(0, 15, "Fulmini...");
  sensoreFulmini.init();
  display.clear();
  display.displayText(0, 15, "NFC...");
  sensoreNfc.init();
  display.clear();
  display.displayText(0, 15, "Fatto!");
  delay(1500);
  display.clear();
  WiFi.mode(WIFI_STA);
  esp_wifi_set_channel(CHANNEL, WIFI_SECOND_CHAN_NONE);
  Serial.println("Esempio ESPNow/Basic/Master");
  // Questo è l'indirizzo MAC del Master in modalità Station
  Serial.print("STA MAC: ");
  Serial.println(WiFi.macAddress());
  Serial.print("CANALE STA ");
  Serial.println(WiFi.channel());
  // Inizializza ESP Now con una logica di fallback
  initEspNow();
  // Una volta che ESP Now è stato inizializzato con successo, ci registreremo per il CB di invio per
  // ottenere lo stato del pacchetto trasmesso
  esp_now_register_send_cb(onDataSent);
}

void loop() {
  //sensorePressione.read();
  //sensoreAria.read();
  //sensoreGesture.handleGesture();
  //sensoreFulmini.read();
  //sensoreNfc.writeMultipleRecords();
  //sensoreNfc.read();
  //delay(3000);
  // Nel loop scansioniamo gli slave
  scanForSlave();
  // Se viene trovato lo Slave, sarà popolato nella variabile `slave`
  // Controlliamo se `slave` è definito e quindi procediamo ulteriormente
  if (slave.channel == CHANNEL) {  // controlla se il canale dello slave è definito
    // `slave` è definito
    // Aggiungi slave come peer se non è già stato aggiunto
    bool isPaired = manageSlave();
    if (isPaired) {
      // accoppiamento riuscito o già accoppiato
      // Invia dati al dispositivo
      sendData();
    } else {
      // accoppiamento slave fallito
      Serial.println("Accoppiamento Slave fallito!");
    }
  } else {
    // Nessuno slave trovato da elaborare
  }
  delay(1000);
}

void printLogo() {
  for (int i = 0; i < 50; i++) {
    Serial.println();
  }
  Serial.println(" ______     _                                           _                      ");
  Serial.println("|  ____|   | |                       /\\                | |                     ");
  Serial.println("| |__ _   _| |_ _   _ _ __ __ _     /  \\   ___ __ _  __| | ___ _ __ ___  _   _ ");
  Serial.println("|  __| | | | __| | | | '__/ _` |   / /\\ \\ / __/ _` |/ _` |/ _ \\ '_ ` _ \\| | | |");
  Serial.println("| |  | |_| | |_| |_| | | | (_| |  / ____ \\ (_| (_| | (_| |  __/ | | | | | |_| |");
  Serial.println("|_|   \\__,_|\\__|\\__,_|_|  \\__,_| /_/    \\_\\___\\__,_|\\__,_|\\___|_| |_| |_|\\__, |");
  Serial.println("                                                                          __/ |");
  Serial.println("                                                                         |___/ ");
  Serial.println("Futura Academy: https://academy.futuranet.it/");
  Serial.println("Canale YouTube: https://www.youtube.com/channel/UCuPuHsNjWX7huiztYu9ROQA");
  Serial.println("Profilo Instagram: https://www.instagram.com/_dario.ciceri_/");
  Serial.println("Versione Firmware: " + String(__DATE__) + " " + String(__TIME__));
  delay(3000);
}
