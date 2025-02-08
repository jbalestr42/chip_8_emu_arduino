#pragma once

#include "IScreen.h"
#include "IInput.h"
#include "Roms.h"

namespace chip8
{

class Chip8
{
public:
	Chip8(IScreen &screen, IInput &input) :
		_screen(screen),
		_input(input),
		_pc(Chip8::ROM_START_ADDR),
		_I(0),
		_stackIndex(0),
		_cyclesPerFrame(12),
		_saveLoadIncrement(true),
		_vfReset(true),
		_clipping(true),
		_shifting(true),
		_displayWait(true),
		_drawThisFrame(false),
		_delayTimer(0),
		_soundTimer(0)
	{ }

	void setup()
	{
		_pc = Chip8::ROM_START_ADDR;
		_I = 0;
		_stackIndex = 0;
		_delayTimer = 0;
		_soundTimer = 0;
		_drawThisFrame = false;
		memset(_registers, 0, Chip8::MAX_REGISTER);
		memset(_data, 0, Chip8::MEMORY_SIZE);
		memset(_stack, 0, Chip8::STACK_SIZE);

		uint8_t fontData[] =
		{
			0xF0, 0x90, 0x90, 0x90, 0xF0,	// 0
			0x20, 0x60, 0x20, 0x20, 0x70,	// 1
			0xF0, 0x10, 0xF0, 0x80, 0xF0,	// 2
			0xF0, 0x10, 0xF0, 0x10, 0xF0,	// 3
			0x90, 0x90, 0xF0, 0x10, 0x10,	// 4
			0xF0, 0x80, 0xF0, 0x10, 0xF0,	// 5
			0xF0, 0x80, 0xF0, 0x90, 0xF0,	// 6
			0xF0, 0x10, 0x20, 0x40, 0x40,	// 7
			0xF0, 0x90, 0xF0, 0x90, 0xF0,	// 8
			0xF0, 0x90, 0xF0, 0x10, 0xF0,	// 9
			0xF0, 0x90, 0xF0, 0x90, 0x90,	// A
			0xE0, 0x90, 0xE0, 0x90, 0xE0,	// B
			0xF0, 0x80, 0x80, 0x80, 0xF0,	// C
			0xE0, 0x90, 0x90, 0x90, 0xE0,	// D
			0xF0, 0x80, 0xF0, 0x80, 0xF0,	// E
			0xF0, 0x80, 0xF0, 0x80, 0x80	// F
		};

		memcpy(&_data[Chip8::FONT_START_ADDRESS], &fontData[0], sizeof(fontData) / sizeof(uint8_t));
	}

	void loop()
	{
		const unsigned long frameDurationMs = 16; // Use to run at 60Hz

		unsigned long startTimer = millis();
		for (size_t i = 0; i < _cyclesPerFrame; i++)
		{
			processOpCode();

			// If "Display wait" option is enabled, we must draw only one sprite per frame
			if (_displayWait && _drawThisFrame)
			{
				break;
			}
		}

		// Wait to reach 60fps
		delay(max(frameDurationMs - (millis() - startTimer), 0ul));

		// Draw only when needed
		if (_drawThisFrame)
		{
			_screen.display();
			_drawThisFrame = false;
		}

		//if (_audioEnabled)
		{
			// Play audio before we update the timer
			if (isSoundTimerActive())
			{
				//_audio.playSound();
			}
			else
			{
				//_audio.stopSound();
			}
		}

		// Update timer once per frame
		updateTimers();
	}

