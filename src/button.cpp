#include <Arduino.h>
#include "button.h"

#define PRESSED_MIN_MS 30
#define PRESSED_LONG_MIN_MS 1000
#define NOT_PRESSED_MIN_MS 200
#define BETWEEN_MULTI_PRESS_MAX_MS 900

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
    bool pressed_now = is_pressed();

    switch (_state)
    {
    case NOT_PRESSED:
        if (!_is_new_state)
        {
            if (pressed_now)
            {
                _press_count = 0;
                _Tmr.reset();
                _state = PRESS_START;
            }
        }
        break;

    case PRESS_START:
        if (!pressed_now)
        {
            _state = NOT_PRESSED;
        }
        else if (_Tmr.get_ms() > PRESSED_MIN_MS)
        {
            _state = VALID_PRESS;
        }
        break;

    case VALID_PRESS:
        if (!pressed_now)
        {
            _press_count++;
            _Tmr.reset();
            _state = SHORT_PRESS;
            if (_press_count == 1)
            {
                _state_to_give = BUTTON_SHORT_PRESS;
            }
            else if (_press_count == 2)
            {
                _state_to_give = BUTTON_DOUPLE_PRESS;
            }
            else if (_press_count == 3)
            {
                _state_to_give = BUTTON_TRIPLE_PRESS;
            }
            _is_new_state = true;
        }
        else if (_Tmr.get_ms() > PRESSED_LONG_MIN_MS)
        {
            _state = LONG_PRESS_START;
            _state_to_give = BUTTON_LONG_HOLD_START;
            _is_new_state = true;
        }
        break;

    case SHORT_PRESS:
        if (pressed_now)
        {
            _Tmr.reset();
            _state = PRESS_START;
        }
        else if (_Tmr.get_ms() > BETWEEN_MULTI_PRESS_MAX_MS)
        {
            _state = NOT_PRESSED;
        }
        break;

    case LONG_PRESS_START:
        if (!pressed_now)
        {
            _Tmr.reset();
            _state = LONG_PRESS_END;
            _state_to_give = BUTTON_LONG_HOLD_END;
            _is_new_state = true;
        }
        break;

    case LONG_PRESS_END:
        if (pressed_now)
        {
            _Tmr.reset();
        }
        else if (_Tmr.get_ms() > NOT_PRESSED_MIN_MS)
        {
            _state = NOT_PRESSED;
        }
        break;
    }
}

button_state_t Button::get_state()
{
    button_state_t state_ret = _state_to_give;
    _state_to_give = BUTTON_NONE;
    _is_new_state = false;
    return state_ret;
}