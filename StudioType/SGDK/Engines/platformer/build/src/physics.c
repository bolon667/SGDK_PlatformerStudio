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

Vect2D_f16 toPointSpd_Vect2D_s32(Vect2D_s32 sourcePosInt, Vect2D_s32 destPosInt, fix16 spd){
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

fix16 lenToPoint(Vect2D_s32 sourcePosInt, Vect2D_s32 destPosInt){
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

void initMoveFromAToB(Vect2D_s32* posA, Vect2D_s32* posB, f16 desiredSpd, Vect2D_f16* spdVect, s16* stepsCount){
	Vect2D_s32 to_point_vec;
	to_point_vec.x = posB->x - posA->x;
	to_point_vec.y = posB->y - posA->y;
	//normalize vector
	fix16 len = fix16Sqrt((to_point_vec.x*to_point_vec.x) + (to_point_vec.y*to_point_vec.y) >> 2);
	if(!len){
		len = 1;
	}
	Vect2D_f16 resultSpd;
	resultSpd.x = fix16Div(FIX16(to_point_vec.x), len);
	resultSpd.y = fix16Div(FIX16(to_point_vec.y), len);

	//apply speed
	resultSpd.x = fix16Mul(resultSpd.x, desiredSpd);
	resultSpd.y = fix16Mul(resultSpd.y, desiredSpd);

	*spdVect = resultSpd;

	//if stepsCount not NULL
	if(stepsCount){
		//Changing amount of steps
		*stepsCount = fix16ToInt(fix16Div(len << 4, desiredSpd));
	}
}

void initMoveCurPosToB(EntityMerged* entity, s16 moveSpd){
	Vect2D_s32 to_point_vec;
	to_point_vec.x = entity->toPos->pos.x - entity->posInt.x;
	to_point_vec.y = entity->toPos->pos.y - entity->posInt.y;

	if(to_point_vec.x == 0) {
		to_point_vec.x = 1;
	}
	if(to_point_vec.y == 0) {
		to_point_vec.y = 1;
	}
	//normalize vector
	fix16 len = fix16Sqrt((to_point_vec.x*to_point_vec.x) + (to_point_vec.y*to_point_vec.y) >> 2);
	if(len == 0){
		len = 1;
	}
	
	Vect2D_f16 resultSpd;
	resultSpd.x = fix16Div(FIX16(to_point_vec.x), len);
	resultSpd.y = fix16Div(FIX16(to_point_vec.y), len);

	//apply speed
	resultSpd.x = fix16Mul(resultSpd.x, moveSpd);
	resultSpd.y = fix16Mul(resultSpd.y, moveSpd);

	entity->spd = resultSpd;

	//Changing amount of steps
	entity->val1 = fix16ToInt(fix16Div(len << 4, moveSpd));

	//Turning on movement
	entity->val2 = 1;

	//Changing movement speed
	entity->val3 = moveSpd;
}

void moveStepToPos1_entityMerged(EntityMerged* entity){
	// if turn_on
	if(entity->val2 > 0) {
		entity->val1--;
		// KLog_S1("test1: ", *stepsCount);

		//If you got trough all steps
		if(entity->val1 == 0){
			//Stoping object
			entity->spd.x = 0;
			entity->spd.y = 0;
			
			//Teleporting to end pos
			entity->pos.x = FIX32(entity->toPos->pos.x);
			entity->pos.y = FIX32(entity->toPos->pos.y);
			//turning off movement
			entity->val2 = 0;

			//if you have ties with other positions, then, move to first tie
			if(entity->toPos->tiedWith_ids_arr_size){
				// KLog_S1("test2: ", entity->toPos->tiedWith_ids_arr[0]);
				Position* firstPos = entity->toPos;
				entity->toPos = getLevelPosById(entity->toPos->tiedWith_ids_arr[0]);
				initMoveFromAToB(&firstPos->pos, &entity->toPos->pos, entity->val3, &entity->spd, &entity->val1);
				entity->val2 = 1;
			}
		}
	}
}

void moveStepToPos1Stop_entityMerged(EntityMerged* entity){
	// if turn_on
	if(entity->val2 > 0) {
		entity->val1--;
		// KLog_S1("test1: ", *stepsCount);

		//If you got trough all steps
		if(entity->val1 == 0){
			//Stoping object
			entity->spd.x = 0;
			entity->spd.y = 0;
			
			//Teleporting to end pos
			entity->pos.x = FIX32(entity->toPos->pos.x);
			entity->pos.y = FIX32(entity->toPos->pos.y);
			//turning off movement
			entity->val2 = 0;

			if(entity->toPos->tiedWith_ids_arr_size){
				// Position* firstPos = entity->toPos;
				entity->toPos = getLevelPosById(entity->toPos->tiedWith_ids_arr[0]);
			}
		}
	}
}

void moveStepToPosCustom_entityMerged(EntityMerged* entity, u16 posNum){
	// if turn_on
	if(entity->val2 > 0) {
		entity->val1--;
		// KLog_S1("test1: ", *stepsCount);

		//If you got trough all steps
		if(entity->val1 == 0){
			//Stoping object
			entity->spd.x = 0;
			entity->spd.y = 0;
			
			//Teleporting to end pos
			entity->pos.x = FIX32(entity->toPos->pos.x);
			entity->pos.y = FIX32(entity->toPos->pos.y);
			//turning off movement
			entity->val2 = 0;

			//if you have ties with other positions, then, move to first tie
			if(entity->toPos->tiedWith_ids_arr_size){
				// KLog_S1("test2: ", entity->toPos->tiedWith_ids_arr[0]);
				Position* firstPos = entity->toPos;
				entity->toPos = getLevelPosById(entity->toPos->tiedWith_ids_arr[posNum]);
				initMoveFromAToB(&firstPos->pos, &entity->toPos->pos, entity->val3, &entity->spd, &entity->val1);
				entity->val2 = 1;
			}
		}
	}
}

void teleportStepToPos1_entityMerged(EntityMerged* entity){		
	//Teleporting to end pos
	entity->posInt.x = entity->toPos->pos.x;
	entity->posInt.y = entity->toPos->pos.y;

	entity->pos.x = FIX32(entity->posInt.x);
	entity->pos.y = FIX32(entity->posInt.y);


	//turning off teleport
	//if you have ties with other positions, then, move to first tie
	if(entity->toPos->tiedWith_ids_arr_size){
		// KLog_S1("test2: ", entity->toPos->tiedWith_ids_arr[0]);
		entity->toPos = getLevelPosById(entity->toPos->tiedWith_ids_arr[0]);
	}
}
