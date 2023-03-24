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


const EntityMerged const EntitySimple_default = {0, FALSE, {0,0}, {0,0}, {0,0}, {0,0}, FALSE, NULL, 0, NULL, NULL, NULL, NULL, 1, 1, 0, 1, 1, };
const Trigger const EntitySimple_defaultTrigger = {FALSE, {0, 0}, {0, 0, 8, 8}, 0, 0, 0, 0, 1, };
const EntityMerged const Coin_default = {1, FALSE, {0,0}, {0,0}, {0,0}, {0,0}, FALSE, NULL, 0, NULL, NULL, NULL, &spr_coin, 1, 1, 0, 1, 1, };
const Trigger const Coin_defaultTrigger = {FALSE, {0, 0}, {0, 0, 20, 25}, TRIGGER_TYPE_gotSomething, 1, 0, 0, 1, };
const EntityMerged const Car_default = {2, FALSE, {0,0}, {0,0}, {0,0}, {0,0}, FALSE, NULL, 0, NULL, NULL, NULL, &spr_Car, 1, 1, 0, 1, 1, };
const Trigger const Car_defaultTrigger = {FALSE, {0, 0}, {0, 21, 128, 11}, TRIGGER_TYPE_hurt, 0, 0, 0, 1, };
const EntityMerged const Octopus_default = {3, FALSE, {0,0}, {0,0}, {0,0}, {0,0}, FALSE, NULL, 0, NULL, NULL, NULL, &spr_octopus, 1, 1, 0, 1, 1, };
const Trigger const Octopus_defaultTrigger = {FALSE, {0, 0}, {0, 0, 55, 55}, TRIGGER_TYPE_hurt, 0, 0, 0, 1, };
const EntityMerged const Lady_default = {4, FALSE, {0,0}, {0,0}, {0,0}, {0,0}, FALSE, NULL, 0, NULL, NULL, NULL, &spr_MacePickup2, 1, 1, 0, 1, 1, };
const Trigger const Lady_defaultTrigger = {FALSE, {0, 0}, {0, 0, 56, 64}, TRIGGER_TYPE_gotSomething, 1, 0, 0, 1, };
const EntityMerged const MacePickup_default = {5, FALSE, {0,0}, {0,0}, {0,0}, {0,0}, FALSE, NULL, 0, NULL, NULL, NULL, &spr_bonus, 1, 1, 0, 1, 1, };
const Trigger const MacePickup_defaultTrigger = {FALSE, {0, 0}, {0, 0, 8, 8}, TRIGGER_TYPE_gotSomething, 1, 0, 0, 1, };


void addNew_EntitySimple(Vect2D_s16 posInt, Vect2D_f16 spd){
    Trigger* foundTrigger = NULL;
    //find first free index in arr
    
    KDebug_Alert("new Trigger");
    for(u16 i=0; i<curEntityAll->Trigger_size; i++){
        if(!curEntityAll->Trigger_arr[i].alive){
            memcpy(&curEntityAll->Trigger_arr[i], &EntitySimple_defaultTrigger, sizeof(Trigger));
            curEntityAll->Trigger_arr[i].pos = posInt;
            curEntityAll->Trigger_arr[i].alive = TRUE;
            KDebug_Alert("Success for Trigger!");
            foundTrigger = &curEntityAll->Trigger_arr[i];
            break;
        }
    }
    if(foundTrigger == NULL) {
        KDebug_Alert("Fail for Trigger and for EntitySimple...");
        return;
    }

    //find first free index in arr
    KDebug_Alert("new EntitySimple");
    for(u16 i=0; i<curEntityAll->EntityMerged_size; i++){
        if(!curEntityAll->EntityMerged_arr[i].alive){
            memcpy(&curEntityAll->EntityMerged_arr[i], &EntitySimple_default, sizeof(EntityMerged));
            curEntityAll->EntityMerged_arr[i].posInt = posInt;
            curEntityAll->EntityMerged_arr[i].pos = (Vect2D_f32){FIX32(posInt.x), FIX32(posInt.y)};
            curEntityAll->EntityMerged_arr[i].spd = spd;
            curEntityAll->EntityMerged_arr[i].alive = TRUE;
            curEntityAll->EntityMerged_arr[i].trigger = foundTrigger;
            KDebug_Alert("Success for EntitySimple!");
            return;
        }
    }
    KDebug_Alert("Fail for EntitySimple...");
    
}

