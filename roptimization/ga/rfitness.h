/*

	R Project Library

	RFitness.h

	Fitness for Chromosomes of Genetic Algorithms - Header

	Copyright 1998-2003 by the Université Libre de Bruxelles.

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
#ifndef RFitness_H
#define RFitness_H


//------------------------------------------------------------------------------
namespace R{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* \ingroup GA
* This class represents a fitness for a specific problem. the cVal parameter
* is used to specify the nature of the fitness, and the max parameter is used
* to indicate if the fitness is to maximized or minimized.
* @param cVal               The type of information contained in the fitness.
* @param Max                If true, the fitness is to maximized.
*
* This example implements a fitness that is represented by a integer value and
* that has to be minimized.
* @code
* class MyFitness : public RFitness<int,false>
* {
*	public:
*		MyFitness(void);
*	};
* @endcode
* @author Pascal Francq
* @short Generic Fitness.
*/
template<class cVal,bool Max>
	class RFitness
{
public:

	/**
	* The value of the fitness.
	*/
	cVal Value;

	/**
	* Construct the fitness. The value is set to 0, so if cVal is an object,
	* an assignment operator with 0 has to be implemented.
	*/
	RFitness(void);

	/**
	* Assignment operator.
	* @param f              The fitness used for the assignment.
	* @return The function returns (*this).
	*/
	inline RFitness& operator=(const RFitness &f);

	/**
	* Assignment operator.
	* @param value          The value used for the assignment.
	* @return The function returns (*this).
	*/
	inline RFitness& operator=(const cVal value);

	/**
	* The equal operator.
	* @param f              The fitness used for the compraraison.
	* @return The function returns true if the value of the two fitnesses are
	* equal.
	*/
	inline bool operator==(const RFitness &f);

	/**
	* The different operator.
	* @param f              The fitness used for the compraraison.
	* @return The function returns true if the value of the two fitnesses are
	* different.
	*/
	inline bool operator!=(const RFitness &f);

	/**
	* The greater operator.
	* @param f              The fitness used for the compraraison.
	* @return The function returns true if the value of the fitness is
	* greater than the value of f.
	*/
	inline bool operator>(const RFitness &f);

	/**
	* The less-than operator.
	* @param f              The fitness used for the compraraison.
	* @return The function returns true if the value of the fitness is less
	* than the value of f.
	*/
	inline bool operator<(const RFitness &f);

	/**
	* Returns true if the fitness is to maximize.
	*/
	inline bool ToMaximize(void);

	/**
	* Returns true if the fitness is to minimize.
	*/
	inline bool ToMinimize(void);
};


//------------------------------------------------------------------------------
// inline implementation
#include <rfitness.hh>


}  //------- End of namespace R ------------------------------------------------


//-----------------------------------------------------------------------------
#endif
