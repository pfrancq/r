/*

  RHashContainer.h

  Container with an hash table

  (C) 2000 by P. Francq.

  Version $Revision$

  Last Modify: $Date$

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

*/

/*

  template <class C,class T,T tSize,bool bAlloc> class RHashContainer
    Class C  :  class representing a member (Ex.: string)
    Class T  :  class representing an iterator (Ex.: int)
    tSize		 :	size of the hash table
    bAlloc   :  If true, the container is responsible for the
                desallocation of his member
	

	
  Class C:
    The Compare(TUse tag) function returns an int:
        C==tag : 0
        C<tag  : <0
        C>tag  : >0
    The class C must declare at least one function with the signature
    Compare(C*).
		The HashIndex(void) function returns the hash index of the element
		The HashIndex of TUse function returns the hash index of the tag
		
*/


//---------------------------------------------------------------------------
#ifndef RHashContainerH
#define RHashContainerH


//---------------------------------------------------------------------------
#include "rcontainer.h"
using namespace RStd;


//---------------------------------------------------------------------------
namespace RStd{
//---------------------------------------------------------------------------


template<class C,class T,T tSize,bool bAlloc> class RHashContainer
{
public:
	RContainer<C,T,bAlloc,true> **Hash;

	RHashContainer(T M,T I) throw(bad_alloc)
	{
		RContainer<C,T,bAlloc,true> **ptr;
		T i;
			
		Hash= new RContainer<C,T,bAlloc,true>*[tSize];
		for(i=tSize+1,ptr=Hash;--i;ptr++)
			(*ptr)=new RContainer<C,T,bAlloc,true>(M,I);	
	}
  inline void InsertPtrAt(C *ins,T Pos) throw(bad_alloc)
  {
  	Hash[ins->HashIndex()]->InsertPtrAt(ins,Pos);
  }
  inline void InsertPtr(C *ins) throw(bad_alloc)
  {
	  Hash[ins->HashIndex()]->InsertPtr(ins);	
  }
  template<class TUse> inline bool IsIn(const TUse &tag)
  {
		Hash[tag->HashIndex()]->IsIn<TUse>(tag);	  	
  }
  template<class TUse> inline C* GetPtr(const TUse &tag)
  {
		return(Hash[tag->HashIndex()]->GetPtr<TUse>(tag));	  	
  }
  template<class TUse> inline C* GetInsertPtr(const TUse &tag) throw(bad_alloc)
  {
		retrun(tag->Hash[HashIndex()]->GetInsertPtr<TUse>(tag));	  	
  }
  inline void DeletePtr(C* del)
  {
		Hash[ins->HashIndex()]->DeletePtr(del);	  	
  }
  template<class TUse> inline void DeletePtr(const TUse &tag)
  {
		Hash[tag->HashIndex()]->DeletePtr<TUse>(tag);	  	
  }
  template<class TUse> inline void ForEach(void f(TUse),const TUse &tag)
  {
		Hash[tag->HashIndex()]->ForEach<void f(TUse),TUse>(f,tag);	  	
  }
  ~RHashContainer(void)
  {
		RContainer<C,T,bAlloc,true> **ptr=Hash;
		for(T i;--i;ptr++)	delete (*ptr);	
		delete[] Hash;  	
  }
};

s
}  //-------- End of namespace RStd ---------------------------------------


//---------------------------------------------------------------------------
#endif
