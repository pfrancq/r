/*

	RStep Project Library

	RNotificationCenter.coo

	Notification center - Implementation.

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
// include files for R Project
#include <rcursor.h>
#include <rnotificationcenter.h>
#include <robject.h>
#include <rstd.h>
using namespace R;
using namespace std;


//-----------------------------------------------------------------------------
// Member shared across the application
RNotificationCenter R::NotificationCenter;
static bool Created=false;



//-----------------------------------------------------------------------------
//
// struct IListener
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
struct IListener
{
public:
	tNotificationHandler Handler;          // The handler.
	RObject* Observer;                     // The observer.
	hNotification Handle;                  // The notification.
	RObject* Object;                       // Eventually, a particular object

	IListener(tNotificationHandler handler,RObject* observer,hNotification handle,RObject* object)
		: Handler(handler), Observer(observer), Handle(handle), Object(object) {}
	int Compare(const IListener& listener) const {return(ComparePtrs(this,&listener));}
	int Compare(const IListener* listener) const {return(ComparePtrs(this,listener));}
};



//-----------------------------------------------------------------------------
//
// struct INotifications
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/**
 * All handlers for a given notification.
 */
struct INotifications : public RContainer<IListener,true,true>
{
	RCString Name;         // Name of the notification observed.

	INotifications(const RCString& name) : RContainer<IListener,true,true>(50), Name(name) {}
	int Compare(const INotifications& msg) const {return(Name.Compare(msg.Name));}
	int Compare(const RCString& name) const {return(Name.Compare(name));}
	void DeleteObserver(const RObject* observer);
	void DeleteObject(const RObject* obj);
};


//-----------------------------------------------------------------------------
void INotifications::DeleteObserver(const RObject* observer)
{
	RContainer<IListener,false,false> Dels(20);
	RCursor<IListener> List(*this);
	for(List.Start();!List.End();List.Next())
		if(List()->Observer==observer)
			Dels.InsertPtr(List());
	List.Set(Dels);
	for(List.Start();!List.End();List.Next())
		DeletePtr(*List());
}


//-----------------------------------------------------------------------------
void INotifications::DeleteObject(const RObject* obj)
{
	RContainer<IListener,false,false> Dels(20);
	RCursor<IListener> List(*this);
	for(List.Start();!List.End();List.Next())
		if(List()->Object==obj)
			Dels.InsertPtr(List());
	List.Set(Dels);
	for(List.Start();!List.End();List.Next())
		DeletePtr(*List());
}



//-----------------------------------------------------------------------------
//
// struct IObjects
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/**
 *  All handlers for a given object.
*/
struct IObjects : public RContainer<IListener,true,true>
{
	RObject* Object;        // Pointer to the object observed.

	IObjects(RObject* object) : RContainer<IListener,true,true>(50), Object(object) {}

	int Compare(const IObjects& object) const {return(ComparePtrs(Object,object.Object));}
	int Compare(RObject* object) const {return(ComparePtrs(Object,object));}
	void DeleteObserver(const RObject* observer,hNotification type);
};


//-----------------------------------------------------------------------------
void IObjects::DeleteObserver(const RObject* observer,hNotification handle)
{
	RContainer<IListener,false,false> Dels(20);
	RCursor<IListener> List(*this);
	for(List.Start();!List.End();List.Next())
		if((List()->Observer==observer)&&((!handle)||((handle)&&(handle==List()->Handle))))
			Dels.InsertPtr(List());
	List.Set(Dels);
	for(List.Start();!List.End();List.Next())
		DeletePtr(*List());
}



//-----------------------------------------------------------------------------
//
// struct RNotificationCenter::Intern
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
struct RNotificationCenter::Intern
{
	RContainer<INotifications,true,true> Notifications;  // All handlers for a given notification.
	RContainer<IObjects,true,true> Objects; 	         // All handlers for a given object.
	RContainer<IListener,true,false> Defaults;           // Observers that handle every notification of every object.
	RContainer<RObject,false,true> Observers;            // All observers.
	Intern(void) : Notifications(50), Objects(50), Defaults(50), Observers(100) {}
};



//-----------------------------------------------------------------------------
//
// class RNotificationCenter
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
RNotificationCenter::RNotificationCenter(void)
	: Data(0)
{
	if(Created)
		throw RException("Cannot add multiple notification centers");
	Created=true;
	Data=new Intern();
}


//-----------------------------------------------------------------------------
hNotification RNotificationCenter::GetNotificationHandle(const RCString& name)
{
	return(reinterpret_cast<hNotification>(Data->Notifications.GetInsertPtr(name)));
}


//-----------------------------------------------------------------------------
RCString RNotificationCenter::GetNotificationName(hNotification handle) const
{
	if(!handle)
		return(RCString::Null);
	return(reinterpret_cast<INotifications*>(handle)->Name);
}


