/*

	R Project Library

	RGA.cpp

	Genetic Algorithms - Implementation

	(C) 1998-2001 by By P. Francq.

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



//-----------------------------------------------------------------------------
// include files for AINSI C/C++
#include <iostream>


//-----------------------------------------------------------------------------
// include files for R Project
#include <rga/rga.h>


//-----------------------------------------------------------------------------
// General Variables
bool RGA::ExternBreak=false;



//-----------------------------------------------------------------------------
//
// Exceptions
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
RGA::eGA::eGA(void)
	: Msg()
{
}


//-----------------------------------------------------------------------------
RGA::eGA::eGA(const RString& msg)
	: Msg(msg)
{
	cerr<<Msg()<<endl;
}


//-----------------------------------------------------------------------------
RGA::eGA::eGA(const char* msg)
	: Msg(msg)
{
	cerr<<Msg()<<endl;
}


//-----------------------------------------------------------------------------
void RGA::eGA::SetMsg(const char* m)
{
	Msg=m;
	cerr<<Msg()<<endl;
}


//-----------------------------------------------------------------------------
RGA::eGAVerify::eGAVerify(const unsigned int g,const unsigned int c)
	: eGA()
{
	char tmp[200];

	sprintf(tmp,"Generation %u: Verify error for chromosome %u",g,c);
	SetMsg(tmp);
}


//-----------------------------------------------------------------------------
RGA::eGACrossover::eGACrossover(const unsigned int g,const unsigned p1,const unsigned int p2,const unsigned int c)
	: eGA()
{
	char tmp[200];

	sprintf(tmp,"Generation %u: Crossover error: parent %u + parent %u -> Child %u",g,p1,p2,c);
	SetMsg(tmp);
}


//-----------------------------------------------------------------------------
RGA::eGAMutation::eGAMutation(const unsigned int g,const unsigned int c)
	: eGA()
{
	char tmp[200];

	sprintf(tmp,"Generation %u: Mutation error: Chromsosome %u",g,c);
	SetMsg(tmp);
}


//-----------------------------------------------------------------------------
RGA::eGAInversion::eGAInversion(const unsigned int g,const unsigned int c)
	: eGA()
{
	char tmp[200];

	sprintf(tmp,"Generation %u: Inversion error: Chromsosome %u",g,c);
	SetMsg(tmp);
}


//-----------------------------------------------------------------------------
RGA::eGAOptimisation::eGAOptimisation(const unsigned int g,const unsigned int c)
	: eGA()
{
	char tmp[200];

	sprintf(tmp,"Generation %u: Optimisation error: Chromsosome %u",g,c);
	SetMsg(tmp);
}


//-----------------------------------------------------------------------------
RGA::eGARandomConstruct::eGARandomConstruct(const unsigned int g,const unsigned int c)
	: eGA()
{
	char tmp[200];

	sprintf(tmp,"Generation %u: Random Construction error: Chromsosome %u",g,c);
	SetMsg(tmp);
}


//-----------------------------------------------------------------------------
RGA::eGAEvaluation::eGAEvaluation(const unsigned int g,const unsigned int c)
	: eGA()
{
	char tmp[200];

	sprintf(tmp,"Generation %u: Evaluation error: Chromsosome %u",g,c);
	SetMsg(tmp);
}


//-----------------------------------------------------------------------------
RGA::eGAPostEvaluation::eGAPostEvaluation(const unsigned int g,const unsigned int c)
	: eGA()
{
	char tmp[200];

	sprintf(tmp,"Generation %u: Post Evaluation error: Chromsosome %u",g,c);
	SetMsg(tmp);
}



//---------------------------------------------------------------------------
//
// General functions
//
//---------------------------------------------------------------------------

//-----------------------------------------------------------------------------
bool RGA::Examine(int argc, char *argv[],const char* Prj,const char *Ver)
{
	char **args;	
	bool bVersion=false;
	bool bHelp=false;
	bool bMaster=false;
	bool bSlave=false;
//	char MasterIP[20];
//	char HostIP[20];

	// Verify if parameters
	if(argc==1)
	{
		cout<<Prj<<": Not input file\n";
		return(true);
	}

	// Analyse parameters
	for(args=argv+1;--argc;args++)
	{
		if(!strcmp(*args,"--version"))
			bVersion=true;
		else
		if(!strcmp(*args,"--help"))
			bHelp=true;
  }

	// Treats parameters
	if(bVersion)
	{
		cout<<Prj<<" Version "<<Ver<<"  (C) 1999-2000 by Pascal Francq\n";
		return(true);
	}
	if(bHelp)
	{
		cout<<"Usage: "<<Prj<<" [options] file\nOptions:\n";
		cout<<"--help\t\t\tDisplay this information\n";
		cout<<"--version\t\tDisplay the version information\n";
		cout<<"-o <file>\t\tPut output into <file>\n";
		cout<<"-s <ip>\t\t\tSlave mode with master address <ip>\n";
		cout<<"-m <ip>\t\t\tMaster mode\n";
		cout<<"-h <ip>\t\t\tIP address of host (DISPLAY)\n";
		return(true);
	}

	// Treat if Stand-alone
	if((!bSlave)&&(!bMaster))
	{
	}

	// Treat if Master
	if(bMaster)
	{
	}

	// Treat if Slave
	if(bSlave)
	{
	}

	// End
	return(true);
}
