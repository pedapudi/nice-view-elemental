#include "../../include/central/initialize_listeners.h"

#include <limits.h>
#include <lvgl.h>
#include <stdint.h>
#include <zephyr/sys/util.h>
#include <zmk/battery.h>
#include <zmk/ble.h>
#include <zmk/display.h>
#include <zmk/endpoints.h>
#include <zmk/event_manager.h>
#include <zmk/events/activity_state_changed.h>
#include <zmk/events/battery_state_changed.h>
#include <zmk/events/ble_active_profile_changed.h>
#include <zmk/events/endpoint_changed.h>
#include <zmk/events/keycode_state_changed.h>
#include <zmk/events/layer_state_changed.h>
#include <zmk/events/usb_conn_state_changed.h>
#include <zmk/keymap.h>
#include <zmk/usb.h>
#include "../../include/main.h"
#include "../../include/central/render.h"

struct states states;

static void battery_state_update_callback(struct battery_state state) {
    states.battery = state;

    render_battery();
}

static struct battery_state get_battery_state(const zmk_event_t* event) {
    // `as_zmk_battery_state_changed(event)->state_of_charge` seems to be
    // usually used but the same value can be retrieved from the following.
    const uint8_t level = zmk_battery_state_of_charge();
    const bool is_charging = zmk_usb_is_powered();

    struct battery_state state = {
        .level = level,
        .is_charging = is_charging,
    };

    return state;
}

// Create a listener named `widget_battery_state_update`. This name is then
// used to create a subscription.
ZMK_DISPLAY_WIDGET_LISTENER(
    widget_battery_state_update,
    struct battery_state,
    // Called after `get_battery_state` with the value it returned.
    battery_state_update_callback,
    get_battery_state
)

// Subscribe the `widget_battery_state_update` listener to the
// `zmk_battery_state_changed` event dispatched by ZMK.
ZMK_SUBSCRIPTION(
    widget_battery_state_update,
    // Triggered when the computed battery percentage has changed.
    zmk_battery_state_changed
);

// Subscribe the `widget_battery_state_update` listener to the
// `zmk_usb_conn_state_changed` event dispatched by ZMK.
ZMK_SUBSCRIPTION(
    widget_battery_state_update,
    // Triggered when connected or disconnected from USB.
    zmk_usb_conn_state_changed
);

static void connectivity_state_update_callback(struct connectivity_state state) {
    states.connectivity = state;

    render_connectivity();
}

static struct connectivity_state get_connectivity_state(const zmk_event_t* event) {
    const struct zmk_endpoint_instance selected_endpoint = zmk_endpoints_selected();
    const int active_profile_index = zmk_ble_active_profile_index();
    const bool active_profile_connected = zmk_ble_active_profile_is_connected();
    const bool active_profile_bonded = !zmk_ble_active_profile_is_open();

    struct connectivity_state state = {
        .selected_endpoint = selected_endpoint,
        .active_profile_index = active_profile_index,
        .active_profile_connected = active_profile_connected,
        .active_profile_bonded = active_profile_bonded,
    };

    return state;
}

// Create a listener named `widget_connectivity_state_update`. This
// name is then used to create a subscription.
ZMK_DISPLAY_WIDGET_LISTENER(
    widget_connectivity_state_update,
    struct connectivity_state,
    // Called after `get_connectivity_state` with the value it returned.
    connectivity_state_update_callback,
    get_connectivity_state
)

// Subscribe the `widget_connectivity_state_update` listener to the
// `zmk_endpoint_changed` event dispatched by ZMK.
ZMK_SUBSCRIPTION(
    widget_connectivity_state_update,
    zmk_endpoint_changed
);

// Subscribe the `widget_connectivity_state_update` listener to the
// `zmk_usb_conn_state_changed` event dispatched by ZMK.
ZMK_SUBSCRIPTION(
    widget_connectivity_state_update,
    // Triggered when connected or disconnected from USB.
    zmk_usb_conn_state_changed
);

