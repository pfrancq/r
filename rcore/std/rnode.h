/*

  RNode.h

  Generic Node - Header.

  (C) 1999-2000 by P. Francq.

  Version $Revision$

  Last Modify: $Date$

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

*/



//---------------------------------------------------------------------------
#ifndef RNodeH
#define RNodeH


//---------------------------------------------------------------------------
#include "rcontainer.h"
using namespace RStd;


//---------------------------------------------------------------------------
namespace RStd{                      // Using RStd namespace
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
/** This class represent a generic node. The user has to derived from this class
	* and implement the Compare function. The paramter N specify the real class and
	* the parameter bOrder specify if the nodes has to be ordered. Here is an example:
	* <pre>
	* class MyNoOrderNode : public RNode<MyNoOrderNode,false>
	*	{
	*	public:
	*		MyNoOrderNode(unsigned int max,unsigned int inc);
	*		int Compare(MyNoOrderNode *node);
	*	};
	* </pre>
	* @author Pascal Francq
	* @short Generic Node.
	*/
template<class N,bool bOrder>
	class RNode	: public RContainer<N,unsigned int,false,bOrder>
{
public:
	/** Parent Node.*/
  N *Parent;

  /** Construct the node.
  	* @param max	The size of initial array of pointer to child.
  	* @param inc	The increment size for the array.
  	*/
  RNode(unsigned int max,unsigned int inc) throw(bad_alloc);

  /** Is used to compare two nodes. The function returns the same type of
  	* information than the strcmp function from the standard C library.
  	*/
  virtual int Compare(RNode*);

  /** Destruct the node.*/
  virtual ~RNode(void);
};


#include "rnode.hh"			// Template implementation


}	//---- End namespace RStd -------------------------------------------------
#endif
