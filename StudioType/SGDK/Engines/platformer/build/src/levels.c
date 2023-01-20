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

//TODO remake entityAll code
// const EntityMerged const EntityMerged_arr_Level_1[] = {{0, TRUE, {100, 100}, {FIX32(100), FIX32(100)}, {0, 0}, {32, 32}, FALSE, NULL,NULL, }, };
// const Trigger const Trigger_arr_Level_1[] = { {TRUE, {0,0}, {0,0}, 0, 0, 1, },};
// const EntityAll const EntityAll_arr_Level_1[] = {1, &EntityMerged_arr_Level_1, 1, &Trigger_arr_Level_1};
// const LevelFull const LevelFull_arr[] = {{{&Level_1_bga_map, NULL, &Level_1_bga_tileset, NULL, &Level_1_bga_pal, NULL, {115, 122}, collisionMap, {320, 224}, {20, 14}}, &EntityAll_arr_Level_1 }};

const Trigger const Trigger_arr_Level_0[] = {{TRUE, {183, 69}, {0, 0, 50, 70}, 0, 1, 1, }, {TRUE, {93, 95}, {0, 0, 8, 8}, 0, 0, 1, }, };
const EntityMerged const EntityMerged_arr_Level_0[] = {{0, TRUE, {183, 69}, {FIX32(183), FIX32(69)}, {0, 0}, {32, 32}, FALSE, &Trigger_arr_Level_0[0],0,NULL,&spr_enemy01, }, {0, TRUE, {93, 95}, {FIX32(93), FIX32(95)}, {0, 0}, {32, 32}, FALSE, &Trigger_arr_Level_0[1],1,NULL,&spr_enemy02, }, };
const EntityAll const EntityAll_arr_Level_0[] = {2, &EntityMerged_arr_Level_0, 2, &Trigger_arr_Level_0, };
const Trigger const Trigger_arr_Level_1[] = {{TRUE, {345, 627}, {0, 0, 30, 60}, 0, 2, 1, }, };
const EntityMerged const EntityMerged_arr_Level_1[] = {{0, TRUE, {345, 627}, {FIX32(345), FIX32(627)}, {0, 0}, {32, 32}, FALSE, &Trigger_arr_Level_1[0],0,NULL,&spr_enemy02, }, };
const EntityAll const EntityAll_arr_Level_1[] = {1, &EntityMerged_arr_Level_1, 1, &Trigger_arr_Level_1, };
const Trigger const Trigger_arr_Level_2[] = {{TRUE, {145, 79}, {0, 0, 60, 60}, 0, 0, 1, }, };
const EntityMerged const EntityMerged_arr_Level_2[] = {{0, TRUE, {145, 79}, {FIX32(145), FIX32(79)}, {0, 0}, {32, 32}, FALSE, &Trigger_arr_Level_2[0],0,NULL,&spr_noSpr, }, };
const EntityAll const EntityAll_arr_Level_2[] = {1, &EntityMerged_arr_Level_2, 1, &Trigger_arr_Level_2, };
const Trigger const Trigger_arr_Level_3[] = {};
const EntityMerged const EntityMerged_arr_Level_3[] = {};
const EntityAll const EntityAll_arr_Level_3[] = {0, &EntityMerged_arr_Level_3, 0, &Trigger_arr_Level_3, };
const LevelFull const LevelFull_arr[] = {{{&Level_1_bga_map, NULL, &Level_1_bga_tileset, NULL, &Level_1_bga_pal, NULL, {55, 53}, collisionMap, {320, 224}, {20, 14}},&EntityAll_arr_Level_0}, {{&Level_2_bga_map, NULL, &Level_2_bga_tileset, NULL, &Level_2_bga_pal, NULL, {55, 53}, collisionMap1, {768, 768}, {48, 48}},&EntityAll_arr_Level_1}, {{&Level_3_bga_map, NULL, &Level_3_bga_tileset, NULL, &Level_3_bga_pal, NULL, {55, 53}, collisionMap2, {320, 224}, {20, 14}},&EntityAll_arr_Level_2}, {{&Level_4_bga_map, NULL, &Level_4_bga_tileset, NULL, &Level_4_bga_pal, NULL, {55, 53}, collisionMap3, {320, 224}, {20, 14}},&EntityAll_arr_Level_3}, };




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
	VDPTilesFilled = TILE_USER_INDEX;
	if(bga) MEM_free(bga);
	//if(bgb) MEM_free(bgb);
	
	//playerBody.globalPosition = getLevelStartPos();
	playerInit(); //janky solution, but who cares if it works... Me.. I will fix that later.
	curLvlData = &LevelFull_arr[levelNum].lvl;

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
		KLog_S1("test: ", &curEntityAll->Trigger_arr[curEntityAll->EntityMerged_arr[i].triggerInd]);
		curEntityAll->EntityMerged_arr[i].trigger =  &curEntityAll->Trigger_arr[curEntityAll->EntityMerged_arr[i].triggerInd];
	}
	
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

