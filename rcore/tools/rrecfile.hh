/*

	R Project Library

	RRecFile.hh

	Binary file for records  - Implementation.

	Copyright 2002-2005 by the Université Libre de Bruxelles.

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
//
// RRecFile
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
template<class C,unsigned int S,bool bOrder>
	RRecFile<C,S,bOrder>::RRecFile(const RString &name)
		: RIOFile(name), Find(false), Dirty(true)
{
}


//------------------------------------------------------------------------------
template<class C,unsigned int S,bool bOrder>
	RRecFile<C,S,bOrder>::RRecFile(RIOFile &file)
		: RIOFile(file), Find(false), Dirty(true)
{
}


//------------------------------------------------------------------------------
template<class C,unsigned int S,bool bOrder>
	void RRecFile<C,S,bOrder>::Open(RIO::ModeType mode)
{
	RIOFile::Open(mode);
}


//------------------------------------------------------------------------------
template<class C,unsigned int S,bool bOrder>
	void RRecFile<C,S,bOrder>::Close(void)
{
	Find=false;
	Dirty=true;
	RIOFile::Close();
}


//------------------------------------------------------------------------------
template<class C,unsigned int S,bool bOrder>
	RRecFile<C,S,bOrder>& RRecFile<C,S,bOrder>::operator>>(double& nb) throw(RString)
{
	Read((char*)(&nb),sizeof(double));
	return(*this);
}


//------------------------------------------------------------------------------
template<class C,unsigned int S,bool bOrder>
	RRecFile<C,S,bOrder>& RRecFile<C,S,bOrder>::operator>>(unsigned int& nb) throw(RString)
{
	Read((char*)(&nb),sizeof(unsigned int));
	return(*this);
}


//------------------------------------------------------------------------------
template<class C,unsigned int S,bool bOrder>
	RRecFile<C,S,bOrder>& RRecFile<C,S,bOrder>::operator<<(unsigned char nb) throw(RString)
{
	Write((char*)(&nb),sizeof(unsigned char));
	return(*this);
}


//------------------------------------------------------------------------------
template<class C,unsigned int S,bool bOrder>
	RRecFile<C,S,bOrder>& RRecFile<C,S,bOrder>::operator<<(unsigned int nb) throw(RString)
{
	Write((char*)(&nb),sizeof(unsigned int));
	return(*this);
}

//------------------------------------------------------------------------------
template<class C,unsigned int S,bool bOrder>
	RRecFile<C,S,bOrder>& RRecFile<C,S,bOrder>::operator<<(unsigned long nb) throw(RString)
{
	Write((char*)(&nb),sizeof(unsigned long));
	return(*this);
}


//------------------------------------------------------------------------------
template<class C,unsigned int S,bool bOrder>
	RRecFile<C,S,bOrder>& RRecFile<C,S,bOrder>::operator<<(double nb) throw(RString)
{
	Write((char*)(&nb),sizeof(double));
	return(*this);
}


//------------------------------------------------------------------------------
template<class C,unsigned int S,bool bOrder>
	void RRecFile<C,S,bOrder>::Seek(unsigned int nb) throw(RString)
{
	Find=false;
	RIOFile::Seek(nb*S);
	Dirty=true;
	Find=true;
}


//------------------------------------------------------------------------------
template<class C,unsigned int S,bool bOrder>
	void RRecFile<C,S,bOrder>::SeekMatrix(unsigned int c,unsigned int l,unsigned int maxc) throw(RString)
{
	Find=false;
	Seek(c+(l*maxc));
	Dirty=true;
	Find=true;
}


//------------------------------------------------------------------------------
template<class C,unsigned int S,bool bOrder>
	template<class TUse>
		unsigned int RRecFile<C,S,bOrder>::Search(const TUse& tag)
{
	unsigned int NbMin,NbMax,i=0;
	int Comp=0;
	bool Cont=true,NotLast=true;

	if(bOrder)
	{
		Find=false;
		if(!GetSize())
			return(0);
		NbMax=GetSize()-1;
		NbMin=0;
		if(NbMax)
		{
			while(Cont)
			{
				i=(NbMax+NbMin)/2;
				Seek(i);
				Current.Read(*this);
				Dirty=false;
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
			Seek(i);
			Current.Read(*this);
			Dirty=false;
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
	unsigned int RRecFile<C,S,bOrder>::GetSize(void) const
{
	return(RIOFile::GetSize()/S);
}


//------------------------------------------------------------------------------
template<class C,unsigned int S,bool bOrder>
	unsigned int RRecFile<C,S,bOrder>::GetPos(void) const
{
	return(RIOFile::GetPos()/S);
}


//------------------------------------------------------------------------------
template<class C,unsigned int S,bool bOrder>
	void RRecFile<C,S,bOrder>::Start(void)
{
	Seek(0);
}


//------------------------------------------------------------------------------
template<class C,unsigned int S,bool bOrder>
	void RRecFile<C,S,bOrder>::Next(void)
{
	if(Dirty)
		Seek(GetPos()+1);
	Dirty=true;
}


//------------------------------------------------------------------------------
template<class C,unsigned int S,bool bOrder>
	C* RRecFile<C,S,bOrder>::operator()(void)
{
	if(!Find)
		return(0);
	if(Dirty)
	{
		Current.Read(*this);
		Dirty=false;
	}
	return(&Current);
}


//------------------------------------------------------------------------------
template<class C,unsigned int S,bool bOrder>
	RRecFile<C,S,bOrder>::~RRecFile(void)
{
}
