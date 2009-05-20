/*

	R Project Library

	RNumCursor.hh

	Cursor over a container of numbers - Inline implementation

	Copyright 2008-2009 by Pascal Francq (pascal@francq.info).

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
		: NbInt(0), List(0), Parse(0), Pos(0)
{
}

//------------------------------------------------------------------------------
template<class I>
	RNumCursor<I>::RNumCursor(const RNumCursor& cur)
		: NbInt(cur.NbInt), List(cur.List), Parse(cur.Parse), Pos(cur.Pos)
{
}


//------------------------------------------------------------------------------
template<class I>
	template<bool a>
		RNumCursor<I>::RNumCursor(const RNumContainer<I,a>& cont)
			: NbInt(cont.NbInt), List(cont.List), Parse(cont.List), Pos(0)
{
}


//------------------------------------------------------------------------------
template<class I>
	template<bool a>
		void RNumCursor<I>::Set(const RNumContainer<I,a>& cont)
{
	NbInt=cont.NbInt;
	List=cont.List;
	Parse=cont.List;
	Pos=0;
}
