#include <Arduino.h>
#include "FS.h"
#include <LittleFS.h>
#include <WiFiManager.h>  // https://github.com/tzapu/WiFiManager

#define FORMAT_LITTLEFS_IF_FAILED true

WiFiManager wm;                     // istanza della classe WiFiManager
WiFiManagerParameter customField;  // istanza della classe WiFiManagerParameter per un parametro custom

void setup() {
  WiFi.mode(WIFI_STA);  // imposta esplicitamente la modalità, esp di default è STA+AP
  Serial.begin(115200);
  Serial.setDebugOutput(true);
  Serial.println("\nConfigurazione in corso...");

  if (!LittleFS.begin(FORMAT_LITTLEFS_IF_FAILED)) {
    Serial.println("Montaggio di LittleFS fallito");
    return;
  }

  int customFieldLength = 40;
  new (&customField) WiFiManagerParameter("customfieldid", "Campo personalizzato", readFile(LittleFS, "/config.txt").c_str(), customFieldLength, "placeholder=\"Inserire parametro\"");

  wm.addParameter(&customField);
  wm.setSaveParamsCallback(saveParamCallback);

  std::vector<const char *> menu = { "wifi", "info", "param", "sep", "restart", "exit" };
  wm.setMenu(menu);

  //wm.setClass("invert"); // usato solo nel secondo file per OTA

  bool res;
  res = wm.autoConnect("ESP32_AP", "password");  // access point protetto da password

  if (!res) {
    Serial.println("Connessione fallita o timeout raggiunto");
  } else {
    Serial.println("Connessione riuscita!");
  }
}

void loop() {
}

String readFile(fs::FS &fs, const char *path) {
  String content = "";

  Serial.printf("Lettura file: %s\r\n", path);

  File file = fs.open(path);
  if (!file || file.isDirectory()) {
    Serial.println("- Impossibile aprire il file in lettura");
    return content;  // Ritorna una stringa vuota se non è possibile aprire il file
  }

  Serial.print("- Lettura dal file: ");
  while (file.available()) {
    content += (char)file.read();  // Aggiungi ogni carattere letto alla stringa
  }
  file.close();

  Serial.println(content);

  return content;  // Ritorna la stringa contenente il testo letto dal file
}

void writeFile(fs::FS &fs, const char *path, const char *message) {
  Serial.printf("Scrittura file: %s\r\n", path);

  File file = fs.open(path, FILE_WRITE);
  if (!file) {
    Serial.println("- Impossibile aprire il file in scrittura");
    return;
  }
  if (file.print(message)) {
    Serial.println("- File scritto correttamente");
  } else {
    Serial.println("- Scrittura fallita");
  }
  file.close();
}

String getParam(String name) {
  // Leggi parametro dal server, per input customhtml
  String value;
  if (wm.server->hasArg(name)) {
    value = wm.server->arg(name);
  }
  return value;
}

void saveParamCallback() {
  String customFieldValue = getParam("customfieldid");
  Serial.println("[CALLBACK] Callback di salvataggio dei parametri attivato");
  Serial.println("Param customfieldid = " + customFieldValue);
  writeFile(LittleFS, "/config.txt", customFieldValue.c_str());
}
