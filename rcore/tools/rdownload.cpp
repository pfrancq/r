/*

	R Research Project

	RDownload.cpp

		Class to download files - Implementation.

	Copyright 2004-2007 by the Université Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be)
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
// include files for ANSI C/C++
#include <stdio.h>
#include <iostream>
#include <cstdlib>


//------------------------------------------------------------------------------
// include files for R library
#include <rdownload.h>
using namespace R;
using namespace std;



//------------------------------------------------------------------------------
//
// Internal function and structure
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
int RDownload::WriteTmpFile(void* buffer,size_t size,size_t nmemb,void* param)
{
	if(((RDownload*)param)->First)
	{
		((RDownload*)param)->First=false;
		if(!((RDownload*)param)->StartDownload())
		{
			((RDownload*)param)->ValidContent=false;
        	return(-1);
		}		
	}
	return(fwrite(buffer,size,nmemb,((RDownload*)param)->Stream));
}


//------------------------------------------------------------------------------
int RDownload::TreatHeader(void* buffer,size_t size,size_t nmemb,void* param)
{
	// Only handle single-byte data
	if(size!=1)
		return(0);

	if(buffer&&nmemb)
	{
		if(!strncmp((char*)buffer,"Content-Type",12))
		{
			char* ptr=(char*)buffer+13;
			while((*ptr)&&(isspace(*ptr)))
				ptr++;
			char* mime=ptr;
			int nb=0;
			while((*ptr)&&(!isspace(*ptr))&&((*ptr)!=';'))
			{
				ptr++;
				nb++;
			}
			((RDownload*)param)->MIME.Copy(mime,nb);
			if(!((RDownload*)param)->IsValidContent(((RDownload*)param)->MIME))
			{
				((RDownload*)param)->ValidContent=false;
				return(-1);
			}
		}
        return(nmemb);
    }
    return(0);
}



//------------------------------------------------------------------------------
//
// class RDownload
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
RDownload::RDownload(void)
	: ValidContent(true), MIME(30), Stream(0), First(true)
{
	// Create link to CURL library and put global options
	Lib = curl_easy_init();
	curl_easy_setopt(Lib, CURLOPT_WRITEFUNCTION,RDownload::WriteTmpFile);
	curl_easy_setopt(Lib, CURLOPT_HEADERFUNCTION,RDownload::TreatHeader);
	curl_easy_setopt(Lib, CURLOPT_CONNECTTIMEOUT,30);
	curl_easy_setopt(Lib, CURLOPT_TIMEOUT,240);
	curl_easy_setopt(Lib, CURLOPT_NOPROGRESS, 1);
	curl_easy_setopt(Lib, CURLOPT_USERAGENT, "libcurl-agent/1.0");
 	curl_easy_setopt(Lib, CURLOPT_COOKIEFILE, "");
 	curl_easy_setopt(Lib, CURLOPT_NOSIGNAL, 1);	
	curl_easy_setopt(Lib, CURLOPT_WRITEHEADER,this);
 	curl_easy_setopt(Lib, CURLOPT_WRITEDATA,this);
}


//------------------------------------------------------------------------------
void RDownload::DownloadFile(const RString& URL,RString& tmpFile)
{
	// Find a local temporary file if tmpFile not specified else use the one specified
	if(tmpFile.IsEmpty())
	{
		char filename[L_tmpnam+1];
		(*filename)=0;
		tmpnam(filename);
		tmpFile=filename;
	}

	// Init Part
	ValidContent=true;  // Suppose the content is OK
	MIME="";            // No MIME type.
	First=true;
	Stream=fopen(tmpFile, "wb");
	if(!Stream)
		throw RException("Cannot create temporary file "+tmpFile);
	
	// Download the file
	curl_easy_setopt(Lib, CURLOPT_URL, URL.Latin1());
	CURLcode err=curl_easy_perform(Lib);

	// Done Part	
	if(Stream)
		fclose(Stream);		
	if(err)
	{
		if(!ValidContent)
			throw RException("Cannot treat the MIME type "+MIME);
		throw RException(curl_easy_strerror(err)+RString(" : ")+URL);
	}
}


//------------------------------------------------------------------------------
void RDownload::DeleteFile(RString& tmpFile)
{
	remove(tmpFile);
}


//------------------------------------------------------------------------------
bool RDownload::IsValidContent(const R::RString&)
{
	return(true);
}


//------------------------------------------------------------------------------
bool RDownload::StartDownload(void)
{
	return(true);
}


//------------------------------------------------------------------------------
RString RDownload::GetMIMEType(void)
{
	return(MIME);
}

//------------------------------------------------------------------------------
RDownload::~RDownload(void)
{
	curl_easy_cleanup(Lib);
}
