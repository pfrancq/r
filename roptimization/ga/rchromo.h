/*

  RChromo.h

  Chromosomes of Genetic Algorithms - Header

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
#ifndef RChromoH
#define RChromoH


//---------------------------------------------------------------------------
// Standard libraries includes
#include "rga.h"
using namespace RGA;


//---------------------------------------------------------------------------
namespace RGA{
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit> class RChromo
{
public:
  cInst *Instance;
  unsigned Id;
  cFit *Fitness;
  bool ToEval;

  RChromo(cInst *,unsigned) throw(bad_alloc);
  virtual bool RandomConstruct(void) {return(true);}
  virtual void Evaluate(void) {}
  virtual bool Crossover(RChromo *,RChromo *) {return(true);}
  virtual bool Mutation(void) {return(true);}
  virtual void Print(void) {}
  virtual bool Verify(void) {return(true);}
  virtual RChromo& operator=(const RChromo &) {return(*this);}
  virtual ~RChromo(void);
};



//---------------------------------------------------------------------------
// RChromo classes codes
#include "rchromo.hh"


}//------- End of namespace RGA ---------------------------------------------
#endif
