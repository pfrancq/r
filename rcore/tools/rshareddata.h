/*

	R Project Library

	RSharedData.h

	Generic class representing shared data - Header.

	Copyright 2004-2012 by Pascal Francq (pascal@francq.info).
	Copyright 2004-2008 by the Université Libre de Bruxelles (ULB).

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
#ifndef RSharedData_H
#define RSharedData_H


//------------------------------------------------------------------------------
namespace R{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* This class implements a generic class representing data that will be shared
* between several object.
* @author Pascal Francq
* @short Generic Shared Data
*/
class RSharedData
{
	/**
	* Number of references of the data.
	*/
	size_t Refs;

public:

	/**
	* Constructor.
	*/
	RSharedData(void) : Refs(1) {}

	/**
	 * Copy constructor.
	 * @param data           Original data.
	 */
	RSharedData(const RSharedData& data);

	/**
	 * Assignment operator.
	 * @param data           Original data.
	 */
	RSharedData& operator=(const RSharedData& data);

	/**
	* Increment the number of references of the data.
	*/
	void IncRef(void) {Refs++;}

	/**
	* Decrement the number of references of the data.
	* return true if there is no more references to this data.
	*/
	bool DecRef(void) {return(!--Refs);}

	/**
	* Get the number of references to the data.
	*/
	size_t GetRefs(void) {return(Refs);}

	/**
	* Destruct.
	*/
	~RSharedData(void) {}
};


//------------------------------------------------------------------------------
// inline declarations here to avoid compiler complains about unused parameters
inline RSharedData::RSharedData(const RSharedData&) : Refs(1) {}
inline RSharedData& RSharedData::operator=(const RSharedData&) {return(*this);}


//------------------------------------------------------------------------------
/**
* Function the increase the number of references of a RSharedData object.
* @tparam D                 Type of the shared data.
* @param data               Pointer to the data.
*/
template <class D>
	inline D* RIncRef(D* data)
{
	if(data)
		data->IncRef();
	return(data);
}


//------------------------------------------------------------------------------
/**
* Function the decrease the number of references of a RSharedData object. If
* there are no more references, the data is destroy and the pointer is set to
* 0.
* @tparam D                 Type of the shared data.
* @param data               Reference to a pointer to the data.
*/
template<class D>
	inline void RDecRef(D* &data)
{
	if(data&&data->DecRef())
	{
		delete data;
		data = 0;
	}
}


//------------------------------------------------------------------------------
/**
 * The RSmartPtr represent a smart pointer of a given type which must inherits
 * from RSharedData.
 * @tparam C                 Class pointed.
 * It is an implementation of Scott Meyers proposition in "MORE EFFECTIVE C++".
 */
template<class C>
	class RSmartPtr
{
	/**
	 * Pointer.
	 */
	C* Ptr;

	/**
	 * Method called by all constructors.
	 */
	inline void Init(void) {RIncRef(Ptr);}

public:

	/**
	 * Default constructor.
	 */
	RSmartPtr(void) : Ptr(0) {}

	/**
	 * Constructor.
	 * @param ptr            Pointer to the object.
	 */
	RSmartPtr(C* ptr) : Ptr(ptr) {Init();}

	/**
	 * Copy constructor.
	 * @param ptr            Original smart pointer.
	 */
	RSmartPtr(const RSmartPtr& ptr) : Ptr(ptr.Ptr) {Init();}

	/**
	 * Assignment operator.
	 * @param ptr            Original smart pointer.
	 */
	RSmartPtr& operator=(const RSmartPtr& ptr)
	{
		if(Ptr!=ptr.Ptr)
		{
			C* OldPtr=Ptr;
            Ptr=ptr.Ptr;
            Init();
            RRefDec(OldPtr);
		}
		return(*this);
	}

	/**
	 * The -> operator.
	 */
	C* operator->() const {return(Ptr);}

	/**
	 * The * operator.
	 */
	C& operator*() const {return(*Ptr);}

	/**
	 * Destructor.
	 */
	~RSmartPtr(void) {RDecRef(Ptr);}
};


}  //-------- End of namespace R -----------------------------------------------


//------------------------------------------------------------------------------
#endif
