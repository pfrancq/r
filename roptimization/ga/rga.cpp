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
#include <iostream.h>


//-----------------------------------------------------------------------------
// include files for R Project
#include <rga/rga.h>
using namespace RGA;


//-----------------------------------------------------------------------------
// General Variables
bool RGA::ExternBreak=false;
RDebug *RGA::Debug=0;



//-----------------------------------------------------------------------------
//
// Exceptions
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
RGA::eGA::eGA(const RString &msg)
	: Msg(msg)
{
	cerr<<Msg()<<endl;
}


//-----------------------------------------------------------------------------
RGA::eGAVerify::eGAVerify(void)
	: eGA("Error when verify chromosomes")
{
}


//-----------------------------------------------------------------------------
RGA::eGACrossover::eGACrossover(void)
	: eGA("Error when doing a crossover")
{
}


//-----------------------------------------------------------------------------
RGA::eGARandomConstruct::eGARandomConstruct(void)
	: eGA("Error when doing random construct")
{
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
