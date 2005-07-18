/*

	R Project Library

	RRecFile.h

	Binary file for records - Header.

	Copyright 2002-2005 by the Université Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).

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
#ifndef RRecFile_H
#define RRecFile_H


//------------------------------------------------------------------------------
// include files for R Project
#include <riofile.h>


//------------------------------------------------------------------------------
namespace R{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* The RRecFile class implements some basic functions needed when working with
* files for records. This files can be accessed as ordered or not. Opening a
* non ordered file as ordered gives problems.
*
* As for the RContainer class, the TUse represent a class or a structure used
* for the comparaisons.
*
* At least, one read function must be implemented in the class C:
* @code
* void Read(R::RecFile<C,S,bOrder>& f);
* @endcode
*
* The user can write his own operators to read and write record from a
* RRecFile. Here is an example:
* @code
* class Person
* {
* public:
* 	R::RString LastName;
* 	R::RString FirstName;
*
* 	Person(void) {}
* 	Person(const R::RString& l,const RString& f) : LastName(l), FirstName(f) {}
* 	Person(const Person* p) : LastName(p->LastName), FirstName(p->FirstName) {}
* 	int Compare(const Person* p) const {return(LastName.Compare(p->LastName));}
* 	int Compare(const R::RString& name) const {return(LastName.Compare(name));}
* 	void Read(R::RRecFile<Person,80,true>& f);
* 	void Write(R::RRecFile<Person,80,true>& f);
* };
*
* void Person::Read(R::RRecFile<Person,80,true>& f)
* {
* 	char Buf[41];
* 	f.Read(Buf,40);
* 	LastName=Buf;
* 	f.Read(Buf,40);
* 	FirstName=Buf;
* }
*
* void Person::Write(R::RRecFile<Person,80,true>& f)
* {
* 	char Buf[41];
* 	strcpy(Buf,LastName.Latin1());
* 	f.Write(Buf,40);
* 	strcpy(Buf,FirstName.Latin1());
* 	f.Write(Buf,40);
* }
*
* int main(int argc, char *argv[])
* {
* 	R::RContainer<Person,true,true> Cont(2,1);
* 	R::RCursor<Person> Cur;
* 	R::RRecFile<Person,80,true> File("/home/pfrancq/test.bin");
*
* 	// Create container -> create file -> write data
* 	cout<<"Create"<<endl;
* 	Cont.InsertPtr(new Person("Jagger","Mike"));
* 	Cont.InsertPtr(new Person("Gillian","Ian"));
* 	Cont.InsertPtr(new Person("Plant","Robert"));
* 	Cont.InsertPtr(new Person("Coverdale","David"));
* 	File.Open(R::RIO::Create);
* 	Cur.Set(Cont);
* 	for(Cur.Start();!Cur.End();Cur.Next())
* 		Cur()->Write(File);
* 	File.Close();
*
* 	// Clean container -> open file -> read data
* 	cout<<"Read"<<endl;
* 	Cont.Clear();
* 	File.Open(R::RIO::Read);
* 	for(File.Start();!File.End();File.Next())
		Cont.InsertPtr(new Person(File()));
* 	Cur.Set(Cont);
* 	for(Cur.Start();!Cur.End();Cur.Next())
* 		cout<<"  "<<Cur()->LastName<<", "<<Cur()->FirstName<<endl;
* 	File.Close();
*
* 	// Just find the firstname of "Gillian" directly in the file
* 	cout<<"Find"<<endl;
* 	File.Open(R::RIO::Read);
* 	Person* Find=File.GetRec<RString>("Gillian");
* 	if(Find)
* 		cout<<"  "<<Find->LastName<<", "<<Find->FirstName<<endl;
* 	File.Close();
* }
* @endcode
* @author Pascal Francq
* @short Fixed-length Records File.
*/
template<class C,unsigned int S,bool bOrder=false>
	class RRecFile : public RIOFile
{
protected:

	/**
	* Current record.
	*/
	C Current;

	/**
	* Was the searched record found?
	*/
	bool Find;

	/**
	* Is the record in memory dirty?
	*/
	bool Dirty;

public:

	/**
	* Construct a binary file.
	* @param name           The name of the file.
	*/
	RRecFile(const RString &name);

	/**
	* Construct a binary file.
	* @param file           A generic input/output file that should be treated
	*                       as binary file.
	* @param encoding       The encoding scheme of the file.
	*/
	RRecFile(RIOFile& file);

	/**
	* Open the file
	* @param mode           The open mode for the file.
	*/
	virtual void Open(RIO::ModeType mode=RIO::Read);

	/**
	* Close the file
	*/
	virtual void Close(void);

	/**
	* >> Operator for unsigned int.
	*/
	RRecFile& operator>>(unsigned int& nb);

	/**
	* >> Operator for a double.
	*/
	RRecFile& operator>>(double& nb);

	/**
	* << Operator for unsigned char.
	*/
	RRecFile& operator<<(unsigned char nb);

	/**
	* << Operator for unsigned int.
	*/
	RRecFile& operator<<(unsigned int nb);

	/**
	* << Operator for unsigned long.
	*/
	RRecFile& operator<<(unsigned long nb);

	/**
	* << Operator for double.
	*/
	RRecFile& operator<<(double nb);

	/**
	* Seek the file to a specific record number.
	* @param nb             Number of record.
	*/
	void Seek(unsigned int nb);

	/**
	* Seek the file to a specific record number if the file represent a matrix.
	* @param c             Column.
	* @param l             Line.
	* @param maxc          Number of Columns.
	* \remarks It is presume that the file is store lines by lines.
	*/
	void SeekMatrix(unsigned int c,unsigned int l,unsigned int maxc);

	/**
	* This function returns the number of a record represented by tag, and it
	* is used when the file is ordered.
	* @param TUse           The type of tag, the file uses the Compare(TUse &)
	*                       member function of the records.
	* @param tag            The tag used.
	* @param find           If the record represented by tag exist, find is set to
	*                       true.
	* @return Returns the number of the record if it exists or the index where
	* is has to inserted.
	*/
	template<class TUse> unsigned int Search(const TUse& tag);

	/**
	* Get the number of records in the file.
	*/
	unsigned int GetSize(void) const;

	/**
	* Get the current record number.
	*/
	unsigned int GetPos(void) const;

	/**
	* Start the file at the first record.
	*/
	void Start(void);

	/**
	* Goto the next record.
	*/
	void Next(void);

	/**
	* Return the current record.
	*/
	C* operator()(void);

	/**
	* Destructs the file.
	*/
	virtual ~RRecFile(void);
};


//------------------------------------------------------------------------------
// inline implementation
#include <rrecfile.hh>


}  //-------- End of namespace R -----------------------------------------------


//------------------------------------------------------------------------------
#endif
