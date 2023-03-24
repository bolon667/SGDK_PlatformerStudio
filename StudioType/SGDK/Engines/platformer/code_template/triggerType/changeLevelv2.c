if(!(trigger->activated && !trigger->prevActivated)){
	return;
}
PAL_fadeOutAll(3,FALSE);
//val1 = level_num
//val2 = side_id
//val3 = side_offset
	
	Vect2D_s16 newStartPos;
	levelNum = trigger->val1;
	switch(trigger->val2) // position mode
	{
		case 0: //Left
			newStartPos.x = LevelFull_arr[levelNum].lvl->sizeinPx.x-playerBody.aabb.max.x;
			newStartPos.y = playerBody.globalPosition.y-trigger->val3;
			break;
		case 1: //Right
			newStartPos.x = 0;
			newStartPos.y = playerBody.globalPosition.y-trigger->val3;
			break;
		case 2: //Up
			newStartPos.x = playerBody.globalPosition.x-trigger->val3;
			newStartPos.y = LevelFull_arr[levelNum].lvl->sizeinPx.y-playerBody.aabb.max.y;
			break;
		case 3: //Down
			newStartPos.x = playerBody.globalPosition.x-trigger->val3;
			newStartPos.y = 0;
			break;
	}
					
	
	loadLevel(levelNum, (Vect2D_s16)newStartPos);