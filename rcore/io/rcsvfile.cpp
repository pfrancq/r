/*

	R Project Library

	RCSVFile.cpp

	CSV File - Implementation.

	Copyright 2009-2011 by Pascal Francq (pascal@francq.info).

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
 // Maximal size of a Buffer.
const size_t MaxBuffer=1024;



//------------------------------------------------------------------------------
//
// class RCSVFile
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
RCSVFile::RCSVFile(const RURI& uri,RChar sep,const RCString& encoding)
	: RTextFile(uri,encoding), Sep(sep), Escape('\\'), Values(20),
	  Internal(0), SizeBuffer(0), Buffer(0)
{
	Internal=new RChar[MaxBuffer+1];
}


//------------------------------------------------------------------------------
void RCSVFile::Open(RIO::ModeType mode)
{
	if(mode!=RIO::Read)
		throw RIOException("RCSVFile can only be read");
	RTextFile::Open(mode);
	NbValues=0;
}


//------------------------------------------------------------------------------
void RCSVFile::Open(const RURI& uri,RChar sep,RIO::ModeType mode,const RCString& encoding)
{
	if(mode!=RIO::Read)
		ThrowRIOException(this,"RCSVFile can only be read");
	Sep=sep;
	RTextFile::Open(uri,mode,encoding);
	NbValues=0;
}


//------------------------------------------------------------------------------
void RCSVFile::Close(void)
{
	RTextFile::Close();
	NbValues=0;
	Values.Clear();
}


//------------------------------------------------------------------------------
inline RString* RCSVFile::NewValue(void)
{
	RString* Cur;

	// Insert a null string
	// First character of the value
	if(NbValues<Values.GetNb())
		ThrowRIOException(this,"Internal Problem");
	Values.InsertPtr(Cur=new RString());
	Cur->SetLen(0);
	if(SizeBuffer)
		ThrowRIOException(this,"Non null buffer size");
	SizeBuffer=0;
	Buffer=Internal;

	return(Cur);
}


//------------------------------------------------------------------------------
void RCSVFile::Read(void)
{
	bool ReadField(false); // Suppose a value to read
	RString* Cur;
	RChar Search;

	Values.Clear();
	NbValues=0;  // No values read
	while(!End())
	{
		RChar Car(GetChar());

		// If no value are read -> make some test
		if(!ReadField)
		{
			if(Eol(Car))
			{
				// Skip the new line
				SkipEol();
				break;
			}
			else if(Car==Sep)
			{
				// Empty field
				NewValue();
				NbValues++;
				continue;
			}
			else if(Car.IsSpace())
				continue;    // Skip spaces

			Cur=NewValue();
			ReadField=true;

			// Value with " ?
			if(Car=='"')
			{
				Search='"';
				continue;
			}
			else
			{
				Search=Sep;
			}
		}

		// If end of a field or of the line -> prepare next value
		if(((Search==Sep)&&(Eol(Car)))||(Car==Search))
		{
			NbValues++;
			ReadField=false;

			if(SizeBuffer)
			{
				(*Buffer)=0;
				(*Cur)+=Internal;
				SizeBuffer=0;
				Buffer=Internal;
			}

			// If the current character is a quote -> Skip all characters until the separation
			if(Car=='\"')
			{
				// Skip spaces
				RChar Next(GetNextChar());
				while((Next==' ')||(Next=='\t'))
					Car=GetChar();

				// Skip the separation if any
				if(GetNextChar()==Sep)
					Car=GetChar();
			}

			continue;
		}

		if(Car==Escape)
		{
			// The next character must be added
			Car=GetChar();
		}

		if(SizeBuffer==MaxBuffer)
		{
			// Verify if the buffer is filled.
			(*Buffer)=0;
			(*Cur)+=Internal;
			SizeBuffer=0;
			Buffer=Internal;
		}

		// Add the character
		SizeBuffer++;
		(*(Buffer++))=Car;
	}
	if(SizeBuffer)
	{
		(*Cur)+=Internal;
		Buffer=Internal;
		SizeBuffer=0;
	}
}



//------------------------------------------------------------------------------
RString RCSVFile::Get(size_t idx) const
{
	if(idx>NbValues)
		throw RIOException(URI()+" ("+RString::Number(GetLineNb()-1)+"): CSV line has not "+RString::Number(idx+1)+" fields");
	RString Field(*Values[idx]);
	if(Field()[0]=='\"') // Verify
		Field=Field.Mid(1,Field.GetLen()-2);
	return(Field);
}


//------------------------------------------------------------------------------
size_t RCSVFile::GetSizeT(size_t idx) const
{
	if(idx>NbValues)
		throw RIOException(URI()+" ("+RString::Number(GetLineNb()-1)+"): CSV line has not "+RString::Number(idx+1)+" fields");
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
	delete[] Internal;
}
