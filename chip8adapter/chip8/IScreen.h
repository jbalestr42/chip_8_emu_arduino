#pragma once

namespace chip8
{

class IScreen
{
public:
	virtual void display() = 0;
	virtual void clear() = 0;
	virtual uint8_t width() const = 0;
	virtual uint8_t height() const = 0;
	virtual bool isPixelOn(uint8_t x, uint8_t y) const = 0;
	virtual void putPixel(uint8_t x, uint8_t y, bool isOn) = 0;
};

}
