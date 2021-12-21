#ifndef BUTTON_H
#define BUTTON_H

#include <Arduino.h>
#include "timer.h"

enum button_state_t
{
    BUTTON_NONE,
    BUTTON_SHORT_PRESS,
    BUTTON_LONG_HOLD_START,
    BUTTON_LONG_HOLD_END,
    BUTTON_DOUPLE_PRESS,
    BUTTON_TRIPLE_PRESS
};

class Button
{
public:
    Button(uint8_t pin, bool polarity_high = false, bool multi_press_enabled = false);
    void begin();
    void update();
    button_state_t get_state();
    bool is_pressed();
    void set_multi_press_enabled(bool enabled);
    Timer _Tmr;

private:
    enum state_t
    {
        NOT_PRESSED,
        PRESS_START,
        VALID_PRESS,
        SHORT_PRESS,
        LONG_PRESS_START,
        LONG_PRESS_END
    };

    uint8_t _pin;
    bool _pol;
    state_t _state = NOT_PRESSED;
    button_state_t _state_to_give = BUTTON_NONE;
    bool _is_new_state = false;
    uint8_t _press_count = 0;
    bool _multi_press_enabled = false;
};

#endif