/*

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

*/


//---------------------------------------------------------------------------
#ifndef RGAH
#define RGAH


//---------------------------------------------------------------------------
// Standard libraries includes
#include <new.h>
#include <values.h>
#include "rstd/random.h"        // Random numbers generator
using namespace RStd;
#include "rfitness.h"
#include "rinst.h"
#include "rchromo.h"
#include "rdebug.h"
using namespace RGA;


//---------------------------------------------------------------------------
namespace RGA{
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Constances
const unsigned int NoObject=0xFFFFFFFF;
const char instMaster=0;                   // The instance is a master
const char instSlave=1;                    // The instance is a slave
const char instMultiProcess=2;             // The instance can use multi-processors
const char instMultiComputers=4;           // The instance can use slaves on other computers
const char instMultiEchos=8;               // The echo of the slaves is received by the master


//---------------------------------------------------------------------------
// General Variables
extern bool ExternBreak;
//extern cInstance *Instance;
extern long PopSize;
extern unsigned MaxBestPopAge;
extern unsigned MaxBestAge;
extern char DebugOutputFileName[];
extern RDebug DebugOutput;


//---------------------------------------------------------------------------
// Local Variables for this module
extern unsigned NbCross;
extern unsigned AgeNextMutation;
extern unsigned AgeNextBestMutation;
extern unsigned long AgeBestPop;
//cChromosome **Parents,**Childs,**tmpChrom;


//---------------------------------------------------------------------------
// Errors
class eGA : public exception {};
class eGAVerify : public eGA {};


}//------- End of namespace rrlGA -------------------------------------------
#endif
