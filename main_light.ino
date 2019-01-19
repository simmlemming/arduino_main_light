#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Button.h>

#define BTN_UP 12
#define BTN_ON 11
#define BTN_DOWN 10

#define BR_MIN 0
#define BR_MAX 9

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

int br = 0;

Button btn_up = Button(BTN_UP, on_up_click);
Button btn_down = Button(BTN_DOWN, on_down_click);

void setup() {
  Serial.begin(9600);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();

  display.setTextSize(6);
  display.setTextColor(WHITE);
}

void loop() {
  btn_up.loop();
  btn_down.loop();
  
  display.clearDisplay();
  display.setCursor(48, 16);
  display.println(br);
  display.display();
}

void on_up_click() {
  br++;
  br = constrain(br, BR_MIN, BR_MAX);
}

void on_down_click() {
  br--;
  br = constrain(br, BR_MIN, BR_MAX);
}
