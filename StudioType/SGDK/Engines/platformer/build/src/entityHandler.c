#include "../inc/levels.h"

#include "../inc/maps.h"
#include "../inc/messages.h"
#include "../inc/global.h"
#include "../inc/enums.h"
#include "../inc/physics.h"

#include "../res/resources.h"
#include "../res/images.h"
#include "../res/music.h"
#include "../res/gfx.h"
#include "../res/sprites.h"
#include "../res/tilesets.h"

#include "../inc/camera.h"
#include "../inc/customScripts.h"


u32 findSprOptIndexBySpriteDefinition(SpriteDefinition* sprDef){
	for(u16 i=0;i < curPreloadSprDefs->len; i++){
		if(curPreloadSprDefs->sprDefArr[i] == sprDef){
			return i;
		}
	}
	return 9999;

}

void frameChangedSprOpt1(Sprite* sprite)
{
    u16 enemyIndex = sprite->data;
    u16 tileIndex = curSprTileIndexes[enemyIndex][sprite->animInd][sprite->frameInd];
    SPR_setVRAMTileIndex(sprite, tileIndex);
}

void updatePosEntityMerged_trigger(EntityMerged* entity){
	entity->pos.x += entity->spd.x;
	entity->pos.y += entity->spd.y;
	entity->posInt.x = fix32ToInt(entity->pos.x);
	entity->posInt.y = fix32ToInt(entity->pos.y);
	entity->trigger->pos.x = entity->posInt.x;
	entity->trigger->pos.y = entity->posInt.y;
}

void updatePosEntityMerged(EntityMerged* entity){
	entity->pos.x += entity->spd.x;
	entity->pos.y += entity->spd.y;
	entity->posInt.x = fix32ToInt(entity->pos.x);
	entity->posInt.y = fix32ToInt(entity->pos.y);
}

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

EntityMerged* entityCheckForBullet(EntityMerged* entity){
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
				return curBullet;
			}
		}
	}
	return NULL;
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

void checkTriggerForPlayer(Trigger* trigger, pBody* plr){
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
	if((plr->playerBounds.min.x < triggerBounds.max.x) && (plr->playerBounds.max.x > triggerBounds.min.x) && (plr->playerBounds.min.y < triggerBounds.max.y) && (plr->playerBounds.max.y > triggerBounds.min.y)){
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

void checkTriggerFor_EntityMerged_trigger(EntityMerged* entity, Trigger* trigger){
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

	// trigger->prevActivated = trigger->activated;
	if(((entity->trigger->pos.x + entity->trigger->rect.min.x) < triggerBounds.max.x) && ((entity->trigger->pos.x + entity->trigger->rect.max.x) > triggerBounds.min.x) && ((entity->trigger->pos.y + entity->trigger->rect.min.y) < triggerBounds.max.y) && ((entity->trigger->pos.y + entity->trigger->rect.max.y) > triggerBounds.min.y)){
		
		// trigger->activated = TRUE;
		
			// KLog_S1("trigger->type: ", trigger->type);
			// KLog_S1("trigger->val1: ", trigger->val1);
			// KLog_S1("trigger->val2: ", trigger->val2);
			// KLog_S1("trigger->val3: ", trigger->val3);
		triggerTypeFuncArr[trigger->type](trigger, &triggerBounds);
	} else {
		// trigger->activated = FALSE;
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
