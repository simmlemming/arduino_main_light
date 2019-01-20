#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Button.h>

#define BTN_UP 12
#define BTN_ON 11
#define BTN_DOWN 10
#define LED 3
#define PHOTO_RES A0

#define BR_MIN 0
#define BR_MAX 9

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

int br = 5;
int photo_res = 0;

Button btn_up = Button(BTN_UP, on_up_click);
Button btn_down = Button(BTN_DOWN, on_down_click);

void setup() {
  Serial.begin(9600);
  pinMode(LED, OUTPUT);
  pinMode(PHOTO_RES, INPUT_PULLUP);

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();

  display.setTextColor(WHITE);
}

void loop() {
  photo_res = analogRead(PHOTO_RES) / 10;

  btn_up.loop();
  btn_down.loop();

  display.clearDisplay();

  display.setCursor(48, 16);
  display.setTextSize(6);
  display.print(br);

  display.setCursor(0, 0);
  display.setTextSize(1);
  display.print(photo_res);
  display.display();

  int pwm = map(br, 0, 9, 0, 255);
  Serial.print(br);
  Serial.print(" -> ");
  Serial.println(pwm);

  update_led(pwm);
}

void update_led(int value) {
  analogWrite(LED, value);
}

void on_up_click() {
  br++;
  br = constrain(br, BR_MIN, BR_MAX);
}

void on_down_click() {
  br--;
  br = constrain(br, BR_MIN, BR_MAX);
}
