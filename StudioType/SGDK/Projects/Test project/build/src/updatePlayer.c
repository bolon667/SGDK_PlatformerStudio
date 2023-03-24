#include "../inc/player.h"
#include "../inc/updatePlayer.h"

#include "../inc/global.h"
#include "../inc/maps.h"

void updatePlayer() {
	//Check if the player wants to climb a stair
	if(collidingAgainstStair && ((playerBody.onStair && playerBody.input.y > 0) || (!playerBody.onStair && playerBody.input.y < 0))){
		playerBody.climbingStair = TRUE;
		playerBody.velocity.fixY = FIX16(playerBody.climbingSpeed * playerBody.input.y);
	}

	//Check if player wants to jump by looking the coyote time and jump buffer
	if(playerBody.curAmountOfJumps > 0){
		if(currentJumpBufferTime > 0){
		currentCoyoteTime = 0;
		currentJumpBufferTime = 0;
		playerBody.curAmountOfJumps--;
		playerBody.jumping = TRUE;
			//Play the SFX with the index 64 (jump sfx) with the highest priority
		XGM_startPlayPCM(64, 15, SOUND_PCM_CH1);
		playerBody.velocity.fixY = FIX16(-playerBody.jumpSpeed);
		}
	}
	//The ground hasn't been checked yet so we only decrease the jump buffer time for now
	currentJumpBufferTime = clamp((currentJumpBufferTime - 1), 0, jumpBufferTime); //Clamp to avoid underflowing, it is unlikely to happen but can happen

	//If the player is climbing a stair, it only needs to go upward, if not, we apply horizontal movement
	if (playerBody.climbingStair) {
		playerBody.velocity.x = playerBody.velocity.fixX = 0;
		playerBody.globalPosition.x = stairLeftEdge - stairPositionOffset;
	}else {
		if (playerBody.input.x > 0) {
			if (playerBody.velocity.x != playerBody.speed)
				playerBody.velocity.fixX += playerBody.acceleration;
		}else if (playerBody.input.x < 0) {
			if (playerBody.velocity.x != -playerBody.speed)
				playerBody.velocity.fixX -= playerBody.acceleration;
		}else if (playerBody.onGround) {
			if (playerBody.velocity.x > 0)
				playerBody.velocity.fixX -= playerBody.deceleration;
			else if (playerBody.velocity.x < 0)
				playerBody.velocity.fixX += playerBody.deceleration;
			else
				playerBody.velocity.fixX = 0;
		}
		playerBody.velocity.x = clamp(fix16ToInt(playerBody.velocity.fixX), -playerBody.speed, playerBody.speed);
	}

	//Apply gravity with a terminal velocity
	if (!playerBody.onGround && !playerBody.climbingStair) { // && !playerBody.onPlatform
		if (fix16ToInt(playerBody.velocity.fixY) <= playerBody.maxFallSpeed) {
			playerBody.velocity.fixY = fix16Add(playerBody.velocity.fixY, gravityScale);
		}else {
			playerBody.velocity.fixY = FIX16(playerBody.maxFallSpeed);
		}
	}

	//Once all the input-related have been calculated, we apply the velocities to the global positions
	playerBody.globalPosition.x += playerBody.velocity.x;
	playerBody.globalPosition.y += fix16ToInt(playerBody.velocity.fixY);

	//Now we can check for collisions and correct those positions
	checkCollisions();

	//Now that the collisions have been checked, we know if the player is on a stair or not
	if (!collidingAgainstStair && playerBody.climbingStair) {
		playerBody.climbingStair = FALSE;
		playerBody.input.y = 0;
	}

	//Once the positions are correct, we position the player taking into account the camera position
	playerBody.position.x = playerBody.globalPosition.x - cameraPosition.x;
	playerBody.position.y = playerBody.globalPosition.y - cameraPosition.y;
	SPR_setPosition(playerBody.sprite, playerBody.position.x, playerBody.position.y);
	SPR_setPosition(playerBody.debugSpr1, playerBody.position.x+playerBody.aabb.min.x, playerBody.position.y+playerBody.aabb.min.y);
SPR_setPosition(playerBody.debugSpr2, playerBody.position.x+playerBody.aabb.max.x-8, playerBody.position.y+playerBody.aabb.max.y-8);

	

	//Reset when falling off the screen
	if (playerBody.falling) {
		dyingSteps++;
		if(dyingSteps > dieDelay){
			SYS_hardReset();
		}
	}
}