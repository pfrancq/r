/*

	R Project Library

	RInstg.h

	Class representing an instance of a GGA - Header

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
#ifndef RInstgH
#define RInstgH


//-----------------------------------------------------------------------------
// include files for R Project
#include <rga/rinst.h>
using namespace RGA;
#include <rgga/robjg.h>
#include <rgga/rgroupingheuristic.h>
using namespace RGGA;


//-----------------------------------------------------------------------------
namespace RGGA{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/**
* This class represent "thread-dependent" data for the GGA.
* @author Pascal Francq
* @short GGA "thread-dependent" Data.
*/
template<class cInst,class cChromo,class cFit,class cThreadData,class cGroup,class cObj>
	class RThreadDataG : public RThreadData<cInst,cChromo>
{
public:
	
	/**
	* Construct the data.
	* @param data           Owner of the data.
	*/
	RThreadDataG(cInst *owner) throw(bad_alloc);

	/**
	* Destruct the data.
	*/
	virtual ~RThreadDataG(void);
	
	friend class RChromoG<cInst,cChromo,cFit,cThreadData,cGroup,cObj>;
};


//-----------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData,class cGroup,class cObj>
	class RInstG : public RInst<cInst,cChromo,cFit,cThreadData>
{
	cObj** Objects;
	unsigned int NbObjects;
	unsigned int MaxGroups;

public:

	/**
	* Construct the instance.
	* @param popsize        Size of the population.
	* @param objs           Objects to place in the tree.
	* @param nbobjs         Number of objects to place.
	* @param debug          Debugger.
	*/
	RInstG(unsigned int popsize,cObj** objs,unsigned int nbobjs,RDebug *debug=0) throw(bad_alloc);

	/**
	* Initialisation of the instance.
	*/
	virtual void Init(void) throw(bad_alloc);

	/**
	* Destruct the instance.
	*/
	virtual ~RInstG(void);

	friend class RChromoG<cInst,cChromo,cFit,cThreadData,cGroup,cObj>;
};


}//------- End of namespace RGGA ----------------------------------------------


//-----------------------------------------------------------------------------
#endif
