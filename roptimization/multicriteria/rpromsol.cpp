/*

	R Project Library

	RPromSol.cpp

	PROMETHEE Solutions - Implementation.

	Copyright 2000-2015 by Pascal Francq (pascal@francq.info).
	Copyright 2000-2008 by the Universit√© Libre de Bruxelles (ULB).

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
// include files for R Project
#include <rpromsol.h>
using namespace R;



//------------------------------------------------------------------------------
//
// class RPromSol
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
RPromSol::RPromSol(size_t id,size_t nbcrit)
	: RContainer<RPromCritValue,true,false>(nbcrit,nbcrit/2), Id(id)
{
}


//------------------------------------------------------------------------------
RPromSol::RPromSol(size_t id,const RString& name,size_t nbcrit)
	: RContainer<RPromCritValue,true,false>(nbcrit,nbcrit/2), Id(id), Name(name)
{
}


//------------------------------------------------------------------------------
RPromSol::~RPromSol(void)
{
}
