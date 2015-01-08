/*

	R Project Library

	RKernelkMeans.hh

	Kernel k-Means Algorithm - Inline Implemenation

	Copyright 2010-2015 by Pascal Francq (pascal@francq.info).

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
	RKernelkMeans<cGroup,cObj,cGroups>::RKernelkMeans(const RString& n,RRandom& r,RCursor<cObj> objs,size_t maxid,double alpha,double convergence,RDebug* debug)
	: Name(n), Debug(debug), Rand(r), MaxId(maxid), Objs(objs), CopyObjs(objs), ObjsUsed(0), Alpha(alpha), Convergence(convergence),
	  P(maxid+1,maxid+1), InvD(maxid+1), H(10,maxid+1), Y(10,maxid+1), Assignments(maxid+1), Temp(maxid+1), WasInit(false),
	  PrintOut(false), OutName("/home/pfrancq/kmeans.txt")
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

//	P.Print(OutName,"P");
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

		// Compute P : Dividing each element by the sum
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
	if(PrintOut)
		Out<<"  Init Random"<<std::endl;

	// Look first if they are not too much group ?
	while(Groups->Used.GetNb()>nb)
		Groups->ReleaseGroup(Groups->Used[Rand.GetValue(Groups->Used.GetNb())]);  // Remove randomly one group

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
		mThrowRException("Cannot initialize the Kernel k-Means");

	// Assign randomly all the non assigned objects
/*	for(i=0,ptr=ObjsUsed;i<Objs.GetNb();i++,ptr++)
	{
		if(Groups->GetGroup(*ptr))
			continue;

		// There are not enough groups -> create one
		Groups->Used[Rand->GetValue(Groups->Used.GetNb())]->Insert(*ptr);
	}*/

	if(PrintOut)
		Out<<"  Number of groups created="+RString::Number(Groups->Used.GetNb())<<std::endl;
}


//-----------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroups>
	void R::RKernelkMeans<cGroup,cObj,cGroups>::UpdateBinaryH(bool center)
{
	// Recompute H
	RCursor<cGroup> Group(Groups->Used);
	for(Group.Start();!Group.End();Group.Next())
	{
		double Avg(0.0);  // Average value
		size_t Nb(0);     // Number of elements

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
			{
				hk()=1.0;
				Avg+=1.0;
			}
			else
				hk()=0.0;

			Nb++;
			Objs.Next(); // Treat next object
		}

		// If not the matrix must be centered goes to the group
		if(!center)
			continue;

		// Center hk
		Avg/=static_cast<double>(Nb);
		for(hk.Start(),Objs.Start();(!hk.End())&&(!Objs.End());hk.Next())  // Objs()->GetId must always be equal to hk.GetPos()
		{
			// Not a valid column ?
			if(Objs()->GetId()!=hk.GetPos())
				continue;  // -> treat next line
			hk()-=Avg;
			Objs.Next(); // Treat next object
		}
	}

	if(PrintOut)
		H.Print(Out,"H");
}


//-----------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroups>
	void R::RKernelkMeans<cGroup,cObj,cGroups>::UpdateH(bool center)
{
	// Recompute H
	RCursor<cGroup> Group(Groups->Used);
	for(Group.Start();!Group.End();Group.Next())
	{
		double Avg(0.0);  // Average value
		size_t Nb(0);     // Number of elements

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
			{
				hk()=1.0/static_cast<double>(Grp->GetNbObjs());
				Avg+=hk();
			}
			else
				hk()=0.0;

			Nb++;
			Objs.Next(); // Treat next object
		}

		// If not the matrix must be centered goes to the group
		if(!center)
			continue;

		// Center hk
		Avg/=static_cast<double>(Nb);
		for(hk.Start(),Objs.Start();(!hk.End())&&(!Objs.End());hk.Next())  // Objs()->GetId must always be equal to hk.GetPos()
		{
			// Not a valid column ?
			if(Objs()->GetId()!=hk.GetPos())
				continue;  // -> treat next line
			hk()-=Avg;
			Objs.Next(); // Treat next object
		}
	}

	if(PrintOut)
		H.Print(Out,"H");
}


