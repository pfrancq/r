/*

	R Project Library

	RRandom

	Class representing random number generators:

	By (c) P. Francq, 1999.

	Version 1.0.0.1

	Last Modify: 1999/10/06

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
// include files for ANSI C/C++
#include <limits.h>
#include <iostream>


//-----------------------------------------------------------------------------
// include files for R Project
#include <rmath/random.h>
using namespace RMath;


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



//-----------------------------------------------------------------------------
//
// RRandom
//
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Global static variable
RRandom *RMath::RRandom::RandomGen=0;


//-----------------------------------------------------------------------------
// Global "random" function
long RMath::RRandom::RRand(long max)
{
	return(RandomGen->Value(max));
}



//-----------------------------------------------------------------------------
//
// RRandomGood
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
void RMath::RRandomGood::Reset(const int seed)
{
	_seed = _seed == MASK ? 1 : MASK ^ seed; //  xor prevents seed == 0
	_value = _seed; //  _value starts as _seed
}


//-----------------------------------------------------------------------------
double RMath::RRandomGood::Value(void)
{
	int k = _value / q;
	_value = a * (_value - k * q) - r * k; // Schrage's technique
	if (_value < 0) _value += INT_MAX;
	return _value * Minv;
}



//-----------------------------------------------------------------------------
//
// RRandomBetter
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
int RMath::RRandomBetter::Calc(void)
{
	int k = _value / q;
	_value = a * (_value - k * q) - r * k; // Schrage's technique
	if (_value < 0) _value += INT_MAX;
	return _value;
}


//-----------------------------------------------------------------------------
void RMath::RRandomBetter::Reset(const int seed)
{
	_seed = _seed == MASK ? 1 : MASK ^ seed; //  xor prevents seed == 0
	_value = _seed; //  _value starts as _seed

	//  1st, a few warmups
	for (int i = 0; i < 8; i++)
		Calc();

	//  ok, NOW fill the table
	for (int i = 31; i >= 0; i--)
		_table[i] = Calc();

	_aux = _table[0];
}


//-----------------------------------------------------------------------------
double RMath::RRandomBetter::Value(void)
{
	Calc();                   //  calc a new _value
	int j = _aux / NDIV;      //  calc an index. j is in [0..31]
	_aux = _table[j];         //  return value at index
	_table[j] = _value;       //  save new value
	double Result = _aux * Minv;    //  float it
	if(Result > MaxR) Result = MaxR;
	return Result;
}



//-----------------------------------------------------------------------------
//
// RRandomBest
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
int RMath::RRandomBest::Calc1(void)
{
	int k = _value / q1;
	_value = a1 * (_value - k * q1) - r1 * k; // Schrage's technique
	if (_value < 0) _value += M1;
	return _value;
}


//-----------------------------------------------------------------------------
int RMath::RRandomBest::Calc2(void)
{
	int k = _aux2 / q2;
	_aux2 = a2 * (_aux2 - k * q2) - r2 * k; // Schrage's technique
	if (_aux2 < 0) _aux2 += M2;
	return _aux2;
}




//-----------------------------------------------------------------------------
void RMath::RRandomBest::Reset(const int seed)
{
	_seed = _seed == MASK ? 1 : MASK ^ seed; //  xor prevents seed == 0
	_value = _seed; //  _value starts as _seed
	_aux2 = _seed;

	//  1st, a few warmups
	for (int i = 0; i < 8; i++)
		Calc1();

	//  ok, NOW fill the table
	for (int i = 31; i >= 0; i--)
	_table[i] = Calc1();

	_aux1 = _table[0];
}


//-----------------------------------------------------------------------------
double RMath::RRandomBest::Value(void)
{
	Calc1();
	Calc2();
	int j = _aux1 / NDIVB;   //  calc an index. j is in [0..31]
	_aux1 = _table[j] - _aux2; //  combine series
	_table[j] = _value;           //  save new value
	if (_aux1 < 0)
		_aux1 += M1; //  ensure subscript remains in range
	double Result = _aux1 * MinvB;      //  float it
	if (Result > MaxR) Result = MaxR;
	return Result;
}
