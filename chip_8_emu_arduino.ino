#include "MainApplication.h"
#include "Chip8App.h"
#include "ScrollingMenuApp.h"
#include "chip8adapter/roms.h"
#include "sprites/sprites.h"

// Setup menu and applications here so we don't have to do dynamic allocations
MainApplication mainApplication;

//////////////////////////// Chip8 app
Chip8App chip8App(mainApplication);

//////////////////////////// Chip8 rom menu
void startChip8Rom(size_t index) { mainApplication.appManager().push(&chip8App); chip8App.setRomConfig(romConfigs[index]); }

Menu::Item chip8RomMenuItems[] = {
	Menu::Item(romConfigs[0].rom().name(), [](){ startChip8Rom(0); }),
	Menu::Item(romConfigs[1].rom().name(), [](){ startChip8Rom(1); }),
	Menu::Item(romConfigs[2].rom().name(), [](){ startChip8Rom(2); }),
	Menu::Item(romConfigs[3].rom().name(), [](){ startChip8Rom(3); }),
	Menu::Item(romConfigs[4].rom().name(), [](){ startChip8Rom(4); })
};

ScrollingMenuApp chip8RomMenuApp(chip8RomMenuItems, sizeof(chip8RomMenuItems) / sizeof(Menu::Item), mainApplication.screen(), mainApplication.controller(), true, 5, 11, 4);

//////////////////////////// Main menu
Menu::Item mainMenuItems[] = {
	Menu::Item("Chip 8", [](){ mainApplication.appManager().push(&chip8RomMenuApp); }, &chip8Icon)
};

ScrollingMenuApp mainMenuApp(mainMenuItems, sizeof(mainMenuItems) / sizeof(Menu::Item), mainApplication.screen(), mainApplication.controller(), false, 3, 22);

void setup()
{
	Serial.begin(9600);
	
	mainApplication.setup();

	// First application is the main menu
	mainApplication.appManager().push(&mainMenuApp);
}

void loop()
{
	mainApplication.loop();
}
