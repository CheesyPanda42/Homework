#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "matrix.h"

int main(int argc, char** argv)
{
	int rows = atoi(argv[1]);
	int col  = atoi(argv[2]);
	int i,j;

	srand(time(NULL));

	Matrix m1, m2, mres, m_t;	
	m1 = matrix_create(rows, col);
	m2 = matrix_create(rows, col);
	mres = matrix_create(rows,col);
	m_t = matrix_create(rows,col);
	
	if (m1) printf("Matrix m1 created at %p\n", m1);
	if (m2) printf("Matrix m2 created at %p\n", m2);
	
	printf("Filling %i x %i matrix\n", rows, col);
	for(i = 0; i < rows; i++)
	{
		printf("%i: i\n",i);
		for (j = 0; j < col; j++)
		{
			printf("j: %i\n", j);
			m1[i][j] = (rand() + (i*j+1)) % 100;
			m2[i][j] = (rand() + (i*j+1)) % 100;
		}
	}
	
	printf("Printing Matrix\n");
	matrix_print(m1, rows, col);
	printf("\n");
	matrix_print(m2, rows, col);
	printf("\n");
	
	mres = matrix_add(m1,m2,mres,rows,col);
	matrix_print(mres,rows,col);
	printf("\n");
	m_t = matrix_transpose(m1, rows, col);
	matrix_print (m_t, rows, col);
	
	printf("Freeing matrix\n");
	/*matrix_delete(m,rows);*/
	
	return 0;
}