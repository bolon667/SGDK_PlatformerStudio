#include "../inc/levels.h"
#include "../inc/loadLevel.h"
#include "../inc/deallocLevel.h"
#include "../inc/allocLevel.h"


#include "../inc/maps.h"
#include "../inc/global.h"
#include "../inc/player.h"
#include "../inc/customScripts.h"

#include "../res/resources.h"
#include "../res/gfx.h"
#include "../res/sprites.h"
#include "../res/sounds.h"
#include "../res/music.h"
#include "../res/images.h"


void loadLevel(u16 levelNum, Vect2D_s16 startPos) {

	//Reset everything
	VDP_resetScreen();
	VDP_setPlaneSize(64,32,TRUE);
	VDP_setScrollingMode(HSCROLL_PLANE, VSCROLL_PLANE);
	SPR_reset();
	SYS_doVBlankProcess();

	//Deallocate prev entityData to avoid memory leak
	deallocLevel();
	allocLevel();

	PAL_setColors(0, palette_black, 64, DMA);
	SYS_doVBlankProcess();

	VDPTilesFilled = TILE_USER_INDEX;

	
	
	if(curLvlData->beforeLevelFunc != NULL){
		(*curLvlData->beforeLevelFunc)();
	}
	if(curLvlData->levelMode == 0){
		playerInit((Vect2D_s16)startPos);
	} else {
		playerBody.globalPosition.x = 0;
		playerBody.globalPosition.y = 0;

	}

	customScriptArr[curLvlData->updateCameraScript]();
	
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

	// memcpy(&palette_full[PAL2*16], curLvlData->pal2->data, 16 * 2);
	
	// if(curLvlData->pal3 == NULL){
	// 	curLvlData->pal3 = spr_coin.palette;
	// }
	
	// memcpy(&palette_full[PAL3*16], curLvlData->pal3->data, 16 * 2);

	//addNewBullet((Bullet){TRUE, {2,2}, {0,0}, {FIX16(0.5),0}, {32,32}, {0,0,32,32}, FALSE, 1, NULL, &spr_noSpr});
	
	//memcpy(&palette_full[32],  spr_Enemy_sponge.palette->data, 16 * 2);
	//memcpy(&palette_full[48],  plr.sprite->definition->palette->data, 16 * 2);

	//Update the number of tiles filled in order to avoid overlaping them when loading more

	//                      |||
	//wtf is THAT?!?!?      VVV
	// VDPTilesFilled += level_tileset.numTile;

	PAL_fadeIn(0, 63, palette_full, 5, TRUE);

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
	

	if(curLvlData->afterLevelFunc != NULL){
		(*curLvlData->afterLevelFunc)();
	}
	

	KLog_U1("FreeMem: ", MEM_getFree());
}
