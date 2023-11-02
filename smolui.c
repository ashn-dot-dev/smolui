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

#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "smolui.h"

#define SMOL_TEXT_SPACING 1

// Create a raylib Color from a microui mu_Color.
#define SMOL_COLOR_FROM_MU(c) ((Color){c.r, c.g, c.b, c.a})

// Create a raylib Font from a microui mu_Font.
// If `f` is `NULL` get the default raylib font.
#define SMOL_FONT_FROM_MU(f) ((f == NULL) ? GetFontDefault() : *(Font*)f)

// Create a raylib Rectangle from a microui mu_Rect.
#define SMOL_RECTANGLE_FROM_MU(r) ((Rectangle){r.x, r.y, r.w, r.h})

// Create a raylib Vector2 from a microui mu_Vec2.
#define SMOL_VECTOR2_FROM_MU(v) ((Vector2){v.x, v.y})

// `mu_Context.text_width` callback. See `smol_setup_font`.
int smol_text_width(mu_Font font, char const* str, int len);

// `mu_Context.text_height` callback. See `smol_setup_font`.
int smol_text_height(mu_Font font);

// Handle right, left & middle clicks.
void smol_handle_mouse_buttons_input(mu_Context* ctx, int x, int y);

// Handle shift, control, alt, enter & backspace presses.
void smol_handle_keyboard_input(mu_Context* ctx);

// Handle text input.
void smol_handle_text_input(mu_Context* ctx);

void
smol_setup_font(mu_Context* ctx, Font const* font)
{
    ctx->style->font = (mu_Font)font;
    ctx->text_width = smol_text_width;
    ctx->text_height = smol_text_height;
    ctx->style->spacing = SMOL_TEXT_SPACING;
}

int
smol_text_width(mu_Font font, char const* str, int len)
{
    // TODO: There is no MeasureText function that takes a character slice, so
    // we are forced to allocate. See we can use a fixed buffer with a fallback
    // to strndup if len exceeds the size of the buffer.
    char* dup = strndup(str, (size_t)len);
    assert(dup != NULL);
    Font rlfont = SMOL_FONT_FROM_MU(font);
    Vector2 size = MeasureTextEx(rlfont, dup, rlfont.baseSize, SMOL_TEXT_SPACING);
    free(dup);
    return size.x;
}

int
smol_text_height(mu_Font font)
{
    Font rlfont = SMOL_FONT_FROM_MU(font);
    return rlfont.baseSize;
}

void
smol_handle_mouse_scroll(mu_Context* ctx)
{
    Vector2 mouse_wheel_scroll = GetMouseWheelMoveV();
    mu_input_scroll(ctx, (int)mouse_wheel_scroll.x * -30, (int)mouse_wheel_scroll.y * -30);
}

void
smol_handle_mouse_buttons_input(mu_Context* ctx, int x, int y)
{
    static struct mouse_button_map {
        MouseButton rl;
        int mu;
    } mouse_buttons[] = {
        {MOUSE_BUTTON_LEFT,   MU_MOUSE_LEFT},
        {MOUSE_BUTTON_RIGHT,  MU_MOUSE_RIGHT},
        {MOUSE_BUTTON_MIDDLE, MU_MOUSE_MIDDLE},
        {-1, -1},
    };
    for (size_t index = 0;; index++) {
        struct mouse_button_map button = mouse_buttons[index];
        if (button.rl == -1U) {
            break;
        }
        if (IsMouseButtonPressed(button.rl)) {
            mu_input_mousedown(ctx, x, y, button.mu);
        }
        else if (IsMouseButtonReleased(button.rl)) {
            mu_input_mouseup(ctx, x, y, button.mu);
        }
    }
}

