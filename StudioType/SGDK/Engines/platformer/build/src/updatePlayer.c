#include "../inc/player.h"
#include "../inc/updatePlayer.h"

#include "../inc/global.h"
#include "../inc/maps.h"

//Platformer Engine

void updatePlayerBase(pBody* plr, u16 joy) {
	//Check if the player wants to climb a stair
	if(collidingAgainstStair && ((plr->onStair && plr->input.y > 0) || (!plr->onStair && plr->input.y < 0))){
		plr->climbingStair = TRUE;
		plr->velocity.fixY = FIX16(plr->climbingSpeed * plr->input.y);
	}

	//Check if player wants to jump by looking the coyote time and jump buffer
	if(plr->curAmountOfJumps > 0){
		if(currentJumpBufferTime > 0){
		currentCoyoteTime = 0;
		currentJumpBufferTime = 0;
		plr->curAmountOfJumps--;
		plr->jumping = TRUE;
			//Play the SFX with the index 64 (jump sfx) with the highest priority
		
		//Commented because, if you not using default XGM driver, you will get a ear damage. It's really DANGEROUS you can get partially deaf in real life.
		// XGM_startPlayPCM(64, 15, SOUND_PCM_CH1);
		plr->velocity.fixY = FIX16(-plr->jumpSpeed);
		}
	}
	//The ground hasn't been checked yet so we only decrease the jump buffer time for now
	currentJumpBufferTime = clamp((currentJumpBufferTime - 1), 0, jumpBufferTime); //Clamp to avoid underflowing, it is unlikely to happen but can happen

	//If the player is climbing a stair, it only needs to go upward, if not, we apply horizontal movement
	if (plr->climbingStair) {
		plr->velocity.x = plr->velocity.fixX = 0;
		plr->globalPosition.x = stairLeftEdge - stairPositionOffset;
	}else {
		if (plr->input.x > 0) {
			if (plr->velocity.x != plr->speed)
				plr->velocity.fixX += plr->acceleration;
		}else if (plr->input.x < 0) {
			if (plr->velocity.x != -plr->speed)
				plr->velocity.fixX -= plr->acceleration;
		}else if (plr->onGround) {
			if (plr->velocity.x > 0)
				plr->velocity.fixX -= plr->deceleration;
			else if (plr->velocity.x < 0)
				plr->velocity.fixX += plr->deceleration;
			// else
			// 	plr->velocity.fixX = 0;
		}
		plr->velocity.x = clamp(fix16ToInt(plr->velocity.fixX), -plr->speed, plr->speed);
	}

	//Apply gravity with a terminal velocity
	if (!plr->onGround && !plr->climbingStair) { // && !plr->onPlatform
		if (fix16ToInt(plr->velocity.fixY) <= plr->maxFallSpeed) {
			plr->velocity.fixY = fix16Add(plr->velocity.fixY, gravityScale);
		}else {
			plr->velocity.fixY = FIX16(plr->maxFallSpeed);
		}
	}

	//Once all the input-related have been calculated, we apply the velocities to the global positions
	plr->globalPosition.x += plr->velocity.x + fix16ToInt(plr->velocity.bufferFixX);
	plr->globalPosition.y += fix16ToInt(plr->velocity.fixY + plr->velocity.bufferFixY);

	plr->velocity.bufferFixX = plr->velocity.bufferFixX - FIX16(fix16ToInt(plr->velocity.bufferFixX));
	plr->velocity.bufferFixY = plr->velocity.bufferFixY - FIX16(fix16ToInt(plr->velocity.bufferFixY));

	//Now we can check for collisions and correct those positions
	checkCollisions(plr);

	//Checking trigger collision with player
	for(u16 i=0; i<curEntityAll->Trigger_size; i++){
		checkTriggerForPlayer(&curEntityAll->Trigger_arr[i], plr);
	}

	//Now that the collisions have been checked, we know if the player is on a stair or not
	if (!collidingAgainstStair && plr->climbingStair) {
		plr->climbingStair = FALSE;
		plr->input.y = 0;
	}

	//Once the positions are correct, we position the player taking into account the camera position
	plr->position.x = plr->globalPosition.x - cameraPosition.x;
	plr->position.y = plr->globalPosition.y - cameraPosition.y;
	//if player outside camera
	if ((plr->position.x > 320) || (plr->position.x < -plr->aabb.max.x) || (plr->position.y > 224) || (plr->position.y < -plr->aabb.max.y)){
		SPR_setVisibility(plr->sprite, 1);
		
	} else {
		SPR_setVisibility(plr->sprite, 0);
		SPR_setPosition(plr->sprite, plr->position.x, plr->position.y);
	}
	
	//$showTriggerRects$
	
	updateAnimations(plr);
	playerInputChanged(plr, joy);
}

void updatePlayer() {
	updatePlayerBase(&playerBody, JOY_1);
	if(twoPlayers){
		updatePlayerBase(&playerBody2, JOY_2);
	}
}