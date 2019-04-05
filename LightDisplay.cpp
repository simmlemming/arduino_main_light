//#include <SPI.h>
//#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Device.h>
#include "Const.h"

#define OLED_RESET LED_BUILTIN

class LightDisplay {
  public:
    void setup() {
      dd.begin(SSD1306_SWITCHCAPVCC, 0x3C);
      dd.clearDisplay();
      dd.setTextColor(WHITE);
    }

    void display(Device device) {
      dd.clearDisplay();

      if (device.get_state() == STATE_OK) {
        _display_on(device);
      } else if (device.get_state() == STATE_OFF) {
        _display_off();
      } else {
        _display_error();
      }

      dd.display();
    }

  private:
    Adafruit_SSD1306 dd = Adafruit_SSD1306(OLED_RESET);
    
    void _draw_bar(float pct) {
      dd.drawRect(0, 0, 128, 16, WHITE);
      dd.fillRect(0, 0, (int)(128 * pct), 16, WHITE);
    }

    void _display_on(Device device) {
      float pct = (float) device.get_value() / LED_LEVEL_MAX;
      _draw_bar(pct);

      dd.setCursor(32, 22);
      dd.setTextSize(6);

      if (device.get_value() < 10) {
        dd.print(0);
      }

      dd.print(device.get_value());
    }

    void _display_off() {
      _draw_bar(0);

      dd.setCursor(48, 22);
      dd.setTextSize(6);
      dd.print("-");
    }

    void _display_error() {
      dd.setCursor(16, 16);
      dd.setTextSize(6);
      dd.print("ERR");
    }
};
