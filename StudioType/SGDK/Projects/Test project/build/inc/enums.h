#pragma once

#include <genesis.h>

typedef enum {
  TRIGGER_TYPE_changeLevel,
  TRIGGER_TYPE_changeLevelv2,
  TRIGGER_TYPE_executeScript,
  TRIGGER_TYPE_forceDownPlayer,
  TRIGGER_TYPE_forceLeftPlayer,
  TRIGGER_TYPE_forceRightPlayer,
  TRIGGER_TYPE_forceUpPlayer,
  TRIGGER_TYPE_gotSomething,
  TRIGGER_TYPE_hurt,
  TRIGGER_TYPE_showMessage,
} triggetType;
