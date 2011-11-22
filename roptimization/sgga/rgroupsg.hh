/*

	R Project Library

	RGroupSG.hh

	Similarity-based Grouping Genetic Algorithm Group - Implementation.

	Copyright 2002-2010 by Pascal Francq (pascal@francq.info).
	Copyright 2002-2008 by the Université Libre de Bruxelles (ULB).

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
// class RGroupSG<cGroup,cObj,cGroups>
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroups>
	RGroupSG<cGroup,cObj,cGroups>::RGroupSG(RGroupSG* grp)
	: RGroup<cGroup,cObj,cGroups>(grp), AvgIntraSim(0.0), AvgAgreement(0.0), AvgDisagreement(0.0), Centroid(0), ToEval(true)
{
}


//-----------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroups>
	RGroupSG<cGroup,cObj,cGroups>::RGroupSG(cGroups* owner,const size_t id)
	: RGroup<cGroup,cObj,cGroups>(owner,id), AvgIntraSim(0.0), AvgAgreement(0.0), AvgDisagreement(0.0), Centroid(0), ToEval(true)
{
}


//---------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroups>
	void RGroupSG<cGroup,cObj,cGroups>::Clear(void)
{
	RGroup<cGroup,cObj,cGroups>::Clear();
	AvgIntraSim=AvgAgreement=AvgDisagreement=0.0;
	Centroid=0;
	ToEval=true;
}


//---------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroups>
	bool RGroupSG<cGroup,cObj,cGroups>::HasSameUser(const cObj* obj) const
{
	size_t usr=obj->GetId();
	if(!usr)
		return(false);
	RCursor<cObj> ptr(Owner->GetObjs(*this));
	for(ptr.Start();!ptr.End();ptr.Next())
		if(usr==ptr()->GetId())
			return(true);
	return(false);
}


//---------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroups>
	bool RGroupSG<cGroup,cObj,cGroups>::CanInsert(const cObj* obj)
{
	// If no objects -> OK
	if(!RGroup<cGroup,cObj,cGroups>::NbSubObjects)
		return(true);

	// If too many objects -> No
	if(RGroup<cGroup,cObj,cGroups>::NbSubObjects>Owner->Instance->Params->NbMaxObjs)
		return(false);

	// Verify that no object of the group has the same parent or a disagreement ratio greater than the
	// maximum allowed.
	bool OneMinSim(false); // Suppose no object in the group as the minimum of similarity
	double tmp;
	const cObj* prof1(obj);
	size_t usr1(obj->GetId());
	RCursor<cObj> ptr(Owner->GetObjs(*this));
	for(ptr.Start(),LastMaxSim=-2.0;!ptr.End();ptr.Next())
	{
		// Do not put two objects with the same parent in the same group
		if(usr1&&(usr1==ptr()->GetId()))
			return(false);

		// Maximum disagreement must be respected
		if(Owner->Instance->GetDisagreementRatio(prof1,ptr())>=Owner->Instance->Params->MinDisagreement)
		  	return(false);

		// Verify the minimum similarity and remember the best similarity
		tmp=Owner->Instance->GetSim(prof1,ptr());
		if(tmp<Owner->Instance->Params->MinSimLevel)
		{
			if(Owner->Instance->Params->AllMinSim)
				return(false);
		}
		else
			OneMinSim=true;
		if(tmp>LastMaxSim)
			LastMaxSim=tmp;
	}

	// Verify that at least one object has the minimum of similarity
	if(!OneMinSim)
		return(false);

	// OK, it can be grouped.
	return(true);
}


//---------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroups>
	void RGroupSG<cGroup,cObj,cGroups>::PostInsert(const cObj* /*obj*/)
{
	ToEval=true;
	if(Owner->VerifyCentroids)
		Centroid=0;
}


//---------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroups>
	void RGroupSG<cGroup,cObj,cGroups>::PostDelete(const cObj* /*obj*/)
{
	ToEval=true;
	if(Owner->VerifyCentroids)
		Centroid=0;
}


