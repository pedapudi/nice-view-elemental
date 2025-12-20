#include "../../include/central/render.h"

#include <ctype.h>
#include <lvgl.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <zephyr/sys/util.h>
#include "../../include/colors.h"
#include "../../include/central/initialize_listeners.h"
#include "../../include/fonts/press_start_2p_8.h"
#include "../../include/fonts/press_start_2p_16.h"
#include "../../include/fonts/press_start_2p_24.h"
#include "../../include/main.h"
#include "../../include/utils/draw_battery.h"
#include "../../include/utils/draw_bluetooth_searching.h"
#include "../../include/utils/draw_bluetooth_logo_outlined.h"
#include "../../include/utils/draw_bluetooth_logo.h"
#include "../../include/utils/draw_usb_logo.h"

void render_battery() {
    lv_canvas_fill_bg(battery_canvas, BACKGROUND_COLOR, LV_OPA_100);

    draw_battery(battery_canvas, 0, 0, states.battery);
}

static void render_bluetooth_logo() {
    if (states.connectivity.active_profile_bonded) {
        if (states.connectivity.active_profile_connected) {
            draw_bluetooth_logo(connectivity_canvas, 16, 0);
        } else {
            draw_bluetooth_logo_outlined(connectivity_canvas, 16, 0);
        }
    } else {
        draw_bluetooth_searching(connectivity_canvas, 16, 0);
    }
}

static void render_bluetooth_profile_index() {
    lv_draw_label_dsc_t label_dsc;
    lv_draw_label_dsc_init(&label_dsc);
    label_dsc.color = FOREGROUND_COLOR;
    label_dsc.font = &press_start_2p_16;
    label_dsc.align = LV_TEXT_ALIGN_RIGHT;
    
    // Magic number
    static const unsigned padding_x_offset = 2;
    static const unsigned padding_x = padding_x_offset;
    
    // Magic number
    static const unsigned padding_y_offset = 1;
    // `ceil` is used to tend towards the bottom of the screen.
    const unsigned padding_y = ceil((CONNECTIVITY_CANVAS_HEIGHT - press_start_2p_16.line_height) / 2) + padding_y_offset;
    static const unsigned width = CONNECTIVITY_CANVAS_WIDTH - 12 - 2;
    static const char bluetooth_profile_label[5][2] = {"1", "2", "3", "4", "5"};
    const char* label = bluetooth_profile_label[states.connectivity.active_profile_index];
    label_dsc.text = label;

    lv_layer_t layer;
    lv_canvas_init_layer(connectivity_canvas, &layer);

    lv_area_t coords_rect = { 
        padding_x,
        padding_y,
        padding_x + width - 1,
        padding_y + press_start_2p_16.line_height - 1
    };
   
    lv_draw_label(&layer, &label_dsc, &coords_rect);

    lv_canvas_finish_layer(connectivity_canvas, &layer);
}

static void render_bluetooth_connectivity() {
    render_bluetooth_logo();
    render_bluetooth_profile_index();
}

static void render_usb_connectivity() {
    draw_usb_logo(connectivity_canvas, 10, 4);
}

void render_connectivity() {
    lv_canvas_fill_bg(connectivity_canvas, BACKGROUND_COLOR, LV_OPA_100);

    switch (states.connectivity.selected_endpoint.transport) {
        case ZMK_TRANSPORT_BLE: {
            render_bluetooth_connectivity();
            break;
        }
        case ZMK_TRANSPORT_USB: {
            render_usb_connectivity();
            break;
        }
    }
}

