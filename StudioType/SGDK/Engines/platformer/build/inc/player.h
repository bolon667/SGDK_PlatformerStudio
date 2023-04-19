#pragma once

#include <genesis.h>

#include "physics.h"
#include "types.h"
#include "camera.h"

struct pBody {
	Sprite* sprite;
	Sprite* debugSpr1;
	Sprite* debugSpr2;
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
	bool semiSolidHasCollision;
	bool jumping;
	bool falling;
	bool climbingStair;
	bool attack;

	bool damaged;
	bool godMode;
	bool isVisible;

	s16 animMode;
	s16 val1;
	s16 state;
	bool isCombo;
	bool animModeChangable;

	u16 attackType;

	bool dead;
	s16 hp;

	u16 vdpIndexPos;
	
	Vect2D_s16 position;
	Vect2D_s16 globalPosition;
	Vect2D_u16 centerOffset;

	struct {
		fix16 fixX;
		s16 x;
		fix16 fixY;
		fix16 bufferFixX;
		fix16 bufferFixY;
	}velocity;

	s16 timer;

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