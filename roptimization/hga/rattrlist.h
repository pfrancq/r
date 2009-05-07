/*

	R Project Library

	RAttrList.h

	Class representing a list of attributes - Header

	Copyright 1998-2009 by Pascal Francq (pascal@francq.info).
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
#ifndef RAttrList_H
#define RAttrList_H


//------------------------------------------------------------------------------
// include files for ANSI C/C++
#include <limits.h>
#include <new>
using namespace std;


//------------------------------------------------------------------------------
// include files for R Project
#include <rvectorint.h>


//------------------------------------------------------------------------------
namespace R{
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
/**
* The RAttrList class provides a representation of a list of attributes, each
* attribute is represented by a number.
* The objects and the nodes are using this class.
* @author Pascal Francq
* @short List of Attributes.
*/
class RAttrList : public RVectorInt<size_t,true>
{
public:

	/**
	* Construct the list of attributes.
	* @param MaxSize        The maximum number of attributes.
	*/
	RAttrList(const size_t MaxSize);

	/**
	* Copy constructor.
	* @param lst             List to copy.
	*/
	RAttrList(const RAttrList& lst);

	/**
	* Look if the current list of attributes is a parent of another list. To be
	* a parent, all the attributes of the current list must be present in the
	* child list.
	* @param Child          List to verify if it is child.
	* @return True if the list can be attached, false else.
	*/
	bool IsParent(const RAttrList& Child) const;

	/**
	* Verify if there is an intersection between two list of attributes.
	* @param PosAttr        The list of attribute to used for the comparison.
	* return True if there is an intersection, false else.
	*/
	bool IsInter(const RAttrList& PosAttr) const;

	/**
	* Count the number of attributes in common in the two list.
	* @param PosAttr        The list of attribute to used for the comparison.
	*/
	size_t GetNbCommon(const RAttrList& PosAttr) const;

	/**
	* Compute the difference between two lists in the current one. This means
	* all the attributes that are in the second list and not in the first one.
	* @param Top            The top list used.
	* @param Child          The child list used.
	*/
	void Diff(const RAttrList& Top,const RAttrList& Child);

	/**
	* Compute the intersection between two lists in the current one. This means
	* all the attributes that are in the two lists.
	* @param Fst            The first list.
	* @param Sec            The second list.
	*/
	void Inter(const RAttrList& Fst,const RAttrList& Sec);

	/**
	* Find the first different attribute between two lists.
	* @param PosAttr        The list used.
	* @return The attribute if one different found, NoAttr else.
	*/
	size_t FindFirstDiff(const RAttrList& PosAttr) const;

	/**
	* Assignment operator.
	* @param src            List used for the assignment.
	*/
	RAttrList& operator=(const RAttrList& src);

	/**
	* Get the number of attributes in the list.
	* @return size_t
	*/
	size_t GetNbAttr(void) const {return(GetNb());}

	/**
	 * Print the contains to the screen (separated by tabs).
	 */
	void Print(void) const;

	/**
	* Destructor of the list.
	*/
	virtual ~RAttrList(void);
};


}  //------- End of namespace R ------------------------------------------------


//------------------------------------------------------------------------------
#endif
