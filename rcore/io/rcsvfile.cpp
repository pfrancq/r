/*

	R Project Library

	RCSVFile.cpp

	CSV File - Implementation.

	Copyright 2009-2014 by Pascal Francq (pascal@francq.info).

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
RCSVFile::RCSVFile(const RURI& uri,RChar sep,bool quotes,const RCString& encoding)
	: RTextFile(uri,encoding), Sep(sep), Quotes(quotes), Escape('\\'), Values(20),
	  Internal(0), SizeBuffer(0), Buffer(0), Debug(false)
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
void RCSVFile::Open(const RURI& uri,RChar sep,bool quotes,RIO::ModeType mode,const RCString& encoding)
{
	if(mode!=RIO::Read)
		mThrowRIOException(this,"RCSVFile can only be read");
	Sep=sep;
	Quotes=quotes;
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
unsigned long RCSVFile::GetLineNb(void) const
{
	unsigned int Nb(RTextFile::GetLineNb());
	if(Nb&&(!End()))
		return(Nb-1);
	return(Nb);
}


//------------------------------------------------------------------------------
void RCSVFile::SetPrintDebug(bool debug)
{
	Debug=debug;
}


//------------------------------------------------------------------------------
RString RCSVFile::CSVValue(const RString val)
{
	RString ret;
	const RChar* ptr;

	ret.SetLen(val.GetLen()+10);
	ret.SetLen(0);
	ret+="\"";
	for(ptr=val();!ptr->IsNull();ptr++)
	{
		if((*ptr)==RChar('\"'))
			ret+='\"';
		ret+=(*ptr);
	}
	ret+=RChar('\"');
	return(ret);
}


//------------------------------------------------------------------------------
inline void RCSVFile::NewValue(void)
{
	// Insert a null string
	// First character of the value
	if(NbValues<Values.GetNb())
		mThrowRIOException(this,"Internal Problem");
	Values.InsertPtr(CurValue=new RString());
	CurValue->SetLen(0);
	NbValues++;
	if(SizeBuffer)
		mThrowRIOException(this,"Non null buffer size");
	SizeBuffer=0;
	Buffer=Internal;
	if(Debug)
	{
		if(NbValues>1)
			cout<<"*";
		cout<<endl<<"   *";
		cout.flush();
	}
}


//------------------------------------------------------------------------------
void RCSVFile::AddBuffer(void)
{
	(*Buffer)=0;
	(*CurValue)+=Internal;
	SizeBuffer=0;
	Buffer=Internal;
}


//------------------------------------------------------------------------------
void RCSVFile::AddToBuffer(void)
{
	if(SizeBuffer==MaxBuffer)
		AddBuffer();

	// Add the character
	SizeBuffer++;
	(*(Buffer++))=CurChar;
}


//------------------------------------------------------------------------------
void RCSVFile::NextChar(void)
{
	CurChar=GetChar();
	if(Debug)
	{
		cout<<CurChar.Latin1();
		cout.flush();
	}
	CurCol++;
}


//------------------------------------------------------------------------------
bool RCSVFile::IsEol(void)
{
	// Skip the spaces (excepted an end of line or the separation character)
	while((!Eol(CurChar))&&(CurChar!=Sep)&&CurChar.IsSpace())
		NextChar();

	// If we found an end of line -> Skip the new line
	if(Eol(CurChar))
	{
		SkipEol();
		if(!NbValues)    // It is an empty line -> Continue to read
			return(false);
		return(true);   // End of the line
	}

	// A new value starts
	NewValue();
	ReadValue=true;   // Read the value

	// Look if the value will ends with a quotes or the separation character
	if(Quotes&&(GetNextChar()=='"'))
	{
		EndValueChar='"';
		NextChar(); // Skip the '"'
	}
	else
	{
		EndValueChar=Sep;
	}

	return(false);
}


//------------------------------------------------------------------------------
void RCSVFile::Read(void)
{
	// Init
	CurValue=0;
	CurCol=0;
	Values.Clear();
	NbValues=0;
	ReadValue=false;
	CurChar=0;

	if(End())
		mThrowRIOException(this,"End of file reached");
	if(Debug)
	{
		cout<<"---------> Start new line";
		cout.flush();
	}

	// Read the values
	while(!End())
	{
		if(ReadValue)
		{
			// Read the next character
			NextChar();

			if(Quotes&&(EndValueChar=='"')&&(CurChar=='"')&&((!End())&&(GetNextChar()=='"')))
			{
				// Valid double quote
				AddToBuffer();  // Add the character "
				NextChar();     // Skip the second Quote
			}
			else if(Eol(CurChar)||(CurChar==EndValueChar))
			{
				// End of a value is reached

				// Problem if we are at the end of the line and a quote is searched to finish the current value
				if(Eol(CurChar)&&(EndValueChar=='"'))
					throw RIOException(URI()+" ("+RString::Number(GetLineNb())+","+RString::Number(CurCol)+"): CSV line doesn't finish with a quote");

				// End the reading of the current value and add the buffer to it
				ReadValue=false;
				if(SizeBuffer)
					AddBuffer();

				// If quotes are delimiting a field and the current character is a quote -> skip it
				if(Quotes&&CurChar=='\"')
					NextChar();

				// Skip all space characters until the separation
				while((!CurChar.IsNull())&&(!Eol(CurChar))&&(CurChar!=Sep))
				{
					if(!CurChar.IsSpace())
						throw RIOException(URI()+" ("+RString::Number(GetLineNb())+","+RString::Number(CurCol)+"): invalid character "+CurChar);
					NextChar();
				}

				// If we are at the end of a line -> finish the reading
				if(Eol(CurChar)||(CurChar.IsNull()))
					break;
			}
			else if(CurChar==Escape)
			{
				// The next character must be added
				if(!End())
				{
					NextChar();
					if(Eol(CurChar))
						break;
				}
				AddToBuffer();
			}
			else
				AddToBuffer();
		}
		else
		{
			if(IsEol())
				break;
		}
	}

	// If something is still in the buffer when the eof is reached -> Add it
	if(SizeBuffer)
		AddBuffer();

	if(CurChar==Sep)
		IsEol();

	SkipEol();
}


//------------------------------------------------------------------------------
//void RCSVFile::Read(void)
//{
//	bool MustCreate(false);  // Suppose a value must be created
//	RChar Last(0);           // Last character treated
//
//	// Init
//	CurValue=0;
//	CurCol=0;
//	Values.Clear();
//	NbValues=0;  // No values read
//	ReadValue=false;
//
//	// Read the value
//	for(bool First=true;!End();First=false)
//	{
//		NextChar();
//
//		// If no value are read -> make some test
///*		if(!ReadField)
//		{
//			// Skip the new line
//			if(Eol(CurChar))
//			{
//				SkipEol();
//				if(First)    // It is an empty line.
//					continue;
//				break;       // End of the line
//			}
//
//			  // Skip spaces other than EOL
//			if(CurChar.IsSpace())
//				continue;
//
//			// Skip separation
//			if(CurChar==Sep)
//			{
//				if(Last==Sep)
//				{
//					// First empty field
//					NewValue();
//					ReadField=true;
//					NbValues++;
//				}
//				continue;
//			}
//
//			NewValue();
//			ReadField=true;
//			NbValues++;
//			MustCreate=false; // A value was created
//
//			// Value with " ?
//			if(Quotes&&(CurChar=='"'))
//			{
//				Search='"';
//				continue;
//			}
//			else
//			{
//				Search=Sep;
//			}
//		}*/
//
//		if(ReadValue)
//		{
//			// Look if we are at the end of a value:
//			if(Eol(CurChar)||(CurChar==EndValueChar))
//			//if(((EndValueChar==Sep)&&(Eol(CurChar)))||(CurChar==EndValueChar))
//			{
//				// Problem if we are at the end of the line and a quote finishes a value
//				if(Eol(CurChar)&&(EndValueChar=='"'))
//					throw RIOException(URI()+" ("+RString::Number(GetLineNb()-1)+","+RString::Number(CurCol)+"): CSV line doesn't finish with a quote");
//
//				// End the reading of the current value and add the buffer
//				ReadValue=false;
//				if(SizeBuffer)
//					AddBuffer();
//
//				// If the current character is a quote -> skip it
//				if(CurChar=='\"')
//					NextChar();
//
//
//				// Skip all space characters until the separation
//				while((!Eol(CurChar))&&(CurChar!=Sep))
//				{
//					if(!CurChar.IsSpace())
//						throw RIOException(URI()+" ("+RString::Number(GetLineNb()-1)+","+RString::Number(CurCol)+"): only spaces characters allowed");
//					NextChar();
//				}
//
//				// If the character is a separation -> Skip it
///*				if(CurChar==Sep)
//					NextChar();*/
//
///*				if(CurChar=='\"')
//				{
//					// Skip spaces
//					RChar Next(GetNextChar());
//					while((!Eol(Next))&&(Next.IsSpace()))
//					{
//						NextChar();
//						if(End())
//							break;
//						Next=GetNextChar();
//					}
//
//					// Skip the separation if any
//		/*			if((!End())&&(GetNextChar()==Sep))
//					{
//						NextChar();
//					}
//				}*/
//
//	/*			if(CurChar==Sep)
//				{
//					MustCreate=true;
//					NextChar();
//				}
//
//				// If end of line or end of file -> break
//				if(End())
//					break;
//				else if(Eol(CurChar))
//				{
//					SkipEol();
//					break;
//				}*/
//
//				//continue;
//			}
//
//			// Look if valid double quote
//			if(Quotes&&(EndValueChar=='"')&&(CurChar=='"')&&((!End())&&(GetNextChar()=='"')))
//			{
//
//				if(SizeBuffer==MaxBuffer)
//					AddBuffer();
//
//				// Add the character "
//				SizeBuffer++;
//				(*(Buffer++))=CurChar;
//				NextChar(); // Second Quote
//				NextChar(); // Next character
//			}
//
//
//			if(CurChar==Escape)
//			{
//				// The next character must be added
//				NextChar();
//			}
//
//			if(SizeBuffer==MaxBuffer)
//				AddBuffer();
//
//			// Add the character
//			SizeBuffer++;
//			(*(Buffer++))=CurChar;
//		}
//		else
//		{
//			if(IsEol())
//				break;
//		}
//	}
//
//	// If something is still in the buffer when the eof is reached -> Add it
//	if(SizeBuffer)
//		AddBuffer();
//
//	if(MustCreate)
//	{
//		// The last character read was the separation one -> Empty field
//		NewValue();
//		NbValues++;
//	}
//}



