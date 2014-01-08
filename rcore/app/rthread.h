/*

	R Project Library

	RThread.h

	Thread - Header.

	Copyright 2006-2014 by Pascal Francq (pascal@francq.info).
	Copyright 2006-2008 by the Université Libre de Bruxelles (ULB).

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



//-----------------------------------------------------------------------------
#ifndef RThread_H
#define RThread_H


//------------------------------------------------------------------------------
// include files for POSIX PTHREAD
#include <pthread.h>


//-----------------------------------------------------------------------------
// include files for R Project
#include <rstring.h>


//-----------------------------------------------------------------------------
namespace R{
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/**
* The RThread class provides a representation for a thread. The method Run must
* be overloaded and contains the code that has to be runned by the thread.
* @code
* class MyThread : public RThread
* {
* public:
*    MyTread(const RString name=RString::Null) : RThread(name) {}
*    virtual void Run(void) {for(int i=0;i<100;i++) cout<<i<<endl;}
* };
*
* MyTread a,b;
* a.Start();
* b.Start();
* a.Wait();
* b.Wait();
* @endcode
* @author Pascal Francq
* @short Thread.
*/
class RThread
{
	/**
	* The thread structure.
	*/
	pthread_t Thread;

	/**
	* Name of the thread.
	*/
	RString Name;

	/**
	* Internal method used by the PTHREAD library.
	*/
	static void* ThreadFunc(void*);

public:

	/**
	* Construct a thread.
	* @param name            Name of the thread.
	*/
	RThread(const RString name=RString::Null);

	/**
	* Start the execution of the thread.
	*/
	void Start(void);

	/**
	* Wait that the thread is finished. In practice, the calling thread is
	* blocked until the thread finishes its execution.
	*/
	void Wait(void);

	/**
	* Get the name of the thread.
	*/
	RString GetName(void) const;

protected:

	/**
	* Method containing the code to be executed by the thread. It must be
	* overloaded by child classes.
	*/
	virtual void Run(void)=0;

public:

	/**
	* Destroy the thread. In fact, the destoying the RThread class does not
	* terminate the execution of the thread.
	*/
	virtual ~RThread(void);
};


}  //-------- End of namespace R ----------------------------------------------


//-----------------------------------------------------------------------------
#endif
