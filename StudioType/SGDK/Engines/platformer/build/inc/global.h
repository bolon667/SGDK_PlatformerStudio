#pragma once

#include <genesis.h>

#include "physics.h"

#define TILEMAP_PLANE BG_A

#define PLAYER_PALETTE PAL2
#define ENEMY_PALETTE PAL3
#define LEVEL_PALETTE PAL0

#define GROUND_TILE 1
#define LADDER_TILE 2
#define ONE_WAY_PLATFORM_UP_TILE 4
#define ONE_WAY_PLATFORM_DOWN_TILE 3
#define SLOPE_90_RIGHT 5
#define SLOPE_90_LEFT 6
#define SLOPE_90_LEFT_UP 7
#define SLOPE_90_RIGHT_UP 8
#define SLOPE_45_RIGHT1 9
#define SLOPE_45_RIGHT2 10
#define SLOPE_45_LEFT2 11
#define SLOPE_45_LEFT1 12
#define SLOPE_45_RIGHT1_UP 13
#define SLOPE_45_RIGHT2_UP 14
#define SLOPE_45_LEFT2_UP 15
#define SLOPE_45_LEFT1_UP 16


extern const fix16 gravityScale;

struct {
	u16 joy;
	u16 changed;
	u16 state;
}input;

extern Map* bga;
extern Map* bgb;

extern AABB roomSize;
extern AABB playerBounds;

extern u16 VDPTilesFilled;