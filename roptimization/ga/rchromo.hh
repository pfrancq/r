/*

	Rainbow Library Project

	RChromo.hh

	Chromosomes of Genetic Algorithms - inline Implementation

	(C) 1998-2000 by P. Francq.

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
// RChromo<cInst,cChromo,cFit,cThreadData>
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData>
	RChromo<cInst,cChromo,cFit,cThreadData>::RChromo(cInst *inst,unsigned int id) throw(bad_alloc)
		: Instance(inst),Id(id),ToEval(true)
{
	Fitness=new cFit();
}


//-----------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData>
	void RChromo<cInst,cChromo,cFit,cThreadData>::Init(cThreadData*) throw(bad_alloc)
{
}


//-----------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData>
  RChromo<cInst,cChromo,cFit,cThreadData>& RChromo<cInst,cChromo,cFit,cThreadData>::
		operator=(const RChromo &chromo)
{
	Id=chromo.Id;
	(*Fitness)=(*(chromo.Fitness));
	return(*this);
}


//-----------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData>
	RChromo<cInst,cChromo,cFit,cThreadData>::~RChromo(void)
{
	if(Fitness) delete Fitness;
}
