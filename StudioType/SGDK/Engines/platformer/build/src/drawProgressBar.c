#include "../inc/player.h"

#include "../inc/global.h"
#include "../inc/maps.h"

#include "../res/resources.h"
#include "../res/sprites.h"

void drawProgressBar(u8 plane, u16 progressBarRootIndex, u8 pal, s16 x, s16 y, u16 value){
	s16 remain = value;
	u16 curTileX = 0;
	while(1) {
		if(remain >= 8){ //Если значение >= 8
                        //То понятно, что тайл полностью заполнен, поэтому делаем +8 к vram индексу
			VDP_setTileMapXY(plane, TILE_ATTR_FULL(pal, 15, 0, 0, progressBarRootIndex+8), x+curTileX, y);
			
		} else { //Если последний тайл прогресс-бара
                        //То рисуем соотвествующий тайл и заканчиваем работу
			VDP_setTileMapXY(plane, TILE_ATTR_FULL(pal, 15, 0, 0, progressBarRootIndex+remain), x+curTileX, y);
			return;
		}
		remain -= 8;
		curTileX++;
	}
	
}