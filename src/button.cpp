#include <Arduino.h>
#include "button.h"

#define UPDATE_INTERVAL_MS 10
#define PRESSED_LONG_MIN_MS 1000
#define NOT_PRESSED_MIN_MS 200

Button::Button(uint8_t pin, bool polarity_high)
{
    _pin = pin;
    _pol = polarity_high;
}

void Button::begin()
{
    pinMode(_pin, INPUT);
}

bool Button::is_pressed()
{
    if (digitalRead(_pin) == _pol)
    {
        return true;
    }
    return false;
}

void Button::update()
{
    if (_Tmr.time_passed(UPDATE_INTERVAL_MS))
    {
        bool pressed_now = is_pressed();

        if (pressed_now)
        {
            _pressed_ms = _pressed_ms + UPDATE_INTERVAL_MS;
            _not_pressed_ms = 0;
        }
        else
        {
            _not_pressed_ms = _not_pressed_ms + UPDATE_INTERVAL_MS;
            _pressed_ms = 0;
        }

        switch (_state)
        {
        case NOT_PRESSED:
            if (pressed_now)
            {
                _state = WAITING_FOR_LONG_HOLD;
            }
            break;

        case WAITING_FOR_LONG_HOLD:
            if (_pressed_ms >= PRESSED_LONG_MIN_MS)
            {
                _state = LONG_HOLD_START;
            }
            if (!pressed_now)
            {
                _state = SHORT_PRESS;
            }
            break;

        case LONG_HOLD:
            if (!pressed_now)
            {
                _state = LONG_HOLD_END;
            }
            break;

        case WAITING_FOR_RELEASE:
            if (_not_pressed_ms >= NOT_PRESSED_MIN_MS)
            {
                _state = NOT_PRESSED;
            }
            break;
        }
    }
}

button_state_t Button::get_state()
{
    switch (_state)
    {
    case SHORT_PRESS:
        _state = WAITING_FOR_RELEASE;
        return BUTTON_SHORT_PRESS;

    case LONG_HOLD_START:
        _state = LONG_HOLD;
        return BUTTON_LONG_HOLD_START;

    case LONG_HOLD_END:
        _state = WAITING_FOR_RELEASE;
        return BUTTON_LONG_HOLD_END;
    }
    return BUTTON_NONE;
}