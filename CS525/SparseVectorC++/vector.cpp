#include "vector.h"
namespace CS225
{


/* this is the only place where dimension is used */
std::ostream& operator<<(std::ostream &out, const SparseVector &v) {
	int i,last_pos=-1;
	ElementNode* p_e=v.pHead;
	while (p_e) {
		for (i=last_pos+1;i<p_e->pos;++i) out << " " << "0";
		out << " " << p_e->data;
		last_pos=p_e->pos;
		p_e = p_e->next;
	}
	for (i=last_pos+1;i<v.dimension;++i) out << " " << "0";

	return out;
}


ElementProxy SparseVector::operator[](long pos)
{
    ElementProxy value(*this,pos);

    return value;
}



void SparseVector::Insert(int val, long pos)
{
    bool found;
    ElementNode* curr;
    ElementNode* prev;
    ElementNode* pNode;

    if(pHead->data == 0)
    {
        if(val != 0)
        {
            pHead->data = val;
            pHead->pos = pos;
            pHead->next = 0;
        }
    }
    else
    {
        pNode = new ElementNode;
        pNode ->data = val;
        pNode ->pos = pos;
        pNode ->next = 0;

        curr = pHead;
        if (val == 0)
        {
            std::cout << "Delete this node\n";
            found = true;
        }
        while (!found)
        {
            if (pos < pHead->pos)
            {
                pHead = pNode;
                pNode->next = curr;
                found = true;
                break;
            }
            prev = curr;
            curr = curr->next;

            // inserting at end of list
            if (!curr)
            {
                curr = pNode;
                prev->next = pNode;
                found = true;
            }
            else if(curr->pos > pos)
            {
                pNode->next = curr;
                prev->next = pNode;
                found = true;
            }

        }
    }
    return;
}

void SparseVector::Delete(long pos)
{
    ElementNode *curr;
    ElementNode *prev;
    ElementNode *pNode;
    bool found;
    found = false;

    curr = pHead;
    prev = pHead;

    while (curr && !found)
    {
        if (curr->pos == pos)
        {
            // deleting head
            if(curr == pHead)
            {
                prev = pHead;
                pNode = curr->next;
                pHead = pNode;
                delete prev;
            }
            else
            {
                pNode = curr->next;
                prev->next = pNode;
                delete curr;
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

//SparseVector::InsertTail(int val)


SparseVector SparseVector::operator+(const SparseVector& op2)
{
    const ElementNode* curr1;
    const ElementNode* curr2;
    SparseVector result;

    curr1 = this->pHead;
    curr2 = op2.pHead;


    while(curr1 && curr2)
    {

        if(curr1->pos == curr2->pos)
        {
            // Use tail insert
            result.Insert(curr1->data + curr2->data, curr1->pos);
            curr1 = curr1->next;
            curr2 = curr2->next;
        }
        else if(curr1->pos < curr2-> pos)
        {
            result.Insert(curr1->data, curr1->pos);
            curr1 = curr1->next;
        }
        else if(curr2->pos < curr1->pos)
        {
            result.Insert(curr2->data, curr2->pos);
            curr2 = curr2-> next;
        }
    }
    // finish up if one list is longer
    if(curr1 && curr1->data != 0)
    {
        while(curr1)
        {
            result.Insert(curr1->data, curr1->pos);
            curr1 = curr1->next;
        }
    }
    if(curr2 && curr2->data)
    {
        while(curr2)
        {
            result.Insert(curr2->data, curr2->pos);
            curr2 = curr2->next;
        }
    }

    return result;
}


ElementProxy::operatorint()
{

}


}
