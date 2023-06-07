void show$entityName$($entityType$* entity){
    if(!entity->alive){
		return;
	}
	if(entity->preDeath){
		if(entity->spr)	SPR_releaseSprite(entity->spr);
		//$showTriggerRects_releaseSprite$

		entity->alive = FALSE;
		return;
	}
    s16 posX_OnCam = entity->posInt.x-cameraPosition.x;
	s16 posY_OnCam = entity->posInt.y-cameraPosition.y;

	//$updatePosition_Entity_always$
	if ((posX_OnCam < -entity->size.x) || (posX_OnCam > 320) || (posY_OnCam < -entity->size.y) || (posY_OnCam > 224)) {
		entity->preDeath = TRUE;
		return;
	}
    else
    {
		if(!entity->onScreen) {
			entity->spr = SPR_addSpriteSafe(entity->sprDef, posX_OnCam, posY_OnCam, TILE_ATTR(entity->pal, 11, FALSE, FALSE));
			entity->spr->data = findSprOptIndexBySpriteDefinition(entity->sprDef);
			if(entity->spr->data != 9999){
				SPR_setAutoTileUpload(entity->spr, FALSE);
				entity->spr->data = findSprOptIndexBySpriteDefinition(entity->sprDef);
				SPR_setFrameChangeCallback(entity->spr, &frameChangedSprOpt1);
			}
			//$showTriggerRects_addSprite$
		}
        if(entity->sprDef) {
			SPR_setPosition(entity->spr, posX_OnCam, posY_OnCam);

			//Update position
			entity->pos.x += entity->spd.x;
			entity->pos.y += entity->spd.y;
			entity->posInt.x = fix32ToInt(entity->pos.x);
			entity->posInt.y = fix32ToInt(entity->pos.y);
		}
		//$showTriggerRects_moveSprite$
		
		entity->onScreen = TRUE;
    }
}