/*

	R Project Library

	RGeoInfos.h

	Set of geometric informations - Header.

	(c) 2001 by P. Francq.

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

*/



//-----------------------------------------------------------------------------
#ifndef RGeoInfosH
#define RGeoInfosH


//-----------------------------------------------------------------------------
// include files for R Project
#include <rstd/rstd.h>
using namespace RStd;
#include <rga2d/robj2d.h>
#include <rga2d/rgeoinfo.h>
#include <rga2d/rgeoinfoconnections.h>
#include <rga2d/robj2dcontainer.h>
#include <rga2d/rproblem2d.h>
using namespace RGA2D;



//-----------------------------------------------------------------------------
namespace RGA2D{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/**
* This class implements a container of geometric information.
* @author Pascal Francq
* @short Container of geometric information.
*/
class RGeoInfos : public RStd::RContainer<RGeoInfo,unsigned int,true,false>
{
public:

	/**
	* Pointer to the problem.
	*/
	RProblem2D* Problem;

	/**
	* Connections of the geometric information.
	*/
	RGeoInfoConnections Cons;

	/**
	* Number of "real" objects represented.
	*/
	unsigned int RealNb;

	/**
	* Which objects were selected for crossover.
	*/
	bool* Selected;

	/**
	* Construct the container.
	* @param prob           The instance of the problem.
	* @param create         If true, nb geometric information are created.
	*/
	RGeoInfos(RProblem2D* prob,bool create) throw(bad_alloc);

	/**
	* Select a "good" set of geometric informations. This is used by the
	* crossover of the GA.
	* @param cont           The container that will be hold the selected
	*                       geomtric information.
	* @param grid           The grid representing the Layout.
	* @param selected       Array given the already selected objects.
	*/
	void GetSetInfos(RObj2DContainer* cont,RGrid* grid,bool* selected);

	/**
	* Calculate the boundary rectangle of all the geometric information.
	* @param rect           The rectangle that will be hold the result.
	*/
	void Boundary(RRect &rect);

	/**
	* Clear all the geometric informations.
	*/
	void ClearInfos(void);

	/**
	* The assignement operator.
	*/
	RGeoInfos& operator=(const RGeoInfos& infos) throw(bad_alloc);

	/**
	* Create a problem file for a specific configuration.
	* @seek                 Actual "seek" value for the random number
	*                       generator.
	*/
	void CreateProblem(unsigned long seek);

	/**
	* Destruct the geometric information.
	*/
	virtual ~RGeoInfos(void);
};


}  //-------- End of namespace RGA2D ------------------------------------------


//-----------------------------------------------------------------------------
#endif
