// ---------------------------------------------------------------------------
// Project Name		:	Platformer
// File Name		:	GameState_Platformer.cpp
// Author			:	Sun Tjen Fam, Samir Abou Samra, Antoine Abi Chakra
// Creation Date	:	2008/03/04
// Purpose			:	Implementation of the platform game state
// History			:
//	- 
// ---------------------------------------------------------------------------



// ---------------------------------------------------------------------------


#include "AEEngine.h"
#include "GameStateMgr.h"
#include "Math2D.h"
#include "Vector2D.h"
#include "Matrix2D.h"
#include <fstream>
// ---------------------------------------------------------------------------

#define GAME_OBJ_NUM_MAX			32
#define GAME_OBJ_INST_NUM_MAX		2048

//Gameplay related variables and values
#define GRAVITY -20.0f
#define JUMP_VELOCITY 11.0f
#define MOVE_VELOCITY_HERO 4.0f
#define MOVE_VELOCITY_ENEMY 7.5f
#define ENEMY_IDLE_TIME 2.0
#define HERO_LIVES 3
static int HeroLives;
static int Hero_Initial_X;
static int Hero_Initial_Y;
static int TotalCoins;

//Flags
#define FLAG_ACTIVE			0x00000001


enum TYPE_OBJECT
{
	TYPE_OBJECT_EMPTY,			//0
	TYPE_OBJECT_COLLISION,		//1
	TYPE_OBJECT_HERO,			//2
	TYPE_OBJECT_ENEMY1,			//3
	TYPE_OBJECT_COIN			//4
};

//State machine states
enum STATE
{
	STATE_NONE,
	STATE_GOING_LEFT,
	STATE_GOING_RIGHT
};

//State machine inner states
enum INNER_STATE
{
	INNER_STATE_ON_ENTER,
	INNER_STATE_ON_UPDATE,
	INNER_STATE_ON_EXIT
};


typedef struct
{
	unsigned int	type;		// object type
	AEGfxTriList*	pMesh;		// pbject
}GameObj;


typedef struct
{
	GameObj*		pObject;	// pointer to the 'original'
	unsigned int	flag;		// bit flag or-ed together
	float			scale;
	Vector2D			posCurr;	// object current position
	Vector2D			velCurr;	// object current velocity
	float			dirCurr;	// object current direction

	Matrix2D			transform;	// object drawing matrix

	//Used to hold the current 
	int				gridCollisionFlag;

	// pointer to custom data specific for each object type
	void*			pUserData;

	//State of the object instance
	enum			STATE state;
	enum			INNER_STATE innerState;

	//General purpose counter (This variable will be used for the enemy state machine)
	double			counter;
}GameObjInst;


// list of original objects
static GameObj			*sGameObjList;
static unsigned int		sGameObjNum;

// list of object instances
static GameObjInst		*sGameObjInstList;
static unsigned int		sGameObjInstNum;

//Binary map data
static int **MapData;
static int **BinaryCollisionArray;
static int BINARY_MAP_WIDTH;
static int BINARY_MAP_HEIGHT;
static Matrix2D MapTransform;
int GetCellValue(int X, int Y);
int CheckInstanceBinaryMapCollision(float PosX, float PosY, float scaleX, float scaleY);
void SnapToCell(float *Coordinate);
int ImportMapDataFromFile(char *FileName);
void FreeMapData(void);


//Collision flags
#define	COLLISION_LEFT		0x00000001	//0001
#define	COLLISION_RIGHT		0x00000002	//0010
#define	COLLISION_TOP		0x00000004	//0100
#define	COLLISION_BOTTOM	0x00000008	//1000



// function to create/destroy a game object instance
static GameObjInst*	gameObjInstCreate (unsigned int type, float scale, Vector2D* pPos, Vector2D* pVel, float dir, enum STATE startState);
static void			gameObjInstDestroy(GameObjInst* pInst);

//We need a pointer to the hero's instance for input purposes
static GameObjInst *pHero;

//State machine functions
void EnemyStateMachine(GameObjInst *pInst);