void render_main() {
    lv_layer_t layer;
    lv_canvas_init_layer(layer_canvas, &layer);

    lv_draw_rect_dsc_t background_dsc;
    lv_draw_rect_dsc_init(&background_dsc);
    background_dsc.bg_color = BACKGROUND_COLOR;

    lv_area_t coords_rect = { 
        0,
        0,
        0 + LAYER_CANVAS_WIDTH - 1,
        0 + LAYER_CANVAS_HEIGHT - 1
    };

    lv_draw_rect(&layer, &background_dsc, &coords_rect);

    // Capitalize the layer name if given or use the layer number otherwise.
    char* text = NULL;
    if (states.layer.name == NULL) {
        text = malloc(10 * sizeof(char));
        sprintf(text, "LAYER %i", states.layer.index);
    }
    else {
        text = malloc((strlen(states.layer.name) + 1) * sizeof(char));
        for (unsigned i = 0; states.layer.name[i] != '\0'; i++) {
#if IS_ENABLED(NICE_VIEW_ELEMENTAL_CAPITALIZATION)
            text[i] = toupper(states.layer.name[i]);
#else
            text[i] = states.layer.name[i];
#endif
        }
        text[strlen(states.layer.name)] = '\0';
    }

    lv_draw_label_dsc_t layer_name_dsc;
    lv_draw_label_dsc_init(&layer_name_dsc);
    layer_name_dsc.color = FOREGROUND_COLOR;
    layer_name_dsc.font = &press_start_2p_24;
    layer_name_dsc.align = LV_TEXT_ALIGN_CENTER;
    layer_name_dsc.text = text;

    lv_area_t layer_name_coords_rect = {
        0,
        0,
        LAYER_CANVAS_WIDTH - 1,
        LAYER_CANVAS_HEIGHT - 1
    };
    
    lv_draw_label(
        &layer,
        &layer_name_dsc,
        &layer_name_coords_rect
    );

    lv_canvas_finish_layer(layer_canvas, &layer);

    free(text);
    text = NULL;
}

