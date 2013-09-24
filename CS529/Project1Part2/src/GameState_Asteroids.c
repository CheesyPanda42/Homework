// ---------------------------------------------------------------------------
// Project Name		:	Asteroid Game
// File Name		:	GameState_Play.cpp
// Author			:	Sun Tjen Fam
// Creation Date	:	2008/01/31
// Purpose			:	implementation of the 'play' game state
// History			:
// - 2008/01/31		:	- initial implementation
// ---------------------------------------------------------------------------

#include "main.h"
#include "GameState_Asteroids.h"

// ---------------------------------------------------------------------------
// Defines
#define GAME_OBJ_INST_NUM_MAX		2048		//The total number of different game object instances
#define GAME_OBJ_NUM_MAX			32

#define SHIP_INITIAL_NUM			3			// initial number of ship lives
#define SHIP_SIZE					40.0f		// ship size
#define SHIP_ACCEL_FORWARD			30.0f		// ship forward acceleration (in m/s^2)
#define SHIP_ACCEL_BACKWARD			10.0f		// ship backward acceleration (in m/s^2)
#define SHIP_ROT_SPEED				(2.0f * PI)	// ship rotation speed (degree/second)

#define BULLET_SPEED				15.0f		// bullet speed (m/s)
#define BULLET_SIZE					15

#define MISSILE_SIZE				30
#define MISSILE_INIT_SPEED			10
#define MISSILE_SPEED				40

#define INI_ASTEROID_CNT			5			// number of initial asteroids
#define ASTEROID_SIZE				75			// asteroid size
#define ASTEROID_MIN_SIZE			35
#define ASTEROID_SPEED				3			// speed of asteroids


#define MAX_MISSILES				15			// maximum number of missiles
// ---------------------------------------------------------------------------
enum TYPE
{
	// list of game object types
	TYPE_SHIP = 0, 
	TYPE_BULLET,
	TYPE_ASTEROID,
	TYPE_MISSILE,
	TYPE_LANDMINE,
	TYPE_TEXT,
	TYPE_NUMBER,


	TYPE_NUM
};

// ---------------------------------------------------------------------------
// object flag definition

#define FLAG_ACTIVE		0x00000001

// ---------------------------------------------------------------------------
// Struct/Class definitions

//Game object structure
typedef struct 
{
	unsigned long		type;		// object type
	AEGfxTriList*		pMesh;		// This will hold the triangles which will form the shape of the object
}GameObj;

// ---------------------------------------------------------------------------

//Game object instance structure
typedef struct 
{
	GameObj*			pObject;	// pointer to the 'original' shape
	unsigned long		flag;		// bit flag or-ed together
	float				scale;		// scaling value of the object instance
	Vector2D			posCurr;	// object current position
	Vector2D			velCurr;	// object current velocity
	float				dirCurr;	// object current direction

	Matrix2D			transform;	// object transformation matrix: Each frame, calculate the object instance's transformation matrix and save it here
	float				counter;
}GameObjInst;

// ---------------------------------------------------------------------------
// Static variables

// list of original object
static GameObj				sGameObjList[GAME_OBJ_NUM_MAX];				// Each element in this array represents a unique game object (shape)
static unsigned long		sGameObjNum;								// The number of defined game objects

// list of object instances
static GameObjInst			sGameObjInstList[GAME_OBJ_INST_NUM_MAX];	// Each element in this array represents a unique game object instance (sprite)
static unsigned long		sGameObjInstNum;							// The number of used game object instances

// pointer ot the ship object
static GameObjInst*			spShip;										// Pointer to the "Ship" game object instance	

// number of ship available (lives 0 = game over)
static long					sShipLives;									// The number of lives left

// the score = number of asteroid destroyed
static unsigned long		sScore;										// Current score

// missile related variables
static int					missileExist;
static int					numMissiles;
static GameObjInst*			missileTarget;

// ---------------------------------------------------------------------------

