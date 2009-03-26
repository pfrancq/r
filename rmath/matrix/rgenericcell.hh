/*
	R Project Library

	RGenericCell.cpp

	Generic Cell Element  - Implementation.

	Copyright 2005-2009 by Pascal Francq (pascal@francq.info).
	Copyright 2003-2005 by Vandaele Valery.
	Copyright 2003-2008 by the Université Libre de Bruxelles (ULB).

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
//
// RGenericCell
//
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
template<class C>
	RGenericCell<C>::RGenericCell(size_t id)
		: Id(id) , Value(0)
{}


//-----------------------------------------------------------------------------
template<class C>
	RGenericCell<C>::RGenericCell(size_t id, C value)
		: Id(id) , Value(value)
{}


//-----------------------------------------------------------------------------
template<class C>
	RGenericCell<C>::RGenericCell(const RGenericCell<C>* src)
		: Id(src->GetId()) , Value(src->GetValue())
{}


//-----------------------------------------------------------------------------
template<class C>
	RGenericCell<C>::RGenericCell(const RGenericCell<C>& src)
		: Id(src.GetId()) , Value(src.GetValue())
{}


//-----------------------------------------------------------------------------
template<class C>
	RGenericCell<C>& RGenericCell<C>::operator=(const RGenericCell<C>& src)
{
	Id= src.Id;
	Value=src.Value;
	return(*this);
}


//-----------------------------------------------------------------------------
template<class C>
	int RGenericCell<C>::Compare(const RGenericCell<C>* c) const
	{
		return (Id-c->Id);
	}


//-----------------------------------------------------------------------------
template<class C>
	int RGenericCell<C>::Compare(const RGenericCell<C>& c) const
{
	return (Id-c.Id);
}


//-----------------------------------------------------------------------------
template<class C>
	int RGenericCell<C>::Compare(const size_t id) const
{
	return (Id-id);
}


//-----------------------------------------------------------------------------
template<class C>
	RGenericCell<C>::~RGenericCell(void)
{
}
