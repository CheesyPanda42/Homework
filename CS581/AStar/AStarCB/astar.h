#ifndef ASTAR
#define ASTAR

#include <list>
#include <queue>
#include <vector>
#include <stdio.h>
#include <map>
#include <functional>

//callback object for Astar
template <typename GraphType, typename AstarType>
class Callback {
    protected:
        GraphType const& g;
    public:
        Callback( GraphType const& _g) : g(_g) {}
        virtual ~Callback() {}
        virtual void OnIteration( AstarType const& ) { }
        virtual void OnFinish( AstarType const& )    { }
};

template<typename GraphType>
class Node
{
    typedef typename GraphType::Vertex Vertex;
    typedef typename GraphType::Edge   Egde;

    public:
        void SetNode(Vertex v, Vertex pV, int h, int cost)
        {
            vertex = v;
            prevVertex = pV;
            heruistic = h;
            costCurrent = cost;

        }
        void CreateNodeFromEdge(Vertex v, Vertex pV,int h, int weight)
        {
            vertex = v;
            prevVertex = pV;
            heruistic = h;
            costCurrent = weight;
        }

        Vertex GetVertex ()
        {
            return vertex;
        }
        Vertex GetPrevVertex()
        {
            return prevVertex;
        }
        int GetHerustic()
        {
            return heruistic;
        }
        int GetCost()
        {
            return costCurrent;
        }
        int GetFValue() const
        {
            return costCurrent+heruistic;
        }
        void PrintNode()
        {
            printf("Ver: %d  PVer: %d    h: %d    g: %d\n", vertex.ID(), prevVertex.ID(),heruistic, costCurrent);
        }
        bool operator<(const Node rhs) const
        {
            // < operator tests compares fitness of node
            // a node with a HIGHER FValue therefore is LESS fit than one with a lower f-value
            // Ex: f(Node1) = 11; f(Node2) = 13
            // Node2 costs more than Node1, so operator returns that Node2 < Node1
            // primarily done to get proper behavior from priority queue
            float fRhs, fLhs;
            fLhs = 1/this->GetFValue();
            fRhs = 1/rhs.GetFValue();
            return fLhs < fRhs;
        }
        bool NodeEquals(Node& rhs, Node& lhs)
        {
            int rhsVID = rhs.GetVertex().ID();
            int lhsVID = lhs.GetVertex().ID();
            //int rhsPVID = rhs.GetPrevVertex().ID();
            //int lhsPVID = lhs.GetPrevVertex().ID();
            bool equal;

            if (rhsVID == lhsVID)// && rhsPVID == lhsPVID)
                equal = true;
            else
                equal = false;

            return equal;
        }

	private:
	    Vertex vertex;
        Vertex prevVertex;
        int heruistic;
        int costCurrent;
};

template <typename GraphType, typename Heuristic>
class Astar
{

typedef std::priority_queue<Node<GraphType> > OpenListContainer;
typedef std::vector<Node<GraphType> > ClosedListContainer;
typedef std::vector<typename GraphType::Edge> SolutionContainer;
typedef std::size_t size_t;

    public:
        ////////////////////////////////////////////////////////////

		Astar( GraphType const& _graph, Callback<GraphType,Astar> & cb ) :
            graph(_graph),
            openlist(),
            closedlist(),
            solution(),
            callback(cb),
            start_id(0),
            goal_id(0)
        {}

        ////////////////////////////////////////////////////////////
        std::vector<typename GraphType::Edge> search(size_t s, size_t g) {
            start_id = s;
            goal_id  = g;
            int i;
            std::vector<typename GraphType::Edge> edges;
            //openlist.clear();
            while(!openlist.empty())
            {
                openlist.pop();
            }
            closedlist.clear();
            solution.clear();
            Heuristic heuristic;
            Node<GraphType> topNode;
            Node<GraphType> currNode;

            //heuristic rom fstart to goal
            typename Heuristic::ReturnType h = heuristic( graph,graph.GetVertex(start_id),graph.GetVertex(goal_id) );
			printf ("start_id: %d,  goal_id: %d,  heuristic %d\n", s, g, h);

            topNode.SetNode(graph.GetVertex(start_id),-1,1,0);
            edges =  graph.GetOutEdges(start_id);
            openlist.push(topNode);

            while(!openlist.empty())
            {
                printf("**************Next node******************\n");
                // get current node from top of open list, check if it's the goal
                currNode = openlist.top();
                printf("--Top of open list: ");
                currNode.PrintNode();
                if(currNode.GetVertex().ID() == graph.GetVertex(goal_id).ID())
                {
                    printf("Found it!\n");
                    closedlist.push_back(currNode);
                    break;
                }

                // if current node isn't the goal, remove from open list and place in closed list
                openlist.pop();
                closedlist.push_back(currNode);
                edges = graph.GetOutEdges(currNode.GetVertex());

                // get neighboring nodes
                i = 0;
                int currCost = currNode.GetCost();
                printf("\n''''''Getting neighbors''''''''\n");
                while(i < edges.size())
                {   // vertex should be ID2, prev should be ID1
                    typename GraphType::Vertex vertex = graph.GetVertex(edges[i].GetID2());
                    typename GraphType::Vertex prevVertex = graph.GetVertex(edges[i].GetID1());
                    int g = edges[i].GetWeight() + currCost;
                    typename Heuristic::ReturnType h = heuristic( graph,graph.GetVertex(vertex.ID()),graph.GetVertex(goal_id) );

                    // for each edge in edges vector, create a node and add it to the open node list
                    currNode.CreateNodeFromEdge(vertex,prevVertex,h,g);
                    printf("Current Node: ");
                    currNode.PrintNode();

                    // check if currNode is already on the closed list
                    int j = 0;
                    bool exists = false;
                    while (j < closedlist.size() && !exists)
                    {
                        printf("j: %d;   closedlist size:%d\n", j, closedlist.size());
                        exists = false;
                        printf("curr node: ");
                        currNode.PrintNode();
                        printf("closedlist[%d]: ", j);
                        closedlist[j].PrintNode();
                        if(currNode.NodeEquals(currNode,closedlist[j]))
                        {
                            exists = true;
                            printf("Node exists\n");
                            // currentNode is LESS FIT then the existing node, continue
                            // else if current node is MORE FIT than the existing node, replace the existing node
                            if(currNode < closedlist[j])
                            {
                                printf("existing node better\n");
                                break;
                            }
                            else
                            {
                                printf("Replacing node\n");
                                closedlist[j] = currNode;
                            }

                        }
                        ++j;
                    }
                    if (!exists)
                    {
                        printf("Put node on OL:  ");
                        currNode.PrintNode();
                        openlist.push(currNode);
                    }

                    ++i;
                }

            }

            while (i < closedlist.size())
            {
                printf("Closed list i: ");
                closedlist[i].PrintNode();
                ++i;
            }


			//solution.push_back();
            callback.OnFinish( *this );
            return solution;
        }
        ////////////////////////////////////////////////////////////////////////
    private:
        // do not modify the next 2 lines
        const GraphType &            graph;
        Callback<GraphType,Astar>  & callback;
        // the next 4 lines are just sugestions
        // OpenListContainer, ClosedListContainer, SolutionContainer are typedefed
        OpenListContainer            openlist;
        ClosedListContainer          closedlist;
        SolutionContainer            solution;
        size_t                       start_id,goal_id;
};

#endif
