#pragma once

#include "app/IApp.h"
#include "Menu.h"

class ScrollingMenuApp : public IApp
{
public:
	ScrollingMenuApp(Menu::Item *items, size_t size, Adafruit_SSD1306 &screen, NesController &controller, bool allowCircular, size_t visibleItemCount, size_t focusedItemHeight, size_t margin = 0) :
		_menu(items, size),
		_screen(screen),
		_controller(controller),
		_allowCircular(allowCircular),
		_focusedItemHeight(focusedItemHeight),
		_margin(margin)
	{
		_visibleItemCount = visibleItemCount % 2 == 0 ? visibleItemCount + 1 : visibleItemCount;
		_itemHeight = (_screen.height() - _margin * 2) / _visibleItemCount;
	}

	void loop()
	{
		bool shouldDraw = true;
		Menu::Item &item = _menu.current();

		if (_controller.isButtonPressed(NesController::A))
		{
			_menu.execute();
			shouldDraw = false;
		}
		else if (_controller.isButtonPressed(NesController::UP))
		{
			if (_allowCircular || _menu.currentIndex() > 0)
			{
				if (item.hasSprite())
				{
					_menu.current().sprite()->pause();
					_menu.current().sprite()->reset();
				}
				
				_menu.prev();
				
				if (item.hasSprite())
				{
					_menu.current().sprite()->play();
				}
			}
		}
		else if (_controller.isButtonPressed(NesController::DOWN))
		{
			if (_allowCircular || _menu.currentIndex() < _menu.count() - 1)
			{
				if (item.hasSprite())
				{
					_menu.current().sprite()->pause();
					_menu.current().sprite()->reset();
				}

				_menu.next();
				
				if (item.hasSprite())
				{
					_menu.current().sprite()->play();
				}
			}
		}

		if (item.hasSprite())
		{
			_menu.current().sprite()->loop();
		}

		if (shouldDraw)
		{
			draw();
		}
	}

	void draw()
	{
		_screen.setTextColor(WHITE);
		_screen.setTextSize(1);

		int focusedItemPosY = (_screen.height() / 2) - _focusedItemHeight / 2;
		_screen.drawRoundRect(0, focusedItemPosY, 128, _focusedItemHeight, 3, WHITE);

		for (byte i = 0; i < _visibleItemCount; i++)
		{
			int menuIndex = _menu.currentIndex() - _visibleItemCount / 2 + i;
			if (_allowCircular)
			{
				menuIndex = (menuIndex + _menu.count()) % _menu.count();
			}
			if (menuIndex >= 0 && menuIndex < _menu.count())
			{
				Menu::Item &item = _menu.item(menuIndex);
				
				int itemPosX = _menu.currentIndex() == menuIndex ? 16 : 8;
				if (item.hasSprite())
				{
					item.sprite()->draw(_screen, itemPosX, _margin + i * _itemHeight + _itemHeight / 2 - item.sprite()->height() / 2 + 1, WHITE);
					itemPosX += item.sprite()->width() + 4;
				}

				_screen.setCursor(itemPosX, _margin + i * _itemHeight + _itemHeight / 2 - 2); // small offset based on character height (which is based on the font)
				_screen.print(item.name());
			}
		}
	}

private:
	Menu _menu;
	Adafruit_SSD1306 &_screen;
	NesController &_controller;
	bool _allowCircular;
	size_t _visibleItemCount;
	size_t _itemHeight;
	size_t _focusedItemHeight;
	size_t _margin;

};
