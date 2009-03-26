/*

	R Project Library

	RPromKernelSC.h

	Similarity-based Clustering PROMETHEE Kernel - Header.

	Copyright 2002-2009 by Pascal Francq (pascal@francq.info).
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
#ifndef RPromKernelSC_H
#define RPromKernelSC_H


//-----------------------------------------------------------------------------
// include files for R Project
#include <rpromkernel.h>
#include <rpromsol.h>
#include <rpromcriterion.h>
#include <scga.h>


//------------------------------------------------------------------------------
namespace R{
//------------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/**
* The RPromSC represented a PROMETHEE Kernel for the a clustering of objects
* having three matrices of measures: similarities, agreement ratios and
* disagreement ratios.
* @author Pascal Francq
* @short Similarity-based Clustering PROMETHEE Kernel.
*/
template<class cChromo>
	class RPromKernelSC : public RPromKernel
{
	/**
	* Parameters.
	*/
	RParamsSC* Params;

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
	*/
	RPromKernelSC(RParamsSC* p);

	/**
	* Assign chromosome as a solution.
	* @param s              Solution.
	* @param c              Chromosome.
	*/
	void AssignChromo(RPromSol* s,cChromo* c);
};


//------------------------------------------------------------------------------
// inline implementation
#include <rpromkernelsc.hh>


}//------- End of namespace R --------------------------------------------------


//-----------------------------------------------------------------------------
#endif
