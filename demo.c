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
#include <raylib.h>
#include <stdbool.h>
#include <stdlib.h>

#include <microui.h>
#include <murl.h>

char buf[256] = {0};

int main(void) {
    InitWindow(800, 600, "demo");
    SetTargetFPS(60);

    mu_Context *ctx = malloc(sizeof(mu_Context));
    mu_init(ctx);
    murl_setup_font_ex(ctx, NULL);

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);

        murl_handle_input(ctx);
        mu_begin(ctx);

        if (mu_begin_window(ctx, "Hello", mu_rect(20, 20, 200, 150))) {
            mu_label(ctx, "Hello, raylib");

            if (mu_button(ctx, "The button")) {
                mu_open_popup(ctx, "popup");
            }

            if (mu_begin_popup(ctx, "popup")) {
                mu_label(ctx, "This is a popup");
                mu_end_popup(ctx);
            }

            mu_textbox(ctx, buf, sizeof(buf));

            mu_end_window(ctx);
        }

        mu_end(ctx);
        murl_render(ctx);
        EndDrawing();
    }

    free(ctx);
    CloseWindow();
    return 0;
}
