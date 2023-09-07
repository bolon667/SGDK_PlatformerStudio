#pragma once

#include <genesis.h>

#include "../inc/levels.h"

extern u32 findSprOptIndexBySpriteDefinition(SpriteDefinition* sprDef);
extern void checkTriggerFor_EntityMerged_trigger(EntityMerged* entity, Trigger* trigger);
EntityMerged* getEntityMergedByInstId(u16 instId);
u16 getEntityMergedIndInArrByInstId(u16 instId);
u16 getTriggerParrentIndInArr(Trigger* trigger);
void deleteEntityMergedTriggerByInstId(u16 instId);
void showEntityAll();
void showEntityMerged(EntityMerged *entity);
extern void frameChangedSprOpt1(Sprite* sprite);

void(* showEntityAll_arr[])(void);