// functions to create/destroy a game object instance
static GameObjInst*			gameObjInstCreate (unsigned long type, float scale, Vector2D* pPos, Vector2D* pVel, float dir);
static void					gameObjInstDestroy(GameObjInst* pInst);

// ---------------------------------------------------------------------------

void MissileGetTarget();

// "Load" function of this state
void GameStateAsteroidsLoad(void)
{
	GameObj* pObj;

	// zero the game object array
	memset(sGameObjList, 0, sizeof(GameObj) * GAME_OBJ_NUM_MAX);
	// No game objects (shapes) at this point
	sGameObjNum = 0;

	// zero the game object instance array
	memset(sGameObjInstList, 0, sizeof(GameObjInst) * GAME_OBJ_INST_NUM_MAX);
	// No game object instances (sprites) at this point
	sGameObjInstNum = 0;

	// The ship object instance hasn't been created yet, so this "spShip" pointer is initialized to 0
	spShip = 0;
	missileExist = 0;
	numMissiles = 0;
	// load/create the mesh data (game objects / Shapes)
	

	// =====================
	// create the ship shape
	// =====================

	pObj		= sGameObjList + sGameObjNum++;
	pObj->type	= TYPE_SHIP;

	AEGfxTriStart();
	AEGfxTriAdd(
		-0.5f,  0.5f, 0xFFFFFFFF, 1.0f, 0.0f, 
		-0.5f, -0.5f, 0xFFFFFFFF, 0.0f, 0.0f,
		 0.5f,  0.0f, 0xFFFFFFFF, 0.5f, 1.0f); 

	pObj->pMesh = AEGfxTriEnd();
	AE_ASSERT_MESG(pObj->pMesh, "Failed to create object!!");


	// =======================
	// create the bullet shape
	// =======================
	pObj		=sGameObjList + sGameObjNum++;
	pObj->type	=TYPE_BULLET;

	AEGfxTriStart();
	AEGfxTriAdd
	(
		0.25f, 0.25f, 0xFF5F00FF, 1.0f, 0.0f,
		0.25f, -0.25f, 0xFF00FFFF, 1.0f, 1.0f,
		-0.25f, -0.25f, 0xFF052F00F, 0.0f, 0.0f
	);
	AEGfxTriAdd
	(
		0.25f, 0.25f,  0xFFF00FFF, 1.0f, 0.0f,
		-0.25f, 0.25f, 0xFFF64F0F, 0.0f, 1.0f,
		-0.25f, -0.25f,0xFFAF45FF, 0.0f, 0.0f
	);
	pObj->pMesh = AEGfxTriEnd();
	AE_ASSERT_MESG(pObj->pMesh, "Failed to create object!!");

	// =========================
	// create the asteroid shape
	// =========================
	pObj		=sGameObjList + sGameObjNum++;
	pObj->type	=TYPE_ASTEROID;

	AEGfxTriStart();
	AEGfxTriAdd
	(
		0.5f, 0.5f, 0xFFFFFFFF, 1.0f, 0.0f,
		0.5f, -0.5f, 0xFFFF00FF, 1.0f, 1.0f,
		-0.5f, -0.5f, 0xFF00FFFF, 0.0f, 1.0f
	);
	AEGfxTriAdd
	(
		0.5f, 0.5f, 0xFF0000FF, 1.0f, 0.0f,
		-0.5f, 0.5f, 0xFF0000FF, 0.0f, 0.0f,
		-0.5f, -0.5f, 0xFF0000FF, 0.0f, 1.0f
	);
	pObj->pMesh = AEGfxTriEnd();
	AE_ASSERT_MESG(pObj->pMesh, "Failed to create object!!");

	
	// ========================
	// create the missile shape
	// ========================
	pObj		=sGameObjList + sGameObjNum++;
	pObj->type	=TYPE_MISSILE;

	AEGfxTriStart();
	AEGfxTriAdd
	(
		0.5f, 0.5f, 0xFFFFFFFF, 1.0f, 0.0f,
		0.5f, -0.5f, 0xFFFF00FF, 1.0f, 1.0f,
		-0.5f, -0.5f, 0xFF00FFFF, 0.0f, 1.0f
	);
	AEGfxTriAdd
	(
		0.5f, 0.5f, 0xFF0000FF, 1.0f, 0.0f,
		-0.5f, 0.5f, 0xFF0000FF, 0.0f, 0.0f,
		-0.5f, -0.5f, 0xFF0000FF, 0.0f, 1.0f
	);
	pObj->pMesh = AEGfxTriEnd();
	AE_ASSERT_MESG(pObj->pMesh, "Failed to create object!!");


	// ========================
	// landmine
	// ========================
	pObj		=sGameObjList + sGameObjNum++;
	pObj->type	=TYPE_LANDMINE;

	AEGfxTriStart();
	AEGfxTriAdd
	(
		0.5f, 0.5f, 0xFFFFFFFF, 1.0f, 0.0f,
		0.5f, -0.5f, 0xFFFF00FF, 1.0f, 1.0f,
		-0.5f, -0.5f, 0xFF00FFFF, 0.0f, 1.0f
	);
	AEGfxTriAdd
	(
		0.5f, 0.5f, 0xFF0000FF, 1.0f, 0.0f,
		-0.5f, 0.5f, 0xFF0000FF, 0.0f, 0.0f,
		-0.5f, -0.5f, 0xFF0000FF, 0.0f, 1.0f
	);
	pObj->pMesh = AEGfxTriEnd();
	AE_ASSERT_MESG(pObj->pMesh, "Failed to create object!!");

	
	//==============================
	// text box
	//==============================
	pObj		=sGameObjList + sGameObjNum++;
	pObj->type	=TYPE_TEXT;

	AEGfxTriStart();
	AEGfxTriAdd
	(
		0.5f, 0.5f, 0xFFFFFFFF, 0.1f, 1.0f,
		0.5f, -0.5f, 0xFFFFFFFF, 0.1f, 0.0f,
		-0.5f, -0.5f, 0xFFFFFFFF, 0.0f, 0.0f
	);
	AEGfxTriAdd
	(
		0.5f, 0.5f, 0xFFFFFFFF, 0.1f, 1.0f,
		-0.5f, 0.5f, 0xFFFFFFFF, 0.0f, 1.0f,
		-0.5f, -0.5f, 0xFFFFFFFF, 0.0f, 0.0f
	);
	pObj->pMesh = AEGfxTriEnd();
	AE_ASSERT_MESG(pObj->pMesh, "Failed to create object!!");

	//==============================
	// number box
	//==============================
	pObj		=sGameObjList + sGameObjNum++;
	pObj->type	=TYPE_NUMBER;

	AEGfxTriStart();
	AEGfxTriAdd
	(
		0.5f, 0.5f, 0xFFFFFFFF, 1.0f, 0.0f,
		0.5f, -0.5f, 0xFFFFFFFF, 1.0f, 1.0f,
		-0.5f, -0.5f, 0xFFFFFFFF, 0.0f, 1.0f
	);
	AEGfxTriAdd
	(
		0.5f, 0.5f, 0xFFFFFFFF, 1.0f, 0.0f,
		-0.5f, 0.5f, 0xFFFFFFFF, 0.0f, 0.0f,
		-0.5f, -0.5f, 0xFFFFFFFF, 0.0f, 1.0f
	);
	pObj->pMesh = AEGfxTriEnd();
	AE_ASSERT_MESG(pObj->pMesh, "Failed to create object!!");
}

