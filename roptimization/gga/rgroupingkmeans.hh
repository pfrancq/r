/*

	R Project Library

	RGroupingKMeans.hh

	Generic Heuristic for Grouping - Inline Implemenation

	Copyright 1998-2001 by the Université Libre de Bruxelles.

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
//
// class RGroupingKMeans<cGroup,cObj,cGroupData,cGroups>
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroupData,class cGroups>
	RGroupingKMeans<cGroup,cObj,cGroupData,cGroups>::RGroupingKMeans(RContainer<cObj,unsigned int,false,true>* objs)
{
	Rand= new RRandomGood(1);
	Rand->Reset(1);
	Objs=objs;
	Groups=0;

	protos = new RContainer<cObj,unsigned int,false,false>(10, 5);
	protoserror = new RContainer<cObj,unsigned int,false,false>(10, 5);
	grpstemp2 = new RContainer<cGroup,unsigned int,false,false>(10, 5);
	grpstemp = new RContainer<cGroup,unsigned int,false,false>(10, 5);
	grpsfinal = new RContainer<cGroup,unsigned int,false,false>(10, 5);
}



//------------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroupData,class cGroups>
	bool RGroupingKMeans<cGroup,cObj,cGroupData,cGroups>::IsValidProto(R::RContainer<cObj,unsigned int,false,false>* prototypes,cObj* obj) throw(std::bad_alloc)
{
	if (prototypes->GetPtr(obj))
		return(false);
	for (prototypes->Start(); !prototypes->End(); prototypes->Next())
	 {
		double sim=Similarity(obj, (*prototypes)());
		if ((sim<=1+Epsilon)&&(sim>=1-Epsilon))
			return(false);
	}
	return(true);
}


//------------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroupData,class cGroups>
	void RGroupingKMeans<cGroup,cObj,cGroupData,cGroups>::RefiningCenters(int nbsub, int level)
{
	RCursor<cObj, unsigned int> curs;
	RContainer<cObj,unsigned int,false,true>* tmpobjs;
	cGroup*g;
	double cost, subcostfunction;
	int r,finalcentersid;
	unsigned int nbsubprofpersubsamp = static_cast <int> (Objs->NbPtr*level/100);

	if (nbsubprofpersubsamp==0)
		cout << "---------------------->    WARNING !!! Number of elements per subsamples = 0 !!! "<<endl;
	RContainer<cGroup,unsigned int,true,true>* subsamples;
	subsamples=new RContainer<cGroup,unsigned int,true,true>(10,5);
	RContainer<cGroup,unsigned int,true,true>* initialcenters;
	initialcenters=new RContainer<cGroup,unsigned int,true,true>(10,5);
	RContainer<cObj, unsigned int, false,true>* allcenters;
	allcenters= new RContainer<cObj,unsigned int,false,true> (20,10);
	tmpobjs= new RContainer<cObj,unsigned int,false,true> (20,10);

	for (int k=0; k<nbsub; k++)
	{
		RContainer<cObj,unsigned int,true,true>* startingprotos ;
		startingprotos = new RContainer<cObj,unsigned int,true,true> (10,5);
		g=new cGroup(subsamples->NbPtr,0,0);
		curs=g->GetCursor();
		while (curs.GetNb()<nbsubprofpersubsamp)
		{
			r=int(Rand->Value(Objs->NbPtr));
			cObj* sub = Objs->GetPtrAt(r);
			g->InsertPtr(sub);
			curs=g->GetCursor();
			cout << curs.GetNb()<<" data set into sample"<<endl;
		}
		subsamples->InsertPtr(g);

		//init the clustering over the subsample
		curs=g->GetCursor();
		tmpobjs->Clear();
		for (curs.Start(); !curs.End(); curs.Next())
			tmpobjs->InsertPtr(curs());
		RandomInitObjects(tmpobjs, GroupsNumber);
		for (protos->Start(); !protos->End(); protos->Next())
			startingprotos->InsertPtr((*protos)());

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
		cGroup* centers= new cGroup(initialcenters->NbPtr,0,true);
		initialcenters->InsertPtr(centers);
		for (protos->Start(); !protos->End(); protos->Next())
		{
			centers->InsertPtr((*protos)());
			allcenters->InsertPtr((*protos)());
		}
	}

	// executes a KMeans over all the estimates of the centroids
	for (int i=0; i<nbsub; i++)
	{
		protos->Clear();
		cGroup* initialcenter = initialcenters->GetPtr(i);
		// initializing the proto with a set of found initial centers
		curs=initialcenter->GetCursor();
		for (curs.Start(); !curs.End(); curs.Next())
			protos->InsertPtr(curs());
		// excutes a kmeans over allcenters with this initial centers
		Execute(allcenters,true);
		// evaluate the distortion
		subcostfunction = Distortion(grpstemp2);
		if ((i==0)||(subcostfunction>cost))
		{
			cost = subcostfunction;
			finalcentersid=i;
		}
	}
	// set the final intial centers as prototypes.
	protos->Clear();
	g = initialcenters->GetPtr(finalcentersid);
	curs=g->GetCursor();
	for (curs.Start(); !curs.End(); curs.Next())
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
	RContainer<cObj,unsigned int,false,true>* wrongprotos;
	wrongprotos= new RContainer<cObj,unsigned int,false,true>(10,5);
	RCursor<cObj, unsigned int> curs;

	nbprotos=protos->NbPtr;
	// check for 'empty' clusters.
	for (grpstemp2->Start(); !grpstemp2->End(); grpstemp2->Next())
	{
		curs=(*grpstemp2)()->GetCursor();
		if (curs.GetNb()==1)
			wrongprotos->InsertPtr((*grpstemp2)()->RelevantObj());
	}

	for (wrongprotos->Start(); !wrongprotos->End(); wrongprotos->Next())
		protos->DeletePtr((*wrongprotos)());

	for (ptr=RandObjects, i=Objs->NbPtr; (protos->NbPtr<nbprotos)&&(i); ptr++,i--)
	{
		if (IsValidProto(protos, (*ptr)))
			protos->InsertPtr((*ptr));
	}
	if (protos->NbPtr!=nbprotos)
		throw RException("KMeans: VerifyKMeans : cannot find valid protos");

	return(wrongprotos->NbPtr==0);
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

	if (Objs->NbPtr==0) return ;

	//evaluate the number of groups
	EvaluateGroupsNumber();

	//temp   - display KmeansCosinus parameters
	DisplayInfos();

	for (GroupsNumber=MinNbGroups; GroupsNumber<(MaxNbGroups+1); GroupsNumber++)
	{
		cout <<" ------------------------------------------------------------------------ init " <<endl;
		if (initial==Refined)
			Init();
		cout <<" ------------------------------------------------------------------------Run " <<endl;
		Execute(Objs, NbTests);


		// evaluate the clustering / each clutering for each number of group
		double finalcost=CostFunction(grpstemp2)/protos->NbPtr;
//		double distortion=Distortion(grpstemp2);
		cout << "----------------->>>>>> "<<GroupsNumber<< " goupes -   final cost function: "<< finalcost<<endl;
//		cout << "----------------->>>>>> "<<GroupsNumber<< " goupes -   final distortion: "<< distortion<<endl;

		if ((GroupsNumber==MinNbGroups)||(finalcost<var))
		{
			var=finalcost;
			grnumber=GroupsNumber;
			grpsfinal->Clear();
			for (grpstemp2->Start(); !grpstemp2->End(); grpstemp2->Next())
				grpsfinal->InsertPtr((*grpstemp2)());
	 	}
	}
	GroupsNumber--;
	cout << "----------------->>>>>>end final cost function: "<<var<<endl;
	cout << "----------------->>>>>>end final nbgroup: "<< grnumber<<endl;
}


//------------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroupData,class cGroups>
	void RGroupingKMeans<cGroup,cObj,cGroupData,cGroups>::DisplayInfos(void)
{
	cout << " *** KMeansCos *** "<< endl;
	cout << "Number of Objects"<<Objs->NbPtr<<endl;
	cout << "nombre de test= "<<NbTests <<endl;
	cout << " parameters max iter:"<<IterNumber<< " nb tests: "<<NbTests<<endl<<endl;
	cout << "nombre de SubSamples= "<<NbSubSamples <<endl;
	cout << "taux des SubSamples= "<<SubSamplesRate <<endl;
	cout << "epsilon= "<<Epsilon <<endl;
	cout << "nombre de groupes "<<GroupsNumber<<endl;

}


//------------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroupData,class cGroups>
	void RGroupingKMeans<cGroup,cObj,cGroupData,cGroups>::RandomInitObjects(RContainer<cObj,unsigned int,false,true>* dataset, unsigned int nbgroups) throw(RException)
{
	cObj **ptr;
	unsigned int i;

	protos->Clear();

	// mix the dataset
	RandObjects=new cObj*[dataset->NbPtr];
	memcpy(RandObjects,dataset->Tab,sizeof(cObj*)*(dataset->NbPtr));
	Rand->RandOrder(RandObjects,dataset->NbPtr);

	for (ptr=RandObjects, i=dataset->NbPtr; (protos->NbPtr<nbgroups)&&(i); ptr++,i--)
	{
		cout <<" i="<<i<<endl;
		cObj* randomobj=(*ptr);
		 if (IsValidProto(protos, randomobj))
			protos->InsertPtr(randomobj);
	}
	cout <<" Random Init : "<<protos->NbPtr<< " centers found !"<<endl;
	if(nbgroups!=protos->NbPtr)
		throw RException("KMeans : Can not find valid protypes");
}


//------------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroupData,class cGroups>
	void RGroupingKMeans<cGroup,cObj,cGroupData,cGroups>::Execute(RContainer<cObj, unsigned int, false, true>* dataset, unsigned int nbtests)
{
	unsigned int iter, error;
	double cost, finalcost;
	grpstemp->Clear();
	srand((unsigned)time(NULL));   //enables the total random parameters

	// initialization
	for (unsigned int test=0; test<nbtests; test++)
	{
		cout <<" ********************* TEST "<<test<<"/"<<nbtests<<" ********************"<<endl;
		if (initial==Random)
			RandomInitObjects(Objs, GroupsNumber); // cycle
		iter=0;
		error=1;
		while ((iter<IterNumber)&&(error!=0))
		{
			cout << " running iteration "<<iter<<"/"<<IterNumber<<endl;
			grpstemp->Clear();
			ReAllocate(dataset);
			ReCenter();

			// error calculation
			error=CalcError();
			iter++;
		}
		cost=CostFunction(grpstemp)/protos->NbPtr;
		if ((test==0)||(cost< finalcost))
		{
			grpstemp2->Clear();
			for (grpstemp->Start();!grpstemp->End(); grpstemp->Next())
				grpstemp2->InsertPtr((*grpstemp)());
			finalcost=cost;
		}
	}
}


//------------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroupData,class cGroups>
	double RGroupingKMeans<cGroup,cObj,cGroupData,cGroups>::CostFunction(RContainer<cGroup,unsigned int,false,false>* grps)     // calculates the intra/min(inter)
{
	unsigned int i, j;
	double intra=0.0, mininter=2.0;
	cObj **s1, **s2;
	RCursor<cObj,unsigned int> curs;

	for (protos->Start(); !protos->End(); protos->Next())
	{
		cObj* proto = (*protos)();
		cGroup* gr=FindGroup(grps,proto);
		curs=gr->GetCursor();
		for (curs.Start(); !curs.End(); curs.Next())
		{
			double dist=(1.0)-Similarity(curs(),proto);
			intra+=(dist*dist);
		}
	}
	intra=intra/Objs->NbPtr;
//	cout << "nombre de protos ====== "<< protos->NbPtr<<endl;
	for(s1=protos->Tab,i=protos->NbPtr;--i;s1++)
	{
		for(s2=s1+1,j=i+1;--j;s2++)
		{
			double dist=(1.0)-Similarity(*s1,*s2);
			dist=dist*dist;
			if(dist<mininter)
				mininter=dist;
		}
	}

	return(intra/mininter);
}


//------------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroupData,class cGroups>
	void RGroupingKMeans<cGroup,cObj,cGroupData,cGroups>::ReAllocate(RContainer<cObj,unsigned int,false,true>* dataset)
{
	cout << " re-allocating"<<endl;
	int i=0;
	cGroup * g;
	for (protos->Start(); !protos->End(); protos->Next())  // groupscontaining one proto are created
	{
		g=new cGroup(i,0, true);
		g->InsertPtr((*protos)());
		grpstemp->InsertPtr(g);
		i++;
	}
	for (dataset->Start(); !dataset->End(); dataset->Next())
	{
		cObj* s=(*dataset)();
		if (protos->GetPtr(s)) continue;
		cObj* parent, *proto;
		protos->Start();
		double dist=Distance(s,(*protos)());
		parent=(*protos)();
		for (protos->Next(); !protos->End(); protos->Next())
		{
			proto=(*protos)();
			if (Distance(s,proto)<dist)
			{
				dist=Distance(s,proto);
				parent=proto;
			}
		}
		cGroup* gr=FindGroup(grpstemp, parent);
		gr->InsertPtr(s);
	}
	cout << " re-allocation done !"<<endl;

}


//------------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroupData,class cGroups>
	void RGroupingKMeans<cGroup,cObj,cGroupData,cGroups>::ReCenter(void)
{
	cout <<" Re centering..."<<endl;
	protoserror->Clear();
	for (grpstemp->Start(); !grpstemp->End(); grpstemp->Next())
	{
		cObj* tmp=(*grpstemp)()->RelevantObj();
		protoserror->InsertPtr(tmp);
	}
	cout <<" Re centering done !"<<endl;
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
	cGroup* RGroupingKMeans<cGroup,cObj,cGroupData,cGroups>::FindGroup(RContainer<cGroup,unsigned int,false,false>* grps,cObj* obj)
{
	for (grps->Start(); !grps->End(); grps->Next())
		if( (*grps)()->IsIn(obj))
			return((*grps)());
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
	int err=protos->NbPtr;
	for (protos->Start(); !protos->End(); protos->Next())
	{
		if (protoserror->GetPtr((*protos)()))
			err=err-1;
	}
	protos->Clear();
	for (protoserror->Start(); !protoserror->End(); protoserror->Next())
	{
		protos->InsertPtr((*protoserror)());
	}
	return(err);
}


//------------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroupData,class cGroups>
	double RGroupingKMeans<cGroup,cObj,cGroupData,cGroups>::Distortion(RContainer<cGroup,unsigned int,false,false>* grps)     // calculates the intra/min(inter)
{
	double var=0;
	RCursor<cObj, unsigned int> curs;


	// calculation of variance
	for (grps->Start(); !grps->End(); grps->Next())
	{
		curs=(*grps)()->GetCursor();
		cObj* proto= (*grps)()->RelevantObj();
		for (curs.Start(); !curs.End(); curs.Next())
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
