/*
	R Project Library

	QRDebug.h

	Widget Debugger - Header.

	Copyright 2000-2010 by Pascal Francq (pascal@francq.info).
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
	Boston, MA  02111-1307  USA
*/



//------------------------------------------------------------------------------
#ifndef QRDebugH
#define QRDebugH


//------------------------------------------------------------------------------
// include files for R Project
#include <rdebug.h>


//------------------------------------------------------------------------------
// include files for Qt/KDE
#include <QtGui/QWidget>


//------------------------------------------------------------------------------
namespace R{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* The QRDebug class provides a widget to display debug information generated by
* an instance of a genetic algorithm (has XML output).
* @author Pascal Francq.
* @short Widget Debugger.
*/
class QRDebug : public QWidget, public RDebug
{
	Q_OBJECT

	class Item;

	void* Ui;

	/**
	* The array is used to manage the deepest of the XML structure.
	*/
	RContainer<Item,false,false> Items;

public:

	/**
	* Construct the XML debug widget.
	* @param parent         Parent of the widget.
	*/
	QRDebug(QWidget* parent=0);

	/**
	* Clear the XML.
	*/
	virtual void clear(void);

protected:

	/**
	* Begin a Tag.
	* @param tag             Name of the tag.
	* @param attrs           Attributes.
	*/
	virtual void WriteBeginTag(const RString& tag,const RString& attrs);

	/**
	* Write Text associate with current tag.
	* @param text            Text to write.
	*/
	virtual void WriteText(const RString& text);

	/**
	* End a Tag.
	* @param tag            Name of the tag.
	*/
	virtual void WriteEndTag(const RString& tag);

signals:

	/**
	* Emit a signal when this widget is closed.
	*/
	void signalXMLClose(void);

public:

	/**
	* Destruct the XML debug widget.
	*/
	~QRDebug(void);
};


}  //------- End of namespace R ------------------------------------------------


//------------------------------------------------------------------------------
#endif
