/*

	R Project Library

	RGenericEdge.h

	Generic edge of a graph - Header.

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
#ifndef RGenericEdge_H
#define RGenericEdge_H


//------------------------------------------------------------------------------
// include files for R
#include <rexception.h>


//------------------------------------------------------------------------------
namespace R{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
//	forward declaration
template<class V,class E> class RGenericVertex;
template<class V,class E,bool bAllocVertices,bool bAllocEdges> class RGraph;


//------------------------------------------------------------------------------
/**
* The RGenericVertex class provides a representation for a generic edge from one
*  vertex to another one in a given graph. The edge is directed but may be
* considered as undirected in some situations.
* @tparam V                  Class representing a vertex. It must inherits from
*                            RVertex.
* @tparam E                  Class representing an edge. It must inherits from
*                            REdge.
* @author Pascal Francq
* @short Generic Edge.
*/
template<class V,class E>
	class RGenericEdge
{
	/**
	* Origin of the edge.
	*/
	V* From;

	/**
	* Destination of the edge.
	*/
	V* To;

	/**
	* Weight of the edge.
	*/
	double Weight;

public:

	/**
	* Constructor of the edge.
	* @param f               Vertex starting the edge.
	* @param t               Vertex ending the edge.
	* @param w               Weight
	*/
	RGenericEdge(V* f,V* t,double w);

	/**
	* The methods used to compare two edges. The function returns the same type
	* of information than the strcmp function from the standard C library.
	* @see R::RContainer.
	* @param e               Edge used for the comparison.
	*/
	int Compare(const RGenericEdge& e) const {return(this!=&e);}

	/**
	* Get the starting vertex of the edge.
	* @return a pointer.
	*/
	inline V* GetFrom(void) const {return(From);}

	/**
	* Get the ending vertex of the edge.
	* @return a pointer.
	*/
	inline V* GetTo(void) const {return(To);}

	/**
	 * Look if the edge connects the node given by the identifier.
	 * @param id             Identifier.
	 * @return true if the connects a node.
	 */
	bool IsConnect(size_t id) const;

	/**
	* Get the weight of the edge.
	*/
	inline double GetWeight(void) const {return(Weight);}

	/**
	* Set the weight of the edge.
	* @param weight          Weight to assign.
	*/
	inline void GetWeight(double weight) {Weight=weight;}

	/**
	* Destruct the edge. It is eventually removed from the corresponding
	* vertices.
	*/
	virtual ~RGenericEdge(void);

	// friend classes
	friend class RGenericVertex<V,E>;
	friend class RGraph<V,E,true,true>;
	friend class RGraph<V,E,true,false>;
	friend class RGraph<V,E,false,true>;
	friend class RGraph<V,E,false,false>;
};


//-----------------------------------------------------------------------------
// Template implementation
#include <rgenericedge.hh>


}  //------- End of namespace R ------------------------------------------------


//------------------------------------------------------------------------------
#endif
