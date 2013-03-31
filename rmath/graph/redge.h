/*

	R Project Library

	REdge.h

	Edge of a graph - Header.

	Copyright 2001-2008 by Pascal Francq (pascal@francq.info).
	Copyright 2001-2011 by the Université Libre de Bruxelles (ULB).

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



//------------------------------------------------------------------------------
#ifndef REdge_H
#define REdge_H


//------------------------------------------------------------------------------
// include files for R
#include <rstd.h>


//------------------------------------------------------------------------------
namespace R{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// forward class declaration
class RVertex;


//------------------------------------------------------------------------------
/**
* The RVertex class provides a representation for an edge from one vertex to
* another one in a given graph. The edge is directed but may be considered as
* undirected in some situations.
* @author Pascal Francq
* @short Edge.
*/
class REdge
{
	/**
	* Origin of the edge.
	*/
	RVertex* From;

	/**
	* Destination of the edge.
	*/
	RVertex* To;

	/**
	* Weight of the vertex.
	*/
	double Weight;

public:

	/**
	* Constructor of the edge.
	* @deprecated This constructor will disappear in a near future.
	*/
	REdge(void);

	/**
	* Constructor of the edge.
	* @param f               Vertex starting the edge.
	* @param t               Vertex ending the edge.
	* @param w               Weight
	*/
	REdge(RVertex* f,RVertex* t,double w);

	/**
	* The methods used to compare two edges. The function returns the same type
	* of information than the strcmp function from the standard C library.
	* @see R::RContainer.
	* @param e               Edge used for the comparison.
	*/
	int Compare(const REdge& e) const {return(this!=&e);}

	/**
	* @return a pointer over the starting vertex of the edge.
	*/
	RVertex* GetFrom(void) const {return(From);}

	/**
	* @return pointer over the ending vertex of the edge.
	*/
	RVertex* GetTo(void) const {return(To);}

	/**
	* @return a pointer over the starting vertex of the edge.
	* \deprecated Use GetFrom() instead.
	*/
	RVertex* GetVertex1(void) const {return(From);}

	/**
	* @return pointer over the ending vertex of the edge.
	* \deprecated Use GetTo() instead.
	*/
	RVertex* GetVertex2(void) const {return(To);}

	/**
	 * @return true if the edge connects the node given by the identifier.
	 * @param id             Identifier.
	 */
	bool Connects(size_t id) const;

	/**
	* Get the weight of the edge.
	*/
	double GetWeight(void) const {return(Weight);}

	/**
	* Destruct the edge.
	*/
	virtual ~REdge(void);

	// friend classes
//	friend class RGraph<RVertex,REdge,true,true>;
//	friend class RGraph<RVertex,REdge,true,false>;
//	friend class RGraph<RVertex,REdge,false,true>;
//	friend class RGraph<RVertex,REdge,false,false>;
	friend class RVertex;
};


}  //------- End of namespace R ------------------------------------------------


//------------------------------------------------------------------------------
#endif
