/*

	RStep Project Library

	RObject.h

	Generic Object that can receive and send messages - Header.

	Copyright 2006-2009 by Pascal Francq (pascal@francq.info).
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
*       cout<<"From "<<GetName()<<" : "<<GetData<long>(notification)<<endl;
*    }
* };
*
* int main(int argc, char *argv[])
* {
*    MyObject Obj1("Object 1");
*    MyObject Obj2("Object 2");
*    NotificationCenter.PostNotification("Example",(long)3);
* }

* @endcode
*
* It may append that a call to a method InsertObserver does not work well when
* objects inheriting from RObject also inherit from other objects libraries
* (such as Qt). The results is that the 'this' may not point to the real
* address. To avoid this, you must explicitly cast 'this':
* @code
*    MyObject(const RString& name) : RObject(name)
*    {
*       reinterpret_cast<RObject*>(this)->InsertObserver(HANDLER(MyObject::Handle),"Example");
*    }
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

	/**
	* Handlers of the object.
	*/
	void* Handlers;

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
	int Compare(const RObject& obj) const;

	/**
	* Compare method used by R::RContainer.
	* @param obj             Object to compare with.
	*/
	int Compare(const RObject* obj) const;

	/**
	* Get the name of the object.
	*/
	inline RString GetName(void) const {return(Name);}

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
	* @param handle          Handle of the notification.
	*/
	void PostNotification(hNotification handle)
	{
		NotificationCenter.PostNotification(RNotification(handle,this));
	}

	/**
	* Post a notification to the notification center.
	* @param name            Name of the notification.
	*/
	void PostNotification(const RCString& name)
	{
		NotificationCenter.PostNotification(RNotification(name,this));
	}

	/**
	* Post a notification to the notification center.
	* @param handle          Handle of the notification.
	* @param data            Data associated to the notification.
	*/
	template<class T> void PostNotification(hNotification handle,T data)
	{
		NotificationCenter.PostNotification(RNotificationData<T>(handle,this,data));
	}

	/**
	* Post a notification to the notification center.
	* @param name            Name of the notification.
	* @param data            Data associated to the notification.
	*/
	template<class T> void PostNotification(const RCString& name,T data)
	{
		NotificationCenter.PostNotification(RNotificationData<T>(name,this,data));
	}

	/**
	* Add a handler for a particular notification for a particular object.
	* @param handler         Handler of the notification.
	* @param handle          Handle of the notification.
	* @param object          Object posting the notification.
	*/
	inline void InsertObserver(tNotificationHandler handler,hNotification handle,RObject* object)
	{
		NotificationCenter.InsertObserver(handler,this,handle,object);
	}

	/**
	* Add a handler for a particular notification for a particular object.
	* @param handler         Handler of the notification.
	* @param name            Name of the notification.
	* @param object          Object emitting the notification.
	*/
	inline void InsertObserver(tNotificationHandler handler,const RCString& name,RObject* object)
	{
		NotificationCenter.InsertObserver(handler,this,name,object);
	}

	/**
	* Add a handler for a particular notification of every object.
	* @param handler         Handler of the notification.
	* @param handle          Handle of the notification.
	*/
	inline void InsertObserver(tNotificationHandler handler,hNotification handle)
	{
		NotificationCenter.InsertObserver(handler,this,handle,0);
	}

	/**
	* Add a handler for a particular notification of every object.
	* @param handler         Handler of the notification.
	* @param name            Name of the notification.
	*/
	inline void InsertObserver(tNotificationHandler handler,const RCString& name)
	{
		NotificationCenter.InsertObserver(handler,this,name,0);
	}

	/**
	* Add a handler for every notification of a particular object.
	* @param handler         Handler of the notification.
	* @param object          Object posting the notification.
	*/
	inline void InsertObserver(tNotificationHandler handler,RObject* object)
	{
		NotificationCenter.InsertObserver(handler,this,0,object);
	}

	/**
	* Add a handler for every notification of every object.
	* @param handler         Handler of the notification.
	*/
	inline void InsertObserver(tNotificationHandler handler)
	{
		NotificationCenter.InsertObserver(handler,this,0,0);
	}

	/**
	 * Delete the object as observer.
	 * @param
	 */
	inline void DeleteObserver(void)
	{
		NotificationCenter.DeleteObserver(this);
	}

	/**
	* Get a handle for a given notification.
	* @param name            Name of the notification.
	* @return Handle.
	*/
	hNotification GetNotificationHandle(const RCString& name) const
	{
		return(NotificationCenter.GetNotificationHandle(name));
	}

	/**
	* Get the name of a notification.
	* @param handle          Handle of the notification.
	*/
	RCString GetNotificationName(hNotification handle) const
	{
		return(NotificationCenter.GetNotificationName(handle));
	}

	/**
	* Destruct the object.
	*/
	virtual ~RObject(void);

	friend class RNotificationCenter;
};


}  //-------- End of namespace R ----------------------------------------------


//-----------------------------------------------------------------------------
#endif
