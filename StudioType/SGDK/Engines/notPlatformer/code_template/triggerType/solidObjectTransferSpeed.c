const Vect2D_s16 const playerCenterPos = {playerBody.globalPosition.x+((playerBody.aabb.min.x + playerBody.aabb.max.x)>>1), playerBody.globalPosition.y+((playerBody.aabb.min.y + playerBody.aabb.max.y)>>1)};

if(playerCenterPos.x > triggerBounds->max.x) {
	playerBody.globalPosition.x = triggerBounds->max.x - playerBody.aabb.min.x;
  	playerBody.velocity.fixX = 0;
}
else if(playerCenterPos.x < triggerBounds->min.x){
	playerBody.globalPosition.x = triggerBounds->min.x-playerBody.aabb.max.x;
	playerBody.velocity.fixX = 0;
}
if(playerCenterPos.y < triggerBounds->min.y){
	playerBody.globalPosition.y = trigger->pos.y-playerBody.aabb.max.y+1;
	playerBody.curAmountOfJumps = playerBody.maxAmountOfJumps;
	playerBody.onGround = TRUE;
}
else if(playerCenterPos.y > triggerBounds->max.y) {
	playerBody.globalPosition.y = trigger->pos.y+trigger->rect.max.y-playerBody.aabb.min.y+1;
	playerBody.velocity.fixY = 0;
}

//Transfer speed

//If, object index in EnetityMertged_arr is not found, then find it 
if(trigger->val1 == -1){
	trigger->val1 = getTriggerParrentIndInArr(trigger);
}

const EntityMerged* parrent = &curEntityAll->EntityMerged_arr[trigger->val1];
playerBody.velocity.bufferFixX += parrent->spd.x >> 4;
playerBody.velocity.bufferFixY += parrent->spd.y >> 4;
