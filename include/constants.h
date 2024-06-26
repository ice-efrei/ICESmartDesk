//
// Created by Killian Zhou on 14/04/2024.
//

#ifndef ICESMARTDESK_CONSTANTS_H
#define ICESMARTDESK_CONSTANTS_H

#include <FS.h>
#include <LittleFS.h>
#define FlashFS LittleFS

#define TFT_ICE_DARK_BLUE 0x11A9
#define TFT_ICE_LIGHT_BLUE 0xCEFD
#define TFT_TEMP_BLUE 0x061F
#define TFT_TEMP_RED 0xF800
#define TFT_TEMP_ORANGE 0xFEA0

#define POPPINS_BOLD_20 "PoppinsBold20"
#define POPPINS_BOLD_50 "PoppinsBold50"
#define POPPINS_REGULAR_20 "PoppinsRegular20"

#define screen_width 240
#define screen_height 240

#define normal_font_height 20
#define big_font_height 50

//#define home_assistant_ip "192.168.0.108"
#define home_assistant_ip "192.168.1.76"
#define home_assistant_port 8123
#define home_assistant_path "/api/websocket"

#endif //ICESMARTDESK_CONSTANTS_H
