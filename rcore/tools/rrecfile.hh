/*

	R Project Library

	RRecFile.hh

	Binary file for records  - Implementation.

	Copyright 2002-2003 by the Université Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).

	Version $Revision$

	Last Modify: $Date$

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
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <sys/stat.h>
#ifdef _BSD_SOURCE
	#include <unistd.h>
#else
	#include <io.h>
#endif
#include <fcntl.h>
#include <string.h>
#include <time.h>



//------------------------------------------------------------------------------
//
// RRecFile
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
template<class C,unsigned int S,bool bOrder>
	RRecFile<C,S,bOrder>::RRecFile(const RString &name,ModeType mode) throw(bad_alloc,RString)
		: Mode(mode), Name(name), NbRecs(0)
{
	int localmode;

	switch(Mode)
	{
		case R::Read:
			localmode=O_RDONLY;
			break;

		case Append:
			localmode=O_WRONLY | O_CREAT | O_APPEND;
			break;

		case Create:
			localmode=O_WRONLY | O_CREAT | O_TRUNC;
			break;

		default:
			throw(RString("No Valid Mode"));
	};
	#ifndef _BSD_SOURCE
		localmode|=O_BINARY;
	#endif
	if(Mode==R::Read)
		handle=open(Name,localmode);
	else
		handle=open(Name,localmode,S_IREAD|S_IWRITE);
	if(Mode!=Create)
	{
		struct stat statbuf;
		fstat(handle, &statbuf);
		NbRecs=statbuf.st_size/S;
	}
	if(handle==-1)
		throw(RString("Can't open file """+Name+""""));
	eof=false;
}


//------------------------------------------------------------------------------
template<class C,unsigned int S,bool bOrder>
	RRecFile<C,S,bOrder>& RRecFile<C,S,bOrder>::operator>>(double& nb) throw(RString)
{
	if(Mode!=R::Read)
		throw(RString("File Mode is not Read"));
	if(eof)
		throw(RString("End of File"));
	eof=!read(handle,&nb,sizeof(double));
	return(*this);
}


//------------------------------------------------------------------------------
template<class C,unsigned int S,bool bOrder>
	RRecFile<C,S,bOrder>& RRecFile<C,S,bOrder>::operator>>(unsigned int& nb) throw(RString)
{
	if(Mode!=R::Read)
		throw(RString("File Mode is not Read"));
	if(eof)
		throw(RString("End of File"));
	eof=!read(handle,&nb,sizeof(unsigned int));
	return(*this);
}


//------------------------------------------------------------------------------
template<class C,unsigned int S,bool bOrder>
	RRecFile<C,S,bOrder>& RRecFile<C,S,bOrder>::operator<<(const unsigned char nb) throw(RString)
{
	if(Mode==R::Read)
		throw(RString("File Mode is Read"));
	write(handle,&nb,sizeof(unsigned char));
	#ifdef windows
		flushall();
	#endif
	return(*this);
}


//------------------------------------------------------------------------------
template<class C,unsigned int S,bool bOrder>
	RRecFile<C,S,bOrder>& RRecFile<C,S,bOrder>::operator<<(const unsigned int nb) throw(RString)
{
	if(Mode==R::Read)
		throw(RString("File Mode is Read"));
	write(handle,&nb,sizeof(unsigned int));
	#ifdef windows
		flushall();
	#endif
	return(*this);
}

//------------------------------------------------------------------------------
template<class C,unsigned int S,bool bOrder>
	RRecFile<C,S,bOrder>& RRecFile<C,S,bOrder>::operator<<(const unsigned long nb) throw(RString)
{
	if(Mode==R::Read)
		throw(RString("File Mode is Read"));
	write(handle,&nb,sizeof(unsigned long));
	#ifdef windows
		flushall();
	#endif
	return(*this);
}


//------------------------------------------------------------------------------
template<class C,unsigned int S,bool bOrder>
	RRecFile<C,S,bOrder>& RRecFile<C,S,bOrder>::operator<<(const double d) throw(RString)
{
	if(Mode==R::Read)
		throw(RString("File Mode is Read"));
	write(handle,&d,sizeof(double));
	#ifdef windows
		flushall();
	#endif
	return(*this);
}


//------------------------------------------------------------------------------
template<class C,unsigned int S,bool bOrder>
	void RRecFile<C,S,bOrder>::Seek(unsigned int nb) throw(RString)
{
	lseek(handle,nb*S,SEEK_SET);
}


//------------------------------------------------------------------------------
template<class C,unsigned int S,bool bOrder>
	void RRecFile<C,S,bOrder>::SeekMatrix(unsigned int c,unsigned int l,unsigned int maxc) throw(RString)
{
	lseek(handle,(c+(l*maxc))*S,SEEK_SET);
}



//------------------------------------------------------------------------------
template<class C,unsigned int S,bool bOrder>
	bool RRecFile<C,S,bOrder>::Read(C& rec) throw(RString)
{
	if(Mode!=R::Read)
		throw(RString("File Mode is not Read"));
	if(eof)
		throw(RString("End of File"));
	try
	{
		rec.Read(*this);
	}
	catch(RString& msg)
	{
		if(eof) return(false);
		throw RString(msg);
	}
	return(true);
}


//------------------------------------------------------------------------------
template<class C,unsigned int S,bool bOrder>
	template<class TUse>
		unsigned int RRecFile<C,S,bOrder>::GetId(const TUse tag,bool &Find)
{
	unsigned int NbMin,NbMax,i=0;
	int Comp=0;
	bool Cont=true,NotLast=true;

	if(bOrder)
	{
		Find=false;
		if(!NbRecs)
			return(0);
		NbMax=NbRecs-1;
		NbMin=0;
		if(NbMax)
		{
			while(Cont)
			{
				i=(NbMax+NbMin)/2;
				lseek(handle,i*S,SEEK_SET);
				Current.Read(*this);
				Comp=Current.Compare(tag);
				if(!Comp)
				{
					Find=true;
					return(i);
				}
				if(Comp>0)
				{
					NbMax = i;
					if(i) NbMax--;
				}
				else
					NbMin = i+1;
				Cont = NotLast;
				if(NbMin>=NbMax) NotLast=false;
			}
		}
		else
		{
			i = 0;
			lseek(handle,i*S,SEEK_SET);
			Current.Read(*this);
			Comp=Current.Compare(tag);
			if(!Comp)
			{
				Find=true;
				return(0);
			}
		}
		if(Comp<0) i++;
		return(i);
	}
}


//------------------------------------------------------------------------------
template<class C,unsigned int S,bool bOrder>
	bool RRecFile<C,S,bOrder>::GetRec(C& Rec) throw(RString)
{
	bool Find;
	unsigned id;

	id=GetId<C&>(Rec,Find);
	if(!Find) return(false);
	Rec=Current;
	return(true);
}


//------------------------------------------------------------------------------
template<class C,unsigned int S,bool bOrder>
	RRecFile<C,S,bOrder>::~RRecFile(void)
{
	if(handle!=-1) close(handle);
}
