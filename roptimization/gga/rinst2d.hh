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
// RThreadData2D<cInst,cChromo>
//
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
template<class cInst,class cChromo>
	RThreadData2D<cInst,cChromo>::RThreadData2D(cInst *owner) throw(bad_alloc)
		: RThreadData<cInst,cChromo>(owner),NbObjs(0),Order(NULL),tmpObj1(NULL),tmpObj2(NULL)
{
}


//---------------------------------------------------------------------------
template<class cInst,class cChromo>
	void RThreadData2D<cInst,cChromo>::Init(void) throw(bad_alloc)
{
	RThreadData<cInst,cChromo>::Init();
	NbObjs=Owner->NbObjs;
	if(NbObjs)
	{
 		Order=new unsigned int[NbObjs];
		Order2=new unsigned int[NbObjs];
		tmpObjs=new RObj2D*[NbObjs];
 		tmpObj1=new RObj2DContainer(NbObjs,NbObjs);
 		tmpObj2=new RObj2DContainer(NbObjs+1,NbObjs);
  }
}


//---------------------------------------------------------------------------
template<class cInst,class cChromo>
	RThreadData2D<cInst,cChromo>::~RThreadData2D(void)
{
 	if(Order) delete[] Order;
	if(tmpObjs)	delete[] tmpObjs;
	if(tmpObj1) delete tmpObj1;
	if(tmpObj2) delete tmpObj2;
}



//---------------------------------------------------------------------------
//
// RInst2D<cInst,cChromo,cFit>
//
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData,class cInfo>
	RInst2D<cInst,cChromo,cFit,cThreadData,cInfo>::
		RInst2D(unsigned int popsize,RObj2D **objs,unsigned int nbobjs,RPoint &limits) throw(bad_alloc)
			: RInst<cInst,cChromo,cFit,cThreadData>(popsize), Objs(objs), NbObjs(nbobjs),
				bLocalOpti(true), bControlBottomLeft(true), Limits(limits)
{
  cChromo **C;
  unsigned int i;

  for(i=PopSize+1,C=Chromosomes;--i;C++)
	{
		(*C)->Objs=Objs;
		(*C)->NbObjs=NbObjs;
	}
	BestChromosome->Objs=Objs;
	BestChromosome->NbObjs=NbObjs;
}


//---------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData,class cInfo>
	RPoint& RInst2D<cInst,cChromo,cFit,cThreadData,cInfo>::GetLimits(void)
{
	RPoint *pt=RPoint::GetPoint();

	(*pt)=Limits;
	return(*pt);
}