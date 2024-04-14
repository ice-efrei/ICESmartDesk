#include <Arduino.h>

#include <SPI.h>
#include <TFT_eSPI.h>       // Hardware-specific library

#include "constants.h"
#include "views.h"

TFT_eSPI tft = TFT_eSPI();


void setup() {

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

    value_view(&tft, "Temp", "Neo 2", "41", "C", 0.75);
}


void loop() {

}