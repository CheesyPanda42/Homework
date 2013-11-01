#include <iostream>

using namespace std;


int main()
{
    int a,b,c,d;
    a = 18;
    b = 1;
    c = 1 << b;
    a &= ~c;
    cout << a << endl;


    return 0;
}
