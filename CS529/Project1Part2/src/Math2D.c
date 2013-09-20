/* Start Header -------------------------------------------------------
Copyright (C) 2013 DigiPen Institute of Technology. Reproduction or disclosure of this file or its contents without the prior written consent of DigiPen Institute of Technology is prohibited.
File Name: Math2D.c
Purpose: Implementation file for the 2D Static Intersection library
Language: C++
Project: CS529_coryprelerson_1
Author: Cory Prelerson, cory.prelerson, 60000913
Creation date: 9.16.2013
- End Header -----------------------------------------------------*/
#include "Math2D.h"
#include "stdio.h"

/*
This function checks if the point P is colliding with the circle whose
center is "Center" and radius is "Radius"
*/
int StaticPointToStaticCircle(Vector2D *pP, Vector2D *pCenter, float Radius)
{
	int collision;
	if (Vector2DDistance(pP, pCenter) > Radius) collision = 0;
	//else collision = 1;
	return collision;
}


/*
This function checks if the point Pos is colliding with the rectangle
whose center is Rect, width is "Width" and height is Height
*/
int StaticPointToStaticRect(Vector2D *pPos, Vector2D *pRect, float Width, float Height)
{
	int collision = 1;
	
	if ((pPos-> x < pRect->x - Width/2) 
		|| (pPos->x > pRect->x + Width/2) 
		|| (pPos -> y > pRect->y + Height/2)
		|| (pPos -> y < pRect->y - Height/2))
		collision = 0;
	//else collision = 1;
	return collision;
}

/*
This function checks for collision between 2 circles.
Circle0: Center is Center0, radius is "Radius0"
Circle1: Center is Center1, radius is "Radius1"
*/
int StaticCircleToStaticCircle(Vector2D *pCenter0, float Radius0, Vector2D *pCenter1, float Radius1)
{
	int collision = 1;
	if(Vector2DSquareDistance(pCenter0, pCenter1) > ((Radius0 + Radius1) * (Radius0 + Radius1))) collision = 0;
	return collision;
}

/*
This functions checks if 2 rectangles are colliding
Rectangle0: Center is pRect0, width is "Width0" and height is "Height0"
Rectangle1: Center is pRect1, width is "Width1" and height is "Height1"
*/
int StaticRectToStaticRect(Vector2D *pRect0, float Width0, float Height0, Vector2D *pRect1, float Width1, float Height1)
{
	int collision = 1;
	float r1, r2, l1, l2, t1,t2,b1,b2;
	r1 = pRect0->x + Width0/2;		// right-most x value of Rectangle0
	r2 = pRect1->x + Width1/2;		// right-most x value of Rectangle1
	l1 = pRect0->x - Width0/2;		// left-tmost x value of Rectangle0
	l2 = pRect1->x + Width1/2;		// left-most x value of Rectangle0
	t1 = pRect0->y + Height0/2;		// top-most x value of Rectangle0
	t2 = pRect1->y + Height1/2;		// top-most x value of Rectangle0
	b1 = pRect0->y - Height0/2;		// bottom-most x value of Rectangle0
	b2 = pRect1->y + Height1/2;		// bottom-most x value of Rectangle0
	if (r1 < l2 || r2 < l1 || t1 < b2 || t2 < b1) collision = 0;
	printf("C: %i\n", collision);
	return collision;
}
