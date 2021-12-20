#include <ArduinoFake.h>
#include <unity.h>
#include "button.h"

using namespace fakeit;

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

    // when button is pressed, make sure we get pressed state
    When(Method(ArduinoFake(), digitalRead).Using(3)).AlwaysReturn(1);
    TestButton.update();
    TestButton._Tmr.MOCK_time_passed(100);
    TestButton.update();
    TEST_ASSERT_EQUAL(BUTTON_NONE, TestButton.get_state());

    When(Method(ArduinoFake(), digitalRead).Using(3)).AlwaysReturn(0);
    TestButton.update();
    TEST_ASSERT_EQUAL(BUTTON_SHORT_PRESS, TestButton.get_state());

    TestButton.update();
    TEST_ASSERT_EQUAL(BUTTON_NONE, TestButton.get_state());
}

void test_long_hold()
{
    Button TestButton(3, true);

    // when button is held down for less than long hold time
    // short press should be returned
    When(Method(ArduinoFake(), digitalRead).Using(3)).AlwaysReturn(1);
    TestButton.update();
    TestButton._Tmr.MOCK_time_passed(500);
    TestButton.update();
    When(Method(ArduinoFake(), digitalRead).Using(3)).AlwaysReturn(0);
    TestButton.update();
    TEST_ASSERT_EQUAL(BUTTON_SHORT_PRESS, TestButton.get_state());
    TestButton._Tmr.MOCK_time_passed(500); // release for > 200ms
    TestButton.update();
    TEST_ASSERT_EQUAL(BUTTON_NONE, TestButton.get_state());

    // when button held for long enough long press start
    When(Method(ArduinoFake(), digitalRead).Using(3)).AlwaysReturn(1);
    TestButton._Tmr.MOCK_time_passed(50);
    TestButton.update();
    TestButton._Tmr.MOCK_time_passed(50);
    TestButton.update();
    TestButton._Tmr.MOCK_time_passed(1200);
    TestButton.update();
    TEST_ASSERT_EQUAL(BUTTON_LONG_HOLD_START, TestButton.get_state());

    // release and expect long hold end
    When(Method(ArduinoFake(), digitalRead).Using(3)).AlwaysReturn(0);
    TestButton.update();
    TestButton._Tmr.MOCK_time_passed(500);
    TestButton.update();
    TEST_ASSERT_EQUAL(BUTTON_LONG_HOLD_END, TestButton.get_state());

    // then no press on release
    TestButton.update();
    TEST_ASSERT_EQUAL(BUTTON_NONE, TestButton.get_state());
}

void test_triple_tap()
{
    Button TestButton(3, true);

    // first press
    When(Method(ArduinoFake(), digitalRead).Using(3)).AlwaysReturn(1);
    TestButton.update();
    TestButton._Tmr.MOCK_time_passed(500);
    TestButton.update();
    When(Method(ArduinoFake(), digitalRead).Using(3)).AlwaysReturn(0);
    TestButton.update();
    TEST_ASSERT_EQUAL(BUTTON_SHORT_PRESS, TestButton.get_state());
    TestButton._Tmr.MOCK_time_passed(500); // release for > 200ms
    TestButton.update();

    // second press
    When(Method(ArduinoFake(), digitalRead).Using(3)).AlwaysReturn(1);
    TestButton.update();
    TestButton._Tmr.MOCK_time_passed(500);
    TestButton.update();
    When(Method(ArduinoFake(), digitalRead).Using(3)).AlwaysReturn(0);
    TestButton.update();
    TEST_ASSERT_EQUAL(BUTTON_DOUPLE_PRESS, TestButton.get_state());
    TestButton._Tmr.MOCK_time_passed(500); // release for > 200ms
    TestButton.update();

    // third press
    When(Method(ArduinoFake(), digitalRead).Using(3)).AlwaysReturn(1);
    TestButton.update();
    TestButton._Tmr.MOCK_time_passed(500);
    TestButton.update();
    When(Method(ArduinoFake(), digitalRead).Using(3)).AlwaysReturn(0);
    TestButton.update();
    TEST_ASSERT_EQUAL(BUTTON_TRIPLE_PRESS, TestButton.get_state());
    TestButton._Tmr.MOCK_time_passed(500); // release for > 200ms
    TestButton.update();
}

int main(int argc, char **argv)
{
    UNITY_BEGIN();

    RUN_TEST(test_not_pressed);
    RUN_TEST(test_short_press);
    RUN_TEST(test_long_hold);
    RUN_TEST(test_triple_tap);

    UNITY_END();

    return 0;
}