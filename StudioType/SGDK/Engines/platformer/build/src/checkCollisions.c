#include "../inc/player.h"
#include "../inc/checkCollisions.h"

#include "../inc/global.h"
#include "../inc/maps.h"


//Platformer Engine

void checkCollisions_forEntityMerged(EntityMerged* entity) {
	//Create level limits
	AABB levelLimits = roomSize;
	levelLimits.max.y += 200;

	entity->onGround = FALSE;

	//Easy access to the bounds in global pos
	AABB entityBounds = newAABB(
		entity->posInt.x + entity->trigger->rect.min.x,
		entity->posInt.x + entity->trigger->rect.max.x,
		entity->posInt.y + entity->trigger->rect.min.y,
		entity->posInt.y + entity->trigger->rect.max.y
	);

	// KLog_S1("-----------", 0);
	// KLog_S1("123: ", entity->trigger->rect.min.x);
	// KLog_S1("123: ", entity->trigger->rect.max.x);
	// KLog_S1("123: ", entity->trigger->rect.min.y);
	// KLog_S1("123: ", entity->trigger->rect.max.y);


	//We can see this variables as a way to avoid thinking that a ground tile is a wall tile
	//Skin width (yIntVelocity) changes depending on the vertical velocity
	s16 yIntVelocity = fix16ToRoundedInt(entity->spd.y);
	s16 entityHeadPos = entityBounds.min.y - yIntVelocity;
	s16 entityFeetPos = entityBounds.max.y - yIntVelocity;

	//Positions in tiles
	Vect2D_u16 minTilePos = posToTile(newVector2D_s16(entityBounds.min.x, entityBounds.min.y));
	Vect2D_u16 maxTilePos = posToTile(newVector2D_s16(entityBounds.max.x, entityBounds.max.y));

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
			if (tileBounds.min.x < levelLimits.max.x && tileBounds.min.y < entityFeetPos && tileBounds.max.y > entityHeadPos) {
				levelLimits.max.x = tileBounds.min.x;
				break;
			}
		} 

		//Left position constant as a helper
		const s16 lx = minTilePos.x;

		u16 lTileValue = getTileValue(lx, y);
		//We do the same here but for the left side
		if (lTileValue == GROUND_TILE) {
			AABB tileBounds = getTileBounds(lx, y);
			if (tileBounds.max.x > levelLimits.min.x && tileBounds.min.y < entityFeetPos && tileBounds.max.y > entityHeadPos) {
				levelLimits.min.x = tileBounds.max.x;
				break;
			}
		}
	}
	//After checking for horizontal positions we can modify the positions if the player is colliding
	if (levelLimits.min.x > entityBounds.min.x) {
		entity->posInt.x = levelLimits.min.x - entity->trigger->rect.min.x;
		entity->pos.x = FIX32(entity->posInt.x);
		entity->spd.x = 0;
	}
	if (levelLimits.max.x < entityBounds.max.x) {
		entity->posInt.x = levelLimits.max.x - entity->trigger->rect.max.x;
		entity->pos.x = FIX32(entity->posInt.x);
		entity->spd.x = 0;
	}

	//Then, we modify the player position so we can use them to check for vertical collisions
	entityBounds = newAABB(
		entity->posInt.x + entity->trigger->rect.min.x,
		entity->posInt.x + entity->trigger->rect.max.x,
		entity->posInt.y + entity->trigger->rect.min.y,
		entity->posInt.y + entity->trigger->rect.max.y
	);

	//And do the same to the variables that are used to check for them

	// minTilePos = posToTile(newVector2D_s16(playerBounds.min.x, playerBounds.min.y));
	// maxTilePos = posToTile(newVector2D_s16(playerBounds.max.x - 1, playerBounds.max.y));
	// tileBoundDifference = newVector2D_u16(maxTilePos.x - minTilePos.x, maxTilePos.y - minTilePos.y);

	minTilePos = posToTile(newVector2D_s16(entityBounds.min.x, entityBounds.min.y));
	maxTilePos = posToTile(newVector2D_s16(entityBounds.max.x - 1, entityBounds.max.y));
	tileBoundDifference = newVector2D_u16(maxTilePos.x - minTilePos.x, maxTilePos.y - minTilePos.y);

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
				u16 bottomEdgePos = getTileTopEdge(y);
				//if(bottomTileValue == ONE_WAY_PLATFORM_UP_TILE)
				//The error correction is used to add some extra width pixels in case the player isn't high enough by just some of them
				if ((bottomEdgePos < levelLimits.max.y) && (bottomEdgePos >= entityFeetPos)) {
					// && bottomEdgePos >= (playerFeetPos - oneWayPlatformErrorCorrection)
					levelLimits.max.y = bottomEdgePos;
				}
			}
		}
	}else {
		for (u16 i = 0; i <= tileBoundDifference.x; i++) {
			s16 x = minTilePos.x + i;
			u16 y = minTilePos.y;

			//And the same once again
			u16 topTileValue = getTileValue(x, y);
			if (topTileValue == GROUND_TILE) {
				if (getTileRightEdge(x) == levelLimits.min.x || getTileLeftEdge(x) == levelLimits.max.x)
					continue;

				u16 upperEdgePos = getTileBottomEdge(y);
				if (upperEdgePos < levelLimits.max.y) {
					levelLimits.min.y = upperEdgePos;
					break;
				}
			}
		}
	}

	//Now we modify the player position and some properties if necessary
	if (levelLimits.min.y > entityBounds.min.y) {
		
		entity->posInt.y = levelLimits.min.y - entity->trigger->rect.min.y;
		entity->pos.y = FIX32(entity->posInt.y);

		entity->spd.y = 0;
	}
	if (levelLimits.max.y <= entityBounds.max.y) {
		entity->onGround = TRUE;
		entity->posInt.y = levelLimits.max.y - entity->trigger->rect.max.y;
		entity->pos.y = FIX32(entity->posInt.y);

		// entity->spd.y = 0;
		//disabled to be able to create jumping gems
	}
	
	//This time we don't need to update the playerBounds as they will be updated at the beginning of the function the next frame
}

