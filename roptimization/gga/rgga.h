/*

	R Project Library

	RGGA.h

	Grouping Genetic Algorithms - Header.

	(C) 2001 by P. Francq.

	Version $Revision$

	Last Modify: $Date$

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



//-----------------------------------------------------------------------------
#ifndef RGGAH
#define RGGAH


//-----------------------------------------------------------------------------
// include files for RProject
#include <rga/rga.h>
using namespace RGA;


//-----------------------------------------------------------------------------
// Forward class declaration
namespace RGGA
{
	class RObjG;
	template<class cInst,class cChromo,class cFit,class cThreadData,class cGroup,class cObj> class RGroup;
	template<class cInst,class cChromo,class cFit,class cThreadData,class cGroup,class cObj> class RThreadDataG;
	template<class cInst,class cChromo,class cFit,class cThreadData,class cGroup,class cObj> class RChromoG;
	template<class cInst,class cChromo,class cFit,class cThreadData,class cGroup,class cObj> class RInstG;
}


//-----------------------------------------------------------------------------
// include files for GGA
#include <rgga/robjg.h>
#include <rgga/rgroup.h>
#include <rgga/rinstg.h>
#include <rgga/rchromog.h>
using namespace RGGA;


//-----------------------------------------------------------------------------
namespace RGGA{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Definitions of templates
#include <rgga/rgroup.hh>
#include <rgga/rchromog.hh>
#include <rgga/rinstg.hh>


}//------- End of namespace RGGA ----------------------------------------------


//-----------------------------------------------------------------------------
#endif