void addNew_Coin(Vect2D_s16 posInt, Vect2D_f16 spd){
    Trigger* foundTrigger = NULL;
    //find first free index in arr
    
    KDebug_Alert("new Trigger");
    for(u16 i=0; i<curEntityAll->Trigger_size; i++){
        if(!curEntityAll->Trigger_arr[i].alive){
            memcpy(&curEntityAll->Trigger_arr[i], &Coin_defaultTrigger, sizeof(Trigger));
            curEntityAll->Trigger_arr[i].pos = posInt;
            curEntityAll->Trigger_arr[i].alive = TRUE;
            KDebug_Alert("Success for Trigger!");
            foundTrigger = &curEntityAll->Trigger_arr[i];
            break;
        }
    }
    if(foundTrigger == NULL) {
        KDebug_Alert("Fail for Trigger and for Coin...");
        return;
    }

    //find first free index in arr
    KDebug_Alert("new Coin");
    for(u16 i=0; i<curEntityAll->EntityMerged_size; i++){
        if(!curEntityAll->EntityMerged_arr[i].alive){
            memcpy(&curEntityAll->EntityMerged_arr[i], &Coin_default, sizeof(EntityMerged));
            curEntityAll->EntityMerged_arr[i].posInt = posInt;
            curEntityAll->EntityMerged_arr[i].pos = (Vect2D_f32){FIX32(posInt.x), FIX32(posInt.y)};
            curEntityAll->EntityMerged_arr[i].spd = spd;
            curEntityAll->EntityMerged_arr[i].alive = TRUE;
            curEntityAll->EntityMerged_arr[i].trigger = foundTrigger;
            KDebug_Alert("Success for Coin!");
            return;
        }
    }
    KDebug_Alert("Fail for Coin...");
    
}

void addNew_Car(Vect2D_s16 posInt, Vect2D_f16 spd){
    Trigger* foundTrigger = NULL;
    //find first free index in arr
    
    KDebug_Alert("new Trigger");
    for(u16 i=0; i<curEntityAll->Trigger_size; i++){
        if(!curEntityAll->Trigger_arr[i].alive){
            memcpy(&curEntityAll->Trigger_arr[i], &Car_defaultTrigger, sizeof(Trigger));
            curEntityAll->Trigger_arr[i].pos = posInt;
            curEntityAll->Trigger_arr[i].alive = TRUE;
            KDebug_Alert("Success for Trigger!");
            foundTrigger = &curEntityAll->Trigger_arr[i];
            break;
        }
    }
    if(foundTrigger == NULL) {
        KDebug_Alert("Fail for Trigger and for Car...");
        return;
    }

    //find first free index in arr
    KDebug_Alert("new Car");
    for(u16 i=0; i<curEntityAll->EntityMerged_size; i++){
        if(!curEntityAll->EntityMerged_arr[i].alive){
            memcpy(&curEntityAll->EntityMerged_arr[i], &Car_default, sizeof(EntityMerged));
            curEntityAll->EntityMerged_arr[i].posInt = posInt;
            curEntityAll->EntityMerged_arr[i].pos = (Vect2D_f32){FIX32(posInt.x), FIX32(posInt.y)};
            curEntityAll->EntityMerged_arr[i].spd = spd;
            curEntityAll->EntityMerged_arr[i].alive = TRUE;
            curEntityAll->EntityMerged_arr[i].trigger = foundTrigger;
            KDebug_Alert("Success for Car!");
            return;
        }
    }
    KDebug_Alert("Fail for Car...");
    
}

