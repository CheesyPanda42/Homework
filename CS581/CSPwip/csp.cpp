#include "csp.h"

#ifdef INLINE_CSP
	//#warning "INFO - inlining CSP methods"
	#define INLINE inline
#else
	//#warning "INFO - NOT inlining CSP methods"
	#define INLINE
#endif

////////////////////////////////////////////////////////////
//CSP constructor
template <typename T>
CSP<T>::CSP(T &cg) :
	arc_consistency(),
	cg(cg),
	solution_counter(0),
	recursive_call_counter(0),
	iteration_counter(0)
{
}

////////////////////////////////////////////////////////////
//CSP solver, brute force - no forward checking
template <typename T>
bool CSP<T>::SolveDFS(unsigned level)
{
    //std::cout << "\n----IN SOLVEDFS LEVEL " << level << " -------\n";
    ++recursive_call_counter;
    if(recursive_call_counter % 500000 == 0)
    {
        std::cout << recursive_call_counter << std::endl;
    }
    // Variable declarations
    bool sol_found = false;
    bool con_holds = false;
   // int num_var;
    std::map<Variable*, std::set<typename Variable::Value> > prevState;
    const typename std::vector<Variable*>& all_var = cg.GetAllVariables();
   // num_var = all_var.size();


    Variable* var = MinRemVal();



   // Get constraints for current variable
    const std::vector<const Constraint*>& constraints = cg.GetConstraints(var);
    Constraint const* curr_constraint;
    // Iterators for constraint



while(!sol_found)
{
    ++iteration_counter;
    if(!var->IsAssigned()) var->Assign();
    typename std::vector<const Constraint*>::const_iterator b_con = constraints.begin();
    typename std::vector<const Constraint*>::const_iterator e_con = constraints.end();
    for(;b_con < e_con; ++b_con)
    {
        curr_constraint = *b_con;
        con_holds = curr_constraint->Satisfiable();
        if(con_holds)
        {
            sol_found = true;
        }
        else
        {
            sol_found = false;
            break;
        }
    }
    if(sol_found && !cg.AllVariablesAssigned())
    {
        prevState = SaveState(var);
        sol_found = SolveDFS(level+1);
        LoadState(prevState);
    }
    if(!sol_found)
    {
        var->RemoveValue(var->GetValue());
        var->UnAssign();
        if(var->IsImpossible()) return false;
    }

}

    //std::cout << "\n----EXITING SOLVEDFS LEVEL " << level << " -------\n";
    return sol_found;
}


////////////////////////////////////////////////////////////
//CSP solver, uses forward checking
template <typename T>
bool CSP<T>::SolveFC(unsigned level) {
	++recursive_call_counter;
	//std::cout << "entering SolveFC (level " << level << ")\n";

	/*

    //choose a variable by MRV
	Variable* var_to_assign = MinRemVal();
	//Variable* var_to_assign = MaxDegreeHeuristic();
 {
        ++iteration_counter;



    }

*/
return false;
}
////////////////////////////////////////////////////////////
//CSP solver, uses arc consistency
template <typename T>
bool CSP<T>::SolveARC(unsigned level) {
	++recursive_call_counter;
	//std::cout << "entering SolveARC (level " << level << ")\n";

return false;

    /*

    //choose a variable by MRV
	Variable* var_to_assign = MinRemVal();


     {
        ++iteration_counter;



    }


*/
}


template <typename T>
INLINE
bool CSP<T>::ForwardChecking(Variable *x) {


return false;

}
////////////////////////////////////////////////////////////
//load states (available values) of all unassigned variables
template <typename T>
void CSP<T>::LoadState(
		std::map<Variable*,
		std::set<typename CSP<T>::Variable::Value> >& saved) const
{
	typename std::map<Variable*, std::set<typename Variable::Value> >::iterator
		b_result = saved.begin();
	typename std::map<Variable*, std::set<typename Variable::Value> >::iterator
		e_result = saved.end();

	for ( ; b_result != e_result; ++b_result ) {
		//std::cout << "loading state for "
		//<< b_result->first->Name() << std::endl;
		(*b_result).first->SetDomain( (*b_result).second );
	}
}


////////////////////////////////////////////////////////////
//save states (available values) of all unassigned variables
//except the current
template <typename T>
INLINE
std::map< typename CSP<T>::Variable*, std::set<typename CSP<T>::Variable::Value> >
CSP<T>::SaveState(typename CSP<T>::Variable* x) const {
	std::map<Variable*, std::set<typename Variable::Value> > result;

	const std::vector<Variable*>& all_vars = cg.GetAllVariables();
	typename std::vector<Variable*>::const_iterator
		b_all_vars = all_vars.begin();
	typename std::vector<Variable*>::const_iterator
		e_all_vars = all_vars.end();
	for ( ; b_all_vars!=e_all_vars; ++b_all_vars) {
		if ( !(*b_all_vars)->IsAssigned() && *b_all_vars!=x ) {
			//std::cout << "saving state for "<< (*b_all_vars)->Name() << std::endl;
			result[ *b_all_vars ] = (*b_all_vars)->GetDomain();
		}
	}
	return result;
}
////////////////////////////////////////////////////////////
//check the current (incomplete) assignment for satisfiability
template <typename T>
INLINE
bool CSP<T>::AssignmentIsConsistent( Variable* p_var ) const {


return false;







}
////////////////////////////////////////////////////////////
//insert pair
//(neighbors of the current variable, the current variable)
// for all y~x insert (y,x)
//into arc-consistency queue
template <typename T>
INLINE
void CSP<T>::InsertAllArcsTo(Variable* cv,Variable *except) {











}
////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////
//AIMA p.146 AC-3 algorithm
template <typename T>
INLINE
bool CSP<T>::CheckArcConsistency(Variable* x) {




return false;







}
////////////////////////////////////////////////////////////
//CHECK that for each value of x there is a value of y
//which makes all constraints involving x and y satisfiable
template <typename T>
INLINE
bool CSP<T>::RemoveInconsistentValues(Variable* x,Variable* y,const Constraint* c) {











return false;
}
////////////////////////////////////////////////////////////
//choose next variable for assignment
//choose the one with minimum remaining values
template <typename T>
INLINE
typename CSP<T>::Variable* CSP<T>::MinRemVal() {

    Variable* var;
    Variable* temp;
    const std::vector <Variable*>& all_var = cg.GetAllVariables();
   // int size_of_var;

    var = all_var.front();
    //size_of_var = var->SizeDomain();

  //  std::cout << *var << "  " << size_of_var << std::endl;

    typename std::vector<Variable*>::const_iterator b_vars = all_var.begin();
    typename std::vector<Variable*>::const_iterator e_vars = all_var.end();

    for ( ;b_vars!=e_vars;++b_vars)
    {
        temp = *b_vars;
        //std::cout << "Temp: " << temp->Name() <<"  " <<  temp->SizeDomain() << std::boolalpha << "  " << temp->IsAssigned();
        //if(temp->IsAssigned()) std::cout << "  curr value: " << temp->GetValue();
        //std::cout << std::endl;
        if(var->IsAssigned())
        {

            var = temp;
        }
        else
        {
            if(var->SizeDomain() > temp->SizeDomain())
            {
                var = temp;
            }

        }

    }

    //std::cout << var->Name() << "chosen.\n\n";
    return var;

}
////////////////////////////////////////////////////////////
//choose next variable for assignment
//choose the one with max degree
template <typename T>
typename CSP<T>::Variable* CSP<T>::MaxDegreeHeuristic() {














}
#undef INLINE
