/*

	R Project Library

	RQConsole.cpp

	Widget that simulates a console - Implementation.

	Copyright 2008-2010 by Pascal Francq (pascal@francq.info).
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
	: QTextEdit(parent), Pos(-1)
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
		{
			if((Pos<100)&&(Pos<Cmds.size()-1))
			{
				Pos++;
				Cursor=textCursor();
				Cursor.select(QTextCursor::BlockUnderCursor);
				Cursor.removeSelectedText();
				Cursor.insertText("\n>"+Cmds.at(Pos));
				setTextCursor(Cursor);
			}
			break;
		}
		case Qt::Key_Down:
		{
			if((Cmds.size()>0)&&(Pos>=0))
			{
				Cursor=textCursor();
				Cursor.select(QTextCursor::BlockUnderCursor);
				Cursor.removeSelectedText();
				if(Pos)
				{
					Pos--;
					Cursor.insertText("\n>"+Cmds.at(Pos));
				}
				else
					Cursor.insertText("\n>");
				setTextCursor(Cursor);
			}
			break;
		}
		case Qt::Key_Home:
		{
			Cursor=textCursor();
			Cursor.movePosition(QTextCursor::StartOfBlock);
			Cursor.movePosition(QTextCursor::Right,QTextCursor::MoveAnchor,1);
			setTextCursor(Cursor);
			break;
		}
		case Qt::Key_Backspace:
		case Qt::Key_Left:
		{
			Cursor=textCursor();
			if(Cursor.position()>Cursor.block().position()+1)
				QTextEdit::keyPressEvent(e);
			break;
		}
		case Qt::Key_Return:
		case Qt::Key_Enter:
		{
			Cursor=textCursor();
			QString line(Cursor.block().text());
			QString Cmd(line.right(line.length()-1));
			if(Cmds.size()==100)
				Cmds.removeLast();
			if(Cmds.size()>0)
			{
				if(Cmds.at(0)!=Cmd)
					Cmds.prepend(Cmd);
			}
			else
				Cmds.prepend(Cmd);
			Cursor.movePosition(QTextCursor::EndOfBlock);
			setTextCursor(Cursor);
			QTextEdit::keyPressEvent(e);
			emit EnterCmd(Cmd);
			Pos=-1;
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
void QRConsole::WriteError(const RString& str)
{
	Cursor=textCursor();
	QTextCharFormat FormatRed;
	QTextCharFormat FormatBlack;
	FormatRed.setTextOutline(QColor(255,0,0));
	Cursor.insertText(ToQString(str),FormatRed);
	Cursor.insertText("\n",FormatBlack);
}


//------------------------------------------------------------------------------
QRConsole::~QRConsole(void)
{
}
