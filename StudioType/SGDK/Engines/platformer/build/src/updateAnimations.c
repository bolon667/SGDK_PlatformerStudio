#include "../inc/player.h"
#include "../inc/updateAnimations.h"

#include "../inc/global.h"
#include "../inc/maps.h"
#include "../inc/physics.h"



void updateAnimations() {
	//Sprite flip depending on the horizontal input
	if (playerBody.input.x > 0) {
		SPR_setHFlip(playerBody.sprite, TRUE);
		playerBody.facingDirection = 1;
	}else if (playerBody.input.x < 0) {
		SPR_setHFlip(playerBody.sprite, FALSE);
		playerBody.facingDirection = -1;
	}

	//If the player is on ground and not climbing the stair it can be idle or running
	if (playerBody.velocity.fixY == 0 && !playerBody.climbingStair) {
		if (playerBody.velocity.x != 0 && runningAnim == FALSE && playerBody.onGround) {
			SPR_setAnim(playerBody.sprite, 1);
			runningAnim = TRUE;
		}else if (playerBody.velocity.x == 0 && playerBody.onGround) {
			SPR_setAnim(playerBody.sprite, 0);
			runningAnim = FALSE;
		}
	}

	//Climb animation
	if (playerBody.climbingStair) {
		SPR_setAnim(playerBody.sprite, 2);
	}
}