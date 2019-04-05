#include "Device.h"

Device::Device(char* name, char* room, char* type) {
    _name = name;
    _room = room;
    _type = type;
}

bool Device::loop() {
    bool c = _changed;
    _changed = false;
    return c;
}

int Device::get_state() { return _state; }

int Device::get_value() { return _value; }

long Device::get_wifi_strength() { return _wifi_strength; }

void Device::set_state(int state) {
    _state = state;
    _changed = true;
}

void Device::set_value(int value) {
    _value = value;
    _changed = true;
}

void Device::set_wifi_strength(long strength) {
    _wifi_strength = strength;
    _changed = true;
}