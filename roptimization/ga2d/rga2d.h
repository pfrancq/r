/*

	R Project Library

	RGA2D.h

	2D Placement Genetic Algorithm - Header

	Copyright 1999-2008 by the Université Libre de Bruxelles.

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
#ifndef RGA2D_H
#define RGA2D_H


//------------------------------------------------------------------------------
// include files for R Project
#include <rga.h>


//------------------------------------------------------------------------------
namespace R{
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
/**
* The R2DGAException class provides an exception generated by the GGA.
* @author Pascal Francq
* @short 2DGA Exception
*/
class RGA2DException : public RGAException
{
public:

	/**
	 * Heuristic used for the placement failed.
	 */
	static const RGAException::tException ePlacementHeuristic=RGAException::tException(RGAException::eGA+1);

	/**
	 * Normal GA2D exception.
	 */
	static const RGAException::tException eGA2D=RGAException::tException(RGAException::eGA+2);

	/**
	* Construct the modify exception.
	* @param msg             The message.
	* @param type            Type of the exception.
	*/
	RGA2DException(const char* msg,tException type=eGA2D) : RGAException(msg,type) {}
};


//------------------------------------------------------------------------------
// Forward class declaration
class RGeoInfo;
class RObj2D;
class RObj2DContainer;
template<class cInst,class cChromo>	class RThreadData2D;
template<class cInst,class cChromo,class cFit,class cThreaData,class cInfo> class RInst2D;
template<class cInst,class cChromo,class cFit,class cThreaData,class cInfo> class RChromo2D;


}//------- End of namespace R --------------------------------------------------


//------------------------------------------------------------------------------
#endif
