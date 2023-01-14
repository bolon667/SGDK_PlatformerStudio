#pragma once

#include <genesis.h>

#include "player.h"
#include "global.h"
#include "types.h"

u16 getTileValue(s16 x, s16 y);

void freeCollisionMap();
void updateRoomSize(Level* lvl);
void generateCollisionMap(Level* lvl);