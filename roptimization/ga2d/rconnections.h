/*

	R Project Library

	RConnections.h

	Connections for the 2D Placement - Header.

	Copyright 2000-2003 by the Universit�Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).

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
#ifndef RConnections_H
#define RConnections_H


//------------------------------------------------------------------------------
// include files for R Project
#include <rga/rga.h>
#include <rga2d/robj2d.h>
#include <rga2d/rgeoinfo.h>
#include <rga2d/rconnection.h>
#include <rpromethee/rpromcriterion.h>


//------------------------------------------------------------------------------
namespace R{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// forward class declaration
class RGeoInfos;


//------------------------------------------------------------------------------
/**
* The RConnections class provides a representation for the connections of the
* problem.
* @short Connections.
* @author Pascal Francq
*/
class RConnections : public RContainer<RConnection,true,false>
{
	/**
	* Random number generator to use.
	*/
	RRandom* Random;

	/**
	* Prom�h� Parameters for the distance.
	*/
	RPromCriterionParams DistParams;
	
	/**
	* Prom�h� Parameters for the weight.
	*/
	RPromCriterionParams WeightParams;

public:

	/**
	* Construct the connections.
	*/
	RConnections(void);

	/**
	* Initialize all the necessary structures when all connections are inserted.
	*/
	void Init(void);
	
	/**
	* Set the parameters for Prom�h�.
	* @param dist           Prom�h� Parameters for the distance.
	* @param weight         Prom�h� Parameters for the weight.
	* @param r              Pointer to the random generator.
	*/
	void SetParams(const RPromCriterionParams& dist,const RPromCriterionParams& weight,RRandom* r);

	/**
	* Calculate the sum of the weight of the connected objects, that are already
	* placed, to a given one representing by a geometric information.
	* @param infos          The geometric information of all the objects.
	* @param cur            Geometric information representing the given object for
	*                       wich calculate the distance.
	*/
	double GetCon(RGeoInfos* infos,RGeoInfo* cur);

	/**
	* Return the object that have the most connected objects go placed and that
	* isn't selected.
	* @param infos          The geometric information of all the objects.
	* @param nb             Number of objects.
	* @param selected       Array to hold which objects are selected.
	* @param bound          Boundary rectangle representing the objects to
	*                       analyse.
	* @return               The geometric information of the object to place.
	*/
	RGeoInfo* GetBestConnected(RGeoInfos* infos,unsigned int nb,bool* selected,RRect& bound);

	/**
	* Return the object that is not placed and that have the most connected
	* weight with the objects already placed.
	* @param infos          The geometric information of all the objects.
	* @param nb             Number of objects.
	* @param order          Order in which to treat the objects.
	* @param nbok           Number of objects already placed.
	* @return The geometric information of the object to place.
	*/
	RGeoInfo* GetMostConnected(RGeoInfos* infos,unsigned int nb,RGeoInfo** order,unsigned int nbok);

	/**
	* Calculate the sum of the Manhattant distance between all placed objects
	* representing by the geometric informations.
	* @param infos          The goemetric informations.
	*/
//	double GetDistances(RGeoInfo** infos);

	/**
	* Calculate the sum of the Manhattant distance between all placed objects
	* representing by the geometric informations and a given one.
	* @param infos          Goemetric information of the objects.
	* @param info           Geometric information info of the single object.
	*/
	double GetDistances(RGeoInfo** infos,RGeoInfo* info);

	/**
	* Destruct the connections.
	*/
	~RConnections(void);

	// friend classes
	friend class RGeoInfoConnections;
};


}  //------- End of namespace R ------------------------------------------------


//------------------------------------------------------------------------------
#endif
