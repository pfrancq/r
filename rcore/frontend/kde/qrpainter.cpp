/*

	R Project Library

	QRPainter.cpp

	Qt Painter - Implementation.

	Copyright 2009-2010 by Pascal Francq (pascal@francq.info).

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
// include files for ANSI C/C++
#include <iostream>
using namespace std;


//------------------------------------------------------------------------------
// include files for R Project
#include <qrpainter.h>
using namespace R;



//------------------------------------------------------------------------------
//
// class QRPainter
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
QRPainter::QRPainter(void)
	: QPainter()
{
}


//------------------------------------------------------------------------------
QRPainter::QRPainter(QPaintDevice* device)
	: QPainter(device)
{
}


//------------------------------------------------------------------------------
void QRPainter::drawManhattan(int x1,int y1,int x2,int y2)
{
	int ix=x1+((x2-x1)/2);
	drawLine(x1,y1,ix,y1);
	drawLine(ix,y1,ix,y2);
	drawLine(ix,y2,x2,y2);
}


//------------------------------------------------------------------------------
void QRPainter::drawManhattan(const QPoint& pt1,const QPoint& pt2)
{
	drawManhattan(pt1.x(),pt1.y(),pt2.x(),pt2.y());
}
