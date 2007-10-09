/*

	R Project Library

	RURI.h

	Uniform Resource Identifier - Header.

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
#ifndef RURI_H
#define RURI_H


//-----------------------------------------------------------------------------
// include files for R Project
#include <rstring.h>


//-----------------------------------------------------------------------------
namespace R{
//-----------------------------------------------------------------------------


/**
 * This class implements a basic support of URI (Uniform Resource Identifier),
 * in particular for the schemes involving files.
 * 
 * Here are two examples explaining how it is handle by the RURI class:
 * @code
 *   foo://example.com:8042/over/there?name=ferret#nose
 *   \ /   \______________/\_________/ \_________/ \__/
 *    |           |             |           |        |
 * scheme     authority        path       query   fragment
 *    |   ______________________|_
 *   / \ /                        \
 *   urn:example:animal:ferret:nose
 * @endcode
 * @remarks The whole specification is not supported.
 */
class RURI : public RString
{
	/**
	 * This structure represent a part of a string.
	 */
	struct PartString
	{
		size_t Pos;
		size_t Size;
		
		inline PartString(void) : Pos(0), Size(0) {}
		inline PartString(const PartString& part) : Pos(part.Pos), Size(part.Size) {}
		inline void Clear(void) {Pos=0; Size=0;}
	};
	
	/**
	 * Structure identifying the schema of the URI.
	 */
	PartString Scheme;
	
	/**
	 * Structure identifying the authority of the URI.
	 */
	PartString Authority;
	
	/**
	 * Structure identifying the path of the URI
	 */
	PartString Path;

	/**
	 * Structure identifying the port of the URI
	 */
	PartString Port;

	/**
	 * Structure identifying the query of the URI.
	 */
	PartString Query;
	
	/**
	 * Structure identifying the fragment of the URI.
	 */
	PartString Fragment;
	
public:
	
	/**
	* Undefined uniform resource identifier.
	*/
	static const RURI Null;

private:
	
	/**
	 * Default constructor.
	 */
	RURI(void);
	
public:
	
	/**
	 * Constructor of uniform resource identifier.
	 * @param uri            String representing the URI.
	 */
	RURI(const RString& uri);

	/**
	 * Constructor of uniform resource identifier.
	 * @param uri            String representing the URI.
	 */
	RURI(const char* uri);

	/**
	 * Copy constructor.
	 * @param uri            Uniform resource identifier.
	 */
	RURI(const RURI& uri);
	
private:
	
	/**
	 * Analyze the current URI.
	 */
	void AnalyzeString(void);
	
	/**
	 * Extract a given part of the URI based on a structure.
	 */
	inline RString Extract(const PartString& part) const
	{
		if(part.Size)
			return(Mid(part.Pos,part.Size));
		return(RString::Null);
	}

public:
	
	/**
	 * Compare method used by R::RContainer.
	 * @param uri            URI used for the comparisons.
	 */
	int Compare(const RURI& uri) const {return(RString::Compare(uri));}
	
	/**
	 * Compare method used by R::RContainer.
	 * @param uri            URI used for the comparisons.
	 */
	int Compare(const RString& uri) const {return(RString::Compare(uri));}
	
	/**
	 * Compare method used by R::RContainer.
	 * @param uri            URI used for the comparisons.
	 */
	int Compare(const char* uri) const {return(RString::Compare(uri));}

	/**
	 * Get the Scheme part of the uniform resource identifier.
	 */
	inline RString GetScheme(void) const {return(Extract(Scheme));}
	
	/**
	 * Get the authority part of the uniform resource identifier.
	 */
	inline RString GetAuthority(void) const {return(Extract(Authority));}
	
	/**
	 * Get the path part of the uniform resource identifier.
	 */
	inline RString GetPath(void) const {return(Extract(Path));}
	
	/**
	 * Get the port part of the uniform resource identifier.
	 */
	inline RString GetPort(void) const {return(Extract(Port));}
	
	/**
	 * Get the query part of the uniform resource identifier.
	 */
	inline RString GetQuery(void) const {return(Extract(Query));}
	
	/**
	 * Get the fragment part of the uniform resource identifier.
	 */
	inline RString GetFragment(void) const {return(Extract(Fragment));}	
};


}  //-------- End of namespace R ----------------------------------------------


//-----------------------------------------------------------------------------
#endif
