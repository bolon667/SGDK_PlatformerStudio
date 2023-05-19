#include "../inc/levels.h"
#include "../inc/player.h"

void loadChunkFull(){
	//Getting chunk position
	//chunk size = 160, for example
	prevChunkPos.x = curChunkPos.x;
	prevChunkPos.y = curChunkPos.y;

	curChunkPos.x = playerBody.globalPosition.x % 160;
	curChunkPos.y = playerBody.globalPosition.y % 160;
	//I got a chunk position of player.

	//Converting chunk coordinates to array index
	u16 chunkIndex = curChunkPos.x+(curChunkPos.y*(curLvlData->sizeinPx.x%160));
	//Add new EntityMerged
	for(u16 i=0; i<LevelFull_arr[levelNum].entityAll_arr->EntityMergedChunk_arr[chunkIndex].size; i++){
		memcpy(&curEntityAll->EntityMerged_arr[i], &LevelFull_arr[levelNum].entityAll_arr->EntityMergedChunk_arr[chunkIndex].entityArr[i], sizeof(EntityMerged));
	}
}
