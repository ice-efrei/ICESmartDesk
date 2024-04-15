//
// Created by Killian Zhou on 14/04/2024.
//

#include "homeassistant.h"
#include <ArduinoWebsockets.h>
#include <WiFi.h>
#include "ArduinoJson.h"
#include "secrets.h"
#include "constants.h"

const int number_of_entities = 1;

String home_assistant_entities[1][4] = {
        {
                "sensor.esphome_web_4dced0_kobra_2_neo_room_temperature", // entity_id
                "0", // state
                "0", // old state
                "" // unit
        },
};

void setup_home_assistant(websockets::WebsocketsClient* client){
    WiFi.begin(ssid, password);

    // Wait some time to connect to wifi
    for(int i = 0; i < 10 && WiFi.status() != WL_CONNECTED; i++) {
        Serial.print(".");
        delay(1000);
    }

    // Check if connected to wifi
    if(WiFi.status() != WL_CONNECTED) {
        Serial.println("No Wifi!");
        return;
    }

    Serial.println("Connected to Wifi, Connecting to server.");
    // try to connect to Websockets server
    bool connected = client->connect(home_assistant_ip, home_assistant_port, home_assistant_path);
    if(!connected) {
        Serial.println("Not Connected!");
        return;
    }

    Serial.println("Connected to home assistant!");
    auth_home_assistant(client);

    // run callback when messages are received
    client->onMessage([&](websockets::WebsocketsMessage message){
        Serial.print("Got Message: ");
        Serial.println(message.data());

        JsonDocument doc;
        DeserializationError error = deserializeJson(doc, message.data());

        if(error) {
            Serial.print(F("deserializeJson() failed: "));
            Serial.println(error.f_str());
            return;
        }

        update_entities_state(doc);
    });

    setup_entities_subscriptions(client);
}

void auth_home_assistant(websockets::WebsocketsClient* client){
    String auth_message = "{\"type\": \"auth\", \"access_token\": \"" + String(long_term_token) + "\"}";
    client->send(auth_message);
}

void update_entities_state(const JsonDocument& doc){
    Serial.println("Updating State ------");
    if (doc["type"].as<String>() == "result") {
        if (!doc["success"].as<boolean>()) Serial.println("Failed to subscribe to entity");
        else Serial.println("Subscribed to entity");
        Serial.println("--------------------");
        return;
    }

    if (doc["type"].as<String>() != "event") {
        Serial.println("Not a state change event");
        Serial.println("--------------------");
        return;
    }

    const int entity_id = doc["id"].as<int>()-1;
    const String state = doc["event"]["variables"]["trigger"]["to_state"]["state"].as<String>();
    const String unit = doc["event"]["variables"]["trigger"]["to_state"]["attributes"]["unit_of_measurement"].as<String>();
    const String device_class = doc["event"]["variables"]["trigger"]["to_state"]["attributes"]["device_class"].as<String>();

    Serial.print("Entity ID: ");
    Serial.println(entity_id);
    Serial.print("State: ");
    Serial.println(state);
    Serial.print("Unit: ");
    Serial.println(unit);
    Serial.print("State Class: ");
    Serial.println(device_class);

    home_assistant_entities[entity_id][2] = home_assistant_entities[entity_id][1];
    home_assistant_entities[entity_id][1] = state;
    if (device_class == "temperature") home_assistant_entities[entity_id][3] = unit;
    Serial.println("--------------------");
}

void setup_entities_subscriptions(websockets::WebsocketsClient* client){
    for(int i = 0; i < number_of_entities; i++){
        String subscribe_message = R"({"id": )" + String(i+1) + R"(, "type": "subscribe_trigger", "trigger": {"platform": "state", "entity_id": ")" + home_assistant_entities[i][0] + R"(", "to": null}})";
        Serial.println(subscribe_message);
        client->send(subscribe_message);
    }
}