void addNew_Octopus(Vect2D_s16 posInt, Vect2D_f16 spd){
    Trigger* foundTrigger = NULL;
    //find first free index in arr
    
    KDebug_Alert("new Trigger");
    for(u16 i=0; i<curEntityAll->Trigger_size; i++){
        if(!curEntityAll->Trigger_arr[i].alive){
            memcpy(&curEntityAll->Trigger_arr[i], &Octopus_defaultTrigger, sizeof(Trigger));
            curEntityAll->Trigger_arr[i].pos = posInt;
            curEntityAll->Trigger_arr[i].alive = TRUE;
            KDebug_Alert("Success for Trigger!");
            foundTrigger = &curEntityAll->Trigger_arr[i];
            break;
        }
    }
    if(foundTrigger == NULL) {
        KDebug_Alert("Fail for Trigger and for Octopus...");
        return;
    }

    //find first free index in arr
    KDebug_Alert("new Octopus");
    for(u16 i=0; i<curEntityAll->EntityMerged_size; i++){
        if(!curEntityAll->EntityMerged_arr[i].alive){
            memcpy(&curEntityAll->EntityMerged_arr[i], &Octopus_default, sizeof(EntityMerged));
            curEntityAll->EntityMerged_arr[i].posInt = posInt;
            curEntityAll->EntityMerged_arr[i].pos = (Vect2D_f32){FIX32(posInt.x), FIX32(posInt.y)};
            curEntityAll->EntityMerged_arr[i].spd = spd;
            curEntityAll->EntityMerged_arr[i].alive = TRUE;
            curEntityAll->EntityMerged_arr[i].trigger = foundTrigger;
            KDebug_Alert("Success for Octopus!");
            return;
        }
    }
    KDebug_Alert("Fail for Octopus...");
    
}

void addNew_Lady(Vect2D_s16 posInt, Vect2D_f16 spd){
    Trigger* foundTrigger = NULL;
    //find first free index in arr
    
    KDebug_Alert("new Trigger");
    for(u16 i=0; i<curEntityAll->Trigger_size; i++){
        if(!curEntityAll->Trigger_arr[i].alive){
            memcpy(&curEntityAll->Trigger_arr[i], &Lady_defaultTrigger, sizeof(Trigger));
            curEntityAll->Trigger_arr[i].pos = posInt;
            curEntityAll->Trigger_arr[i].alive = TRUE;
            KDebug_Alert("Success for Trigger!");
            foundTrigger = &curEntityAll->Trigger_arr[i];
            break;
        }
    }
    if(foundTrigger == NULL) {
        KDebug_Alert("Fail for Trigger and for Lady...");
        return;
    }

    //find first free index in arr
    KDebug_Alert("new Lady");
    for(u16 i=0; i<curEntityAll->EntityMerged_size; i++){
        if(!curEntityAll->EntityMerged_arr[i].alive){
            memcpy(&curEntityAll->EntityMerged_arr[i], &Lady_default, sizeof(EntityMerged));
            curEntityAll->EntityMerged_arr[i].posInt = posInt;
            curEntityAll->EntityMerged_arr[i].pos = (Vect2D_f32){FIX32(posInt.x), FIX32(posInt.y)};
            curEntityAll->EntityMerged_arr[i].spd = spd;
            curEntityAll->EntityMerged_arr[i].alive = TRUE;
            curEntityAll->EntityMerged_arr[i].trigger = foundTrigger;
            KDebug_Alert("Success for Lady!");
            return;
        }
    }
    KDebug_Alert("Fail for Lady...");
    
}

