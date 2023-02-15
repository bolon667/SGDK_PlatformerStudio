void loadLevel(u16 levelNum, u16 positionNum) {

	//Reset everything
	VDP_resetScreen();
	VDP_setPlaneSize(64,32,TRUE);
	VDP_setScrollingMode(HSCROLL_PLANE, VSCROLL_PLANE);
	SPR_reset();

	//Deallocate prev entityData to avoid memory leak
	MEM_free(curEntityAll->Bullet_arr);
	MEM_free(curEntityAll->EntityMerged_arr);
	MEM_free(curEntityAll->Trigger_arr);
	MEM_free(curEntityAll);

	MEM_free(bga);
	MEM_free(bgb);

	VDPTilesFilled = TILE_USER_INDEX;
	
	//playerBody.globalPosition = getLevelStartPos();
	curLvlData = LevelFull_arr[levelNum].lvl;
	curMessagePacks = LevelFull_arr[levelNum].messagePacks;

	playerInit(positionNum);

	//memcpy(&palette_full[16],  curLvlData->foregroundPallete->data, 16 * 2);

	//duplicate entityAll
	curEntityAll = MEM_alloc(sizeof(EntityAll));
	memcpy(curEntityAll, LevelFull_arr[levelNum].entityAll_arr, sizeof(EntityAll));

	//allocate Bullet for entityAll
	curEntityAll->Bullet_arr = MEM_alloc(sizeof(Bullet)*LevelFull_arr[levelNum].entityAll_arr->Bullet_size);
	//Gen data for Bullet_arr
	for(u16 i=0; i < LevelFull_arr[levelNum].entityAll_arr->Bullet_size; i++){
		curEntityAll->Bullet_arr[i] = (Bullet){FALSE, {0,0}, {0,0}, {0,0}, {0,0}, {0,0,0,0}, FALSE, NULL, NULL};
	}

	//duplicate entityMerged_arr for entityAll
	curEntityAll->EntityMerged_arr = MEM_alloc(sizeof(EntityMerged)*LevelFull_arr[levelNum].entityAll_arr->EntityMerged_size);
	memcpy(curEntityAll->EntityMerged_arr, LevelFull_arr[levelNum].entityAll_arr->EntityMerged_arr, sizeof(EntityMerged)*LevelFull_arr[levelNum].entityAll_arr->EntityMerged_size);

	//duplicate Trigger_arr for entityAll
	curEntityAll->Trigger_arr = MEM_alloc(sizeof(Trigger)*LevelFull_arr[levelNum].entityAll_arr->Trigger_size);
	memcpy(curEntityAll->Trigger_arr, LevelFull_arr[levelNum].entityAll_arr->Trigger_arr, sizeof(Trigger)*LevelFull_arr[levelNum].entityAll_arr->Trigger_size);

	//update trigger reference in entityMerged_arr
	for(u16 i=0 ; i < curEntityAll->EntityMerged_size; i++){
		curEntityAll->EntityMerged_arr[i].trigger =  &curEntityAll->Trigger_arr[curEntityAll->EntityMerged_arr[i].triggerInd];
	}
	
	
	if(curLvlData->foregroundTileset != NULL){
		memcpy(&palette_full[16],  curLvlData->foregroundPallete->data, 16 * 2);
		VDP_loadTileSet(curLvlData->foregroundTileset, VDPTilesFilled, DMA);
		bga = MAP_create(curLvlData->foregroundMap, BG_A, TILE_ATTR_FULL(PAL1, 2, FALSE, FALSE, VDPTilesFilled));
		VDPTilesFilled += curLvlData->foregroundTileset->numTile;
	} else {
		bga = NULL;
	}
	
	if(curLvlData->backgroundTileset != NULL){
		memcpy(&palette_full[0], curLvlData->backgroundPallete->data, 16 * 2);
		VDP_loadTileSet(curLvlData->backgroundTileset, VDPTilesFilled, DMA);
		bgb = MAP_create(curLvlData->backgroundMap, BG_B, TILE_ATTR_FULL(PAL0, 1, FALSE, FALSE, VDPTilesFilled));
		VDPTilesFilled += curLvlData->backgroundTileset->numTile;
	} else {
		bgb = NULL;
	}

	if(curLvlData->foregroundImage != NULL){
		memcpy(&palette_full[16],  curLvlData->foregroundImage->palette->data, 16 * 2);
		VDP_drawImageEx(BG_A, curLvlData->foregroundImage, TILE_ATTR_FULL(PAL1, FALSE, FALSE, FALSE, VDPTilesFilled), 0, 0, FALSE, TRUE);
		VDPTilesFilled += curLvlData->foregroundImage->tileset->numTile;
	}
	if(curLvlData->backgroundImage != NULL){
		memcpy(&palette_full[0],  curLvlData->backgroundImage->palette->data, 16 * 2);
		VDP_drawImageEx(BG_B, curLvlData->backgroundImage, TILE_ATTR_FULL(PAL0, FALSE, FALSE, FALSE, VDPTilesFilled), 0, 0, FALSE, TRUE);
		VDPTilesFilled += curLvlData->backgroundImage->tileset->numTile;
	}
	PAL_setColors(0, palette_black, 63,DMA);
	memcpy(&palette_full[PLAYER_PALETTE*16], spr_player.palette->data, 16 * 2);
	memcpy(&palette_full[ENEMY_PALETTE*16], spr_coin.palette->data, 16 * 2);

	//addNewBullet((Bullet){TRUE, {2,2}, {0,0}, {FIX16(0.5),0}, {32,32}, {0,0,32,32}, FALSE, 1, NULL, &spr_noSpr});
	
	//memcpy(&palette_full[32],  spr_Enemy_sponge.palette->data, 16 * 2);
	//memcpy(&palette_full[48],  plr.sprite->definition->palette->data, 16 * 2);

	//Update the number of tiles filled in order to avoid overlaping them when loading more
	VDPTilesFilled += level_tileset.numTile;

	PAL_fadeIn(0, 63, palette_full, 5, TRUE);

	updateRoomSize(curLvlData);

	if(curLvlData->music != NULL){
		
	}

	KLog_U1("FreeMem: ", MEM_getFree());
}