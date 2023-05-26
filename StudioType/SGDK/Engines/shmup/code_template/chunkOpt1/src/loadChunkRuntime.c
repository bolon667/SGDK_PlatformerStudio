#include "../inc/levels.h"
#include "../inc/player.h"

void loadEntitiesFromChunk(Vect2D_s16 curChunkPos){
	//MUST be updated chunkPos variable beforehand

	//Converting chunk coordinates to array index
	u16 chunkIndex = curChunkPos.x+(curChunkPos.y*curLvlData->sizeinChunks.x);
	//Add new EntityMerged

	// KLog_S1("------------ levelNum: ", levelNum);
	// KLog_S1("curChunkPos.x: ", curChunkPos.x);
	// KLog_S1("curChunkPos.y: ", curChunkPos.y);


	for(u16 i=0; i<LevelFull_arr[levelNum].entityAll_arr->EntityMergedChunk_arr[chunkIndex].size; i++){
		const s16 curInstId = LevelFull_arr[levelNum].entityAll_arr->EntityMergedChunk_arr[chunkIndex].entityArr[i].instId;
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
			// KLog_S1("add EntityMerged with instId: ", curInstId);
			addNew_EntityMerged(&LevelFull_arr[levelNum].entityAll_arr->EntityMergedChunk_arr[chunkIndex].entityArr[i]);
		}
	}
}

void loadChunkRuntime(){

	// Getting chunk position
	// chunk size = 160, for example

	//prevChunkPos.x = 0;
	prevChunkPos.x = curChunkPos.x;
	//KLog_S1("prevChunkPos2222222222.x: ", prevChunkPos.x);
	prevChunkPos.y = curChunkPos.y;

	curChunkPos.x = (cameraPosition.x / $chunkSizeX$);
	curChunkPos.y = (cameraPosition.y / $chunkSizeY$);

	Vect2D_s16 tempCurChunkPos = (Vect2D_s16){curChunkPos.x, curChunkPos.y};
	
	const s16 chunkPosXDiff = curChunkPos.x - prevChunkPos.x;
	const s16 chunkPosYDiff = curChunkPos.y - prevChunkPos.y;


	//If you are on the same chunk, then, no need to waste precious CPU ticks
	if(chunkPosXDiff != 0){
		
		Vect2D_s16 lastChunkPos = {((cameraPosition.x+320)/$chunkSizeX$)+1, ((cameraPosition.y+224)/$chunkSizeY$)+1};
		if(lastChunkPos.y > curLvlData->sizeinChunks.y){
			lastChunkPos.y = curLvlData->sizeinChunks.y;
		}
		if(lastChunkPos.x > curLvlData->sizeinChunks.x){
			lastChunkPos.x = curLvlData->sizeinChunks.x;
		}
		//If you go right
		if(chunkPosXDiff > 0){
			//Checking last column of chunks
			tempCurChunkPos.x = lastChunkPos.x;
			tempCurChunkPos.y = curChunkPos.y;
			while(tempCurChunkPos.y <= lastChunkPos.y){
				// KLog_S1("lastChunkPos.y: ", lastChunkPos.y);

				//If chunks outside of level bounds

				if((tempCurChunkPos.y >= curLvlData->sizeinChunks.y) || (tempCurChunkPos.y < 0) || (tempCurChunkPos.x >= curLvlData->sizeinChunks.x) || (tempCurChunkPos.x < 0)){
					//Then, skipping chunk
					tempCurChunkPos.y++;
					// KLog_S1("SKIP!!!!: ", curChunkPos.y);
					continue;
				}

				//Otherwise, adding entities from chunks on scene
				loadEntitiesFromChunk(tempCurChunkPos);
				
				tempCurChunkPos.y++;
			}
		} 
		else //If you go left
		{
			tempCurChunkPos.x = curChunkPos.x;
			tempCurChunkPos.y = curChunkPos.y;
			while(tempCurChunkPos.y <= lastChunkPos.y){

				//If chunks outside of level bounds
				if((tempCurChunkPos.y >= curLvlData->sizeinChunks.y) || (tempCurChunkPos.y < 0) || (tempCurChunkPos.x >= curLvlData->sizeinChunks.x) || (tempCurChunkPos.x < 0)){
					//Then, skipping chunk
					tempCurChunkPos.y++;
					// KLog_S1("SKIP!!!!: ", curChunkPos.y);
					continue;
				}
				//Otherwise, adding entities from chunks on scene
				loadEntitiesFromChunk(tempCurChunkPos);
				
				tempCurChunkPos.y++;
			}
		}
	}

	if(chunkPosYDiff != 0){
		Vect2D_s16 lastChunkPos = {((cameraPosition.x+320)/$chunkSizeX$)+1, ((cameraPosition.y+224)/$chunkSizeY$)+1};
		if(lastChunkPos.y > curLvlData->sizeinChunks.y){
			lastChunkPos.y = curLvlData->sizeinChunks.y;
		}
		if(lastChunkPos.x > curLvlData->sizeinChunks.x){
			lastChunkPos.x = curLvlData->sizeinChunks.x;
		}
		//If you go down
		if(chunkPosYDiff > 0){
			// KLog_S1("curChunkPos.x: ", curChunkPos.x);
			// KLog_S1("prevChunkPos.x: ", prevChunkPos.x);
			//Checking last row of chunks
			tempCurChunkPos.x = curChunkPos.x;
			tempCurChunkPos.y = lastChunkPos.y;
			if(tempCurChunkPos.y > curLvlData->sizeinChunks.y) {
				tempCurChunkPos.y = curLvlData->sizeinChunks.y;
			}
			while(tempCurChunkPos.x <= lastChunkPos.x){
				// KLog_S1("LOAD NEW CHUNK DOWN!!!!", 1111);
				KLog_S1("tempCurChunkPos.x: ", tempCurChunkPos.x);
				KLog_S1("lastChunkPos.x: ", lastChunkPos.x);
				//If chunks outside of level bounds
				if((tempCurChunkPos.y >= curLvlData->sizeinChunks.y) || (tempCurChunkPos.y < 0) || (tempCurChunkPos.x >= curLvlData->sizeinChunks.x) || (tempCurChunkPos.x < 0)){
					//Then, skipping chunk
					tempCurChunkPos.x++;
					// KLog_S1("SKIP!!!!: ", curChunkPos.y);
					continue;
				}
				//Otherwise, adding entities from chunks on scene
				loadEntitiesFromChunk(tempCurChunkPos);
				
				tempCurChunkPos.x++;
			}
		} 
		else //If you go up
		{
			tempCurChunkPos.x = curChunkPos.x;
			tempCurChunkPos.y = curChunkPos.y;
			while(tempCurChunkPos.x <= lastChunkPos.x){
				//If chunks outside of level bounds
				if((tempCurChunkPos.y >= curLvlData->sizeinChunks.y) || (tempCurChunkPos.y < 0) || (tempCurChunkPos.x >= curLvlData->sizeinChunks.x) || (tempCurChunkPos.x < 0)){
					//Then, skipping chunk
					tempCurChunkPos.x++;
					// KLog_S1("SKIP!!!!: ", curChunkPos.y);
					continue;
				}
				//Otherwise, adding entities from chunks on scene
				loadEntitiesFromChunk(tempCurChunkPos);
				
				tempCurChunkPos.x++;
			}
		}
	}
}
