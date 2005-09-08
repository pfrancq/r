/*

	Rainbow Library Project

	RDblHashContainer.h

	Double Hash Table Container - Header

	Copyright 2001-2005 by the Université Libre de Bruxelles.

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
#ifndef RDblHashContainerH
#define RDblHashContainerH


//------------------------------------------------------------------------------
// include files for R Project
#include <rcontainer.h>
#include <rcursor.h>


//------------------------------------------------------------------------------
namespace R{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* @param C                  The class of the element to be contained.
* @param tSize1             First Size of the hash table.
* @param tSize2             Second Size of the hash table.
* @param bAlloc             Specify if the elements are desallocated by the
*                           container.
* This class represent a container of elements (class C) with a double hash
* table.
*
* To make the necessary comparaisons, the container used member functions of
* the class representing the elements (class C). These functions have the
* signature:
* @code
* int Compare(const TUse& tag) const;
* int Compare(const TUse* tag) const;
* static int HashIndex(const TUse& tag);
* static int HashIndex(const TUse* tag);
* static int HashIndex2(const TUse& tag);
* static int HashIndex2(const TUse* tag);
* @endcode
*
* The TUse represent a class or a structure used for the comparaisons. The
* Compare methods are working like the strcmp function from the standard C/C++
* library. The result returned specifies if the tag preceeds (>0), is the
* same (0) or is after (<0) the element used. The HashIndex methods return the
* hash index of the given argument for the first hash table. The HashIndex2
* methods return the hash index of the given argument for the Second hash
* table.
*
* At least, a compare function and a HashIndex and HashIndex2 method must be
* implemented in the class C:
* @code
* int Compare(const C&) const;
* static int HashIndex(const C&);
* static int HashIndex2(const C&);
* @endcode
*
* Here is an example of class MyElement that will be contained in the
* variable c:
* @code
* #include <string.h>
* #include <lhashcontainer.h>
* using namespace R;
*
*
* class MyElement
* {
* 	char* Text;
* public:
* 	MyElement(const char* text) {Text=strdup(text);}
* 	MyElement(const MyElement& e) {Text=strdup(e.Text);}
* 	int Compare(const MyElement& e) const {return(strcmp(Text,e.Text));}
* 	int Compare(const char* text) const {return(strcmp(Text,text));}
*	~MyElement(void) {free(Text);}
* 	static int HashIndex(const MyElement& e)
* 	{
* 		int c=(*e.Text);
* 		if(c>='a'&&c<='z') return(c-'a');
* 		if(c>='A'&&c<='Z') return(c-'A');
* 		return(26);
* 	}
* 	static int HashIndex2(const MyElement& e)
* 	{
* 		int c=(*(e.Text+1));
* 		if(c>='a'&&c<='z') return(c-'a');
* 		if(c>='A'&&c<='Z') return(c-'A');
* 		return(26);
* 	}
* 	static int HashIndex(const char *u)
* 	{
* 		int c=*u;
* 		if(c>='a'&&c<='z') return(c-'a');
* 		if(c>='A'&&c<='Z') return(c-'A');
* 		return(26);
* 	}
* 	static int HashIndex2(const char *u)
* 	{
* 		int c=*(u+1);
* 		if(c>='a'&&c<='z') return(c-'a');
* 		if(c>='A'&&c<='Z') return(c-'A');
* 		return(26);
* 	}
* };
*
*
* int main()
* {
* 	RDblHashContainer<MyElement,27,27,true> c(20,10);
*
* 	c.InsertPtr(new MyElement("Hello World"));
* 	if(c.IsIn<const char*>("Hello World"))
* 		cout<<"An element of value \"Hello World\" is in the container"<<endl;
* 	c.InsertPtr(new MyElement("Other"));
* }
* @endcode
*
* @author Pascal Francq
* @short Double Hash Table Container.
*/
template<class C,size_t tSize1,size_t tSize2,bool bAlloc>
	class RDblHashContainer
{
public:

	class Hash;

