//
// Created by Killian Zhou on 14/04/2024.
//

#ifndef ICESMARTDESK_VIEWS_H
#define ICESMARTDESK_VIEWS_H

#include <TFT_eSPI.h>

void value_view(TFT_eSPI* tft, String top_string, String bottom_string, String value_string, String unit_string, float progress);

#endif //ICESMARTDESK_VIEWS_H