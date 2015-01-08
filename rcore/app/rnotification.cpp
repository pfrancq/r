/*

	R Project Library

	RNotification.cpp

	Generic message - Implementation.

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
#include <rnotification.h>
#include <rnotificationcenter.h>
using namespace R;



//-----------------------------------------------------------------------------
//
// class RNotification
//
//-----------------------------------------------------------------------------

//------------------------------------------------------------------------------
RNotification::RNotification(const RCString& name,RObject* sender)
	: Handle(NotificationCenter.GetNotificationHandle(name)), Sender(sender), Receiver(0)
{
}


//------------------------------------------------------------------------------
RNotification::RNotification(const hNotification handle,RObject* sender)
	: Handle(handle), Sender(sender), Receiver(0)
{
}


//------------------------------------------------------------------------------
RCString RNotification::GetName(void) const
{
	return(NotificationCenter.GetNotificationName(Handle));
}


//------------------------------------------------------------------------------
RNotification::~RNotification(void)
{
}
