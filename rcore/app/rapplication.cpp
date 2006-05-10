/*

	R Project Library

	RApplication.cpp

	Generic Application - Implementation.

	Copyright 2006 by Pascal Francq.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).

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



//-----------------------------------------------------------------------------
// include files for R Project
#include <rapplication.h>
using namespace R;



//-----------------------------------------------------------------------------
//
// class RApplication
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
RApplication::RApplication(const RString& name,int argc, char** argv)
	: Name(name), Args(argc)
{
	// Verify parameters
	RAssert(argc);
	RAssert(argv);
	RAssert(name.GetLen());

	// Initialise information
	File=argv[0];
    for(int i=0;i<argc;i++)
		Args.InsertPtr(new RString(argv[i]));
}


//-----------------------------------------------------------------------------
RString RApplication::GetName(void) const
{
	return(Name);
}


//-----------------------------------------------------------------------------
RString RApplication::GetApplicationFile(void) const
{
	return(File);
}


//-----------------------------------------------------------------------------
void RApplication::Init(void)
{
}


//-----------------------------------------------------------------------------
void RApplication::Run(void)
{
}


//-----------------------------------------------------------------------------
RApplication::~RApplication(void)
{
}
