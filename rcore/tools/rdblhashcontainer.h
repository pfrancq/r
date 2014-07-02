/*

	Rainbow Library Project

	RDblHashContainer.h

	Double Hash Table Container - Header

	Copyright 2001-2012 by Pascal Francq (pascal@francq.info).
	Copyright 2001-2008 by the Université Libre de Bruxelles (ULB).

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
* This class represent a container of elements (class C) with a double hash
* table. The container can be responsible for the deallocation of the elements
* (bAlloc).
* @tparam C                 The class of the element to be contained.
* @tparam bAlloc            Specify if the elements are deallocated by the
*                           container.
*
* To make the necessary comparisons, the container uses member functions of
* the class representing the elements (class C). These functions have the
* signature:
* @code
* int Compare(const TUse& tag) const;
* int Compare(const TUse* tag) const;
* size_t HashIndex(size_t idx) const;
* @endcode
*
* The TUse represent a class or a structure used for the comparisons. The
* Compare methods are working like the strcmp function from the standard C/C++
* library. The result returned specifies if the tag precedes (>0), is the
* same (0) or is after (<0) the element used. The HashIndex methods return the
* hash index of the given argument for the first hash table (idx=1) or for the
* second hash table (idx=2).
*
* At least, a compare function and a HashIndex method must be implemented in the
*  class C:
* @code
* int Compare(const C&) const;
* size_t HashIndex(size_t idx) const;
* @endcode
*
* Here is an example of class MyElement that will be contained in the
* variable c:
* @code
* #include <string.h>
* #include <rdblhashcontainer.h>
* #include <rcursor.h>
* using namespace R;
*
*
* class MyElement
* {
*    char* Text;
* public:
*    MyElement(const char* text) {Text=strdup(text);}
*    MyElement(const MyElement& e) {Text=strdup(e.Text);}
*    int Compare(const MyElement& e) const {return(strcmp(Text,e.Text));}
*    int Compare(const char* text) const {return(strcmp(Text,text));}
*    size_t HashIndex(size_t idx) const
*    {
*       if(strlen(Text)<idx)
*          return(26);
*       int c=tolower(Text[idx]);
*       if(c>='a'&&c<='z') return(c-'a');
*       return(26);
*    }
*    void DoSomething(void) {cout<<Text<<endl;}
*    ~MyElement(void) {free(Text);}
* };
*
*
* int main()
* {
*    RDblHashContainer<MyElement,true> c(27,27,20,10);
*
*    c.InsertPtr(new MyElement("Hello World"));
*    if(c.IsIn<const char*>("Hello World"))
*       cout<<"An element of value \"Hello World\" is in the container"<<endl;
*    c.InsertPtr(new MyElement("Other"));
*
*    // Parse the double hash table
*    RCursor<RDblHashContainer<MyElement,true>::Hash> Cur(c.GetCursor());
*    for(Cur.Start();!Cur.End();Cur.Next())
*    {
*       RCursor<RDblHashContainer<MyElement,true>::Hash2> Cur2(*Cur());
*       for(Cur2.Start();!Cur2.End();Cur2.Next())
*       {
*          RCursor<MyElement> Cur3(*Cur2());
*          for(Cur3.Start();!Cur3.End();Cur3.Next())
*             Cur3()->DoSomething();
*      }
*    }
* }
* @endcode
*
* @author Pascal Francq
* @short Double Hash Table Container.
*/
template<class C,bool bAlloc>
	class RDblHashContainer
{
public:

	class Hash;

	/**
	* Class representing the second index in a double hash table container.
	* Here is an example of class MyElement that will be contained in the
	* variable c:
	* @code
	* #include <string.h>
	* #include <rdblhashcontainer.h>
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
	*    size_t HashIndex(size_t idx) const
	*    {
	*       if(strlen(Text)<idx)
	*          return(26);
	*       int c=tolower(Text[idx]);
	*       if(c>='a'&&c<='z') return(c-'a');
	*       return(26);
	*    }
	* 	void DoSomething(void) {cout<<Text<<endl;}
	*	~MyElement(void) {free(Text);}
	* };
	*
	*
	* int main()
	* {
	* 	RDblHashContainer<MyElement,true> c(27,27,20,10);
	*
	* 	c.InsertPtr(new MyElement("Hello World"));
	* 	if(c.IsIn<const char*>("Hello World"))
	* 		cout<<"An element of value \"Hello World\" is in the container"<<endl;
	* 	c.InsertPtr(new MyElement("Other"));
	*
	*	// Parse the double hash table
	*	RCursor<RDblHashContainer<MyElement,true>::Hash> Cur(c.GetCursor());
	*	for(Cur.Start();!Cur.End();Cur.Next())
	*	{
	*		RCursor<RDblHashContainer<MyElement,true>::Hash2> Cur2(*Cur());
	*		for(Cur2.Start();!Cur2.End();Cur2.Next())
	*		{
	*			RCursor<MyElement> Cur3(*Cur2());
	*			for(Cur3.Start();!Cur3.End();Cur3.Next())
	*				Cur3()->DoSomething();
	*		}
	*	}
	* }
	* @endcode
	* @short Second Index Container.
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

	public:

		/**
		 * Compare method.
		 */
		int Compare(const Hash2&) const {return(-1);}

