/*

  rxmlfile.cpp

  Description - Implementation.

  (c) 2000 by P. Francq.

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
#include <rstd/rxmlfile.h>
using namespace RStd;


/*-----------------------------------------------------------------------------
 * Construct the XML file.
 *
 * Parameters:
 *   name		Name of the file
 *
 * Returns:
 *
 *---------------------------------------------------------------------------*/
RXMLFile::RXMLFile(const char *name,RXMLStruct *xmlstruct)
 : RTextFile(name), XMLStruct(xmlstruct)
{
	RXMLTag *top;

	SetRemStyle(MultiLineComment);
	SetRem("<!--","-->");
	while(*ptr)
	{
		SkipSpaces();
		top=new RXMLTag();
		top->Load(this,XMLStruct);
		XMLStruct->AddNode(0,top);
	}
}


/*-----------------------------------------------------------------------------
 * Verify if the file is at the begin of a tag.
 *
 * Parameters:
 *
 * Returns:
 *   true if it is so.
 *
 *---------------------------------------------------------------------------*/
bool RXMLFile::BeginTag(void)
{
	return(((*ptr)=='<')&&((*(ptr+1))!='/'));
}


/*-----------------------------------------------------------------------------
 * Verify if the file is at the end of a tag.
 *
 * Parameters:
 *
 * Returns:
 *   true if it is so.
 *
 *---------------------------------------------------------------------------*/
bool RXMLFile::EndTag(void)
{
	return(((*ptr)=='<')&&((*(ptr+1))=='/'));
}


/*-----------------------------------------------------------------------------
 * Return the next tag of the XML file.
 *
 * Parameters:
 *
 * Returns:
 *   A string containing the tag.
 *
 *---------------------------------------------------------------------------*/
char* RXMLFile::GetTag(void)
{
	char *tmp=GetCharPtr();
	char c,*ptr2;

	SkipSpaces();
	ptr2=ptr;
	while((*ptr)!='>')
		ptr++;
	ptr++;			// Skip '>'

	// Making ptr2 pointed to a valid C string
	c=(*ptr);
	(*ptr)=0;
	strcpy(tmp,ptr2);
	(*ptr)=c;
	SkipSpaces();

	return(tmp);
}


/*-----------------------------------------------------------------------------
 * Return the contain of the current tag.
 *
 * Parameters:
 *
 * Returns:
 *   A string containing the contains.
 *
 *---------------------------------------------------------------------------*/
char* RXMLFile::GetContains(void)
{
	char *tmp=GetCharPtr();
	char c,*ptr2;

	ptr2=ptr;
	while((*ptr)!='<')
		ptr++;

	c=(*ptr);
	(*ptr)=0;
	strcpy(tmp,ptr2);
	(*ptr)=c;

	return(tmp);
}


/*-----------------------------------------------------------------------------
 * Destruct the XML file.
 *
 * Parameters:
 *   name		Name of the file
 *
 * Returns:
 *
 *---------------------------------------------------------------------------*/
RXMLFile::~RXMLFile()
{
}
