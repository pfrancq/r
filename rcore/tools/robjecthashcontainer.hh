/*

	R Project Library

	RObjectHashContainer.h

	Object Hash Container - Inline implementation.

	Copyright 2011-2015 by Pascal Francq (pascal@francq.info).

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
//
//  RObjectHashContainer::GObjectRef
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
template<class C,bool bAlloc>
	class RObjectHashContainer<C,bAlloc>::ObjectRef
{
public:
	C* Obj;

	ObjectRef(C* obj) : Obj(obj) {}
	int Compare(const ObjectRef& obj) const {return(Obj->Search(*obj.Obj));}
	int Compare(const C& obj) const {return(Obj->Search(obj));}
	size_t HashCode(size_t max) const {return(Obj->HashCode(max));}
	template<class TUse> int Compare(const TUse& tag) const {return(Obj->Search(tag));}
};



//-----------------------------------------------------------------------------
//
// class RObjectHashContainer
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
template<class C,bool bAlloc>
	RObjectHashContainer<C,bAlloc>::RObjectHashContainer(size_t first,size_t nb,size_t nbi,size_t s,size_t m,size_t i)
		: Objects(nb,nbi), ObjectsByRef(s,m,i), FirstId(first)
{
}


//-----------------------------------------------------------------------------
template<class C,bool bAlloc>
	void RObjectHashContainer<C,bAlloc>::VerifyTab(size_t max)
{
	Objects.VerifyTab(max);
}


//-----------------------------------------------------------------------------
template<class C,bool bAlloc>
	void RObjectHashContainer<C,bAlloc>::Clear(size_t m,size_t i)
{
	Objects.Clear(m,i);
	ObjectsByRef.Clear(m,i);
}


//------------------------------------------------------------------------------
template<class C,bool bAlloc>
	size_t RObjectHashContainer<C,bAlloc>::GetObjs(C** &tab,bool alloc,size_t min,size_t max)
{
	if(alloc)
		tab=new C*[Objects.GetMaxPos()+1];
	return(Objects.GetTab(tab,min,max));
}


//-------------------------------------------------------------------------------
template<class C,bool bAlloc>
	const C* RObjectHashContainer<C,bAlloc>::operator[](size_t id) const
{
	// Look first in memory
	if((Objects.GetNb())&&(id<=Objects.GetMaxPos()))
	{
		return(Objects[id]);
	}
	return(0);
}


//-------------------------------------------------------------------------------
template<class C,bool bAlloc>
	C* RObjectHashContainer<C,bAlloc>::operator[](size_t id)
{
	// Look first in memory
	if((Objects.GetNb())&&(id<=Objects.GetMaxPos()))
	{
		return(Objects[id]);
	}
	return(0);
}


//-------------------------------------------------------------------------------
template<class C,bool bAlloc>
	template<class TUse>
		size_t RObjectHashContainer<C,bAlloc>::GetIndex(const TUse& tag,size_t& hash,bool& find) const
{
	return(ObjectsByRef.GetIndex(tag,hash,find));
}


//-------------------------------------------------------------------------------
template<class C,bool bAlloc> template<class TUse>
	C* RObjectHashContainer<C,bAlloc>::GetObj(const TUse& tag,bool sortkey)
{
	ObjectRef* ref(ObjectsByRef.GetPtr(tag,sortkey));

	if(!ref)
		return(0);
	return(ref->Obj);
}


//-----------------------------------------------------------------------------
template<class C,bool bAlloc>
	bool RObjectHashContainer<C,bAlloc>::InsertObj(C* obj)
{
	if(!obj)
		throw std::invalid_argument("RObjectHashContainer<C,bAlloc>::InsertObj : Cannot insert a null pointer");

	bool NewOne(false); // Suppose it is not a new object

	// Test if the object has an identifier
	if(obj->GetId()==R::cNoRef)
	{
		// An identifier must be assigned
		AssignId(obj);
		NewOne=true;
	}

	// Insert the object
	Objects.InsertPtrAt(obj,obj->GetId());
	ObjectsByRef.InsertPtr(new ObjectRef(obj));

	// If it is a new one, it should perhaps be saved
	return(NewOne);
}


//-----------------------------------------------------------------------------
template<class C,bool bAlloc>
	bool RObjectHashContainer<C,bAlloc>::InsertObjAt(C* obj,size_t hash,size_t idx,bool del)
{
	if(!obj)
		throw std::invalid_argument("RObjectHashContainer<C,bAlloc>::InsertObj : Cannot insert a null pointer");

	bool NewOne(false); // Suppose it is not a new object

	// Test if the object has an identifier
	if(obj->GetId()==R::cNoRef)
	{
		// An identifier must be assigned
		AssignId(obj);
		NewOne=true;
	}

	// Insert the object
	Objects.InsertPtrAt(obj,obj->GetId());
	ObjectsByRef.InsertPtrAt(new ObjectRef(obj),hash,idx,del);

	// If it is a new one, it should perhaps be saved
	return(NewOne);
}


//------------------------------------------------------------------------------
template<class C,bool bAlloc>
	void RObjectHashContainer<C,bAlloc>::DeleteObj(C* obj)
{
	if(!obj)
		throw std::invalid_argument("RObjectHashContainer<C,bAlloc>::InsertObj : Cannot delete a null pointer");

	Objects.DeletePtr(*obj);
	ObjectsByRef.DeletePtr(*obj);
}


//------------------------------------------------------------------------------
template<class C,bool bAlloc>
	void RObjectHashContainer<C,bAlloc>::AssignId(C* obj)
{
	// The first object has the identifier 1
	if(Objects.GetNb())
		obj->SetId(Objects[Objects.GetMaxPos()]->GetId()+FirstId);
	else
		obj->SetId(1);
}



//-----------------------------------------------------------------------------
template<class C,bool bAlloc>
	RObjectHashContainer<C,bAlloc>::~RObjectHashContainer(void)
{
}
