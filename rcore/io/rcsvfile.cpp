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
	: RTextFile(uri,encoding), Sep(sep), Escape('\\'), Values(20), MaxValues(0)
{
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
		throw RIOException("RCSVFile can only be read");
	Sep=sep;
	RTextFile::Open(uri,mode,encoding);
	NbValues=0;
}


//------------------------------------------------------------------------------
inline RString* RCSVFile::NewValue(void)
{
	RString* Cur;

	// Insert a null string
	// First character of the value
	if(NbValues>=MaxValues)
	{
		Values.InsertPtr(Cur=new RString());
		MaxValues++;
	}
	else
	{
		Cur=Values[NbValues];
		Cur->SetLen(0);
	}

	return(Cur);
}


//------------------------------------------------------------------------------
void RCSVFile::Read(void)
{
	bool ReadField(false); // Suppose a value to read
	RString* Cur;
	RChar Search;

	NbValues=0;  // No values read
	while(!End())
	{
		RChar Car(GetChar());
//		cout<<Car.Latin1()<<endl;

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

		// Look if the end of field is reached ?
		if(Search==Sep)
		{
			if(Eol(Car))
			break;
		}
		else if (Car==Search)
		{
			NbValues++;
			ReadField=false;

			// Read the next character
			Car=GetChar();

			// Skip "normal spaces"
			while(Car.IsSpace()&&(!Eol(Car)))
				Car=GetChar();

			// Treat next non-normal space character
			if(Car==Sep)
			{
				// Skip "normal spaces"
				while(Car.IsSpace()&&(!Eol(Car)))
					Car=GetChar();
			}
			else if(!Eol(Car))
				ThrowRIOException("Separator character '"+Sep+"' expected");

			continue;
		}

		if(Car==Escape)
		{
			// The next character must be added
			Car=GetChar();
		}

		// Add the character
		(*Cur)+=Car;
	}
}


//------------------------------------------------------------------------------
/*void RCSVFile::Read(void)
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
}*/


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
}
