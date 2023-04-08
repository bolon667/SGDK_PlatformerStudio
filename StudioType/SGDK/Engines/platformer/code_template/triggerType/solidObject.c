// if((playerBody.globalPosition.y+playerBody.aabb.min.y) < (trigger->pos.y+trigger->rect.max.y)) {
// 		playerBody.globalPosition.y = trigger->pos.y+trigger->rect.max.y-playerBody.aabb.min.y+1;
// 		playerBody.velocity.fixY = 0;
// 	}
// else if((playerBody.globalPosition.y+playerBody.aabb.max.y) > (trigger->pos.y+trigger->rect.min.y)) {
// 		playerBody.globalPosition.y = trigger->pos.y-playerBody.aabb.max.y+1;
// 		playerBody.curAmountOfJumps = playerBody.maxAmountOfJumps;
// 		playerBody.velocity.fixY = 0;
// 		playerBody.onGround = TRUE;
// 	}
// if((playerBody.globalPosition.x+playerBody.aabb.max.x) > (trigger->pos.x+trigger->rect.min.x)) {
// 		playerBody.globalPosition.x = trigger->pos.x+trigger->rect.min.x-playerBody.aabb.max.x-1;
// 		playerBody.velocity.x = 0;
// 	}
if((playerBounds.max.x > triggerBounds->min.x) && (playerBounds.min.x < triggerBounds->max.x)) {
	KLog_S1("playerBounds.max.x: ", playerBounds.max.x);
	KLog_S1("triggerBounds->min.x: ", triggerBounds->min.x);
	if(playerBounds.max.x > triggerBounds->min.x){
		playerBody.globalPosition.x = triggerBounds->min.x-playerBody.aabb.max.x;
		playerBody.velocity.fixX = 0;
	}
	if(playerBounds.min.x < triggerBounds->max.x) {
		playerBody.globalPosition.x = triggerBounds->max.x;
  		playerBody.velocity.fixX = 0;
	}
	
}
// else if(playerBounds.min.x < triggerBounds->max.x) {
//  	playerBody.globalPosition.x = triggerBounds->max.x+playerBody.aabb.min.x;
//  	playerBody.velocity.fixX = 0;
// }

//This crap not working properly

