#pragma once

#include "RomConfig.h"
#include "data/breakout_rom.h"
#include "data/danm8ku_rom.h"
#include "data/down8_rom.h"
#include "data/pong_rom.h"
#include "data/tetris_rom.h"

const RomConfig romConfigs[] = {
	RomConfig("Tetris", &tetris[0], sizeof(tetris) / sizeof(uint8_t), tetrisBindings, 12, true, true, true, true, false),
	RomConfig("Breakout", &breakout[0], sizeof(breakout) / sizeof(uint8_t), breakoutBindings, 18, true, true, true, true, false),
	RomConfig("Pong", &pong[0], sizeof(pong) / sizeof(uint8_t), pongBindings, 24, true, true, true, true, false),
	RomConfig("Danm8ku", &danm8ku[0], sizeof(danm8ku) / sizeof(uint8_t), danm8kuBindings, 768, true, false, true, true, false),
	RomConfig("Down8", &down8[0], sizeof(down8) / sizeof(uint8_t), down8Bindings, 18, true, true, true, true, false)
};
