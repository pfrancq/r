/*

	R Project Library

	RVertex.h

	Vertex of a graph - Header.

	Copyright 2001-2003 by the Université Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).

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



//------------------------------------------------------------------------------
#ifndef RVertex_H
#define RVertex_H


//------------------------------------------------------------------------------
// include files for R Project
#include <rstd/rcontainer.h>
#include <rmath/redge.h>


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
		unsigned int id;
		unsigned int idx;
		unsigned int nb;
	};


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

	/**
	* Constructor of the vertex.
	* @param id             Identificator.
	* @param idx            Index.
	* @param nb             Number of edges.
	*/
	RVertex(const unsigned int id,const unsigned int idx,const unsigned int nb);

	/**
	* Copy constructor.
	* @param v              Vertex used.
	*/
	RVertex(const RVertex* v);

	/**
	* Copy constructor.
	* @param v              Vertex used.
	*/
	RVertex(const RVertex& v);

	/**
	* Construct a vertex from a structure.
	*/
	RVertex(const VertexStruct& s);

	/**
	* Is used to compare two vertices. The function returns the same type of
	* information than the strcmp function from the standard C library.
	* @param e              Edge used for the comparaison.
	*/
	int Compare(const RVertex& v) {return(this!=&v);}

	/**
	* Is used to compare two vertices. The function returns the same type of
	* information than the strcmp function from the standard C library.
	* @param e              Edge used for the comparaison.	
	*/
	int Compare(const RVertex* v) {return(this!=v);}
	
	/**
	* Is used to compare a vertex and an identificator. The function returns
	* the same type of information than the strcmp function from the standard
	* C library.
	* @param i              Identificator used for the comparaison.
	*/
	int Compare(const unsigned int i) {return(Id-i);}

	/**
	* Is used to compare a vertex and a structure. The function returns
	* the same type of information than the strcmp function from the standard
	* C library.
	* @param s              Structure used for the comparaison.
	*/
	int Compare(const VertexStruct& s) {return(Id-s.id);}

	/**
	* Destructor of the vertex.
	*/
	~RVertex(void);

	// friend classes
	friend class RGraph;
};


}  //------- End of namespace R ------------------------------------------------


//------------------------------------------------------------------------------
#endif
