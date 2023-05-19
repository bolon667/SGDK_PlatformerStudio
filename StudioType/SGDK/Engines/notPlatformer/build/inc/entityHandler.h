#pragma once

#include <genesis.h>

#include "../inc/levels.h"

EntityMerged* getEntityMergedByInstId(u16 instId);
u16 getEntityMergedIndInArrByInstId(u16 instId);
u16 getTriggerParrentIndInArr(Trigger* trigger);
void deleteEntityMergedTriggerByInstId(u16 instId);
void showEntityAll();
void showEntityMerged(EntityMerged *entity);

void(* showEntityAll_arr[])(void);


