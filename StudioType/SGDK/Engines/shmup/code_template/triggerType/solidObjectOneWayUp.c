const Vect2D_s16 const playerCenterPos = {playerBody.globalPosition.x+((playerBody.aabb.min.x + playerBody.aabb.max.x)>>1), playerBody.globalPosition.y+((playerBody.aabb.min.y + playerBody.aabb.max.y)>>1)};

if(playerCenterPos.y <= triggerBounds->min.y){
	if(playerBody.velocity.fixY >= 0){
		// playerBody.velocity.fixY = 0;
		playerBody.globalPosition.y = (trigger->pos.y-playerBody.aabb.max.y)+1;
		playerBody.curAmountOfJumps = playerBody.maxAmountOfJumps;
		playerBody.onGround = TRUE;
	}
}

