#include "../inc/levels.h"

#include "../inc/maps.h"
#include "../inc/messages.h"
#include "../inc/global.h"

#include "../res/resources.h"
#include "../res/gfx.h"
#include "../res/sprites.h"
#include "../inc/camera.h"
#include "../inc/messages.h"
#include "../inc/customScripts.h"


//$entityDefaultConsts$

//$addNewEntityFuncs$

void entityCheckForBullet(EntityMerged* entity){
	entity->damaged = FALSE;
	AABB entityTriggerBounds = newAABB(
		entity->trigger->pos.x + entity->trigger->rect.min.x,
		entity->trigger->pos.x + entity->trigger->rect.max.x,
		entity->trigger->pos.y + entity->trigger->rect.min.y,
		entity->trigger->pos.y + entity->trigger->rect.max.y
	);
	for(u16 i = 0; i < curEntityAll->Bullet_size; i++){
		Bullet* curBullet = &curEntityAll->Bullet_arr[i];
		if(!curBullet->alive){
			continue;
		}
		AABB bulletBounds = newAABB(
			curBullet->posInt.x + curBullet->rect.min.x,
			curBullet->posInt.x + curBullet->rect.max.x,
			curBullet->posInt.y + curBullet->rect.min.y,
			curBullet->posInt.y + curBullet->rect.max.y
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

void addNewBullet(Bullet entity){
	for(u16 i = 0; i < curEntityAll->Bullet_size; i++){
		if(!curEntityAll->Bullet_arr[i].alive){
			curEntityAll->Bullet_arr[i] = entity;
			break;
		}
	}
}

void showBullet(Bullet* entity){
    if(!entity->alive){
		return;
	}
	if(entity->preDeath){
		if(entity->spr)	SPR_releaseSprite(entity->spr);
		if(entity->spr2) SPR_releaseSprite(entity->spr2);
		if(entity->spr3) SPR_releaseSprite(entity->spr3);
		entity->alive = FALSE;
		return;
	}

    s16 posX_OnCam = entity->posInt.x-cameraPosition.x;
	s16 posY_OnCam = entity->posInt.y-cameraPosition.y;

	//$updatePosition_Entity_always$
	if ((posX_OnCam < -entity->size.x) || (posX_OnCam > 320) || (posY_OnCam < -entity->size.y) || (posY_OnCam > 224)) {
		if(entity->onScreen) {
			entity->preDeath = TRUE;
			return;
		}
		entity->onScreen = FALSE;
		
	}
    else
    {
		if(!entity->onScreen) {
			if(entity->sprDef) {
				entity->spr = SPR_addSprite(entity->sprDef, posX_OnCam, posY_OnCam, TILE_ATTR(ENEMY_PALETTE, 15, FALSE, FALSE));
				
			}
			entity->spr2 = SPR_addSprite(&spr_debugLeftTopCorner, posX_OnCam, posY_OnCam, TILE_ATTR(PAL3, 11, FALSE, FALSE));
			entity->spr3 = SPR_addSprite(&spr_debugRightBottom, posX_OnCam, posY_OnCam, TILE_ATTR(PAL3, 11, FALSE, FALSE));
			
		}
        if(entity->sprDef){
			SPR_setPosition(entity->spr, posX_OnCam, posY_OnCam);
			
			
			//Update position
			entity->pos.x += entity->spd.x;
			entity->pos.y += entity->spd.y;
			entity->posInt.x = fix16ToInt(entity->pos.x);
			entity->posInt.y = fix16ToInt(entity->pos.y);

		}
		SPR_setPosition(entity->spr2, posX_OnCam, posY_OnCam);
		SPR_setPosition(entity->spr3, posX_OnCam+entity->size.x-8, posY_OnCam+entity->size.y-8);
		entity->lifeTime--;
		if(entity->lifeTime < 0){
			entity->preDeath = TRUE;
			return;
		}
		
		entity->onScreen = TRUE;
    }
}

void forcePlayerUp(Trigger* trigger){
	if((playerBody.globalPosition.y+playerBody.aabb.max.y) > (trigger->pos.y+trigger->rect.min.y)) {
		playerBody.globalPosition.y = trigger->pos.y-playerBody.aabb.max.y+1;
		playerBody.curAmountOfJumps = playerBody.maxAmountOfJumps;
		playerBody.velocity.fixY = 0;
		playerBody.onGround = TRUE;
	}
}

void forcePlayerDown(Trigger* trigger){
	if((playerBody.globalPosition.y+playerBody.aabb.min.y) < (trigger->pos.y+trigger->rect.max.y)) {
		playerBody.globalPosition.y = trigger->pos.y+trigger->rect.max.y-playerBody.aabb.min.y+1;
		playerBody.velocity.fixY = 0;
	}
}

void forcePlayerLeft(Trigger* trigger){
	if((playerBody.globalPosition.x+playerBody.aabb.max.x) > (trigger->pos.x+trigger->rect.min.x)) {
		playerBody.globalPosition.x = trigger->pos.x+trigger->rect.min.x-playerBody.aabb.max.x-1;
	}
}

void forcePlayerRight(Trigger* trigger){
	if((playerBody.globalPosition.x+playerBody.aabb.min.x) < (trigger->pos.x+trigger->rect.max.x)) {
		playerBody.globalPosition.x = trigger->pos.x+trigger->rect.max.x-playerBody.aabb.min.x;
	}
}

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
	if((playerBounds.min.x < triggerBounds.max.x) && (playerBounds.max.x > triggerBounds.min.x)){
		if((playerBounds.min.y < triggerBounds.max.y) && (playerBounds.max.y > triggerBounds.min.y)){
			KLog_S1("trigger->type: ", trigger->type);
			//KLog_S1("trigger->value: ", trigger->value);
			switch(trigger->type) {
				case 0: //Load level and player position
					PAL_fadeOutAll(5,FALSE);
					loadLevel(trigger->val1, trigger->val2);
					break;
				case 1: //got something
					trigger->alive = FALSE;
					break;
				case 2: //damaged
					//playerBody.damaged = TRUE;
					playerBody.animMode = 1;
					break;
				case 3: //show message
					trigger->alive = FALSE;
					KLog_S1("curMessagePacks->len: ", curMessagePacks[trigger->val1].len);
					for(s16 i=0; i<curMessagePacks[trigger->val1].len; i++){
						printMessage(curMessagePacks[trigger->val1].messages[i].str);
					}
					
					break;
				case 4: //execute custom script
					customScriptArr[trigger->val1]();
					break;
				case 5: //semi-solid Up
					forcePlayerUp(trigger);
					break;
				case 6: //semi-solid Down
					forcePlayerDown(trigger);
					break;
				case 7: //semi-solid Left
					forcePlayerLeft(trigger);
					break;
				case 8: //semi-solid Right
					forcePlayerRight(trigger);
					break;
			}
		}
	}
}

//$showEntityFuncs$

void showEntityMerged(EntityMerged* entity){
	showEntityFuncArr[entity->entityType](entity);
}

void showEntityAll(){
	if(playerBody.animModeChangable){
		playerBody.animMode = 0;
	}
	
	for(u16 i=0; i<curEntityAll->Bullet_size; i++){
		showBullet(&curEntityAll->Bullet_arr[i]);
	}
	for(u16 i=0; i<curEntityAll->Trigger_size; i++){
		checkTriggerForPlayer(&curEntityAll->Trigger_arr[i]);
	}
	for(u16 i=0; i<curEntityAll->EntityMerged_size; i++){
		showEntityMerged(&curEntityAll->EntityMerged_arr[i]);
	}
}

