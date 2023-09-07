#include "../inc/player.h"
#include "../inc/playerInit.h"

#include "../inc/global.h"
#include "../inc/maps.h"

#include "../res/sounds.h"
#include "../res/sprites.h"
#include "../res/resources.h"
#include "../res/images.h"

//Platformer Engine

void playerInit(Vect2D_s32 levelStartPos, pBody* plr) {
	//Create the sprite and palette for the player
	
	//$showTriggerRects$
	plr->sprite = SPR_addSprite(&spr_player, levelStartPos.x, levelStartPos.y, TILE_ATTR(PLAYER_PALETTE, FALSE, FALSE, FALSE));
	

	SPR_setHFlip(plr->sprite, facingRight);

	//Set the global position of the player, the local position will be updated once we are in the main loop
	plr->globalPosition = levelStartPos;

	//We set collider size of the player
	plr->aabb = newAABB(4, 20, 4, 24);
	//This collider is thinner because if the width is >=16 it could interfere with the lateral walls
	plr->climbingStairAABB = newAABB(8, 20, plr->aabb.min.y, plr->aabb.max.y);
	//Calculate where's the center point of the player
	plr->centerOffset.x = ((plr->aabb.min.x + plr->aabb.max.x) >> 1);
	plr->centerOffset.y = ((plr->aabb.min.y + plr->aabb.max.y) >> 1);

	//Default movement values
	plr->attack = FALSE;
	plr->speed = 2;
	plr->climbingSpeed = 1;
	plr->maxFallSpeed = 6;
	plr->jumpSpeed = 7;
	plr->facingDirection = 1;
	plr->acceleration = FIX16(.25);
	plr->deceleration = FIX16(.2);

	plr->maxAmountOfJumps = 2;
	plr->curAmountOfJumps = plr->maxAmountOfJumps;
	

	plr->playerBounds = newAABB(
			plr->globalPosition.x + plr->aabb.min.x,
			plr->globalPosition.x + plr->aabb.max.x,
			plr->globalPosition.y + plr->aabb.min.y,
			plr->globalPosition.y + plr->aabb.max.y
		);

	//Commented because, if you not using default XGM driver, you will get a ear damage. It's really DANGEROUS you can get partially deaf in real life.
	
	//Setup the jump SFX with an index between 64 and 255
	// XGM_setPCM(64, sfx_jump, sizeof(sfx_jump));
	
}