/*

	R Project Library

	RGA.h

	Genetic Algorithms - Header

	Copyright 1998-2005 by the Université Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).

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
#ifndef RGA_H
#define RGA_H


//------------------------------------------------------------------------------
// include files for AINSI C/C++
#include <stdlib.h>
#ifndef RGADEBUG
	#define RGADEBUG 1
#endif


//------------------------------------------------------------------------------
// include files for R Project
#include <rstd.h>
#include <rstring.h>
#include <rcontainer.h>
#include <rtrace.h>
#include <rtextfile.h>
#include <random.h>


//------------------------------------------------------------------------------
namespace R{
//------------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/**
* This constance represent a non-identificator. It is used, for example, to
* specify that a variable containing an identificator of an object. is
* referencing no object.
*/
const size_t NoObject=0xFFFFFFFF;


//------------------------------------------------------------------------------
// General Variables
/**
* If true, GA stops
*/
extern bool ExternBreak;


//------------------------------------------------------------------------------
/**
* The RGAException class provides an exception generated by the GA.
* @author Pascal Francq
* @short GA Exception.
*/
class RGAException : public RException
{
public:

	/**
	 * Type of exception generated by the GA
	 */
	enum tException
	{
		eGAVerify                /** Verification of the chromosomes failed. */,
		eGACrossover             /** Crossover failed. */,
		eGAMutation              /** Mutation failed. */,
		eGAInversion             /** Inversion failed. */,
		eGAOptimisation          /** Optimization failed. */,
		eGARandomConstruct       /** Random initialization failed. */,
		eGAEvaluation            /** Evaluation failed. */,
		eGAPostEvaluation        /** Post-evaluation failed. */,
		eGA                      /** Normal GA exception. */
	};

protected:

	/**
	 * Type of the exception.
	 */
	tException Type;

public:

	/**
	* Construct an GA exception.
	* @param type            Type of the exception.
	*/
	RGAException(tException type=eGA) throw();

	/**
	* Construct an GA exception.
	* @param type            Type of the exception.
	* @param str             Message of the error.
	*/
	RGAException(const char* str,tException type=eGA) throw();

	/**
	* Construct an GA exception.
	* @param type            Type of the exception.
	* @param g               Generation.
	* @param c               Identifier of chromosome.
	*/
	RGAException(tException type,const size_t g,const size_t c) throw();

	/**
	* Construct the crossover exception.
	* @param g              Generation.
	* @param p1             Identifier of the first parent.
	* @param p2             Identifier of the second parent.
	* @param c              Identifier of the child.
	*/
	RGAException(const size_t g,const size_t p1,const size_t p2,const size_t c) throw();

	/**
	 * Get the type of the exception.
	 */
	tException GetType(void) const {return(Type);}
};


//------------------------------------------------------------------------------
// Forward declaration
template<class cVal,bool Max> class RFitness;
template<class cInst,class cChromo,class cFit,class cThreadData> class RChromo;
template<class cInst,class cChromo,class cFit,class cThreadData> class RInst;


}  //------- End of namespace R ------------------------------------------------


//------------------------------------------------------------------------------
#endif
