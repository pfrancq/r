/*
 	R Project Library

	QXMLContainer.cpp

	Qt XML Debugger - Implementation.

	Copyright 2000-2007 by the Université Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).

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
	Boston, MA  02111-1307  USA* 

	 QXMLContainer.cpp
*/



//------------------------------------------------------------------------------
// include files for ANSI C/C++
#include <stdio.h>


//------------------------------------------------------------------------------
// include files for R Library
#include <qxmlcontainer.h>
#include <rqt.h>
using namespace R;



//------------------------------------------------------------------------------
//
// QXMLContainer
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
QXMLContainer::QXMLContainer(QWidget* parent,RString name)
	: QListView(parent,name), RDebug()
{
	memset(Items,0,50*sizeof(QListViewItem *));
	addColumn("Steps");
	addColumn("Params");
	addColumn("Infos");
	setRootIsDecorated(true);
	setSorting(-1,true);
	setAllColumnsShowFocus(true);
	BeginTag("RDebug");
}


//------------------------------------------------------------------------------
void QXMLContainer::clear(void)
{
	QListView::clear();
	memset(Items,0,50*sizeof(QListViewItem *));
	Deep=-1;
}


//------------------------------------------------------------------------------
void QXMLContainer::WriteBeginTag(RString tag,RString options)
{
	QListViewItem *ptr;

	if(Deep)
	{
		ptr=Items[Deep];
		if(ptr)
			Items[Deep]=new QListViewItem(Items[Deep-1],ptr,ToQString(tag));
		else
			Items[Deep]=new QListViewItem(Items[Deep-1],ToQString(tag));
	}
	else
		Items[Deep]=new QListViewItem(this,ToQString(tag));
	if(!options.IsEmpty())
	{
		Items[Deep]->setText(1,ToQString(options));
	}
	Items[Deep+1]=0;
}


//------------------------------------------------------------------------------
void QXMLContainer::WriteEndTag(RString /*tag*/)
{
	Items[Deep+1]=0;
}


//------------------------------------------------------------------------------
void QXMLContainer::WriteText(RString text)
{
	Items[Deep]->setText(2,ToQString(text));
}


//------------------------------------------------------------------------------
QXMLContainer::~QXMLContainer(void)
{
	emit signalXMLClose();
}
