/*

  rcons.cpp

  Connections for the 2D Placement - Implementation.

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



//-----------------------------------------------------------------------------
// include files for Rainbow
#include <rmath/random.h>
using namespace RMath;
#include <rga/rconnections.h>
using namespace RGA;



//-----------------------------------------------------------------------------
//
// class RCons
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
RConnections::RConnections(unsigned int m)
	: RContainer<RConnection,unsigned int,true,false>(m,m/2)
{
}


//-----------------------------------------------------------------------------	
void RConnections::Init(void)
{
}


//-----------------------------------------------------------------------------	
double RConnections::GetCon(RGeoInfo **infos,unsigned int nb,RGeoInfo *cur)
{
	double sum;
/*	unsigned int j;
	RGeoInfo **i;
	RConObj *obj;
	RConEntity *e;*/
		
	sum=0.0;
/*	obj=Cons.GetPtr<RGeoInfo*>(cur);
	if(!obj) return(0);
	con=0;
	for(j=nb+1,i=infos;--j;i++)
		if((*i)->IsValid())	// Allready placed
		{
			e=obj->GetPtr<RGeoInfo*>(*i);
			if(e)
				con+=e->Weight;
		}
	return(con);*/
	return(sum);
}


//-----------------------------------------------------------------------------	
RGeoInfo* RConnections::GetMostConnected(RGeoInfo **infos,unsigned int nb)
{
/*	RGeoInfo **i,*best,**Infos;
	unsigned int j;
	unsigned int bestcon;
   	unsigned int con;
			
	// Init part
	Infos = new RGeoInfo*[nb];
	memcpy(Infos,infos,nb*sizeof(RGeoInfo*));
	RRandom::randorder<RGeoInfo*>(Infos,nb);	
	
	// Find first object no placed (=no valid geometric information)
	// Calculate sum of flux off all object placed	
	j=nb-1;
	i=Infos;
	while((*i)->IsValid())
	{
		j--;
		i++;
	}
	if(!j) return(0);	// all object placed		
	best=(*i);
	bestcon=GetCon(Infos,nb,best);	
		
	//	Look for the rest of the object no placed
	for(j++;--j;i++)	
		if(!(*i)->IsValid())	// If no valid -> no place ok
		{
			con=GetCon(Infos,nb,*i);
			if(con>bestcon)
			{
				con=bestcon;
				best=(*i);
			}
		}

	// Return the best find	
	return(best);*/
	return(0);
}


//-----------------------------------------------------------------------------	
double RConnections::GetDistance(RGeoInfo **infos,unsigned int nb,RGeoInfo *cur,RPoint& pos)
{
/*	RGeoInfo **i;
	unsigned int j;
	double sum=0.0;
	RPoint act;
	RConObj *obj;

		
	obj=Cons.GetPtr<RGeoInfo*>(cur);
	if(!obj) return(0.0);
	pos.X+=cur->Width()/2;
	pos.Y+=cur->Height()/2;
	for(j=nb+1,i=infos;--j;i++)
		if(((*i)->IsValid())&&(obj->GetPtr<RGeoInfo*>(*i)))
		{
			act=(*i)->GetPos();
			act.X+=(*i)->Width();
			act.Y+=(*i)->Height();			
			sum+=act.ManhattanDist(pos);
		}
	return(sum);*/
	return(0.0);
}


//-----------------------------------------------------------------------------	
void RConnections::AddConnection(RObj2DConnector* from,RObj2DConnector* to,double weight)
{
	InsertPtr(new RConnection(from,to,weight));
}


//-----------------------------------------------------------------------------	
RConnections::~RConnections(void)
{
}
