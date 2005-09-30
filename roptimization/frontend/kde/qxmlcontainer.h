/*

	QXMLContainer.h

	Qt XML Debugger - Header.

	(c) 2000-2001 by P. Francq.

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



//------------------------------------------------------------------------------
#ifndef QXMLContainerH
#define QXMLContainerH


//------------------------------------------------------------------------------
// include files for R Project
#include <rdebug.h>


//------------------------------------------------------------------------------
// include files for Qt
#include <qlistview.h>


//------------------------------------------------------------------------------
namespace R{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* The QXMLContainer class provides a widget to display debug information
* generated by the GA has XML output.
* @author Pascal Francq.
* @short XML Debug Qt Widget.
*/
class QXMLContainer : public QListView, public RDebug
{
	Q_OBJECT

	/**
	* The array is used to manage the deepest of the XML structure.
	*/
	QListViewItem *Items[50];

public:

	/**
	* Construct the XML debug widget.
	* @param parent         Parent of the widget.
	* @param name           Name of the instance.
	*/
	QXMLContainer(QWidget* parent=0,RString name=RString::Null);

	/**
	* Clear the XML.
	*/
	virtual void clear(void);

protected:

	/**
	* Begin a Tag.
	* @param tag            Name of the tag.
	* @param options        Options of the tag.
	*/
	virtual void WriteBeginTag(RString tag,RString options=RString::Null);

	/**
	* Write Text associate with current tag.
	* @param text           Text to write.
	*/
	virtual void WriteText(RString text);

	/**
	* End a Tag.
	* @param tag            Name of the tag.
	*/
	virtual void WriteEndTag(RString tag);

signals:

	/**
	* Emit a signal when this widget is closed.
	*/
	void signalXMLClose(void);

public:

	/**
	* Destruct the XML debug widget.
	*/
	~QXMLContainer(void);
};


}  //------- End of namespace R ------------------------------------------------


//------------------------------------------------------------------------------
#endif
