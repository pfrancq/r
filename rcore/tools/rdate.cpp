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
// include files for ANSI C/C++
#include <stdlib.h>
#include <time.h>


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
// static instance
RDate RTimeDate::RDate::null(0,0,0);


//-----------------------------------------------------------------------------
RTimeDate::RDate::RDate(void)
{
	SetToday();
}


//-----------------------------------------------------------------------------
RTimeDate::RDate::RDate(const int day,const int month,const int year)
	: Day(day), Month(month), Year(year)
{
}


//-----------------------------------------------------------------------------
RTimeDate::RDate::RDate(const char* date)
	: Day(0), Month(0), Year(0)
{
	const char* ptr;
	char* begin;
	char num[10];

	if(date)
	{
		ptr=date;

		// Read Year
		begin=num;
		while((*ptr)&&((*ptr)!='-'))
			(*(begin++)) = (*(ptr++));
		(*begin)=0;
		ptr++; // Skip "-"
		Year=atol(num);
		if(Year<1000)
		{
			if(Year<70) Year+=1970; else Year+=2000;
		}

		// Read Month
		begin=num;
		while((*ptr)&&((*ptr)!='-'))
			(*(begin++)) = (*(ptr++));
		(*begin)=0;
		ptr++; // Skip "-"
		Month=atol(num);

		// Read Year
		begin=num;
		while(*ptr)
			(*(begin++)) = (*(ptr++));
		(*begin)=0;
		Day=atol(num);
	}
}


//-----------------------------------------------------------------------------
int RTimeDate::RDate::Compare(const RDate& d) const
{
	if(Year!=d.Year) return(Year-d.Year);
	if(Month!=d.Month) return(Month-d.Month);
	return(Day-d.Day);
}


//-----------------------------------------------------------------------------
int RTimeDate::RDate::Compare(const RDate* d) const
{
	if(Year!=d->Year) return(Year-d->Year);
	if(Month!=d->Month) return(Month-d->Month);
	return(Day-d->Day);
}


//-----------------------------------------------------------------------------
RDate& RTimeDate::RDate::operator=(const RDate& date)
{
	Year=date.Year;
	Month=date.Month;
	Day=date.Day;
	return(*this);
}


//-----------------------------------------------------------------------------
bool RTimeDate::RDate::operator==(const RDate& d) const
{
	return((Year==d.Year)&&(Month==d.Month)&&(Day==d.Day));
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
void RTimeDate::RDate::SetToday(void)
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
RTimeDate::RDate::~RDate(void)
{
}
