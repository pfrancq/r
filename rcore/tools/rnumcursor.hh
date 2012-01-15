/*

	R Project Library

	RNumCursor.hh

	Cursor over a container of numbers - Inline implementation

	Copyright 2008-2012 by Pascal Francq (pascal@francq.info).

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
// class RNumCursor
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
template<class I>
	RNumCursor<I>::RNumCursor(void)
		: NbInt(0), List(0), Parse(0), Pos(0), First(0), Last(0)
{
}

//------------------------------------------------------------------------------
template<class I>
	RNumCursor<I>::RNumCursor(const RNumCursor& cur)
		: NbInt(cur.NbInt), List(cur.List), Parse(cur.Parse), Pos(cur.Pos), First(cur.First), Last(cur.Last)
{
}


//------------------------------------------------------------------------------
template<class I>
	template<bool o>
		RNumCursor<I>::RNumCursor(const RNumContainer<I,o>& cont,size_t min,size_t max)
			: NbInt(0), List(0), Parse(0), Pos(0), First(0), Last(0)
{
	Set(cont,min,max);
}


//------------------------------------------------------------------------------
template<class I>
	template<bool o>
		void RNumCursor<I>::Set(const RNumContainer<I,o>& cont,size_t min,size_t max)
{
	NbInt=cont.NbInt;
	List=cont.List;
	Parse=cont.List;
	Pos=0;
	if((max!=SIZE_MAX)&&(max<cont.GetNb()))
		Last=max+1;
	else
		Last=cont.GetNb();
	if((min<=max)&&(min<cont.GetNb()))
		First=min;
	else
		First=0;
}


//-----------------------------------------------------------------------------
template<class I>
	void RNumCursor<I>::Start(void)
{
	Pos=First;
	if(!NbInt)
	{
		Parse=0;
		return;
	}
	Parse=&List[First];
}


//-----------------------------------------------------------------------------
template<class I>
	void RNumCursor<I>::StartFromEnd(void)
{
	Pos=Last-1;
	if(!NbInt)
	{
		Parse=0;
		return;
	}
	Parse=&List[Last-1];
}


//-----------------------------------------------------------------------------
template<class I>
	void RNumCursor<I>::GoTo(size_t idx)
{
	idx+=First;
	if(idx>=Last)
		throw std::range_error("void RNumCursor::GoTo(size_t) : column "+RString::Number(idx)+" outside range ["+RString::Number(First)+","+RString::Number(Last-1)+"]");
	Parse=&List[idx];
	Pos=idx;
}


//-----------------------------------------------------------------------------
template<class I>
	void RNumCursor<I>::Next(size_t inc)
{
	if(!NbInt) return;
	if(Pos==Last)
		Start();
	else
	{
		Pos+=inc;
		Parse+=inc;
	}
}


//-----------------------------------------------------------------------------
template<class I>
	void RNumCursor<I>::Prev(size_t inc)
{
	if(!NbInt) return;
	if(Pos==cNoRef)
	{
		StartFromEnd();
		if(inc>1)
			Prev(inc);
	}
	else
	{
		if(inc>Pos)
			Pos=cNoRef;
		else
		{
			Pos-=inc;
			Parse-=inc;
		}
	}
}
