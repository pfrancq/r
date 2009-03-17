/*

	R Project Library

	RSGObj.cpp

	Similarity-based Clustering Object - Implementation.

	Copyright 2002-2009 by the Université Libre de Bruxelles.

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
#include <robjsc.h>
using namespace R;



//------------------------------------------------------------------------------
//
// class RObjSC::CloseObj
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
int RObjSC::CloseObj::sortOrder(const void* a,const void* b)
{
	double af=(*((CloseObj**)(a)))->Ratio;
	double bf=(*((CloseObj**)(b)))->Ratio;

	if(af==bf) return(0);
	if(af>bf)
		return(-1);
	else
		return(1);
}



//------------------------------------------------------------------------------
//
// class RObjSC
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
RObjSC::RObjSC(const size_t id,const RString& name,bool social,size_t parentid)
	: RObjG(id,name), CloseObjs(10), Social(social), ParentId(parentid), MustReOrder(false)
{
}


//------------------------------------------------------------------------------
void RObjSC::AddCloseObject(size_t id,double ratio)
{
	CloseObjs.InsertPtr(new CloseObj(id,ratio));
	MustReOrder=true;
}


//------------------------------------------------------------------------------
RObjSC::~RObjSC(void)
{
}
