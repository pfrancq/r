/*
	R Project Library

	RGenericCell.cpp

	Generic Cell Element  - Implementation.

	Copyright 2003 by the Université Libre de Bruxelles.

	Authors
		 Vandaele Valery(vavdaele@ulb.ac.be)

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
//
// RGenericCell
//
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
template<class C>
	RGenericCell<C>::RGenericCell(unsigned int id) throw(std::bad_alloc)
		: Id(id) , Value(0)
{}


//-----------------------------------------------------------------------------
template<class C>
	RGenericCell<C>::RGenericCell(unsigned int id, C value) throw(std::bad_alloc)
		: Id(id) , Value(value)
{}


//-----------------------------------------------------------------------------
template<class C>
	RGenericCell<C>::RGenericCell(const RGenericCell<C>* src) throw(std::bad_alloc)
		: Id(src->GetId()) , Value(src->GetValue())
{}


//-----------------------------------------------------------------------------
template<class C>
	RGenericCell<C>::RGenericCell(const RGenericCell<C>& src) throw(std::bad_alloc)
		: Id(src.GetId()) , Value(src.GetValue())
{}


//-----------------------------------------------------------------------------
template<class C>
	RGenericCell<C>& RGenericCell<C>::operator=(const RGenericCell<C>& src) throw(std::bad_alloc)
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
	int RGenericCell<C>::Compare(const unsigned int id) const
{
	return (Id-id);
}


//-----------------------------------------------------------------------------
template<class C>
	RGenericCell<C>::~RGenericCell(void)
{
}

