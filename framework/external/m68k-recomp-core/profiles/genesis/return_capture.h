/*
 * return_capture.h — conservative proof for 68000 return-address consumers.
 */
#pragma once

#include <stdbool.h>
#include <stdint.h>

#include "rom_parser.h"

/*
 * True only when every reachable path from `start` consumes a longword from
 * (A7)+ before it can return, call another routine, or grow the stack.
 *
 * This covers both the straight-line Obj_WaitOffscreen idiom and helpers such
 * as Puyo Puyo's timer routine, whose conditional paths all consume the
 * caller's return address at different instructions.
 */
bool m68k_return_capture_is_unconditional(const GenesisRom *rom, uint32_t start);