//---------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroups>
	double RGroupSG<cGroup,cObj,cGroups>::ComputeSumSim(cObj* obj)
{
	if(!NbSubObjects)
		return(0.0);
	double Sum(0.0);
	RCursor<cObj> ptr(Owner->GetObjs(*this));
	for(ptr.Start();!ptr.End();ptr.Next())
	{
		if(ptr()==obj) continue;
		Sum+=Owner->Instance->GetSim(obj,ptr());
	}
	return(Sum);
}


//---------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroups>
	void RGroupSG<cGroup,cObj,cGroups>::ComputeCentroid(void)
{
	// If no objects -> No relevant one.
	if(!NbSubObjects)
	{
		Centroid=0;
		AvgIntraSim=0.0;
		return;
	}

	// Suppose the first element is the most relevant.
	RCursor<cObj> ptr(Owner->GetObjs(*this));
	ptr.Start();
	Centroid=(ptr());
	if(NbSubObjects==1)
	{
		AvgIntraSim=1.0;
		return;
	}
	AvgIntraSim=ComputeSumSim(ptr());

	// Look if in the other objects, there is a better one
	for(ptr.Next();!ptr.End();ptr.Next())
	{
		double SumSim(ComputeSumSim(ptr()));
		if(SumSim<AvgIntraSim)
		{
			Centroid=(ptr());
			AvgIntraSim=SumSim;
		}
	}
	AvgIntraSim/=static_cast<double>(NbSubObjects-1);
}


//---------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroups>
	void RGroupSG<cGroup,cObj,cGroups>::Evaluate(double& dist,double& agree,double& disagree)
{
	if(ToEval)
	{
		if(!Centroid)
			ComputeCentroid();
		RCursor<cObj> CurObj(Owner->GetObjs(*this));
		RCursor<cObj> CurObj2(Owner->GetObjs(*this));
		size_t i;
		for(CurObj.Start(),i=0,AvgAgreement=AvgDisagreement=0.0;i<NbSubObjects-1;CurObj.Next(),i++)
		{
			for(CurObj2.GoTo(i+1);!CurObj2.End();CurObj2.Next())
			{
				AvgAgreement+=Owner->Instance->GetAgreementRatio(CurObj(),CurObj2());
				AvgDisagreement+=Owner->Instance->GetDisagreementRatio(CurObj(),CurObj2());
			}
		}
		AvgAgreement/=static_cast<double>(NbSubObjects);
		AvgDisagreement/=static_cast<double>(NbSubObjects);
		ToEval=false;
	}
	dist+=AvgIntraSim;
	agree+=AvgAgreement;
	disagree+=AvgDisagreement;
}


//---------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroups>
	void RGroupSG<cGroup,cObj,cGroups>::SetCentroid(cObj* obj)
{
	Centroid=obj;
	ToEval=true;
}


//---------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroups>
	void RGroupSG<cGroup,cObj,cGroups>::CopyInfos(const cGroup* grp)
{
	AvgIntraSim=grp->AvgIntraSim;
	AvgAgreement=grp->AvgAgreement;
	AvgDisagreement=grp->AvgDisagreement;
	Centroid=grp->Centroid;
	ToEval=grp->ToEval;
}



//------------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroups>
RGroupSG<cGroup,cObj,cGroups>::OrderBySim::OrderBySim(cGroup* group,cObj** objs,size_t nb)
		: RQuickSort<cObj>(objs,nb), Group(group)
	{
	}

template<class cGroup,class cObj,class cGroups>
	int RGroupSG<cGroup,cObj,cGroups>::OrderBySim::Compare(cObj* obj1,cObj* obj2)
	{
		double af(Group->Owner->Instance->GetSim(obj1,Group->GetCentroid()));
		double bf(Group->Owner->Instance->GetSim(obj2,Group->GetCentroid()));

		if(fabs(af-bf)<0.000000001) return(0);
		if(af>bf)
			return(-1);
		else
			return(1);
	}


