/*

	R Project Library

	RMath.cpp

	Basic mathematical functions - Implementation

	Copyright 2004 by the Université Libre de Bruxelles.

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



//-----------------------------------------------------------------------------
// include files for R Project
#include <rstd/rstd.h>
#include <rmath/rmath.h>
using namespace R;
using namespace std;



//-----------------------------------------------------------------------------
//
// Generic functions
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
double R::Factorial(unsigned int n)
{
	if(n==1)
		return(1.0);
	return(n*Factorial(n-1));
}


//-----------------------------------------------------------------------------
double R::Combinatory(unsigned int n,unsigned int p)
{
	return(Factorial(n)/(Factorial(p)*Factorial(n-p)));
}
