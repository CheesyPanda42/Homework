#include <stdlib.h>
#include <stdio.h>
#include "spvector.h"

/* definition of Node is hidden from the client (driver) so that client CANNOT
 * use INTERNALS of the Node.  this is poor-man encapsulations - hiding
 * implementation details.  The main reason for encapsulations -- if I ever
 * decide to change Node struct no client code will break. Example: I decide to
 * change "left" to "Left", all I do is change map.h and map.c, no client code
 * is effected, since clients were FORCED to use getleft(...) function, rather
 * than field name "left".  Also see typedef in map.h
 */
struct ElementNode {
  int    data;
  int    pos;
  struct ElementNode* next;
};

struct RowNode {
  int pos;
  struct ElementNode* elements;
  struct RowNode* next;
};

typedef struct ElementNode ElementNode;
typedef struct RowNode RowNode;


/*print functions*/
void printf_elements(ConstElementNode_handle p_e, char *fmt, int dim) {
	int i,last_pos=-1;
	while (p_e) {
		for (i=last_pos+1;i<p_e->pos;++i) { printf(fmt,0); }
		printf(fmt,p_e->data);
		last_pos=p_e->pos;
		p_e = p_e->next;
	}
	for (i=last_pos+1;i<dim;++i) { printf(fmt,0); }
}

void print_elements(ConstElementNode_handle p_e) {
	while (p_e) {
		printf("%i at pos %i, ",p_e->data,p_e->pos);
		p_e = p_e->next;
	}
}

void print_rows(ConstRowNode_handle p_r) {
	while (p_r) {
		printf("row %i: ", p_r->pos);
		print_elements(p_r->elements);
		printf("\n");
		p_r = p_r->next;
	}
}

void printf_rows(ConstRowNode_handle p_r, char *fmt, int dim) {
	int i,j,last_pos=-1;
	while (p_r) {
		for (i=last_pos+1;i<p_r->pos;++i) { /* output row of 0's */
			for (j=0;j<dim;++j) printf(fmt,0); 
			printf("\n");
		}
		printf_elements(p_r->elements,fmt,dim);
		printf("\n");
		last_pos=p_r->pos;
		p_r = p_r->next;
	}
	for (i=last_pos+1;i<dim;++i) { /* output row of 0's */
		for (j=0;j<dim;++j) printf(fmt,0); 
		printf("\n");
	}
}



/* insert an element into a list 
 * list is ordered using pos
 * if position pos is already occupied, the value of the node
 * should be updated with val
 * if val=0, then the element should be deleted
 * return 0 if operation is succesfull 
 *        1 if malloc failed */
int insert_element(ElementNode_handle * p_e,int pos,int val)
{
	ElementNode_handle head;
	ElementNode_handle temp;
	ElementNode_handle insert;



	
	insert =(ElementNode_handle) malloc(sizeof(ElementNode));
	head = *p_e;
	
	
	
	if(*p_e == 0)
	{
		printf("Adding first element!\n");
		*p_e = insert;
	}
	else if(head-> pos > pos)
	{
		printf("need to reorganize\n");
	}
	
	
	
	temp = *p_e;
	
	//printf ("%p, %p\n", p_e, *p_e);
	printf("Pos: %i, Val %i\n", pos, val);
	printf("Temp: %p, Insert:%p\n", temp, insert);
	
	insert -> pos = pos;
	insert -> data = val;
	insert -> next = 0;
	
	//*p_e = insert;
	
}



/*
 * get the value at the given position,
 * p_e is the head pointer 
 */
int  get( ConstElementNode_handle p_e, int pos )
{
	ElementNode_handle temp;
	int i;
	while (i < pos + 1 && p_e)
	{
		temp = p_e->next;
		i++;
	}
	return temp->data;
}




int  determinant(ConstRowNode_handle p_r,int dim) {
	/* this is a meaningless code to get rid of "unsed argument" warnings in 
	 * Borland an MS */
	int result=1;
	if (dim==0) ++result;
	if (p_r)    --result;
	return result;
}
