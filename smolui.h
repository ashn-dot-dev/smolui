/*
** Copyright (c) 2023 Mario Nachbaur
** Copyright (c) 2023 ashn <me@ashn.dev>
**
** This library is free software; you can redistribute it and/or modify it
** under the terms of the MIT license. See `microui.c` for details.
*/

#ifndef SMOLUI_H
#define SMOLUI_H

#include <raylib.h>
#include "microui.h"

#define SMOLUI_TEXT_SPACING 1

// Create a raylib Color from a microui mu_Color.
#define SMOLUI_COLOR_FROM_MU(c) ((Color){c.r, c.g, c.b, c.a})

// Create a raylib Font from a microui mu_Font.
// If `f` is `NULL` get the default raylib font.
#define SMOLUI_FONT_FROM_MU(f) ((f == NULL) ? GetFontDefault() : *(Font*)f)

// Create a raylib Rectangle from a microui mu_Rect.
#define SMOLUI_RECTANGLE_FROM_MU(r) ((Rectangle){r.x, r.y, r.w, r.h})

// Create a raylib Vector2 from a microui mu_Vec2.
#define SMOLUI_VECTOR2_FROM_MU(v) ((Vector2){v.x, v.y})

// Set the text height/width callbacks and the font.
// Providing a NULL font pointer wll use the default raylib font.
void smol_setup_font(mu_Context* ctx, Font const* font);

// `mu_Context.text_width` callback. See `smol_setup_font`.
int smol_text_width(mu_Font font, char const* str, int len);

// `mu_Context.text_height` callback. See `smol_setup_font`.
int smol_text_height(mu_Font font);

// Handle all keyboard & mouse events.
void smol_handle_input(mu_Context* ctx);

// Handle mouse cursor position update.
#define smol_handle_mouse_move(ctx)                                            \
    mu_input_mousemove(ctx, GetMouseX(), GetMouseY())

// Handle mouse wheel scroll.
void smol_handle_mouse_scroll(mu_Context* ctx);

// Handle right, left & middle clicks.
void smol_handle_mouse_buttons_input_ex(mu_Context* ctx, int x, int y);
#define smol_handle_mouse_buttons_input(ctx)                                   \
    smol_handle_mouse_buttons_input_ex(ctx, GetMouseX(), GetMouseY())

// Handle shift, control, alt, enter & backspace presses.
void smol_handle_keyboard_input(mu_Context* ctx);

// Handle text input.
void smol_handle_text_input(mu_Context* ctx);

// Draw controls, text & icons using raylib.
void smol_render(mu_Context* ctx);

#endif // SMOLUI_H
