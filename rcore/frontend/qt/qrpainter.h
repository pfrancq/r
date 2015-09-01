/*

	R Project Library

	QRPainter.h

	Qt Painter - Header.

	Copyright 2009-2015 by Pascal Francq (pascal@francq.info).

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
#ifndef QRPainterH
#define QRPainterH


//------------------------------------------------------------------------------
// include files for Qt/KDE
#include <QtGui/QPainter>


//------------------------------------------------------------------------------
namespace R{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
 * The QRPainter provides a class to paint for Qt. In fact, it proposes a set
 * convenient methods to mix R and Qt.
 * @short Qt Painter
 */
class QRPainter : public QPainter
{
public:

	/**
	 * Constructor.
	 */
	QRPainter(void);

	/**
	 * Constructor
	 * @param device         Device where the paint must be done.
	 */
	QRPainter(QPaintDevice* device);

	/**
	 * Draw the "Manhattan distance" between two points. Basically, the middle
	 * point is computed and acts as intermediate point.
	 * @param x1             First X coordinate.
	 * @param y1             First Y coordinate.
	 * @param x2             Second X coordinate.
	 * @param y2             Second Y coordinate.
	 */
	void drawManhattan(int x1,int y1,int x2,int y2);

	/**
	 * Draw the "Manhattan distance" between two points. Basically, the middle
	 * point is computed and acts as intermediate point.
	 * @param pt1            First point.
	 * @param pt2            Second point.
	 */
	void drawManhattan(const QPoint& pt1,const QPoint& pt2);
};


}  //-------- End of namespace R -----------------------------------------------


//------------------------------------------------------------------------------
#endif
