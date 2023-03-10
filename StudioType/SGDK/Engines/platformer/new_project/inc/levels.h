#pragma once

#include <genesis.h>

#include "global.h"
#include "types.h"


extern const LevelFull const LevelFull_arr[];
extern u16 levelNum;

extern Level* curLvlData;
extern EntityAll* curEntityAll;

void updateRoomSize(Level* lvl);
void loadLevel(u16 levelNum, u16 positionNum);

u16 getTileValue(s16 x, s16 y);

Vect2D_s16 getLevelPos(u16 posNum);
