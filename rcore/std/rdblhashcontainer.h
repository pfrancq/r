/*

	Rainbow Library Project

	RDblHashContainer.h

	Container with a double hash table

	(C) 2001-2002 by P. Francq.

	Version $Revision$

	Last Modify: $Date$

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
#ifndef RDblHashContainerH
#define RDblHashContainerH


//-----------------------------------------------------------------------------
// include files for R Project
#include <rstd/rcontainer.h>


//-----------------------------------------------------------------------------
namespace RStd{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/**
* @param C                  The class of the element to be contained.
* @param T                  The type of the iterator used.
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
* <pre>
* int Compare(const TUse& tag) const;
* int Compare(const TUse* tag) const;
* static int HashIndex(const TUse& tag);
* static int HashIndex(const TUse* tag);
* static int HashIndex2(const TUse& tag);
* static int HashIndex2(const TUse* tag);
* </pre>
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
* <pre>
* int Compare(const C*) const;
* static int HashIndex(const C*);
* static int HashIndex2(const C*);
* </pre>
*
* Here is an example of class MyElement that will be contained in the
* variable c:
* <pre>
* #include <string.h>
* #include <rstd/rdblhashcontainer.h>
* using namespace RStd;
*
*
* class MyElement
* {
* 	char Text[25];
* public:
* 	MyElement(const char* text) {Text=strdup(text);}
* 	MyElement(MyElement *e) {Text=strdup(e->Text);}
* 	int Compare(MyElement *e) {return(strcmp(Text,e->Text));}
* 	int Compare(const char* text) {return(strcmp(Text,text));}
* 	static int HashIndex(MyElement *e)
* 	{
* 		int c=(*e->Text);
* 		if(c>='a'&&c<='z') return(c-'a');
* 		if(c>='A'&&c<='Z') return(c-'A');
* 		return(26);
* 	}
* 	static int HashIndex2(MyElement *e)
* 	{
* 		int c=(*(e->Text+1));
* 		if(c>='a'&&c<='z') return(c-'a');
* 		if(c>='A'&&c<='Z') return(c-'A');
* 		return(26);
* 	}
* };
*
*
* int main()
* {
* 	RDblHashContainer<MyElement,unsigned int,27,true> c(20,10);
*
* 	c.InsertPtr(new MyElement("Coucou"));
* 	if(c.IsIn<const char*>("Coucou"))
* 		cout<<"An element of value 5 is in the container"<<endl;
* 	c.InsertPtr(new MyElement("Autre"));
* }
* </pre>
*
* @author Pascal Francq
* @short Container Template with Double Hash Table.
*/
template<class C,class T,T tSize1,T tSize2,bool bAlloc>
	class RDblHashContainer
{
public:
	/**
	* This container represents the hash table of the elements.
	*/
	RContainer<C,T,bAlloc,true>*** Hash;

	/**
	* Construct a Hash container.
	* @param M              Default maximum number of elements.
	* @param I              Incremental number of elements.
	*/
	RDblHashContainer(T M,T I) throw(bad_alloc)
	{
		RContainer<C,T,bAlloc,true>*** ptr;
		RContainer<C,T,bAlloc,true>** ptr2;
		T i,j;

		Hash= new RContainer<C,T,bAlloc,true>**[tSize1];
		for(i=tSize1+1,ptr=Hash;--i;ptr++)
		{
			(*ptr)=new RContainer<C,T,bAlloc,true>*[tSize2];
			for(j=tSize2+1,ptr2=*ptr;--j;ptr2++)
				(*ptr2)=new RContainer<C,T,bAlloc,true>(M,I);
		}
	}

	/**
	* Clear the hash container.
	*/
	void Clear(void)
	{
		RContainer<C,T,bAlloc,true>*** ptr;
		RContainer<C,T,bAlloc,true>** ptr2;
		T i,j;

		for(i=tSize1+1,ptr=Hash;--i;ptr++)
			for(j=tSize2+1,ptr2=*ptr;--j;ptr2++)
				(*ptr2)->Clear();
	}

	/**
	* Insert an element.
	* @param ins            Pointer to the element to insert.
	*/
	inline void InsertPtr(C *ins) throw(bad_alloc)
	{
		RReturnIfFail(ins);
		Hash[C::HashIndex(ins)][C::HashIndex2(ins)]->InsertPtr(ins);
	}

	/**
	* Get the number of elements in the hash container.
	* @returns Number of elements.
	*/
	inline unsigned int GetNb(void) const
	{
		unsigned int nb=0;
		RContainer<C,T,bAlloc,true>*** ptr;
		RContainer<C,T,bAlloc,true>** ptr2;
		T i,j;

		for(i=tSize1+1,ptr=Hash;--i;ptr++)
		{
			for(j=tSize2+1,ptr2=*ptr;--j;ptr2++)
				nb+=(*ptr2)->NbPtr;
		}
		return(nb);
	}

	/**
	* Verify if an element is in the hash container.
	* @param TUse           The type of tag, the hash container uses the
	*                       Compare(TUse &) member function of the elements.
	* @param tag            The tag used.
	* @returns The function returns true if the element is in the hash
	* container, else false.
	*/
	template<class TUse> inline bool IsIn(const TUse tag,bool sortkey=true) const
	{
		return(Hash[C::HashIndex(tag)][C::HashIndex2(tag)]->IsIn<TUse>(tag,sortkey));
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
		return(Hash[C::HashIndex(tag)][C::HashIndex2(tag)]->GetPtr<TUse>(tag,sortkey));
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
	template<class TUse> inline C* GetInsertPtr(const TUse tag,bool sortkey=true) throw(bad_alloc)
	{
		return(Hash[C::HashIndex(tag)][C::HashIndex2(tag)]->GetInsertPtr<TUse>(tag,sortkey));
	}

	/**
	* Delete an element of the hash container.
	* @param del            Pointer to the element to delete.
	*/
	inline void DeletePtr(C* del)
	{
		RReturnIfFail(del);
		Hash[C::HashIndex(del)][C::HashIndex2(del)]->DeletePtr(del);
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
	template<class TUse> inline void DeletePtr(const TUse tag,bool sortkey=true)
	{
		Hash[C::HashIndex(tag)][C::HashIndex2(tag)]->DeletePtr<TUse>(tag,sortkey);
	}

	/**
	* Destruct the hash container.
	*/
	virtual ~RDblHashContainer(void)
	{
		RContainer<C,T,bAlloc,true>*** ptr;
		RContainer<C,T,bAlloc,true>** ptr2;
		T i,j;

		for(i=tSize1+1,ptr=Hash;--i;ptr++)
		{
			for(j=tSize2+1,ptr2=*ptr;--j;ptr2++)
				delete (*ptr2);
			delete[] (*ptr);
		}
		delete[] Hash;
	}
};


}  //-------- End of namespace RStd -------------------------------------------


//-----------------------------------------------------------------------------
#endif