// void checkCollisions_forEntityMerged(EntityMerged* entity){	
// 	//We can see this variables as a way to avoid thinking that a ground tile is a wall tile
// 	//Skin width (yIntVelocity) changes depending on the vertical velocity
// 	AABB entityBounds = newAABB(
// 		entity->posInt.x + entity->trigger->rect.min.x,
// 		entity->posInt.x + entity->trigger->rect.max.x,
// 		entity->posInt.y + entity->trigger->rect.min.y,
// 		entity->posInt.y + entity->trigger->rect.max.y
// 	);

// 	entity->onGround = FALSE;

// 	//Create level limits
// 	AABB levelLimits = roomSize;
// 	levelLimits.max.y += 200; //to be able to fall trough floor


// 	//Positions in tiles
// 	Vect2D_u16 minTilePos = posToTile(newVector2D_s16(entityBounds.min.x, entityBounds.min.y));
// 	Vect2D_u16 maxTilePos = posToTile(newVector2D_s16(entityBounds.max.x, entityBounds.max.y));
// 	//Used to limit how many tiles we have to check for collision
// 	Vect2D_u16 tileBoundDifference = newVector2D_u16(maxTilePos.x - minTilePos.x, maxTilePos.y - minTilePos.y);

// 	s16 yIntVelocity = fix16ToRoundedInt(entity->spd.y);
// 	s16 entityHeadPos = entity->posInt.y - yIntVelocity  + entity->trigger->rect.min.y;
// 	s16 entityFeetPos = entity->posInt.y - yIntVelocity  + entity->trigger->rect.max.y;

// 	//First we check for horizontal collisions
// 	for (u16 i = 0; i <= tileBoundDifference.y; i++) {
// 		//Height position constant as a helper
// 		const u16 y = minTilePos.y + i;

// 		//Right position constant as a helper
// 		const u16 rx = maxTilePos.x;

// 		u16 rTileValue = getTileValue(rx, y);
// 		//After getting the tile value, we check if that is one of whom we can collide/trigger with horizontally
// 		if (rTileValue == GROUND_TILE) {
// 			AABB tileBounds = getTileBounds(rx, y);
// 			//Before taking that tile as a wall, we have to check if that is within the player hitbox, e.g. not seeing ground as a wall


// 			levelLimits.max.x = tileBounds.min.x;
// 			//tileBounds.min.x < levelLimits.max.x && 
// 			// if (tileBounds.min.y < entityFeetPos && tileBounds.max.y > entityHeadPos) {
// 			// 	levelLimits.max.x = tileBounds.min.x;
// 			// 	break;
// 			// }
// 		}else if (rTileValue == LADDER_TILE) {
// 			stairLeftEdge = getTileLeftEdge(rx);
// 			collidingAgainstStair = TRUE;
// 		}

