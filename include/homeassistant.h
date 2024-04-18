//
// Created by Killian Zhou on 14/04/2024.
//

#ifndef ICESMARTDESK_HOMEASSISTANT_H
#define ICESMARTDESK_HOMEASSISTANT_H

#include <ArduinoWebsockets.h>
#include <ArduinoJson.h>

String* get_entities_names();
void setup_home_assistant(websockets::WebsocketsClient* client);
void auth_home_assistant(websockets::WebsocketsClient* client);
void update_entities_state(const JsonDocument& doc);
void setup_entities_subscriptions(websockets::WebsocketsClient* client);

extern const int number_of_entities;
extern String home_assistant_entities[3][4];

#endif //ICESMARTDESK_HOMEASSISTANT_H
