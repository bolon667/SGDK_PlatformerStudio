//$tilePosArr$

const u8 pal = $pal$;
const u8 maxMenuInd = $maxMenuInd$;
prev_joy_value = joy_value;
joy_value = JOY_readJoypad(JOY_1);

//--------Reminder---------
//[Go to level]// 
//  levelNum = 1;
//  loadLevel(levelNum, getLevelPosCoords(0));

//$switchActions$

if((joy_value & BUTTON_DOWN) && !(prev_joy_value & BUTTON_DOWN)) 
{
    VDP_drawTextEx(BG_A, " ", TILE_ATTR(pal, 0, FALSE, FALSE), tilePos_arr[curMenuInd].x-1, tilePos_arr[curMenuInd].y, CPU);
    curMenuInd--;
    if(curMenuInd < 0) {
        curMenuInd = maxMenuInd;
        VDP_drawTextEx(BG_A, ">", TILE_ATTR(pal, 0, FALSE, FALSE), tilePos_arr[curMenuInd].x-1, tilePos_arr[curMenuInd].y, CPU);
    }
}
else if((joy_value & BUTTON_UP) && !(prev_joy_value & BUTTON_UP)) {
    VDP_drawTextEx(BG_A, " ", TILE_ATTR(pal, 0, FALSE, FALSE), tilePos_arr[curMenuInd].x-1, tilePos_arr[curMenuInd].y, CPU);
    curMenuInd++;
    if(curMenuInd > maxMenuInd) {
        curMenuInd = 0;
        VDP_drawTextEx(BG_A, ">", TILE_ATTR(pal, 0, FALSE, FALSE), tilePos_arr[curMenuInd].x-1, tilePos_arr[curMenuInd].y, CPU);
    }
}



menuTimer++;
if(menuTimer > 10){
	menuTimer = 0;
	menuBool1 = !menuBool1;
	if(menuBool1){
        VDP_drawTextEx(BG_A, ">", TILE_ATTR(1, 0, FALSE, FALSE), tilePos_arr[curMenuInd].x-1, tilePos_arr[curMenuInd].y, CPU);
} else {
    for(u16 i=0; i<=maxMenuInd; i++){
        VDP_drawTextEx(BG_A, " ", TILE_ATTR(1, 0, FALSE, FALSE), tilePos_arr[i].x-1, tilePos_arr[i].y, CPU);
    }
}
}