//-----------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroups>
	void R::RKernelkMeans<cGroup,cObj,cGroups>::NormalizeH(bool center)
{
	if(PrintOut)
		Out<<"   Normalize H"<<std::endl;

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

			// Sum only the elements that are part of group k
			if(Group()->IsIn(Objs()->GetId()))
				Sum+=yk();

			Objs.Next(); // Treat next object
		}
		Sum=sqrt(Sum);

		double Avg(0.0);  // Average value
		size_t Nb(0);     // Number of elements

		// Compute hk by dividing it by
		RNumCursor<double> hk(H[Group()->GetId()]->GetCols());
		for(hk.Start(),yk.Start(),Objs.Start();(!hk.End())&&(!Objs.End());hk.Next(),yk.Next())  // Objs()->GetId must always be equal to hk.GetPos()
		{
			// Not a valid column ?
			if(Objs()->GetId()!=hk.GetPos())
				continue;  // -> treat next line

			hk()=yk()/Sum;
			Avg+=hk();
			Nb++;
			Objs.Next(); // Treat next object
		}

		// If not the matrix must be centered goes to the group
		if(!center)
			continue;

		// Center hk
		Avg/=static_cast<double>(Nb);
		for(hk.Start(),Objs.Start();(!hk.End())&&(!Objs.End());hk.Next())  // Objs()->GetId must always be equal to hk.GetPos()
		{
			// Not a valid column ?
			if(Objs()->GetId()!=hk.GetPos())
				continue;  // -> treat next line
			hk()-=Avg;
			Objs.Next(); // Treat next object
		}
	}

	if(PrintOut)
		H.Print(Out,"H");
}


//-----------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroups>
	void R::RKernelkMeans<cGroup,cObj,cGroups>::ComputeY(bool center)
{
	// Treat each group (line)
	RCursor<cGroup> Group(Groups->Used);
	for(Group.Start();!Group.End();Group.Next())
	{
		// Init yk(t) to zero
		RVector* hk(H[Group()->GetId()]);
		RVector* ykt(Y[Group()->GetId()]);
		RNumCursor<double> ykt_i(ykt->GetCols());

		// Compute first iteration : yk(t)=(1/D)*hk
		for(ykt_i.Start(),Objs.Start();(!ykt_i.End())&&(!Objs.End());ykt_i.Next())
		{
			// Not a valid column ?
			if(Objs()->GetId()!=ykt_i.GetPos())
				continue;  // -> treat next line
			ykt_i()=InvD[Objs()->GetId()]*(*hk)[Objs()->GetId()];
			Objs.Next(); // Treat next object
		}
//		Y.Print("Y"+RString::Number(Group()->GetId())+"(0)");

		// While |yk(t)-yk(t+1)|<Convergence for each object repeat
		size_t nb(1);
		double avg(1.0);
		for(;avg>Convergence;nb++)
		{
			avg=0;

			// Compute yk(t')
			RNumCursor<double> yktprim_i(Temp.GetCols());
			for(yktprim_i.Start(),Objs.Start();(!yktprim_i.End())&&(!Objs.End());yktprim_i.Next())  // Objs()->GetId must always be equal to yktprim_i.GetPos()
			{
				// Not a valid column ?
				if(Objs()->GetId()!=yktprim_i.GetPos())
					continue;  // -> treat next line

				yktprim_i()=0;

				// Compute yk(t')=P*yk(t)
				RCursor<RValue> P_i(*P[Objs()->GetId()]);
				for(P_i.Start(),ykt_i.Start(),CopyObjs.Start();(!P_i.End())&&(!CopyObjs.End());P_i.Next()) // Line1()->Id==Cols()->Id==CopyObjs()->GetId()
				{
					// If Line1()->Id<CopyObjs()->GetId() -> Next Line
					if(P_i()->Id<CopyObjs()->GetId())
						continue;

					// P_i can be sparse -> Put CopyObjs and yk(t) to the same id
					while(P_i()->Id!=CopyObjs()->GetId())
						CopyObjs.Next();
					while(P_i()->Id!=ykt_i.GetPos())
						ykt_i.Next();

					yktprim_i()+=P_i()->Value*ykt_i();

					CopyObjs.Next(); // Treat next object
				}
				yktprim_i()=Alpha*yktprim_i()+InvD[Objs()->GetId()]*(*hk)[Objs()->GetId()];

				Objs.Next(); // Treat next object
			}

			// Copy yk(t') in yk(t) and looks if the relative error is greater than the convergence
			size_t Nb(0);
			for(ykt_i.Start(),yktprim_i.Start(),Objs.Start();(!ykt_i.End())&&(!Objs.End());ykt_i.Next(),yktprim_i.Next()) // Objs()->GetId must always be equal to yktprim_i.GetPos()
			{
				// Not a valid column ?
				if(Objs()->GetId()!=yktprim_i.GetPos())
					continue;  // -> treat next line

				if(ykt_i()!=0.0)
					avg+=(fabs(ykt_i()-yktprim_i())/ykt_i());
				else
				{
					if(yktprim_i()!=0.0)
						avg=std::numeric_limits<double>::max();
				}
				ykt_i()=yktprim_i();
				Nb++;
				Objs.Next(); // Treat next object
			}
//			Y.Print("Y"+RString::Number(Group()->GetId())+"("+RString::Number(nb)+")");
//			std::cout<<std::endl;
			avg/=static_cast<double>(Nb);
//			std::cout<<nb<<": "<<avg<<std::endl;
		}

		// If not the matrix must be centered goes to the group
		if(!center)
		{
			if(PrintOut)
				Out<<"Iterations needed="+RString::Number(nb)<<std::endl;
			continue;
		}

		// Center yk
		double Avg(0.0);  // Average value
		size_t Nb(0);     // Number of elements
		for(ykt_i.Start(),Objs.Start();(!ykt_i.End())&&(!Objs.End());ykt_i.Next())
		{
			// Not a valid column ?
			if(Objs()->GetId()!=ykt_i.GetPos())
				continue;  // -> treat next line
			Avg+=ykt_i();
			Nb++;
			Objs.Next(); // Treat next object
		}
		Avg/=static_cast<double>(Nb);
		for(ykt_i.Start(),Objs.Start();(!ykt_i.End())&&(!Objs.End());ykt_i.Next())
		{
			// Not a valid column ?
			if(Objs()->GetId()!=ykt_i.GetPos())
				continue;  // -> treat next line
			ykt_i()-=Avg;
			Objs.Next(); // Treat next object
		}

		if(PrintOut)
			Out<<"Iterations needed="+RString::Number(nb)<<std::endl;
	}

	if(PrintOut)
		Y.Print(Out,"Y");
}


