/*

  RContainer.h

  Container - Header.

  (C) 1999-2000 by P. Francq.

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

  template <class C,class T,bool bAlloc,bool bOrder=false> class RContainer
    Class C  :  class representing a member (Ex.: string)
    Class T  :  class representing an iterator (Ex.: int)
    bAlloc   :  If true, the container is responsible for the
                desallocation of his member
    bOrder   :  If true, the container holds the members in a specific
                order using the Compare function of the members

  Class C:
    The Compare(TUse tag) function returns an int:
        C==tag : 0
        C<tag  : <0
        C>tag  : >0
    The class C must declare at least one function with the signature
    Compare(C*).

*/


//---------------------------------------------------------------------------
#ifndef RContainerH
#define RContainerH


//---------------------------------------------------------------------------
#ifdef unix
	#ifndef NULL
		#define NULL 0
	#endif
	#include <string.h>
#else
	#include <mem.h>
#endif
#include <new.h>
using namespace std;


//---------------------------------------------------------------------------
namespace RStd{
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
template<class C,class T,bool bAlloc,bool bOrder=false> class RContainer
{
public:
  C **Tab;
  T NbPtr,MaxPtr,IncPtr;

  // Constructors
  RContainer(T M,T I) throw(bad_alloc);
	RContainer(RContainer *container) throw(bad_alloc);
	// operators
	RContainer& operator=(const RContainer& container) throw(bad_alloc);
  // Get Index
  template<class TUse> T GetId(const TUse &tag,bool &Find);
  // Verify size
  void VerifyTab(void) throw(bad_alloc);
	// Clear the container
	void Clear(void);
  // Insert
  void InsertPtrAt(C *ins,T Pos) throw(bad_alloc);
  void InsertPtr(C* ins) throw(bad_alloc);
  // Get
  template<class TUse> bool IsIn(const TUse &tag);
  template<class TUse> C* GetPtr(const TUse &tag);
  // Insert or Get
  template<class TUse> C* GetInsertPtr(const TUse &tag) throw(bad_alloc);
  // Delete
  void DeletePtr(C* del);
  template<class TUse> void DeletePtr(const TUse &tag);
  // For each
  template<class TUse> void ForEach(void f(TUse),const TUse &tag);
  // Class destructor
  ~RContainer(void);
};


#include "rcontainer.hh" // implementation

}  //-------- End of namespace RStd ---------------------------------------


//---------------------------------------------------------------------------
#endif


