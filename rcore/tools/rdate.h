/*

	R Project Library

	RDate.h

	Date - Header.

	Copyright 2001-2003 by the Université Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).

	Version $Revision$

	Last Modify: $Date$

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
#ifndef RDate_H
#define RDate_H


//------------------------------------------------------------------------------
namespace R{
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
/**
* The RDate class provides a representation for a date.
* @author Pascal Francq
* @short Date.
*/
class RDate
{
	/**
	* The day.
	*/
	int Day;

	/**
	* The month.
	*/
	int Month;

	/**
	* The Year
	*/
	int Year;

public:

	/**
	* Construct a date.
	*/
	RDate(void);

	/**
	* Construct a date.
	* @param day                      The day.
	* @param month                    The month.
	* @param year                     The year.
	*/
	RDate(const int day,const int month,const int year);

	/**
	* Construct a date. If string is null, then the current date is used.
	* @param date                     String representing a date in (yy-mm-dd).
	*/
	RDate(const char* date);

	/**
	* Compare function like strcmp used in particular for RStd::RContainer class.
	*/
	int Compare(const RDate& d) const;

	/**
	* Compare function like strcmp used in particular for RStd::RContainer class.
	*/
	int Compare(const RDate* d) const;

	/**
	* Assignment operator using another date.
	*/
	RDate& operator=(const RDate& date);

	/**
	* Equal operator.
	*/
	bool operator==(const RDate& d) const;

	/**
	* Less operator.
	*/
	bool operator<(const RDate& d) const;

	/**
	* Less or equal operator.
	*/
	bool operator<=(const RDate& d) const;

	/**
	* Greather operator.
	*/
	bool operator>(const RDate& d) const;

	/**
	* Greather or equal operator.
	*/
	bool operator>=(const RDate& d) const;

	/**
	* Get the day of the date.
	* @returns Integer.
	*/
	int GetDay(void) const {return(Day);}

	/**
	* Get the month of the date.
	* @returns Integer.
	*/
	int GetMonth(void) const {return(Month);}

	/**
	* Get the year of the date.
	* @returns Integer.
	*/
	int GetYear(void) const {return(Year);}

	/**
	* Set the date to the one given by the system.
	*/
	void SetToday(void);

	/**
	* Need to manage temporary dates.
	*/
	static RDate* GetDate(void);

	/**
	* Get the current date.
	*/
	static RDate& GetToday(void);

	/**
	* Destruct the date.
	*/
	~RDate(void);

	/**
	* Represents a null date.
	*/
	static RDate null;
};


}  //-------- End of namespace R -----------------------------------------------


//------------------------------------------------------------------------------
#endif
