/*

	R Project Library

	QRConsole.h

	Widget that simulates a console - Header.

	Copyright 2008-2012 by Pascal Francq (pascal@francq.info).
	Copyright 2008 by the Université Libre de Bruxelles (ULB).

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
#ifndef QRConsole_H
#define QRConsole_H


//------------------------------------------------------------------------------
// include files for R Project
#include <rprgoutput.h>


//------------------------------------------------------------------------------
// include files for Qt
#include <QtGui/QTextEdit>
#include <QtCore/QList>


//------------------------------------------------------------------------------
namespace R{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
 * The RQConsole class provides a Qt widget that simulates a console. It
 * maintains a list of the last 100 different commands entered. The up and down
 * keys are used to navigate through the history.
 *
 * In particular, this widget can be used in interaction with a RInterpreter
 * instance.
 *
 * @author Pascal Francq.
 * @short Console Widget.
 */
class QRConsole : public QTextEdit, public RPrgOutput
{
	Q_OBJECT

	/**
	 * Current cursor.
	 */
	QTextCursor Cursor;

	/**
	 * Previous commands.
	 */
	QList<QString> Cmds;

	/**
	 * Position in the list.
	 */
	int Pos;

public:

	/**
	 * Construct the console.
	 * @param parent         Parent widget.
	 */
	QRConsole(QWidget* parent);

	/**
	 * Catch the key event to simulate a real console.
	 * @param e              Event.
	 */
	virtual void keyPressEvent(QKeyEvent* e);

	/**
	 * Write some messages.
	 * @param str            Message to print.
	 */
	virtual void WriteStr(const RString& str);

	/**
	 * Write some error messages. The message is displayed in red.
	 * @param str            Message to print.
	 */
	virtual void WriteError(const RString& str);

signals:

	/**
	 * Signal emits when a command was entered.
	 */
	void EnterCmd(QString cmd);

public:

	~QRConsole(void);
};


}  //-------- End of namespace R -----------------------------------------------


//------------------------------------------------------------------------------
#endif
