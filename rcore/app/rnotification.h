/*

	R Project Library

	RNotification.h

	Generic notification - Header.

	Copyright 2006-2015 by Pascal Francq (pascal@francq.info).
	Copyright 2006-2008 by the Universit√© Libre de Bruxelles (ULB).

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
// forward class declaration
class RObject;


//-----------------------------------------------------------------------------
/**
* The hNotification provides a handle for a given notification.
* @short Notification Handle
*/
typedef void* hNotification;


//-----------------------------------------------------------------------------
/**
* The RNotification provides a representation for a notification that can be
* send by an object.
* @short Notification
*/
class RNotification
{
	/**
	* Type of the notification.
	*/
	const hNotification Handle;

	/**
	* Object that has send the notification.
	*/
	RObject* Sender;

	/**
	 * Object which is currently receiving the notification.
	 */
	RObject* Receiver;

public:

	/**
	* Constructor of a notification.
	* @param name            Name of the notification.
	* @param sender          Object sending the notification.
	*/
	RNotification(const RCString& name,RObject* sender);

	/**
	* Constructor of a notification.
	* @param handle          Handle of the notification.
	* @param sender          Object sending the notification.
	*/
	RNotification(const hNotification handle,RObject* sender);

	/**
	* Get the name of a notification.
	*/
	RCString GetName(void) const;

	/**
	* Get the handle of the notification.
	*/
	hNotification GetHandle(void) const {return(Handle);}

	/**
	* Get the sender of the notification.
	*/
	RObject* GetSender(void) const {return(Sender);}

	/**
	* Get the receiver of the notification.
	*/
	RObject* GetReceiver(void) const {return(Receiver);}

	/**
	* Destruct. This is only used for OO purposes.
	*/
	virtual ~RNotification(void);

	friend class RNotificationCenter;
};


//-----------------------------------------------------------------------------
/**
* The RNotificationData provides a representation for a notification that can
* be send by an object with a specific data.
* @tparam T                  Type of the parameter.
*
* The type used for template is very important. For example, the following code
* generates an exception since the default type of 3 is int:
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
*       cout<<"From "<<notification.GetName()<<" : "<<GetData<long>(msg)<<endl;
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
* To be sure that PostNotification calls the right handlers, it is always
* better to explicitly cast the parameter when sending.
* @short Notification with Data.
*/
template <class T>
	class RNotificationData : public RNotification
{
	/**
	* Data associated with the notification.
	*/
	T Data;

public:

	/**
	* Constructor of a notification.
	* @param name            Name of the notification.
	* @param sender          Object sending the notification.
	* @param data            Data associated.
	*/
	RNotificationData(const RCString& name,RObject* sender,T data)
		: RNotification(name,sender), Data(data) {}

	/**
	* Constructor of a notification.
	* @param handle          Handle of the notification.
	* @param sender          Object sending the notification.
	* @param data            Data associated.
	*/
	RNotificationData(hNotification handle,RObject* sender,T data)
		: RNotification(handle,sender), Data(data) {}

	/**
	* Get the data of the notification.
	*/
	T Get(void) const {return(Data);}
};


//-----------------------------------------------------------------------------
/**
* Get the data associated with a given notification.
* @tparam T                  Type of the parameter.
* @param notification        Notification.
* @short Data extracted of notification.
*/
template<class T> T GetData(const RNotification& notification)
{
	return(dynamic_cast<const RNotificationData<T>&>(notification).Get());
}


//-----------------------------------------------------------------------------
/**
* The tNotificationHandler type provides a representation for a method of an
* object deriving from RObject and taking a notification as parameter.
* @code
* class MyObject : RObject
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
* tNotificationHandler handler=HANDLER(MyObject::Handle);
* @endcode
* It is necessary to use the HANDLER macro to define a handler.
* @short Message handler.
*/
typedef void (RObject::*tNotificationHandler)(const RNotification& msg);


}  //-------- End of namespace R ----------------------------------------------


//-----------------------------------------------------------------------------
#endif
