#include "screens.h"
#include "images.h"

objects_t objects;
lv_obj_t *tick_value_change_obj;

void create_screen_intro() {
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
}

void create_screen_dashboard() {
    lv_obj_t *obj = lv_obj_create(0);
    objects.dashboard = obj;
    lv_obj_set_pos(obj, 0, 0);
    lv_obj_set_size(obj, 480, 320);
    lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_bg_color(obj, lv_color_hex(0xff131313), LV_PART_MAIN | LV_STATE_DEFAULT);
    {
        lv_obj_t *parent_obj = obj;
        {
            // temp_panel
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.temp_panel = obj;
            lv_obj_set_pos(obj, 5, 5);
            lv_obj_set_size(obj, 100, 50);
            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xff1e1e1e), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    // temp_label
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.temp_label = obj;
                    lv_obj_set_pos(obj, -18, -17);
                    lv_obj_set_size(obj, 100, LV_SIZE_CONTENT);
                    lv_label_set_text(obj, "23.50\n°C");
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_22, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                }
            }
        }
        {
            // alt_panel
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.alt_panel = obj;
            lv_obj_set_pos(obj, 190, 5);
            lv_obj_set_size(obj, 100, 50);
            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xff1e1e1e), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    // alt_label
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.alt_label = obj;
                    lv_obj_set_pos(obj, -18, -15);
                    lv_obj_set_size(obj, 100, LV_SIZE_CONTENT);
                    lv_label_set_text(obj, "253.06\nm");
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                }
            }
        }
        {
            // press_panel
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.press_panel = obj;
            lv_obj_set_pos(obj, 375, 5);
            lv_obj_set_size(obj, 100, 50);
            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xff1e1e1e), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    // press_label
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.press_label = obj;
                    lv_obj_set_pos(obj, -18, -14);
                    lv_obj_set_size(obj, 100, LV_SIZE_CONTENT);
                    lv_label_set_text(obj, "98321.75\nhPa");
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                }
            }
        }
        {
            // co2_panel
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.co2_panel = obj;
            lv_obj_set_pos(obj, 5, 75);
            lv_obj_set_size(obj, 100, 50);
            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xff1e1e1e), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    // co2_label
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.co2_label = obj;
                    lv_obj_set_pos(obj, -18, -14);
                    lv_obj_set_size(obj, 100, LV_SIZE_CONTENT);
                    lv_label_set_text(obj, "400.00\nppm");
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                }
            }
        }
        {
            // h2_panel
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.h2_panel = obj;
            lv_obj_set_pos(obj, 252, 75);
            lv_obj_set_size(obj, 100, 50);
            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xff1e1e1e), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    // h2_label
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.h2_label = obj;
                    lv_obj_set_pos(obj, -18, -14);
                    lv_obj_set_size(obj, 100, LV_SIZE_CONTENT);
                    lv_label_set_text(obj, "14011.00\nH2");
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                }
            }
        }
        {
            // eth_panel
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.eth_panel = obj;
            lv_obj_set_pos(obj, 375, 75);
            lv_obj_set_size(obj, 100, 50);
            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xff1e1e1e), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    // eth_label
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.eth_label = obj;
                    lv_obj_set_pos(obj, -18, -14);
                    lv_obj_set_size(obj, 100, LV_SIZE_CONTENT);
                    lv_label_set_text(obj, "18543.00\nEthanol");
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                }
            }
        }
        {
            // tvoc_panel
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.tvoc_panel = obj;
            lv_obj_set_pos(obj, 128, 75);
            lv_obj_set_size(obj, 100, 50);
            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xff1e1e1e), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    // tvoc_label
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.tvoc_label = obj;
                    lv_obj_set_pos(obj, -18, -15);
                    lv_obj_set_size(obj, 100, LV_SIZE_CONTENT);
                    lv_label_set_text(obj, "9.00\nppb");
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                }
            }
        }
        {
            // RFID
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.rfid = obj;
            lv_obj_set_pos(obj, 0, 291);
            lv_obj_set_size(obj, 480, LV_SIZE_CONTENT);
            lv_label_set_text(obj, "VALORE RFID");
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_22, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // gesture
            lv_obj_t *obj = lv_img_create(parent_obj);
            objects.gesture = obj;
            lv_obj_set_pos(obj, 176, 145);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_img_set_src(obj, &img_down);
            lv_obj_add_flag(obj, LV_OBJ_FLAG_ADV_HITTEST);
            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
            lv_obj_set_style_img_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
    }
}

void tick_screen_dashboard() {
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
