/*

	Rainbow Library Project

  RChromo2D.hh

  Chromosome for 2D placement GA - Inline Implementation

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
// RChromo2D<cInst,cChromo,cFit,cInfo>
//
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cInfo>
	RChromo2D<cInst,cChromo,cFit,cInfo>::RChromo2D(cInst *inst,unsigned int id) throw(bad_alloc)
		: RChromo<cInst,cChromo,cFit>(inst,id), Objs(NULL),NbObjs(0),OccupiedX(NULL),OccupiedY(NULL),thOrder(NULL),thInObj(NULL),thInfos(NULL),thObjs(NULL),Infos(NULL)
{
 	RCoord j;
	unsigned int **bptr;
		
	// Init Occupied	
 	OccupiedX = new unsigned int*[Limits.Pt2.X];
  for(j=Limits.Pt2.X+1,bptr=OccupiedX;--j;bptr++)
  {
    (*bptr) = new unsigned int[Limits.Pt2.Y];
    memset(*bptr,NoObject,sizeof(unsigned int)*Limits.Pt2.Y);
  }
  OccupiedY = new unsigned int*[Limits.Pt2.Y];
  for(j=Limits.Pt2.Y+1,bptr=OccupiedY;--j;bptr++)
  {
    (*bptr) = new unsigned int[Limits.Pt2.X];
    memset(*bptr,NoObject,sizeof(unsigned int)*Limits.Pt2.X);
  }
}


//---------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cInfo>
	void RChromo2D<cInst,cChromo,cFit,cInfo>::Init(void) throw(bad_alloc)
{
 	unsigned int i;
 	cInfo **ptr;

	RChromo<cInst,cChromo,cFit>::Init();
	if(NbObjs)
	{
	  // Create Infos
  	Infos=new cInfo*[NbObjs];
   	for(i=NbObjs+1,ptr=Infos;--i;ptr++)
   			(*ptr)=new cInfo();  		
  }
}


//---------------------------------------------------------------------------
// Regroup a set of object from Objs in obj and add it to objs
// Make sure that the objects already assigned (through inobj) are not used
template<class cInst,class cChromo,class cFit,class cInfo>
	bool RChromo2D<cInst,cChromo,cFit,cInfo>::GetSetOfObjs(RObj2D **objs,unsigned int &nbobjs,unsigned int *inobj)
{
	RCoord X,Y;
	unsigned int idx;
	RObj2D *obj=objs[nbobjs];								// obj to generate
	
	// Find an object not used
	X=Limits.Pt1.X+RRand(Limits.Length());
	Y=Limits.Pt1.Y+RRand(Limits.Width());
	idx=OccupiedX[X][Y];
	while(idx==NoObject||inobj[idx]!=NoObject)
	{
		RandomPlace(X,Y);
		idx=OccupiedX[X][Y];
	}

	// Construct obj
	return(true);
}


//---------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cInfo>
  bool RChromo2D<cInst,cChromo,cFit,cInfo>::Heuristic(RObj2D **objs,cInfo **infos,unsigned int nbobjs,unsigned int **OccX,unsigned int **OccY)
{
  unsigned int i,*ptr;
	RCoord j;								// counter
  RCoord PosX=0,PosY=0;   // Left-Bottom
  RCoord NextPosY=0;
  RCoord TempY;
  RObj2D *Current;
  RObj2D *First=NULL;       // First object "of the line"
  RRect Rect;
  unsigned int **bptr;
  char Ori;
  RGeoInfo *info,*FirstInfo;

  // Initialisation
  for(j=Limits.Pt2.X+1,bptr=OccX;--j;bptr++)
    memset(*bptr,NoObject,sizeof(unsigned int)*Limits.Pt2.Y);
  for(j=Limits.Pt2.Y+1,bptr=OccY;--j;bptr++)
    memset(*bptr,NoObject,sizeof(unsigned int)*Limits.Pt2.X);
  for(i=0,ptr=thOrder;i<nbobjs;ptr++,i++) (*ptr)=i;
  randorder<unsigned int>(thOrder,nbobjs);

  // Place them
  while(nbobjs--) // Decrease and index for current object
  {
    Current=objs[thOrder[nbobjs]];
		info=infos[Current->Id];
    Ori = Current->PossOri[RRand(Current->NbPossOri)];
    info->AssignBound(Current->Polygons[Ori]);
    info->Pos.X=info->Pos.Y=0;
    if(!First)
    {
      First=Current;
      FirstInfo=info;
    }
    info->Boundary(Rect);
    TempY=Rect.Width();
    if(TempY+PosY>NextPosY)
      NextPosY=PosY+TempY;
    if(PosX+Rect.Pt2.X>Limits.Pt2.X)
    {
      PosX=0;
      FirstInfo->Boundary(Rect);
      PosY=NextPosY;
      NextPosY=0;
      First=NULL;
    }
    info->Assign(OccX,OccY,PosX,PosY,Current->Id);
    info->Boundary(Rect);
    PosX=Rect.Pt2.X+1;
  }
	
	return(true);
}


//---------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cInfo>
	bool RChromo2D<cInst,cChromo,cFit,cInfo>::RandomConstruct(void)
{
	return(Heuristic(Instance->Objs,Infos,NbObjs,OccupiedX,OccupiedY));
}


//---------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cInfo>
	bool RChromo2D<cInst,cChromo,cFit,cInfo>::Crossover(cChromo *parent1,cChromo *parent2)
{
	memset(thInObj,0xFF,sizeof(unsigned int)*NbObjs);
	return(true);
}


//---------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cInfo>
	bool RChromo2D<cInst,cChromo,cFit,cInfo>::Mutation(void)
{
	memset(thInObj,0xFF,sizeof(unsigned int)*NbObjs);
	return(true);
}


//---------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cInfo>
  RChromo2D<cInst,cChromo,cFit,cInfo>& RChromo2D<cInst,cChromo,cFit,cInfo>::operator=(const RChromo2D &chromo)
{
	unsigned int i;
	RCoord j;
	unsigned int **bptr,**bptr2;
 	cInfo **ptr,**ptr2;

	RChromo<cInst,cChromo,cFit>::operator=(chromo);
  for(j=Limits.Pt2.X+1,bptr=OccupiedX,bptr2=chromo.OccupiedX;--j;bptr++,bptr2++)
    memcpy(*bptr,*bptr2,sizeof(unsigned int)*Limits.Pt2.Y);
  for(j=Limits.Pt2.Y+1,bptr=OccupiedY,bptr2=chromo.OccupiedY;--j;bptr++,bptr2++)
    memcpy(*bptr,*bptr2,sizeof(unsigned int)*Limits.Pt2.X);
 	for(i=NbObjs+1,ptr=Infos,ptr2=chromo.Infos;--i;ptr++,ptr2++)
		(**ptr)=(**ptr2);
	return(*this);
}


//---------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cInfo>
	RChromo2D<cInst,cChromo,cFit,cInfo>::~RChromo2D(void)
{
  cInfo **ptr;
  unsigned int i;
	RCoord j;
	unsigned int **bptr;
	
	if(Infos)
	{
  	for(i=NbObjs+1,ptr=Infos;--i;ptr++) delete(*ptr);
	  delete[] Infos;
	}
  for(j=Limits.Pt2.X+1,bptr=OccupiedX;--j;bptr++)
    delete[] (*bptr);
  delete[] OccupiedX;
  for(j=Limits.Pt2.Y+1,bptr=OccupiedY;--j;bptr++)
    delete[] (*bptr);
  delete[] OccupiedY;
}