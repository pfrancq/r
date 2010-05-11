/*

	R Project Library

	RKernelkMeans.hh

	Kernnel k-Means Algorithm - Inline Implemenation

	Copyright 2010 by Pascal Francq (pascal@francq.info).

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
// class RKernelkMeans<cGroup,cObj,cGroups>
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroups>
	R::RKernelkMeans<cGroup,cObj,cGroups>::RKernelkMeans(const R::RString& n,R::RRandom* r,R::RCursor<cObj> objs,size_t maxid,double alpha,double convergence,R::RDebug* debug)
	: Name(n), Debug(debug), Rand(r), MaxId(maxid), Objs(objs), CopyObjs(objs), ObjsUsed(0), Alpha(alpha), Convergence(convergence),
	  P(maxid+1,maxid+1), InvD(maxid+1), H(10,maxid+1), Y(10,maxid+1), Temp(maxid+1), WasInit(false)
{
	// Fill the structures with the objects
	cObj** ptr;
	ptr=ObjsUsed=new cObj*[Objs.GetNb()];
	for(Objs.Start();!Objs.End();Objs.Next())
		(*(ptr++))=Objs();

/*	{
		size_t i;
		cObj** ptr;

		std::cout<<"Verify ObjsUsed ";
		for(ptr=ObjsUsed,i=0;i<Objs.GetNb();i++,ptr++)
			std::cout<<(*ptr)->GetId()<<" ";
		std::cout<<"OK"<<std::endl;
	}*/
}


//-----------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroups>
	void R::RKernelkMeans<cGroup,cObj,cGroups>::Init(void)
{
	// Compute the P matrix and the D vector.
	FillSimilarities(P);  // Compute the similarities -> P;

//	P.Print("P");
	for(Objs.Start();!Objs.End();Objs.Next())
	{
		// Compute D.
		double sum(0.0);
		RSparseVector* Line(P[Objs()->GetId()]);
		RCursor<RValue> Val(*Line);
		for(Val.Start(),CopyObjs.Start();(!Val.End())&&(!CopyObjs.End());Val.Next()) // Val()->Id must be equal to CopyObjs()->GetId
		{
			if(Val()->Id!=CopyObjs()->GetId())
				continue;
			sum+=Val()->Value;
			CopyObjs.Next();
		}
		InvD[Objs()->GetId()]=1/sum;

		// Compute P : Dividing each element by sum
		for(Val.Start();!Val.End();Val.Next())
		{
			Val()->Value/=sum;
		}
	}

//	InvD.Print("InvD");
//	P.Print("P");
}


//------------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroups>
	void R::RKernelkMeans<cGroup,cObj,cGroups>::InitRandom(size_t nb)
{
	Out<<"  Init Random"<<std::endl;
	// Look first if they are not too much group ?
	while(Groups->Used.GetNb()>nb)
		Groups->ReleaseGroup(Groups->Used[Rand->GetValue(Groups->Used.GetNb())]);  // Remove randomly one group

	// Treat each object to fill the H matrix
	size_t i;
	cObj** ptr;
	for(i=0,ptr=ObjsUsed;(i<Objs.GetNb())&&(Groups->Used.GetNb()<nb);i++,ptr++)
	{
		if(Groups->GetGroup(*ptr))
			continue;

		// There are not enough groups -> create one
		Groups->ReserveGroup()->Insert(*ptr);
	}

	// Eventually re-affect objects to new groups
	for(i=0,ptr=ObjsUsed;(i<Objs.GetNb())&&(Groups->Used.GetNb()<nb);i++,ptr++)
	{
		// Delete an object of its group and assign it to a new one
		cGroup* Grp(Groups->GetGroup(*ptr));
		if(Grp->GetNbObjs()==1)
			continue;
		Grp->Delete(*ptr);
		Groups->ReserveGroup()->Insert(*ptr);
	}
	if(Groups->Used.GetNb()!=nb)
		ThrowRException("Cannot initialize the Kernel k-Means");

	// Assign randomly all the non assigned objects
/*	for(i=0,ptr=ObjsUsed;i<Objs.GetNb();i++,ptr++)
	{
		if(Groups->GetGroup(*ptr))
			continue;

		// There are not enough groups -> create one
		Groups->Used[Rand->GetValue(Groups->Used.GetNb())]->Insert(*ptr);
	}*/

	Out<<"  Number of groups created="+RString::Number(Groups->Used.GetNb())<<std::endl;
}


