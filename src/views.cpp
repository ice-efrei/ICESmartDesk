//
// Created by Killian Zhou on 14/04/2024.
//

#include "views.h"
#include "constants.h"
#include <TFT_eSPI.h>

void value_view(TFT_eSPI* tft, String top_string, String bottom_string, String value_string, String unit_string, float progress){
    tft->fillScreen(TFT_ICE_DARK_BLUE);
    tft->setTextColor(TFT_WHITE, TFT_ICE_DARK_BLUE);
    tft->loadFont(POPPINS_BOLD_20, LittleFS);

    int string_width = tft->textWidth(top_string) / 2;
    tft->setCursor(120 - string_width, 15);
    tft->println(top_string);

    int radius = 2;
    int w = 130;
    int h = 4;
    int x = 120 - w/2;
    int y = 38;

    tft->drawSmoothRoundRect(x, y, radius, 0, w, h, TFT_ICE_LIGHT_BLUE, TFT_ICE_DARK_BLUE);

    y = 195;
    tft->drawSmoothRoundRect(x, y, radius, 0, w, h, TFT_ICE_LIGHT_BLUE, TFT_ICE_DARK_BLUE);

    string_width = tft->textWidth(bottom_string) / 2;
    tft->setCursor(120-string_width, 205);
    tft->println(bottom_string);
    tft->unloadFont();


    tft->loadFont(POPPINS_REGULAR_20, LittleFS);
    string_width = tft->textWidth(unit_string);
    tft->unloadFont();

    tft->loadFont(POPPINS_BOLD_50, LittleFS);
    string_width += tft->textWidth(value_string);
    tft->setCursor(120-(string_width / 2), 95);
    tft->print(value_string);
    tft->unloadFont();
    tft->loadFont(POPPINS_REGULAR_20, LittleFS);
    tft->println(unit_string);
    tft->unloadFont();

    uint16_t fg_color = TFT_WHITE;
    uint16_t bg_color = TFT_ICE_DARK_BLUE;

    x = 120;
    y = 120;

    radius       = 117; // Outer arc radius
    uint8_t thickness    = 4;
    uint8_t inner_radius = radius - thickness;

    uint16_t start_angle = 180;
    uint16_t end_angle   = int(180 + 360.0 * progress) % 360;

    bool arc_end = true;

    tft->drawSmoothArc(x, y, radius, inner_radius, start_angle, end_angle, fg_color, bg_color, arc_end);
}