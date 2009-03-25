/*

	R Project Library

	RRandom

	Class representing random number generators.

	Copyright 1999-2008 by the Université Libre de Bruxelles.

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


//-----------------------------------------------------------------------------
// include file for ANSI C/C++
#include <stdlib.h>


//------------------------------------------------------------------------------
namespace R{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* This is a class representing a random generator. In practice, it is an abstract
* class internally re-implemented.
* @author Pascal Francq
* @short Random Generator.
*/
class RRandom
{
public:

	/**
	 * Type of the random generator.
	 */
	enum Type
	{
		Good     /* A good and fast random generator using the Park & Miller
		            minimal standard congruential generator.*/,
		Better   /* A better but slower random generator using the Park &
		            Miller with a Bays & Durham shuffle*/,
		Best     /* The best but also the slower random generator using the
		            L'Ecuyer's two-series combo plus a shuffle for a period
		             > 2e18 */
	};

protected:

	/**
	* Next random value.
	*/
	int Seed;

public:

	/**
	* Construct the random generator.
	* @param seed            Initial seed. Since this value cannot be null, if
	*                        so, seed is set to 1.
	*/
	RRandom(const int seed=1) {Seed=seed;}

	/**
	* Restart the sequence.
	* @param seed            Initial seed. Since this value cannot be null, if
	*                        so, seed is set to 1.
	*/
	virtual void Reset(const int seed);

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
		return(static_cast<long>(static_cast<double>(max)*GetValue()));
	}

	/**
	* Return the seed value.
	*/
	int GetSeed(void) const {return(Seed);}

	/**
	* Randomize the position of elements of a vector.
	* @param arr            Pointer to the array representing the vector.
	* @param size           Size of the vector.
	*/
    template<class T> inline void RandOrder(T *arr,size_t size)
    {
    	if(!size) return;
		register size_t i,jump;
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

    /**
     * Create a random number generator.
     * @param type           Type of the generator.
     * @param seed           Initial seed.
     */
    static RRandom* Create(Type type,const int seed=0);

    /**
     */
	virtual ~RRandom(void);
};


}  //-------- End of namespace R -----------------------------------------------


//------------------------------------------------------------------------------
#endif
