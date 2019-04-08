#include "Homenet.h"
#include "Const.h"

#define DEBUG true

Homenet::Homenet(char* mqtt_device_name) {
    _wifi = WiFiClient();
    _mqtt = PubSubClient(_wifi);
    _wifi_connecting = false;
    _mqtt_device_name = mqtt_device_name;
}

void Homenet::setup(void (*on_cmd)(Cmd cmd)) {
    _mqtt.setServer(mqtt_server, 1883);
    // _mqtt.setCallback(a);
}

void Homenet::loop() {
    _setup_wifi();
    if (WiFi.status() == WL_CONNECTED) {
        _setup_mqtt();
    }

    _mqtt.loop();
}

void Homenet::send(Device device) {
    DynamicJsonBuffer jsonBuffer;
    char jsonMessageBuffer[256];

    JsonObject& root = jsonBuffer.createObject();
    root["name"] = device.get_name();
    root["room"] = device.get_room();
    root["type"] = device.get_type();
    root["signal"] = device.get_wifi_strength();
    root["value"] = device.get_value();

    root["state"] = device.get_state();

    root.printTo(jsonMessageBuffer, sizeof(jsonMessageBuffer));
    _mqtt.publish(outTopic, jsonMessageBuffer);

#if DEBUG
    Serial.print("--> ");
    Serial.println(jsonMessageBuffer);
#endif
}

bool Homenet::_setup_wifi() {
    bool connected = WiFi.status() == WL_CONNECTED;
    bool error = WiFi.status() == WL_CONNECT_FAILED;

    if (connected || error) {
        _wifi_connecting = false;
    }

    if (connected) {
        return true;
    }

    if (!_wifi_connecting) {
        _wifi_connecting = true;
        WiFi.begin(ssid, ssid_password);
    }

    return false;
}

void Homenet::_setup_mqtt() {
    bool connected = _mqtt.connected();

    if (connected) {
        return;
    }

    if (_mqtt.connect(_mqtt_device_name)) {
        _mqtt.subscribe(inTopic);
    }
}

int Homenet::get_state() {
    if (_mqtt.connected()) {
        return STATE_CONNECTED;
    }

    if (_wifi_connecting) {
        return STATE_CONNECTING;
    }

    return STATE_UNKNOWN;
}

long Homenet::get_wifi_strength() {
    return WiFi.RSSI();
}

void a(char* topic, uint8_t* payload, unsigned int length) {
}

void Homenet::_on_new_message(char* topic, uint8_t* payload, unsigned int length) {
}