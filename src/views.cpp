//
// Created by Killian Zhou on 14/04/2024.
//

#include <Arduino.h>
#include "views.h"
#include "constants.h"
#include <TFT_eSPI.h>

void top_bottom_text(TFT_eSPI* tft, String top_string, String bottom_string, String value_string){
    tft->setTextColor(TFT_WHITE, TFT_ICE_DARK_BLUE);
    tft->loadFont(POPPINS_BOLD_20, LittleFS);

    int string_width = tft->textWidth(top_string) / 2;
    tft->setCursor(screen_width / 2 - string_width, 15);
    tft->println(top_string);

    int radius = 2;
    int w = 130;
    int h = 4;
    int x = screen_width / 2 - w/2;
    int y = 38;

    tft->drawSmoothRoundRect(x, y, radius, 0, w, h, TFT_ICE_LIGHT_BLUE, TFT_ICE_DARK_BLUE);

    y = 195;
    tft->drawSmoothRoundRect(x, y, radius, 0, w, h, TFT_ICE_LIGHT_BLUE, TFT_ICE_DARK_BLUE);

    string_width = tft->textWidth(bottom_string) / 2;
    tft->setCursor(screen_width / 2 - string_width, 205);
    tft->println(bottom_string);
    tft->unloadFont();
}

void select_view(TFT_eSPI* tft, String top_string, String bottom_string, String* options, int option_size, int selected_option){
    tft->fillScreen(TFT_ICE_DARK_BLUE);
    top_bottom_text(tft, top_string, bottom_string, options[selected_option]);
    tft->loadFont(POPPINS_BOLD_20, LittleFS);

    String selected_option_string = options[selected_option];
    int selected_option_string_width = tft->textWidth(selected_option_string);

    int gap = 6, margin = 5, line_height = normal_font_height + (2 * margin);
    int top = (screen_height / 2) - 1.5 * line_height - gap;

    // Selected option square

    int radius = 7;
    int w = selected_option_string_width;
    int h = normal_font_height;
    int x = screen_width / 2 - w/2;
    int y = screen_height / 2 - h/2;

    tft->fillSmoothRoundRect(x - margin, y - margin, w + 2 * margin, h + 2 * margin, radius, TFT_ICE_LIGHT_BLUE, TFT_ICE_DARK_BLUE);

    // Draw options
    int c = 0;
    for (int i = selected_option-1; i <= selected_option+1; i++){
        if (i < 0 || i >= option_size) continue;
        int string_width = tft->textWidth(options[i]);
        int x = screen_width / 2 - string_width / 2;
        int y = top + margin + c * (line_height + gap);
        if (i == selected_option) tft->setTextColor(TFT_ICE_DARK_BLUE, TFT_ICE_LIGHT_BLUE);
        else tft->setTextColor(TFT_WHITE, TFT_ICE_DARK_BLUE);
        tft->setCursor(x, y);
        tft->println(options[i]);
        c += 1;
    }
}

void value_view(TFT_eSPI* tft, String top_string, String bottom_string, String value_string, String unit_string, float progress){
    tft->fillScreen(TFT_ICE_DARK_BLUE);
    top_bottom_text(tft, top_string, bottom_string, value_string);

    tft->setTextColor(TFT_WHITE, TFT_ICE_DARK_BLUE);

    tft->loadFont(POPPINS_REGULAR_20, LittleFS);
    int string_width = tft->textWidth(unit_string);
    tft->unloadFont();

    tft->loadFont(POPPINS_BOLD_50, LittleFS);
    string_width += tft->textWidth(value_string);
    tft->setCursor(screen_width / 2 - (string_width / 2), 95);
    tft->print(value_string);
    tft->unloadFont();
    tft->loadFont(POPPINS_REGULAR_20, LittleFS);
    tft->println(unit_string);
    tft->unloadFont();

    uint16_t fg_color = TFT_WHITE;
    uint16_t bg_color = TFT_ICE_DARK_BLUE;

    int x = screen_width / 2, y = screen_height / 2;

    int radius = 117;
    uint8_t thickness = 4;
    uint8_t inner_radius = radius - thickness;

    uint16_t start_angle = 180;
    uint16_t end_angle   = int(180 + 360.0 * progress) % 360;

    bool arc_end = true;

    tft->drawSmoothArc(x, y, radius, inner_radius, start_angle, end_angle, fg_color, bg_color, arc_end);
}