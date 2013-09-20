#include <ctime> //for random seed
#include <cmath> 
#include <iostream>
#include "agent.h"

using namespace std;
Agent::Agent(int random_seed) { 

	s.hasMoved = 0;
	s.prevAction = NOOP;
	squares_cleaned=0;
  //supplying your own seed may help debugging, same seed will cause 
  //same random number sequence 
  if (random_seed==0) srand( static_cast<unsigned>(std::time(0))); // random seed from time
  else                srand( random_seed ); // random seed from user
} 

Action Agent::GetAction(Percept p) {
	
	Action next_action;

	action_stack.push(SHUTOFF);
	action_stack.push(LEFT);
	action_stack.push(RIGHT);
	action_stack.push(FORWARD);

	s.prevPer = p;
  //straight-forward behaviour
  if (p.dirt) 
  {
	squares_cleaned++;
	return SUCK;
  }
  if (p.home)
	{
		if (s.hasMoved == 0 || s.prevAction == NOOP) 
		{
			s.hasMoved = 1;
			s.prevAction = FORWARD;
			printNextAction(FORWARD);
			return (FORWARD);
		}
		if (s.prevPer.bump) s.currPer.bump;
		else return(SHUTOFF);
	}
  if (p.bump) 
  {
	action_stack.pop();
	next_action = action_stack.top();
	printNextAction (next_action);
	s.prevAction = NOOP;
	s.prevPer = p;
	action_stack.pop();
	return (next_action);
  }

  
  //default rule
  
  next_action = action_stack.top();
  
  s.prevPer = p;
  s.prevAction = next_action;
  printNextAction (next_action);
  action_stack.pop();
  

	
  
  return (next_action);
}

void Agent::printNextAction (Action next_action)
{
	cout << "-----SIZE OF STACK: " << action_stack.size() << "-----NEXT ACTION: ";
  switch (next_action)
  {
	case FORWARD:
		cout << "FORWARD ------------------" << endl;
		break;
	case LEFT:
		cout << "LEFT ------------------" << endl;
		break;
	case RIGHT:
		cout << "RIGHT ------------------" << endl;
		break;
	case SHUTOFF:
		cout << "SHUTOFF ------------------" << endl;
		break;
	default:
		cout << "DEFAULT CASE" << endl;
		break;
  }
  return;
}

