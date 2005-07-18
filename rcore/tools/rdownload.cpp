/*

	R Research Project

	RDownload.cpp

		Class to download files (using the CURL library) - Implementation.

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
	char filename[L_tmpnam+1];
	FILE *stream;
};


//------------------------------------------------------------------------------
int RDownload::WriteTmpFile(void* buffer, size_t size, size_t nmemb, void* stream)
{
	struct DwnFile* out=(struct DwnFile*)stream;
	if(out && !out->stream)
	{
		/* open file for writing */
		out->stream=fopen(out->filename, "wb");
		if(!out->stream)
		return(-1);
	}
	return(fwrite(buffer, size, nmemb, out->stream)); 
}



//------------------------------------------------------------------------------
//
// class RDownload
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
RDownload::RDownload(void) throw(std::bad_alloc)
{
	Lib = curl_easy_init();
}


//------------------------------------------------------------------------------
void RDownload::Download(const char* URL,RString& tmpFile) throw(RException)
{
	struct DwnFile tmpfile;
	int err;

	// Fill structure -> 
	//find a local temporary file if tmpFile not specified else use the one specified
	if(tmpFile.IsEmpty())
	{
		(*tmpfile.filename)=0;
		tmpnam(tmpfile.filename);
		tmpfile.stream=0;
		tmpFile=tmpfile.filename;
	}
	else 
	{
		tmpfile.stream=0;
		strcpy(tmpfile.filename,tmpFile.Latin1());
	}

	// Download the file
	curl_easy_setopt(Lib, CURLOPT_URL, URL);
	curl_easy_setopt(Lib, CURLOPT_WRITEFUNCTION,RDownload::WriteTmpFile);
	curl_easy_setopt(Lib, CURLOPT_FILE, &tmpfile);
	//curl_easy_setopt(Lib, CURLOPT_CONNECTTIMEOUT,120);
	//curl_easy_setopt(Lib, CURLOPT_TIMEOUT,120);
	
	err=curl_easy_perform(Lib);
	if(tmpfile.stream)
		fclose(tmpfile.stream);
	if(err)
		throw RException(RString("Can't download url : ")+ URL);
}


//------------------------------------------------------------------------------
void RDownload::Delete(RString& tmpFile) throw(RException)
{
	remove(tmpFile);
}


//------------------------------------------------------------------------------
RDownload::~RDownload(void)
{
	curl_easy_cleanup(Lib);
}
