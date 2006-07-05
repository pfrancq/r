/*

	R Project Library

	RConditionVar.cpp

	Conditional variables for synchronizing threads - Implementation.

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
// include files for ANSI C/C++
#include <sys/time.h>


//------------------------------------------------------------------------------
// include files for R Project
#include <rconditionvar.h>
#include <rmutex.h>
using namespace R;



//------------------------------------------------------------------------------
//
// class RConditionVar
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
RConditionVar::RConditionVar(void)
{
	pthread_cond_init(&Cond,NULL);
}


//------------------------------------------------------------------------------
void RConditionVar::Wait(RMutex* mutex,unsigned long time)
{
	if(time)
	{
		// Get the current time
		struct timeval tv;
		gettimeofday(&tv, 0);

		// Add it the number of milliseconds defined by the function
		timespec ti;
		ti.tv_nsec=(tv.tv_usec+(time%1000)*1000)* 1000;
		ti.tv_sec=tv.tv_sec+(time/1000)+(ti.tv_nsec/1000000000);
		ti.tv_nsec%=1000000000;
		pthread_cond_timedwait(&Cond,mutex->GetMutex(),&ti);
	}
	else
		pthread_cond_wait(&Cond,mutex->GetMutex());
}


//------------------------------------------------------------------------------
void RConditionVar::Wait(unsigned long time)
{
	// A imaginary mutex must be created and lock because pthread_cond_* methods need one
	static pthread_mutex_t Mutex = PTHREAD_MUTEX_INITIALIZER;
	pthread_mutex_lock(&Mutex);

	if(time)
	{
		// Get the current time
		struct timeval tv;
		gettimeofday(&tv, 0);

		// Add it the number of milliseconds defined by the function
		timespec ti;
		ti.tv_nsec=(tv.tv_usec+(time%1000)*1000)* 1000;
		ti.tv_sec=tv.tv_sec+(time/1000)+(ti.tv_nsec/1000000000);
		ti.tv_nsec%=1000000000;
		pthread_cond_timedwait(&Cond,&Mutex,&ti);
	}
	else
		pthread_cond_wait(&Cond,&Mutex);
	pthread_mutex_unlock(&Mutex);
}


//------------------------------------------------------------------------------
RConditionVar::~RConditionVar(void)
{
	int status=pthread_cond_destroy(&Cond);
	if(status)
	{
		// A problem occurs -> wakeup all blocked thread.
		pthread_cond_broadcast(&Cond);
	}
}
