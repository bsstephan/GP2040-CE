/*
 * SPDX-License-Identifier: MIT
 * SPDX-FileCopyrightText: Copyright (c) 2021 Jason Skuby (mytechtoybox.com)
 */

#ifndef FLASHPROM_H_
#define FLASHPROM_H_

#include <stdint.h>
#include <pico/lock_core.h>
#include <pico/multicore.h>
#include <hardware/flash.h>
#include <hardware/timer.h>

#define UCONFIG_EEPROM_SIZE_BYTES    0x8000		// Reserve 32k of flash memory (ensure this value is divisible by 256)
#define UCONFIG_EEPROM_ADDRESS_START _u(0x101F8000)
#define RCONFIG_EEPROM_SIZE_BYTES    0x0100		// 256 bytes
#define RCONFIG_EEPROM_ADDRESS_START _u(0x101F7F00)

// Warning: If the write wait is too long it can stall other processes
#define EEPROM_WRITE_WAIT    50             // Amount of time in ms to wait before blocking core1 and committing to flash

class FlashPROM
{
	public:
		void userConfigStart();
		void userConfigCommit();
		void userConfigReset();
		void runtimeConfigStart();
		void runtimeConfigCommit();
		void runtimeConfigReset();

		static uint8_t userConfigWriteCache[UCONFIG_EEPROM_SIZE_BYTES];
		static uint8_t runtimeConfigWriteCache[RCONFIG_EEPROM_SIZE_BYTES];
};

inline FlashPROM EEPROM;

#endif
