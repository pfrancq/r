/*

  RInst2D.cpp

  Instance for 2D placement GA - Inline Implementation

  (C) 1999-2000 by P. Francq.

  Version $Revision$

  Last Modify: $Date$

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

	As a special exception to the GNU General Public License, permission is
	granted for additional uses of the text contained in its release
	of the Rainbow Library.

	The exception is that, if you link the Rainbow with other files
	to produce an executable, this does not by itself cause the
	resulting executable to be covered by the GNU General Public License.
	Your use of that executable is in no way restricted on account of
	linking the Rainbow library code into it.

	This exception does not however invalidate any other reasons why
	the executable file might be covered by the GNU General Public License.

	This exception applies only to the code released under the
	name Rainbow.  If you copy code from other releases into a copy of
	RAinbow, as the General Public License permits, the exception does
	not apply to the code that you add in this way.  To avoid misleading
	anyone as to the status of such modified files, you must delete
	this exception notice from them.

	If you write modifications of your own for Rainbow, it is your choice
	whether to permit this exception to apply to your modifications.
	If you do not wish that, delete this exception notice.

*/



//---------------------------------------------------------------------------
//
// RThreadData2D<cInfo>
//
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
template<class cInfo>
	RThreadData2D<cInfo>::RThreadData2D(unsigned int nbobjs)
		: NbObjs(nbobjs),Order(NULL),InObj(NULL),tmpInfos(NULL)
{
 	unsigned int i;
 	cInfo **ptr;
	RObj2D **obj;
	
	if(NbObjs)
	{
		tmpInfos=new cInfo*[NbObjs];
   	for(i=NbObjs+1,ptr=tmpInfos;--i;ptr++)
   			(*ptr)=new cInfo();  		
 		Order=new unsigned int[NbObjs];
 		InObj=new unsigned int[NbObjs];
		tmpObjs=new RObj2D*[NbObjs];
		for(i=0,obj=tmpObjs;i<NbObjs;obj++,i++)
			(*obj)=new RObj2D(i,false);	
  }
}


//---------------------------------------------------------------------------
template<class cInfo>
	RThreadData2D<cInfo>::~RThreadData2D(void)
{
  cInfo **ptr;
  unsigned int i;
	RObj2D **obj;
	
	if(tmpInfos)
	{
  	for(i=NbObjs+1,ptr=tmpInfos;--i;ptr++)	delete(*ptr);
	  delete[] tmpInfos;
	}
	if(tmpObjs)
	{
		for(i=NbObjs,obj=tmpObjs;--i;obj++) delete (*obj);
		delete[] tmpObjs;
	}
 	if(Order) delete[] Order;
	if(InObj) delete[] InObj;
}



//---------------------------------------------------------------------------
//
// RInst2D<cInst,cChromo,cFit>
//
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cInfo>
	RInst2D<cInst,cChromo,cFit,cInfo>::RInst2D(unsigned int popsize,RObj2D **objs,unsigned int nbobjs) throw(bad_alloc)
		: RInst<cInst,cChromo,cFit>(popsize), Objs(objs), NbObjs(nbobjs), thDatas(NULL), bLocalOpti(true)
{
	RObj2D **obj;
  cChromo **C;
  unsigned int i,nb;
	RRect Rect;

	AvLen=AvWidth=0;
	if(NbObjs)
	{
		for(i=NbObjs+1,obj=Objs,nb=0;--i;obj++)
		{		
			(*obj)->Polygon.Boundary(Rect);
			AvLen+=Rect.Pt2.X;
			AvWidth+=Rect.Pt2.Y;
			nb++;
			if((*obj)->IsOriSet(orRota90)||(*obj)->IsOriSet(orRota90MirrorX)||(*obj)->IsOriSet(orRota90MirrorY)||(*obj)->IsOriSet(orRota90MirrorYX))
			{
				AvLen+=Rect.Pt2.Y;
				AvWidth+=Rect.Pt2.X;
				nb++;
			}
		}
		AvLen/=nb;
		AvWidth/=nb;
	}
  for(i=PopSize+1,C=Chromosomes;--i;C++)
	{
		(*C)->Objs=Objs;
		(*C)->NbObjs=NbObjs;
	}
	BestChromosome->Objs=Objs;
	BestChromosome->NbObjs=NbObjs;
}


//---------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cInfo>
	void RInst2D<cInst,cChromo,cFit,cInfo>::Init(void) throw(bad_alloc)
{
  cChromo **C;
  unsigned int i;
	
	RInst<cInst,cChromo,cFit>::Init();
	thDatas=new RThreadData2D<cInfo>(NbObjs);
  for(i=PopSize+1,C=Chromosomes;--i;C++)
	{
		(*C)->thInfos=thDatas->tmpInfos;
		(*C)->thOrder=thDatas->Order;
		(*C)->thInObj=thDatas->InObj;
		(*C)->thObjs=thDatas->tmpObjs;
	}
}


//---------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cInfo>
	RInst2D<cInst,cChromo,cFit,cInfo>::~RInst2D(void)
{
	if(thDatas) delete thDatas;
}