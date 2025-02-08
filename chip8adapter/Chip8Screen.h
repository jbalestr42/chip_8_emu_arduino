#pragma once

#include "chip8/IScreen.h"
#include <Adafruit_SSD1306.h>

class Chip8Screen : public chip8::IScreen
{
public:
	Chip8Screen(Adafruit_SSD1306 &screen) :
		_screen(screen),
		_width(64),
		_height(32)
	{}

	void setup()
	{
		_screen.clearDisplay();
	}

	void display() { _screen.display(); }
	void clear() { _screen.clearDisplay(); }

	uint8_t width() const { return _width; }
	uint8_t height() const { return _height; }

	void putPixel(uint8_t x, uint8_t y, bool isOn) { putPixel(x, y, (uint16_t)(isOn ? WHITE : BLACK)); }
	void putPixel(uint8_t x, uint8_t y, uint16_t color)
	{
		// The real screen resolution is 128x64 and chip8 is 64x32
		_screen.drawPixel(x * 2, y * 2, color);
		_screen.drawPixel(x * 2 + 1, y * 2, color);
		_screen.drawPixel(x * 2, y * 2 + 1, color);
		_screen.drawPixel(x * 2 + 1, y * 2 + 1, color);
	}

	bool isPixelOn(uint8_t x, uint8_t y) const { return getPixel(x, y) == WHITE; }
	uint16_t getPixel(uint8_t x, uint8_t y) const { return _screen.getPixel(x * 2, y * 2); }

private:
	Adafruit_SSD1306 &_screen;

	uint8_t _width;
	uint8_t _height;
};
