#include "spvector.h"
#include <stdio.h>

int main()
{
  	ElementNode_handle pE1=0;
  	int i;
  	printf("\n====== INSERTS BACK =============================\n");
  	insert_element(&pE1,10,1);
  	insert_element(&pE1,11,1);
  	insert_element(&pE1,2,2);
  	insert_element(&pE1,4,4);
  	insert_element(&pE1,3,3);
  	insert_element(&pE1,5,5);
  	insert_element(&pE1,7,7);
  	
  	printf("vector 1 formatted: %p \n", &pE1);
	printf_elements(pE1,"%4d",80); printf("\n");
	printf("vector 1 raw:\n");
 	print_elements(pE1); printf("\n");

	i = get(pE1, 4);
	
	printf ("%i",i);	
	
	return 0;
}