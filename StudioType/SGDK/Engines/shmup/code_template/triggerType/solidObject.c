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