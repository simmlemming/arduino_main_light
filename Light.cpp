#include <Adafruit_SSD1306.h>
#include "Const.h"
#include <Device.h>

class Light : public Device {

  public:
    Light() : Device("main_light", "living_room", "light") { }

    void up() {
      value_up();
    }

    void down() {
      if (get_state() == DEVICE_STATE_OK) {
        value_down();
      }
    }

    void value_up() {
      set_value(get_value() + LED_LEVEL_STEP);
    }

    void value_down() {
      set_value(get_value() - LED_LEVEL_STEP);
    }

    void set_value(int value) {
      int _value = constrain(value, LED_LEVEL_MIN, LED_LEVEL_MAX);

      Device::set_value(_value); // calling super method
      set_state(_value == 0 ? DEVICE_STATE_OFF : DEVICE_STATE_OK);
    }

    void toggle_on_off() {
      if (get_state() == DEVICE_STATE_OK) {
        off();
      } else if (get_state() == DEVICE_STATE_OFF) {
        on();
      }
    }

    void on() {
      set_value(get_value());
//      set_state(DEVICE_STATE_OK);
    }

    void off() {
      set_state(DEVICE_STATE_OFF);
    }

    void set_wifi_state(int state) {
      if (_wifi_state != state) {
        _wifi_state = state;
        _changed = true;
      }
    }

    int get_wifi_state() {
      return _wifi_state;
    }

  private:
    int _wifi_state;
};
