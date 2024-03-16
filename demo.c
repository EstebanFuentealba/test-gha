#include "demo.h"
#include <stdio.h> // Para sprintf
#include <string.h> // Para strlen

static void app_draw_callback(Canvas* canvas, void* _model) {
    DemoModel* model = _model;
    UNUSED(model);
    canvas_set_color(canvas, ColorBlack);
    canvas_set_font(canvas, FontSecondary);
    canvas_draw_str(canvas, 37, 35, "Hola Mundo");
}
static uint32_t app_exit(void* context) {
    UNUSED(context);
    return VIEW_NONE;
}
static void app_free(DemoApp* app) {
    furi_assert(app);
    // Free views
    view_dispatcher_remove_view(app->view_dispatcher, 0);

    view_free(app->view);
    view_dispatcher_free(app->view_dispatcher);

    // Close gui record
    furi_record_close(RECORD_GUI);
    furi_record_close(RECORD_NOTIFICATION);
    app->gui = NULL;

    // furi_stream_buffer_free(app->rx_stream);

    // Free rest
    free(app);
}
static DemoApp* app_alloc() {
    DemoApp* app = malloc(sizeof(DemoApp));
    // Gui
    app->gui = furi_record_open(RECORD_GUI);

    // View dispatcher
    app->view_dispatcher = view_dispatcher_alloc();
    view_dispatcher_enable_queue(app->view_dispatcher);
    view_dispatcher_attach_to_gui(app->view_dispatcher, app->gui, ViewDispatcherTypeFullscreen);

    // Views
    app->view = view_alloc();
    view_set_context(app->view, app);
    view_set_draw_callback(app->view, app_draw_callback);
    view_allocate_model(app->view, ViewModelTypeLocking, sizeof(DemoModel));

    with_view_model(app->view, DemoModel * model, { model->num = 1; }, true);

    view_set_previous_callback(app->view, app_exit);
    view_dispatcher_add_view(app->view_dispatcher, 0, app->view);
    view_dispatcher_switch_to_view(app->view_dispatcher, 0);

    furi_hal_power_disable_otg();

    furi_delay_ms(1);
    return app;
}

int32_t test_app(void* p) {
    UNUSED(p);

    DemoApp* app = app_alloc();
    view_dispatcher_run(app->view_dispatcher);
    app_free(app);

    furi_hal_power_disable_otg();

    return 0;
}