/*

	R Project Library

	RGroupingKMeans.hh

	Generic Heuristic for Grouping - Inline Implemenation

	Copyright 1998-2001 by the Universit�Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).
		David Wartel (dwartel@ulb.ac.be).

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
// include ANSI/C++ files
#include <iostream>


//------------------------------------------------------------------------------
//
// class RGroupingKMeans<cGroup,cObj,cGroupData,cGroups>
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroupData,class cGroups>
	RGroupingKMeans<cGroup,cObj,cGroupData,cGroups>::RGroupingKMeans(RContainer<cObj,false,true>* objs)
{
	Rand= new RRandomGood(1);
	Rand->Reset(1);
	Objs=objs;
	Groups=0;

	protos = new RContainer<cObj,false,false>(10, 5);
	protoserror = new RContainer<cObj,false,false>(10, 5);
	grpstemp2 = new RContainer<cGroup,false,false>(10, 5);
	grpstemp = new RContainer<cGroup,false,false>(10, 5);
	grpsfinal = new RContainer<cGroup,false,false>(10, 5);
}



//------------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroupData,class cGroups>
	bool RGroupingKMeans<cGroup,cObj,cGroupData,cGroups>::IsValidProto(R::RContainer<cObj,false,false>* prototypes,cObj* obj) throw(std::bad_alloc)
{
	if(prototypes->GetPtr(obj))
		return(false);
	RCursor<cObj> Cur(*prototypes);
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		double sim=Similarity(obj, Cur());
		if ((sim<=1+Epsilon)&&(sim>=1-Epsilon))
			return(false);
	}
	return(true);
}


//------------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroupData,class cGroups>
	void RGroupingKMeans<cGroup,cObj,cGroupData,cGroups>::RefiningCenters(int nbsub, int level)
{
	RCursor<cObj> curs;
	RContainer<cObj,false,true>* tmpobjs;
	cGroup*g;
	double cost, subcostfunction;
	int r,finalcentersid;
	unsigned int nbsubprofpersubsamp = static_cast <int> (Objs->GetNb()*level/100);

	RContainer<cGroup,true,true>* subsamples;
	subsamples=new RContainer<cGroup,true,true>(10,5);
	RContainer<cGroup,true,true>* initialcenters;
	initialcenters=new RContainer<cGroup,true,true>(10,5);
	RContainer<cObj,false,true>* allcenters;
	allcenters= new RContainer<cObj,false,true> (20,10);
	tmpobjs= new RContainer<cObj,false,true> (20,10);

	for (int k=0; k<nbsub; k++)
	{
		RContainer<cObj,true,true>* startingprotos ;
		startingprotos = new RContainer<cObj,true,true> (10,5);
		g=new cGroup(subsamples->GetNb(),0,0);
		curs=g->GetCursor();
		while (curs.GetNb()<nbsubprofpersubsamp)
		{
			r=int(Rand->Value(Objs->GetNb()));
			cObj* sub = (*Objs)[r];
			g->InsertPtr(sub);
			curs=g->GetCursor();
		}
		subsamples->InsertPtr(g);

		//init the clustering over the subsample
		curs=g->GetCursor();
		tmpobjs->Clear();
		for(curs.Start(); !curs.End(); curs.Next())
			tmpobjs->InsertPtr(curs());
		RandomInitObjects(tmpobjs, GroupsNumber);
		RCursor<cObj> Cur2(*protos);
		for(Cur2.Start();!Cur2.End();Cur2.Next())
			startingprotos->InsertPtr(Cur2());

		// executes the clustering
		bool okkmeans=false;
		unsigned int kmeanstry=0;
		while(okkmeans==false&&kmeanstry<VerifyKMeansMaxIters)
		{
			Execute(tmpobjs, 1);
			okkmeans=VerifyKMeansMod();
			kmeanstry++;
		}
		// save the centers
		cGroup* centers= new cGroup(initialcenters->GetNb(),0,true);
		initialcenters->InsertPtr(centers);
		RCursor<cObj> Cur(*protos);
		for(Cur.Start();!Cur.End();Cur.Next())
		{
			centers->InsertPtr(Cur());
			allcenters->InsertPtr(Cur());
		}
	}

	// executes a KMeans over all the estimates of the centroids
	for (int i=0; i<nbsub; i++)
	{
		protos->Clear();
		cGroup* initialcenter = initialcenters->GetPtr(i);
		// initializing the proto with a set of found initial centers
		curs=initialcenter->GetCursor();
		for(curs.Start(); !curs.End(); curs.Next())
			protos->InsertPtr(curs());
		// excutes a kmeans over allcenters with this initial centers
		Execute(allcenters,true);
		// evaluate the distortion
		subcostfunction = Distortion(grpstemp2);
		if((i==0)||(subcostfunction>cost))
		{
			cost = subcostfunction;
			finalcentersid=i;
		}
	}
	// set the final intial centers as prototypes.
	protos->Clear();
	g = initialcenters->GetPtr(finalcentersid);
	curs=g->GetCursor();
	for(curs.Start();!curs.End();curs.Next())
	{
		protos->InsertPtr(curs());
	}
}


//------------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroupData,class cGroups>
	bool RGroupingKMeans<cGroup,cObj,cGroupData,cGroups>::VerifyKMeansMod(void) throw(RException)
{
	cObj** ptr;
	unsigned int i, nbprotos;
	RContainer<cObj,false,true>* wrongprotos;
	wrongprotos= new RContainer<cObj,false,true>(10,5);
	RCursor<cObj> curs;

	nbprotos=protos->GetNb();
	// check for 'empty' clusters.
	RCursor<cGroup> Cur(*grpstemp2);
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		curs=Cur()->GetCursor();
		if (curs.GetNb()==1)
			wrongprotos->InsertPtr(Cur()->RelevantObj());
	}

	RCursor<cObj> Cur2(*wrongprotos);
	for(Cur2.Start();!Cur2.End();Cur2.Next())
		protos->DeletePtr(Cur2());

	for(ptr=RandObjects, i=Objs->GetNb(); (protos->GetNb()<nbprotos)&&(i); ptr++,i--)
	{
		if(IsValidProto(protos, (*ptr)))
			protos->InsertPtr((*ptr));
	}
	if (protos->GetNb()!=nbprotos)
		throw RException("KMeans: VerifyKMeans : cannot find valid protos");

	return(wrongprotos->GetNb()==0);
}


//------------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroupData,class cGroups>
	void RGroupingKMeans<cGroup,cObj,cGroupData,cGroups>::EvaluateGroupsNumber(void)
{
	MinNbGroups=GroupsNumber;
	MaxNbGroups=GroupsNumber;
}


//------------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroupData,class cGroups>
	void RGroupingKMeans<cGroup,cObj,cGroupData,cGroups>::Run(void)
{
	int grnumber;
	double var;

	if (Objs->GetNb()==0) return ;

	//evaluate the number of groups
	EvaluateGroupsNumber();

	//temp   - display KmeansCosinus parameters
	DisplayInfos();

	for (GroupsNumber=MinNbGroups; GroupsNumber<(MaxNbGroups+1); GroupsNumber++)
	{
		if (initial==Refined)
			Init();
		Execute(Objs, NbTests);


		// evaluate the clustering / each clutering for each number of group
		double finalcost=CostFunction(grpstemp2)/protos->GetNb();

		if ((GroupsNumber==MinNbGroups)||(finalcost<var))
		{
			var=finalcost;
			grnumber=GroupsNumber;
			grpsfinal->Clear();
			RCursor<cGroup> Cur(*grpstemp2);
			for(Cur.Start();!Cur.End();Cur.Next())
				grpsfinal->InsertPtr(Cur());
	 	}
	}
	GroupsNumber--;
}


//------------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroupData,class cGroups>
	void RGroupingKMeans<cGroup,cObj,cGroupData,cGroups>::DisplayInfos(void)
{
	std::cout<<" *** KMeansCos *** "<<std::endl;
	std::cout<<"Number of Objects "<<Objs->GetNb()<<std::endl;
	std::cout<<"Number of tests "<<NbTests<<std::endl;
	std::cout<<"Parameters: max iter="<<IterNumber<<" ; nb tests="<<NbTests<<std::endl<<std::endl;
	std::cout<<"Number of SubSamples= "<<NbSubSamples<<std::endl;
	std::cout<<"Rate of SubSamples= "<<SubSamplesRate<<std::endl;
	std::cout<<"Epsilon="<<Epsilon<<std::endl;
	std::cout<<"Number of groups "<<GroupsNumber<<std::endl;

}


//------------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroupData,class cGroups>
	void RGroupingKMeans<cGroup,cObj,cGroupData,cGroups>::RandomInitObjects(RContainer<cObj,false,true>* dataset, unsigned int nbgroups) throw(RException)
{
	cObj **ptr;
	unsigned int i;

	protos->Clear();

	// mix the dataset
	RandObjects=new cObj*[dataset->GetNb()];
	dataset->GetTab(RandObjects);

	Rand->RandOrder(RandObjects,dataset->GetNb());

	for (ptr=RandObjects, i=dataset->GetNb(); (protos->GetNb()<nbgroups)&&(i); ptr++,i--)
	{
		cObj* randomobj=(*ptr);
		 if (IsValidProto(protos, randomobj))
			protos->InsertPtr(randomobj);
	}
	if(nbgroups!=protos->GetNb())
		throw RException("KMeans : Can not find valid protypes");
}


//------------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroupData,class cGroups>
	void RGroupingKMeans<cGroup,cObj,cGroupData,cGroups>::Execute(RContainer<cObj,false,true>* dataset, unsigned int nbtests)
{
	unsigned int iter, error;
	double cost, finalcost;
	grpstemp->Clear();
	srand((unsigned)time(NULL));   //enables the total random parameters

	// initialization
	for (unsigned int test=0; test<nbtests; test++)
	{
		if (initial==Random)
			RandomInitObjects(Objs, GroupsNumber); // cycle
		iter=0;
		error=1;
		while ((iter<IterNumber)&&(error!=0))
		{
			grpstemp->Clear();
			ReAllocate(dataset);
			ReCenter();

			// error calculation
			error=CalcError();
			iter++;
		}
		cost=CostFunction(grpstemp)/protos->GetNb();
		if ((test==0)||(cost< finalcost))
		{
			grpstemp2->Clear();
			RCursor<cGroup> Cur(*grpstemp);
			for(Cur.Start();!Cur.End();Cur.Next())
				grpstemp2->InsertPtr(Cur());
			finalcost=cost;
		}
	}
}


//------------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroupData,class cGroups>
	double RGroupingKMeans<cGroup,cObj,cGroupData,cGroups>::CostFunction(RContainer<cGroup,false,false>* grps)     // calculates the intra/min(inter)
{
	unsigned int i;
	double intra=0.0, mininter=2.0;
	RCursor<cObj> curs;

	RCursor<cObj> Cur(*protos);
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		cObj* proto = Cur();
		cGroup* gr=FindGroup(grps,proto);
		curs=gr->GetCursor();
		for (curs.Start(); !curs.End(); curs.Next())
		{
			double dist=(1.0)-Similarity(curs(),proto);
			intra+=(dist*dist);
		}
	}
	intra=intra/Objs->GetNb();
//	cout << "nombre de protos ====== "<< protos->NbPtr<<endl;
	RCursor<cObj> Cur2(*protos);
	for(Cur.Start(),i=0;i<protos->GetNb();Cur.Next())
	{
		for(Cur2.GoTo(i+1);!Cur2.End();Cur2.Next())
		{
			double dist=(1.0)-Similarity(Cur(),Cur2());
			dist=dist*dist;
			if(dist<mininter)
				mininter=dist;
		}
	}

	return(intra/mininter);
}


//------------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroupData,class cGroups>
	void RGroupingKMeans<cGroup,cObj,cGroupData,cGroups>::ReAllocate(RContainer<cObj,false,true>* dataset)
{
	int i=0;
	cGroup * g;
	RCursor<cObj> Cur(*protos);
	for(Cur.Start();!Cur.End();Cur.Next())            // groupscontaining one proto are created
	{
		g=new cGroup(i,0, true);
		g->InsertPtr(Cur());
		grpstemp->InsertPtr(g);
		i++;
	}
	RCursor<cObj> Cur2(*dataset);
	for(Cur2.Start();!Cur2.End();Cur2.Next())
	{
		cObj* s=Cur2();
		if (protos->GetPtr(s)) continue;
		cObj* parent, *proto;
		Cur.Start();
		double dist=Distance(s,Cur());
		parent=Cur();
		for(Cur.Next();!Cur.End();Cur.Next())
		{
			proto=Cur();
			if (Distance(s,proto)<dist)
			{
				dist=Distance(s,proto);
				parent=proto;
			}
		}
		cGroup* gr=FindGroup(grpstemp, parent);
		gr->InsertPtr(s);
	}

}


//------------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroupData,class cGroups>
	void RGroupingKMeans<cGroup,cObj,cGroupData,cGroups>::ReCenter(void)
{
	protoserror->Clear();
	RCursor<cGroup> Cur(*grpstemp);
	for(Cur.Start();!Cur.End();Cur.Next())
		protoserror->InsertPtr(Cur()->RelevantObj());
}


//------------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroupData,class cGroups>
	double RGroupingKMeans<cGroup,cObj,cGroupData,cGroups>::Distance(cObj* obj, cObj* proto)
{
	double dist;
	cGroup* gr;
	gr=FindGroup(grpstemp, proto);
	dist=(1.0-Similarity(obj,proto));

	return(dist);
}


//------------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroupData,class cGroups>
	cGroup* RGroupingKMeans<cGroup,cObj,cGroupData,cGroups>::FindGroup(RContainer<cGroup,false,false>* grps,cObj* obj)
{
	RCursor<cGroup> Cur(*grps);
	for(Cur.Start();!Cur.End();Cur.Next())
		if(Cur()->IsIn(obj))
			return(Cur());
	return(0);
}


//------------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroupData,class cGroups>
	void  RGroupingKMeans<cGroup,cObj,cGroupData,cGroups>::Init(void)
{
	RefiningCenters(NbSubSamples,SubSamplesRate);
}


//------------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroupData,class cGroups>
	int RGroupingKMeans<cGroup,cObj,cGroupData,cGroups>::CalcError(void)
{
	int err=protos->GetNb();
	RCursor<cObj> Cur(*protos);
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		if(protoserror->GetPtr(Cur()))
			err=err-1;
	}
	protos->Clear();
	RCursor<cObj> Cur2(*protoserror);
	for(Cur2.Start();!Cur2.End();Cur2.Next())
	{
		protos->InsertPtr(Cur2());
	}
	return(err);
}


//------------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroupData,class cGroups>
	double RGroupingKMeans<cGroup,cObj,cGroupData,cGroups>::Distortion(RContainer<cGroup,false,false>* grps)     // calculates the intra/min(inter)
{
	double var=0;
	RCursor<cObj> curs;


	// calculation of variance
	RCursor<cGroup> Cur(*grps);
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		curs=Cur()->GetCursor();
		cObj* proto= Cur()->RelevantObj();
		for(curs.Start(); !curs.End(); curs.Next())
			var+=Similarity(curs(), proto);
	}

	return(var);
}


//------------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroupData,class cGroups>
	double RGroupingKMeans<cGroup,cObj,cGroupData,cGroups>::GroupVariance(cGroup* gr)     // calculates the intra/min(inter)
{
	double variance=0.0;
	unsigned int i;
	cObj **s1;

	cObj* center=gr->RelevantObj();
	for (s1=gr->Tab, i=gr->NbPtr; --i;s1++)
		variance+=(1-Similarity(center, (*s1)))*(1-Similarity(center, (*s1)));
	return(variance/gr->NbPtr);
}


//------------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroupData,class cGroups>
	double RGroupingKMeans<cGroup,cObj,cGroupData,cGroups>::Similarity(cObj* obj1, cObj* obj2)
{
	return (obj1->Similarity(obj2));
}


//------------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroupData,class cGroups>
	RGroupingKMeans<cGroup,cObj,cGroupData,cGroups>::~RGroupingKMeans(void)
{
	if(protos) delete protos;
	if(protoserror) delete protoserror;
	if(grpstemp) delete grpstemp;
	if(grpstemp2) delete grpstemp2;
	if(grpsfinal) delete grpsfinal;
}
