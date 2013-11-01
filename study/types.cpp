#include <iostream>
#include <typeinfo>
using namespace std;


template <typename T>
void foo(T p0)
{
    cout << typeid(T).name() << endl;
}




int main()
{
    float* i;
    foo(i);
       
}
