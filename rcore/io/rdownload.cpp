/*

	R Research Project

	RDownload.cpp

	Class to download files - Implementation.

	Copyright 2004-2015 Pascal Francq (pascal@francq.info).
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
// include files for ANSI C/C++
#include <stdio.h>
#include <iostream>
#include <cstdlib>


//------------------------------------------------------------------------------
// include files for CURL
#include <curl/curl.h>


//------------------------------------------------------------------------------
// include files for R library
#include <rdownload.h>
#include <rexception.h>
using namespace R;
using namespace std;



//------------------------------------------------------------------------------
//
// Internal function and structure
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
bool RDownload::MustInitCURL=true;


//------------------------------------------------------------------------------
size_t RDownload::WriteTmpFile(void* buffer,size_t size,size_t nmemb,void* param)
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
	return(((RDownload*)param)->GetData(buffer,size,nmemb));
}


//------------------------------------------------------------------------------
size_t RDownload::TreatHeader(void* buffer,size_t size,size_t nmemb,void* param)
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
        return(static_cast<int>(nmemb));
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
	: ValidContent(true), MIME(30), First(true)
{
	if(MustInitCURL)
	{
		CURLcode err;
		if(err=curl_global_init(CURL_GLOBAL_ALL))
			mThrowRException(RString("Error in CURL: ")+curl_easy_strerror(err));
		MustInitCURL=false;
	}

	// Create link to CURL library and put global options
	Lib = static_cast<void*>(curl_easy_init());
	curl_easy_setopt(static_cast<CURL*>(Lib), CURLOPT_WRITEFUNCTION,RDownload::WriteTmpFile);
	curl_easy_setopt(static_cast<CURL*>(Lib), CURLOPT_HEADERFUNCTION,RDownload::TreatHeader);
	curl_easy_setopt(static_cast<CURL*>(Lib), CURLOPT_CONNECTTIMEOUT,30);
	curl_easy_setopt(static_cast<CURL*>(Lib), CURLOPT_TIMEOUT,240);
	curl_easy_setopt(static_cast<CURL*>(Lib), CURLOPT_NOPROGRESS, 1);
	curl_easy_setopt(static_cast<CURL*>(Lib), CURLOPT_USERAGENT, "libcurl-agent/1.0");
 	curl_easy_setopt(static_cast<CURL*>(Lib), CURLOPT_COOKIEFILE, "");
 	curl_easy_setopt(static_cast<CURL*>(Lib), CURLOPT_NOSIGNAL, 1);
	curl_easy_setopt(static_cast<CURL*>(Lib), CURLOPT_WRITEHEADER,this);
 	curl_easy_setopt(static_cast<CURL*>(Lib), CURLOPT_WRITEDATA,this);
}


//------------------------------------------------------------------------------
size_t RDownload::GetData(void* buffer, size_t size, size_t nmemb)
{
	return(size*nmemb);
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
void RDownload::Download(const RURI& uri)
{
	CURLcode err;

	// Init Part
	ValidContent=true;  // Suppose the content is OK
	MIME="";            // No MIME type.
	First=true;
	URI=uri;

	// Download the file
	if(err=curl_easy_setopt(static_cast<CURL*>(Lib), CURLOPT_URL, uri().ToLatin1()))
		mThrowRException(RString("Error in CURL: ")+curl_easy_strerror(err));

	if(err=curl_easy_perform(static_cast<CURL*>(Lib)))
		mThrowRException(RString("Error in CURL: ")+curl_easy_strerror(err));

	// Done Part
	EndDownload();

	if(!ValidContent)
		mThrowRException("Cannot treat the MIME type '"+MIME+"'");
}


//------------------------------------------------------------------------------
void RDownload::EndDownload(void)
{
}


//------------------------------------------------------------------------------
RString RDownload::GetMIMEType(void)
{
	return(MIME);
}

//------------------------------------------------------------------------------
RDownload::~RDownload(void)
{
	curl_easy_cleanup(static_cast<CURL*>(Lib));
}



//------------------------------------------------------------------------------
//
// class RDownloadFile
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
RDownloadFile::RDownloadFile(void)
	: RDownload(), Stream(0)
{
}


//------------------------------------------------------------------------------
void RDownloadFile::Download(const RURI& uri,const R::RURI& local)
{
	Stream=fopen(local.GetPath(), "wb");
	if(!Stream)
		mThrowRException("Cannot create file '"+local()+"'");
	RDownload::Download(uri);

	// Done Part
	if(Stream)
	{
		fclose(Stream);
		Stream=0;
	}
}


//------------------------------------------------------------------------------
size_t RDownloadFile::GetData(void* buffer, size_t size, size_t nmemb)
{
	return(fwrite(buffer,size,nmemb,Stream));
}
