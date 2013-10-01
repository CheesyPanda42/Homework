// --------------------------------------------------------------------------------
// Project Name		:	Platformer - Part 1
// File Name		:	BinaryMap.cpp
// Author			:	Antoine Abi Chakra
// Creation Date	:	2008/03/05
// Purpose			:	Implementation of the binary map functions
// History			:
//	- 
//© Copyright 1996-2010, DigiPen Institute of Technology (USA). All rights reserved.
// ---------------------------------------------------------------------------------

#include <fstream>
#include "BinaryMap.h"



/*The number of horizontal elements*/
static int BINARY_MAP_WIDTH;

/*The number of vertical elements*/
static int BINARY_MAP_HEIGHT;

/*This will contain all the data of the map, which will be retireved from a file
when the "ImportMapDataFromFile" function is called*/
static int **MapData;

/*This will contain the collision dataof the binary map. It will be filled in the 
"ImportMapDataFromFile" after filling "MapData". Basically, if an array element 
in MapData is 1, it represents a collision cell, any other value is a non-collision
cell*/
static int **BinaryCollisionArray;



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

// Get the map data from the text file

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