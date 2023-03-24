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

const Vect2D_s16 const Position_arr_Level_0[] = {{0, 0}, };
const Trigger const Trigger_arr_Level_0[] = {{TRUE, {206, 101}, {0, 0, 10, 100}, TRIGGER_TYPE_showMessage, 0, 0, 0, 1, TRUE, TRUE, }, {TRUE, {399, 136}, {0, 0, 1, 136}, TRIGGER_TYPE_changeLevelv2, 1, 1, 136, 1, TRUE, TRUE, }, };
const EntityMerged const EntityMerged_arr_Level_0[] = {{0, TRUE, {206, 101}, {FIX32(206), FIX32(101)}, {0, 0}, {32, 32}, FALSE, &Trigger_arr_Level_0[0],0,NULL,NULL, NULL, NULL, 1, FALSE, FALSE, }, {0, TRUE, {399, 136}, {FIX32(399), FIX32(136)}, {0, 0}, {1, 136}, FALSE, &Trigger_arr_Level_0[1],1,NULL,NULL, NULL, NULL, 1, FALSE, FALSE, }, };
const EntityAll const EntityAll_arr_Level_0[] = {10, 10, NULL, 2, &EntityMerged_arr_Level_0, 2, &Trigger_arr_Level_0, };
const Message const msgArr_Level_0_msgPack_0[] = {{"Here a gates example", 20}, };
const MessagePack const MessagePack_Level_0[] = {{msgArr_Level_0_msgPack_0, 1, "0"}, };
const Level const lvl_Level_0 = {NULL, &map_Level_0, NULL, &tileset_Level_0, NULL, &pal_Level_0, NULL, NULL, &Position_arr_Level_0, 1, collisionMap, {400, 272}, {25, 17}, NULL, NULL, emptyScript, NULL};
const Vect2D_s16 const Position_arr_Level_1[] = {{0, 0}, };
const Trigger const Trigger_arr_Level_1[] = {{TRUE, {0, 0}, {0, 0, 1, 136}, TRIGGER_TYPE_changeLevelv2, 0, 0, -136, 1, TRUE, TRUE, }, };
const EntityMerged const EntityMerged_arr_Level_1[] = {{0, TRUE, {0, 0}, {FIX32(0), FIX32(0)}, {0, 0}, {1, 136}, FALSE, &Trigger_arr_Level_1[0],0,NULL,NULL, NULL, NULL, 1, FALSE, FALSE, }, };
const EntityAll const EntityAll_arr_Level_1[] = {10, 10, NULL, 1, &EntityMerged_arr_Level_1, 1, &Trigger_arr_Level_1, };
const MessagePack const MessagePack_Level_1[] = {};
const Level const lvl_Level_1 = {NULL, &map_Level_21, NULL, &tileset_Level_21, NULL, &pal_Level_21, NULL, NULL, &Position_arr_Level_1, 1, collisionMap1, {592, 368}, {37, 23}, NULL, NULL, emptyScript, NULL};
const LevelFull const LevelFull_arr[] = {{&lvl_Level_0, &EntityAll_arr_Level_0, &MessagePack_Level_0}, {&lvl_Level_1, &EntityAll_arr_Level_1, &MessagePack_Level_1}, };


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

