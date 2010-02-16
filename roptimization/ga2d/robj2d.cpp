/*

	R Project Library

	RObj2D.hh

	Object for 2D placement GA - Implementation

	Copyright 1998-2010 by Pascal Francq (pascal@francq.info).
	Copyright 1998-2008 by the Université Libre de Bruxelles (ULB).

	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU Library General Public
	License as published by the Free Software Foundation; either
	version 2.0 of the License, or (at your option) any later version.

	This library is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
	Library General Public License for more details.

	You should have received a copy of the GNU Library General Public
	License along with this library, as a file COPYING.LIB; if not, write
	to the Free Software Foundation, Inc., 59 Temple Place, Suite 330,
	Boston, MA  02111-1307  USA

*/



//------------------------------------------------------------------------------
// include files for ANSI C/C++
#include <math.h>


//------------------------------------------------------------------------------
// include files for R Project
#include <robj2d.h>
#include <robj2dconfig.h>
#include <rgeoinfo.h>
#include <rconnection.h>
using namespace R;



//------------------------------------------------------------------------------
//
// class RObj2DPin
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
RObj2DPin::RObj2DPin(RObj2DConnector* con,size_t id,const RString& name)
	: Connector(con), Id(id), Name(name)
{
	Connector->InsertPtr(this);
}


//------------------------------------------------------------------------------
int RObj2DPin::Compare(const RObj2DPin& pin) const
{
	return(CompareIds(Id,pin.Id));
}


//------------------------------------------------------------------------------
int RObj2DPin::Compare(const size_t pin) const
{
	return(CompareIds(Id,pin));
}



//------------------------------------------------------------------------------
//
// class RObj2DConnector
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
RObj2DConnector::RObj2DConnector(RObj2D* obj,size_t id,const RString& name)
	:  RContainer<RObj2DPin,true,true>(4), Obj(obj), Id(id), Name(name), Connections(10,5)
{
	if(Obj)
		Obj->Connectors.InsertPtr(this);
}


//------------------------------------------------------------------------------
RObj2DConnector::~RObj2DConnector(void)
{
}



//------------------------------------------------------------------------------
//
// RObj2D
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
RObj2D::RObj2D(size_t id,const RString& name)
	: RContainer<RObj2DConfig,true,true>(8), Id(id), Name(name), Connectors(5,5)
{
}


//------------------------------------------------------------------------------
void RObj2D::CreateOri(tOrientation ori)
{
	RObj2DConfig* Normal(GetPtr(oNormal));

	if(!Normal)
		ThrowRException("No normal configuration is created");

	if(ori==oNormal)
		return;

	RPoint Min;
	RObj2DConfig* New;

	// If Polygon is a rectangle
	if(Normal->GetPolygon().IsRect())
	{
		// Test if a square -> 1 configuration only
		if(Normal->GetPolygon().IsSquare())
			return;

		// It is Rectangle -> Max 2 configuration only
		if(GetNb()==2)
			return;  // Already two

		// Create the configuration
		New=new RObj2DConfig(this,ori);
		RPolygon Polygon(Normal->GetPolygon());
		Polygon.ChangeOrientation(oRota90,Min);
		New->Set(Polygon);
		RCursor<RObj2DConfigConnector> Cur(Normal->GetConnectors());
		for(Cur.Start();!Cur.End();Cur.Next())
		{
			// Create a connector
			RObj2DConfigConnector* Con(new RObj2DConfigConnector(Cur()->GetConnector()));
			New->Add(Con);
			RCursor<RObj2DConfigPin> Cur2(*Cur());
			for(Cur2.Start();!Cur2.End();Cur2.Next())
			{
				// Create a pin
				RRect Rect(Cur2()->GetRect());
				Rect.ChangeOrientation(oRota90,Min);
				RObj2DConfigPin* Pin(new RObj2DConfigPin(Cur2()->GetPin(),Rect));
				Con->InsertPtr(Pin);
			}
		}
	}
	else
	{
		// All configurations are allowed.
		New=new RObj2DConfig(this,ori);
		RPolygon Polygon(Normal->GetPolygon());
		Polygon.ChangeOrientation(ori,Min);
		New->Set(Polygon);
		RCursor<RObj2DConfigConnector> Cur(Normal->GetConnectors());
		for(Cur.Start();!Cur.End();Cur.Next())
		{
			// Create a connector
			RObj2DConfigConnector* Con(new RObj2DConfigConnector(Cur()->GetConnector()));
			New->Add(Con);
			RCursor<RObj2DConfigPin> Cur2(*Cur());
			for(Cur2.Start();!Cur2.End();Cur2.Next())
			{
				// Create a pin
				RRect Rect(Cur2()->GetRect());
				Rect.ChangeOrientation(oRota90,Min);
				RObj2DConfigPin* Pin(new RObj2DConfigPin(Cur2()->GetPin(),Rect));
				Con->InsertPtr(Pin);
			}
		}
	}

	// Initialize the configuration
	InsertPtr(New);
}


//------------------------------------------------------------------------------
void RObj2D::SetId(size_t id)
{
	if(Id==cNoRef)
		Id=id;
}


//------------------------------------------------------------------------------
RObj2DConfig* RObj2D::GetDefaultConfig(void)
{
	if(!GetNb())
	{
		RObj2DConfig* ptr(new RObj2DConfig(this,oNormal));
		InsertPtrAt(ptr,oNormal);
		return(ptr);
	}
	return((*this)[oNormal]);
}


//------------------------------------------------------------------------------
void RObj2D::Copy(const RObj2D& obj)
{
	// Copy the internal data
	RCursor<RObj2DConnector> Con(obj.Connectors);
	for(Con.Start();!Con.End();Con.Next())
	{
		RObj2DConnector* ptr(new RObj2DConnector(this,Con()->GetId(),Con()->GetName()));
		RCursor<RObj2DPin> Pins(*Con());
		for(Pins.Start();!Pins.End();Pins.Next())
			new RObj2DPin(ptr,Pins()->GetId(),Pins()->GetName());
	}

	// Copy the possibles configurations
	RCursor<RObj2DConfig> Config(obj);
	for(Config.Start();!Config.End();Config.Next())
	{
		RObj2DConfig* cfg(new RObj2DConfig(this,Config()->GetOrientation()));
		InsertPtr(cfg);
		cfg->Set(Config()->GetPolygon());
		RCursor<RObj2DConfigConnector> Con(Config()->GetConnectors());
		for(Con.Start();!Con.End();Con.Next())
		{
			RObj2DConnector* con(Connectors.GetPtr(Con()->GetConnector()->GetId()));
			RObj2DConfigConnector* ptr(new RObj2DConfigConnector(con));
			cfg->Add(ptr);
			RCursor<RObj2DConfigPin> Pins(*Con());
			for(Pins.Start();!Pins.End();Pins.Next())
				ptr->InsertPtr(new RObj2DConfigPin(con->GetPtr(Pins()->GetPin()->GetId()),Pins()->GetRect()));
		}
	}
}


//------------------------------------------------------------------------------
RObj2D::~RObj2D(void)
{
}
