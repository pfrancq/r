/*

	R Project Library

	RGGA.h

	Grouping Genetic Algorithms - Header.

	(C) 2001 by P. Francq.

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
#ifndef RGGAH
#define RGGAH


//-----------------------------------------------------------------------------
// include files for RProject
#include <rga/rga.h>


//-----------------------------------------------------------------------------
/**
* \namespace RGGA
* \brief Generic Grouping Genetic Algorithm Classes.
*
* This namespace declares classes for working with Grouping Genetic Algorithms.
*/


//-----------------------------------------------------------------------------
namespace RGGA{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Forward class declaration
class RObjG;
template<class cGroup,class cObj,class cGroupData,class cChromo> class RGroup;
template<class cGroup,class cObj,class cGroupData,class cChromo> class RGroups;
template<class cGroup,class cObj,class cGroupData,class cGroups> class RGroupingHeuristic;
template<class cInst,class cChromo,class cFit,class cThreadData,class cGroup,class cObj,class cGroupData> class RThreadDataG;
template<class cInst,class cChromo,class cFit,class cThreadData,class cGroup,class cObj,class cGroupData> class RChromoG;
template<class cInst,class cChromo,class cFit,class cThreadData,class cGroup,class cObj,class cGroupData> class RInstG;


//-----------------------------------------------------------------------------
// Heuristic Types
enum HeuristicType{Nothing,FirstFit,FirstFitDes};


//-----------------------------------------------------------------------------
/**
* The eGAModify class provides an exception fot the modify operator.
* @author Pascal Francq
* @short Modify GA Exception
*/
class eGAModify : public RGA::eGA
{
public:

	/**
	* Construct the modify exception.
	* @param g              Generation.
	* @param c              Identificator of the child.
	*/
	eGAModify(const unsigned int g,const unsigned int c);

	/**
	* Construct the modify exception.
	* @param msg            The message.
	*/
	eGAModify(const char* msg) : eGA(msg) {}
};


//-----------------------------------------------------------------------------
/**
* The eGALocalOptimisation class provides an exception for the local
* optimisation process.
* @author Pascal Francq
* @short Local Optimisation GA Exception
*/
class eGALocalOptimisation : public RGA::eGA
{
public:

	/**
	* Construct the local optimisation exception.
	* @param g              Generation.
	* @param c              Identificator of the child.
	*/
	eGALocalOptimisation(const unsigned int g,const unsigned int c);

	/**
	* Construct the local optimisation exception.
	* @param msg            The message.
	*/
	eGALocalOptimisation(const char* msg) : eGA(msg) {}
};


}//------- End of namespace RGGA ----------------------------------------------


//-----------------------------------------------------------------------------
#endif