void GameStatePlatformLoad(void)
{
	// USed for the cicle shape
	float CircleAngleStep, i;
	int Parts;
	GameObj* pObj;
	Matrix2D mapScale, mapTrans;


	sGameObjList = (GameObj *)calloc(GAME_OBJ_NUM_MAX, sizeof(GameObj));
	sGameObjInstList = (GameObjInst *)calloc(GAME_OBJ_INST_NUM_MAX, sizeof(GameObjInst));
	sGameObjNum = 0;



	//Creating the black object
	pObj		= sGameObjList + sGameObjNum++;
	pObj->type	= TYPE_OBJECT_EMPTY;

	AEGfxTriStart();

	//1st argument: X
	//2nd argument: Y
	//3rd argument: ARGB
	AEGfxTriAdd(
		-0.5f, -0.5f, 0xFF000000, 0.0f, 0.0f,
		 0.5f,  -0.5f, 0xFF000000, 0.0f, 0.0f, 
		-0.5f,  0.5f, 0xFF000000, 0.0f, 0.0f);
	
	AEGfxTriAdd(
		-0.5f, 0.5f, 0xFF000000, 0.0f, 0.0f, 
		 0.5f,  -0.5f, 0xFF000000, 0.0f, 0.0f, 
		0.5f,  0.5f, 0xFF000000, 0.0f, 0.0f);

	pObj->pMesh = AEGfxTriEnd();

		
	
	//Creating the white object
	pObj		= sGameObjList + sGameObjNum++;
	pObj->type	= TYPE_OBJECT_COLLISION;

	AEGfxTriStart();

	//1st argument: X
	//2nd argument: Y
	//3rd argument: ARGB
	AEGfxTriAdd(
		-0.5f, -0.5f, 0xFFFFFFFF, 0.0f, 0.0f, 
		 0.5f,  -0.5f, 0xFFFFFFFF, 0.0f, 0.0f, 
		-0.5f,  0.5f, 0xFFFFFFFF, 0.0f, 0.0f);
	
	AEGfxTriAdd(
		-0.5f, 0.5f, 0xFFFFFFFF, 0.0f, 0.0f, 
		 0.5f,  -0.5f, 0xFFFFFFFF, 0.0f, 0.0f, 
		0.5f,  0.5f, 0xFFFFFFFF, 0.0f, 0.0f);

	pObj->pMesh = AEGfxTriEnd();


	//Creating the hero object
	pObj		= sGameObjList + sGameObjNum++;
	pObj->type	= TYPE_OBJECT_HERO;

	AEGfxTriStart();

	//1st argument: X
	//2nd argument: Y
	//3rd argument: ARGB
	AEGfxTriAdd(
		-0.5f, -0.5f, 0xFF0000FF, 0.0f, 0.0f, 
		 0.5f,  -0.5f, 0xFF0000FF, 0.0f, 0.0f, 
		-0.5f,  0.5f, 0xFF0000FF, 0.0f, 0.0f);
	
	AEGfxTriAdd(
		-0.5f, 0.5f, 0xFF0000FF, 0.0f, 0.0f, 
		 0.5f,  -0.5f, 0xFF0000FF, 0.0f, 0.0f, 
		0.5f,  0.5f, 0xFF0000FF, 0.0f, 0.0f);

	pObj->pMesh = AEGfxTriEnd();


	//Creating the enemey1 object
	pObj		= sGameObjList + sGameObjNum++;
	pObj->type	= TYPE_OBJECT_ENEMY1;

	AEGfxTriStart();

	//1st argument: X
	//2nd argument: Y
	//3rd argument: ARGB
	AEGfxTriAdd(
		-0.5f, -0.5f, 0xFFFF0000, 0.0f, 0.0f, 
		 0.5f,  -0.5f, 0xFFFF0000, 0.0f, 0.0f, 
		-0.5f,  0.5f, 0xFFFF0000, 0.0f, 0.0f);
	
	AEGfxTriAdd(
		-0.5f, 0.5f, 0xFFFF0000, 0.0f, 0.0f, 
		 0.5f,  -0.5f, 0xFFFF0000, 0.0f, 0.0f, 
		0.5f,  0.5f, 0xFFFF0000, 0.0f, 0.0f);

	pObj->pMesh = AEGfxTriEnd();


	//Creating the Coin object
	pObj		= sGameObjList + sGameObjNum++;
	pObj->type	= TYPE_OBJECT_COIN;

	AEGfxTriStart();

	//1st argument: X
	//2nd argument: Y
	//3rd argument: ARGB

	//Creating the circle shape
	CircleAngleStep = PI/12.0f;
	Parts = 12;
	for(i = 0; i < Parts; ++i)
	{
		AEGfxTriAdd(
		0.0f, 0.0f, 0xFFFFFF00, 0.0f, 0.0f, 
		cosf(i*2*PI/Parts)*0.5f,  sinf(i*2*PI/Parts)*0.5f, 0xFFFFFF00, 0.0f, 0.0f, 
		cosf((i+1)*2*PI/Parts)*0.5f,  sinf((i+1)*2*PI/Parts)*0.5f, 0xFFFFFF00, 0.0f, 0.0f);
	}

	pObj->pMesh = AEGfxTriEnd();

	//Setting intital binary map values
	MapData = 0;
	BinaryCollisionArray = 0;
	BINARY_MAP_WIDTH = 0;
	BINARY_MAP_HEIGHT = 0;

	//Importing Data
	if(!ImportMapDataFromFile("Exported.txt"))
		gGameStateNext = GS_QUIT;


	//Computing the matrix which take a point out of the normalized coordinates system
	//of the binary map
	/***********
	Compute a transformation matrix and save it in "MapTransform".
	This transformation transforms any point from the normalized coordinates system of the binary map.
	Later on, when rendering each object instance, we should concatenate "MapTransform" with the
	object instance's own transformation matrix

	Compute a translation matrix (-Grid width/2, -Grid height/2) and save it in "mapTrans"
	Compute a scaling matrix and save it in "mapScale")
	Concatenate scale * translate and save the result in "MapTransform"
	***********/
	Matrix2D MapTrans;
	Matrix2D MapScale;
	
	Matrix2DTranslate (&MapTrans, -BINARY_MAP_WIDTH/2,-BINARY_MAP_WIDTH/2);
	Matrix2DScale     (&MapScale, 1, 1);
	Matrix2DConcat   (&MapTransform, &MapTrans, &MapScale);
	
}

