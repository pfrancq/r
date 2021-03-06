/*

	R Research Project

	RDownload.h

	Class to download files - Header.

	Copyright 2004-2015 by Pascal Francq (pascal@francq.info).
	Copyright 2004 by Valery Vandaele.
	Copyright 2004-2008 by the Universit√© Libre de Bruxelles (ULB).

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
// include files for R library
#include <ruri.h>


//------------------------------------------------------------------------------
namespace R{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* The RDownload class provides a session that will download content retrieved
* from URLs. To store the content in a file, use the RDownloadFile class.
*
* This class is a encapsulation of libCURL.
* @short Downloader.
*/
class RDownload
{
	/**
	* Pointer to an instance of the library.
	*/
	void* Lib;

	/**
	 * Valid Content?
	 */
	bool ValidContent;

	/**
	 * Current MIME Type;
	 */
	RString MIME;

	/**
	 * Is it the first bloc of data downloaded?
	 */
	bool First;

	/**
	 * URI to download.
	 */
	RURI URI;

	/**
	 * Was CURL Initialize ?
	 */
	static bool MustInitCURL;

public:

	/**
	* Construct the downloader.
	*/
	RDownload(void);

private:

	/**
	* Parameter function given to CURLOPT_WRITEFUNCTION and called by CURL when
	* data are downloaded.
	*
	* The total size downloaded is given by size multiplied with nmemb.
	* @param buffer          Pointer to the delivered data
	* @param size            Size of an element downloaded.
	* @param nmemb           Number of elements downloaded.
	*/
	static size_t WriteTmpFile(void* buffer, size_t size, size_t nmemb,void* param);

	/**
	* Parameter function given to CURLOPT_HEADERFUNCTION and called by CURL when
	* a part of the header is downloaded.
	*
	* The total size downloaded is given by size multiplied with nmemb.
	* @param buffer          Pointer to the delivered data
	* @param size            Size of an element downloaded.
	* @param nmemb           Number of elements downloaded.
	*
	*/
	static size_t TreatHeader(void* buffer, size_t size, size_t nmemb,void* param);

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

	/**
	* Call each time a given amount of data is downloaded. By default, it does
	* nothing.
	* @param buffer          Pointer to the delivered data
	* @param size            Size of an element downloaded.
	* @param nmemb           Number of elements downloaded.
   * @return the number of bytes actually taken care of. If it differs from the
	* number of bytes downloaded, the download is stopped.
   */
	virtual size_t GetData(void* buffer, size_t size, size_t nmemb);

	/**
	 * Method called when the dowload finishes. By default, it does nothing
    */
	virtual void EndDownload(void);

public:

	/**
	* Download a document given by an URI.
	* @param uri            URI of the document.
	*/
	void Download(const RURI& uri);

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



//------------------------------------------------------------------------------
/**
* The RDownloadFile class provides a session that will download content
* retrieved from URLs and storeit in a file.
* @short File Downloader.
*/
class RDownloadFile : public RDownload
{
	/**
	 * Stream that will store the file to download.
	 */
	FILE* Stream;

public:

	/**
	* Download a document given by an URI and store it in a local file.
	*/
	RDownloadFile(void);

	/**
	* Download a document given by an URI.
	* @param uri            URI of the document.
	* @param local          Local file where to store.
	*/
	void Download(const RURI& uri,const R::RURI& local);

private:

	/**
	* Call each time a given amount of data is downloaded and store it in the
	* file.
	* @param buffer          Pointer to the delivered data
	* @param size            Size of an element downloaded.
	* @param nmemb           Number of elements downloaded.
   * @return the number of bytes written in a file.
   */
	virtual size_t GetData(void* buffer, size_t size, size_t nmemb);
};


}  //-------- End of namespace R -----------------------------------------------


//------------------------------------------------------------------------------
#endif
