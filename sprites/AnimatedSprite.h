#pragma once

class AnimatedSprite
{
public:
	AnimatedSprite(size_t width, size_t height, size_t frameCount, size_t frameSize, float framePerSecond, const unsigned char *data) :
		_width(width),
		_height(height),
		_frameCount(frameCount),
		_frameSize(frameSize),
		_frameDuration(1.f / framePerSecond * 1000.f),
		_data(data),
		_prevTime(millis()),
		_currentIndex(0),
		_isPlaying(true)
	{}

	void loop()
	{
		if (_isPlaying)
		{
			float currentTime = millis();
			if ((currentTime - _prevTime) >= _frameDuration)
			{
				_currentIndex = (_currentIndex + 1) % _frameCount;
				_prevTime = currentTime;
			}
		}
	}

	void draw(Adafruit_SSD1306 &screen, size_t posX, size_t posY, uint8_t color)
	{
		screen.drawBitmap(posX, posY, &_data[_currentIndex * _frameSize], _width, _height, color);
	}

	void play() { _isPlaying = true; }

	void pause() { _isPlaying = false; }

	void reset() { _currentIndex = 0; }

	size_t width() const { return _width; }
	size_t height() const { return _height; }
	size_t frameCount() const { return _frameCount; }
	size_t frameSize() const { return _frameSize; }

private:
	size_t _width;
	size_t _height;
	size_t _frameCount;
	size_t _frameSize;
	float _frameDuration;
	const unsigned char *_data;

	float _prevTime;
	size_t _currentIndex;
	bool _isPlaying;
};
