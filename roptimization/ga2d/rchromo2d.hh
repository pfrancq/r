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
	Rainbow, as the General Public License permits, the exception does
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
template<class cInst,class cChromo,class cFit,class cThreadData,class cInfo>
	RChromo2D<cInst,cChromo,cFit,cThreadData,cInfo>::
		RChromo2D(cInst *inst,unsigned int id) throw(bad_alloc)
			: RChromo<cInst,cChromo,cFit,cThreadData>(inst,id), Levels(0), NbLevels(0),
				Objs(0), NbObjs(0),	OccupiedX(0), OccupiedY(0), thOrder(0), thObjs(0),
				thObj1(0), thObj2(0), Infos(0), Selected(0), Limits()
{
	
}


//---------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData,class cInfo>
	void RChromo2D<cInst,cChromo,cFit,cThreadData,cInfo>::
		Init(cThreadData *thData) throw(bad_alloc)
{
 	unsigned int i;
 	cInfo **ptr;
 	RCoord j;
	unsigned int **bptr;

	// Call the initialisation of the parent
	RChromo<cInst,cChromo,cFit,cThreadData>::Init(thData);

	// If objects to place -> create the information and selection
	if(NbObjs)
	{
	  // Create Infos
  	Infos=new cInfo*[NbObjs+2];	// +2 -> because need of temporary objects
   	for(i=NbObjs+3,ptr=Infos;--i;ptr++)
   			(*ptr)=new cInfo();  		

		// Create Selected
		Selected=new bool[NbObjs];
		memset(Selected,0,NbObjs*sizeof(bool));
  }

	// Init Limits
	Limits=Instance->GetLimits();

	// Init Occupied	(Add 1 Because <= Limits)
 	OccupiedX = new unsigned int*[Limits.X+1];
  for(j=Limits.X+2,bptr=OccupiedX;--j;bptr++)
  {
    (*bptr) = new unsigned int[Limits.Y+1];
    memset(*bptr,NoObject,sizeof(unsigned int)*(Limits.Y+1));
  }
  OccupiedY = new unsigned int*[Limits.Y+1];
  for(j=Limits.Y+2,bptr=OccupiedY;--j;bptr++)
  {
    (*bptr) = new unsigned int[Limits.X+1];
    memset(*bptr,NoObject,sizeof(unsigned int)*(Limits.X+1));
  }

	// Init "thread-dependent" data
 	thOrder=thData->Order;
 	thOrder2=thData->Order2;
 	thObjs=thData->tmpObjs;
 	thObj1=thData->tmpObj1;
 	thObj2=thData->tmpObj2;

	// Init Levels if needed
	if((Instance->GetHeuristic()==BottomLeft)||(Instance->GetHeuristic()==Edge))
		Levels = new RPoint[40];
}


//---------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData,class cInfo>
	inline void RChromo2D<cInst,cChromo,cFit,cThreadData,cInfo>::
		ClearInfos(void)
{
	RCoord R;
	unsigned int **ptr;
	unsigned int i;
	cInfo **info;
	RPoint *pt;

  // Clear Grid -> Put NoObject in each position
  for(R=Limits.X+2,ptr=OccupiedX;--R;ptr++)
    memset(*ptr,0xFF,sizeof(unsigned int)*(Limits.Y+1));
  for(R=Limits.Y+2,ptr=OccupiedY;--R;ptr++)
    memset(*ptr,0xFF,sizeof(unsigned int)*(Limits.X+1));

	// All objects place in (MaxCoord,MaxCoord)
	for(i=NbObjs+3,info=Infos;--i;info++)  	// +3 because there are 2 temporary objects
		(*info)->Clear();

	// ActLimits -> (0,0)
	ActLimits.X=ActLimits.Y=0;

	// Levels -> (0,0)	
	for(i=NbLevels+1,pt=Levels;--i;pt++)
		pt->Set(0,0);
	NbLevels=0;
}


