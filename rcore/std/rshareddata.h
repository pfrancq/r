/*

	R Project Library

	RSharedData.h

	Generic class representing shared data - Header.

	Copyright 2004-2005 by the Université Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).

	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU Library General Public
	License as published by the Free Software Foundation; either
	version 2.0 of the License, or (at your option) any later version.

	This library is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
	Library General Public License for more details.

	You should have received a copy of the GNU Library General Public
	License along with this library, as a file COPYING.LIB; if not, write
	to the Free Software Foundation, Inc., 59 Temple Place, Suite 330,
	Boston, MA  02111-1307  USA

*/



//------------------------------------------------------------------------------
#ifndef RSharedData_H
#define RSharedData_H


//------------------------------------------------------------------------------
namespace R{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* This class implements a generic class representing data that will be shared
* between several object.
* @author Pascal Francq
* @short Generic Shared Data
*/
class RSharedData
{
	/**
	* Number of references of the data.
	*/
	unsigned int Refs;

public:

	/**
	* Constructor.
	*/
	RSharedData(void) : Refs(1) {}

	/**
	* Increment the number of references of the data.
	*/
	void IncRef(void) {Refs++;}

	/**
	* Decrement the number of references of the data.
	* return true if there is no more references to this data.
	*/
	bool DecRef(void) {return(!--Refs);}

	/**
	* Get the number of references to the data.
	*/
	unsigned int GetRefs(void) {return(Refs);}

	/**
	* Destructor.
	*/
	~RSharedData(void) {}
};


//------------------------------------------------------------------------------
/**
* Function the increase the number of references of a RShareData object.
* @param D                  Type of the shared data.
* @param data               Pointer to the data.
*/
template <class D>
	void RIncRef(D* data)
{
	if(data)
		data->IncRef();
}


//------------------------------------------------------------------------------
/**
* Function the decrease the number of references of a RShareData object. If there
* are no more references, the data is destroy and the pointer is set to 0.
* @param D                  Type of the shared data.
* @param data               Reference to a pointer to the data.
*/
template<class D>
	void RDecRef(D* data)
{
	if(data&&data->DecRef())
	{
		delete data;
		data = 0;
	}
}


}  //-------- End of namespace R -----------------------------------------------


//------------------------------------------------------------------------------
#endif
