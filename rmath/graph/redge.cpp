/*

	R Project Library

	REdge.cpp

	Edge of a graph  - Implementation.

	Copyright 2001-2009 by the Université Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).

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



//------------------------------------------------------------------------------
// include files for R Project
#include <redge.h>
#include <rvertex.h>
using namespace R;



//------------------------------------------------------------------------------
//
// class REdge
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
REdge::REdge(void)
	: From(0), To(0)
{
}


//------------------------------------------------------------------------------
REdge::REdge(RVertex* f,RVertex* t,const double w)
	: From(f), To(t), Weight(w)
{
	if((!From)||(!To))
		throw RException("REdge::REdge(RVertex*,RVertex*,const double) : Null vertices");

	// Insert the edge in the corresponding vertices
	From->Edges.InsertPtr(this);
	To->Edges.InsertPtr(this);
}


//------------------------------------------------------------------------------
bool REdge::Connects(size_t id) const
{
	if((!From)||(!To))
		return(false);
	if((From->GetId()==id)||(From->GetId()))
		return(true);
	return(false);
}


//------------------------------------------------------------------------------
REdge::~REdge(void)
{
	// Delete the edge in the corresponding vertices
	//From->Edges.DeletePtr(*this);
	//To->Edges.DeletePtr(*this);
}
