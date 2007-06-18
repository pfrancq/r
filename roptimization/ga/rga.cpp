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
RGAException::RGAException(tException type,const unsigned int g,const unsigned int c) throw()
	: RException(), Type(type)
{
	char tmp[200];
	
	switch(Type)
	{
		case eGAVerify:		
			sprintf(tmp,"Generation %u: Verify error for chromosome %u",g,c);
			break;
		case eGAMutation:	
			sprintf(tmp,"Generation %u: Mutation error: Chromsosome %u",g,c);
			break;
		case eGAInversion:
			sprintf(tmp,"Generation %u: Inversion error: Chromsosome %u",g,c);
			break;
		case eGAOptimisation:
			sprintf(tmp,"Generation %u: Optimisation error: Chromsosome %u",g,c);
			break;
		case eGARandomConstruct:
			sprintf(tmp,"Generation %u: Random Construction error: Chromsosome %u",g,c);
			break;
		case eGAEvaluation:	
			sprintf(tmp,"Generation %u: Evaluation error: Chromsosome %u",g,c);
			break;  		
		case eGAPostEvaluation:
			sprintf(tmp,"Generation %u: Post Evaluation error: Chromsosome %u",g,c);
			break;
		default:
			sprintf(tmp,"Generation %u: Unknown error for chromosome %u",g,c);
	}
	SetMsg(tmp);
}



//------------------------------------------------------------------------------
RGAException::RGAException(const unsigned int g,const unsigned p1,const unsigned int p2,const unsigned int c) throw()
	: RException(), Type(eGACrossover)
{
	char tmp[200];

	sprintf(tmp,"Generation %u: Crossover error: parent %u + parent %u -> Child %u",g,p1,p2,c);
	SetMsg(tmp);
}
