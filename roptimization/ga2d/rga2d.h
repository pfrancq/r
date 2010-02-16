/*

	R Project Library

	RGA2D.h

	2D Placement Genetic Algorithm - Header

	Copyright 1998-2010 by Pascal Francq (pascal@francq.info).
	Copyright 1998-2008 by the Université Libre de Bruxelles (ULB).

	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU Library General Public
	License as published by the Free Software Foundation; either
	version 2.0 of the License, or (at your option) any later version.

	This library is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
	Library General Public License for more details.

	You should have received a copy of the GNU Library General Public
	License along with this library, as a file COPYING.LIB; if not, write
	to the Free Software Foundation, Inc., 59 Temple Place, Suite 330,
	Boston, MA  02111-1307  USA

*/



//------------------------------------------------------------------------------
#ifndef RGA2D_H
#define RGA2D_H


//------------------------------------------------------------------------------
// include files for R Project
#include <rga.h>
#include <rstring.h>
#include <rpoint.h>
#include <rsize.h>
#include <rrects.h>
#include <rpolygons.h>
#include <rpromkernel.h>


//------------------------------------------------------------------------------
namespace R{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// Forward class declaration
class RGeoInfo;
class RLayout;
class RObj2D;
class RObj2DContainer;
class RObj2DConnector;
class RObj2DPin;
class RConnection;
class RObj2DConfig;
class RObj2DConfigPin;
template<class cInst,class cChromo>	class RThreadData2D;
template<class cInst,class cChromo,class cFit,class cThreaData,class cInfo> class RInst2D;
template<class cInst,class cChromo,class cFit,class cThreaData,class cInfo> class RChromo2D;


}//------- End of namespace R --------------------------------------------------


//------------------------------------------------------------------------------
#endif
