#include "../inc/levels.h"

#include "../inc/maps.h"
#include "../inc/global.h"

#include "../res/resources.h"
#include "../res/gfx.h"
#include "../res/sprites.h"
#include "../inc/camera.h"

void showEntityAll(){
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

	KLog_S1("1: ", entity->posInt.x);
	KLog_S1("2: ", posX_OnCam);
	//$updatePosition_Entity_always$
	if ((posX_OnCam < -entity->size.x) || (posX_OnCam > 320) || (posY_OnCam < -entity->size.y) || (posY_OnCam > 224)) {
		entity->onScreen = FALSE;
		
	}
    else
    {
		if(!entity->onScreen) {
			entity->spr = SPR_addSpriteSafe(&spr_noSpr, posX_OnCam, posY_OnCam, TILE_ATTR(PAL0, 0, FALSE, FALSE));
		}
        SPR_setPosition(entity->spr, posX_OnCam, posY_OnCam);
		
		entity->onScreen = TRUE;
    }

}