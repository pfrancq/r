/*

	R Project Library

	RGGA.h

	Grouping Genetic Algorithms - Header.

	Copyright 2001-2010 by Pascal Francq (pascal@francq.info).
	Copyright 2001-2008 by the Université Libre de Bruxelles (ULB).

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
#ifndef RGGA_H
#define RGGA_H


//------------------------------------------------------------------------------
// include files for RProject
#include <rga.h>


//------------------------------------------------------------------------------
namespace R{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// Forward class declaration
class RObjG;
template<class cGroup,class cObj,class cGroups> class RGroup;
template<class cGroup,class cObj,class cGroups> class RGroups;
template<class cGroup,class cObj,class cGroups> class RGroupingHeuristic;
template<class cInst,class cChromo,class cFit,class cThreadData,class cGroup,class cObj> class RThreadDataG;
template<class cInst,class cChromo,class cFit,class cThreadData,class cGroup,class cObj> class RChromoG;
template<class cInst,class cChromo,class cFit,class cThreadData,class cGroup,class cObj> class RInstG;


//------------------------------------------------------------------------------
/**
* The RGGAException class provides an exception generated by the GGA.
* @author Pascal Francq
* @short GGA Exception
*/
class RGGAException : public RGAException
{
public:

	/**
	 * Modify operator failed.
	 */
	static const RGAException::tException eGAModify=RGAException::tException(RGAException::eGA+1);

	/**
	 * Local optimization failed.
	 */
	static const RGAException::tException eGALocalOptimisation=RGAException::tException(RGAException::eGA+2);

	/**
	 *  Normal GGA exception.
	 */
	static const RGAException::tException eGGA=RGAException::tException(RGAException::eGA+3);

	/**
	* Construct the modify exception.
	* @param type            Type of the exception.
	* @param g               Generation.
	* @param c               Identificator of the child.
	*/
	RGGAException(tException type,const size_t g,const size_t c);

	/**
	* Construct the modify exception.
	* @param msg             The message.
	* @param type            Type of the exception.
	*/
	RGGAException(const char* msg,tException type=eGGA) : RGAException(msg,type) {}
};


}//------- End of namespace R --------------------------------------------------


//------------------------------------------------------------------------------
#endif
