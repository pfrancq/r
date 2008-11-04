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
// include files for R Project
#include <rqconsole.h>
#include <rqt.h>
using namespace R;
using namespace std;



//------------------------------------------------------------------------------
//
// class RQConsole
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
RQConsole::RQConsole(QWidget* parent,const QString& name)
	: QTextEdit(parent,name)
{
    setTextFormat( QTextEdit::RichText );
    setWordWrap( QTextEdit::FixedColumnWidth );
    setReadOnly(false);
    setTextFormat(Qt::PlainText);
    append(">");
}


//------------------------------------------------------------------------------
void RQConsole::keyPressEvent(QKeyEvent * e )
{
	int index;
	switch(e->key())
	{
		case Qt::Key_Up:
			break;
		case Qt::Key_Backspace:
		case Qt::Key_Left:
			getCursorPosition(&Para,&index);
			if(index>1)
				QTextEdit::keyPressEvent(e);
			break;
		case Qt::Key_Return:
		case Qt::Key_Enter:
		{
			getCursorPosition(&Para,&index);
			QString line(text(Para));
			QTextEdit::keyPressEvent(e);
			emit EnterCmd(line.right(line.length()-1));
			append(">");
			setCursorPosition(++Para,1);
			break;
		}
		default:
			QTextEdit::keyPressEvent(e);
	}
}


//------------------------------------------------------------------------------
void RQConsole::WriteStr(const RString& str)
{
//	int para,index;
//	getCursorPosition(&para,&index);
//	setCursorPosition(para-1,0);
	append(ToQString(str));
	Para++;
//	getCursorPosition(&para,&index);
//	setCursorPosition(para+2,0);
//	cout<<str<<endl;
}
