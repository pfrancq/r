/*

  RContainer.hh

  Container - Inline Implementation.

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

#ifdef __BORLANDC__
	#pragma warn -ccc
	#pragma warn -rch
#endif


//---------------------------------------------------------------------------
template<class C,class T,bool bAlloc,bool bOrder>
	RContainer<C,T,bAlloc,bOrder>::RContainer(T M,T I) throw(bad_alloc)
{
  NbPtr=0;
  MaxPtr=M;
  IncPtr=I;
  Tab = new C*[MaxPtr];
  memset(Tab,0,MaxPtr*sizeof(C*));
}


//---------------------------------------------------------------------------
template<class C,class T,bool bAlloc,bool bOrder>
	RContainer<C,T,bAlloc,bOrder>::RContainer(RContainer<C,T,bAlloc,bOrder> *container) throw(bad_alloc)
{
	T i;
	C **tab;

	NbPtr=0;
	MaxPtr=container->MaxPtr;
	IncPtr=container->IncPtr;
  Tab = new C*[MaxPtr];
  memset(Tab,0,MaxPtr*sizeof(C*));
	for(i=container->NbPtr+1,tab=container->Tab;--i;tab++)
		InsertPtr(new C(*tab));	
}


//---------------------------------------------------------------------------
template<class C,class T,bool bAlloc,bool bOrder>
	RContainer<C,T,bAlloc,bOrder>& RContainer<C,T,bAlloc,bOrder>::
		operator=(const RContainer<C,T,bAlloc,bOrder>& container) throw(bad_alloc)
{
	T i;
	C **tab;

	Clear();
	for(i=container.NbPtr+1,tab=container.Tab;--i;tab++)
		InsertPtr(new C(*tab));
	return(*this);
}


//---------------------------------------------------------------------------
template<class C,class T,bool bAlloc,bool bOrder> template<class TUse>
	T RContainer<C,T,bAlloc,bOrder>::GetId(const TUse &tag,bool &Find)
{
  T NbMin,NbMax,i;
  int Comp;
  bool Cont=true,NotLast=true;
  C *ptr,**ptr2;

  if(bOrder)
  {
    Find=false;
    if(!NbPtr)
      return(0);
    NbMax=NbPtr-1;
    NbMin=0;
    if(NbMax)
    {
      while(Cont)
      {
        i = (NbMax+NbMin)/2;
        ptr = Tab[i];
        Comp= ptr->Compare(tag);
        if(!Comp)
        {
          Find=true;
          return(i);
        }
        if(Comp>0)
        {
          NbMax = i;
          if(i) NbMax--;
        }
        else
          NbMin = i+1;
        Cont = NotLast;
        if(NbMin>=NbMax) NotLast=false;
      }
    }
    else
    {
      i = 0;
      Comp= (*Tab)->Compare(tag);
      if(!Comp)
      {
        Find=true;
        return(0);
      }
    }
    if(Comp<0) i++;
    return(i);
  }
  else
  {
    Find=true;
    for(i=0,ptr2=Tab;i<NbPtr;ptr2++,i++)
      if(!((*ptr2)->Compare(tag))) return(i);
    Find=false;
    return(i);
  }
}


//---------------------------------------------------------------------------
template<class C,class T,bool bAlloc,bool bOrder>
	void RContainer<C,T,bAlloc,bOrder>::VerifyTab(void) throw(bad_alloc)
{
  if(NbPtr==MaxPtr)
  {
    C **ptr;
    MaxPtr+=IncPtr;
    ptr=new C*[MaxPtr];
    memcpy(ptr,Tab,NbPtr*sizeof(C*));
    delete[] Tab;
    Tab=ptr;
    memset(&Tab[NbPtr],0,IncPtr*sizeof(C*));
  }
}


//---------------------------------------------------------------------------
template<class C,class T,bool bAlloc,bool bOrder>
	void RContainer<C,T,bAlloc,bOrder>::Clear(void)
{
  if(bAlloc)
  {
    C **ptr;

    for(NbPtr++,ptr=Tab;--NbPtr;ptr++) delete (*ptr);
  }
	memset(Tab,0,MaxPtr*sizeof(C*));
}


//---------------------------------------------------------------------------
// Insert the pointer ins at position Pos
template<class C,class T,bool bAlloc,bool bOrder>
	void RContainer<C,T,bAlloc,bOrder>::InsertPtrAt(C *ins,T Pos) throw(bad_alloc)
{
  T i;
  C **ptr;

  if(Pos>NbPtr) return;
  NbPtr++;
  VerifyTab();
  for(i=0,ptr=Tab;i<Pos;i++,ptr++);
  memmove(ptr+1,ptr,(NbPtr-i-1)*sizeof(C*));
  (*ptr)=ins;
}


//---------------------------------------------------------------------------
// Insert a pointer C* in the container
template<class C,class T,bool bAlloc,bool bOrder>
	void RContainer<C,T,bAlloc,bOrder>::InsertPtr(C* ins) throw(bad_alloc)
{
  if(bOrder)
  {
    bool Find;
    T Index=GetId<C*>(ins,Find);
    if(Find)
    {
      if(bAlloc)
        delete(ins);
    }
    else
      InsertPtrAt(ins,Index);
  }
  else
  {
    VerifyTab();
    Tab[NbPtr++]=ins;
  }
}


//---------------------------------------------------------------------------
// Return true if the tag already exists
template<class C,class T,bool bAlloc,bool bOrder> template<class TUse>
	bool RContainer<C,T,bAlloc,bOrder>::IsIn(const TUse &tag)
{
  if(bOrder)
  {
    bool Find;
    GetId<TUse>(tag,Find);
    return(Find);
  }
  else
  {
    T i;
    C **ptr;

    for(i=NbPtr+1,ptr=Tab;--i;ptr++)
      if(!((*ptr)->Compare(tag))) return(true);
    return(false);
  }
}


//---------------------------------------------------------------------------
// Return the pointer to the member corresponding to tag
// If not find -> Return NULL
template<class C,class T,bool bAlloc,bool bOrder> template<class TUse>
	C* RContainer<C,T,bAlloc,bOrder>::GetPtr(const TUse &tag)
{
  if(bOrder)
  {
    bool Find;
    T Index=GetId<TUse>(tag,Find);
    if(Find)
      return(Tab[Index]);
    else
      return(NULL);
  }
  else
  {
    T i;
    C **ptr;

    for(i=NbPtr+1,ptr=Tab;--i;ptr++)
      if(!((*ptr)->Compare(tag))) return(*ptr);
    return(NULL);
  }
}


//---------------------------------------------------------------------------
// Return the pointer to the member corresponding to tag
// If not find -> Create a member with tag
template<class C,class T,bool bAlloc,bool bOrder> template<class TUse>
	C* RContainer<C,T,bAlloc,bOrder>::GetInsertPtr(const TUse &tag) throw(bad_alloc)
{
  if(bOrder)
  {
    bool Find;
    T Index=GetId<TUse>(tag,Find);
    if(!Find)
    {
      InsertPtrAt(new C(tag),Index);
    }
    return(Tab[Index]);
  }
  else
  {
    T i;
    C **ptr;

    for(i=NbPtr+1,ptr=Tab;--i;ptr++)
      if(!((*ptr)->Compare(tag))) return(*ptr);
    VerifyTab();
    (*ptr)=new C(tag);
    InsertPtr(*ptr);
    return(*ptr);
  }
}


//---------------------------------------------------------------------------
// Delete the pointer to the member
template<class C,class T,bool bAlloc,bool bOrder>
	void RContainer<C,T,bAlloc,bOrder>::DeletePtr(C* del)
{
  C **ptr;
  T Index;

  if(bOrder)
  {
    bool Find;
    Index=GetId<C*>(del,Find);
    if(!Find) return;
    ptr=&Tab[Index];
  }
  else
  {
    for(Index=0,ptr=Tab;(*ptr)!=del;Index++,ptr++);
  }
  memcpy(ptr,ptr+1,((--NbPtr)-Index)*sizeof(C*));
  if(bAlloc) delete(del);
}

//---------------------------------------------------------------------------
// Delete the mmber corresponding to tag
template<class C,class T,bool bAlloc,bool bOrder> template<class TUse>
	void RContainer<C,T,bAlloc,bOrder>::DeletePtr(const TUse &tag)
{
  C **ptr,*del;
  T Index;

  if(bOrder)
  {
    bool Find;
    Index=GetId<TUse>(tag,Find);
    if(!Find) return;
    ptr=&Tab[Index];
  }
  else
  {
    for(Index=0,ptr=Tab;(*ptr)->Compare(tag);Index++,ptr++);

  }
  del=*ptr;
  memcpy(ptr,ptr+1,((--NbPtr)-Index)*sizeof(C*));
  if(bAlloc) delete(del);
}


//---------------------------------------------------------------------------
// Class destructor
template<class C,class T,bool bAlloc,bool bOrder> template<class TUse>
void RContainer<C,T,bAlloc,bOrder>::ForEach(void f(TUse),const TUse &tag)
{
  T Index;
  C **ptr;

  for(Index=NbPtr+1,ptr=Tab;--Index;ptr++) (*ptr)->f(tag);
}


//---------------------------------------------------------------------------
// Class destructor
template<class C,class T,bool bAlloc,bool bOrder>
	RContainer<C,T,bAlloc,bOrder>::~RContainer(void)
{
  if(bAlloc)
  {
    C **ptr;

    for(NbPtr++,ptr=Tab;--NbPtr;ptr++) delete (*ptr);
  }
  delete[] Tab;
}

#ifdef __BORLANDC__
	#pragma warn .ccc
	#pragma warn .rch
#endif