/*

	Rainbow Library Project

  RGA.h

  Genetic Algorithms - Header

  (C) 1998-2000 by P. Francq.

  Version $Revision$

  Last Modify: $Date$

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

	As a special exception to the GNU General Public License, permission is
	granted for additional uses of the text contained in its release
	of the Rainbow Library.

	The exception is that, if you link the Rainbow with other files
	to produce an executable, this does not by itself cause the
	resulting executable to be covered by the GNU General Public License.
	Your use of that executable is in no way restricted on account of
	linking the Rainbow library code into it.

	This exception does not however invalidate any other reasons why
	the executable file might be covered by the GNU General Public License.

	This exception applies only to the code released under the
	name Rainbow.  If you copy code from other releases into a copy of
	Rainbow, as the General Public License permits, the exception does
	not apply to the code that you add in this way.  To avoid misleading
	anyone as to the status of such modified files, you must delete
	this exception notice from them.

	If you write modifications of your own for Rainbow, it is your choice
	whether to permit this exception to apply to your modifications.
	If you do not wish that, delete this exception notice.

*/



//---------------------------------------------------------------------------
#ifndef RGAH
#define RGAH


//---------------------------------------------------------------------------
// include files for AINSI C/C++
#include <iostream.h>
#include <new.h>
#include <values.h>
#include <limits.h>
#ifndef RGADEBUG
	#define RGADEBUG 1
#endif


//---------------------------------------------------------------------------
// include files for Rainbow
#include "rstd/random.h"        // Random numbers generator
#include "rstd/rstring.h"       // Strings
#include "rstd/rcontainer.h"    // Container
#include "rtextfile.h"					// Text files
using namespace RStd;


//---------------------------------------------------------------------------
// Forward class declaration
namespace RGA
{
	class RDebug;
	template<class cVal,bool Max>	class RFitness;
	template<class cInst,class cChromo,class cFit,class cThreadData> class RChromo;
	template<class cInst,class cChromo,class cFit,class cThreadData> class RInst;
	class eGA;
}


//---------------------------------------------------------------------------
// include files for GA
#include "rgasignals.h"
#include "rdebug.h"
#include "rfitness.h"
#include "rinst.h"
#include "rchromo.h"
using namespace RGA;


//---------------------------------------------------------------------------
namespace RGA{
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Constances for objects manipulation
const unsigned int NoObject=UINT_MAX;				// Reference to no object


//---------------------------------------------------------------------------
// General Variables
extern bool ExternBreak;										// If true, GA stops
extern RDebug *Debug;


//---------------------------------------------------------------------------
/** This class is a basic exception for the GA.
	* @author Pascal Francq
	* @short Basic GA Exception.
	*/
class eGA
{
public:
	/** A String to hold a message */
	RString Msg;

	/** Construct the exception.
		* @param msg	The message.
		*/
	eGA(const RString &msg);
};


//---------------------------------------------------------------------------
/** This class represent a verify exception, when a chromosome is not valid
	* after a generation.
	* @author Pascal Francq
	* @short Verify GA Exception
	*/
class eGAVerify : public eGA
{
public:
	/** Construct the verify exception.*/
	eGAVerify(void);
};


//---------------------------------------------------------------------------
/** This class represent a crossover exception, when a crossover can't be done.
	* @author Pascal Francq
	* @short Crossover GA Exception
	*/
class eGACrossover : public eGA
{
public:
	/** Construct the crossover esception.*/
	eGACrossover(void);
};


//---------------------------------------------------------------------------
/** The eGARancomConstruct class provides a exeception that occurs when an error
	* arrived during the construction.
	* @author Pascal Francq
	* @short Random Construct GA Exception
	*/
class eGARandomConstruct : public eGA
{
public:
	eGARandomConstruct(void);
};


//---------------------------------------------------------------------------
/* Examine the program parameters for a GA use in a non-graphical way.
	* @author Pascal Francq
	* @short Examine program parameters.
	*/
bool Examine(int argc, char *argv[],const char* Prj,const char *Ver);


//---------------------------------------------------------------------------
// Definitions of templates
#include "rfitness.hh"
#include "rinst.hh"
#include "rchromo.hh"


}//------- End of namespace RGA ---------------------------------------------
#endif
