#include "../inc/levels.h"

#include "../inc/maps.h"
#include "../inc/messages.h"
#include "../inc/global.h"
#include "../inc/enums.h"

#include "../res/resources.h"
#include "../res/images.h"
#include "../res/gfx.h"
#include "../res/sprites.h"
#include "../inc/camera.h"
#include "../inc/messages.h"
#include "../inc/customScripts.h"


//$entityDefaultConsts$

//$entityBulletDefaultConsts$

//$addNewEntityFuncs$

//$addNewEntityBulletFuncs$

EntityMerged* addNew_EntityMerged(EntityMerged* entity){
	Trigger* foundTrigger = NULL;
	// If entity have trigger
	if(entity->trigger != NULL){
		
		//Find first dead Trigger
		for(u16 i=0; i<curEntityAll->Trigger_size; i++){
			
			//If trigger found
			if(!curEntityAll->Trigger_arr[i].alive){
				KLog_S1("Adding trigger to slot: ", i);
				//Saving reference to trigger in var foundTrigger.
				foundTrigger = &curEntityAll->Trigger_arr[i]; 
				//And adding entity trigger to local trigger storage
				// KLog_S1("entity->trigger->pos.x: ", entity->trigger->pos.x);
				// KLog_S1("entity->trigger->pos.y: ", entity->trigger->pos.y);
				memcpy(&curEntityAll->Trigger_arr[i], entity->trigger, sizeof(Trigger));
				break;
			}
		}
	} else {
		KLog_S1("Cant add a trigger: ", curEntityAll->Trigger_size);
	}
	
	//Find first dead EntityMerged
    for(u16 i=0; i<curEntityAll->EntityMerged_size; i++){
		//If EntityMerged found
        if(!curEntityAll->EntityMerged_arr[i].alive){
			KLog_S1("Adding EntityMerged to slot: ", i);
            //Copying entity data from chunk to local entity storage
            memcpy(&curEntityAll->EntityMerged_arr[i], entity, sizeof(EntityMerged));

			
            //Adding trigger to entity
			if(foundTrigger != NULL){
				curEntityAll->EntityMerged_arr[i].trigger = foundTrigger;
			}
            
			//After you found entity, no need to waste resource on useless loop iterations.
			break;
        }
    }

    return NULL;
}

    
    

void entityCheckForBullet(EntityMerged* entity){
	//WARNING
	//If you wanna make an enemy which not desappearing after death, then, make sure that enemy does not call this function, otherwise you will get a MEMORY LEAK!!!
	entity->damaged = FALSE;
	AABB entityTriggerBounds = newAABB(
		entity->trigger->pos.x + entity->trigger->rect.min.x,
		entity->trigger->pos.x + entity->trigger->rect.max.x,
		entity->trigger->pos.y + entity->trigger->rect.min.y,
		entity->trigger->pos.y + entity->trigger->rect.max.y
	);
	for(u16 i = 0; i < curEntityAll->EntityBulletMerged_size; i++){
		EntityBulletMerged* curBullet = &curEntityAll->EntityBulletMerged_arr[i];
		if(!curBullet->alive){
			continue;
		}
		AABB bulletBounds = newAABB(
			curBullet->posInt.x + curBullet->aabb.min.x,
			curBullet->posInt.x + curBullet->aabb.max.x,
			curBullet->posInt.y + curBullet->aabb.min.y,
			curBullet->posInt.y + curBullet->aabb.max.y
		);
		if((bulletBounds.min.x < entityTriggerBounds.max.x) && (bulletBounds.max.x > entityTriggerBounds.min.x)){
			if((bulletBounds.min.y < entityTriggerBounds.max.y) && (bulletBounds.max.y > entityTriggerBounds.min.y)){
				entity->hp -= curBullet->damage;
				entity->damaged = TRUE;
				curBullet->preDeath = TRUE;
				return;
			}
		}
	}
}

void entityCheckForBulletBySize(EntityMerged* entity){
	entity->damaged = FALSE;
	AABB entityTriggerBounds = newAABB(
		entity->posInt.x,
		entity->posInt.x + entity->size.x,
		entity->posInt.y,
		entity->posInt.y + entity->size.y
	);
	for(u16 i = 0; i < curEntityAll->EntityBulletMerged_size; i++){
		EntityBulletMerged* curBullet = &curEntityAll->EntityBulletMerged_arr[i];
		if(!curBullet->alive){
			continue;
		}
		AABB bulletBounds = newAABB(
			curBullet->posInt.x + curBullet->aabb.min.x,
			curBullet->posInt.x + curBullet->aabb.max.x,
			curBullet->posInt.y + curBullet->aabb.min.y,
			curBullet->posInt.y + curBullet->aabb.max.y
		);
		if((bulletBounds.min.x < entityTriggerBounds.max.x) && (bulletBounds.max.x > entityTriggerBounds.min.x)){
			if((bulletBounds.min.y < entityTriggerBounds.max.y) && (bulletBounds.max.y > entityTriggerBounds.min.y)){
				entity->hp -= curBullet->damage;
				entity->damaged = TRUE;
				curBullet->preDeath = TRUE;
				return;
			}
		}
	}
}

