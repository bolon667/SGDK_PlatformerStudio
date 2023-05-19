#pragma once

#include <genesis.h>

#include "types.h"

Vect2D_f16 normalize_Vect2D_s16(Vect2D_s16 vector);
Vect2D_f16 toPointSpd(Vect2D_s16 sourcePosInt, Vect2D_s16 destPosInt, fix16 spd);
fix16 lenToPoint(Vect2D_s16 sourcePosInt, Vect2D_s16 destPosInt);
u16 getTileLeftEdge(u16 x);
u16 getTileRightEdge(u16 x);
u16 getTileTopEdge(u16 y);
u16 getTileBottomEdge(u16 y);
AABB getTileBounds(s16 x, s16 y);
Vect2D_u16 posToTile(Vect2D_s16 position);