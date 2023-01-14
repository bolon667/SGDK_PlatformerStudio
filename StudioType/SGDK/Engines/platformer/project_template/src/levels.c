#include "../inc/levels.h"

#include "../inc/levelgenerator.h"
#include "../inc/maps.h"
#include "../inc/global.h"

#include "../res/resources.h"
#include "../res/gfx.h"

u16 levelNum = 0;
Level* curLvlData;

//$levelFullArr$

Vect2D_s16 getLevelStartPos() {
	return curLvlData->startPos;
}

void updateRoomSize(Level* lvl){
	roomSize = newAABB(0, lvl->sizeinPx.x, 0, lvl->sizeinPx.y);
}

void loadLevel(u16 levelNum) {
	//Reset everything
	VDP_resetScreen();
	VDP_setPlaneSize(64,32,TRUE);
	VDP_setScrollingMode(HSCROLL_PLANE, VSCROLL_PLANE);
	SPR_reset();
	if(bga) MEM_free(bga);
	//if(bgb) MEM_free(bgb);
	
	curLvlData = &LevelFull_arr[levelNum].lvl;
	PAL_setPalette(PAL0, curLvlData->foregroundPallete->data, DMA);
	VDP_loadTileSet(curLvlData->foregroundTileset, VDPTilesFilled, DMA);
	bga = MAP_create(curLvlData->foregroundMap, BG_A, TILE_ATTR_FULL(PAL0, FALSE, FALSE, FALSE, VDPTilesFilled));

	//Update the number of tiles filled in order to avoid overlaping them when loading more
	VDPTilesFilled += level_tileset.numTile;

	updateRoomSize(curLvlData);

	//Start play the level's song
	XGM_startPlay(song);
}


u16 getTileValue(s16 x, s16 y) {
	if (x >= curLvlData->sizeinTiles.x || x < 0 || y < 0 || y >= curLvlData->sizeinTiles.y)
		return 0;

	//If the position is inside the collision map return the value of the tile from it
	return curLvlData->collArr[x+(y*curLvlData->sizeinTiles.x)];
}

