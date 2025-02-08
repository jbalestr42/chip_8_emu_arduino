 /* *NES Controller*		     
  *
  * GND: Brown
  * Clock: Red
  * VCC (5v): White
  * Latch: Orange
  * Data: Yellow
  *
  * NES protocol ~20ms between each data cycle
  *           _
  * Latch - _| |___________________________________________________
  *         ______   ___   ___   ___   ___   ___   ___   ___   ____
  * Clock -       |_|   |_|   |_|   |_|   |_|   |_|   |_|   |_|
  *            _____________________________________________
  *  Data - __|_A_|_|_B_|_|Sel|_|Sta|_|Up_|_|Dow|_|Lef|_|Rig|______
  *  
  *  Note1: Last clock does nothing
  *  Note2: After Latch goes HIGH, Data goes HIGH and becomes active low (if a button is pushed after clock signal, data wire reads low)
  */
#pragma once

class NesController
{
public:
	enum InputState
	{
		None,
		Pressed,
		Down,
		Released,
	};

	enum Button
	{
		A = 0,
		B = 1,
		SELECT = 2,
		START = 3,
		UP = 4,
		DOWN = 5,
		LEFT = 6,
		RIGHT = 7,
	};

	NesController(int latch, int clock, int data) :
		_latch(latch),
		_clock(clock),
		_data(data)
	{
		for (int i = 0; i < INPUT_COUNT; i++)
		{
			_inputs[i] = InputState::None;
		}
	}

	void setup()
	{
		pinMode(_latch, OUTPUT);
		pinMode(_clock, OUTPUT);
		pinMode(_data, INPUT);
	}

	void loop()
	{
		digitalWrite(_latch, LOW);
		digitalWrite(_clock, LOW);
		digitalWrite(_latch, HIGH);
		delayMicroseconds(12);
		digitalWrite(_latch, LOW);
	
		for (int i = 0; i < 8; i++)
		{
			int data = digitalRead(_data) * (1 << i);

			if (data == 0) // Pressed
			{
				_inputs[i] = _inputs[i] == None ? Pressed : Down;
			}
			else // Released
			{
				_inputs[i] = _inputs[i] == Pressed || _inputs[i] == Down ? Released : None;
			}

			digitalWrite(_clock, HIGH);
			delayMicroseconds(12);
			digitalWrite(_clock, LOW);
			delayMicroseconds(12);
		}
	}

	InputState getButtonState(Button button) const
	{
		return _inputs[(int)button];
	}

	bool isButtonPressed(Button button) const
	{
		return getButtonState(button) == Pressed;
	}

	bool isButtonDown(Button button) const
	{
		return getButtonState(button) == Down;
	}

	bool isButtonReleased(Button button) const
	{
		return getButtonState(button) == Released;
	}

	static const int INPUT_COUNT = 8;

private:
	InputState _inputs[INPUT_COUNT];
	int _latch;
	int _clock;
	int _data;

};
