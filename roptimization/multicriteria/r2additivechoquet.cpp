/*

	R Project Library

	R2AdditiveChoquet.cpp

	The 2-additive Choquet Integral - Implementation.

	Copyright 2013 by Pascal Francq (pascal@francq.info).

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
// include files for R Project
#include <r2additivechoquet.h>
#include <rgenericmatrix.h>
using namespace std;
using namespace R;



//------------------------------------------------------------------------------
//
// class R2AdditiveChoquet
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
R2AdditiveChoquet::R2AdditiveChoquet(size_t n,const RString& name)
	: Name(name), NbCriteria(n), Params(n), Verified(false)
{
	if(!NbCriteria)
		throw std::range_error("R2AdditiveChoquet::R2AdditiveChoquet : Null number of criteria");
}


//------------------------------------------------------------------------------
R2AdditiveChoquet::R2AdditiveChoquet(size_t n,RSymmetricMatrix& params,const RString& name)
	: Name(name), NbCriteria(n),Params(n), Verified(false)
{
	if(!NbCriteria)
		throw std::range_error("R2AdditiveChoquet::R2AdditiveChoquet : Null number of criteria");
	SetParams(params);
}


//------------------------------------------------------------------------------
int R2AdditiveChoquet::Compare(const R2AdditiveChoquet& choquet) const
{
	return(Name.Compare(choquet.Name));
}


//------------------------------------------------------------------------------
int R2AdditiveChoquet::Compare(const RString& name) const
{
	return(Name.Compare(name));
}


//------------------------------------------------------------------------------
bool R2AdditiveChoquet::Verify(void)
{
	if(Verified)
		return(true);
	double Sum(0.0);
	for(size_t i=0;i<NbCriteria;i++)
		for(size_t j=i;j<NbCriteria;j++)
			Sum+=Params(i,j);
	Verified=fabs(Sum-1.0)<0.00000000001;
	return(Verified);
}


//------------------------------------------------------------------------------
void R2AdditiveChoquet::SetParams(RSymmetricMatrix& params)
{
	// Verify the matrix
	if(params.GetNbCols()!=NbCriteria)
		throw std::range_error("R2AdditiveChoquet::SetParams : The matrix is not of size ("+RString::Number(NbCriteria)+","+RString::Number(NbCriteria)+")");
	double Sum(0.0);
	for(size_t i=0;i<NbCriteria;i++)
		for(size_t j=i;j<NbCriteria;j++)
			Sum+=params(i,j);
	Verified=fabs(Sum-1.0)<0.00000000001;
	if(!Verified)
		throw std::invalid_argument("R2AdditiveChoquet::SetParams : The sum of the parameters is not 1");

	// Do the assignation
	Params=params;
}


//------------------------------------------------------------------------------
void R2AdditiveChoquet::SetCriteria(size_t i,double weight)
{
	if(i>=NbCriteria)
		throw std::range_error("R2AdditiveChoquet::SetCriteria : "+RString::Number(i)+" is not a valid criteria index");
	Params(i,i)=weight;
	Verified=false;
}


//------------------------------------------------------------------------------
double R2AdditiveChoquet::GetCriteria(size_t i) const
{
	if(i>=NbCriteria)
		throw std::range_error("R2AdditiveChoquet::GetCriteria : "+RString::Number(i)+" is not a valid criteria index");
	return(Params(i,i));
}


//------------------------------------------------------------------------------
void R2AdditiveChoquet::SetInteraction(size_t i,size_t j,double weight)
{
	if(i>=NbCriteria)
		throw std::range_error("R2AdditiveChoquet::SetInteraction : "+RString::Number(i)+" is not a valid criteria index");
	if(j>=NbCriteria)
		throw std::range_error("R2AdditiveChoquet::SetInteraction : "+RString::Number(j)+" is not a valid criteria index");
	if(i==j)
		throw std::range_error("R2AdditiveChoquet::SetInteraction : Different criteria indexes are needed");
	Params(i,j)=weight;
	Verified=false;
}


//------------------------------------------------------------------------------
double R2AdditiveChoquet::GetInteraction(size_t i,size_t j) const
{
	if(i>=NbCriteria)
		throw std::range_error("R2AdditiveChoquet::GetInteraction : "+RString::Number(i)+" is not a valid criteria index");
	if(j>=NbCriteria)
		throw std::range_error("R2AdditiveChoquet::GetInteraction : "+RString::Number(j)+" is not a valid criteria index");
	if(i==j)
		throw std::range_error("R2AdditiveChoquet::GetInteraction : Different criteria indexes are needed");
	return(Params(i,j));
}


//------------------------------------------------------------------------------
double R2AdditiveChoquet::Compute(RNumContainer<double,false>& obj)
{
	// Verifications
	if(!Verify())
		throw std::invalid_argument("R2AdditiveChoquet::SetParams : The sum of the parameters is not 1");
	if(obj.GetNb()!=NbCriteria)
		throw std::range_error("R2AdditiveChoquet::Compute : The vector has not a size of "+RString::Number(NbCriteria));
	double Choquet(0.0);

	// Go first through each interaction
	for(size_t i=0;i<NbCriteria-1;i++)
		for(size_t j=i+1;j<NbCriteria;j++)
	{
			if(Params(i,j)>0)
				Choquet+=Params(i,j)*fmin(obj[i],obj[j]);
			else
				Choquet+=fabs(Params(i,j))*fmax(obj[i],obj[j]);
	}

	// Go trough each criteria
	for(size_t i=0;i<NbCriteria;i++)
	{
		double tmp(0);
		for(size_t j=0;j<NbCriteria;j++)
			if(i!=j)
				tmp+=fabs(Params(i,j));
		Choquet+=(obj[i]*(Params(i,i)-0.5*tmp));
	}

	return(Choquet);
}

//------------------------------------------------------------------------------
R2AdditiveChoquet::~R2AdditiveChoquet(void)
{
}
