#include "../inc/player.h"
#include "../inc/updateAnimations.h"

#include "../inc/global.h"
#include "../inc/maps.h"



void updateAnimations() {
	//Sprite flip depending on the horizontal input
	if (playerBody.input.x > 0) {
		SPR_setHFlip(playerBody.sprite, TRUE);
		playerBody.facingDirection = 1;
	}else if (playerBody.input.x < 0) {
		SPR_setHFlip(playerBody.sprite, FALSE);
		playerBody.facingDirection = -1;
	}

	switch(playerBody.animMode){
		case 0: //default mode
			if (playerBody.velocity.fixY == 0 && !playerBody.climbingStair) {
				if (playerBody.velocity.x != 0 && playerBody.onGround) {
					curPlayerAnim = 0;
					SPR_setAnim(playerBody.sprite, curPlayerAnim); //player run
				}else if (playerBody.velocity.x == 0 && playerBody.onGround) {
					curPlayerAnim = 1;
					SPR_setAnim(playerBody.sprite, curPlayerAnim); //player idle
				}
			}
			break;
		case 1: //Damaged
			curPlayerAnim = 3;
			SPR_setAnim(playerBody.sprite, curPlayerAnim); //damaged anim
			playerBody.animModeChangable = TRUE;
			break;
		case 2: // Cannon fire
			curPlayerAnim = 2;
			SPR_setAnim(playerBody.sprite, curPlayerAnim);
			if((playerBody.sprite->frameInd == playerBody.sprite->animation->numFrame-1) && (playerBody.sprite->timer == 1)) {
				playerBody.animMode = 0;
				playerBody.animModeChangable = TRUE;
			}
			break;
			//&& (playerBody.sprite->timer == 1)
			//playerBody.attack = FALSE;
	}
	
	//Climb animation
	if (playerBody.climbingStair) {
		SPR_setAnim(playerBody.sprite, 2);
	}
}