/*

	R Project Library

	RInterpreter.cpp

	Interpreter - Implementation.

	Copyright 2008-2009 by Pascal Francq (pascal@francq.info).
	Copyright 2008 by the Université Libre de Bruxelles (ULB).

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
#include <rprginstprint.h>
#include <rprginstsub.h>
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
	: Cout(o), Classes(10,5), Subroutines(20), tmpBlocks(10,5), Scopes(20,10),
	  Parameters(20), Char(0), File()

{
	Classes.InsertPtr(new RPrgStringClass());

	// Push the main block (containing only global variables and classes).
	tmpBlocks.Push(new RPrgInstBlock(this,0));
	Scopes.Push(new RPrgScope());
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
void RInterpreter::Exec(void)
{
	RCursor<RPrgInst> Cur(tmpBlocks()->Insts);
	for(Cur.Start();!Cur.End();Cur.Next())
		Cur()->Run(this,Cout);
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
RString RInterpreter::WhatTreated(void) const
{
	if(File().IsEmpty())
		return(RString::Null);
	return(File()+"("+RString::Number(Line)+"): ");
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
		case '(':
		{
			// Instruction with parameters
			TreatInst(Inst,depth,0);
			break;
		}
		case '.':   // Call to the method of an object
		{
			// Read the methods name
			RString Method(GetToken(RString('(')));
			if((*Char)!='(')
				throw RPrgException(this,"Wrong call to the method '"+Method+"' for object '"+Inst+"'");
			Char++;

			// Create the instruction
			AnalyseParams();
			AddInst(new RPrgInstMethod(this,Inst,Method,Parameters));
			CleanupParams();
			Eol(false);
			break;
		}
		case '=':     // Must be an assignment
		{
			// Get the next token after the = and the first "
			RString Cmd(GetToken(RString('\"')));

			if(Cmd=="new")
			{
				// Read the class name
				RString ClassName(GetToken(RString('(')));
				RPrgClass* Class=Classes.GetPtr(ClassName);
				if(!Class)
					throw RPrgException(this,"Unknown class '"+ClassName+"'");
				Char++; // Skip (

				// Create the instruction
				AnalyseParams();
				AddInst(new RPrgInstNew(this,Inst,Class,Parameters));
				CleanupParams();
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
		{
			// Instructions without parameters
			TreatInst(Inst,depth,Car);
		}
	}
}


//-----------------------------------------------------------------------------
void RInterpreter::TreatInst(const RString& inst,size_t depth,RChar param)
{
	// If is "for"
	if(inst=="for")
	{
		// No "(" parameters
		if(!param)
			throw RPrgException(this,"'for' needs first the name of a parameter");

		// Read name of variable
		RString Var(param+GetWord());

		// Read type of for (in or from)
		RString Cmd(GetWord());
		if(Cmd=="in")
		{
			SkipSpaces();
			if((*Char)!='(')
				throw RPrgException(this,"'for' with 'in' needs a list of parameters between parenthesis");
			Char++; // Skip (
			AnalyseParams();
		}
		else if(Cmd=="from")
		{
			bool ok;

			// The variable takes a list of numbers given by a range and a step
			RString Next(GetWord());
			double start=Next.ToDouble(ok);
			if(!ok)
				throw RPrgException(this,"'"+Next+"' is not valid parameter of 'from'");
			Next=GetWord();
			if(Next!="to")
				throw RPrgException(this,"'to' is excepted and not '"+Next+"'");
			Next=GetWord();
			double end=Next.ToDouble(ok);
			if(!ok)
				throw RPrgException(this,"'"+Next+"' is not valid parameter of 'to'");
			Next=GetWord();
			if(Next!="step")
				throw RPrgException(this,"'step' is excepted and not '"+Next+"'");
			Next=GetWord();
			double step=Next.ToDouble(ok);
			if(!ok)
				throw RPrgException(this,"'"+Next+"' is not valid parameter of 'step'");
			if(step==0.0)
				throw RPrgException(this,"'step' cannot be null");
			double last;
			for(double i=start;i<=end;i+=step)
			{
				last=i;
				Parameters.InsertPtr(new RPrgVarLiteral(RString::Number(i)));
			}
			if(last<end)
				Parameters.InsertPtr(new RPrgVarLiteral(RString::Number(end)));
		}
		else
			throw RPrgException(this,"'"+Cmd+"' is not valid for a 'for' instruction");
		RPrgInstFor* f=new RPrgInstFor(this,depth+1,Var,Parameters);
		CleanupParams();
		AddInst(f);
		tmpBlocks.Push(f);
		Eol(true);
	}
	else if(inst=="delete")
	{
		AnalyseParams();
		if(Parameters.GetNb()!=1)
			throw RPrgException(this,"'delete' takes one parameter only");
		if(!dynamic_cast<RPrgVarRef*>(Parameters[0]))
			throw RPrgException(this,"'delete' takes the name of a variable");
		AddInst(new RPrgInstDelete(this,Parameters[0]->GetName()));
		Parameters.Clear();
		Eol(false);
	}
	else if(inst=="print")
	{
		AnalyseParams();
		if(Parameters.GetNb()!=1)
			throw RPrgException(this,"'print' takes one parameter only");
		AddInst(new RPrgInstPrint(this,Parameters[0]));
		Parameters.Clear();
		Eol(false);
	}
	else if(inst=="sub")
	{
		// No "(" parameters
		if(!param)
			throw RPrgException(this,"'sub' needs first the name of a subroutine");

		// Read name of subroutine
		RString Sub(param+GetToken(RString('(')));
		Char++; // Skip (
		AnalyseParams();
		RPrgInstSub* f=new RPrgInstSub(this,depth+1,Sub,Parameters);
		CleanupParams();
		AddInst(f);
		Subroutines.InsertPtr(f);
		tmpBlocks.Push(f);
		Eol(true);
	}
	else
	{
		AnalyseParams();
		AddInst(new RPrgInstMethod(this,inst,Parameters));
		Parameters.Clear();
		Eol(false);
	}
}


//-----------------------------------------------------------------------------
void RInterpreter::AnalyseParams(void)
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
			Parameters.InsertPtr(new RPrgVarLiteral(ReadLiteral()));
			SomethingToRead=false;
		}
		else
		{
			RString Var(What+GetToken("),"));
			bool ok;
			Var.ToDouble(ok);
			if(ok)
			{
				Parameters.InsertPtr(new RPrgVarLiteral(Var));
			}
			else
			{
				Var.ToULong(ok);
				if(ok)
				{
					Parameters.InsertPtr(new RPrgVarLiteral(Var));
				}
				else
				{
					Var.ToLong(ok);
					if(ok)
					{
						Parameters.InsertPtr(new RPrgVarLiteral(Var));
					}
					else
					{
						bool b=Var.ToBool(ok,true);
						if(ok)
						{
							if(b)
								Parameters.InsertPtr(new RPrgVarLiteral("1"));
							else
								Parameters.InsertPtr(new RPrgVarLiteral("0"));
						}
						else
						{
							Parameters.InsertPtr(new RPrgVarRef(Var));
						}
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
void RInterpreter::CleanupParams(void)
{
	// Empty?
	if(Parameters.GetNb())
	{
		// No -> Delete the parameters and generate an error.
		while(Parameters.GetNb())
			Parameters.DeletePtrAt(0,true,true);
		throw RPrgException(this,"Unused parameters");
	}
}


//-----------------------------------------------------------------------------
RString RInterpreter::ReadLiteral(void)
{
	bool Cont(true);
	RString Val;
	while(Cont)
	{
		RString Part(GetToken(RString('\"')));
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
void RInterpreter::Eol(bool dbl)
{
	bool One(false);

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
			throw RPrgException(this,RString("Only spaces are allowed at the end of a line and not a '")+car+"'");
	}
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
	while(Parameters.GetNb())
		Parameters.DeletePtrAt(0,true,true);
}
