void lazyCam1() {
	//Clamp camera to the limits of the level
	u16 maxCameraPosX = curLvlData->sizeinPx.x - 320;
	u16 maxCameraPosY = curLvlData->sizeinPx.y - 224;
	cameraPosition.x = clamp(cameraPosition.x, 0, maxCameraPosX); // 768 - 320 = 448 (level width - screen width)
	cameraPosition.y = clamp(cameraPosition.y, 0, maxCameraPosY); // 768 - 224 = 544 (level height - screen height)

	if(bga) MAP_scrollTo(bga, cameraPosition.x, cameraPosition.y);
	if(bgb) MAP_scrollTo(bgb, cameraPosition.x, cameraPosition.y);
}