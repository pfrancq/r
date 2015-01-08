/*

	R Project Library

	Random.cpp

	Class representing random number generators. Implementation

	Copyright 1999-2015 by Pascal Francq (pascal@francq.info).
	Copyright 1999-2008 by the Université Libre de Bruxelles (ULB).

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
// include files for ANSI C/C++
#include <limits.h>
#include <iostream>


//------------------------------------------------------------------------------
// include files for R Project
#include <rrandom.h>
using namespace R;


//-----------------------------------------------------------------------------
// General Variables
static const int a=16807;
static const int q=INT_MAX/a;                //  cf limits.h; INT_MAX == LONG_MAX in C++
static const int r=INT_MAX%a;
static const double Minv=1.0/INT_MAX;
static const int NDIV=1+((INT_MAX-1)/32);
static const double MaxR=1.0-1.2e-7;         //  maximum value to return
static const int M1=2147483563;
static const int a1=16807;
static const int q1=M1/a1;
static const int r1=M1%a1;
static const int M2=2147483399;
static const int a2=16807;
static const int q2=M2/a2;
static const int r2=M2%a2;
static const int NDIVB=1+((M1-1)/32);
static const double MinvB=1.0/M1;
static const int MASK=123456987;



//------------------------------------------------------------------------------
//
// RRandom
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
RRandom::RRandom(tRandom type,const int seed)
	: Type(type), Seed(seed)
{
	Reset(seed);
}


//------------------------------------------------------------------------------
int RRandom::Calc1(void)
{
	int k(Value/q);
	Value=(a*(Value-(k*q)))-(r*k); // Schrage's technique
	if(Value<0)
		Value+=INT_MAX;
	return(Value);
}


//------------------------------------------------------------------------------
int RRandom::Calc2(void)
{
	int k(Value/q1);
	Value=(a1*(Value-(k*q1)))-(r*k); // Schrage's technique
	if(Value<0)
		Value+=M1;
	return(Value);
}


//------------------------------------------------------------------------------
int RRandom::Calc3(void)
{
	int k(Aux2/q2);
	Aux2=(a2*(Aux2-(k*q2)))-(r2*k); // Schrage's technique
	if(Aux2<0)
		Aux2+=M2;
	return(Aux2);
}


//------------------------------------------------------------------------------
void RRandom::Reset(const int seed)
{
	switch(Type)
	{
		case Good:
		{
			if(!seed)
				Seed=1;
			else
				Seed=seed;
			Value=Seed;                      // Value starts as Seed
			break;
		}
		case Better:
		{
			if(!seed)
				Seed=1;
			else
				Seed=seed;
			Value=Seed;                    // Value starts as Seed

			// 1st, a few warmups
			for(int i=0;i<8;i++)
				Calc1();

			// Ok, NOW fill the table
			for(int i=31;i>=0;i--)
				Table[i] = Calc1();
			Aux1=Table[0];
		}
		case Best:
		{
			if(!seed)
				Seed=1;
			else
				Seed=seed;
			Value=Seed;                   // Value starts as Seed
			Aux2=Seed;

			// 1st, a few warmups
			for(int i=0;i< 8;i++)
				Calc2();

			// Ok, NOW fill the table
			for(int i=31;i>=0;i--)
				Table[i]=Calc2();

			Aux1=Table[0];
		}
	}
}


//------------------------------------------------------------------------------
double RRandom::GetValue(void)
{
	double Result;

	switch(Type)
	{
		case Good:
		{
			int k(Value/q);
			Value=(a*(Value-(k*q)))-(r*k); // Schrage's technique
			if(Value<0)
				Value+=INT_MAX;
			Result=Value*Minv;
			break;
		}
		case Better:
		{
			Calc1();                   // Compute a new _value
			int j(Aux1/NDIV);          // Compute an index. j is in [0..31]
			Aux1=Table[j];             // Return value at index
			Table[j]=Value;            // Save new value
			Result=Aux1*Minv;          // float it
			if(Result>MaxR)
				Result=MaxR;
			break;
		}
		case Best:
		{
			Calc2();
			Calc3();
			int j(Aux1/NDIVB);         // Compute an index. j is in [0..31]
			Aux1=Table[j]-Aux2;        // Combine series
			Table[j]=Value;            // Save new value
			if(Aux1<0)
				Aux1+=M1;              // Ensure subscript remains in range
			Result=Aux1*MinvB;         // Float it
			if(Result>MaxR)
				Result=MaxR;
			break;
		}
	}
	return(Result);
}
