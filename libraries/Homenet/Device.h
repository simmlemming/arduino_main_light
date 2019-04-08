#ifndef STATE
#define STATE "state"

#include <ArduinoJson.h>

#define STATE_OFF 0
#define STATE_OK 1
#define STATE_INIT 2
#define STATE_ERROR 3
#define STATE_ALARM 4
#define STATE_ALARM_PENDING 5
#define STATE_PAUSED 6

class Device {
   private:
    char *_name;
    char *_room;
    char *_type;
    long _wifi_strength;
    int _state;
    volatile int _value;

   protected:
    bool _changed;

   public:
    Device(char *name, char *room, char *type);
    bool loop();

    JsonObject to_json();

    void set_wifi_strength(long strength);
    void set_state(int state);
    void set_value(int value);

    long get_wifi_strength();
    int get_state();
    int get_value();
    char *get_name();
    char *get_room();
    char *get_type();
};

#endif