/*

	R Project Library

	REdge.h

	Edge of a graph - Header.

	Copyright 2001-2005 by the Université Libre de Bruxelles.

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
#ifndef REdge_H
#define REdge_H



//------------------------------------------------------------------------------
namespace R{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// forward class declaration
class RVertex;


//------------------------------------------------------------------------------
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
	RVertex* Vertex1;

	/**
	* Second vertex that the edge connect.
	*/
	RVertex* Vertex2;

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
	* @param v1              First Vertex.
	* @param v2              First Vertex.
	* @param w               Weight
	*/
	REdge(RVertex* v1,RVertex* v2,double w);

	/**
	* Get a pointer over the first vertex of the edge.
	*/
	RVertex* GetVertex1(void) const {return(Vertex1);}

	/**
	* Get a pointer over the second vertex of the edge.
	*/
	RVertex* GetVertex2(void) const {return(Vertex2);}

	/**
	* Get the weight of the edge.
	*/
	double GetWeight(void) const {return(Weight);}

	/**
	* Is used to compare two edges. The function returns the same type of
	* information than the strcmp function from the standard C library.
	* @param e               Edge used for the comparaison.
	*/
	int Compare(const REdge& e) const {return(this!=&e);}

	/**
	* Destructor of the edge.
	*/
	~REdge(void);

	// friend classes
	friend class RGraph;
	friend class RVertex;
};


}  //------- End of namespace R ------------------------------------------------


//------------------------------------------------------------------------------
#endif
