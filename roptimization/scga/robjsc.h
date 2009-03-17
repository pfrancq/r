/*

	R Project Library

	RObjSC.h

	Similarity-based Clustering Object- Header.

	Copyright 2002-2009 by the Université Libre de Bruxelles.

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
#ifndef RObjSC_H
#define RObjSC_H


//------------------------------------------------------------------------------
// include files for R Project
#include <robjg.h>
#include <rcursor.h>
#include <rcontainer.h>


//------------------------------------------------------------------------------
namespace R{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* The RObjSC class provides a representation of an object to group by the
* similarity-based clustering algorithm.
* @author Pascal Francq
* @short Similarity-based Clustering Object.
*/
class RObjSC : public RObjG
{
	/*
	 * A close object.
	 */
	class CloseObj
	{
	public:

		/**
		 * Identifier of the object.
		 */
		size_t Id;

		/*
		 * Ratio.
		 */
		double Ratio;

		/**
		 * Constructor.
		 * @param id         Identifier.
		 * @param ratio      Ratio.
		 */
		CloseObj(size_t id,double ratio) : Id(id), Ratio(ratio) {}

		/**
		* Compare two ratios.
		*/
		int Compare(const CloseObj& ratio) const {return(static_cast<int>(Ratio-ratio.Ratio));}

		/**
		* Static function used to order the object by ratio.
		* @param a              Pointer to the first object.
		* @param b              Pointer to the second object.
		*/
		static int sortOrder(const void* a,const void* b);
	};

	/**
	 * All close objects.
	 */
	RContainer<CloseObj,true,false> CloseObjs;

protected:

	/**
	 * Can the element be grouped in a group alone?
	 */
	bool Social;

	/**
	 * Identifier of a common parent between elements that could prevent to
	 * group them together.
	 */
	size_t ParentId;

	/**
	 * Must the close objects be re-ordered.
	 */
	bool MustReOrder;

public:

	/**
	* Construct the object.
	* @param id             Identifier.
	* @param name           Name of the object.
	* @param social         Is the object social.
	* @param parentid       Identifier of the parent (if any).
	*/
	RObjSC(const size_t id,const RString& name,bool social=true,size_t parentid=0);

	/**
	 * See if the element is social or not.
	 */
	inline bool IsSocial(void) const {return(Social);}

	/**
	 * Get the identifier of the parent or 0 if there is no parent.
	 */
	inline size_t GetParentId(void) const {return(ParentId);}

	/**
	 * Add a close object with the current one.
	 * @param id             Identifier of the object.
	 * @param ratio          Ratio representing the closeness.
	 */
	void AddCloseObject(size_t id,double ratio);

	/**
	 * Find the best group for the object based on the current clustering and
	 * the ratio.
	 * @param groups         Clustering.
	 * @param grp            Variable that will hold the pointer to the group
	 *                       (or 0 if no group was found).
	 */
	template<class cGroup,class cGroups>
		void FindBestGroup(cGroups* groups,cGroup* &grp)
	{
		if(MustReOrder)
		{
			MustReOrder=false;
			CloseObjs.ReOrder(CloseObj::sortOrder);
		}
		RCursor<CloseObj> Best(CloseObjs);
		for(Best.Start(),grp=0;(!Best.End())&&(!grp);Best.Next())
			grp=groups->GetGroup(Best()->Id);
	}

	/**
	* Destruct the object.
	*/
	virtual ~RObjSC(void);
};


}  //------- End of namespace R ------------------------------------------------


//------------------------------------------------------------------------------
#endif
