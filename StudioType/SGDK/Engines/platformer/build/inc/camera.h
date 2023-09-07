#pragma once

#include <genesis.h>

#include "types.h"
#include "player.h"
#include "physics.h"
#include "levels.h"

extern Vect2D_s32 cameraPosition;
extern Vect2D_s32 cameraPosition2;
extern Vect2D_f16 cameraSpdBuffer;
extern Vect2D_f16 cameraSpd;

void setupCamera(Vect2D_u16 deadZoneCenter, u16 deadZoneWidth, u16 deadZoneHeight);
void updateCamera();