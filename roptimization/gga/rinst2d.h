/*

	Rainbow Library Project

  RInst2D.h

  Instance for 2D placement GA - Header

  (C) 1999-2000 by P. Francq.

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
#ifndef RInst2DH
#define RInst2DH


//---------------------------------------------------------------------------
// include files for Rainbow
#include "rga2d.h"
using namespace RGA;


//---------------------------------------------------------------------------
namespace RGA{
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
template<class cInfo>
	class RThreadData2D
{
public:
	unsigned int NbObjs;			// Nb Objects to place
	unsigned int *Order;			// Array
	unsigned int *InObj;			// Id of the object containing an object (Crossover & Mutation)
	cInfo **tmpInfos;					// Geometric information of the temporary objects (Crossover & Mutation)
	RObj2D **tmpObjs;					// Temporary Objects (Crossover & Mutation)

	RThreadData2D(unsigned int nbobjs);
	~RThreadData2D(void);
};


//---------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cInfo>
	class RInst2D	: public RInst<cInst,cChromo,cFit>
{
public:
	RObj2D **Objs;											// Objects to place (ref)
	unsigned int NbObjs;								// Nb Objects to place
	RThreadData2D<cInfo> *thDatas;			// Thread dependant data

  RInst2D(unsigned int popsize,RObj2D** objs,unsigned int nbobjs) throw(bad_alloc);
	virtual void Init(void) throw(bad_alloc);
  virtual ~RInst2D(void);
};


}//------- End of namespace RGA ---------------------------------------------



//---------------------------------------------------------------------------
#endif
