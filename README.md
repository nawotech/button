# button
Simple button Arduino library

## How to Use

1. Create a button, pin number and polarity (true if high is pressed, false if low is pressed)
```c++
Button MyButton(3, true);
```

2. In setup, call begin function (this sets pin as input)
```c++
void setup()
{
  MyButton.begin();
}
```

4. In the main loop, call the update function
```c++
void loop()
{
  MyButton.update();
}
```
3. Check the state of the button, you can so something when it is pressed quickly, at the start of when its held long, and at the end of when the long hold is released
```c++
void loop()
{
  MyButton.update();
  
  button_state_t button_state = MyButton.get_state();
  if (button_state == BUTTON_SHORT_PRESS)
  {
    Serial.println("Short press");
  }
  else if (button_state == BUTTON_LONG_HOLD_START)
  {
    Serial.println("Long hold start");
  }
  else if (button_state == BUTTON_LONG_HOLD_END)
  {
    Serial.println("Long hold end");
  }
}
```
