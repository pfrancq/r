/*

	R Project Library

	RRWLock.cpp

	Reader/Writer Lock - Implementation.

	Copyright 2015-2015 by Pascal Francq (pascal@francq.info).

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
#include <rrwlock.h>
#include <rexception.h>
using namespace R;



//------------------------------------------------------------------------------
//
// class RRWLock
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
RRWLock::RRWLock(void)
	: NbReaders(0), NbWriters(0)
{
	pthread_mutex_init(&Mutex,NULL);
	pthread_cond_init(&Cond,NULL);
}


//------------------------------------------------------------------------------
void RRWLock::ReadLock(void)
{
	pthread_mutex_lock(&Mutex);
	while(NbWriters)
	{
		pthread_cond_wait(&Cond,&Mutex);
	}
	NbReaders++;
	pthread_mutex_unlock(&Mutex);
}


//------------------------------------------------------------------------------
void RRWLock::ReadUnLock(void)
{
	pthread_mutex_lock(&Mutex);
	if(!NbReaders)
	{
		pthread_mutex_unlock(&Mutex);
		mThrowRException("Invalid read unlocking");
	}
	NbReaders--;
	if(!NbReaders)
		pthread_cond_signal(&Cond);
	pthread_mutex_unlock(&Mutex);
}


//------------------------------------------------------------------------------
void RRWLock::WriteLock(void)
{
	pthread_mutex_lock(&Mutex);
	while(NbWriters||NbReaders)
	{
		pthread_cond_wait(&Cond,&Mutex);
	}
	NbWriters++;
	pthread_mutex_unlock(&Mutex);
}


//------------------------------------------------------------------------------
void RRWLock::WriteUnLock(void)
{
	pthread_mutex_lock(&Mutex);
	if(!NbWriters)
	{
		pthread_mutex_unlock(&Mutex);
		mThrowRException("Invalid write unlocking");
	}
	NbWriters=0;  // That can be only thread writing at a given time
	pthread_cond_broadcast(&Cond);
	pthread_mutex_unlock(&Mutex);
}


//------------------------------------------------------------------------------
RRWLock::~RRWLock(void)
{
	pthread_mutex_destroy(&Mutex);
	int status=pthread_cond_destroy(&Cond);
	if(status)
	{
		// A problem occurs -> wakeup all blocked thread.
		pthread_cond_broadcast(&Cond);
	}
}