//-----------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroups>
	size_t R::RKernelkMeans<cGroup,cObj,cGroups>::ReAllocate(void)
{
	if(PrintOut)
		Out<<"   ReAllocate"<<std::endl;
	RCursor<cGroup> Group(Groups->Used);

	if(!Spherical)
	{
		// Compute hkT*yk and store it into Temp
		for(Group.Start();!Group.End();Group.Next())
		{
			double Sum(0.0);

			RNumCursor<double> yi(Y[Group()->GetId()]->GetCols()); // Goes through Y
			RNumCursor<double> hi(H[Group()->GetId()]->GetCols()); // Goes through H
			for(yi.Start(),hi.Start(),Objs.Start();(!yi.End())&&(!Objs.End());yi.Next(),hi.Next()) // Objs()->GetId must always be equal to hi.GetPos()
			{
				// Not a valid column ?
				if(Objs()->GetId()!=hi.GetPos())
					continue;  // -> treat next line
				if(yi.GetPos()!=hi.GetPos())
					std::cerr<<"Big Problem"<<std::endl;

				Sum+=yi()*hi();
				Objs.Next(); // Treat next object
			}
			Temp[Group()->GetId()]=Sum;
		}
		if(PrintOut)
			Temp.Print(Out,"Temp");
	}

	if(PrintOut)
		Y.Print(Out,"Y");

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
			if((!Best)||((!Spherical)&&(Value<BestValue))||((Spherical)&&(Value>BestValue)))
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
	if(PrintOut)
		Out<<count<<"      objects re-allocated"<<std::endl;
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
		if(PrintOut)
			Out<<"  Iteration..."+RString::Number(NbIterations)<<std::endl;
		if(Spherical)
			UpdateBinaryH();
		else
			UpdateH();
		ComputeY();
		if(Spherical)
		{
			//Nb=ReAllocate();

			// Second iteration is necessary
			NormalizeH();
			ComputeY();
		}
		size_t Nb(ReAllocate()); // Allocate the elements
		error=static_cast<double>(Nb)/static_cast<double>(Objs.GetNb());
		if(PrintOut)
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
		mThrowRException("There are only "+R::RString::Number(Objs.GetNb())+" for "+R::RString::Number(nb)+" groups");

	// Initialize the algorithm
	if(PrintOut)
	{
		Out.Open(OutName,RIO::Create,"utf-8");
		Out<<"Compute P & D"<<std::endl;
	}
	Spherical=spherical;
	H.VerifySize(nb,MaxId+1);    // Verify the size of M
	Y.VerifySize(nb,MaxId+1);    // Verify the size of Y
	if(!WasInit)
	{
		Init();
		WasInit=true;
	}

	// Initialize randomly
	if(PrintOut)
		Out<<"Init randomly the necessary group"<<std::endl;
	Groups=groups;
	Rand.RandOrder(ObjsUsed,Objs.GetNb());
	InitRandom(nb);

	if(PrintOut)
	{
		if(Spherical)
			Out<<"Do Spherical Kernel kMeans"<<std::endl;
		else
			Out<<"Do Kernel kMeans"<<std::endl;
	}
	DokMeans(max);
}


//------------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroups>
	R::RKernelkMeans<cGroup,cObj,cGroups>::~RKernelkMeans(void)
{
	if(ObjsUsed)
		delete[] ObjsUsed;
}