//------------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroups>
	bool RGroupSG<cGroup,cObj,cGroups>::Test(cObj** del,size_t& nbdel,cObj* obj1,cObj* obj2)
{
	cObj **cur1,**cur2,**cur3;
	size_t size1,size2,size3;

	// Fill thObjs2 with the objects of the group and order them by ascending order of similarity with the centroid
	cObj** thObjs2(Owner->thObjs2);
	RCursor<cObj> obj(GetObjs());
	for(obj.Start(),cur1=thObjs2,size1=0;!obj.End();obj.Next())
	{
		if(obj()==GetCentroid())
			continue;
		(*(cur1++))=obj();
		size1++;
	}
	OrderBySim QSort(static_cast<cGroup*>(this),thObjs2,size1);
	QSort.Order();


/*	size_t i;
	for(cur1=thObjs2,i=0;i<NbSubObjects;i++,cur1++)
		std::cout<<Owner->Instance->GetSim(*cur1,GetCentroid())<<std::endl;
	std::cout<<std::endl<<std::endl;
	return(false);*/

	// Compute the new average similarity with objects
	double NewAvgIntraSim(AvgIntraSim*(NbSubObjects-1));
	size_t NewNbObjs(NbSubObjects);
	NewAvgIntraSim+=Owner->Instance->GetSim(obj1,GetCentroid());
	if(obj2)
	{
		NewAvgIntraSim+=Owner->Instance->GetSim(obj2,GetCentroid());
		NewNbObjs++;
	}
	double NewBase(NewAvgIntraSim);
	NewAvgIntraSim/=static_cast<double>(NewNbObjs);

	// If the new average similarity is better than the old one -> Make the changes
	if(NewAvgIntraSim>AvgIntraSim)
		return(true);

	// Look if removing the most dissimilar objects (maximum 3) increases NewAvgIntraSim

	// Remove one ?
	for(size1=NbSubObjects,cur1=thObjs2;--size1;cur1++)
	{
		// Remove cur1?
		NewAvgIntraSim=(NewBase-Owner->Instance->GetSim(*cur1,GetCentroid()))/static_cast<double>(NewNbObjs-1);
		if(NewAvgIntraSim>AvgIntraSim)
		{
			// OK remove cur1
			del[nbdel++]=(*cur1);
			return(true);
		}
	}

	// Remove two ?
	if(NbSubObjects==2)
		return(false);
	for(size1=NbSubObjects,cur1=thObjs2;--size1;cur1++)
	{
		// Remove cur1?
		double tmpBase(NewBase-Owner->Instance->GetSim(*cur1,GetCentroid()));
		for(size2=NbSubObjects,cur2=thObjs2;--size2;cur2++)
		{
			if((*cur1)==(*cur2))
				continue;
			NewAvgIntraSim=(tmpBase-Owner->Instance->GetSim(*cur2,GetCentroid()))/static_cast<double>(NewNbObjs-2);
			if(NewAvgIntraSim>AvgIntraSim)
			{
				// OK remove cur1
				del[nbdel++]=(*cur1);
				del[nbdel++]=(*cur2);
				return(true);
			}
		}
	}

	// Remove three ?
	if(NbSubObjects==3)
		return(false);
	for(size1=NbSubObjects,cur1=thObjs2;--size1;cur1++)
	{
		// Remove cur1?
		double tmpBase(NewBase-Owner->Instance->GetSim(*cur1,GetCentroid()));
		for(size2=NbSubObjects,cur2=thObjs2;--size2;cur2++)
		{
			if((*cur1)==(*cur2))
				continue;
			double tmpBase2(tmpBase-Owner->Instance->GetSim(*cur2,GetCentroid()));
			for(size3=NbSubObjects,cur3=thObjs2;--size3;cur3++)
			{
				if(((*cur3)==(*cur1))||((*cur3)==(*cur2)))
					continue;
				NewAvgIntraSim=(tmpBase2-Owner->Instance->GetSim(*cur3,GetCentroid()))/static_cast<double>(NewNbObjs-3);
				if(NewAvgIntraSim>AvgIntraSim)
				{
					// OK remove cur1
					del[nbdel++]=(*cur1);
					del[nbdel++]=(*cur2);
					del[nbdel++]=(*cur3);
					return(true);
				}
			}
		}
	}

	// Nothing can be done
	return(false);
}