	/**
	* Class representing the second index in a double hash table container.
	*/
	class Hash2 : public RContainer<C,bAlloc,true>
	{
		/*
		* Constructor.
		* @param m               The initial maximal size of the array.
		* @param i               The value used when increasing the array. If
		*                        null value, the size is set to the half the
		*                        maximal size.
		*/
		Hash2(size_t m,size_t i) : RContainer<C,bAlloc,true>(m,i) {}

		friend class RDblHashContainer;
		friend class Hash;
	};

	/**
	* Class representing the first index in a double hash table container.
	*/
	class Hash : public RContainer<Hash2,true,true>
	{
		/*
		* Constructor.
		* @param m               The initial maximal size of the array for a
		*                        pair of index.
		* @param i               The value used when increasing the array for a
		*                        pair of index. If null value, the size is set
		*                        to the half the maximal size.
		*/
		Hash(size_t m,size_t i) : RContainer<Hash2,true,true>(tSize2)
		{
			for(size_t pos=0;pos<tSize2;pos++)
				InsertPtrAt(new Hash2(m,i),pos);
		}

		/*
		* Get the number of elements in the container.
		*/
		size_t GetNb(void) const
		{
			RCursor<Hash2> Cur(*this);
			size_t nb;
			for(Cur.Start(),nb=0;!Cur.End();Cur.Next())
				nb+=Cur()->GetNb();
			return(nb);
		}

		/*
		* Clear the container.
		*/
		void Clear(void)
		{
			RCursor<Hash2> Cur(*this);
			for(Cur.Start();!Cur.End();Cur.Next())
				Cur()->Clear();
		}

		friend class RDblHashContainer;
	};

private:

	/**
	* This container represents the hash table of the elements.
	*/
	RContainer<Hash,true,true> HashTable;

public:

	/**
	* Construct a Hash container.
	* @param m               The initial maximal size of the array for a pair
	*                        of index.
	* @param i               The value used when increasing the array for a pair
	*                        of index. If null value, the size is set to the
	*                        half the maximal size.
	*/
	RDblHashContainer(size_t m,size_t i=0)
		: HashTable(tSize1)
	{
		for(size_t pos=0;pos<tSize1;pos++)
			HashTable.InsertPtrAt(new Hash(m,i),pos);
	}

	/**
	* Get the number of elements in the hash container.
	*/
	size_t GetNb(void) const
	{
		RCursor<Hash> Cur(HashTable);
		size_t nb;
		for(Cur.Start(),nb=0;!Cur.End();Cur.Next())
			nb+=Cur()->GetNb();
		return(nb);
	}

	/**
	* Get a cursor over the main hash table.
	*/
	RCursor<Hash> GetCursor(void) const
	{
		return(RCursor<Hash>(HashTable));
	}

	/**
	* Get a pointer to the ith element in the container (Only read).
	* @param idx             Index of the element to get.
	* @return Return the pointer or 0 if the index is outside the scope of the
	*         container.
	*/
	const Hash* operator[](size_t idx) const {return(HashTable[idx]);}

	/**
	* Get a pointer to the ith element in the container (Read/Write).
	* @param idx             Index of the element to get.
	* @return Return the pointer or 0 if the index is outside the scope of the
	*         container.
	*/
	Hash* operator[](size_t idx) {return(HashTable[idx]);}

	/**
	* Clear the hash container.
	*/
	void Clear(void)
	{
		RCursor<Hash> Cur(HashTable);
		for(Cur.Start();!Cur.End();Cur.Next())
			Cur()->Clear();
	}

