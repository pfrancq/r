/*

	R Project Library

	RPromethee.h

	Rainbow Promethee Library - Header.

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
#ifndef RPrometheeH
#define RPrometheeH


//-----------------------------------------------------------------------------
// include files for R Project
#include <rstd/rstd.h>
#include <rstd/rcontainer.h>
#include <rstd/rstring.h>
using namespace RStd;


//-----------------------------------------------------------------------------
/**
* \namespace RPromethee
* \brief Promethee Classes.
*
* This namespace declares the classes to run prométhée instances.
*/


//-----------------------------------------------------------------------------
namespace RPromethee{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/**
* Different types for criteria.
*/
enum CriteriaType{Minimize,Maximize};
enum FilterType{Low, LowEqual, Equal, GreatEqual, Great};


//-----------------------------------------------------------------------------
// forward class declaration
class RPromEvalFunc;
class RPromCriterion;
class RPromCritValue;
class RPromSol;
class RPromKernel;


}  //-------- End of namespace RPromethee -------------------------------------


//-----------------------------------------------------------------------------
#endif
