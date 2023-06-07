#pragma once
#include <genesis.h>

#define clamp01(X)   (min(max((X), (0)), (1)))

//$definesCode$

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
	Image* foregroundImage;
	Image* backgroundImage;
	Vect2D_s16* posArr;
	u16 posAmount;
	u8* collArr;
	Vect2D_s16 sizeinPx;
	Vect2D_s16 sizeinTiles;
	Vect2D_s16 sizeinChunks;
	u8* music;
	u32 musicSizeof;
	void (*beforeLevelFunc)(void);
	void (*everyFrameFunc)(void);
	void (*afterLevelFunc)(void);
	Palette* pal0;
	Palette* pal1;
	Palette* pal2;
	Palette* pal3;
	u8 levelMode;
	u16 controlScript;
	u16 updateCameraScript;
	u8 bgaPalIndex;
	u8 bgbPalIndex;
	u8 musicMode;
	u8 pcmChannel;
	u8 musicLoop;
	u8 freshMusicStart;
} Level;

typedef struct {
	SpriteDefinition** sprDefArr;
	u16 len;
} PreloadSprDefs;


typedef struct {
	bool alive;
	Vect2D_s16 pos;
	AABB rect;
	u8 type;
	s16 val1;
	s16 val2;
	s16 val3;
	s16 triggerHp;
	bool activated;
	bool prevActivated;
} Trigger;

/*
typedef struct {
	bool alive;
	bool preDeath;
	Vect2D_s16 posInt;
	Vect2D_f16 pos;
	Vect2D_f16 spd;
	Vect2D_s16 size;

	AABB rect;
	bool onScreen;
	s16 damage;
	Sprite* spr;
	SpriteDefinition* sprDef;
	s16 lifeTime;
	Sprite* spr2;
	Sprite* spr3;
} Bullet;
*/

typedef struct {
	char* str;
	u16 len;
} Message;

typedef struct {
	Message* messages;
	u16 len;
	char* name
} MessagePack;

//$entityBulletMergedStruct$

//$entityMergedStruct$

//$variableMergedStruct$

typedef struct
{
	u16 size;
	EntityMerged* entityArr;
} EntityMergedChunk;


typedef struct {
	u16 additionalEntityMergedSlots;
    u16 EntityBulletMerged_size;
	EntityBulletMerged* EntityBulletMerged_arr;
	//$EntityMergedArrStruct$
	u16 Trigger_size;
	Trigger* Trigger_arr;
} EntityAll;

typedef struct {
	u16 additionalEntityMergedSlots;
    u16 EntityBulletMerged_size;
	EntityBulletMerged* EntityBulletMerged_arr;
	u16 EntityMerged_size;
	EntityMerged* EntityMerged_arr;
	u16 Trigger_size;
	Trigger* Trigger_arr;
} EntityAllPlayer;

typedef struct {
    Level* lvl;
    EntityAll* entityAll_arr;
	MessagePack* messagePacks;
	LocalVariableMerged* variable_arr;
	PreloadSprDefs* preloadSprDef_arr;
	u16** sprTileIndexes;
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