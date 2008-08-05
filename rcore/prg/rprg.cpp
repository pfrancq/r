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
	: FileName(f), Cout(o), Classes(10,5), Blocks(10,5),
	  Prg(FileName), Line(0), Main(0), HasLoad(false)

{
	// Program settings
	Prg.SetRem("#");
	Prg.SetParseSpace(RTextFile::LeaveSpaces);

	// Create the main Blocks
	Main=new RPrgInstBlock(this,0);
	Blocks.Push(Main);
	Classes.InsertPtr(new RPrgStringClass());
}


//-----------------------------------------------------------------------------
void RPrg::Load(void)
{
	// Open the file
	Prg.Open(RIO::Read);
	if(Prg.End())
		return;

	// Treat the file
	while(!Prg.End())
	{
		// Skip Spaces and count tabs
		size_t tabs=Prg.SkipCountSpaces('\t');

		// Verify the correct tabs
		if(tabs==Blocks()->GetTabs()-1)
		{
			// Leaving current block
			Blocks.Pop();
		}
		else if(tabs!=Blocks()->GetTabs())
			RPrgException(this,"Wrong number of tabs");

		// Read next instruction (in Inst)
		Line=Prg.GetLineNb();
		RString Inst=Prg.GetToken(".(=");
		Prg.SkipSpaces();  // Spaces between the instruction are allowed
		char Car=Prg.GetChar().Latin1();
		if(!Car)
			continue;
		//cout<<"("<<Line<<","<<tabs<<","<<Blocks()->GetTabs()<<"): "<<Inst<<endl;
		switch(Car)
		{
			case '(': // Read of an instruction
			{
				// If is "for"
				if(Inst=="for")
				{
					RPrgInstFor* f=new RPrgInstFor(this,tabs+1);
					AddInst(f);
					Blocks.Push(f);
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
				RString Method(Prg.GetToken("("));
				if(Prg.GetChar()!='(')
					throw RPrgException(this,"Wrong call to the method '"+Method+"' for object '"+Inst+"'");

				// Create the instruction
				AddInst(new RPrgInstMethod(this,Inst,Method));
				Eol(false);
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
					Prg.GetChar(); // Skip "
					AddInst(new RPrgInstAssignVar(this,Inst,new RPrgVarLiteral(ReadLiteral())));
				}
				Eol(false);
				break;
			}
			default:
				throw RPrgException(this,"Invalid '"+RString(Car)+"' character after '"+Inst+"'");
		}

		// Reading end of line
/*		RString End=Prg.GetLine().Trim();
		cout<<"*"<<End<<"*"<<endl;
		if(!End.IsEmpty())
			throw RPrgException(this,"Only spaces allowed at the end of the line");*/
	}
	HasLoad=true;
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
void RPrg::Eol(bool dbl)
{
	bool One=false;

	while((!Prg.End())&&(!RTextFile::Eol(Prg.GetNextChar())))
	{
		RChar car=Prg.GetChar();
		if(dbl&&(car==':'))
		{
			if(One)
				throw RPrgException(this,"Only one ':' is accepted");
			One=true;
		}
		else if(!car.IsSpace())
			throw RPrgException(this,"Only spaces are allowed");
	}

	if((!Prg.End())&&(RTextFile::Eol(Prg.GetNextChar())))
		Prg.GetChar();
}


//-----------------------------------------------------------------------------
void RPrg::Exec(void)
{
	if(!HasLoad)
		Load();
	Blocks.Clear();
	Blocks.Push(Main);
	RCursor<RPrgInst> Cur(Main->Insts);
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		Line=Cur()->GetLine();
		Cur()->Run(this,Cout);
	}
}


//-----------------------------------------------------------------------------
void RPrg::AddVar(RPrgVar* var)
{
	if(!var)
		return;
	Blocks()->Vars.InsertPtr(var,true);
}


//-----------------------------------------------------------------------------
void RPrg::DelVar(RPrgVar* var)
{
	if(!var)
		return;
	Blocks()->Vars.DeletePtr(*var);
}


//-----------------------------------------------------------------------------
void RPrg::AddInst(RPrgInst* inst)
{
	if(!inst)
		return;
	Blocks()->AddInst(inst);
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
	return(0);
}


//-----------------------------------------------------------------------------
RString RPrg::GetValue(const RString& var)
{
	RPrgVar* v=Find(var);
	if(!v)
		throw RPrgException(this,"Unknown variable '"+var+"'");
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
	if(Main)
		delete Main;
}
