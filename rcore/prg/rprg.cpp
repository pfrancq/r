/*

	R Project Library

	RPrg.cpp

	Generic script program - Implementation.

	Copyright 2002-2003 by the Universit�Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).

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
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>


//------------------------------------------------------------------------------
// include files for R Project
#include <rprg/rprg.h>
#include <rprg/rprginstfor.h>
#include <rprg/rprginstmethod.h>
#include <rprg/rprgvarconst.h>
#include <rprg/rprgvarref.h>
#include <rprg/rprgclass.h>
using namespace R;



//-----------------------------------------------------------------------------
//
// RPrg
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
RPrg::RPrg(RString f,RPrgOutput* o) throw(std::bad_alloc)
	: FileName(f), Cout(o), Insts(40), Vars(10,5), Classes(10,5), Prg(FileName)
{
	Prg.Open(RIO::Read);
}


//-----------------------------------------------------------------------------
void RPrg::Load(void) throw(std::bad_alloc,RException)
{
	RPrgInst* i;

	Prg.SetRem("#");
	ReadLine=true;
	while((!ReadLine)||(!Prg.Eof()&&(ReadLine)))
	{
		i=AnalyseLine(Prg);
		if(i)
			Insts.InsertPtr(i);
	}
}


//-----------------------------------------------------------------------------
unsigned int RPrg::CountTabs(const RString& line)
{
	int tabs;
	RCharCursor Cur(line);

	// Count tabs
	tabs=0;
	while((!Cur.End())&&(Cur()=='\t'))
	{
		tabs++;
		Cur.Next();
	}
	return(tabs);
}


//-----------------------------------------------------------------------------
RPrgInst* RPrg::AnalyseLine(RTextFile& prg) throw(std::bad_alloc,RException)
{
	RString l;
	RString obj;
	RString name;
	RChar what;
	char tabs;
	unsigned int pos;
	unsigned int len;
	RCharCursor Cur;

	// Read the line
	if(ReadLine)
		buf=prg.GetLine();
	Cur.Set(buf);

	// Skip Spaces and count tabs
	tabs=CountTabs(buf);
	while((!Cur.End())&&(Cur().IsSpace()))
		Cur.Next();

	// Read if it is an Object or instruction or something dummy
	pos=Cur.GetPos();
	len=0;
	while((!Cur.End())&&(Cur()!=RChar('.'))&&(Cur()!=RChar('='))&&(Cur()!=RChar('('))&&(!Cur().IsSpace()))
	{
		Cur.Next();
		len++;
	}
	obj=buf.Mid(pos,len);
	what=Cur();
	Cur.Next();
	pos=Cur.GetPos();

	// Look if instruction
	if(what=='(')
	{
		// If is "for"
		if(obj=="for")
		{
			RPrgInstFor* f=new RPrgInstFor(buf.Mid(Cur.GetPos(),buf.GetLen()-Cur.GetPos()+1),tabs);

			// Read the next lines
			buf=Prg.GetLine();
			if(buf.IsEmpty()) return(f);
			pos=0;
			ReadLine=false;
			while((!ReadLine)||(!Prg.Eof()&&(ReadLine)))
			{
				if(CountTabs(buf)<=f->GetTabs())
					break;
				f->AddInst(AnalyseLine(prg));
				if(ReadLine)
				{
					buf=prg.GetLine();
					if(buf.IsEmpty()) return(f);
					pos=0;
					ReadLine=false;
				}
			}
			return(f);
		}
		throw RException(RString("Instruction \"")+RString(obj)+"\" does not exist.");
	}

	// Look if call to an object
	if(what=='.')
	{
		// Look if the object is known
		RPrgClass* c=Classes.GetPtr<RString>(obj);
		if(!c)
			throw RException(RString("Object \"")+obj+"\" unknown");

		// Read the methods name
		len=0;
		pos=Cur.GetPos();
		while((!Cur.End())&&(Cur()!=RChar('(')))
		{
			//ptr++;
			Cur.Next();
			len++;
		}
		RPrgFunc* method=c->GetMethod(buf.Mid(pos,len));
		Cur.Next();
		if(!method)
		{
			RString msg("Method \"");
			msg+=name+"\" unknown for object \""+obj+"\"";
			throw RException(msg);
		}

		// Create the instruction
		RPrgInstMethod* inst=new RPrgInstMethod(method);
		inst->AnalyseParam(buf.Mid(Cur.GetPos(),buf.GetLen()-Cur.GetPos()+1));
		ReadLine=true;
		return(inst);
	}

	ReadLine=true;
	return(0);
}


//-----------------------------------------------------------------------------

void RPrg::AnalyseParam(const RString& params,RContainer<RPrgVar,true,false>* values) throw(std::bad_alloc,RException)
{
	unsigned int len;
	unsigned int pos;
	RCharCursor Cur(params);

	while(!Cur.End())
	{
		// Skip before something
		while((!Cur.End())&&(Cur()!=RChar('"'))&&(!Cur().IsAlpha()))
			Cur.Next();
		if(Cur.End())
			return;

		if(Cur()==RChar('"'))
		{
			Cur.Next();

			// Read Value and skip " and next thing
			len=0;
			pos=Cur.GetPos();
			while((!Cur.End())&&(Cur()!=RChar('"')))
			{
				Cur.Next();
				len++;
			}

			// Value
			values->InsertPtr(new RPrgVarConst(params.Mid(pos,len)));

			Cur.Next();
		}
		else
		{
			// Look until , or )
			len=0;
			pos=Cur.GetPos();
			while((!Cur.End())&&(Cur()!=RChar(','))&&(Cur()!=RChar(')')))
			{
				Cur.Next();
				len++;
			}

			// Ref
			values->InsertPtr(new RPrgVarRef(params.Mid(pos,len)));

			Cur.Next();
		}
	}
}


//-----------------------------------------------------------------------------
void RPrg::Exec(void) throw(RException)
{
	for(Insts.Start();!Insts.End();Insts.Next())
		Insts()->Run(this,Cout);
}


//-----------------------------------------------------------------------------
void RPrg::AddVar(RPrgVar* var) throw(std::bad_alloc,RException)
{
	Vars.InsertPtr(var);
}


//-----------------------------------------------------------------------------
void RPrg::DelVar(RPrgVar* var) throw(std::bad_alloc,RException)
{
	Vars.DeletePtr(var);
}


//-----------------------------------------------------------------------------
const char* RPrg::GetValue(const char* var) throw(RException)
{
	RPrgVar* v=Vars.GetPtr<const char*>(var);

	if(!v) return(0);
	return(v->GetValue(this));
}


//-----------------------------------------------------------------------------
RPrg::~RPrg(void)
{
}
