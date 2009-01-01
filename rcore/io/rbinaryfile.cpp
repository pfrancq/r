/*

	R Project Library

	RBinaryFile.hh

	Binary file - Implementation.

	Copyright 2006-2008 by the Université Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).

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
// include files for R Project
#include <rbinaryfile.h>
using namespace R;
using namespace std;



//------------------------------------------------------------------------------
//
// RBinaryFile
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
RBinaryFile::RBinaryFile(void)
	: RIOFile()
{
}


//------------------------------------------------------------------------------
RBinaryFile::RBinaryFile(const RURI &name)
	: RIOFile(name)
{
}


//------------------------------------------------------------------------------
RBinaryFile::RBinaryFile(RIOFile& file)
	: RIOFile(file)
{
}

//------------------------------------------------------------------------------
void RBinaryFile::Open(RIO::ModeType mode)
{
	RIOFile::Open(mode);
}


//------------------------------------------------------------------------------
void RBinaryFile::Open(const RURI& uri,RIO::ModeType mode)
{
	RIOFile::Open(uri,mode);
}


//------------------------------------------------------------------------------
RBinaryFile& RBinaryFile::operator>>(bool& b)
{
	Read((char*)(&b),sizeof(bool));
	return(*this);
}


//------------------------------------------------------------------------------
RBinaryFile& RBinaryFile::operator>>(char* str)
{
	char read;
	do
	{
		Read(&read,sizeof(char));
		(*(str++))=read;
	} while(read);
	return(*this);
}


//------------------------------------------------------------------------------
RBinaryFile& RBinaryFile::operator>>(RString& str)
{
	size_t len,pos=0;
	UChar read;

	Read((char*)(&len),sizeof(size_t));
	str.SetLen(len);
	while(len--)
	{
		Read((char*)(&read),sizeof(UChar));
		str[pos++]=read;
	}
	return(*this);
}


//------------------------------------------------------------------------------
RBinaryFile& RBinaryFile::operator>>(char& nb)
{
	Read((char*)(&nb),sizeof(char));
	return(*this);
}


//------------------------------------------------------------------------------
RBinaryFile& RBinaryFile::operator>>(unsigned char& nb)
{
	Read((char*)(&nb),sizeof(unsigned char));
	return(*this);
}


//------------------------------------------------------------------------------
RBinaryFile& RBinaryFile::operator>>(int& nb)
{
	Read((char*)(&nb),sizeof(int));
	return(*this);
}


//------------------------------------------------------------------------------
RBinaryFile& RBinaryFile::operator>>(unsigned int& nb)
{
	Read((char*)(&nb),sizeof(size_t));
	return(*this);
}


//------------------------------------------------------------------------------
RBinaryFile& RBinaryFile::operator>>(long& nb)
{
	Read((char*)(&nb),sizeof(long));
	return(*this);
}


//------------------------------------------------------------------------------
RBinaryFile& RBinaryFile::operator>>(unsigned long& nb)
{
	Read((char*)(&nb),sizeof(unsigned long));
	return(*this);
}


//------------------------------------------------------------------------------
RBinaryFile& RBinaryFile::operator>>(long long& nb)
{
	Read((char*)(&nb),sizeof(long long));
	return(*this);
}


//------------------------------------------------------------------------------
RBinaryFile& RBinaryFile::operator>>(unsigned long long& nb)
{
	Read((char*)(&nb),sizeof(unsigned long long));
	return(*this);
}


//------------------------------------------------------------------------------
RBinaryFile& RBinaryFile::operator>>(float& nb)
{
	Read((char*)(&nb),sizeof(float));
	return(*this);
}


//------------------------------------------------------------------------------
RBinaryFile& RBinaryFile::operator>>(double& nb)
{
	Read((char*)(&nb),sizeof(double));
	return(*this);
}


//------------------------------------------------------------------------------
RBinaryFile& RBinaryFile::operator<<(bool b)
{
	Write((char*)(&b),sizeof(bool));
	return(*this);
}


//------------------------------------------------------------------------------
RBinaryFile& RBinaryFile::operator<<(char* str)
{
	Write(str,sizeof(bool)*(strlen(str)+1));
	return(*this);
}


//------------------------------------------------------------------------------
RBinaryFile& RBinaryFile::operator<<(RString& str)
{
	size_t len=str.GetLen();
	Write((char*)(&len),sizeof(size_t));
	const RChar* ptr=str();
	Write((char*)(ptr),sizeof(UChar)*len);
	return(*this);
}


//------------------------------------------------------------------------------
RBinaryFile& RBinaryFile::operator<<(char nb)
{
	Write((char*)(&nb),sizeof(char));
	return(*this);
}


//------------------------------------------------------------------------------
RBinaryFile& RBinaryFile::operator<<(unsigned char nb)
{
	Write((char*)(&nb),sizeof(unsigned char));
	return(*this);
}


//------------------------------------------------------------------------------
RBinaryFile& RBinaryFile::operator<<(int nb)
{
	Write((char*)(&nb),sizeof(int));
	return(*this);
}


//------------------------------------------------------------------------------
RBinaryFile& RBinaryFile::operator<<(unsigned int nb)
{
	Write((char*)(&nb),sizeof(size_t));
	return(*this);
}


//------------------------------------------------------------------------------
RBinaryFile& RBinaryFile::operator<<(long nb)
{
	Write((char*)(&nb),sizeof(long));
	return(*this);
}


//------------------------------------------------------------------------------
RBinaryFile& RBinaryFile::operator<<(unsigned long nb)
{
	Write((char*)(&nb),sizeof(unsigned long));
	return(*this);
}


//------------------------------------------------------------------------------
RBinaryFile& RBinaryFile::operator<<(long long nb)
{
	Write((char*)(&nb),sizeof(long long));
	return(*this);
}


//------------------------------------------------------------------------------
RBinaryFile& RBinaryFile::operator<<(unsigned long long nb)
{
	Write((char*)(&nb),sizeof(unsigned long long));
	return(*this);
}


//------------------------------------------------------------------------------
RBinaryFile& RBinaryFile::operator<<(float nb)
{
	Write((char*)(&nb),sizeof(float));
	return(*this);
}


//------------------------------------------------------------------------------
RBinaryFile& RBinaryFile::operator<<(double nb)
{
	Write((char*)(&nb),sizeof(double));
	return(*this);
}


//------------------------------------------------------------------------------
RBinaryFile::~RBinaryFile(void)
{
}
