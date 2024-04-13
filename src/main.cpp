#include <Arduino.h>

#define POPPINS_BOLD_20 "PoppinsBold20"
#define POPPINS_BOLD_50 "PoppinsBold50"
#define POPPINS_REGULAR_20 "PoppinsRegular20"

// Font files are stored in Flash FS
#include <FS.h>
#include <LittleFS.h>
#define FlashFS LittleFS

#include <SPI.h>
#include <TFT_eSPI.h>       // Hardware-specific library

#include "constants.h"

TFT_eSPI tft = TFT_eSPI();


void setup(void) {

    Serial.begin(250000);

    tft.begin();

    tft.setRotation(0);

    if (!LittleFS.begin()) {
        Serial.println("Flash FS initialisation failed!");
        while (1) yield(); // Stay here twiddling thumbs waiting
    }
    Serial.println("\nFlash FS available!");

    bool font_missing = false;
    if (!LittleFS.exists("/PoppinsBold20.vlw")) font_missing = true;
    if (!LittleFS.exists("/PoppinsBold50.vlw")) font_missing = true;
    if (!LittleFS.exists("/PoppinsRegular20.vlw")) font_missing = true;

    if (font_missing)
    {
        Serial.println("\nFont missing in Flash FS, did you upload it?");
        while(1) yield();
    }
    else Serial.println("\nFonts found OK.");

    tft.fillScreen(TFT_ICE_DARK_BLUE);
    tft.setTextColor(TFT_WHITE, TFT_ICE_DARK_BLUE);
    tft.loadFont(POPPINS_BOLD_20, LittleFS);
    tft.setCursor(87, 15);
    tft.println("Panda");
    tft.setCursor(87, 205);
    tft.println("Panda");
    tft.unloadFont();
}


void loop() {

}