//---------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData,class cInfo>
	inline void RChromo2D<cInst,cChromo,cFit,cThreadData,cInfo>::
		thVarsClear(void)
{
	thNbObjs=0;
	memset(thObjs,0,NbObjs*sizeof(RObj2D*));
	thObj1->Clear();
	thObj2->Clear();
}


//---------------------------------------------------------------------------
// Regroup a set of object from Objs in obj and add it to objs
// Make sure that the objects are not already selected
template<class cInst,class cChromo,class cFit,class cThreadData,class cInfo>
	bool RChromo2D<cInst,cChromo,cFit,cThreadData,cInfo>::
		GetSetOfObjs(RObj2DContainer *obj,bool *selected)
{
	unsigned int idx;												// Id of the current object
  bool bContinue;													// Continue to construct
	unsigned long Current;									// Current index in obj
	RObj2D *CurObj;													// Current object use to search others
	RGeoInfo* CurInfo;											// Current geometric information
	RPoint *start,*end;												
	unsigned int nbpts;
	RCoord X,Y,FindX,FindY;
	int FromDir;		// 0=left ; 1=right ; 2=up ; 3=down

	// Find an object not used and add it to selection
  RRandom::randorder<unsigned int>(thOrder,NbObjs);
	idx=0;
	while(selected[thOrder[idx]]) idx++;
	idx=thOrder[idx];
	CurObj=Objs[idx];
	CurInfo=Infos[idx];	
	selected[idx]=true;
	obj->AddObj(CurObj,CurInfo);
	bContinue=true;
	Current=0;
	start=CurInfo->Bound.GetBottomLeft();
	end=CurInfo->Bound.GetConX(start);
	FromDir=0;
	X=start->X+CurInfo->Pos.X;
	Y=start->Y+CurInfo->Pos.Y;
	nbpts=CurInfo->Bound.NbPtr-1;
 	
	// Select other objects
	while(bContinue)
	{
		// Where to search for an object
		switch(FromDir)
		{
			case 0: // from left
				FindX=X;
				FindY=Y-1;
				break;

			case 1: // from right
				FindX=X;
				FindY=Y+1;
				break;

			case 2: // from bottom
				FindY=Y;
				FindX=X+1;
				break;

			case 3: // from up
				FindY=Y;
				FindX=X-1;
				break;
		}

		// Is in (FindX,FindY) an object to select
		if((FindX>0)&&(FindY>0)&&(FindX<Limits.X)&&(FindY<Limits.Y))
		{
  		idx=OccupiedX[FindX][FindY];
  		if((idx!=NoObject)&&(!selected[idx]))	// Find an object not selected
  		{
  			obj->AddObj(Objs[idx],Infos[idx]);
  			selected[idx]=true;
      }
		}

		// If end of an edge
		if((X==end->X+CurInfo->Pos.X)&&(Y==end->Y+CurInfo->Pos.Y))
		{
			start=end;
			nbpts--;			// Next point
			X=start->X+CurInfo->Pos.X;
			Y=start->Y+CurInfo->Pos.Y;
			if(FromDir<2)	// Go to up/bottom
			{
				end=CurInfo->Bound.GetConY(start);
				if(start->Y<end->Y) FromDir=2; else FromDir=3;
			}
			else		// Go to left/right
			{
				end=CurInfo->Bound.GetConX(start);
				if(start->X<end->X) FromDir=0; else FromDir=1;
			}
		}

		// Go to next pos
		switch(FromDir)
		{
			case 0: // from left
				X++;
				break;

			case 1: // from right
				X--;
				break;

			case 2: // from bottom
				Y++;
				break;

			case 3: // from up
				Y--;
				break;
		}

		// If all the current object was search -> new object
		if(!nbpts)
		{
			Current++;
			if(Current==obj->Nb)	// Scanning for all objects selected done
				bContinue=false;
			else
			{
				idx=obj->Ids[Current];
      	CurObj=Objs[idx];
      	CurInfo=Infos[idx];	
				start=CurInfo->Bound.GetBottomLeft();
      	end=CurInfo->Bound.GetConX(start);
				X=start->X+CurInfo->Pos.X;
				Y=start->Y+CurInfo->Pos.Y;
      	FromDir=0;
      	nbpts=CurInfo->Bound.NbPtr-1;	
			}
		}

    // Test if continue to add other objects
		if(obj->Nb>=(NbObjs/3)) bContinue=false;
  }

	obj->EndObjs();
	return(true);
}