void GameStatePlatformInit(void)
{
	int i, j;
	GameObjInst *pInst;
	Vector2D Pos;

	pHero = 0;
	TotalCoins = 0;

	//Setting the inital number of hero lives
	HeroLives = HERO_LIVES;

	// Creating the map, the main character, the enemies and the coins according to their initial positions in MapData
	// Looping twice might be needed in order to create the map cells first

	/***********
	Loop through all the array elements of MapData (which was initialized in the "GameStatePlatformLoad" function
	from the .txt file
		
		 - Create a white or black cell

		 - if the element represents the hero
			Create a hero instance
			Set its position depending on its array indices in MapData
			Save its array indices in Hero_Initial_X and Hero_Initial_Y (Used when the hero dies and its position needs to be reset)

		 - if the element represents an enemy
			Create an enemy instance
			Set its position depending on its array indices in MapData
			
		 - if the element represents a coin
			Create a coin instance
			Set its position depending on its array indices in MapData
	***********/
	for(i = 0; i < BINARY_MAP_WIDTH; ++i)
		for(j = 0; j < BINARY_MAP_HEIGHT; ++j)
		{
			int scale = 5;
			Vector2D pos;
			Vector2D vel;
			Vector2DSet(&pos, BINARY_MAP_WIDTH*j, BINARY_MAP_HEIGHT*i);
			Vector2DSet(&vel, 0,0);
			switch(MapData[i][j])
			{
			case(TYPE_OBJECT_EMPTY):
									{
										gameObjInstCreate(TYPE_OBJECT_EMPTY, scale, &pos,&vel, 0,STATE_NONE);
										break;
									}
			case(TYPE_OBJECT_COLLISION):
									{
										gameObjInstCreate(TYPE_OBJECT_COLLISION, scale, &pos,&vel, 0,STATE_NONE);
										break;
									}
			case(TYPE_OBJECT_HERO):
				break;
			case(TYPE_OBJECT_ENEMY1):
				break;
			case(TYPE_OBJECT_COIN):
				break;
			}
		}
}

