#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "sdkconfig.h"

#include "esp_log.h"

#include "screens.h"

#include <esp_now.h>
#include <WiFi.h>

#define CHANNEL 1

#define TAG "Demo"

// these do not have to be the same, not sure what is optimal
#define TASK_SLEEP_PERIOD_MS 4
#define LV_TICK_PERIOD_MS 4

#define LGFX_USE_V1

#define LV_DOUBLE_BUFFER
#define LANDSCAPE  // if changing this, make sure to uncheck landscape in menuconfig -> components -> lvgl -> demos -> music

#include "LGFX_MakerFabs_Parallel_S3.hpp"
// if you get "will not fit, dram segment overflow" reduce this
#ifdef LANDSCAPE
#define LV_BUFFER_SIZE 80 /* if not double buffering, then buf will be 2x this */
#else
#define LV_BUFFER_SIZE 120 /* if not double buffering, then buf will be 2x this */
#endif

static LGFX lcd;

#include "lvgl.h"

/*** Setup screen resolution for LVGL ***/
#ifdef LANDSCAPE
static const uint16_t screenWidth = 480;
static const uint16_t screenHeight = 320;
#else
static const uint16_t screenWidth = 320;
static const uint16_t screenHeight = 480;
#endif
static lv_disp_draw_buf_t draw_buf;

#ifdef LV_DOUBLE_BUFFER
static lv_color_t buf[screenWidth * LV_BUFFER_SIZE];
static lv_color_t buf2[screenWidth * LV_BUFFER_SIZE];
#else
static lv_color_t buf[screenWidth * LV_BUFFER_SIZE * 2];
#endif

static bool disable_flush = false;

/*** Function declaration ***/
static void display_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p);
static void touchpad_read(lv_indev_drv_t *indev_driver, lv_indev_data_t *touchData);
static void lv_tick_task(void *arg);

static void init_lvgl_lgfx() {
  lcd.init();
  lv_init();

#ifdef LANDSCAPE
  // Rotate to landscape
  lcd.setRotation(1);
#endif

  //lcd.setBrightness(10);

#ifdef LV_DOUBLE_BUFFER
  lv_disp_draw_buf_init(&draw_buf, buf, buf2, screenWidth * LV_BUFFER_SIZE);
#else
  lv_disp_draw_buf_init(&draw_buf, buf, NULL, screenWidth * LV_BUFFER_SIZE * 2);
#endif

  /*** LVGL : Setup & Initialize the display device driver ***/
  static lv_disp_drv_t disp_drv;
  lv_disp_drv_init(&disp_drv);
  disp_drv.hor_res = screenWidth;
  disp_drv.ver_res = screenHeight;
  disp_drv.flush_cb = display_flush;
  disp_drv.draw_buf = &draw_buf;
  lv_disp_drv_register(&disp_drv);

  /*** LVGL : Setup & Initialize the input device driver ***/
  static lv_indev_drv_t indev_drv;
  lv_indev_drv_init(&indev_drv);
  indev_drv.type = LV_INDEV_TYPE_POINTER;
  indev_drv.read_cb = touchpad_read;
  lv_indev_drv_register(&indev_drv);

  /* Create and start a periodic timer interrupt to call lv_tick_inc */
  const esp_timer_create_args_t periodic_timer_args = {
    .callback = &lv_tick_task,
    .name = "periodic_gui"
  };
  esp_timer_handle_t periodic_timer;
  ESP_ERROR_CHECK(esp_timer_create(&periodic_timer_args, &periodic_timer));
  ESP_ERROR_CHECK(esp_timer_start_periodic(periodic_timer, LV_TICK_PERIOD_MS * 1000));
}

void setup() {
  Serial.begin(115200);
  init_lvgl_lgfx();
  create_screens();
  lv_scr_load(objects.intro);
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

void loop() {
  lv_timer_handler(); /* let the GUI do its work */
  vTaskDelay(pdMS_TO_TICKS(TASK_SLEEP_PERIOD_MS));
}

/*** Display callback to flush the buffer to screen ***/
static void display_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p) {
#ifdef DISABLE_FLUSH_DURING_BENCHMARK
  if (disable_flush) {
    lv_disp_flush_ready(disp);
    return;
  }
#endif
  uint32_t w = (area->x2 - area->x1 + 1);
  uint32_t h = (area->y2 - area->y1 + 1);

  lcd.startWrite();
  lcd.setAddrWindow(area->x1, area->y1, w, h);
  lcd.writePixels((uint16_t *)&color_p->full, w * h, true);
  lcd.endWrite();

  lv_disp_flush_ready(disp);
}

