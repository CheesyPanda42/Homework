#include <iostream>

int main()
{
	int a,b,c,d;
	a = 7;
	b = 17;
	c = 35;


	d = b|a;
	std::cout << d << std::endl;

	d = c&b;
	std::cout << d << std::endl;

	d = c&b|a;
	std::cout << d << std::endl;
	
	d = d&b|a<<1;
	std::cout << d << std::endl;





	a = -42;
	a >>= 1;
	std::cout << a << std::endl;










}
