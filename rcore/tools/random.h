/*

	R Project Library

	RRandom

	Class representing random number generators:
    RRandom       A pure base class
    RRandomGood	  Park & Miller minimal standard congruential generator
    RRandomBetter	Park & Miller with a Bays & Durham shuffle
    RRandomBest	  L'Ecuyer's two-series combo plus a shuffle for a period > 2e18

	(C) 1999-2000 by P. Francq.

	Version $Revision: 1.5 $

	Last Modify: $Date: 2000/11/29 09:23:50 $

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
#ifndef RRandomH
#define RRandomH


//-----------------------------------------------------------------------------
namespace RMath{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
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
	int _seed;

public:

	/**
	* Pointer to the current random generator used for randorder and RRand. This
	* pointer must be initialise by the user
	*/
	static RRandom *RandomGen;

	/**
	* Construct the random generator.
	*/
	RRandom(void) {}

	/**
	* Restart the sequence.
	*/
	virtual void Reset(const int) {}

	/**
	* Return the next value in the sequence. This is an pure virtual function
	* that	must be implement for a real random generator.
	*/
	virtual double Value(void)=0;

	/**
	* Return a number in the interval [0,max[.
	* @param max	Variable used to calculate the number.
	*/
	long Value(const long max)
  	{
    	return(static_cast<long>(max*Value()));
  	}

	/**
	* Return the seed value.
	*/
	int	Seed(void) const { return(_seed); }

	/**
	* Return a number in the interval [0,max[ using the current random generator.
	* @param max				Variable used to calculate the number.
	*/
	static long RRand(long max);

	/**
	* Random the position of elements of a vector using the current random generator.
	* @param arr		A pointer to the array representing the vector.
	* @param size		The size of the vector.
	*/
    template<class T> static inline void randorder(T *arr,unsigned size)
    {
		register unsigned i,jump;
		T aux;
		register T *p1;
		register T *p2;

    	#if __BORLANDC__
    	  #pragma warn -pia
    	#endif
      	if(size>1)
        	for(p1=arr,i=size;;p1++)
	        {
    	      if(jump=RRand(i)) { aux=*(p2=p1+jump); *p2=*p1; *p1=aux; }
        	  if(!--i) break;
	        }
		#if __BORLANDC__
			#pragma warn +pia
		#endif
	}
	
	/**
	* Random the position of elements of a vector.
	* @param arr		A pointer to the array representing the vector.
	* @param size		The size of the vector.
	*/
    template<class T> inline void RandOrder(T *arr,unsigned size)
    {
      register unsigned i,jump;
      T aux;
      register T *p1;
      register T *p2;

    	#if __BORLANDC__
    	  #pragma warn -pia
    	#endif
      if(size>1)
        for(p1=arr,i=size;;p1++)
        {
          if(jump=Value(i)) { aux=*(p2=p1+jump); *p2=*p1; *p1=aux; }
          if(!--i) break;
        }
		#if __BORLANDC__
			#pragma warn +pia
		#endif
	}	
};


//-----------------------------------------------------------------------------
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
	int	_value;

public:

	/**
	* Construct the random generator.
	*/
	RRandomGood(const int seed) : RRandom() {Reset(seed);}

	/**
	* Restart the sequence.
	* @param seed		Value used to restart.
	*/
	virtual void Reset(const int seed);

	/**
	* Return the next value in the sequence.
	*/
	virtual double Value(void);
};


//-----------------------------------------------------------------------------
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
	int	_aux;

	/**
	* Variable for Internal use.
	*/
	int	_table[32];

	/**
	* Variable for Internal use.
	*/
	int	_value;

	/**
	* Calculation function for Internal use.
	*/
	int Calc(void);

public:

	/**
	* Construct the random generator.
	*/
	RRandomBetter(const int& seed) : RRandom() {Reset(seed);}

	/**
	* Restart the sequence.
	* @param seed		Value used to restart.
	*/
	virtual void Reset(const int seed);

	/**
	* Return the next value in the sequence.
	*/
	virtual double Value(void);
};


//-----------------------------------------------------------------------------
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
	int	_aux1;

	/**
	* Variable for Internal use.
	*/
	int	_aux2;

	/**
	* Variable for Internal use.
	*/
	int	_table[32];

	/**
	* Variable for Internal use.
	*/
	int	_value;

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
	RRandomBest(const int& seed) : RRandom() {Reset(seed);}

	/**
	* Restart the sequence.
	* @param seed		Value used to restart.
	*/
	virtual void Reset(const int seed);

	/**
	* Return the next value in the sequence.
	*/
	virtual double Value(void);
};


}  //-------- End of namespace RStd -------------------------------------------


//-----------------------------------------------------------------------------
#endif
