/*

	R Project Library

	RMatrixStrorage.cpp

	Matrix Storage - Implementation.

	Copyright 2009-2015 by Pascal Francq (pascal@francq.info).

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
// include files for R Project
#include <rmatrixstorage.h>
#include <rmatrix.h>
#include <rsparsematrix.h>
#include <rmaxmatrix.h>
using namespace R;
using namespace std;


//------------------------------------------------------------------------------
// Constants
const off_t ValRec=sizeof(double)+2*sizeof(size_t)+2*sizeof(off_t);



//------------------------------------------------------------------------------
//
//    Class RMatrixStorage
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
RMatrixStorage::RMatrixStorage(void)
	: File1(), File2(), Index(), BaseURI(), NbLines(0), NbCols(0)
{
}


//------------------------------------------------------------------------------
void RMatrixStorage::Open(const RString& baseuri,RGenericMatrix::tType type)
{
	BaseURI=baseuri;
	Type=type;

	// Open the Index
	Index.Open(BaseURI+".info",RIO::ReadWrite);
	Index.Seek(0);
	if(!Index.End())
		Index>>NbLines>>NbCols;

	// Open the other files
	switch(Type)
	{
		case RGenericMatrix::tNormal:
			File1.Open(BaseURI+".lower",RIO::ReadWrite);
			File2.Open(BaseURI+".upper",RIO::ReadWrite);
			Upper=true;
			Sparse=false;
			Max=false;
			break;
		case RGenericMatrix::tLowerTriangular:
		case RGenericMatrix::tSymmetric:
			File1.Open(BaseURI+".lower",RIO::ReadWrite);
			Upper=false;
			Sparse=false;
			Max=false;
			break;
		case RGenericMatrix::tSparse:
		case RGenericMatrix::tSparseSymmetric:
			File1.Open(BaseURI+".rec",RIO::ReadWrite);
			Upper=false;
			Sparse=true;
			Max=false;
			break;
		case RGenericMatrix::tMax:
			File1.Open(BaseURI+".rec",RIO::ReadWrite);
			Upper=false;
			Sparse=false;
			Max=true;
			break;
		default:
			throw RException("RMatrixStorage::Open(const RURI&,RGenericMatrix::tType) : Type "+RString::Number(type)+" is not supported");
	}
}


//------------------------------------------------------------------------------
void RMatrixStorage::Close(void)
{
	File1.Close();
	File2.Close();
	Index.Seek(0);
	Index<<NbLines<<NbCols;
	Index.Close();
	BaseURI=RString::Null;
	NbLines=0;
	NbCols=0;
}


//------------------------------------------------------------------------------
inline size_t RMatrixStorage::GetLowerPos(size_t i,size_t j)
{
	if((NbLines>=NbCols)&&(i>NbCols))
		return((NbCols*(NbCols+1))/2+(i-NbCols)*NbCols+j);
	return((i*(i+1))/2+j);
}


//------------------------------------------------------------------------------
inline size_t RMatrixStorage::GetUpperPos(size_t i,size_t j)
{
	if((NbCols>=NbLines)&&(j>NbLines))
		return((NbLines*(NbLines+1))/2+(j-1-NbLines)*NbLines+i);
	return((j*(j-1))/2+i);
}


//------------------------------------------------------------------------------
size_t RMatrixStorage::GetNbCols(size_t line) const
{
	if(line>NbLines)
		throw std::range_error("RMatrixStorage::GetNbCols(size_t) : index "+RString::Number(line)+" outside range [0,"+RString::Number(NbLines)+"]");

	if(Max)
	{
		size_t nbcols;
		RMatrixStorage* ptr(const_cast<RMatrixStorage*>(this));
		ptr->Index.Seek(2*sizeof(size_t)+line*(4*sizeof(off_t)+sizeof(size_t))+4*sizeof(off_t));
		ptr->Index>>nbcols;
		return(nbcols);
	}
	return(NbCols);
}


//------------------------------------------------------------------------------
void RMatrixStorage::Clear(double val)
{
	if(Sparse||Max)
	{
		// File has no records anymore
		File1.Truncate(0);

		// Each column and each line has no values
		Index.Truncate(2*sizeof(size_t));
		Index.Seek(2*sizeof(size_t));
		for(size_t i=GetMaxDim()+1;--i;)
		{
			Index<<MaxOffT<<MaxOffT<<MaxOffT<<MaxOffT;
			if(Max)
				Index<<static_cast<size_t>(0);
		}
	}
	else
	{
		// Write val for each element of File1
		File1.Seek(0);
		while(!File1.End())
			File1<<val;

		if(Upper)
		{
			// Write val for each element of File1
			File2.Seek(0);
			while(!File1.End())
				File2<<val;
		}
	}
}


//------------------------------------------------------------------------------
void RMatrixStorage::Init(double val)
{
	if(Sparse||Max)
	{
		if(val==0.0)
			Clear(val);
		else
		{
			for(size_t i=0;i<NbLines;i++)
				for(size_t j=0;j<NbCols;j++)
					Write(i,j,val);
		}
	}
	else
	{
		Clear(val);
	}
}


//------------------------------------------------------------------------------
void RMatrixStorage::InitLine(size_t line,double val)
{
	for(size_t j=0;j<NbCols;j++)
		Write(line,j,val);
}


//------------------------------------------------------------------------------
void RMatrixStorage::InitCol(size_t col,double val)
{
	for(size_t i=0;i<NbLines;i++)
		Write(i,col,val);
}


//------------------------------------------------------------------------------
void RMatrixStorage::ReCreateLowerFile(size_t newlines,size_t newcols,double val)
{
	size_t i,j; // variables used to parse elements of the matrix

	// Create a new file that will store the new results
	RBinaryFile NewFile1;
	NewFile1.Open(BaseURI+".lower_new",RIO::Create);

	// Read element (i,j) in the existing file
	File1.Seek(0);
	size_t maxcols; // Maximum number of columns to read
	for(i=0;(i<NbLines)&&(i<newlines);i++)
	{
		if(i<NbCols)
			maxcols=i+1;
		else
			maxcols=NbCols;

		// Read all the existing columns
		for(j=0;j<maxcols;j++)
		{
			double tmp;
			File1>>tmp;

			// If the element read is valid in the new matrix -> Write it.
			if(j<newcols)
				NewFile1<<tmp;
		}

		// Look if new columns must be added for the current line in the new file
		if(i<newcols)
			maxcols=i+1;
		else
			maxcols=newcols;

		// Add columns if necessary by filling it with the default value
		for(;j<maxcols;j++)
			NewFile1<<val;
	}

	// Look if new lines  must be added to the new file
	for(;i<newlines;i++)
	{
		if(i<newcols)
			maxcols=i+1;
		else
			maxcols=newcols;

		// Add columns by filling it with the default value
		for(size_t j=0;j<maxcols;j++)
			NewFile1<<val;
	}

	// Replace File1 by NewFile1
	File1.Close();
	NewFile1.Close();
	RFile::RemoveFile(BaseURI+".lower");
	RFile::RenameFile(BaseURI+".lower_new",BaseURI+".lower");
	File1.Open(BaseURI+".lower",RIO::ReadWrite);
}


//------------------------------------------------------------------------------
void RMatrixStorage::ReCreateUpperFile(size_t newlines,size_t newcols,double val)
{
	size_t i,j; // variables used to parse elements of the matrix

	// Create a new file that will store the new results
	RBinaryFile NewFile2;
	NewFile2.Open(BaseURI+".upper_new",RIO::Create);

	// Read element (i,j) in the existing file
	File2.Seek(0);
	size_t maxlines; // Maximum number of lines to read
	for(j=1;(j<NbCols)&&(j<newcols);j++)
	{
		if(j<NbLines)
			maxlines=j;
		else
			maxlines=NbLines;

		// Read all the existing lines
		for(i=0;i<maxlines;i++)
		{
			double tmp;
			File2>>tmp;

			// If the element read is valid in the new matrix -> Write it.
			if(i<newlines)
				NewFile2<<tmp;
		}

		// Look if new columns must be added for the current line in the new file
		if(j<newcols)
			maxlines=j;
		else
			maxlines=newcols;

		// Add lines if necessary by filling it with the default value
		for(;i<maxlines;i++)
			NewFile2<<val;
	}

	// Look if new columns must be added to the new file
	for(;j<newcols;j++)
	{
		if(j<newcols)
			maxlines=j;
		else
			maxlines=newcols;

		// Add lines by filling it with the default value
		for(i=0;i<maxlines;i++)
			NewFile2<<val;
	}

	// Replace File2 by NewFile2
	File2.Close();
	NewFile2.Close();
	RFile::RemoveFile(BaseURI+".upper");
	RFile::RenameFile(BaseURI+".upper_new",BaseURI+".upper");
	File2.Open(BaseURI+".upper",RIO::ReadWrite);
}


//------------------------------------------------------------------------------
void RMatrixStorage::TruncUpperFile(size_t newlines,size_t newcols)
{
	// Line of the last element in upper part.
	size_t maxlines;
	if(newcols>newlines)
		maxlines=newlines;
	else
		maxlines=newcols-1;

	size_t tmp1(NbLines);
	size_t tmp2(NbCols);
	NbLines=newlines;
	NbCols=newcols;
	File2.Truncate((GetUpperPos(maxlines-1,newcols-1)+1)*sizeof(double));
	NbLines=tmp1;
	NbCols=tmp2;
}


//------------------------------------------------------------------------------
void RMatrixStorage::TruncLowerFile(size_t newlines,size_t newcols)
{
	// Column of the last element in lower part.
	size_t maxcols;
	if(newlines>=newcols)
		maxcols=newcols;
	else
		maxcols=newlines;

	size_t tmp1(NbLines);
	size_t tmp2(NbCols);
	NbLines=newlines;
	NbCols=newcols;
	File1.Truncate((GetLowerPos(newlines-1,maxcols-1)+1)*sizeof(double));
	NbLines=tmp1;
	NbCols=tmp2;
}


//------------------------------------------------------------------------------
void RMatrixStorage::ExtendUpperFile(size_t newlines,size_t newcols,bool fill,double val)
{
	// Line of the last element in upper part.
	size_t maxlines;
	if(newcols>newlines)
		maxlines=newlines;
	else
		maxlines=newcols-1;

	// Columns are added or lines extended
	if(fill)
	{
		// Go through at the first position to add the default value until the end of the file
		size_t max(NbCols>NbLines?NbLines:NbCols-1);
		size_t pos(0);
		if(NbLines||NbCols)
		{
			pos=GetUpperPos(max,NbCols-1);
			if(pos)
				pos++;
		}
		File2.Seek(pos*sizeof(double));

		// While the end of the file is not reached, fill the default value
		size_t tmp1(NbLines);
		size_t tmp2(NbCols);
		NbLines=newlines;
		NbCols=newcols;
		for(size_t max=GetUpperPos(maxlines-1,newcols-1)+1;pos<max;pos++)
			File2<<val;
		NbLines=tmp1;
		NbCols=tmp2;
	}
	else
	{
		// Write the default value to the last position
		size_t tmp1(NbLines);
		size_t tmp2(NbCols);
		NbLines=newlines;
		NbCols=newcols;
		File2.Seek(GetUpperPos(maxlines-1,newcols-1)*sizeof(double));
		File2<<val;
		NbLines=tmp1;
		NbCols=tmp2;
	}
}


//------------------------------------------------------------------------------
void RMatrixStorage::ExtendLowerFile(size_t newlines,size_t newcols,bool fill,double val)
{
	// Column of the last element in lower part.
	size_t maxcols;
	if(newlines>=newcols)
		maxcols=newcols;
	else
		maxcols=newlines;

	// Lines are added or columns extended
	if(fill)
	{
		// Go through at the first position to add the default value until the end of the file
		size_t max(NbLines>=NbCols?NbCols:NbLines);
		size_t pos(0);
		if(NbLines||NbCols)
		{
			pos=GetLowerPos(NbLines-1,max-1);
			if(pos)
				pos++;
		}
		File1.Seek(pos*sizeof(double));

		// While the end of the file is not reached, fill the default value
		size_t tmp1(NbLines);
		size_t tmp2(NbCols);
		NbLines=newlines;
		NbCols=newcols;
		for(max=GetLowerPos(newlines-1,maxcols-1)+1;pos<max;pos++)
			File1<<val;
		NbLines=tmp1;
		NbCols=tmp2;
	}
	else
	{
		// Write the default value to the last position
		size_t tmp1(NbLines);
		size_t tmp2(NbCols);
		NbLines=newlines;
		NbCols=newcols;
		File1.Seek(GetLowerPos(newlines-1,maxcols-1)*sizeof(double));
		File1<<val;
		NbLines=tmp1;
		NbCols=tmp2;
	}
}


//------------------------------------------------------------------------------
void RMatrixStorage::ModifyFull(size_t newlines,size_t newcols,bool fill,double val)
{
	// Treat the lower part
	if(newlines>NbLines)
	{
		if((newcols>=NbLines)&&(NbCols>=NbLines))
			ExtendLowerFile(newlines,newcols,fill,val);
		else
			ReCreateLowerFile(newlines,newcols,val);
	}
	else if(newlines<NbLines)
	{
		if((newcols>=newlines)&&(NbCols>=newlines))
			TruncLowerFile(newlines,newcols);
		else
			ReCreateLowerFile(newlines,newcols,val);
	}
	else if((newcols<NbLines)||(NbCols<NbLines))
		ReCreateLowerFile(newlines,newcols,val);

	if(!Upper)
		return;

	// Treat the upper part
	if(newcols>NbCols)
	{
		//if((newlines>=NbCols)&&(NbLines>=NbCols))
		if((newcols>=NbLines)&&(NbCols>=NbLines))
			ExtendUpperFile(newlines,newcols,fill,val);
		else
			ReCreateUpperFile(newlines,newcols,val);
	}
	else if(newcols<NbCols)
	{
		//if((newlines>=newcols)&&(newlines>=newcols))
		if((newcols>=newlines)&&(NbCols>=newlines))
			TruncUpperFile(newlines,newcols);
		else
			ReCreateUpperFile(newlines,newcols,val);
	}
	else if((newlines<NbCols)||(NbLines<NbCols))
		ReCreateUpperFile(newlines,newcols,val);
}


//------------------------------------------------------------------------------
void RMatrixStorage::ReCreateSparse(size_t newlines,size_t newcols,bool keepnull)
{
	RNumContainer<off_t,false> FirstLines(newlines);   // Remember position of the first element of a line
	RNumContainer<off_t,false> LastLines(newlines);    // Remember position of the last element of a line
	RNumContainer<off_t,false> PrevLines(newlines);    // Remember position of the previous element of a line
	RNumContainer<off_t,false> FirstCols(newcols);     // Remember position of the first element of a column
	RNumContainer<off_t,false> LastCols(newcols);      // Remember position of the last element of a column
	RNumContainer<off_t,false> PrevCols(newcols);      // Remember position of the previous element of a column
	RNumContainer<size_t,false> NbElements(newlines);  // Number of elements in each line (only for ascending ordered sparse matrices)

	for(size_t i=newlines+1;--i;)
	{
		FirstLines.Insert(MaxOffT);
		LastLines.Insert(MaxOffT);
		PrevLines.Insert(MaxOffT);
		if(Max)
			NbElements.Insert(0);
	}
	for(size_t j=newcols+1;--j;)
	{
		FirstCols.Insert(MaxOffT);
		LastCols.Insert(MaxOffT);
		PrevCols.Insert(MaxOffT);
	}

	// Create a new file
	RBinaryFile NewFile1;
	off_t CurPos(0); // Current position in the file
	NewFile1.Open(BaseURI+".rec_new",RIO::Create);

	// Parse File1 and copy in NewFile1 if (i,j) are OK and the value is not null
	File1.Seek(0);
	while(!File1.End())
	{
		// Read the value in the current file
		double nb;
		size_t i,j;
		off_t ni,nj;
		File1>>nb>>i>>j>>ni>>nj;

		// Look if it must be store in the file
		if((i>=newlines)||(j>=newcols))
			continue;
		if((!keepnull)&&(nb==0.0))
			continue;

		// Store it as the last element
		NewFile1.Seek(CurPos);
		NewFile1<<nb<<i<<j<<MaxOffT<<MaxOffT;
		if(Max)
			NbElements[i]++;

		// Make the previous element of the line points to CurPos
		off_t& PrevLine(PrevLines[i]);
		if(PrevLine!=MaxOffT)
		{
			NewFile1.Seek(PrevLine+sizeof(double)+2*sizeof(size_t));
			NewFile1<<CurPos;
		}
		PrevLine=CurPos;

		// Make the previous element of the column points to CurPos
		off_t& PrevCol(PrevCols[j]);
		if(PrevCol!=MaxOffT)
		{
			NewFile1.Seek(PrevCol+sizeof(double)+2*sizeof(size_t)+sizeof(off_t));
			NewFile1<<CurPos;
		}
		PrevCol=CurPos;

		// Verify if it is the first element of the line
		off_t& FirstLine(FirstLines[i]);
		if(FirstLine==MaxOffT)
			FirstLine=CurPos;

		// Verify if it is the first element of the column
		off_t& FirstCol(FirstCols[j]);
		if(FirstCol==MaxOffT)
			FirstCol=CurPos;

		// Suppose it the last element in the column and in the line
		LastCols[j]=CurPos;
		LastLines[i]=CurPos;

		// Increase Position
		CurPos+=ValRec;
	}

	// Replace File1 by NewFile1
	File1.Close();
	NewFile1.Close();
	RFile::RemoveFile(BaseURI+".rec");
	RFile::RenameFile(BaseURI+".rec_new",BaseURI+".rec");
	File1.Open(BaseURI+".rec",RIO::ReadWrite);

	// Write the index
	Index.Truncate(0);
	Index.Seek(2*sizeof(size_t));
	RNumCursor<off_t> FirstLine(FirstLines);
	RNumCursor<off_t> LastLine(LastLines);
	RNumCursor<off_t> FirstCol(FirstCols);
	RNumCursor<off_t> LastCol(LastCols);
	RNumCursor<size_t> NbElement(NbElements);
	for(FirstLine.Start(),LastLine.Start(),FirstCol.Start(),LastCol.Start(),NbElement.Start();(!FirstLine.End())||(!FirstCol.End());)
	{
		// Write first and last element of the line (or MaxOffT if no lines anymore)
		if(FirstLine.End())
			Index<<MaxOffT<<MaxOffT;
		else
		{
			Index<<FirstLine()<<LastLine();
			FirstLine.Next();
			LastLine.Next();
		}

		// Write first and last element of the column (or MaxOffT if no columns anymore)
		if(FirstCol.End())
			Index<<MaxOffT<<MaxOffT;
		else
		{
			Index<<FirstCol()<<LastCol();
			FirstCol.Next();
			LastCol.Next();
		}

		// Write number of elements in the columns for each line (only for ascending ordered sparse matrices)
		if(Max)
		{
			if(NbElement.End())
				Index<<static_cast<size_t>(0);
			else
			{
				Index<<NbElement();
				NbElement.Next();
			}
		}
	}
}


//------------------------------------------------------------------------------
void RMatrixStorage::ExtendSparse(size_t newlines,size_t newcols)
{
	// Compute at which "line position" in the index must begin the work
	size_t LinePos(0);
	if(NbLines||NbCols)
	{
		if(NbLines>NbCols)
			LinePos=NbCols-1;
		else
			LinePos=NbLines-1;
	}

	// Put the index at the good "line position".
	if(Max)
		Index.Seek(2*sizeof(size_t)+LinePos*(4*sizeof(off_t)+sizeof(size_t)));
	else
		Index.Seek(2*sizeof(size_t)+LinePos*4*sizeof(off_t));

	// while the current "line position" is not the last one
	// -> fill with "MaxOffT" to indicate that no values exist for the new elements
	for(;(LinePos<newlines)||(LinePos<newcols);LinePos++)
	{
		// Must something be written for the two first positions corresponding to the lines
		if(LinePos<NbLines)
			Index.SeekRel(2*sizeof(off_t));   // No -> Let the old information in place
		else
			Index<<MaxOffT<<MaxOffT;          // Yes -> Put MaxOffT

		// Must something be written for the next two positions corresponding to the columns
		if(LinePos<NbCols)
			Index.SeekRel(2*sizeof(off_t));   // No -> Let the old information in place
		else
			Index<<MaxOffT<<MaxOffT;          // Yes -> Put MaxOffT

		if(Max)
		{
			if(LinePos<NbLines)
				Index.SeekRel(sizeof(size_t));   // No -> Let the old information in place
			else
				Index<<static_cast<size_t>(0);                       // Yes -> Put 0
		}
	}
}


//------------------------------------------------------------------------------
void RMatrixStorage::VerifySize(size_t newlines,size_t newcols,bool fill,double val)
{
	if(BaseURI==RString::Null)
		mThrowRIOException(&Index,"File not open");

	if(!newlines)
		mThrowRException("Matrix must have at least one line");
	if(!newcols)
		mThrowRException("Matrix must have at least one column");

	if((NbLines==newlines)&&(NbCols==newcols))
		return;

	if(Sparse||Max)
	{
		if((NbLines>newlines)||(NbCols>newcols))
			ReCreateSparse(newlines,newcols,Max);
		else
			ExtendSparse(newlines,newcols);
	}
	else
		ModifyFull(newlines,newcols,fill,val);

	// Write the size
	NbLines=newlines;
	NbCols=newcols;
	Index.Seek(0);
	Index<<NbLines<<NbCols;
}


//------------------------------------------------------------------------------
void RMatrixStorage::Load(RGenericMatrix& matrix)
{
	if(BaseURI==RString::Null)
		mThrowRIOException(&Index,"File not open");
	if(Type!=matrix.GetType())
		mThrowRIOException(&Index,"Invalid type");

	matrix.VerifySize(NbLines,NbCols); // Verify the size of the matrix

	if(Sparse||Max)
	{
		File1.Seek(0);
		while(!File1.End())
		{
			double nb;
			size_t i,j;
			off_t ni,nj;
			File1>>nb>>i>>j>>ni>>nj;
			matrix.Add(i,j,nb);
		}
	}
	else
	{
		// Load the lower part
		File1.Seek(0);
		RCursor<RVector> Lines(static_cast<RMatrix&>(matrix).GetLines());
		for(Lines.Start();!Lines.End();Lines.Next())
		{
			RNumCursor<double> Cols(Lines()->GetCols(0,Lines.GetPos()));
			for(Cols.Start();!Cols.End();Cols.Next())
				File1>>Cols();
		}

		// Load the upper part if necessary
		if(Upper)
		{
			File2.Seek(0);
			for(size_t j=1;j<NbCols;j++)
			{
				size_t Max(j>NbLines?NbLines:j);
				for(size_t i=0;i<Max;i++)
				{
					File2>>matrix(i,j);
				}
			}
		}
	}
}


//------------------------------------------------------------------------------
void RMatrixStorage::SaveFull(const RGenericMatrix& matrix)
{
	// Load the lower part
	File1.Seek(0);
	RCursor<RVector> Lines(static_cast<const RMatrix&>(matrix).GetLines());
	for(Lines.Start();!Lines.End();Lines.Next())
	{
		RNumCursor<double> Cols(Lines()->GetCols(0,Lines.GetPos()));
		for(Cols.Start();!Cols.End();Cols.Next())
			File1<<Cols();
	}

	// Save the upper part if necessary
	if(Upper)
	{
		File2.Truncate(0);
		File2.Seek(0);

		for(size_t j=1;j<NbCols;j++)
		{
			size_t Max(j>NbLines?NbLines:j);
			for(size_t i=0;i<Max;i++)
			{
				File2<<matrix(i,j);
			}
		}
	}
}


//------------------------------------------------------------------------------
template<class cMatrix,class cVector,class cValue>
	void RMatrixStorage::SaveSparse(const RGenericMatrix& matrix)
{
	RNumContainer<off_t,false> FirstLines(NbLines);   // Remember position of the first element of a line
	RNumContainer<off_t,false> LastLines(NbLines);    // Remember position of the last element of a line
	RNumContainer<off_t,false> PrevLines(NbLines);    // Remember position of the previous element of a line
	RNumContainer<off_t,false> FirstCols(NbCols);     // Remember position of the first element of a column
	RNumContainer<off_t,false> LastCols(NbCols);      // Remember position of the last element of a column
	RNumContainer<off_t,false> PrevCols(NbCols);      // Remember position of the previous element of a column
	RNumContainer<size_t,false> NbElements(NbLines);  // Number of elements in each line (only for ascending ordered sparse matrices)

	for(size_t i=NbLines+1;--i;)
	{
		FirstLines.Insert(MaxOffT);
		LastLines.Insert(MaxOffT);
		PrevLines.Insert(MaxOffT);
		if(Max)
			NbElements.Insert(0);
	}
	for(size_t j=NbCols+1;--j;)
	{
		FirstCols.Insert(MaxOffT);
		LastCols.Insert(MaxOffT);
		PrevCols.Insert(MaxOffT);
	}

	File1.Seek(0);
	off_t CurPos(0); // Current position in the file
	size_t i;
	RCursor<cVector> Lines(static_cast<const cMatrix&>(matrix).GetLines());
	for(Lines.Start(),i=0;!Lines.End();Lines.Next(),i++)
	{
		RCursor<cValue> Cols(*Lines());
		if(!Cols.GetNb())
			continue;
		FirstLines[Lines()->GetId()]=CurPos;
		for(Cols.Start();!Cols.End();Cols.Next())
		{
			// Write element
			File1.Seek(CurPos);
			File1<<Cols()->Value<<Lines()->GetId()<<Cols()->Id<<MaxOffT<<MaxOffT;
			if(Max)
				NbElements[i]++;

			// Make the previous element of the line points to CurPos
			off_t& PrevLine(PrevLines[Lines()->GetId()]);
			if(PrevLine!=MaxOffT)
			{
				File1.Seek(PrevLine+sizeof(double)+2*sizeof(size_t));
				File1<<CurPos;
			}
			PrevLine=CurPos;

			// Make the previous element of the column points to CurPos
			off_t& PrevCol(PrevCols[Cols()->Id]);
			if(PrevCol!=MaxOffT)
			{
				File1.Seek(PrevCol+sizeof(double)+2*sizeof(size_t)+sizeof(off_t));
				File1<<CurPos;
			}
			PrevCol=CurPos;

			// Verify if it is the first element of the column
			off_t& FirstCol(FirstCols[Cols()->Id]);
			if(FirstCol==MaxOffT)
				FirstCol=CurPos;

			// Suppose it the last element in the column
			LastCols[Cols()->Id]=CurPos;

			// Increase Position
			CurPos+=ValRec;
		}
		LastLines[Lines()->GetId()]=CurPos-ValRec;
	}

	// Write the index
	Index.Truncate(0);
	Index.Seek(2*sizeof(size_t));
	RNumCursor<off_t> FirstLine(FirstLines);
	RNumCursor<off_t> LastLine(LastLines);
	RNumCursor<off_t> FirstCol(FirstCols);
	RNumCursor<off_t> LastCol(LastCols);
	RNumCursor<size_t> NbElement(NbElements);
	for(FirstLine.Start(),LastLine.Start(),FirstCol.Start(),LastCol.Start(),NbElement.Start();(!FirstLine.End())||(!FirstCol.End());)
	{
		// Write first and last element of the line (or MaxOffT if no lines anymore)
		if(FirstLine.End())
			Index<<MaxOffT<<MaxOffT;
		else
		{
			Index<<FirstLine()<<LastLine();
			FirstLine.Next();
			LastLine.Next();
		}

		// Write first and last element of the column (or MaxOffT if no columns anymore)
		if(FirstCol.End())
			Index<<MaxOffT<<MaxOffT;
		else
		{
			Index<<FirstCol()<<LastCol();
			FirstCol.Next();
			LastCol.Next();
		}

		// Write number of elements in the columns for each line (only for ascending ordered sparse matrices)
		if(Max)
		{
			if(NbElement.End())
				Index<<static_cast<size_t>(0);
			else
			{
				Index<<NbElement();
				NbElement.Next();
			}
		}
	}
}


//------------------------------------------------------------------------------
void RMatrixStorage::Save(const RGenericMatrix& matrix)
{
	if(BaseURI==RString::Null)
		mThrowRIOException(&Index,"File not open");
	if(Type!=matrix.GetType())
		mThrowRIOException(&Index,"Invalid type");

	// Verify the size of the file if needed
	VerifySize(matrix.GetNbLines(),matrix.GetNbCols());

	// Truncate the files
	File1.Truncate(0);
	Index.Truncate(0);

	if(Sparse)
		SaveSparse<RSparseMatrix,RSparseVector,RValue>(matrix);
	else if(Max)
		SaveSparse<RMaxMatrix,RMaxVector,RMaxValue>(matrix);
	else
		SaveFull(matrix);
}


//------------------------------------------------------------------------------
double RMatrixStorage::ReadSparse(size_t i,size_t j)
{
	// Search the position of the first element of the line
	Index.Seek(2*sizeof(size_t)+i*4*sizeof(off_t));
	off_t NextPos;
	Index>>NextPos;

	// Scan the whole line until the column is found or the line is ended
	while(NextPos!=MaxOffT)
	{
		size_t line,col;
		double res;

		File1.Seek(NextPos);
		File1>>res>>line>>col>>NextPos;
		if(col==j)
			return(res);
	}

	// Nothing found -> res=0
	return(0);
}


//------------------------------------------------------------------------------
double RMatrixStorage::ReadMax(size_t i,size_t j)
{
	// Search the position of the first element and the number of elements of the line
	Index.Seek(2*sizeof(size_t)+i*(4*sizeof(off_t)+sizeof(size_t)));
	off_t NextPos,tmp1,tmp2,tmp3;
	size_t NbElements;
	Index>>NextPos>>tmp1>>tmp2>>tmp3>>NbElements;

	// Enough value computed ?
	if(j>=NbElements)
		mThrowRException(BaseURI+" has not enough values for index "+RString::Number(i));


	// Scan the whole line until the column is found or the line is ended
	for(size_t col=0;NextPos!=MaxOffT;col++)
	{
		size_t line,tmpcol;
		double res;

		File1.Seek(NextPos);
		File1>>res>>line>>tmpcol>>NextPos;

		// If the value is NAN -> nothing more after
		if(res!=res)
			break;

		if(col==j)
			return(res);
	}

	// Nothing found -> Big problem.
	mThrowRException(BaseURI+" is not an ascending ordered sparse matrix");
}


//------------------------------------------------------------------------------
double RMatrixStorage::Read(size_t i,size_t j)
{
	if(BaseURI==RString::Null)
		mThrowRIOException(&Index,"File not open");
	if((i>NbLines)||(j>NbCols))
		throw std::range_error("RMatrixStorage::Read(size_t,size_t) : index "+RString::Number(i)+","+RString::Number(j)+" outside range ("+RString::Number(NbLines)+","+RString::Number(NbCols)+")");

	double res;

	// Switch line and column if symmetric matrix
	if(((Type==RGenericMatrix::tSymmetric)||(Type==RGenericMatrix::tSparseSymmetric))&&(j>i))
	{
		size_t tmp(i);
		i=j;
		j=tmp;
	}

	if(Max)
		return(ReadMax(i,j));
	else if(Sparse)
		return(ReadSparse(i,j));
	else
	{
		if(i<j)
		{
			// Read in the upper part
			File2.Seek(GetUpperPos(i,j)*sizeof(double));
			File2>>res;
		}
		else
		{
			// Read in the lower part
			File1.Seek(GetLowerPos(i,j)*sizeof(double));
			File1>>res;
		}
	}

	return(res);
}


//------------------------------------------------------------------------------
void RMatrixStorage::WriteSparse(size_t i,size_t j,double val)
{
	// Search the position of the first element of the line
	Index.Seek(2*sizeof(size_t)+i*4*sizeof(off_t));
	off_t NextPos;
	Index>>NextPos;

	// Scan the whole line until the right position in the column is found or the line is ended
	while(NextPos!=MaxOffT)
	{
		double res;
		size_t line,col;

		File1.Seek(NextPos);
		File1>>res>>line>>col>>NextPos;

		if(col==j)
		{
			// Element already inserted -> Rewrite if necessary
			if(res!=val)
			{
				// Go back to point to the value
				File1.SeekRel(-2*sizeof(size_t)-sizeof(off_t)-sizeof(double));
				File1<<val;
			}
			return;
		}
	}

	// Nothing found -> Test if null value
	if(val==0.0)
		return;

	// Add a new value
	off_t NewPos(File1.GetSize());
	File1.Seek(NewPos);
	File1<<val<<i<<j<<MaxOffT<<MaxOffT;

	// Change the line indexes
	Index.Seek(2*sizeof(size_t)+i*4*sizeof(off_t));
	off_t FirstLine,LastLine;
	Index>>FirstLine>>LastLine;
	if(FirstLine==MaxOffT)
		FirstLine=NewPos;
	if(LastLine!=MaxOffT)
	{
		// Change the pointer of the last element of the line.
		File1.Seek(LastLine+sizeof(double)+2*sizeof(size_t));
		File1<<NewPos;
	}
	LastLine=NewPos;
	Index.SeekRel(-2*sizeof(off_t));
	Index<<FirstLine<<LastLine;

	// Change the column indexes
	Index.Seek(2*sizeof(size_t)+j*4*sizeof(off_t)+2*sizeof(off_t));
	off_t FirstCol,LastCol;
	Index>>FirstCol>>LastCol;
	if(FirstCol==MaxOffT)
		FirstCol=NewPos;
	if(LastCol!=MaxOffT)
	{
		// Change the pointer of the last element of the line.
		File1.Seek(LastCol+sizeof(double)+2*sizeof(size_t)+sizeof(off_t));
		File1<<NewPos;
	}
	LastCol=NewPos;
	Index.SeekRel(-2*sizeof(off_t));
	Index<<FirstCol<<LastCol;
}


//------------------------------------------------------------------------------
void RMatrixStorage::WriteMax(size_t i,size_t j,double val)
{
	double res;
	size_t line,col;
	off_t NextLine,NextCol,LastLine,tmp2,tmp3;
	size_t NbElements;

	// Search the position of the first element of the line
	Index.Seek(2*sizeof(size_t)+i*(4*sizeof(off_t)+sizeof(size_t)));
	Index>>NextLine>>LastLine>>tmp2>>tmp3>>NbElements;

	// Scan the whole line to determine if the element is already in the line and
	// where it should be stored
	// Pos represents the current position in the file
	// Last represents the position of the last element read
	bool IsIn(false);           // Suppose the element is not already in that line
	off_t Where(MaxOffT);       // Position in the file where the element must be inserted
	off_t Act(MaxOffT);         // Actual position in the file of the element (if MaxOffT, the element is not already there)
	for(off_t Last(NextLine),Pos;NextLine!=MaxOffT;)
	{
		Pos=NextLine;
		File1.Seek(Pos);
		File1>>res>>line>>col>>NextLine>>NextCol;

		// If the value is NAN -> nothing more after
		if(res!=res)
		{
			if(!IsIn)
				Act=Pos;  // The existing unused position will be used for the element to insert
			break;
		}

		if((Where==MaxOffT)&&(res<=val))
		{
			if(IsIn)
				Where=Last;
			else
				Where=Pos;
		}

		if(j==col)
		{
			IsIn=true;
			Act=Pos;
		}

		if(IsIn&&(Where!=MaxOffT))
			break;

		Last=Pos;
	}

	// Rewriting the new value is enough
	if(IsIn&&(Where==Act))
	{
		File1.Seek(Where);
		File1<<val;
		return;
	}

	if(IsIn)
	{
		// Elements must be shifted. This doesn't modify the first or the last position in the line.
		if(Where>Act)
		{
			// The element inserted must be put at the end of the line
			if(Where==MaxOffT)
				Where=LastLine;

			// Elements must be shifted to the left until Where included starting from the next element after Act
			File1.Seek(Act);
			File1>>res>>line>>col>>NextLine;
			for(;NextLine!=MaxOffT;)
			{
				size_t Pos(Act);
				Act=NextLine;
				File1.Seek(Act);
				File1>>res>>line>>col>>NextLine;
				File1.Seek(Pos);
				File1<<res<<line<<col;
				if(Act==Where)
					NextLine=MaxOffT;
			}

			// Copy now the current element at Where
			File1.Seek(Where);
			File1<<val<<i<<j;
		}
		else
		{
			// Elements at Where and before Act must be shifted to the right
			for(NextLine=Where;NextLine!=MaxOffT;)
			{
				File1.Seek(Where);
				File1>>res>>line>>col>>NextLine;
				File1.Seek(Where);
				File1<<val<<i<<j;
				i=line;
				j=col;
				val=res;
				if(Where==Act)
					NextLine=MaxOffT;
				Where=NextLine;
			}
		}
	}
	else
	{
		// Element must be added to the line

		if(Act!=MaxOffT)
		{
			// Use an existing invalid record at Act. This doesn't modify the
			// first or the last position in the line.
			if(Where!=MaxOffT)
			{
				// The element must be put at the end of the line at position Act
				File1.Seek(Act);
				File1<<val<<i<<j;
			}
			else
			{
				// Elements after Where must be shifted to the right until Act not included
				for(;Where!=Act;)
				{
					File1.Seek(Where);
					File1>>res>>line>>col>>NextLine;
					File1.Seek(Where);
					File1<<val<<i<<j;
					Where=NextLine;
					val=res;
					i=line;
					j=col;
				}

				// Write the last element in Act
				File1.Seek(Act);
				File1<<res<<line<<col;
			}
		}
		else
		{
			if(NbElements==NbCols)
			{
				// No record can be added
				// If the element must be inserted at the end -> nothing to do
				if(Where==MaxOffT)
					return;

				// The value must be put at Where and the elements must be shifted to the right except the last one
				for(NextLine=Where;NextLine!=MaxOffT;)
				{
					File1.Seek(Where);
					File1>>res>>line>>col>>NextLine;
					File1.Seek(Where);
					File1<<val<<i<<j;
					i=line;
					j=col;
					val=res;
					Where=NextLine;
				}
			}
			else
			{
				// Add a next record at the end

				// Go where the element must be inserted
				if(Where!=MaxOffT)
				{
					File1.Seek(Where);
					File1>>res>>line>>col>>NextLine;

					// Write the current element
					File1.Seek(Where);
					File1<<val<<i<<j;
					Where=NextLine;
				}
				else
				{
					res=val;
					line=i;
					col=j;
				}

				while(Where!=MaxOffT)
				{
					val=res;
					i=line;
					j=col;

					File1.Seek(Where);
					File1>>res>>line>>col>>NextLine;
					File1.Seek(Where);
					File1<<val<<i<<j;
					Where=NextLine;
				}

				// Write the last element at the end
				Act=File1.GetSize();
				File1.Seek(Act);
				File1<<res<<line<<col<<MaxOffT<<MaxOffT;

				// Change the line indexes
				Index.Seek(2*sizeof(size_t)+i*(4*sizeof(off_t)+sizeof(size_t)));
				off_t FirstLine,LastLine;
				Index>>FirstLine>>LastLine;
				if(FirstLine==MaxOffT)
					FirstLine=Act;
				if(LastLine!=MaxOffT)
				{
					// Change the pointer of the last element of the line.
					File1.Seek(LastLine+sizeof(double)+2*sizeof(size_t));
					File1<<Act;
				}
				LastLine=Act;
				Index.SeekRel(-2*sizeof(off_t));
				Index<<FirstLine<<LastLine;
				Index.SeekRel(2*sizeof(off_t));
				Index<<NbElements+1;

				// Change the column indexes
				Index.Seek(2*sizeof(size_t)+NbElements*(4*sizeof(off_t)+sizeof(size_t))+2*sizeof(off_t));
				off_t FirstCol,LastCol;
				Index>>FirstCol>>LastCol;
				if(FirstCol==MaxOffT)
					FirstCol=Act;
				if(LastCol!=MaxOffT)
				{
					// Change the pointer of the last element of the line.
					File1.Seek(LastCol+sizeof(double)+2*sizeof(size_t)+sizeof(off_t));
					File1<<Act;
				}
				LastCol=Act;
				Index.SeekRel(-2*sizeof(off_t));
				Index<<FirstCol<<LastCol;
			}
		}
	}
}


//------------------------------------------------------------------------------
void RMatrixStorage::Write(size_t i,size_t j,double val)
{
	if(BaseURI==RString::Null)
		throw RException("RMatrixStorage::Save(RGenericMatrix&) : File not open");
	if((Max&&(i>NbLines))||((!Max)&&((i>NbLines)||(j>NbCols))))
		throw std::range_error("RMatrixStorage::Write(size_t,size_t,double) : index "+RString::Number(i)+","+RString::Number(j)+" outside range ("+RString::Number(NbLines)+","+RString::Number(NbCols)+")");

	if(((Type==RGenericMatrix::tSymmetric)||(Type==RGenericMatrix::tSparseSymmetric))&&(j>i))
	{
		size_t tmp(i);
		i=j;
		j=tmp;
	}

	if(Max)
		WriteMax(i,j,val);
	else if(Sparse)
		WriteSparse(i,j,val);
	else
	{
		if(i<j)
		{
			// Read in the upper part
			File2.Seek(GetUpperPos(i,j)*sizeof(double));
			File2<<val;
		}
		else
		{
			// Read in the lower part
			File1.Seek(GetLowerPos(i,j)*sizeof(double));
			File1<<val;
		}
	}
}


//------------------------------------------------------------------------------
void RMatrixStorage::ReadInfo(size_t posinfo,char* buffer,size_t nb)
{
	if(BaseURI==RString::Null)
		mThrowRException("File not open");

	size_t deb(2*sizeof(size_t));
	if(Sparse||Max)
	{
		size_t max;
		if(NbLines>NbCols)
			max=NbLines;
		else
			max=NbCols;
		max*=sizeof(off_t)*4;
		deb+=max;
	}
	Index.Seek(deb+posinfo);
	size_t nbread(Index.Read(buffer,nb));
	if(nbread!=nb)
		mThrowRException("Internal problem in the information stored");
}


//------------------------------------------------------------------------------
void RMatrixStorage::WriteInfo(size_t posinfo,const char* buffer,size_t nb)
{
	if(BaseURI==RString::Null)
		mThrowRException("File not open");

	size_t deb(2*sizeof(size_t));
	if(Sparse||Max)
	{
		size_t max;
		if(NbLines>NbCols)
			max=NbLines;
		else
			max=NbCols;
		max*=sizeof(off_t)*4;
		deb+=max;
	}
	Index.Seek(deb+posinfo);
	Index.Write(buffer,nb);
}


//------------------------------------------------------------------------------
bool RMatrixStorage::HasInfo(void) const
{
	off_t deb(2*sizeof(size_t));
	if(Sparse||Max)
	{
		size_t max;
		if(NbLines>NbCols)
			max=NbLines;
		else
			max=NbCols;
		max*=sizeof(off_t)*4;
		deb+=max;
	}
	return(deb<Index.GetSize());
}


//------------------------------------------------------------------------------
RMatrixStorage::~RMatrixStorage(void)
{
	if(Index.IsOpen())
		Close();
}
