/*

	R Project Library

	RURI.cpp

	Uniform Resource Identifier - Implementation.

	Copyright 2007-2009 by Pascal Francq (pascal@francq.info).
	Copyright 2007-2008 by the Université Libre de Bruxelles (ULB).

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
	: URI(RString::Null)
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
	: URI(uri)
{
	AnalyzeString();
}


//-----------------------------------------------------------------------------
RURI::RURI(const char* uri)
	: URI(uri)
{
	AnalyzeString();
}


//-----------------------------------------------------------------------------
RURI::RURI(const RURI& uri)
	: URI(uri.URI),	Scheme(uri.Scheme), Authority(uri.Authority), Path(uri.Path),
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
	if(!URI.GetLen()) return;

	// Test if it is a local file
	if(URI()[0]=='/')
	{
		// No, it is a path -> add file:
		Scheme.Size=4;
		Path.Pos=7;
		Path.Size=URI.GetLen();
		URI="file://"+URI;
		return;
	}

	// Extract Scheme (search first :)
	for(i=0;(i<URI.GetLen())&&(URI[i]!=':');i++,Scheme.Size++) ;
	if(i==URI.GetLen())
		throw RException(URI+" is not a valid URI");

	// Test if it is a local file
	if(GetScheme()=="file")
	{
		// Search if multiple '/' after file:
		if((URI.GetLen()>7)&&(URI()[5]=='/')&&(URI()[6]=='/'))
		{
			Path.Pos=7;
			Path.Size=URI.GetLen()-7;
		}
		else
		{
			Path.Pos=5;
			Path.Size=URI.GetLen()-5;
		}
		Scheme.Size=4;
		return;
	}


	// Skip ":"
	i++;
	if(i==URI.GetLen())
		throw RException(URI+" is not a valid URI");
	RChar c1=URI[i];
	i++;
	if(i==URI.GetLen())
		throw RException(URI+" is not a valid URI");
	RChar c2=URI[i];
	if(i==URI.GetLen())
		throw RException(URI+" is not a valid URI");
	i++;

	if((c1=='/')&&(c2=='/'))
	{
		// Next characters are '//' -> URL
		// Authority id the part until the next '/'
		for(Authority.Pos=i;((i<URI.GetLen())&&(URI[i]!='/')&&(URI[i]!=':'));i++,Authority.Size++) ;
		if(i==URI.GetLen())
			return;          // URL pointed to home page.

		// Verify if a port is defined
		if(URI[i]==':')
		{
			i++;
			if(i==URI.GetLen())
				throw RException(URI+" is not a valid URI");
			for(Port.Pos=i;((i<URI.GetLen())&&(URI[i]!='/'));i++,Port.Size++) ;
		}

		// Extract the path
		for(Path.Pos=i;((i<URI.GetLen())&&(URI[i]!='?'));i++,Path.Size++) ;
		if(i==URI.GetLen())
			return;

		// Must be a query and/or a fragment
		if(URI[i]=='?')
		{
			// Test Query
			i++;
			if(i==URI.GetLen())
				throw RException(URI+" is not a valid URI");
			for(Query.Pos=i;((i<URI.GetLen())&&(URI[i]!='#'));i++,Query.Size++) ;
		}
		if(URI[i]=='#')
		{
			// Test Query
			i++;
			if(i==URI.GetLen())
				throw RException(URI+" is not a valid URI");
			for(Fragment.Pos=i;i<URI.GetLen();i++,Fragment.Size++) ;
		}

		return;
	}

	// Normal URI -> rest is the path.
	Path.Pos=i-2;  // Must use car1 and car2
	Path.Size=URI.GetLen()-i+2;
}


//-----------------------------------------------------------------------------
RURI& RURI::operator=(const RURI& src)
{
	URI=src.URI;
	Scheme=src.Scheme;
	Authority=src.Authority;
	Path=src.Path;
	Port=src.Port;
	Query=src.Query;
	Fragment=src.Fragment;
	return(*this);
}
