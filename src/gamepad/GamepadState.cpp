#include <algorithm>

#include "GamepadState.h"
#include "drivermanager.h"
#include "storagemanager.h"

// Gradually convert an analog axis value to its desired value
// Given the current state (neg/pos value of a specific axis), and the current dpad
// direction, scale the progress to that value
uint16_t decayAnalogViaDpad(const uint8_t dpad, const char axis, const uint16_t currentValue)
{
	if (axis == 'x') {
		switch (dpad & (GAMEPAD_MASK_LEFT | GAMEPAD_MASK_RIGHT))
		{
			case GAMEPAD_MASK_LEFT:
				return decayAnalogToPosOrNeg(GAMEPAD_JOYSTICK_MIN, currentValue);
			case GAMEPAD_MASK_RIGHT:
				return decayAnalogToPosOrNeg(GAMEPAD_JOYSTICK_MAX, currentValue);
			default:
				return decayAnalogToPosOrNeg(GAMEPAD_JOYSTICK_MID, currentValue);
		}
	} else {
		switch (dpad & (GAMEPAD_MASK_UP | GAMEPAD_MASK_DOWN))
		{
			case GAMEPAD_MASK_UP:
				return decayAnalogToPosOrNeg(GAMEPAD_JOYSTICK_MIN, currentValue);
			case GAMEPAD_MASK_DOWN:
				return decayAnalogToPosOrNeg(GAMEPAD_JOYSTICK_MAX, currentValue);
			default:
				return decayAnalogToPosOrNeg(GAMEPAD_JOYSTICK_MID, currentValue);
		}
	}
}


// Gradually convert an analog axis value to its desired value
// Given the current state (neg/pos value of a generic axis), and where (neg/pos on that axis) it
// should go, scale the progress to that value
uint16_t decayAnalogToPosOrNeg(const uint16_t direction, const uint16_t currentValue)
{
	static const double scale = Storage::getInstance().getGamepadOptions().analogEmulationUpdateRate / 50000.0;
	static const uint16_t increment = GAMEPAD_JOYSTICK_MID * scale;	// TODO: make this taper off as it approaches target?
	if (direction < GAMEPAD_JOYSTICK_MID)		// analog is going towards left/up
		return std::max((currentValue - increment), GAMEPAD_JOYSTICK_MIN);
	else if (direction > GAMEPAD_JOYSTICK_MID)	// ... right/down
		return std::min((currentValue + increment), GAMEPAD_JOYSTICK_MAX);
	else						// ... neutral
		return (currentValue > GAMEPAD_JOYSTICK_MID) ? (currentValue - increment) : (currentValue + increment);
}

uint8_t getMaskFromDirection(DpadDirection direction)
{
	return dpadMasks[direction-1];
}

uint8_t updateDpad(uint8_t dpad, DpadDirection direction)
{
	static bool inList[] = {false, false, false, false, false}; // correspond to DpadDirection: none, up, down, left, right
	static list<DpadDirection> dpadList;

	if(dpad & getMaskFromDirection(direction))
	{
		if(!inList[direction])
		{
			dpadList.push_back(direction);
			inList[direction] = true;
		}
	}
	else
	{
		if(inList[direction])
		{
			dpadList.remove(direction);
			inList[direction] = false;
		}
	}

	if(dpadList.empty()) {
		return 0;
	}
	else {
		return getMaskFromDirection(dpadList.back());
	}
}

/**
 * @brief Filter diagonals out of the dpad, making the device work as a 4-way lever.
 *
 * The most recent cardinal direction wins.
 *
 * @param dpad The GameState.dpad value.
 * @return uint8_t The new dpad value.
 */
uint8_t filterToFourWayMode(uint8_t dpad)
{
	updateDpad(dpad, DIRECTION_UP);
	updateDpad(dpad, DIRECTION_DOWN);
	updateDpad(dpad, DIRECTION_LEFT);
	return updateDpad(dpad, DIRECTION_RIGHT);
}

/**
 * @brief Run SOCD cleaning against a D-pad value.
 *
 * @param mode The SOCD cleaning mode.
 * @param dpad The GamepadState.dpad value.
 * @return uint8_t The clean D-pad value.
 */
uint8_t runSOCDCleaner(SOCDMode mode, uint8_t dpad)
{
	if (mode == SOCD_MODE_BYPASS) {
		return dpad;
	}

	static DpadDirection lastUD = DIRECTION_NONE;
	static DpadDirection lastLR = DIRECTION_NONE;
	uint8_t newDpad = 0;

	switch (dpad & (GAMEPAD_MASK_UP | GAMEPAD_MASK_DOWN))
	{
		case (GAMEPAD_MASK_UP | GAMEPAD_MASK_DOWN):
			if (mode == SOCD_MODE_UP_PRIORITY)
			{
				newDpad |= GAMEPAD_MASK_UP;
				lastUD = DIRECTION_UP;
			}
			else if (mode == SOCD_MODE_SECOND_INPUT_PRIORITY && lastUD != DIRECTION_NONE)
				newDpad |= (lastUD == DIRECTION_UP) ? GAMEPAD_MASK_DOWN : GAMEPAD_MASK_UP;
			else if (mode == SOCD_MODE_FIRST_INPUT_PRIORITY && lastUD != DIRECTION_NONE)
				newDpad |= (lastUD == DIRECTION_UP) ? GAMEPAD_MASK_UP : GAMEPAD_MASK_DOWN;
			else
				lastUD = DIRECTION_NONE;
			break;

		case GAMEPAD_MASK_UP:
			newDpad |= GAMEPAD_MASK_UP;
			lastUD = DIRECTION_UP;
			break;

		case GAMEPAD_MASK_DOWN:
			newDpad |= GAMEPAD_MASK_DOWN;
			lastUD = DIRECTION_DOWN;
			break;

		default:
			lastUD = DIRECTION_NONE;
			break;
	}

	switch (dpad & (GAMEPAD_MASK_LEFT | GAMEPAD_MASK_RIGHT))
	{
		case (GAMEPAD_MASK_LEFT | GAMEPAD_MASK_RIGHT):
			if (mode == SOCD_MODE_SECOND_INPUT_PRIORITY && lastLR != DIRECTION_NONE)
				newDpad |= (lastLR == DIRECTION_LEFT) ? GAMEPAD_MASK_RIGHT : GAMEPAD_MASK_LEFT;
			else if (mode == SOCD_MODE_FIRST_INPUT_PRIORITY && lastLR != DIRECTION_NONE)
				newDpad |= (lastLR == DIRECTION_LEFT) ? GAMEPAD_MASK_LEFT : GAMEPAD_MASK_RIGHT;
			else
				lastLR = DIRECTION_NONE;
			break;

		case GAMEPAD_MASK_LEFT:
			newDpad |= GAMEPAD_MASK_LEFT;
			lastLR = DIRECTION_LEFT;
			break;

		case GAMEPAD_MASK_RIGHT:
			newDpad |= GAMEPAD_MASK_RIGHT;
			lastLR = DIRECTION_RIGHT;
			break;

		default:
			lastLR = DIRECTION_NONE;
			break;
	}

	return newDpad;
}
