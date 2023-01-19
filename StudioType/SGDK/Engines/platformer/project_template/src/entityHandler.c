#include "../inc/levels.h"

#include "../inc/maps.h"
#include "../inc/global.h"

#include "../res/resources.h"
#include "../res/gfx.h"
#include "../res/sprites.h"
#include "../inc/camera.h"

void checkTriggerForPlayer(Trigger* trigger){
	AABB triggerBounds = newAABB(
		trigger->pos.x + trigger->rect.min.x,
		trigger->pos.x + trigger->rect.max.x,
		trigger->pos.y + trigger->rect.min.y,
		trigger->pos.y + trigger->rect.max.y
	);
	if((playerBounds.min.x > triggerBounds.min.x) && (playerBounds.max.x < triggerBounds.max.x)){
		if((playerBounds.min.y > triggerBounds.min.y) && (playerBounds.max.y < triggerBounds.max.y)){
			KLog_S1("trigger->type: ", trigger->type);
			KLog_S1("trigger->value: ", trigger->value);
			switch(trigger->type) {
				case 0:
					loadLevel(trigger->value);
					break;
			}
		}
	}
}


void showEntityAll(){
	for(u16 i=0; i<curEntityAll->Trigger_size; i++){
		checkTriggerForPlayer(&curEntityAll->Trigger_arr[i]);
	}
	for(u16 i=0; i<curEntityAll->EntityMerged_size; i++){
		showEntityMerged(&curEntityAll->EntityMerged_arr[i]);
	}
}

void showEntityMerged(EntityMerged* entity){
	if(!entity->alive){
		return;
	}
    // clip out of screen sprites
	

    s16 posX_OnCam = entity->posInt.x-cameraPosition.x;
	s16 posY_OnCam = entity->posInt.y-cameraPosition.y;

	//$updatePosition_Entity_always$
	if ((posX_OnCam < -entity->size.x) || (posX_OnCam > 320) || (posY_OnCam < -entity->size.y) || (posY_OnCam > 224)) {
		if(entity->onScreen) {
			if(entity->sprDef) SPR_releaseSprite(entity->spr);
		}
		entity->onScreen = FALSE;
		
	}
    else
    {
		if(!entity->onScreen) {
			if(entity->sprDef) entity->spr = SPR_addSprite(entity->sprDef, posX_OnCam, posY_OnCam, TILE_ATTR(PAL0, 0, FALSE, FALSE));
		}
        if(entity->sprDef) SPR_setPosition(entity->spr, posX_OnCam, posY_OnCam);
		
		entity->onScreen = TRUE;
    }

}