		friend class RDblHashContainer;
		friend class Hash;
	};

	/**
	* Class representing the first index in a double hash table container.
	* Here is an example of class MyElement that will be contained in the
	* variable c:
	* @code
	* #include <string.h>
	* #include <rdblhashcontainer.h>
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
	*    size_t HashIndex(size_t idx) const
	*    {
	*       if(strlen(Text)<idx)
	*          return(26);
	*       int c=tolower(Text[idx]);
	*       if(c>='a'&&c<='z') return(c-'a');
	*       return(26);
	*    }
	* 	void DoSomething(void) {cout<<Text<<endl;}
	*	~MyElement(void) {free(Text);}
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
	*
	*	// Parse the double hash table
	*	RCursor<RDblHashContainer<MyElement,true>::Hash> Cur(c.GetCursor());
	*	for(Cur.Start();!Cur.End();Cur.Next())
	*	{
	*		RCursor<RDblHashContainer<MyElement,true>::Hash2> Cur2(*Cur());
	*		for(Cur2.Start();!Cur2.End();Cur2.Next())
	*		{
	*			RCursor<MyElement> Cur3(*Cur2());
	*			for(Cur3.Start();!Cur3.End();Cur3.Next())
	*				Cur3()->DoSomething();
	*		}
	*	}
	* }
	* @endcode
	* @short First Index Container.
	*/
	class Hash : public RContainer<Hash2,true,true>
	{
		using RContainer<Hash2,true,true>::Tab;
		using RContainer<Hash2,true,true>::MaxPtr;
		using RContainer<Hash2,true,true>::InsertPtrAt;

		/**
		* Constructor.
		* @param s               Size of the initial hash table.
		* @param m               The initial maximal size of the array for a
		*                        pair of index.
		* @param i               The value used when increasing the array for a
		*                        pair of index. If null value, the size is set
		*                        to the half the maximal size.
		*/
		Hash(size_t s,size_t m,size_t i) : RContainer<Hash2,true,true>(s)
		{
			for(size_t pos=0;pos<s;pos++)
				InsertPtrAt(new Hash2(m,i),pos);
		}

	public:

		/**
		 * Compare method.
		 */
		int Compare(const Hash&) const {return(-1);}

		/**
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

		/**
		* Clear the container.
		*/
		void Clear(void)
		{
			RCursor<Hash2> Cur(*this);
			for(Cur.Start();!Cur.End();Cur.Next())
				Cur()->Clear();
		}

		/**
		 * Get a pointer to the hash table.
		 * @param hash           Hash index 1.
		 */
		const Hash2* GetHash(size_t hash) const
		{
			const Hash2* ptr;
			if((hash>MaxPtr)||(!(ptr=static_cast<Hash2*>(Tab[hash]))))
				return(0);
			return(ptr);
		}

		/**
		 * Get a pointer to the hash table.
		 * @param hash           Hash index 1.
		 * @param m              Maximum.
		 * @param i              Increase.
		 */
		Hash2* GetHash(size_t hash,size_t m,size_t i)
		{
			Hash2* ptr;
			if((hash>MaxPtr)||(!(ptr=static_cast<Hash2*>(Tab[hash]))))
				InsertPtrAt(ptr=new Hash2(m,i),hash);
			return(ptr);
		}

	public:

		/**
		* Get a pointer to the ith element in the container (Only read).
		* @param idx             Index of the element to get.
		* @return Return the pointer or 0 if the index is outside the scope of the
		*         container.
		*/
		const Hash2* operator[](size_t idx) const {return(RContainer<Hash2,true,true>::operator[](idx));}

		/**
		* Get a pointer to the ith element in the container (Only read).
		* @param idx             Index of the element to get.
		* @return Return the pointer or 0 if the index is outside the scope of the
		*         container.
		*/
		Hash2* operator[](size_t idx) {return(RContainer<Hash2,true,true>::operator[](idx));}

		friend class RDblHashContainer;
	};

private:

	/**
	* This container represents the hash table of the elements.
	*/
	RContainer<Hash,true,true> HashTable;

	/**
	 * Initial size of the second hash index.
	 */
	size_t Size;

	/**
	 * Maximum size of last container.
	 */
	size_t Max;

	/**
	 * Incremental size of last container.
	 */
	size_t Inc;

