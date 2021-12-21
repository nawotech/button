#include <ArduinoFake.h>
#include <unity.h>
#include "button.h"

#define PRESS_TOO_SHORT_MS 29
#define PRESS_LONG_ENOUGH_MS 50
#define UNPRESS_LONG_ENOUGH_MS 150
#define LONG_PRESS_HOLD_TOO_SHORT_MS 900
#define LONG_PRESS_HOLD_LONG_ENOUGH_MS 1100
#define MULTI_PRESS_RELEASE_LONG_ENOUGH_MS 1000

using namespace fakeit;

void simulate_button_press(Button *Btn, bool pressed, int t_ms)
{
    When(Method(ArduinoFake(), digitalRead).Using(3)).AlwaysReturn(pressed);
    Btn->_Tmr.MOCK_time_passed(1);
    Btn->update();
    Btn->_Tmr.MOCK_time_passed(1);
    Btn->update();
    Btn->_Tmr.MOCK_time_passed(1);
    Btn->update();
    Btn->_Tmr.MOCK_time_passed(t_ms);
    Btn->update();
    Btn->_Tmr.MOCK_time_passed(1);
    Btn->update();
    Btn->_Tmr.MOCK_time_passed(1);
    Btn->update();
}

void test_not_pressed()
{
    Button TestButton(3, true);

    // when button is not pressed, make sure we get not pressed state
    When(Method(ArduinoFake(), digitalRead).Using(3)).AlwaysReturn(0);
    TEST_ASSERT_EQUAL(BUTTON_NONE, TestButton.get_state());
}

void test_short_press()
{
    Button TestButton(3, true);

    // too short press, expect nothing
    simulate_button_press(&TestButton, 1, PRESS_TOO_SHORT_MS);
    TEST_ASSERT_EQUAL(BUTTON_NONE, TestButton.get_state());

    // short press, expect short presss
    simulate_button_press(&TestButton, 1, PRESS_LONG_ENOUGH_MS);
    simulate_button_press(&TestButton, 0, UNPRESS_LONG_ENOUGH_MS);
    TEST_ASSERT_EQUAL(BUTTON_SHORT_PRESS, TestButton.get_state());

    // once read, expect nothing again
    TEST_ASSERT_EQUAL(BUTTON_NONE, TestButton.get_state());
}

void test_long_hold()
{
    Button TestButton(3, true);

    // hold not long enough, expect short press
    simulate_button_press(&TestButton, 1, LONG_PRESS_HOLD_TOO_SHORT_MS);
    simulate_button_press(&TestButton, 0, UNPRESS_LONG_ENOUGH_MS);
    TEST_ASSERT_EQUAL(BUTTON_SHORT_PRESS, TestButton.get_state());

    // once read, expect nothing again
    TEST_ASSERT_EQUAL(BUTTON_NONE, TestButton.get_state());

    // hold long enough, while holding long press start
    simulate_button_press(&TestButton, 1, PRESS_LONG_ENOUGH_MS);
    simulate_button_press(&TestButton, 1, LONG_PRESS_HOLD_LONG_ENOUGH_MS);
    TEST_ASSERT_EQUAL(BUTTON_LONG_HOLD_START, TestButton.get_state());
    // release, long press end
    simulate_button_press(&TestButton, 0, UNPRESS_LONG_ENOUGH_MS);
    TEST_ASSERT_EQUAL(BUTTON_LONG_HOLD_END, TestButton.get_state());

    // once read, expect nothing again
    TEST_ASSERT_EQUAL(BUTTON_NONE, TestButton.get_state());
}

void test_multi_press()
{
    Button TestButton(3, true);
    // multi press disabled

    // only short press
    simulate_button_press(&TestButton, 1, PRESS_LONG_ENOUGH_MS);
    simulate_button_press(&TestButton, 0, UNPRESS_LONG_ENOUGH_MS);
    TEST_ASSERT_EQUAL(BUTTON_SHORT_PRESS, TestButton.get_state());

    // should not see double press
    simulate_button_press(&TestButton, 1, PRESS_LONG_ENOUGH_MS);
    simulate_button_press(&TestButton, 0, UNPRESS_LONG_ENOUGH_MS);
    TEST_ASSERT_EQUAL(BUTTON_SHORT_PRESS, TestButton.get_state());

    // multi press enabled
    TestButton.set_multi_press_enabled(true);

    // short press
    simulate_button_press(&TestButton, 1, PRESS_LONG_ENOUGH_MS);
    simulate_button_press(&TestButton, 0, MULTI_PRESS_RELEASE_LONG_ENOUGH_MS);
    TEST_ASSERT_EQUAL(BUTTON_SHORT_PRESS, TestButton.get_state());

    // double press
    simulate_button_press(&TestButton, 1, PRESS_LONG_ENOUGH_MS);
    simulate_button_press(&TestButton, 0, UNPRESS_LONG_ENOUGH_MS);
    TEST_ASSERT_EQUAL(BUTTON_NONE, TestButton.get_state());

    simulate_button_press(&TestButton, 1, PRESS_LONG_ENOUGH_MS);
    simulate_button_press(&TestButton, 0, MULTI_PRESS_RELEASE_LONG_ENOUGH_MS);
    TEST_ASSERT_EQUAL(BUTTON_DOUPLE_PRESS, TestButton.get_state());

    // triple press
    simulate_button_press(&TestButton, 1, PRESS_LONG_ENOUGH_MS);
    simulate_button_press(&TestButton, 0, UNPRESS_LONG_ENOUGH_MS);
    TEST_ASSERT_EQUAL(BUTTON_NONE, TestButton.get_state());

    simulate_button_press(&TestButton, 1, PRESS_LONG_ENOUGH_MS);
    simulate_button_press(&TestButton, 0, UNPRESS_LONG_ENOUGH_MS);
    TEST_ASSERT_EQUAL(BUTTON_NONE, TestButton.get_state());

    simulate_button_press(&TestButton, 1, PRESS_LONG_ENOUGH_MS);
    simulate_button_press(&TestButton, 0, MULTI_PRESS_RELEASE_LONG_ENOUGH_MS);
    TEST_ASSERT_EQUAL(BUTTON_TRIPLE_PRESS, TestButton.get_state());
}

int main(int argc, char **argv)
{
    // test_long_hold();

    UNITY_BEGIN();

    RUN_TEST(test_not_pressed);
    RUN_TEST(test_short_press);
    RUN_TEST(test_long_hold);
    RUN_TEST(test_multi_press);

    UNITY_END();

    return 0;
}