// ---------------------------------------------------------------------------

// "Initialize" function of this state
void GameStateAsteroidsInit(void)
{
	AEGfxSetBackgroundColor(0.0f, 0.0f, 0.0f);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);

	// create the main ship
	spShip = gameObjInstCreate(TYPE_SHIP, SHIP_SIZE, 0, 0, 0.0f);
	AE_ASSERT(spShip);

	
	// CREATE THE INITIAL ASTEROIDS INSATNCES USING THE "GAMEOBJINSTCREATE" FUNCTION
	Vector2D aVel;
	Vector2D aPos;
	float dir;

	srand(time(NULL));

	// create initial asteroids
	for (int i = 0; i < INI_ASTEROID_CNT; i++)
	{
		Vector2DSet(&aPos, rand()%800, -rand()%600);
		Vector2DSet(&aVel, (rand() % ASTEROID_SPEED) - 2.1, (rand() % ASTEROID_SPEED) - 2.1);
		dir = rand() % 360;

		gameObjInstCreate(TYPE_ASTEROID, (rand() % ASTEROID_SIZE) + ASTEROID_SIZE/2, &aPos, &aVel, dir);
	}

	// create text field
	for (int i = 0; i < 5; i++)
	{
		Vector2D tPos;
		Vector2D tVel = {0,0};
		Vector2DSet(&tPos, i*100, 0);
		gameObjInstCreate(TYPE_TEXT, 20, &tPos, &tVel, 0);
	}
	// reset the score and the number of ship
	sScore      = 0;
	sShipLives  = SHIP_INITIAL_NUM;

	missileExist = 0;
	numMissiles = 0;
}

