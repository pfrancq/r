/*

  RFunc.h

  Some important functions - Header.

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
#ifndef RFuncH
#define RFuncH


//---------------------------------------------------------------------------
#ifdef unix
	#ifndef NULL
		#define NULL 0
	#endif
#else
	#include <mem.h>
#endif
#include <new.h>
#include <string.h>
#include <stdio.h>


//---------------------------------------------------------------------------
namespace RStd{
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// General Variables
extern char CommentIndicator;                        // Default ';'


//---------------------------------------------------------------------------
char *ReadLine(char *s, int n, FILE *f);
void strupr(char *s);
void strlwr(char *s);


//---------------------------------------------------------------------------
/** This function provides a way to handle temporary objects needed when using
	* standard operators with non standard classes.
	* @author Pascal Francq
	* @param C				The type of class.
	* @param Max      Maximal size of the array (neested operators).
	*/
template<class C,unsigned long Max>
	inline C* GetTemporaryObject(void)
{
  static C tab[Max];
  static long act=0;

	if(act==Max) act=0;
	return(&tab[act++]);
}


}  //-------- End of namespace RStd ---------------------------------------


//---------------------------------------------------------------------------
#endif