void GameStatePlatformUpdate(void)
{
	int i, j;
	GameObjInst *pInst;
	float winMaxX, winMaxY, winMinX, winMinY;
	double frameTime;
	
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

	//Handle Input
	/***********
	if right is pressed
		Set hero velocity X to MOVE_VELOCITY_HERO
	else
	if left is pressed
		Set hero velocity X to -MOVE_VELOCITY_HERO
	else
		Set hero velocity X to 0

	if space is pressed AND Hero is colliding from the bottom
		Set hero velocity Y to JUMP_VELOCITY

	if q is pressed
		Exit game
	***********/


	//Update object instances physics and behavior
	for(i = 0; i < GAME_OBJ_INST_NUM_MAX; ++i)
	{
		GameObjInst *pInst = sGameObjInstList + i;

		// skip non-active object
		if (0 == (pInst->flag & FLAG_ACTIVE))
			continue;


		/****************
		Apply gravity
			Velcoty Y = Gravity * Frame Time + Velocity Y

		If object instance is an enemy
			Apply enemy state machine
		****************/
	}

	//Update object instances positions
	for(i = 0; i < GAME_OBJ_INST_NUM_MAX; ++i)
	{
		GameObjInst *pInst = sGameObjInstList + i;

		// skip non-active object
		if (0 == (pInst->flag & FLAG_ACTIVE))
			continue;

		/**********
		update the position using: P1 = V1*t + P0
		**********/
	}

	//Check for grid collision
	for(i = 0; i < GAME_OBJ_INST_NUM_MAX; ++i)
	{
		GameObjInst *pInst = sGameObjInstList + i;

		// skip non-active object instances
		if (0 == (pInst->flag & FLAG_ACTIVE))
			continue;

		/*************
		Update grid collision flag

		if collision from bottom
			Snap to cell on Y axis
			Velocity Y = 0

		if collision from top
			Snap to cell on Y axis
			Velocity Y = 0
	
		if collision from left
			Snap to cell on X axis
			Velocity X = 0

		if collision from right
			Snap to cell on X axis
			Velocity X = 0
		*************/
	}


	//Checking for collision among object instances:
	//Hero against enemies
	//Hero against coins

	/**********
	for each game object instance
		Skip if it's inactive or if it's non collidable

		If it's an enemy
			If collision between the enemy instance and the hero (rectangle - rectangle)
				Decrement hero lives
				Reset the hero's position in case it has lives left, otherwise restart the level

		If it's a coin
			If collision between the coin instance and the hero (circle - rectangle)
				Remove the coin and decrement the coin counter.
				Quit the game in case no more coins are left
	**********/
	for(i = 0; i < GAME_OBJ_INST_NUM_MAX; ++i)
	{
	}

	
	//Computing the transformation matrices of the game object instances
	for(i = 0; i < GAME_OBJ_INST_NUM_MAX; ++i)
	{
		Matrix2D scale, rot, trans;
		GameObjInst *pInst = sGameObjInstList + i;

		// skip non-active object
		if (0 == (pInst->flag & FLAG_ACTIVE))
			continue;

		Matrix2DScale(&scale, pInst->scale, pInst->scale);
		Matrix2DRotDeg(&rot, pInst->dirCurr);
		Matrix2DTranslate(&trans, pInst->posCurr.x, pInst->posCurr.y);

		Matrix2DConcat(&pInst->transform, &trans, &rot);
		Matrix2DConcat(&pInst->transform, &pInst->transform, &scale);
	}
}

