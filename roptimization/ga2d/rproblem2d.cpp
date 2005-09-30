/*

	R Project Library

	RProblem2D.cpp

	2D Placement Problem - Implementation.

	Copyright 2001-2005 by the Université Libre de Bruxelles.

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



//------------------------------------------------------------------------------
// include files for ANSI C/C++
#include <stdlib.h>


//------------------------------------------------------------------------------
// include files for R Project
#include <rxmlstruct.h>
#include <rxmlfile.h>
#include <rproblem2d.h>
using namespace R;



//------------------------------------------------------------------------------
//
// class RProblem2D
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
RProblem2D::RProblem2D(void)
	: Problem(NoObject,"Problem",false), Objs(100,50), Cons()
{
}


//------------------------------------------------------------------------------
void RProblem2D::Load(const char* name)
{
	RXMLStruct s;
	RXMLFile f(name,&s);
	RXMLTag *tag;
	unsigned int i,X,Y;
	RString Attr;
	RContainer<RObj2D,true,true> Templates(50,25);
	RPoint Tr;

	// Clear the Problem
	f.Open(RIO::Read);
	Clear();

	// Take the Limits
	tag=s.GetTag("Shape",s.GetTop());
	if(tag)
	{
		// Read the points but the last
		RCursor<RXMLTag> Cur(tag->GetNodes());
		for(Cur.Start(),i=tag->GetNbNodes();--i;Cur.Next())  // Last point = first point
		{
			if(Cur()->GetName()!="Point")
				throw RIOException("Wrong sub-tags in tag 'Shape'");
			// Point
			X = atoi(Cur()->GetAttrValue("X"));
			Y = atoi(Cur()->GetAttrValue("Y"));
			Problem.Polygon.InsertPtr(new RPoint(X,Y));
		}
		Tr=Problem.Polygon.Calibrate();
		Problem.Polygon.ReOrder();
	}

	// Templates (Master Instances)
	tag=s.GetTag("InstanceMasters",s.GetTop());
	if(tag)
	{
		Templates.Clear(tag->GetNbNodes());
		RCursor<RXMLTag> Cur(tag->GetNodes());
		for(Cur.Start();!Cur.End();Cur.Next())
		{
			if(Cur()->GetName()=="Master")
				CreateObj(Cur(),Templates);
		}
	}

	// Objects (Instances)
	tag=s.GetTag("Instances",s.GetTop());
	if(tag)
	{
		Objs.Clear(tag->GetNbNodes());
		RCursor<RXMLTag> Cur(tag->GetNodes());
		for(Cur.Start();!Cur.End();Cur.Next())
		{
			if(Cur()->GetName()==RString("Instance"))
				CreateObj(Cur(),Templates);
		}
	}

	// Connectors of global shape
	tag=s.GetTag("Terminals",s.GetTop());
	if(tag)
	{
		RCursor<RXMLTag> Cur(tag->GetNodes());
		for(Cur.Start();!Cur.End();Cur.Next())
			CreateConnector(Cur(),&Problem,Tr);
	}

	// Connections
	tag=s.GetTag("Connections",s.GetTop());
	if(tag)
	{
		Cons.Clear(tag->GetNbNodes());
		RCursor<RXMLTag> Cur(tag->GetNodes());
		for(Cur.Start();!Cur.End();Cur.Next())
			CreateNet(Cur());
		Cons.Init();
	}

	DetermineLimit();
	// The object problem have the greathers id
	Problem.Id=Objs.GetNb();
	Problem.SetOri(Normal);
	Problem.Init();
}


//------------------------------------------------------------------------------
void RProblem2D::CreateObj(RXMLTag* o,RContainer<RObj2D,true,true>& ts)
{
	RObj2D *obj,*t;
	bool IsObj;
	RXMLTag *tag;
	unsigned int i;
	RCoord X,Y;
	RPoint Tr;

	IsObj=(o->GetName()=="Instance");
	if(IsObj)
	{
		obj = new RObj2D(Objs.GetNb(),o->GetAttrValue("Id"),false);
		Objs.InsertPtr(obj);
	}
	else
	{
		obj = new RObj2D(ts.GetNb(),o->GetAttrValue("Id"),false);
		ts.InsertPtr(obj);
	}

	if(IsObj&&(o->IsAttrDefined("Master")))
	{
		t=ts.GetPtr<const char*>(o->GetAttrValue("Master"),false);
		if(t)
		{
			obj->Polygon=t->Polygon;
			obj->CopyConnectors(t);
		}
	}
	else
	{
		// Read Geometric Informations
		tag=o->GetTag("Shape");
		if(tag)
		{
			RCursor<RXMLTag> Cur(tag->GetNodes());
			for(Cur.Start(),i=tag->GetNbNodes();--i;Cur.Next())              // Last Point==First Point
			{
				if(Cur()->GetName()=="Point")
				{
					X = atoi(Cur()->GetAttrValue("X"));
					Y = atoi(Cur()->GetAttrValue("Y"));
					obj->Polygon.InsertPtr(new RPoint(X,Y));
				}
			}
			Tr=obj->Polygon.Calibrate();
			obj->Polygon.ReOrder();
		}

		// Read connectors
		tag=o->GetTag("Terminals");
		if(tag)
		{
			RCursor<RXMLTag> Cur(tag->GetNodes());
			for(Cur.Start();!Cur.End();Cur.Next())
				CreateConnector(Cur(),obj,Tr);
		}
	}

	if(IsObj)
	{
		// Set the Orientation
		obj->SetOri(Normal);
		obj->SetOri(NormalX);
		obj->SetOri(NormalY);
		obj->SetOri(NormalYX);
		obj->SetOri(Rota90);
		obj->SetOri(Rota90X);
		obj->SetOri(Rota90Y);
		obj->SetOri(Rota90YX);

		// Initialise the object
		obj->Init();
	}
}


//------------------------------------------------------------------------------
void RProblem2D::CreateConnector(RXMLTag* c,RObj2D* obj,const RPoint& t)
{
	RObj2DConnector* con;
	unsigned int i;
	RXMLTag *pt;

	if(c->GetName()!="Terminal") return;
	con=new RObj2DConnector(obj,obj->Connectors.GetNb(),c->GetAttrValue("Id"),c->GetNbNodes());
	RCursor<RXMLTag> Cur(c->GetNodes());
	for(Cur.Start(),i=c->GetNbNodes();--i;Cur.Next())              // Last Point==First Point
	{
		pt=Cur()->GetTag("Shape")->GetTag("Point");
		con->Pos[i].Set(atoi(pt->GetAttrValue("X")),atoi(pt->GetAttrValue("Y")));
		con->Pos[i]-=t;
	}
	con->NbPos=c->GetNbNodes();
	obj->Connectors.InsertPtr(con);
}


//------------------------------------------------------------------------------
void RProblem2D::CreateNet(RXMLTag* n)
{
	double w;
	RObj2DConnector* con;
	RObj2D* obj;
	RConnection* cnt;

	if(n->GetName()!="Net") return;

	if(n->IsAttrDefined("Weight"))
		w=atof(n->GetAttrValue("Weight"));
	else
		w=1.0;

	// Look if connection with a external pin
	cnt=new RConnection(n->GetNbNodes(),w);
	con=Problem.Connectors.GetPtr<const char*>(n->GetAttrValue("Id"),false);
	if(con)
		cnt->Connect.InsertPtr(con);

	// Go through objects
	RCursor<RXMLTag> tab(n->GetNodes());
	for(tab.Start();!tab.End();tab.Next())
	{
		if(tab()->GetName()!="Connect") continue;
		obj=Objs.GetPtr<const char*>(tab()->GetAttrValue("Instance"),false);
		if(!obj) continue;
		con=obj->Connectors.GetPtr<const char*>(tab()->GetAttrValue("Terminal"),false);
		if(!con) continue;
		cnt->Connect.InsertPtr(con);
	}
	Cons.InsertPtr(cnt);
}


//------------------------------------------------------------------------------
void RProblem2D::Clear(void)
{
	Objs.Clear();
	Cons.Clear();
}


//------------------------------------------------------------------------------
void RProblem2D::DetermineLimit(void)
{
	RRect r;
	bool Cont=true;
	unsigned int j;

	Problem.Polygon.Boundary(r);
	GlobalLimits.X = r.Width();
	GlobalLimits.Y = r.Height();
	while(Cont)
	{
		r.Pt1.X++;
		r.Pt1.Y++;
		r.Pt2.X--;
		r.Pt2.Y--;
		Cont=false;
		RCursor<RObj2DConnector> con(Problem.Connectors);
		for(con.Start();!con.End();con.Next())
		{
			for(j=0;j<con()->NbPos;j++)
				if(r.IsIn(con()->Pos[j]))
					Cont=true;
		}
	}
	Limits.X = r.Width();
	Limits.Y = r.Height();
	Translation=r.Pt1;

	// Translate the Connectors Position.
//	for(i=Problem.Connectors.NbPtr+1,con=Problem.Connectors.Tab;--i;con++)
//	{
//		for(j=0;j<(*con)->NbPos;j++)
//			(*con)->Pos[j]-=Translation;
//	}
}


//------------------------------------------------------------------------------
RProblem2D::~RProblem2D(void)
{
}