//-----------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroups>
	void R::RKernelkMeans<cGroup,cObj,cGroups>::CopyEinH(void)
{
	// Recompute H
	RCursor<cGroup> Group(Groups->Used);
	for(Group.Start();!Group.End();Group.Next())
	{
		// Compute hk
		RNumCursor<double> hk(H[Group()->GetId()]->GetCols());
		for(hk.Start(),Objs.Start();(!hk.End())&&(!Objs.End());hk.Next())  // Objs()->GetId must always be equal to hk.GetPos()
		{
			// Not a valid column ?
			if(Objs()->GetId()!=hk.GetPos())
				continue;  // -> treat next line

			// Find the group of the current object and look if it is the current group
			cGroup* Grp(Groups->GetGroup(Objs()));
			if(Grp==Group())
				hk()=1.0;
			else
				hk()=0.0;

			Objs.Next(); // Treat next object
		}
	}

	H.Print(Out,"H");
}


//-----------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroups>
	void R::RKernelkMeans<cGroup,cObj,cGroups>::UpdateH(void)
{
	// Recompute H
	RCursor<cGroup> Group(Groups->Used);
	for(Group.Start();!Group.End();Group.Next())
	{
		// Compute hk
		RNumCursor<double> hk(H[Group()->GetId()]->GetCols());
		for(hk.Start(),Objs.Start();(!hk.End())&&(!Objs.End());hk.Next())  // Objs()->GetId must always be equal to hk.GetPos()
		{
			// Not a valid column ?
			if(Objs()->GetId()!=hk.GetPos())
				continue;  // -> treat next line

			// Find the group of the current object and look if it is the current group
			cGroup* Grp(Groups->GetGroup(Objs()));
			if(Grp==Group())
				hk()=1.0/static_cast<double>(Grp->GetNbObjs());
			else
				hk()=0.0;

			Objs.Next(); // Treat next object
		}
	}

	H.Print(Out,"H");
}


//-----------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroups>
	void R::RKernelkMeans<cGroup,cObj,cGroups>::NormalizeH(void)
{
	RCursor<cGroup> Group(Groups->Used);
	for(Group.Start();!Group.End();Group.Next())
	{
		// Compute the sum of yk
		double Sum(0.0);
		RNumCursor<double> yk(Y[Group()->GetId()]->GetCols());
		for(yk.Start(),Objs.Start();(!yk.End())&&(!Objs.End());yk.Next())  // Objs()->GetId must always be equal to Yk.GetPos()
		{
			// Not a valid column ?
			if(Objs()->GetId()!=yk.GetPos())
				continue;  // -> treat next line
			Sum+=yk();
			Objs.Next(); // Treat next object
		}
		Sum=sqrt(Sum);

		// Compute hk by dividing it by
		RNumCursor<double> hk(H[Group()->GetId()]->GetCols());
		for(hk.Start(),yk.Start(),Objs.Start();(!hk.End())&&(!Objs.End());hk.Next(),yk.Next())  // Objs()->GetId must always be equal to hk.GetPos()
		{
			// Not a valid column ?
			if(Objs()->GetId()!=hk.GetPos())
				continue;  // -> treat next line

			hk()=yk()/Sum;
			Objs.Next(); // Treat next object
		}
	}

	H.Print(Out,"H");
}


