#include "../inc/levels.h"

#include "../inc/maps.h"
#include "../inc/global.h"

#include "../res/resources.h"
#include "../res/gfx.h"

u16 levelNum = 0;
Level* curLvlData;
EntityAll* curEntityAll;

const EntityMerged const EntityMerged_arr_Level_1[] = {{0, TRUE, {100, 100}, {FIX32(100), FIX32(100)}, {0, 0}, {32, 32}, FALSE, NULL,NULL, }, };
const Trigger const Trigger_arr_Level_1[] = { {TRUE, {0,0}, {0,0}, 0, 0, 1, },};
const EntityAll const EntityAll_arr_Level_1[] = {1, &EntityMerged_arr_Level_1, 1, &Trigger_arr_Level_1};
//495
const LevelFull const LevelFull_arr[] = {{{&Level_1_bga_map, NULL, &Level_1_bga_tileset, NULL, &Level_1_bga_pal, NULL, {115, 122}, collisionMap, {320, 224}, {20, 14}}, &EntityAll_arr_Level_1 }};


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

	curEntityAll = MEM_alloc(sizeof(EntityAll));
	memcpy(curEntityAll, LevelFull_arr[levelNum].entityAll_arr, sizeof(EntityAll));

	curEntityAll->EntityMerged_arr = MEM_alloc(sizeof(EntityMerged)*LevelFull_arr[levelNum].entityAll_arr->EntityMerged_size);
	memcpy(curEntityAll->EntityMerged_arr, LevelFull_arr[levelNum].entityAll_arr->EntityMerged_arr, sizeof(EntityMerged)*LevelFull_arr[levelNum].entityAll_arr->EntityMerged_size);

	
	//LevelFull_arr[levelNum].entityAll_arr->EntityMerged_size
	//KLog_S1("test1: ", curEntityMerged[0].posInt.x);


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

