/*

	GALILEI Research Project

	QRXMLStruct.h

	Widget to show a XML structure - Header.

	Copyright 2001-2012 by Pascal Francq (pascal@francq.info).
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
#ifndef QRXMLStructH
#define QRXMLStructH


//-----------------------------------------------------------------------------
// include files for R Project
#include <rxmlstruct.h>


//-----------------------------------------------------------------------------
// include files for Qt
#include <QtGui/QTreeWidgetItem>


//-----------------------------------------------------------------------------
namespace R{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/**
* The QRXMLStruct class provides a representation of a XML structure.
* @author Pascal Francq.
* @short XML Structure Widget.
*/
class QRXMLStruct : public QWidget
{
	Q_OBJECT
	void* Ui;

public:

	/**
	* Constructor.
	* @param parent         Parent of the widget.
	*/
	QRXMLStruct(QWidget* parent=0);

protected:

	/**
	* Construct a tag in the list view.
	* @param t              Tag to construct.
	* @param parent         Parent that holds the tag.
	*/
	void ConstructTag(R::RXMLTag* t,QTreeWidgetItem* parent);

protected slots:

	/**
	* Slot to called when another tag is chosen in the list view.
	* @param item           Item selected.
	*/
	void slotSelectionTag(QTreeWidgetItem* item);

public:

	/**
	* Assign a new XML to the widget and redraw it.
	* @param xml             Structure to handle.
	*/
	void Set(RXMLStruct* xml);

	/**
	* Destruct.
	*/
	~QRXMLStruct(void);
};


} //------- End of namespace R ------------------------------------------------


//-----------------------------------------------------------------------------
#endif
