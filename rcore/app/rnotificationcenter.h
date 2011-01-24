/*

	RStep Project Library

	RNotificationCenter.h

	Notification center - Header.

	Copyright 2006-2011 by Pascal Francq (pascal@francq.info).
	Copyright 2006-2008 by the Université Libre de Bruxelles (ULB).

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
* @code
* class MyObject : public RObject
* {
* public:
*    MyObject(const RString& name) : RObject(name)
*    {
*       InsertObserver(HANDLER(MyObject::Handle),"Example");
*    }
*    virtual RCString GetClassName(void) const {return("MyObject");}
*    void Handle(const RNotification& notification)
*    {
*       cout<<"From "<<GetName()<<" : "<<GetData<long>(msg)<<endl;
*    }
* };
*
* int main(int argc, char *argv[])
* {
*    MyObject Obj1("Object 1");
*    MyObject Obj2("Object 2");
*    NotificationCenter.PostNotification("Message",(long)3);
* }
* @endcode
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
	* Get a handle for a given notification.
	* @param name            Name of the notification.
	* @return Handle.
	*/
	hNotification GetNotificationHandle(const RCString& name);

	/**
	* Get the name of a notification.
	* @param handle          Handle of the notification.
	*/
	RCString GetNotificationName(hNotification handle) const;

	/**
	* Add an observer, eventually for a particular object and/or a particular
	* notification.
	* @param handler         Method that should handle the notification.
	* @param observer        Observer of the notification.
	* @param handle          Handle of the notification.
	* @param object          Object which notifications must be handle.
	* Several possibilities exists:
	* -# handle is not null, but object is. The handler catches a particular
	*    notification for any object.
	* -# handle is null, but object isn't. The handler catches every notification
	*    for a particular object.
	* -# handle and object are null. The handler catches every notification of
	*    every object.
	*/
	void InsertObserver(tNotificationHandler handler,RObject* observer,hNotification handle=0,RObject* object=0);

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
	void InsertObserver(tNotificationHandler handler,RObject* observer,const RCString& name=RCString::Null,RObject* object=0)
	{
		InsertObserver(handler,observer,GetNotificationHandle(name),object);
	}

	/**
	* Post a notification.
	* @param notification    Notification.
	*/
	void PostNotification(const RNotification& notification);

	/**
	* Post a notification.
	* @tparam T              Class of the data to the send with the notification.
	* @param name            Name of the notification.
	* @param data            Data associated to the notification.
	*/
	template<class T> void PostNotification(const RCString& name,T data)
	{
		PostNotification(RNotificationData<T>(name,0,data));
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
	* @param handle          Handle of the notification.
	* @param object          Object which notifications must be handle.
	* Several possibilities exists:
	* -# handle is not null, but object is. All handlers defined for the
	*    particular notification are deleted.
	* -# handle is null, but object isn't. All handlers defined for the object
	*    are deleted.
	* -# handle and object are null. All default handlers are deleted.
	*/
	void DeleteObserver(RObject* observer,hNotification handle,RObject* object);

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
	void DeleteObserver(RObject* observer,const RCString& name,RObject* object)
	{
		DeleteObserver(observer,GetNotificationHandle(name),object);
	}

private:

	/**
	 * An object is deleted. It must be removed from the observer and no other
	 * objects can observed it anymore.
	 * @param obj
	 */
	void DeleteObject(RObject* obj);

public:

	/**
	* Destruct the notification center.
	*/
	~RNotificationCenter(void);

	friend class RObject;
};


//-----------------------------------------------------------------------------
/**
* Pointer to the current notification center.
*/
extern RNotificationCenter NotificationCenter;


}  //-------- End of namespace R ----------------------------------------------


//-----------------------------------------------------------------------------
#endif
