/*

	R Project Library

	RProblem2D.cpp

	2D Placement Problem - Implementation.

	Copyright 2001-2003 by the Universit�Libre de Bruxelles.

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
#include <rstd/rxmlstruct.h>
#include <rstd/rxmlfile.h>
#include <rga/rproblem2d.h>
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
void RProblem2D::Load(const char* name) throw(RIOException)
{
	RXMLStruct s;
	RXMLFile f(name,&s);
	RXMLTag *tag,**tab;
	unsigned int i,X,Y;
	RString Attr;
	RContainer<RObj2D,true,true> Templates(50,25);
	RPoint Tr;
	RCursor<RXMLTag> Cur;

	// Clear the Problem
	f.Open(RIO::Read);
	Clear();

	// Take the Limits
	tag=s.GetTag("Shape",s.GetTop());
	if(tag)
	{
		// Read the points but the last
		Cur.Set(tag);
		for(Cur.Start(),i=Cur.GetNb();--i;Cur.Next())  // Last point = first point
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
		Templates.Clear(tag->NbPtr,tag->NbPtr/2);
		for(i=tag->NbPtr+1,tab=tag->Tab;--i;tab++)
		{
			if((*tab)->GetName()=="Master")
				CreateObj(*tab,Templates);
		}
	}

	// Objects (Instances)
	tag=s.GetTag("Instances",s.GetTop());
	if(tag)
	{
		Objs.Clear(tag->NbPtr,tag->NbPtr/2);
		for(i=tag->NbPtr+1,tab=tag->Tab;--i;tab++)
			if((*tab)->GetName()==RString("Instance"))
				CreateObj(*tab,Templates);
	}

	// Connectors of global shape
	tag=s.GetTag("Terminals",s.GetTop());
	if(tag)
	{
		for(i=tag->NbPtr+1,tab=tag->Tab;--i;tab++)
			CreateConnector(*tab,&Problem,Tr);
	}

	// Connections
	tag=s.GetTag("Connections",s.GetTop());
	if(tag)
	{
		Cons.Clear(tag->NbPtr,tag->NbPtr/2);
		for(i=tag->NbPtr+1,tab=tag->Tab;--i;tab++)
			CreateNet(*tab);
		Cons.Init();
	}

	DetermineLimit();
	// The object problem have the greathers id
	Problem.Id=Objs.NbPtr;
	Problem.SetOri(Normal);
	Problem.Init();
}


//------------------------------------------------------------------------------
void RProblem2D::CreateObj(RXMLTag* o,RContainer<RObj2D,true,true>& ts)
{
	RObj2D *obj,*t;
	bool IsObj;
	RXMLTag *tag,**tab;
	unsigned int i;
	RCoord X,Y;
	RPoint Tr;

	IsObj=(o->GetName()=="Instance");
	if(IsObj)
	{
		obj = new RObj2D(Objs.NbPtr,o->GetAttrValue("Id"),false);
		Objs.InsertPtr(obj);
	}
	else
	{
		obj = new RObj2D(ts.NbPtr,o->GetAttrValue("Id"),false);
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
			for(i=tag->NbPtr,tab=tag->Tab;--i;tab++)             // Last Point==First Point
			{
				if((*tab)->GetName()=="Point")
				{
					X = atoi((*tab)->GetAttrValue("X"));
					Y = atoi((*tab)->GetAttrValue("Y"));
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
			for(i=tag->NbPtr+1,tab=tag->Tab;--i;tab++)
				CreateConnector(*tab,obj,Tr);
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
	RXMLTag *pt,**tab;

	if(c->GetName()!="Terminal") return;
	con=new RObj2DConnector(obj,obj->Connectors.NbPtr,c->GetAttrValue("Id"),c->NbPtr);
	for(i=0,tab=c->Tab;i<c->NbPtr;i++,tab++)
	{
		pt=(*tab)->GetTag("Shape")->GetTag("Point");
		con->Pos[i].Set(atoi(pt->GetAttrValue("X")),atoi(pt->GetAttrValue("Y")));
		con->Pos[i]-=t;
	}
	con->NbPos=c->NbPtr;
	obj->Connectors.InsertPtr(con);
}


//------------------------------------------------------------------------------
void RProblem2D::CreateNet(RXMLTag* n)
{
	double w;
	RXMLTag** tab;
	unsigned int i;
	RObj2DConnector* con;
	RObj2D* obj;
	RConnection* cnt;

	if(n->GetName()!="Net") return;

	if(n->IsAttrDefined("Weight"))
		w=atof(n->GetAttrValue("Weight"));
	else
		w=1.0;

	// Look if connection with a external pin
	cnt=new RConnection(n->NbPtr,w);
	con=Problem.Connectors.GetPtr<const char*>(n->GetAttrValue("Id"),false);
	if(con)
		cnt->Connect.InsertPtr(con);

	// Go through objects
	for(i=n->NbPtr+1,tab=n->Tab;--i;tab++)
	{
		if((*tab)->GetName()!="Connect") continue;
		obj=Objs.GetPtr<const char*>((*tab)->GetAttrValue("Instance"),false);
		if(!obj) continue;
		con=obj->Connectors.GetPtr<const char*>((*tab)->GetAttrValue("Terminal"),false);
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
	unsigned int i,j;
	RObj2DConnector **con;

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
		for(i=Problem.Connectors.NbPtr+1,con=Problem.Connectors.Tab;--i;con++)
		{
			for(j=0;j<(*con)->NbPos;j++)
				if(r.IsIn((*con)->Pos[j]))
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