//---------------------------------------------------------------------------
// Fill objs with all the objects not already selected
template<class cInst,class cChromo,class cFit,class cThreadData,class cInfo>
	void RChromo2D<cInst,cChromo,cFit,cThreadData,cInfo>::
		FillObjs(RObj2D **objs,unsigned int &nbobjs)
{
	unsigned int i;
	RObj2D **obj;
	bool *ptr;

	nbobjs=1;
	(*(objs++))=thObj1;
//	(*(objs++))=thObj2;
	for(i=NbObjs+1,ptr=Selected,obj=Objs;--i;ptr++,obj++)
		if(!(*ptr))
		{
			nbobjs++;
			(*(objs++))=(*obj);
		}
}


//---------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData,class cInfo>
  void RChromo2D<cInst,cChromo,cFit,cThreadData,cInfo>::
		GetDirections(char *dirs,char &nbdirs,const RPoint &pos)
{
	RCoord TempX,TempY;

	// Look in wheach quadrant
	TempX=pos.X-Attraction.X;
	TempY=pos.Y-Attraction.Y;

	// By default
	nbdirs=0;

	// If bottom or up
	if(TempY<0)
		dirs[nbdirs++]=2;
	else
		if(TempY>0)
			dirs[nbdirs++]=3;

	// If left or right
	if(TempX<0)
		dirs[nbdirs++]=1;
	else
		if(TempX>0)
			dirs[nbdirs++]=0;
}


//---------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData,class cInfo>
  void RChromo2D<cInst,cChromo,cFit,cThreadData,cInfo>::
		LocalOptimisation(const RRects &rects,RPoint &pos)
{
	bool bCanPush;						// Can contune to push
	bool bClip;								// Must clip to test
	bool bAnotherTry;					// Try it again
	bool bInvertDirs;					// Invert Direction has been done
  unsigned int i;
	RCoord j;
  RRect **rect;
	RRect Rect;								// Current rectangle to treat
  unsigned int *nptr;
	RPoint Test;     					// Test coordinates
	char Dirs[2],Dir,*dir;		// Directions (0=left ; 1=right ; 2=up ; 3=down)
	char NbDir;								// Number of possible directions

	// Init
	Test=pos;
	bAnotherTry=true;
	bInvertDirs=false;

	// Get the directions
	GetDirections(Dirs,NbDir,pos);
	if(!NbDir) return;
	
	while(bAnotherTry)
	{
		// By default: the last
		bAnotherTry=false;

		for(Dir=NbDir+1,dir=Dirs;--Dir;dir++)
		{
			switch(*dir)    	
			{
				case 0:		// Push Left        	
          bCanPush=true;
        	while(bCanPush&&Test.X)
        	{				
        		bClip=false;
        		for(i=rects.NbPtr+1,rect=rects.Tab;(--i)&&bCanPush;rect++)
        		{
        			Rect=(**rect);
         			Rect+=Test;
      				if(Rect.Clip(Limits)) bClip=true;/*bCanPush=false;*/
        			for(j=Rect.Height()+1,nptr=&OccupiedX[Rect.Pt1.X-1][Rect.Pt1.Y];(--j)&&bCanPush;nptr++)
        				if((*nptr)!=NoObject) bCanPush=false;	
        		}
        		if(bCanPush)
      			{
      				Test.X--;		// Push it left
      				bAnotherTry=true;
      			}
        		if((!bClip)&&bCanPush)
        			pos=Test;
        	}
          break;

				case 3:		// Push Down        	
          bCanPush=true;
        	while(bCanPush&&Test.Y)
         	{				
         		bClip=false;
         		for(i=rects.NbPtr+1,rect=rects.Tab;(--i)&&bCanPush;rect++)
         		{
         			Rect=(**rect);
         			Rect+=Test;
       				if(Rect.Clip(Limits)) bClip=true;/*bCanPush=false;*/
         			for(j=Rect.Width()+1,nptr=&OccupiedY[Rect.Pt1.Y-1][Rect.Pt1.X];(--j)&&bCanPush;nptr++)
         				if((*nptr)!=NoObject) bCanPush=false;	
         		}
         		if(bCanPush) Test.Y--;
        		if((!bClip)&&bCanPush)
         			pos=Test;
         	}
					break;
      }
    }
		if(NbDir==1) bAnotherTry=false;
		if((!bAnotherTry)&&(!bInvertDirs)&&(NbDir==2)&&(bClip))
		{
			bAnotherTry=true;
			bInvertDirs=true;
			Dir=Dirs[0];
			Dirs[0]=Dirs[1];
			Dirs[1]=Dir;
		}
	}
}


