#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Button.h>
#include "State.cpp"

#define BTN_UP 12
#define BTN_DOWN 11
#define BTN_POWER 10
#define LED 3
#define PHOTO_RES A0

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

State state = State();

Button btn_up = Button(BTN_UP, on_up_click);
Button btn_down = Button(BTN_DOWN, on_down_click);
Button btn_power = Button(BTN_POWER, on_power_click);

void setup() {
  Serial.begin(9600);
  
  pinMode(LED, OUTPUT);
  pinMode(PHOTO_RES, INPUT_PULLUP);

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.setTextColor(WHITE);
}

void loop() {
  update_state();
  
  btn_up.loop();
  btn_down.loop();
  btn_power.loop();

  display.clearDisplay();
  state.display(display);
  display.display();

  int pwm = map(state.led_level, 0, 9, 0, 255);
  update_led(pwm);
}

void update_state() {
  state.current_br = analogRead(PHOTO_RES) / 10;  
}

void update_led(int value) {
  analogWrite(LED, value);
}

void on_up_click() {
  state.up();
}

void on_down_click() {
  state.down();
}

void on_power_click() {
  state.toggle_manual_auto();
}
