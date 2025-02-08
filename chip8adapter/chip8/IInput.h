#pragma once

namespace chip8
{

class IInput
{
public:
	virtual bool isKeyPressed(uint8_t keyCode) const = 0;
	virtual bool isKeyDown(uint8_t keyCode) const = 0;
	virtual bool isKeyReleased(uint8_t keyCode) const = 0;
};

}
