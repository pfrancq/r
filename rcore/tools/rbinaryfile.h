/*

	R Project Library

	RBinaryFile.h

	Binary file - Header.

	Copyright 2006 by the Université Libre de Bruxelles.

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
#ifndef RBinaryFile_H
#define RBinaryFile_H


//------------------------------------------------------------------------------
// include files for R Project
#include <riofile.h>


//------------------------------------------------------------------------------
namespace R{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* The RBinaryFile class implements some basic functions needed when working with
* binariy files.
*
* Here is an example:
* @code
* #include <string.h>
* #include <rbinaryfile.h>
* using namespace R;
*
* int main(int argc, char *argv[])
* {
*    // Create a binary file
*    R::RBinaryFile File("/home/pfrancq/test.bin");
*
*    // Fill it
*    std::cout<<"Create"<<std::endl;
*    File.Open(R::RIO::Create);
*    double d=2.6;
*    char str[8]="coucou";
*    RString str2("coucou");
*    File<<d<<strlen(str)<<str<<str2;
*    File.Close();
*
*    // Read it
*    std::cout<<"Read"<<std::endl;
*    File.Open(R::RIO::Read);
*    size_t len;  // strlen returns the length in a size_t
*    File>>d>>len;
*    char* read=new char[len+1];
*    RString read2;
*    File>>read>>read2;
*    std::cout<<"double="<<d<<" ; string="<<read<<" ; rstring="<<read2<<std::endl;
*    delete[] read;
*    File.Close();
* }
* @endcode
* @author Pascal Francq
* @short Binary File.
*/
class RBinaryFile : public RIOFile
{
public:

	/**
	* Construct a binary file.
	* @param name           The name of the file.
	*/
	RBinaryFile(const RString &name);

	/**
	* Construct a binary file.
	* @param file           A generic input/output file that should be treated
	*                       as binary file.
	*/
	RBinaryFile(RIOFile& file);

	/**
	* >> Operator for bool.
	*/
	RBinaryFile& operator>>(bool& b);

	/**
	* >> Operator for string.
	*/
	RBinaryFile& operator>>(char* str);

	/**
	* >> Operator for string.
	*/
	RBinaryFile& operator>>(RString& str);

	/**
	* >> Operator for char.
	*/
	RBinaryFile& operator>>(char& nb);

	/**
	* >> Operator for unsigned char.
	*/
	RBinaryFile& operator>>(unsigned char& nb);

	/**
	* >> Operator for int.
	*/
	RBinaryFile& operator>>(int& nb);

	/**
	* >> Operator for unsigned int.
	*/
	RBinaryFile& operator>>(unsigned int& nb);

	/**
	* >> Operator for long.
	*/
	RBinaryFile& operator>>(long& nb);

	/**
	* >> Operator for unsigned long.
	*/
	RBinaryFile& operator>>(unsigned long& nb);

	/**
	* >> Operator for long long.
	*/
	RBinaryFile& operator>>(long long& nb);

	/**
	* >> Operator for unsigned long long.
	*/
	RBinaryFile& operator>>(unsigned long long& nb);

	/**
	* >> Operator for a float.
	*/
	RBinaryFile& operator>>(float& nb);

	/**
	* >> Operator for a double.
	*/
	RBinaryFile& operator>>(double& nb);

	/**
	* << Operator for bool.
	*/
	RBinaryFile& operator<<(bool b);

	/**
	* >> Operator for string.
	*/
	RBinaryFile& operator<<(char* str);

	/**
	* >> Operator for string.
	*/
	RBinaryFile& operator<<(RString& str);

	/**
	* << Operator for char.
	*/
	RBinaryFile& operator<<(char nb);

	/**
	* << Operator for unsigned char.
	*/
	RBinaryFile& operator<<(unsigned char nb);

	/**
	* << Operator for int.
	*/
	RBinaryFile& operator<<(int nb);

	/**
	* << Operator for unsigned int.
	*/
	RBinaryFile& operator<<(unsigned int nb);

	/**
	* << Operator for long.
	*/
	RBinaryFile& operator<<(long nb);

	/**
	* << Operator for unsigned long.
	*/
	RBinaryFile& operator<<(unsigned long nb);

	/**
	* << Operator for long long.
	*/
	RBinaryFile& operator<<(long long nb);

	/**
	* << Operator for unsigned long long.
	*/
	RBinaryFile& operator<<(unsigned long long nb);

	/**
	* << Operator for float.
	*/
	RBinaryFile& operator<<(float nb);

	/**
	* << Operator for double.
	*/
	RBinaryFile& operator<<(double nb);

	/**
	* Destructs the file.
	*/
	virtual ~RBinaryFile(void);
};


}  //-------- End of namespace R -----------------------------------------------


//------------------------------------------------------------------------------
#endif
