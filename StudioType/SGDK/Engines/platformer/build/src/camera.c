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
	if (playerBody.globalPosition.x + playerBody.centerOffset.x > cameraPosition.x + cameraDeadzone.max.x) {
		cameraPosition.x = playerBody.globalPosition.x + playerBody.centerOffset.x - cameraDeadzone.max.x;

	}else if (playerBody.globalPosition.x + playerBody.centerOffset.x < cameraPosition.x + cameraDeadzone.min.x) {
		cameraPosition.x = playerBody.globalPosition.x + playerBody.centerOffset.x - cameraDeadzone.min.x;
	}

	if (playerBody.globalPosition.y + playerBody.centerOffset.y > cameraPosition.y + cameraDeadzone.max.y) {
		cameraPosition.y = playerBody.globalPosition.y + playerBody.centerOffset.y - cameraDeadzone.max.y;

	}else if (playerBody.globalPosition.y + playerBody.centerOffset.y < cameraPosition.y + cameraDeadzone.min.y) {
		cameraPosition.y = playerBody.globalPosition.y + playerBody.centerOffset.y - cameraDeadzone.min.y;
	}
	//roomSize.y - 224
	//Clamp camera to the limits of the level
	u16 maxCameraPosX = curLvlData->sizeinPx.x - 320;
	u16 maxCameraPosY = curLvlData->sizeinPx.y - 224;
	cameraPosition.x = clamp(cameraPosition.x, 0, maxCameraPosX); // 768 - 320 = 448 (level width - screen width)
	cameraPosition.y = clamp(cameraPosition.y, 0, maxCameraPosY); // 768 - 224 = 544 (level height - screen height)

	if(bga) MAP_scrollTo(bga, cameraPosition.x, cameraPosition.y);
	if(bgb) MAP_scrollTo(bgb, cameraPosition.x, cameraPosition.y);
}