void addNew_MacePickup(Vect2D_s16 posInt, Vect2D_f16 spd){
    Trigger* foundTrigger = NULL;
    //find first free index in arr
    
    KDebug_Alert("new Trigger");
    for(u16 i=0; i<curEntityAll->Trigger_size; i++){
        if(!curEntityAll->Trigger_arr[i].alive){
            memcpy(&curEntityAll->Trigger_arr[i], &MacePickup_defaultTrigger, sizeof(Trigger));
            curEntityAll->Trigger_arr[i].pos = posInt;
            curEntityAll->Trigger_arr[i].alive = TRUE;
            KDebug_Alert("Success for Trigger!");
            foundTrigger = &curEntityAll->Trigger_arr[i];
            break;
        }
    }
    if(foundTrigger == NULL) {
        KDebug_Alert("Fail for Trigger and for MacePickup...");
        return;
    }

    //find first free index in arr
    KDebug_Alert("new MacePickup");
    for(u16 i=0; i<curEntityAll->EntityMerged_size; i++){
        if(!curEntityAll->EntityMerged_arr[i].alive){
            memcpy(&curEntityAll->EntityMerged_arr[i], &MacePickup_default, sizeof(EntityMerged));
            curEntityAll->EntityMerged_arr[i].posInt = posInt;
            curEntityAll->EntityMerged_arr[i].pos = (Vect2D_f32){FIX32(posInt.x), FIX32(posInt.y)};
            curEntityAll->EntityMerged_arr[i].spd = spd;
            curEntityAll->EntityMerged_arr[i].alive = TRUE;
            curEntityAll->EntityMerged_arr[i].trigger = foundTrigger;
            KDebug_Alert("Success for MacePickup!");
            return;
        }
    }
    KDebug_Alert("Fail for MacePickup...");
    
}



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

void triggerTypeFunc_changeLevel(Trigger* trigger, AABB* triggerBounds) {
PAL_fadeOutAll(3,FALSE);
levelNum = trigger->val1;
loadLevel(levelNum, (Vect2D_s16)getLevelPos(trigger->val2));
}
void triggerTypeFunc_changeLevelv2(Trigger* trigger, AABB* triggerBounds) {
PAL_fadeOutAll(3,FALSE);
	levelNum = trigger->val1;
	Vect2D_s16 newStartPos = getLevelPos(trigger->val2);
	switch(trigger->val3) // position mode
	{
		case 0: //Horizontal, bottom anchor
			newStartPos.y -= triggerBounds->max.y-playerBounds.max.y+playerBody.aabb.max.y-playerBody.aabb.min.y;
		case 1: //Vertical, left anchor
			//Not sure if it works, need more testing
			newStartPos.x += triggerBounds->min.x-playerBounds.min.x+playerBody.aabb.min.y;
	}
					
	
	loadLevel(levelNum, (Vect2D_s16)newStartPos);
}
void triggerTypeFunc_executeScript(Trigger* trigger, AABB* triggerBounds) {
customScriptArr[trigger->val1]();
}
void triggerTypeFunc_forceDownPlayer(Trigger* trigger, AABB* triggerBounds) {
if((playerBody.globalPosition.y+playerBody.aabb.min.y) < (trigger->pos.y+trigger->rect.max.y)) {
		playerBody.globalPosition.y = trigger->pos.y+trigger->rect.max.y-playerBody.aabb.min.y+1;
		playerBody.velocity.fixY = 0;
	}
}
void triggerTypeFunc_forceLeftPlayer(Trigger* trigger, AABB* triggerBounds) {
if((playerBody.globalPosition.x+playerBody.aabb.max.x) > (trigger->pos.x+trigger->rect.min.x)) {
		playerBody.globalPosition.x = trigger->pos.x+trigger->rect.min.x-playerBody.aabb.max.x-1;
	}
}
void triggerTypeFunc_forceRightPlayer(Trigger* trigger, AABB* triggerBounds) {
if((playerBody.globalPosition.x+playerBody.aabb.min.x) < (trigger->pos.x+trigger->rect.max.x)) {
		playerBody.globalPosition.x = trigger->pos.x+trigger->rect.max.x-playerBody.aabb.min.x;
	}
}
void triggerTypeFunc_forceUpPlayer(Trigger* trigger, AABB* triggerBounds) {
if((playerBody.globalPosition.y+playerBody.aabb.max.y) > (trigger->pos.y+trigger->rect.min.y)) {
		playerBody.globalPosition.y = trigger->pos.y-playerBody.aabb.max.y+1;
		playerBody.curAmountOfJumps = playerBody.maxAmountOfJumps;
		playerBody.velocity.fixY = 0;
		playerBody.onGround = TRUE;
	}
}
void triggerTypeFunc_gotSomething(Trigger* trigger, AABB* triggerBounds) {
trigger->alive = FALSE;
}
void triggerTypeFunc_hurt(Trigger* trigger, AABB* triggerBounds) {

}
void triggerTypeFunc_showMessage(Trigger* trigger, AABB* triggerBounds) {
trigger->alive = FALSE;
					KLog_S1("curMessagePacks->len: ", curMessagePacks[trigger->val1].len);
					for(s16 i=0; i<curMessagePacks[trigger->val1].len; i++){
						printMessage(curMessagePacks[trigger->val1].messages[i].str);
					}
}
void(* triggerTypeFuncArr[])(Trigger*, AABB*) = {triggerTypeFunc_changeLevel, triggerTypeFunc_changeLevelv2, triggerTypeFunc_executeScript, triggerTypeFunc_forceDownPlayer, triggerTypeFunc_forceLeftPlayer, triggerTypeFunc_forceRightPlayer, triggerTypeFunc_forceUpPlayer, triggerTypeFunc_gotSomething, triggerTypeFunc_hurt, triggerTypeFunc_showMessage, };


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
			triggerTypeFuncArr[trigger->type](trigger, &triggerBounds);
		}
	}
}



