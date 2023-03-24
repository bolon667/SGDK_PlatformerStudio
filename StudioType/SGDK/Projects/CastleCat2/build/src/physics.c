#include "../inc/physics.h"

Vect2D_f16 normalize_Vect2D_s16(Vect2D_s16 vector){
	fix16 len = fix16Sqrt((vector.x*vector.x) + (vector.y*vector.y) >> 2);
	if(!len){
		len = 1;
	}
	Vect2D_f16 result;
	result.x = fix16Div(FIX16(vector.x), len);
	result.y = fix16Div(FIX16(vector.y), len);
	return result;
}

Vect2D_f16 toPointSpd(Vect2D_s16 sourcePosInt, Vect2D_s16 destPosInt, fix16 spd){
	Vect2D_s16 to_point_vec;
	to_point_vec.x = destPosInt.x - sourcePosInt.x;
	to_point_vec.y = destPosInt.y - sourcePosInt.y;
	//normalize vector
	fix16 len = fix16Sqrt((to_point_vec.x*to_point_vec.x) + (to_point_vec.y*to_point_vec.y) >> 2);
	if(!len){
		len = 1;
	}
	Vect2D_f16 resultSpd;
	resultSpd.x = fix16Div(FIX16(to_point_vec.x), len);
	resultSpd.y = fix16Div(FIX16(to_point_vec.y), len);

	//apply speed
	resultSpd.x = fix16Mul(resultSpd.x, spd);
	resultSpd.y = fix16Mul(resultSpd.y, spd);
	return resultSpd;
}

fix16 lenToPoint(Vect2D_s16 sourcePosInt, Vect2D_s16 destPosInt){
	Vect2D_s16 to_point_vec;
	to_point_vec.x = destPosInt.x - sourcePosInt.x;
	to_point_vec.y = destPosInt.y - sourcePosInt.y;
	fix16 len = fix16Sqrt((to_point_vec.x*to_point_vec.x) + (to_point_vec.y*to_point_vec.y) >> 2);
	//>> 2 it's nessesary evil, because of small range of possible numbers in fix16 type (-512.0...512.0)
	return (len<<2);
}

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