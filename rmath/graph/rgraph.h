/*

	R Project Library

	RGraph.h

	A Graph - Header.

	(c) 2001 by P. Francq.

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



//-----------------------------------------------------------------------------
#ifndef RGraphH
#define RGraphH


//-----------------------------------------------------------------------------
// include files for R Project
#include <rstd/rcontainer.h>
using namespace RStd;
#include <rgraph/redge.h>
#include <rgraph/rvertex.h>


//-----------------------------------------------------------------------------
namespace RGraphs{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/**
* The RGraph class provides a representation of a graph.
* @author Pascal Francq
* @short Graph
*/
class RGraph
{
	/**
	* The vertices of the graph.
	*/
	RContainer<RVertex,unsigned int,true,false> Vertices;

	/**
	* The edges of the graph.
	*/
	RContainer<REdge,unsigned int,true,false> Edges;

public:
	/**
	* Constructor of the graph.
	* @param nb		Number of edges.
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
	* @param id		Identficator.
	* @returns Pointer to the new created vertex.
	*/
	RVertex* CreateVertex(const unsigned int id);

	/**
	* Create an edge.
	* @param v1		First Vertex.
	* @param v2		First Vertex.
	* @param w		Weight.
	*/
	REdge* CreateEdge(RVertex* v1,RVertex* v2,double w);

	/**
	* Compute the minimum spanning trees using the Prim's algorithm.
	* @param g		The graph that will hold the result.
	*/
	void MinSpanningTree(RGraph* g);
	
	/**
	* Destructor of the graph.
	*/
	~RGraph(void);
};


}  //------- End of namespace RGraphs -----------------------------------------


//-----------------------------------------------------------------------------
#endif
