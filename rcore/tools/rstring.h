/*

  RString.h

  String - Header.

  (C) 1999-2000 by P. Francq.

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


//---------------------------------------------------------------------------
#ifndef RStringH
#define RStringH


//---------------------------------------------------------------------------
#ifdef unix
	#ifndef NULL
		#define NULL 0
	#endif
	#include <string.h>
#else
	#include <mem.h>
#endif
#include <new.h>
#ifndef __RMAXSTRING__
	#define __RMAXSTRING__ 30
#endif


//---------------------------------------------------------------------------
namespace RStd{
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
class RString
{
  char *Text;
  int Len,MaxLen;
public:
	// Constructors
  RString(void) throw(bad_alloc);
  RString(const char *text) throw(bad_alloc);
  RString(const int maxlen) throw(bad_alloc);
  RString(const RString& str) throw(bad_alloc);
	RString(const RString* str) throw(bad_alloc);

	// Assignation
  RString& operator=(const RString &str) throw(bad_alloc);
  RString& operator=(const char *text) throw(bad_alloc);

  // Transform to uppercase
  inline void StrUpr(void);
  void StrUpr(const char *text) throw(bad_alloc);
	void StrUpr(const RString &str) throw(bad_alloc);

  // Transform to lowercase
  inline void StrLwr(void);
  void StrLwr(const char *text) throw(bad_alloc);
  void StrLwr(const RString &str) throw(bad_alloc);

  // Addition
  RString& operator+=(const RString &str) throw(bad_alloc);
  RString& operator+=(const char *text) throw(bad_alloc);

  // Content
  inline char operator[](int i) const	{return(Text[i]);}
  inline char* operator()(void) const {return(Text);}

  // Comparaison
  bool operator==(const RString &str) const;
  bool operator!=(const RString &str) const;
  int Compare(const RString &str);
  int Compare(const RString *str);
  inline char HashIndex(void) const
  {
  	char c=(*Text);
    #pragma warn -sig
  	if(c>='a'&&c<='z') return(c-'a');
		if(c>='A'&&c<='Z') return(c-'A');
    #pragma warn .sig
		return(26);
  }

  // Find a string
  char* FindStr(const char *text);

  // Return lengths
  inline int GetLen(void) {return(Len);}
  inline int GetMaxLen(void) {return(MaxLen);}

  // Destructor
  virtual ~RString(void){if(Text) delete[] Text;}

protected:
	inline void Verify(const int maxlen) throw(bad_alloc);	
};


//---------------------------------------------------------------------------
// Need for temporary strings
RString* GetString(void);


//---------------------------------------------------------------------------
// Operators
RString& operator+(const RString &arg1,const RString &arg2);
RString& operator+(const RString &arg1,const char *arg2);
RString& operator+(const char *arg1,const RString &arg2);


//---------------------------------------------------------------------------
// Conversion functions
RString& itoa(const int nb);
RString& itoa(const unsigned int nb);
RString& ltoa(const long nb);
RString& chr(const unsigned char c);
RString& ltoa(const unsigned long nb);
RString& ftoa(const float nb);
RString& dtoa(const double nb);


}  //-------- End of namespace RStd ---------------------------------------


//---------------------------------------------------------------------------
#endif

