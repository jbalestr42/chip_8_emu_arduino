#pragma once

#include "AnimatedSprite.h"
#include "data/system_icon.h"
#include "data/chip8_icon.h"

AnimatedSprite systemIcon(21, 18, 30, sizeof(animated_sprite_system) / 30, 48, animated_sprite_system);
AnimatedSprite chip8Icon(21, 18, 32, sizeof(animated_sprite_chip8) / 32, 12, animated_sprite_chip8);
