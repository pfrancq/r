/*

	R Research Project

	RDownload.h

	Class to download files (using the CURL library) - Header.

	Copyright 2004 by the Universit�Libre de Bruxelles.

	Authors:
		Valery Vandaele (vavdaele@ulb.ac.be)

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
#ifndef RDownloadH
#define RDownloadH


//------------------------------------------------------------------------------
// include files for ANSI C/C++
#include <stdexcept>


//------------------------------------------------------------------------------
// include files for CURL
#include <curl/curl.h>


//------------------------------------------------------------------------------
// include files for R library
#include <rstd/rstring.h>



//------------------------------------------------------------------------------
namespace R{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* The RDownload class provides a representation for a downloader
* using the CURL library.
* @author Valery Vandaele 
* @short Downloader using the CURL library.
*/
class RDownload
{
	/**
	* Pointer to an instance of the library.
	*/
	CURL* Lib;

public:

	/**
	* Construct the URL manager.
	* @param path            Path to find the plugins.
	* @param dlg             Should the dialog box be loaded.
	*/
	RDownload(void) throw(std::bad_alloc);

protected:

	/**
	* Parameter function gived to curl_easy_setopt
	*/
	static int WriteTmpFile(void* buffer, size_t size, size_t nmemb, void* stream);

public:

	/**
	* Download and store locally a document given by an URL. If the tmpFile contains 
	* an url, the temporary document will be saved using this filename esle a file is 
	* created with a random name
	* @param URL            URL of the document.
	* @param tmpFile        Temporary file created.
	*/
	virtual void Download(const char* URL,R::RString& tmpFile) throw(RException);

	/**
	* Delete a temporary copy of a file created by the manager. This method is
	* only called if a temporary file was really created.
	* @param tmpFile        Temporary file to delete.
	*/
	virtual void Delete(R::RString& tmpFile) throw(RException);

	/**
	* Destructor of the URL manager.
	*/
	virtual ~RDownload(void);
};


}  //-------- End of namespace R -----------------------------------------------


//------------------------------------------------------------------------------
#endif
