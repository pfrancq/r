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
	RAinbow, as the General Public License permits, the exception does
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
#include <new.h>
#include <values.h>
#ifndef NULL
	const long int NULL=0;
#endif
#ifndef RGADEBUG
	#define RGADEBUG 1
#endif


//---------------------------------------------------------------------------
// include files for Rainbow
#include "rstd/random.h"        // Random numbers generator
using namespace RStd;


//---------------------------------------------------------------------------
// Forward class declaration
namespace RGA
{
	class RDebug;
	template<class cVal,bool Max>	class RFitness;
	template<class cInst,class cChromo,class cFit> class RChromo;
	template<class cInst,class cChromo,class cFit> class RInst;
}


//---------------------------------------------------------------------------
// include files for GA
#include "rdebug.h"
#include "rfitness.h"
#include "rinst.h"
#include "rchromo.h"
using namespace RGA;


//---------------------------------------------------------------------------
namespace RGA{
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Constances for Instance type
const unsigned int NoObject=0xFFFFFFFF;
const char instMaster=0;                   	// The instance is a master
const char instSlave=1;                    	// The instance is a slave
const char instMultiProcess=2;             	// The instance can use multi-processors
const char instMultiComputers=4;           	// The instance can use slaves on other computers
const char instMultiEchos=8;               	// The echo of the slaves is received by the master


//---------------------------------------------------------------------------
// Constances for Signals
const unsigned int sigNothing=0;						// Nothing
const unsigned int sigInteract=1;						// Gives the world possibility to Interact
const unsigned int sigGAInit=2;							// GA is initialise
const unsigned int sigGARun=3;							// GA starts to run
const unsigned int sigGADone=4;							// GA is done
const unsigned int sigGAPostRun=5;					// GA starts the post-run step
const unsigned int sigGAEndRun=6;						// GA stops to run
const unsigned int sigGAGen=7;							// One generation generated
const unsigned int sigNewBest=8;						// A new best chromosome


//---------------------------------------------------------------------------
// General Variables
extern bool ExternBreak;										// If true, GA stops
extern unsigned int MaxBestPopAge;
extern unsigned int MaxBestAge;
extern RDebug *Debug;


//---------------------------------------------------------------------------
// Signal Macanism
#ifdef RGANOSIGNALS
	inline void EmitSig(void) {}
#else
	typedef void typeSigFunc(unsigned int);
	extern typeSigFunc *SigFunc;								// Signal Function
	inline void EmitSig(unsigned int sig) {if(SigFunc) SigFunc(sig);}
#endif


//---------------------------------------------------------------------------
// Local Variables for this module
extern unsigned int NbCross;
extern unsigned int AgeNextMutation;
extern unsigned int AgeNextBestMutation;
extern unsigned int AgeBestPop;


//---------------------------------------------------------------------------
// Errors
class eGA : public exception {};
class eGAVerify : public eGA {};


//---------------------------------------------------------------------------
// Examine
bool Examine(int argc, char *argv[],const char* Prj,const char *Ver);


//---------------------------------------------------------------------------
// Definitions of templates
#include "rfitness.hh"
#include "rinst.hh"
#include "rchromo.hh"


}//------- End of namespace RGA ---------------------------------------------
#endif