//---------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData,class cInfo>
  bool RChromo2D<cInst,cChromo,cFit,cThreadData,cInfo>::
		HeuristicBL(RObj2D **objs,unsigned int nbobjs)
{
  unsigned int i,*ptr;													// Variables to initialise 'thOrder'
  RObj2D *Current;                              // Current object to place
  char Ori;																			// Current Orientation
  RGeoInfo *info;																// Current info
	RRect Rect;																		// Bound rectangle of current object
  RCoord SizeX,SizeY;														// Length and Height of Rect
	unsigned int nbswitch;												// Number of switch that can be tried	
	RPoint Pos(0,0);															// Test Position
	RPoint Actual(0,0);														// Actual Left Position
	RPoint Max(0,0);															// Max Position	
	double FactorX,FactorY;												// Ratio between BestPoss and Limits
	unsigned int CurLevel;												// Actual "Level"
	unsigned int Hold=nbobjs;

	// Calculate an initial random order
  for(i=0,ptr=thOrder;i<nbobjs;ptr++,i++) (*ptr)=i;
  RRandom::randorder<unsigned int>(thOrder,nbobjs);
	memcpy(thOrder2,thOrder,nbobjs*sizeof(unsigned int));
	Attraction.Set(0,0);
	if(Instance->GetHeuristic()==BottomLeft)
		Max.X=Limits.X;;
	CurLevel=0;

	// Clear Geometric informations
	ClearInfos();

	
  // Place the objects
	nbswitch=nbobjs;
  while(nbobjs--) // Decrease and index for current object
  {

		// Decrease number of switchs possible
		if(nbswitch) nbswitch--;


		// Find a polygon for the object
    Current=objs[thOrder[nbobjs]];
		info=Infos[Current->Id];
    Ori = Current->PossOri[RRandom::RRand(Current->NbPossOri)];
    info->AssignBound(Current->Id,Current->Polygons[Ori]);
    info->Bound.Boundary(Rect);
    SizeX=Rect.Width();
    SizeY=Rect.Height();
		info->Order=Hold-nbobjs;


		// Verify if add normally or on bottom
		if((Instance->GetHeuristic()==Edge)&&((ActLimits.X+1)<Limits.X))
		{
			FactorX=(static_cast<double>(ActLimits.X))/(static_cast<double>(Limits.X));
 			FactorY=(static_cast<double>(ActLimits.Y))/(static_cast<double>(Limits.Y));
 			if(FactorX<FactorY)
 			{
				// Next element at bottom
				Actual.Set(ActLimits.X,0);
 				Max.X=ActLimits.X+SizeX+1;
 				if(Max.X>Limits.X)
	 				Max.X=Limits.X;
				CurLevel=0;
 			}
    }
					
		// Do a local optimisationn at actual position
		Pos=Actual;
		if(Instance->bLocalOpti)	
			LocalOptimisation(info->Rects,Pos);


		// If to long than begin from left again
		while((Pos.X>0)&&(Pos.X+SizeX>Max.X))
		{
			if(Instance->GetHeuristic()==Edge)
			{
				Levels[CurLevel++]=Actual;
				if(CurLevel>NbLevels)
				{
					NbLevels=CurLevel;
					Actual.Set(0,Max.Y);
				}
				else
					Actual=Levels[CurLevel];									
      }
      else
				Actual.Set(0,Max.Y);
			Pos=Actual;
			if(Instance->bLocalOpti)	// Do a local optimisation
 				LocalOptimisation(info->Rects,Pos);
    }


		// If to high than try to switch objects and place another one
		if(Pos.Y+SizeY>Limits.Y)
		{
 			// Try to switch
 			if(nbswitch) 	
 			{
  			i=thOrder[nbobjs];
 				thOrder[nbobjs++]=(*thOrder);
 				(*thOrder)=i;
 				nbswitch--;
 				continue;
 			}
 			return(false);
		}


		// Assign the object to the current position
    info->Assign(OccupiedX,OccupiedY,Pos.X,Pos.Y);
		

		// Calculate Next position
		if(Pos.X+SizeX>Actual.X)
			Actual.X=Pos.X+SizeX;
		if((Pos.X==0)&&(Pos.X+SizeX)>Max.X)
			Max.X=Pos.X+SizeX;
		if(Pos.Y+SizeY>Max.Y)
			Max.Y=Pos.Y+SizeY;

		// Verify if down level needed update
		for(i=0;i<CurLevel;i++)
			if(Levels[i].X<Max.X) Levels[i].X=Max.X;


		// Verify if some levels must be skipped
		i=CurLevel+1;
		while((i<NbLevels)&&(Levels[i].Y<Pos.Y+SizeY))
		{
			NbLevels--;
			for(int j=i;j<NbLevels;j++)
				Levels[j]=Levels[j+1];
		}


		// Verify ActLimits
		if(Pos.X+SizeX>ActLimits.X)
		{
			ActLimits.X=Pos.X+SizeX;
			if(Max.X==0) Max.X=ActLimits.X;
		}
		if(Pos.Y+SizeY>ActLimits.Y)
			ActLimits.Y=Pos.Y+SizeY;
  }

	// Ok, all objects are placed
	cerr<<NbLevels<<endl;
	return(true);
}