void showEntitySimple(EntityMerged* entity){
    if(!entity->alive){
		return;
	}
    s16 posX_OnCam = entity->posInt.x-cameraPosition.x;
	s16 posY_OnCam = entity->posInt.y-cameraPosition.y;

	//$updatePosition_Entity_always$
	if ((posX_OnCam < -entity->size.x) || (posX_OnCam > 320) || (posY_OnCam < -entity->size.y) || (posY_OnCam > 224)) {
		if(entity->onScreen) {
			if(entity->sprDef) {
				SPR_releaseSprite(entity->spr);
				if(entity->debugSpr1) SPR_releaseSprite(entity->debugSpr1);
if(entity->debugSpr2) SPR_releaseSprite(entity->debugSpr2);

			}
		}
		entity->onScreen = FALSE;
		
	}
    else
    {
		if(!entity->onScreen) {
			if(entity->sprDef) entity->spr = SPR_addSprite(entity->sprDef, posX_OnCam, posY_OnCam, TILE_ATTR(ENEMY_PALETTE, 0, FALSE, FALSE));
			entity->debugSpr1 = SPR_addSprite(&spr_debugLeftTopCorner, posX_OnCam, posY_OnCam, TILE_ATTR(PAL3, 11, FALSE, FALSE));
entity->debugSpr2 = SPR_addSprite(&spr_debugRightBottom, posX_OnCam, posY_OnCam, TILE_ATTR(PAL3, 11, FALSE, FALSE));

		}
        if(entity->sprDef) SPR_setPosition(entity->spr, posX_OnCam, posY_OnCam);
		if(entity->debugSpr1) SPR_setPosition(entity->debugSpr1, (entity->trigger->pos.x-cameraPosition.x)+entity->trigger->rect.min.x, (entity->trigger->pos.y-cameraPosition.y)+entity->trigger->rect.min.y);
if(entity->debugSpr2) SPR_setPosition(entity->debugSpr2, (entity->trigger->pos.x-cameraPosition.x)+entity->trigger->rect.max.x-8, (entity->trigger->pos.y-cameraPosition.y)+entity->trigger->rect.max.y-8);

		
		entity->onScreen = TRUE;
    }
}
void showCoin(EntityMerged* entity){
    if(!entity->alive){
		return;
	}
	if(!entity->trigger->alive){
		entity->alive = FALSE;
		if(entity->spr) SPR_releaseSprite(entity->spr);
		if(entity->debugSpr1) SPR_releaseSprite(entity->debugSpr1);
if(entity->debugSpr2) SPR_releaseSprite(entity->debugSpr2);

		return;
	}
    s16 posX_OnCam = entity->posInt.x-cameraPosition.x;
	s16 posY_OnCam = entity->posInt.y-cameraPosition.y;

	//$updatePosition_Entity_always$
	if ((posX_OnCam < -entity->size.x) || (posX_OnCam > 320) || (posY_OnCam < -entity->size.y) || (posY_OnCam > 224)) {
		if(entity->onScreen) {
			entity->trigger->alive = FALSE;
		}
		entity->onScreen = FALSE;
		
	}
    else
    {
		if(!entity->onScreen) {
			if(entity->sprDef) entity->spr = SPR_addSprite(entity->sprDef, posX_OnCam, posY_OnCam, TILE_ATTR(ENEMY_PALETTE, 0, FALSE, FALSE));
			entity->debugSpr1 = SPR_addSprite(&spr_debugLeftTopCorner, posX_OnCam, posY_OnCam, TILE_ATTR(PAL3, 11, FALSE, FALSE));
entity->debugSpr2 = SPR_addSprite(&spr_debugRightBottom, posX_OnCam, posY_OnCam, TILE_ATTR(PAL3, 11, FALSE, FALSE));

		}
        if(entity->spr) SPR_setPosition(entity->spr, posX_OnCam, posY_OnCam);
		if(entity->debugSpr1) SPR_setPosition(entity->debugSpr1, (entity->trigger->pos.x-cameraPosition.x)+entity->trigger->rect.min.x, (entity->trigger->pos.y-cameraPosition.y)+entity->trigger->rect.min.y);
if(entity->debugSpr2) SPR_setPosition(entity->debugSpr2, (entity->trigger->pos.x-cameraPosition.x)+entity->trigger->rect.max.x-8, (entity->trigger->pos.y-cameraPosition.y)+entity->trigger->rect.max.y-8);

		entity->onScreen = TRUE;
    }
}
void showCar(EntityMerged* entity){
    if(!entity->alive){
		return;
	}
    s16 posX_OnCam = entity->posInt.x-cameraPosition.x;
	s16 posY_OnCam = entity->posInt.y-cameraPosition.y;

	//$updatePosition_Entity_always$
	if ((posX_OnCam < -entity->size.x) || (posX_OnCam > 320) || (posY_OnCam < -entity->size.y) || (posY_OnCam > 224)) {
		if(entity->onScreen) {
			if(entity->sprDef) SPR_releaseSprite(entity->spr);
			if(entity->debugSpr1) SPR_releaseSprite(entity->debugSpr1);
if(entity->debugSpr2) SPR_releaseSprite(entity->debugSpr2);

		}
		entity->onScreen = FALSE;
		
	}
    else
    {
		if(!entity->onScreen) {
			if(entity->sprDef) entity->spr = SPR_addSprite(entity->sprDef, posX_OnCam, posY_OnCam, TILE_ATTR(ENEMY_PALETTE, 0, FALSE, FALSE));
			entity->debugSpr1 = SPR_addSprite(&spr_debugLeftTopCorner, posX_OnCam, posY_OnCam, TILE_ATTR(PAL3, 11, FALSE, FALSE));
entity->debugSpr2 = SPR_addSprite(&spr_debugRightBottom, posX_OnCam, posY_OnCam, TILE_ATTR(PAL3, 11, FALSE, FALSE));

		}
        
		
		if(posX_OnCam < 0){
			entity->val2 = 1;
		}
		if(posX_OnCam > 224-entity->size.x){
			entity->val2 = 0;
		}

		if(entity->val2) {
			//Update pos
			entity->posInt.x += entity->val1;
		} else {
			entity->posInt.x -= entity->val1;
		}
		
		//Update trigger position
		entity->trigger->pos = entity->posInt;
		
		
		
		if(entity->sprDef) SPR_setPosition(entity->spr, posX_OnCam, posY_OnCam);
		if(entity->debugSpr1) SPR_setPosition(entity->debugSpr1, (entity->trigger->pos.x-cameraPosition.x)+entity->trigger->rect.min.x, (entity->trigger->pos.y-cameraPosition.y)+entity->trigger->rect.min.y);
if(entity->debugSpr2) SPR_setPosition(entity->debugSpr2, (entity->trigger->pos.x-cameraPosition.x)+entity->trigger->rect.max.x-8, (entity->trigger->pos.y-cameraPosition.y)+entity->trigger->rect.max.y-8);


		entity->onScreen = TRUE;
    }
}
void showOctopus(EntityMerged* entity){
    if(!entity->alive){
		return;
	}
	if(!entity->trigger->alive){
		entity->alive = FALSE;
		if(entity->spr) SPR_releaseSprite(entity->spr);
		if(entity->debugSpr1) SPR_releaseSprite(entity->debugSpr1);
if(entity->debugSpr2) SPR_releaseSprite(entity->debugSpr2);

		return;
	}
	
    s16 posX_OnCam = entity->posInt.x-cameraPosition.x;
	s16 posY_OnCam = entity->posInt.y-cameraPosition.y;

	//$updatePosition_Entity_always$
	if ((posX_OnCam < -entity->size.x) || (posX_OnCam > 320) || (posY_OnCam < -entity->size.y) || (posY_OnCam > 224)) {
		if(entity->onScreen) {
			if(entity->sprDef) SPR_releaseSprite(entity->spr);
			if(entity->debugSpr1) SPR_releaseSprite(entity->debugSpr1);
if(entity->debugSpr2) SPR_releaseSprite(entity->debugSpr2);

		}
		entity->onScreen = FALSE;
		
	}
    else
    {
		if(!entity->onScreen) {
			if(entity->sprDef) entity->spr = SPR_addSprite(entity->sprDef, posX_OnCam, posY_OnCam, TILE_ATTR(ENEMY_PALETTE, 0, FALSE, FALSE));
			entity->debugSpr1 = SPR_addSprite(&spr_debugLeftTopCorner, posX_OnCam, posY_OnCam, TILE_ATTR(PAL3, 11, FALSE, FALSE));
entity->debugSpr2 = SPR_addSprite(&spr_debugRightBottom, posX_OnCam, posY_OnCam, TILE_ATTR(PAL3, 11, FALSE, FALSE));

		}
        if(entity->sprDef) {
			SPR_setPosition(entity->spr, posX_OnCam, posY_OnCam);
			if(entity->debugSpr1) SPR_setPosition(entity->debugSpr1, (entity->trigger->pos.x-cameraPosition.x)+entity->trigger->rect.min.x, (entity->trigger->pos.y-cameraPosition.y)+entity->trigger->rect.min.y);
if(entity->debugSpr2) SPR_setPosition(entity->debugSpr2, (entity->trigger->pos.x-cameraPosition.x)+entity->trigger->rect.max.x-8, (entity->trigger->pos.y-cameraPosition.y)+entity->trigger->rect.max.y-8);


			//Check for bullet
			entityCheckForBullet(entity);
			if(entity->hp <= 0){
				entity->trigger->alive = FALSE;
				return;
			}
		}
		
		entity->onScreen = TRUE;
    }
}
void showLady(EntityMerged* entity){
    if(!entity->alive){
		return;
	}
	if(!entity->trigger->alive){
		entity->alive = FALSE;
		if(entity->spr) SPR_releaseSprite(entity->spr);
		if(entity->debugSpr1) SPR_releaseSprite(entity->debugSpr1);
if(entity->debugSpr2) SPR_releaseSprite(entity->debugSpr2);

		return;
	}
    s16 posX_OnCam = entity->posInt.x-cameraPosition.x;
	s16 posY_OnCam = entity->posInt.y-cameraPosition.y;

	//$updatePosition_Entity_always$
	if ((posX_OnCam < -entity->size.x) || (posX_OnCam > 320) || (posY_OnCam < -entity->size.y) || (posY_OnCam > 224)) {
		if(entity->onScreen) {
			entity->trigger->alive = FALSE;
		}
		entity->onScreen = FALSE;
		
	}
    else
    {
		if(!entity->onScreen) {
			if(entity->sprDef) entity->spr = SPR_addSprite(entity->sprDef, posX_OnCam, posY_OnCam, TILE_ATTR(ENEMY_PALETTE, 0, FALSE, FALSE));
			entity->debugSpr1 = SPR_addSprite(&spr_debugLeftTopCorner, posX_OnCam, posY_OnCam, TILE_ATTR(PAL3, 11, FALSE, FALSE));
entity->debugSpr2 = SPR_addSprite(&spr_debugRightBottom, posX_OnCam, posY_OnCam, TILE_ATTR(PAL3, 11, FALSE, FALSE));

		}
        if(entity->spr) SPR_setPosition(entity->spr, posX_OnCam, posY_OnCam);
		if(entity->debugSpr1) SPR_setPosition(entity->debugSpr1, (entity->trigger->pos.x-cameraPosition.x)+entity->trigger->rect.min.x, (entity->trigger->pos.y-cameraPosition.y)+entity->trigger->rect.min.y);
if(entity->debugSpr2) SPR_setPosition(entity->debugSpr2, (entity->trigger->pos.x-cameraPosition.x)+entity->trigger->rect.max.x-8, (entity->trigger->pos.y-cameraPosition.y)+entity->trigger->rect.max.y-8);

		entity->onScreen = TRUE;
    }
}
void showMacePickup(EntityMerged* entity){
    if(!entity->alive){
		return;
	}
	if(!entity->trigger->alive){
		entity->alive = FALSE;
		if(entity->spr) SPR_releaseSprite(entity->spr);
		if(entity->debugSpr1) SPR_releaseSprite(entity->debugSpr1);
if(entity->debugSpr2) SPR_releaseSprite(entity->debugSpr2);

		return;
	}
    s16 posX_OnCam = entity->posInt.x-cameraPosition.x;
	s16 posY_OnCam = entity->posInt.y-cameraPosition.y;

	//$updatePosition_Entity_always$
	if ((posX_OnCam < -entity->size.x) || (posX_OnCam > 320) || (posY_OnCam < -entity->size.y) || (posY_OnCam > 224)) {
		if(entity->onScreen) {
			entity->trigger->alive = FALSE;
		}
		entity->onScreen = FALSE;
		
	}
    else
    {
		if(!entity->onScreen) {
			if(entity->sprDef) entity->spr = SPR_addSprite(entity->sprDef, posX_OnCam, posY_OnCam, TILE_ATTR(ENEMY_PALETTE, 0, FALSE, FALSE));
			entity->debugSpr1 = SPR_addSprite(&spr_debugLeftTopCorner, posX_OnCam, posY_OnCam, TILE_ATTR(PAL3, 11, FALSE, FALSE));
entity->debugSpr2 = SPR_addSprite(&spr_debugRightBottom, posX_OnCam, posY_OnCam, TILE_ATTR(PAL3, 11, FALSE, FALSE));

		}
        if(entity->spr) SPR_setPosition(entity->spr, posX_OnCam, posY_OnCam);
		if(entity->debugSpr1) SPR_setPosition(entity->debugSpr1, (entity->trigger->pos.x-cameraPosition.x)+entity->trigger->rect.min.x, (entity->trigger->pos.y-cameraPosition.y)+entity->trigger->rect.min.y);
if(entity->debugSpr2) SPR_setPosition(entity->debugSpr2, (entity->trigger->pos.x-cameraPosition.x)+entity->trigger->rect.max.x-8, (entity->trigger->pos.y-cameraPosition.y)+entity->trigger->rect.max.y-8);

		entity->onScreen = TRUE;
    }
}
void(* showEntityFuncArr[])(EntityMerged*) = {showEntitySimple, showCoin, showCar, showOctopus, showLady, showMacePickup, };


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

