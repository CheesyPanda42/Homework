#include "GSM.h"

using namespace std;


void initGSM()
{
	cout << "GSM initialized" << endl;
	current = 1;
	next = 1;
	prev = 1;
	return;
}


int GSM()
{
	int state = 1;

	if (current == QUIT) state = 0;

	return (state);

}