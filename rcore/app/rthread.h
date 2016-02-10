/*

	R Project Library

	RThread.h

	Thread - Header.

	Copyright 2006-2015 by Pascal Francq (pascal@francq.info).
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
#include <rrwlock.h>
#include <rcontainer.h>
#include <rboolvector.h>
#include <rboolcursor.h>
#include <rnumcontainer.h>


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
* An identifier and a name can be assigned to a thread. There have no management
* role. In particular, multiple threads may have the same identifier (a default
* null value is specified). The identifier may serve to ensure that each thread
* running the same portion of code uses it own data:
* @code
* class CallBy20Threads
* {
*   int Data[20];
* public:
*   CallBy20Threads(void) {}
*   void CallByThread(RThread* thread)
*   {
*     cout<<Data[thread->GetId()]<<endl;
*   }
* };
* @endcode
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
	 * Thread identifier.
    */
	size_t Id;

	/**
	* Internal method used by the PTHREAD library.
	*/
	static void* ThreadFunc(void*);

public:

	/**
	* Construct a thread.
	* @param id              Identifier of the thread.
	* @param name            Name of the thread.
	*/
	RThread(size_t id=0,const RString name=RString::Null);

	/**
	 * Get the identifier of the thread.
    * @return a size_t
    */
	size_t GetId(void) const {return(Id);}

	/**
	* Get the name of the thread.
	* @return a RString.
	*/
	RString GetName(void) const {return(Name);}

	/**
	* Start the execution of the thread.
	*/
	void Start(void);

	/**
	* Wait that the thread is finished. In practice, the calling thread is
	* blocked until the thread finishes its execution.
	*/
	void Wait(void);

private:

	/**
	* Method containing the code to be executed by the thread. It must be
	* overloaded by child classes.
	*/
	virtual void Run(void)=0;

public:

	/**
	* Destroy the thread. In fact, destroying the RThread class does not
	* terminate the execution of the thread.
	*/
	virtual ~RThread(void);
};


//-----------------------------------------------------------------------------
/**
 * The RThreadLocalStorage class provides local storage for threads. It is
 * sometimes necessary that each thread works on its own copy of a data
 * structure. Moreover, this data structure could be persistent, i.e. once a
 * thread uses the data structure, non other one should use it.
 *
 * When a thread needs a structure, it calls the
 * RThreadLocalStorage::Reserve method and passes an unique identifier that can
 * be its own identifier (remember that it is the responsibility of the
 * developer to ensure the unicity of the thread identifiers). A call to
 * RThreadLocalStorage::Release makes the data structure free again to be used
 * by another thread. The persistent of a data structure can be obtained by
 * never calling the RThreadLocalStorage::Release method.
 *
 * The RThreadLocalStorage class ensures that only the necessary amount of data
 * structures is allocated on the heap.
 * @short Thread Local Storage.
 */
template<class C>
	class RThreadLocalStorage
{
	/**
	 * Container of data structures.
	 */
	RContainer<C,true,false> Data;

	/**
	 * Vector that remember which data structures are actually used.
	 */
	RBoolVector DataUsed;

	/**
	 * Relation between an identifier and the index of the corresponding data
	 * structure used.
	 */
	RNumContainer<size_t,false> Index;

	/**
	 * Read/Write lock.
	 */
	RRWLock lData;

public:

	/**
	 * Constructor.
    * @param size           Initial size of the internal containers. It is set
	 *                       to 20 by default.
    */
	RThreadLocalStorage(size_t size=20) : Data(size,10), DataUsed(0,20), Index(size)
	{
		Index.Init(size,cNoRef);
	}

	/**
	 * Method that allocates a new data structure. This method can overloaded
	 * to eventually call more complex constructors.
    * @return a pointer to a new allocated data structure.
    */
	virtual C* New(void) {return(new C());}

	/**
	 * Reserve a data structure.
    * @param id             Unique identifier that will be associated to the
	 *                       data structure.
    * @return a pointer to a data structure.
    */
	C* Reserve(size_t id)
	{
		lData.ReadLock();
		if(id<Index.GetNb())
		{
			size_t Idx(Index[id]);
			if(Idx!=cNoRef)
			{
				lData.ReadUnLock();
				return(Data[Idx]);
			}
		}
		lData.ReadUnLock();

		// Lock for writing purposes
		lData.WriteLock();

		// Add necessary identifiers
		while(Index.GetNb()<=id)
			Index.Insert(cNoRef);

		// Find an unused data structure
		size_t Idx;
		C* ptr;
		RBoolCursor Cur(DataUsed);
		for(Cur.Start();(!Cur.End())&&Cur();Cur.Next());
		if(Cur.End())
		{
			// Create new structure
			ptr=New();
			Idx=Data.GetNb();
			Data.InsertPtrAt(ptr,Idx);
			DataUsed.ReSize(Data.GetNb());
		}
		else
		{
			Idx=Cur.GetPos();
			ptr=Data[Idx];
		}
		Index[id]=Idx;
		DataUsed[Idx]=true;
		lData.WriteUnLock();
		return(ptr);
	}

	/**
	 * Get a data structure. If no data structure is reserved, an exception is
	 * generated or a null pointer is returned.
	 * @param id             Identifier that has reserved the data structure.
	 * @param null           Return a null pointer or generate an exception ?
	 * @return a pointer to a data structure.
	 */
	C* Get(size_t id,bool null=false)
	{
		lData.ReadLock();
		if(id<Index.GetNb())
		{
			size_t Idx(Index[id]);
			if(Idx!=cNoRef)
			{
				lData.ReadUnLock();
				return(Data[Idx]);
			}
		}
		lData.ReadUnLock();
		if(null)
			return(0);
		mThrowRException("No reserved data for identifier "+RString::Number(id));
	}

	/**
	 * Get a data structure (const version). If no data structure is reserved, an
	 * exception is generated or a null pointer is returned.
	 * @param id             Identifier that has reserved the data structure.
	 * @param null           Return a null pointer or generate an exception ?
	 * @return a pointer to a data structure.
	 */
	const C* Get(size_t id,bool null=false) const
	{
		const_cast<RThreadLocalStorage*>(this)->lData.ReadLock();
		if(id<Index.GetNb())
		{
			size_t Idx(Index[id]);
			if(Idx!=cNoRef)
			{
				const_cast<RThreadLocalStorage*>(this)->lData.ReadUnLock();
				return(Data[Idx]);
			}
		}
		const_cast<RThreadLocalStorage*>(this)->lData.ReadUnLock();
		if(null)
			return(0);
		mThrowRException("No reserved data for identifier "+RString::Number(id));
	}

	/**
	 * Release a data structure.
    * @param id             Identifier that has reserved the data structure.
    */
	void Release(size_t id)
	{
		lData.ReadLock();
		if(id>=Index.GetNb())
		{
			lData.ReadUnLock();
			mThrowRException("No reserved data for identifier "+RString::Number(id));
		}
		size_t Idx(Index[id]);
		if(Idx==cNoRef)
		{
			lData.ReadUnLock();
			mThrowRException("No reserved data for identifier "+RString::Number(id));
		}
		lData.ReadUnLock();
		lData.WriteLock();
		DataUsed[Idx]=false;
		Index[id]=cNoRef;
		lData.WriteUnLock();
	}
};


}  //-------- End of namespace R ----------------------------------------------


//-----------------------------------------------------------------------------
#endif
