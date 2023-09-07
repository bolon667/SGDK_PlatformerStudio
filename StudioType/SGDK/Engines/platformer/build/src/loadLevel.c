#include "../inc/levels.h"
#include "../inc/loadLevel.h"
#include "../inc/deallocLevel.h"
#include "../inc/allocLevel.h"


#include "../inc/maps.h"
#include "../inc/global.h"
#include "../inc/player.h"
#include "../inc/customScripts.h"
#include "../inc/entityHandler.h"

#include "../res/resources.h"
#include "../res/gfx.h"
#include "../res/sprites.h"
#include "../res/tilesets.h"
#include "../res/sounds.h"
#include "../res/music.h"
#include "../res/images.h"

void preloadSpriteTiles(){
	u16 numTile;
	for(u16 i=0;i < curPreloadSprDefs->len; i++){
		curSprTileIndexes[i] = SPR_loadAllFrames(curPreloadSprDefs->sprDefArr[i], VDPTilesFilled, &numTile);
		VDPTilesFilled += numTile;
	}
}

void stopLevel(){
	//Cleaning screen and deallocating sprites
	VDP_resetScreen();
	SPR_reset();
	SYS_doVBlankProcess();

	VDPTilesFilled = TILE_USER_INDEX;

	//Ususeful, because it's a const data, so i can deallocate MAP data
	MEM_free(bga);
	MEM_free(bgb);
}

void reloadLevelSprites(){
	//preloadTilesForSprites
	preloadSpriteTiles();
	//Return entity sprites
	EntityMerged *curEntity;
	for(u16 i=0; i<curEntityAll->EntityMerged_size; i++){
		curEntity = &curEntityAll->EntityMerged_arr[i];
		if(curEntity->onScreen && curEntity->alive){
			if(curEntity->sprDef) {
				s16 posX_OnCam = curEntity->posInt.x-cameraPosition.x;
				s16 posY_OnCam = curEntity->posInt.y-cameraPosition.y;
				curEntity->spr = SPR_addSpriteSafe(curEntity->sprDef, posX_OnCam, posY_OnCam, TILE_ATTR(curEntity->pal, 11, FALSE, FALSE));
				curEntity->spr->data = findSprOptIndexBySpriteDefinition(curEntity->sprDef);
				if(curEntity->spr->data != 9999){
					SPR_setAutoTileUpload(curEntity->spr, FALSE);
					curEntity->spr->data = findSprOptIndexBySpriteDefinition(curEntity->sprDef);
					SPR_setFrameChangeCallback(curEntity->spr, &frameChangedSprOpt1);
				}
			}
		}
	}
	//Return bullet sprites
	EntityBulletMerged *curBullet;
	for(u16 i=0; i<curEntityAll->EntityBulletMerged_size; i++){
		curBullet = &curEntityAll->EntityBulletMerged_arr[i];
		if(curBullet->onScreen && curBullet->alive){
			if(curBullet->sprDef) {
				s16 posX_OnCam = curBullet->posInt.x-cameraPosition.x;
				s16 posY_OnCam = curBullet->posInt.y-cameraPosition.y;
				curBullet->spr = SPR_addSpriteSafe(curBullet->sprDef, posX_OnCam, posY_OnCam, TILE_ATTR(curBullet->pal, 11, FALSE, FALSE));
				curBullet->spr->data = findSprOptIndexBySpriteDefinition(curBullet->sprDef);
				if(curBullet->spr->data != 9999){
					SPR_setAutoTileUpload(curBullet->spr, FALSE);
					curBullet->spr->data = findSprOptIndexBySpriteDefinition(curBullet->sprDef);
					SPR_setFrameChangeCallback(curBullet->spr, &frameChangedSprOpt1);
				}
			}
		}
	}
}