	/**
	* Verify if an element is in the hash container.
	* @param TUse           The type of tag, the hash container uses the
	*                       Compare(TUse &) member function of the elements.
	* @param tag            The tag used.
	* @param sortkey        The tag represents the sorting key. The default value
	*                       depends if the container is ordered (true) or not
	*                       (false).
	* @returns The function returns true if the element is in the hash
	* container, else false.
	*/
	template<class TUse> inline bool IsIn(const TUse tag,bool sortkey=true) const
	{
		size_t hash1=C::HashIndex(tag);
		if(hash1>=tSize1)
			throw RException("Invalid first hash index");
		size_t hash2=C::HashIndex2(tag);
		if(hash2>=tSize2)
			throw RException("Invalid second hash index");
		return((*HashTable[hash1])[hash2]->IsIn<TUse>(tag,sortkey));
	}

	/**
	* Get a pointer to a certain element in the hash container.
	* @param TUse           The type of tag, the hash container uses the
	*                       Compare(TUse &) member function of the elements.
	* @param tag            The tag used.
	* @param sortkey        The tag represents the sorting key. The default value
	*                       depends if the container is ordered (true) or not
	*                       (false).
	* @return Return the pointer or 0 if the element is not in the container.
	*/
	template<class TUse> inline C* GetPtr(const TUse tag,bool sortkey=true) const
	{
		size_t hash1=C::HashIndex(tag);
		if(hash1>=tSize1)
			throw RException("Invalid first hash index");
		size_t hash2=C::HashIndex2(tag);
		if(hash2>=tSize2)
			throw RException("Invalid second hash index");
		return((*HashTable[hash1])[hash2]->GetPtr<TUse>(tag,sortkey));
	}

	/**
	* Get a pointer to a certain element in the container. If the element is
	* not existing, the container creates it by using the constructor with TUse
	* as parameter.
	* @param TUse           The type of tag, the container uses the
	*                       Compare(TUse &) member function of the elements.
	* @param tag            The tag used.
	* @param sortkey        The tag represents the sorting key. The default
	*                       value depends if the container is ordered (true) or
	*                       not (false).
	* @return The function returns a pointer to the element of the container.
	*/
	template<class TUse> inline C* GetInsertPtr(const TUse tag,bool sortkey=true)
	{
		size_t hash1=C::HashIndex(tag);
		if(hash1>=tSize1)
			throw RException("Invalid first hash index");
		size_t hash2=C::HashIndex2(tag);
		if(hash2>=tSize2)
			throw RException("Invalid second hash index");
		return((*HashTable[hash1])[hash2]->GetInsertPtr<TUse>(tag,sortkey));
	}

	/**
	* Insert an element in the container. If the corresponding index is already
	* used, the previously inserted element is removed from the container (and
	* destroy if the container is responsible for the allocation).
	* @param ins             A pointer to the element to insert.
	* @param del             Specify if a similar existing element must be
	*                        deleted.
	*/
	inline void InsertPtr(const C* ins,bool del=false)
	{
		RReturnIfFail(ins);
		size_t hash1=C::HashIndex(*ins);
		if(hash1>=tSize1)
			throw RException("Invalid first hash index");
		size_t hash2=C::HashIndex2(*ins);
		if(hash2>=tSize2)
			throw RException("Invalid second hash index");
		(*HashTable[hash1])[hash2]->InsertPtr(ins,del);
	}

	/**
	* Delete an element of the hash container.
	* @param TUse           The type of tag, the container uses the
	*                       Compare(TUse &) member function of the elements.
	* @param tag            The tag used.
	* @param sortkey        The tag represents the sorting key. The default
	*                       value depends if the container is ordered (true) or
	*                       not (false).
	*/
	template<class TUse> inline void DeletePtr(const TUse& tag,bool sortkey=true)
	{
		size_t hash1=C::HashIndex(tag);
		if(hash1>=tSize1)
			throw RException("Invalid first hash index");
		size_t hash2=C::HashIndex2(tag);
		if(hash2>=tSize2)
			throw RException("Invalid second hash index");
		(*HashTable[hash1])[hash2]->DeletePtr<TUse>(tag,sortkey);
	}

	/**
	* Destruct the hash container.
	*/
	virtual ~RDblHashContainer(void) {}
};


}  //-------- End of namespace R -----------------------------------------------


//------------------------------------------------------------------------------
#endif
