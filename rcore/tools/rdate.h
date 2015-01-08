/*

	R Project Library

	RDate.h

	Date - Header.

	Copyright 2001-2015 by Pascal Francq (pascal@francq.info).
	Copyright 2001-2008 by the Université Libre de Bruxelles (ULB).

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
// forward declaration
class RString;


//------------------------------------------------------------------------------
/**
* The RDate class provides a representation for a date.
* @code
* #include <rdate.h>
* using namespace R;
*
* RDate Date(30,12,2005,12,12,11);
* cout<<Date.ToString()<<endl;
* @endcode
* @author Pascal Francq
* @short Date.
*/
class RDate
{
	/**
	* Year
	*/
	int Year;

	/**
	* Month.
	*/
	char Month;

	/**
	* Day.
	*/
	char Day;

	/**
	* Hour.
	*/
	char Hour;

	/**
	* Minute.
	*/
	char Minute;

	/**
	* Second.
	*/
	char Second;

public:

	/**
	* Construct a date. The current date is used.
	*/
	RDate(void);

	/**
	* Copy constructor.
	*/
	RDate(const RDate& src);

	/**
	* Construct a date.
	* @param day             Day.
	* @param month           Month.
	* @param year            Year.
	* @param hour            Hour.
	* @param minute          Minute.
	* @param second          Second.
	*/
	RDate(char day,char month,int year,char hour=0,char minute=0,char second=0);

	/**
	* Construct a date. If string is null, then the current date is used.
	* @param date            String representing a date in YYYY-MM-DD HH:MM:SS
	*                        or YY-MM-DD HH:MM:SS.
	*/
	RDate(const RString& date);

	/**
	* Lexically compares two dates and returns an integer less than, equal
	* to, or greater than zero if this is less than, equal to, or greater than
	* d.
	* @param d               Date to compare with.
	* @see R::RContainer.
	*/
	int Compare(const RDate& d) const;

	/**
	* Assignment operator using another date.
	*/
	RDate& operator=(const RDate& date);

	/**
	* Equal operator.
	*/
	bool operator==(const RDate& d) const;

	/**
	 * Difference operator.
	 */
	bool operator!=(const RDate& d) const;

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
	*/
	char GetDay(void) const {return(Day);}

	/**
	* Get the month of the date.
	*/
	char GetMonth(void) const {return(Month);}

	/**
	* Get the year of the date.
	*/
	int GetYear(void) const {return(Year);}

	/**
	* Get the hour of the date.
	*/
	char GetHour(void) const {return(Hour);}

	/**
	* Get the minute of the date.
	*/
	char GetMinute(void) const {return(Minute);}

	/**
	* Get the second of the date.
	*/
	char GetSecond(void) const {return(Second);}

	/**
	* Get a string representring the date.
	*/
	operator RString () const;

	/**
	* Get a string representring the date.
	*/
	RString ToString(void) const;

	/**
	* Set the date.
	* @param day             Day.
	* @param month           Month.
	* @param year            Year.
	* @param hour            Hour.
	* @param minute          Minute.
	* @param second          Second.
	*/
	void SetDate(char day,char month,int year,char hour=0,char minute=0,char second=0);

	/**
	* Set the date. If string is null, then the current date is used.
	* @param date            String representing a date in YYYY-MM-DD HH:MM:SS
	*                        or YY-MM-DD HH:MM:SS.
	*/
	void SetDate(const RString& date);

	/**
	* Set the date to the one given by the system.
	*/
	void SetToday(void);

	/**
	* Get the current date.
	*/
	static RDate GetToday(void);

	/**
	* Destruct the date.
	*/
	~RDate(void);

	/**
	* Represents a null date.
	*/
	static RDate Null;
};


}  //-------- End of namespace R -----------------------------------------------


//------------------------------------------------------------------------------
#endif
