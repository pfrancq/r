/*

  RString.cpp

  String - Implementation.

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
#include <rstring.h>
using namespace RStd;
#include <stdio.h>
#include <string.h>


//---------------------------------------------------------------------------
//
// RString
//
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
RString::RString(void) throw(bad_alloc)
{
	MaxLen=200;
  Len=0;
  Text=new char[MaxLen+1];
  (*Text)=0;
}


//---------------------------------------------------------------------------
RString::RString(const char *text) throw(bad_alloc)
{
  MaxLen=Len=strlen(text);
  Text=new char[MaxLen+1];
  memcpy(Text,text,Len+1);
}


//---------------------------------------------------------------------------
RString::RString(const int maxlen) throw(bad_alloc)
{
  MaxLen=maxlen;
  Len=0;
  Text=new char[MaxLen+1];
  (*Text)=0;
}


//---------------------------------------------------------------------------
RString::RString(const RString& str) throw(bad_alloc)
{
  MaxLen=str.MaxLen;
  Len=str.Len;
  Text=new char[MaxLen+1];
  memcpy(Text,str.Text,Len+1);
}


//---------------------------------------------------------------------------
RString::RString(const RString* str) throw(bad_alloc)
{
  MaxLen=str->MaxLen;
  Len=str->Len;
  Text=new char[MaxLen+1];
  memcpy(Text,str->Text,Len+1);
}


//---------------------------------------------------------------------------
inline void RString::Verify(const int maxlen) throw(bad_alloc)
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


//---------------------------------------------------------------------------
RString& RString::operator=(const RString &str) throw(bad_alloc)
{
  Verify(str.MaxLen);
  Len=str.Len;
  memcpy(Text,str.Text,Len+1);
  return(*this);
}


//---------------------------------------------------------------------------
RString& RString::operator=(const char *text) throw(bad_alloc)
{
  Len=strlen(text),
  Verify(Len);
  memcpy(Text,text,Len+1);
  return(*this);
}


//---------------------------------------------------------------------------
inline void RString::StrUpr(void)
{
	char *ptr=Text;
	while(*ptr)
	{
    #pragma warn -sig
		if(((*ptr)>='a')&&((*ptr)<='z'))
			(*ptr)-='a'-'A';
    #pragma warn .sig
		ptr++;
	}
}


//---------------------------------------------------------------------------
void RString::StrUpr(const char *text) throw(bad_alloc)
{
  Len=strlen(text),
  Verify(Len);
  memcpy(Text,text,Len+1);
  StrUpr();
}


//---------------------------------------------------------------------------
void RString::StrUpr(const RString &str) throw(bad_alloc)
{
  Verify(str.MaxLen);
  Len=str.Len;
  memcpy(Text,str.Text,Len+1);
  StrUpr();
}


//---------------------------------------------------------------------------
inline void RString::StrLwr(void)
{
	char *ptr=Text;

	while(*ptr)
	{
    #pragma warn -sig
		if(((*ptr)>='A')&&((*ptr)<='Z'))
			(*ptr)+='a'-'A';
    #pragma warn .sig
		ptr++;
	}
}


//---------------------------------------------------------------------------
void RString::StrLwr(const char *text) throw(bad_alloc)
{
  Len=strlen(text),
  Verify(Len);
  memcpy(Text,text,Len+1);
  StrLwr();
}


//---------------------------------------------------------------------------
void RString::StrLwr(const RString &str) throw(bad_alloc)
{
  Verify(str.MaxLen);
  Len=str.Len;
  memcpy(Text,str.Text,Len+1);
  StrLwr();
}


//---------------------------------------------------------------------------
RString& RString::operator+=(const RString &str) throw(bad_alloc)
{
  Verify(str.Len+Len);
  memcpy(&Text[Len],str.Text,str.Len+1);
  Len+=str.Len;
  return(*this);
}


//---------------------------------------------------------------------------
RString& RString::operator+=(const char *text) throw(bad_alloc)
{
  int len=strlen(text);
  Verify(len+Len);
  memcpy(&Text[Len],text,len+1);
  Len+=len;
  return(*this);
}


//---------------------------------------------------------------------------
bool RString::operator==(const RString &str) const
{
  return(!strcmp(Text,str.Text));
}


//---------------------------------------------------------------------------
bool RString::operator!=(const RString &str) const
{
  return(strcmp(Text,str.Text));
}


//---------------------------------------------------------------------------
int RString::Compare(const RString &str)
{
  return(strcmp(Text,str.Text));
}


//---------------------------------------------------------------------------
int RString::Compare(const RString *str)
{
  return(strcmp(Text,str->Text));
}


//---------------------------------------------------------------------------
char* RString::FindStr(const char *text)
{
	return(strstr(Text,text));
}



//---------------------------------------------------------------------------
//
// Global Functions and Operators
//
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
inline RString* RStd::GetString(void)
{
  static RString tab[__RMAXSTRING__];
  static long act=0;

	if(act==__RMAXSTRING__) act=0;
	return(&tab[act++]);
}


//---------------------------------------------------------------------------
RString& RStd::operator+(const RString &arg1,const RString &arg2)
{
  RString *res=GetString();
  (*res)=arg1;
  return((*res)+=arg2);
}


//---------------------------------------------------------------------------
RString& RStd::operator+(const RString &arg1,const char *arg2)
{
  RString *res=GetString();
  (*res)=arg1;
  return((*res)+=arg2);
}


//---------------------------------------------------------------------------
RString& RStd::operator+(const char *arg1,const RString &arg2)
{
  RString *res=GetString();
  (*res)=arg1;
  return((*res)+=arg2);
}

//---------------------------------------------------------------------------
RString& RStd::itoa(const int nb)
{
	char Tmp[20];	
	RString *res=GetString();
	
	sprintf(Tmp,"%i",nb);
	(*res)=Tmp;
	return((*res));
}


//---------------------------------------------------------------------------
RString& RStd::itoa(const unsigned int nb)
{
	char Tmp[20];	
	RString *res=GetString();
	
	sprintf(Tmp,"%u",nb);
	(*res)=Tmp;
	return((*res));
}


//---------------------------------------------------------------------------
RString& RStd::ltoa(const long nb)
{
	char Tmp[20];	
	RString *res=GetString();
	
	sprintf(Tmp,"%li",nb);
	(*res)=Tmp;
	return((*res));
}


//---------------------------------------------------------------------------
RString& RStd::chr(const unsigned char c)
{
	char Tmp[2];	
	RString *res=GetString();
	
	(*Tmp)=c;
	(*(Tmp+1))=0;
	(*res)=Tmp;
	return((*res));
}


//---------------------------------------------------------------------------
RString& RStd::ltoa(const unsigned long nb)
{
	char Tmp[20];	
	RString *res=GetString();
	
	sprintf(Tmp,"%lu",nb);
	(*res)=Tmp;
	return((*res));
}


//---------------------------------------------------------------------------
RString& RStd::ftoa(const float nb)
{
	char Tmp[20];	
	RString *res=GetString();
	
	sprintf(Tmp,"%f",nb);
	(*res)=Tmp;
	return((*res));
}


//---------------------------------------------------------------------------
RString& RStd::dtoa(const double nb)
{
	char Tmp[20];	
	RString *res=GetString();
	
	sprintf(Tmp,"%f",nb);
	(*res)=Tmp;
	return((*res));
}


//---------------------------------------------------------------------------