#pragma once

#include <zmk/display/status_screen.h>

#include <lvgl.h>

#define SCREEN_WIDTH 160
#define SCREEN_HEIGHT 68

#define COLOR_FORMAT LV_COLOR_FORMAT_L8

#define PADDING 2

#define STATUS_HEIGHT 21

#define BATTERY_CANVAS_WIDTH 24
#define BATTERY_CANVAS_HEIGHT 11

#define CONNECTIVITY_CANVAS_WIDTH 28
#define CONNECTIVITY_CANVAS_HEIGHT 17

#define IMAGE_CANVAS_WIDTH 116
#define IMAGE_CANVAS_HEIGHT 64

#define MODIFIERS_GAP 3
#define MODIFIERS_COUNT 4
#define MODIFIER_FONT_WIDTH 7
#define MODIFIER_PADDING_X 1
#define MODIFIER_WIDTH (MODIFIER_FONT_WIDTH + 2 * MODIFIER_PADDING_X)
#define MODIFIERS_CANVAS_WIDTH (MODIFIERS_COUNT * MODIFIER_WIDTH + (MODIFIERS_COUNT - 1) * MODIFIERS_GAP)
#define MODIFIER_PADDING_Y 3
#define MODIFIERS_CANVAS_HEIGHT 13

#define LAYER_CANVAS_WIDTH (SCREEN_WIDTH - 2 * PADDING)
// This seemingly magic number comes from the font line height used, but the
// variable cannot be used since it is not a static constant.
#define LAYER_CANVAS_HEIGHT 24

extern lv_obj_t* battery_canvas;
extern lv_color_t battery_canvas_buffer[];

extern lv_obj_t* connectivity_canvas;
extern lv_color_t connectivity_canvas_buffer[];

extern lv_obj_t* layer_canvas;
extern lv_color_t layer_canvas_buffer[];

extern lv_obj_t* modifiers_canvas;
extern lv_color_t modifiers_canvas_buffer[];

#if (defined(CONFIG_ZMK_SPLIT) && !defined(CONFIG_ZMK_SPLIT_ROLE_CENTRAL))
extern lv_obj_t* image_canvas;
extern lv_color_t image_canvas_buffer[];
#endif
