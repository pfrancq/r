/*

	R Project Library

	RConditionVar.h

	Conditional variables for synchronizing threads - Header.

	Copyright 2006-2009 by Pascal Francq (pascal@francq.info).
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
#ifndef RConditionVar_H
#define RConditionVar_H


//------------------------------------------------------------------------------
// include files for POSIX PTHREAD
#include <pthread.h>


//-----------------------------------------------------------------------------
namespace R{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// forward declaration
class RMutex;


//-----------------------------------------------------------------------------
/**
* The RConditionVar class provides a representation of a conditional variable
* used for thread synchronization.
*
* Lets suppose that a function func1 (of a given thread) is waiting that some
* kind of event occurs (for example in a func2 of a second thread). A
* conditional variable can be used to block to first thread as long as the
* event occurs.
* @code
* RConditionVar cond;
*
* void func1(void)
* {
*    // Wait that a conditional variable corresponds to a given event
*    cond.Wait();
*    // The thread can work now
* }
*
* void func2(void)
* {
*    // Do something
*    cond.WakeUpOne();
*    // Continue to work
* }
* @endcode
* @author Pascal Francq
* @short Conditional Variable
*/
class RConditionVar
{
	/**
	* The conditional variable structure.
	*/
	pthread_cond_t Cond;

public:

	/**
	* Construct a conditional variable.
	*/
	RConditionVar(void);

	/**
	* Wait that the conditional variable corresponds to a event.
	* @param mutex           Mutex associated with the conditional variable.
	* @param time            Number of milliseconds to wait for the conditional
	*                        variable. If null, the thread waits forever.
	*/
	void Wait(RMutex* mutex,unsigned long time=0);

	/**
	* Wait that the conditional variable corresponds to a event.
	* @param time            Number of milliseconds to wait for the conditional
	*                        variable. If null, the thread waits forever.
	*/
	void Wait(unsigned long time=0);

	/**
	* Wake up one of the blocked thread by this conditional variable (depending
	* of the priority schema).
	*/
	void WakeUpOne(void) {pthread_cond_signal(&Cond);}

	/**
	* Wake up all the blocked thread by this conditional vairbale.
	*/
	void WakeUpAll(void) {pthread_cond_broadcast(&Cond);}

	/**
	* Destruct the conditional variable.
	*/
	virtual ~RConditionVar(void);
};


}  //-------- End of namespace R ----------------------------------------------


//-----------------------------------------------------------------------------
#endif
