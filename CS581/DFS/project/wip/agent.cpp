#include <ctime> //for random seed
#include <cmath> 
#include <iostream>
#include "agent.h"

using namespace std;
Agent::Agent(int random_seed) 
{ 
	emptyNode = InitializeNode (emptyNode);

	prevNode = InitializeNode (prevNode);
	prevNode.id = -1;

	currentNode = InitializeNode (currentNode);

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
		// if the current square is dirty, clean it
		squares_cleaned ++;
		return SUCK;
	}
	if (p.bump)
	{	
		// if agent bumped, previous node will be the same as the 
		// current, and the next action on the stack will take place
		UpdateNode(NOOP);
	}
	else
	{
		// if neither of these is the case, the current node 
		// is updated using the previous node's stats,
		UpdateNode(prevAction);
		//PrintNextAction(prevAction);
		
	}	
	// The next action is decided
	next_action = DFS(p);
	prevAction = next_action;
	// PrintNextAction(prevAction);
	return(next_action);
}

Action Agent::DFS (Percept p)
{
	Action next_action;
	next_action = NOOP;
	
	cout << "Current Node: " << endl;
	PrintNode(currentNode);
	cout << "Previous Node: " << endl;
	PrintNode(prevNode);
	
	// If goal state is reached, turn off
	if (p.home && squares_cleaned > 1) return SHUTOFF;
	// If the current node has not been reached before, set up 
	// the action stack for the node
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
	// if the previous node existed, add it to the unbacktrack list
	if (prevNode.id != -1)
	{
		unbacktracked[currentNode] = prevNode;
		//cout << "PrevNode " << prevNode.id << " put on unbacktracked [" << currentNode.id << "]" << endl;
	}
	
	// if the current node has no more possible actions, shut down
	// the agent should only have no possible actions if it winds up
	// stuck, or back on the home square
	if (untried_actions[currentNode.id].empty())
	{
		cerr << "Untried actions is empty" << endl;
		return SHUTOFF;
	}
	// if there are possible actions, get the next one and try it
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

// Update the current node based off the previous action
void Agent::UpdateNode (Action prevAction)
{
	Node temp;
	temp = currentNode;
	switch (prevAction)
	{
		// If agent moved forward, the x or y position will 
		// changed based off of its heading (relative to the start position)
		case FORWARD:
				if(prevNode.heading == 0) currentNode.y++;
				if(prevNode.heading == 1) currentNode.x++;
				if(prevNode.heading == 2) currentNode.y--;
				if(prevNode.heading == 3) currentNode.x--;
				break;
		// changed the heading if the agent moved left or right
		// Headings (relative to start)
		// 0 = up; 1 = right; 2 = down; 3 = left
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


Node InitializeNode (Node node)
{
	node.id = 0;
	node.x = 0;
	node.y = 0;
	node.heading = 0;
	return node;
}

void PrintNode (Node node)
{	
	cout << "ID: " << node.id << " X: " << node.x << " Y: " << node.y << " Heading: " << node.heading << endl;
}
