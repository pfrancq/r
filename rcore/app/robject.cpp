/*

	R Project Library

	RObject.cpp

	Generic Object that can receive and send messages - Implementation.

	Copyright 2006-2015 by Pascal Francq (pascal@francq.info).
	Copyright 2006-2008 by the Université Libre de Bruxelles (ULB).

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
// include files for R Project
#include <robject.h>
#include <rnotificationcenter.h>
using namespace R;



//-----------------------------------------------------------------------------
//
// class RObject
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
RObject::RObject(const RString& name)
	: Handlers(0), Name(name)
{
}


//-----------------------------------------------------------------------------
int RObject::Compare(const RObject& obj) const
{
	return(ComparePtrs(this,&obj));
}


//-----------------------------------------------------------------------------
int RObject::Compare(const RObject* obj) const
{
	return(ComparePtrs(this,obj));
}


//-----------------------------------------------------------------------------
void RObject::HandlerNotFound(const RNotification& notification)
{
	std::cout<<Name<<" : Message '"<<notification.GetName()<<"' not treated."<<std::endl;
}


//-----------------------------------------------------------------------------
RObject::~RObject(void)
{
	NotificationCenter.DeleteObject(this);
}