public:

	/**
	* Construct a Hash container.
	* @param s1              Size of the initial hash table.
	* @param s2              Size of the second hash table.
	* @param m               The initial maximal size of the array for a pair
	*                        of index.
	* @param i               The value used when increasing the array for a pair
	*                        of index. If null value, the size is set to the
	*                        half the maximal size.
	*/
	RDblHashContainer(size_t s1,size_t s2,size_t m,size_t i=0)
		: HashTable(s1), Size(s2), Max(m), Inc(i)
	{
		for(size_t pos=0;pos<s1;pos++)
			HashTable.InsertPtrAt(new Hash(Size,Max,Inc),pos);
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

private:

	/**
	 * Get a pointer to the hash table.
	 * @param hash           Hash index.
	 */
	const Hash* GetHash(size_t hash) const
	{
		const Hash* ptr;
		if((hash>HashTable.GetMaxPos())||(!(ptr=HashTable[hash])))
			return(0);
		return(ptr);
	}

	/**
	 * Get a pointer to the hash table.
	 * @param hash           Hash index.
	 */
	Hash* GetHash(size_t hash)
	{
		Hash* ptr;
		if((hash>HashTable.GetMaxPos())||(!(ptr=HashTable[hash])))
			HashTable.InsertPtrAt(ptr=new Hash(Size,Max,Inc),hash);
		return(ptr);
	}

public:

	/**
	* Verify if an element is in the hash container.
	* @tparam TUse          The type of tag, the hash container uses the
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
		const Hash* ptr=GetHash(tag.HashIndex(1));
		if(!ptr)
			return(false);
		const Hash2* ptr2=ptr->GetHash(tag.HashIndex(2));
		if(!ptr2)
			return(false);
		return(ptr2->IsIn<TUse>(tag,sortkey));
	}

	/**
	* Get a pointer to a certain element in the hash container.
	* @tparam TUse          The type of tag, the hash container uses the
	*                       Compare(TUse &) member function of the elements.
	* @param tag            The tag used.
	* @param sortkey        The tag represents the sorting key. The default value
	*                       depends if the container is ordered (true) or not
	*                       (false).
	* @return Return the pointer or 0 if the element is not in the container.
	*/
	template<class TUse> inline C* GetPtr(const TUse tag,bool sortkey=true) const
	{
		size_t hash1(tag.HashIndex(1));
		const Hash* ptr=GetHash(hash1);
		if(!ptr)
			return(0);
		size_t hash2(tag.HashIndex(2));
		const Hash2* ptr2=ptr->GetHash(hash2);
		if(!ptr2)
			return(0);
		return(ptr2->GetPtr<TUse>(tag,sortkey));
	}

	/**
	* Get a pointer to a certain element in the container. If the element is
	* not existing, the container creates it by using the constructor with TUse
	* as parameter.
	* @tparam TUse          The type of tag, the container uses the
	*                       Compare(TUse &) member function of the elements.
	* @param tag            The tag used.
	* @param sortkey        The tag represents the sorting key. The default
	*                       value depends if the container is ordered (true) or
	*                       not (false).
	* @return The function returns a pointer to the element of the container.
	*/
	template<class TUse> inline C* GetInsertPtr(const TUse tag,bool sortkey=true)
	{
		Hash* ptr=GetHash(tag.HashIndex(1));
		Hash2* ptr2=ptr->GetHash(tag.HashIndex(2),Max,Inc);
		return(ptr2->GetInsertPtr<TUse>(tag,sortkey));
	}

	/**
	* Insert an element in the container. If the corresponding index is already
	* used, the previously inserted element is removed from the container (and
	* destroy if the container is responsible for the allocation).
	* @param ins             A pointer to the element to insert.
	* @param del             Specify if a similar existing element must be
	*                        deleted.
	*/
	inline void InsertPtr(C* ins,bool del=false)
	{
		mReturnIfFail(ins);
		Hash* ptr=GetHash(ins->HashIndex(1));
		Hash2* ptr2=ptr->GetHash(ins->HashIndex(2),Max,Inc);
		ptr2->InsertPtr(ins,del);
	}

	/**
	* Delete an element of the hash container.
	* @tparam TUse          The type of tag, the container uses the
	*                       Compare(TUse &) member function of the elements.
	* @param tag            The tag used.
	* @param sortkey        The tag represents the sorting key. The default
	*                       value depends if the container is ordered (true) or
	*                       not (false).
	* @param del             Specify if the object must deleted or not. By
	*                        default, the element is destruct if the container
	*                        is responsible of the deallocation.
	*/
	template<class TUse> inline void DeletePtr(const TUse& tag,bool sortkey=true,bool del=bAlloc)
	{
		Hash* ptr=GetHash(tag.HashIndex(1));
		if(!ptr)
			return;
		Hash2* ptr2=ptr->GetHash(tag.HashIndex(2),Max,Inc);
		if(!ptr2)
			return;
		ptr2->DeletePtr<TUse>(tag,sortkey,del);
	}

	/**
	* Destruct the hash container.
	*/
	virtual ~RDblHashContainer(void) {}
};


}  //-------- End of namespace R -----------------------------------------------


//------------------------------------------------------------------------------
#endif
