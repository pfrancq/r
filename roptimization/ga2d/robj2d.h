/*

  RObj2D.h

  Object for 2D placement GA - Header

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
#ifndef RObj2DH
#define RObj2DH


//---------------------------------------------------------------------------
// Standard libraries includes
#include "rgeometry/polygons.h"
#include "rgeometry/rgeoinfo.h"
using namespace RGeometry;


//---------------------------------------------------------------------------
namespace RGA{
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
class RObj2D
{
public:
  char *PossOri,NbPossOri;
  RPolygon *Polygon;
  bool Deformable;            // If not deformable -> unique cGeoInfo for all chromosomes

  RObj2D(bool);
  virtual ~RObj2D(void);
};


}//------- End of namespace RGA ---------------------------------------------



//---------------------------------------------------------------------------
#endif
