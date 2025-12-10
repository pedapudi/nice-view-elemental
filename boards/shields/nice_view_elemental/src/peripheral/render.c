#include "../../include/peripheral/render.h"

#include <lvgl.h>
#include <zephyr/sys/util.h>
#include "../../include/colors.h"
#include "../../include/main.h"
#include "../../include/peripheral/initialize_listeners.h"
#include "../../include/utils/draw_battery.h"
#include "../../include/utils/draw_bluetooth_logo_outlined.h"
#include "../../include/utils/draw_bluetooth_logo.h"

#include <stdlib.h>
#include <zephyr/kernel.h>
#include "../../include/images/noise_0.h"
#include "../../include/images/noise_1.h"

void rotate_battery_canvas() {
    static lv_color_t tmp_buffer[LV_CANVAS_BUF_SIZE_TRUE_COLOR(BATTERY_CANVAS_WIDTH, BATTERY_CANVAS_HEIGHT)];
    memcpy(tmp_buffer, battery_canvas_buffer, sizeof(tmp_buffer));

    lv_img_dsc_t tmp_canvas;
    tmp_canvas.data = (void*)tmp_buffer;
    tmp_canvas.header.cf = LV_IMG_CF_TRUE_COLOR;
    tmp_canvas.header.w = BATTERY_CANVAS_WIDTH;
    tmp_canvas.header.h = BATTERY_CANVAS_HEIGHT;

    lv_canvas_fill_bg(battery_canvas, BACKGROUND_COLOR, LV_OPA_COVER);
    lv_canvas_transform(
        battery_canvas,
        &tmp_canvas,
        1800, LV_IMG_ZOOM_NONE,
        -1, 0,
        BATTERY_CANVAS_WIDTH / 2, BATTERY_CANVAS_HEIGHT / 2,
        false
    );
}

void render_battery() {
    lv_canvas_fill_bg(battery_canvas, BACKGROUND_COLOR, LV_OPA_COVER);

    draw_battery(battery_canvas, 0, 0, states.battery);

    rotate_battery_canvas();
}

void rotate_connectivity_canvas() {
    static lv_color_t tmp_buffer[LV_CANVAS_BUF_SIZE_TRUE_COLOR(CONNECTIVITY_CANVAS_WIDTH, CONNECTIVITY_CANVAS_HEIGHT)];
    memcpy(tmp_buffer, connectivity_canvas_buffer, sizeof(tmp_buffer));

    lv_img_dsc_t tmp_canvas;
    tmp_canvas.data = (void*)tmp_buffer;
    tmp_canvas.header.cf = LV_IMG_CF_TRUE_COLOR;
    tmp_canvas.header.w = CONNECTIVITY_CANVAS_WIDTH;
    tmp_canvas.header.h = CONNECTIVITY_CANVAS_HEIGHT;

    lv_canvas_fill_bg(connectivity_canvas, BACKGROUND_COLOR, LV_OPA_COVER);
    lv_canvas_transform(
        connectivity_canvas,
        &tmp_canvas,
        1800, LV_IMG_ZOOM_NONE,
        -1, 0,
        CONNECTIVITY_CANVAS_WIDTH / 2, CONNECTIVITY_CANVAS_HEIGHT / 2,
        false
    );
}

void render_connectivity() {
    lv_canvas_fill_bg(connectivity_canvas, BACKGROUND_COLOR, LV_OPA_COVER);

    if (states.connectivity.connected) {
        draw_bluetooth_logo(connectivity_canvas, 16, 0);
    } else {
        draw_bluetooth_logo_outlined(connectivity_canvas, 16, 0);
    }

    rotate_connectivity_canvas();
}

// 1. Insert images here.
const lv_img_dsc_t* images[] = {
    &noise_0,
    &noise_1
};

// 2. Update the number of frames in the animation to match the number of images.
static const unsigned int frame_count = 1;

void initialize_animation() {
    lv_animimg_set_src(image_canvas, (const void**)images, frame_count);
    // 3. Set the time for the whole animation.
    lv_animimg_set_duration(image_canvas, 100);
}

void start_animation() {
    lv_animimg_set_repeat_count(image_canvas, LV_ANIM_REPEAT_INFINITE);
    lv_animimg_start(image_canvas);
}

void stop_animation() {
    lv_animimg_set_repeat_count(image_canvas, 1);
    lv_animimg_start(image_canvas);
}