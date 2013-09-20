#ifndef AGENT_H
#define AGENT_H
#include "definitions.h"
#include <stack>
#include <map>

#define MAX_NODES 10000
struct Node
{
	int id;
	int x;
	int y;
	int heading;
	Node * parentNode;
};

struct nodecomp {
  bool operator() (const Node& lhs, const Node& rhs) const
  {return lhs.id<rhs.id;}
};


void InitializeNode(Node);
void PrintNode(Node);
struct State
{
	Node prevNode;
	Action prevAction;
};

class Agent 
{
	std::stack<Action> untried_actions[MAX_NODES];	// addressed by Node id, stack of actions node yet done at a node
	std::map<State, Node> result;				// given a node and an action (state), the resulting node
	int steps_taken;
	int squares_cleaned;
	Node emptyNode;
	Node currentNode;							// current position and heading
	Node prevNode;								// previous position and heading
	Action prevAction;							// last thing the agent did
	int position[40][40];						// Do I actually need this?
	std::map<Node, Node, nodecomp > unbacktracked;// for each node, stack of nodes that have not been backtracked to yet
													// for this implementation, may be equivalent to previous node. 
	
	
  public:
    Agent(int random_seed);
    Action GetAction(Percept p);
	Action DFS(Percept p);
	void   UpdateNode (Action prevAction);
	void   PrintNextAction (Action next_action);
	
};



#endif

