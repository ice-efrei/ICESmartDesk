#include <Arduino.h>
#include <ArduinoWebsockets.h>
#include <TFT_eSPI.h>
#include "views.h"
#include "homeassistant.h"

TFT_eSPI tft = TFT_eSPI();
websockets::WebsocketsClient client;

int counter = 0;

void setup() {
    Serial.begin(115200);

    tft.begin();
    tft.setRotation(2);
    setup_fonts();
    setup_home_assistant(&client);

    value_view(&tft, "Neo 2", "Fan", home_assistant_entities[0][1], "", 0);
}


void loop() {
    if (home_assistant_entities[0][1] != home_assistant_entities[0][2]) {
        Serial.println("Updating View ------");
        update_value_view(&tft, home_assistant_entities[0][1], home_assistant_entities[0][3], 0);
        home_assistant_entities[0][2] = home_assistant_entities[0][1];
        Serial.print("State: ");
        Serial.println(home_assistant_entities[0][1]);
        Serial.print("Old State: ");
        Serial.println(home_assistant_entities[0][2]);
        Serial.print("Unit: ");
        Serial.println(home_assistant_entities[0][3]);
        Serial.println("--------------------");
    }

    if(client.available()) {
        client.poll();
    }

    counter++;
    delay(500);
}