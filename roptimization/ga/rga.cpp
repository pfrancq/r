/*

	R Project Library

	RGA.cpp

	Genetic Algorithms - Implementation

	Copyright 1998-2005 by the Université Libre de Bruxelles.

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
RGAException::RGAException(tException type) throw()
	: RException(), Type(type)
{
}


//------------------------------------------------------------------------------
RGAException::RGAException(const char* str,tException type) throw()
	: RException(str), Type(type)
{
}


//------------------------------------------------------------------------------
RGAException::RGAException(tException type,const size_t g,const size_t c) throw()
	: RException(), Type(type)
{
	char tmp[200];

	switch(Type)
	{
		case eGAVerify:
			sprintf(tmp,"Generation %z: Verify error for chromosome %z",g,c);
			break;
		case eGAMutation:
			sprintf(tmp,"Generation %z: Mutation error: Chromsosome %z",g,c);
			break;
		case eGAInversion:
			sprintf(tmp,"Generation %z: Inversion error: Chromsosome %z",g,c);
			break;
		case eGAOptimisation:
			sprintf(tmp,"Generation %z: Optimisation error: Chromsosome %z",g,c);
			break;
		case eGARandomConstruct:
			sprintf(tmp,"Generation %z: Random Construction error: Chromsosome %z",g,c);
			break;
		case eGAEvaluation:
			sprintf(tmp,"Generation %z: Evaluation error: Chromsosome %z",g,c);
			break;
		case eGAPostEvaluation:
			sprintf(tmp,"Generation %z: Post Evaluation error: Chromsosome %z",g,c);
			break;
		default:
			sprintf(tmp,"Generation %z: Unknown error for chromosome %z",g,c);
	}
	SetMsg(tmp);
}



//------------------------------------------------------------------------------
RGAException::RGAException(const size_t g,const size_t p1,const size_t p2,const size_t c) throw()
	: RException(), Type(eGACrossover)
{
	char tmp[200];

	sprintf(tmp,"Generation %z: Crossover error: parent %z + parent %z -> Child %z",g,p1,p2,c);
	SetMsg(tmp);
}
