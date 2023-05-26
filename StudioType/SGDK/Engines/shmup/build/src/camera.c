#include "../inc/camera.h"

#include "../inc/global.h"

#include "../res/resources.h"

Vect2D_s16 cameraPosition;
Vect2D_f16 cameraSpd;
Vect2D_f16 cameraSpdBuffer;


AABB cameraDeadzone;

void setupCamera(Vect2D_u16 deadZoneCenter, u16 deadZoneWidth, u16 deadZoneHeight) {
	//Calculates the bounds of the deadZone
	cameraDeadzone.min.x = deadZoneCenter.x - (deadZoneWidth >> 1);
	cameraDeadzone.max.x = deadZoneCenter.x + (deadZoneWidth >> 1);
	cameraDeadzone.min.y = deadZoneCenter.y - (deadZoneHeight >> 1);
	cameraDeadzone.max.y = deadZoneCenter.y + (deadZoneHeight >> 1);
	

	updateCamera();
	SYS_doVBlankProcess();
	//We force to update the whole layer in order to prevent wrong tile being loaded depending on the position
	if(bga) MAP_scrollToEx(bga, cameraPosition.x, cameraPosition.y, TRUE);
	SYS_doVBlankProcess();
	if(bgb) MAP_scrollToEx(bgb, cameraPosition.x, cameraPosition.y, TRUE);

	SYS_doVBlankProcess();
}

void updateCamera() {
	//Update camera only if the center of the player is outside the deadZone of the camera
	cameraSpd.x = FIX16(1.0);
	cameraSpdBuffer.x += cameraSpd.x;
	
	const s16 integerPartX = fix16ToInt(cameraSpdBuffer.x);
	cameraPosition.x += integerPartX;
	//geting only reminder
	cameraSpdBuffer.x -= FIX16(integerPartX);

	//Clamp camera to the limits of the level
	u16 maxCameraPosX = curLvlData->sizeinPx.x - 320;
	u16 maxCameraPosY = curLvlData->sizeinPx.y - 224;

	if(cameraPosition.x < 0){
		cameraPosition.x = 0;
	} else if(cameraPosition.x > maxCameraPosX){
		cameraPosition.x = maxCameraPosX;
		cameraSpd.x = 0;
	}

	if(cameraPosition.y < 0){
		cameraPosition.y = 0;
	} else if(cameraPosition.y > maxCameraPosY){
		cameraPosition.y = maxCameraPosY;
		cameraSpd.y = 0;
	}

	if(bga) MAP_scrollTo(bga, cameraPosition.x, cameraPosition.y);
	if(bgb) MAP_scrollTo(bgb, cameraPosition.x, cameraPosition.y);
}