// 		//Left position constant as a helper
// 		const s16 lx = minTilePos.x;

// 		u16 lTileValue = getTileValue(lx, y);
// 		//We do the same here but for the left side
// 		if (lTileValue == GROUND_TILE) {
// 			AABB tileBounds = getTileBounds(lx, y);
// 			levelLimits.min.x = tileBounds.max.x;
// 			// if (tileBounds.max.x > levelLimits.min.x && tileBounds.min.y < entityFeetPos && tileBounds.max.y > entityHeadPos) {
// 			// 	levelLimits.min.x = tileBounds.max.x;
// 			// 	break;
// 			// }
// 		}else if (lTileValue == LADDER_TILE) {
// 			stairLeftEdge = getTileLeftEdge(lx);
// 			collidingAgainstStair = TRUE;
// 		}
// 	}

// 	//After checking for horizontal positions we can modify the positions if the player is colliding
// 	if (levelLimits.min.x > entityBounds.min.x) {
// 		entity->posInt.x = (levelLimits.min.x - entity->trigger->rect.min.x);
// 		entity->pos.x = FIX32(entity->posInt.x);
// 		// entity->spd.x = FIX32(1);
// 		entity->spd.x = 0;

// 	}
// 	if (levelLimits.max.x < entityBounds.max.x) {
// 		entity->posInt.x = (levelLimits.max.x - entity->trigger->rect.max.x);
// 		entity->pos.x = FIX32(entity->posInt.x);
// 		// entity->spd.x = FIX32(-1);
// 		entity->spd.x = 0;
// 	}

// 	//Then, we modify the entity position so we can use them to check for vertical collisions
// 	entityBounds = newAABB(
// 		entity->posInt.x + entity->trigger->rect.min.x,
// 		entity->posInt.x + entity->trigger->rect.max.x,
// 		entity->posInt.y + entity->trigger->rect.min.y,
// 		entity->posInt.y + entity->trigger->rect.max.y
// 	);

// 	//And do the same to the variables that are used to check for them
// 	minTilePos = posToTile(newVector2D_s16(entityBounds.min.x, entityBounds.min.y));
// 	maxTilePos = posToTile(newVector2D_s16(entityBounds.max.x, entityBounds.max.y));
// 	tileBoundDifference = newVector2D_u16(maxTilePos.x - minTilePos.x, maxTilePos.y - minTilePos.y);

// 	//To avoid having troubles with player snapping to ground ignoring the upward velocity, we separate top and bottom collisions depending on the velocity
// 	if (yIntVelocity >= 0) {
// 		for (u16 i = 0; i <= tileBoundDifference.x; i++) {
// 			s16 x = minTilePos.x + i;
// 			u16 y = maxTilePos.y;

// 			//This is the exact same method that we use for horizontal collisions
// 			u16 bottomTileValue = getTileValue(x, y);
// 			if (bottomTileValue == GROUND_TILE || bottomTileValue == ONE_WAY_PLATFORM_UP_TILE) {
// 				if (getTileRightEdge(x) == levelLimits.min.x || getTileLeftEdge(x) == levelLimits.max.x)
// 					continue;
// 				u16 bottomEdgePos = getTileTopEdge(y);

// 				levelLimits.max.y = bottomEdgePos;

// 				//The error correction is used to add some extra width pixels in case the player isn't high enough by just some of them
// 				// if (bottomEdgePos < levelLimits.max.y && bottomEdgePos >= (entityFeetPos - oneWayPlatformErrorCorrection)) {
// 				// 	levelLimits.max.y = bottomEdgePos;
// 				// }
// 			}
// 		}
// 	}else {
// 		for (u16 i = 0; i <= tileBoundDifference.x; i++) {
// 			s16 x = minTilePos.x + i;
// 			u16 y = minTilePos.y;

// 			//And the same once again
// 			u16 topTileValue = getTileValue(x, y);
// 			if (topTileValue == GROUND_TILE) {
// 				if (getTileRightEdge(x) == levelLimits.min.x || getTileLeftEdge(x) == levelLimits.max.x)
// 					continue;

// 				u16 upperEdgePos = getTileBottomEdge(y);
// 				if (upperEdgePos < levelLimits.max.y) {
// 					levelLimits.min.y = upperEdgePos;
// 					break;
// 				}
// 			}
// 		}
// 	}

