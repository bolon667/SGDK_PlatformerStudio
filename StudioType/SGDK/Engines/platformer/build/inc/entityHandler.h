#pragma once

#include <genesis.h>

#include "../inc/levels.h"

EntityMerged* getEntityMergedByInstId(u16 instId);
void deleteEntityMergedTriggerByInstId(u16 instId);
void showEntityAll();
void showEntityMerged(EntityMerged *entity);



