/*

	R Research Project

	RDownload.h

	Class to download files - Header.

	Copyright 2004-2011 by Pascal Francq (pascal@francq.info).
	Copyright 2004 by Valery Vandaele.
	Copyright 2004-2008 by the Université Libre de Bruxelles (ULB).

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
#include <ruri.h>



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

	/**
	 * Stream that will store the file to download.
	 */
	FILE* Stream;

	/**
	 * Is it the first bloc of data downloaded?
	 */
	bool First;

public:

	/**
	* Construct the downloader.
	*/
	RDownload(void);

private:

	/**
	* Parameter function given to CURLOPT_WRITEFUNCTION and called by CURL when
	* data are downloaded.
	*/
	static int WriteTmpFile(void* buffer, size_t size, size_t nmemb,void* param);

	/**
	* Parameter function given to CURLOPT_HEADERFUNCTION and called by CURL when
	* a part of the header is downloaded.
	 */
	static int TreatHeader(void* buffer, size_t size, size_t nmemb,void* param);

public:

	/**
	* Download a document given by an URI and store it in a local file.
	* @param uri            URI of the document.
	* @param local          Local file where to store.
	*/
	virtual void DownloadFile(const RURI& uri,const R::RURI& local);

	/**
	* Delete a temporary copy of a file created by the manager. This method is
	* only called if a temporary file was really created.
	* @param tmpFile        Temporary file to delete.
	*/
	virtual void DeleteFile(const R::RURI& tmpFile);

protected:

	/**
	 * If the protocol is HTTP and the server returns a content type for the
	 * downloaded file, this function is called to verify if the file should be
	 * really downloaded.
	 * @param MIME           MIME type send by the server.
	 * @return true if the file should be downloaded (default).
	 */
	virtual bool IsValidContent(const R::RString& MIME);

	/**
	 * Method called when the download of file starts.
	 * @return true if the file should really be downloaded (default).
	 */
	virtual bool StartDownload(void);

public:

	/**
	 * Get the current MIME type.
	 */
	RString GetMIMEType(void);

	/**
	* Destruct the downloader.
	*/
	virtual ~RDownload(void);
};


}  //-------- End of namespace R -----------------------------------------------


//------------------------------------------------------------------------------
#endif
