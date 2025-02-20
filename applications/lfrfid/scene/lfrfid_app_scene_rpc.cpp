#include "lfrfid_app_scene_rpc.h"
#include <core/common_defines.h>
#include <dolphin/dolphin.h>

static const NotificationSequence sequence_blink_start_magenta = {
    &message_blink_start_10,
    &message_blink_set_color_magenta,
    &message_do_not_reset,
    NULL,
};

static const NotificationSequence sequence_blink_stop = {
    &message_blink_stop,
    NULL,
};

void LfRfidAppSceneRpc::on_enter(LfRfidApp* app, bool /* need_restore */) {
    auto popup = app->view_controller.get<PopupVM>();

    popup->set_header("LF RFID", 89, 30, AlignCenter, AlignTop);
    popup->set_text("RPC mode", 89, 43, AlignCenter, AlignTop);
    popup->set_icon(0, 3, &I_RFIDDolphinSend_97x61);

    app->view_controller.switch_to<PopupVM>();

    notification_message(app->notification, &sequence_display_backlight_on);
}

bool LfRfidAppSceneRpc::on_event(LfRfidApp* app, LfRfidApp::Event* event) {
    UNUSED(app);
    UNUSED(event);
    bool consumed = false;

    if(event->type == LfRfidApp::EventType::Exit) {
        consumed = true;
        LfRfidApp::Event view_event;
        view_event.type = LfRfidApp::EventType::Back;
        app->view_controller.send_event(&view_event);
    } else if(event->type == LfRfidApp::EventType::EmulateStart) {
        auto popup = app->view_controller.get<PopupVM>();
        consumed = true;
        emulating = true;

        app->text_store.set("emulating\n%s", app->worker.key.get_name());
        popup->set_text(app->text_store.text, 89, 43, AlignCenter, AlignTop);

        notification_message(app->notification, &sequence_blink_start_magenta);
    }
    return consumed;
}

void LfRfidAppSceneRpc::on_exit(LfRfidApp* app) {
    if(emulating) {
        app->worker.stop_emulate();
        notification_message(app->notification, &sequence_blink_stop);
    }
    app->view_controller.get<PopupVM>()->clean();
}
