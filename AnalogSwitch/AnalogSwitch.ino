
#include "ApplicationConfiguration.h"

void setup_io();

void setup()
{
    setup_io();
}

void loop()
{

}

void setup_io()
{
    pinMode(PIN_REL_IN1, OUTPUT);
    pinMode(PIN_REL_IN2, OUTPUT);
    pinMode(PIN_REL_IN3, OUTPUT);
    pinMode(PIN_REL_IN4, OUTPUT);

    digitalWrite(PIN_REL_IN1, LOW);
    digitalWrite(PIN_REL_IN2, LOW);
    digitalWrite(PIN_REL_IN3, LOW);
    digitalWrite(PIN_REL_IN4, LOW);

    pinMode(PIN_BTN_LEARN, INPUT_PULLUP);
    pinMode(PIN_BTN_VOL_DN, INPUT_PULLUP);
    pinMode(PIN_BTN_VOL_UP, INPUT_PULLUP);

    pinMode(PIN_BTN_IN1, INPUT_PULLUP);
    pinMode(PIN_BTN_IN2, INPUT_PULLUP);
    pinMode(PIN_BTN_IN3, INPUT_PULLUP);
    pinMode(PIN_BTN_IN4, INPUT_PULLUP);

    pinMode(PIN_LED_STATUS, OUTPUT);
}