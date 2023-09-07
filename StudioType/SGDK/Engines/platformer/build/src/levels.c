#include "../inc/levels.h"

#include "../inc/maps.h"
#include "../inc/enums.h"
#include "../inc/global.h"
#include "../inc/player.h"
#include "../inc/customScripts.h"

#include "../res/resources.h"
#include "../res/gfx.h"
#include "../res/sprites.h"
#include "../res/sounds.h"
#include "../res/music.h"
#include "../res/palette.h"
#include "../res/images.h"

u16 levelNum = 0;
Level* curLvlData;
EntityAll* curEntityAll;
MessagePack* curMessagePacks;
LocalVariableMerged* curLocalVariables;
u16** curSprTileIndexes[20];
PreloadSprDefs* curPreloadSprDefs;

u16 palette_full[64];

//$levelFullArr$

Position* getLevelPos(u16 posNum) {
	return &(LevelFull_arr[levelNum].lvl->posArr[posNum]);
}

Position* getLevelPosById(u16 posInstId) {
	//Finding array index by instId
	for(u16 i=0; i<LevelFull_arr[levelNum].lvl->posAmount; i++){
		if(LevelFull_arr[levelNum].lvl->posArr[i].instId == posInstId){
			return &(LevelFull_arr[levelNum].lvl->posArr[i]);
		}
	}
	
}

Vect2D_s32 getLevelPosCoords(u16 posNum) {
	//KLog_S1("test: ", LevelFull_arr[levelNum].lvl->posArr[posNum].pos.y);
	return LevelFull_arr[levelNum].lvl->posArr[posNum].pos;
}

void updateRoomSize(Level* lvl){
	roomSize = newAABB(0, lvl->sizeinPx.x, 0, lvl->sizeinPx.y);
}

u16 getTileValue(s16 x, s16 y) {
	if (x >= curLvlData->sizeinTiles.x || x < 0 || y < 0 || y >= curLvlData->sizeinTiles.y)
		return 0;

	//If the position is inside the collision map return the value of the tile from it
	return curLvlData->collArr[x+(y*curLvlData->sizeinTiles.x)];
}

