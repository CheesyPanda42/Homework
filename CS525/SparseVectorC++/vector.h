#include <iostream>

namespace CS225
{
    struct ElementNode
    {
        int data;
        int pos;
        ElementNode* next;
    };

    class ElementProxy;

    class SparseVector
    {
        public:
            SparseVector()
            {
                pHead = new ElementNode;
                pHead->data = 0;
                pHead->pos = -1;
                pHead->next = 0;
                pTail = pHead;
                dimension = 0;
            }
            int Get(long pos) const;
            void Insert (int val, long pos);
            void InsertTail(int val);
            void Delete (long pos);
            void PrintRaw();

            // Operators
            SparseVector operator+(const SparseVector& op2);
            ElementProxy operator[](long pos);
            int operator[](long pos)const;
            friend std::ostream& operator<<(std::ostream &out, const SparseVector &v);


        private:
            ElementNode* pHead;
            ElementNode* pTail;
            long dimension;

    };


    class ElementProxy
    {
    public:
        ElementProxy(SparseVector& v, long pos): vect(v), position(pos) {}
        operator int() const;
        ElementProxy operator=(ElementProxy&);
        ElementProxy operator=(int);

    private:
        SparseVector &vect;
        long position;
    };

}
