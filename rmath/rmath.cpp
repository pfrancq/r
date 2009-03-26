/*

	R Project Library

	RMath.cpp

	Basic mathematical functions - Implementation

	Copyright 2004-2009 by Pascal Francq (pascal@francq.info).
	Copyright 2004-2008 by the Université Libre de Bruxelles (ULB).

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
#include <rstd.h>
#include <rmath.h>
using namespace R;
using namespace std;



//-----------------------------------------------------------------------------
//
// Generic functions
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
double R::Factorial(size_t n)
{
	double res=1.0;

	for(;n>1;n--)
		res*=static_cast<double>(n);
	return(res);
}


//-----------------------------------------------------------------------------
double R::Combinatory(size_t n,size_t p)
{
	return(Factorial(n)/(Factorial(p)*Factorial(n-p)));
}
