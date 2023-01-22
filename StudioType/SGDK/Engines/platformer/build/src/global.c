#include "../inc/global.h"

//Global constant that could be used in other files
const fix16 gravityScale = FIX16(0.5);

//Level tilemap bga
Map* bga;
//Level tilemap bgb
Map* bgb;

//Size in pixels of the room
AABB roomSize;
//Player collider bounds position
AABB playerBounds;

//Index of the last tile that has been placed, useful to avoid overlapping
u16 VDPTilesFilled = TILE_USER_INDEX;