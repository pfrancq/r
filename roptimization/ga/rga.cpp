/*

  RGA.cpp

  Genetic Algorithms - Implementation

  (C) 1998-2000 by By P. Francq.

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
// Includes
#include "rga.h"
using namespace RGA;


//---------------------------------------------------------------------------
// General Variables
bool RGA::ExternBreak=false;
//cInstance *rrlGA::Instance=NULL;
long RGA::PopSize=0;
unsigned RGA::MaxBestPopAge=5;
unsigned RGA::MaxBestAge=10;
#ifdef __WIN32__
	char RGA::DebugOutputFileName[250]="C:\\TEMP\\rrlGA.xml";
#endif
#ifdef unix
	char RGA::DebugOutputFileName[250]="/home/pfrancq/rrlGA.xml";
#endif
RDebug RGA::DebugOutput;


//---------------------------------------------------------------------------
// Local Variables for this module
unsigned RGA::NbCross;
unsigned RGA::AgeNextMutation;
unsigned RGA::AgeNextBestMutation;
unsigned long RGA::AgeBestPop;
