/*

	R Project Library

	RMath.h

	Basic mathematical functions - Header

	Copyright 2004-2011 by Pascal Francq (pascal@francq.info).
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
#ifndef RMath_H
#define RMath_H


//-----------------------------------------------------------------------------
namespace R{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/**
* Function computing the factorial of a given number.
* @param n                   Number.
* @returns double
*/
double Factorial(size_t n);


//-----------------------------------------------------------------------------
/**
* Function computing the number of combinations of p objects in a set of n.
* @param n                   Total number of objects.
* @param p                   Number of objects in a combination.
* @returns double
*/
double Combinatory(size_t n,size_t p);


}  //-------- End of namespace R ----------------------------------------------


//-----------------------------------------------------------------------------
#endif
