#include "../../include/utils/draw_battery.h"

#include <lvgl.h>
#include "../../include/colors.h"

static void draw_battery_outline(lv_obj_t* canvas, lv_coord_t x, lv_coord_t y) {
    lv_draw_rect_dsc_t rect_dsc;
    lv_draw_rect_dsc_init(&rect_dsc);
    rect_dsc.bg_color = FOREGROUND_COLOR;

    lv_layer_t layer;
    lv_canvas_init_layer(canvas, &layer);

    lv_area_t b1 = { x + 2, y, x + 2 + 19 - 1, y + 1 - 1 };
    lv_area_t b2 = { x + 22, y + 2, x + 22 + 1 - 1, y + 2 + 7 - 1 };
    lv_area_t b3 = { x + 2, y + 10, x + 2 + 19 - 1, y + 10 + 1 - 1 };
    lv_area_t b4 = { x, y + 2, x + 1 - 1, y + 2 + 7 - 1 };
    lv_draw_rect(&layer, &rect_dsc, &b1); // Top
    lv_draw_rect(&layer, &rect_dsc, &b2); // Right
    lv_draw_rect(&layer, &rect_dsc, &b3); // Bottom
    lv_draw_rect(&layer, &rect_dsc, &b4); // Left

    lv_canvas_set_px(canvas, x + 1, y + 1, FOREGROUND_COLOR, LV_OPA_100); // Top left
    lv_canvas_set_px(canvas, x + 21, y + 1, FOREGROUND_COLOR, LV_OPA_100); // Top right
    lv_canvas_set_px(canvas, x + 21, y + 9, FOREGROUND_COLOR, LV_OPA_100); // Bottom right
    lv_canvas_set_px(canvas, x + 1, y + 9, FOREGROUND_COLOR, LV_OPA_100); // Bottom left

    lv_area_t b5 = { x + 23, y + 4, x + 23 + 1 - 1, y + 4 + 3 - 1 };
    lv_draw_rect(&layer, &rect_dsc, &b5); // Right-side terminal

    lv_canvas_finish_layer(canvas, &layer);
}

// The lightning bolt is drawn line by line.
static void draw_battery_lightning_bolt(lv_obj_t* canvas, lv_coord_t x, lv_coord_t y) {
    lv_canvas_set_px(canvas, x + 11, y + 2, BACKGROUND_COLOR, LV_OPA_100);
    lv_canvas_set_px(canvas, x + 12, y + 2, FOREGROUND_COLOR, LV_OPA_100);
    lv_canvas_set_px(canvas, x + 13, y + 2, BACKGROUND_COLOR, LV_OPA_100);
   
    lv_canvas_set_px(canvas, x + 10, y + 3, BACKGROUND_COLOR, LV_OPA_100);
    lv_canvas_set_px(canvas, x + 11, y + 3, FOREGROUND_COLOR, LV_OPA_100);
    lv_canvas_set_px(canvas, x + 12, y + 3, BACKGROUND_COLOR, LV_OPA_100);

    lv_canvas_set_px(canvas, x + 9, y + 4, BACKGROUND_COLOR, LV_OPA_100);
    lv_canvas_set_px(canvas, x + 10, y + 4, FOREGROUND_COLOR, LV_OPA_100);
    lv_canvas_set_px(canvas, x + 11, y + 4, FOREGROUND_COLOR, LV_OPA_100);
    lv_canvas_set_px(canvas, x + 12, y + 4, BACKGROUND_COLOR, LV_OPA_100);

    lv_canvas_set_px(canvas, x + 9, y + 5, BACKGROUND_COLOR, LV_OPA_100);
    lv_canvas_set_px(canvas, x + 10, y + 5, FOREGROUND_COLOR, LV_OPA_100);
    lv_canvas_set_px(canvas, x + 11, y + 5, FOREGROUND_COLOR, LV_OPA_100);
    lv_canvas_set_px(canvas, x + 12, y + 5, FOREGROUND_COLOR, LV_OPA_100);
    lv_canvas_set_px(canvas, x + 13, y + 5, BACKGROUND_COLOR, LV_OPA_100);

    lv_canvas_set_px(canvas, x + 10, y + 6, BACKGROUND_COLOR, LV_OPA_100);
    lv_canvas_set_px(canvas, x + 11, y + 6, FOREGROUND_COLOR, LV_OPA_100);
    lv_canvas_set_px(canvas, x + 12, y + 6, FOREGROUND_COLOR, LV_OPA_100);
    lv_canvas_set_px(canvas, x + 13, y + 6, BACKGROUND_COLOR, LV_OPA_100);

    lv_canvas_set_px(canvas, x + 10, y + 7, BACKGROUND_COLOR, LV_OPA_100);
    lv_canvas_set_px(canvas, x + 11, y + 7, FOREGROUND_COLOR, LV_OPA_100);
    lv_canvas_set_px(canvas, x + 12, y + 7, BACKGROUND_COLOR, LV_OPA_100);

    lv_canvas_set_px(canvas, x + 9, y + 8, BACKGROUND_COLOR, LV_OPA_100);
    lv_canvas_set_px(canvas, x + 10, y + 8, FOREGROUND_COLOR, LV_OPA_100);
    lv_canvas_set_px(canvas, x + 11, y + 8, BACKGROUND_COLOR, LV_OPA_100);
}

void draw_battery(lv_obj_t* canvas, lv_coord_t x, lv_coord_t y, struct battery_state state) {
    draw_battery_outline(canvas, x, y);

    lv_layer_t layer;
    lv_canvas_init_layer(canvas, &layer);

    // Draw the main part of the battery
    const int width = 19 * (state.level / 100.0);
    lv_draw_rect_dsc_t rect_dsc;
    lv_draw_rect_dsc_init(&rect_dsc);
    rect_dsc.bg_color = FOREGROUND_COLOR;
    lv_area_t rect_area = { 
        x + 2,
        y + 2,
        x + 2 + width - 1,
        y + 2 + 7 - 1
    };
    lv_draw_rect(&layer, &rect_dsc, &rect_area);
    
    lv_canvas_finish_layer(canvas, &layer);

    // Round the corners
    lv_canvas_set_px(canvas, x + 2, y + 2, BACKGROUND_COLOR, LV_OPA_100); // Top left
    lv_canvas_set_px(canvas, x + 20, y + 2, BACKGROUND_COLOR, LV_OPA_100); // Top right
    lv_canvas_set_px(canvas, x + 20, y + 8, BACKGROUND_COLOR, LV_OPA_100); // Bottom right
    lv_canvas_set_px(canvas, x + 2, y + 8, BACKGROUND_COLOR, LV_OPA_100); // Bottom left

    if (state.is_charging) {
        draw_battery_lightning_bolt(canvas, x, y);
    }
}
