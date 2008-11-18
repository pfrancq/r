/*

	R Project Library

	RInterpreter.cpp

	Interpreter - Implementation.

	Copyright 2008 by the Université Libre de Bruxelles.

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
#include <rinterpreter.h>
#include <rprginstfor.h>
#include <rprginstdelete.h>
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
RPrgException::RPrgException(const RInterpreter* kernel,const char* str) throw()
	: RException(kernel->WhatTreated()+str)
{
}



//-----------------------------------------------------------------------------
//
// RInterpreter
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
RInterpreter::RInterpreter(RPrgOutput* o)
	: Cout(o), Classes(10,5), tmpBlocks(10,5), Scopes(20,10), Char(0), File()

{
	Classes.InsertPtr(new RPrgStringClass());

	// Push the main block (containing only global variables and classes).
	tmpBlocks.Push(new RPrgInstBlock(this,0));
	Scopes.Push(new RPrgScope());
}


//-----------------------------------------------------------------------------
void RInterpreter::TreatLine(size_t depth,RString line)
{
	// Verify the correct tabs and eventually pop the blocks
	if(depth>tmpBlocks()->GetDepth())
		throw RPrgException(this,"Wrong number of tabs");
	while(depth<tmpBlocks()->GetDepth())
		tmpBlocks.Pop();

	// Read next instruction (in Inst)
	if(line.IsEmpty())
		return;
	Char=line();
	RString Inst(GetToken(".(="));
	SkipSpaces();
	char Car=Char->Latin1();
	Char++;

	// Treat the instruction
	switch(Car)
	{
		case '(': // Read of an instruction
		{
			// If is "for"
			if(Inst=="for")
			{
				RPrgInstFor* f=new RPrgInstFor(this,depth+1);
				AddInst(f);
				tmpBlocks.Push(f);
				Eol(true);
			}
			else if(Inst=="delete")
			{
				AddInst(new RPrgInstDelete(this));
				Eol(false);
			}
			else
				throw RPrgException(this,RString("Unknown instruction '")+Inst+"'.");
			break;
		}
		case '.':   // Call to the method of an object
		{
			// Read the methods name
			RString Method(GetToken('('));
			if((*Char)!='(')
				throw RPrgException(this,"Wrong call to the method '"+Method+"' for object '"+Inst+"'");
			Char++;

			// Create the instruction
			AddInst(new RPrgInstMethod(this,Inst,Method));
			Eol(false);
			break;
		}
		case '=':     // Must be an assignment
		{
			// Get the next token after the = and the first "
			RString Cmd(GetToken('\"'));

			if(Cmd=="new")
			{
				// Read the class name
				RString ClassName(GetToken('('));
				RPrgClass* Class=Classes.GetPtr(ClassName);
				if(!Class)
					throw RPrgException(this,"Unknown class '"+ClassName+"'");
				Char++; // Skip (

				// Create the instruction
				AddInst(new RPrgInstNew(this,Inst,Class));
			}
			else if(!Cmd.IsEmpty())
			{
				bool ok;
				Cmd.ToULong(ok);
				if(ok)
				{
					AddInst(new RPrgInstAssignVar(this,Inst,new RPrgVarLiteral(Cmd)));
				}
				else
				{
					Cmd.ToLong(ok);
					if(ok)
					{
						AddInst(new RPrgInstAssignVar(this,Inst,new RPrgVarLiteral(Cmd)));
					}
					else
					{
						bool b=Cmd.ToBool(ok,true);
						if(ok)
						{
							if(b)
								AddInst(new RPrgInstAssignVar(this,Inst,new RPrgVarLiteral("1")));
							else
								AddInst(new RPrgInstAssignVar(this,Inst,new RPrgVarLiteral("0")));
						}
						else
						{
							// Assign of an existing variable
							AddInst(new RPrgInstAssignVar(this,Inst,new RPrgVarRef(Cmd)));
						}
					}
				}
			}
			else
			{
				// Assign of an literal.
				Char++; // Skip "
				AddInst(new RPrgInstAssignVar(this,Inst,new RPrgVarLiteral(ReadLiteral())));
			}
			Eol(false);
			break;
		}
		default:
			throw RPrgException(this,"Invalid instruction '"+Inst+"'");
	}
}


//-----------------------------------------------------------------------------
RString RInterpreter::WhatTreated(void) const
{
	if(File.IsEmpty())
		return(RString::Null);
	return(File+"("+RString::Number(Line)+"): ");
}


//-----------------------------------------------------------------------------
RString RInterpreter::ReadLiteral(void)
{
	bool Cont(true);
	RString Val;
	while(Cont)
	{
		RString Part(GetToken('\"'));
		Val+=Part;
		RChar end=(*Char);
		Char++;
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
void RInterpreter::AnalyseParam(RContainer<RPrgVar,true,false>& values)
{
	bool SomethingToRead=false;

	// This function reads something such as "val1",val,"val3"....,"valn")
	while((*Char)!=')')
	{
		while((!Char->IsNull())&&(Char->IsSpace()))
			Char++;

		// Two possibilities:
		// " : Value
		// Either : Reference
		RChar What=(*(Char++));
		if(What=='"')
		{
			values.InsertPtr(new RPrgVarLiteral(ReadLiteral()));
			SomethingToRead=false;
		}
		else
		{
			RString Var(What+GetToken("),"));
			bool ok;
			Var.ToULong(ok);
			if(ok)
			{
				values.InsertPtr(new RPrgVarLiteral(Var));
			}
			else
			{
				Var.ToLong(ok);
				if(ok)
				{
					values.InsertPtr(new RPrgVarLiteral(Var));
				}
				else
				{
					bool b=Var.ToBool(ok,true);
					if(ok)
					{
						if(b)
							values.InsertPtr(new RPrgVarLiteral("1"));
						else
							values.InsertPtr(new RPrgVarLiteral("0"));
					}
					else
					{
						values.InsertPtr(new RPrgVarRef(Var));
					}
				}
			}
			SomethingToRead=false;
		}

		// Skip spaces
		while((!Char->IsNull())&&(Char->IsSpace()))
			Char++;

		if((*Char)==',')
		{
			SomethingToRead=true;
			Char++;
		}
	}
	if(SomethingToRead)
		throw RPrgException(this,"A parameter is missing");
	Char++;     	// Skip )
}


//-----------------------------------------------------------------------------
void RInterpreter::Eol(bool dbl)
{
	bool One=false;

	while(!Char->IsNull())
	{
		RChar car=(*(Char++));
		if(dbl&&(car==':'))
		{
			if(One)
				throw RPrgException(this,"Only one ':' is accepted");
			One=true;
		}
		else if(!car.IsSpace())
			throw RPrgException(this,"Only spaces are allowed");
	}
}


//-----------------------------------------------------------------------------
void RInterpreter::Exec(void)
{
	RCursor<RPrgInst> Cur(tmpBlocks()->Insts);
	for(Cur.Start();!Cur.End();Cur.Next())
		Cur()->Run(this,Cout);
}


//-----------------------------------------------------------------------------
void RInterpreter::Run(const RURI& uri)
{
	// Open the file
	RPrg Prg(uri);
	File=uri;
	Prg.Open(RIO::Read);

	// Load the instructions
	tmpBlocks.Push(new RPrgInstBlock(this,1));  // Create a block for the program
	Scopes.Push(new RPrgScope());
	while(!Prg.End())
	{
		// Skip Spaces and count tabs
		size_t depth=Prg.SkipCountSpaces('\t')+1;
		Line=Prg.GetLineNb();
		TreatLine(depth,Prg.GetLine());
	}
	while(1<tmpBlocks()->GetDepth())
		tmpBlocks.Pop();
	Exec();                  // Execute the instructions
	tmpBlocks.Pop();         // Remove the block of the program
	Scopes.Pop();
}


//-----------------------------------------------------------------------------
void RInterpreter::Exec(const RString& insts)
{
	try
	{
		File=RString::Null;
		Line=0;

		TreatLine(0,insts);
		Exec();               // Execute the instructions
	}
	catch(RPrgException& e)
	{
		if(Cout)
			Cout->WriteStr(e.GetMsg());
		else
			throw e;
	}
	catch(...)
	{
		if(Cout)
			Cout->WriteStr("RInterpreter: Unknown error");
		else
			throw RPrgException(this,"RInterpreter: Unknown error");
	}
	tmpBlocks()->ClearInstructions();
}


//-----------------------------------------------------------------------------
void RInterpreter::AddVar(RPrgVar* var)
{
	if(!var)
		return;
//	cout<<"AddVar '"<<var->GetName()<<"' in "<<Blocks()->GetDepth()<<endl;
	Scopes()->InsertPtr(var,true);
}


//-----------------------------------------------------------------------------
void RInterpreter::DelVar(RPrgVar* var)
{
	if(!var)
		return;
//	cout<<"DelVar '"<<var->GetName()<<"' in "<<Blocks()->GetDepth()<<endl;
	Scopes()->DeletePtr(*var);
}


//-----------------------------------------------------------------------------
void RInterpreter::AddClass(RPrgClass* c)
{
	Classes.InsertPtr(c);
}


//-----------------------------------------------------------------------------
void RInterpreter::AddInst(RPrgInst* inst)
{
	if(!inst)
		return;
	tmpBlocks()->AddInst(inst);
}


//-----------------------------------------------------------------------------
RPrgVar* RInterpreter::Find(const RString& name) const
{
	for(size_t i=0;i<Scopes.GetNb();i++)
	{
		RPrgVar* v=Scopes[i]->GetPtr(name);
		if(v)
			return(v);
	}
	throw RPrgException(this,"RInterpreter::Find(const RString&): Unknown variable '"+name+"'");
}


//-----------------------------------------------------------------------------
RString RInterpreter::GetValue(const RString& var)
{
	RPrgVar* v=Find(var);
	return(v->GetValue(this));
}


//-----------------------------------------------------------------------------
RCursor<RPrgClass> RInterpreter::GetClasses(void) const
{
	return(RCursor<RPrgClass>(Classes));
}


//-----------------------------------------------------------------------------
RString RInterpreter::GetWord(void)
{
	SkipSpaces();
	const RChar* ptr=Char;
	RString Word;
	size_t nb=0;
	while((!Char->IsNull())&&(!Char->IsSpace()))
	{
		Char++;
		nb++;
	}
	Word.Copy(ptr,nb);
	return(Word);
}


//-----------------------------------------------------------------------------
RString RInterpreter::GetToken(const RString chars)
{
	SkipSpaces();
	const RChar* ptr=Char;
	RString Word;
	size_t nb=0;
	while((!Char->IsNull())&&(!Char->IsSpace())&&(chars.Find(*Char)==-1))
	{
		Char++;
		nb++;
	}
	Word.Copy(ptr,nb);
	return(Word);
}


//-----------------------------------------------------------------------------
void RInterpreter::SkipSpaces(void)
{
	while((!Char->IsNull())&&(Char->IsSpace()))
		Char++;
}


//-----------------------------------------------------------------------------
RInterpreter::~RInterpreter(void)
{
}
