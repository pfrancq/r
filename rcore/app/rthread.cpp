/*

	R Project Library

	RThread.cpp

	Thread - Implementation.

	Copyright 2006 by the Université libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).

	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU Library General Public
	License as published by the Free Software Foundation; either
	version 2.0 of the License, or (at your option) any later version.

	This library is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
	Library General Public License for more details.

	You should have received a copy of the GNU Library General Public
	License along with this library, as a file COPYING.LIB; if not, write
	to the Free Software Foundation, Inc., 59 Temple Place, Suite 330,
	Boston, MA  02111-1307  USA

*/



//------------------------------------------------------------------------------
// include files for R Project
#include <rthread.h>
using namespace R;



//------------------------------------------------------------------------------
//
// class RThread
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void* RThread::ThreadFunc(void* p)
{
	static_cast<RThread*>(p)->Run(); return(0);
}


//------------------------------------------------------------------------------
RThread::RThread(const RString name)
	: Thread(), Name(name)
{
}


//-----------------------------------------------------------------------------
void RThread::Start(void)
{
	pthread_create(&Thread,NULL,RThread::ThreadFunc,(void *)this);
}


//-----------------------------------------------------------------------------
void RThread::Wait(void)
{
	pthread_join(Thread,NULL);
}


//-----------------------------------------------------------------------------
RString RThread::GetName(void) const
{
	return(Name);
}


//-----------------------------------------------------------------------------
RThread::~RThread(void)
{
}
