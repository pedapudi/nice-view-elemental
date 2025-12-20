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
    static lv_color_t tmp_buffer[
        LV_CANVAS_BUF_SIZE(
            BATTERY_CANVAS_WIDTH,
            BATTERY_CANVAS_HEIGHT,
            LV_COLOR_FORMAT_GET_BPP(COLOR_FORMAT),
            LV_DRAW_BUF_STRIDE_ALIGN
        )
    ];
    memcpy(tmp_buffer, battery_canvas_buffer, sizeof(tmp_buffer));

    const uint32_t stride = lv_draw_buf_width_to_stride(BATTERY_CANVAS_WIDTH, COLOR_FORMAT);
    lv_draw_sw_rotate(
        tmp_buffer,
        battery_canvas_buffer,
        BATTERY_CANVAS_WIDTH,
        BATTERY_CANVAS_HEIGHT,
        stride,
        stride,
        LV_DISPLAY_ROTATION_180,
        COLOR_FORMAT
    );
}

void render_battery() {
    lv_canvas_fill_bg(battery_canvas, BACKGROUND_COLOR, LV_OPA_100);

    draw_battery(battery_canvas, 0, 0, states.battery);

    rotate_battery_canvas();
}

void rotate_connectivity_canvas() {
    static lv_color_t tmp_buffer[
        LV_CANVAS_BUF_SIZE(
            CONNECTIVITY_CANVAS_WIDTH,
            CONNECTIVITY_CANVAS_HEIGHT,
            LV_COLOR_FORMAT_GET_BPP(COLOR_FORMAT),
            LV_DRAW_BUF_STRIDE_ALIGN
        )
    ];
    memcpy(tmp_buffer, connectivity_canvas_buffer, sizeof(tmp_buffer));

    const uint32_t stride = lv_draw_buf_width_to_stride(CONNECTIVITY_CANVAS_WIDTH, COLOR_FORMAT);
    lv_draw_sw_rotate(
        tmp_buffer,
        connectivity_canvas_buffer,
        CONNECTIVITY_CANVAS_WIDTH,
        CONNECTIVITY_CANVAS_HEIGHT,
        stride,
        stride,
        LV_DISPLAY_ROTATION_180,
        COLOR_FORMAT
    );
}

void render_connectivity() {
    lv_canvas_fill_bg(connectivity_canvas, BACKGROUND_COLOR, LV_OPA_100);

    if (states.connectivity.connected) {
        draw_bluetooth_logo(connectivity_canvas, 16, 0);
    } else {
        draw_bluetooth_logo_outlined(connectivity_canvas, 16, 0);
    }

    rotate_connectivity_canvas();
}

// 1. Insert images here.
const lv_image_dsc_t* images[] = {
    &noise_0,
    &noise_1
};

// 2. Update the number of frames in the animation to match the number of images.
static const unsigned int frame_count = 2;

void initialize_animation() {
    lv_animimg_set_src(image_canvas, (const void**)images, frame_count);
    // 3. Set the time for the whole animation.
    lv_animimg_set_duration(image_canvas, 250);
}

void start_animation() {
    lv_animimg_set_repeat_count(image_canvas, LV_ANIM_REPEAT_INFINITE);
    lv_animimg_start(image_canvas);
}

void stop_animation() {
    lv_animimg_set_repeat_count(image_canvas, 1);
    lv_animimg_start(image_canvas);
}