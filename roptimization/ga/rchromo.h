/*

	Rainbow Library Project

  RChromo.h

  Chromosomes of Genetic Algorithms - Header

  (C) 1998-2000 by P. Francq.

  Version $Revision$

  Last Modify: $Date$

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
#ifndef RChromoH
#define RChromoH


//---------------------------------------------------------------------------
// include files for Rainbow
#include "rga.h"
using namespace RGA;


//---------------------------------------------------------------------------
namespace RGA{
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
/** Chromosome for the generic GA.
  * @author Pascal Francq
  * @short Generic chromosome.
  */
template<class cInst,class cChromo,class cFit,class cThreadData>
	class RChromo
{
public:
	/** Pointer to the instance.*/
  cInst *Instance;
	/** Identificator of the chromosome.*/
  unsigned int Id;
	/** Pointer to the fitness of the chromosome.*/
  cFit *Fitness;
	/** Variable to indicate if the chromosome has to be evaluated again,
		*	for example after a crossover or a mutation.
		*/
  bool ToEval;

	/** Construct the chromosome.
		* @param inst			Pointer to the instance.
		* @param id				Identificator of the chromosome.
		*/
  RChromo(cInst *inst,unsigned int id) throw(bad_alloc);

	/** Initialisation of the chromosome.
		* @param thData		Pointer to the "thread-dependent" data of the chromosome.
		*/
	virtual void Init(cThreadData *thData) throw(bad_alloc);

	/** Construct a valid solution.
		* @return		The function must retrun true if a solution has been constructed.
		*/
  virtual bool RandomConstruct(void) {return(true);}

	/** Evaluate the fitness of the chromosome.*/
  virtual void Evaluate(void) {}

	/** Do a crossover by using the chromosome as child.
		* @param parent1	First parent used.
		* @param parent2	Second parent used.
		* @return 				The function must return true if the crossover has been done.
		*/
  virtual bool Crossover(cChromo *parent1,cChromo *parent2) {return(true);}

	/** Do a mutation with the chromosome.
			@return		The function must return true if the mutation has been done.
		*/
  virtual bool Mutation(void) {return(true);}

	/** Print information on screen.*/
  virtual void Print(void) {}

	/** Verify the validity of the chromosome.
			@return The function must return true if the chromosome is a valid one.
		*/
  virtual bool Verify(void) {return(true);}

	/** The assignment operator.*/
  virtual RChromo& operator=(const RChromo &chromo);

	/** Destruct the chromosome.*/
  virtual ~RChromo(void);
};


}//------- End of namespace RGA ---------------------------------------------

//---------------------------------------------------------------------------
#endif
