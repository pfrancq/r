/*

	R Project Library

	HGA.h

	Hierarchical Genetic Algorithms - Header.

	Copyright 1998-2015 by Pascal Francq (pascal@francq.info).
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
#ifndef RHGA_H
#define RHGA_H


//------------------------------------------------------------------------------
// include files for R Project
#include <rga.h>


//------------------------------------------------------------------------------
namespace R{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// Forward class declaration
class RAttrList;
class RObjH;
template<class cNode,class cObj,class cNodes> class RTreeHeuristic;
template<class cNode,class cObj,class cNodes> class RFirstNodeHeuristic;
template<class cNode,class cObj,class cNodes> class RNodeGA;
template<class cNode,class cObj,class cNodes> class RNodesGA;
template<class cInst,class cChromo,class cFit,class cThreadData,class cNode,class cObj> class RThreadDataH;
template<class cInst,class cChromo,class cFit,class cThreadData,class cNode,class cObj> class RChromoH;
template<class cInst,class cChromo,class cFit,class cThreadData,class cNode,class cObj> class RInstH;


}//------- End of namespace R --------------------------------------------------


//------------------------------------------------------------------------------
#endif
