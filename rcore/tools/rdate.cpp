/*

	R Project Library

	RDate.cpp

	Date - Implementation.

	(c) 2001 by P. Francq.

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



//-----------------------------------------------------------------------------
// include files for R Project
#include <rtimedate/rdate.h>
using namespace RTimeDate;



//-----------------------------------------------------------------------------
//
// class RDate
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
RTimeDate::RDate::RDate(void)
{
	time_t now;
	struct tm *l_time;

	now=time((time_t *)0);
	l_time = localtime(&now);
	Year=l_time->tm_year+1900;
	Month=l_time->tm_mon+1;
	Day=l_time->tm_mday;
}


//-----------------------------------------------------------------------------
RTimeDate::RDate::RDate(const char day,const char month,const int year)
	: Day(0), Month(0), Year(0)
{
	Day=day;
	Month=month;
	Year=year;
}


//-----------------------------------------------------------------------------
int RTimeDate::RDate::Compare(const RDate& d) const
{
	if(Year!=d.Year) return(Year-d.Year);
	if(Month!=d.Month) return(Month-d.Month);
	if(Day!=d.Day) return(Day-d.Day);
	return(0);
}


//-----------------------------------------------------------------------------
int RTimeDate::RDate::Compare(const RDate* d) const
{
	if(Year!=d->Year) return(Year-d->Year);
	if(Month!=d->Month) return(Month-d->Month);
	if(Day!=d->Day) return(Day-d->Day);
	return(0);
}


//-----------------------------------------------------------------------------
bool RTimeDate::RDate::operator==(const RDate& d) const
{
	return((Year==d.Year)&&(Month==d.Month)&&(Day=d.day));
}


//-----------------------------------------------------------------------------
bool RTimeDate::RDate::operator<(const RDate& d) const
{
	int diff;

	diff=Year-d.Year;
	if(diff<0) return(true);
	if(!diff)
	{
		diff=Month-d.Month;
		if(diff<0) return(true);
		if(!diff)
			return(Day<d.Day);
	}
	return(false);
}


//-----------------------------------------------------------------------------
bool RTimeDate::RDate::operator<=(const RDate& d) const
{
	int diff;

	diff=Year-d.Year;
	if(diff<0) return(true);
	if(!diff)
	{
		diff=Month-d.Month;
		if(diff<0) return(true);
		if(!diff)
			return(Day<=d.Day);
	}
	return(false);
}


//-----------------------------------------------------------------------------
bool RTimeDate::RDate::operator>(const RDate& d) const
{
	int diff;

	diff=Year-d.Year;
	if(diff>0) return(true);
	if(!diff)
	{
		diff=Month-d.Month;
		if(diff>0) return(true);
		if(!diff)
			return(Day>d.Day);
	}
	return(false);
}


//-----------------------------------------------------------------------------
bool RTimeDate::RDate::operator>=(const RDate& d) const
{
	int diff;

	diff=Year-d.Year;
	if(diff>0) return(true);
	if(!diff)
	{
		diff=Month-d.Month;
		if(diff>0) return(true);
		if(!diff)
			return(Day>=d.Day);
	}
	return(false);
}


//-----------------------------------------------------------------------------
RTimeDate::RDate::~RDate(void)
{
}
