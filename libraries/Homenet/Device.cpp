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

char* Device::get_name() {
    return _name;
}

char* Device::get_room() {
    return _room;
}

char* Device::get_type() {
    return _type;
}

long Device::get_wifi_strength() { return _wifi_strength; }

void Device::set_state(int state) {
    if (_state != state) {
        _state = state;
        _changed = true;
    }
}

void Device::set_value(int value) {
    if (_value != value) {
        _value = value;
        _changed = true;
    }
}

void Device::set_wifi_strength(long strength) {
    _changed = _changed || abs(_wifi_strength - strength) > 3;
    _wifi_strength = strength;
}