// Subscribe the `widget_connectivity_state_update` listener to the
// `zmk_ble_active_profile_changed` event dispatched by ZMK.
ZMK_SUBSCRIPTION(
    widget_connectivity_state_update,
    // Triggered when the selected profile has changed.
    zmk_ble_active_profile_changed
);

static void layer_state_update_callback(struct layer_state state) {
    states.layer = state;

    render_main();
}

// Retrieve the data we want from the event
static struct layer_state get_layer_state(const zmk_event_t* event) {
    const zmk_keymap_layer_index_t index = zmk_keymap_highest_layer_active();
    const char* name = zmk_keymap_layer_name(zmk_keymap_layer_index_to_id(index));

    struct layer_state state = {
        .index = index,
        .name = name
    };

    return state;
}

// Create a listener named `widget_layer_state_update`. This name is then used
// to create a subscription.
ZMK_DISPLAY_WIDGET_LISTENER(
    widget_layer_state_update,
    struct layer_state,
    // Called after `get_layer_state` with the value it returned.
    layer_state_update_callback,
    get_layer_state
)

// Subscribe the `widget_layer_state_update` listener to the
// `zmk_battery_state_changed` event dispatched by ZMK.
ZMK_SUBSCRIPTION(
    widget_layer_state_update,
    // Triggered when the selected layer has changed.
    zmk_layer_state_changed
);

static void modifiers_state_update_callback(struct modifiers_state state) {
    states.modifiers = state;

    render_modifiers();
}

// Retrieve the data we want from the event
static struct modifiers_state get_modifiers_state(const zmk_event_t* event) {
    const zmk_mod_flags_t modifiers = zmk_hid_get_explicit_mods();
    const bool is_shift_active = modifiers & (MOD_LSFT | MOD_RSFT);
    const bool is_ctrl_active = modifiers & (MOD_LCTL | MOD_RCTL);
    const bool is_alt_active = modifiers & (MOD_LALT | MOD_RALT);
    const bool is_gui_active = modifiers & (MOD_LGUI | MOD_RGUI);

    struct modifiers_state state = {
        .is_shift_active = is_shift_active,
        .is_ctrl_active = is_ctrl_active,
        .is_alt_active = is_alt_active,
        .is_gui_active = is_gui_active,
    };

    return state;
}

// Create a listener named `widget_modifiers_state_update`. This name is then used
// to create a subscription.
ZMK_DISPLAY_WIDGET_LISTENER(
    widget_modifiers_state_update,
    struct modifiers_state,
    // Called after `get_modifiers_state` with the value it returned.
    modifiers_state_update_callback,
    get_modifiers_state
)

// Subscribe the `widget_modifiers_state_update` listener to the
// `zmk_keycode_state_changed` event dispatched by ZMK.
ZMK_SUBSCRIPTION(
    widget_modifiers_state_update,
    zmk_keycode_state_changed
);

// 1. Insert images here.
LV_IMG_DECLARE(grid);
LV_IMG_DECLARE(grid_inverse);
const lv_image_dsc_t* images[] = {
    &grid,
    // &grid_inverse
};

static const unsigned int frame_count = sizeof(images) / sizeof(images[0]);

void initialize_animation() {
    lv_animimg_set_src(image_canvas, (const void**)images, frame_count);
    // 2. Set the time for the whole animation.
    lv_animimg_set_duration(image_canvas, 200);
}

void start_animation() {
    lv_animimg_set_repeat_count(image_canvas, LV_ANIM_REPEAT_INFINITE);
    lv_animimg_start(image_canvas);
}

void stop_animation() {
    lv_animimg_set_repeat_count(image_canvas, 1);
    lv_animimg_start(image_canvas);
}

void initialize_listeners() {
    widget_layer_state_update_init();
    widget_connectivity_state_update_init();
    widget_battery_state_update_init();
    widget_modifiers_state_update_init();

    initialize_animation();
    start_animation();
}
