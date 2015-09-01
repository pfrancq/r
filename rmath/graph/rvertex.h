/*

	R Project Library

	RVertex.h

	Vertex of a graph - Header.

	Copyright 2001-2015 by Pascal Francq (pascal@francq.info).
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
#ifndef RVertex_H
#define RVertex_H


//------------------------------------------------------------------------------
// include files for R Project
#include <redge.h>
#include <rgenericvertex.h>


//------------------------------------------------------------------------------
namespace R{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* The RVertex class provides a representation for a vertex.
* @short Vertex.
*/
class RVertex : public RGenericVertex<RVertex,REdge>
{
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
	* Is used to compare two vertices. The function returns the same type of
	* information than the strcmp function from the standard C library.
	* @param v               Vertex used for the comparison.
	*/
	int Compare(const RVertex& v) const {return(Id-v.Id);}
};


}  //------- End of namespace R ------------------------------------------------


//------------------------------------------------------------------------------
#endif
