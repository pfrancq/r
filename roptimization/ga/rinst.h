/*

	Rainbow Library Project

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
#ifndef RInstH
#define RInstH


//---------------------------------------------------------------------------
// include files for ANSI C/C++
#include <stdlib.h>


//---------------------------------------------------------------------------
// include files for Rainbow
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
  unsigned int PopSize;
  unsigned int Gen;
  unsigned int AgeBest;

  RInst(unsigned int popsize) throw(bad_alloc);
	virtual void Init(void) throw(bad_alloc);
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
  static int sort_function_cChromosome( const void *a, const void *b);
  cChromo **Parents,**Childs,**tmpChrom;
};


}//------- End of namespace RGA ---------------------------------------------
#endif
