               /*

	R Project Library

	RPrg.cpp

	Generic script program - Implementation.

	Copyright 2002-2003 by the Université Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).

	Version $Revision$

	Last Modify: $Date$

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
RPrg::RPrg(RString f,RPrgOutput* o) throw(bad_alloc)
	: FileName(f), Cout(o), Insts(40), Vars(10,5), Classes(10,5), Prg(FileName)
{
}


//-----------------------------------------------------------------------------
void RPrg::Load(void) throw(bad_alloc,RException)
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
unsigned int RPrg::CountTabs(char* line)
{
	int tabs;
	char* ptr;

	// Count tabs
	ptr=line;
	tabs=0;
	while((*ptr)&&((*ptr)=='\t'))
	{
		tabs++;
		ptr++;
	}
	return(tabs);
}


//-----------------------------------------------------------------------------
RPrgInst* RPrg::AnalyseLine(RTextFile& prg) throw(bad_alloc,RException)
{
	RString l;
	char* ptr;
	char* obj;
	char* line;
	char* name;
	char what;
	char tabs;
	RPrgVar* r;
	char buf[200];
	char* tbuf;

	// Read the line
	if(ReadLine)
	{
		tbuf=prg.GetLine();
		strcpy(tmp,tbuf);
	}
	line=tmp;

	// Skip Spaces and count tabs
	tabs=CountTabs(line);
	ptr=line;
	while((*ptr)&&(isspace(*ptr)))
	{
		ptr++;
	}

	// Read if it is an Object or instruction
	obj=ptr;
	while((*ptr)&&((*ptr)!='.')&&((*ptr)!='=')&&((*ptr)!='('))
		ptr++;
	what=(*ptr);
	(*(ptr++))=0;

	// Look if instruction
	if(what=='(')
	{
		// If is "for"
		if(!strcmp(obj,"for"))
		{
			RPrgInstFor* f=new RPrgInstFor(ptr,tabs);

			// Read the next lines
			tbuf=Prg.GetLine();
			if(!tbuf) return(f);
			strcpy(tmp,tbuf);
			ReadLine=false;
			while((!ReadLine)||(!Prg.Eof()&&(ReadLine)))
			{
				if(CountTabs(tmp)<=f->GetTabs())
					break;
				f->AddInst(AnalyseLine(prg));
				if(ReadLine)
				{
					tbuf=prg.GetLine();
					if(!tbuf) return(f);
					strcpy(tmp,tbuf);
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
		RPrgClass* c=Classes.GetPtr<const char*>(obj);
		if(!c)
			throw RException(RString("Object \"")+obj+"\" unknown");

		// Read the methods name
		name=ptr;
		while((*ptr)&&((*ptr)!='('))
			ptr++;
		(*(ptr++))=0;
		RPrgFunc* method=c->GetMethod(name);
		if(!method)
		{
			sprintf(buf,"Method \"%s\" unknown for object \"%s\"",name,obj);
			throw RException(buf);
		}

		// Create the instruction
		RPrgInstMethod* inst=new RPrgInstMethod(method);
		while((*ptr))
		{
			r=RPrg::AnalyseParam(ptr);
			if(r)
				inst->AddParam(r);
		}
		ReadLine=true;
		return(inst);
	}

	ReadLine=true;
	return(0);
}


//-----------------------------------------------------------------------------
RPrgVar* RPrg::AnalyseParam(char* &param) throw(bad_alloc,RException)
{
	char* ptr;

	// Skip before something
	while((*param)&&((*param)!='"')&&(!isalpha(*param)))
		param++;
	if(!(*param))
		return(0);

	if((*param)=='"')
	{
		// Skip "
		param++;

		// Read Value and skip " and next thing
		ptr=param;
		while((*param)&&((*param)!='"'))
			param++;
		(*(param++))=0;
		param++;

		// Value
		return(new RPrgVarConst(ptr));
	}
	else
	{
		// Look until , or )
		ptr=param;
		while((*param)&&((*param)!=',')&&((*param)!=')'))
			param++;
		(*(param++))=0;

		// Ref
		return(new RPrgVarRef(ptr));
	}
}


//-----------------------------------------------------------------------------
void RPrg::Exec(void) throw(RException)
{
	for(Insts.Start();!Insts.End();Insts.Next())
		Insts()->Run(this,Cout);
}


//-----------------------------------------------------------------------------
void RPrg::AddVar(RPrgVar* var) throw(bad_alloc,RException)
{
	Vars.InsertPtr(var);
}


//-----------------------------------------------------------------------------
void RPrg::DelVar(RPrgVar* var) throw(bad_alloc,RException)
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
