/*

	R Project Library

	RTrace.cpp

	Trace system - Implementation.

	Copyright 2001-2010 by Pascal Francq (pascal@francq.info).
	Copyright 2001-2008 by the Université Libre de Bruxelles (ULB).

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
#include <rtrace.h>
using namespace R;


//------------------------------------------------------------------------------
// Global variable
static RContainer<RTrace,true,false> Msgs(30,15);



//------------------------------------------------------------------------------
//
// class RTrace
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
RTrace::RTrace(const RString& msg)
	: Msg(msg)
{
}


//------------------------------------------------------------------------------
RTrace* RTrace::LookMsg(const RString& msg)
{
	return(Msgs.GetPtr(msg));
}


//------------------------------------------------------------------------------
RTrace* RTrace::InsertMsg(const RString& msg)
{
	RTrace* Msg=new RTrace(msg);
	Msgs.InsertPtr(Msg);
	return(Msg);
}


//------------------------------------------------------------------------------
void RTrace::DeleteMsg(RTrace* msg)
{
	Msgs.DeletePtr(*msg);
}


//------------------------------------------------------------------------------
RTrace::~RTrace(void)
{
}
