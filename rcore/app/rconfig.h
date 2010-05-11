/*

	R Project Library

	RConfig.h

	XML Structure representing a configuration - Header.

	Copyright 2003-2010 by the Pascal Francq (pascal@francq.info).
	Copyright 2003-2008 by the Université Libre de Bruxelles (ULB).

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



//-----------------------------------------------------------------------------
#ifndef RConfigH
#define RConfigH


//-----------------------------------------------------------------------------
// include files for R Project
#include <rxmlstruct.h>
#include <rparam.h>


//-----------------------------------------------------------------------------
namespace R{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/**
* The RConfig class provides a representation for a XML structure representing
* a set of parameters that can load from and store to a file.
*
* The parameters should be declare (with the InsertParam method) before the
* structure is loaded. Parameters that are in the file but not declared are
* lost.
* @author Pascal Francq
* @short XML Configuration Structure.
*/
class RConfig
{
	class lParams;

	/**
	* Category of the configuration.
	*/
	RString Category;

	/**
	* Name of the configuration.
	*/
	RString Name;

	/**
	* Root of all parameters.
	*/
	lParams* Root;

public:

	/**
	* Constructor. The category is used to specify where to search. Examples:
	* - "app", for an application.
	* - "lib/galilei" for a library.
	* - "lib/galilei/plugins/profiles" for a specific type of plug-in of a
	*   specific library 'galilei'.
	* @param cat             Category of the config.
	* @param name            Name of the config ('/' are replaced by '-').
	*/
	RConfig(const RString& cat=RString::Null,const RString& name=RString::Null);

private:

	/**
	* Analyze a tag that is supposed to represent a parameter if a given
	* category.
	* @param tag             Tag.
	* @param cat             Category.
	*/
	void Analyze(RXMLTag* tag,lParams* cat);

public:

	/**
	* Set the information about the configuration.
	* @param cat             Category of the config.
	* @param name            Name of the config ('/' are replaced by '-').
	*/
	void SetConfigInfos(const RString& cat,const RString& name);

	/**
	 * Load a given category (and its parameters and sub-categories).
	 * @param cat            Category to load.
	 * @param tag            Tag coming from the XML file.
	 */
	void Load(lParams* cat,RXMLTag* tag);

	/**
	 * Save a given category (and its parameters and sub-categories).
	 * @param cat            Category to save.
	 * @param config         XML configuration file.
	 * @param parent         Parent tag in the configuration.
	 */
	void Save(lParams* cat,RXMLStruct& config,RXMLTag* parent);

public:

	/**
	* Load a file in the structure.
	* @param global          Global configuration file must be searched?
	*/
	void Load(bool global=true);

	/**
	* Save a file in the structure.
	*/
	void Save(void);

	/**
	* Get the name of the configuration.
	*/
	RString GetName(void) const {return(Name);}

	/**
	* Get the category of the configuration.
	*/
	RString GetCategory(void) const {return(Category);}

private:

	/**
	* Find a given parameter.
	* @param name            Name of the parameter.
	* @param cat             Category of the parameter.
	* @param subcat          Sub-category of the parameter.
	* @param subsubcat       Sub-Sub-category of the parameter.
	* @return Pointer or null if not found.
	*/
	RParam* GetParam(const RString& name,const RString& cat,const RString& subcat,const RString& subsubcat);

public:

	/**
	* @return a cursor over the parameters.
	* @param cat             Category of the parameter.
	* @param subcat          Sub-category of the parameter.
	* @param subsubcat       Sub-Sub-category of the parameter.
	*/
	RCursor<RParam> GetParams(const RString& cat=RString::Null,const RString& subcat=RString::Null,const RString& subsubcat=RString::Null);

	/**
	* Fill a container with the categories name.
	* @param cats            Container that will be filled (it is emptied by
	*                        the method).
	* @param cat             Category of the parameter.
	* @param subcat          Sub-category of the parameter.
	*/
	void GetCategories(RContainer<RString,true,false>& cats,const RString& cat=RString::Null,const RString& subcat=RString::Null);

