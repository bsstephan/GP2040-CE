/*
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

// -----------------------------------------------------
// NOTE: THIS HEADER IS ALSO INCLUDED BY ASSEMBLER SO
//       SHOULD ONLY CONSIST OF PREPROCESSOR DIRECTIVES
// -----------------------------------------------------


#ifndef _BOARDS_RP2040ADVANCEDBREAKOUTBOARDUSBPASSTHROUGH_BSS_H
#define _BOARDS_RP2040ADVANCEDBREAKOUTBOARDUSBPASSTHROUGH_BSS_H

// For board detection
#define RP2040ADVANCEDBREAKOUTBOARDUSBPASSTHROUGH_BSS

// --- LED --- no
#ifdef PICO_DEFAULT_LED_PIN
#undef PICO_DEFAULT_LED_PIN
#endif

// --- FLASH ---
#define PICO_BOOT_STAGE2_CHOOSE_W25Q080 1

#ifndef PICO_FLASH_SPI_CLKDIV
#define PICO_FLASH_SPI_CLKDIV 2
#endif

#ifndef PICO_FLASH_SIZE_BYTES
#define PICO_FLASH_SIZE_BYTES (2 * 1024 * 1024)
#endif

#ifndef PICO_RP2040_B0_SUPPORTED
#define PICO_RP2040_B0_SUPPORTED 1
#endif

#endif
