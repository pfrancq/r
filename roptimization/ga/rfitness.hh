/*

	Rainbow Library Project

	RFitness.hh

	Fitness for Chromsomes of Genetic Algorithms - Inline Implementation

	(C) 1998-2000 by P. Francq.

	Version $Revision$

	Last Modify: $Date$

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



//---------------------------------------------------------------------------
//
// RFitness<cVal,bool>
//
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
template<class cVal,bool Max>
	RFitness<cVal,Max>::RFitness(void)
{
	Value=0;
}


//---------------------------------------------------------------------------
template<class cVal,bool Max>
	inline RFitness<cVal,Max>& RFitness<cVal,Max>::operator=(const RFitness &f)
{
	Value=f.Value;
	return(*this);
}


//---------------------------------------------------------------------------
template<class cVal,bool Max>
	inline RFitness<cVal,Max>& RFitness<cVal,Max>::operator=(const cVal value)
{
	Value=value;
	return(*this);
}


//---------------------------------------------------------------------------
template<class cVal,bool Max>
	inline bool RFitness<cVal,Max>::operator==(const RFitness &f)
{
	return(Value==f.Value);
}


//---------------------------------------------------------------------------
template<class cVal,bool Max>
	inline bool RFitness<cVal,Max>::operator!=(const RFitness &f)
{
	return(Value!=f.Value);
}


//---------------------------------------------------------------------------
template<class cVal,bool Max>
	inline bool RFitness<cVal,Max>::operator>(const RFitness &f)
{
	if(Max)
		return(Value>f.Value);
	else
		return(Value<f.Value);
}


//---------------------------------------------------------------------------
template<class cVal,bool Max>
	inline bool RFitness<cVal,Max>::operator<(const RFitness &f)
{
	if(Max)
		return(Value<f.Value);
	else
		return(Value>f.Value);
}


//---------------------------------------------------------------------------
template<class cVal,bool Max>
	inline bool ToMaximize(void)
{
	return(Max);
}


//---------------------------------------------------------------------------
template<class cVal,bool Max>
	inline bool ToMinimize(void)
{
	return(!Max);
}