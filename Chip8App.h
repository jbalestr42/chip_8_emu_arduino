#pragma once

#include "app/IApp.h"
#include "chip8adapter/chip8/Chip8.h"
#include "chip8adapter/Chip8Screen.h"
#include "chip8adapter/Chip8Input.h"
#include "chip8adapter/RomConfig.h"
#include "MainApplication.h"

class Chip8App : public IApp
{
public:
	Chip8App(MainApplication &mainApp) :
		_mainApp(mainApp),
		_chip8Screen(mainApp.screen()),
		_chip8Input(mainApp.controller()),
		_emulator(_chip8Screen, _chip8Input)
	{ }

	void loop()
	{
		_emulator.loop();
	}

	void onAppEnabled() override
	{
		_mainApp.setEnableGlobalScreenManagement(false);
		_chip8Screen.setup();
		_emulator.setup();
	}

	void onAppDisabled() override
	{
		_mainApp.setEnableGlobalScreenManagement(true);
	}

	void setRomConfig(const RomConfig &romConfig)
	{
		_chip8Input.setBindings(romConfig.bindings());
		_emulator.loadRom(romConfig.rom());
	}

private:
	MainApplication &_mainApp;
	Chip8Screen _chip8Screen;
	Chip8Input _chip8Input;
	chip8::Chip8 _emulator;
};