//---------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData,class cInfo>
  bool RChromo2D<cInst,cChromo,cFit,cThreadData,cInfo>::
		HeuristicC(RObj2D **objs,unsigned int nbobjs)
{
  unsigned int i,*ptr;													// Variables to initialise 'thOrder'
  RObj2D *Current;                              // Current object to place
  char Ori;																			// Current Orientation
  RGeoInfo *info;																// Current info
	RRect Rect;																		// Bound rectangle of current object
  RCoord TempX,TempY;														// Length and Height of Rect
	RPoint Pos(0,0);															// Test Position

	// Calculate an initial random order
  for(i=0,ptr=thOrder;i<nbobjs;ptr++,i++) (*ptr)=i;
  RRandom::randorder<unsigned int>(thOrder,nbobjs);
	Attraction.X=Limits.X/2;
	Attraction.Y=Limits.Y/2;

	// Find a polygon for the object
  Current=objs[thOrder[--nbobjs]];
	info=Infos[Current->Id];
  Ori = Current->PossOri[RRandom::RRand(Current->NbPossOri)];
  info->AssignBound(Current->Id,Current->Polygons[Ori]);
  info->Boundary(Rect);
  TempX=Rect.Width();
  TempY=Rect.Height();

	// Calculate Position for the first object and assign it
	Pos.X=(Limits.X-TempX)/2;
	Pos.Y=(Limits.Y-TempY)/2;
  info->Assign(OccupiedX,OccupiedY,Pos.X,Pos.Y);

	while(nbobjs--)
	{
	}
	return(true);	
}


