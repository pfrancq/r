/*

  RChromo2D.hh

  Chromosome for 2D placement GA - Inline Implementation

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
template<class cInst,class cChromo,class cFit,class cInfo>
	RChromo2D<cInst,cChromo,cFit,cInfo>::RChromo2D(cInst *inst,unsigned id) throw(bad_alloc)
		: RChromo<cInst,cChromo,cFit>(inst,id)
{
 	RObj2D **obj;
 	unsigned int i;
 	cInfo **ptr,**ptr2;

  Infos=new cInfo*[NbObjects];
  for(i=NbObjects+1,obj=Instance->Objs,ptr=Infos,ptr2=Instance->Chromosomes[0]->Infos;--i;ptr++,obj++,ptr2++)
  	if(Id&&!(*obj)->Deformable)
  		(*ptr)=(*ptr2);
  	else
  		(*ptr)=new cInfo();
}


//---------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cInfo>
	bool RChromo2D<cInst,cChromo,cFit,cInfo>::RandomConstruct(void)
{
	return(true);
}


//---------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cInfo>
	RChromo2D<cInst,cChromo,cFit,cInfo>::~RChromo2D(void)
{
  cInfo **ptr;
	RObj2D **obj;
  unsigned int i;

  for(i=NbObjects+1,ptr=Infos,obj=Instance->Objs;--i;ptr++,obj++)
		if(!Id||(*obj)->Deformable)
      delete(*ptr);
  delete[] Infos;
}
