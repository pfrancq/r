/*

	R Project Library

	RWorksheet.h

	Worksheet - Header.

	Copyright 2010-2015 by Pascal Francq (pascal@francq.info).

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
#ifndef RWorksheet_H
#define RWorksheet_H


//------------------------------------------------------------------------------
// include files for R Project
#include <rstring.h>
#include <rcontainer.h>
#include <rnumcontainer.h>
#include <rtextfile.h>


//------------------------------------------------------------------------------
namespace R{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
 * The RWorksheet class provides a representation for a worksheet, i.a. a
 * two-dimensional array of data.
 *
 * In practice, it is represented by a set of lines representing different
 * items for which different measures (columns) are computed.
 * @short Worksheet
  */
class RWorksheet
{
	/**
	 * Name of the statistics.
	 */
	RString Name;

	/**
	 * Name of the first column.
	 */
	RString ColName;

	/**
	 * Labels of the Lines.
	 */
	RContainer<RString,true,false> LineLabels;

	/**
	 * Labels of the Cols.
	 */
	RContainer<RString,true,false> ColLabels;

	/**
	 * The values stored as columns of numbers.
	 */
	RContainer<RNumContainer<double,false>,true,false> Values;

	/**
	 * Number of lines.
	 */
	size_t NbLines;

protected:

	/**
	 * Verify that a given value is initialized.
	 * @param col            Column.
	 * @param line           Line.
	 * @return Reference to the vector corresponding to the column.
	 */
	inline RNumContainer<double,false>& Verify(size_t col,size_t line);

public:

	/**
	 * Constructor.
	 * @param name           Name of the statistics.
	 * @param colname        Name of the first column.
	 */
	RWorksheet(const RString& name,const RString& colname=RString::Null);

	/**
	 * @return number of lines added.
	 */
	inline size_t GetNbLines(void) {return(NbLines);}

	/**
	 * Add a column to the statistics.
	 * @param col            Column to add.
	 * @param label          Label to assign.
	 * @warning The column 0 corresponds to the first values and not the labels
	 * of the lines.
	 */
	void AddCol(size_t col,const RString& label);

	/**
	 * Add a line to the statistics.
	 * @param line           Line to add.
	 * @param label          Label to assign if it is a new line.
	 */
	void AddLine(size_t line,const RString& label);

	/**
	 * Invalid a given value.
	 * @param col            Column.
	 * @param line           Line.
	 */
	void InvalidValue(size_t col,size_t line);

	/**
	 * Get a given value.
	 * @param col            Column.
	 * @param line           Line.
	 */
	double GetValue(size_t col,size_t line);

	/**
	 * Set a given value.
	 * @param col            Column.
	 * @param line           Line.
	 * @param val            Value.
	 */
	void SetValue(size_t col,size_t line,double val);

	/**
	 * Add a given value.
	 * @param col            Column.
	 * @param line           Line.
	 * @param val            Value.
	 */
	void AddValue(size_t col,size_t line,double val);

	/**
	 * Subtract a given value.
	 * @param col            Column.
	 * @param line           Line.
	 * @param val            Value.
	 */
	void SubtractValue(size_t col,size_t line,double val);

	/**
	 * Multiply a given value.
	 * @param col            Column.
	 * @param line           Line.
	 * @param val            Value.
	 */
	void MultiplyValue(size_t col,size_t line,double val);

	/**
	 * Divide a given value.
	 * @param col            Column.
	 * @param line           Line.
	 * @param val            Value.
	 */
	void DivideValue(size_t col,size_t line,double val);

	/**
	 * Get the values of a column.
	 * @param col            Column.
	 * @return a reference to the vector.
	 */
	const RNumContainer<double,false>& GetCol(size_t col) const;

	/**
	 * Get the values of a column.
	 * @param col            Column.
	 * @return a reference to the vector.
	 */
	RNumContainer<double,false>& GetCol(size_t col);

	/**
	 * Save the statistics in an opened file as CVS file.
	 * @param file           File where to store.
	 * @param headers        Must the headers be stored.
	 * @param sep            Separate character.
	 */
	void Save(RTextFile& file,bool headers=true,RChar sep=';');
};


}  //------- End of namespace R ------------------------------------------------


//------------------------------------------------------------------------------
#endif
