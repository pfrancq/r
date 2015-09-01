/*

	R Project Library

	RTrace.h

	Trace system - Header.

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
#ifndef RTraceH
#define RTraceH


//------------------------------------------------------------------------------
// include files for R Project
#include <rstring.h>


//------------------------------------------------------------------------------
namespace R{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* The RTrace class provides a representation for a message that can be leave by
* some objects and that can be retrieve by others. This system can be used for
* debugging purposes to handle inter-dependency problems.
* @code
* // Library A
* #include <rtrace.h>
* using namespace R;
*
* void SomeWhereInTheCode(int money)
* {
*    ...
*    if(!money)
*       RTrace::InsertMsg("Special situation : No money");
*    ...
* }
*
*
* // Library B
* #include <rtrace.h>
* using namespace R;
* void SomeWhereElseInTheCode(void)
* {
*    ...
*    RTrace* msg=RTrace::LookMsg("Special situation : No money");
*    if(msg)
*    {
*       cout<<"Put here a breakpoint"<<endl;
*       RTrace::DeleteMsg(msg);     // Situation handled
*    }
*    ...
* }
* @endcode
* @short Trace System.
*/
class RTrace
{
	/**
	* The message to trace.
	*/
	RString Msg;

public:

	/**
	* Construct the tracing of a message.
	* @param msg            Contain.
	*/
	RTrace(const RString& msg);

	/**
	* Compare two messages by comparing their content.
	* @see R::RContainer
	* @param msg            Message.
	* @return int
	*/
	int Compare(const RTrace& msg) const {return(Msg.Compare(msg.Msg));}

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
	bool operator==(const RTrace& msg) const
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
	static RTrace* LookMsg(const RString& msg);

	/**
	* Insert a new message.
	* @param msg            String representing the message.
	* @return Pointer to the message inserted.
	*/
	static RTrace* InsertMsg(const RString& msg);

	/**
	* Delete a message.
	* @param msg            Message.
	*/
	static void DeleteMsg(RTrace* msg);

	/**
	* Destruct the message.
	*/
	~RTrace(void);
};


}  //-------- End of namespace R -----------------------------------------------


//------------------------------------------------------------------------------
#endif
