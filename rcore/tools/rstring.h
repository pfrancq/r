/*

  RString

  Class representing a string

  By (c) P. Francq, 1999.

  Version 1.0.0.1

  Last Modify: 1999/07/26

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
#include <string.h>


//---------------------------------------------------------------------------
namespace RStd{
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
inline char* StrDup(char *Origin) throw(bad_alloc)
{
  char *Dest;
  int i=strlen(Origin)+1;
  Dest=new char[i];
  memcpy(Dest,Origin,i);
  return(Dest);
}


//---------------------------------------------------------------------------
class RString
{
  char *Text;
  int Len,MaxLen;

public:

  // Default Constructor
  RString(void)
  {
    MaxLen=Len=0;
    Text=new char[1];
    (*Text)=0;
  }
  // Constructor with a text
  RString(const char *text) throw(bad_alloc)
  {
    MaxLen=Len=strlen(text);
    Text=new char[MaxLen+1];
    memcpy(Text,text,Len+1);
  }
  // Constructor with a maximum
  RString(const int maxlen) throw(bad_alloc)
  {
    MaxLen=maxlen;
    Len=0;
    Text=new char[MaxLen+1];
    (*Text)=0;
  }
  // Constructor with a copy
  RString(const RString& str)
  {
    MaxLen=str.MaxLen;
    Len=str.Len;
    Text=new char[MaxLen+1];
    memcpy(Text,str.Text,Len+1);
  }
  // Verify MaxLen
  inline void Verify(const int maxlen) throw(bad_alloc)
  {
    if(MaxLen<maxlen)
    {
      char *tmp;
      MaxLen=maxlen;
      tmp=new char[MaxLen+1];
      if(Text)
      {
        memcpy(tmp,Text,Len+1);
        delete[] Text;
      }
      Text=tmp;
    }
  }
  // Assignment
  RString& operator=(const RString &str)
  {
    Verify(str.MaxLen);
    Len=str.Len;
    memcpy(Text,str.Text,Len+1);
    return(*this);
  }
  RString& operator=(const char *text)
  {
    Len=strlen(text),
    Verify(Len);
    memcpy(Text,text,Len+1);
    return(*this);
  }
  // Transform to uppercase
  inline void StrUpr(void)
  {
		char *ptr=Text;
  	while(*ptr)
  	{
  		if(((*ptr)>='a')&&((*ptr)<='z'))
  			(*ptr)-='a'-'A';
  		ptr++;
  	}
  }
  inline void StrUpr(char *text)
  {
    Len=strlen(text),
    Verify(Len);
    memcpy(Text,text,Len+1);		
    StrUpr();
  }
  inline void StrUpr(RString &str)
  {
    Verify(str.MaxLen);
    Len=str.Len;
    memcpy(Text,str.Text,Len+1);
    StrUpr();
  }
  // Transform to lowercase
  inline void StrLwr(void)
  {
  	char *ptr=Text;
  	
  	while(*ptr)
  	{
  		if(((*ptr)>='A')&&((*ptr)<='Z'))
  			(*ptr)+='a'-'A';
  		ptr++;
  	}
  }
  inline void StrLwr(char *text)
  {
    Len=strlen(text),
    Verify(Len);
    memcpy(Text,text,Len+1);
    StrLwr();
  }
  inline void StrLwr(RString &str)
  {
    Verify(str.MaxLen);
    Len=str.Len;
    memcpy(Text,str.Text,Len+1);
    StrLwr();
  }
  // Addition
  RString& operator+=(const RString &str)
  {
    Verify(str.Len+Len);
    memcpy(&Text[Len],str.Text,str.Len+1);
    Len+=str.Len;
    return(*this);
  }
  RString& operator+=(const char *text)
  {
    int len=strlen(text);
    Verify(len+Len);
    memcpy(&Text[Len],text,len+1);
    Len+=len;
    return(*this);
  }
  // Find a string
  inline char* FindStr(char *text)
  {
    return(strstr(Text,text));
  }
  // Index operator
  inline char& operator[](int i)		        // str[i] = 'x'; l-value
  {
    return(Text[i]);
  }
  inline char operator[](int i) const	    // x = str[i]; r-value
  {
    return(Text[i]);
  }
  // Return a pointer
  inline char* operator()(void)
  {
    return(Text);
  }
  inline bool operator==(const RString &str)
  {
    return(!strcmp(Text,str.Text));
  }
  inline bool operator!=(const RString &str)
  {
    return(strcmp(Text,str.Text));
  }
  // Compare
  inline int Compare(const RString &str) throw(bad_alloc)
  {
    return(strcmp(Text,str.Text));
  }
  inline int Compare(const RString *str) throw(bad_alloc)
  {
    return(strcmp(Text,str->Text));
  }
  inline char HashIndex(void)
  {
  	char c=(*Text);
  	if(c>='a'&&c<='z') return(c-'a');
		if(c>='A'&&c<='Z') return(c-'A');  	
		return(26);
  }
  // Return the length
  inline int GetLen(void)
  {
    return(Len);
  }
  inline int GetMaxLen(void)
  {
    return(MaxLen);
  }
  // Destructor
  ~RString(void)
  {
    if(Text) delete[] Text;
  }
};

inline RString& operator+(const RString &arg1,const RString &arg2)
{
  RString res=arg1;
  return(res+=arg2);
}
inline RString& operator+(const RString &arg1,const char *arg2)
{
  RString res=arg1;
  return(res+=arg2);
}
inline RString& operator+(const char *arg1,const RString &arg2)
{
  RString res=arg1;
  return(res+=arg2);
}

}  //-------- End of namespace RStd ---------------------------------------


//---------------------------------------------------------------------------
#endif
