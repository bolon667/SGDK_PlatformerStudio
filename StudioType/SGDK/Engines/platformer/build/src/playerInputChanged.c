#include "../inc/player.h"
#include "../inc/playerInputChanged.h"

#include "../inc/global.h"
#include "../inc/maps.h"

#include "../res/sounds.h"
#include "../res/sprites.h"
#include "../res/resources.h"

//Platformer Engine

void playerInputChanged(pBody* plr, u16 joy) {
	plr->inputStates.prevState = plr->inputStates.state;
	plr->inputStates.state = JOY_readJoypad(joy);

	if(plr->damaged) {
		if(plr->dead) {
			plr->input.x = 0;
			return;
		}
		
		switch(plr->state){
			case 0:
				if(plr->onGround){
					plr->onGround = FALSE;
					// plr->globalPosition.y -=1;
					// if(facingRight){
						
					// 	plr->velocity.fixX = FIX16(10);
					// 	plr->velocity.x = 10;
					// } else {
					// 	plr->velocity.fixX = FIX16(-10);
					// 	plr->velocity.x = -10;
					// }
					// // plr->plr->inputStates.x = plr->facingDirection;
					plr->velocity.fixY = FIX16(-5);
					
					plr->state = 1;
				}
				break;
			case 1:
				if(plr->onGround){
					plr->damaged = FALSE;
				} 
				break;
		}

		return;
	}

	//We only read data from the joypad 1
	
		//Update x velocity
		if (plr->inputStates.state & BUTTON_RIGHT) {
			plr->input.x = 1;
		} else if (plr->inputStates.state & BUTTON_LEFT) {
			plr->input.x = -1;
		} else {
			plr->input.x = 0;
		}

		plr->semiSolidHasCollision = TRUE;
		//Down and up buttons are only used when it is climbing stair
		//NOTE: Up direction is -1 and down direction is 1, this is because the Mega Drive draws the screen from top to bottom
		if (plr->inputStates.state & BUTTON_DOWN) {
			plr->input.y = 1;
			if(plr->onSemiSolid && (plr->inputStates.state & (BUTTON_A | BUTTON_B)) && !(plr->inputStates.prevState & (BUTTON_A | BUTTON_B))){
				plr->semiSolidHasCollision = FALSE;
				plr->globalPosition.y += 4;
			}
			// if (changed & BUTTON_DOWN) {
				
			// 	if (plr->climbingStair) {
			// 		plr->velocity.fixY = FIX16(plr->climbingSpeed);
			// 	}else if (plr->onStair) {
			// 		plr->velocity.fixY = FIX16(plr->climbingSpeed);
			// 		plr->climbingStair = TRUE;
			// 	}
			// }else {
			// 	plr->input.y = 0;
			// 	if (plr->climbingStair) {
			// 		plr->velocity.fixY = 0;
			// 	}
			// }
		} 
		// return;
		else if ((plr->inputStates.state & (BUTTON_A | BUTTON_B)) && !(plr->inputStates.prevState & (BUTTON_A | BUTTON_B))) {
			if (plr->climbingStair) {
					plr->climbingStair = FALSE;
			} else {
				currentJumpBufferTime = jumpBufferTime;
			}
		}
		if((plr->inputStates.state & BUTTON_C) && !(plr->inputStates.prevState & BUTTON_C)){
			//Attack logic, change for your needs
			if(TRUE){ //curPlayerAnim != 0
			plr->attack = TRUE;
			Vect2D_s32 pos;
			if(facingRight){
				pos = plr->globalPosition;
			} else {
				pos = plr->globalPosition;
				pos.x += 32;
			}
			//addNew_DefaultBullet(pos, (Vect2D_f16){0,0});
			}
		} else if((plr->inputStates.prevState & BUTTON_C) && !(plr->inputStates.state & BUTTON_C)) {
			//If the button is released we remove half of the velocity
			plr->velocity.fixY = fix16Mul(plr->velocity.fixY, FIX16(.5));
		}

		if (plr->inputStates.state & BUTTON_UP) {
			plr->input.y = -1;
			if (collidingAgainstStair && !plr->onStair) {
				plr->climbingStair = TRUE;
				plr->velocity.fixY = FIX16(-plr->climbingSpeed);
			}
		}else {
			plr->input.y = 0;
			if (plr->climbingStair) {
				plr->velocity.fixY = 0;
			}
		}
}