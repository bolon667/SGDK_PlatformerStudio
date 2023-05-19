#include "../inc/player.h"
#include "../inc/playerInputChanged.h"

#include "../inc/global.h"
#include "../inc/maps.h"

#include "../res/sounds.h"
#include "../res/sprites.h"
#include "../res/resources.h"

void playerInputChanged() {
	input.prevState = input.state;
	input.state = JOY_readJoypad(JOY_1);

	if(playerBody.damaged) {
		if(playerBody.dead) {
			playerBody.input.x = 0;
			return;
		}
		
		switch(playerBody.state){
			case 0:
				if(playerBody.onGround){
					playerBody.onGround = FALSE;
					// playerBody.globalPosition.y -=1;
					// if(facingRight){
						
					// 	playerBody.velocity.fixX = FIX16(10);
					// 	playerBody.velocity.x = 10;
					// } else {
					// 	playerBody.velocity.fixX = FIX16(-10);
					// 	playerBody.velocity.x = -10;
					// }
					// // playerBody.input.x = playerBody.facingDirection;
					playerBody.velocity.fixY = FIX16(-5);
					
					playerBody.state = 1;
				}
				break;
			case 1:
				if(playerBody.onGround){
					playerBody.damaged = FALSE;
				} 
				break;
		}

		return;
	}

	//We only read data from the joypad 1
	
		//Update x velocity
		if (input.state & BUTTON_RIGHT) {
			playerBody.input.x = 1;
		} else if (input.state & BUTTON_LEFT) {
			playerBody.input.x = -1;
		} else {
			playerBody.input.x = 0;
		}

		playerBody.semiSolidHasCollision = TRUE;
		//Down and up buttons are only used when it is climbing stair
		//NOTE: Up direction is -1 and down direction is 1, this is because the Mega Drive draws the screen from top to bottom
		if (input.state & BUTTON_DOWN) {
			playerBody.input.y = 1;
			if(playerBody.onSemiSolid && (input.state & (BUTTON_A | BUTTON_B)) && !(input.prevState & (BUTTON_A | BUTTON_B))){
				playerBody.semiSolidHasCollision = FALSE;
				playerBody.globalPosition.y += 4;
			}
			// if (changed & BUTTON_DOWN) {
				
			// 	if (playerBody.climbingStair) {
			// 		playerBody.velocity.fixY = FIX16(playerBody.climbingSpeed);
			// 	}else if (playerBody.onStair) {
			// 		playerBody.velocity.fixY = FIX16(playerBody.climbingSpeed);
			// 		playerBody.climbingStair = TRUE;
			// 	}
			// }else {
			// 	playerBody.input.y = 0;
			// 	if (playerBody.climbingStair) {
			// 		playerBody.velocity.fixY = 0;
			// 	}
			// }
		} 
		// return;
		else if ((input.state & (BUTTON_A | BUTTON_B)) && !(input.prevState & (BUTTON_A | BUTTON_B))) {
			if (playerBody.climbingStair) {
					playerBody.climbingStair = FALSE;
			} else {
				currentJumpBufferTime = jumpBufferTime;
			}
		}
		if((input.state & BUTTON_C) && !(input.prevState & BUTTON_C)){
			//Attack logic, change for your needs
			if(TRUE){ //curPlayerAnim != 0
			playerBody.attack = TRUE;
			Vect2D_s16 pos;
			if(facingRight){
				pos = playerBody.globalPosition;
			} else {
				pos = playerBody.globalPosition;
				pos.x += 32;
			}
			//addNew_DefaultBullet(pos, (Vect2D_f16){0,0});
			}
		} else if((input.prevState & BUTTON_C) && !(input.state & BUTTON_C)) {
			//If the button is released we remove half of the velocity
			playerBody.velocity.fixY = fix16Mul(playerBody.velocity.fixY, FIX16(.5));
		}

		if (input.state & BUTTON_UP) {
			playerBody.input.y = -1;
			if (collidingAgainstStair && !playerBody.onStair) {
				playerBody.climbingStair = TRUE;
				playerBody.velocity.fixY = FIX16(-playerBody.climbingSpeed);
			}
		}else {
			playerBody.input.y = 0;
			if (playerBody.climbingStair) {
				playerBody.velocity.fixY = 0;
			}
		}
}