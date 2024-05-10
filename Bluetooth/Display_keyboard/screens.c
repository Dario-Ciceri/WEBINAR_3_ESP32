#include "screens.h"
#include "images.h"

objects_t objects;
lv_obj_t *tick_value_change_obj;

void create_screen_intro() {
    //void *flowState = getFlowState(0, 0);
    lv_obj_t *obj = lv_obj_create(0);
    objects.intro = obj;
    lv_obj_set_pos(obj, 0, 0);
    lv_obj_set_size(obj, 480, 320);
    lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_bg_color(obj, lv_color_hex(0xff131313), LV_PART_MAIN | LV_STATE_DEFAULT);
    {
        lv_obj_t *parent_obj = obj;
        {
            // logo
            lv_obj_t *obj = lv_img_create(parent_obj);
            objects.logo = obj;
            lv_obj_set_pos(obj, 0, 91);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_img_set_src(obj, &img_logo);
            lv_obj_add_flag(obj, LV_OBJ_FLAG_ADV_HITTEST);
            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
        }
    }
}

void tick_screen_intro() {
    //void *flowState = getFlowState(0, 0);
}

void create_screen_dashboard() {
    //void *flowState = getFlowState(0, 1);
    lv_obj_t *obj = lv_obj_create(0);
    objects.dashboard = obj;
    lv_obj_set_pos(obj, 0, 0);
    lv_obj_set_size(obj, 480, 320);
    lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_bg_color(obj, lv_color_hex(0xff131313), LV_PART_MAIN | LV_STATE_DEFAULT);
    {
        lv_obj_t *parent_obj = obj;
        {
            // terminal
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.terminal = obj;
            lv_obj_set_pos(obj, 0, 0);
            lv_obj_set_size(obj, 480, 320);
            lv_label_set_text(obj, ">");
            lv_obj_set_style_text_color(obj, lv_color_hex(0xff17ff00), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_24, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // cursor
            lv_obj_t *obj = lv_img_create(parent_obj);
            objects.cursor = obj;
            lv_obj_set_pos(obj, 464, 304);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_img_set_src(obj, &img_cursor);
            lv_obj_add_flag(obj, LV_OBJ_FLAG_ADV_HITTEST);
            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
        }
    }
}

void tick_screen_dashboard() {
    //void *flowState = getFlowState(0, 1);
}


void create_screens() {
    lv_disp_t *dispp = lv_disp_get_default();
    lv_theme_t *theme = lv_theme_default_init(dispp, lv_palette_main(LV_PALETTE_BLUE), lv_palette_main(LV_PALETTE_RED), false, LV_FONT_DEFAULT);
    lv_disp_set_theme(dispp, theme);
    
    create_screen_intro();
    create_screen_dashboard();
}

typedef void (*tick_screen_func_t)();

tick_screen_func_t tick_screen_funcs[] = {
    tick_screen_intro,
    tick_screen_dashboard,
};

void tick_screen(int screen_index) {
    tick_screen_funcs[screen_index]();
}
