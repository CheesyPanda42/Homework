/* Start Header -------------------------------------------------------
Copyright (C) 2013 DigiPen Institute of Technology. Reproduction or disclosure of this file or its contents without the prior written consent of DigiPen Institute of Technology is prohibited.
File Name: README
Purpose: Read me file for Project 1
Project: CS529_coryprelerson_1
Author: Cory Prelerson, cory.prelerson, 60000913
Creation date: 9.16.2013
- End Header -----------------------------------------------------*/

This program implements and tests libraries for 2D vector, matrix, and static collision math and testing. 

Matrix:
The matrix library provides functions for:
- setting the identity matrix
- transposing a matrix
- multiplying 2 3x3 matrices
- creating a translation matrix
- creating a rotation matrix from an angle in degrees
- creating a rotation matrix from an angle in radians
- creating a scaling matrix
- multiplying a 3x3 matrix by a vector


Vector: 
The vector library provides functions for:
- zeroing a vector
- setting the coordinates (x,y) of a vector
- negating a vector
- adding 2 vectors
- subtracting 2 vectors
- calculating a unit vector
- scaling a vector
	- scaling and adding a vector
	- scaling and subtracting a vector
- calculating the length and square length of a vector
- calculating the distance and square distance between two points
- calculating the dot product of 2 vectors
- calculating a unit vector at a given angle in radians and degrees

Math:
The math library calculates static intersections between:
- a point and a circle
- a point and a rectangle
- a circle and a circle
- a rectangle and a rectangle