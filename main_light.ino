#include <Button.h>
#include <Homenet.h>
#include "Light.cpp"
#include "Const.h"
#include "LedStrip.cpp"
#include "Throttle.cpp"

#define ENCODER_A D6
#define ENCODER_B D7
#define BTN_POWER D4
#define LED D5
#define LED_POWER D3

Light light = Light();
Homenet net = Homenet(light.get_name());
LedStrip led_strip = LedStrip();
Throttle throttle = Throttle(300);

Button btn_power = Button(BTN_POWER, on_power_click);

//volatile bool allow_interrupts = true;

void setup() {
  Serial.begin(115200);
  led_strip.setup();
  net.setup(on_cmd);

  pinMode(ENCODER_A, INPUT);
  pinMode(ENCODER_B, INPUT);
  pinMode(LED, OUTPUT);
  pinMode(LED_POWER, OUTPUT);

  analogWriteFreq(1000);
  attachInterrupt(digitalPinToInterrupt(ENCODER_A), turn, RISING);

  digitalWrite(LED_POWER, LOW);
  light.set_value(0);
  light.set_state(DEVICE_STATE_OFF);

//  allow_interrupts = true;
}


void loop() {
  net.loop();
  btn_power.loop();

  light.set_wifi_state(net.get_state());
  light.set_wifi_strength(net.get_wifi_strength());

  bool state_changed = light.loop();
  bool state_settled = throttle.throttled(state_changed);

  if (state_changed) {
    apply_state();
  }

  if (state_settled) {
    net.send(light);
  }

//  allow_interrupts = true;
}

void apply_state() {
  led_strip.display(light, light.get_wifi_state());

  int pwm = map(light.get_value(), LED_LEVEL_MIN, LED_LEVEL_MAX, 0, 1023);
  pwm = (light.get_state() == DEVICE_STATE_OFF) ? 0 : pwm;

  analogWrite(LED, pwm);

  if (light.get_state() == DEVICE_STATE_OK) {
    digitalWrite(LED_POWER, HIGH);
  } else if (light.get_state() == DEVICE_STATE_OFF) {
    digitalWrite(LED_POWER, LOW);
  }
}

void turn() {
//  if (!allow_interrupts) {
//    return;
//  }

//  allow_interrupts = false;

  int a = digitalRead(ENCODER_A);
  int b = digitalRead(ENCODER_B);

  if (a == b) {
    light.down();
  } else {
    light.up();
  }
}

void on_cmd(Cmd cmd) {
  if (eq(cmd.cmd, CMD_STATE)) {
    light.set_state(light.get_state()); // just to set _changed flag to true
    return;
  }

  if (eq(cmd.cmd, CMD_VALUE)) {
    light.set_value(cmd.value);
    return;
  }

  if (eq(cmd.cmd, CMD_OFF)) {
    light.off();
    return;
  }

  if (eq(cmd.cmd, CMD_ON)) {
    light.on();
    return;
  }
}

void on_power_click() {
  light.toggle_on_off();
}

boolean eq(const char* a1, const char* a2) {
  return strcmp(a1, a2) == 0;
}
