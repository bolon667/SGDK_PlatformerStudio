#include "../inc/levels.h"
void allocLevel(){
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
		curEntityAll->EntityBulletMerged_arr[i].entityType = FALSE;
	}

	//duplicate entityMerged_arr for entityAll
	//and allocate additional slots
	curEntityAll->EntityMerged_arr = MEM_alloc(sizeof(EntityMerged)*(LevelFull_arr[levelNum].entityAll_arr->EntityMerged_size+LevelFull_arr[levelNum].entityAll_arr->additionalEntityMergedSlots));
	//Since i possibly can add additional slots, i need to clean this slots to avoid unexpected behavior.
	for(u16 i=0; i < curEntityAll->EntityMerged_size+curEntityAll->additionalEntityMergedSlots; i++){
		curEntityAll->EntityMerged_arr[i].alive = FALSE;
		curEntityAll->EntityMerged_arr[i].entityType = FALSE;
	}
	//And then, put entity data for level
	memcpy(curEntityAll->EntityMerged_arr, LevelFull_arr[levelNum].entityAll_arr->EntityMerged_arr, sizeof(EntityMerged)*LevelFull_arr[levelNum].entityAll_arr->EntityMerged_size);
	//Data is now OK, changing the size
	curEntityAll->EntityMerged_size += LevelFull_arr[levelNum].entityAll_arr->additionalEntityMergedSlots;
	//duplicate Trigger_arr for entityAll
	//and allocate additional slots for EntityMerged
	curEntityAll->Trigger_arr = MEM_alloc(sizeof(Trigger)*(LevelFull_arr[levelNum].entityAll_arr->Trigger_size+LevelFull_arr[levelNum].entityAll_arr->additionalEntityMergedSlots));
	//Since i possibly can add additional slots, i need to clean this slots to avoid unexpected behavior.
	for(u16 i=0; i < curEntityAll->Trigger_size+curEntityAll->additionalEntityMergedSlots; i++){
		curEntityAll->Trigger_arr[i].alive = FALSE;
		curEntityAll->Trigger_arr[i].isFree = TRUE;
	}
	//And then, put trigger data for level
	memcpy(curEntityAll->Trigger_arr, LevelFull_arr[levelNum].entityAll_arr->Trigger_arr, sizeof(Trigger)*LevelFull_arr[levelNum].entityAll_arr->Trigger_size);
	//Data is now OK, changing the size
	curEntityAll->Trigger_size += curEntityAll->additionalEntityMergedSlots;

	//update trigger reference in entityMerged_arr
	for(u16 i=0 ; i < curEntityAll->EntityMerged_size; i++){
		curEntityAll->EntityMerged_arr[i].trigger =  &curEntityAll->Trigger_arr[curEntityAll->EntityMerged_arr[i].triggerInd];
	}
}
