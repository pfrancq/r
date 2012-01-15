/*

	R Project Library

	RMatrixStrorage.cpp

	Matrix Storage - Implementation.

	Copyright 2009-2012 by Pascal Francq (pascal@francq.info).

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
const off_t SizeRec=sizeof(double)+2*sizeof(size_t)+2*sizeof(off_t);



//------------------------------------------------------------------------------
//
//    Class RMatrixStorage
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
RMatrixStorage::RMatrixStorage(void)
	: File1(), File2(), Index(), BaseURI(), NbLines(0), NbCols(0), Pos01(0)
{
}


//------------------------------------------------------------------------------
void RMatrixStorage::Open(const RString& baseuri,RGenericMatrix::tType type)
{
	BaseURI=baseuri;
	Type=type;

	// Open the Index
	Index.Open(BaseURI+".info",RIO::ReadWrite);
	if(!Index.End())
		Index>>NbLines>>NbCols;

	// Open the other files
	switch(Type)
	{
		case RGenericMatrix::tNormal:
			File1.Open(BaseURI+".lower",RIO::ReadWrite);
			File2.Open(BaseURI+".upper",RIO::ReadWrite);
			Pos01=0;
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
		return((NbLines*(NbLines-1))/2+(j-NbLines)*NbLines+i+Pos01);
	return((j*(j-1))/2+i+Pos01);
}


//------------------------------------------------------------------------------
size_t RMatrixStorage::GetNbLines(size_t col) const
{
	if((NbCols>=NbLines)&&(col>NbLines))
		return(NbLines);
	return(col);
}


//------------------------------------------------------------------------------
size_t RMatrixStorage::GetNbCols(size_t line) const
{
	if((NbLines>=NbCols)&&(line>NbCols))
			return(NbCols);
	return(line);
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
			Index<<MaxOffT<<MaxOffT<<MaxOffT<<MaxOffT;
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
void RMatrixStorage::ReCreateUpperFile(size_t newlines,size_t newcols,double val)
{
	// Rec-create the lower part
	RBinaryFile NewFile2;
	NewFile2.Open(BaseURI+".upper_new",RIO::Create);
	File2.Seek(0);
	size_t maxlines,maxcols;
	if(NbCols>newcols)
		maxcols=newcols;
	else
		maxcols=NbCols;
	if(Pos01)
	{
		// Upper matrix contains (0,0) element.
		double tmp;
		File2>>tmp;
		if(newlines&&newcols)
			NewFile2<<tmp;
	}

	// Read all existing columns
	for(size_t j=1;j<maxcols;j++)
	{
		if(j<=NbLines)
			maxlines=j;
		else
			maxlines=NbLines;

		// Read all the existing lines
		for(size_t i=0;i<maxlines;i++)
		{
			double tmp;
			File2>>tmp;

			// If line is valid in the matrix -> Write it.
			if(i<newlines)
				NewFile2<<tmp;
		}
	}

	// Add columns ?
	for(size_t j=NbCols;j<newcols;j++)
	{
		if(j<=newlines)
			maxlines=j;
		else
			maxlines=newlines;

		// Add lines
		for(size_t i=0;i<maxlines;i++)
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
void RMatrixStorage::ReModifyUpperFile(size_t newlines,size_t newcols,bool fill,double val)
{
	// Line of the last element in lower part.
	size_t maxlines;
	if(newcols>newlines)
		maxlines=newlines;
	else
		maxlines=newcols-1;

	// 1. Columns are reduced -> Truncate the file.
	// 2. Columns are added or lines extended -> Write at the last position NAN.
	if(newcols<NbCols)
	{
		// Truncate position
		File2.Truncate((GetUpperPos(maxlines,newcols)+1)*sizeof(double));
	}
	else if((newcols>NbCols)||(newlines>NbLines))
	{
		if(fill)
		{
			// Go through at the first position to add
			size_t max(NbCols>NbLines?NbLines:NbCols-1);
			size_t pos(GetUpperPos(max,NbCols)+1);
			File2.Seek(pos*sizeof(double));

			// While the end of the file is not reached, fill the new value
			for(max=GetUpperPos(maxlines,newcols)+1;pos<max;pos++)
				File2<<val;
		}
		else
		{
			File2.Seek((GetUpperPos(maxlines,newcols)+1)*sizeof(double));
			File2<<val;
		}
	}
	else
		cerr<<"Problem in RMatrixStorage::VerifySize"<<endl;
}


//------------------------------------------------------------------------------
void RMatrixStorage::VerifySize(size_t newlines,size_t newcols,bool fill,double val)
{
	if(BaseURI==RString::Null)
		ThrowRIOException(&Index,"File not open");

	if((NbLines==newlines)&&(NbCols==newcols))
		return;


	if(Sparse||Max)
	{
		// Only if the matrix is smaller a be re-created
		if((NbLines>newlines)||(NbCols>newcols))
		{
			RNumContainer<off_t,false> FirstLines(NbLines);   // Remember position of the first element of a line
			RNumContainer<off_t,false> LastLines(NbLines);    // Remember position of the last element of a line
			RNumContainer<off_t,false> PrevLines(NbCols);     // Remember position of the previous element of a line
			RNumContainer<off_t,false> FirstCols(NbCols);     // Remember position of the first element of a column
			RNumContainer<off_t,false> LastCols(NbCols);      // Remember position of the last element of a column
			RNumContainer<off_t,false> PrevCols(NbCols);      // Remember position of the previous element of a column
			for(size_t i=NbLines+1;--i;)
			{
				FirstLines.Insert(MaxOffT);
				LastLines.Insert(MaxOffT);
			}
			for(size_t j=NbCols+1;--j;)
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
				double nb;
				size_t i,j;
				off_t ni,nj;
				File1>>nb>>i>>j>>ni>>nj;
				if((nb==0.0)||(i>=newlines)||(j>=newcols))
					continue;
				NewFile1.Seek(CurPos);
				NewFile1<<nb<<i<<j<<ni<<nj;

				// Make the previous element of the line points to CurPos
				off_t& PrevLine(PrevLines[i]);
				if(PrevLine!=MaxOffT)
				{
					File1.Seek(PrevLine+sizeof(double)+2*sizeof(size_t));
					File1<<CurPos;
				}
				PrevLine=CurPos;

				// Make the previous element of the column points to CurPos
				off_t& PrevCol(PrevCols[j]);
				if(PrevCol!=MaxOffT)
				{
					File1.Seek(PrevCol+sizeof(double)+2*sizeof(size_t)+sizeof(off_t));
					File1<<CurPos;
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
				CurPos+=SizeRec;
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
			for(FirstLine.Start(),LastLine.Start(),FirstCol.Start(),LastCol.Start();(!FirstLine.End())||(!FirstCol.End());)
			{
				// Write first and last element of the line (or SIZE_MAX if no lines anymore)
				if(FirstLine.End())
					Index<<MaxOffT<<MaxOffT;
				else
				{
					Index<<FirstLine()<<LastLine();
					FirstLine.Next();
					LastLine.Next();
				}

				// Write first and last element of the column (or SIZE_MAX if no columns anymore)
				if(FirstCol.End())
					Index<<MaxOffT<<MaxOffT;
				else
				{
					Index<<FirstCol()<<LastCol();
					FirstCol.Next();
					LastCol.Next();
				}
			}
		}
	}
	else
	{
		// Must the lower part be re-created:
		// 1. Number of columns are reduced : Something was there before and must be there now
		// 2. Number of columns are extended : Something was not there before and must be there now
		if( ((newcols<NbCols)&&(newcols<NbLines)&&(newlines>newcols+1)) || ((newcols>NbCols)&&(NbCols<NbLines)&&(newcols>=newlines)) )
		{
			// Rec-create the lower part
			RBinaryFile NewFile1;
			NewFile1.Open(BaseURI+".lower_new",RIO::Create);
			File1.Seek(0);
			size_t maxlines,maxcols;
			if(NbLines>newlines)
				maxlines=newlines;
			else
				maxcols=NbLines;
			for(size_t i=0;i<maxlines;i++)
			{
				if(i<=NbCols)
					maxcols=i;
				else
					maxcols=NbCols;

				// Read all the existing columns
				for(size_t j=0;j<=maxcols;j++)
				{
					double tmp;
					File1>>tmp;

					// If column is valid in the matrix -> Write it.
					if(j<newcols)
						NewFile1<<tmp;
				}
			}

			// Add lines ?
			for(size_t i=NbLines;i<newlines;i++)
			{
				if(i<=newcols)
					maxcols=i;
				else
					maxcols=newcols;

				// Add columns
				for(size_t j=0;j<=maxcols;j++)
					NewFile1<<val;
			}

			// Replace File1 by NewFile1
			File1.Close();
			NewFile1.Close();
			RFile::RemoveFile(BaseURI+".lower");
			RFile::RenameFile(BaseURI+".lower_new",BaseURI+".lower");
			File1.Open(BaseURI+".lower",RIO::ReadWrite);
		}
		else
		{
			// Modify the file.

			// Column of the last element in lower part.
			size_t maxcols;
			if(newlines>=newcols)
				maxcols=newcols;
			else
				maxcols=newlines;

			// 1. Lines are reduced -> Truncate the file.
			// 2. Lines are added or columns extended -> Write at the last position NAN.
			if(newlines<NbLines)
			{
				// Truncate position
				File1.Truncate((GetLowerPos(newlines,maxcols)+1)*sizeof(double));
			}
			else if((newlines>NbLines)||(newcols>NbCols))
			{
				if(fill)
				{
					// Go through at the first position to add
					size_t max(NbLines>=NbCols?NbCols:NbLines);
					size_t pos(GetLowerPos(NbLines,max)+1);
					File1.Seek(pos*sizeof(double));

					// While the end of the file is not reached, fill the new value
					for(max=GetLowerPos(newlines,maxcols)+1;pos<max;pos++)
						File1<<val;
				}
				else
				{
					File1.Seek((GetLowerPos(newlines,maxcols)+1)*sizeof(double));
					File1<<val;
				}
			}
			else
				cerr<<"Problem in RMatrixStorage::VerifySize"<<endl;
		}

		if(Upper)
		{
			// Must the upper part be re-created:  <----
			// 1. Number of lines are reduced : Something was there before and must be there now
			// 2. Number of lines are extended : Something was not there before and must be there now
			if(((newlines<NbLines)&&(newlines<NbCols)&&(newcols>newlines+2)) || ((newlines>NbLines)&&(NbLines<NbCols)&&(newlines!=NbLines+1)&&(newlines+1!=newcols)) )
				ReCreateUpperFile(newlines,newcols,val);
			else
				ReModifyUpperFile(newlines,newcols,fill,val);
		}
	}

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
		ThrowRIOException(&Index,"File not open");
	if(Type!=matrix.GetType())
		ThrowRIOException(&Index,"Invalid type");

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
		RCursor<RMatrixLine> Lines(static_cast<RMatrix&>(matrix).GetLines());
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
			if(Pos01)
				File2>>matrix(0,0);   // Upper contains element (0,0)
			for(size_t j=1;j<NbCols;j++)
				for(size_t i=0;i<j;i++)
					File2>>matrix(i,j);
		}
	}
}


//------------------------------------------------------------------------------
void RMatrixStorage::SaveFull(const RGenericMatrix& matrix)
{
	// Load the lower part
	File1.Seek(0);
	RCursor<RMatrixLine> Lines(static_cast<const RMatrix&>(matrix).GetLines());
	for(Lines.Start();!Lines.End();Lines.Next())
	{
		RNumCursor<double> Cols(Lines()->GetCols(0,Lines.GetPos()));
		for(Cols.Start();!Cols.End();Cols.Next())
			File1<<Cols();
	}

	// Load the upper part if necessary
	if(Upper)
	{
		File2.Truncate(0);
		File2.Seek(0);
		if(Pos01)
			File2<<matrix(0,0);   // Upper contains element (0,0)
		for(size_t j=1;j<NbCols;j++)
			for(size_t i=0;i<j;i++)
				File2<<matrix(i,j);
	}
}


//------------------------------------------------------------------------------
void RMatrixStorage::SaveSparse(const RGenericMatrix& matrix)
{
	RNumContainer<off_t,false> FirstLines(NbLines);   // Remember position of the first element of a line
	RNumContainer<off_t,false> LastLines(NbLines);    // Remember position of the last element of a line
	RNumContainer<off_t,false> PrevLines(NbCols);     // Remember position of the previous element of a line
	RNumContainer<off_t,false> FirstCols(NbCols);     // Remember position of the first element of a column
	RNumContainer<off_t,false> LastCols(NbCols);      // Remember position of the last element of a column
	RNumContainer<off_t,false> PrevCols(NbCols);      // Remember position of the previous element of a column
	for(size_t i=NbLines+1;--i;)
	{
		FirstLines.Insert(MaxOffT);
		LastLines.Insert(MaxOffT);
	}
	for(size_t j=NbCols+1;--j;)
	{
		FirstCols.Insert(MaxOffT);
		LastCols.Insert(MaxOffT);
		PrevCols.Insert(MaxOffT);
	}

	File1.Seek(0);
	off_t CurPos(0); // Current position in the file
	RCursor<RSparseVector> Lines(static_cast<const RSparseMatrix&>(matrix).GetLines());
	for(Lines.Start();!Lines.End();Lines.Next())
	{
		RCursor<RValue> Cols(*Lines());
		if(!Cols.GetNb())
			continue;
		FirstLines[Lines()->GetId()]=CurPos;
		for(Cols.Start();!Cols.End();Cols.Next())
		{
			// Write element
			File1.Seek(CurPos);
			File1<<Cols()->Value<<Lines()->GetId()<<Cols()->Id<<MaxOffT<<MaxOffT;

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
			CurPos+=SizeRec;
		}
		LastLines[Lines()->GetId()]=CurPos-SizeRec;
	}

	// Write the index
	Index.Seek(2*sizeof(size_t));
	RNumCursor<off_t> FirstLine(FirstLines);
	RNumCursor<off_t> LastLine(LastLines);
	RNumCursor<off_t> FirstCol(FirstCols);
	RNumCursor<off_t> LastCol(LastCols);
	for(FirstLine.Start(),LastLine.Start(),FirstCol.Start(),LastCol.Start();(!FirstLine.End())||(!FirstCol.End());)
	{
		// Write first and last element of the line (or SIZE_MAX if no lines anymore)
		if(FirstLine.End())
			Index<<MaxOffT<<MaxOffT;
		else
		{
			Index<<FirstLine()<<LastLine();
			FirstLine.Next();
			LastLine.Next();
		}

		// Write first and last element of the column (or SIZE_MAX if no columns anymore)
		if(FirstCol.End())
			Index<<MaxOffT<<MaxOffT;
		else
		{
			Index<<FirstCol()<<LastCol();
			FirstCol.Next();
			LastCol.Next();
		}
	}
}


//------------------------------------------------------------------------------
void RMatrixStorage::SaveMax(const RGenericMatrix& matrix)
{
	RNumContainer<off_t,false> FirstLines(NbLines);   // Remember position of the first element of a line
	RNumContainer<off_t,false> LastLines(NbLines);    // Remember position of the last element of a line
	RNumContainer<off_t,false> PrevLines(NbCols);     // Remember position of the previous element of a line
	RNumContainer<off_t,false> FirstCols(NbCols);     // Remember position of the first element of a column
	RNumContainer<off_t,false> LastCols(NbCols);      // Remember position of the last element of a column
	RNumContainer<off_t,false> PrevCols(NbCols);      // Remember position of the previous element of a column

	for(size_t i=NbLines+1;--i;)
	{
		FirstLines.Insert(MaxOffT);
		LastLines.Insert(MaxOffT);
	}
	for(size_t j=NbCols+1;--j;)
	{
		FirstCols.Insert(MaxOffT);
		LastCols.Insert(MaxOffT);
		PrevCols.Insert(MaxOffT);
	}

	File1.Seek(0);
	off_t CurPos(0); // Current position in the file
	RCursor<RMaxVector> Lines(static_cast<const RMaxMatrix&>(matrix).GetLines());
	for(Lines.Start();!Lines.End();Lines.Next())
	{
		RCursor<RMaxValue> Cols(*Lines());
		if(!Cols.GetNb())
			continue;
		FirstLines[Lines()->GetId()]=CurPos;
		for(Cols.Start();!Cols.End();Cols.Next())
		{
			// Write element
			File1.Seek(CurPos);
			File1<<Cols()->Value<<Lines()->GetId()<<Cols()->Id<<MaxOffT<<MaxOffT;

			// Make the previous element of the line points to CurPos
			off_t& PrevLine(PrevLines[Lines()->GetId()]);
			if(PrevLine!=MaxOffT)
			{
				File1.Seek(PrevLine+sizeof(double)+2*sizeof(size_t));
				File1<<CurPos;
			}
			PrevLine=CurPos;

			// Make the previous element of the column points to CurPos
			off_t& PrevCol(PrevCols[Cols.GetPos()]);
			if(PrevCol!=MaxOffT)
			{
				File1.Seek(PrevCol+sizeof(double)+2*sizeof(size_t)+sizeof(off_t));
				File1<<CurPos;
			}
			PrevCol=CurPos;

			// Verify if it is the first element of the column
			off_t& FirstCol(FirstCols[Cols.GetPos()]);
			if(FirstCol==MaxOffT)
				FirstCol=CurPos;

			// Suppose it the last element in the column
			LastCols[Cols.GetPos()]=CurPos;

			// Increase Position
			CurPos+=SizeRec;
		}
		LastLines[Lines()->GetId()]=CurPos-SizeRec;
	}

	// Write the index
	Index.Seek(2*sizeof(size_t));
	RNumCursor<off_t> FirstLine(FirstLines);
	RNumCursor<off_t> LastLine(LastLines);
	RNumCursor<off_t> FirstCol(FirstCols);
	RNumCursor<off_t> LastCol(LastCols);
	for(FirstLine.Start(),LastLine.Start(),FirstCol.Start(),LastCol.Start();(!FirstLine.End())||(!FirstCol.End());)
	{
		// Write first and last element of the line (or SIZE_MAX if no lines anymore)
		if(FirstLine.End())
			Index<<MaxOffT<<MaxOffT;
		else
		{
			Index<<FirstLine()<<LastLine();
			FirstLine.Next();
			LastLine.Next();
		}

		// Write first and last element of the column (or SIZE_MAX if no columns anymore)
		if(FirstCol.End())
			Index<<MaxOffT<<MaxOffT;
		else
		{
			Index<<FirstCol()<<LastCol();
			FirstCol.Next();
			LastCol.Next();
		}
	}
}


//------------------------------------------------------------------------------
void RMatrixStorage::Save(const RGenericMatrix& matrix)
{
	if(BaseURI==RString::Null)
		ThrowRIOException(&Index,"File not open");
	if(Type!=matrix.GetType())
		ThrowRIOException(&Index,"Invalid type");

	// Verify the size of the file if needed
	VerifySize(matrix.GetNbLines(),matrix.GetNbCols());

	// Truncate the files
	File1.Truncate(0);
	Index.Truncate(0);

	if(Sparse)
		SaveSparse(matrix);
	else if(Max)
		SaveMax(matrix);
	else
		SaveFull(matrix);
}


//------------------------------------------------------------------------------
double RMatrixStorage::Read(size_t i,size_t j)
{
	if(BaseURI==RString::Null)
		ThrowRIOException(&Index,"File not open");
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
	{
		ThrowRIOException(&Index,"RMaxMatrix not supported");
	}
	else if(Sparse)
	{
		// Search the position of the first element of the line
		Index.Seek(2*sizeof(size_t)+i*4*sizeof(off_t));
		off_t NextPos;
		Index>>NextPos;

		// Scan the whole line until the column is found or the line is ended
		while(NextPos!=MaxOffT)
		{
			size_t line,col;

			File1.Seek(NextPos);
			File1>>res>>line>>col>>NextPos;
			if(col==j)
				return(res);
		}

		// Nothing found -> res=0
		res=0;
	}
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
void RMatrixStorage::Write(size_t i,size_t j,double val)
{
	if(BaseURI==RString::Null)
		throw RException("RMatrixStorage::Save(RGenericMatrix&) : File not open");
	if((i>NbLines)||(j>NbCols))
		throw std::range_error("RMatrixStorage::Write(size_t,size_t,double) : index "+RString::Number(i)+","+RString::Number(j)+" outside range ("+RString::Number(NbLines)+","+RString::Number(NbCols)+")");

	if(((Type==RGenericMatrix::tSymmetric)||(Type==RGenericMatrix::tSparseSymmetric))&&(j>i))
	{
		size_t tmp(i);
		i=j;
		j=tmp;
	}

	if(Max)
	{
		ThrowRIOException(&Index,"RMaxMatrix not supported");
	}
	else if(Sparse)
	{
		// Search the position of the first element of the line
		Index.Seek(2*sizeof(size_t)+i*4*sizeof(off_t));
		off_t NextPos;
		Index>>NextPos;

		// Scan the whole line until the column is found or the line is ended
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
		Index.Seek(2*sizeof(size_t)+i*4*sizeof(off_t)+2*sizeof(off_t));
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
	else
	{
		if(i<j)
		{
			// Read in the upper part
			File2.Seek((j*(j-1))/2+i);
			File2<<val;
		}
		else
		{
			// Read in the lower part
			File1.Seek((i*(i+1))/2+j);
			File1<<val;
		}
	}
}


//------------------------------------------------------------------------------
void RMatrixStorage::ReadInfo(size_t posinfo,char* buffer,size_t nb)
{
	if(BaseURI==RString::Null)
		throw RException("RMatrixStorage::ReadInfo(size_t,char*,size_t) : File not open");

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
		throw RException("RMatrixStorage::ReadInfo(size_t,char*,size_t) : Internal problem in the information stored");
}


//------------------------------------------------------------------------------
void RMatrixStorage::WriteInfo(size_t posinfo,const char* buffer,size_t nb)
{
	if(BaseURI==RString::Null)
		throw RException("RMatrixStorage::WriteInfo(size_t,const char*,size_t) : File not open");

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
