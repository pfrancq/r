/*

	R Project Library

	R2AdditiveChoquet.h

	The 2-additive Choquet Integral - Header.

	Copyright 2013 by Pascal Francq (pascal@francq.info).

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
#ifndef R2AdditiveChoquet_H
#define R2AdditiveChoquet_H


//------------------------------------------------------------------------------
// include files for R Project
#include <rsymmetricmatrix.h>


//------------------------------------------------------------------------------
namespace R{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
 * The R2AdditiveChoquet class allows to compute the 2-additive Choquet integral
 * for a given vector of criteria values. It is an aggregating method that
 * computes a global score from a set of criteria values by taking into account
 * not only the importance of each criteria, but also the importance of the
 * interaction between pairs of criteria (for example, a good student scores
 * well in English and in mathematics).
 *
 * In practice, the models needs, for n criteria, the following parameters :
 * - n weights, \f$\phi(i)\f$ representing the importance of each criteria.
 * - \f$\frac{n(n-1)}{2}\f$ weights, \f$I_ij\f$ representing the importance of
 *   the interaction between a pair of criteria.
 *
 * The sum of these parameters must be 1.
 *
 * The 2-additive Choquet integral for a given object, \f$f\f$ can then be
 * computed using:
 * \f[
 		C_{\mu}(f)=
                  \sum_{i,j|I_{i,j}>0}(f(i)\wedge f(j))I_{ij}
                 +\sum_{i,j|I_{i,j}<0}(f(i)\vee f(j))\left|I_{ij}\right|
                 +\sum_{i\in N}f(i)\left[\phi(i)-\frac{1}{2}\sum_{j\neq i}\left|I_{ij}\right|\right]
 * \f]
 * where \f$f(i)\f$ is the value of the object for the \f$i\f$th criteria,
 * \f$\wedge\f$ is the minimum operator, and \f$\vee\f$ is the maximum operator.
 * @short 2-Additive Choquet Integral.
 * @author Pascal Francq.
 */
class R2AdditiveChoquet
{
	/**
	 * Number of criteria.
	 */
	size_t NbCriteria;

	/**
	 * Parameters. Params(i,i) stores the weights of each criteria while
	 * Params(i,j) stores the weights of each interactions.
    */
	RSymmetricMatrix Params;

public:

	/**
	 * Constructor.
    * @param n               Number of criteria.
    */
	R2AdditiveChoquet(size_t n);

	/**
	 * Constructor.
    * @param n               Number of criteria.
    */
	R2AdditiveChoquet(size_t n,RSymmetricMatrix& params);

	/**
	 * Set the parameters of the Choquet integral.
	 * @param params          Parameters. params(i,i) is supposed to be the
	 * weights of each criteria while params(i,j) is supposed to be the weights
	 * of each interactions.
	 * \exception std::range_error is generated if the container has not a value
	 * for each criteria.
	 */
	void SetParams(RSymmetricMatrix& params);

	/**
	 * Compute the 2-Additive Choquet integral of an object defined by a set of
	 * criteria stored in a container.
    * @param obj             Values of the criteria for the objects.
    * @return the 2-Additive Choquet integral.
	 * \exception std::range_error is generated if the container has not a value
	 * for each criteria.
    */
	double Compute(RNumContainer<double,false>& obj);

	/**
	 * Destructor.
    */
	virtual ~R2AdditiveChoquet(void);
};

}  //-------- End of namespace R -----------------------------------------------


//------------------------------------------------------------------------------
#endif
