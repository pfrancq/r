/*

	R Project Library

	RGraph.h

	A Graph - Header.

	Copyright 2001-2011 by Pascal Francq (pascal@francq.info).
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
#ifndef RGraph_H
#define RGraph_H


//------------------------------------------------------------------------------
// include files for R Project
#include <rcontainer.h>
#include <rcursor.h>
#include <redge.h>
#include <rvertex.h>


//------------------------------------------------------------------------------
namespace R{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* The RGraph class provides a representation of a graph.
* @author Pascal Francq
* @short Graph
*/
class RGraph
{
protected:

	/**
	* The vertices of the graph.
	*/
	RContainer<RVertex,true,false> Vertices;

	/**
	* The edges of the graph.
	*/
	RContainer<REdge,true,false> Edges;

public:

	/**
	* Constructor of the graph.
	* @param nb	             Number of edges.
	*/
	RGraph(size_t nb);

	/**
	* Clear the graph and destruct the elements.
	*/
	void Clear(void);

	/**
	* Get a cursor over the vertices of the graph.
	*/
	RCursor<RVertex> GetVertices(void) const;

	/**
	* Get a cursor over the edges of the graph.
	*/
	RCursor<REdge> GetEdges(void) const;

	/**
	 * Insert a vertex. It cannot have edges.
	 * @param v              Vertex to insert.
	 */
	void Insert(RVertex* v);

	/**
	* Create a vertex.
	* @returns Pointer to the new created vertex.
	*/
	RVertex* CreateVertex(void);

	/**
	* Create a vertex with a given identifier.
	* @param id              Identifier.
	* @returns Pointer to the new created vertex.
	*/
	RVertex* CreateVertex(const size_t id);

	/**
	* Get the vertex with a given identifier. If the vertex doesn't exist,
	* it's created.
	* @param id              Identifier.
	* @returns Pointer to the vertex.
	*/
	RVertex* GetVertex(const size_t id);

	/**
	 * Insert an edge.
	 * @param e              Edge to insert.
	 */
	void Insert(REdge* e);

	/**
	* Create an edge.
	* @param v1              First Vertex.
	* @param v2              First Vertex.
	* @param w               Weight.
	*/
	REdge* CreateEdge(RVertex* v1,RVertex* v2,double w);

	/**
	* Compute the minimum spanning trees using the Prim's algorithm.
	* @param g               The graph that will hold the result.
	*/
	void MinSpanningTree(RGraph* g);

	/**
	* Destruct the graph.
	*/
	~RGraph(void);
};


}  //------- End of namespace R ------------------------------------------------


//------------------------------------------------------------------------------
#endif
