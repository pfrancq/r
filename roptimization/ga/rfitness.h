/*

  RFitness.h

  Fitness for Chromosomes of Genetic Algorithms - Header

  (C) 1998-2000 by P. Francq.

  Version $Revision$

  Last Modify: $Date$

  GA.cpp  : Code of this module
  GA.hh   : Implementation of templates

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
#ifndef RFitnessH
#define RFitnessH


//---------------------------------------------------------------------------
namespace RGA{
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
template<class cVal,bool Max> class RFitness
{
public:
  cVal Value;

  RFitness(void);
  inline RFitness& operator=(const RFitness &);
  inline bool operator==(const RFitness &);
  inline bool operator!=(const RFitness &);
  inline bool operator>(const RFitness &);
  inline bool operator<(const RFitness &);
};


//---------------------------------------------------------------------------
// rrlGA classes codes
#include "rfitness.hh"


}//------- End of namespace RGA ---------------------------------------------
#endif
