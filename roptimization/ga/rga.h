/*

	R Project Library

	RGA.h

	Genetic Algorithms - Header

	Copyright 1998-2003 by the Université Libre de Bruxelles.

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
#include <rmsg.h>
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
const unsigned int NoObject=0xFFFFFFFF;


//------------------------------------------------------------------------------
// General Variables
/**
* If true, GA stops
*/
extern bool ExternBreak;


//------------------------------------------------------------------------------
/**
* The eGA class provides a basic exception for the GA.
* @author Pascal Francq
* @short Basic GA Exception.
*/
class eGA : public R::RException
{
public:

	/**
	* Construct an exception.
	*/
	eGA(void) throw()
		: R::RException() {}

	/**
	* Construct an exception.
	* @param str                      Message of the error.
	*/
	eGA(const char* str) throw()
		: R::RException(str) {}
};


//------------------------------------------------------------------------------
/**
* The eGAVerify class provides an exception for the verification.
* @author Pascal Francq
* @short Verify GA Exception
*/
class eGAVerify : public eGA
{
public:

	/**
	* Construct the verify exception.
	* @param g             Generation.
	* @param c             Identificator of chromosome.
	*/
	eGAVerify(const unsigned int g,const unsigned int c) throw();

	/**
	* Construct the verify exception.
	* @param msg            The message.
	*/
	eGAVerify(const char* msg) throw() : eGA(msg) {}
};


//------------------------------------------------------------------------------
/**
* The eGACrossover class provides an exception for the crossover operator.
* @author Pascal Francq
* @short Crossover GA Exception
*/
class eGACrossover : public eGA
{
public:

	/**
	* Construct the crossover exception.
	* @param g              Generation.
	* @param p1             Identificator of the first parent.
	* @param p2             Identificator of the second parent.
	* @param c              Identificator of the child.
	*/
	eGACrossover(const unsigned int g,const unsigned p1,const unsigned int p2,const unsigned int c) throw();

	/**
	* Construct the crossover exception.
	* @param msg            The message.
	*/
	eGACrossover(const char* msg) throw() : eGA(msg) {}
};


//------------------------------------------------------------------------------
/**
* The eGAMutation class provides an exception for the mutation operator.
* @author Pascal Francq
* @short Mutation GA Exception
*/
class eGAMutation : public eGA
{
public:

	/**
	* Construct the mutation exception.
	* @param g              Generation.
	* @param c              Identificator of the child.
	*/
	eGAMutation(const unsigned int g,const unsigned int c) throw();

	/**
	* Construct the mutation exception.
	* @param msg            The message.
	*/
	eGAMutation(const char* msg) throw() : eGA(msg) {}
};


//------------------------------------------------------------------------------
/**
* The eGAInversion class provides an exception for the inversion operator.
* @author Pascal Francq
* @short Invesion GA Exception
*/
class eGAInversion : public eGA
{
public:

	/**
	* Construct the inversion exception.
	* @param g              Generation.
	* @param c              Identificator of the child.
	*/
	eGAInversion(const unsigned int g,const unsigned int c) throw();

	/**
	* Construct the inversion exception.
	* @param msg            The message.
	*/
	eGAInversion(const char* msg) throw() : eGA(msg) {}
};


//------------------------------------------------------------------------------
/**
* The eGAOptimisation class provides an exception for the optimisation
* operator.
* @author Pascal Francq
* @short Optimisation GA Exception
*/
class eGAOptimisation : public eGA
{
public:

	/**
	* Construct the optimisation exception.
	* @param g              Generation.
	* @param c              Identificator of the child.
	*/
	eGAOptimisation(const unsigned int g,const unsigned int c) throw();

	/**
	* Construct the optimisation exception.
	* @param msg            The message.
	*/
	eGAOptimisation(const char* msg) throw() : eGA(msg) {}
};


//------------------------------------------------------------------------------
/**
* The eGARancomConstruct class provides a exception for the random
* construction operator.
* @author Pascal Francq
* @short Random Construct GA Exception
*/
class eGARandomConstruct : public eGA
{
public:

	/**
	* Construct the random construction exception.
	* @param g              Generation.
	* @param c              Identificator of the child.
	*/
	eGARandomConstruct(const unsigned int g,const unsigned int c) throw();

	/**
	* Construct the random construction exception.
	* @param msg            The message.
	*/
	eGARandomConstruct(const char* msg) throw() : eGA(msg) {}
};


//------------------------------------------------------------------------------
/**
* The eGAEvaluate class provides a exeception for the evaluation operator.
* @author Pascal Francq
* @short Evaluation GA Exception
*/
class eGAEvaluation : public eGA
{
public:

	/**
	* Construct the evaluation exception.
	* @param g              Generation.
	* @param c              Identificator of the child.
	*/
	eGAEvaluation(const unsigned int g,const unsigned int c) throw();

	/**
	* Construct the evaluation exception.
	* @param msg            The message.
	*/
	eGAEvaluation(const char* msg) throw() : eGA(msg) {}
};


//------------------------------------------------------------------------------
/**
* The eGAPostEvaluation class provides a exeception for the post evaluation
* operator.
* @author Pascal Francq
* @short Post Evaluation GA Exception
*/
class eGAPostEvaluation : public eGA
{
public:

	/**
	* Construct the post evaluation exception.
	* @param g              Generation.
	* @param c              Identificator of the child.
	*/
	eGAPostEvaluation(const unsigned int g,const unsigned int c) throw();

	/**
	* Construct the post evaluation exception.
	* @param msg            The message.
	*/
	eGAPostEvaluation(const char* msg) throw() : eGA(msg) {}
};


//------------------------------------------------------------------------------
// Forward declaration
template<class cVal,bool Max> class RFitness;
template<class cInst,class cChromo,class cFit,class cThreadData> class RChromo;
template<class cInst,class cChromo,class cFit,class cThreadData> class RInst;
template<class cObj> class RObjs;


}  //------- End of namespace R ------------------------------------------------


//------------------------------------------------------------------------------
#endif
