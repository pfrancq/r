/*

	R Project Library

	REdge.h

	Edge of a graph - Header.

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
#ifndef REDGE_H
#define REDGE_H



//-----------------------------------------------------------------------------
namespace RGraphs{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// forward class declaration
class RVertex;


//-----------------------------------------------------------------------------
/**
* The RVertex class provides a representation for an edge for a given graph.
* @author Pascal Francq
* @short Edge.
*/
class REdge
{
	/**
	* First vertex that the edge connect.
	*/
	RVertex* v1;

	/**
	* Second vertex that the edge connect.
	*/
	RVertex* v2;

	/**
	* Weight of the vertex.
	*/
	double Weight;

public:

	/**
	* Constructor of the edge.
	*/
	REdge(void);

	/**
	* Constructor of the edge.
	* @param _v1		First Vertex.
	* @param _v2		First Vertex.
	* @param w			Weight
	*/
	REdge(RVertex* _v1,RVertex* _v2,double w);

	/**
	* Is used to compare two edges. The function returns the same type of
	* information than the strcmp function from the standard C library.
	* @param e		Edge used for the comparaison.
	*/
	int Compare(const REdge& e) {return(this!=&e);}

	/**
	* Is used to compare two edges. The function returns the same type of
	* information than the strcmp function from the standard C library.
	* @param e		Edge used for the comparaison.	
	*/
	int Compare(const REdge* e) {return(this!=e);}

	/**
	* Destructor of the edge.
	*/
	~REdge(void);

	// friend classes
	friend class RGraph;
	friend class RVertex;
};


}  //------- End of namespace RGraphs -----------------------------------------


//-----------------------------------------------------------------------------
#endif
