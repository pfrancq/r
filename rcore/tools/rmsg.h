/*

	R Project Library

	rmsg.h

	Message systems - Header.

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
#ifndef RMsgH
#define RMsgH


//-----------------------------------------------------------------------------
// include files for R Project
#include <rstd/rstring.h>
#include <rstd/rcontainer.h>


//-----------------------------------------------------------------------------
namespace RStd{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
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
	* Construct the message.
	* @param msg            Contain.
	*/
	RMsg(const char* msg);
	
	/**
	* Compare function like strcmp used in particular for RContainer class.
	*/
	int Compare(const RMsg& msg) {return(Msg.Compare(msg.Msg));}

	/**
	* Compare function like strcmp used in particular for RContainer class.
	*/
	int Compare(const RMsg* msg) {return(Msg.Compare(msg->Msg));}

	/**
	* Compare function like strcmp used in particular for RContainer class.
	*/
	int Compare(const RString& msg) {return(Msg.Compare(msg));}

	/**
	* Compare function like strcmp used in particular for RContainer class.
	*/
	int Compare(const char* msg) {return(Msg.Compare(msg));}

	/**
	* Equal operator.
	*/
	bool operator==(const RMsg& msg) const
		{return(Msg==msg.Msg);}

	/**
	* Equal operator.
	*/
	bool operator==(const RString& msg) const
		{return(Msg==msg);}

	/**
	* Equal operator.
	*/
	bool operator==(const char* msg) const
		{return(Msg==msg);}

	/**
	* Look after a specific message.
	* @param msg	Message.
	* @returns Pointer to the specific message, or 0 if not found.
	*/
	static RMsg* LookMsg(const RString& msg);

	/**
	* Look after a specific message.
	* @param msg	Message.
	* @returns Pointer to the specific message, or 0 if not found.
	*/
	static RMsg* LookMsg(const char* msg);

	/**
	* Insert a new message.
	* @param msg	Message.
	*/
	static RMsg* InsertMsg(const RString& msg) throw(bad_alloc);

	/**
	* Insert a new message.
	* @param msg	Message.
	*/
	static RMsg* InsertMsg(const char* msg) throw(bad_alloc);

	/**
	* Delete a message.
	* @param msg	Message.
	*/
	static void DeleteMsg(RMsg* msg) throw(bad_alloc);

	/**
	* Destruct the message.
	*/
	~RMsg(void);
};


}  //-------- End of namespace RStd -------------------------------------------


//-----------------------------------------------------------------------------
#endif
