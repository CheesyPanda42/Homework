#include <stdlib.h>
#include <stdio.h>
#include "spvector.h"
#define true 1
#define false 0

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
	int found;

	found = false;

    if(!*p_e)
	{
		if (val != 0)
        {
            head_node = (ElementNode*) malloc(sizeof(ElementNode));
            *p_e = head_node;
            head_node -> data = val;
            head_node -> pos = pos;
            head_node -> next = 0;
        }
	}
	else
	{
		head_node = *p_e;

		pNode = (ElementNode*) malloc(sizeof(ElementNode));
		pNode -> data = val;
		pNode -> pos  = pos;
		pNode -> next = 0;

		curr = head_node;
        if(val == 0)
        {
            delete_element(p_e, pos);
            found = true;
        }
		while(!found)
		{
			/* Insert new head */
			if ( pos < head_node-> pos)
			{
				*p_e = pNode;
				pNode->next = curr;
				found = true;
				break;
			}

			prev = curr;
			curr = curr->next;

			/* inserting at end of list */
			if(!curr)
			{
				curr = pNode;
				prev->next = pNode;
				found = true;
			}
			/* inserting in middle of list */
			else if(curr -> pos > pos)
			{
				pNode->next = curr;
				prev->next = pNode;
				found = true;
			}
		}
	}
    return 0;
}


/*
 * delete an element at position pos if it exists */
void delete_element( ElementNode_handle *p_e,int pos )
{
    ElementNode * curr;
    ElementNode * prev;
    ElementNode * pNode;
    int found;
    found = false;

    curr = *p_e;
    prev = *p_e;




    /* traverse list until the last node or until the position in question is found */
    while(curr && !found)
    {
        if (curr->pos == pos)
        {
            /* if deleting head */
            if(curr == *p_e)
            {
                prev = *p_e;
                pNode = curr->next;
                *p_e = pNode;
                free(prev);
            }
            else
            {
                pNode = curr->next;
                free(curr);
                prev->next = pNode;
            }
            found = true;
        }
        else
        {
            prev = curr;
            curr = curr->next;
        }
    }
}



/*
 * adds 2 lists as vectors, returns a new list */
ElementNode_handle add( ConstElementNode_handle p_e1,ConstElementNode_handle p_e2 )
{
    ConstElementNode_handle curr1;
    ConstElementNode_handle curr2;
    ElementNode * result;

    curr1 = p_e1;
    curr2 = p_e2;
    result = 0;
    /*result = (ElementNode*) malloc(sizeof(ElementNode));*/


    while(curr1->next && curr2->next)
    {
        if(curr1->pos == curr2->pos)
        {
            insert_element(&result,curr1->pos,curr1->data + curr2->data);
            curr1 = curr1->next;
            curr2 = curr2->next;
        }
        else if(curr1->pos < curr2->pos)
        {
            insert_element(&result,curr1->pos,curr1->data);
            curr1 = curr1->next;

        }
        else if(curr1->pos > curr2->pos)
        {
            insert_element(&result,curr2->pos,curr2->data);
            curr2 = curr2 ->next;
        }
    }

    /* finish up if one list is longer */
    if (curr1-> data != 0)
    {
        while(curr1)
        {
            insert_element(&result,curr1->pos,curr1->data );
            curr1 = curr1->next;
        }
    }
    else if(curr2->data !=0)
    {
        while(curr2)
        {
            insert_element(&result,curr2->pos,curr2->data );
            curr2 = curr2->next;
        }

    }
    return result;
}





/*
 * get the value at the given position,
 * p_e is the head pointer
 */
int  get( ConstElementNode_handle p_e, int pos )
{
	ConstElementNode_handle temp;
	int i;
	int data;
	i=0;
	data = 0;
	temp = p_e;
	while (i < pos + 1 && temp)
	{
		if(temp->pos == pos)
        {
            data = temp->data;
            break;
        }
        temp = temp->next;
		i++;
	}
	return data;
}



/*
 * scalar product of 2 lists.
 * */
int scalar_product( ConstElementNode_handle p_e1, ConstElementNode_handle p_e2 )
{
    ConstElementNode_handle curr1;
    ConstElementNode_handle curr2;
    int result;
    int list1;
    int list2;
    curr1 = p_e1;
    curr2 = p_e2;


    list1 = list2 = 0;
    result = 0;
    while (curr1 && curr2)
    {
        if(curr1->pos == curr2->pos)
        {
            list1 = curr1->data;
            list2 = curr2->data;
            curr1 = curr1-> next;
            curr2 = curr2-> next;
        }
        else if(curr1->pos < curr2->pos)
        {
            list1 = curr1 -> data;
            list2 = 0;
            curr1 = curr1 -> next;
        }
        else if(curr1->pos > curr2->pos)
        {
            list2 = curr2 -> data;
            list1 = 0;
            curr2 = curr2 -> next;
        }
        result += (list1 * list2);

    }


    return result;
}





/*
 * deallocate a list */
void free_elements( ElementNode_handle p_e )
{
    ElementNode * old;
    while(p_e)
    {
        old = p_e;
        p_e = p_e->next;
        free(old);
    }
}



/*
 * The row/matrix code is not implemented. The following code is dummy code to eliminate warnings */
int insert_row( RowNode_handle *p_r,int pos,ElementNode_handle p_e )
{
    *p_r = *p_r;
    pos = pos;
    p_e = p_e;
    return 0;
}

int insert_element2( RowNode_handle *pp_r,int pos_row,int pos_col,int val )
{
    *pp_r = *pp_r;
    pos_row = pos_row;
    pos_col = pos_col;
    val = val;
    return 0;
}


RowNode_handle find_row( RowNode_handle *pp_r,int pos )
{
    *pp_r = *pp_r;
    pos = pos;
    return *pp_r;
}

RowNode_handle transpose( ConstRowNode_handle p_r )
{
    ConstRowNode_handle p_a = p_r;
    int i;
    i = p_a->pos;
    i++;
    return 0;
}


RowNode_handle mult( ConstRowNode_handle p_r1, ConstRowNode_handle p_r2 )
{
    ConstRowNode_handle p_a = p_r1;
    ConstRowNode_handle p_b = p_r2;
    int i,j,k;
    i = p_a->pos;
    j = p_b->pos;
    k = i+j;
    k++;
    return 0;
}


void free_rows( RowNode_handle p_r )
{
    p_r = p_r;
}


int  determinant(ConstRowNode_handle p_r,int dim) {
	/* this is a meaningless code to get rid of "unsed argument" warnings in
	 * Borland an MS */
	int result=1;
	if (dim==0) ++result;
	if (p_r)    --result;
	return result;
}
