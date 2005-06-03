/*

	R Project Library

	rmsg.h

	Message systems - Header.

	Copyright 2001-2004 by the Université Libre de Bruxelles.

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
#ifndef RMsgH
#define RMsgH


//------------------------------------------------------------------------------
// include files for R Project
#include <rstd/rstring.h>
#include <rstd/rcontainer.h>


//------------------------------------------------------------------------------
namespace R{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* The RMsg class provides a representation for a message that can be leave by
* some objects and that can be retrieve by others.
* @author Pascal Francq
* @short Message.
*/
class RMsg
{
	/**
	* The contain of the message.
	*/
	RString Msg;

public:

	/**
	* Construct the message.
	* @param msg            Contain.
	*/
	RMsg(const RString& msg);

	/**
	* Compare two messages by comparing their content.
	* @see R::RContainer
	* @param msg            Message.
	* @return int
	*/
	int Compare(const RMsg& msg) const {return(Msg.Compare(msg.Msg));}

	/**
	* Compare a message with a given string.
	* @see R::RContainer
	* @param msg            RString.
	* @return int
	*/
	int Compare(const RString& msg) const {return(Msg.Compare(msg));}

	/**
	* Equal operator between messages.
	* @param msg            Message.
	* @return true if equal, false else.
	*/
	bool operator==(const RMsg& msg) const
		{return(Msg==msg.Msg);}

	/**
	* Equal operator between a message and a string.
	* @param msg            RString.
	* @return true if equal, false else.
	*/
	bool operator==(const RString& msg) const
		{return(Msg==msg);}

	/**
	* Look after a specific message.
	* @param msg            String.
	* @return Pointer to the specific message, or 0 if not found.
	*/
	static RMsg* LookMsg(const RString& msg);

	/**
	* Insert a new message.
	* @param msg            String representing the message.
	* @return Pointer to the message inserted.
	*/
	static RMsg* InsertMsg(const RString& msg);

	/**
	* Delete a message.
	* @param msg            Message.
	*/
	static void DeleteMsg(RMsg* msg);

	/**
	* Destruct the message.
	*/
	~RMsg(void);
};


}  //-------- End of namespace R -----------------------------------------------


//------------------------------------------------------------------------------
#endif
