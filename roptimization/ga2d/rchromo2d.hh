/*

	R Project Library

	RChromo2D.hh

	Chromosome for 2D placement GA - Inline Implementation

	(C) 1999-2001 by P. Francq.

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



//-----------------------------------------------------------------------------
//
// RChromo2D<cInst,cChromo,cFit,cInfo>
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData,class cInfo>
	RGA2D::RChromo2D<cInst,cChromo,cFit,cThreadData,cInfo>::
		RChromo2D(cInst *inst,unsigned int id) throw(bad_alloc)
			: RChromo<cInst,cChromo,cFit,cThreadData>(inst,id), Heuristic(0), Grid(0),
				Objs(0), NbObjs(0), thOrder(0), thObjs(0),
				thObj1(0), thObj2(0), Infos(0), Selected(0), Limits()
{
	
}


//-----------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData,class cInfo>
	void RGA2D::RChromo2D<cInst,cChromo,cFit,cThreadData,cInfo>::
		Init(cThreadData *thData) throw(bad_alloc)
{
	unsigned int i;
	cInfo **ptr;
	RObj2D **objs;

	// Call the initialisation of the parent
	RChromo<cInst,cChromo,cFit,cThreadData>::Init(thData);

	// Init "thread-dependent" data
	thOrder=thData->Order;
	thOrder2=thData->Order2;
	thObjs=thData->tmpObjs;
	thObj1=thData->tmpObj1;
	thObj2=thData->tmpObj2;
	Heuristic=thData->Heuristic;

	// If objects to place -> create the information and selection
	if(NbObjs)
	{
		// Create Infos
		Infos=new cInfo*[NbObjs+2];	// +2 -> because need of temporary objects
			for(i=NbObjs+1,ptr=Infos,objs=Objs;--i;ptr++,objs++)
				(*ptr)=new cInfo(*objs);  		
		(*(ptr++))=new cInfo(thObj1);	
		(*ptr)=new cInfo(thObj2);

		// Create Selected
		Selected=new bool[NbObjs];
		memset(Selected,0,NbObjs*sizeof(bool));
	}

	// Init Limits
	Limits=Instance->GetLimits();

	// Init Occupied	(Add 1 Because <= Limits)
	Grid=new RGrid(Limits);
}


//-----------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData,class cInfo>
	inline void RGA2D::RChromo2D<cInst,cChromo,cFit,cThreadData,cInfo>::
		ClearInfos(void)
{
	unsigned int i;
	cInfo **info;

	// All objects place in (MaxCoord,MaxCoord)
	for(i=NbObjs+3,info=Infos;--i;info++)  	// +3 because there are 2 temporary objects
		(*info)->Clear();

	// ActLimits -> (0,0)
	ActLimits.Set(0,0);

}


//-----------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData,class cInfo>
	inline void RGA2D::RChromo2D<cInst,cChromo,cFit,cThreadData,cInfo>::
		thVarsClear(void)
{
	thNbObjs=0;
	memset(thObjs,0,NbObjs*sizeof(RObj2D*));
	thObj1->Clear();
	thObj2->Clear();
}


//-----------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData,class cInfo>
	void RGA2D::RChromo2D<cInst,cChromo,cFit,cThreadData,cInfo>::
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


//-----------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData,class cInfo>
	bool RGA2D::RChromo2D<cInst,cChromo,cFit,cThreadData,cInfo>::
		RandomConstruct(void)
{
	memset(Selected,0,NbObjs*sizeof(bool));
	Heuristic->Run(Instance->Problem,Infos,Grid);
	RRect r=Heuristic->GetResult();
	ActLimits.Set(r.Width(),r.Height());
	return(true);
}


//-----------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData,class cInfo>
	bool RGA2D::RChromo2D<cInst,cChromo,cFit,cThreadData,cInfo>::
		Crossover(cChromo* /*parent1*/,cChromo* /*parent2*/)
{
//	RGeoInfo *info;
//
//	// Init Part
	memset(Selected,0,NbObjs*sizeof(bool));						// No object selected
//	thVarsClear();															// Clear all temporary variables
//
//	// Select objs in parent1 & parent2
//	parent1->GetSetOfObjs(thObj1,Selected);						
////	parent2->GetSetOfObjs(thObj2,Selected);						
//
//	// Put the rest of the objects & apply the placement heuristic
//	FillObjs(thObjs,thNbObjs);												
////	Heuristic->Run(Limits,Grid,Infos,0,thNbObjs);
//
//	// Remplace container by objects
//	info=Infos[NbObjs];
///*	if((info->Pos.X<MaxCoord)&&(info->Pos.Y<MaxCoord))
//		thObj1->Assign(Infos,Grid,info->Pos);*/
////	info=Infos[NbObjs+1];
////	thObj2->Assign(Infos,OccupiedX,OccupiedY,info->Pos.X,info->Pos.Y);
	return(true);
}


