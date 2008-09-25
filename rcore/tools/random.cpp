/*

	R Project Library

	RRandom

	Class representing random number generators:

	Copyright 1999-2003 by the Universit� Libre de Bruxelles.

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



//------------------------------------------------------------------------------
// include files for ANSI C/C++
#include <limits.h>
#include <iostream>


//------------------------------------------------------------------------------
// include files for R Project
#include <random.h>
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
// RRandomGood
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void RRandomGood::Reset(const int seed)
{
	Seed=(Seed==(MASK?1:MASK^seed)); // XOR prevents seed == 0
	Value=Seed;                      // Value starts as Seed
}


//------------------------------------------------------------------------------
double RRandomGood::GetValue(void)
{
	int k(Value/q);
	Value=(a*(Value-(k*q)))-(r*k); // Schrage's technique
	if(Value<0)
		Value+=INT_MAX;
	return(Value*Minv);
}



//------------------------------------------------------------------------------
//
// RRandomBetter
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
int RRandomBetter::Calc(void)
{
	int k(Value/q);
	Value=(a*(Value-(k*q)))-(r*k); // Schrage's technique
	if(Value<0)
		Value+=INT_MAX;
	return(Value);
}


//------------------------------------------------------------------------------
void RRandomBetter::Reset(const int seed)
{
	Seed=(Seed==MASK?1:MASK^seed); // XOR prevents seed == 0
	Value=Seed;                    // Value starts as Seed

	// 1st, a few warmups
	for(int i=0;i<8;i++)
		Calc();

	// Ok, NOW fill the table
	for(int i=31;i>=0;i--)
		Table[i] = Calc();
	Aux=Table[0];
}


//------------------------------------------------------------------------------
double RRandomBetter::GetValue(void)
{
	Calc();                   // Compute a new _value
	int j(Aux/NDIV);          // Compute an index. j is in [0..31]
	Aux=Table[j];             // Return value at index
	Table[j]=Value;           // Save new value
	double Result(Aux*Minv);  // float it
	if(Result>MaxR)
		Result=MaxR;
	return(Result);
}



//------------------------------------------------------------------------------
//
// RRandomBest
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
int RRandomBest::Calc1(void)
{
	int k(Value/q1);
	Value=(a1*(Value-(k*q1)))-(r*k); // Schrage's technique
	if(Value<0)
		Value+=M1;
	return(Value);
}


//------------------------------------------------------------------------------
int RRandomBest::Calc2(void)
{
	int k(Aux2/q2);
	Aux2=(a2*(Aux2-(k*q2)))-(r2*k); // Schrage's technique
	if(Aux2<0)
		Aux2+=M2;
	return(Aux2);
}


//------------------------------------------------------------------------------
void RRandomBest::Reset(const int seed)
{
	Seed=(Seed==MASK?1:MASK^seed);  // XOR prevents seed == 0
	Value=Seed;                   // Value starts as Seed
	Aux2=Seed;

	// 1st, a few warmups
	for(int i=0;i< 8;i++)
		Calc1();

	// Ok, NOW fill the table
	for(int i=31;i>=0;i--)
		Table[i]=Calc1();

	Aux1=Table[0];
}


//------------------------------------------------------------------------------
double RRandomBest::GetValue(void)
{
	Calc1();
	Calc2();
	int j(Aux1/NDIVB);    // Compute an index. j is in [0..31]
	Aux1=Table[j]-Aux2;   // Combine series
	Table[j]=Value;       // Save new value
	if(Aux1<0)
		Aux1+=M1;               // Ensure subscript remains in range
	double Result(Aux1*MinvB);  // Float it
	if(Result>MaxR)
		Result=MaxR;
	return(Result);
}