	void processOpCode()
	{
		// Get the current opCode
		uint16_t opCode = (read8(_pc) << 8) | read8(_pc + 1);

		// Increment program counter
		_pc += 2;

		// Execute the instruction
		// We preprocess opCode for convenience, they are not always used
		// NNN: address
		uint16_t NNN = opCode & 0x0FFF;
		// NN: 8 bit constant
		uint8_t NN = opCode & 0x00FF;
		// N: 4 bit constant
		uint8_t N = opCode & 0x000F;
		// X and Y: 4 bit register identifier
		uint8_t X = (opCode & 0x0F00) >> 8;
		uint8_t Y = (opCode & 0x00F0) >> 4;

		switch (opCode >> 12 & 0x0F)
		{
		case 0x0:
		{
			if (NN == 0xE0)
			{
				// 00E0: Clears the screen
				_screen.clear();
			}
			else if (NN == 0XEE)
			{
				// 00EE: Returns from a subroutine
				// Set pc to the the last address from the stack
				if (_stackIndex > 0)
				{
					_pc = _stack[_stackIndex - 1];
					_stackIndex--;
				}
			}
			break;
		}
		case 0x1:
		{
			// 1NNN: Jumps to address NNN
			_pc = NNN;
			break;
		}
		case 0x2:
		{
			// 2NNN: Calls subroutine at NNN
			_stack[_stackIndex] = _pc;
			_stackIndex++;
			_pc = NNN;
			break;
		}
		case 0x3:
		{
			// 3XNN: Skips the next instruction if VX equals NN
			if (_registers[X] == NN)
			{
				_pc += 2;
			}
			break;
		}
		case 0x4:
		{
			// 4XNN: Skips the next instruction if VX does not equal NN
			if (_registers[X] != NN)
			{
				_pc += 2;
			}
			break;
		}
		case 0x5:
		{
			// 5XY0: Skips the next instruction if VX equals VY
			if (_registers[X] == _registers[Y])
			{
				_pc += 2;
			}
			break;
		}
		case 0x6:
		{
			// 6XNN: Sets VX to NN
			_registers[X] = NN;
			break;
		}
		case 0x7:
		{
			// 7XNN: Adds NN to VX
			_registers[X] += NN;
			break;
		}
		case 0x8:
		{
			switch (N)
			{
			case 0x0:
			{
				// 8XY0: Sets VX to the value of VY
				_registers[X] = _registers[Y];
				break;
			}
			case 0x1:
			{
				// 8XY1: Sets VX to VX or VY
				_registers[X] |= _registers[Y];
				if (_vfReset)
				{
					_registers[0xF] = 0;
				}
				break;
			}
			case 0x2:
			{
				// 8XY2: Sets VX to VX and VY
				_registers[X] &= _registers[Y];
				if (_vfReset)
				{
					_registers[0xF] = 0;
				}
				break;
			}
			case 0x3:
			{
				// BXY3: Sets VX to VX xor VY
				_registers[X] ^= _registers[Y];
				if (_vfReset)
				{
					_registers[0xF] = 0;
				}
				break;
			}
			case 0x4:
			{
				// 8XY4: Adds VY to VX.
				// VF is set to 1 when there's an overflow, and to 0 when there is not
				bool isOverflow = (_registers[X] + _registers[Y]) > 0xFF;
				_registers[X] = _registers[X] + _registers[Y];
				_registers[0xF] = isOverflow;
				break;
			}
			case 0x5:
			{
				// 8XY5: VY is subtracted from VX
				// VF is set to 0 when there's an underflow, and 1 when there is not
				bool isOverflow = _registers[X] >= _registers[Y];
				_registers[X] = _registers[X] - _registers[Y];
				_registers[0xF] = isOverflow;
				break;
			}
			case 0x6:
			{
				// 8XY6: Shifts VX to the right by 1
				// Stores the least significant bit of VX prior to the shift into VF
				bool isOverflow = _registers[X] & 0x01;
				if (_shifting)
				{
					_registers[X] = _registers[Y];
				}
				_registers[X] >>= 1;
				_registers[0xF] = isOverflow;
				break;
			}
			case 0x7:
			{
				// 8XY7: Sets VX to VY minus VX
				// VF is set to 0 when there's an underflow, and 1 when there is not
				bool isOverflow = _registers[Y] >= _registers[X];
				_registers[X] = _registers[Y] - _registers[X];
				_registers[0xF] = isOverflow;
				break;
			}
			case 0xE:
			{
				// 8XYE: Shifts VX to the left by 1
				// Sets VF to 1 if the most significant bit of VX prior to that shift was set, or to 0 if it was unset
				bool isOverflow = (_registers[X] & 0x80) >> 7;
				if (_shifting)
				{
					_registers[X] = _registers[Y];
				}
				_registers[X] <<= 1;
				_registers[0xF] = isOverflow;
				break;
			}
			default:
				break;
			}
			break;
		}
		case 0x9:
		{
			// 9XY0: Skips the next instruction if VX does not equal VY
			if (_registers[X] != _registers[Y])
			{
				_pc += 2;
			}
			break;
		}
		case 0xA:
		{
			// ANNN: Sets I to the address NNN
			_I = NNN;
			break;
		}
		case 0xB:
		{
			// BNNN: Jumps to the address NNN plus V0
			_pc = _registers[0] + NNN;
			break;
		}
		case 0xC:
		{
			// CXNN: Sets VX to the result of a bitwise and operation on a random number and NN
			_registers[X] = rand() & NN;
			break;
		}
		case 0xD:
		{
			// DXYN: Draws a sprite at coordinate (VX, VY) that has a width of 8 pixels and a height of N pixels.
			// Each row of 8 pixels is read as bit-coded starting from memory location I
			// I value does not change after the execution of this instruction.
			// As described above, VF is set to 1 if any screen pixels are flipped from set to unset when the sprite is drawn, and to 0 if that does not happen.

			uint8_t startX = _registers[X];
			uint8_t startY = _registers[Y];
			uint8_t height = N;

			_registers[0xF] = 0;

			for (uint8_t y = 0; y < height; y++)
			{
				uint8_t spriteY = read8(_I + y);

				// Sprite are always 8 pixels wide
				for (uint8_t x = 0; x < Chip8::SPRITE_WIDTH; x++)
				{
					// A sprite will be clipped if it’s partially drawn outside of display
					// but it will be wrapped around if all of the sprite is drawn outside of the display
					if (_clipping)
					{
						if ((startX < 64 && startX + x > 63) || (startY < 32 && startY + y > 31))
						{
							continue;
						}
					}

					uint8_t spritePixel = spriteY & (0x80 >> x);
					if (spritePixel)
					{
						uint8_t posX = (startX + x) % _screen.width();
						uint8_t posY = (startY + y) % _screen.height();
						bool isPixelOn = _screen.isPixelOn(posX, posY);

						// Pixel is colliding so we set the flag
						if (isPixelOn)
						{
							_registers[0xF] = 1;
						}

						// Flip the pixel color
						_screen.putPixel(posX, posY, !isPixelOn);
					}
				}
			}
			_drawThisFrame = true;
			break;
		}
		case 0xE:
		{
			if (NN == 0x9E)
			{
				// EX9E: Skips the next instruction if the key stored in VX is pressed
				if (_input.isKeyDown(_registers[X]))
				{
					_pc += 2;
				}
			}
			else if (NN == 0xA1)
			{
				// EXA1: Skips the next instruction if the key stored in VX is not pressed
				if (!_input.isKeyDown(_registers[X]))
				{
					_pc += 2;
				}
			}
			break;
		}
		case 0xF:
		{
			switch (NN)
			{
			case 0x07:
			{
				// FX07: Sets VX to the value of the delay timer
				_registers[X] = _delayTimer;
				break;
			}
			case 0x0A:
			{
				// FX0A: A key press is awaited, and then stored in VX (blocking operation, all instruction halted until next key event)
				bool isKeyPressed = false;
				for (uint8_t i = 0; i < Chip8::INPUT_COUNT; i++)
				{
					if (_input.isKeyReleased(i))
					{
						_registers[X] = i;
						isKeyPressed = true;
						break;
					}
				}

				if (!isKeyPressed)
				{
					_pc -= 2;
				}
				break;
			}
			case 0x15:
			{
				// FX15: Sets the delay timer to VX
				_delayTimer = _registers[X];
				break;
			}
			case 0x18:
			{
				// FX18: Sets the sound timer to VX
				_soundTimer = _registers[X];
				break;
			}
			case 0x1E:
			{
				// FX1E: Adds VX to I. VF is not affected
				_I += _registers[X];
				break;
			}
			case 0x29:
			{
				// FX29: Sets I to the location of the character in VX
				// Characters 0-F are represented by a 4x5 font
				_I = Chip8::FONT_START_ADDRESS + (_registers[X] * 5);
				break;
			}
			case 0x33:
			{
				// FX33: Stores the binary-coded decimal representation of VX in I:
				// - hundreds digit in memory at location in I,
				// - tens digit at location I+1
				// - ones digit at location I+2.
				write8(_I, (_registers[X] / 100) % 10);
				write8(_I + 1, (_registers[X] / 10) % 10);
				write8(_I + 2, _registers[X] % 10);
				break;
			}
			case 0x55:
			{
				// FX55: Stores from V0 to VX (including VX) in memory starting at address I
				// The offset from I is increased by 1 for each value written, but I itself is left unmodified
				for (uint8_t i = 0; i <= X; i++)
				{
					write8(_I + i, _registers[i]);
				}

				if (_saveLoadIncrement)
				{
					_I += X + 1;
				}
				break;
			}
			case 0x65:
			{
				// FX65: Fills from V0 to VX (including VX) with values from memory, starting at address I
				// The offset from I is increased by 1 for each value read, but I itself is left unmodified
				for (uint8_t i = 0; i <= X; i++)
				{
					_registers[i] = read8(_I + i);
				}

				if (_saveLoadIncrement)
				{
					_I += X + 1;
				}
				break;
			}
			default:
				break;
			}
			break;
		}
		default:
			break;
		}
	}

