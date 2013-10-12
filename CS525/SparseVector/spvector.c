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
	ElementNode * head_node;
	ElementNode * pNode;
	ElementNode * curr;
	ElementNode * prev;
	int curr_pos;
	bool found;

	found = false;
	curr_pos = 0;
	//printf("p_e: %p,    %p\n Data:   %d    pos:     %d\n", p_e, *p_e, val, pos);
	if(!*p_e)
	{
		//printf("NULL POINTER\n");
		head_node = (ElementNode*) malloc(sizeof(ElementNode));
		*p_e = head_node;
		head_node -> data = val;
		head_node -> pos = pos;
		head_node -> next = 0;
	}
	else
	{
		head_node = *p_e;

		pNode = (ElementNode*) malloc(sizeof(ElementNode));
		pNode -> data = val;
		pNode -> pos  = pos;
		pNode -> next = 0;
		printf("\npNode: %p    data: %d      pos: %d\n", pNode, pNode->data, pNode -> pos);

		curr = head_node;

		while(!found)
		{
			// Insert new head
			if ( pos < head_node-> pos)
			{
				printf("\nNew head\n");
				*p_e = pNode;
				pNode->next = curr;
		 		printf("\nInsert at head: data:  %d    pos:  %d     node:    %p      next:    %p\n",pNode->data, pNode->pos, pNode, pNode->next);
				printf("\nHead points to: %p    data: %d    pos %d\n", pNode->next, pNode->next->data, pNode->next->pos);
				found = true;
				break;
			}


			prev = curr;
			printf("\ncurr->data: %d        curr->pos:     %d\n", curr->data,  curr->pos);
			curr = curr->next;


			// inserting at end of list
			if(!curr)
			{
				printf("\nInsert at end: data:  %d    pos:  %d     node:    %p      next:    %p\n",pNode->data, pNode->pos, pNode, pNode->next);
				curr = pNode;
				found = true;
			}
			// inserting in middle of list
			else if(curr -> pos > pos)
			{
				printf("\ninsert middle - pos: %d    curr->pos: %d\n", pos, curr->pos);
				pNode->next = curr;
				prev->next = pNode;
				found = true;
			}

		}
		printf("outside loop\n");
		//curr->next  = pNode;
	}
    return 0;
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
