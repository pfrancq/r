/*

	R Project Library

	R2AdditiveChoquet.h

	The 2-additive Choquet Integral - Header.

	Copyright 2013-2014 by Pascal Francq (pascal@francq.info).

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
 * computed using [1]:
 * \f[
 		C_{\mu}(f)=
                  \sum_{i,j|I_{i,j}>0}(f(i)\wedge f(j))I_{ij}
                 +\sum_{i,j|I_{i,j}\leq 0}(f(i)\vee f(j))\left|I_{ij}\right|
                 +\sum_{i\in N}f(i)\left[\phi(i)-\frac{1}{2}\sum_{j\neq i}\left|I_{ij}\right|\right]
 * \f]
 * where \f$f(i)\f$ is the value of the object for the \f$i\f$th criteria,
 * \f$\wedge\f$ is the minimum operator, and \f$\vee\f$ is the maximum operator.
 *
 * <div class="groupHeader">
 * References
 * </div>
 * [1] M. Grabisch (2006), L'utilisation de l'intégrale de Choquet en aide multicritère à la décision. <em>Newsletter of the European Working
 *   Group "Multicriteria Aid for Decisions"</em>, 3(14), pp. 5-10.
 * @short 2-Additive Choquet Integral.
 * @author Pascal Francq.
 */
class R2AdditiveChoquet
{
	/**
	 * Name representing the function computed.
	 */
	RString Name;

	/**
	 * Number of criteria.
	 */
	size_t NbCriteria;

	/**
	 * Parameters. Params(i,i) stores the weights of each criteria while
	 * Params(i,j) stores the weights of each interactions.
    */
	RSymmetricMatrix Params;

	/**
	 * Parameters verified?
    */
	bool Verified;

public:

	/**
	 * Constructor.
    * @param n               Number of criteria.
	 * @param name            Name of the function computed.
    */
	R2AdditiveChoquet(size_t n,const RString& name=RString::Null);

	/**
	 * Constructor.
    * @param n               Number of criteria.
	 * @param params          Parameters. params(i,i) is supposed to be the
	 *                        weights of each criteria while params(i,j) is
	 *                        supposed to be the weights of each interactions.
	 * @param name            Name of the function computed.
	 * \exception std::range_error is generated if the container has not a value
	 * for each criteria.
	 * \exception std::invalid_argument is generated if the sum of parameters is
	 * not equal to 1.
    */
	R2AdditiveChoquet(size_t n,RSymmetricMatrix& params,const RString& name=RString::Null);

	/**
	 * Compare two choquet function regarding their name.
    * @param choquet         Choquet function to compare with.
    * @return an integral value indicating the relationship between the names.
    */
	int Compare(const R2AdditiveChoquet& choquet) const;

	/**
	 * Compare a choquet function based on its name.
    * @param name            Name to compare with.
    * @return an integral value indicating the relationship between the names.
    */
	int Compare(const RString& name) const;

	/**
    * @return the name of the function computed.
    */
	RString GetName(void) const {return(Name);}

	/**
	 * Verify that the sum of the parameters is one.
	 * @return true if it is the case.
    */
	bool Verify(void);

	/**
	 * Set the parameters of the Choquet integral.
	 * @param params          Parameters. params(i,i) is supposed to be the
	 * weights of each criteria while params(i,j) is supposed to be the weights
	 * of each interactions.
	 * \exception std::range_error is generated if the container has not a value
	 * for each criteria.
	 * \exception std::invalid_argument is generated if the sum of parameters is
	 * not equal to 1.
	 */
	void SetParams(RSymmetricMatrix& params);

	/**
	 * Set the weight of a given criteria.
	 * @param i               Index of the criteria.
	 * @param weight          Weight of the criteria.
    */
	void SetCriteria(size_t i,double weight);

	/**
	 * Get the weight of a given criteria.
    * @param i               Index of the criteria.
    * @return the weight of the criteria.
    */
	double GetCriteria(size_t i) const;

	/**
	 * Set the weight of the interaction of a pair of criteria..
	 * @param i               Index of the first criteria.
	 * @param j               Index of the second criteria.
	 * @param weight          Weight of the interaction.
    */
	void SetInteraction(size_t i,size_t j,double weight);

	/**
	 * Get the weight of the interaction of a pair of criteria..
	 * @param i               Index of the first criteria.
	 * @param j               Index of the second criteria.
    * @return the weight of the interaction.
    */
	double GetInteraction(size_t i,size_t j) const;

	/**
	 * Compute the 2-Additive Choquet integral of an object defined by a set of
	 * criteria stored in a container.
    * @param obj             Values of the criteria for the objects.
    * @return the 2-Additive Choquet integral.
	 * \exception std::range_error is generated if the container has not a value
	 * for each criteria.
	 * \exception std::invalid_argument is generated if the sum of parameters is
	 * not equal to 1.
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
