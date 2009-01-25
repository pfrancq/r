/*

	R Project Library

	RVertex.h

	Vertex of a graph - Header.

	Copyright 2001-2009 by the Université Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).

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
#ifndef RVertex_H
#define RVertex_H


//------------------------------------------------------------------------------
// include files for R Project
#include <rcontainer.h>
#include <rcursor.h>
#include <redge.h>


//------------------------------------------------------------------------------
namespace R{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* The RVertex class provides a representation for a vertex.
* @author Pascal Francq
* @short Vertex.
*/
class RVertex
{
public:

	struct VertexStruct
	{
		size_t id;
		size_t idx;
		size_t nb;
	};

protected:

	/**
	* Identifier of the vertex.
	*/
	size_t Id;

	/**
	* All the edges connecting the given vertex.
	*/
	RContainer<REdge,false,false> Edges;

public:

	/**
	* Constructor of the vertex.
	* @param id              Identifier.
	* @param nb              Supposed number of edges.
	*/
	RVertex(size_t id,size_t nb=10);

	/**
	* Copy constructor.
	* @param v               Vertex used.
	*/
	RVertex(const RVertex& v);

	/**
	* Construct a vertex from a structure.
	* @param s               Structure.
	*/
	RVertex(const VertexStruct& s);

	/**
	* Is used to compare two vertices. The function returns the same type of
	* information than the strcmp function from the standard C library.
	* @param e               Edge used for the comparison.
	* @param v               Vertex used for the comparison.
	*/
	int Compare(const RVertex& v) const {return(Id-v.Id);}

	/**
	* Is used to compare a vertex and an identifier. The function returns
	* the same type of information than the strcmp function from the standard
	* C library.
	* @param e               Edge used for the comparison.
	* @param i               Identifier used for the comparison.
	*/
	int Compare(const size_t i) const {return(Id-i);}

	/**
	* Is used to compare a vertex and a structure. The function returns
	* the same type of information than the strcmp function from the standard
	* C library.
	* @param e               Edge used for the comparison.
	* @param s               Structure used for the comparison.
	*/
	int Compare(const VertexStruct& s) const {return(Id-s.id);}

	/**
	* Get the identifier of the vertex.
	*/
	size_t GetId(void) const {return(Id);}

	/**
	* Get a cursor over the edges of the graph.
	*/
	RCursor<REdge> GetEdges(void) const;

	/**
	 * @return Pointer to the edge that connects the current node with a node
	 * given by the identifier. If no edge is found, the method returns 0.
	 * @note This methods supposes an undirected graph.
	 * @param id             Identifier of the node.
	 */
	REdge* GetEdge(size_t id) const;

	/**
	 * @return Pointer to the edge that connects the current node to a node
	 * given by the identifier. If no edge is found, the method returns 0.
	 * @note This methods supposes an directed graph.
	 * @param id             Identifier of the node.
	 */
	REdge* GetEdgeTo(size_t id) const;

	/**
	 * @return Pointer to the edge that connects a node given by the identifier
	 * to the current node. If no edge is found, the method returns 0.
	 * @note This methods supposes an directed graph.
	 * @param id             Identifier of the node.
	 */
	REdge* GetEdgeFrom(size_t id) const;

	/**
	* Destruct the vertex.
	*/
	virtual ~RVertex(void);

	// friend classes
	friend class RGraph;
	friend class REdge;
};


}  //------- End of namespace R ------------------------------------------------


//------------------------------------------------------------------------------
#endif
