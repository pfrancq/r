/*

	RInsth.hh

	Class representing an instance of a HGA - Implementation

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
//
// RThreadDataG<cInst,cChromo,cFit,cThreadData,cGroup,cObj>
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData,class cGroup,class cObj>
	RGGA::RThreadDataG<cInst,cChromo,cFit,cThreadData,cGroup,cObj>::RThreadDataG(cInst *owner) throw(bad_alloc)
		: RThreadData<cInst,cChromo>(owner)
{
}


//-----------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData,class cGroup,class cObj>
	RGGA::RThreadDataG<cInst,cChromo,cFit,cThreadData,cGroup,cObj>::~RThreadDataG(void)
{
}



//-----------------------------------------------------------------------------
//
// RInstG<cInst,cChromo,cFit,cThreadData,cGroup,cObj>
//
//-----------------------------------------------------------------------------

//---------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData,class cGroup,class cObj>
	RGGA::RInstG<cInst,cChromo,cFit,cThreadData,cGroup,cObj>::RInstG(unsigned int popsize,cObj** objs,unsigned int nbobjs,RDebug *debug) throw(bad_alloc)
		: RInst<cInst,cChromo,cFit,cThreadData>(popsize,debug)
{
	cObj **ptr;
	unsigned int tmp,tmp2,n;

	Objects=objs;
	MaxGroups=NbObjects=nbobjs;
}


//-----------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData,class cGroup,class cObj>
	void RGGA::RInstG<cInst,cChromo,cFit,cThreadData,cGroup,cObj>::Init(void) throw(bad_alloc)
{
	RInst<cInst,cChromo,cFit,cThreadData>::Init();
}


//---------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData,class cGroup,class cObj>
	RGGA::RInstG<cInst,cChromo,cFit,cThreadData,cGroup,cObj>::~RInstG(void)
{
}
