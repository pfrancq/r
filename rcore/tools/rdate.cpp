/*

	R Project Library

	RDate.cpp

	Date - Implementation.

	Copyright 2001-2003 by the Universit�Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).

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
// include files for ANSI C/C++
#include <stdlib.h>
#include <time.h>


//------------------------------------------------------------------------------
// include files for R Project
#include <rstd.h>
#include <rdate.h>
#include <rstring.h>
using namespace R;



//------------------------------------------------------------------------------
//
// class RDate
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// static instance
RDate RDate::null(1,1,1970,0,0,0);


//------------------------------------------------------------------------------
RDate::RDate(void)
{
	SetToday();
}


//------------------------------------------------------------------------------
RDate::RDate(const RDate& src)
	: Year(src.Year), Month(src.Month), Day(src.Day), Hour(src.Hour), Minute(src.Minute),
	  Second(src.Second)
{
}


//------------------------------------------------------------------------------
RDate::RDate(char day,char month,int year,char hour,char minute,char second)
	: Year(0), Month(0), Day(0), Hour(0), Minute(0), Second(0)
{
	SetDate(day,month,year,hour,minute,second);
}


//------------------------------------------------------------------------------
RDate::RDate(const RString& date)
	: Year(0), Month(0), Day(0), Hour(0), Minute(0), Second(0)
{
	SetDate(date);
}


//------------------------------------------------------------------------------
int RDate::Compare(const RDate& d) const
{
	if(Year!=d.Year) return(Year-d.Year);
	if(Month!=d.Month) return(Month-d.Month);
	if(Day!=d.Day) return(Day-d.Day);
	if(Hour!=d.Hour) return(Hour-d.Hour);
	if(Minute!=d.Minute) return(Minute-d.Minute);
	return(Second-d.Second);
}


//------------------------------------------------------------------------------
int RDate::Compare(const RDate* d) const
{
	if(Year!=d->Year) return(Year-d->Year);
	if(Month!=d->Month) return(Month-d->Month);
	if(Day!=d->Day) return(Day-d->Day);
	if(Hour!=d->Hour) return(Hour-d->Hour);
	if(Minute!=d->Minute) return(Minute-d->Minute);
	return(Second-d->Second);
}


//------------------------------------------------------------------------------
RDate& RDate::operator=(const RDate& date)
{
	Year=date.Year;
	Month=date.Month;
	Day=date.Day;
	Hour=date.Hour;
	Minute=date.Minute;
	Second=date.Second;
	return(*this);
}


//------------------------------------------------------------------------------
bool RDate::operator==(const RDate& d) const
{
	return((Year==d.Year)&&(Month==d.Month)&&(Day==d.Day)&&(Hour==d.Hour)&&(Minute==d.Minute)&&(Second==d.Second));
}


//------------------------------------------------------------------------------
bool RDate::operator!=(const RDate& d) const
{
	return((Year!=d.Year)||(Month!=d.Month)||(Day!=d.Day)||(Hour!=d.Hour)||(Minute!=d.Minute)||(Second!=d.Second));
}


//------------------------------------------------------------------------------
bool RDate::operator<(const RDate& d) const
{
	int diff;

	diff=Year-d.Year;
	if(diff<0) return(true);
	if(diff>0) return(false);
	diff=Month-d.Month;
	if(diff<0) return(true);
	if(diff>0) return(false);
	diff=Day-d.Day;
	if(diff<0) return(true);
	if(diff>0) return(false);
	diff=Hour-d.Hour;
	if(diff<0) return(true);
	if(diff>0) return(false);
	diff=Minute-d.Minute;
	if(diff<0) return(true);
	if(diff>0) return(false);
	return(Second<d.Second);
}


//------------------------------------------------------------------------------
bool RDate::operator<=(const RDate& d) const
{
	int diff;

	diff=Year-d.Year;
	if(diff<0) return(true);
	if(diff>0) return(false);
	diff=Month-d.Month;
	if(diff<0) return(true);
	if(diff>0) return(false);
	diff=Day-d.Day;
	if(diff<0) return(true);
	if(diff>0) return(false);
	diff=Hour-d.Hour;
	if(diff<0) return(true);
	if(diff>0) return(false);
	diff=Minute-d.Minute;
	if(diff<0) return(true);
	if(diff>0) return(false);
	return(Second<=d.Second);
}


//------------------------------------------------------------------------------
bool RDate::operator>(const RDate& d) const
{
	int diff;

	diff=Year-d.Year;
	if(diff>0) return(true);
	if(diff<0) return(false);
	diff=Month-d.Month;
	if(diff>0) return(true);
	if(diff<0) return(false);
	diff=Day-d.Day;
	if(diff>0) return(true);
	if(diff<0) return(false);
	diff=Hour-d.Hour;
	if(diff>0) return(true);
	if(diff<0) return(false);
	diff=Minute-d.Minute;
	if(diff>0) return(true);
	if(diff<0) return(false);
	return(Second>d.Second);
}


//------------------------------------------------------------------------------
bool RDate::operator>=(const RDate& d) const
{
	int diff;

	diff=Year-d.Year;
	if(diff>0) return(true);
	if(diff<0) return(false);
	diff=Month-d.Month;
	if(diff>0) return(true);
	if(diff<0) return(false);
	diff=Day-d.Day;
	if(diff>0) return(true);
	if(diff<0) return(false);
	diff=Hour-d.Hour;
	if(diff>0) return(true);
	if(diff<0) return(false);
	diff=Minute-d.Minute;
	if(diff>0) return(true);
	if(diff<0) return(false);
	return(Second>=d.Second);
}


//------------------------------------------------------------------------------
RDate::operator RString () const
{
	return(itou(Year)+"-"+itou(Month)+"-"+itou(Day)+" "+itou(Hour)+":"+itou(Minute)+":"+itou(Second));
}


//------------------------------------------------------------------------------
RString RDate::ToString(void) const
{
	return(operator RString());
}


//------------------------------------------------------------------------------
void RDate::SetToday(void)
{
	time_t now;
	struct tm *l_time;

	now=time((time_t *)0);
	l_time = localtime(&now);
	Year=l_time->tm_year+1900;
	Month=l_time->tm_mon+1;
	Day=l_time->tm_mday;
	Hour=l_time->tm_hour;
	Minute=l_time->tm_min;
	Second=l_time->tm_sec;
}


//------------------------------------------------------------------------------
void RDate::SetDate(char day,char month,int year,char hour,char minute,char second)
{
	// Verify the values
	if((month<1)||(month>12)||(day<1)||(day>31)||(hour<0)||(hour>23)||(minute<0)||(minute>59)||(second<0)||(second>59))
		throw RException("Not a valid date");
	Year=year;
	Month=month;
	Day=day;
	Hour=hour;
	Minute=minute;
	Second=second;
}


//------------------------------------------------------------------------------
void RDate::SetDate(const RString& date)
{
	const RChar* ptr;
	char* begin;
	char num[10];
	int year;
	char month,day,hour,minute,second;

	if(!date.IsEmpty())
	{
		ptr=date();

		// Read Year
		begin=num;
		while((!ptr->IsNull())&&((*ptr)!='-'))
		{
			(*(begin++)) = ptr->Latin1();
			ptr++;
		}
		(*begin)=0;
		if(!ptr->IsNull())
			ptr++; // Skip "-"
		year=atoi(num);
		if(year<1000)
		{
			if(year<70) year+=1900; else year+=2000;
		}

		// Read Month
		begin=num;
		while((!ptr->IsNull())&&((*ptr)!='-'))
		{
			(*(begin++)) = ptr->Latin1();
			ptr++;
		}
		(*begin)=0;
		if(!ptr->IsNull())
			ptr++; // Skip "-"
		month=atoi(num);

		// Read Day
		begin=num;
		while((!ptr->IsNull())&&(!ptr->IsSpace()))
		{
			(*(begin++)) = ptr->Latin1();
			ptr++;
		}
		(*begin)=0;
		day=atoi(num);

		// Skip spaces
		while((!ptr->IsNull())&&(ptr->IsSpace()))
			ptr++;

		// Read Hour
		begin=num;
		while((!ptr->IsNull())&&((*ptr)!=':'))
		{
			(*(begin++)) = ptr->Latin1();
			ptr++;
		}
		(*begin)=0;
		if(!ptr->IsNull())
			ptr++; // Skip ":"
		hour=atoi(num);

		// Read Minute
		begin=num;
		while((!ptr->IsNull())&&((*ptr)!=':'))
		{
			(*(begin++)) = ptr->Latin1();
			ptr++;
		}
		(*begin)=0;
		if(!ptr->IsNull())
			ptr++; // Skip ":"
		minute=atoi(num);

		// Read Second
		begin=num;
		while((!ptr->IsNull()))
		{
			(*(begin++)) = ptr->Latin1();
			ptr++;
		}
		(*begin)=0;
		if(!ptr->IsNull())
			ptr++; // Skip ":"
		second=atoi(num);

		// Set the date
		SetDate(day,month,year,hour,minute,second);
	}
	else
		SetToday();
}


//------------------------------------------------------------------------------
RDate RDate::GetToday(void)
{
	return(RDate());
}


//------------------------------------------------------------------------------
RDate::~RDate(void)
{
}