void GameStatePlatformDraw(void)
{
	//Drawing the tile map (the grid)
	int i, j;
	Matrix2D cellTranslation, cellFinalTransformation;
	double frameTime;
	char strBuffer[100];


	// ======================
	// Getting the frame time
	// ======================

	frameTime = AEFrameRateControllerGetFrameTime();

	
	AEGfxSetRenderMode(AE_GFX_RM_COLOR);


	//Drawing the tile map

	/******REMINDER*****
	You need to concatenate MapTransform with the transformation matrix of any object you want to draw.
	MapTransform transform the instance from the normalized coordinates system of the binary map
	*******************/


	//Drawing the object instances
	/**********
	For each active and visible object instance
		Concatenate MapTransform with its transformation matrix
		Send the resultant matrix to the graphics manager using "AEGfxSetTransform"
		Draw the instance's shape using "AEGfxTriDraw"
	**********/
	for (i = 0; i < GAME_OBJ_INST_NUM_MAX; i++)
	{
		GameObjInst* pInst = sGameObjInstList + i;

		// skip non-active object
		if (0 == (pInst->flag & FLAG_ACTIVE))
			continue;
		
		//Don't forget to concatenate the MapTransform matrix with the transformation of each game object instance
		
		
		

	}

}


void GameStatePlatformFree(void)
{
	unsigned int i;
	// kill all object in the list
	for (i = 0; i < GAME_OBJ_INST_NUM_MAX; i++)
		gameObjInstDestroy(sGameObjInstList + i);
}

void GameStatePlatformUnload(void)
{
	unsigned int i;
	// free all CREATED mesh
	for (i = 0; i < sGameObjNum; i++)
		AEGfxTriFree(sGameObjList[i].pMesh);

	/*********
	Free the map data
	*********/
}


GameObjInst* gameObjInstCreate(unsigned int type, float scale, Vector2D* pPos, Vector2D* pVel, float dir, enum STATE startState)
{
	unsigned int i;
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
			pInst->pObject	 = sGameObjList + type;
			pInst->flag		 = FLAG_ACTIVE;
			pInst->scale	 = scale;
			pInst->posCurr	 = pPos ? *pPos : zero;
			pInst->velCurr	 = pVel ? *pVel : zero;
			pInst->dirCurr	 = dir;
			pInst->pUserData = 0;
			pInst->gridCollisionFlag = 0;
			pInst->state = startState;
			pInst->innerState = INNER_STATE_ON_ENTER;
			pInst->counter = 0;
			
			// return the newly created instance
			return pInst;
		}
	}
}

void gameObjInstDestroy(GameObjInst* pInst)
{
	// if instance is destroyed before, just return
	if (pInst->flag == 0)
		return;

	// zero out the flag
	pInst->flag = 0;
}


//import map data
int ImportMapDataFromFile(char *FileName)
{
	std::fstream input;
	int read;
	char dummy[10];
	int i,j;
	
	// dummy string for input
	memset(dummy,0,10);

	// Open map fine and ensure it opened correctly
	input.open(FileName);
	if (!input.is_open())
	{
		printf("Couldn't open file %s/n", FileName);
		return 0;
	}

	// Get width data
	input >> dummy;
	input >> BINARY_MAP_WIDTH;

	// Get height data
	input >> dummy;
	input >> BINARY_MAP_HEIGHT;

	// Initialize map and collision data arrays
	MapData = new int * [BINARY_MAP_HEIGHT];
	BinaryCollisionArray = new int * [BINARY_MAP_HEIGHT];

	for (i = 0; i < BINARY_MAP_HEIGHT; i++)
	{
		MapData[i] = new int[BINARY_MAP_WIDTH];
		BinaryCollisionArray[i] = new int[BINARY_MAP_HEIGHT];
	}

	// Read in map and collision data
	while(!input.eof())
	{
		for (i = 0; i < BINARY_MAP_WIDTH; i++)
		{
			for (j = 0; j < BINARY_MAP_HEIGHT; j++)
			{
				// if map data is a collision point, store in collision data
				// all other cases, collision data is 0
				input >> MapData[j][(BINARY_MAP_WIDTH-1)-i];
				if (MapData[i][j] == 1) BinaryCollisionArray[i][j] = MapData[i][j];
				else BinaryCollisionArray[i][j] = 0;
			}
		}
	}

	input.close();
	return 1;
}

void FreeMapData(void)
{
	int i;
	{
		for (i = 0; i < BINARY_MAP_WIDTH; i++)
		{
			delete[] MapData[i];
			delete[] BinaryCollisionArray[i];
		}
		delete[] MapData;
		delete[] BinaryCollisionArray;
	}
} 

