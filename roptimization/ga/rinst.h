/*

  RInst.h

  Instance of Genetic Algorithms - Header

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
#ifndef RInstH
#define RInstH


//---------------------------------------------------------------------------
// Standard libraries includes
#include <stdlib.h>

//---------------------------------------------------------------------------
// GA Rainbow libraries includes
#include "rga.h"
using namespace RGA;


//---------------------------------------------------------------------------
namespace RGA{
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit>
	class RInst
{
public:
  cChromo **Chromosomes;
  cChromo *BestChromosome,*BestInPop;
  unsigned PopSize;
  unsigned long Gen;
  unsigned long AgeBest;

  RInst(unsigned popsize) throw(bad_alloc);
  virtual bool RandomConstruct(void);
  virtual bool StopCondition(void)=0;
  virtual void PostRun(void) {}
  virtual void DisplayInfos(void) {}
  inline void Evaluate(void);
  inline void Crossover(void);
  inline void Mutation(void);
  void Generation(void);
  void Run(void);
  virtual bool Verify(void);
  virtual ~RInst(void);

private:
  int sort_function_cChromosome( const void *a, const void *b);
  cChromo **Parents,**Childs,**tmpChrom;
};


//---------------------------------------------------------------------------
// RInst classes codes
#include "rinst.hh"


}//------- End of namespace RGA ---------------------------------------------
#endif
