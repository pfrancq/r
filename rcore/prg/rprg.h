/*

	R Project Library

	RPrg.h

	Generic script program - Header.

	Copyright 2002-2003 by the Universit�Libre de Bruxelles.

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



//------------------------------------------------------------------------------
#ifndef RPrgH
#define RPrgH


//------------------------------------------------------------------------------
// include files for R Project
#include <rtextfile.h>
#include <rcontainer.h>
#include <rcursor.h>


//------------------------------------------------------------------------------
namespace R{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// forward class declaration
class RPrgOutput;
class RPrgInst;
class RPrgVar;
class RPrgClass;


//------------------------------------------------------------------------------
/**
* The RPrg provides a class for executing a script program.
* @author Pascal Francq
* @short Script Program.
*/
class RPrg
{
protected:

	/**
	* Name of the file to execute.
	*/
	R::RString FileName;

	/**
	* Generic output.
	*/
	R::RPrgOutput* Cout;

	/**
	* List of all "Instructions" to execute.
	*/
	R::RContainer<RPrgInst,true,false> Insts;

	/**
	* List of all "Variables" defined in the program.
	*/
	R::RContainer<RPrgVar,true,false> Vars;

	/**
	* List of all "Classes" defined in the program.
	*/
	R::RContainer<RPrgClass,true,false> Classes;

	/**
	* Program file.
	*/
	R::RTextFile Prg;

	/**
	* Temporary string.
	*/
	RString buf;

	/**
	* Must a line be readed;
	*/
	bool ReadLine;

public:

	/**
	* Constructor of a program.
	* @param f              Name of the file.
	* @param o              Output.
	*/
	RPrg(R::RString f,RPrgOutput* o);

	/**
	* Load the script file.
	*/
	void Load(void);

protected :

	/**
	* Count the number of tabs at the begin of a line of source code.
	* @param line           Line to analyze.
	* @returns unsigned int.
	*/
	static unsigned int CountTabs(const RString& line);

public:

	/**
	* Analyse a line of source code.
	* @param prg            File containing the program.
	* @returns Instruction to insert.
	*/
	RPrgInst* AnalyseLine(R::RTextFile& prg);

	/**
	* Analyse a parameter.
	* @param params         Parameter to analyse.
	* @param values         Values of the paramter.
	*/
	static void AnalyseParam(const RString& params,RContainer<RPrgVar,true,false>* values);

	/**
	* Execute a "program".
	*/
	void Exec(void);

	/**
	* Add a variable.
	* @param var             Pointer to the variable.
	*/
	void AddVar(RPrgVar* var);

	/**
	* Remove a variable.
	* @param var             Pointer to the variable.
	*/
	void DelVar(RPrgVar* var);

	/**
	* Get the value of the variable.
	* @param var            Name of the variable.
	*/
	virtual RString GetValue(const RString& var);

	/**
	* Add a class.
	* @param c               Pointer to the class.
	*/
	void AddVar(RPrgClass* c);

	/**
	* Remove a class.
	* @param c               Pointer to the class.
	*/
	void DelVar(RPrgClass* c);

	/**
	* Get a cursor over the classes.
	*/
	RCursor<RPrgClass> GetClasses(void) const;

	/**
	* Destructor of the program.
	*/
	virtual ~RPrg(void);
};


}  //-------- End of namespace R -----------------------------------------------


//------------------------------------------------------------------------------
#endif
