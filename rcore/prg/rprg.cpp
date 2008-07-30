/*

	R Project Library

	RPrg.cpp

	Generic script program - Implementation.

	Copyright 2002-2008 by the Université Libre de Bruxelles.

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
#include <rprg.h>
#include <rprginstfor.h>
#include <rprginstassign.h>
#include <rprginstmethod.h>
#include <rprgvarliteral.h>
#include <rprgvarref.h>
#include <rprgclass.h>
#include <rprgfunc.h>
using namespace std;
using namespace R;



//------------------------------------------------------------------------------
//
// class RPrgException
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
RPrgException::RPrgException(const RPrg* prg) throw()
	: RException(), Prg(prg)
{
}


//------------------------------------------------------------------------------
RPrgException::RPrgException(const RPrg* prg,const char* str) throw()
	: RException(prg->WhatTreated()+": "+str), Prg(prg)
{
}



//-----------------------------------------------------------------------------
//
// RPrg
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
RPrg::RPrg(RString f,RPrgOutput* o)
	: FileName(f), Cout(o), Insts(40,40), Vars(10,5), Classes(10,5), Blocks(10,5),
	  Prg(FileName)

{
	Prg.SetRem("#");
	Prg.SetParseSpace(RTextFile::LeaveSpaces);
	Classes.InsertPtr(new RPrgStringClass());
}


//-----------------------------------------------------------------------------
void RPrg::Load(void)
{
	Prg.Open(RIO::Read);
	if(Prg.End())
		return;

	// Treat the file
	while(!Prg.End())
	{
		// Skip Spaces and count tabs
		size_t tabs=CountTabs(buf);

		// Verify the correct tabs
		if(Blocks.GetNb())
		{
			if(tabs==Blocks()->GetTabs()-1)
			{
				// Leaving current block
				Blocks.Pop();
			}
			else if(tabs!=Blocks()->GetTabs())
				RPrgException(this,"Wrong number of tabs");
		}
		else if(tabs)
			throw RPrgException(this,"No tabs are allowed");

		// Read next instruction (in Inst)
		Line=Prg.GetLineNb();
		RString Inst=Prg.GetToken(".(=");
		switch(Prg.GetChar().Latin1())
		{
			case '(': // Read of an instruction
			{
				// If is "for"
				if(Inst=="for")
				{
					RPrgInstFor* f=new RPrgInstFor(this,tabs);
					AddInst(f);
					Blocks.Push(f);
				}
				else
					throw RPrgException(this,RString("Unknown instruction '")+Inst+"'.");
				break;
			}
			case '.':   // Call to the method of an object
			{
				// Read the methods name
				RString Method(Prg.GetToken("("));
				if(Prg.GetChar()!='(')
					throw RPrgException(this,"Wrong call to the method '"+Method+"' for object '"+Inst+"'");

				// Create the instruction
				AddInst(new RPrgInstMethod(this,Inst,Method));
				break;
			}
			case '=':     // Must be an assignment
			{
				// Get the next token after the = and the first "
				RString Cmd(Prg.GetToken('\"'));

				if(Cmd=="new")
				{
					// Read the class name
					RString ClassName(Prg.GetToken("("));
					RPrgClass* Class=Classes.GetPtr(ClassName);
					if(!Class)
						throw RPrgException(this,"Unknown class '"+ClassName+"'");
					Prg.GetChar(); // Skip (

					// Create the instruction
					AddInst(new RPrgInstNew(this,Inst,Class));
				}
				else if(!Cmd.IsEmpty())
				{
					// Assign of an existing variable
					AddInst(new RPrgInstAssignVar(Inst,new RPrgVarRef(Prg.GetWord())));
				}
				else
				{
					// Assign of an literal.
					Prg.GetChar(); // Skip "
					RString Val=ReadLiteral();
					AddInst(new RPrgInstAssignVar(Inst,new RPrgVarLiteral(Val)));//ReadLiteral())));  // = Literal
				}
				break;
			}
		}
	}
}


//-----------------------------------------------------------------------------
size_t RPrg::CountTabs(const RString&)
{
	// Count leading spaces
	size_t tabs(0);
	while((!Prg.End())&&(Prg.GetNextChar()=='\t'))
	{
		tabs++;
		Prg.GetChar();
	}
	Prg.SkipSpaces();
	return(tabs);
}


//-----------------------------------------------------------------------------
RString RPrg::WhatTreated(void) const
{
	return(FileName+"("+RString::Number(Line)+")");
}


//-----------------------------------------------------------------------------
RString RPrg::ReadLiteral(void)
{
	bool Cont(true);
	RString Val;
	while(Cont)
	{
		RString Part(Prg.GetToken("\""));
		Val+=Part;
		RChar end=Prg.GetChar();
		if(end=='"')
			Cont=false;
		else
		{
			// Can be a space but not an end of line
			if(RTextFile::Eol(end))
				throw RPrgException(this,"A value cannot be separated on multiple lines");
			Val+=end;
		}
	}
	return(Val);
}


//-----------------------------------------------------------------------------
void RPrg::AnalyseParam(RContainer<RPrgVar,true,false>& values)
{
	bool SomethingToRead=false;

	// This function reads something such as "val1",val,"val3"....,"valn")
	while(Prg.GetNextChar()!=')')
	{
		Prg.SkipSpaces();

		// Two possibilities:
		// " : Value
		// Either : Reference
		RChar What=Prg.GetChar();
		if(What=='"')
		{
			values.InsertPtr(new RPrgVarLiteral(ReadLiteral()));
			SomethingToRead=false;
		}
		else
		{
			RString Var(What+Prg.GetToken("),"));
			values.InsertPtr(new RPrgVarRef(Var));
			SomethingToRead=false;
		}

		// Skip spaces
		Prg.SkipSpaces();
		if(Prg.GetNextChar()==',')
		{
			SomethingToRead=true;
			Prg.GetChar();
		}
	}
	if(SomethingToRead)
		throw RPrgException(this,"A parameter is missing");
	Prg.GetChar();     	// Skip )
}


//-----------------------------------------------------------------------------
void RPrg::Exec(void)
{
	RCursor<RPrgInst> Cur(Insts);
	for(Cur.Start();!Cur.End();Cur.Next())
		Cur()->Run(this,Cout);
}


//-----------------------------------------------------------------------------
void RPrg::AddVar(RPrgVar* var)
{
	if(!var)
		return;
	if(Blocks.GetNb())
		Blocks()->Vars.InsertPtr(var,true);
	else
		Vars.InsertPtr(var,true);
}


//-----------------------------------------------------------------------------
void RPrg::DelVar(RPrgVar* var)
{
	if(!var)
		return;
	if(Blocks.GetNb())
		Blocks()->Vars.DeletePtr(*var);
	else
		Vars.DeletePtr(*var);
}


//-----------------------------------------------------------------------------
void RPrg::AddInst(RPrgInst* inst)
{
	if(!inst)
		return;
	if(Blocks.GetNb())
		Blocks()->AddInst(inst);
	else
		Insts.InsertPtr(inst);
}


//-----------------------------------------------------------------------------
RPrgVar* RPrg::Find(const RString& name) const
{
	for(size_t i=0;i<Blocks.GetNb();i++)
	{
		RPrgVar* v=Blocks[i]->Vars.GetPtr(name);
		if(v)
			return(v);
	}
	return(Vars.GetPtr(name));
}


//-----------------------------------------------------------------------------
RString RPrg::GetValue(const RString& var)
{
	RPrgVar* v=Find(var);
	if(!v) return(RString::Null);
	return(v->GetValue(this));
}


//-----------------------------------------------------------------------------
RCursor<RPrgClass> RPrg::GetClasses(void) const
{
	return(RCursor<RPrgClass>(Classes));
}


//-----------------------------------------------------------------------------
RPrg::~RPrg(void)
{
}
