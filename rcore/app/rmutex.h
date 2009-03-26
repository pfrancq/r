/*

	R Project Library

	RMutex.h

	Mutex for synchronizing threads - Header.

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
#ifndef RMutex_H
#define RMutex_H


//------------------------------------------------------------------------------
// include files for POSIX PTHREAD
#include <pthread.h>
#include <rstd.h>


//-----------------------------------------------------------------------------
namespace R{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/**
* The RMutex class provides a representation for a mutex used for thread
* synchronisation.
*
* Lets suppose that two functions, func1 and func2, used by two different
* threads access the same variable (an account for example). A mutex can be
* used to ensure that, at one moment, only one thread can access the variable.
* @code
* double account=0.0;
* RMutex mutex;
*
* void func1(void)
* {
*    double act;
*    mutex.Lock();
*    act=account+10.0;
*    account=act;
*    mutex.UnLock();
* }
*
* void func2(void)
* {
*    double act;
*    mutex.Lock();
*    act=account-30.0;
*    account=act;
*    mutex.UnLock();
* }
* @endcode
* @author Pascal Francq
* @short Mutex
*/
class RMutex
{
	/**
	* Pointer to a mutex structure.
	*/
	pthread_mutex_t Mutex;

public:

	/**
	* Construct the mutex.
	*/
	RMutex(void);

	/**
	* Lock the mutex. If the mutex is already locked, the calling thread is
	*blocked.
	*/
	void Lock(void) {pthread_mutex_lock(&Mutex);}

	/**
	* Unlock the mutex. If threads are blocked because of this mutex, one of
	* them (depending of the priority schema) is wake up.
	*/
	void UnLock(void) {pthread_mutex_unlock(&Mutex);}

	/**
	* Try to lock the mutex. If the mutex is already locked, the method returns
	* also. This method must be used carefully.
	*/
	void TryLock(void) {pthread_mutex_trylock(&Mutex);}

	/**
	* Get the pthread pointer of the mutex.
	*/
	pthread_mutex_t* GetMutex(void) {return(&Mutex);}

	/**
	* Destruct the mutex. The behaviour of the program is unkwown if the mutex
	* is still locked.
	*/
	virtual ~RMutex(void);
};


//-----------------------------------------------------------------------------
/**
* The RSmartMutexPtr provides a smart pointer for a given mutex. It has the
* same role than the classic auto_ptr template of the standard C++ template
* library. In fact, when a mutex is locked in a function, it must be unlock at
* all closing point.
*
* Take the following example:
* @code
* RMutex mutex;
* int Func(void)
* {
*    mutex.Lock();
*    a=a+5;
*    if(a<25)
*    {
*       mutex.UnLock();
*       return(-1);
*    }
*    if(a<50)
*    {
*       mutex.UnLock();
*       return(1);
*    }
*    mutex.UnLock();
*    return(0);
* }
* @endcode
* Using the RSmartMutexPtr class, the code is less complex:
* @code
* RMutex mutex;
* int Func(void)
* {
*    RSmartMutexPtr m(&mutex);
*    a=a+5;
*    if(a<25)
*       return(-1);
*    if(a<50)
*       return(1);
*    return(0);
* }
* @endcode
* @author Pascal Francq
* @short Smart Mutex Pointer
*/
class RSmartMutexPtr
{
	/**
	* Pointer to the mutex.
	*/
	RMutex* Mutex;

public:

	/**
	* Construct the smart pointer, i.e. lock the mutex managed.
	* @param mutex           Pointer to the mutex.
	*/
	RSmartMutexPtr(RMutex* mutex) : Mutex(mutex) {Mutex->Lock();}

	/**
	* Get the mutex managed.
	*/
	RMutex* GetMutex(void) const {return(Mutex);}

	/**
	* Destruct the smart pointer, i.e. u,lock the mutex managed.
	*/
	~RSmartMutexPtr(void) {Mutex->UnLock();}
};


}  //-------- End of namespace R ----------------------------------------------


//-----------------------------------------------------------------------------
#endif
