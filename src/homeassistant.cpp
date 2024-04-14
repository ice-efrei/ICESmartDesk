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

String home_assistant_entities[1][3] = {
        {
                "input_boolean.fan_neo_2", // entity_id
                "on", // state
                "off" // old state
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
    if (doc["type"].as<String>() == "result" && !doc["success"].as<boolean>()) {
        Serial.println("Failed to subscribe to entity");
        return;
    }

    if (doc["type"].as<String>() != "event") {
        Serial.println("Not a state change event");
        return;
    }

    const int entity_id = doc["id"].as<int>()-1;
    const String state = doc["event"]["variables"]["trigger"]["to_state"]["state"].as<String>();

    Serial.print("Entity ID: ");
    Serial.println(entity_id);
    Serial.print("State: ");
    Serial.println(state);

    home_assistant_entities[entity_id][2] = home_assistant_entities[entity_id][1];
    home_assistant_entities[entity_id][1] = state;
}

void setup_entities_subscriptions(websockets::WebsocketsClient* client){
    //{
    //    "id": 1,
    //    "type": "subscribe_trigger",
    //    "trigger": {
    //        "platform": "state",
    //        "entity_id": "input_boolean.fan_neo_2",
    //        "to": null
    //    }
    //}
    for(int i = 0; i < number_of_entities; i++){
        String subscribe_message = R"({"id": )" + String(i+1) + R"(, "type": "subscribe_trigger", "trigger": {"platform": "state", "entity_id": ")" + home_assistant_entities[i][0] + R"(", "to": null}})";
        Serial.println(subscribe_message);
        client->send(subscribe_message);
    }
}