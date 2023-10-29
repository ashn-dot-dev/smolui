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

mu_Context* smolui_mu_context_new(void);
void smolui_mu_context_del(mu_Context* ctx);

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
void smolui_setup_font_ex(mu_Context* ctx, Font const* font);
#define smolui_setup_font(ctx) smolui_setup_font_ex(ctx, NULL)

// `mu_Context.text_width` callback. See `smolui_setup_font`.
int smolui_text_width(mu_Font font, char const* str, int len);

// `mu_Context.text_height` callback. See `smolui_setup_font`.
int smolui_text_height(mu_Font font);

// Handle all keyboard & mouse events.
void smolui_handle_input(mu_Context* ctx);

// Handle mouse cursor position update.
#define smolui_handle_mouse_move(ctx)                                          \
    mu_input_mousemove(ctx, GetMouseX(), GetMouseY())

// Handle mouse wheel scroll.
void smolui_handle_mouse_scroll(mu_Context* ctx);

// Handle right, left & middle clicks.
void smolui_handle_mouse_buttons_input_ex(mu_Context* ctx, int x, int y);
#define smolui_handle_mouse_buttons_input(ctx)                                 \
    smolui_handle_mouse_buttons_input_ex(ctx, GetMouseX(), GetMouseY())

// Handle shift, control, alt, enter & backspace presses.
void smolui_handle_keyboard_input(mu_Context* ctx);

// Handle text input.
void smolui_handle_text_input(mu_Context* ctx);

// Draw controls, text & icons using raylib.
void smolui_render(mu_Context* ctx);

#endif // SMOLUI_H
