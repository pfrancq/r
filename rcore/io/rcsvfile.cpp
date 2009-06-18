/*

	R Project Library

	RCSVFile.cpp

	CSV File - Implementation.

	Copyright 2009 by Pascal Francq (pascal@francq.info).

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
#include <rcsvfile.h>
#include <rcursor.h>
using namespace R;
using namespace std;



//------------------------------------------------------------------------------
//
// class RCSVFile
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
RCSVFile::RCSVFile(const RURI& uri,RChar sep,const RCString& encoding)
	: RTextFile(uri,encoding), Sep(sep), Escape('\\'), Values(20), SizeTmpChar(5000), TmpChar(0)
{
	TmpChar=new RChar[SizeTmpChar];
}


//------------------------------------------------------------------------------
void RCSVFile::Open(RIO::ModeType mode)
{
	if(mode!=RIO::Read)
		throw RIOException("RCSVFile can only be read");
	RTextFile::Open(mode);
}


//------------------------------------------------------------------------------
void RCSVFile::Open(const RURI& uri,RChar sep,RIO::ModeType mode,const RCString& encoding)
{
	if(mode!=RIO::Read)
		throw RIOException("RCSVFile can only be read");
	Sep=sep;
	RTextFile::Open(uri,mode,encoding);
}


//------------------------------------------------------------------------------
void RCSVFile::Read(void)
{
	Values.Clear();
	RString Line(GetLine());
	const RChar* ptr(Line());
	RChar* Tmp(TmpChar);
	size_t pos(0),nb(0);
	RChar Search;
	bool ReadField(true);   // A field is currently read

	// Skip Spaces
	if((!ptr->IsNull())&&(ptr->IsSpace()))
	{
		ptr++;
		pos++;
	}

	// Look if the first character is a "
	if((!ptr->IsNull())&&((*ptr)=='"'))
	{
		Search='"';
		ptr++;
		pos++;
	}
	else
	{
		Search=Sep;
	}

	while(!ptr->IsNull())
	{
		if((*ptr)==Escape)
		{
			// Skip the Escape character and add the following one
			ptr++;
			nb++;
			if(nb>SizeTmpChar)
				throw RIOException(URI()+" ("+RString::Number(GetLineNb()-1)+"): 'Internal buffer overflow");
			(*(Tmp++))=(*(ptr++));
		}
		else if((*ptr)==Search)
		{
			// New field
			Values.InsertPtr(new RString(TmpChar,nb));
			pos+=nb;
			if(Search=='"')
			{
				ptr++;	// Skip "
				pos++;

				// Skip Spaces
				if((!ptr->IsNull())&&(ptr->IsSpace()))
				{
					ptr++;
					pos++;
				}
			};
			ReadField=false;

			// End of line ?
			if(ptr->IsNull())
				break;

			if((*ptr)!=Sep)
				throw RIOException(URI()+" ("+RString::Number(GetLineNb()-1)+"): '"+Sep+"' separation character is excepted and '"+(*ptr)+"' was found");

			// Prepare reading next field
			nb=0;
			Tmp=TmpChar;
			ReadField=true;
			ptr++; // Skip Sep
			pos++;

			// Skip Spaces
			if((!ptr->IsNull())&&(ptr->IsSpace()))
			{
				ptr++;
				pos++;
			}

			if((!ptr->IsNull())&&((*ptr)=='"'))
			{
				Search='"';
				ptr++;
				pos++;
			}
			else
				Search=Sep;
		}
		else
		{
			nb++;
			if(nb>SizeTmpChar)
				throw RIOException(URI()+" ("+RString::Number(GetLineNb()-1)+"): 'Internal buffer overflow");
			(*(Tmp++))=(*(ptr++));
		}
	}

	// One value left ?
	if(ReadField)
		Values.InsertPtr(new RString(TmpChar,nb));
}


//------------------------------------------------------------------------------
RString RCSVFile::Get(size_t idx) const
{
	if(idx>Values.GetMaxPos())
		throw RIOException(URI()+" ("+RString::Number(GetLineNb()-1)+"): CSV line has not "+RString::Number(idx+1)+" fields");
	RString Field(*Values[idx]);
	if(Field()[0]=='\"') // Verify
		Field=Field.Mid(1,Field.GetLen()-2);
	return(Field);
}


//------------------------------------------------------------------------------
size_t RCSVFile::GetSizeT(size_t idx) const
{
	RString Field(*Values[idx]);
	if(Field()[0]=='\"') // Verify
		Field=Field.Mid(1,Field.GetLen()-2);
	bool Ok;
	size_t res(Field.ToSizeT(Ok));
	if(!Ok)
		throw RIOException(URI()+" ("+RString::Number(GetLineNb()-1)+"): '"+Field+"' is not a size_t");
	return(res);
}


//------------------------------------------------------------------------------
RCSVFile::~RCSVFile(void)
{
	delete[] TmpChar;
}
