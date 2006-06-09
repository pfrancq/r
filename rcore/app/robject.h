/*

	RStep Project Library

	RObject.h

	Generic Object that can receive and send messages - Header.

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
#ifndef RObject_H
#define RObject_H


//-----------------------------------------------------------------------------
// include files for R Project
#include <rcstring.h>
#include <rnotificationcenter.h>


//-----------------------------------------------------------------------------
namespace R{
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/**
* The RObject class provides a representation for a generic object that can
* send and receive notifications. The deriving objects should overload the
* GetClassName method.
* @code
* class MyObject : RObject
* {
* public:
*	MyObject(const RString& name) : RObject(name) {}
*	virtual RCString GetClassName(void) const {return("MyObject");}
*	void Handle(const RNotification& notification);
* };
*
* MyObject::MyObject(const RString& name)
*	: RObject(name)
* {
*	InsertObserber(HANDLER(MyObject::Handle),"Example");
* }
* @endcode
* @short Object.
* @author Pascal Francq
*/
class RObject
{
	/**
	* Name fo the object.
	*/
	RString Name;

public:

	/**
	* Constructor of the object.
	* @param name            Name given to the object.
	*/
	RObject(const RString& name=RString::Null);

	/**
	* Compare method used by R::RContainer.
	* @param obj             Object to compare with.
	*/
	int Compare(const RObject& obj) const {return(this-&obj);}

	/**
	* Compare method used by R::RContainer.
	* @param obj             Object to compare with.
	*/
	int Compare(const RObject* obj) const {return(this-obj);}

	/**
	* Get the name of the object.
	*/
	RString GetName(void) const {return(Name);}

	/**
	* This is the handler that is called when an object does not find any
	* handler for a sended notification. By default, it prints the name of the
	* message.
	* @param notification    Notification.
	*/
	virtual void HandlerNotFound(const RNotification& notification);

	/**
	* Get the name of the class of the object.
	*/
	virtual RCString GetClassName(void) const {return("RObject");}

	/**
	* Post a notification to the notification center.
	* @param name            Name of the notification.
	* @param data            Data associated to the notification.
	*/
	template<class T> void PostNotification(const RCString& name,T data)
	{
		NotificationCenter.PostNotification(RNotification(name,this,data));
	}

	/**
	* Add a handler for a particular notification for a particular object.
	* @param handler         Handler of the notification.
	* @param name            Name of the notification.
	* @param object          Object emitting the notification.
	*/
	void InsertObserver(tNotificationHandler handler,const RCString& name,RObject* object)
	{
		NotificationCenter.InsertObserver(handler,this,name,object);
	}

	/**
	* Add a handler for a particular notification of every object.
	* @param handler         Handler of the notification.
	* @param name            Name of the notification.
	*/
	void InsertObserver(tNotificationHandler handler,const RCString& name)
	{
		NotificationCenter.InsertObserver(handler,this,name,0);
	}

	/**
	* Add a handler for every notification of a particular object.
	* @param handler         Handler of the notification.
	* @param object          Object emitting the notification.
	*/
	void InsertObserver(tNotificationHandler handler,RObject* object)
	{
		NotificationCenter.InsertObserver(handler,this,RCString::Null,object);
	}

	/**
	* Add a handler for every notification of every object.
	* @param handler         Handler of the notification.
	*/
	void InsertObserver(tNotificationHandler handler)
	{
		NotificationCenter.InsertObserver(handler,this,RCString::Null,0);
	}

	/**
	* Destructor of the object.
	*/
	virtual ~RObject(void);
};


}  //-------- End of namespace R ----------------------------------------------


//-----------------------------------------------------------------------------
#endif
