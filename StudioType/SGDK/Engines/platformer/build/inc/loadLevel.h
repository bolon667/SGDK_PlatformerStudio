#pragma once

#include <genesis.h>

#include "global.h"
#include "types.h"

extern void stopLevel();
extern void reloadLevelSprites();
extern void resumeLevel();
extern void loadLevel(u16 levelNum, Vect2D_s16 startPos);