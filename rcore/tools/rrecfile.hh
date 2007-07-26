/*

	R Project Library

	RRecFile.hh

	Binary file for records - Implementation.

	Copyright 2002-2007 by the Université Libre de Bruxelles.

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
template<class C,bool bOrder>
	RRecFile<C,bOrder>::RRecFile(const RURI& uri,size_t recsize)
		: RBinaryFile(uri), Find(false), Dirty(true), RecSize(recsize)
{
}


//------------------------------------------------------------------------------
template<class C,bool bOrder>
	void RRecFile<C,bOrder>::Close(void)
{
	Find=false;
	Dirty=true;
	RBinaryFile::Close();
}


//------------------------------------------------------------------------------
template<class C,bool bOrder>
	void RRecFile<C,bOrder>::GoToRec(unsigned int nb)
{
	Find=false;
	RIOFile::Seek(nb*RecSize);
	Dirty=true;
	Find=true;
}


//------------------------------------------------------------------------------
template<class C,bool bOrder>
	void RRecFile<C,bOrder>::GoToRec(unsigned int c,unsigned int l,unsigned int maxc)
{
	Find=false;
	Seek((c+(l*maxc))*RecSize);
	Dirty=true;
	Find=true;
}


//------------------------------------------------------------------------------
template<class C,bool bOrder>
	template<class TUse>
		unsigned int RRecFile<C,bOrder>::Search(const TUse& tag)
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
template<class C,bool bOrder>
	unsigned int RRecFile<C,bOrder>::GetRecNb(void) const
{
	return(RIOFile::GetSize()/RecSize);
}


//------------------------------------------------------------------------------
template<class C,bool bOrder>
	unsigned int RRecFile<C,bOrder>::GetCurrentRec(void) const
{
	return(RIOFile::GetPos()/RecSize);
}


//------------------------------------------------------------------------------
template<class C,bool bOrder>
	void RRecFile<C,bOrder>::ReadRec(C& rec)
{
	rec.Read(*this);
}


//------------------------------------------------------------------------------
template<class C,bool bOrder>
	void RRecFile<C,bOrder>::WriteRec(C& rec)
{
	rec.Write(*this);
}


//------------------------------------------------------------------------------
template<class C,bool bOrder>
	void RRecFile<C,bOrder>::Start(void)
{
	Seek(0);
}


//------------------------------------------------------------------------------
template<class C,bool bOrder>
	void RRecFile<C,bOrder>::Next(void)
{
	if(Dirty)
		SeekRel(RecSize);
	Dirty=true;
}


//------------------------------------------------------------------------------
template<class C,bool bOrder>
	void RRecFile<C,bOrder>::Prev(void)
{
	if(Dirty)
		SeekRel(-RecSize);
	Dirty=true;
}


//------------------------------------------------------------------------------
template<class C,bool bOrder>
	C* RRecFile<C,bOrder>::operator()(void)
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
template<class C,bool bOrder>
	RRecFile<C,bOrder>::~RRecFile(void)
{
}