void
smol_handle_keyboard_input(mu_Context* ctx)
{
    static struct key_map {
        KeyboardKey rl;
        int mu;
    } keyboard_keys[] = {
        {KEY_LEFT_SHIFT,    MU_KEY_SHIFT},
        {KEY_RIGHT_SHIFT,   MU_KEY_SHIFT},
        {KEY_LEFT_CONTROL,  MU_KEY_CTRL},
        {KEY_RIGHT_CONTROL, MU_KEY_CTRL},
        {KEY_LEFT_ALT,      MU_KEY_ALT},
        {KEY_RIGHT_ALT,     MU_KEY_ALT},
        {KEY_ENTER,         MU_KEY_RETURN},
        {KEY_KP_ENTER,      MU_KEY_RETURN},
        {KEY_BACKSPACE,     MU_KEY_BACKSPACE},
        {-1, -1},
    };
    for (size_t index = 0;; index++) {
        struct key_map key = keyboard_keys[index];
        if (key.rl == -1U) {
            break;
        }
        if (IsKeyPressed(key.rl)) {
            mu_input_keydown(ctx, key.mu);
        }
        else if (IsKeyReleased(key.rl)) {
            mu_input_keyup(ctx, key.mu);
        }
    }
}

void
smol_handle_text_input(mu_Context* ctx)
{
    char buffer[512];
    for (size_t index = 0; index < 512; index++) {
        char c = GetCharPressed();
        buffer[index] = c;
        if (c == '\0') {
            break;
        }
    }
    mu_input_text(ctx, buffer);
}

void
smol_handle_input(mu_Context* ctx)
{
    int mouse_position_x = GetMouseX();
    int mouse_position_y = GetMouseY();
    mu_input_mousemove(ctx, mouse_position_x, mouse_position_y);
    smol_handle_mouse_scroll(ctx);
    smol_handle_mouse_buttons_input(ctx, mouse_position_x, mouse_position_y);
    smol_handle_keyboard_input(ctx);
    smol_handle_text_input(ctx);
}

void
smol_render(mu_Context* ctx)
{
    BeginScissorMode(0, 0, GetScreenWidth(), GetScreenHeight());

    mu_Command* cmd = NULL;
    while (mu_next_command(ctx, &cmd)) {
        switch (cmd->type) {
        case MU_COMMAND_TEXT: {
            Font font = SMOL_FONT_FROM_MU(cmd->text.font);
            Vector2 text_position = SMOL_VECTOR2_FROM_MU(cmd->text.pos);
            int font_size = ctx->text_height(&font);
            Color text_color = SMOL_COLOR_FROM_MU(cmd->text.color);
            DrawTextEx(
                font,
                cmd->text.str,
                text_position,
                font_size,
                ctx->style->spacing,
                text_color);
        } break;

        case MU_COMMAND_RECT: {
            Rectangle rect = SMOL_RECTANGLE_FROM_MU(cmd->rect.rect);
            Color rect_color = SMOL_COLOR_FROM_MU(cmd->rect.color);
            DrawRectangleRec(rect, rect_color);
        } break;

        case MU_COMMAND_ICON: {
            Color icon_color = SMOL_COLOR_FROM_MU(cmd->icon.color);
            char* icon = "?";
            switch (cmd->icon.id) {
            case MU_ICON_CLOSE: {
                icon = "x";
            } break;
            case MU_ICON_CHECK: {
                icon = "*";
            } break;
            case MU_ICON_COLLAPSED: {
                icon = "+";
            } break;
            case MU_ICON_EXPANDED: {
                icon = "-";
            } break;
            default:
                assert(0 && "unreachable");
            }
            DrawText(
                icon,
                cmd->icon.rect.x,
                cmd->icon.rect.y,
                cmd->icon.rect.h,
                icon_color);
        } break;

        case MU_COMMAND_CLIP: {
            EndScissorMode();
            BeginScissorMode(
                cmd->clip.rect.x,
                cmd->clip.rect.y,
                cmd->clip.rect.w,
                cmd->clip.rect.h);
        } break;

        default:
            assert(0 && "unreachable");
            break;
        }
    }

    EndScissorMode();
}