// ---------------------------------------------------------------------------

// "Update" function of this state
void GameStateAsteroidsUpdate(void)
{
	unsigned long i;
	float winMaxX, winMaxY, winMinX, winMinY;
	double frameTime;
	double acceleration;
	Vector2D vAcc;

	// ==========================================================================================
	// Getting the window's world edges (These changes whenever the camera moves or zooms in/out)
	// ==========================================================================================
	winMaxX = AEGfxGetWinMaxX();
	winMaxY = AEGfxGetWinMaxY();
	winMinX = AEGfxGetWinMinX();
	winMinY = AEGfxGetWinMinY();

	
	// ======================
	// Getting the frame time
	// ======================

	frameTime = AEFrameRateControllerGetFrameTime();


	// =========================
	// update according to input
	// =========================

	// This input handling moves the ship without any velocity nor acceleration
	// It should be changed when implementing the Asteroids project
	if (AEInputCheckCurr(VK_UP))
	{
		Vector2D accel;
		Vector2DSet(&accel, cosf(spShip->dirCurr), sinf(spShip->dirCurr));
		Vector2DScale(&accel, &accel, SHIP_ACCEL_FORWARD);
		Vector2DScale(&accel, &accel, frameTime);
		Vector2DAdd(&spShip->velCurr, &accel, &spShip->velCurr);
		Vector2DScale(&spShip->velCurr, &spShip->velCurr, 0.85);
	}

	if (AEInputCheckCurr(VK_DOWN))
	{
		Vector2D accel;
		Vector2DSet(&accel, -cosf(spShip->dirCurr), -sinf(spShip->dirCurr));
		Vector2DScale(&accel, &accel, SHIP_ACCEL_BACKWARD);
		Vector2DScale(&accel, &accel, frameTime);
		Vector2DAdd(&spShip->velCurr, &accel, &spShip->velCurr);
		Vector2DScale(&spShip->velCurr, &spShip->velCurr, 0.8);
	}

	if (AEInputCheckCurr(VK_LEFT))
	{
		spShip->dirCurr += SHIP_ROT_SPEED * (float)(frameTime);
		spShip->dirCurr =  AEWrap(spShip->dirCurr, -PI, PI);
	}

	if (AEInputCheckCurr(VK_RIGHT))
	{
		spShip->dirCurr -= SHIP_ROT_SPEED * (float)(frameTime);
		spShip->dirCurr =  AEWrap(spShip->dirCurr, -PI, PI);
	}


	// Shoot a bullet if space is triggered (Create a new object instance)
	if (AEInputCheckTriggered(' '))
	{
		Vector2D bPos = spShip->posCurr;
		Vector2D bVel = {cosf(spShip->dirCurr) * BULLET_SPEED, sinf(spShip->dirCurr)* BULLET_SPEED};
		//Vector2DScale(&bVel, &bVel,BULLET_SPEED);

		GameObjInst * spBullet = gameObjInstCreate(TYPE_BULLET, BULLET_SIZE , &bPos, &bVel, spShip->dirCurr);
	}
	
	// create an asteroid when 'a' is pressed
	if (AEInputCheckTriggered(0x41))
	{
		Vector2D aPos;
		Vector2D aVel;
		float dir;
		
		Vector2DSet(&aPos, 0, 0);
		Vector2DSet(&aVel, (rand() % 4) - 2.1, (rand() % 4) - 2.1);
		dir = rand() % 360;

		GameObjInst * spAsteroid = gameObjInstCreate(TYPE_ASTEROID, (rand() % ASTEROID_SIZE) + ASTEROID_SIZE/2, &aPos, &aVel, dir);
		AE_ASSERT(spAsteroid);
	}

	// Shoot a homing missle if 'm' is triggered (Create a new object instance)
	if (AEInputCheckTriggered(0x4D))
	{
		if(!missileExist && numMissiles < MAX_MISSILES)
		{ 
			MissileGetTarget();

			printf("found asteroid %p, %i  x:%d  y:%d\n", missileTarget, missileTarget->pObject->type,missileTarget->posCurr.x, missileTarget->posCurr.y);
			Vector2D bPos = spShip->posCurr;
			Vector2D bVel = {cosf(spShip->dirCurr) * MISSILE_INIT_SPEED, sinf(spShip->dirCurr)* MISSILE_INIT_SPEED};

			GameObjInst * pMissile = gameObjInstCreate(TYPE_MISSILE, MISSILE_SIZE , &bPos, &bVel, spShip->dirCurr);
			pMissile->counter = 10;
			missileExist = 1;
			numMissiles ++;
		}
	}


	// Landmine if l is pressed
	if (AEInputCheckTriggered(0x4C))
	{
		Vector2D bPos = spShip->posCurr;
		Vector2D bVel = {0,0};
		//Vector2DScale(&bVel, &bVel,BULLET_SPEED);

		GameObjInst * spMine = gameObjInstCreate(TYPE_LANDMINE, 20 , &bPos, &bVel, spShip->dirCurr);
	}


	if (AEInputCheckTriggered('R'))
	{
		gGameStateNext = GS_RESTART;
	}

	// ==================================================
	// update physics of all active game object instances
	//	-- Positions are updated here
	// ==================================================
	for (i = 0; i < GAME_OBJ_INST_NUM_MAX; i++)
	{
		GameObjInst* pInst = sGameObjInstList + i;

		// skip non-active object
		if ((pInst->flag & FLAG_ACTIVE) == 0)
			continue;
		
		Vector2DAdd(&pInst->posCurr, &pInst->velCurr, &pInst->posCurr);
	}

	// ===================================
	// update active game object instances
	// Example:
	//		-- Wrap specific object instances around the world (Needed for the assignment)
	//		-- Removing the bullets as they go out of bounds (Needed for the assignment)
	//		-- If you have a homing missile for example, compute its new orientation (Homing missiles are not required for the Asteroids project)
	//		-- Update a particle effect (Not required for the Asteroids project)
	// ===================================
	for (i = 0; i < GAME_OBJ_INST_NUM_MAX; i++)
	{
		GameObjInst* pInst = sGameObjInstList + i;

		// skip non-active object
		if ((pInst->flag & FLAG_ACTIVE) == 0)
			continue;
		
		// check if the object is a ship
		if (pInst->pObject->type == TYPE_SHIP)
		{
			// warp the ship from one end of the screen to the other
			pInst->posCurr.x = AEWrap(pInst->posCurr.x, winMinX - SHIP_SIZE, winMaxX + SHIP_SIZE);
			pInst->posCurr.y = AEWrap(pInst->posCurr.y, winMinY - SHIP_SIZE, winMaxY + SHIP_SIZE);
		}
		// Wrap asteroids here
		if (pInst->pObject->type == TYPE_ASTEROID)
		{
			// warp the ship from one end of the screen to the other
			pInst->posCurr.x = AEWrap(pInst->posCurr.x, winMinX - ASTEROID_SIZE, winMaxX + ASTEROID_SIZE);
			pInst->posCurr.y = AEWrap(pInst->posCurr.y, winMinY - ASTEROID_SIZE, winMaxY + ASTEROID_SIZE);
		}
		// Remove bullets that go out of bounds
		if (pInst->pObject->type == TYPE_BULLET)
		{
			//gameObjInstDestroy(pInst);
			if(pInst->posCurr.x < winMinX || pInst->posCurr.x > winMaxX || pInst->posCurr.y < winMinY || pInst->posCurr.y > winMaxY)
				gameObjInstDestroy(pInst);
		}
		// Missile behavior
		if (pInst->pObject->type == TYPE_MISSILE)
		{
			if(pInst->posCurr.x < winMinX || pInst->posCurr.x > winMaxX || pInst->posCurr.y < winMinY || pInst->posCurr.y > winMaxY)
			{
				pInst->posCurr.x = AEWrap(pInst->posCurr.x, winMinX - MISSILE_SIZE, winMaxX + MISSILE_SIZE);
				pInst->posCurr.y = AEWrap(pInst->posCurr.y, winMinY - MISSILE_SIZE, winMaxY + MISSILE_SIZE);
			}

			pInst->counter -= frameTime;
			if (pInst -> counter < 0)
			{
				gameObjInstDestroy(pInst);
				missileExist = 0;
			}
			else
			{	
				if((missileTarget->flag & FLAG_ACTIVE) == 0)
				{
					MissileGetTarget();
				}

				Vector2DAdd(&pInst->velCurr, &pInst->velCurr, &missileTarget->velCurr);
				Vector2DScale(&pInst->velCurr, &pInst->velCurr, frameTime);
				Vector2DScale(&pInst->velCurr, &pInst->velCurr, MISSILE_SPEED);
				Vector2DScale(&pInst->velCurr, &pInst->velCurr, 0.4);
				
			}
		}
	}


	// ====================
	// check for collision
	// ====================
	for ( int i = 0; i < GAME_OBJ_INST_NUM_MAX; i ++)
	{
		GameObjInst * pInst1 = sGameObjInstList + i;

		// if instance is inactive, ignore
		if ((pInst1->flag & FLAG_ACTIVE) == 0)
			continue;
		
		// all collision checks are only if pInst1 is an asteroid
		if (pInst1->pObject->type == TYPE_ASTEROID)
		{
			for (int j = 0; j < GAME_OBJ_INST_NUM_MAX; j++)
			{
				GameObjInst * pInst2 = sGameObjInstList + j;
				int collision = 0;

				// if pInst2 is inactive it is ignored 
				// asteroid to asteroid collision is ignored
				if((pInst2->flag & FLAG_ACTIVE) == 0 || pInst2->pObject->type == TYPE_ASTEROID)
				{
					continue;
				}

				if(pInst2->pObject->type == TYPE_SHIP)
				{
					if(StaticRectToStaticRect(&pInst1->posCurr,pInst1->scale,pInst1->scale, &pInst2-> posCurr, pInst2->scale/2, pInst2->scale/2))
					{
						Vector2DSet(&pInst2->posCurr, 0,0);
						Vector2DSet(&pInst2->velCurr, 0,0);
						gameObjInstDestroy(pInst1);
						sShipLives--;
					}
				}
				else
				if(pInst2->pObject->type == TYPE_BULLET)
				{
					if(StaticPointToStaticRect(&pInst2->posCurr,&pInst1->posCurr, pInst1->scale,pInst1->scale))
					{
						for (int i = 0; i < 4; i++)
						{
							Vector2D miniVel = pInst1->velCurr;
							Vector2D randVel;
							Vector2DSet(&randVel, i*cosf(rand()%360), i*sinf(rand()%360));
							Vector2DAdd(&miniVel, &miniVel, &randVel);
							if(pInst1->scale > ASTEROID_MIN_SIZE)
								GameObjInst * spAsteroid = gameObjInstCreate(TYPE_ASTEROID, pInst1->scale/2, &pInst1->posCurr, &miniVel, pInst1->dirCurr*i);
						}
						sScore += 10;
						printf("Score %d\n", sScore);
						gameObjInstDestroy(pInst1);
						gameObjInstDestroy(pInst2);
					}

				}
				else
				if(pInst2->pObject->type == TYPE_MISSILE)
				{
					if(StaticPointToStaticRect(&pInst2->posCurr,&pInst1->posCurr, pInst1->scale,pInst1->scale))
					{
						sScore += 10;
						printf("Score %d\n", sScore);
						gameObjInstDestroy(pInst1);
						gameObjInstDestroy(pInst2);
						missileExist = 0;
					}

				}
			}
		}
	}
	// =====================================
	// calculate the matrix for all objects
	// =====================================

	for (i = 0; i < GAME_OBJ_INST_NUM_MAX; i++)
	{
		Matrix2D	 trans, rot, scale;
		GameObjInst* pInst = sGameObjInstList + i;
		

		// skip non-active object
		if ((pInst->flag & FLAG_ACTIVE) == 0)
			continue;

		// Compute the scaling matrix
		Matrix2DScale(&scale, pInst->scale,pInst->scale);
		// Compute the rotation matrix 
		Matrix2DRotRad(&rot, pInst->dirCurr);
		// Compute the translation matrix
		Matrix2DTranslate(&trans,pInst->posCurr.x , pInst->posCurr.y);

		// Concatenate the 3 matrix in the correct order in the object instance's "transform" matrix
		Matrix2DConcat(&pInst->transform, &rot, &scale);
		Matrix2DConcat(&pInst->transform, &trans, &pInst->transform);
	}

	if (sShipLives == 0 )
	{
		gGameStateNext = GS_QUIT;
	}
}

