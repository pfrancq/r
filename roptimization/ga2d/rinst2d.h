/*

	Rainbow Library Project

  RInst2D.h

  Instance for 2D placement GA - Header

  (C) 1999-2000 by P. Francq.

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
#ifndef RInst2DH
#define RInst2DH


//---------------------------------------------------------------------------
// include files for Rainbow
#include "rga2d.h"
using namespace RGA;


//---------------------------------------------------------------------------
namespace RGA{
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
/** This is a class that holds all data needed by each thread for the 2D GA.
	*	@author Pascal Francq
	*	@short 2D GA "thread-dependent" data.
	*/
template<class cInst,class cChromo>
	class RThreadData2D : public RThreadData<cInst,cChromo>
{
public:
	/** Number of objects to place. */
	unsigned int NbObjs;				
	/** Array of identificators for objects. */
	unsigned int *Order;				
	/** Array of identificators for objects. */
	unsigned int *Order2;
	/** Temporary array of pointers to Objects (Crossover & Mutation). */
	RObj2D **tmpObjs;						
	/** Temporary objects container. */
  RObj2DContainer *tmpObj1; 	
	/** Temporary objects container. */
  RObj2DContainer *tmpObj2;

	/** Construct the data.
		* @param owner		The instance of the problem.
		*/
	RThreadData2D(cInst *owner) throw(bad_alloc);

	/** Initialise thje data.*/
	virtual void Init(void) throw(bad_alloc);

	/** Destruct the data.*/
	virtual ~RThreadData2D(void);
};


//---------------------------------------------------------------------------
/**	Instance for the 2D placement GA.
	*	@author Pascal Francq
	*	@short 2D GA Instance.
	*/
template<class cInst,class cChromo,class cFit,class cThreaData,class cInfo>
	class RInst2D	: public RInst<cInst,cChromo,cFit,cThreaData>
{
public:
	/** Objects to place.*/
	RObj2D **Objs;
	/** Number of objects.*/
	unsigned int NbObjs;
	/** Local Optimisation.*/
	bool bLocalOpti;

protected:

	/** Type of the heuristic that used.*/
	HeuristicType Heuristic;

public:

	/** Point representing the limits for the placement.*/
	RPoint Limits;

	/** Construct the instance. The instance is not responsible for the desallocation
		* of the objects to place.
		* @param popsize		The size of the population.
		* @param objs				The objects to place.
		* @param nbobjs			Number of objects to place.
		* @param limits			The limits for the placement.
		* @param h					The heuristic that has to be used.
		*/
  RInst2D(unsigned int popsize,RObj2D** objs,unsigned int nbobjs,RPoint &limits,HeuristicType h) throw(bad_alloc);

	/** Return the limits for the placement.*/
	inline RPoint& GetLimits(void);

	/** Return the heuristic type.*/
	inline HeuristicType GetHeuristic(void) { return(Heuristic); }

	/** Return true if a local optimisation is needed.*/
	inline bool LocalOpti(void) { return(bLocalOpti); }
};


}//------- End of namespace RGA ---------------------------------------------

//---------------------------------------------------------------------------
#endif
