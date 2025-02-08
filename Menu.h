#pragma once

#include "sprites/AnimatedSprite.h"

class Menu
{
public:
	class Item
	{
	public:
		typedef void (*MenuAction)(void);

		Item(const char *name, MenuAction action, AnimatedSprite *sprite = nullptr) :
			_name(name),
			_action(action),
			_sprite(sprite)
		{ }

		void execute() { _action(); }

		const char *name() const { return _name; }
		AnimatedSprite *sprite() { return _sprite; }
		bool hasSprite() const { return _sprite != nullptr; }

	private:
		const char *_name;
		MenuAction _action;
		AnimatedSprite *_sprite;
	};

	Menu(Menu::Item *items, int count) :
		_items(items),
		_count(count),
		_index(0)
	{ }

	void next()
	{
		_index = (_index + 1) % _count;
	}

	void prev()
	{
		_index = _index == 0 ? _count - 1 : _index - 1;
	}

	void execute()
	{
		_items[_index].execute();
	}

	int count() const { return _count; }
	const Menu::Item& item(int index) const { return _items[index]; }
	Menu::Item& item(int index) { return _items[index]; }
	const Menu::Item& current() const { return _items[_index]; }
	Menu::Item& current() { return _items[_index]; }
	int currentIndex() const { return _index; }

private:
	Item *_items;
	int _count;
	int _index;

};
