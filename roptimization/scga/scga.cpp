/*

	R Project Library

	SGGA.cpp

	Similarity-bases Clustering Genetic Algorithm - Implementation.

	Copyright 2002-2010 by Pascal Francq (pascal@francq.info).
	Copyright 2002-2008 by the Université Libre de Bruxelles (ULB).

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



//-----------------------------------------------------------------------------
// includes files for GCA
#include <scga.h>
using namespace R;



//-----------------------------------------------------------------------------
//
// class GCAParams
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
RParamsSC::RParamsSC(void)
	: PopSize(16), MaxGen(50), Step(false), StepGen(1),MinSimLevel(0.0),
	  MinAgreement(1.0), MinDisagreement(1.0), ParamsSim(0), ParamsAgreement(0),
	  ParamsDisagreement(0), Incremental(false), NbMinObjs(0), NbMaxObjs(cNoRef)
{
}



//-----------------------------------------------------------------------------
//
// class RObjSC
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
RObjSC::RObjSC(const size_t id,const RString& name,bool social,size_t parentid)
	: RObjG(id,name), Social(social), ParentId(parentid)
{
}