//-----------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData,class cInfo>
	bool RGA2D::RChromo2D<cInst,cChromo,cFit,cThreadData,cInfo>::Mutation(void)
{
	memset(Selected,0,NbObjs*sizeof(bool));
//	Heuristic->Run(Limits,Grid,Infos,0,NbObjs);
	return(true);
}


//-----------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData,class cInfo>
	bool RGA2D::RChromo2D<cInst,cChromo,cFit,cThreadData,cInfo>::Verify(void)
{
	unsigned int i,j;
	RGeoInfo **infoi,**infoj;

	for(i=0,infoi=Infos;i<NbObjs-1;i++,infoi++)
		for(j=i+1,infoj=&Infos[i+1];j<NbObjs;j++,infoj++)
			if(((*infoi)->IsValid())&&((*infoj)->IsValid()))
 				if((*infoi)->Overlap(*infoj))
 				{
 					cerr<<"Overlapping Problem: (Id=="<<Id<<")"<<endl<<"  "<<i<<" : ";
 					for((*infoi)->Start();!(*infoi)->End();(*infoi)->Next())
 						cerr<<"("<<((**infoi)()).X<<","<<((**infoi)()).Y<<")     ";
 					cerr<<endl<<"  "<<j<<" : ";
 					for((*infoj)->Start();!(*infoi)->End();(*infoj)->Next())
 						cerr<<"("<<((**infoj)()).X<<","<<((**infoj)()).Y<<")     ";
 					cerr<<endl;
 					return(false);
 				}
	return(true);
}


//-----------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData,class cInfo>
  RChromo2D<cInst,cChromo,cFit,cThreadData,cInfo>&
		RGA2D::RChromo2D<cInst,cChromo,cFit,cThreadData,cInfo>::operator=(const RChromo2D &chromo)
{
	unsigned int i;
	cInfo **ptr,**ptr2;

	RChromo<cInst,cChromo,cFit,cThreadData>::operator=(chromo);
	for(i=NbObjs+1,ptr=Infos,ptr2=chromo.Infos;--i;ptr++,ptr2++)
		(**ptr)=(**ptr2);
	memcpy(Selected,chromo.Selected,NbObjs*sizeof(bool));
	return(*this);
}


//-----------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData,class cInfo>
	RObj2D* RGA2D::RChromo2D<cInst,cChromo,cFit,cThreadData,cInfo>::GetObj(RCoord X,RCoord Y)
{
	unsigned int obj;
	
	if((X>Limits.X)||(Y>Limits.Y)) return(0);
	obj=Grid->GetObjId(X,Y);
	if(obj!=NoObject)
		return(Objs[obj]);
	else
		return(0);
}


//-----------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData,class cInfo>
	cInfo* RGA2D::RChromo2D<cInst,cChromo,cFit,cThreadData,cInfo>::GetInfo(RCoord X,RCoord Y)
{
	unsigned int obj;
	
	if((X>Limits.X)||(Y>Limits.Y)) return(0);
	obj=Grid->GetObjId(X,Y);
	if(obj!=NoObject)
		return(Infos[obj]);
	else
		return(0);
}


//-----------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData,class cInfo>
	RPoint& RGA2D::RChromo2D<cInst,cChromo,cFit,cThreadData,cInfo>::GetActLimits(void)
{
	RPoint *pt=RPoint::GetPoint();

	(*pt)=ActLimits;
	return(*pt);
}


//-----------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData,class cInfo>
	RPoint& RGA2D::RChromo2D<cInst,cChromo,cFit,cThreadData,cInfo>::GetLevel(unsigned int i)
{
	RPoint *pt=RPoint::GetPoint();

	(*pt)=Levels[i];
	return(*pt);
}


//-----------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData,class cInfo>
	RGA2D::RChromo2D<cInst,cChromo,cFit,cThreadData,cInfo>::~RChromo2D(void)
{
	cInfo **ptr;
	unsigned int i;
	
	if(Infos)
	{
		for(i=NbObjs+3,ptr=Infos;--i;ptr++) delete(*ptr);
		delete[] Infos;
	}
	if(Selected) delete[] Selected;
	if(Grid) delete Grid;
}
