#pragma once

#include "chip8/Roms.h"
#include "../NesController.h"

class RomConfig
{
public:
	RomConfig(const char *name, const uint8_t *data, size_t size, const NesController::Button *bindings, size_t cyclesPerFrame, bool saveLoadIncrement, bool vfReset, bool clipping, bool shifting, bool displayWait) :
		_rom(name, data, size, cyclesPerFrame, saveLoadIncrement, vfReset, clipping, shifting, displayWait),
		_bindings(bindings)
	{ }

	const chip8::Rom &rom() const { return _rom; }
	const NesController::Button *bindings() const { return _bindings; }

private:
	const chip8::Rom _rom;
	const NesController::Button *_bindings;
};
