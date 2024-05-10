#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

const char* ssid = "webinar";
const char* password = "12345678";

void setup() {
  Serial.begin(115200);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connessione in corso...");
  }
  Serial.println("Connesso alla rete WiFi");
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;

    http.begin("https://api.open-meteo.com/v1/forecast?latitude=52.52&longitude=13.41&hourly=temperature_2m");

    int httpCode = http.GET();

    if (httpCode > 0) {
      String payload = http.getString();
      Serial.println(httpCode);
      Serial.println(payload);
      jsonDeserialize(payload);
    } else {
      Serial.println("Errore nella richiesta HTTP");
    }

    http.end();
  }

  delay(60000); // Attendi un minuto prima di fare la prossima richiesta
}

String formatDateTime(String dateTimeStr) {
    // Estrarre anno, mese, giorno, ora e minuti dalla stringa
    String day = dateTimeStr.substring(8, 10);
    String month = dateTimeStr.substring(5, 7);
    String year = dateTimeStr.substring(0, 4);
    String hour = dateTimeStr.substring(11, 13);
    String minute = dateTimeStr.substring(14, 16);

    // Costruire la stringa formattata
    String formattedDateTime = day + "/" + month + "/" + year + " " + hour + ":" + minute;

    return formattedDateTime;
}

void jsonDeserialize(String input) {
  // Stream& input;

  JsonDocument doc;

  DeserializationError error = deserializeJson(doc, input);

  if (error) {
    Serial.print("deserializeJson() failed: ");
    Serial.println(error.c_str());
    return;
  }

  JsonArray hourly_time = doc["hourly"]["time"];
  JsonArray hourly_temperature_2m = doc["hourly"]["temperature_2m"];

  for(int i = 0; i < hourly_time.size(); i++){
    Serial.print(formatDateTime(hourly_time[i].as<String>()));
    Serial.print(": ");
    Serial.println(hourly_temperature_2m[i].as<String>());
  }
}
