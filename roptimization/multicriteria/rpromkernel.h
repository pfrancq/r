/*

	R Project Library

	RPromKernel.h

	PROMETHEE Kernel - Header.

	Copyright 2000-2015 by Pascal Francq (pascal@francq.info).
	Copyright 2000 by Thomas L'Eglise.
	Copyright 2000-2008 by the Université Libre de Bruxelles (ULB).

	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU Library General Public
	License as published by the Free Software Foundation; either
	version 2.0 of the License, or (at your option) any later version.

	This library is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
	Library General Public License for more details.

	You should have received a copy of the GNU Library General Public
	License along with this library, as a file COPYING.LIB; if not, write
	to the Free Software Foundation, Inc., 59 Temple Place, Suite 330,
	Boston, MA  02111-1307  USA

*/



//------------------------------------------------------------------------------
#ifndef RPromKernel_H
#define RPromKernel_H


//------------------------------------------------------------------------------
// include files for R Project
#include <rpromsol.h>
#include <rpromcriterion.h>


//------------------------------------------------------------------------------
namespace R{
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
/**
* The RPromKernel provides a kernel to manage PROMETHEE session.
* @code
* // Create kernel, solutions and criteria
* RPromKernel Prom("Test",4,4);
* RPromSol* Car1=Prom.NewSol("Car1");
* RPromSol* Car2=Prom.NewSol("Car2");
* RPromSol* Car3=Prom.NewSol("Car3");
* RPromSol* Car4=Prom.NewSol("Car4");
*
* RPromCriterion* Price;
* RPromCriterion* Cons;
* RPromCriterion* Comfort;
* RPromCriterion* Power;
*
* Prom.AddCriterion(Price=new RPromLinearCriterion(RPromCriterion::Minimize,0.2,0.05,1.0,"Price"));
* Prom.AddCriterion(Cons=new RPromLinearCriterion(RPromCriterion::Minimize,0.2,0.05,1.0,"Cons"));
* Prom.AddCriterion(Comfort=new RPromLinearCriterion(RPromCriterion::Maximize,0.2,0.05,1.0,"Comfort"));
* Prom.AddCriterion(Power=new RPromLinearCriterion(RPromCriterion::Maximize,0.2,0.05,1.0,"Power"));
*
* // Assign Values
* Prom.Assign(Car1,Price,8.75);  Prom.Assign(Car1,Cons,6.2); Prom.Assign(Car1,Comfort,1.0);	Prom.Assign(Car1,Power,30);
* Prom.Assign(Car2,Price,13.75); Prom.Assign(Car2,Cons,7.5); Prom.Assign(Car2,Comfort,1.0);	Prom.Assign(Car2,Power,50);
* Prom.Assign(Car3,Price,25);    Prom.Assign(Car3,Cons,8);   Prom.Assign(Car3,Comfort,3.0);	Prom.Assign(Car3,Power,80);
* Prom.Assign(Car4,Price,62.5);  Prom.Assign(Car4,Cons,20);  Prom.Assign(Car4,Comfort,2.0);	Prom.Assign(Car4,Power,120);
*
* // Compute Prom
* Prom.ComputePrometheeII();
*
* // Print Fi
* RCursor<RPromSol> Res(Prom.GetSols());
* cout<<"Sol\tFiPlus\t\tFiMinus\t\tFi"<<endl;
* cout.precision(3);
* for(Res.Start();!Res.End();Res.Next())
*    cout<<"Car"<<Res()->GetId()+1<<"\t"<<Res()->GetFiPlus()<<"\t\t"<<Res()->GetFiMinus()<<"\t\t"<<Res()->GetFi()<<endl;
* @endcode
* @short PROMETHEE Kernel.
*/
class RPromKernel
{
	/**
	* The name of the kernel.
	*/
	RString Name;

	/**
	* Solutions.
	*/
	RContainer<RPromSol,true,true> Solutions;

	/**
	 * Solutions ordered by fitness.
	 */
	RContainer<RPromSol,false,false> OrderedSolutions;

	/**
	* Criteria.
	*/
	RContainer<RPromCriterion,true,true> Criteria;

	/**
	 * Look if the solution must be reordered (for example after a computing).
	 */
	bool MustReOrder;

public:

