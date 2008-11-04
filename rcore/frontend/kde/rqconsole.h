/*

	R Project Library

	RQConsole.h

	Widget that simulates a console - Header.

	Copyright 2008 by the Université libre de Bruxelles.

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
	Boston, MA  02111-1307  USA

*/



//------------------------------------------------------------------------------
#ifndef RQConsole_H
#define RQConsole_H


//------------------------------------------------------------------------------
// include files for R Project
#include <rprgoutput.h>


//------------------------------------------------------------------------------
// include files for Qt
#include <qtextedit.h>


//------------------------------------------------------------------------------
namespace R{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
 * The RQConsole class provides a Qt widget that simulates a console. In
 * particular, this widget can be used in interaction with a RInterpreter
 * instance.
 * @author Pascal Francq.
 * @short Console Widget.
 */
class RQConsole : public QTextEdit, public RPrgOutput
{
	Q_OBJECT

	/**
	 * Current paragraph treated.
	 */
	int Para;

public:

	/**
	 * Construct the console.
	 * @param parent         Parent widget.
	 * @param name           Name of the widget
	 */
	RQConsole(QWidget* parent,const QString& name);

	/**
	 * Catch the key event to simulate a real console.
	 * @param e              Event.
	 */
	virtual void keyPressEvent(QKeyEvent* e);

	/**
	 * Write some messages, for example if a RInterpreter generates an
	 * exception.
	 * @param str            Message to print.
	 */
	virtual void WriteStr(const RString& str);

signals:

	/**
	 * Signal emits when a command was entered.
	 */
	void EnterCmd(QString cmd);
};


}  //-------- End of namespace R -----------------------------------------------


//------------------------------------------------------------------------------
#endif
