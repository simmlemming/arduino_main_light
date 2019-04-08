#ifndef HOMENET
#define HOMENET 1

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
// #include <ArduinoJson.h>
#include "Cmd.h"
#include "Device.h"

#define STATE_CONNECTING 1
#define STATE_CONNECTED 2
#define STATE_UNKNOWN 3

class Homenet {
   public:
    Homenet(char* mqtt_device_name);

    void setup(void (*on_cmd)(Cmd cmd));
    void loop();

    void send(Device s);
    int get_state();
    long get_wifi_strength();

   private:
    WiFiClient _wifi;
    PubSubClient _mqtt;
    char* _mqtt_device_name;

    bool _wifi_connecting;

    bool _setup_wifi();
    void _setup_mqtt();

    void _on_new_message(char* topic, uint8_t* payload, unsigned int length);
};

#endif