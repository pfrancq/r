/*

	R Project Library

	RGenericVertex.h

	Generic vertex of a graph - Header.

	Copyright 2001-2012 by Pascal Francq (pascal@francq.info).
	Copyright 2001-2008 by the Université Libre de Bruxelles (ULB).

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
#ifndef RGenericVertex_H
#define RGenericVertex_H


//------------------------------------------------------------------------------
// include files for R Project
#include <rcontainer.h>
#include <rcursor.h>


//------------------------------------------------------------------------------
namespace R{
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//	forward declaration
template<class V,class E> class RGenericEdge;


//------------------------------------------------------------------------------
/**
* The RGenericVertex class provides a representation for a generic vertex.
* @tparam V                  Class representing a vertex. It must inherits from
*                            RVertex.
* @tparam E                  Class representing an edge. It must inherits from
*                            REdge.
* @author Pascal Francq
* @short Generic Vertex.
*/
template<class V,class E>
	class RGenericVertex
{
protected:

	/**
	* Identifier of the vertex.
	*/
	size_t Id;

	/**
	* All the edges connecting the given vertex.
	*/
	RContainer<E,false,false> Edges;

public:

	/**
	* Constructor of the vertex.
	* @param id              Identifier.
	* @param nb              Supposed number of edges.
	*/
	RGenericVertex(size_t id,size_t nb=10);

	/**
	* Copy constructor.
	* @param v               Vertex used.
	*/
	RGenericVertex(const RGenericVertex& v);

	/**
	* Is used to compare two vertices. The function returns the same type of
	* information than the strcmp function from the standard C library.
	* @param v               Vertex used for the comparison.
	*/
	int Compare(const RGenericVertex& v) const {return(Id-v.Id);}

	/**
	* Is used to compare a vertex and an identifier. The function returns
	* the same type of information than the strcmp function from the standard
	* C library.
	* @param i               Identifier used for the comparison.
	*/
	int Compare(const size_t i) const {return(Id-i);}

	/**
	* Get the identifier of the vertex.
	*/
	size_t GetId(void) const {return(Id);}

	/**
	* Get a cursor over the edges of the graph.
	*/
	RCursor<E> GetEdges(void) const;

	/**
	 * Get the number of edges to which the vertex is connected.
    * @return the number of adjacent edges.
    */
	size_t GetNbEdges(void) const {return(Edges.GetNb());}

	/**
	 * @return Pointer to the edge that connects the current node with a node
	 * given by the identifier. If no edge is found, the method returns 0.
	 * @note This methods supposes an undirected graph.
	 * @param id             Identifier of the node.
	 */
	E* GetEdge(size_t id) const;

	/**
	 * @return Pointer to the edge that connects the current node to a node
	 * given by the identifier. If no edge is found, the method returns 0.
	 * @note This methods supposes an directed graph.
	 * @param id             Identifier of the node.
	 */
	E* GetEdgeTo(size_t id) const;

	/**
	 * @return Pointer to the edge that connects a node given by the identifier
	 * to the current node. If no edge is found, the method returns 0.
	 * @note This methods supposes an directed graph.
	 * @param id             Identifier of the node.
	 */
	E* GetEdgeFrom(size_t id) const;


	/**
	 * Find all the adjacent vertices from the current one. In practice, it
	 * parses the edges and add the other vertex to the container.
    * @param vertices        Container that will hold the adjacent vertices.
	 * @param verify          Specify if the method should verify that that a
	 *                        given vertex is not stored twice. If true, it is
	 *                        better to pass an ordered container.
	 * @tparam bOrder         Specify if the vertices are ordered in the
	 *                        container.
	 * @attention The container is not cleared by the method before vertices are
	 * added.
    */
	template<bool bOrder>
		void GetAdjacentVertices(RContainer<V,false,bOrder>& vertices,bool verify=false) const;

	/**
	* Destruct the vertex.
	*/
	virtual ~RGenericVertex(void);

	// friend classes
	friend class RGenericEdge<V,E>;
};


//-----------------------------------------------------------------------------
// Template implementation
#include <rgenericvertex.hh>


}  //------- End of namespace R ------------------------------------------------


//------------------------------------------------------------------------------
#endif
