// TODO
// (DONE) Fix performance when not being charged
// (DONE) Add UART command parsing to Esp32
// (DONE) Prepare application and icon on github
// (DONE) Write snapshots to SD card
// 5. Set a constant refresh rate to the Flipper's display
// 6. Emulate grayscale
// 7. Photo browser app

#include <furi.h>
#include <furi_hal.h>
#include <gui/gui.h>
#include <gui/icon_i.h>
#include <notification/notification.h>
#include <notification/notification_messages.h>
#include <gui/elements.h>
#include <gui/view_dispatcher.h>
#include <gui/modules/dialog_ex.h>
#include <storage/filesystem_api_defines.h>
#include <storage/storage.h>

typedef struct DemoModel DemoModel;

typedef struct {
    Gui* gui;
    ViewDispatcher* view_dispatcher;
    View* view;
    FuriHalSerialHandle* serial_handle;
    FuriThread* worker_thread;
    FuriStreamBuffer* rx_stream;
} DemoApp;

struct DemoModel {
    int num;
};

const NotificationSequence sequence_notification = {
    &message_display_backlight_on,
    &message_delay_10,
    NULL,
};