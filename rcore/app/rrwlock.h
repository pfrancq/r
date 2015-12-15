/*

	R Project Library

	RRWLock.h

	Reader/Writer Lock - Header.

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



//-----------------------------------------------------------------------------
#ifndef RRWLock_H
#define RRWLock_H


//------------------------------------------------------------------------------
// include files for POSIX PTHREAD
#include <pthread.h>
#include <rstd.h>


//-----------------------------------------------------------------------------
namespace R{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/**
 * The RRWLock class provides a reader/writer lock for synchronising threads
 * that access the same data structure when write operations should only be
 * allowed when no thread is reading some data.
 *
 * Lets suppose that two functions, Print and Insert, used by two
 * different threads access the same variable (a container for example).
* @code
* R::RContainer<R::RString,true,true> Cont(20);
* RRWLock lock;
*
* void Print(int idx)
* {
*    lock.ReadLock();
*	  std::cout<<*Cont[idx]<<std::endl;
*    lock.ReadUnLock();
* }
*
* void Insert(const R::RString& str)
* {
*    lock.WriteLock();
*    Cont.InsertPtr(new R::RString(str));
*    lock.WriteUnLock();
* }
* @endcode
* @short Reader/Writer Lock
*/
class RRWLock
{
	/**
	 * Number of readers.
	 */
	size_t NbReaders;

	/**
	 * Number of readers.
	 */
	size_t NbWriters;

	/**
	* Pointer to a mutex structure.
	*/
	pthread_mutex_t Mutex;

	/**
	 * Pointer to a conditional variable.
	 */
	pthread_cond_t Cond;

public:

	/**
	* Construct the lock.
	*/
	RRWLock(void);

	/**
	* Read locking.
	*/
	void ReadLock(void);

	/**
	* Read unlocking.
	*/
	void ReadUnLock(void);

	/**
	* Write locking.
	*/
	void WriteLock(void);

	/**
	* Write unlocking.
	*/
	void WriteUnLock(void);

	/**
	* Destruct the lock.
	*/
	virtual ~RRWLock(void);
};


//-----------------------------------------------------------------------------
/**
* The RSmartRLockPtr provides a smart pointer for a lock in read mode. It has
* the same role than the classic auto_ptr template of the standard C++ template
* library. In fact, when a read lock is acquired in a function, it must be
* unlock at all closing point.
*
* Take the following example:
* @code
* RRWLock lTest;
* int Func(void)
* {
*    lTest.ReadLock();
*    size_t z(a+5);
*    if(z<25)
*    {
*       lTest.ReadUnLock();
*       return(-1);
*    }
*    if(z<50)
*    {
*       lTest.ReadUnLock();
*       return(1);
*    }
*    lTest.ReadUnLock();
*    return(0);
* }
* @endcode
* Using the RSmartRLockPtr class, the code is less complex:
* @code
* RRWLock lTest;
* int Func(void)
* {
*    RSmartRLockPtr m(&lTest);
*    a=a+5;
*    if(a<25)
*       return(-1);
*    if(a<50)
*       return(1);
*    return(0);
* }
* @endcode
* @short Smart Read Lock Pointer
*/
class RSmartRLockPtr
{
	/**
	* Pointer to the Read/Write lock.
	*/
	RRWLock* Lock;

public:

	/**
	* Construct the smart pointer, i.e. lock in read mode.
	* @param lock            Pointer to the read/write lock.
	*/
	RSmartRLockPtr(RRWLock* lock) : Lock(lock) {Lock->ReadLock();}

	/**
	* Get the read/write lock managed.
	*/
	RRWLock* GetLock(void) const {return(Lock);}

	/**
	* Destruct the smart pointer, i.e.  release the read lock.
	*/
	~RSmartRLockPtr(void) {Lock->ReadUnLock();}
};


//-----------------------------------------------------------------------------
/**
* The RSmartWLockPtr provides a smart pointer for a lock in write mode. It has
* the same role than the classic auto_ptr template of the standard C++ template
* library. In fact, when a write lock is acquired in a function, it must be
* unlock at all closing point.
*
* Take the following example:
* @code
* RRWLock lTest;
* int Func(void)
* {
*    lTest.WriteLock();
*    a=a+5;
*    if(a<25)
*    {
*       lTest.WriteUnLock();
*       return(-1);
*    }
*    if(a<50)
*    {
*       lTest.WriteUnLock();
*       return(1);
*    }
*    lTest.WriteUnLock();
*    return(0);
* }
* @endcode
* Using the RSmartWLockPtr class, the code is less complex:
* @code
* RRWLock lTest;
* int Func(void)
* {
*    RSmartWLockPtr m(&lTest);
*    a=a+5;
*    if(a<25)
*       return(-1);
*    if(a<50)
*       return(1);
*    return(0);
* }
* @endcode
* @short Smart Write Lock Pointer
*/
class RSmartWLockPtr
{
	/**
	* Pointer to the Read/Write lock.
	*/
	RRWLock* Lock;

public:

	/**
	* Construct the smart pointer, i.e. lock in write mode.
	* @param lock            Pointer to the read/write lock.
	*/
	RSmartWLockPtr(RRWLock* lock) : Lock(lock) {Lock->WriteLock();}

	/**
	* Get the read/write lock managed.
	*/
	RRWLock* GetLock(void) const {return(Lock);}

	/**
	* Destruct the smart pointer, i.e. release the write lock.
	*/
	~RSmartWLockPtr(void) {Lock->WriteUnLock();}
};


}  //-------- End of namespace R ----------------------------------------------


//-----------------------------------------------------------------------------
#endif
