/*

  robjh.cpp

  Description - Implementation.

  (c) 2001 by P. Francq.

  Version $Revision$

  Last Modify: $Date$

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
#include <rgga/robjg.h>
using namespace RGGA;



//-----------------------------------------------------------------------------
//
// class RObjG
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
RObjG::RObjG(const unsigned int id,const RString& name)
	: Id(id), Name(name)
{
}


//-----------------------------------------------------------------------------
RObjG::RObjG(const RObjG* obj)
	: Id(obj->Id), Name(obj->Name)
{
}


//-----------------------------------------------------------------------------
RString& RObjG::GetName(void) const
{
	RString *str=RString::GetString();

	(*str)=Name;
	return(*str);
}


//-----------------------------------------------------------------------------
RObjG::~RObjG(void)
{
}
