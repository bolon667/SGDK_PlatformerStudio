#include "../inc/levels.h"
#include "../inc/player.h"

void loadChunkFull(){
	// Getting chunk position
	// chunk size = 160, for example

	prevChunkPos.x = curChunkPos.x;
	prevChunkPos.y = curChunkPos.y;

	curChunkPos.x = (cameraPosition.x / $chunkSizeX$);
	curChunkPos.y = (cameraPosition.y / $chunkSizeY$);
	
	//If you are on the same chunk, then, no need to waste precious CPU ticks
	if((curChunkPos.x != prevChunkPos.x) || (curChunkPos.y != prevChunkPos.y)) {
		KLog_S1("LOAD NEW CHUNK!!!!", 1111);
		Vect2D_s16 firstChunkPos = {curChunkPos.x-1, curChunkPos.y-1};
		Vect2D_s16 lastChunkPos = {((cameraPosition.x+320)/$chunkSizeX$)+1, ((cameraPosition.y+224)/$chunkSizeY$)+1};


		while(curChunkPos.y <= lastChunkPos.y){
			if((curChunkPos.y >= curLvlData->sizeinChunks.y) || (curChunkPos.y < 0)){
				curChunkPos.y++;
				curChunkPos.x = firstChunkPos.x;
				continue;
			}
			while(curChunkPos.x <= lastChunkPos.x) {
				// KLog_S1("curChunkPos.x = ", curChunkPos.x);
				// KLog_S1("curChunkPos.y = ", curChunkPos.y);

				//If chunk outside of level bounds, then, drop it
				if((curChunkPos.x >= curLvlData->sizeinChunks.x) || (curChunkPos.x < 0)){
					curChunkPos.x++;
					continue;
				}

				//Converting chunk coordinates to array index
				u16 chunkIndex = curChunkPos.x+(curChunkPos.y*curLvlData->sizeinChunks.x);
				//Add new EntityMerged
				for(u16 i=0; i<LevelFull_arr[levelNum].entityAll_arr->EntityMergedChunk_arr[chunkIndex].size; i++){
					s16 curInstId = LevelFull_arr[levelNum].entityAll_arr->EntityMergedChunk_arr[chunkIndex].entityArr[i].instId;
					bool entityNotOnScene = TRUE;
					//Check if this entity not in scene, to avoid dupe
					for(u16 j=0; j < curEntityAll->additionalEntityMergedSlots; j++){
						if(!curEntityAll->EntityMerged_arr[j].alive){
							continue;
						}
						//If entity with instId=curInstId not in scene (curEntityAll->EntityMerged_arr), then, adding a new entity.
						if(curEntityAll->EntityMerged_arr[j].instId == curInstId){
							entityNotOnScene = FALSE;

							// KLog_S1("Insts id already exists, no dupe allowed: ", curInstId);
							break;
						}
					}

					if(entityNotOnScene){
						//KLog_S1("add EntityMerged with instId: ", curInstId);
						addNew_EntityMerged(&LevelFull_arr[levelNum].entityAll_arr->EntityMergedChunk_arr[chunkIndex].entityArr[i]);
					}
				}
				curChunkPos.x++;
			}
			curChunkPos.y++;
			curChunkPos.x = firstChunkPos.x;
		}
		//returning curChunkPos, to previous position
		curChunkPos.x = firstChunkPos.x;
		curChunkPos.y = firstChunkPos.y;
	}
}
