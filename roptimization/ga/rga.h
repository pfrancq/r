/*

	R Project Library

	RGA.h

	Genetic Algorithms - Header

	(C) 1998-2001 by P. Francq.

	Version $Revision$

	Last Modify: $Date$

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
#ifndef RGAH
#define RGAH


//-----------------------------------------------------------------------------
// include files for AINSI C/C++
#include <iostream.h>
#include <new.h>
#ifndef RGADEBUG
	#define RGADEBUG 1
#endif


//-----------------------------------------------------------------------------
// include files for R Project
#include <rstd/rstring.h>
#include <rstd/rcontainer.h>
#include <rstd/rmsg.h>
using namespace RStd;
#include <rio/rtextfile.h>
using namespace RIO;
#include <rmath/random.h>
using namespace RMath;


//-----------------------------------------------------------------------------
/**
* \namespace RGA
* \brief Generic Genetic Algorithm Classes.
*
* This namespace declares classes for working with Genetic Algorithms.
*/


//-----------------------------------------------------------------------------
namespace RGA{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// General Variables
/**
* If true, GA stops
*/
extern bool ExternBreak;


//-----------------------------------------------------------------------------
/**
* This class is a basic exception for the GA.
* @author Pascal Francq
* @short Basic GA Exception.
*/
class eGA
{
public:

	/**
	* A String to hold a message
	*/
	RString Msg;

	/**
	* Construct the exception.
	*/
	eGA(void);

	/**
	* Construct the exception.
	* @param msg            The message.
	*/
	eGA(const RString &msg);

	/**
	* Set Message.
	* m                     The message.
	*/
	void SetMsg(const char* m);
};


//-----------------------------------------------------------------------------
/**
* This class represent a verify exception, when a chromosome is not valid
* after a generation.
* @author Pascal Francq
* @short Verify GA Exception
*/
class eGAVerify : public eGA
{
public:

	/**
	* Construct the verify exception.
	* @param g              Generation.
	* @param c             Identificator of chromosome.
	*/
	eGAVerify(const unsigned int g,const unsigned int c);
};


//-----------------------------------------------------------------------------
/**
* This class represent a crossover exception, when a crossover can't be done.
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
	eGACrossover(const unsigned int g,const unsigned p1,const unsigned int p2,const unsigned int c);
};


//-----------------------------------------------------------------------------
/**
* This class represent a mutation exception, when a mutation can't be done.
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
	eGAMutation(const unsigned int g,const unsigned int c);
};


//-----------------------------------------------------------------------------
/**
* The eGARancomConstruct class provides a exeception that occurs when an error
* arrived during the construction.
* @author Pascal Francq
* @short Random Construct GA Exception
*/
class eGARandomConstruct : public eGA
{
public:
	/**
	* Construct the construction exception.
	*/
	eGARandomConstruct(void);
};


//-----------------------------------------------------------------------------
/*
* Examine the program parameters for a GA use in a non-graphical way.
* @author Pascal Francq
* @short Examine program parameters.
*/
bool Examine(int argc, char *argv[],const char* Prj,const char *Ver);


//-----------------------------------------------------------------------------
// Forward declaration
template<class cVal,bool Max> class RFitness;
template<class cInst,class cChromo,class cFit,class cThreadData> class RChromo;
template<class cInst,class cChromo,class cFit,class cThreadData> class RInst;
template<class cObj> class RObjs;


}  //------- End of namespace RGA ---------------------------------------------


//-----------------------------------------------------------------------------
#endif
