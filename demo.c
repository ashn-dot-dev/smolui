/*
** Copyright (c) 2023 Mario Nachbaur
** Copyright (c) 2023 ashn <me@ashn.dev>
**
** Permission is hereby granted, free of charge, to any person obtaining a copy
** of this software and associated documentation files (the "Software"), to
** deal in the Software without restriction, including without limitation the
** rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
** sell copies of the Software, and to permit persons to whom the Software is
** furnished to do so, subject to the following conditions:
**
** The above copyright notice and this permission notice shall be included in
** all copies or substantial portions of the Software.
**
** THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
** IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
** FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
** AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
** LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
** FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
** IN THE SOFTWARE.
*/

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#include <raylib.h>
#include <smolui.h>

int checked = 0;
float slider = 0.0;
float number = 0.0;
char buf_a[256] = {0};
char buf_b[256] = {0};
int incremented = 0;

int incrementer(mu_Context *ctx, int *value) {
    mu_Id     id = mu_get_id(ctx, &value, sizeof(value));
    mu_Rect rect = mu_layout_next(ctx);
    mu_update_control(ctx, id, rect, 0);

    /* handle input */
    int res = 0;
    if (ctx->mouse_pressed == MU_MOUSE_LEFT && ctx->focus == id) {
        (*value)++;
        res |= MU_RES_CHANGE;
    }

    /* draw */
    char buf[32];
    sprintf(buf, "%d", *value);
    mu_draw_control_frame(ctx, id, rect, MU_COLOR_BUTTON, 0);
    mu_draw_control_text(ctx, buf, rect, MU_COLOR_TEXT, MU_OPT_ALIGNCENTER);

    return res;
}

int main(void) {
    InitWindow(800, 600, "demo");
    SetTargetFPS(60);

    mu_Context* ctx = smolui_mu_context_new();
    mu_init(ctx);
    smolui_setup_font(ctx, NULL);

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);

        smolui_handle_input(ctx);
        mu_begin(ctx);

        if (mu_begin_window_ex(ctx, "Hello", mu_rect(20, 20, 800 - 2 * 20, 600 - 2 * 20), MU_OPT_NOCLOSE)) {
            mu_text(ctx, "some text");
            mu_label(ctx, "label");
            if (mu_button(ctx, "button")) {
                puts("button pressed");
            }
            if (mu_button_ex(ctx, "button ex", MU_ICON_MAX, MU_OPT_ALIGNCENTER)) {
                puts("button (ex) pressed");
            }
            if (mu_checkbox(ctx, "checkbox", &checked)) {
                puts("checkbox interaction");
            }
            if (mu_textbox(ctx, buf_a, sizeof(buf_a))) {
                puts("textbox interaction");
            }
            int textbox_ex_res = mu_textbox_ex(ctx, buf_b, sizeof(buf_b), MU_OPT_ALIGNCENTER);
            if (textbox_ex_res) {
                puts("textbox interaction");
                if (textbox_ex_res & MU_RES_SUBMIT) {
                    puts("\ttextbox submit");
                }
                if (textbox_ex_res & MU_RES_CHANGE) {
                    puts("\ttextbox change");
                }
            }
            if (mu_slider(ctx, &slider, 0.0, 100.0)) {
                puts("slider interaction");
            }
            if (mu_number(ctx, &number, 5.0)) {
                puts("number interaction");
            }
            if (mu_header(ctx, "header")) {
                mu_text(ctx, "text under header");
            }
            if (mu_header_ex(ctx, "header ex", MU_OPT_EXPANDED)) {
                mu_text(ctx, "text under header (ex)");
            }
            if (mu_begin_treenode(ctx, "treenode")) {
                mu_text(ctx, "text under treenode");
                mu_end_treenode(ctx);
            }
            if (mu_begin_treenode_ex(ctx, "treenode ex", MU_OPT_EXPANDED)) {
                mu_text(ctx, "text under treenode (ex)");
                mu_end_treenode(ctx);
            }
            if (mu_button(ctx, "open popup")) {
                mu_open_popup(ctx, "popup");
            }
            if (mu_begin_popup(ctx, "popup")) {
                mu_text(ctx, "text inside popup");
                mu_end_popup(ctx);
            }
            mu_begin_panel(ctx, "panel");
            mu_text(ctx, "text inside panel");
            mu_end_panel(ctx);

            // custom control
            if (incrementer(ctx, &incremented)) {
                puts("custom incrementer interaction");
            }

            mu_end_window(ctx);
        }

        mu_end(ctx);
        smolui_render(ctx);
        EndDrawing();
    }

    smolui_mu_context_del(ctx);
    CloseWindow();
    return 0;
}
