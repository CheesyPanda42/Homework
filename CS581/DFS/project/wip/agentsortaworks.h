#ifndef AGENT_H
#define AGENT_H
#include "definitions.h"
#include <stack>


struct State
{
	Percept prevPer;
	Percept currPer;
	Action  prevAction;
	bool hasMoved;
	std::stack<Action> untried;
};

class Agent {
	std::stack<Action> action_stack;
	int squares_cleaned;
	//Percept prevPer;
	//Action prevAct;
	State s;
	
  public:
    Agent(int random_seed);
    Action GetAction(Percept p);
	void printNextAction (Action next_action);
	
	
};



#endif

