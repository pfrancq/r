/*

	Rainbow Library Project

	RHashContainer.h

	Single Hash Table Container - Header

	Copyright 2000-2007 by the Université Libre de Bruxelles.

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
#ifndef RHashContainerH
#define RHashContainerH


//------------------------------------------------------------------------------
// include files for R Project
#include <rcontainer.h>


//------------------------------------------------------------------------------
namespace R{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* This class represent a container of elements (class C) with a hash table.
* @param C                  The class of the element to be contained.
* @param bAlloc             Specify if the elements are desallocated by the
*                           container.
* To make the necessary comparaisons, the container uses member functions of
* the class representing the elements (class C). These functions have the
* signature:
* @code
* int Compare(const TUse& tag) const;
* int Compare(const TUse* tag) const;
* size_t HashIndex(size_t idx) const;
* @endcode
*
* The TUse represent a class or a structure used for the comparaisons. The
* Compare methods are working like the strcmp function from the standard C/C++
* library. The result returned specifies if the tag preceeds (>0), is the
* same (0) or is after (<0) the element used. The HashIndex methods return the
* hash index of the given argument.
*
* At least, a compare function and a HashIndex method must be implemented in
* the class C:
* @code
* int Compare(const C&) const;
* size_t HashIndex(size_t idx) const;
* @endcode
*
* Here is an example of class MyElement that will be contained in the
* variable c:
* @code
* #include <string.h>
* #include <rhashcontainer.h>
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
*    RHashContainer<MyElement,27,true> c(20,10);
*
*    c.InsertPtr(new MyElement("Hello World"));
*    if(c.IsIn<const char*>("Hello World"))
*       cout<<"An element of value \"Hello World\" is in the container"<<endl;
*    c.InsertPtr(new MyElement("Other"));
*
*    // Parse the hash table
*    RCursor<RHashContainer<MyElement,27,true>::Hash> Cur(c.GetCursor());
*    for(Cur.Start();!Cur.End();Cur.Next())
*    {
*       RCursor<MyElement> Cur2(*Cur());
*       for(Cur2.Start();!Cur2.End();Cur2.Next())
*          Cur2()->DoSomething();
*    }
* }
* @endcode
*
* @author Pascal Francq
* @short Single Hash Table Container.
*/
template<class C,bool bAlloc>
	class RHashContainer
{
public:

	/**
	* Class representing the index in a hash table container.
	* Here is an example of class MyElement that will be contained in the
	* variable c:
	* @code
	* #include <string.h>
	* #include <rhashcontainer.h>
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
	* 	RHashContainer<MyElement,true> c(27,20,10);
	*
	* 	c.InsertPtr(new MyElement("Hello World"));
	* 	if(c.IsIn<const char*>("Hello World"))
	* 		cout<<"An element of value \"Hello World\" is in the container"<<endl;
	* 	c.InsertPtr(new MyElement("Other"));
	*
	*	// Parse the hash table
	*	RCursor<RHashContainer<MyElement,true>::Hash> Cur(c.GetCursor());
	*	for(Cur.Start();!Cur.End();Cur.Next())
	*	{
	*		RCursor<MyElement> Cur2(*Cur());
	*		for(Cur2.Start();!Cur2.End();Cur2.Next())
	*			Cur2()->DoSomething();
	*	}
	* }
	* @endcode
	*/
	class Hash : public RContainer<C,bAlloc,true>
	{
		/*
		* Constructor.
		* @param m               The initial maximal size of the array.
		* @param i               The value used when increasing the array. If
		*                        null value, the size is set to the half the
		*                        maximal size.
		*/
		Hash(size_t m,size_t i) : RContainer<C,bAlloc,true>(m,i) {}

		friend class RHashContainer;
	};

private:

	/**
	* This container represents the hash table of the elements.
	*/
	RContainer<Hash,true,true> HashTable;

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
	* Constructor of a hash container.
	* @param s               Size of the initial hash table.
	* @param m               The initial maximal size of the array for a index.
	* @param i               The value used when increasing the array for a
	*                        index. If null value, the size is set to the half
	*                        of the maximal size.
	*/
	RHashContainer(size_t s,size_t m,size_t i=0)
		: HashTable(s), Max(m), Inc(i)
	{
		for(size_t pos=0;pos<s;pos++)
			HashTable.InsertPtrAt(new Hash(Max,Inc),pos);
	}

	/**
	* Get the number of elements in the hash container.
	*/
	inline size_t GetNb(void) const
	{
		RCursor<Hash> Cur(HashTable);
		size_t nb;
		for(Cur.Start(),nb=0;!Cur.End();Cur.Next())
			nb+=Cur()->GetNb();
		return(nb);
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
			throw RException("Invalid hash index");
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
			HashTable.InsertPtrAt(ptr=new Hash(Max,Inc),hash);
		return(ptr);
	}
	
public:
	
	/**
	* Look if a certain element is in the container.
	* @param TUse            The type of tag, the container uses the Compare(TUse &)
	*                        member function of the elements.
	* @param tag             The tag used.
	* @param sortkey         The tag represents the sorting key. The default value
	*                        depends if the container is ordered (true) or not
	*                        (false).
	* @return Return true if the element is in the container.
	*/
	template<class TUse> inline bool IsIn(const TUse& tag,bool sortkey=true) const
	{
		
		const Hash* ptr=GetHash(tag.HashIndex(1));
		return(ptr->IsIn<TUse>(tag,sortkey));
	}

	/**
	* Get a pointer to a certain element in the container.
	* @param TUse            The type of tag, the container uses the Compare(TUse &)
	*                        member function of the elements.
	* @param tag             The tag used.
	* @param sortkey         The tag represents the sorting key. The default value
	*                        depends if the container is ordered (true) or not
	*                        (false).
	* @return Return the pointer or 0 if the element is not in the container.
	*/
	template<class TUse> inline C* GetPtr(const TUse& tag,bool sortkey=true) const
	{
		const Hash* ptr=GetHash(tag.HashIndex(1));
		return(ptr->GetPtr<TUse>(tag,sortkey));
	}

	/**
	* Get a pointer to a certain element in the container. If the element is
	* not existing, the container creates it by using the constructor with TUse
	* as parameter.
	* @param TUse            The type of tag, the container uses the Compare(TUse &)
	*                        member function of the elements.
	* @param tag             The tag used.
	* @param sortkey         The tag represents the sorting key. The default value
	*                        depends if the container is ordered (true) or not
	*                        (false).
	* @return The function returns a pointer to the element of the container.
	*/
	template<class TUse> inline C* GetInsertPtr(const TUse& tag,bool sortkey=true)
	{
		Hash* ptr=GetHash(tag.HashIndex(1));
		return(ptr->GetInsertPtr<TUse>(tag,sortkey));
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
		Hash* ptr=GetHash(ins->HashIndex(1));
		ptr->InsertPtr(ins,del);
	}

	/**
	* Delete an element from the container. The element is destruct if the
	* container is responsible of the desallocation.
	* @param TUse            The type of tag, the container uses the Compare(TUse &)
	*                        member function of the elements.
	* @param tag             The tag used.
	* @param sortkey         The tag represents the sorting key. The default value
	*                        depends if the container is ordered (true) or not
	*                        (false).
	*/
	template<class TUse> inline void DeletePtr(const TUse& tag,bool sortkey=true)
	{
		Hash* ptr=GetHash(tag.HashIndex(1));
		ptr->DeletePtr<TUse>(tag,sortkey);
	}

	/**
	* Destruct the hash container.
	*/
	virtual ~RHashContainer(void) {}
};


}  //-------- End of namespace R -----------------------------------------------


//------------------------------------------------------------------------------
#endif
