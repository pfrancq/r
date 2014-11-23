/*

	GALILEI Research Project

	RXMLStruct.cpp

	Widget to show a XML structure - Implementation.

	Copyright 2001-2014 by Pascal Francq (pascal@francq.info).
	Copyright 2001 by Julien Lamoral.
	Copyright 2001-2008 by the Université Libre de Bruxelles (ULB).

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
// includes files for Qt
#include <QtGui/QPixmap>


//-----------------------------------------------------------------------------
// include files for R
#include <rnodecursor.h>
#include <qrxmlstruct.h>
#include <ui_qrxmlstruct.h>
#include <rqt.h>
using namespace R;



//-----------------------------------------------------------------------------
//
// QListViewItemXMLTag
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
class QListViewItemXMLTag : public QTreeWidgetItem
{
public:
	RXMLTag*  Tag;

	QListViewItemXMLTag(QTreeWidget* v,RXMLTag* t,QString c1)
		: QTreeWidgetItem(v), Tag(t)
		{
			setText(0, c1);
		}

	QListViewItemXMLTag(QTreeWidget* v,QTreeWidgetItem* a,RXMLTag* t,QString c1)
		: QTreeWidgetItem(v,a), Tag(t)
		{
			setText(0, c1);
		}

	QListViewItemXMLTag(QTreeWidgetItem* v,RXMLTag* t,QString c1)
		: QTreeWidgetItem(v), Tag(t)
	{
		setText(0, c1);
	}

	QListViewItemXMLTag(QTreeWidgetItem* v,QTreeWidgetItem* v2,RXMLTag* t,QString c1)
		: QTreeWidgetItem(v,v2), Tag(t)
		{
			setText(0, c1);
		}
};



//-----------------------------------------------------------------------------
//
// QRXMLStruct
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
QRXMLStruct::QRXMLStruct(QWidget* parent)
	: QWidget(parent), Ui(new Ui_QRXMLStruct())
{
	static_cast<Ui_QRXMLStruct*>(Ui)->setupUi(this);
	connect(static_cast<Ui_QRXMLStruct*>(Ui)->Struct,SIGNAL(currentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)),this,SLOT(slotSelectionTag(QTreeWidgetItem*)));
}


//-----------------------------------------------------------------------------
void QRXMLStruct::ConstructTag(RXMLTag* t,QTreeWidgetItem* parent)
{
	RNodeCursor<RXMLStruct,RXMLTag> Cur(t);
	QTreeWidgetItem* ptr=0,*ptr2;
	QTreeWidgetItem* prec=0;

	parent->setIcon(0,QIcon::fromTheme("xml_element.png"));
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		if(!prec)
		{
			prec=ptr=new QListViewItemXMLTag(parent,Cur(),ToQString(Cur()->GetName()));
		}
		else
		{
			prec=ptr=new QListViewItemXMLTag(parent,prec,Cur(),ToQString(Cur()->GetName()));
		}
		if(!Cur()->GetContent().IsEmpty())
		{
			ptr2=new QListViewItemXMLTag(ptr,0,ToQString(Cur()->GetContent().Trim()));
			ptr2->setIcon(0,QIcon::fromTheme("xml_text.png"));
		}
		ConstructTag(Cur(),ptr);
	}
}


//-----------------------------------------------------------------------------
void QRXMLStruct::slotSelectionTag(QTreeWidgetItem* item)
{
	QListViewItemXMLTag* t=(QListViewItemXMLTag*)item;
	RCursor<RXMLAttr> Cur;

	if(t->Tag)
	{
		static_cast<Ui_QRXMLStruct*>(Ui)->TagInfos->setTabEnabled(0,true);
		static_cast<Ui_QRXMLStruct*>(Ui)->TagAttributes->clear();
		Cur=t->Tag->GetAttrs();
		int i;
		for(Cur.Start(),i=0;!Cur.End();Cur.Next(),i++)
			new QTreeWidgetItem(static_cast<Ui_QRXMLStruct*>(Ui)->TagAttributes,QStringList()<<ToQString(Cur()->GetName())<<ToQString(Cur()->GetValue()));
		static_cast<Ui_QRXMLStruct*>(Ui)->TagInfos->setTabEnabled(1,false);
	}
	else
	{
		static_cast<Ui_QRXMLStruct*>(Ui)->TagInfos->setTabEnabled(1,true);
		static_cast<Ui_QRXMLStruct*>(Ui)->TagContent->setText(t->text(0));
		static_cast<Ui_QRXMLStruct*>(Ui)->TagInfos->setTabEnabled(0,false);
	}
}


//-----------------------------------------------------------------------------
void QRXMLStruct::Set(RXMLStruct* xml)
{
	static_cast<Ui_QRXMLStruct*>(Ui)->Struct->clear();
	if(xml)
	{
		RXMLTag* ptr=xml->GetTop();
		if(ptr)
			ConstructTag(ptr,new QListViewItemXMLTag(static_cast<Ui_QRXMLStruct*>(Ui)->Struct,ptr,ToQString(ptr->GetName())));
	}
}


//-----------------------------------------------------------------------------
QRXMLStruct::~QRXMLStruct(void)
{
	delete static_cast<Ui_QRXMLStruct*>(Ui);
}
