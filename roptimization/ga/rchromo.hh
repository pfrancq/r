/*

	R Project Library

	RChromo.hh

	Chromosomes of Genetic Algorithms - inline Implementation

	Copyright 1998-2008 by the Université Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).

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
//
// RChromo<cInst,cChromo,cFit,cThreadData>
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData>
	R::RChromo<cInst,cChromo,cFit,cThreadData>::RChromo(cInst *inst,unsigned int id)
		: Instance(inst),Id(id),ToEval(true)
{
	Fitness=new cFit();
}


//------------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData>
	void R::RChromo<cInst,cChromo,cFit,cThreadData>::Init(cThreadData*)
{
}


//------------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData>
	void R::RChromo<cInst,cChromo,cFit,cThreadData>::Crossover(cChromo*,cChromo*)
{
}


//------------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData>
  R::RChromo<cInst,cChromo,cFit,cThreadData>& R::RChromo<cInst,cChromo,cFit,cThreadData>::operator=(const RChromo &chromo)
{
	(*Fitness)=(*(chromo.Fitness));
	return(*this);
}


//------------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData>
	R::RChromo<cInst,cChromo,cFit,cThreadData>::~RChromo(void)
{
	if(Fitness) delete Fitness;
}
