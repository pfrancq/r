/*

  RDebug.cpp

  Debugging file in XML format (eXtended Markup Language) - Implementation

  (C) 1998-2000 by P. Francq.

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
#include "rdebug.h"
using namespace RGA;


//---------------------------------------------------------------------------
// Variables
char TempString[2000];
char TempString2[2000];
char TempBlank[200];
char TempNL[5];
bool LevelOutput[50];
unsigned LenBlank;
unsigned LenNL;



//---------------------------------------------------------------------------
//
// Class RDebug
//
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
RDebug::RDebug(void)
{
  memset(TempBlank,' ',200);
  strcpy(TempNL,"\n");
  FileName=NULL;
  LenNL=strlen(TempNL);
  FileHandle=0;
  Inc=0;
}


//---------------------------------------------------------------------------
void RDebug::Init(char *Name) throw(bad_alloc)
{
  if(Name)
  {
    FileName=new char[strlen(Name)+1];
    strcpy(FileName,Name);
  }
  FileHandle=open(FileName,O_WRONLY	| O_CREAT | O_TRUNC/* | O_TEXT*/); // O_TEXT no support on UNIX
}


//---------------------------------------------------------------------------
inline void RDebug::PrintBlanks(void)
{
  write(FileHandle,TempBlank,Inc*2);
}


//---------------------------------------------------------------------------
inline void RDebug::PrintNL(void)
{
  write(FileHandle,TempNL,LenNL);
}


//---------------------------------------------------------------------------
void RDebug::Print(char *Text)
{
  LevelOutput[Inc-1]=true;
  PrintNL();
  PrintBlanks();
  write(FileHandle,&"<Info>",6);
  write(FileHandle,Text,strlen(Text));
  write(FileHandle,&"</Info>",7);  
}


//---------------------------------------------------------------------------
void RDebug::PrintTag(char *Text)
{
  LevelOutput[Inc-1]=true;
  PrintBlanks();
  write(FileHandle,Text,strlen(Text));
  PrintNL();
}


//---------------------------------------------------------------------------
void RDebug::BeginTag(char *Text,unsigned NbAttr,...)
{
  va_list ap;
    
  if(Inc) PrintNL();
  PrintBlanks();
  write(FileHandle,"<",1);
  write(FileHandle,Text,strlen(Text));
  va_start(ap, NbAttr);
  while(NbAttr--)
    AddAttribute(va_arg(ap,char*),va_arg(ap,char*));
  va_end(ap);
  write(FileHandle,">",1);
  LevelOutput[Inc++]=false;
}


//---------------------------------------------------------------------------
void RDebug::EndTag(char *Text)
{
  Inc--;
  if(!LevelOutput[Inc])
    write(FileHandle,"No Special Information",22);
  else
  {
    PrintNL();
    PrintBlanks();
  }
  if(Inc)
    LevelOutput[Inc-1]=true;
  write(FileHandle,"</",2);
  write(FileHandle,Text,strlen(Text));
  write(FileHandle,">",1);
}


//---------------------------------------------------------------------------
void RDebug::AddAttribute(char *Value,char *Attr)
{
  write(FileHandle,&" ",1);
  write(FileHandle,Attr,strlen(Attr));
  write(FileHandle,&"=\"",2);
  write(FileHandle,Value,strlen(Value));
  write(FileHandle,&"\"",1);
}


//---------------------------------------------------------------------------
void RDebug::BeginFunc(char *Name,char *Object)
{
  BeginTag(Name,1,"Object",Object);
}


//---------------------------------------------------------------------------
#pragma argsused
void RDebug::EndFunc(char *Name,char *Object)
{
  EndTag(Name);
}

                               
//---------------------------------------------------------------------------
void RDebug::BeginApp(char *App,char *Author)
{
  time_t t;

  time(&t);
  strcpy(TempString,ctime(&t));
  TempString[strlen(TempString)-1]=0;
  BeginTag(App,2,"Author",Author,"Date",TempString);
}


//---------------------------------------------------------------------------
#pragma argsused
void RDebug::EndApp(char *App,char *Author)
{
  EndTag(App);
}


//---------------------------------------------------------------------------
void RDebug::Done(void)
{
  if(FileHandle)
  {
    close(FileHandle);
    chmod(FileName,S_IREAD|S_IWRITE);
    FileHandle=0;
  }
}


//---------------------------------------------------------------------------
RDebug::~RDebug(void)
{
  Done();
  delete[] FileName;
}


//---------------------------------------------------------------------------