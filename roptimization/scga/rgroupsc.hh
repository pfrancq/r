/*

	R Project Library

	RGroupSC.hh

	Similarity-based Clustering Group - Implementation.

	Copyright 2002-2009 by the Université Libre de Bruxelles.

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



//-----------------------------------------------------------------------------
//
// class RGroupSC<cGroup,cObj,cGroups>
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroups>
	RGroupSC<cGroup,cObj,cGroups>::RGroupSC(RGroupSC* grp)
	: RGroup<cGroup,cObj,cGroups>(grp), AvgIntraSim(0.0), AvgAgreement(0.0), AvgDisagreement(0.0), Centroid(0), ToEval(true)
{
}


//-----------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroups>
	RGroupSC<cGroup,cObj,cGroups>::RGroupSC(cGroups* owner,const size_t id)
	: RGroup<cGroup,cObj,cGroups>(owner,id), AvgIntraSim(0.0), AvgAgreement(0.0), AvgDisagreement(0.0), Centroid(0), ToEval(true)
{
}


//---------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroups>
	void RGroupSC<cGroup,cObj,cGroups>::Clear(void)
{
	RGroup<cGroup,cObj,cGroups>::Clear();
	AvgIntraSim=AvgAgreement=AvgDisagreement=0.0;
	Centroid=0;
	ToEval=true;
}


//---------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroups>
	bool RGroupSC<cGroup,cObj,cGroups>::HasSameUser(const cObj* obj) const
{
	size_t usr=obj->GetParentId();
	if(!usr)
		return(false);
	RCursor<cObj> ptr(Owner->GetObjs(*this));
	for(ptr.Start();!ptr.End();ptr.Next())
		if(usr==ptr()->GetParentId())
			return(true);
	return(false);
}


//---------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroups>
	bool RGroupSC<cGroup,cObj,cGroups>::CanInsert(const cObj* obj)
{
	// If no objects -> OK
	if(!RGroup<cGroup,cObj,cGroups>::NbSubObjects)
		return(true);

	// Verify that no object of the group has the same parent of a disagreement ratios greater than the
	// maximum allowed.
	double tmp;
	size_t prof1(obj->GetElementId());
	size_t usr1(obj->GetParentId());
	RCursor<cObj> ptr(Owner->GetObjs(*this));
	for(ptr.Start(),LastMaxSim=-2.0;!ptr.End();ptr.Next())
	{
		// Do not put two objects with the same parent in the same group
		if(usr1&&(usr1==ptr()->GetParentId()))
			return(false);

		// Maximum disagreement must be respected
		if(Owner->Instance->GetDisagreementRatio(prof1,ptr()->GetElementId())>=Owner->Instance->Params->MinDisagreement)
		  	return(false);

		// Verify the minimum similarity and remember the best similarity
		tmp=Owner->Instance->GetSim(prof1,ptr()->GetElementId());
		if(tmp<=0)
			return(false);

		// Min similairty
		if(tmp<=Owner->Instance->Params->MinSimLevel)
			return(false);
		if(tmp>LastMaxSim)
			LastMaxSim=tmp;
	}

/*	if(LastMaxSim<=Owner->Instance->Params->MinSimLevel)
		return(false);*/

	// OK, it can be grouped.
	return(true);
}


//---------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroups>
	void RGroupSC<cGroup,cObj,cGroups>::PostInsert(const cObj* /*obj*/)
{
	ToEval=true;
	Centroid=0;
}


//---------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroups>
	void RGroupSC<cGroup,cObj,cGroups>::PostDelete(const cObj* /*obj*/)
{
	ToEval=true;
	Centroid=0;
}


//---------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroups>
	double RGroupSC<cGroup,cObj,cGroups>::ComputeSumSim(cObj* obj)
{
	if(!NbSubObjects)
		return(0.0);
	size_t prof(obj->GetElementId());
	double Sum(0.0);
	RCursor<cObj> ptr(Owner->GetObjs(*this));
	for(ptr.Start();!ptr.End();ptr.Next())
	{
		if(ptr()==obj) continue;
		Sum+=Owner->Instance->GetSim(prof,ptr()->GetElementId());
	}
	return(Sum);
}


//---------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroups>
	void RGroupSC<cGroup,cObj,cGroups>::ComputeCentroid(void)
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
	void RGroupSC<cGroup,cObj,cGroups>::Evaluate(double& dist,double& agree,double& disagree)
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
				AvgAgreement+=Owner->Instance->GetAgreementRatio(CurObj()->GetElementId(),CurObj2()->GetElementId());
				AvgDisagreement+=Owner->Instance->GetDisagreementRatio(CurObj()->GetElementId(),CurObj2()->GetElementId());
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
	void RGroupSC<cGroup,cObj,cGroups>::SetCentroid(cObj* obj)
{
	Centroid=obj;
	ToEval=true;
}


//---------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroups>
	RGroupSC<cGroup,cObj,cGroups>& RGroupSC<cGroup,cObj,cGroups>::operator=(const RGroupSC& grp)
{
	RGroup<cGroup,cObj,cGroups>::operator=(grp);
	AvgIntraSim=grp.AvgIntraSim;
	AvgAgreement=grp.AvgAgreement;
	AvgDisagreement=grp.AvgDisagreement;
	Centroid=grp.Centroid;
	ToEval=grp.ToEval;
	return(*this);
}


//---------------------------------------------------------------------------
/*
template<class cGroup,class cObj,class cGroups>
	double RGroupSC::GetMaxRatioSame(GCAObj* obj)
{
	double max,tmp;
	size_t prof=obj->GetElementId();

	// Look if in the other objects, there is a better one
	RCursor<GCAObj> ptr=Owner->GetObjs(*this);
	for(ptr.Start(),max=0.0;!ptr.End();ptr.Next())
	{
		tmp=Owner->Instance->GetAgreementRatio(prof,ptr()->GetElementId());
		if(tmp>max)
			max=tmp;
	}
	return(max);
}
*/


//---------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroups>
	RGroupSC<cGroup,cObj,cGroups>::~RGroupSC(void)
{
}