	void updateTimers()
	{
		// This timer is intended to be used for timing the events of games.
		// If the timer value is zero, it stays zero otherwise it will decrement.
		if (_delayTimer > 0)
		{
			_delayTimer--;
		}

		// This timer is used for sound effects. When its value is nonzero, a beeping sound is made.
		// If the timer value is zero, it stays zero otherwise it will decrement.
		if (_soundTimer > 0)
		{
			_soundTimer--;
		}
	}

	void loadRom(const Rom &rom)
	{
		_cyclesPerFrame = rom.cyclesPerFrame();
		_saveLoadIncrement = rom.saveLoadIncrement();
		_vfReset = rom.vfReset();
		_clipping = rom.clipping();
		_shifting = rom.shifting();
		_displayWait = rom.displayWait();
		memcpy(&_data[Chip8::ROM_START_ADDR], rom.data(), rom.size());
	}
	
	bool isSoundTimerActive() const { return _soundTimer > 0; }

	uint8_t read8(uint16_t addr) const { return _data[addr]; }
	void write8(uint16_t addr, uint8_t value) { _data[addr] = value; }

	static const size_t MAX_REGISTER = 16;
	static const uint16_t MEMORY_SIZE = 4096;
	static const uint8_t STACK_SIZE = 16;
	static const uint16_t FONT_START_ADDRESS = 0x050;
	static const uint16_t ROM_START_ADDR = 0x200;
	static const uint8_t SPRITE_WIDTH = 8;
	static const uint8_t INPUT_COUNT = 16;

private:
	IScreen &_screen;
	IInput &_input;
	uint16_t _pc;
	uint16_t _I;
	uint8_t _registers[MAX_REGISTER];
	uint8_t _data[MEMORY_SIZE];
	uint16_t _stack[STACK_SIZE];
	uint8_t _stackIndex;
	size_t _cyclesPerFrame;
	bool _saveLoadIncrement;
	bool _vfReset;
	bool _clipping;
	bool _shifting;
	bool _displayWait;
	bool _drawThisFrame;

	uint8_t _delayTimer;
	uint8_t _soundTimer;
};

}
