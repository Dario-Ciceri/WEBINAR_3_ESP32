#include <WiFi.h> //libreria per il wifi 
#include <WiFiClientSecure.h> //libreria per il wifi 
#include <UniversalTelegramBot.h> //libreria per telegram 

#define WIFI_SSID "nome_wifi" //nome wifi 
#define PASSWORD_WIFI "password_wifi" //password wifi 
#define BOT_API_TOKEN "il_tuo_token_qui" //token chatbot telegram 

#define LED_PIN 2

const unsigned long intervallo = 10; //pausa per la ricezione dei messaggi 
unsigned long ultima_chiamata = 0; //gestione del millis 

// Stringa JSON per la tastiera inline
String inlineKeyboardJson = "";

WiFiClientSecure s_client; //wifi client 
UniversalTelegramBot bot(BOT_API_TOKEN, s_client); //telegram client 

String chat_id = "";
int message_id = -1;
String from_name = "Unknown";

void gestisci_messaggi(int num_nuovi_messaggi) { //funzione per la gestione della chat telegram

  for (int i = 0; i < num_nuovi_messaggi; i++) {

    message_id = bot.messages[i].message_id;
    chat_id = bot.messages[i].chat_id; //id univoco della chat
    String text = bot.messages[i].text; //testo inviato in chat dall'utente
    from_name = bot.messages[i].from_name; //chi manda il messaggio

    if (from_name == "") {
      from_name = "Sconosciuto"; //se non possiamo sapere chi ci scrive, allora è uno sconosciuto
    }

    if (text == "/start") { //se il testo ricevuto è /start allora invia il messaggio di benvenuto
      updateInlineKeyboard();
      bot.sendMessageWithInlineKeyboard(chat_id, "Benvenuto " + (from_name != "Unknown" ? from_name + "!" : "!"), "Markdown", inlineKeyboardJson);
    } else if (text == "/toggleLED") {
      // Funzione per invertire lo stato del LED
      toggleLED();
    }else if (text == "/printMillis") {
      Serial.println(millis());
    }

    Serial.println(from_name); //debug su seriale
    Serial.println(chat_id); //debug su seriale
    Serial.println(text); //debug su seriale
  }
}

void setup() {
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  Serial.begin(115200); //inizializzo la seriale su 115200 baudrate

  Serial.println(); //debug su seriale
  Serial.print("Connetto alla rete wifi "); //debug su seriale
  Serial.println(WIFI_SSID); //debug su seriale

  WiFi.begin(WIFI_SSID, PASSWORD_WIFI); //connetto al wifi

  s_client.setCACert(TELEGRAM_CERTIFICATE_ROOT); //creo una connessione con Telegram utilizzando un certificato di sicurezza

  while (WiFi.status() != WL_CONNECTED) { //finchè il wifi non è connesso attendi e manda dei puntini di sospensione su seriale
    Serial.print("."); //debug su seriale
    delay(500);
  }

  Serial.print("Wifi connesso. Indirizzo IP: "); //debug su seriale
  Serial.println(WiFi.localIP()); //debug su seriale
}

void loop() {
  if (millis() - ultima_chiamata > intervallo) { //se il valore del timer "millis" - l'ultimo valore in millisecondi di quando abbiamo eseguito le istruzioni, è maggiore del tempo di attesa "intervallo", allora esegui le istruzioni
    updateInlineKeyboard();
    bot.sendMessageWithInlineKeyboard(chat_id, "Benvenuto " + (from_name != "Unknown" ? from_name + "!" : "!"), "Markdown", inlineKeyboardJson, message_id);
    int num_nuovi_messaggi = bot.getUpdates(bot.last_message_received + 1); //aggiorna la lista dei messaggi in chat
    while (num_nuovi_messaggi) { //finchè ci sono nuovi messaggi, processali
      Serial.println("messaggio ricevuto"); //debug su seriale
      gestisci_messaggi(num_nuovi_messaggi);
      num_nuovi_messaggi = bot.getUpdates(bot.last_message_received + 1);
    }
    ultima_chiamata = millis(); //aggiorna l'ultimo periodo in millisecondi in cui sono state eseguite le istruzioni all'interno del ciclo while
  }
}

void toggleLED() {
  digitalWrite(LED_PIN, !digitalRead(LED_PIN));
  // Aggiorna la tastiera inline con lo stato del LED
  updateInlineKeyboard();
  // Invia il messaggio con la tastiera inline aggiornata
  bot.sendMessageWithInlineKeyboard(chat_id, "Benvenuto " + (from_name != "Unknown" ? from_name + "!" : "!"), "Markdown", inlineKeyboardJson, message_id);
}

void updateInlineKeyboard() {
  inlineKeyboardJson =
    "["
    "[{ \"text\" : " + String((digitalRead(LED_PIN) ? "\"Spegni LED\"" : "\"Accendi LED\"")) + ", \"callback_data\" : \"/toggleLED\" }],"
    "[{ \"text\" : " + String(millis()) + ", \"callback_data\" : \"/printMillis\" }]"
    "]";
}
