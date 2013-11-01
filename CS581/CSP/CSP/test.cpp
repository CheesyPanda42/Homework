#include <vector>
#include <iostream>
#include <fstream>
#include "contraints.graph.h"
#include "contraints.h"
#include "variable.h"
#include "csp.h"
#include <cmath>
#include <cstdlib>
#include <ctime>


////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////

//global function
std::vector<int> getVector(int val, ...) {
	std::vector<int> result;

	va_list valist;
	int arg;
	va_start(valist, val);
	result.push_back(val);
	while ((arg = va_arg(valist, int)) != 0) {
		result.push_back(arg);
	}
	va_end(valist);
	return result;
}



int main () {
	std::vector<int> range = getVector(0,1,2,3,NULL);
	Variable x ( "x", range );
	Variable y ( "y", range );
	Variable z ( "z", range );


	SumEqual<Variable,8> c1(&x,&y,&z,NULL);
	SumEqual<Variable,5> c2(&x,&z,NULL);
	SumEqual<Variable,8> c4(&x,&z,NULL); //x+z cannot be 8
	std::cout << c1;
	std::cout << c2;
	std::cout << c4;
	std::cout << x;
	std::cout << y;
	std::cout << z;

	if ( c1.Satisfiable() ) std::cout << "c1 is Satisfiable\n";
	else std::cout << "c1 is not Satisfiable\n";
	if ( c2.Satisfiable() ) std::cout << "c2 is Satisfiable\n";
	else std::cout << "c2 is not Satisfiable\n";
	if ( c4.Satisfiable() ) std::cout << "c4 is Satisfiable\n";
	else std::cout << "c4 is not Satisfiable\n";

	std::cout << "================\n";

	AllDiff<Variable> c3(&x,&y,&z,NULL);
	std::cout << c3;
	if ( c3.Satisfiable() ) std::cout << "c3 is Satisfiable\n";
	else std::cout << "c3 is not Satisfiable\n";

	std::cout << "Let's assign values to variables\n";
	x.Assign(3);
	y.Assign(2);
	z.Assign(2);
	std::cout << x << y << z;
	if ( c3.Satisfiable() ) std::cout << "c3 is Satisfiable\n";
	else std::cout << "c3 is not Satisfiable\n";

	std::vector<int> range01 = getVector(0,1,NULL);

	Variable u ( "u", range01 );
	Variable v ( "v", range01 );
	Variable w ( "w", range01 );
	Variable w2 ( "w2", getVector(0,1,2,NULL) );

	AllDiff<Variable> c5(&u,&v,&w,NULL);
	AllDiff<Variable> c6(&u,&v,&w2,NULL);
	std::cout << c5 << u << v << w;

	if ( c5.Satisfiable() ) std::cout << "c5 is Satisfiable\n";
	else std::cout << "c5 is not Satisfiable\n";

	std::cout << c6 << u << v << w2;
	if ( c6.Satisfiable() ) std::cout << "c6 is Satisfiable\n";
	else std::cout << "c6 is not Satisfiable\n";

	try {
		ConstraintGraph<Constraint<Variable> > cg;
		cg.InsertVariable(u);
		cg.InsertVariable(v);
		cg.InsertVariable(w);
		cg.InsertVariable(w2);
		cg.InsertConstraint(c5);
		cg.InsertConstraint(c6);
		cg.Print();
		cg.PreProcess();

		std::set<Variable*> neighbors = cg.GetNeighbors(&w2);
		std::set<Variable*>::const_iterator b_neighbor = neighbors.begin();
		std::set<Variable*>::const_iterator e_neighbor = neighbors.end();
		std::cout << "Neighbors of " << w2 << " are \n";
		for ( ; b_neighbor!=e_neighbor; ++b_neighbor ) {
			std::cout << "--- " << **b_neighbor;
		}

		CSP<ConstraintGraph<Constraint<Variable> > > csp( cg );

#ifdef DFS
		if (
				csp.SolveDFS(0)
		   ) {
			cg.Print();
		}
		else {
			std::cout << "No solution found\n";
		}
#else
		std::cout << "Skip SolveDFS\n";
#endif
	} catch ( const char * msg ) {
		std::cout << msg << std::endl;
	}
}