// 	//Now we modify the player position and some properties if necessary
// 	if (levelLimits.min.y > entityBounds.min.y) {
// 		entity->posInt.y = levelLimits.min.y - entity->trigger->rect.min.y+1;
// 		entity->pos.y = FIX32(entity->posInt.y);
// 		entity->spd.y = 0;
// 	}
// 	if (levelLimits.max.y <= entityBounds.max.y) {
// 		entity->posInt.y = levelLimits.max.y - entity->trigger->rect.max.y-1;
// 		entity->pos.y = FIX32(entity->posInt.y);

// 		entity->onGround = TRUE;
// 		entity->spd.y = 0;
// 	}
// 	//This time we don't need to update the playerBounds as they will be updated at the beginning of the function the next frame
// }

void checkCollisions(pBody* plr) {
	//As we now have to check for collisions, we will later check if it is true or false, but for now it is false
	collidingAgainstStair = FALSE;

	//Create level limits
	AABB levelLimits = roomSize;

	//Easy access to the bounds in global pos
	if (playerBody.climbingStair) {
		plr->playerBounds = newAABB(
			plr->globalPosition.x + plr->climbingStairAABB.min.x,
			plr->globalPosition.x + plr->climbingStairAABB.max.x,
			plr->globalPosition.y + plr->climbingStairAABB.min.y,
			plr->globalPosition.y + plr->climbingStairAABB.max.y
		);
	}else {
		plr->playerBounds = newAABB(
			plr->globalPosition.x + plr->aabb.min.x,
			plr->globalPosition.x + plr->aabb.max.x,
			plr->globalPosition.y + plr->aabb.min.y,
			plr->globalPosition.y + plr->aabb.max.y
		);
	}

	//We can see this variables as a way to avoid thinking that a ground tile is a wall tile
	//Skin width (yIntVelocity) changes depending on the vertical velocity
	s16 yIntVelocity = fix16ToRoundedInt(plr->velocity.fixY);
	s16 playerHeadPos = plr->aabb.min.y - yIntVelocity + plr->globalPosition.y;
	s16 playerFeetPos = plr->aabb.max.y - yIntVelocity + plr->globalPosition.y;

	//Positions in tiles
	Vect2D_u16 minTilePos = posToTile(newVector2D_s16(plr->playerBounds.min.x, plr->playerBounds.min.y));
	Vect2D_u16 maxTilePos = posToTile(newVector2D_s16(plr->playerBounds.max.x, plr->playerBounds.max.y));

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
				levelLimits.max.y = bottomEdgePos + rightEdgePos-plr->globalPosition.x- plr->aabb.max.x;
			}
			 
		}
		else if(rTileValue == SLOPE_45_RIGHT1){
			
			u16 bottomEdgePos = getTileTopEdge(y);
			u16 rightEdgePos = getTileRightEdge(rx);
			if (bottomEdgePos < levelLimits.max.y) { // && bottomEdgePos >= playerFeetPos
				levelLimits.max.y = bottomEdgePos + ((rightEdgePos-plr->globalPosition.x-plr->aabb.min.x)>>1) ;
			} 
		}
		else if(rTileValue == SLOPE_45_RIGHT2){
			
			u16 bottomEdgePos = getTileTopEdge(y);
			u16 rightEdgePos = getTileRightEdge(rx);
			if (bottomEdgePos < levelLimits.max.y) { // && bottomEdgePos >= playerFeetPos
				levelLimits.max.y = bottomEdgePos + ((rightEdgePos-plr->globalPosition.x-plr->aabb.min.x)>>1)-CELL_SIZE_DIV_2;
			} 
		}
		else if (rTileValue == LADDER_TILE) {
			stairLeftEdge = getTileLeftEdge(rx);
			collidingAgainstStair = TRUE;
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
				levelLimits.max.y = bottomEdgePos - (leftEdgePos-plr->globalPosition.x-plr->aabb.min.x) ;
			}
	 
		}
		
		else if(lTileValue == SLOPE_45_LEFT1){
			
			u16 bottomEdgePos = getTileTopEdge(y);
			u16 leftEdgePos = getTileLeftEdge(lx);
			if (bottomEdgePos < levelLimits.max.y) { // && bottomEdgePos >= playerFeetPos
				levelLimits.max.y = bottomEdgePos - ((leftEdgePos-plr->globalPosition.x-plr->aabb.max.x)>>1) ;
			} 
		}
		else if(lTileValue == SLOPE_45_LEFT2){
			
			u16 bottomEdgePos = getTileTopEdge(y);
			u16 leftEdgePos = getTileLeftEdge(lx);
			if (bottomEdgePos < levelLimits.max.y) { // && bottomEdgePos >= playerFeetPos
				levelLimits.max.y = bottomEdgePos - ((leftEdgePos-plr->globalPosition.x-plr->aabb.max.x)>>1)-CELL_SIZE_DIV_2;
			} 
		}
		
		else if (lTileValue == LADDER_TILE) {
			stairLeftEdge = getTileLeftEdge(lx);
			collidingAgainstStair = TRUE;
		}
	}

	//After checking for horizontal positions we can modify the positions if the player is colliding
	if (levelLimits.min.x > plr->playerBounds.min.x) {
		plr->globalPosition.x = levelLimits.min.x - plr->aabb.min.x;
		plr->velocity.x = plr->velocity.fixX = 0;
	}
	if (levelLimits.max.x < plr->playerBounds.max.x) {
		plr->globalPosition.x = levelLimits.max.x - plr->aabb.max.x;
		plr->velocity.x = plr->velocity.fixX = 0;
	}

	//Then, we modify the player position so we can use them to check for vertical collisions
	if (plr->climbingStair) {
		plr->playerBounds = newAABB(
			plr->globalPosition.x + plr->climbingStairAABB.min.x,
			plr->globalPosition.x + plr->climbingStairAABB.max.x,
			plr->globalPosition.y + plr->climbingStairAABB.min.y,
			plr->globalPosition.y + plr->climbingStairAABB.max.y
		);
	}else {
		plr->playerBounds = newAABB(
			plr->globalPosition.x + plr->aabb.min.x,
			plr->globalPosition.x + plr->aabb.max.x,
			plr->globalPosition.y + plr->aabb.min.y,
			plr->globalPosition.y + plr->aabb.max.y
		);
	}

	//And do the same to the variables that are used to check for them
	minTilePos = posToTile(newVector2D_s16(plr->playerBounds.min.x, plr->playerBounds.min.y));
	maxTilePos = posToTile(newVector2D_s16(plr->playerBounds.max.x - 1, plr->playerBounds.max.y));
	tileBoundDifference = newVector2D_u16(maxTilePos.x - minTilePos.x, maxTilePos.y - minTilePos.y);
	
	bool onStair = FALSE;

	//To avoid having troubles with player snapping to ground ignoring the upward velocity, we separate top and bottom collisions depending on the velocity
	if (yIntVelocity >= 0) {
		for (u16 i = 0; i <= tileBoundDifference.x; i++) {
			s16 x = minTilePos.x + i;
			u16 y = maxTilePos.y;

			//This is the exact same method that we use for horizontal collisions
			u16 bottomTileValue = getTileValue(x, y);
			bool onSemiSolidUp = bottomTileValue == ONE_WAY_PLATFORM_UP_TILE;
			if(onSemiSolidUp) {
				plr->onSemiSolid = TRUE;
			} else {
				plr->onSemiSolid = FALSE;
			}
			if (bottomTileValue == GROUND_TILE || (onSemiSolidUp && plr->semiSolidHasCollision)) {
				if (getTileRightEdge(x) == levelLimits.min.x || getTileLeftEdge(x) == levelLimits.max.x)
					continue;

				
				u16 bottomEdgePos = getTileTopEdge(y);
				//if(bottomTileValue == ONE_WAY_PLATFORM_UP_TILE)
				//The error correction is used to add some extra width pixels in case the player isn't high enough by just some of them
				if ((bottomEdgePos < levelLimits.max.y) && (bottomEdgePos >= playerFeetPos)) {
					// && bottomEdgePos >= (playerFeetPos - oneWayPlatformErrorCorrection)
					levelLimits.max.y = bottomEdgePos;
				}
			}
			else if(bottomTileValue == SLOPE_90_RIGHT){
				u16 bottomEdgePos = getTileTopEdge(y);
				u16 rightEdgePos = getTileRightEdge(x);
				if (bottomEdgePos < levelLimits.max.y) { // && bottomEdgePos >= playerFeetPos
					s16 downValue = rightEdgePos-plr->globalPosition.x- plr->aabb.max.x;
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
					s16 downValue = ((rightEdgePos-plr->globalPosition.x-plr->aabb.min.x)>>1);
					levelLimits.max.y = bottomEdgePos + downValue;
				} 
			}
			else if(bottomTileValue == SLOPE_45_RIGHT2){
				u16 bottomEdgePos = getTileTopEdge(y);
				u16 rightEdgePos = getTileRightEdge(x);
				if (bottomEdgePos < levelLimits.max.y) { // && bottomEdgePos >= playerFeetPos
					s16 downValue = ((rightEdgePos-plr->globalPosition.x-plr->aabb.min.x)>>1)-CELL_SIZE_DIV_2;
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
					s16 downValue = ((leftEdgePos-plr->globalPosition.x-plr->aabb.max.x)>>1);
					levelLimits.max.y = bottomEdgePos - downValue;
				} 
			}
			else if(bottomTileValue == SLOPE_45_LEFT2){
				
				u16 bottomEdgePos = getTileTopEdge(y);
				u16 leftEdgePos = getTileLeftEdge(x);
				if (bottomEdgePos < levelLimits.max.y) { // && bottomEdgePos >= playerFeetPos
					s16 downValue = ((leftEdgePos-plr->globalPosition.x-plr->aabb.max.x)>>1)-CELL_SIZE_DIV_2;
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
				if (bottomEdgePos <= levelLimits.max.y && bottomEdgePos >= playerFeetPos && !plr->climbingStair && getTileValue(x, y - 1) != LADDER_TILE) {
					collidingAgainstStair = TRUE;
					onStair = TRUE;
					levelLimits.max.y = bottomEdgePos;
					break;
				}
			} else if (bottomTileValue == DEATH_TILE) {
				PAL_fadeOutAll(3,FALSE);
				loadLevel(levelNum, getLevelPos(0)->pos);
				return;
			}
		}
	}else {
		for (u16 i = 0; i <= tileBoundDifference.x; i++) {
			s16 x = minTilePos.x + i;
			u16 y = minTilePos.y;

			//And the same once again
			u16 topTileValue = getTileValue(x, y);
			if (topTileValue == GROUND_TILE) {
				if (getTileRightEdge(x) == levelLimits.min.x || getTileLeftEdge(x) == levelLimits.max.x)
					continue;

				u16 upperEdgePos = getTileBottomEdge(y);
				if (upperEdgePos < levelLimits.max.y) {
					levelLimits.min.y = upperEdgePos;
					break;
				}
			}
			else if(topTileValue == SLOPE_90_LEFT_UP){
				u16 upperEdgePos = getTileBottomEdge(y);
				u16 leftEdgePos = getTileLeftEdge(x);
				if (upperEdgePos < levelLimits.max.y) {
					levelLimits.min.y = upperEdgePos-(leftEdgePos-plr->globalPosition.x);
				}
				plr->velocity.fixY = 0;
			}
			else if (topTileValue == LADDER_TILE) {
				stairLeftEdge = getTileLeftEdge(x);
				collidingAgainstStair = TRUE;
			} else if (topTileValue == DEATH_TILE) {
				PAL_fadeOutAll(3,FALSE);
				loadLevel(levelNum, getLevelPos(0)->pos);
				return;
			}
		}
	}

	//Now we modify the player position and some properties if necessary
	if (levelLimits.min.y > plr->playerBounds.min.y) {
		
		plr->globalPosition.y = levelLimits.min.y - plr->aabb.min.y;
		plr->velocity.fixY = plr->velocity.y = 0;
	}
	if (levelLimits.max.y <= plr->playerBounds.max.y) {
		plr->curAmountOfJumps = plr->maxAmountOfJumps;
		if (levelLimits.max.y == 768) {
			plr->falling = TRUE;
		}else {
			plr->onStair = onStair;
			plr->onGround = TRUE;
			plr->climbingStair = FALSE;
			currentCoyoteTime = coyoteTime;
			plr->jumping = FALSE;
			plr->globalPosition.y = levelLimits.max.y - plr->aabb.max.y;
			plr->velocity.fixY = plr->velocity.y = 0;
		}
	} else {
		plr->onStair = plr->onGround = FALSE;
		currentCoyoteTime--;
	}
	
	//This time we don't need to update the playerBounds as they will be updated at the beginning of the function the next frame
}