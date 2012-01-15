/*

	R Project Library

	SGGA.cpp

	Similarity-based Grouping Genetic Algorithm - Implementation.

	Copyright 2002-2012 by Pascal Francq (pascal@francq.info).
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
#include <sgga.h>
using namespace R;


//-----------------------------------------------------------------------------
//
// RCloseObjSG
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
RCloseObjSG::RCloseObjSG(size_t id,double ratio)
	: Id(id), Ratio(ratio)
{
}


//-----------------------------------------------------------------------------
int RCloseObjSG::Compare(const RCloseObjSG& ratio) const
{
	return(static_cast<int>(Ratio-ratio.Ratio));
}


//-----------------------------------------------------------------------------
int RCloseObjSG::sortOrder(const void* a,const void* b)
{
	double af=(*((RCloseObjSG**)(a)))->Ratio;
	double bf=(*((RCloseObjSG**)(b)))->Ratio;

	if(af==bf) return(0);
	if(af>bf)
		return(-1);
	else
		return(1);
}


//-----------------------------------------------------------------------------
//
// RObjRefSG
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
RObjRefSG::RObjRefSG(size_t nb)
	: CloseObjs(nb), MustReOrder(false), Social (true)
{
}


//-----------------------------------------------------------------------------
int RObjRefSG::Compare(const RObjRefSG&) const
{
	return(-1);
}


//-----------------------------------------------------------------------------
//
// RParamsSG
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
RParamsSG::RParamsSG(void)
	: Objs(2000),
	  PopSize(16), MaxGen(50), Step(false), StepGen(1),
	  MinSimLevel(0.0), MinAgreement(1.0), MinDisagreement(1.0),
	  MaxKMeans(10), Convergence(0.0001), NbDivChromo(2),
	  ParamsSim(0), ParamsAgreement(0), ParamsDisagreement(0),
	  LocalOptimisation(false), Optimisation(false), Incremental(false),
	  NbMinObjs(0), NbMaxObjs(cNoRef), AllMinSim(false)
{
}


//------------------------------------------------------------------------------
void RParamsSG::InitObjs(size_t highid,size_t nb)
{
	Objs.Clear(highid+1);
	for(size_t i=0;i<=highid;i++)
		Objs.InsertPtrAt(new RObjRefSG(nb),i);
}


//------------------------------------------------------------------------------
void RParamsSG::AddCloseObject(size_t id,size_t closeid,double ratio)
{
	RObjRefSG* Obj(Objs[id]);
	Obj->CloseObjs.InsertPtr(new RCloseObjSG(closeid,ratio));
	Obj->MustReOrder=true;
}