/*** Touchpad callback to read the touchpad ***/
static void touchpad_read(lv_indev_drv_t *indev_driver, lv_indev_data_t *touchData) {
  uint16_t touchX, touchY;
  bool touched = lcd.getTouch(&touchX, &touchY);

  if (!touched) {
    touchData->state = LV_INDEV_STATE_REL;
  } else {
    touchData->state = LV_INDEV_STATE_PR;

    /*Set the coordinates*/
    touchData->point.x = touchX;
    touchData->point.y = touchY;
  }
}

/* Setting up tick task for lvgl */
static void lv_tick_task(void *arg) {
  (void)arg;
  lv_tick_inc(LV_TICK_PERIOD_MS);
}

// Inizializza ESP Now con fallback
void initEspNow() {
  WiFi.disconnect();
  if (esp_now_init() == ESP_OK) {
    Serial.println("Inizializzazione ESPNow riuscita");
  } else {
    Serial.println("Inizializzazione ESPNow fallita");
    // Riprova initEspNow, aggiungi un conteggio e poi riavvia?
    // initEspNow();
    // o semplicemente riavvia
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

// callback quando i dati vengono ricevuti dal Master
void onDataRecv(const uint8_t *macAddr, const uint8_t *espNowData, int dataLen) {
  lv_scr_load(objects.dashboard);
  char macStr[18];
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
           macAddr[0], macAddr[1], macAddr[2], macAddr[3], macAddr[4], macAddr[5]);
  Serial.print("Ultimo Pacchetto Ricevuto da: ");
  Serial.println(macStr);

  // Converti i dati byte in valori float per il sensore di aria
  float co2Value, tvocValue, h2Value, ethanolValue;
  memcpy(&co2Value, espNowData, sizeof(float));
  memcpy(&tvocValue, espNowData + sizeof(float), sizeof(float));
  memcpy(&h2Value, espNowData + 2 * sizeof(float), sizeof(float));
  memcpy(&ethanolValue, espNowData + 3 * sizeof(float), sizeof(float));

  // Converti i dati byte in valori float per il sensore di pressione
  float temperatureValue, pressureValue, altitudeValue;
  memcpy(&temperatureValue, espNowData + 4 * sizeof(float), sizeof(float));
  memcpy(&pressureValue, espNowData + 5 * sizeof(float), sizeof(float));
  memcpy(&altitudeValue, espNowData + 6 * sizeof(float), sizeof(float));

  // Calcola la lunghezza del messaggio RFID
  int rfidLength = dataLen - 7 * sizeof(float);
  
  // Copia il messaggio RFID come array di caratteri
  char rfid[rfidLength + 1]; 
  memcpy(rfid, espNowData + 7 * sizeof(float), rfidLength);
  rfid[rfidLength] = '\0';

  // Aggiorna le label dell'interfaccia utente con i nuovi valori
  lv_label_set_text(objects.co2_label, (String(co2Value) + "\nppm").c_str());
  lv_label_set_text(objects.h2_label, (String(h2Value) + "\nH2").c_str());
  lv_label_set_text(objects.eth_label, (String(ethanolValue) + "\nEthanol").c_str());
  lv_label_set_text(objects.tvoc_label, (String(tvocValue) + "\nppb").c_str());
  lv_label_set_text(objects.temp_label, (String(temperatureValue) + "\n°C").c_str());
  lv_label_set_text(objects.press_label, (String(pressureValue) + "\nhPa").c_str());
  lv_label_set_text(objects.alt_label, (String(altitudeValue) + "\nm").c_str());
  lv_label_set_text(objects.rfid, rfid);

  // Stampa tutti i valori ricevuti per debug
  Serial.println("Valori ricevuti:");
  Serial.print("CO2: ");
  Serial.println(co2Value);
  Serial.print("TVOC: ");
  Serial.println(tvocValue);
  Serial.print("H2: ");
  Serial.println(h2Value);
  Serial.print("Ethanol: ");
  Serial.println(ethanolValue);
  Serial.print("Temperature: ");
  Serial.println(temperatureValue);
  Serial.print("Pressure: ");
  Serial.println(pressureValue);
  Serial.print("Altitude: ");
  Serial.println(altitudeValue);
  Serial.print("RFID: ");
  Serial.println(rfid);
}