//------------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroups>
	bool RGroupSG<cGroup,cObj,cGroups>::DoOptimisation(cObj** objs,size_t& nbobjs)
{
	// The group must have more than one object
	if(NbSubObjects<2)
		return(false);

	size_t idx[2];           // Indexed of the objects to add in objs.
	cObj* del[3];            // Pointers to the objects to delete.
	cObj* add[2];            // Pointers to the objects to add.
	size_t nbdel;            // Number of objects to delete.
	size_t nbadd;            // Number of objects to add.
	cObj** obj;              // Current object eventually to add.
	cObj** obj2;             // Current object eventually to add.
	size_t i,j;

	// Init Part
	add[0]=add[1]=del[0]=del[1]=del[2]=0;   // No objects added or deleted
	idx[0]=idx[1]=cNoRef;
	nbdel=nbadd=0;

	// Try to add two objects
	for(i=0,obj=objs;i<nbobjs-1;obj++,i++)
	{
//		std::cout<<"   (1) "<<i<<" iterations"<<std::endl;
		for(j=i+1,obj2=obj+1;j<nbobjs;obj2++,j++)
		{
			if(Test(del,nbdel,(*obj),(*obj2)))
			{
				idx[nbadd]=i;
				add[nbadd++]=(*obj);
				idx[nbadd]=j;
				add[nbadd++]=(*obj2);
//				std::cout<<"         Insert "<<(*obj)->GetId()<<" and "<<(*obj2)->GetId()<<std::endl;
				break;
			}
		}
		if(nbadd) break;
	}

	// Try to add a first object
	if(!nbadd)
	{
		for(i=0,obj=objs;i<nbobjs;obj++,i++)
		{
//			std::cout<<"   (2) "<<i<<" iterations"<<std::endl;
			if(Test(del,nbdel,(*obj),0))
			{
				idx[nbadd]=i;
				add[nbadd++]=(*obj);
//				std::cout<<"         Insert "<<(*obj)->GetId()<<std::endl;
				break;
			}
		}
	}

	// If nothing to add -> return
	if(!nbadd) return(false);

	if((Owner->Instance->Debug)&&(nbdel||nbadd))
	{
		Owner->Instance->Debug->BeginFunc("DoOptimisation","RGroupSG");
		Owner->Instance->Debug->PrintInfo("Group "+RString::Number(Id)+": "+RString::Number(nbdel)+" objects replaced by "+RString::Number(nbadd));
		Owner->Instance->Debug->EndFunc("DoOptimisation","RGroupSG");
	}

	// Delete the objects from the group and insert them in objs.
	for(i=0;i<nbdel;i++)
	{
		Delete(del[i]);
		objs[nbobjs++]=del[i];
//		std::cout<<"         Delete "<<del[i]->GetId()<<std::endl;
	}

	// Insert the objects in the group and delete them from objs.
	// If idx[1] > idx[0] -> idx[1]-- because idx[0] will be deleted first.
	if(idx[1]>idx[0]) idx[1]--;
	for(i=0;i<nbadd;i++)
	{
		Insert(add[i]);
		memcpy(&objs[idx[i]],&objs[idx[i]+1],((--nbobjs)-idx[i])*sizeof(cObj*));
	}
	return(true);
}


//---------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroups>
	RGroupSG<cGroup,cObj,cGroups>::~RGroupSG(void)
{
}
