#include "../inc/levels.h"

#include "../inc/maps.h"
#include "../inc/messages.h"
#include "../inc/global.h"
#include "../inc/enums.h"

#include "../res/resources.h"
#include "../res/gfx.h"
#include "../res/sprites.h"
#include "../inc/camera.h"
#include "../inc/messages.h"
#include "../inc/customScripts.h"


//$entityDefaultConsts$

//$entityBulletDefaultConsts$

//$addNewEntityFuncs$

//$addNewEntityBulletFuncs$

void entityCheckForBullet(EntityMerged* entity){
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
	trigger->prevActivated = trigger->activated;
	if((playerBounds.min.x < triggerBounds.max.x) && (playerBounds.max.x > triggerBounds.min.x)){
		if((playerBounds.min.y < triggerBounds.max.y) && (playerBounds.max.y > triggerBounds.min.y)){
			
			trigger->activated = TRUE;
			
			KLog_S1("trigger->activated: ", trigger->activated);
			KLog_S1("trigger->prevActivated: ", trigger->prevActivated);
			KLog_S1("trigger->type: ", trigger->type);
			KLog_S1("trigger->val1: ", trigger->val1);
			KLog_S1("trigger->val2: ", trigger->val2);
			KLog_S1("trigger->val3: ", trigger->val3);
			//KLog_S1("trigger->value: ", trigger->value);
			triggerTypeFuncArr[trigger->type](trigger, &triggerBounds);
			return;
		}
	}
	trigger->activated = FALSE;
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
	if(playerBody.animModeChangable){
		playerBody.animMode = 0;
	}
	
	for(u16 i=0; i<curEntityAll->EntityBulletMerged_size; i++){
		showEntityBulletMerged(&curEntityAll->EntityBulletMerged_arr[i]);
	}
	for(u16 i=0; i<curEntityAll->Trigger_size; i++){
		checkTriggerForPlayer(&curEntityAll->Trigger_arr[i]);
	}
	for(u16 i=0; i<curEntityAll->EntityMerged_size; i++){
		showEntityMerged(&curEntityAll->EntityMerged_arr[i]);
	}
}

