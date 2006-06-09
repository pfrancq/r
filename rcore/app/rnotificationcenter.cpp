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
// include files for R Project
#include <rcursor.h>
#include <rnotificationcenter.h>
#include <robject.h>
using namespace R;


//-----------------------------------------------------------------------------
// Member shared accross the application
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
	tNotificationHandler Handler;
	RObject* Observer;
	tNotificationType Type;
	RObject* Object;

	IListener(tNotificationHandler handler,RObject* observer,tNotificationType type,RObject* object)
		: Handler(handler), Observer(observer), Type(type), Object(object) {}
	int Compare(const IListener& listener) const {return(this-&listener);}
	int Compare(const IListener* listener) const {return(this-listener);}
};



//-----------------------------------------------------------------------------
//
// struct INotifications
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
struct INotifications : public RContainer<IListener,true,true>
{
	tNotificationType Type;

	INotifications(tNotificationType type) : RContainer<IListener,true,true>(50), Type(type) {}
	int Compare(const INotifications& msg) const {return(Type-msg.Type);}
	int Compare(tNotificationType type) const {return(Type-type);}
	void Delete(const RObject* observer);
};


//-----------------------------------------------------------------------------
void INotifications::Delete(const RObject* observer)
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
//
// struct IObjects
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
struct IObjects : public RContainer<IListener,true,true>
{
	RObject* Object;

	IObjects(RObject* object) : RContainer<IListener,true,true>(50), Object(object) {}

	int Compare(const IObjects& object) const {return(Object-object.Object);}
	int Compare(RObject* object) const {return(Object-object);}
	void Delete(const RObject* observer,tNotificationType type);
};


//-----------------------------------------------------------------------------
void IObjects::Delete(const RObject* observer,tNotificationType type)
{
	RContainer<IListener,false,false> Dels(20);
	RCursor<IListener> List(*this);
	for(List.Start();!List.End();List.Next())
		if((List()->Observer==observer)&&((!type)||((type)&&(type==List()->Type))))
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
void RNotificationCenter::InsertObserver(tNotificationHandler handler,RObject* observer,const RCString& name,RObject* object)
{
	// Verify that the observer is not null
	RReturnIfFail(observer);
	RReturnIfFail(handler);
	tNotificationType type=RNotification::GetType(name);

	// Register the notification and the observer
	IListener* listener=new IListener(handler,observer,type,object);
	if(!Data->Observers.IsIn(observer))
		Data->Observers.InsertPtr(observer);

	if(!object)
	{
		// Notification to track does not depend of a particular object
		if(!type)
			Data->Defaults.InsertPtr(listener);          // Observer catch all messages
		else
		{
			// Observer catch a given message from all objects
			INotifications* msg=Data->Notifications.GetInsertPtr(type);
			msg->InsertPtr(listener);
		}
	}
	else
	{
		// Observer catch for a given object all notification (name="") or a particular notification
		IObjects* obj=Data->Objects.GetInsertPtr(object);
		obj->InsertPtr(listener);
	}
}


//-----------------------------------------------------------------------------
void RNotificationCenter::PostNotification(const RNotification& notification)
{
	bool Call=false;

	// Tell the observers that want to know about a particular notification
	INotifications* ptr=Data->Notifications.GetPtr(notification.Type);
	if(ptr)
	{
		RCursor<IListener> Notification(*ptr);
		for(Notification.Start();!Notification.End();Notification.Next())
		{
			(Notification()->Observer->*(Notification()->Handler))(notification);
			Call=true;
		}
	}

	// Tell the observers that want to know about a particular object
	IObjects* ptr2=Data->Objects.GetPtr(notification.Sender);
	if(ptr2)
	{
		RCursor<IListener> Object(*ptr2);
		for(Object.Start();!Object.End();Object.Next())
		{
			if((Object()->Type)&&(Object()->Type!=notification.Type))
				continue;
			(Object()->Observer->*(Object()->Handler))(notification);
			Call=true;
		}
	}

	// Tell the catch all observers
	RCursor<IListener> Default(Data->Defaults);
	for(Default.Start();!Default.End();Default.Next())
	{
		(Default()->Observer->*(Default()->Handler))(notification);
		Call=true;
	}

	if((!Call)&&notification.GetSender())
		notification.GetSender()->HandlerNotFound(notification);
}


//-----------------------------------------------------------------------------
void RNotificationCenter::DeleteObserver(RObject* observer)
{
	// Verify that the observer is not null
	RReturnIfFail(observer);
	RContainer<IListener,false,false> Dels(20);

	// Goes through to all listerners that receive a particular notification
	RCursor<INotifications> Notifications(Data->Notifications);
	for(Notifications.Start();!Notifications.End();Notifications.Next())
		Notifications()->Delete(observer);

	// Goes through to all listerners that receive notifications for a particular object
	RCursor<IObjects> Objects(Data->Objects);
	for(Objects.Start();!Objects.End();Objects.Next())
		Objects()->Delete(observer,0);

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
void RNotificationCenter::DeleteObserver(RObject* observer,const RCString& name,RObject* object)
{
	// Verify that the observer is not null
	RReturnIfFail(observer);
	tNotificationType type=RNotification::GetType(name);

	// Goes through to all listerners that receive a particular notification
	if(type&&(!object))
	{
		INotifications* ptr=Data->Notifications.GetPtr(type);
		if(ptr)
			ptr->Delete(observer);
	}

	// Goes through to all listerners that receive notifications for a particular object
	IObjects* ptr2=Data->Objects.GetPtr(object);
	if(ptr2)
		ptr2->Delete(observer,type);

	// Go trough the all default listeners
	if((!type)&&(!object))
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
RNotificationCenter::~RNotificationCenter(void)
{
	delete Data;
}
