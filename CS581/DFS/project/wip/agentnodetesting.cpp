#include <ctime> //for random seed
#include <cmath> 
#include <iostream>
#include "agent.h"

using namespace std;
Agent::Agent(int random_seed) 
{ 
	InitializeNode (prevNode);
	InitializeNode (currentNode);
} 

Action Agent::GetAction(Percept p) 
{
	srand(time(NULL));
	int i = rand() % 4;
	Action next_action;
	cout <<endl<< i << endl;
	if (p.dirt) return SUCK;
	if (p.bump)
	{	
		UpdateNode(RIGHT);
		PrintNextAction(RIGHT);
		cout << "Prev node: ";
		PrintNode(prevNode);
		cout << "Current node: ";
		PrintNode(currentNode);
		prevNode = currentNode;
		return RIGHT;
	}
	cout << "Prev node: ";
	PrintNode(prevNode);
	cout << "Current node: ";
	PrintNode(currentNode);
	
	switch (i)
	{
		case 0:
			next_action = FORWARD;
			break;
		case 1:
			next_action = LEFT;
			break;
		case 2:
			next_action = RIGHT;
			break;
		case 3:
			next_action = SHUTOFF;
			break;
		default:
			break;
	}
	PrintNextAction(next_action);
	UpdateNode(next_action);
	cout << "Prev node: ";
	PrintNode(prevNode);
	cout << "Current node: ";
	PrintNode(currentNode);
	
	prevNode = currentNode;
	//return (next_action);
	return(next_action);
}

void Agent::PrintNextAction (Action next_action)
{
	//cout << "-----SIZE OF STACK: " << action_stack.size() << "-----NEXT ACTION: ";
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
	case SUCK:
		cout << "CLEAN --------------------" << endl;
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

void Agent::UpdateNode (Action prevAction)
{
	switch (prevAction)
	{
		case FORWARD:
				if(prevNode.heading == 0) currentNode.y++;
				if(prevNode.heading == 1) currentNode.x++;
				if(prevNode.heading == 2) currentNode.y--;
				if(prevNode.heading == 3) currentNode.x--;
				break;
		case LEFT:
				currentNode.heading = (prevNode.heading - 1) % 4;
				break;
		case RIGHT:
				currentNode.heading = (prevNode.heading + 1) % 4;
				break;
		default:
				break;
	}
	currentNode.id = abs((100*currentNode.x + 10*currentNode.y + currentNode.heading));
}

void InitializeNode (Node node)
{
	node.id = 0;
	node.x = 0;
	node.y = 0;
	node.heading = 0;
	return;
}

void PrintNode (Node node)
{	
	cout << "ID: " << node.id << " X: " << node.x << " Y: " << node.y << " Heading: " << node.heading << endl;
}
