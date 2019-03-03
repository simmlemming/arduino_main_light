#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Button.h>
#include "State.cpp"
#include "Const.h"

#define ENCODER_A D6
#define ENCODER_B D7
#define BTN_POWER D4 // 10
#define LED D5 // 3
#define PHOTO_RES A0

#define OLED_RESET LED_BUILTIN
Adafruit_SSD1306 display(OLED_RESET);

State state = State();
Button btn_power = Button(BTN_POWER, on_power_click);

void setup() {
  Serial.begin(9600);

  pinMode(ENCODER_A, INPUT_PULLUP);
  pinMode(ENCODER_B, INPUT_PULLUP);
  pinMode(LED, OUTPUT);
  pinMode(PHOTO_RES, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(ENCODER_A), turn, RISING);

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.setTextColor(WHITE);
}

void loop() {
  btn_power.loop();

  display.clearDisplay();
  state.display(display);
  display.display();

  int pwm = map(state.led_level, LED_LEVEL_MIN, LED_LEVEL_MAX, 0, 1023);
  update_led(pwm);
}

void turn() {
  int a = digitalRead(ENCODER_A);
  int b = digitalRead(ENCODER_B);

  if (a == b) {
    state.down();
  } else {
    state.up();
  }
}

long br_start_ms = 0;
const long BR_DELAY_MS = 1500;
const int BR_DELTA = 5;

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
  state.toggle_on_off();
}
