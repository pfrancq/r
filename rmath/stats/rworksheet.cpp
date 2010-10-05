/*

	R Project Library

	RWorksheet.cpp

	Worksheet - Implementation.

	Copyright 2010 by Pascal Francq (pascal@francq.info).

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
// include files for ANSI C/C++
#include <math.h>


//------------------------------------------------------------------------------
// include files for R project
#include <rworksheet.h>
#include <rcursor.h>
using namespace R;
using namespace std;



//------------------------------------------------------------------------------
//
// class RWorksheet
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
RWorksheet::RWorksheet(const RString& name,const RString& colname)
	: Name(name), ColName(colname), LineLabels(20), ColLabels(20), Values(20), NbLines(0)
{
}


//------------------------------------------------------------------------------
RVector& RWorksheet::Verify(size_t col,size_t line)
{
	if(LineLabels.GetMaxPos()<line)
		ThrowRException("Line "+RString::Number(line)+" is outside [0,"+RString::Number(LineLabels.GetMaxPos())+"]");
	if(ColLabels.GetMaxPos()<col)
		ThrowRException("Col "+RString::Number(col)+" is outside [0,"+RString::Number(ColLabels.GetMaxPos())+"]");
	RVector& Col(*Values[col]);
	Col.Verify(line+1);
	return(Col);
}


//------------------------------------------------------------------------------
void RWorksheet::AddCol(size_t col,const RString& label)
{
	ColLabels.InsertPtrAt(new RString(label),col);
	Values.InsertPtrAt(new RVector(20),col);
}


//------------------------------------------------------------------------------
void RWorksheet::AddLine(size_t line,const RString& label)
{
	// Verify the sizes
	LineLabels.VerifyTab(line+1);
	Values.VerifyTab(line+1);

	// if no label is assign, put the current one
	if((!LineLabels.GetMaxPos())||(LineLabels.GetMaxPos()<line)||(!LineLabels[line]))
	{
		NbLines++;
		LineLabels.InsertPtrAt(new RString(label),line);
	}
}


//------------------------------------------------------------------------------
void RWorksheet::InvalidValue(size_t col,size_t line)
{
	Verify(col,line)[line]=NAN;
}


//------------------------------------------------------------------------------
double RWorksheet::GetValue(size_t col,size_t line)
{
	return(Verify(col,line)[line]);
}


//------------------------------------------------------------------------------
void RWorksheet::SetValue(size_t col,size_t line,double val)
{
	Verify(col,line)[line]=val;
}


//------------------------------------------------------------------------------
void RWorksheet::AddValue(size_t col,size_t line,double val)
{
	Verify(col,line)[line]+=val;
}


//------------------------------------------------------------------------------
void RWorksheet::SubtractValue(size_t col,size_t line,double val)
{
	Verify(col,line)[line]-=val;
}


//------------------------------------------------------------------------------
void RWorksheet::MultiplyValue(size_t col,size_t line,double val)
{
	Verify(col,line)[line]*=val;
}


//------------------------------------------------------------------------------
void RWorksheet::DivideValue(size_t col,size_t line,double val)
{
	Verify(col,line)[line]/=val;
}


//------------------------------------------------------------------------------
const RVector& RWorksheet::GetCol(size_t col) const
{
	if(ColLabels.GetMaxPos()<col)
		ThrowRException("Col "+RString::Number(col)+"is outside [0,"+RString::Number(ColLabels.GetMaxPos())+"]");
	return(*Values[col]);
}


//------------------------------------------------------------------------------
RVector& RWorksheet::GetCol(size_t col)
{
	if(ColLabels.GetMaxPos()<col)
		ThrowRException("Col "+RString::Number(col)+"is outside [0,"+RString::Number(ColLabels.GetMaxPos())+"]");
	return(*Values[col]);
}


//------------------------------------------------------------------------------
void RWorksheet::Save(RTextFile& file,bool headers,RChar sep)
{
	// The Labels
	RCursor<RString> Cols(ColLabels);

	// Write the headers if necessary
	if(headers)
	{
		RString Line("\""+ColName+"\"");  // First column are the names
		for(Cols.Start();!Cols.End();Cols.Next())
			Line+=sep+"\""+(*Cols())+"\"";
		file<<Line<<endl;
	}

	// Write the lines
	RCursor<RString> Lines(LineLabels);
	for(Lines.Start();!Lines.End();Lines.Next())
	{
		RString Line("\""+(*Lines())+"\"");
		for(Cols.Start();!Cols.End();Cols.Next())
			Line+=sep+"\""+RString::Number(Verify(Cols.GetPos(),Lines.GetPos())[Lines.GetPos()])+"\"";
		file<<Line<<endl;
	}
}