void deleteEntityMergedTriggerByInstId(u16 instId){
	for(u16 i = 0; i < curEntityAll->EntityMerged_size; i++){
		EntityMerged* curEntityMerged = &curEntityAll->EntityMerged_arr[i];
		if(!curEntityMerged->alive){
			continue;
		}
		if(curEntityMerged->instId == instId){
			curEntityMerged->trigger->alive = FALSE;
			break;
		}
	}
}

EntityMerged* getEntityMergedByInstId(u16 instId){
	for(u16 i = 0; i < curEntityAll->EntityMerged_size; i++){
		EntityMerged* curEntityMerged = &curEntityAll->EntityMerged_arr[i];
		if(!curEntityMerged->alive){
			continue;
		}
		if(curEntityMerged->instId == instId){
			return curEntityMerged;
		}
	}
	return NULL;
}

u16 getEntityMergedIndInArrByInstId(u16 instId){
	for(u16 i = 0; i < curEntityAll->EntityMerged_size; i++){
		EntityMerged* curEntityMerged = &curEntityAll->EntityMerged_arr[i];
		if(!curEntityMerged->alive){
			continue;
		}
		if(curEntityMerged->instId == instId){
			return i;
		}
	}
	return NULL;
}

u16 getTriggerParrentIndInArr(Trigger* trigger){
	for(u16 i = 0; i < curEntityAll->EntityMerged_size; i++){
		EntityMerged* curEntityMerged = &curEntityAll->EntityMerged_arr[i];
		if(!curEntityMerged->alive){
			continue;
		}
		if(curEntityMerged->trigger == trigger){
			return i;
		}
	}
	return NULL;
}

// EntityMerged* getRandomEntityMergedByDefId(u16 instId){
// 	//DefId = EntityType
// 	//May be will add that in the future
// 	for(u16 i = 0; i < curEntityAll->EntityMerged_size; i++){
// 		EntityMerged* curEntityMerged = &curEntityAll->EntityMerged_arr[i];
// 		if(!curEntityMerged->alive){
// 			continue;
// 		}
// 		if(curEntityMerged->instId == instId){
// 			return curEntityMerged;
// 		}
// 	}
// 	return NULL;
// }


//$triggerTypeFuncs$

void checkTriggerForPlayer(Trigger* trigger){
	if(!trigger->alive){
		return;
	}
	AABB triggerBounds = newAABB(
		trigger->pos.x + trigger->rect.min.x,
		trigger->pos.x + trigger->rect.max.x,
		trigger->pos.y + trigger->rect.min.y,
		trigger->pos.y + trigger->rect.max.y
	);
	//KLog_S1("-----------", 0);
	//KLog_S1("trigger->activated: ", trigger->activated);
	//KLog_S1("trigger->prevActivated: ", trigger->prevActivated);
	trigger->prevActivated = trigger->activated;
	if((playerBounds.min.x < triggerBounds.max.x) && (playerBounds.max.x > triggerBounds.min.x) && (playerBounds.min.y < triggerBounds.max.y) && (playerBounds.max.y > triggerBounds.min.y)){
		trigger->activated = TRUE;
			// KLog_S1("trigger->type: ", trigger->type);
			// KLog_S1("trigger->val1: ", trigger->val1);
			// KLog_S1("trigger->val2: ", trigger->val2);
			// KLog_S1("trigger->val3: ", trigger->val3);
		triggerTypeFuncArr[trigger->type](trigger, &triggerBounds);
	} else {
		trigger->activated = FALSE;
		if(trigger->prevActivated) {
			triggerTypeFuncArr[trigger->type](trigger, &triggerBounds);
		}
	}
	
}

//$showEntityBulletFuncs$

//$showEntityFuncs$

void showEntityBulletMerged(EntityBulletMerged* entity){
	showEntityBulletFuncArr[entity->entityType](entity);
}

void showEntityMerged(EntityMerged* entity){
	showEntityFuncArr[entity->entityType](entity);
}


void showEntityAll(){
	for(u16 i=0; i<curEntityAll->EntityBulletMerged_size; i++){
		showEntityBulletMerged(&curEntityAll->EntityBulletMerged_arr[i]);
	}
	for(u16 i=0; i<curEntityAll->EntityMerged_size; i++){
		showEntityMerged(&curEntityAll->EntityMerged_arr[i]);
	}
}
