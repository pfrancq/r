/*

	Rainbow Library Project

  RGA.cpp

  Genetic Algorithms - Implementation

  (C) 1998-2000 by By P. Francq.

  Version $Revision$

  Last Modify: $Date$

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

	As a special exception to the GNU General Public License, permission is
	granted for additional uses of the text contained in its release
	of the Rainbow Library.

	The exception is that, if you link the Rainbow with other files
	to produce an executable, this does not by itself cause the
	resulting executable to be covered by the GNU General Public License.
	Your use of that executable is in no way restricted on account of
	linking the Rainbow library code into it.

	This exception does not however invalidate any other reasons why
	the executable file might be covered by the GNU General Public License.

	This exception applies only to the code released under the
	name Rainbow.  If you copy code from other releases into a copy of
	RAinbow, as the General Public License permits, the exception does
	not apply to the code that you add in this way.  To avoid misleading
	anyone as to the status of such modified files, you must delete
	this exception notice from them.

	If you write modifications of your own for Rainbow, it is your choice
	whether to permit this exception to apply to your modifications.
	If you do not wish that, delete this exception notice.

*/



//---------------------------------------------------------------------------
// include files for AINSI C/C++
#include <iostream.h>


//---------------------------------------------------------------------------
// include files for Rainbow
#include "rga.h"
using namespace RGA;


//---------------------------------------------------------------------------
// General Variables
bool RGA::ExternBreak=false;
//cInstance *rrlGA::Instance=NULL;
unsigned int RGA::MaxBestPopAge=5;
unsigned int RGA::MaxBestAge=10;
RDebug *RGA::Debug=NULL;
typeSigFunc *RGA::SigFunc=NULL;


//---------------------------------------------------------------------------
// Local Variables for this module
unsigned int RGA::NbCross;
unsigned int RGA::AgeNextMutation;
unsigned int RGA::AgeNextBestMutation;
unsigned int RGA::AgeBestPop;


//---------------------------------------------------------------------------
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