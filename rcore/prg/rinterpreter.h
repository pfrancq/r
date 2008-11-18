/*

	R Project Library

	RInterpreter.h

	Interpreter - Header.

	Copyright 2008 by the Université Libre de Bruxelles.

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
#ifndef RInterpreterH
#define RInterpreterH


//------------------------------------------------------------------------------
// include files for R Project
#include <rtextfile.h>
#include <rstack.h>
#include <rcursor.h>
#include <rprgscope.h>
#include <rprgoutput.h>


//------------------------------------------------------------------------------
namespace R{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// forward class declaration
class RPrgInst;
class RPrgInstBlock;
class RPrgVar;
class RPrgClass;
class RPrg;
class RInterpreter;


//------------------------------------------------------------------------------
/**
* The RPrgException class provides an exception generated by the program.
* @author Pascal Francq
* @short Program Exception.
*/
class RPrgException : public RException
{
public:

	/**
	* Construct an exception.
	* @param str             Message of the error.
	*/
	RPrgException(const RInterpreter* kernel,const char* str) throw();
};


//------------------------------------------------------------------------------
/**
* The RInterpreter provides a class for executing a script program.
* @author Pascal Francq
* @short Script Program.
*/
class RInterpreter
{
protected:

	/**
	* Generic output.
	*/
	RPrgOutput* Cout;

	/**
	* List of all "Classes" defined in the program.
	*/
	R::RContainer<RPrgClass,true,false> Classes;

	/**
	 * Temporary LIFO Stack of instructions blocks (used for analyze).
	 */
	R::RStack<RPrgInstBlock,false,true,true> tmpBlocks;

	/**
	 * LIFO Stack of scopes.
	 */
	R::RStack<RPrgScope,true,true,true> Scopes;

	/**
	 * Next character to treat.
	 */
	const RChar* Char;

	/**
	 * Number of the line where the last instruction begins.
	 */
	size_t Line;

	/**
	 * Name of the file.
	 */
	RString File;

public:

	/**
	* Constructor of a program.
	* @param o              Output.
	*/
	RInterpreter(RPrgOutput* o);

	/**
	* Run a program.
	* @param uri             URI of the program.
	*/
	void Run(const RURI& uri);

	/**
	* Execute a block of instructions.
	* @param insts           Instruction.
	*/
	void Exec(const RString& insts);

protected:

	/**
	* Execute the instructions.
	*/
	void Exec(void);

	/**
	 * Return a string of the form "filename(line)" representing the current
	 * element treated.
	 */
	RString WhatTreated(void) const;

	/**
	* Analyze the parameters.
	* @param values         Values of the parameter.
	*/
	void AnalyseParam(RContainer<RPrgVar,true,false>& values);

	/**
	 * Read a literal value.
	 */
	RString ReadLiteral(void);

	/**
	 * Treat end of line
	 * @param dbl            Must the line contains a double point.
	 */
	void Eol(bool dbl);

	/**
	* Load the script file.
	*/
	void TreatLine(size_t depth,RString line);

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
	 * Add an instruction.
	 * @param inst           Instruction.
	 */
	void AddInst(RPrgInst* inst);

	/**
	 * Find a variable by searching in all local blocks.
	 * @param name           Name of the variable.
	 * @return Pointer to the variable or 0 if not found.
	 */
	RPrgVar* Find(const RString& name) const;

	/**
	* Get the value of the variable.
	* @param var            Name of the variable.
	*/
	virtual RString GetValue(const RString& var);

	/**
	 *
	 */
	void AddClass(RPrgClass* c);

	/**
	 * Get the next word at the line containing the instruction.
	 */
	RString GetWord(void);

	/**
	 * Get the next token at the line containing the instruction.
	 */
	RString GetToken(const RString chars);

	/**
	 * Line number of the current instruction.
	 */
	size_t GetLine(void) const {return(Line);}

	/**
	 *
	 * @return
	 */
	void SkipSpaces(void);

public:

	/**
	* Get a cursor over the classes.
	*/
	RCursor<RPrgClass> GetClasses(void) const;

	/**
	* Destruct of the program.
	*/
	virtual ~RInterpreter(void);

	// friend class
	friend class RPrg;
	friend class RPrgVarRef;
	friend class RPrgInstFor;
	friend class RPrgInstBlock;
	friend class RPrgInstMethod;
	friend class RPrgInstNew;
	friend class RPrgInstDelete;
	friend class RPrgInstAssignVar;
	friend class RPrgException;
};


}  //-------- End of namespace R -----------------------------------------------


//------------------------------------------------------------------------------
#endif
