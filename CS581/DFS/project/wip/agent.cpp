#include <ctime> //for random seed
#include <cmath> 
#include <iostream>
#include "agent.h"

using namespace std;
Agent::Agent(int random_seed) 
{ 
	InitializeNode (emptyNode);
	InitializeNode (prevNode);
	prevNode.id = -1;
	InitializeNode (currentNode);
	squares_cleaned = 0;
	prevAction = NOOP;
	for (int i = 0; i < MAX_NODES; i++)
	{
		untried_actions[i].push(NOOP);
	}
} 

Action Agent::GetAction(Percept p) 
{
	Action next_action;
	if (p.dirt)
	{
		squares_cleaned ++;
		return SUCK;
	}
	if (p.bump)
	{	
		//next_action = NOOP;
		UpdateNode(NOOP);
	}
	else
	{
		UpdateNode(prevAction);
		PrintNextAction(prevAction);
		
	}	
	next_action = DFS(p);
	prevAction = next_action;
	PrintNextAction(prevAction);
	return(next_action);
}

Action Agent::DFS (Percept p)
{
	Node temp;
	Action next_action;
	next_action = NOOP;
	temp.id = -1;
	cout << "Current Node: " << endl;
	PrintNode(currentNode);
	cout << "Previous Node: " << endl;
	PrintNode(prevNode);
	if (p.home && squares_cleaned > 1) return SHUTOFF;
	if (!untried_actions[currentNode.id].empty() && untried_actions[currentNode.id].top() == NOOP)
	{
		cout << "Pushing actions" << endl;
		untried_actions[currentNode.id].pop();
		cout << "Popped stack" << endl;
		untried_actions[currentNode.id].push(RIGHT);
		cout << "Right pushed" << endl;
		untried_actions[currentNode.id].push(RIGHT);
		cout << "Right pushed" << endl;
		untried_actions[currentNode.id].push(LEFT);
		cout << "Left pushed" << endl;
		untried_actions[currentNode.id].push(FORWARD);
		cout << "Forward pushed" << endl;
	}
	if (prevNode.id != -1)
	{
		unbacktracked[currentNode] = prevNode;
		cout << "PrevNode " << prevNode.id << " put on unbacktracked [" << currentNode.id << "]" << endl;
	}
	
	// ERROR CHECK
	if(!untried_actions[currentNode.id].empty())
	cerr <<"TOP OF UNTRIED: " <<  untried_actions[currentNode.id].top() << endl;
	else 
	cerr << "Top of stack empty" << endl;
	// DONE
	
	if (untried_actions[currentNode.id].empty())
	{
		cerr << "Untried actions is empty" << endl;
		//if (unbacktracked[currentNode].id == temp.id) return SHUTOFF;
		//else temp.x = 1;// action to return to prevNode
		return SHUTOFF;
	}
	
	else
	{
		next_action = untried_actions[currentNode.id].top();
		cout << "Next action decided: ";
		PrintNextAction(next_action);
		untried_actions[currentNode.id].pop();
	}
	prevNode = currentNode;
	PrintNextAction (next_action);
	return next_action;
}



void Agent::PrintNextAction (Action next_action)
{
	//cout << "-----SIZE OF STACK: " << action_stack.size() << "-----NEXT ACTION: ";
  switch (next_action)
  {
	case FORWARD:
		cout << "FORWARD" << endl;
		break;
	case LEFT:
		cout << "LEFT" << endl;
		break;
	case RIGHT:
		cout << "RIGHT" << endl;
		break;
	case SUCK:
		cout << "CLEAN" << endl;
		break;
	case SHUTOFF:
		cout << "SHUTOFF" << endl;
		break;
	default:
		cout << "DEFAULT CASE" << endl;
		break;
  }
  return;
}

void Agent::UpdateNode (Action prevAction)
{
	Node temp;
	temp = currentNode;
	switch (prevAction)
	{
		case FORWARD:
				if(prevNode.heading == 0) currentNode.y++;
				if(prevNode.heading == 1) currentNode.x++;
				if(prevNode.heading == 2) currentNode.y--;
				if(prevNode.heading == 3) currentNode.x--;
				break;
		case LEFT:
				currentNode.heading = (prevNode.heading +3) % 4;
				break;
		case RIGHT:
				currentNode.heading = (prevNode.heading + 1) % 4;
				break;
		default:
				break;
	}
	currentNode.id = abs((100*currentNode.x + 10*currentNode.y + currentNode.heading));

}

/*
Node Agent::UpdateNode (Node node, Action prevAction)
{
	Node temp;
	temp = currentNode;
	switch (prevAction)
	{
		case FORWARD:
				if(prevNode.heading == 0) nodeNode.y++;
				if(prevNode.heading == 1) nodeNode.x++;
				if(prevNode.heading == 2) nodeNode.y--;
				if(prevNode.heading == 3) nodeNode.x--;
				break;
		case LEFT:
				currentNode.heading = (prevNode.heading +3) % 4;
				break;
		case RIGHT:
				currentNode.heading = (prevNode.heading + 1) % 4;
				break;
		default:
				break;
	}
	currentNode.id = abs((100*currentNode.x + 10*currentNode.y + currentNode.heading));
}
*/


void InitializeNode (Node node)
{
	node.id = 0;
	node.x = 0;
	node.y = 0;
	node.heading = 0;
	node.parentNode = NULL;
	return;
}

void PrintNode (Node node)
{	
	cout << "ID: " << node.id << " X: " << node.x << " Y: " << node.y << " Heading: " << node.heading << endl;
}
