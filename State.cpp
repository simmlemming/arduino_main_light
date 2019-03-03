#include <Adafruit_SSD1306.h>
#include "Const.h"

const int MODE_ON = 1;
const int MODE_OFF = 2;

class State {
  public:
    volatile int led_level = 5;
    int mode = MODE_ON;

    void up() {
      if (mode == MODE_ON) {
        led_level_up();
      }
    }

    void down() {
      if (mode == MODE_ON) {
        led_level_down();
      }
    }

    void led_level_up() {
      led_level += LED_LEVEL_STEP;
      led_level = constrain(led_level, LED_LEVEL_MIN, LED_LEVEL_MAX);
    }

    void led_level_down() {
      led_level -= LED_LEVEL_STEP;
      led_level = constrain(led_level, LED_LEVEL_MIN, LED_LEVEL_MAX);
    }

    void display(Adafruit_SSD1306 display) {
      if (mode == MODE_ON) {
        _display_on(display);
      } else if (mode == MODE_OFF) {
        _display_off(display);
      } else {
        _display_error(display);
      }
    }

    void toggle_on_off() {
      if (mode == MODE_ON) {
        _last_on_level = led_level;
        led_level = 0;
        mode = MODE_OFF;
      } else if (mode == MODE_OFF) {
        led_level = _last_on_level;
        mode = MODE_ON;
      }
    }

  private:
    int _last_on_level = 0;

    void _display_on(Adafruit_SSD1306 display) {
      float pct = (float) led_level / LED_LEVEL_MAX;
      _draw_bar(display, pct);

      display.setCursor(32, 22);
      display.setTextSize(6);
      
      if (led_level < 10) {
        display.print(0);
      }
      display.print(led_level);
    }

    void _display_off(Adafruit_SSD1306 display) {
      _draw_bar(display, 0);
      
      display.setCursor(48, 22);
      display.setTextSize(6);
      display.print("-");
    }

    void _display_error(Adafruit_SSD1306 display) {
      display.setCursor(16, 16);
      display.setTextSize(6);
      display.print("ERR");
    }

    void _draw_bar(Adafruit_SSD1306 display, float pct) {
      display.drawRect(0, 0, 128, 16, WHITE);
      display.fillRect(0, 0, (int)(128 * pct), 16, WHITE);
    }
};
