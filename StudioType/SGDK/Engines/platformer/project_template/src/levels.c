#include "../inc/levels.h"

#include "../inc/maps.h"
#include "../inc/global.h"
#include "../inc/player.h"

#include "../res/resources.h"
#include "../res/gfx.h"
#include "../res/sprites.h"


u16 levelNum = 0;
Level* curLvlData;
EntityAll* curEntityAll;

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

	//Deallocate prev entityData to avoid memory leak
	MEM_free(curEntityAll->EntityMerged_arr);
	MEM_free(curEntityAll->Trigger_arr);
	MEM_free(curEntityAll);

	MEM_free(bga);
	MEM_free(bgb);

	VDPTilesFilled = TILE_USER_INDEX;
	
	//playerBody.globalPosition = getLevelStartPos();
	playerInit(); //janky solution, but who cares if it works... Me.. I will fix that later.
	curLvlData = LevelFull_arr[levelNum].lvl;

	//duplicate entityAll
	curEntityAll = MEM_alloc(sizeof(EntityAll));
	memcpy(curEntityAll, LevelFull_arr[levelNum].entityAll_arr, sizeof(EntityAll));

	//duplicate entityMerged_arr for entityAll
	curEntityAll->EntityMerged_arr = MEM_alloc(sizeof(EntityMerged)*LevelFull_arr[levelNum].entityAll_arr->EntityMerged_size);
	memcpy(curEntityAll->EntityMerged_arr, LevelFull_arr[levelNum].entityAll_arr->EntityMerged_arr, sizeof(EntityMerged)*LevelFull_arr[levelNum].entityAll_arr->EntityMerged_size);

	//duplicate Trigger_arr for entityAll
	curEntityAll->Trigger_arr = MEM_alloc(sizeof(Trigger)*LevelFull_arr[levelNum].entityAll_arr->Trigger_size);
	memcpy(curEntityAll->Trigger_arr, LevelFull_arr[levelNum].entityAll_arr->Trigger_arr, sizeof(Trigger)*LevelFull_arr[levelNum].entityAll_arr->Trigger_size);

	//update trigger reference in entityMerged_arr
	for(u16 i=0 ; i < curEntityAll->EntityMerged_size; i++){
		curEntityAll->EntityMerged_arr[i].trigger =  &curEntityAll->Trigger_arr[curEntityAll->EntityMerged_arr[i].triggerInd];
	}
	
	
	if(curLvlData->foregroundTileset != NULL){
		PAL_setPalette(PAL1, curLvlData->foregroundPallete->data, DMA);
		VDP_loadTileSet(curLvlData->foregroundTileset, VDPTilesFilled, DMA);
		bga = MAP_create(curLvlData->foregroundMap, BG_A, TILE_ATTR_FULL(PAL1, FALSE, FALSE, FALSE, VDPTilesFilled));
		VDPTilesFilled += curLvlData->foregroundTileset->numTile;
	} else {
		bga = NULL;
	}
	
	if(curLvlData->backgroundTileset != NULL){
		PAL_setPalette(PAL0, curLvlData->backgroundPallete->data, DMA);
		VDP_loadTileSet(curLvlData->backgroundTileset, VDPTilesFilled, DMA);
		bgb = MAP_create(curLvlData->backgroundMap, BG_B, TILE_ATTR_FULL(PAL0, FALSE, FALSE, FALSE, VDPTilesFilled));
		VDPTilesFilled += curLvlData->backgroundTileset->numTile;
	} else {
		bgb = NULL;
	}
	

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

