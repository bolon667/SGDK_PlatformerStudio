#include "../inc/physics.h"


//Used to get the left edge of a tile by inputting the tile position
u16 getTileLeftEdge(u16 x) {
	return (x << TILE_SHIFT_LEFT_VALUE);
}
//Used to get the right edge of a tile by inputting the tile position
u16 getTileRightEdge(u16 x) {
	return (x << TILE_SHIFT_LEFT_VALUE) + CELL_SIZE;
}
//Used to get the top edge of a tile by inputting the tile position
u16 getTileTopEdge(u16 y) {
	return (y << TILE_SHIFT_LEFT_VALUE);
}
//Used to get the bottom edge of a tile by inputting the tile position
u16 getTileBottomEdge(u16 y) {
	return (y << TILE_SHIFT_LEFT_VALUE) + CELL_SIZE;
}
//Used to get the bounds of a tile by inputting the tile position
AABB getTileBounds(s16 x, s16 y) {
	return newAABB((x << TILE_SHIFT_LEFT_VALUE), (x << TILE_SHIFT_LEFT_VALUE) + CELL_SIZE, (y << TILE_SHIFT_LEFT_VALUE), (y << TILE_SHIFT_LEFT_VALUE) + CELL_SIZE);
}

//Used to get the tile position out of a pixel position
Vect2D_u16 posToTile(Vect2D_s16 position) {
	return newVector2D_u16((position.x >> TILE_SHIFT_LEFT_VALUE), (position.y >> TILE_SHIFT_LEFT_VALUE));
}