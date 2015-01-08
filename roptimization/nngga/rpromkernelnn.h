/*

	R Project Library

	RPromKernelNN.h

	NNGA PROMETHEE Kernel - Header.

	Copyright 2002-2015 by Pascal Francq (pascal@francq.info).
	Copyright 2002-2008 by the Université Libre de Bruxelles (ULB).

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



//-----------------------------------------------------------------------------
#ifndef RPromKernelNN_H
#define RPromKernelNN_H


//-----------------------------------------------------------------------------
// include files for R Project
#include <rpromkernel.h>
#include <rpromsol.h>
#include <rpromcriterion.h>
#include <nngga.h>


//------------------------------------------------------------------------------
namespace R{
//------------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/**
* The RPromNN represented a PROMETHEE Kernel for the a clustering of objects
* having three matrices of measures: similarities, agreement ratios and
* disagreement ratios.
* @author Pascal Francq
* @short NNGGA PROMETHEE Kernel.
*/
template<class cChromo>
	class RPromKernelNN : public RPromKernel
{
	/**
	* Parameters.
	*/
	RParamsNN* Params;

	/**
	* Criteria representing the Similarity criterion "J".
	*/
	RPromCriterion* CritSimJ;

	/**
	* Criteria depending of the agreement ratios.
	*/
	RPromCriterion* CritAgreement;

	/**
	* Criteria depending of the disagreement ratios.
	*/
	RPromCriterion* CritDisagreement;

public:

	/**
	* Constructor.
	* @param p              Parameters.
	* @param nbsols         Number of solutions.
	*/
	RPromKernelNN(RParamsNN* p,size_t nbsols);

	/**
	* Assign chromosome as a solution.
	* @param s              Solution.
	* @param c              Chromosome.
	*/
	void AssignChromo(RPromSol* s,cChromo* c);

	/**
	* Assign values to a solution.
	* @param s              Solution.
	* @param sim            Similarity criterion.
	* @param agree          Agreement criterion.
	* @param disagree       Disagreement criterion.
	*/
	void AssignSol(RPromSol* s,double sim,double agree,double disagree);
};


//------------------------------------------------------------------------------
// inline implementation
#include <rpromkernelnn.hh>


}//------- End of namespace R --------------------------------------------------


//-----------------------------------------------------------------------------
#endif
