/*

	R Project Library

	RPromSol.cpp

	Promethee Solutions - Implementation.

	(C) 2000-2001 by P. Francq.

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
// include files for R Project
#include <rpromethee/rpromsol.h>
using namespace RPromethee;



//-----------------------------------------------------------------------------
//
// class RPromSol
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
RPromethee::RPromSol::RPromSol(const unsigned int id,const unsigned int nbcrit)
	: RContainer<RPromCritValue,unsigned int,true,false>(nbcrit,nbcrit/2), Id(id)
{
}


//-----------------------------------------------------------------------------
RPromethee::RPromSol::RPromSol(const unsigned int id,const RString&  name,const unsigned int nbcrit)
	: RContainer<RPromCritValue,unsigned int,true,false>(nbcrit,nbcrit/2), Id(id), Name(name)
{
}


//-----------------------------------------------------------------------------
RPromethee::RPromSol::~RPromSol(void)
{
}
