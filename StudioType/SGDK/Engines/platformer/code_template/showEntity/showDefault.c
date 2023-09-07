void show$entityName$($entityType$* entity){
    if(!entity->alive){
		return;
	}
    s16 posX_OnCam = entity->posInt.x-cameraPosition.x;
	s16 posY_OnCam = entity->posInt.y-cameraPosition.y;

	//$updatePosition_Entity_always$
	if ((posX_OnCam < -entity->size.x) || (posX_OnCam > 320) || (posY_OnCam < -entity->size.y) || (posY_OnCam > 224)) {
		if(entity->onScreen) {
			//entity->curAnimInd = entity->spr->animInd;
			//entity->curFrameInd = entity->spr->frameInd;
			
			if(entity->spr)	{
				SPR_releaseSprite(entity->spr);
				//$showTriggerRects_releaseSprite$

				//entity->spr = NULL;
			}
		}
		entity->onScreen = FALSE;
	}
    else
    {
		if(!entity->onScreen) {
			// if(entity->spr == NULL) {
			if(entity->sprDef) {
				entity->spr = SPR_addSprite(entity->sprDef, posX_OnCam, posY_OnCam, TILE_ATTR(entity->pal, 11, FALSE, FALSE));
				SPR_setHFlip(entity->spr, entity->hFlip);
				SPR_setVFlip(entity->spr, entity->vFlip);
				//SPR_setAnimAndFrame(entity->spr, entity->curAnimInd, entity->curFrameInd);

				//$showTriggerRects_addSprite$
			}
		}
        if(entity->sprDef) {
			//$showTriggerRects_moveSprite$
			SPR_setPosition(entity->spr, posX_OnCam, posY_OnCam);

			//updatePosEntityMerged(entity);
		}
		
		entity->onScreen = TRUE;
    }
}