/* Start Header -------------------------------------------------------
Copyright (C) 2013 DigiPen Institute of Technology. Reproduction or disclosure of this file or its contents without the prior written consent of DigiPen Institute of Technology is prohibited.
File Name: Matrix2D.c
Purpose: Implementation file for the 2D Matrix library
Language: C++
Project: CS529_coryprelerson_1
Author: Cory Prelerson, cory.prelerson, 60000913
Creation date: 9.16.2013
- End Header -----------------------------------------------------*/
#include "Matrix2D.h"


const float PI = 3.1415926535897932384626433832795;
const int MSIZE = 2;
	
			/* 
	   This function sets the matrix Result to the identity matrix */
void Matrix2DIdentity(Matrix2D * pResult)
{
	int i = 0;
	int j = 0;
	for ( i = 0; i <=MSIZE; i++)
	{
		for ( j = 0; j <=MSIZE; j++)
		{
			pResult->m[i][j] = 0;
		}
	}
	pResult->m[0][0] = 1;
	pResult->m[1][1] = 1;
	pResult->m[2][2] = 1;

}

// ---------------------------------------------------------------------------

/* 
   This functions calculated the transpose matrix of Mtx and saves it in
   Result */
void Matrix2DTranspose(Matrix2D * pResult, Matrix2D * pMtx)
{
	int i = 0;
	int j = 0;
	for (i = 0; i <= MSIZE; i++)
	{
		for(j = 0; j <= MSIZE; j++)
		{
			pResult->m[i][j] = pMtx->m[j][i];
		}
	}
}

// ---------------------------------------------------------------------------

/* 
   This function multiplies Mtx0 with Mtx1 and saves the result in Result
   Result = Mtx0*Mtx1 */
void Matrix2DConcat(Matrix2D * pResult, Matrix2D * pMtx0, Matrix2D * pMtx1)
{
	int i,j,k;
	Matrix2D tmpMtx;	// result stored in temporary matrix

	// Simple Matrix Multiplicaiton
	for (i = 0; i <= MSIZE; i++)
	{
		for (j = 0; j <= MSIZE; j++)
		{
			tmpMtx.m[i][j] = 0;		// temporary matrix set to all 0's
			for (k = 0; k <= MSIZE;k++)
			{
				tmpMtx.m[i][j] = tmpMtx.m[i][j] + (pMtx0->m[i][k] * pMtx1->m[k][j]);
			}
		}
	}

	// Copy temp matrix into Result matrix
	for (i = 0; i <= MSIZE; i++)
	{
		for (j = 0; j <= MSIZE; j++)
		{
			pResult->m[i][j] = tmpMtx.m[i][j];
		}
	}
}

// ---------------------------------------------------------------------------

/* 
   This function creates a translation matrix from x *p y and saves it in
   Result */
void Matrix2DTranslate(Matrix2D * pResult, float x, float y)
{
	pResult->m[0][0] = 1;
	pResult->m[0][1] = 0;
	pResult->m[0][2] = x;
	pResult->m[1][0] = 0;
	pResult->m[1][1] = 1;
	pResult->m[1][2] = y;
	pResult->m[2][0] = 0;
	pResult->m[2][1] = 0;
	pResult->m[2][2] = 1;
}

// ---------------------------------------------------------------------------

/* 
   This function creates a scaling matrix from x *p y and saves it in Result */
void Matrix2DScale(Matrix2D * pResult, float x, float y)
{
	pResult->m[0][0] = x;
	pResult->m[0][1] = 0;
	pResult->m[0][2] = 0;
	pResult->m[1][0] = 0;
	pResult->m[1][1] = y;
	pResult->m[1][2] = 0;
	pResult->m[2][0] = 0;
	pResult->m[2][1] = 0;
	pResult->m[2][2] = 1;
}

// ---------------------------------------------------------------------------

/* 
   This matrix creates a rotation matrix from "Angle" whose value is in
   degree. Save the resultant matrix in Result */
void Matrix2DRotDeg(Matrix2D * pResult, float Angle)
{
	Angle = Angle * (PI/180);
	Matrix2DRotRad(pResult, Angle);
}

// ---------------------------------------------------------------------------

/* 
   This matrix creates a rotation matrix from "Angle" whose value is in
   radian. Save the resultant matrix in Result */
void Matrix2DRotRad(Matrix2D * pResult, float Angle)
{
	pResult->m[0][0] = cos(Angle);
	pResult->m[0][1] = -sin(Angle);
	pResult->m[0][2] = 0;
	pResult->m[1][0] = sin(Angle);
	pResult->m[1][1] = cos(Angle);
	pResult->m[1][2] = 0;
	pResult->m[2][0] = 0;
	pResult->m[2][1] = 0;
	pResult->m[2][2] = 1;
	

}

// ---------------------------------------------------------------------------

/* 
   This function multiplies the matrix Mtx with the vector Vec and saves the
   result in Result Result = Mtx * Vec */
void Matrix2DMultVec(Vector2D * pResult, Matrix2D * pMtx, Vector2D * pVec)
{
	Vector2D tmp;
	tmp.x = pVec->x;
	tmp.y = pVec->y;

	pResult->x = (pMtx->m[0][0] * tmp.x) + (pMtx->m[0][1] * tmp.y) + (pMtx->m[0][2] * 1);
	pResult->y = (pMtx->m[1][0] * tmp.x) + (pMtx->m[1][1] * tmp.y) + (pMtx->m[1][2] * 1);
}

// ---------------------------------------------------------------------------