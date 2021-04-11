
#ifndef _APPLICATIONCONFIGURATION_h
#define _APPLICATIONCONFIGURATION_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif


#pragma region Status LED

/**
 * @brief Status LED blink interval.
 * 
 */
#define BLINK_INTERVAL 1000

#pragma endregion

#pragma region Pot Parameter

/**
 * @brief Minimum pot value.
 * 
 */
#define MIN_POT_VAL 39

/**
 * @brief Maximum pot value.
 * 
 */
#define MAX_POT_VAL 0

/**
 * @brief Minimum volume value.
 * 
 */
#define MIN_VOL_VAL 0

/**
 * @brief Maximum volume value.
 * 
 */
#define MAX_VOL_VAL 100

#pragma endregion

#pragma region GPIOs

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
#define PIN_BTN_IN1 5

/**
 * @brief Pin button input 2.
 * 
 */
#define PIN_BTN_IN2 3

/**
 * @brief Pin button input 3.
 * 
 */
#define PIN_BTN_IN3 2

/**
 * @brief Pin button input 4.
 * 
 */
#define PIN_BTN_IN4 15

/**
 * @brief Pin button learn IR commands from the remote.
 * 
 */
#define PIN_BTN_LEARN 25

/**
 * @brief Pin relay input 1.
 * 
 */
#define PIN_REL_IN1 27

/**
 * @brief Pin relay input 2.
 * 
 */
#define PIN_REL_IN2 14

/**
 * @brief Pin relay input 3.
 * 
 */
#define PIN_REL_IN3 32

/**
 * @brief Pin relay input 4.
 * 
 */
#define PIN_REL_IN4 33

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

#pragma endregion

#pragma region Display Params

/**
 * @brief Reset pin # (or -1 if sharing Arduino reset pin).
 * 
 */
#define OLED_RESET 4

/**
 * @brief Display I2C address.
 * 
 */
#define SCREEN_ADDRESS 0x3C

/**
 * @brief Screen width, [pixels].
 * 
 */
#define SCREEN_WIDTH 128

/**
 * @brief Screen height, [pixels].
 * 
 */
#define SCREEN_HEIGHT 32

/**
 * @brief Character width, [pixels].
 * 
 */
#define CHARATER_WIDTH 6

/**
 * @brief Channel animation screen offset, [pixels].
 * 
 */
#define OFS_X_CH (int)((SCREEN_WIDTH - MAX_VOL_VAL) / 2.0)

/**
 * @brief Channel square X position, [pixels].
 * 
 */
#define RECT_X_CH 20

/**
 * @brief Channel square width, [pixels].
 * 
 */
#define RECT_W_CH 10

/**
 * @brief Channel square height, [pixels].
 * 
 */
#define RECT_H_CH 5

/**
 * @brief Channel 1 X position, [pixels].
 * 
 */
#define RECT_X_CH1 (OFS_X_CH + RECT_X_CH * 3)

/**
 * @brief Channel 2 X position, [pixels].
 * 
 */
#define RECT_X_CH2 (OFS_X_CH + RECT_X_CH * 2)

/**
 * @brief Channel 3 X position, [pixels].
 * 
 */
#define RECT_X_CH3 (OFS_X_CH + RECT_X_CH * 1)

/**
 * @brief Channel 4 X position, [pixels].
 * 
 */
#define RECT_X_CH4 (OFS_X_CH + RECT_X_CH * 0)

/**
 * @brief Channel 1 Y position, [pixels].
 * 
 */
#define RECT_Y_CH1 (5)

/**
 * @brief Channel 2 Y position, [pixels].
 * 
 */
#define RECT_Y_CH2 (5)

/**
 * @brief Channel 3 Y position, [pixels].
 * 
 */
#define RECT_Y_CH3 (5)

/**
 * @brief Channel 4 Y position, [pixels].
 * 
 */
#define RECT_Y_CH4 (5)


/**
 * @brief Volume rectangle X position, [pixels].
 * 
 */
#define RECT_X_VOL (int)((SCREEN_WIDTH - MAX_VOL_VAL) / 2.0)

/**
 * @brief Volume rectangle Y position, [pixels].
 * 
 */
#define RECT_Y_VOL 20

/**
 * @brief Volume rectangle height, [pixels].
 * 
 */
#define RECT_H_VOL 10

/**
 * @brief Volume text X position, [pixels].
 * 
 */
#define TX_X_VOL (int)((SCREEN_WIDTH / 2.0) - CHARATER_WIDTH)

/**
 * @brief Volume text Y position, [pixels].
 * 
 */
#define TX_Y_VOL (RECT_Y_VOL + 2)

#pragma endregion

#endif // _APPLICATIONCONFIGURATION_h