void render_modifiers() {
    lv_draw_rect_dsc_t background_dsc;
    lv_draw_rect_dsc_init(&background_dsc);
    background_dsc.bg_color = BACKGROUND_COLOR;

    lv_area_t background_coords_rect = { 
        0,
        0,
        0 + MODIFIERS_CANVAS_WIDTH - 1,
        0 + MODIFIERS_CANVAS_HEIGHT - 1
    };

    lv_layer_t layer;
    lv_canvas_init_layer(modifiers_canvas, &layer);
    
    lv_draw_rect(
        &layer,
        &background_dsc,
        &background_coords_rect
    );

    lv_draw_rect_dsc_t active_modifier_background_dsc;
    lv_draw_rect_dsc_init(&active_modifier_background_dsc);
    active_modifier_background_dsc.bg_color = FOREGROUND_COLOR;

    lv_draw_label_dsc_t active_modifier_text_dsc;
    lv_draw_label_dsc_init(&active_modifier_text_dsc);
    active_modifier_text_dsc.color = BACKGROUND_COLOR;
    active_modifier_text_dsc.font = &press_start_2p_8;
    active_modifier_text_dsc.align = LV_TEXT_ALIGN_LEFT;
    
    lv_draw_label_dsc_t inactive_modifier_text_dsc;
    lv_draw_label_dsc_init(&inactive_modifier_text_dsc);
    inactive_modifier_text_dsc.color = FOREGROUND_COLOR;
    inactive_modifier_text_dsc.font = &press_start_2p_8;
    inactive_modifier_text_dsc.align = LV_TEXT_ALIGN_LEFT;
    
    if (states.modifiers.is_gui_active) {
        lv_area_t active_modifier_coords_rect = {
            0, // 0 * (MODIFIER_WIDTH + MODIFIER_PADDING_X),
            0,
            0 + MODIFIER_WIDTH - 1,
            0 + MODIFIERS_CANVAS_HEIGHT - 1
        };
        
        lv_draw_rect(
            &layer,
            &active_modifier_background_dsc,
            &active_modifier_coords_rect
        );
    }
    
    lv_area_t win_modifier_coords_rect = {
        MODIFIER_PADDING_X,
        MODIFIER_PADDING_Y,
        MODIFIER_PADDING_X + MODIFIER_WIDTH - 1,
        MODIFIER_PADDING_Y + press_start_2p_8.line_height - 1
    };
    
    active_modifier_text_dsc.text = "W";
    inactive_modifier_text_dsc.text = "W";
    lv_draw_label(
        &layer,
        states.modifiers.is_gui_active ? &active_modifier_text_dsc : &inactive_modifier_text_dsc,
        &win_modifier_coords_rect
    );

    if (states.modifiers.is_alt_active) {
        lv_area_t active_modifier_coords_rect = {
            1 * (MODIFIER_WIDTH + MODIFIERS_GAP),
            0,
            1 * (MODIFIER_WIDTH + MODIFIERS_GAP) + MODIFIER_WIDTH - 1,
            0 + MODIFIERS_CANVAS_HEIGHT - 1
        };
        lv_draw_rect(
            &layer,
            &active_modifier_background_dsc,
            &active_modifier_coords_rect
        );
    }

    lv_area_t alt_modifier_coords_rect = {
        1 * (MODIFIER_WIDTH + MODIFIERS_GAP) + MODIFIER_PADDING_X,
        MODIFIER_PADDING_Y,
        1 * (MODIFIER_WIDTH + MODIFIERS_GAP) + MODIFIER_PADDING_X + MODIFIER_WIDTH - 1,
        MODIFIER_PADDING_Y + press_start_2p_8.line_height - 1
    };

    active_modifier_text_dsc.text = "A";
    inactive_modifier_text_dsc.text = "A";
    lv_draw_label(
        &layer,
        states.modifiers.is_alt_active ? &active_modifier_text_dsc : &inactive_modifier_text_dsc,
        &alt_modifier_coords_rect
    );

    if (states.modifiers.is_ctrl_active) {
        lv_area_t active_modifier_coords_rect = {
            2 * (MODIFIER_WIDTH + MODIFIERS_GAP),
            0,
            2 * (MODIFIER_WIDTH + MODIFIERS_GAP) + MODIFIER_WIDTH - 1,
            0 + MODIFIERS_CANVAS_HEIGHT - 1
        };
        lv_draw_rect(
            &layer,
            &active_modifier_background_dsc,
            &active_modifier_coords_rect
        );
    }

    lv_area_t ctrl_modifier_coords_rect = {
        2 * (MODIFIER_WIDTH + MODIFIERS_GAP) + MODIFIER_PADDING_X,
        MODIFIER_PADDING_Y,
        2 * (MODIFIER_WIDTH + MODIFIERS_GAP) + MODIFIER_PADDING_X + MODIFIER_WIDTH - 1,
        MODIFIER_PADDING_Y + press_start_2p_8.line_height - 1
    };

    active_modifier_text_dsc.text = "C";
    inactive_modifier_text_dsc.text = "C";
    lv_draw_label(
        &layer,
        states.modifiers.is_ctrl_active ? &active_modifier_text_dsc : &inactive_modifier_text_dsc,
        &ctrl_modifier_coords_rect
    );

    if (states.modifiers.is_shift_active) {
        lv_area_t active_modifier_coords_rect = {
            3 * (MODIFIER_WIDTH + MODIFIERS_GAP),
            0,
            3 * (MODIFIER_WIDTH + MODIFIERS_GAP) + MODIFIER_WIDTH - 1,
            0 + MODIFIERS_CANVAS_HEIGHT - 1
        };
        lv_draw_rect(
            &layer,
            &active_modifier_background_dsc,
            &active_modifier_coords_rect
        );
    }

    lv_area_t shift_modifier_coords_rect = {
        3 * (MODIFIER_WIDTH + MODIFIERS_GAP) + MODIFIER_PADDING_X,
        MODIFIER_PADDING_Y,
        3 * (MODIFIER_WIDTH + MODIFIERS_GAP) + MODIFIER_PADDING_X + MODIFIER_WIDTH - 1,
        MODIFIER_PADDING_Y + press_start_2p_8.line_height - 1
    };

    active_modifier_text_dsc.text = "S";
    inactive_modifier_text_dsc.text = "S";
    lv_draw_label(
        &layer,
        states.modifiers.is_shift_active ? &active_modifier_text_dsc : &inactive_modifier_text_dsc,
        &shift_modifier_coords_rect
    );

    lv_canvas_finish_layer(modifiers_canvas, &layer);
}