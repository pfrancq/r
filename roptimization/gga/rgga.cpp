/*

	R Project Library

	RGGA.cpp

	Grouping Genetic Algorithms - Implementation

	Copyright 2001-2007 by the Université Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).

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
#include <rgga.h>
#include <rstring.h>
using namespace R;



//------------------------------------------------------------------------------
//
// class RGGAException
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
RGGAException::RGGAException(tException type,const size_t g,const size_t c)
	: RGAException(type)
{
	switch(Type)
	{
		case eGAModify:
			SetMsg("Generation "+RString::Number(g)+" : Modify error for chromosome "+RString::Number(c));
			break;
		case eGALocalOptimisation:
			SetMsg("Generation "+RString::Number(g)+" : Local optimization error for chromosome "+RString::Number(c));
			break;
		default:
			RGAException(Type,g,c);
	}
}
