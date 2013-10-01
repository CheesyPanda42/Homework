/* Start Header -------------------------------------------------------
Copyright (C) 2013 DigiPen Institute of Technology. Reproduction or disclosure of this file or its contents without the prior written consent of DigiPen Institute of Technology is prohibited.
File Name: Vector2D.c
Purpose: Implementation file for the 2D Vector library
Language: C++
Project: CS529_coryprelerson_1
Author: Cory Prelerson, cory.prelerson, 60000913
Creation date: 9.16.2013
- End Header -----------------------------------------------------*/
#include "Vector2D.h"

#define EPSILON 0.0001

// ---------------------------------------------------------------------------

void Vector2DZero(Vector2D *pResult)
{
	pResult->x = 0;
	pResult->y = 0;
}

// ---------------------------------------------------------------------------

void Vector2DSet(Vector2D *pResult, float x, float y)
{
	pResult->x = x;
	pResult->y = y;
}

// ---------------------------------------------------------------------------

void Vector2DNeg(Vector2D *pResult, Vector2D *pVec0)
{
	pResult->x = -pVec0->x;
	pResult->y = -pVec0->y;
}

// ---------------------------------------------------------------------------

void Vector2DAdd(Vector2D *pResult, Vector2D *pVec0, Vector2D *pVec1)
{
	pResult->x = pVec0->x + pVec1->x;
	pResult->y = pVec0->y + pVec1->y;
}

// ---------------------------------------------------------------------------

void Vector2DSub(Vector2D *pResult, Vector2D *pVec0, Vector2D *pVec1)
{
	pResult->x = pVec0->x - pVec1->x;
	pResult->y = pVec0->y - pVec1->y;
}

// ---------------------------------------------------------------------------

void Vector2DNormalize(Vector2D *pResult, Vector2D *pVec0)
{
	float length = Vector2DLength(pVec0);
	pResult->x = pVec0->x/length;
	pResult->y = pVec0->y/length;

}

// ---------------------------------------------------------------------------

void Vector2DScale(Vector2D *pResult, Vector2D *pVec0, float c)
{
	pResult->x = c * pVec0->x;
	pResult->y = c * pVec0->y;

}

// ---------------------------------------------------------------------------

void Vector2DScaleAdd(Vector2D *pResult, Vector2D *pVec0, Vector2D *pVec1, float c)
{
	Vector2DScale(pResult,pVec0,c);
	Vector2DAdd(pResult,pResult,pVec1);

}

// ---------------------------------------------------------------------------

void Vector2DScaleSub(Vector2D *pResult, Vector2D *pVec0, Vector2D *pVec1, float c)
{
	Vector2DScale(pResult,pVec0,c);
	Vector2DSub(pResult,pResult,pVec1);
}

// ---------------------------------------------------------------------------

float Vector2DLength(Vector2D *pVec0)
{
	float length;
	length = sqrt((pVec0->x * pVec0->x) + (pVec0->y * pVec0->y));
	return length;
}

// ---------------------------------------------------------------------------

float Vector2DSquareLength(Vector2D *pVec0)
{
	float squareLength = (pVec0->x * pVec0->x) + (pVec0->y * pVec0->y);
	return squareLength;
}

// ---------------------------------------------------------------------------

float Vector2DDistance(Vector2D *pVec0, Vector2D *pVec1)
{
	float dist = sqrt(Vector2DSquareDistance(pVec0,pVec1));
	return dist;
}

// ---------------------------------------------------------------------------

float Vector2DSquareDistance(Vector2D *pVec0, Vector2D *pVec1)
{
	float sqDist;
	sqDist = (pVec0->x - pVec1->x)*(pVec0->x - pVec1->x) + (pVec0->y - pVec1->y)*(pVec0->y - pVec1->y);
	return sqDist;
}

// ---------------------------------------------------------------------------

float Vector2DDotProduct(Vector2D *pVec0, Vector2D *pVec1)
{
	float dotProduct = (pVec0->x * pVec1->x) + (pVec0->y * pVec1->y);
	return dotProduct;
}

// ---------------------------------------------------------------------------

void Vector2DFromAngleDeg(Vector2D *pResult, float angle)
{
	const float PI = 3.1415926;
	angle = angle * PI /180;
	Vector2DFromAngleRad(pResult,angle);
}

// ---------------------------------------------------------------------------

void Vector2DFromAngleRad(Vector2D *pResult, float angle)
{
	pResult->x = cos(angle);
	pResult->y = sin(angle);
}

// ---------------------------------------------------------------------------
