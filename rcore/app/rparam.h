/*

	R Project Library

	RParam.h

	Parameter - Header.

	Copyright 2003-2009 by the Pascal Francq (pascal@francq.info).
	Copyright 2003-2008 by the Université Libre de Bruxelles (ULB).

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
#ifndef RParamH
#define RParamH


//-----------------------------------------------------------------------------
// include files for R Project
#include <rcontainer.h>
#include <rxmltag.h>


//-----------------------------------------------------------------------------
namespace R{
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/**
* The RParam class provides a representation for a parameter.
* @author Pascal Francq
* @short Parameter.
*/
class RParam
{
protected:

	/**
	* Name of the parameter.
	*/
	RString Name;

	/**
	 * Description of the parameter.
	 */
	RString Description;

public:

	/**
	* Constructor of the parameter.
	* @param n               Name of the parameter.
	* @param desc            Description of the parameter.
	*/
	RParam(const RString& n,const RString& desc=RString::Null);

	/**
	* Get the name of the parameter.
	*/
	RString GetName(void) const {return(Name);}

	/**
	* Get the description of the parameter.
	*/
	RString GetDescription(void) const {return(Description);}

	/**
	* Compare the name of the parameter with a given string. This function is
	* used by RContainer.
	* @param name           The string used for the comparison.
	* @returns -1,0 or 1 if the parameter is greater, the same or lesser than
	* the parameter.
	*/
	int Compare(const RString& name) const;

	/**
	* Compare the names of two parameters. This function is used by RContainer.
	* @param param          The parameter used for the comparison.
	* @returns -1,0 or 1 if the parameter is greater, the same or lesser than
	* the parameter.
	*/
	int Compare(const RParam& param) const;

	/**
	* Add a tag representing the values of the parameter.
	* @param xml             XML structure.
	* @param parent          Parent tag.
	*/
	virtual void AddTag(RXMLStruct* xml,RXMLTag* parent);

	/**
	* Set the parameter based on a tag.
	* @param parent          Tag.
	* @return true if the tag correspond to the current parameter or false if
	* not.
	*/
	virtual bool Set(RXMLTag* parent);

	/**
	* Reset the value of the parameter.
	*/
	virtual void Reset(void);

	/**
	* Destruct.
	*/
	virtual ~RParam(void);
};


//-----------------------------------------------------------------------------
/**
* The RParamValue class provides a representation for a parameter with an unique
* value.
* @author Pascal Francq
* @short Only-one value of parameter.
*/
class RParamValue : public RParam
{
	/**
	* Value of the parameter.
	*/
	RString Value;

public:

	/**
	* Constructor of the parameter.
	* @param tag             Tag;
	*/
	RParamValue(const RXMLTag* tag);

	/**
	* Constructor of the parameter with a 'value' attribute.
	* @param n               Name of the parameter.
	* @param v               Value.
	* @param desc            Description of the parameter.
	*/
	RParamValue(const RString& n,const RString& v,const RString& desc=RString::Null);

	/**
	* Constructor of the parameter with a 'value' attribute.
	* @param n               Name of the parameter.
	* @param v               Value.
	* @param desc            Description of the parameter.
	*/
	RParamValue(const RString& n,const char* v,const RString& desc=RString::Null);

	/**
	* Constructor of the parameter with a 'value' attribute.
	* @param n               Name of the parameter.
	* @param v               Value.
	* @param desc            Description of the parameter.
	*/
	RParamValue(const RString& n,long v,const RString& desc=RString::Null);

	/**
	* Constructor of the parameter with a 'value' attribute.
	* @param n               Name of the parameter.
	* @param v               Value.
	* @param desc            Description of the parameter.
	*/
	RParamValue(const RString& n,unsigned long v,const RString& desc=RString::Null);

	/**
	* Constructor of the parameter with a 'value' attribute.
	* @param n               Name of the parameter.
	* @param v               Value.
	* @param desc            Description of the parameter.
	*/
	RParamValue(const RString& n,int v,const RString& desc=RString::Null);

	/**
	* Constructor of the parameter with a 'value' attribute.
	* @param n               Name of the parameter.
	* @param v               Value.
	* @param desc            Description of the parameter.
	*/
	RParamValue(const RString& n,unsigned int v,const RString& desc=RString::Null);

	/**
	* Constructor of the parameter with a 'value' attribute.
	* @param n               Name of the parameter.
	* @param v               Value.
	* @param desc            Description of the parameter.
	*/
	RParamValue(const RString& n,double v,const RString& desc=RString::Null);

	/**
	* Constructor of the parameter with a 'value' attribute.
	* @param n               Name of the parameter.
	* @param v               Value.
	* @param desc            Description of the parameter.
	*/
	RParamValue(const RString& n,bool v,const RString& desc=RString::Null);

	/**
	* Get the content of the parameter.
	* @return int.
	*/
	int GetInt(void) const;

