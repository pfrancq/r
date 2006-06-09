/*

	RStep Project Library

	RNotificationCenter.h

	Notification center - Header.

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
#ifndef RNotificationCenter_H
#define RNotificationCenter_H



//-----------------------------------------------------------------------------
// include files for R Project
#include <rnotification.h>


//-----------------------------------------------------------------------------
namespace R{
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
class RObject;

//-----------------------------------------------------------------------------
/**
* The RNotificationCenter class provides a representation for a notification
* center that dispatch notifications send by objects.
* @short Notification Center
* @author Pascal Francq
*/
class RNotificationCenter
{
	struct Intern;
	Intern* Data;

public:

	/**
	* Constructor of a notification center.
	*/
	RNotificationCenter(void);

	/**
	* Add an observer, eventually for a particular object and/or a particular
	* notification.
	* @param handler         Method that should handle the notification.
	* @param observer        Observer of the notification.
	* @param name            Name of the notification to handle.
	* @param object          Object which notifications must be handle.
	* Several possibilities exists:
	* -# name is not null, but object is. The handler catches a particular
	*    notification for any object.
	* -# name is null, but object isn't. The handler catches every notification
	*    for a particular object.
	* -# name and object are null. The handler catches every notification of
	*    every object.
	*/
	void InsertObserver(tNotificationHandler handler,RObject* observer,const RCString& name=RCString::Null,RObject* object=0);

	/**
	* Post a notification.
	* @param notification    Notification.
	*/
	void PostNotification(const RNotification& notification);

	/**
	* Post a notification.
	* @param name            Name of the notification.
	* @param data            Data associated to the notification.
	*/
	template<class T> void PostNotification(const RCString& name,T data)
	{
		PostNotification(RNotification(name,0,data));
	}

	/**
	* Post a notification without any data.
	* @param name            Name of the notification.
	*/
	void PostNotification(const RCString& name)
	{
		PostNotification(RNotification(name,0));
	}

	/**
	* Delete a given observer.
	* @param observer        Observer to delete.
	*/
	void DeleteObserver(RObject* observer);

	/**
	* Delete a given handler of a given observer.
	* @param observer        Observer of the handle.
	* @param name            Name of the notification to handle.
	* @param object          Object which notifications must be handle.
	* Several possibilities exists:
	* -# name is not null, but object is. All handlers defined for the
	*    particular notification are deleted.
	* -# name is null, but object isn't. All handlers defined for the object
	*    are deleted.
	* -# name and object are null. All default handlers are deleted.
	*/
	void DeleteObserver(RObject* observer,const RCString& name,RObject* object);

	/**
	* Destructor of the notification center.
	*/
	~RNotificationCenter(void);
};


//-----------------------------------------------------------------------------
/**
* Pointer to the current notification center.
*/
extern RNotificationCenter NotificationCenter;


}  //-------- End of namespace R ----------------------------------------------


//-----------------------------------------------------------------------------
#endif
