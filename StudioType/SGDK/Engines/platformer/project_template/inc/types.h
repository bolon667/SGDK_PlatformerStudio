#pragma once
#include <genesis.h>

#define clamp01(X)   (min(max((X), (0)), (1)))

typedef struct {
	u8 x;
	u8 y;
}Vect2D_u8;

typedef struct {
	s8 x;
	s8 y;
}Vect2D_s8;

typedef struct {
	Vect2D_s16 min;
	Vect2D_s16 max;
}AABB;

typedef struct {
	MapDefinition* foregroundMap;
	MapDefinition* backgroundMap;
	TileSet* foregroundTileset;
	TileSet* backgroundTileset;
	Palette* foregroundPallete;
	Palette* backgroundPallete;
	Vect2D_s16 startPos;
	u8* collArr;
	Vect2D_s16 sizeinPx;
	Vect2D_s16 sizeinTiles;
} Level;

typedef struct {
	bool alive;
	Vect2D_s16 firstPos;
	Vect2D_s16 lastPos;
	s8 trigger_type;
	s8 trigger_value;
	s16 triggerHp;
} Trigger;

//$entityMergedStruct$

typedef struct {
    u16 EntityMerged_size;
	EntityMerged* EntityMerged_arr;
	u16 Trigger_size;
	Trigger* Trigger_arr;
} EntityAll;

typedef struct {
    Level lvl;
    EntityAll* entityAll_arr;
} LevelFull;

AABB newAABB(s16 x1, s16 x2, s16 y1, s16 y2);

Vect2D_f16 newVector2D_f16(f16 x, f16 y);
Vect2D_f32 newVector2D_f32(f32 x, f32 y);

Vect2D_s8 newVector2D_s8(s8 x, s8 y);
Vect2D_s16 newVector2D_s16(s16 x, s16 y);
Vect2D_s32 newVector2D_s32(s32 x, s32 y);

Vect2D_u8 newVector2D_u8(u8 x, u8 y);
Vect2D_u16 newVector2D_u16(u16 x, u16 y);
Vect2D_u32 newVector2D_u32(u32 x, u32 y);