/*

	R Project Library

	RGA.cpp

	Genetic Algorithms - Implementation

	Copyright 1998-2012 by Pascal Francq (pascal@francq.info).
	Copyright 1998-2008 by the Université Libre de Bruxelles (ULB).

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
// include files for AINSI C/C++
#include <iostream>


//------------------------------------------------------------------------------
// include files for R Project
#include <rga.h>
using namespace R;


//------------------------------------------------------------------------------
// General Variables
bool R::ExternBreak=false;



//------------------------------------------------------------------------------
//
// class RGAException
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
RGAException::RGAException(const RString& type,const RString& str)
	: RException(type+": "+str), Type(type)
{
}


//------------------------------------------------------------------------------
RGAException::RGAException(const RString& type,const RString& func,long where,const RString& str)
	: RException(func,where,str), Type(type)
{
}
