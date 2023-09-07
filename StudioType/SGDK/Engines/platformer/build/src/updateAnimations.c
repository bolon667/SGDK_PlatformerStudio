#include "../inc/player.h"
#include "../inc/updateAnimations.h"

#include "../inc/global.h"
#include "../inc/maps.h"
#include "../inc/physics.h"



void updateAnimations(pBody* plr) {
	//Sprite flip depending on the horizontal input
	if (plr->input.x > 0) {
		SPR_setHFlip(plr->sprite, TRUE);
		plr->facingDirection = 1;
	}else if (plr->input.x < 0) {
		SPR_setHFlip(plr->sprite, FALSE);
		plr->facingDirection = -1;
	}

	//If the player is on ground and not climbing the stair it can be idle or running
	if (plr->velocity.fixY == 0 && !plr->climbingStair) {
		if (plr->velocity.x != 0 && runningAnim == FALSE && plr->onGround) {
			SPR_setAnim(plr->sprite, 1);
			runningAnim = TRUE;
		}else if (plr->velocity.x == 0 && plr->onGround) {
			SPR_setAnim(plr->sprite, 0);
			runningAnim = FALSE;
		}
	}

	//Climb animation
	if (plr->climbingStair) {
		SPR_setAnim(plr->sprite, 2);
	}
}