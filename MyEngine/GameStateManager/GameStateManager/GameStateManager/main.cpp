#include <iostream>
#include "GSM.h"
using namespace std;

void Initialize();


int main ()
{
	bool gameRunning = true;
	int state = 1;

	Initialize();
	initGSM();

	while (state !=0)
	{
		GSM();
	}

	return 0;
}


void Initialize()
{
	cout << "Stuff initialized" << endl;
	return;
}

