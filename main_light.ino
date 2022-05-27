/*
   ESP board definition v2.4.2
   Board: NodeMCU 1.0 (ESP-12E Module)
   CPU freq: 80MHz
*/

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

long last_off_ms = 0;

void on_power_click() {
  light.toggle_on_off();

  if (light.get_state() == DEVICE_STATE_OFF) {
    last_off_ms = millis();
  }
}

Button btn_power = Button(BTN_POWER, on_power_click);

void setup() {
  Serial.begin(115200);
  led_strip.setup();
//  net.setup(on_cmd);

  pinMode(ENCODER_A, INPUT);
  pinMode(ENCODER_B, INPUT);
  pinMode(LED, OUTPUT);
  pinMode(LED_POWER, OUTPUT);

  analogWriteFreq(1000);
  attachInterrupt(digitalPinToInterrupt(ENCODER_A), turn, RISING);

  digitalWrite(LED_POWER, LOW);
  light.set_value(0);
  light.set_state(DEVICE_STATE_OFF);
}


void loop() {
//  net.loop();
  btn_power.loop();

  light.set_wifi_state(2); // STATE_CONNECTED from Homenet
//  light.set_wifi_state(net.get_state());
//  light.set_wifi_strength(net.get_wifi_strength());

  bool state_changed = light.loop();
  bool state_settled = throttle.throttled(state_changed);

  if (state_changed) {
    apply_state();
  }

//  if (state_settled) {
//    net.send(light);
//  }

}

void apply_state() {
  led_strip.display(light, light.get_wifi_state());

  // 70% corresponds to max current for diodes,
  // so mapping to range 0..716 instead of 0..1023
  int pwm = map(light.get_value(), LED_LEVEL_MIN, LED_LEVEL_MAX, 0, 716);
  pwm = (light.get_state() == DEVICE_STATE_OFF) ? 0 : pwm;

  analogWrite(LED, pwm);

  if (light.get_state() == DEVICE_STATE_OK) {
    digitalWrite(LED_POWER, HIGH);
  } else if (light.get_state() == DEVICE_STATE_OFF) {
    digitalWrite(LED_POWER, LOW);
  }
}

// Declare like this if working with ESP board definitions ver > 2.4.2
// ICACHE_RAM_ATTR void turn() {
void turn() {
  // To prevent turning on by rotating the encoder while pressing it to turn off
  if (millis() - last_off_ms < 1000) {
    return;
  }

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

boolean eq(const char* a1, const char* a2) {
  return strcmp(a1, a2) == 0;
}
