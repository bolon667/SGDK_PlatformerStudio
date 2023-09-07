#pragma once

#include <genesis.h>

#include "types.h"
#include "levels.h"

Vect2D_f16 normalize_Vect2D_s16(Vect2D_s16 vector);
Vect2D_f16 toPointSpd(Vect2D_s16 sourcePosInt, Vect2D_s16 destPosInt, fix16 spd);
Vect2D_f16 toPointSpd_Vect2D_s32(Vect2D_s32 sourcePosInt, Vect2D_s32 destPosInt, fix16 spd);
fix16 lenToPoint(Vect2D_s32 sourcePosInt, Vect2D_s32 destPosInt);
u16 getTileLeftEdge(u16 x);
u16 getTileRightEdge(u16 x);
u16 getTileTopEdge(u16 y);
u16 getTileBottomEdge(u16 y);
AABB getTileBounds(s16 x, s16 y);
Vect2D_u16 posToTile(Vect2D_s16 position);
extern void initMoveFromAToB(Vect2D_s32* posA, Vect2D_s32* posB, f16 desiredSpd, Vect2D_f16* spdVect, s16* stepsCount);
extern void initMoveCurPosToB(EntityMerged* entity, s16 moveSpd);
extern void moveStepToPos1_entityMerged(EntityMerged* entity);
extern void moveStepToPosCustom_entityMerged(EntityMerged* entity, u16 posNum);
extern void teleportStepToPos1_entityMerged(EntityMerged* entity);
extern void moveStepToPos1Stop_entityMerged(EntityMerged* entity);