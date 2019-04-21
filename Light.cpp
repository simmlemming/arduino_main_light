#include <Adafruit_SSD1306.h>
#include "Const.h"
#include <Device.h>

class Light : public Device {

  public:
    Light(int led_pin, int led_power_pin) : Device("main_light", "living_room", "light") {
      _led_pin = led_pin;
      _led_power_pin = led_power_pin;
    }

    void up() {
      if (get_state() == DEVICE_STATE_OK) {
        value_up();
      }
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
      Device::set_state(DEVICE_STATE_OK);
      Device::set_value(_value); // calling super method

      if (_value == 0) {
        digitalWrite(_led_power_pin, LOW);
      } else {
        digitalWrite(_led_power_pin, HIGH);
      }

      int pwm = map(_value, LED_LEVEL_MIN, LED_LEVEL_MAX, 0, 1023);
      pwm = 1023 - pwm; // invert for two-transistors-scheme
      analogWrite(_led_pin, pwm);
    }

    void toggle_on_off() {
      if (get_state() == DEVICE_STATE_OK) {
        off();
      } else if (get_state() == DEVICE_STATE_OFF) {
        on();
      }
    }

    void on() {
      set_value(_last_on_level);
      set_state(DEVICE_STATE_OK);
    }


    void off() {
      if (get_state() == DEVICE_STATE_OK) {
        _last_on_level = get_value();
      }

      set_value(0);
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
    int _last_on_level = 0;
    int _led_pin, _led_power_pin;
    int _wifi_state;
};
