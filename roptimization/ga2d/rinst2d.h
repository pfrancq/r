/*

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

*/


//---------------------------------------------------------------------------
#ifndef RInst2DH
#define RInst2DH


//---------------------------------------------------------------------------
// Standard libraries includes
#include "rga.h"
#include "rga2d.h"
#include "robj2d.h"
using namespace RGA;
#include "rgeometry/polygons.h"
#include "rgeometry/rgeoinfo.h"
using namespace RGeometry;


//---------------------------------------------------------------------------
namespace RGA{
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit>
	class RInst2D	: public RInst<cInst,cChromo,cFit>
{
public:
	RObj2D **Objs;	
	
  RInst2D(unsigned popsize,RObj2D** objs) throw(bad_alloc);
  virtual ~RInst2D(void);

};



//---------------------------------------------------------------------------
// RInst2D codes
#include "rinst2d.hh"


}//------- End of namespace RGA ---------------------------------------------



//---------------------------------------------------------------------------
#endif