void PrintRetrievedInformation(void)
{
	int i, j;

	printf("Width: %i\n", BINARY_MAP_WIDTH);
	printf("Height: %i\n", BINARY_MAP_HEIGHT);

	printf("Map Data:\n");
	for(j = BINARY_MAP_HEIGHT - 1; j >= 0; --j)
	{
		for(i = 0; i < BINARY_MAP_WIDTH; ++i)
		{
			printf("%i ", MapData[i][j]);
		}

		printf("\n");
	}

	printf("\n\nBinary Collision Data:\n");
	for(j = BINARY_MAP_HEIGHT - 1; j >= 0; --j)
	{
		for(i = 0; i < BINARY_MAP_WIDTH; ++i)
		{
			printf("%i ", BinaryCollisionArray[i][j]);
		}

		printf("\n");
	}
}


int GetCellValue(int X, int Y)
{
	int cellData;

	if (X > BINARY_MAP_WIDTH-1 || X < 0 || Y > BINARY_MAP_HEIGHT-1 || Y < 0)
	{
		return 0;
	}

	cellData = BinaryCollisionArray[X][Y];

	return cellData;
}


int CheckInstanceBinaryMapCollision(float PosX, float PosY, float scaleX, float scaleY)
{
	int Flag = 0;
	float x1, x2, y1, y2;

	// check top
		// HS 1
		y1 = PosY + scaleY/2;
		x1 = PosX + scaleX/4;
		// HS 2
		y2 = PosY + scaleY/2;
		x2 = PosX - scaleX/4;

	if(GetCellValue(x1,y1) == 1) Flag |= COLLISION_TOP;
	if(GetCellValue(x2,y2) == 1) Flag |= COLLISION_TOP;

	// check bottom
		// HS 1
		y1 = PosY - scaleY/2;
		x1 = PosX + scaleX/4;
		// HS 2
		y2 = PosY - scaleY/2;
		x2 = PosX - scaleX/4;		

	if(GetCellValue(x1,y1) == 1) Flag |= COLLISION_BOTTOM;
	if(GetCellValue(x2,y2) == 1) Flag |= COLLISION_BOTTOM;


	// check left
		// HS 1
		y1 = PosY + scaleY/4;
		x1 = PosX - scaleX/2;
		// HS 2
		y2 = PosY - scaleY/4;
		x2 = PosX - scaleX/2;		

	if(GetCellValue(x1,y1) == 1) Flag |= COLLISION_LEFT;
	if(GetCellValue(x2,y2) == 1) Flag |= COLLISION_LEFT;


	// check right
		// HS 1
		y1 = PosY + scaleY/4;
		x1 = PosX + scaleX/2;
		// HS 2
		y2 = PosY - scaleY/4;
		x2 = PosX + scaleX/2;		

	if(GetCellValue(x1,y1) == 1) Flag |= COLLISION_RIGHT;
	if(GetCellValue(x2,y2) == 1) Flag |= COLLISION_RIGHT;

	return Flag;
}


void SnapToCell(float *Coordinate)
{
	*Coordinate = (int)*Coordinate + 0.5;
}






void EnemyStateMachine(GameObjInst *pInst)
{
	/***********
	This state machine has 2 states: STATE_GOING_LEFT and STATE_GOING_RIGHT
	Each state has 3 inner states: INNER_STATE_ON_ENTER, INNER_STATE_ON_UPDATE, INNER_STATE_ON_EXIT
	Use "switch" statements to determine which state and inner state the enemy is currently in.


	STATE_GOING_LEFT
		INNER_STATE_ON_ENTER
			Set velocity X to -MOVE_VELOCITY_ENEMY
			Set inner state to "on update"

		INNER_STATE_ON_UPDATE
			If collision on left side OR bottom left cell is non collidable
				Initialize the counter to ENEMY_IDLE_TIME
				Set inner state to on exit
				Set velocity X to 0


		INNER_STATE_ON_EXIT
			Decrement counter by frame time
			if counter is less than 0 (sprite's idle time is over)
				Set state to "going right"
				Set inner state to "on enter"

	STATE_GOING_RIGHT is basically the same, with few modifications.

	***********/
}