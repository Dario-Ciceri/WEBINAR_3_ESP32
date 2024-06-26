#ifndef EEZ_LVGL_UI_IMAGES_H
#define EEZ_LVGL_UI_IMAGES_H

#ifdef __cplusplus
extern "C" {
#endif

#include <lvgl.h>

extern const lv_img_dsc_t img_logo;
extern const lv_img_dsc_t img_left;
extern const lv_img_dsc_t img_right;
extern const lv_img_dsc_t img_up;
extern const lv_img_dsc_t img_down;

#ifndef EXT_IMG_DESC_T
#define EXT_IMG_DESC_T
typedef struct _ext_img_desc_t {
    const char *name;
    const lv_img_dsc_t *img_dsc;
} ext_img_desc_t;
#endif

extern const ext_img_desc_t images[5];


#ifdef __cplusplus
}
#endif

#endif /*EEZ_LVGL_UI_IMAGES_H*/
