/*

	R Project Library

	RMsg.cpp

	Message systems - Implementation.

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
// include files for R Project
#include <rstd/rmsg.h>
using namespace RStd;



//-----------------------------------------------------------------------------
//
// class RMsg
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
RStd::RMsg::RMsg(const RString& msg)
	: Msg(msg)
{
}


//-----------------------------------------------------------------------------
RStd::RMsg::RMsg(const char* msg)
	: Msg(msg)
{
}


//-----------------------------------------------------------------------------
RStd::RMsg::~RMsg(void)
{
}



//-----------------------------------------------------------------------------
//
// General Functions
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
RContainer<RMsg,unsigned int,true,false> Msgs(30,15);


//-----------------------------------------------------------------------------
RMsg* RStd::LookMsg(const RString& msg)
{
	return(Msgs.GetPtr<const RString>(msg));
}


//-----------------------------------------------------------------------------
RMsg* RStd::LookMsg(const char* msg)
{
	return(Msgs.GetPtr<const char*>(msg));
}


//-----------------------------------------------------------------------------
RMsg* RStd::InsertMsg(const RString& msg) throw(bad_alloc)
{
	RMsg* Msg=new RMsg(msg);
	Msgs.InsertPtr(Msg);
	return(Msg);
}


//-----------------------------------------------------------------------------
RMsg* RStd::InsertMsg(const char* msg) throw(bad_alloc)
{
	RMsg* Msg=new RMsg(msg);
	Msgs.InsertPtr(Msg);
	return(Msg);
}


//-----------------------------------------------------------------------------
void RStd::DeleteMsg(RMsg* msg) throw(bad_alloc)
{
	Msgs.DeletePtr(msg);
}
