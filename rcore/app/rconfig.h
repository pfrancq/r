/*

	R Project Library

	RConfig.h

	XML Structure representing a configuration - Header.

	Copyright 2003-2006 by the Université libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).

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
* a set of parameters.
* @author Pascal Francq
* @short XML Config Structure.
*/
class RConfig
{
	class lParams : public RContainer<RParam,true,true>
	{
	public:
		RString Cat;
		lParams(const RString& cat);
		int Compare(const lParams& params) const;
		int Compare(const RString& params) const;
	};

	/**
	* Category of the config.
	*/
	RString Category;

	/**
	* Name of the config.
	*/
	RString Name;

	/**
	* Parameters.
	*/
	RContainer<lParams,true,true> Params;

public:

	/**
	* Constructor. The category is used to specify where to search. Examples:
	* - "app", for an application.
	* - "lib/galilei" for a library.
	* - "lib/galilei/plugins/profiles" for a specific type of plugin of a
	*  specifc library 'galilei'.
	* @param cat             Category of the config.
	* @param name            Name of the config.
	*/
	RConfig(const RString& cat=RString::Null,const RString& name=RString::Null);

private:

	/**
	* Analyse a tag that is supposed to represent a parameter if a given
	* category.
	* @param tag             Tag.
	* @param cat             Category.
	*/
	void Analyse(RXMLTag* tag,lParams* cat);

public:

	/**
	* Set the information about the configuration.
	* @param cat             Category of the config.
	* @param name            Name of the config.
	*/
	void SetConfigInfos(const RString& cat,const RString& name);

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
	* Get the name of the config.
	*/
	RString GetName(void) const {return(Name);}

	/**
	* Get the category of the config.
	*/
	RString GetCategory(void) const {return(Category);}

	/**
	* Insert a new parameter in the configuration.
	* @param param           Parameter.
	* @param cat             Category of the parameter.
	*/
	void InsertParam(RParam* param,const RString& cat=RString::Null);

	/**
	* Find a given parameter of a given category.
	* @param name            Name of the parameter.
	* @param cat             Category of the parameter.
	* @return Pointer or null if not found.
	*/
	template<class T> T* FindParam(const RString& name,const RString& cat=RString::Null)
	{
		lParams* c=Params.GetPtr(cat);
		if(!c)
			return(0);
		return(dynamic_cast<T*>(c->GetPtr(name)));
	}

	/**
	* Reset a parameter with a given name and from a given category.
	* @param name            Name of the parameter.
	* @param cat             Category of the parameter.
	* @return Pointer or null if not found.
	*/
 	void Reset(const RString& name,const RString& cat=RString::Null);

	/**
	* Get the content of the parameter.
	* @param name            Name of the parameter.
	* @param cat             Category of the parameter.
	* @return C string.
	*/
	RString Get(const RString& name,const RString& cat=RString::Null);

	/**
	* Get the content of the parameter.
	* @param name            Name of the parameter.
	* @param cat             Category of the parameter.
	* @return int.
	*/
	int GetInt(const RString& name,const RString& cat=RString::Null);

	/**
	* Get the content of the parameter.
	* @param name            Name of the parameter.
	* @param cat             Category of the parameter.
	* @return unsigned int.
	*/
	unsigned int GetUInt(const RString& name,const RString& cat=RString::Null);

	/**
	* Get the content of the parameter.
	* @param name            Name of the parameter.
	* @param cat             Category of the parameter.
	* @return double.
	*/
	double GetDouble(const RString& name,const RString& cat=RString::Null);

	/**
	* Get the content of the parameter.
	* @param name            Name of the parameter.
	* @param cat             Category of the parameter.
	* @return bool.
	*/
	bool GetBool(const RString& name,const RString& cat=RString::Null);

	/**
	* Get the content of a list.
	* @param name            Name of the list.
	* @param cat             Category of the list.
	* @return Cursor of string.
	*/
	RCursor<RString> GetList(const RString& name,const RString& cat=RString::Null);

	/**
	* Get the content of a structure.
	* @param name            Name of the structure.
	* @param cat             Category of the structure.
	* @return Cursor of parameters.
	*/
	RCursor<RParam> GetStruct(const RString& name,const RString& cat=RString::Null);

	/**
	* Set the content of the parameter.
	* @param name            Name of the parameter.
	* @param v               Value of the parameter.
	* @param cat             Category of the parameter.
	*/
	void Set(const RString& name,const RString& v,const RString& cat=RString::Null);

	/**
	* Set the content of the parameter.
	* @param name            Name of the parameter.
	* @param v               Value of the parameter.
	* @param cat             Category of the parameter.
	*/
	void SetInt(const RString& name,int v,const RString& cat=RString::Null);

	/**
	* Set the content of the parameter.
	* @param name            Name of the parameter.
	* @param v               Value of the parameter.
	* @param cat             Category of the parameter.
	*/
	void SetUInt(const RString& name,unsigned int v,const RString& cat=RString::Null);

	/**
	* Set the content of the parameter.
	* @param name            Name of the parameter.
	* @param v               Value of the parameter.
	* @param cat             Category of the parameter.
	*/
	void SetDouble(const RString& name,double v,const RString& cat=RString::Null);

	/**
	* Set the content of the parameter.
	* @param name            Name of the parameter.
	* @param v               Value of the parameter.
	* @param cat             Category of the parameter.
	*/
	void SetBool(const RString& name,bool v,const RString& cat=RString::Null);

	/**
	* Add a value to a list.
	* @param name            Name of the parameter.
	* @param v               Value of the parameter.
	* @param cat             Category of the parameter.
	*/
	void AddToList(const RString& name,const RString& v,const RString& cat=RString::Null);

	/**
	* Add a parameter to a structure.
	* @param name            Name of the structure.
	* @param param           Parameter.
	* @param cat             Category of the structure.
	*/
	void AddToStruct(const RString& name,RParam* param,const RString& cat=RString::Null);

	/**
	* Destructor.
	*/
	virtual ~RConfig(void);
};


}  //-------- End of namespace R ----------------------------------------------


//-----------------------------------------------------------------------------
#endif