//-----------------------------------------------------------------------------
void RNotificationCenter::InsertObserver(tNotificationHandler handler,RObject* observer,hNotification handle,RObject* object)
{
	// Verify that the observer is not null
	RReturnIfFail(observer);
	RReturnIfFail(handler);

	// Register the notification and the observer
	IListener* listener=new IListener(handler,observer,handle,object);
	if(!Data->Observers.IsIn(observer))
		Data->Observers.InsertPtr(observer);

	if(!object)
	{
		// Notification to track does not depend of a particular object
		if(!handle)
			Data->Defaults.InsertPtr(listener);          // Observer catch all messages
		else
		{
			// Observer catch a given message from all objects
			INotifications* msg=reinterpret_cast<INotifications*>(handle);
			msg->InsertPtr(listener);
		}
	}
	else
	{
		// Observer catch for a given object all notification (name="") or a particular notification
		IObjects* obj;
		if(!object->Handlers)
		{
			obj=Data->Objects.GetInsertPtr(object);
			object->Handlers=obj;
		}
		else
			obj=static_cast<IObjects*>(object->Handlers);
		obj->InsertPtr(listener);
	}
}


//-----------------------------------------------------------------------------
void RNotificationCenter::PostNotification(const RNotification& notification)
{
	bool Call=false;

	// Tell the observers that want to know about a particular notification
	if(notification.Handle)
	{
		RCursor<IListener> Notification(*reinterpret_cast<INotifications*>(notification.Handle));
		for(Notification.Start();!Notification.End();Notification.Next())
		{
			const_cast<RNotification&>(notification).Receiver=Notification()->Observer;
			(Notification()->Observer->*(Notification()->Handler))(notification);
			Call=true;
		}
	}

	// Tell the observers that want to know about a particular object
	if(notification.Sender&&notification.Sender->Handlers)
	{
		RCursor<IListener> Object(*reinterpret_cast<IObjects*>(notification.Sender->Handlers));
		for(Object.Start();!Object.End();Object.Next())
		{
			if((Object()->Handle)&&(Object()->Handle!=notification.Handle))
				continue;
			const_cast<RNotification&>(notification).Receiver=Object()->Observer;
			(Object()->Observer->*(Object()->Handler))(notification);
			Call=true;
		}
	}

	// Tell the catch all observers
	RCursor<IListener> Default(Data->Defaults);
	for(Default.Start();!Default.End();Default.Next())
	{
		const_cast<RNotification&>(notification).Receiver=Default()->Observer;
		(Default()->Observer->*(Default()->Handler))(notification);
		Call=true;
	}

	// If nobody has catch the message, call the 'HandlerNotFound' method.
	const_cast<RNotification&>(notification).Receiver=0;
	if((!Call)&&notification.GetSender())
		notification.GetSender()->HandlerNotFound(notification);
}


//-----------------------------------------------------------------------------
void RNotificationCenter::DeleteObserver(RObject* observer)
{
	// Verify that the observer is not null
	RReturnIfFail(observer);
	RContainer<IListener,false,false> Dels(20);

	// Goes through to all listeners that receive a particular notification
	RCursor<INotifications> Notifications(Data->Notifications);
	for(Notifications.Start();!Notifications.End();Notifications.Next())
		Notifications()->DeleteObserver(observer);

	// Goes through to all listeners that receive notifications for a particular object
	RCursor<IObjects> Objects(Data->Objects);
	for(Objects.Start();!Objects.End();Objects.Next())
		Objects()->DeleteObserver(observer,0);

	// Go trough the all default listeners
	RCursor<IListener> Listener(Data->Defaults);
	for(Listener.Start();!Listener.End();Listener.Next())
		if(Listener()->Observer==observer)
			Dels.InsertPtr(Listener());
	Listener.Set(Dels);
	for(Listener.Start();!Listener.End();Listener.Next())
		Dels.DeletePtr(*Listener());

	// Remove the observer
	Data->Observers.DeletePtr(observer);
}


//-----------------------------------------------------------------------------
void RNotificationCenter::DeleteObserver(RObject* observer,hNotification handle,RObject* object)
{
	// Verify that the observer is not null
	RReturnIfFail(observer);

	// Goes through to all listeners that receive a particular notification
	if(handle&&(!object))
		reinterpret_cast<INotifications*>(handle)->DeleteObserver(observer);

	// Goes through to all listeners that receive notifications for a particular object
	if(object&&object->Handlers)
		static_cast<IObjects*>(object->Handlers)->DeleteObserver(observer,handle);

	// Go trough the all default listeners
	if((!handle)&&(!object))
	{
		RContainer<IListener,false,false> Dels(20);
		RCursor<IListener> Listener(Data->Defaults);
		for(Listener.Start();!Listener.End();Listener.Next())
			if(Listener()->Observer==observer)
				Dels.InsertPtr(Listener());
		Listener.Set(Dels);
		for(Listener.Start();!Listener.End();Listener.Next())
			Dels.DeletePtr(*Listener());
	}
}


//-----------------------------------------------------------------------------
void RNotificationCenter::DeleteObject(RObject* obj)
{
	// Delete the objects are observer
	DeleteObserver(obj);

	// Delete all notifications related to the objects
	RCursor<INotifications> Notifications(Data->Notifications);
	for(Notifications.Start();!Notifications.End();Notifications.Next())
		Notifications()->DeleteObject(obj);

	// Delete all listeners for the particular object
	Data->Objects.DeletePtr(obj);
}


//-----------------------------------------------------------------------------
RNotificationCenter::~RNotificationCenter(void)
{
	delete Data;
}
