/*

	R Project Library

	RKernelkMeans.h

	Kernel k-Means Algorithm  - Header

	Copyright 2010 by Pascal Francq (pascal@francq.info).

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
#ifndef RKernelkMeans_H
#define RKernelkMeans_H


//------------------------------------------------------------------------------
// include files for R Project
#include <rstring.h>
#include <rcontainer.h>
#include <random.h>
#include <rcursor.h>
#include <rdebug.h>
#include <rsparsematrix.h>
#include <rmatrix.h>
#include <rvector.h>
#include <rtextfile.h>


//------------------------------------------------------------------------------
namespace R{
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
/**
* The RKernelkMeans class provides the implementation of a kernel k-Means
* grouping algorithm.
* @author Pascal Francq
* @short Kernel k-Means Algorithm.
*/
template<class cGroup,class cObj,class cGroups>
	class RKernelkMeans
{
protected:

	/**
	 * Name of the k-Means
	 */
	RString Name;

	/**
	 * Debugger.
	 */
	RDebug* Debug;

	/**
	* Random number generator to use.
	*/
	RRandom* Rand;

	/**
	* Groups.
	*/
	cGroups* Groups;

	/**
	 * Maximum identifier of an object.
	 */
	size_t MaxId;

	/**
	* Objects to be grouped.
	*/
	RCursor<cObj> Objs;

	/**
	* Objects to be grouped.
	*/
	RCursor<cObj> CopyObjs;

	/**
	 * Objects used to build the clusters.
	 */
	cObj** ObjsUsed;

	/**
	 * Number of Iterations.
	 */
	size_t NbIterations;

	/**
	 * Aplha.
	 */
	double Alpha;

	/**
	 * Convergence ratio used to compute yk.
	 */
	double Convergence;

	/**
	 * The P matrix (D^-1*S)
	 */
	RSparseMatrix P;

	/**
	 * Inverse of the D vector.
	 */
	RVector InvD;

	/**
	 * The H matrix (each column is a vector corresponding to hk).
	 */
	RMatrix H;

	/**
	 * The Y matrix (each column is a vector corresponding to hk).
	 */
	RMatrix Y;

	/**
	 * Temporary vector.
	 */
	RVector Temp;

	/**
	 * Algorithm initialized ?
	 */
	bool WasInit;

	/**
	 * Output file with information.
	 */
	RTextFile Out;

	/**
	 * Fitness of the solution.
	 */
	double FitnessValue;

	/**
	 * Spherical k-Means;
	 */
	bool Spherical;

public:

	/**
	* Construct the k-Means.
	* @param n               Name of the k-Means.
	* @param r               Random number generator to use.
	* @param objs            Cursor over the objects to group (ordered by
	*                        identifier).
	* @param maxid           Maximum identifier of an object.
	* @param alpha           Alpha parameter.
	* @param convergence     Convergence when computing yk.
	* @param debug           Debugger.
	*/
	RKernelkMeans(const RString& n,RRandom* r,RCursor<cObj> objs,size_t maxid,double alpha,double convergence,RDebug* debug=0);

	/**
	 * Initialize (once) the algorithm.
	 */
	void Init(void);

	/**
	* Get the name of the heuristic.
	*/
	RString GetName(void) const {return(Name);}

	/**
	 * Get the number of iterations run.
	 */
	size_t GetNbIterations(void) const {return(NbIterations);}

protected:

	/**
	 * Creates the initial prototypes by randomly choosing them.
	 * @param nb              Number of groups to create.
	 */
	void InitRandom(size_t nb);

	/**
	 * Update the H matrix when the objects are assigned.
	 */
	void CopyEinH(void);

	/**
	 * Update the H matrix when the objects are assigned.
	 */
	void UpdateH(void);

	/**
	 * Normalize the H matrix.
	 */
	void NormalizeH(void);

	/**
	 * Compute re-cursively the Y matrix.
	 */
	void ComputeY(void);

	/**
	* Re-Allocation step where the objects are put in the group that minimize
	* the right expression.
	* @return the number of object re-allocated
	*/
	size_t ReAllocate(void);

public:

	/**
	 * Fill a sparse matrix with the similarities. It may be the full matrix or
	 * the nearest neighbors.
	 * @param sims           Matrix to compute.
	 */
	virtual void FillSimilarities(RSparseMatrix& sims)=0;

	/**
	* Compute the fitness of the given clustering (the higher is the fitness,
	* the best is the clustering.
	*
	* By default, it computes the ratio between the average intra-similarity
	* and the similarity between the centroids and the meta-centroid.
	*/
	virtual double Fitness(void);

	/**
	 * Perform a kMeans.
	 * @param max             Maximal number of iterations.
	 */
	void DokMeans(size_t max);

	/**
	* Run the heuristic.
	* @param groups          Group to initialize.
	* @param max             Maximal number of iterations.
	* @param nb              Number of groups to create.
	* @param spherical       Spherical Kernel k-Means.
	*/
	void Run(cGroups* groups,size_t max,size_t nb,bool spherical);

	/**
	* Destruct the k-Means.
	*/
	virtual ~RKernelkMeans(void);
};


//------------------------------------------------------------------------------
// inline implementation
#include <rkernelkmeans.hh>


}  //------- End of namespace R ------------------------------------------------


//------------------------------------------------------------------------------
#endif
