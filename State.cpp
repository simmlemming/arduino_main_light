#include <Adafruit_SSD1306.h>
#include "Const.h"

const int MODE_MANUAL = 1;
const int MODE_AUTO = 2;

class State {
  public:
    int led_level = 6;
    int current_br = 0;
    int target_br = 40;
    int mode = MODE_AUTO;

    void up() {
      if (mode == MODE_MANUAL) {
        led_level_up();
      } else if (mode == MODE_AUTO) {
        br_up();
      }
    }

    void down() {
      if (mode == MODE_MANUAL) {
        led_level_down();
      } else if (mode == MODE_AUTO) {
        br_down();
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

    void br_up() {
      target_br += BR_STEP;
      target_br = constrain(target_br, BR_MIN, BR_MAX);
    }

    void br_down() {
      target_br -= BR_STEP;
      target_br = constrain(target_br, BR_MIN, BR_MAX);
    }

    void display(Adafruit_SSD1306 display) {
      if (mode == MODE_MANUAL) {
        _display_manual(display);
      } else if (mode == MODE_AUTO) {
        _display_auto(display);
      } else {
        _display_error(display);
      }
    }

    void toggle_manual_auto() {
      if (mode == MODE_MANUAL) {
        mode = MODE_AUTO;
        target_br = current_br;
      } else if (mode == MODE_AUTO) {
        mode = MODE_MANUAL;
      }
    }

  private:
    void _display_manual(Adafruit_SSD1306 display) {
      float pct = (float) led_level / LED_LEVEL_MAX;
      display.drawRect(0, 0, 128, 16, WHITE);
      display.fillRect(0, 0, (int)(128 * pct), 16, WHITE);

      display.setCursor(48, 22);
      display.setTextSize(6);
      display.print(led_level);
    }

    void _display_auto(Adafruit_SSD1306 display) {
      _display_manual(display);

      display.setTextSize(3);

      display.setCursor(0, 32);
      display.print(current_br);

      display.setCursor(90, 32);
      display.print(target_br);
    }

    void _display_error(Adafruit_SSD1306 display) {
      display.setCursor(48, 16);
      display.setTextSize(6);
      display.print("ERR");
    }
};
