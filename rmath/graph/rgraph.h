/*

	R Project Library

	RGraph.h

	A Graph - Header.

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
#ifndef RGraph_H
#define RGraph_H


//------------------------------------------------------------------------------
// include files for ANSI C/C++
#include <limits>


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
* The RGraph class provides a template that represents of a graph.
* @tparam V                  Class representing a vertex. It must inherits from
*                            RVertex.
* @tparam E                  Class representing an edge. It must inherits from
*                            REdge.
* @tparam bAllocVertices     Specify if the vertices are deallocated by the
*                            graph.
* @tparam bAllocEdges        Specify if the edges are deallocated by the graph.
* @author Pascal Francq
* @short Graph
*
* In practice, the RVertex and REdge classes can be used directly to initialize
* an instance:
* @code
* int main()
* {
*    // Create a graph
*    RGraph<RVertex,REdge,true,true> Graph(5);
*    RVertex* v1(Graph.CreateVertex());
*    RVertex* v2(Graph.CreateVertex());
*    RVertex* v3(Graph.CreateVertex());
*    Graph.CreateEdge(v1,v2,2.0);
*    Graph.CreateEdge(v2,v3,1.0);
*    Graph.CreateEdge(v2,v3,5.0);
*
*    // Compute the minimum spanning tree
*    RGraph<RVertex,REdge,true,true> MinTree(5);
*	  Graph.MinSpanningTree(&MinTree);
* }
* @endcode
*/
template<class V,class E,bool bAllocVertices=true,bool bAllocEdges=true>
	class RGraph
{
protected:

	/**
	* The vertices of the graph.
	*/
	RContainer<V,bAllocVertices,false> Vertices;

	/**
	* The edges of the graph.
	*/
	RContainer<E,bAllocEdges,false> Edges;

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
	RCursor<V> GetVertices(void) const;

	/**
	* Get a cursor over the edges of the graph.
	*/
	RCursor<E> GetEdges(void) const;

	/**
	 * Insert a vertex. It cannot have edges.
	 * @param v              Vertex to insert.
	 */
	void Insert(V* v);

	/**
	* Create a vertex.
	* @returns Pointer to the new created vertex.
	*/
	virtual V* CreateVertex(void);

	/**
	* Create a vertex with a given identifier.
	* @param id              Identifier.
	* @returns Pointer to the new created vertex.
	*/
	virtual V* CreateVertex(const size_t id);

	/**
	* Get the vertex with a given identifier. If the vertex doesn't exist,
	* it's created.
	* @param id              Identifier.
	* @returns Pointer to the vertex.
	*/
	V* GetVertex(const size_t id);

	/**
	 * Insert an edge.
	 * @param e              Edge to insert.
	 */
	void Insert(E* e);

	/**
	* Create an edge.
	* @param v1              First Vertex.
	* @param v2              First Vertex.
	* @param w               Weight.
	*/
	virtual E* CreateEdge(V* v1,V* v2,double w);

	/**
	* Compute the minimum spanning trees using the Prim's algorithm.
	* @param g               The graph that will hold the result.
	*/
	void MinSpanningTree(RGraph* g);

	/**
	* Destruct the graph.
	*/
	virtual ~RGraph(void);
};


//-----------------------------------------------------------------------------
// Template implementation
#include <rgraph.hh>


}  //------- End of namespace R ------------------------------------------------


//------------------------------------------------------------------------------
#endif
