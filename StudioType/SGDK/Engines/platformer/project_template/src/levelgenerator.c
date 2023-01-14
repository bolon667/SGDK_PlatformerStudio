#include "../inc/levelgenerator.h"

#include "../res/resources.h"
#include "../inc/global.h"
#include "../inc/map.h"

//Dynamic 2D array where we store the collision map data
//We could read that directly from ROM but in the long term it's cleaner and/or more performant
//u8** currentMap;


/*
//Downlscaled size of the map in order to match the collision map size
Vect2D_u16 roomTileSize;

void freeCollisionMap() {
	//We have to free the collision map data in this way in order to avoid memory leaks
	for (u16 i = 0; i < roomTileSize.y; i++) {
		MEM_free(currentMap[i]);
	}
	MEM_free(currentMap);
}

void updateRoomSize(Level* lvl){
	roomSize = newAABB(0, lvl->sizeinPx.x, 0, lvl->sizeinPx.y);
	roomTileSize = newVector2D_u16(lvl->sizeinTiles.x, lvl->sizeinTiles.y);
}

void generateCollisionMap(Level* lvl) {
	
	roomSize = newAABB(0, lvl->sizeinPx.x, 0, lvl->sizeinPx.y);

	//Each tile is 16x16px so we divide 768(size of the room in pixels) / 16, we use bitshifts because it is more performant than divisions

	roomTileSize = newVector2D_u16(lvl->sizeinTiles.x, lvl->sizeinTiles.y);

	//To store a 2D array with pointers we have to do it in that way
	currentMap = (u8**)MEM_alloc(roomTileSize.y * sizeof(u8*));
	for (u16 i = 0; i < roomTileSize.y; i++) {
		currentMap[i] = (u8*)MEM_alloc(roomTileSize.x);
		memcpy(currentMap[i], lvl->collArr[i], roomTileSize.x);
	}
	
}

*/
