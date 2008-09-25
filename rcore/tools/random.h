/*

	R Project Library

	RRandom

	Class representing random number generators:
	RRandom       A pure base class
	RRandomGood	  Park & Miller minimal standard congruential generator
	RRandomBetter Park & Miller with a Bays & Durham shuffle
	RRandomBest   L'Ecuyer's two-series combo plus a shuffle for a period > 2e18

	Copyright 1999-2007 by the Université Libre de Bruxelles.

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
#ifndef RRandom_H
#define RRandom_H


//------------------------------------------------------------------------------
namespace R{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* This is a abstract class for random number generation classes.
* @author Pascal Francq
* @short Generic Random Generator.
*/
class RRandom
{
protected:

	/**
	* Next random value.
	*/
	int Seed;

public:

	/**
	* Construct the random generator.
	*/
	RRandom(void) {Seed=0;}

	/**
	* Restart the sequence.
	*/
	virtual void Reset(const int) {}

	/**
	* Return the next value in [0,1] from the sequence.
	*
	* This is an pure virtual function that	must be implement for a real random
	* generator.
	*/
	virtual double GetValue(void)=0;

	/**
	* Return a number in the interval [0,max[.
	* @param max            Variable used to calculate the number.
	*/
	long GetValue(const long max)
	{
		return(static_cast<long>(max*GetValue()));
	}

	/**
	* Return the seed value.
	*/
	int GetSeed(void) const {return(Seed);}

	/**
	* Random the position of elements of a vector.
	* @param arr            A pointer to the array representing the vector.
	* @param size           The size of the vector.
	*/
    template<class T> inline void RandOrder(T *arr,unsigned size)
    {
		register unsigned i,jump;
		T aux;
		register T* p1;
		register T* p2;

		if(size>1)
		{
			for(p1=arr,i=size;;p1++)
			{
				if((jump=GetValue(i))) { aux=*(p2=p1+jump); *p2=*p1; *p1=aux; }
				if(!--i) break;
			}
		}
	}

	virtual ~RRandom(void) {}
};


//------------------------------------------------------------------------------
/**
* A good and fast random generator using the Park & Miller minimal standard
*	congruential generator.
* @author Pascal Francq
* @short Good Random Generator.
*/
class RRandomGood : public RRandom
{
	/**
	* Variable for Internal use.
	*/
	int Value;

public:

	/**
	* Construct the random generator.
	*/
	RRandomGood(const int seed=0) : RRandom() {Seed=seed;}

	/**
	* Restart the sequence.
	* @param seed           Value used to restart.
	*/
	virtual void Reset(const int seed);

	/**
	* Return the next value in [0,1] from the sequence.
	*/
	virtual double GetValue(void);
};


//------------------------------------------------------------------------------
/**
* A better but slower random generator using the Park & Miller with a Bays &
* Durham shuffle.
* author Pascal Francq
* @short Better Random Generator.
*/
class RRandomBetter : public RRandom
{
	/**
	* Variable for Internal use.
	*/
	int Aux;

	/**
	* Variable for Internal use.
	*/
	int Table[32];

	/**
	* Variable for Internal use.
	*/
	int Value;

	/**
	* Calculation function for Internal use.
	*/
	int Calc(void);

public:

	/**
	* Construct the random generator.
	*/
	RRandomBetter(const int seed=0) : RRandom() {Seed=seed;}

	/**
	* Restart the sequence.
	* @param seed           Value used to restart.
	*/
	virtual void Reset(const int seed);

	/**
	* Return the next value in [0,1] from the sequence.
	*/
	virtual double GetValue(void);
};


//------------------------------------------------------------------------------
/**
* The best but also the slower random generator using the L'Ecuyer's two-series
* combo plus a shuffle for a period > 2e18.
* @author Pascal Francq
* @short Best Random Generator.
*/
class RRandomBest : public RRandom
{
	/**
	* Variable for Internal use.
	*/
	int Aux1;

	/**
	* Variable for Internal use.
	*/
	int Aux2;

	/**
	* Variable for Internal use.
	*/
	int Table[32];

	/**
	* Variable for Internal use.
	*/
	int Value;

	/**
	* Calculation function for Internal use.
	*/
	int Calc1(void);

	/**
	* Calculation function for Internal use.
	*/
	int Calc2(void);

public:

	/**
	* Construct the random generator.
	*/
	RRandomBest(const int seed=0) : RRandom() {Seed=seed;}

	/**
	* Restart the sequence.
	* @param seed           Value used to restart.
	*/
	virtual void Reset(const int seed);

	/**
	* Return the next value in [0,1] from the sequence.
	*/
	virtual double GetValue(void);
};


}  //-------- End of namespace R -----------------------------------------------


//------------------------------------------------------------------------------
#endif