	/**
	* Get the content of the parameter.
	*/
	unsigned int GetUInt(void) const;

	/**
	* Get the content of the parameter.
	* @return int.
	*/
	long GetLong(void) const;

	/**
	* Get the content of the parameter.
	*/
	unsigned long GetULong(void) const;

	/**
	* Get the content of the parameter.
	* @return double.
	*/
	double GetDouble(void) const;

	/**
	* Get the content of the parameter.
	* @return string.
	*/
	RString Get(void) const;

	/**
	* Get the content of the parameter.
	* @return bool.
	*/
	bool GetBool(void) const;

	/**
	* Set the parameter based on a tag.
	* @param parent          Tag.
	* @return true if the tag correspond to the current parameter or false if
	* not.
	*/
	virtual bool Set(RXMLTag* parent);

	/**
	* Set the content of the parameter.
	* @param v               Value of the parameter.
	*/
	void SetInt(long long int v);

	/**
	* Set the content of the parameter.
	* @param v               Value of the parameter.
	*/
	void SetUInt(unsigned long long int v);

	/**
	* Set the content of the parameter.
	* @param v               Value of the parameter.
	*/
	void SetDouble(double v);

	/**
	* Set the content of the parameter.
	* @param v               Value of the parameter.
	*/
	void Set(const RString& v);

	/**
	* Set the content of the parameter.
	* @param v               Value of the parameter.
	*/
	void SetBool(bool v);

	/**
	* Add a tag representing the values of the parameter.
	* @param xml             XML structure.
	* @param parent          Parent tag.
	*/
	virtual void AddTag(RXMLStruct* xml,RXMLTag* parent);

	/**
	* Reset the value of the parameter.
	*/
	virtual void Reset(void);
};


//-----------------------------------------------------------------------------
/**
* The RParamList class provides a representation for a parameter with multiple
* value.
* @author Pascal Francq
* @short List of parameter's values.
*/
class RParamList : public RParam
{
	/**
	* Values of the paramter.
	*/
	RContainer<RString,true,false> Values;

public:

	/**
	* Constructor of the parameter.
	* @param tag             Tag.
	*/
	RParamList(const RXMLTag* tag);

	/**
	* Constructor of the parameter.
	* @param name            Name.
	* @param desc            Description of the parameter.
	*/
	RParamList(const RString& name,const RString& desc=RString::Null);

	/**
	* Return the values.
	*/
	RCursor<RString> GetList(void) const;

	/**
	* Get the position of a particular value.
	* @param value           Value to search for.
	* @return Position or SIZE_MAX if the value does not exist.
	*/
	size_t GetPos(const RString& value) const;

	/**
	* Set the parameter based on a tag.
	* @param parent          Tag.
	* @return true if the tag correspond to the current parameter or false if
	* not.
	*/
	virtual bool Set(RXMLTag* parent);

	/**
	* Add a new value to the list.
	* @param value           Value.
	*/
	void Insert(const RString& value);

	/**
	* Add a tag representing the values of the parameter.
	* @param xml             XML structure.
	* @param parent          Parent tag.
	*/
	virtual void AddTag(RXMLStruct* xml,RXMLTag* parent);

	/**
	* Reset the values of the parameter.
	*/
	virtual void Reset(void);
};


//-----------------------------------------------------------------------------
/**
* The RParamStruct class provides a representation for a structure of multiple
* parameters.
* @author Pascal Francq
* @short Structure of Parameters.
*/
class RParamStruct : public RParam
{
	/**
	* Paramters.
	*/
	RContainer<RParam,true,true> Parameters;

public:

	/**
	* Constructor of the parameter.
	* @param tag             Tag.
	*/
	RParamStruct(const RXMLTag* tag);

	/**
	* Constructor of the parameter.
	* @param name            Name.
	* @param desc            Description of the parameter.
	*/
	RParamStruct(const RString& name,const RString& desc=RString::Null);

	/**
	* Return the parameters.
	*/
	RCursor<RParam> GetStruct(void) const;

	/**
	* Set the parameter based on a tag.
	* @param parent          Tag.
	* @return true if the tag correspond to the current parameter or false if
	* not.
	*/
	virtual bool Set(RXMLTag* parent);

	/**
	* Add a new parameter to the list.
	* @param param           Parameter to add.
	*/
	void Insert(const RParam* param);

	/**
	* Get a parameter with a given name.
	* @param name            Name of the parameter.
	*/
	template<class T> T* Get(const RString& name) const
	{
		return(dynamic_cast<T*>(Parameters.GetPtr(name)));
	}

	/**
	* Add a tag representing the values of the parameter.
	* @param xml             XML structure.
	* @param parent          Parent tag.
	*/
	virtual void AddTag(RXMLStruct* xml,RXMLTag* parent);

	/**
	* Reset the parameters of this structure.
	*/
	virtual void Reset(void);
};


}  //-------- End of namespace R ----------------------------------------------


//-----------------------------------------------------------------------------
#endif