	/**
	* Find a given parameter (template version).
	* @tparam T              Type of the parameter.
	* @param name            Name of the parameter.
	* @param cat             Category of the parameter.
	* @param subcat          Sub-category of the parameter.
	* @param subsubcat       Sub-Sub-category of the parameter.
	* @return Pointer or null if not found.
	*/
	template<class T> T* FindParam(const RString& name,const RString& cat=RString::Null,const RString& subcat=RString::Null,const RString& subsubcat=RString::Null)
	{
		return(dynamic_cast<T*>(GetParam(name,cat,subcat,subsubcat)));
	}

	/**
	* Declare a new parameter. If it exist, the parameter passed as argument is
	* deleted.
	* @param param           Parameter.
	* @param cat             Category of the parameter.
	* @param subcat          Sub-category of the parameter.
	* @param subsubcat       Sub-Sub-category of the parameter.
	*/
	void InsertParam(RParam* param,const RString& cat=RString::Null,const RString& subcat=RString::Null,const RString& subsubcat=RString::Null);

	/**
	* Reset a parameter with a given name and from a given category.
	* @param name            Name of the parameter.
	* @param cat             Category of the parameter.
	* @param subcat          Sub-category of the parameter.
	* @param subsubcat       Sub-Sub-category of the parameter.
	*/
 	void Reset(const RString& name,const RString& cat=RString::Null,const RString& subcat=RString::Null,const RString& subsubcat=RString::Null);

	/**
	* @return the content of the parameter.
	* @param name            Name of the parameter.
	* @param cat             Category of the parameter.
	* @param subcat          Sub-category of the parameter.
	* @param subsubcat       Sub-Sub-category of the parameter.
	*/
	RString Get(const RString& name,const RString& cat=RString::Null,const RString& subcat=RString::Null,const RString& subsubcat=RString::Null);

	/**
	* @return the content of the parameter.
	* @param name            Name of the parameter.
	* @param cat             Category of the parameter.
	* @param subcat          Sub-category of the parameter.
	* @param subsubcat       Sub-Sub-category of the parameter.
	*/
	int GetInt(const RString& name,const RString& cat=RString::Null,const RString& subcat=RString::Null,const RString& subsubcat=RString::Null);

	/**
	* @return the content of the parameter.
	* @param name            Name of the parameter.
	* @param cat             Category of the parameter.
	* @param subcat          Sub-category of the parameter.
	* @param subsubcat       Sub-Sub-category of the parameter.
	*/
	unsigned int GetUInt(const RString& name,const RString& cat=RString::Null,const RString& subcat=RString::Null,const RString& subsubcat=RString::Null);

	/**
	* @return the content of the parameter.
	* @param name            Name of the parameter.
	* @param cat             Category of the parameter.
	* @param subcat          Sub-category of the parameter.
	* @param subsubcat       Sub-Sub-category of the parameter.
	*/
	long GetLong(const RString& name,const RString& cat=RString::Null,const RString& subcat=RString::Null,const RString& subsubcat=RString::Null);

	/**
	* @return the content of the parameter.
	* @param name            Name of the parameter.
	* @param cat             Category of the parameter.
	* @param subcat          Sub-category of the parameter.
	* @param subsubcat       Sub-Sub-category of the parameter.
	*/
	unsigned long GetULong(const RString& name,const RString& cat=RString::Null,const RString& subcat=RString::Null,const RString& subsubcat=RString::Null);

	/**
	* Get the content of the parameter.
	* @param name            Name of the parameter.
	* @param cat             Category of the parameter.
	* @param subcat          Sub-category of the parameter.
	* @param subsubcat       Sub-Sub-category of the parameter.
	* @return double.
	*/
	double GetDouble(const RString& name,const RString& cat=RString::Null,const RString& subcat=RString::Null,const RString& subsubcat=RString::Null);

	/**
	* @return the content of the parameter.
	* @param name            Name of the parameter.
	* @param cat             Category of the parameter.
	* @param subcat          Sub-category of the parameter.
	* @param subsubcat       Sub-Sub-category of the parameter.
	*/
	bool GetBool(const RString& name,const RString& cat=RString::Null,const RString& subcat=RString::Null,const RString& subsubcat=RString::Null);

