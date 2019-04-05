#include <Button.h>
#include <Homenet.h>
#include "Light.cpp"
#include "Const.h"
#include "LightDisplay.cpp"

#define ENCODER_A D6
#define ENCODER_B D7
#define BTN_POWER D4 // 10
#define LED D5 // 3

Homenet net = Homenet();
LightDisplay display = LightDisplay();
Light light = Light(LED);

Button btn_power = Button(BTN_POWER, on_power_click);

volatile bool allow_interrupts = true;

void setup() {
  Serial.begin(115200);
  display.setup();
  net.setup(on_cmd);

  pinMode(ENCODER_A, INPUT_PULLUP);
  pinMode(ENCODER_B, INPUT_PULLUP);
  pinMode(LED, OUTPUT);

  analogWriteFreq(1000);
  attachInterrupt(digitalPinToInterrupt(ENCODER_A), turn, RISING);

  light.set_state(STATE_OK);
  light.set_value(1);

  allow_interrupts = true;
}

void loop() {
  net.loop();
  btn_power.loop();
  
  if (light.loop()) {
    net.send(light);
    display.display(light);
//    Serial.println(light.get_value());
  }

  allow_interrupts = true;
}

void turn() {
  if (!allow_interrupts) {
    return;
  }

  allow_interrupts = false;

  int a = digitalRead(ENCODER_A);
  int b = digitalRead(ENCODER_B);

  if (a == b) {
    light.down();
  } else {
    light.up();
  }
}

void on_cmd(Cmd cmd) {

}

void on_power_click() {
  light.toggle_on_off();
}
