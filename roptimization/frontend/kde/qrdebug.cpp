/*
 	R Project Library

	QRDebug.cpp

	Widget Debugger - Implementation.

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



//-----------------------------------------------------------------------------
// include files for Qt/KDE
#include <QtGui/QTreeWidgetItem>
#include <QtGui/QTreeWidget>


//------------------------------------------------------------------------------
// include files for R Library
#include <ui_qrdebug.h>
#include <qrdebug.h>
#include <rqt.h>
using namespace R;



//------------------------------------------------------------------------------
//
// QRDebug::Item
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
class QRDebug::Item : public QTreeWidgetItem
{
public:
	Item(QTreeWidget* parent) : QTreeWidgetItem(parent) {}
	Item(Item* item) : QTreeWidgetItem(item) {}
	Item(Item* item,Item* ptr) : QTreeWidgetItem(item,ptr) {}
	int Compare(const Item&) const {return(-1);}
};



//------------------------------------------------------------------------------
//
// QRDebug
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
QRDebug::QRDebug(QWidget* parent)
	: QWidget(parent), RDebug(), Ui(new Ui_QRDebug()), Items(50)
{
	static_cast<Ui_QRDebug*>(Ui)->setupUi(this);
	BeginTag("RDebug");
}


//------------------------------------------------------------------------------
void QRDebug::clear(void)
{
	static_cast<Ui_QRDebug*>(Ui)->Infos->clear();
	Items.Clear();
	Depth=-1;
}


//------------------------------------------------------------------------------
void QRDebug::WriteBeginTag(const RString& tag,const RString& attrs)
{
	if(Depth)
	{
		if(static_cast<size_t>(Depth)<=Items.GetMaxPos())
			Items.InsertPtrAt(new Item(Items[Depth-1],Items[Depth]),Depth,true);
		else
			Items.InsertPtrAt(new Item(Items[Depth-1]),Depth,true);
	}
	else
		Items.InsertPtrAt(new Item(static_cast<Ui_QRDebug*>(Ui)->Infos),Depth,true);
	Items[Depth]->setText(0,ToQString(tag));
	if(!attrs.IsEmpty())
	{
		Items[Depth]->setText(1,ToQString(attrs));
	}
	if(static_cast<size_t>(Depth)<Items.GetMaxPos())
		Items.DeletePtrAt(Depth+1,false);
}


//------------------------------------------------------------------------------
void QRDebug::WriteEndTag(const RString& /*tag*/)
{
	if(static_cast<size_t>(Depth)<Items.GetMaxPos())
		Items.DeletePtrAt(Depth+1,false);
}


//------------------------------------------------------------------------------
void QRDebug::WriteText(const RString& text)
{
	Items[Depth]->setText(2,ToQString(text));
}


//------------------------------------------------------------------------------
QRDebug::~QRDebug(void)
{
	emit signalXMLClose();
}
