/*

	R Project Library

	RRecFile.h

	Binary file for records - Header.

	Copyright 2002-2015 by Pascal Francq (pascal@francq.info).
	Copyright 2002-2008 by the Universit√© Libre de Bruxelles (ULB).

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
#include <rbinaryfile.h>


//------------------------------------------------------------------------------
namespace R{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* @tparam C                  The class of the elements to be read/write.
* @tparam bOrder             Specify if the elements are ordered in the file.
*
* The RRecFile class implements some basic functions needed when working with
* files for records. This files can be accessed as ordered or not. Opening a
* non ordered file as ordered gives problems.
*
* As for the RContainer class, the TUse represent a class or a structure used
* for the comparaisons.
*
*
* At least, one read function must be implemented in the class C:
* @code
* void Read(R::RecFile<C,bOrder>& f);
* @endcode
*
* The user can write his own operators to read and write record from a
* RRecFile. Here is an example:
* @code
* #include <rrecfile.h>
* using namespace R;
*
* class Person
* {
* public:
*    R::RString LastName;
*    R::RString FirstName;
*
*    Person(void) {}
*    Person(const R::RString& l,const RString& f) : LastName(l), FirstName(f) {}
*    Person(const Person& p) : LastName(p.LastName), FirstName(p.FirstName) {}
*    int Compare(const Person& p) const {return(LastName.Compare(p.LastName));}
*    int Compare(const R::RString& name) const {return(LastName.Compare(name));}
*    void Read(R::RRecFile<Person,true>& f);
*    void Write(R::RRecFile<Person,true>& f);
* };
*
* void Person::Read(R::RRecFile<Person,true>& f)
* {
*    char Buf[41];
*    f.Read(Buf,40);
*    LastName=Buf;
*    f.Read(Buf,40);
*    FirstName=Buf;
* }
*
* void Person::Write(R::RRecFile<Person,true>& f)
* {
*    char Buf[41];
*    strcpy(Buf,LastName.Latin1());
*    f.Write(Buf,40);
*    strcpy(Buf,FirstName.Latin1());
*    f.Write(Buf,40);
* }
*
* int main(int argc, char *argv[])
* {
*    R::RContainer<Person,true,true> Cont(2,1);
*    R::RCursor<Person> Cur;
*    R::RRecFile<Person,true> File("/home/pfrancq/test.bin",80); // Each record have a size of 80 bytes.
*
*    // Create container -> create file -> write data
*    std::cout<<"Create"<<std::endl;
*    Cont.InsertPtr(new Person("Jagger","Mike"));
*    Cont.InsertPtr(new Person("Gillian","Ian"));
*    Cont.InsertPtr(new Person("Plant","Robert"));
*    Cont.InsertPtr(new Person("Coverdale","David"));
*    File.Open(R::RIO::Create);
*    Cur.Set(Cont);
*    for(Cur.Start();!Cur.End();Cur.Next())
*       Cur()->Write(File);
*    File.Close();
*
*    // Clean container -> open file -> read data
*    std::cout<<"Read"<<std::endl;
*    Cont.Clear();
*    File.Open(R::RIO::Read);
*    for(File.Start();!File.End();File.Next())
*       Cont.InsertPtr(new Person(*File()));
*    Cur.Set(Cont);
*    for(Cur.Start();!Cur.End();Cur.Next())
*       std::cout<<"  "<<Cur()->LastName<<", "<<Cur()->FirstName<<std::endl;
*    File.Close();
*
*    // Just find the firstname of "Gillian" directly in the file
*    std::cout<<"Find"<<std::endl;
*    File.Open(R::RIO::Read);
*    if(File.Search("Gillian"))
*       std::cout<<"  "<<File()->LastName<<", "<<File()->FirstName<<std::endl;
*    File.Close();
* }
* @endcode
* @short Fixed-length Records File.
*/
template<class C,bool bOrder=false>
	class RRecFile : public RBinaryFile
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

	/**
	 * Size of a record.
	 */
	 size_t RecSize;

public:

	/**
	 * Default constructor.
	 */
	RRecFile(void);

	/**
	* Construct a binary file for records.
	* @param uri            URI of the file.
	* @param recsize        Size of a given record.
	*/
	RRecFile(const RURI& uri,size_t recsize);

	/**
	* Open the file
	* @param mode            The open mode for the file.
	*/
	virtual void Open(RIO::ModeType mode);

	/**
	* Open the file
	* @param uri             URI of the file.
	* @param recsize         Size of a given record.
	* @param mode            The open mode for the file.
	*/
	void Open(const RURI& uri,size_t recsize,RIO::ModeType mode=RIO::Read);

	/**
	* Close the file
	*/
	virtual void Close(void);

	/**
	* Seek the file to a specific record number.
	* @param nb             Number of record.
	*/
	void GoToRec(size_t nb);

	/**
	* Seek the file to a specific record number if the file represent a matrix.
	* @param c             Column.
	* @param l             Line.
	* @param maxc          Number of Columns.
	* \remarks It is presume that the file is store lines by lines.
	*/
	void GoToRec(size_t c,size_t l,size_t maxc);

	/**
	* This function returns the number of a record represented by tag, and it
	* is used when the file is ordered. If the record was found, it becomes the
	* current one accessible through the operator().
	* @tparam TUse          The type of tag, the file uses the Compare(TUse &)
	*                       member function of the records.
	* @param tag            The tag used.
	* @return Returns the number of the record if it exists or the index where
	* is has to inserted.
	*/
	template<class TUse> size_t Search(const TUse& tag);

	/**
	* Is the file pointing to a valid record?
	*/
	bool FindRecord(void) {return(Find);}

	/**
	* Get the number of records in the file.
	*/
	size_t GetRecNb(void) const;

	/**
	* Get the current record number.
	*/
	size_t GetCurrentRec(void) const;

	/**
	* Read the current record.
	* \remarks This method change the current position of the file to next one.
	*/
	void ReadRec(C& rec);

	/**
	* Write the current record.
	*/
	void WriteRec(C& rec);

	/**
	* Start the file at the first record.
	*/
	void Start(void);

	/**
	* Goto the next record.
	*/
	void Next(void);

	/**
	* Goto the previous record.
	*/
	void Prev(void);

	/**
	* Return the current record.
	* \remarks This method change the current position of the file to next one.
	*/
	C* operator()(void);

	/**
	* Destruct the file.
	*/
	virtual ~RRecFile(void);
};


//------------------------------------------------------------------------------
// inline implementation
#include <rrecfile.hh>


}  //-------- End of namespace R -----------------------------------------------


//------------------------------------------------------------------------------
#endif
