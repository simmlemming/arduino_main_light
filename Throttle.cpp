#include <ESP8266WiFi.h>

class Throttle {
  public:
    Throttle(long ms) {
      _ms = ms;
    }

    bool throttled(bool input) {
      if (input) {
        _last_input_ms = millis();
      }

      bool output = _last_input_ms != 0 && millis() -_last_input_ms > _ms;
      if (output) {
        _last_input_ms = 0;
      }

      return output;
    }
    
  private:
    long _ms;
    long _last_input_ms;
};
