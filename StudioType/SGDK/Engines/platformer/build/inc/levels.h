#pragma once

#include <genesis.h>

#include "global.h"
#include "types.h"



extern const LevelFull const LevelFull_arr[];
extern u16 levelNum;

extern Level* curLvlData;
extern EntityAll* curEntityAll;
extern MessagePack* curMessagePacks;
extern LocalVariableMerged* curLocalVariables;
extern u16** curSprTileIndexes[20];
extern PreloadSprDefs* curPreloadSprDefs;

extern u16 palette_full[];

void updateRoomSize(Level* lvl);

u16 getTileValue(s16 x, s16 y);

extern Position* getLevelPos(u16 posNum);
extern Position* getLevelPosById(u16 posInstId);

extern Vect2D_s32 getLevelPosCoords(u16 posNum);
