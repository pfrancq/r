/*

  RChromo2D.h

  Chromosome for 2D placement GA - Header

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
#ifndef RChromo2DH
#define RChromo2DH


//---------------------------------------------------------------------------
// Standard libraries includes
#include "rga.h"
#include "rga2d.h"
using namespace RGA;
#include "rgeometry/polygons.h"
#include "rgeometry/rgeoinfo.h"
using namespace RGeometry;


//---------------------------------------------------------------------------
namespace RGA{
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cInfo>
	class RChromo2D : public RChromo<cInst,cChromo,cFit>
{
public:
  cInfo **Infos;

  RChromo2D(cInst *inst,unsigned id) throw(bad_alloc);
  virtual bool RandomConstruct(void);
  virtual ~RChromo2D(void);
};


//---------------------------------------------------------------------------
// RChromo2D codes
#include "rchromo2d.hh"


}//------- End of namespace RGA ---------------------------------------------



//---------------------------------------------------------------------------
#endif