//-----------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroups>
	void R::RKernelkMeans<cGroup,cObj,cGroups>::ComputeY(void)
{
	// Treat each group (line)
	RCursor<cGroup> Group(Groups->Used);
	for(Group.Start();!Group.End();Group.Next())
	{
		Out<<"    Compute y("+RString::Number(Group()->GetId())+")...";

		// Init yk(t) to zero
		RMatrixLine* hk(H[Group()->GetId()]);
		RMatrixLine* ykt(Y[Group()->GetId()]);
		RNumCursor<double> Cols(ykt->GetCols());

		// Compute first iteration
		for(Cols.Start(),Objs.Start();!Cols.End();Cols.Next())
		{
			// Not a valid column ?
			if(Objs()->GetId()!=Cols.GetPos())
				continue;  // -> treat next line
			Cols()=InvD[Objs()->GetId()]*(*hk)[Objs()->GetId()];
			Objs.Next(); // Treat next object
		}
//		Y.Print("Y"+RString::Number(Group()->GetId())+"(0)");

		// While |yk(t)-yk(t+1)|<Convergence for each object repeat
		size_t nb(1);
		double avg(1.0);
		for(;avg>Convergence;nb++)
		{
			avg=0;

			// Compute yk(t+1)
			RNumCursor<double> Cols2(Temp);
			for(Cols2.Start(),Objs.Start();(!Cols2.End())&&(!Objs.End());Cols2.Next())  // Objs()->GetId must always be equal to Cols2.GetPos()
			{
				// Not a valid column ?
				if(Objs()->GetId()!=Cols2.GetPos())
					continue;  // -> treat next line

				Cols2()=0;

				RCursor<RValue> Line1(*P[Objs()->GetId()]);
				for(Line1.Start(),Cols.Start(),CopyObjs.Start();(!Line1.End())&&(!CopyObjs.End());Line1.Next()) // Line1()->Id==Cols()->Id==CopyObjs()->GetId()
				{
					// If Line1()->Id<CopyObjs()->GetId() -> Next Line
					if(Line1()->Id<CopyObjs()->GetId())
						continue;

					// Line1 can be sparse -> Put CopyObjs and Cols to the same id
					while(Line1()->Id!=CopyObjs()->GetId())
						CopyObjs.Next();
					while(Line1()->Id!=Cols.GetPos())
						Cols.Next();
					Cols2()+=Line1()->Value*Cols();

					CopyObjs.Next(); // Treat next object
				}
				Cols2()=Alpha*Cols2()+InvD[Objs()->GetId()]*(*hk)[Objs()->GetId()];

				Objs.Next(); // Treat next object
			}

			// Copy yk(t+1) in yk(t) and looks if the relative error is greater than the convergence
			for(Cols.Start(),Cols2.Start(),Objs.Start();(!Cols.End())&&(!Objs.End());Cols.Next(),Cols2.Next()) // Objs()->GetId must always be equal to Cols2.GetPos()
			{
				// Not a valid column ?
				if(Objs()->GetId()!=Cols2.GetPos())
					continue;  // -> treat next line

				if(Cols()!=0.0)
					avg+=(fabs(Cols()-Cols2())/Cols());
				else
				{
					if(Cols2()!=0.0)
						avg=std::numeric_limits<double>::max();
				}
				Cols()=Cols2();
				Objs.Next(); // Treat next object
			}
//			Y.Print("Y"+RString::Number(Group()->GetId())+"("+RString::Number(nb)+")");
//			std::cout<<std::endl;
			avg/=Objs.GetNb();
//			std::cout<<nb<<": "<<avg<<std::endl;
		}
		Out<<"Iterations needed="+RString::Number(nb)<<std::endl;
	}
}


