#pragma once

#include <Adafruit_SSD1306.h>
#include "NesController.h"
#include "app/AppManager.h"

class MainApplication
{
public:
	MainApplication() :
		_screen(128, 64, &Wire, -1),
		_controller(8, 6, 7),
		_appManager(),
		_enableGlobalScreenManagement(true)
	{}

	void setup()
	{
		_screen.begin(SSD1306_SWITCHCAPVCC, 0x3C);
		_controller.setup();
	}

	void loop()
	{
		if (_enableGlobalScreenManagement)
		{
			_screen.clearDisplay();
		}

		_controller.loop();
		_appManager.current()->loop();

		if (_controller.isButtonPressed(NesController::SELECT))
		{
			_appManager.pop();
		}

		if (_enableGlobalScreenManagement)
		{
			_screen.display();
		}
	}

	Adafruit_SSD1306 &screen() { return _screen; }
	NesController &controller() { return _controller; }
	AppManager &appManager() { return _appManager; }

	bool enableGlobalScreenManagement() const { return _enableGlobalScreenManagement; }
	void setEnableGlobalScreenManagement(bool enableGlobalScreenManagement) { _enableGlobalScreenManagement = enableGlobalScreenManagement; }

private:
	Adafruit_SSD1306 _screen;
	NesController _controller;
	AppManager _appManager;
	bool _enableGlobalScreenManagement;
};
