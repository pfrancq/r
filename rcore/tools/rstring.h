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
  RString(const RString& str);
	RString(RString* str);

	// Assignation
  RString& operator=(const RString &str);
  RString& operator=(const char *text);

  // Transform to uppercase
  inline void StrUpr(void);
  void StrUpr(char *text);
	void StrUpr(RString &str);

  // Transform to lowercase
  inline void StrLwr(void);
  void StrLwr(char *text);
  void StrLwr(RString &str);

  // Addition
  RString& operator+=(const RString &str);
  RString& operator+=(const char *text);

  // Content
  inline char operator[](int i) const	{return(Text[i]);}
  inline char* operator()(void) const {return(Text);}

  // Comparaison
  bool operator==(const RString &str);
  bool operator!=(const RString &str);
  int Compare(const RString &str);
  int Compare(const RString *str);
  inline char HashIndex(void) const
  {
  	char c=(*Text);
  	if(c>='a'&&c<='z') return(c-'a');
		if(c>='A'&&c<='Z') return(c-'A');  	
		return(26);
  }

  // Find a string
  inline char* FindStr(char *text);

  // Return lengths
  inline int GetLen(void) {return(Len);}
  inline int GetMaxLen(void) {return(MaxLen);}

  // Destructor
  ~RString(void){if(Text) delete[] Text;}

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
