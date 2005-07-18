/*

	R Project Library

	RGraph.h

	A Graph - Header.

	Copyright 2001-2003 by the Universit�Libre de Bruxelles.

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
#ifndef RGraph_H
#define RGraph_H


//------------------------------------------------------------------------------
// include files for R Project
#include <rcontainer.h>
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
public:

	/**
	* The vertices of the graph.
	*/
	RContainer<RVertex,true,false> Vertices;

	/**
	* The edges of the graph.
	*/
	RContainer<REdge,true,false> Edges;

	/**
	* Constructor of the graph.
	* @param nb	            Number of edges.
	*/
	RGraph(unsigned int nb);

	/**
	* Clear the graph and destruct the elements.
	*/
	void Clear(void);

	/**
	* Create a vertex.
	* @returns Pointer to the new created vertex.
	*/
	RVertex* CreateVertex(void);

	/**
	* Create a vertex with a given identificator.
	* @param id             Identficator.
	* @returns Pointer to the new created vertex.
	*/
	RVertex* CreateVertex(const unsigned int id);

	/**
	* Get the vertex with a given identificator. If the vertex doesn't exist,
	* it's created.
	* @param id             Identficator.
	* @returns Pointer to the vertex.
	*/
	RVertex* GetVertex(const unsigned int id);

	/**
	* Create an edge.
	* @param v1             First Vertex.
	* @param v2             First Vertex.
	* @param w              Weight.
	*/
	REdge* CreateEdge(RVertex* v1,RVertex* v2,double w);

	/**
	* Compute the minimum spanning trees using the Prim's algorithm.
	* @param g              The graph that will hold the result.
	*/
	void MinSpanningTree(RGraph* g);
	
	/**
	* Destructor of the graph.
	*/
	~RGraph(void);
};


}  //------- End of namespace R ------------------------------------------------


//------------------------------------------------------------------------------
#endif
