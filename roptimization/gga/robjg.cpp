/*

	R Project Library

	RObjG.cpp

	Objects to Group - Implementation.

	Copyright 2001-2015 by Pascal Francq (pascal@francq.info).
	Copyright 2001-2008 by the Universit√© Libre de Bruxelles (ULB).

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
#include <robjg.h>
using namespace R;



//------------------------------------------------------------------------------
//
// class RObjG
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
RObjG::RObjG(const size_t id,const RString& name)
	: Id(id), Name(name)
{
}


//------------------------------------------------------------------------------
int RObjG::Compare(const size_t id) const
{
	return(CompareIds(Id,id));
}


//------------------------------------------------------------------------------
int RObjG::Compare(const RObjG& obj) const
{
  return(CompareIds(Id,obj.Id));
}


//------------------------------------------------------------------------------
int RObjG::Compare(const RObjG* obj) const
{
  return(CompareIds(Id,obj->Id));
}


//------------------------------------------------------------------------------
RString RObjG::GetName(void) const
{
	return(Name);
}


//------------------------------------------------------------------------------
RObjG::~RObjG(void)
{
}
