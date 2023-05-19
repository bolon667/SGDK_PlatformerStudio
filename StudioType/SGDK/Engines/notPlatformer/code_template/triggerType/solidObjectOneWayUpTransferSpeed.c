const Vect2D_s16 const playerCenterPos = {playerBody.globalPosition.x+((playerBody.aabb.min.x + playerBody.aabb.max.x)>>1), playerBody.globalPosition.y+((playerBody.aabb.min.y + playerBody.aabb.max.y)>>1)};

if(trigger->val1 == -1){
	trigger->val1 = getTriggerParrentIndInArr(trigger);
}

if(playerCenterPos.y <= triggerBounds->min.y){
	if(playerBody.velocity.fixY >= 0){
		const EntityMerged* parrent = &curEntityAll->EntityMerged_arr[trigger->val1];
		playerBody.velocity.bufferFixX += parrent->spd.x >> 4;
		playerBody.velocity.bufferFixY += parrent->spd.y >> 4;

		// playerBody.velocity.fixY = 0;
		playerBody.globalPosition.y = trigger->pos.y-playerBody.aabb.max.y+1;
		playerBody.curAmountOfJumps = playerBody.maxAmountOfJumps;
		playerBody.onGround = TRUE;
	}
}
	

