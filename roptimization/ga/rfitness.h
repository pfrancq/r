/*

	Rainbow Library Project

  RFitness.h

  Fitness for Chromosomes of Genetic Algorithms - Header

  (C) 1998-2000 by P. Francq.

  Version $Revision$

  Last Modify: $Date$

  GA.cpp  : Code of this module
  GA.hh   : Implementation of templates

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

	As a special exception to the GNU General Public License, permission is
	granted for additional uses of the text contained in its release
	of the Rainbow Library.

	The exception is that, if you link the Rainbow with other files
	to produce an executable, this does not by itself cause the
	resulting executable to be covered by the GNU General Public License.
	Your use of that executable is in no way restricted on account of
	linking the Rainbow library code into it.

	This exception does not however invalidate any other reasons why
	the executable file might be covered by the GNU General Public License.

	This exception applies only to the code released under the
	name Rainbow.  If you copy code from other releases into a copy of
	RAinbow, as the General Public License permits, the exception does
	not apply to the code that you add in this way.  To avoid misleading
	anyone as to the status of such modified files, you must delete
	this exception notice from them.

	If you write modifications of your own for Rainbow, it is your choice
	whether to permit this exception to apply to your modifications.
	If you do not wish that, delete this exception notice.

*/



//---------------------------------------------------------------------------
#ifndef RFitnessH
#define RFitnessH


//---------------------------------------------------------------------------
namespace RGA{
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
/** This class represents a fitness for a specific problem. the cVal parameter
	* is used to specify the nature of the fitness, and the max parameter is used
	* to indicate if the fitness is to maximised or minimised.
	*
	*	This example implements a fitness that is represented by a integer value and
	* that has to be minimized.
	* <pre>
	* class MyFitness : public RFitness<int,false>
	* {
	*	public:
	*		MyFitness(void);
	*	};
	* </pre>
	* @author Pascal Francq
	* @short Generic Fitness.
	*/
template<class cVal,bool Max>
	class RFitness
{
public:
	/** The value of the fitness.*/
  cVal Value;

	/** Construct the fitness. The value is set to 0, so if cVal is an object,
		* an assignment operator with 0 has to be implemented.*/
  RFitness(void);

	/**	Assignment operator.
		*	@param f	The fitness used for the assignment.
		*	@return	The function returns (*this).
		*/
  inline RFitness& operator=(const RFitness &f);

	/**	Assignment operator.
		*	@param value	The value used for the assignment.
		*	@return	The function returns (*this).
		*/
	inline RFitness& operator=(const cVal value);

	/** The equal operator.
		* @param	f	The fitness used for the compraraison.
		* @return The function returns true if the value of the two fitnesses are equal.
		*/
  inline bool operator==(const RFitness &f);

	/** The different operator.
		* @param	f	The fitness used for the compraraison.
		* @return The function returns true if the value of the two fitnesses are different.
		*/
  inline bool operator!=(const RFitness &f);

	/** The greater operator.
		* @param	f	The fitness used for the compraraison.
		* @return The function returns true if the value of the fitness is greater than the value of f.
		*/
  inline bool operator>(const RFitness &f);

	/** The less-than operator.
		* @param	f	The fitness used for the compraraison.
		* @return The function returns true if the value of the fitness is less than the value of f.
		*/
  inline bool operator<(const RFitness &f);

	/** Returns true if the fitness is to maximize.*/
	inline bool ToMaximize(void);

	/** Returns true if the fitness is to minimize.*/
	inline bool ToMinimize(void);
};


}//------- End of namespace RGA ---------------------------------------------

//---------------------------------------------------------------------------
#endif
