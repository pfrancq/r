/*

	R Project Library

	RURI.cpp

	Uniform Resource Identifier - Implementation.

	Copyright 2007 by the Université Libre de Bruxelles.

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
#include <ruri.h>
using namespace R;



//-----------------------------------------------------------------------------
//
// class RURI
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
RURI::RURI(void)
	: RString(RString::Null)
{
	// Clear all elements
	Scheme.Clear();
	Authority.Clear();
	Path.Clear();
	Port.Clear();
	Query.Clear();
	Fragment.Clear();
}


//-----------------------------------------------------------------------------
RURI::RURI(const RString& uri)
	: RString(uri)
{
	AnalyzeString();
}


//-----------------------------------------------------------------------------
RURI::RURI(const char* uri)
	: RString(uri)
{
	AnalyzeString();
}


//-----------------------------------------------------------------------------
RURI::RURI(const RURI& uri)
	: RString(uri),	Scheme(uri.Scheme), Authority(uri.Authority), Path(uri.Path),
	  Port(uri.Port), Query(uri.Query), Fragment(uri.Fragment)
{
}


//-----------------------------------------------------------------------------
void RURI::AnalyzeString(void)
{
	size_t i;

	// Clear all elements
	Scheme.Clear();
	Authority.Clear();
	Path.Clear();
	Port.Clear();
	Query.Clear();
	Fragment.Clear();

	// Test if it is a local file
	if((*this)[static_cast<size_t>(0)]=='/')
	{
		// No, it is a path -> add file:
		Scheme.Size=4;
		Path.Pos=5;
		Path.Size=GetLen();
		(*this)="file:"+(*this);
		return;
	}

	// Extract Scheme (search first :)
	for(i=0;(i<GetLen())&&((*this)[i]!=':');i++,Scheme.Size++);
	if(i==GetLen())
		throw RException((*this)+" is not a valid URI");

	// Test if it is a local file
	if(GetScheme()=="file")
	{
		// No, it is a path -> add file:
		Scheme.Size=4;
		Path.Pos=5;
		Path.Size=GetLen()-5;
		return;
	}


	// Skip ":"
	i++;
	if(i==GetLen())
		throw RException((*this)+" is not a valid URI");
	RChar c1=(*this)[i];
	i++;
	if(i==GetLen())
		throw RException((*this)+" is not a valid URI");
	RChar c2=(*this)[i];
	if(i==GetLen())
		throw RException((*this)+" is not a valid URI");
	i++;

	if((c1=='/')&&(c2=='/'))
	{
		// Next characters are '//' -> URL
		// Authority id the part until the next '/'
		for(Authority.Pos=i;((i<GetLen())&&((*this)[i]!='/')&&((*this)[i]!=':'));i++,Authority.Size++);
		if(i==GetLen())
			return;          // URL pointed to home page.

		// Verify if a port is defined
		if((*this)[i]==':')
		{
			i++;
			if(i==GetLen())
				throw RException((*this)+" is not a valid URI");
			for(Port.Pos=i;((i<GetLen())&&((*this)[i]!='/'));i++,Port.Size++);
		}

		// Extract the path
		for(Path.Pos=i;((i<GetLen())&&((*this)[i]!='?'));i++,Path.Size++);
		if(i==GetLen())
			return;

		// Must be a query and/or a fragment
		if((*this)[i]=='?')
		{
			// Test Query
			i++;
			if(i==GetLen())
				throw RException((*this)+" is not a valid URI");
			for(Query.Pos=i;((i<GetLen())&&((*this)[i]!='#'));i++,Query.Size++);
		}
		if((*this)[i]=='#')
		{
			// Test Query
			i++;
			if(i==GetLen())
				throw RException((*this)+" is not a valid URI");
			for(Fragment.Pos=i;i<GetLen();i++,Fragment.Size++);
		}

		return;
	}

	// Normal URI -> rest is the path.
	Path.Pos=i-2;  // Must use car1 and car2
	Path.Size=GetLen()-i+2;
}


//-----------------------------------------------------------------------------
RURI& RURI::operator=(const RURI& src)
{
	RString::operator=(src);
	AnalyzeString();
	return(*this);
}
