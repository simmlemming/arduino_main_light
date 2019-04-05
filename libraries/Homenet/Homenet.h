#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include "Device.h"
#include "Cmd.h"

class Homenet
{
  public:
    void setup(void (*on_cmd)(Cmd cmd));
    void loop();

    void send(Device s);

  private:
    char *_device_name = "main_light";
    char *_room_name = "living_room";
    char *_device_type = "light";

    void _setup_wifi();
    void _setup_mqtt();
};
