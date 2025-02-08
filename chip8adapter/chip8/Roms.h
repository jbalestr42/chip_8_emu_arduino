#pragma once

namespace chip8
{

class Rom
{
public:
	Rom(const char *name, const uint8_t *data, size_t size, size_t cyclesPerFrame, bool saveLoadIncrement, bool vfReset, bool clipping, bool shifting, bool displayWait) :
		_name(name),
		_data(data),
		_size(size),
		_cyclesPerFrame(cyclesPerFrame),
		_saveLoadIncrement(saveLoadIncrement),
		_vfReset(vfReset),
		_clipping(clipping),
		_shifting(shifting),
		_displayWait(displayWait)
	{ }

	const char *name() const { return _name; }
	const uint8_t *data() const { return _data; }
	const size_t size() const { return _size; }
	const size_t cyclesPerFrame() const { return _cyclesPerFrame; }
	bool saveLoadIncrement() const { return _saveLoadIncrement; }
	bool vfReset() const { return _vfReset; }
	bool clipping() const { return _clipping; }
	bool shifting() const { return _shifting; }
	bool displayWait() const { return _displayWait; }

private:
	const char *_name;
	const uint8_t *_data;
	const size_t _size;
	const size_t _cyclesPerFrame;
	bool _saveLoadIncrement;
	bool _vfReset;
	bool _clipping;
	bool _shifting;
	bool _displayWait;
};

}
