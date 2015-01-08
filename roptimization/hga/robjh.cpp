/*

	R Project Library

	RObjH.cpp

	Object to place in a specific node - Implementation.

	Copyright 1998-2015 by Pascal Francq (pascal@francq.info).
	Copyright 1998-2008 by the Université Libre de Bruxelles (ULB).

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



//-----------------------------------------------------------------------------
// include files for RProject
#include <robjh.h>
using namespace R;



//------------------------------------------------------------------------------
//
// class RObjH
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
RObjH::RObjH(const size_t id,const RString& name,const size_t nb)
	: Id(id), Name(name), Attr(nb)
{
}


//------------------------------------------------------------------------------
RObjH::RObjH(const RObjH* obj)
	: Id(obj->Id), Name(obj->Name), Attr(obj->Attr)
{
}


//------------------------------------------------------------------------------
RString RObjH::GetName(void) const
{
	return(Name);
}

//------------------------------------------------------------------------------
bool RObjH::IsSame(const RObjH* obj) const
{
	return(Attr.IsSame(obj->GetAttr()));
}


//------------------------------------------------------------------------------
RObjH::~RObjH(void)
{
}
