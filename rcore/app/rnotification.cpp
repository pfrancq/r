/*

	R Project Library

	RNotification.cpp

	Generic message - Implementation.

	Copyright 2006 by Pascal Francq.

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



//-----------------------------------------------------------------------------
// include files for R Project
#include <rnotification.h>
using namespace R;



//-----------------------------------------------------------------------------
//
// class RNotification
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// internal constant used
unsigned int const shift=6;
unsigned int const mask = ~0U << (sizeof(unsigned int)*8-shift);


//------------------------------------------------------------------------------
tNotificationType RNotification::GetType(const RCString& name)
{
	const char* ptr=name();
	unsigned int res=0;

	for(;*ptr;ptr++)
		res= (res & mask) ^ (res<<shift) ^ (*ptr);
	return(res);
}


