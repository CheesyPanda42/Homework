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
	
	m = malloc(num_rows *  sizeof(int*));
	
	/* Allocate memory for matrix rows */
	if (!m)
	{
		printf("Error allocating memory\n");
		exit(0);
	}
	
	/* Allocate memory for matrix columns */
	
	for (i = 0; i < num_rows; i++)
	{
		m[i] = malloc(num_columns * sizeof(int));
		
		if (!m[i])
		{
			printf("Error allocating memory\n");
			exit(0);
		}
	}
	
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
	Matrix m_t = matrix_create(num_columns, num_rows);
	for(i = 0; i < num_columns; i++)
	{
		for(j=0; j < num_rows; j++)
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
		free (m[i]);
	}
	
	free (m);
	
	return;
}

/* Delete row of matrix */
void   matrix_delete_row( Matrix m, int r, int num_rows )
{
	int i;
	int* row_head;
	i = r;
	row_head = m[r];
	while(i < num_rows-1)
	{
		m[i] = m[i+1];
		i++;
	}
		
	free(row_head);
	return;
}

void   matrix_delete_column( Matrix m, int c, int num_rows, int num_columns )
{
	int i, j;
	i = 0;
	
	while (i < num_rows)
	{
		j = c;
		while (j < num_columns-1)
		{	
			m[i][j] = m[i][j+1];
			j++;
		}
		/*m[i][j] = 0;*/
		i++;
	}
	
	return;
}
