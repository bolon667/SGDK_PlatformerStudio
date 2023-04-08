#include "../inc/player.h"
#include "../inc/checkCollisions.h"

#include "../inc/global.h"
#include "../inc/maps.h"


void checkCollisions_forEntityMerged(EntityMerged* entity){	
	AABB entityBounds = newAABB(
		entity->posInt.x + entity->trigger->rect.min.x,
		entity->posInt.x + entity->trigger->rect.max.x,
		entity->posInt.y + entity->trigger->rect.min.y,
		entity->posInt.y + entity->trigger->rect.max.y
	);

	entity->onGround = FALSE;

	//Create level limits
	AABB maxPosLimits = roomSize;

	//Positions in tiles
	Vect2D_u16 minTilePos = posToTile(newVector2D_s16(entityBounds.min.x, entityBounds.min.y));
	Vect2D_u16 maxTilePos = posToTile(newVector2D_s16(entityBounds.max.x, entityBounds.max.y));
	//Used to limit how many tiles we have to check for collision
	Vect2D_u16 tileBoundDifference = newVector2D_u16(maxTilePos.x - minTilePos.x, maxTilePos.y - minTilePos.y);

	s16 yIntVelocity = fix16ToRoundedInt(entity->spd.y);
	s16 entityHeadPos = entity->posInt.y - yIntVelocity  + entity->trigger->rect.min.y;
	s16 entityFeetPos = entity->posInt.y - yIntVelocity  + entity->trigger->rect.max.y;

	//First we check for horizontal collisions
	for (u16 i = 0; i <= tileBoundDifference.y; i++) {
		//Height position constant as a helper
		const u16 y = minTilePos.y + i;

		//Right position constant as a helper
		const u16 rx = maxTilePos.x;

		u16 rTileValue = getTileValue(rx, y);
		//After getting the tile value, we check if that is one of whom we can collide/trigger with horizontally
		if (rTileValue == GROUND_TILE) {
			AABB tileBounds = getTileBounds(rx, y);
			//Before taking that tile as a wall, we have to check if that is within the player hitbox, e.g. not seeing ground as a wall
			if (tileBounds.min.x < maxPosLimits.max.x && tileBounds.min.y < entityFeetPos && tileBounds.max.y > entityHeadPos) {
				maxPosLimits.max.x = tileBounds.min.x;
				break;
			}
		}

		//Left position constant as a helper
		const s16 lx = minTilePos.x;

		u16 lTileValue = getTileValue(lx, y);
		//We do the same here but for the left side
		if (lTileValue == GROUND_TILE) {
			AABB tileBounds = getTileBounds(lx, y);
			if (tileBounds.max.x > maxPosLimits.min.x && tileBounds.min.y < entityFeetPos && tileBounds.max.y > entityHeadPos) {
				maxPosLimits.min.x = tileBounds.max.x;
				break;
			}
		} 
	}

	
	//After checking for horizontal positions we can modify the positions if the player is colliding
	if (maxPosLimits.min.x > entityBounds.min.x) {
		// entity->posInt.x = maxPosLimits.min.x + entity->trigger->rect.min.x;
	
		entity->pos.x = FIX32(maxPosLimits.min.x - entity->trigger->rect.min.x);
		entity->spd.x = 0;
	}
	if (maxPosLimits.max.x < entityBounds.max.x) {
		// entity->posInt.x = maxPosLimits.max.x + entity->trigger->rect.max.x;
		entity->pos.x = FIX32(maxPosLimits.max.x - entity->trigger->rect.max.x-1);
		entity->spd.x = 0;
	}

	//And do the same to the variables that are used to check for them
	minTilePos = posToTile(newVector2D_s16(entityBounds.min.x, entityBounds.min.y));
	maxTilePos = posToTile(newVector2D_s16(entityBounds.max.x - 1, entityBounds.max.y));
	tileBoundDifference = newVector2D_u16(maxTilePos.x - minTilePos.x, maxTilePos.y - minTilePos.y);

	if (yIntVelocity >= 0) {
		for (u16 i = 0; i <= tileBoundDifference.x; i++) {
			s16 x = minTilePos.x + i;
			u16 y = maxTilePos.y;

			//This is the exact same method that we use for horizontal collisions
			u16 bottomTileValue = getTileValue(x, y);
			if (bottomTileValue == GROUND_TILE || bottomTileValue == ONE_WAY_PLATFORM_UP_TILE) {
				// if (getTileRightEdge(x) == maxPosLimits.min.x || getTileLeftEdge(x) == maxPosLimits.max.x)
				// 	continue;

				u16 bottomEdgePos = getTileTopEdge(y);
				if (bottomEdgePos < maxPosLimits.max.y) { // && bottomEdgePos >= (playerFeetPos - oneWayPlatformErrorCorrection)
					maxPosLimits.max.y = bottomEdgePos;
					
				}
				//entity->spd.y = 0;
			}
		}
	} else {
		for (u16 i = 0; i <= tileBoundDifference.x; i++) {
			s16 x = minTilePos.x + i;
			u16 y = minTilePos.y;

			//And the same once again
			u16 topTileValue = getTileValue(x, y);
			
			// || topTileValue == ONE_WAY_PLATFORM_DOWN_TILE
			if (topTileValue == GROUND_TILE) {
				// if (getTileRightEdge(x) == maxPosLimits.min.x || getTileLeftEdge(x) == maxPosLimits.max.x)
				// 	continue;

				u16 upperEdgePos;
				
				upperEdgePos = getTileBottomEdge(y);
				
				if (upperEdgePos < maxPosLimits.max.y) {
					maxPosLimits.min.y = upperEdgePos;
					break;
				}
				//entity->spd.y = 0;
				
			}
		}
	}

	if (maxPosLimits.min.y > entityBounds.min.y) {
		// entity->posInt.y = maxPosLimits.min.y - entity->trigger->rect.min.y;
		// entity->pos.y = FIX32(entity->posInt.y);
		entity->pos.y = FIX32(maxPosLimits.min.y - entity->trigger->rect.min.y);

		// entity->spd.y = 0;
	}
	if (maxPosLimits.max.y <= entityBounds.max.y) {
		// entity->posInt.y = maxPosLimits.max.y - entity->trigger->rect.max.y;
		// entity->pos.y = FIX32(entity->posInt.y);
		entity->pos.y = FIX32(maxPosLimits.max.y - entity->trigger->rect.max.y);
		// entity->spd.y = 0;
		entity->onGround = TRUE;
	}
}

