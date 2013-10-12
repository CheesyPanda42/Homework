#ifndef ASTAR
#define ASTAR

#include <list>
#include <vector>
#include <stdio.h>
#include <map>

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
struct Node 
{
	typename GraphType::Vertex vertex;
	typename GraphType::Vertex prevVertex;
	int heruistic;
	int costCurrent;
};

template <typename GraphType, typename Heuristic> 
class Astar 
{

typedef std::list<Node<GraphType> > OpenListContainer;
typedef std::list<Node<GraphType> > ClosedListContainer;
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
            openlist.clear();
            closedlist.clear();
            solution.clear();
            Heuristic heuristic;
			
			Node<GraphType> currentNode;
			
			
            //heuristic from start to goal
            typename Heuristic::ReturnType h = heuristic( graph,graph.GetVertex(start_id),graph.GetVertex(goal_id) );
			printf ("start_id: %d,  goal_id: %d,  heuristic %d\n", s, g, h);

			
			
            while ( openlist.size() > 0 ) {
                callback.OnIteration( *this );


            }

for (int i = 0; i < goal_id; i++){			
			typename Heuristic::ReturnType h = heuristic( graph,graph.GetVertex(i),graph.GetVertex(goal_id) );
			printf ("vertex_id: %d,  goal_id: %d,  heuristic %d\n", i, g, h);
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