// ---------------------------------------------------------------------------

void GameStateAsteroidsDraw(void)
{
	char strBuffer[1024];
	unsigned long i;
	double frameTime;
	AEGfxTexture * asteroidTex;
	AEGfxTexture * shipTex;
	AEGfxTexture * bulletTex;
	AEGfxTexture * missileTex;
	AEGfxTexture * mineTex;
	AEGfxTexture * targetTex;
	AEGfxTexture * textTex;

	asteroidTex = AEGfxTextureLoad("asteroid.png");
	shipTex =	  AEGfxTextureLoad("spaceship.png");
	bulletTex =	  AEGfxTextureLoad("laser.png");
	missileTex =  AEGfxTextureLoad("missile.png");
	mineTex =	  AEGfxTextureLoad("landmine.png");
	targetTex =	  AEGfxTextureLoad("asteroidtarget.png");
	textTex =	  AEGfxTextureLoad("text.png");

	AEGfxSetRenderMode (AE_GFX_RM_TEXTURE);
	AEGfxTextureSet(NULL, 0, 0);
	AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);

	// draw all object instances in the list

	for (i = 0; i < GAME_OBJ_INST_NUM_MAX; i++)
	{
		GameObjInst* pInst = sGameObjInstList + i;

		// skip non-active object
		if ((pInst->flag & FLAG_ACTIVE) == 0)
			continue;
		
		if(pInst->pObject->type == TYPE_ASTEROID)
		{
			if (pInst == missileTarget) AEGfxTextureSet(targetTex,0.0f,0.0f);
			else	AEGfxTextureSet(asteroidTex, 0.0f, 0.0f);
		}
		if(pInst->pObject->type == TYPE_SHIP)
		{
			AEGfxTextureSet(shipTex, 0.0f, 0.0f);
		}
		if(pInst->pObject->type == TYPE_BULLET)
		{
			AEGfxTextureSet(bulletTex, 0.0f, 0.0f);
		}
		if(pInst->pObject->type == TYPE_MISSILE)
		{
			AEGfxTextureSet(missileTex,0.0f,0.0f);
		}
		if(pInst->pObject->type == TYPE_LANDMINE)
		{
			AEGfxTextureSet(mineTex,0.0f,0.0f);
		}
		if(pInst->pObject->type == TYPE_TEXT)
		{
			AEGfxTextureSet(textTex,0.0f,0.0f);
		}
		
		// Set the current object instance's transform matrix using "AEGfxSetTransform"
		AEGfxSetPosition (pInst->posCurr.x, pInst->posCurr.y);
		AEGfxSetTransform(pInst->transform.m);
		
		// Draw the shape used by the current object instance using "AEGfxTriDraw"
		AEGfxTriDraw(pInst->pObject->pMesh);
	}

	AEGfxTextureUnload(asteroidTex);
	AEGfxTextureUnload(shipTex);
	AEGfxTextureUnload(bulletTex);
	AEGfxTextureUnload(missileTex);
	AEGfxTextureUnload(mineTex);
	AEGfxTextureUnload(targetTex);

	frameTime = AEFrameRateControllerGetFrameTime();
	sprintf(strBuffer, "FrameTime: %lf", frameTime);
	AEGfxPrint(10, 30, -1, strBuffer);
	
	sprintf(strBuffer, "Score: %d", sScore);
	AEGfxPrint(10, 10, -1, strBuffer);

	sprintf(strBuffer, "Ship Left: %d", sShipLives >= 0 ? sShipLives : 0);
	AEGfxPrint(600, 10, -1, strBuffer);

	// display the game over message
	if (sShipLives < 0)
		AEGfxPrint(280, 260, 0xFFFFFFFF, "       GAME OVER       ");
}