//---------------------------------------------------------------------------
// Initial construction
template<class cInst,class cChromo,class cFit,class cThreadData,class cInfo>
	bool RChromo2D<cInst,cChromo,cFit,cThreadData,cInfo>::
		RandomConstruct(void)
{
	bool b;

	memset(Selected,0,NbObjs*sizeof(bool));
	switch(Instance->GetHeuristic())
	{
		case BottomLeft:
		case Edge:
			b=HeuristicBL(Objs,NbObjs);
			break;

		case Central:
			b=HeuristicC(Objs,NbObjs);
			break;
	}
	return(b);
}


//---------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData,class cInfo>
	bool RChromo2D<cInst,cChromo,cFit,cThreadData,cInfo>::
		Crossover(cChromo *parent1,cChromo *parent2)
{
	RGeoInfo *info;
	bool b;

	// Init Part
	memset(Selected,0,NbObjs*sizeof(bool));						// No object selected
	thVarsClear();																		// Clear all temporary variables

	// Select objs in parent1 & parent2
	parent1->GetSetOfObjs(thObj1,Selected);						
//	parent2->GetSetOfObjs(thObj2,Selected);						

	// Put the rest of the objects & apply the placement heuristic
  FillObjs(thObjs,thNbObjs);												
	switch(Instance->GetHeuristic())
	{
		case BottomLeft:
		case Edge:
			b=HeuristicBL(thObjs,thNbObjs);
			break;

		case Central:
			b=HeuristicC(thObjs,thNbObjs);
			break;
	}

	// Remplace container by objects
	info=Infos[NbObjs];
	if((info->Pos.X<MaxCoord)&&(info->Pos.Y<MaxCoord))
		thObj1->Assign(Infos,OccupiedX,OccupiedY,info->Pos.X,info->Pos.Y);
/*	info=Infos[NbObjs+1];
	thObj2->Assign(Infos,OccupiedX,OccupiedY,info->Pos.X,info->Pos.Y);*/
	return(b);
}


//---------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData,class cInfo>
	bool RChromo2D<cInst,cChromo,cFit,cThreadData,cInfo>::Mutation(void)
{
	bool b;

	memset(Selected,0,NbObjs*sizeof(bool));
	switch(Instance->GetHeuristic())
	{
		case BottomLeft:
		case Edge:
			b=HeuristicBL(Objs,NbObjs);
			break;

		case Central:
			b=HeuristicC(Objs,NbObjs);
			break;
	}
	return(b);
}


//---------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData,class cInfo>
	bool RChromo2D<cInst,cChromo,cFit,cThreadData,cInfo>::Verify(void)
{
	unsigned int i,j;
	RGeoInfo **infoi,**infoj;
	RPoint **point;
	unsigned int k;

	for(i=0,infoi=Infos;i<NbObjs-1;i++,infoi++)
		for(j=i+1,infoj=&Infos[i+1];j<NbObjs;j++,infoj++)
			if(((*infoi)->Pos.X<MaxCoord)&&((*infoi)->Pos.Y<MaxCoord))
				if(((*infoj)->Pos.X<MaxCoord)&&((*infoj)->Pos.Y<MaxCoord))
  				if((*infoi)->Overlap(*infoj))
  				{
  					cerr<<"Overlapping Problem (Id=="<<Id<<") between ";
  					cerr<<i<<"("<<(*infoi)->Pos.X<<","<<(*infoi)->Pos.Y<<") and ";
  					cerr<<j<<"("<<(*infoj)->Pos.X<<","<<(*infoj)->Pos.Y<<")\n";
  					cerr<<"  i :";
  					for(k=(*infoi)->Bound.NbPtr+1,point=(*infoi)->Bound.Tab;--k;point++)
  						cerr<<"("<<(*point)->X<<","<<(*point)->Y<<")     ";
  					cerr<<"\n  j :";
  					for(k=(*infoj)->Bound.NbPtr+1,point=(*infoj)->Bound.Tab;--k;point++)
  						cerr<<"("<<(*point)->X<<","<<(*point)->Y<<")     ";			
  					cerr<<"\n";
  					return(false);
  				}
	return(true);
}


