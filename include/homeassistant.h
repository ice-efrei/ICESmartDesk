//
// Created by Killian Zhou on 14/04/2024.
//

#ifndef ICESMARTDESK_HOMEASSISTANT_H
#define ICESMARTDESK_HOMEASSISTANT_H

#include <ArduinoWebsockets.h>
#include <ArduinoJson.h>

void setup_home_assistant(websockets::WebsocketsClient* client);
void auth_home_assistant(websockets::WebsocketsClient* client);
void update_entities_state(const JsonDocument& doc);
void setup_entities_subscriptions(websockets::WebsocketsClient* client);

extern String home_assistant_entities[1][3];

#endif //ICESMARTDESK_HOMEASSISTANT_H
