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
	int * p_m;
	
	m = malloc(num_rows *  sizeof(int*));
	
	/* Allocate memory for matrix rows */
	printf("m: %p\n", m);
	if (!m)
	{
		printf("Error allocating memory\n");
		exit(0);
	}
	
	/* Allocate memory for matrix columns */
	
	for (i = 0; i < num_rows; i++)
	{
		m[i] = malloc(num_columns * sizeof(int));
		
		//m[i] = p_m + (num_columns * i);
		printf("m[%i]: %p\n", i, m[i] );
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
	Matrix m_t = matrix_create(num_rows, num_columns);
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
		free (m[i]);
	}
	
	free (m);
	
	return;
}

/* Delete row of matrix */
void   matrix_delete_row( Matrix m, int r, int num_rows )
{
	int i;
	// set m[r] = m[r+1], do so for rest of array, delete last row pointer.
	// Maybe
	i = r;
	while(i < num_rows-1)
	{
		m[i] = m[i+1];
		i++;
	}
	free(m[num_rows ]);
	return;
}

void   matrix_delete_column( Matrix m, int c, int num_rows, int num_columns )
{
	int i, j;
	int* row_head;
	// copy the first element in each row to the 2nd, on to the column to be deleted
	// copy the address of the first element into a seperate variable
	// move the pointer to m[i] to m[i] + 1
	// delete m[i]
	
	
	j = c;
	while (i < num_rows)
	{
		row_head = (m[i]+1);
		printf("row_head: %p; %d;\nm[%i]: %p;  %i\n\n", row_head, *row_head, i, m[i], *m[i]);
		while (j > 0)
		{	
			m[i][j+1] = m[i][j];
			j--;
		}
		free(m[i]);
		m[i] = row_head;
		printf("row_head: %p; %d;\nm[%i]: %p;  %i\n\n", row_head, *row_head, i, m[i], *m[i]);
		//m = &m[0];
		i++;
	}
	
	return;
}