void resumeLevel() {
	VDP_resetScreen();
	VDP_setPlaneSize(64,32,TRUE);
	VDP_setScrollingMode(HSCROLL_PLANE, VSCROLL_PLANE);
	SPR_reset();

	PAL_setColors(0, palette_black, 64, DMA);
	SYS_doVBlankProcess();

	VDPTilesFilled = TILE_USER_INDEX;

	playerInit(playerBody.globalPosition);

	Palette* tempBgaPal;
	Palette* tempBgbPal;
	
	if(curLvlData->foregroundTileset != NULL){
		tempBgaPal = curLvlData->foregroundPallete;
		//memcpy(&palette_full[16],  curLvlData->pal1->data, 16 * 2);
		VDP_loadTileSet(curLvlData->foregroundTileset, VDPTilesFilled, CPU);
		bga = MAP_create(curLvlData->foregroundMap, BG_A, TILE_ATTR_FULL(curLvlData->bgaPalIndex, FALSE, FALSE, FALSE, VDPTilesFilled));
		VDPTilesFilled += curLvlData->foregroundTileset->numTile;
	} else {
		bga = NULL;
	}
	
	if(curLvlData->backgroundTileset != NULL){
		tempBgbPal = curLvlData->backgroundPallete;
		//memcpy(&palette_full[0],  curLvlData->pal0->data, 16 * 2);
		VDP_loadTileSet(curLvlData->backgroundTileset, VDPTilesFilled, CPU);
		bgb = MAP_create(curLvlData->backgroundMap, BG_B, TILE_ATTR_FULL(curLvlData->bgbPalIndex, FALSE, FALSE, FALSE, VDPTilesFilled));
		VDPTilesFilled += curLvlData->backgroundTileset->numTile;
	} else {
		bgb = NULL;
	}

	if(curLvlData->foregroundImage != NULL){
		tempBgaPal = curLvlData->foregroundImage->palette;
		//memcpy(&palette_full[16],  curLvlData->pal1->data, 16 * 2);
		VDP_drawImageEx(BG_A, curLvlData->foregroundImage, TILE_ATTR_FULL(curLvlData->bgaPalIndex, FALSE, FALSE, FALSE, VDPTilesFilled), 0, 0, FALSE, FALSE);
		VDPTilesFilled += curLvlData->foregroundImage->tileset->numTile;
	}
	
	if(curLvlData->backgroundImage != NULL){
		tempBgbPal = curLvlData->backgroundImage->palette;
		//memcpy(&palette_full[0],  curLvlData->pal0->data, 16 * 2);
		VDP_drawImageEx(BG_B, curLvlData->backgroundImage, TILE_ATTR_FULL(curLvlData->bgbPalIndex, FALSE, FALSE, FALSE, VDPTilesFilled), 0, 0, FALSE, FALSE);
		VDPTilesFilled += curLvlData->backgroundImage->tileset->numTile;
	}

	//Apply bga pal to whatewer pal_index you choose
	memcpy(&palette_full[curLvlData->bgaPalIndex*16], tempBgaPal->data, 16 * 2);
	//Apply bgb pal to whatewer pal_index you choose
	memcpy(&palette_full[curLvlData->bgbPalIndex*16], tempBgbPal->data, 16 * 2);

	//But, if you want to customize pal, then rewriting that.
	if(curLvlData->pal0 != NULL){
		memcpy(&palette_full[0], curLvlData->pal0->data, 16 * 2);
	}
	if(curLvlData->pal1 != NULL){
		memcpy(&palette_full[16], curLvlData->pal1->data, 16 * 2);
	}
	if(curLvlData->pal2 != NULL){
		memcpy(&palette_full[32], curLvlData->pal2->data, 16 * 2);
	}
	if(curLvlData->pal3 != NULL){
		memcpy(&palette_full[48], curLvlData->pal3->data, 16 * 2);
	}

	PAL_fadeIn(0, 63, palette_full, 5, TRUE);

	//If music is exists
	if(curLvlData->music != NULL) {
		//If music is not playing
		if((curMusic != curLvlData->music) || curLvlData->freshMusicStart){
			switch(curLvlData->musicMode){
				case 0: //XGM
					XGM_startPlay(curLvlData->music);
					break;
				case 1: //4PCM
					SND_startPlay_4PCM_ENV(curLvlData->music, curLvlData->musicSizeof, curLvlData->pcmChannel, curLvlData->musicLoop);
					break;
				case 2: //2ADPCM
					SND_startPlay_2ADPCM(curLvlData->music, curLvlData->musicSizeof, curLvlData->pcmChannel, curLvlData->musicLoop);
					break;
				case 3: //PCM
					SND_startPlay_PCM(curLvlData->music, curLvlData->musicSizeof, SOUND_RATE_32000, SOUND_PAN_CENTER, curLvlData->musicLoop);
					break;
			}
		}
		curMusic = curLvlData->music;
	}

	reloadLevelSprites();

	KLog_U1("FreeMem: ", MEM_getFree());

}

