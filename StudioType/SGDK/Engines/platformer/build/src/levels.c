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
#include "../res/images.h"

u16 levelNum = 0;
Level* curLvlData;
EntityAll* curEntityAll;
MessagePack* curMessagePacks;

u16 palette_full[64];

//$levelFullArr$

Vect2D_s16 getLevelPos(u16 posNum) {
	return LevelFull_arr[levelNum].lvl->posArr[posNum];
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