	/**
	* Get the content of a list.
	* @param name            Name of the list.
	* @param cat             Category of the list.
	* @param subcat          Sub-category of the parameter.
	* @param subsubcat       Sub-Sub-category of the parameter.
	* @return Cursor of string.
	*/
	RCursor<RString> GetList(const RString& name,const RString& cat=RString::Null,const RString& subcat=RString::Null,const RString& subsubcat=RString::Null);

	/**
	* Get the content of a structure.
	* @param name            Name of the structure.
	* @param cat             Category of the structure.
	* @param subcat          Sub-category of the parameter.
	* @param subsubcat       Sub-Sub-category of the parameter.
	* @return Cursor of parameters.
	*/
	RCursor<RParam> GetStruct(const RString& name,const RString& cat=RString::Null,const RString& subcat=RString::Null,const RString& subsubcat=RString::Null);

	/**
	* Set the content of the parameter.
	* @param name            Name of the parameter.
	* @param v               Value of the parameter.
	* @param cat             Category of the parameter.
	* @param subcat          Sub-category of the parameter.
	* @param subsubcat       Sub-Sub-category of the parameter.
	*/
	void Set(const RString& name,const RString& v,const RString& cat=RString::Null,const RString& subcat=RString::Null,const RString& subsubcat=RString::Null);

	/**
	* Set the content of the parameter.
	* @param name            Name of the parameter.
	* @param v               Value of the parameter.
	* @param cat             Category of the parameter.
	* @param subcat          Sub-category of the parameter.
	* @param subsubcat       Sub-Sub-category of the parameter.
	*/
	void SetInt(const RString& name,int v,const RString& cat=RString::Null,const RString& subcat=RString::Null,const RString& subsubcat=RString::Null);

	/**
	* Set the content of the parameter.
	* @param name            Name of the parameter.
	* @param v               Value of the parameter.
	* @param cat             Category of the parameter.
	* @param subcat          Sub-category of the parameter.
	* @param subsubcat       Sub-Sub-category of the parameter.
	*/
	void SetUInt(const RString& name,size_t v,const RString& cat=RString::Null,const RString& subcat=RString::Null,const RString& subsubcat=RString::Null);

	/**
	* Set the content of the parameter.
	* @param name            Name of the parameter.
	* @param v               Value of the parameter.
	* @param cat             Category of the parameter.
	* @param subcat          Sub-category of the parameter.
	* @param subsubcat       Sub-Sub-category of the parameter.
	*/
	void SetDouble(const RString& name,double v,const RString& cat=RString::Null,const RString& subcat=RString::Null,const RString& subsubcat=RString::Null);

	/**
	* Set the content of the parameter.
	* @param name            Name of the parameter.
	* @param v               Value of the parameter.
	* @param cat             Category of the parameter.
	* @param subcat          Sub-category of the parameter.
	* @param subsubcat       Sub-Sub-category of the parameter.
	*/
	void SetBool(const RString& name,bool v,const RString& cat=RString::Null,const RString& subcat=RString::Null,const RString& subsubcat=RString::Null);

	/**
	* Add a value to a list.
	* @param name            Name of the parameter.
	* @param v               Value of the parameter.
	* @param cat             Category of the parameter.
	* @param subcat          Sub-category of the parameter.
	* @param subsubcat       Sub-Sub-category of the parameter.
	*/
	void AddToList(const RString& name,const RString& v,const RString& cat=RString::Null,const RString& subcat=RString::Null,const RString& subsubcat=RString::Null);

	/**
	* Add a parameter to a structure.
	* @param name            Name of the structure.
	* @param param           Parameter.
	* @param cat             Category of the structure.
	* @param subcat          Sub-category of the parameter.
	* @param subsubcat       Sub-Sub-category of the parameter.
	*/
	void AddToStruct(const RString& name,RParam* param,const RString& cat=RString::Null,const RString& subcat=RString::Null,const RString& subsubcat=RString::Null);

	/**
	* Destructor.
	*/
	virtual ~RConfig(void);
};


}  //-------- End of namespace R ----------------------------------------------


//-----------------------------------------------------------------------------
#endif
