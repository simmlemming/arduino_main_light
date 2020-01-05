#define FASTLED_ESP8266_RAW_PIN_ORDER

#include <FastLED.h>
#include <Device.h>
#include "Const.h"
#include <Homenet.h>

#define LED_TYPE WS2812B
#define COLOR_ORDER GRB
#define NUM_LEDS 29
#define LED_PIN D8

class LedStrip {
  public:
    void setup() {
      FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(_leds, NUM_LEDS).setCorrection(TypicalLEDStrip).setDither(0);
      FastLED.setBrightness(40);
    }

    void display(Device device, int wifi_state) {
      if (device.get_state() == DEVICE_STATE_OK) {
        _display_on(device);
      } else if (device.get_state() == DEVICE_STATE_OFF) {
        _display_off();
      } else {
        _display_error(device);
      }

      _display_wifi_state(wifi_state);
      
      _invert_orientation(); // Physically last LED in the led strip is first
      FastLED.show();
    }

  private:
    CRGB _color_filled = CRGB(255, 60, 0);
    CRGB _color_empty = CRGB::Yellow;
    
    CRGB _leds[NUM_LEDS];

    void _invert_orientation() {
      CRGB tmp;
      int j = 0;
      for (int i = 0; i < NUM_LEDS / 2; i++) {
        j = NUM_LEDS - 1 - i;
        tmp = _leds[i];
        _leds[i] = _leds[j];
        _leds[j] = tmp;
      }
    }

    void _display_on(Device device) {
      float pct = (float) device.get_value() / LED_LEVEL_MAX;
      _fill(pct);
    }

    void _display_off() {
      for (int i = 0; i < NUM_LEDS; i++) {
          _leds[i] = CRGB::Black;
      }
    }

    void _fill(float pct) {
      int lastLed = (int) (NUM_LEDS * pct) + 1;

      if (pct == 0) {
        lastLed = 0;
      }

      for (int i = 0; i < NUM_LEDS; i++) {
        if (i < lastLed) {
          _leds[i] = _color_filled;
        } else {
          _leds[i] = _color_empty;
        }
      }
    }

    void _display_error(Device device) {
      _leds[NUM_LEDS - 1] = CRGB::Red;
      _leds[NUM_LEDS - 2] = CRGB::Red;
    }

    void _display_wifi_state(int state) {
      if (state == STATE_CONNECTING) {
        _leds[NUM_LEDS - 1] = CRGB::Blue;
      } else if (state == STATE_UNKNOWN) {
        _leds[NUM_LEDS - 1] = CRGB::Red;
      }
    }
};
