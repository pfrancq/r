/*

	R Project Library

	RNotification.h

	Generic notification - Header.

	Copyright 2006 by Pascal Francq.

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



//-----------------------------------------------------------------------------
#ifndef RNotification_H
#define RNotification_H


//-----------------------------------------------------------------------------
// include files for R Project
#include <rcstring.h>
#include <rstring.h>
#include <rshareddata.h>



//-----------------------------------------------------------------------------
#define HANDLER(handler) \
	reinterpret_cast<tNotificationHandler>(&handler)


//-----------------------------------------------------------------------------
namespace R{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// foward class declaration
class RObject;


//-----------------------------------------------------------------------------
/**
* The tNotificationType provides a representation for a unique identifier representing a
* notification. It is computed using a hashing function on the name of the
* notification.
* @short Notification type
* @author Pascal Francq
*/
typedef unsigned int tNotificationType;


//-----------------------------------------------------------------------------
/**
* The RNotification provides a representation for a notification that can be send by an
* object.
* @short Message
* @author Pascal Francq
*/
class RNotification
{
	/**
	* Name of the notification.
	*/
	RCString Name;

	/**
	* Type of the notification.
	*/
	tNotificationType Type;

	/**
	* Object that has send the notification.
	*/
	RObject* Sender;

	/**
	* Data associated to the notification.
	*/
	union
	{
		long LongValue;
		unsigned long ULongValue;
		long long LongLongValue;
		unsigned long long ULongLongValue;
		double DoubleValue;
		char* CharSValue;
		RString* StringSValue;
		void* VoidSValue;
	};

public:

	/**
	* Build an hash key for a notification name.
	* @param name            Name of the notification.
	* @return Type of the notification.
	*/
	static tNotificationType GetType(const RCString& name);

	/**
	* Constructor of a notification.
	* @param name            Name of the notification.
	* @param sender          Object sending the notification.
	*/
	RNotification(const RCString& name,RObject* sender)
		: Name(name), Type(GetType(name)), Sender(sender) {}

	/**
	* Constructor of a notification.
	* @param name            Name of the notification.
	* @param sender          Object sending the notification.
	* @param data            Data associated with the notification.
	*/
	RNotification(const RCString& name,RObject* sender,long data)
		: Name(name), Type(GetType(name)), Sender(sender), LongValue(data) {}

	/**
	* Constructor of a notification.
	* @param name            Name of the notification.
	* @param sender          Object sending the notification.
	* @param data            Data associated with the notification.
	*/
	RNotification(const RCString& name,RObject* sender,unsigned long data)
		: Name(name), Type(GetType(name)), Sender(sender), ULongValue(data) {}

	/**
	* Constructor of a notification.
	* @param name            Name of the notification.
	* @param sender          Object sending the notification.
	* @param data            Data associated with the notification.
	*/
	RNotification(const RCString& name,RObject* sender,long long data)
		: Name(name), Type(GetType(name)), Sender(sender), LongLongValue(data) {}

	/**
	* Constructor of a notification.
	* @param name            Name of the notification.
	* @param sender          Object sending the notification.
	* @param data            Data associated with the notification.
	*/
	RNotification(const RCString& name,RObject* sender,unsigned long long data)
		: Name(name), Type(GetType(name)), Sender(sender), ULongLongValue(data) {}

	/**
	* Constructor of a notification.
	* @param name            Name of the notification.
	* @param sender          Object sending the notification.
	* @param data            Data associated with the notification.
	*/
	RNotification(const RCString& name,RObject* sender,double data)
		: Name(name), Type(GetType(name)), Sender(sender), DoubleValue(data) {}

	/**
	* Constructor of a notification.
	* @param name            Name of the notification.
	* @param sender          Object sending the notification.
	* @param data            Data associated with the notification.
	*/
	RNotification(const RCString& name,RObject* sender,char* data)
		: Name(name), Type(GetType(name)), Sender(sender), CharSValue(data) {}

	/**
	* Constructor of a notification.
	* @param name            Name of the notification.
	* @param sender          Object sending the notification.
	* @param data            Data associated with the notification.
	*/
	RNotification(const RCString& name,RObject* sender,RString* data)
		: Name(name), Type(GetType(name)), Sender(sender), StringSValue(data) {}

	/**
	* Constructor of a notification.
	* @param name            Name of the notification.
	* @param sender          Object sending the notification.
	* @param data            Data associated with the notification.
	*/
	RNotification(const RCString& name,RObject* sender,void* data)
		: Name(name), Type(GetType(name)), Sender(sender), VoidSValue(data) {}

	/**
	* Get the name of a notification.
	*/
	RCString GetName(void) const {return(Name);}

	/**
	* Get the type of the notification.
	*/
	tNotificationType GetType(void) const {return(Type);}

	/**
	* Get the senderof the notification.
	*/
	RObject* GetSender(void) const {return(Sender);}

	/**
	* Get the value of the data associated with the notification.
	*/
	long GetLong(void) const {return(LongValue);}

	/**
	* Get the value of the data associated with the notification.
	*/
	unsigned long GetULong(void) const {return(ULongValue);}

	/**
	* Get the value of the data associated with the notification.
	*/
	long long GetLongLong(void) const {return(LongLongValue);}

	/**
	* Get the value of the data associated with the notification.
	*/
	unsigned long long GetULongLong(void) const {return(ULongLongValue);}

	/**
	* Get the value of the data associated with the notification.
	*/
	double GetDouble(void) const {return(DoubleValue);}

	/**
	* Get the value of the data associated with the notification.
	*/
	char* GetCharS(void) const {return(CharSValue);}

	/**
	* Get the value of the data associated with the notification.
	*/
	RString* GetStringS(void) const {return(StringSValue);}

	/**
	* Get the value of the data associated with the notification.
	*/
	void* GetVoidS(void) const {return(VoidSValue);}

	friend class RNotificationCenter;
};


//-----------------------------------------------------------------------------
/**
* The tNotificationHandler type provides a representation for a method of an
* object deriving from RObject and taking a notification as parameter.
* @code
* class MyObject : RObject
* {
* public:
*	MyObject(const RString& name) : RObject(name) {}
*	virtual RCString GetClassName(void) const {return("MyObject");}
*	void Handle(const RNotification& notification);
* };
*
* tNotificationHandler handler=HANDLER(MyObject::Handle);
* @endcode
* It is necessary to use the HANDLER macro to define a handler.
* @short Message handler.
* @author Pascal Francq
*/
typedef void (RObject::*tNotificationHandler)(const RNotification& msg);


}  //-------- End of namespace R ----------------------------------------------


//-----------------------------------------------------------------------------
#endif