void checkCollisions() {
	//As we now have to check for collisions, we will later check if it is true or false, but for now it is false
	collidingAgainstStair = FALSE;

	//Create level limits
	AABB levelLimits = roomSize;

	//Easy access to the bounds in global pos
	if (playerBody.climbingStair) {
		playerBounds = newAABB(
			playerBody.globalPosition.x + playerBody.climbingStairAABB.min.x,
			playerBody.globalPosition.x + playerBody.climbingStairAABB.max.x,
			playerBody.globalPosition.y + playerBody.climbingStairAABB.min.y,
			playerBody.globalPosition.y + playerBody.climbingStairAABB.max.y
		);
	}else {
		playerBounds = newAABB(
			playerBody.globalPosition.x + playerBody.aabb.min.x,
			playerBody.globalPosition.x + playerBody.aabb.max.x,
			playerBody.globalPosition.y + playerBody.aabb.min.y,
			playerBody.globalPosition.y + playerBody.aabb.max.y
		);
	}

	//We can see this variables as a way to avoid thinking that a ground tile is a wall tile
	//Skin width (yIntVelocity) changes depending on the vertical velocity
	s16 yIntVelocity = fix16ToRoundedInt(playerBody.velocity.fixY);
	s16 playerHeadPos = playerBody.aabb.min.y - yIntVelocity + playerBody.globalPosition.y;
	s16 playerFeetPos = playerBody.aabb.max.y - yIntVelocity + playerBody.globalPosition.y;

	//Positions in tiles
	Vect2D_u16 minTilePos = posToTile(newVector2D_s16(playerBounds.min.x, playerBounds.min.y));
	Vect2D_u16 maxTilePos = posToTile(newVector2D_s16(playerBounds.max.x, playerBounds.max.y));

	//Used to limit how many tiles we have to check for collision
	Vect2D_u16 tileBoundDifference = newVector2D_u16(maxTilePos.x - minTilePos.x, maxTilePos.y - minTilePos.y);

	//First we check for horizontal collisions
	for (u16 i = 0; i <= tileBoundDifference.y; i++) {
		//Height position constant as a helper
		const u16 y = minTilePos.y + i;

		//Right position constant as a helper
		const u16 rx = maxTilePos.x;

		u16 rTileValue = getTileValue(rx, y);
		//After getting the tile value, we check if that is one of whom we can collide/trigger with horizontally
		if (rTileValue == GROUND_TILE) {
			AABB tileBounds = getTileBounds(rx, y);
			//Before taking that tile as a wall, we have to check if that is within the player hitbox, e.g. not seeing ground as a wall
			if (tileBounds.min.x < levelLimits.max.x && tileBounds.min.y < playerFeetPos && tileBounds.max.y > playerHeadPos) {
				levelLimits.max.x = tileBounds.min.x;
				break;
			}
		} else if(rTileValue == SLOPE_90_RIGHT){
			u16 bottomEdgePos = getTileTopEdge(y);
			u16 rightEdgePos = getTileRightEdge(rx);
			if (bottomEdgePos < levelLimits.max.y) { // && bottomEdgePos >= playerFeetPos
				levelLimits.max.y = bottomEdgePos + rightEdgePos-playerBody.globalPosition.x- playerBody.aabb.max.x;
			}
			 
		}
		else if(rTileValue == SLOPE_45_RIGHT1){
			
			u16 bottomEdgePos = getTileTopEdge(y);
			u16 rightEdgePos = getTileRightEdge(rx);
			if (bottomEdgePos < levelLimits.max.y) { // && bottomEdgePos >= playerFeetPos
				levelLimits.max.y = bottomEdgePos + ((rightEdgePos-playerBody.globalPosition.x-playerBody.aabb.min.x)>>1) ;
			} 
		}
		else if(rTileValue == SLOPE_45_RIGHT2){
			
			u16 bottomEdgePos = getTileTopEdge(y);
			u16 rightEdgePos = getTileRightEdge(rx);
			if (bottomEdgePos < levelLimits.max.y) { // && bottomEdgePos >= playerFeetPos
				levelLimits.max.y = bottomEdgePos + ((rightEdgePos-playerBody.globalPosition.x-playerBody.aabb.min.x)>>1)-CELL_SIZE_DIV_2;
			} 
		}
		else if (rTileValue == LADDER_TILE) {
			stairLeftEdge = getTileLeftEdge(rx);
			collidingAgainstStair = TRUE;
		}
		else if (rTileValue == DEATH_TILE) {
			PAL_fadeOutAll(3,FALSE);
			loadLevel(levelNum, (Vect2D_s16)getLevelPos(0));
		}

		//Left position constant as a helper
		const s16 lx = minTilePos.x;

		u16 lTileValue = getTileValue(lx, y);
		//We do the same here but for the left side
		if (lTileValue == GROUND_TILE) {
			AABB tileBounds = getTileBounds(lx, y);
			if (tileBounds.max.x > levelLimits.min.x && tileBounds.min.y < playerFeetPos && tileBounds.max.y > playerHeadPos) {
				levelLimits.min.x = tileBounds.max.x;
				break;
			}
		} 
		else if(lTileValue == SLOPE_90_LEFT){
			u16 bottomEdgePos = getTileTopEdge(y);
			u16 leftEdgePos = getTileLeftEdge(lx);
			if (bottomEdgePos < levelLimits.max.y) { // && bottomEdgePos >= playerFeetPos
				levelLimits.max.y = bottomEdgePos - (leftEdgePos-playerBody.globalPosition.x-playerBody.aabb.min.x) ;
			}
	 
		}
		
		else if(lTileValue == SLOPE_45_LEFT1){
			
			u16 bottomEdgePos = getTileTopEdge(y);
			u16 leftEdgePos = getTileLeftEdge(lx);
			if (bottomEdgePos < levelLimits.max.y) { // && bottomEdgePos >= playerFeetPos
				levelLimits.max.y = bottomEdgePos - ((leftEdgePos-playerBody.globalPosition.x-playerBody.aabb.max.x)>>1) ;
			} 
		}
		else if(lTileValue == SLOPE_45_LEFT2){
			
			u16 bottomEdgePos = getTileTopEdge(y);
			u16 leftEdgePos = getTileLeftEdge(lx);
			if (bottomEdgePos < levelLimits.max.y) { // && bottomEdgePos >= playerFeetPos
				levelLimits.max.y = bottomEdgePos - ((leftEdgePos-playerBody.globalPosition.x-playerBody.aabb.max.x)>>1)-CELL_SIZE_DIV_2;
			} 
		}
		
		else if (lTileValue == LADDER_TILE) {
			stairLeftEdge = getTileLeftEdge(lx);
			collidingAgainstStair = TRUE;
		} else if (lTileValue == DEATH_TILE) {
			PAL_fadeOutAll(3,FALSE);
			loadLevel(levelNum, (Vect2D_s16)getLevelPos(0));
		}
	}

	//After checking for horizontal positions we can modify the positions if the player is colliding
	if (levelLimits.min.x > playerBounds.min.x) {
		playerBody.globalPosition.x = levelLimits.min.x - playerBody.aabb.min.x;
		playerBody.velocity.x = playerBody.velocity.fixX = 0;
	}
	if (levelLimits.max.x < playerBounds.max.x) {
		playerBody.globalPosition.x = levelLimits.max.x - playerBody.aabb.max.x;
		playerBody.velocity.x = playerBody.velocity.fixX = 0;
	}

	//Then, we modify the player position so we can use them to check for vertical collisions
	if (playerBody.climbingStair) {
		playerBounds = newAABB(
			playerBody.globalPosition.x + playerBody.climbingStairAABB.min.x,
			playerBody.globalPosition.x + playerBody.climbingStairAABB.max.x,
			playerBody.globalPosition.y + playerBody.climbingStairAABB.min.y,
			playerBody.globalPosition.y + playerBody.climbingStairAABB.max.y
		);
	}else {
		playerBounds = newAABB(
			playerBody.globalPosition.x + playerBody.aabb.min.x,
			playerBody.globalPosition.x + playerBody.aabb.max.x,
			playerBody.globalPosition.y + playerBody.aabb.min.y,
			playerBody.globalPosition.y + playerBody.aabb.max.y
		);
	}

	//And do the same to the variables that are used to check for them
	minTilePos = posToTile(newVector2D_s16(playerBounds.min.x, playerBounds.min.y));
	maxTilePos = posToTile(newVector2D_s16(playerBounds.max.x - 1, playerBounds.max.y));
	tileBoundDifference = newVector2D_u16(maxTilePos.x - minTilePos.x, maxTilePos.y - minTilePos.y);
	
	bool onStair = FALSE;
	playerBody.onSemiSolid = FALSE;

	//To avoid having troubles with player snapping to ground ignoring the upward velocity, we separate top and bottom collisions depending on the velocity
	if (yIntVelocity >= 0) {
		for (u16 i = 0; i <= tileBoundDifference.x; i++) {
			s16 x = minTilePos.x + i;
			u16 y = maxTilePos.y;

			//This is the exact same method that we use for horizontal collisions
			u16 bottomTileValue = getTileValue(x, y);
			if (bottomTileValue == GROUND_TILE || bottomTileValue == ONE_WAY_PLATFORM_UP_TILE) {
				if (getTileRightEdge(x) == levelLimits.min.x || getTileLeftEdge(x) == levelLimits.max.x)
					continue;

				u16 bottomEdgePos;
				if(bottomTileValue == ONE_WAY_PLATFORM_UP_TILE && playerBody.input.y == 1 && ((input.state & BUTTON_A) && (input.changed & BUTTON_A))){
					// playerBody.onSemiSolid = TRUE;
					bottomEdgePos = getTileBottomEdge(y);
				} else {
					bottomEdgePos = getTileTopEdge(y);
					if(playerFeetPos < bottomEdgePos){
						playerBody.velocity.fixY = 0;
					}
				}
				
				//The error correction is used to add some extra width pixels in case the player isn't high enough by just some of them
				if (bottomEdgePos < levelLimits.max.y && (bottomEdgePos >= playerFeetPos)) { // && bottomEdgePos >= (playerFeetPos - oneWayPlatformErrorCorrection)
					levelLimits.max.y = bottomEdgePos;
					playerBody.curAmountOfJumps = playerBody.maxAmountOfJumps;
				}
				
				
			}
			else if(bottomTileValue == SLOPE_90_RIGHT){
				u16 bottomEdgePos = getTileTopEdge(y);
				u16 rightEdgePos = getTileRightEdge(x);
				if (bottomEdgePos < levelLimits.max.y) { // && bottomEdgePos >= playerFeetPos
					s16 downValue = rightEdgePos-playerBody.globalPosition.x- playerBody.aabb.max.x;
					if(downValue < 0){
						downValue = 0;
					}
					levelLimits.max.y = bottomEdgePos + downValue;
				}
				
			}
			else if(bottomTileValue == SLOPE_45_RIGHT1){
				u16 bottomEdgePos = getTileTopEdge(y);
				u16 rightEdgePos = getTileRightEdge(x);
				if (bottomEdgePos < levelLimits.max.y) { // && bottomEdgePos >= playerFeetPos
					s16 downValue = ((rightEdgePos-playerBody.globalPosition.x-playerBody.aabb.min.x)>>1);
					levelLimits.max.y = bottomEdgePos + downValue;
				} 
			}
			else if(bottomTileValue == SLOPE_45_RIGHT2){
				u16 bottomEdgePos = getTileTopEdge(y);
				u16 rightEdgePos = getTileRightEdge(x);
				if (bottomEdgePos < levelLimits.max.y) { // && bottomEdgePos >= playerFeetPos
					s16 downValue = ((rightEdgePos-playerBody.globalPosition.x-playerBody.aabb.min.x)>>1)-CELL_SIZE_DIV_2;
					if(downValue < 0){
						downValue = 0;
					}
					levelLimits.max.y = bottomEdgePos + downValue;// CELL_SIZE
				} 
			}
			else if(bottomTileValue == SLOPE_45_LEFT1){
				u16 bottomEdgePos = getTileTopEdge(y);
				u16 leftEdgePos = getTileLeftEdge(x);
				if (bottomEdgePos < levelLimits.max.y) { // && bottomEdgePos >= playerFeetPos
					s16 downValue = ((leftEdgePos-playerBody.globalPosition.x-playerBody.aabb.max.x)>>1);
					levelLimits.max.y = bottomEdgePos - downValue;
				} 
			}
			else if(bottomTileValue == SLOPE_45_LEFT2){
				
				u16 bottomEdgePos = getTileTopEdge(y);
				u16 leftEdgePos = getTileLeftEdge(x);
				if (bottomEdgePos < levelLimits.max.y) { // && bottomEdgePos >= playerFeetPos
					s16 downValue = ((leftEdgePos-playerBody.globalPosition.x-playerBody.aabb.max.x)>>1)-CELL_SIZE_DIV_2;
					if(downValue > 0){
						downValue = 0;
					}
					levelLimits.max.y = bottomEdgePos - downValue;
				} 
			}
			else if (bottomTileValue == LADDER_TILE) {
				stairLeftEdge = getTileLeftEdge(x);

				u16 bottomEdgePos = getTileTopEdge(y);
				//Only in this case we check for ladder collisions, as we need them to climb them down
				if (bottomEdgePos <= levelLimits.max.y && bottomEdgePos >= playerFeetPos && !playerBody.climbingStair && getTileValue(x, y - 1) != LADDER_TILE) {
					collidingAgainstStair = TRUE;
					onStair = TRUE;
					levelLimits.max.y = bottomEdgePos;
					break;
				}
			} else if (bottomTileValue == DEATH_TILE) {
				PAL_fadeOutAll(3,FALSE);
				loadLevel(levelNum, (Vect2D_s16)getLevelPos(0));
			}
		}
	} else {
		for (u16 i = 0; i <= tileBoundDifference.x; i++) {
			s16 x = minTilePos.x + i;
			u16 y = minTilePos.y;

			//And the same once again
			u16 topTileValue = getTileValue(x, y);
			
			// || topTileValue == ONE_WAY_PLATFORM_DOWN_TILE
			if (topTileValue == GROUND_TILE) {
				if (getTileRightEdge(x) == levelLimits.min.x || getTileLeftEdge(x) == levelLimits.max.x)
					continue;

				u16 upperEdgePos;
				
				upperEdgePos = getTileBottomEdge(y);
				
				if (upperEdgePos < levelLimits.max.y) {
					levelLimits.min.y = upperEdgePos+2;
					break;
				}
				playerBody.velocity.fixY = 0;
				
			}
			else if(topTileValue == SLOPE_90_LEFT_UP){
				u16 upperEdgePos = getTileBottomEdge(y);
				u16 leftEdgePos = getTileLeftEdge(x);
				if (upperEdgePos < levelLimits.max.y) {
					levelLimits.min.y = upperEdgePos-(leftEdgePos-playerBody.globalPosition.x);
				}
				playerBody.velocity.fixY = 0;
			}
			else if (topTileValue == LADDER_TILE) {
				stairLeftEdge = getTileLeftEdge(x);
				collidingAgainstStair = TRUE;
			} else if (topTileValue == DEATH_TILE) {
				PAL_fadeOutAll(3,FALSE);
				loadLevel(levelNum, (Vect2D_s16)getLevelPos(0));
			}
		}
	}

	//Now we modify the player position and some properties if necessary
	if (levelLimits.min.y > playerBounds.min.y) {
		playerBody.globalPosition.y = levelLimits.min.y - playerBody.aabb.min.y;
		//playerBody.velocity.fixY = 0;
	}
	if (levelLimits.max.y <= playerBounds.max.y) {
		if (levelLimits.max.y == 768) {
			playerBody.falling = TRUE;
		}else {
			playerBody.onStair = onStair;
			playerBody.onGround = TRUE;
			playerBody.curAmountOfJumps = playerBody.maxAmountOfJumps;
			playerBody.climbingStair = FALSE;
			currentCoyoteTime = coyoteTime;
			playerBody.jumping = FALSE;
			playerBody.globalPosition.y = levelLimits.max.y - playerBody.aabb.max.y;
			//playerBody.velocity.fixY = 0;
		}
	}else {
		playerBody.onStair = playerBody.onGround = FALSE;
		currentCoyoteTime--;
	}
	//This time we don't need to update the playerBounds as they will be updated at the beginning of the function the next frame
}