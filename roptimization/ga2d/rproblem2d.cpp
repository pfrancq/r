/*

	R Project Library

	RProblem2D.cpp

	2D Placement Problem - Implementation.

	Copyright 1998-2009 by Pascal Francq (pascal@francq.info).
	Copyright 1998-2008 by the Université Libre de Bruxelles (ULB).

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
using namespace std;


//------------------------------------------------------------------------------
// include files for R Project
#include <rxmlstruct.h>
#include <rxmlfile.h>
#include <rproblem2d.h>
#include <rsize.h>
#include <rxmlparser.h>
#include <rpoints.h>
#include <rvectorbool.h>
using namespace R;



//------------------------------------------------------------------------------
//
// class RProblem2D::Parser
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
class RProblem2D::Parser : public RXMLParser
{
	enum tSection {Undefined,Circuit,Masters,Instances,Connections};
	enum tTag {unknown,main,circuit,masters,master,polygon,terminal,pin,instances,instance,connections,net,connect};
	enum tAttr {noattr,points,id,layer,masterattr,instanceattr,terminalattr};
	tSection Section;
	tTag Tag;
	tAttr Attr;
	RProblem2D* Problem;
	RPolygon* Polygon;
	RObj2D* Obj;
	RStack<RString,true,true,true> Ids;
	RPoints Pts;
	RPoint Tr;
	bool ReadPolygon;
	RString AttrValue;
	RString AttrValue2;
	bool Pin;
	RVectorBool Read;
	RConnection* Connection;
	RObj2DConnector* Connector;

public:
	Parser(RProblem2D* problem,const RURI& uri);
	tTag GetTagType(const RString& ns,const RString& name);
	tAttr GetAttrType(const RString& ns,const RString& name);
	virtual void BeginTag(const RString &namespaceURI, const RString &lName, const RString &name);
	virtual void BeginTagParsed(const RString &namespaceURI, const RString &lName, const RString &name);
	virtual void EndTag(const RString &namespaceURI, const RString &lName, const RString &name);
	virtual void AddAttribute(const RString &namespaceURI, const RString &lName, const RString &name);
	virtual void Value(const RString &value);
};


//------------------------------------------------------------------------------
RProblem2D::Parser::Parser(RProblem2D* problem,const RURI& uri)
	: RXMLParser(uri), Problem(problem), Obj(0), Ids(20), Pts(20), Pin(false), Read(10)
{
	Section=Undefined;
	Read.Init(10,false);
}


//------------------------------------------------------------------------------
RProblem2D::Parser::tTag RProblem2D::Parser::GetTagType(const RString& ns,const RString& name)
{
	if(name=="placement2D")
		return(main);
	if(ns=="http://www.w3.org/2000/svg")
	{
		if(name=="polygon")
			return(polygon);
	}
	else if(ns=="http://www.otlet-institute.org/placement2D")
	{
		if(name=="circuit")
			return(circuit);
		if(name=="terminal")
			return(terminal);
		if(name=="pin")
			return(pin);
		if(name=="masters")
			return(masters);
		if(name=="master")
			return(master);
		if(name=="instances")
			return(instances);
		if(name=="instance")
			return(instance);
		if(name=="connections")
			return(connections);
		if(name=="net")
			return(net);
		if(name=="connect")
			return(connect);
	}
	return(unknown);
}


//------------------------------------------------------------------------------
RProblem2D::Parser::tAttr RProblem2D::Parser::GetAttrType(const RString& ns,const RString& name)
{
	if((name=="xmlns")||(name=="svg"))
		return(noattr);
	else if(ns=="http://www.w3.org/2000/svg")
	{
		if(name=="points")
			return(points);
	}
	else if(ns=="http://www.otlet-institute.org/placement2D")
	{
		if(name=="id")
			return(id);
		if(name=="layer")
			return(layer);
		if(name=="master")
			return(masterattr);
		if(name=="instance")
			return(instanceattr);
		if(name=="terminal")
			return(terminalattr);
	}
	throw RException(GetURI()()+"("+RString::Number(GetLineNb())+"): Invalid attribute '"+name+"'");
}


//------------------------------------------------------------------------------
void RProblem2D::Parser::BeginTag(const RString &namespaceURI, const RString &lName, const RString& name)
{
	Tag=GetTagType(namespaceURI,lName);
	if(Tag==unknown)
		throw RException(GetURI()()+"("+RString::Number(GetLineNb())+"): Invalid tag '"+name+"'");

	switch(Section)
	{
		case Undefined:
		{
			switch(Tag)
			{
				case main:
					return;

				case circuit:
					Section=Circuit;
					ReadPolygon=false;
					Obj=&Problem->Problem;
					Polygon=&Obj->Polygon;
					break;

				case masters:
					Section=Masters;
					break;

				case instances:
					if(!Read[Masters])
						throw RException(GetURI()()+"("+RString::Number(GetLineNb())+"): 'instances' cannot precede 'masters'");
					Section=Instances;
					break;

				case connections:
					if(!Read[Instances])
						throw RException(GetURI()()+"("+RString::Number(GetLineNb())+"): 'connections' cannot precede 'instances'");
					Section=Connections;
					break;

				default:
					throw RException(GetURI()()+"("+RString::Number(GetLineNb())+"): Invalid tag '"+name+"' as root child");
			}
			break;
		}

		case Circuit:
		{
			switch(Tag)
			{
				case polygon:
					AttrValue.Clear();
					break;

				case pin:
					Pin=true;
					Polygon=new RPolygon(2);
					break;

				case terminal:
					Connector=0;
					if(!ReadPolygon)
						throw RException(GetURI()()+"("+RString::Number(GetLineNb())+"): 'polygon' tag must precede '"+name+"'");
					Pts.Clear();
					break;

				default:
					throw RException(GetURI()()+"("+RString::Number(GetLineNb())+"): Invalid tag '"+name+"' in 'circuit'");
			}
			break;
		}

		case Masters:
		{
			switch(Tag)
			{
				case master:
					ReadPolygon=false;
					Obj=0;
					break;

				case terminal:
					Connector=0;
					if(!ReadPolygon)
						throw RException(GetURI()()+"("+RString::Number(GetLineNb())+"): 'polygon' tag must precede '"+name+"'");
					Pts.Clear();
					break;

				case polygon:
					AttrValue.Clear();
					break;

				case pin:
					Pin=true;
					Polygon=new RPolygon(2);
					break;

				default:
					throw RException(GetURI()()+"("+RString::Number(GetLineNb())+"): Invalid tag '"+name+"' in 'masters'");
			}
			break;
		}

		case Instances:
		{
			switch(Tag)
			{
				case instance:
					Obj=0;
					AttrValue.Clear();
					break;

				default:
					throw RException(GetURI()()+"("+RString::Number(GetLineNb())+"): Invalid tag '"+name+"' in 'instances'");
					break;
			}
			break;
		}

		case Connections:
		{
			switch(Tag)
			{
				case net:
					Connection=0;
					break;

				case connect:
					AttrValue.Clear();
					AttrValue2.Clear();
					break;

				default:
					throw RException(GetURI()()+"("+RString::Number(GetLineNb())+"): Invalid tag '"+name+"' in 'connections'");
					break;
			}
			break;
		}

		default:
			throw RException(GetURI()()+"("+RString::Number(GetLineNb())+"): Problem with '"+name+"'");
	}

//	cout<<"Begin: "<<namespaceURI<<" "<<name<<endl;
}


//------------------------------------------------------------------------------
void RProblem2D::Parser::BeginTagParsed(const RString &namespaceURI, const RString &lName, const RString&)
{
	Tag=GetTagType(namespaceURI,lName);
	switch(Tag)
	{
		case master:
			Obj=new RObj2D(Problem->Templates.GetNb(),*Ids(),false);
			Ids.Pop();
			Problem->Templates.InsertPtr(Obj);
			Polygon=&Obj->Polygon;
			break;

		case terminal:
		{
			Connector=new RObj2DConnector(Obj,Obj->Connectors.GetNb(),*Ids(),4);
			Obj->Connectors.InsertPtr(Connector);
			Ids.Pop();
			break;
		}

		case instance:
		{
			// Find the template
			RObj2D* Template(Problem->Templates.GetPtr(AttrValue,false));
			if(!Template)
				throw RException(GetURI()()+"("+RString::Number(GetLineNb())+"): Master '"+AttrValue+"' does not exist");

			// Create the object
			Obj=new RObj2D(Problem->Objs.GetNb(),*Ids(),false);
			Ids.Pop();
			Problem->Objs.InsertPtr(Obj);
			Obj->Polygon=Template->Polygon;
			Obj->CopyConnectors(Template);

			// Initialize the object
			Obj->SetOri(Normal);
			Obj->SetOri(NormalX);
			Obj->SetOri(NormalY);
			Obj->SetOri(NormalYX);
			Obj->SetOri(Rota90);
			Obj->SetOri(Rota90X);
			Obj->SetOri(Rota90Y);
			Obj->SetOri(Rota90YX);
			Obj->Init();

			break;
		}

		case net:
		{
			// Look if connection with a external pin
			Connection=new RConnection(5,1.0);
			RObj2DConnector* External(Problem->Problem.Connectors.GetPtr(*Ids(),false));
			Ids.Pop();
			if(External)
				Connection->Connect.InsertPtr(External);
			Problem->Cons.InsertPtr(Connection);
		}
			break;

		default:
			break;
	}
}


//------------------------------------------------------------------------------
void RProblem2D::Parser::EndTag(const RString &namespaceURI, const RString &lName, const RString &)
{
	Tag=GetTagType(namespaceURI,lName);
	switch(Tag)
	{
		case circuit:
			Obj=0;
			Read[Circuit]=true;
			Section=Undefined;
			break;

		case polygon:
		{
			// Read the points
			bool Ok;
			RChar* Car(AttrValue());
			while(!Car->IsNull())
			{
				// Skip Spaces
				while((!Car->IsNull())&&(Car->IsSpace()))
					Car++;

				// Read X
				RString ReadX;
				while((!Car->IsNull())&&(!Car->IsSpace())&&((*Car)!=','))
					ReadX+=(*(Car++));
				size_t X=ReadX.ToSizeT(Ok);
				if(!Ok)
					throw RException(GetURI()()+"("+RString::Number(GetLineNb())+"): Invalid attribute for 'points'");

				// Skip Spaces, the comma and spaces
				while((!Car->IsNull())&&(Car->IsSpace()))
					Car++;
				if((Car->IsNull())||((*Car)!=','))
					throw RException(GetURI()()+"("+RString::Number(GetLineNb())+"): Invalid attribute for 'points'");
				Car++; // Skip ','
				while((!Car->IsNull())&&(Car->IsSpace()))
					Car++;

				// Read Y
				RString ReadY;
				while((!Car->IsNull())&&(!Car->IsSpace())&&((*Car)!=','))
					ReadY+=(*(Car++));
				size_t Y=ReadY.ToSizeT(Ok);
				if(!Ok)
					throw RException(GetURI()()+"("+RString::Number(GetLineNb())+"): Invalid attribute for 'points'");

				// Add the point
				Polygon->InsertPtr(new RPoint(X,Y));
			}

			// Make computations
			if(!Pin)
			{
				Tr=Polygon->Calibrate();
				Polygon->ReOrder();
				ReadPolygon=true;
			}

			break;
		}

		case pin:
		{
			if(Polygon->GetNb()!=1)
				throw RException(GetURI()()+"("+RString::Number(GetLineNb())+"): 'pin' defines "+RString::Number(Polygon->GetNb())+" points rather than one");
			Pts.InsertPtr(new RPoint(*(*Polygon)[0]));
			delete Polygon;
			Polygon=0;
			Pin=false;
			break;
		}

		case terminal:
		{
			RCursor<RPoint> Cur(Pts);
			size_t i;
			for(Cur.Start(),i=0;!Cur.End();Cur.Next(),i++)
			{
				Connector->Pos[i]=(*Cur());
				Connector->Pos[i]-=Tr;
			}
			Connector->NbPos=Pts.GetNb();
			break;
		}


		case master:
			Obj=0;
			break;

		case masters:
			Read[Masters]=true;
			Section=Undefined;
			break;

		case instances:
			Read[Instances]=true;
			Section=Undefined;
			break;

		case connections:
			Read[Connections]=true;
			Section=Undefined;
			break;

		case connect:
		{
			RObj2D* obj(Problem->Objs.GetPtr(AttrValue,false));
			if(!obj)
				throw RException(GetURI()()+"("+RString::Number(GetLineNb())+"): Wrong object identifier '"+AttrValue+"'");
			RObj2DConnector* con(obj->Connectors.GetPtr(AttrValue2,false));
			if(!con)
				throw RException(GetURI()()+"("+RString::Number(GetLineNb())+"): Wrong terminal identifier '"+AttrValue2+"'");
			Connection->Connect.InsertPtr(con);
			break;
		}

		default:
			break;
	}
}


//------------------------------------------------------------------------------
void RProblem2D::Parser::AddAttribute(const RString& namespaceURI,const RString & lName, const RString &)
{
	Attr=GetAttrType(namespaceURI,lName);
	switch(Attr)
	{
		case points:
			if(Tag!=polygon)
				throw RException(GetURI()()+"("+RString::Number(GetLineNb())+"): Invalid attribute '"+lName+"'");
			break;

		case id:
			if((Tag!=terminal)&&(Tag!=master)&&(Tag!=instance)&&(Tag!=pin)&&(Tag!=net))
				throw RException(GetURI()()+"("+RString::Number(GetLineNb())+"): Invalid attribute '"+lName+"'");
			Ids.Push(new RString());
			break;

		case masterattr:
			if(Tag!=instance)
				throw RException(GetURI()()+"("+RString::Number(GetLineNb())+"): Invalid attribute '"+lName+"'");
			break;

		case instanceattr:
			if(Tag!=connect)
				throw RException(GetURI()()+"("+RString::Number(GetLineNb())+"): Invalid attribute '"+lName+"'");
			break;

		case terminalattr:
			if(Tag!=connect)
				throw RException(GetURI()()+"("+RString::Number(GetLineNb())+"): Invalid attribute '"+lName+"'");
			break;

		case noattr:
		case layer:
			// Nothing to do
			break;
	}
}


//------------------------------------------------------------------------------
void RProblem2D::Parser::Value(const RString& value)
{
	switch(Attr)
	{
		case instanceattr:
		case masterattr:
		case points:
			AttrValue+=value;
			break;

		case terminalattr:
			AttrValue2+=value;

		case id:
			(*Ids())+=value;
			break;

		case noattr:
		case layer:
			// Nothing to do
			break;
	}
}



//------------------------------------------------------------------------------
//
// class RProblem2D
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
RProblem2D::RProblem2D(const RURI& uri)
	: Problem(cNoRef,"Problem",false), Templates(50), Objs(100), Cons(), URI(uri)
{
	Load();
}


//------------------------------------------------------------------------------
void RProblem2D::Load(void)
{
	// Read the file
	Parser File(this,URI);
	File.Open(RIO::Read);

	// Initialize
	DetermineLimit();

	// The object problem have the greatest identifier
	Problem.Id=Objs.GetNb();
	Problem.SetOri(Normal);
	Problem.Init();
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
	size_t j;

	Problem.Polygon.Boundary(r);
	GlobalLimits.X = r.GetWidth();
	GlobalLimits.Y = r.GetHeight();
	while(Cont)
	{
		r.X1++;
		r.Y1++;
		r.X2--;
		r.Y2--;
		Cont=false;
		RCursor<RObj2DConnector> con(Problem.Connectors);
		for(con.Start();!con.End();con.Next())
		{
			for(j=0;j<con()->NbPos;j++)
				if(r.IsIn(con()->Pos[j]))
					Cont=true;
		}
	}
	Limits.X = r.GetWidth();
	Limits.Y = r.GetHeight();
	Translation=r.GetPt1();

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
