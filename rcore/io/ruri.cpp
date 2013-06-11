/*

	R Project Library

	RURI.cpp

	Uniform Resource Identifier - Implementation.

	Copyright 2007-2012 by Pascal Francq (pascal@francq.info).
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
#include <rexception.h>
#include <rdir.h>
#include <rfile.h>
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
	char FirstCar(URI.ToLatin1()[0]);
	if((FirstCar=='/')||((RFile::GetDirSeparator()=='/')&&(FirstCar=='~')))
	{
		// No, it is a path under Unix scheme -> add file:
		Scheme.Size=4;
		Path.Pos=5;
		Path.Size=URI.GetLen();
		URI="file:"+URI;
		ReplaceChars();
		return;
	}
	else
	{
		// Test if have a windows scheme
		int pos(URI.Find(':'));
		if(pos!=-1)
		{
			// Look if next character a '\'
			if((pos+1<URI.GetLen())&&(URI[pos+1]=='\\'))
			{
				// No, it is a path under Unix scheme -> add file:
				Scheme.Size=4;
				Path.Pos=8;
				Path.Size=URI.GetLen();
				URI="file:///"+URI;
				ReplaceChars();
				return;
			}
		}
	}

	// Extract Scheme (search first :)
	for(i=0;(i<URI.GetLen())&&(URI[i]!=':');i++,Scheme.Size++) ;
	if(i==URI.GetLen())
		mThrowRException(URI+" is not a valid URI");

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
		ReplaceChars();
		return;
	}


	// Skip ":"
	i++;
	if(i==URI.GetLen())
		mThrowRException(URI+" is not a valid URI");
	RChar c1=URI[i];
	i++;
	if(i==URI.GetLen())
		mThrowRException(URI+" is not a valid URI");
	RChar c2=URI[i];
	i++;
	if(i==URI.GetLen())
		mThrowRException(URI+" is not a valid URI");

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
				mThrowRException(URI+" is not a valid URI");
			for(Port.Pos=i;((i<URI.GetLen())&&(URI[i]!='/'));i++,Port.Size++) ;
			if(i==URI.GetLen())
				return;
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
				return; // May be the last character of a valid URI.
			for(Query.Pos=i;((i<URI.GetLen())&&(URI[i]!='#'));i++,Query.Size++) ;
			if(i==URI.GetLen())
				return;
		}
		if(URI[i]=='#')
		{
			// Test Query
			i++;
			if(i==URI.GetLen())
				mThrowRException(URI+" is not a valid URI");
			for(Fragment.Pos=i;i<URI.GetLen();i++,Fragment.Size++) ;
			if(i==URI.GetLen())
				mThrowRException(URI+" is not a valid URI");
		}

		return;
	}

	// Normal URI -> rest is the path.
	Path.Pos=i-2;  // Must use car1 and car2
	Path.Size=URI.GetLen()-i+2;
}


//-----------------------------------------------------------------------------
void RURI::ReplaceChars(bool keepspaces)
{
	if((RDir::GetDirSeparator()=='/')&&keepspaces)
		return;
	size_t pos(Path.Pos);
	for(size_t i=Path.Size+1;--i;)
	{
		RChar Car(URI[pos]);
		if(Car=='\\')
		{
			URI[pos]='/';
			pos++;
		}
		else if(Car==' ')
		{
			if(keepspaces)
				pos++;
			else
			{
				// Replace it by '%20'
				URI=URI.Mid(0,pos)+"%20"+URI.Mid(pos+1);
				Path.Size+=2;
				pos+=3;
			}
		}
		else
			pos++;
	}
}


//-----------------------------------------------------------------------------
RString RURI::GetExtension(void) const
{
	RString path(GetPath());
	int pos(path.Find('.',-1));
	if(pos==-1)
		return(RString::Null);
	return(path.Mid(pos+1));
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
