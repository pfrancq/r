/*

	R Project Library

	iRInst.h

	Genetic Algorithm Instance Interface - Header

	Copyright 1998-2014 by Pascal Francq (pascal@francq.info).
	Copyright 1998-2008 by the Université Libre de Bruxelles (ULB).

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
#ifndef iRInst_H
#define iRInst_H



//------------------------------------------------------------------------------
// include files for R Project
#include <rga.h>
#include <rdebug.h>
#include <rcursor.h>
#include <robject.h>
#include <rnotification.h>


//------------------------------------------------------------------------------
namespace R{
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
/**
* Interface for an instance of the generic GA.
* @author Pascal Francq
* @short Genetic Algorithm Instance Interface.
*/
class iRInst
{
protected:

	/**
	* Size of the population.
	*/
	size_t PopSize;

	/**
	* Number of generations.
	*/
	size_t Gen;

	/**
	* Age of the best chromosome.
	*/
	size_t AgeBest;

	/**
	* Age of the best chromosome ever calculate.
	*/
	size_t AgeBestPop;

	/**
	 * Determine if the GA is paused.
	 */
	bool Pause;

	/**
	* Construct the instance.
	* @param popsize        The size of the population.
	* @param name           Name of the genetic algorithm.
	*/
	iRInst(size_t popsize);

public:

	/**
	 * @return the size of the population.
	 */
	inline size_t GetPopSize(void) const {return(PopSize);}

	/**
	 * @return the number of generations run.
	 */
	inline size_t GetGen(void) const {return(Gen);}

	/**
	* @return the age of the best chromosome.
	*/
	inline size_t GetAgeBest(void) const {return(AgeBest);}

	/**
	* @return the age of the best chromosome ever calculate.
	*/
	inline size_t GetAgeBestPop(void) const {return(AgeBestPop);}

	/**
	* This functions runs the GA.
	*/
	virtual void Run(void)=0;

	/**
	* Destruct the instance.
	*/
	virtual ~iRInst(void);
};


}  //------- End of namespace R ------------------------------------------------


//------------------------------------------------------------------------------
#endif
