#include "../inc/levels.h"
void allocLevel(){
	//imposible pos, to make sure, that you will get entities from chunk, when you appeared on level
	curChunkPos.x = -1;
	curChunkPos.y = -1;

    curLvlData = LevelFull_arr[levelNum].lvl;
	curMessagePacks = LevelFull_arr[levelNum].messagePacks;
	curLocalVariables = MEM_alloc(sizeof(LocalVariableMerged));
	memcpy(curLocalVariables, LevelFull_arr[levelNum].variable_arr, sizeof(LocalVariableMerged));
	//duplicate entityAll
	curEntityAll = MEM_alloc(sizeof(EntityAll));
	memcpy(curEntityAll, LevelFull_arr[levelNum].entityAll_arr, sizeof(EntityAll));
	//allocate trash memory for Bullet_arr
	curEntityAll->EntityBulletMerged_arr = MEM_alloc(sizeof(EntityBulletMerged)*LevelFull_arr[levelNum].entityAll_arr->EntityBulletMerged_size);

	//Change sensitive values, to avoid out of bounds error in arrays
	for(u16 i=0; i < curEntityAll->EntityBulletMerged_size; i++){
		curEntityAll->EntityBulletMerged_arr[i].alive = FALSE;
		curEntityAll->EntityBulletMerged_arr[i].entityType = 0;
	}

	//Allocate additionalEntityMergedSlots slots to be able to load new entity when you go trough chunks
	curEntityAll->EntityMerged_arr = MEM_alloc(sizeof(EntityMerged)*(LevelFull_arr[levelNum].entityAll_arr->additionalEntityMergedSlots));
	//Since, i possibly can add additional slots, i need to clean this slots to avoid unexpected behavior.
	for(u16 i=0; i < curEntityAll->additionalEntityMergedSlots; i++){
		curEntityAll->EntityMerged_arr[i].alive = FALSE;
		curEntityAll->EntityMerged_arr[i].entityType = FALSE;
	}
	//No need to copy entityData, because it's chunk optimization. You will load new entity data when you go trough chunks.
	//OK, changing the size to additionalEntityMergedSlots, this is maximum count of entities on scene.
	curEntityAll->EntityMerged_size = LevelFull_arr[levelNum].entityAll_arr->additionalEntityMergedSlots;
	//duplicate Trigger_arr for entityAll
	//and allocate additional slots for EntityMerged
	curEntityAll->Trigger_arr = MEM_alloc(sizeof(Trigger)*(LevelFull_arr[levelNum].entityAll_arr->additionalEntityMergedSlots));
	//Since i possibly can add additional slots, i need to clean this slots to avoid unexpected behavior.
	for(u16 i=0; i < curEntityAll->additionalEntityMergedSlots; i++){
		curEntityAll->Trigger_arr[i].alive = FALSE;
	}
	//And then, put trigger data for level
	//memcpy(curEntityAll->Trigger_arr, LevelFull_arr[levelNum].entityAll_arr->Trigger_arr, sizeof(Trigger)*LevelFull_arr[levelNum].entityAll_arr->Trigger_size);
	
	//Data is now OK, changing the size
	curEntityAll->Trigger_size = LevelFull_arr[levelNum].entityAll_arr->additionalEntityMergedSlots;

}