//------------------------------------------------------------------------------
RString RCSVFile::Get(size_t idx) const
{
	if(idx>=NbValues)
		throw RIOException(URI()+" ("+RString::Number(GetLineNb())+"): CSV line has not "+RString::Number(idx+1)+" fields");
	RString Field(*Values[idx]);
	return(Field);
}


//------------------------------------------------------------------------------
size_t RCSVFile::GetSizeT(size_t idx,bool zero) const
{
	if(idx>=NbValues)
		throw RIOException(URI()+" ("+RString::Number(GetLineNb())+"): CSV line has not "+RString::Number(idx+1)+" fields");
	RString Field(*Values[idx]);
	if(zero&&Field.IsEmpty())
		return(0);
	bool Ok;
	size_t res(Field.ToSizeT(Ok));
	if(!Ok)
		throw RIOException(URI()+" ("+RString::Number(GetLineNb())+"): Column "+RString::Number(idx)+": '"+Field+"' is not a size_t");
	return(res);
}


//------------------------------------------------------------------------------
double RCSVFile::GetDouble(size_t idx,bool zero) const
{
	if(idx>=NbValues)
		throw RIOException(URI()+" ("+RString::Number(GetLineNb())+"): CSV line has not "+RString::Number(idx+1)+" fields");
	RString Field(*Values[idx]);
	if(zero&&Field.IsEmpty())
		return(0.0);
	bool Ok;
	double res(Field.ToDouble(Ok));
	if(!Ok)
		throw RIOException(URI()+" ("+RString::Number(GetLineNb())+"): Column "+RString::Number(idx)+": '"+Field+"' is not a size_t");
	return(res);
}


//------------------------------------------------------------------------------
RCSVFile::~RCSVFile(void)
{
	delete[] Internal;
}
