void show$entityName$($entityType$* entity){
    if(!entity->alive){
		return;
	}
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
			if(entity->sprDef) entity->spr = SPR_addSprite(entity->sprDef, posX_OnCam, posY_OnCam, TILE_ATTR(ENEMY_PALETTE, 0, FALSE, FALSE));
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
		
		
		
		
		
		if(entity->sprDef) SPR_setPosition(entity->spr, posX_OnCam, posY_OnCam);

		entity->onScreen = TRUE;
    }
}