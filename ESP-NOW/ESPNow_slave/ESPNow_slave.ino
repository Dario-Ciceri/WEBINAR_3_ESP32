#include <esp_now.h>
#include <WiFi.h>

#define CHANNEL 1

// Inizializza ESP Now con fallback
void initEspNow() {
  WiFi.disconnect();
  if (esp_now_init() == ESP_OK) {
    Serial.println("Inizializzazione ESPNow riuscita");
  } else {
    Serial.println("Inizializzazione ESPNow fallita");
    ESP.restart();
  }
}

// Configura l'AP SSID
void configuraDispositivoAP() {
  const char *SSID = "Slave_1";
  bool result = WiFi.softAP(SSID, "Slave_1_Password", CHANNEL, 0);
  if (!result) {
    Serial.println("Configurazione AP fallita.");
  } else {
    Serial.println("Configurazione AP riuscita. Trasmissione con AP: " + String(SSID));
    Serial.print("CANALE AP ");
    Serial.println(WiFi.channel());
  }
}

void setup() {
  Serial.begin(115200);
  Serial.println("Esempio ESPNow/Basic/Slave");
  pinMode(2, OUTPUT);
  // Imposta il dispositivo in modalità AP per iniziare
  WiFi.mode(WIFI_AP);
  // Configura la modalità AP del dispositivo
  configuraDispositivoAP();
  // Questo è l'indirizzo MAC dello Slave in modalità AP
  Serial.print("MAC AP: ");
  Serial.println(WiFi.softAPmacAddress());
  // Inizializza ESP Now con una logica di fallback
  initEspNow();
  // Una volta che ESP Now è stato inizializzato con successo, ci registreremo per il CB di ricezione per
  // ottenere le informazioni del pacchetto ricevuto
  esp_now_register_recv_cb(onDataRecv);
}

// callback quando i dati vengono ricevuti dal Master
void onDataRecv(const uint8_t *macAddr, const uint8_t *data, int dataLen) {
  char macStr[18];
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
           macAddr[0], macAddr[1], macAddr[2], macAddr[3], macAddr[4], macAddr[5]);
  Serial.print("Ultimo Pacchetto Ricevuto da: ");
  Serial.println(macStr);
  Serial.print("Ultimo Pacchetto Ricevuto Dati: ");
  Serial.println(*data);
  Serial.println("");
  digitalWrite(2, *data % 2 == 0 ? HIGH : LOW);
}

void loop() {
  // non fare niente...
}
