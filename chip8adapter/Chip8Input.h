#pragma once

#include "chip8/IInput.h"
#include "../NesController.h"

class Chip8Input : public chip8::IInput
{
public:
	Chip8Input(NesController &nesController) :
		_nesController(nesController),
		_bindings(nullptr)
	{ }

	void setBindings(const NesController::Button *bindings) { _bindings = bindings; }
	bool isKeyPressed(uint8_t keyCode) const { return isKeyState(keyCode, NesController::InputState::Pressed); }
	bool isKeyDown(uint8_t keyCode) const { return isKeyState(keyCode, NesController::InputState::Down); }
	bool isKeyReleased(uint8_t keyCode) const { return isKeyState(keyCode, NesController::InputState::Released); }

	bool isKeyState(uint8_t keyCode, NesController::InputState state) const
	{
		if (_bindings == nullptr)
		{
			return false;
		}
		return _nesController.getButtonState(_bindings[keyCode]) == state;
	}

private: NesController &_nesController;
	const NesController::Button *_bindings;
};