	/**
	* Construct the kernel.
	* @param name           The name of the kernel.
	* @param sol            Initial maximal number of solutions.
	* @param crit           Initial maximal number of criteria.
	*/
	RPromKernel(const char* name,size_t sol,size_t crit);

	/**
	* Compute the evaluation functions for the solutions.
	*/
	void ComputeEvalFunc(void);

	/**
	* Calculate PROMETHEE II.
	*/
	virtual void ComputePrometheeII(void);

	/**
	 * Add a new criterion.
	 * @param crit           Pointer to the new criterion.
	 */
	void AddCriterion(RPromCriterion* crit);

	/**
	 * @return a pointer to a given criterion.
	 * @param name           Name of the criterion.
	 */
	RPromCriterion* GetCriterion(const RString& name) const {return(Criteria.GetPtr(name,false));}

	/**
	 * @return a pointer to a given criterion.
	 * @param id             Identifier of the criterion.
	 */
	RPromCriterion* GetCriterion(const size_t id) const {return(Criteria.GetPtr(id));}

	/**
	 * Add a solution to the PROMETHEE kernel. Each solution must have an unique
	 * identifier. A way to manage that is to assign to each solution added an
	 * identifier equals to GetNbSols().
    * @param sol            Solution to add.
    */
	void AddSol(RPromSol* sol);

	/**
	* Create a new solution.
	*/
	virtual RPromSol* NewSol(void);

	/**
	* Create a new solution.
	* @param name           Name of the solution.
	*/
	virtual RPromSol* NewSol(const RString& name);

	/**
	 * @return the number of solutions managed by the kernel.
	 */
	size_t GetNbSols(void) const {return(Solutions.GetNb());}

	/**
	* Assign a value to a criterion.
	* @param sol            Pointer to the solution.
	* @param crit           Pointer to the criterion.
	* @param v              Value.
	*/
	virtual void Assign(RPromSol* sol,RPromCriterion* crit,const double v);

	/**
	* Assign a value to a criterion.
	* @param sol            Name of to the solution.
	* @param crit           Pointer to the criterion.
	* @param v              Value.
	*/
	virtual void Assign(const RString& sol,RPromCriterion* crit,const double v);

	/**
	* Assign a value to a criterion.
	* @param sol            Pointer to the solution.
	* @param crit           Name of the criterion.
	* @param v              Value.
	*/
	virtual void Assign(RPromSol* sol,const RString& crit,const double v);

	/**
	* Assign a value to a criterion.
	* @param sol            Name of the solution.
	* @param crit           Name of the criterion.
	* @param v              Value.
	*/
	virtual void Assign(const RString& sol,const RString& crit,const double v);

private:

	/**
	* Function used to sort the solutions by fit.
	* @param a              Pointer to the first solution.
	* @param b              Pointer to the second solution.
	*/
	static int sort_function_solutions(const void *a,const void *b);

	/**
	 * Order the solutions by descending order of their fitness.
	 */
	void OrderSolutions(void);

public:

	/**
	* @return the best solution.
	*/
	const RPromSol* GetBestSol(void);

	/**
	* @return a cursor over the solutions ordered descending by their fitness.
	*/
	RCursor<RPromSol> GetSols(void);

	/**
	 * Copy the solutions ordered descending by their fitness into a container.
	 * @param sols           Container holding the results.
	 */
	void CopySols(RContainer<RPromSol,false,false>& sols);

	/**
    * @return the Fi of the worst solution.
    */
	double GetMinFi(void);

	/**
    * @return the Fi of the best solution.
    */
	double GetMaxFi(void);

	/**
	* Return the best solution identifier.
	*/
	size_t GetBestSolId(void) {return(GetBestSol()->Id);}

	/**
	* Clear the kernel.
	*/
	void Clear(void);

	/**
	* Clear the solutions of the kernel.
	*/
	void ClearSols(void);

	/**
	 * Print the solutions ordered on screen.
	 * @param normalized     Show the values normalized or not.
    */
	void Print(bool normalized);

	/**
	* Destruct the kernel.
	*/
	virtual ~RPromKernel(void);

	// friend classes
	friend class RPromCriterion;
};


}  //-------- End of namespace R -----------------------------------------------


//------------------------------------------------------------------------------
#endif