// ---------------------------------------------------------------------------

void GameStateAsteroidsFree(void)
{
	// kill all object instances in the array using "gameObjInstDestroy"
	for (int i = 0; i < GAME_OBJ_INST_NUM_MAX; i++)
	{
		GameObjInst* pInst = sGameObjInstList + i;
		gameObjInstDestroy(pInst);
	}
}

// ---------------------------------------------------------------------------

void GameStateAsteroidsUnload(void)
{
	// free all mesh data (shapes) of each object using "AEGfxTriFree"
	for (int i = 0; i < GAME_OBJ_NUM_MAX; i++)
	{
		GameObj * pObj = sGameObjList + i;
		if(pObj->pMesh)
			AEGfxTriFree(pObj->pMesh);
	}
}

// ---------------------------------------------------------------------------

GameObjInst* gameObjInstCreate(unsigned long type, float scale, Vector2D* pPos, Vector2D* pVel, float dir)
{
	unsigned long i;
	Vector2D zero = { 0.0f, 0.0f };

	AE_ASSERT_PARM(type < sGameObjNum);
	
	// loop through the object instance list to find a non-used object instance
	for (i = 0; i < GAME_OBJ_INST_NUM_MAX; i++)
	{
		GameObjInst* pInst = sGameObjInstList + i;

		// check if current instance is not used
		if (pInst->flag == 0)
		{
			// it is not used => use it to create the new instance
			pInst->pObject	= sGameObjList + type;
			pInst->flag		= FLAG_ACTIVE;
			pInst->scale	= scale;
			pInst->posCurr	= pPos ? *pPos : zero;
			pInst->velCurr	= pVel ? *pVel : zero;
			pInst->dirCurr	= dir;
			pInst->counter  = 0;
			
			// return the newly created instance
			return pInst;
		}
	}

	// cannot find empty slot => return 0
	return 0;
}

// ---------------------------------------------------------------------------

void gameObjInstDestroy(GameObjInst* pInst)
{
	// if instance is destroyed before, just return
	if (pInst->flag == 0)
		return;

	// zero out the flag
	pInst->flag = 0;
}
// ---------------------------------------------------------------------------

void MissileGetTarget()
{
		int i = 0;
		GameObjInst * pObj = sGameObjInstList + i;
		while(pObj ->pObject->type != TYPE_ASTEROID && i < GAME_OBJ_INST_NUM_MAX)
		{
			i++;
			pObj = sGameObjInstList + i;
		}
		missileTarget = pObj;
}
