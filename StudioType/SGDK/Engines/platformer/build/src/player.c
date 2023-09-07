#include "../inc/player.h"

#include "../inc/global.h"
#include "../inc/maps.h"

#include "../res/resources.h"
#include "../res/sprites.h"

pBody playerBody;
pBody playerBody2;

bool runningAnim;

const s16 coyoteTime = 10;
s16 currentCoyoteTime;
const s16 jumpBufferTime = 10;
s16 currentJumpBufferTime;

bool collidingAgainstStair;
bool runningAnim;

u16 dyingSteps;
const u16 dieDelay = 10;

const u16 oneWayPlatformErrorCorrection = 5;

u16 curPlayerAnim = 0;

s16 stairLeftEdge;
const u16 stairPositionOffset = 4;

bool facingRight = FALSE;
