#include "matrix.h"
#include <stdlib.h>
#include <stdio.h>

void  matrix_print( Matrix m, int num_rows, int num_columns ) {
    int i, j;
    for ( i=0; i<num_rows; ++i ) {
        for ( j=0; j<num_columns; ++j ) {
            printf( "%4i ", m[i][j] );
        }
        printf( "\n" );
    }
}

/* Create a matrix on the given size */
Matrix matrix_create( int num_rows, int num_columns )
{
	Matrix m;
	int i;
	
	/* Allocate memory for matrix rows */
	m = (int**) malloc(sizeof * m *num_rows);
	
	if (!m)
	{
		printf("Error allocating memory\n");
		exit(0);
	}
	
	/* Allocate memory for matrix columns */
	
	for (i = 0; i < num_rows; i++)
	{
		m[i] = (int*) malloc(sizeof * m[i] *num_columns);
		if (!m[i])
		{
			printf("Error allocating memory\n");
			exit(0);
		}
	}
	
	printf("%i x %i matrix created at %p\n", num_rows, num_columns, m);
	
	return m;
}

/* Add 2 matricies */
Matrix matrix_add( Matrix m1, Matrix m2, Matrix result, int num_rows, int num_columns )
{
	int i,j;
	for (i = 0; i < num_rows; i++)
	{
		for (j = 0; j < num_columns; j++)
		{
			result[i][j] = m1[i][j] + m2[i][j];
		}
	}
	
	return result;
}


/* Matrix transposition */
Matrix matrix_transpose(Matrix m, int num_rows, int num_columns)
{
	int i,j;
	Matrix m_t;
	for(i = 0; i < num_rows; i++)
	{
		for(j=0; j < num_columns; j++)
		{
			m_t[i][j] = m[j][i];
		}
	}
	return m_t;
}


/* Delete matrix */
void matrix_delete (Matrix m, int num_rows)
{
	int i;
	for (i = 0; i < num_rows; i++)
	{
		printf("Freeing row\n");
		free (m[i]);
	}
	
	printf("Freeing matrix\n");
	free (m);
	
	return;
}

/* Delete row of matrix */
void   matrix_delete_row( Matrix m, int r, int num_rows )
{
	int i;
	for (i = 0; i < r; i++)
	{
		
	}
}