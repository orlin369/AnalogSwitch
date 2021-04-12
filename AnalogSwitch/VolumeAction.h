
#ifndef _VOLUMEACTION_h
#define _VOLUMEACTION_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

#pragma region Enums

/**
 * @brief Volume action.
 * 
 */
enum VolumeAction_t : uint8_t
{
    Nothing, ///< Nothing
    Up, ///< Increase volume.
    Down ///< Decrease volume.
};

#pragma endregion

#endif // _APPLICATIONCONFIGURATION_h