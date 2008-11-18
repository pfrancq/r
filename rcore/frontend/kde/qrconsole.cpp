/*

	R Project Library

	RQConsole.cpp

	Widget that simulates a console - Implementation.

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
// include files for Qt
#include <QtGui/QKeyEvent>
#include <QtGui/QTextBlock>


//------------------------------------------------------------------------------
// include files for R Project
#include <qrconsole.h>
#include <rqt.h>
using namespace R;
using namespace std;



//------------------------------------------------------------------------------
//
// class QRConsole
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
QRConsole::QRConsole(QWidget* parent)
	: QTextEdit(parent)
{
    Cursor=textCursor();
    Cursor.insertText(">");
}


//------------------------------------------------------------------------------
void QRConsole::keyPressEvent(QKeyEvent * e )
{
	switch(e->key())
	{
		case Qt::Key_Up:
			break;
		case Qt::Key_Backspace:
		case Qt::Key_Left:
		{
			Cursor=textCursor();
			if(Cursor.position()>1)
				QTextEdit::keyPressEvent(e);
			break;
		}
		case Qt::Key_Return:
		case Qt::Key_Enter:
		{
			Cursor=textCursor();
			QString line(Cursor.block().text());
			QTextEdit::keyPressEvent(e);
			emit EnterCmd(line.right(line.length()-1));
			Cursor=textCursor();
			Cursor.insertText(">");
			setTextCursor(Cursor);
			break;
		}
		default:
			QTextEdit::keyPressEvent(e);
	}
}


//------------------------------------------------------------------------------
void QRConsole::WriteStr(const RString& str)
{
	Cursor=textCursor();
	Cursor.insertText(ToQString(str+"\n"));
}


//------------------------------------------------------------------------------
QRConsole::~QRConsole(void)
{
}
