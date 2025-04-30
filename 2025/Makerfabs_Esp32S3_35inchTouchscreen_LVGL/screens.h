#ifndef EEZ_LVGL_UI_SCREENS_H
#define EEZ_LVGL_UI_SCREENS_H

#include <lvgl.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _objects_t {
    lv_obj_t *intro;
    lv_obj_t *dashboard;
    lv_obj_t *alt_label;
    lv_obj_t *alt_panel;
    lv_obj_t *co2_label;
    lv_obj_t *co2_panel;
    lv_obj_t *eth_label;
    lv_obj_t *eth_panel;
    lv_obj_t *gesture;
    lv_obj_t *h2_label;
    lv_obj_t *h2_panel;
    lv_obj_t *logo;
    lv_obj_t *press_label;
    lv_obj_t *press_panel;
    lv_obj_t *rfid;
    lv_obj_t *temp_label;
    lv_obj_t *temp_panel;
    lv_obj_t *tvoc_label;
    lv_obj_t *tvoc_panel;
} objects_t;

extern objects_t objects;

void create_screen_intro();
void tick_screen_intro();

void create_screen_dashboard();
void tick_screen_dashboard();

void create_screens();
void tick_screen(int screen_index);


#ifdef __cplusplus
}
#endif

#endif /*EEZ_LVGL_UI_SCREENS_H*/
