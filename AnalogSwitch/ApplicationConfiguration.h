
#ifndef _APPLICATIONCONFIGURATION_h
#define _APPLICATIONCONFIGURATION_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

/**
 * @brief Digital potentiometer I2C address.
 * 
 */
#define ADDRESS_POT 0x44

/**
 * @brief OLED Display I2C address.
 * 
 */
#define ADDRESS_OLED 0x3C

/**
 * @brief Pin button volume up.
 * 
 */
#define PIN_BTN_VOL_UP 12

/**
 * @brief Pin button volume down.
 * 
 */
#define PIN_BTN_VOL_DN 13

/**
 * @brief Pin button input 1.
 * 
 */
#define PIN_BTN_IN1 15

/**
 * @brief Pin button input 2.
 * 
 */
#define PIN_BTN_IN2 2

/**
 * @brief Pin button input 3.
 * 
 */
#define PIN_BTN_IN3 3

/**
 * @brief Pin button input 4.
 * 
 */
#define PIN_BTN_IN4 5

/**
 * @brief Pin button learn IR commands from the remote.
 * 
 */
#define PIN_BTN_LEARN 25

/**
 * @brief Pin relay input 1.
 * 
 */
#define PIN_REL_IN1 33

/**
 * @brief Pin relay input 2.
 * 
 */
#define PIN_REL_IN2 32

/**
 * @brief Pin relay input 3.
 * 
 */
#define PIN_REL_IN3 14

/**
 * @brief Pin relay input 4.
 * 
 */
#define PIN_REL_IN4 27

/**
 * @brief Pin IR receiver.
 * 
 */
#define PIN_IR 18

/**
 * @brief Pin status LED.
 * 
 */
#define PIN_LED_STATUS 26

#endif // _APPLICATIONCONFIGURATION_h