/*
 * SPDX-License-Identifier: MIT
 * SPDX-FileCopyrightText: Copyright (c) 2021 Jason Skuby (mytechtoybox.com)
 */

#include <string.h>

#include "FlashPROM.h"

uint8_t FlashPROM::userConfigWriteCache[UCONFIG_EEPROM_SIZE_BYTES];
uint8_t FlashPROM::runtimeConfigWriteCache[RCONFIG_EEPROM_SIZE_BYTES];
volatile static alarm_id_t flashWriteAlarm = 0;
volatile static spin_lock_t *flashLock = nullptr;

int64_t writeUserConfigToFlash(alarm_id_t id, void *flashCache)
{
	while (is_spin_locked(flashLock));

	multicore_lockout_start_blocking();
	uint32_t interrupts = spin_lock_blocking(flashLock);

	flash_range_erase((intptr_t)UCONFIG_EEPROM_ADDRESS_START -
			(intptr_t)XIP_BASE, UCONFIG_EEPROM_SIZE_BYTES);
	flash_range_program((intptr_t)UCONFIG_EEPROM_ADDRESS_START -
			(intptr_t)XIP_BASE, reinterpret_cast<uint8_t *>(flashCache), UCONFIG_EEPROM_SIZE_BYTES);

	flashWriteAlarm = 0;

	multicore_lockout_end_blocking();
	spin_unlock(flashLock, interrupts);

	return 0;
}

/* sorry for the copy and paste job here, I can't DRY this easily because of the interface for add_alarm_in_ms ~bss */
int64_t writeRuntimeConfigToFlash(alarm_id_t id, void *flashCache)
{
	while (is_spin_locked(flashLock));

	multicore_lockout_start_blocking();
	uint32_t interrupts = spin_lock_blocking(flashLock);

	flash_range_erase((intptr_t)RCONFIG_EEPROM_ADDRESS_START -
			(intptr_t)XIP_BASE, RCONFIG_EEPROM_SIZE_BYTES);
	flash_range_program((intptr_t)RCONFIG_EEPROM_ADDRESS_START -
			(intptr_t)XIP_BASE, reinterpret_cast<uint8_t *>(flashCache), RCONFIG_EEPROM_SIZE_BYTES);

	flashWriteAlarm = 0;

	multicore_lockout_end_blocking();
	spin_unlock(flashLock, interrupts);

	return 0;
}

void FlashPROM::userConfigStart()
{
	if (flashLock == nullptr)
		flashLock = spin_lock_instance(spin_lock_claim_unused(true));

	memcpy(userConfigWriteCache, reinterpret_cast<uint8_t *>(UCONFIG_EEPROM_ADDRESS_START), UCONFIG_EEPROM_SIZE_BYTES);
}

void FlashPROM::runtimeConfigStart()
{
	if (flashLock == nullptr)
		flashLock = spin_lock_instance(spin_lock_claim_unused(true));

	memcpy(runtimeConfigWriteCache, reinterpret_cast<uint8_t *>(RCONFIG_EEPROM_ADDRESS_START), RCONFIG_EEPROM_SIZE_BYTES);
}

/* We don't have an actual EEPROM, so we need to be extra careful about minimizing writes. Instead
	of writing when a commit is requested, we update a time to actually commit. That way, if we receive multiple requests
	to commit in that timeframe, we'll hold off until the user is done sending changes. */
void FlashPROM::userConfigCommit()
{
	while (is_spin_locked(flashLock));
	if (flashWriteAlarm != 0)
		cancel_alarm(flashWriteAlarm);
	flashWriteAlarm = add_alarm_in_ms(EEPROM_WRITE_WAIT, writeUserConfigToFlash, userConfigWriteCache, true);
}

void FlashPROM::runtimeConfigCommit()
{
	while (is_spin_locked(flashLock));
	if (flashWriteAlarm != 0)
		cancel_alarm(flashWriteAlarm);
	flashWriteAlarm = add_alarm_in_ms(EEPROM_WRITE_WAIT, writeRuntimeConfigToFlash, runtimeConfigWriteCache, true);
}

void FlashPROM::userConfigReset()
{
	memset(userConfigWriteCache, 0, UCONFIG_EEPROM_SIZE_BYTES);
	userConfigCommit();
}

void FlashPROM::runtimeConfigReset()
{
	memset(runtimeConfigWriteCache, 0, RCONFIG_EEPROM_SIZE_BYTES);
	runtimeConfigCommit();
}
