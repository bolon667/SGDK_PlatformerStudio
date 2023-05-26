#include "../inc/player.h"
#include "../inc/updatePlayer.h"

#include "../inc/global.h"
#include "../inc/maps.h"

//Shmup Engine

void updatePlayer() {
	//Check if player wants to jump by looking the coyote time and jump buffer
	if(playerBody.curAmountOfJumps > 0){
		if(currentJumpBufferTime > 0){
		currentCoyoteTime = 0;
		currentJumpBufferTime = 0;
		playerBody.curAmountOfJumps--;
		playerBody.jumping = TRUE;
			//Play the SFX with the index 64 (jump sfx) with the highest priority
		
		//Commented because, if you not using default XGM driver, you will get a ear damage. It's really DANGEROUS you can get partially deaf in real life.
		// XGM_startPlayPCM(64, 15, SOUND_PCM_CH1);
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
		}else {
			if (playerBody.velocity.x > 0)
				playerBody.velocity.fixX -= playerBody.deceleration;
			else if (playerBody.velocity.x < 0)
				playerBody.velocity.fixX += playerBody.deceleration;
		}

		if (playerBody.input.y > 0) {
			if (playerBody.velocity.y != playerBody.speed)
				playerBody.velocity.fixY += playerBody.acceleration;
		}else if (playerBody.input.y < 0) {
			if (playerBody.velocity.y != -playerBody.speed)
				playerBody.velocity.fixY -= playerBody.acceleration;
		}else {
			if (playerBody.velocity.y > 0)
				playerBody.velocity.fixY -= playerBody.deceleration;
			else if (playerBody.velocity.y < 0)
				playerBody.velocity.fixY += playerBody.deceleration;
		}
		
		playerBody.velocity.x = fix16ToInt(playerBody.velocity.fixX);
		playerBody.velocity.y = fix16ToInt(playerBody.velocity.fixY);
		// playerBody.velocity.x = clamp(fix16ToInt(playerBody.velocity.fixX), -playerBody.speed, playerBody.speed);
		// playerBody.velocity.y = clamp(fix16ToInt(playerBody.velocity.fixY), -playerBody.speed, playerBody.speed);
	
		

	}

	if(playerBody.gravityOn){
		//Apply gravity with a terminal velocity
		if (!playerBody.onGround && !playerBody.climbingStair) { // && !playerBody.onPlatform
			if (fix16ToInt(playerBody.velocity.fixY) <= playerBody.maxFallSpeed) {
				playerBody.velocity.fixY = fix16Add(playerBody.velocity.fixY, gravityScale);
			}else {
				playerBody.velocity.fixY = FIX16(playerBody.maxFallSpeed);
			}
		}
	}
	

	//Once all the input-related have been calculated, we apply the velocities to the global positions
	playerBody.globalPosition.x += playerBody.velocity.x + fix16ToInt(playerBody.velocity.bufferFixX) + fix16ToInt(cameraSpd.x);
	playerBody.globalPosition.y += fix16ToInt(playerBody.velocity.fixY + playerBody.velocity.bufferFixY) + fix16ToInt(cameraSpd.y);

	playerBody.velocity.bufferFixX = playerBody.velocity.bufferFixX - FIX16(fix16ToInt(playerBody.velocity.bufferFixX));
	playerBody.velocity.bufferFixY = playerBody.velocity.bufferFixY - FIX16(fix16ToInt(playerBody.velocity.bufferFixY));

	//Now we can check for collisions and correct those positions
	checkCollisions();

	//Checking trigger collision with player
	for(u16 i=0; i<curEntityAll->Trigger_size; i++){
		checkTriggerForPlayer(&curEntityAll->Trigger_arr[i]);
	}

	//Once the positions are correct, we position the player taking into account the camera position
	playerBody.position.x = playerBody.globalPosition.x - cameraPosition.x;
	playerBody.position.y = playerBody.globalPosition.y - cameraPosition.y;
	SPR_setPosition(playerBody.sprite, playerBody.position.x, playerBody.position.y);
	//$showTriggerRects$
	
	updateAnimations();
	playerInputChanged();
}