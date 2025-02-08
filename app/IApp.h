#pragma once

class IApp
{
public:
	virtual void loop() = 0;

	virtual void onAppEnabled() {};
	virtual void onAppDisabled() {};
};
