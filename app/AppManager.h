#pragma once

#include "IApp.h"

class AppManager
{
public:
	void push(IApp *app)
	{
		// Disable current app
		if (_appStackCount > 0)
		{
			_appStack[_appStackCount - 1]->onAppDisabled();
		}

		// Enable new app
		app->onAppEnabled();
		_appStack[_appStackCount] = app;
		_appStackCount++;
	}

	IApp *pop()
	{
		if (_appStackCount > 1)
		{
			_appStackCount--;
			IApp *app = _appStack[_appStackCount];
			_appStack[_appStackCount] = nullptr;

			// Disable removed app
			app->onAppDisabled();

			// Enable previous app
			_appStack[_appStackCount - 1]->onAppEnabled();
			return app;
		}
		return nullptr;
	}

	IApp *current()
	{
		return _appStack[_appStackCount - 1];
	}

public:
	IApp *_appStack[16];
	size_t _appStackCount;
};
