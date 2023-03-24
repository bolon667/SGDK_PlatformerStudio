PAL_fadeOutAll(3,FALSE);
	levelNum = trigger->val1;
	Vect2D_s16 newStartPos = getLevelPos(trigger->val2);
	switch(trigger->val3) // position mode
	{
		case 0: //Horizontal, bottom anchor
			newStartPos.y -= triggerBounds->max.y-playerBounds.max.y+playerBody.aabb.max.y-playerBody.aabb.min.y;
		case 1: //Vertical, left anchor
			//Not sure if it works, need more testing
			newStartPos.x += triggerBounds->min.x-playerBounds.min.x+playerBody.aabb.min.y;
	}
					
	
	loadLevel(levelNum, (Vect2D_s16)newStartPos);