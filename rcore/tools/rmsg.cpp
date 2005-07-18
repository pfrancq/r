/*

	R Project Library

	RMsg.cpp

	Message systems - Implementation.

	Copyright 2001-2003 by the Universit�Libre de Bruxelles.

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
#include <rmsg.h>
using namespace R;


//------------------------------------------------------------------------------
// Global variable
static RContainer<RMsg,true,false> Msgs(30,15);



//------------------------------------------------------------------------------
//
// class RMsg
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
RMsg::RMsg(const RString& msg)
	: Msg(msg)
{
}


//------------------------------------------------------------------------------
RMsg* RMsg::LookMsg(const RString& msg)
{
	return(Msgs.GetPtr(msg));
}


//------------------------------------------------------------------------------
RMsg* RMsg::InsertMsg(const RString& msg)
{
	RMsg* Msg=new RMsg(msg);
	Msgs.InsertPtr(Msg);
	return(Msg);
}


//------------------------------------------------------------------------------
void RMsg::DeleteMsg(RMsg* msg)
{
	Msgs.DeletePtr(*msg);
}


//------------------------------------------------------------------------------
RMsg::~RMsg(void)
{
}
