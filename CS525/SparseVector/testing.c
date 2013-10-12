#include "spvector.h"
#include <stdio.h>

int main()
{
  	ElementNode_handle pE1=0;
  	int i;
  	printf("\n====== INSERTS BACK =============================\n");
  	insert_element(&pE1,10,10);
  	insert_element(&pE1,11,11);
  	insert_element(&pE1,9,9);
  	insert_element(&pE1,4,4);
  	insert_element(&pE1,3,3);
  	insert_element(&pE1,5,5);
  	insert_element(&pE1,2,2);
  	insert_element(&pE1,7,7);
  	insert_element(&pE1,4,1);
	insert_element(&pE1,12,12);
	insert_element(&pE1,17,17);
	insert_element(&pE1,13,13);
	insert_element(&pE1,20,20);
	insert_element(&pE1,30,30);
	insert_element(&pE1,25,25);
  	printf("vector 1 formatted: %p \n", &pE1);
	printf_elements(pE1,"%4d",80); printf("\n");
	printf("vector 1 raw:\n");
 	print_elements(pE1); printf("\n");

	i = get(pE1, 4);
	
	printf ("%i",i);	
	
	return 0;
}