//---------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData,class cInfo>
  RChromo2D<cInst,cChromo,cFit,cThreadData,cInfo>&
		RChromo2D<cInst,cChromo,cFit,cThreadData,cInfo>::operator=(const RChromo2D &chromo)
{
	unsigned int i;
	RCoord j;
	unsigned int **bptr,**bptr2;
 	cInfo **ptr,**ptr2;

	RChromo<cInst,cChromo,cFit,cThreadData>::operator=(chromo);
  for(j=Limits.X+2,bptr=OccupiedX,bptr2=chromo.OccupiedX;--j;bptr++,bptr2++)
    memcpy(*bptr,*bptr2,sizeof(unsigned int)*(Limits.Y+1));
  for(j=Limits.Y+2,bptr=OccupiedY,bptr2=chromo.OccupiedY;--j;bptr++,bptr2++)
    memcpy(*bptr,*bptr2,sizeof(unsigned int)*(Limits.X+1));
 	for(i=NbObjs+1,ptr=Infos,ptr2=chromo.Infos;--i;ptr++,ptr2++)
		(**ptr)=(**ptr2);
	memcpy(Selected,chromo.Selected,NbObjs*sizeof(bool));
	return(*this);
}


//---------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData,class cInfo>
	RObj2D* RChromo2D<cInst,cChromo,cFit,cThreadData,cInfo>::GetObj(RCoord X,RCoord Y)
{
	unsigned int obj;
	
	if((X>Limits.X)||(Y>Limits.Y)) return(0);
	obj=OccupiedX[X][Y];
	if(obj!=NoObject)
		return(Objs[obj]);
	else
		return(0);
}


//---------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData,class cInfo>
	cInfo* RChromo2D<cInst,cChromo,cFit,cThreadData,cInfo>::GetInfo(RCoord X,RCoord Y)
{
	unsigned int obj;
	
	if((X>Limits.X)||(Y>Limits.Y)) return(0);
	obj=OccupiedX[X][Y];
	if(obj!=NoObject)
		return(Infos[obj]);
	else
		return(0);
}


//---------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData,class cInfo>
	RPoint& RChromo2D<cInst,cChromo,cFit,cThreadData,cInfo>::GetActLimits(void)
{
	RPoint *pt=RPoint::GetPoint();

	(*pt)=ActLimits;
	return(*pt);
}


//---------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData,class cInfo>
	RPoint& RChromo2D<cInst,cChromo,cFit,cThreadData,cInfo>::GetLevel(unsigned int i)
{
	RPoint *pt=RPoint::GetPoint();

	(*pt)=Levels[i];
	return(*pt);
}


//---------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData,class cInfo>
	RChromo2D<cInst,cChromo,cFit,cThreadData,cInfo>::~RChromo2D(void)
{
  cInfo **ptr;
  unsigned int i;
	RCoord j;
	unsigned int **bptr;
	
	if(Infos)
	{
  	for(i=NbObjs+3,ptr=Infos;--i;ptr++) delete(*ptr);
	  delete[] Infos;
	}
	if(OccupiedX)
	{
    for(j=Limits.X+2,bptr=OccupiedX;--j;bptr++)
      delete[] (*bptr);
    delete[] OccupiedX;
	}
	if(OccupiedY)
	{
    for(j=Limits.Y+2,bptr=OccupiedY;--j;bptr++)
      delete[] (*bptr);
    delete[] OccupiedY;
	}
	if(Selected) delete[] Selected;
	if(Levels) delete[] Levels;
}