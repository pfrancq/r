/*

	R Project Library

	RTestStd.h

	Standard Test Macros - Header.

	Copyright 1999-2013 by Pascal Francq (pascal@francq.info).
	Copyright 1999-2008 by the Université Libre de Bruxelles (ULB).

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
#ifndef RTestStdH
#define RTestStdH


//------------------------------------------------------------------------------
// include files for R library
#include <rcursor.h>


//------------------------------------------------------------------------------
// Assertion macros
#ifdef __RDISABLEASSERT__

	#define mAssertSameContainers(class1,obj1,class2,obj2)

#else 		// !__RDISABLEASSERT__

	#define mAssertSameContainers(class1,obj1,class2,obj2)                              \
	{                                                                                   \
		R::RCursor<class1> Cur1(obj1);                                                   \
		R::RCursor<class2> Cur2(obj2);                                                   \
		if(Cur1.GetNb()==Cur2.GetNb())                                                   \
		{                                                                                \
			for(Cur1.Start(),Cur2.Start();                                                \
             (!Cur1.End())&&(!Cur2.End());                                             \
				 Cur1.Next(),Cur2.Next())                                                  \
			{                                                                             \
				if(!(*Cur1()==*Cur2()))                                                    \
				{                                                                          \
					size_t Idx(Cur1.GetPos()<=Cur2.GetPos()?Cur1.GetPos():Cur2.GetPos());   \
					std::cerr<<mInfo                                                        \
								<<": assertion failed: "                                       \
								<<#obj1<<"["<<Idx<<"]=="                                       \
								<<#obj2<<"["<<Idx<<"]"<<std::endl;                             \
					break;                                                                  \
				}                                                                          \
			}                                                                             \
		}                                                                                \
		else                                                                             \
		{                                                                                \
			std::cerr<<mInfo                                                              \
			         <<": assertion failed: |"                                            \
			         <<#obj1<<"|==|"<<#obj2<<"|"<<std::endl;                              \
		}                                                                                \
}

#endif 		// !__RDISABLEASSERT__


//------------------------------------------------------------------------------
#endif