void loadLevel(u16 levelNum, Vect2D_s32 startPos) {
	//Reset everything
	VDP_resetScreen();
	
	// VDP_setScreenWidth256();
	// VDP_setScreenHeight240();

	VDP_setPlaneSize(64,32,TRUE);
	VDP_setScrollingMode(HSCROLL_PLANE, VSCROLL_PLANE);
	SPR_reset();
	SYS_doVBlankProcess();

	//Deallocate prev entityData to avoid memory leak
	deallocLevel();
	allocLevel();

	// VDP_loadTileSet(&ts_font_default1, TILE_FONT_INDEX, DMA);
	VDP_loadFont(&ts_font_1, CPU);

	// memcpy(&palette_full[0], palette_black, 64 * 2);
	// PAL_setColors(0, palette_full, 64, DMA);
	
	SYS_doVBlankProcess();

	if(curLvlData->beforeLevelFunc != NULL){
		(*curLvlData->beforeLevelFunc)();
	}

	VDPTilesFilled = TILE_USER_INDEX;
	
	if(curLvlData->levelMode == 0){
		playerInit(startPos, &playerBody);
		if(twoPlayers){
			playerInit(startPos, &playerBody2);
		}
	} else {
		playerBody.globalPosition.x = 0;
		playerBody.globalPosition.y = 0;

	}

	cameraPosition2.x = playerBody.globalPosition.x-160;
	cameraPosition2.y = playerBody.globalPosition.y-112;

	customScriptArr[curLvlData->updateCameraScript]();
	
	Palette* tempBgaPal;
	Palette* tempBgbPal;

	bga = NULL;
	bgb = NULL;
	
	// 0 - map mode
	// 1 - map mode (16Tile)
	// 2 - image mode

	switch(curLvlData->bgaLoadMode){
		case 0:
			if(curLvlData->foregroundTileset != NULL){
				tempBgaPal = curLvlData->foregroundPallete;
				//memcpy(&palette_full[16],  curLvlData->pal1->data, 16 * 2);
				VDP_loadTileSet(curLvlData->foregroundTileset, VDPTilesFilled, CPU);
				bga = MAP_create(curLvlData->foregroundMap, BG_A, TILE_ATTR_FULL(curLvlData->bgaPalIndex, FALSE, FALSE, FALSE, VDPTilesFilled));
				VDPTilesFilled += curLvlData->foregroundTileset->numTile;
				//Apply bga pal to whatewer pal_index you choose
				memcpy(&palette_full[curLvlData->bgaPalIndex*16], tempBgaPal->data, 16 * 2);
			}
			break;
		case 2:
			if(curLvlData->foregroundTileset != NULL){
				//memcpy(&palette_full[16],  curLvlData->pal1->data, 16 * 2);
				VDP_loadTileSet(curLvlData->foregroundTileset, VDPTilesFilled, CPU);
				bga = MAP_create(curLvlData->foregroundMap, BG_A, TILE_ATTR_FULL(curLvlData->bgaPalIndex, FALSE, FALSE, FALSE, VDPTilesFilled));
				VDPTilesFilled += curLvlData->foregroundTileset->numTile;

				memcpy(&palette_full[0], curLvlData->foregroundPallete->data, 64 * 2);
			}
			break;
		case 1:
			if(curLvlData->foregroundImage != NULL){
				tempBgaPal = curLvlData->foregroundImage->palette;
				//memcpy(&palette_full[16],  curLvlData->pal1->data, 16 * 2);
				VDP_drawImageEx(BG_A, curLvlData->foregroundImage, TILE_ATTR_FULL(curLvlData->bgaPalIndex, FALSE, FALSE, FALSE, VDPTilesFilled), 0, 0, FALSE, FALSE);
				VDPTilesFilled += curLvlData->foregroundImage->tileset->numTile;
				//Apply bga pal to whatewer pal_index you choose
				memcpy(&palette_full[curLvlData->bgaPalIndex*16], tempBgaPal->data, 16 * 2);
			}
			break;
	}
	SYS_doVBlankProcess();
	switch(curLvlData->bgbLoadMode){
		case 0:
			if(curLvlData->backgroundTileset != NULL){
				tempBgbPal = curLvlData->backgroundPallete;
				//memcpy(&palette_full[0],  curLvlData->pal0->data, 16 * 2);
				VDP_loadTileSet(curLvlData->backgroundTileset, VDPTilesFilled, CPU);
				bgb = MAP_create(curLvlData->backgroundMap, BG_B, TILE_ATTR_FULL(curLvlData->bgbPalIndex, FALSE, FALSE, FALSE, VDPTilesFilled));
				VDPTilesFilled += curLvlData->backgroundTileset->numTile;
				//Apply bgb pal to whatewer pal_index you choose
				memcpy(&palette_full[curLvlData->bgbPalIndex*16], tempBgbPal->data, 16 * 2);
			}
			
			break;
		case 2:
			if(curLvlData->backgroundTileset != NULL){
				//memcpy(&palette_full[0],  curLvlData->pal0->data, 16 * 2);
				VDP_loadTileSet(curLvlData->backgroundTileset, VDPTilesFilled, CPU);
				bgb = MAP_create(curLvlData->backgroundMap, BG_B, TILE_ATTR_FULL(curLvlData->bgbPalIndex, FALSE, FALSE, FALSE, VDPTilesFilled));
				VDPTilesFilled += curLvlData->backgroundTileset->numTile;

				memcpy(&palette_full[0], curLvlData->backgroundPallete->data, 64 * 2);
			}
			break;
		case 1:
			if(curLvlData->backgroundImage != NULL){
				tempBgbPal = curLvlData->backgroundImage->palette;
				//memcpy(&palette_full[0],  curLvlData->pal0->data, 16 * 2);
				VDP_drawImageEx(BG_B, curLvlData->backgroundImage, TILE_ATTR_FULL(curLvlData->bgbPalIndex, FALSE, FALSE, FALSE, VDPTilesFilled), 0, 0, FALSE, FALSE);
				VDPTilesFilled += curLvlData->backgroundImage->tileset->numTile;
				//Apply bgb pal to whatewer pal_index you choose
				memcpy(&palette_full[curLvlData->bgbPalIndex*16], tempBgbPal->data, 16 * 2);
			}
			break;
	}

	SYS_doVBlankProcess();

	//But, if you want to customize pal, then rewriting that.
	if(curLvlData->pal0 != NULL){
		memcpy(&palette_full[0], curLvlData->pal0->data, 16 * 2);
	}
	if(curLvlData->pal1 != NULL){
		memcpy(&palette_full[16], curLvlData->pal1->data, 16 * 2);
	}
	if(curLvlData->pal2 != NULL){
		memcpy(&palette_full[32], curLvlData->pal2->data, 16 * 2);
	}
	if(curLvlData->pal3 != NULL){
		memcpy(&palette_full[48], curLvlData->pal3->data, 16 * 2);
	}

	switch(curLvlData->changeLevelAnim){
		case 0:
			PAL_setColors(0, palette_full, 63, CPU);
			// PAL_fadeIn(0, 63, palette_full, 5, TRUE);
			break;
		case 1:
			PAL_fadeIn(0, 63, palette_full, 5, TRUE);
			break;
	}
	
	updateRoomSize(curLvlData);

	//Music mode reminder
	// 0 - XGM driver (default), can play VGM music and Wav 14 Khz sample.
	// 1 - WAV 4PCM Driver, 4 channels driver 16 Khz, good driver if you want to play WAV background music.
	// 2 - WAV 2ADPCM 2 channels driver 22050 Hz.
	// 3 - PCM, 1 channel driver, best quality 8-32 KHz, but it's just stupid, 32 KHz music takes too much ROM. And you only have 4 MB (unless you using a crunch, i mean, bank switch).

	//If music is exists
	if(curLvlData->music != NULL) {
		//If music is not playing
		if((curMusic != curLvlData->music) || curLvlData->freshMusicStart){
			switch(curLvlData->musicMode){
				case 0: //XGM
					XGM_startPlay(curLvlData->music);
					break;
				case 1: //4PCM
					SND_startPlay_4PCM_ENV(curLvlData->music, curLvlData->musicSizeof, curLvlData->pcmChannel, curLvlData->musicLoop);
					break;
				case 2: //2ADPCM
					SND_startPlay_2ADPCM(curLvlData->music, curLvlData->musicSizeof, curLvlData->pcmChannel, curLvlData->musicLoop);
					break;
				case 3: //PCM
					SND_startPlay_PCM(curLvlData->music, curLvlData->musicSizeof, SOUND_RATE_32000, SOUND_PAN_CENTER, curLvlData->musicLoop);
					break;
			}
		}
		curMusic = curLvlData->music;
	}

	setupCamera(newVector2D_u16(160, 112), 20, 20);
	

	if(curLvlData->afterLevelFunc != NULL){
		(*curLvlData->afterLevelFunc)();
	}
	
	//$loadScreenChunks$

	preloadSpriteTiles();
	
	

	KLog_U1("FreeMem: ", MEM_getFree());
}
