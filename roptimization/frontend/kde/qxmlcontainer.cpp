/*
 	R Project Library

	QXMLContainer.cpp

	Qt XML Debugger - Implementation.

	Copyright 2000-2009 by Pascal Francq (pascal@francq.info).
	Copyright 2000-2008 by the Université Libre de Bruxelles (ULB).

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
// QXMLContainer::Item
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
class QXMLContainer::Item : public QTreeWidgetItem
{
public:
	Item(QXMLContainer* parent) : QTreeWidgetItem(parent) {}
	Item(Item* item) : QTreeWidgetItem(item) {}
	Item(Item* item,Item* ptr) : QTreeWidgetItem(item,ptr) {}
	int Compare(const Item&) const {return(-1);}
};



//------------------------------------------------------------------------------
//
// QXMLContainer
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
QXMLContainer::QXMLContainer(QWidget* parent)
	: QTreeWidget(parent), RDebug(), Items(50)
{
	setColumnCount(3);
	setHeaderLabels(QStringList() << "Steps" << "Params" << "Infos");
	setRootIsDecorated(true);
//	setSorting(-1,true);
	setAllColumnsShowFocus(true);
	BeginTag("RDebug");
}


//------------------------------------------------------------------------------
void QXMLContainer::clear(void)
{
	QTreeWidget::clear();
	Items.Clear();
	Depth=-1;
}


//------------------------------------------------------------------------------
void QXMLContainer::WriteBeginTag(const RString& tag,const RString& attrs)
{
	Item *ptr;

	if(Depth)
	{
		ptr=Items[Depth];
		if(ptr)
			Items.InsertPtrAt(new Item(Items[Depth-1],ptr),Depth,true);
		else
			Items.InsertPtrAt(new Item(Items[Depth-1]),Depth,true);
	}
	else
		Items.InsertPtrAt(new Item(this),Depth,true);
	Items[Depth]->setText(0,ToQString(tag));
	if(!attrs.IsEmpty())
	{
		Items[Depth]->setText(1,ToQString(attrs));
	}
	Items.DeletePtrAt(Depth+1,false);
}


//------------------------------------------------------------------------------
void QXMLContainer::WriteEndTag(const RString& /*tag*/)
{
	Items.DeletePtrAt(Depth+1,false);
}


//------------------------------------------------------------------------------
void QXMLContainer::WriteText(const RString& text)
{
	Items[Depth]->setText(2,ToQString(text));
}


//------------------------------------------------------------------------------
QXMLContainer::~QXMLContainer(void)
{
	emit signalXMLClose();
}
