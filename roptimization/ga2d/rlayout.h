/*

	R Project Library

	RLayout.h

	Layout - Header.

	Copyright 1998-2012 by Pascal Francq (pascal@francq.info).
	Copyright 1998-2008 by the Université Libre de Bruxelles (ULB).

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

*/



//------------------------------------------------------------------------------
#ifndef RLayout_H
#define RLayout_H


//------------------------------------------------------------------------------
// include files for R Project
#include <rstd.h>
#include <robj2d.h>
#include <rgeoinfo.h>
#include <rgeoinfoconnection.h>
#include <robj2dcontainer.h>
#include <rproblem2d.h>


//------------------------------------------------------------------------------
namespace R{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* The RLayout class provides a representation for a given layout. In practice,
* the container holds several instances of RGeoInfo in a specific
* order:
* - NbObjs ones corresponding to the objects (RObj2D).
* - Eventually, several ones corresponding to aggregators (RObj2DContainer).
* @author Pascal Francq
* @short Layout.
*/
class RLayout : private RContainer<RGeoInfo,true,false>
{
protected:

	/**
	* Pointer to the problem.
	*/
	RProblem2D* Problem;

	/**
	 * The board.
	 */
	RGeoInfo Board;

	/**
	* Placement of the connections.
	*/
	RContainer<RGeoInfoConnection,true,false> PlacedConnections;

	/**
	 * Number of aggregators.
	 */
	size_t NbAggregators;

public:

	/**
	* Construct the layout. A geometric information is created for each object
	* and for the board.
	* @param prob           The instance of the problem.
	* @param create         If true, create the geometric information.
	*/
	RLayout(RProblem2D* prob,bool create);

	/**
	* Calculate the boundary rectangle of the layout.
	* @param rect           The rectangle that will be hold the result.
	*/
	void Boundary(RRect& rect);

	/**
	* Clear the layout.
	*/
	void ClearInfos(void);

	/**
	* Copy a layout.
	* @param infos           Layout.
	* @param grid            Grid.
	*/
	void Copy(const RLayout& infos,RGrid* grid);

	/**
	 * @return pointer to the problem to solve.
	 */
	RProblem2D* GetProblem(void) const {return(Problem);}

	/**
	 * @return the geometric information related to a given object.
	 * @param id             Identifier of the object.
	 */
	RGeoInfo* operator[](size_t id);

	/**
	 * @return the geometric information related to a given object (const version).
	 * @param id             Identifier of the object.
	 */
	const RGeoInfo* operator[](size_t id) const;

	/**
	 * @return the geometric information related to the board.
	 */
	inline const RGeoInfo* GetBoard(void) const {return(&Board);}

	/**
	 * Fill an array with the layout.
	 * @param tab            Array to fill (must have the correct size).
	 */
	inline void GetTab(RGeoInfo** tab) {RContainer<RGeoInfo,true,false>::GetTab(tab);}

	/**
	 * @return a cursor on the layout.
	 */
	RCursor<RGeoInfo> GetInfos(void) const {return(RCursor<RGeoInfo>(*this));}

	/**
	 * @return the number of aggregators.
	 */
	inline size_t GetNbAggregators(void) const {return(NbAggregators);}

	/**
	 * @return a cursor over the placed connections.
	 */
	inline RCursor<RGeoInfoConnection> GetConnections(void) const {return(RCursor<RGeoInfoConnection>(PlacedConnections));}

	/**
	 * @return a new aggregator linked to the layout
	 */
	RObj2DContainer* GetNewAggregator(void);

	/**
	 * Destroy the aggregators (if any).
	 */
	void DestroyAggregators(void);

	/**
	 * @return the distances of the connections for a given object. Eventually,
	 * the object is considered to be placed at a given position.
	 * @param info           Object.
	 * @param pos            Position where the object may be placed. If
	 *                       RPoint::Null, the method uses the current
	 *                       position.
	 */
	tCoord ComputeDist(RGeoInfo* info,const RPoint& pos=RPoint::Null);

private:

	/**
	* @return the sum of the weights of the connections involving a given
	* object and which are composed from at least one already placed object.
	* @param info            Object.
	*/
	double ComputeWeight(RGeoInfo* info);

	/**
	* Determine the two objects that :
	* - have the most connected objects;
	* - are in a given rectangle;
	* - are not selected.
	* @param i1             Pointer to the first object.
	* @param i2             Pointer to the second object.
	* @param bound          Rectangle.
	* @param selected       Which objects are selected.
	* @param kernel         PROMETHEE kernel used.
	* @param random         Random number generator.
	*/
	void GetBestsConnected(RGeoInfo* (&i1),RGeoInfo* (&i2),const RRect& bound,bool* selected,RPromKernel* kernel,RRandom& random);

public:

	/**
	* @return the object that is not placed and that have the most connected
	* weight with the objects already placed.
	* @param order           Order in which to treat the objects.
	* @param nbobjs          Number of objects to treat.
	* @param nbok            Number of objects already placed.
	* @return The geometric information of the object to place.
	*/
	RGeoInfo* GetMostConnected(RGeoInfo** order,size_t nbobjs,size_t nbok);

	/**
	* Fill an aggregator relative to another layout with a "good" set of
	* geometric information.
	*
	* This method is used by the crossover of the GA.
	* @param cont            Aggregator to build (is emptied by this method).
	* @param selected        Array given the already selected objects.
	* @param kernel         PROMETHEE kernel used.
	* @param random         Random number generator.
	*/
	void FillAggregator(RObj2DContainer* cont,bool* selected,RPromKernel* kernel,RRandom& random);

	/**
	* Compute all the connections from scratch.
	* @return the total length of the connections.
	*/
	tCoord ComputeConnections(void);

	/**
	* Destruct the placement.
	*/
	virtual ~RLayout(void);
};


}  //-------- End of namespace R -----------------------------------------------


//------------------------------------------------------------------------------
#endif
