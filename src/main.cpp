#include <Arduino.h>

#include <SPI.h>
#include <TFT_eSPI.h>       // Hardware-specific library

#include "constants.h"
#include "views.h"

TFT_eSPI tft = TFT_eSPI();


void setup() {
    Serial.begin(115200);

    tft.begin();
    tft.setRotation(0);
    setup_fonts();
}


void loop() {
    value_view(&tft, "Temp", "Neo 2", "41", "C", 0.75);
    delay(5000);

    select_view(&tft, "ICE", "Desk", new String[3]{"Option 1", "Option 2", "Option 3"}, 3, 1);
    delay(5000);
}