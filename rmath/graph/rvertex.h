/*

	R Project Library

	RVertex.h

	Vertex of a graph - Header.

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
#ifndef RVertexH
#define RVertexH


//-----------------------------------------------------------------------------
// include files for R Project
#include <rstd/rcontainer.h>
using namespace RStd;
#include <rgraph/redge.h>


//-----------------------------------------------------------------------------
namespace RGraphs{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/**
* The RVertex class provides a representation for a vertex.
* @author Pascal Francq
* @short Vertex.
*/
class RVertex
{
	/**
	* Identificator of the vertex.
	*/
	unsigned int Id;

	/**
	* Index of the vertex in the graph's container.
	*/
	unsigned int Index;

	/**
	* All the edges connecting the given vertex.
	*/
	RContainer<REdge,unsigned int,false,false> Edges;

public:

	/**
	* Constructor of the vertex.
	* @param id		Identificator.
	* @param idx	Index.
	* @param nb		Number of edges.
	*/
	RVertex(const unsigned int id,const unsigned int idx,const unsigned int nb);

	/**
	* Copy constructor.
	* @param v		Vertex used.
	*/
	RVertex(const RVertex* v);

	/**
	* Copy constructor.
	* @param v		Vertex used.
	*/
	RVertex(const RVertex& v);

	/**
	* Is used to compare two vertices. The function returns the same type of
	* information than the strcmp function from the standard C library.
	* @param e		Edge used for the comparaison.
	*/
	int Compare(const RVertex& v) {return(this!=&v);}

	/**
	* Is used to compare two vertices. The function returns the same type of
	* information than the strcmp function from the standard C library.
	* @param e		Edge used for the comparaison.	
	*/
	int Compare(const RVertex* v) {return(this!=v);}
	
	/**
	* Is used to compare a vertex and an identificator. The function returns
	* the same type of information than the strcmp function from the standard
	* C library.
	* @param i		Identificator used for the comparaison.
	*/
	int Compare(const unsigned int i) {return(Id-i);}

	/**
	* Destructor of the vertex.
	*/
	~RVertex(void);

	// friend classes
	friend class RGraph;
};


}  //------- End of namespace RGraphs -----------------------------------------


//-----------------------------------------------------------------------------
#endif