//-----------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroups>
	size_t R::RKernelkMeans<cGroup,cObj,cGroups>::ReAllocate(void)
{
	Out<<"   ReAllocate"<<std::endl;
	RCursor<cGroup> Group(Groups->Used);

	if(!Spherical)
	{
		// Compute hkT*yk and store it into Temp
		for(Group.Start();!Group.End();Group.Next())
		{
			double Sum(0.0);

			RNumCursor<double> Cols(Y[Group()->GetId()]->GetCols()); // Goes through Y
			RNumCursor<double> Cols2(H[Group()->GetId()]->GetCols()); // Goes through H
			for(Cols.Start(),Cols2.Start(),Objs.Start();(!Cols.End())&&(!Objs.End());Cols.Next(),Cols2.Next()) // Objs()->GetId must always be equal to Cols2.GetPos()
			{
				// Not a valid column ?
				if(Objs()->GetId()!=Cols2.GetPos())
					continue;  // -> treat next line
				if(Cols.GetPos()!=Cols2.GetPos())
					std::cerr<<"Big Problem"<<std::endl;

				Sum+=Cols()*Cols2();
				Objs.Next(); // Treat next object
			}
			Temp[Group()->GetId()]=Sum;
		}
//		Temp.Print(Out,"Temp");
	}

//	Y.Print(Out,"Y");

	// Go trough each object
	size_t count(0); // Suppose no object re-allocated
	FitnessValue=0;  // Fitness value is null
	for(Objs.Start();!Objs.End();Objs.Next())
	{
		cGroup* Best(0);
		double BestValue;

		// Go trough each group and look the one that minimize the expression
		for(Group.Start();!Group.End();Group.Next())
		{
			double Value;
			if(Spherical)
				Value=-Y(Group()->GetId(),Objs()->GetId());
			else
				Value=-2*Y(Group()->GetId(),Objs()->GetId())+Temp[Group()->GetId()];
			FitnessValue+=Value;
			if((!Best)||(Value<BestValue))
			{
				Best=Group();
				BestValue=Value;
			}
		}

		cGroup* Old(Groups->GetGroup(Objs()->GetId()));
		if(Old!=Best)
		{
			count++;
			if(Old)
				Old->Delete(Objs());
			Best->Insert(Objs());
		}
	}

	// Return the number of object re-allocated
	return(count);
}


//------------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroups>
	double R::RKernelkMeans<cGroup,cObj,cGroups>::Fitness(void)
{
	return(FitnessValue);
}


//------------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroups>
	void R::RKernelkMeans<cGroup,cObj,cGroups>::DokMeans(size_t max)
{
	double error(1.0);
	for(NbIterations=0;((!max)||(max&&(NbIterations<max)))&&(error>0.0001);NbIterations++)
	{
		Out<<"  Iteration..."+RString::Number(NbIterations)<<std::endl;
		if(Spherical)
			CopyEinH();
		else
			UpdateH();
		ComputeY();
		if(Spherical)
		{
			// Second iteration is necessary
			NormalizeH();
			ComputeY();
		}
		size_t Nb(ReAllocate());
		error=static_cast<double>(Nb)/static_cast<double>(Objs.GetNb());
		Out<<"  Re-allocation="+RString::Number(error*100.0)+"%, Fitness="+RString::Number(Fitness())<<std::endl;
	}
}


//------------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroups>
	void R::RKernelkMeans<cGroup,cObj,cGroups>::Run(cGroups* groups,size_t max,size_t nb,bool spherical)
{
	// Verify that some objects must be grouped
	FitnessValue=0;
	if(!Objs.GetNb())
		return;

	if(Objs.GetNb()<nb)
		ThrowRException("There are only "+R::RString::Number(Objs.GetNb())+" for "+R::RString::Number(nb)+" groups");

	// Initialize the algorithm
	Out.Open("/home/pfrancq/kmeans.txt",RIO::Create,"utf-8");
	Out<<"Compute P & D"<<std::endl;
	Spherical=spherical;
	H.VerifySize(nb,MaxId+1);    // Verify the size of M
	Y.VerifySize(nb,MaxId+1);    // Verify the size of Y
	if(!WasInit)
	{
		Init();
		WasInit=false;
	}

	// Initialize randomly
	Out<<"Init randomly the necessary group"<<std::endl;
	Groups=groups;
	Rand->RandOrder(ObjsUsed,Objs.GetNb());
	InitRandom(nb);

	if(Spherical)
		Out<<"Do Spherical Kernel kMeans"<<std::endl;
	else
		Out<<"Do Kernel kMeans"<<std::endl;
	DokMeans(max);
}


//------------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroups>
	R::RKernelkMeans<cGroup,cObj,cGroups>::~RGroupingSkMeans(void)
{
	if(ObjsUsed)
		delete[] ObjsUsed;
}
