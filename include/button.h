#ifndef BUTTON_H
#define BUTTON_H

#include <Arduino.h>
#include "timer.h"

enum button_state_t
{
    BUTTON_NONE,
    BUTTON_SHORT_PRESS,
    BUTTON_LONG_HOLD_START,
    BUTTON_LONG_HOLD_END
};

class Button
{
public:
    Button(uint8_t pin, bool polarity_high = false);
    void begin();
    void update();
    button_state_t get_state();
    bool is_pressed();

private:
    enum state_t
    {
        NOT_PRESSED,
        WAITING_FOR_LONG_HOLD,
        SHORT_PRESS,
        LONG_HOLD_START,
        LONG_HOLD,
        LONG_HOLD_END,
        WAITING_FOR_RELEASE
    };

    uint8_t _pin;
    bool _pol;
    Timer _Tmr;
    uint16_t _pressed_ms = 0;
    uint16_t _not_pressed_ms = 0;
    state_t _state = NOT_PRESSED;
};

#endif