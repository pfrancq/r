/*

	 QXMLContainer.cpp

	Qt XML Debugger - Implementation.

	(c) 2000-2002 by P. Francq.

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
// include files for ANSI C/C++
#include <stdio.h>


//-----------------------------------------------------------------------------
// include files for Qt Widgets
#include <qxmlcontainer.h>
using namespace R;



//-----------------------------------------------------------------------------
//
// QXMLContainer
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
QXMLContainer::QXMLContainer(QWidget* parent,const char* app,const char* author,const char* name)
	: QListView(parent,name),RDebug(app,author)
{
	memset(Items,0,50*sizeof(QListViewItem *));
	addColumn("Steps");
	addColumn("Params");
	addColumn("Infos");
	setRootIsDecorated(true);
	setSorting(-1,true);
	setAllColumnsShowFocus(true);
}


//-----------------------------------------------------------------------------
void QXMLContainer::clear(void)
{
	QListView::clear();
	memset(Items,0,50*sizeof(QListViewItem *));
	Deep=-1;
}


//-----------------------------------------------------------------------------
void QXMLContainer::WriteBeginTag(const char* tag,const char* options)
{
	QListViewItem *ptr;

	if(Deep)
	{
		ptr=Items[Deep];
		if(ptr)
			Items[Deep]=new QListViewItem(Items[Deep-1],ptr,tag);
		else
			Items[Deep]=new QListViewItem(Items[Deep-1],tag);
	}
	else
		Items[Deep]=new QListViewItem(this,tag);
	if(options)
	{
		Items[Deep]->setText(1,options);
	}
	Items[Deep+1]=0;
}


//-----------------------------------------------------------------------------
void QXMLContainer::WriteEndTag(const char* /*tag*/)
{
	Items[Deep+1]=0;
}


//-----------------------------------------------------------------------------
void QXMLContainer::WriteText(const char* text)
{
	Items[Deep]->setText(2,text);
}


//-----------------------------------------------------------------------------
QXMLContainer::~QXMLContainer(void)
{
	emit signalXMLClose();
}
