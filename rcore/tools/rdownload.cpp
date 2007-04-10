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
struct DwnFile
{
	FILE* Stream;
	RDownload* Caller;
	
	DwnFile(const char* filename,RDownload* caller)
		: Stream(0), Caller(caller)
	{
		Stream=fopen(filename, "wb");
		if(!Stream)
			throw RException(RString("Cannot create temporary file ")+filename);
	}
	~DwnFile(void)
	{
		if(Stream)
			fclose(Stream);		
	}
	
};


//------------------------------------------------------------------------------
int RDownload::WriteTmpFile(void* buffer, size_t size, size_t nmemb, void* param)
{
	return(fwrite(buffer, size, nmemb, ((struct DwnFile*)param)->Stream));
}


//------------------------------------------------------------------------------
int RDownload::TreatHeader(void* buffer, size_t size, size_t nmemb,void* param)
{
   // only handle single-byte data
    if (size!=1)
        return(0);

    char* buf = (char*)malloc(nmemb + 1);
    if(buf)
    {
        memset(buf,0,nmemb + 1);
        memcpy(buf,buffer,nmemb);
        char* mime=buf;
        while(((*mime)&&((*mime)!=':')))
        	mime++;
        (*mime)=0;
        if(!strcmp(buf,"Content-Type"))
        {
        	mime++;
        	while(isspace(*mime))
        		mime++;
       		char* ptr=mime;
       		while((!isspace(*ptr))&&((*ptr)!=';'))
       			ptr++;
        	(*ptr)=0;
        	((struct DwnFile*)param)->Caller->MIME=mime;
        	if(!((struct DwnFile*)param)->Caller->IsValidContent(((struct DwnFile*)param)->Caller->MIME))
        	{
        		((struct DwnFile*)param)->Caller->ValidContent=false;
        		return(-1);
        	}
        }
        free(buf);
        return nmemb;
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
{
	Lib = curl_easy_init();
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

	// Fill structure
	ValidContent=true;  // Suppose the content is OK
	MIME="";            // No MIME type.
	struct DwnFile Dwn(tmpFile,this);
	
	// Set the option
	curl_easy_setopt(Lib, CURLOPT_URL, URL.Latin1());
	curl_easy_setopt(Lib, CURLOPT_WRITEFUNCTION,RDownload::WriteTmpFile);
	curl_easy_setopt(Lib, CURLOPT_HEADERFUNCTION,RDownload::TreatHeader);
	curl_easy_setopt(Lib, CURLOPT_CONNECTTIMEOUT,30);
	curl_easy_setopt(Lib, CURLOPT_TIMEOUT,240);
	curl_easy_setopt(Lib, CURLOPT_NOPROGRESS, 1);
	curl_easy_setopt(Lib, CURLOPT_WRITEDATA,&Dwn);
	curl_easy_setopt(Lib, CURLOPT_WRITEHEADER,&Dwn);
	curl_easy_setopt(Lib, CURLOPT_USERAGENT, "libcurl-agent/1.0");
 	curl_easy_setopt(Lib, CURLOPT_COOKIEFILE, "");
 	curl_easy_setopt(Lib, CURLOPT_NOSIGNAL, 1);

	// Download the file
	CURLcode err=curl_easy_perform(Lib);
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
RString RDownload::GetMIMEType(void)
{
	return(MIME);
}

//------------------------------------------------------------------------------
RDownload::~RDownload(void)
{
	curl_easy_cleanup(Lib);
}
