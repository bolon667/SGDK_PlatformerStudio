#pragma once

#include <genesis.h>

#include "physics.h"
#include "types.h"
#include "camera.h"

struct pBody {
	Sprite* sprite;
	AABB aabb;
	AABB climbingStairAABB;

	int facingDirection;
	int speed;
	fix16 acceleration;
	fix16 deceleration;
	int climbingSpeed;
	u16 maxFallSpeed;
	u16 jumpSpeed;

	u16 currentAnimation;

	bool onGround;
	bool onStair;
	bool onSemiSolid;
	bool jumping;
	bool falling;
	bool climbingStair;
	bool attack;

	bool damaged;

	s16 animMode;
	bool animModeChangable;

	s16 hp;
	
	Vect2D_s16 position;
	Vect2D_s16 globalPosition;
	Vect2D_u16 centerOffset;

	struct {
		fix16 fixX;
		s16 x;
		fix16 fixY;
	}velocity;

	

	s16 curAmountOfJumps;
	s16 maxAmountOfJumps;

	Vect2D_s16 input;
};

extern struct pBody playerBody;
extern bool runningAnim;

extern const s16 coyoteTime;
extern s16 currentCoyoteTime;
extern const s16 jumpBufferTime;
extern s16 currentJumpBufferTime;

extern bool collidingAgainstStair;
extern bool runningAnim;

extern u16 dyingSteps;
extern const u16 dieDelay;

extern const u16 oneWayPlatformErrorCorrection;

extern u16 curPlayerAnim;

extern s16 stairLeftEdge;
extern const u16 stairPositionOffset;

extern bool facingRight;