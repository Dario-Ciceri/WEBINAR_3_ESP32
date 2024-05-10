#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "sdkconfig.h"

#include "esp_log.h"

#include "screens.h"

#include "BluetoothServices.h"

#define TAG "Demo"

// these do not have to be the same, not sure what is optimal
#define TASK_SLEEP_PERIOD_MS 1
#define LV_TICK_PERIOD_MS 1

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
  initBt();
}

void loop() {
  btLoop();
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