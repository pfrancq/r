/*

	R Research Project

	RDownload.h

	Class to download files - Header.

	Copyright 2004-2007 by the Université Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.Be)
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
#include <rstring.h>



//------------------------------------------------------------------------------
namespace R{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* The RDownload class provides a representation for a downloader.
* @author Valery Vandaele
* @short Downloader.
*/
class RDownload
{
	/**
	* Pointer to an instance of the library.
	*/
	CURL* Lib;

	/**
	 * Valid Content?
	 */
	bool ValidContent;
	
	/**
	 * Current MIME Type;
	 */
	RString MIME;
	
public:

	/**
	* Construct the downloader.
	*/
	RDownload(void);

protected:

	/**
	* Parameter function gived to curl_easy_setopt
	*/
	static int WriteTmpFile(void* buffer, size_t size, size_t nmemb,void* param);

	/**
	 * Treat Header.
	 */
	static int TreatHeader(void* buffer, size_t size, size_t nmemb,void* param);

public:

	/**
	* Download and store locally a document given by an URL. If the tmpFile contains
	* an url, the temporary document will be saved using this filename else a file is
	* created with a random name
	* @param URL            URL of the document.
	* @param tmpFile        Temporary file created.
	*/
	virtual void DownloadFile(const RString& URL,R::RString& tmpFile);

	/**
	* Delete a temporary copy of a file created by the manager. This method is
	* only called if a temporary file was really created.
	* @param tmpFile        Temporary file to delete.
	*/
	virtual void DeleteFile(R::RString& tmpFile);

	/**
	 * If the protocol is HTTP and the server returns a content type for the
	 * downloaded file, this function is called to verify if the file should be
	 * really downloaded.
	 * @param MIME           MIME type send by the server.
	 * @return true if the file should be downloaded (default).
	 */
	virtual bool IsValidContent(const R::RString& MIME);
	
	/**
	 * Get the current MIME type.
	 */
	RString GetMIMEType(void);
	
	/**
	* Destructor of the downloader.
	*/
	virtual ~RDownload(void);
};


}  //-------- End of namespace R -----------------------------------------------


//------------------------------------------------------------------------------
#endif
