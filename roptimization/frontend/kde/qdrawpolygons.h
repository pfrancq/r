/*

	R Project Library

	QDrawPolygons.h

	Qt Widget to draw polygons - Header.

	Copyright 2000-2009 by Pascal Francq (pascal@francq.info).
	Copyright 2000-2008 by the Université Libre de Bruxelles (ULB).

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
#ifndef QDrawPolygonsH
#define QDrawPolygonsH


//------------------------------------------------------------------------------
// include files for R Project
#include <rpoint.h>
#include <rgeoinfos.h>
#include <rconnections.h>
#include <rfreepolygons.h>
#include <rproblem2d.h>
#include <rgeoinfoconnection.h>


//------------------------------------------------------------------------------
// include files for Qt
#include <QtGui/QPixmap>
#include <QtGui/QBrush>
#include <QtGui/QMenu>


//------------------------------------------------------------------------------
namespace R{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* The QInfoBox class provides a popupmenu that display informartion about a
* specific object and geometric information.
* @author Pascal Francq
* @short Popup object information
*/
class QInfoBox : public QMenu
{
	/**
	* Is the popup empty?
	*/
	bool Empty;

	/**
	* A pixmap to hold the display.
	*/
	QPixmap* pixmap;

	/**
	* A pixmap to hold the display temporary (for example to show connections).
	*/
	QPixmap* tmppixmap;

	/**
	* Widget that must have the focus after.
	*/
	QWidget* afterFocus;

public:

	/**
	* Constructor of the popup.
	* @param parent         Parent of the widget.
	* @param info           Geometric information to represent.
	*/
	QInfoBox(QWidget* parent,RGeoInfo* info);

	/**
	* Constructor of the popup.
	* @param parent         Parent of the widget.
	* @param con            Connector to represent.
	*/
	QInfoBox(QWidget* parent,RGeoInfoConnector* con);

	/**
	* Constructor of the popup.
	* @param parent         Parent of the widget.
	* @param p              Pixmap to show on close.
	* @param tp             Pixmap to delete on close.
	*/
	QInfoBox(QWidget* parent,QPixmap* p,QPixmap* tp);

	/**
	* Add a connection info.
	* @param con            Connector to add.
	*/
	void AddConnectionInfo(RGeoInfoConnection* con);

protected:

	/**
	* Mouse release event method. When the mouse button is released, the popup
	* is closed.
	*/
	virtual void mouseReleaseEvent(QMouseEvent*);
};


//------------------------------------------------------------------------------
/**
* The QDrawPolygons class provides a widget to display some polygons given by
* a set of geometric information.
*	@author Pascal Francq
* @short Draw polygons widget.
*/
class QDrawPolygons : public QWidget
{
	/**
	* Pointer to the current problem if one.
	*/
	RProblem2D* Problem;

	/**
	* Limits.
	*/
	RPoint Limits;

	/**
	* Translation to do
	*/
	RPoint Translation;

	/**
	* X Factor.
	*/
	double FactorX;

	/**
	* Y Factor.
	*/
	double FactorY;

	/**
	* A pixmap to hold the display.
	*/
	QPixmap* pixmap;

	/**
	* A pixmap to hold the display temporary (for example to show connections).
	*/
	QPixmap* tmppixmap;

	/**
	* Has the display changed.
	*/
	bool Changed;

	/**
	* Last Information draw.
	*/
	RGeoInfo* Last;

	/**
	* Number of information.
	*/
	size_t NbInfos;

	/**
	* Pointer to all geometric information.
	*/
	RGeoInfos* Infos;

	/**
	* QPainter object used for external drawing.
	*/
	QPainter* Painter;

	/**
	* Pointer to free polygons to paint.
	*/
	RFreePolygons* FreePolygons;

	/**
	* Pointer to the connections.
	*/
	RConnections* Cons;

	/**
	* A black brush.
	*/
	QBrush brBlack;

	/**
	* A green brush.
	*/
	QBrush brGreen;

	/**
	* An yellow brush.
	*/
	QBrush brYellow;

	/**
	* A blue brush.
	*/
	QBrush brBlue;

	/**
	* A red brush.
	*/
	QBrush brRed;

	/**
	* Transform the coordinates for the external connectors.
	* @param x              X Coordinate.
	* @param y              Y Coordinate.
	*/
	void TransformExternCon(int& x,int& y);

	/**
	* Adapt the coordinates for the external connectors.
	* @param x              X Coordinate.
	* @param y              Y Coordinate.
	*/
	void AdaptExternCon(int& x,int& y);

	/**
	* Transform a Real X coordinate to a Screen one.
	* @param x              X Coordinate.
	*/
	int RealToScreenX(const tCoord x) {return(static_cast<int>(x*FactorX));}

	/**
	* Transform a Real Y coordinate to a Screen one.
	* @param y              Y Coordinate.
	*/
	int RealToScreenY(const tCoord y) {return(static_cast<int>((Limits.Y-y)*FactorY));}

public:

	/**
	* Construct the widget.
	* @param parent         Pointer to window holding this widget.
	*/
	QDrawPolygons(QWidget* parent=0);

	/**
	* Construct the widget.
	* @param prob           Pointer to the problem.
	* @param parent         Pointer to window holding this widget.
	*/
	QDrawPolygons(RProblem2D* prob,QWidget* parent=0);

	/**
	* The widget has changed and has to be repainted.
	*/
	void setChanged(void) {Changed=true;repaint();}

	/**
	* Set the connections.
	* @param c              Pointer to the connections.
	*/
	void setCons(RConnections* c) {Cons=c;}

	/**
	* Add Polygons to paint.
	* @param p              Pointer to the free polygons.
	*/
	void setPolys(RFreePolygons* p) {FreePolygons=p;}

	/**
	* Add a geometric information "on the fly".
	* @param info           Geometric Info to add.
	*/
	void addInfo(RGeoInfo* info);

	/**
	* Add a free polygon "on the fly".
	* @param poly           Free Polygon to add.
	*/
	void addFree(RFreePolygon* poly);

	/**
	* Set new limits.
	* @param limits         The limits.
	*/
	void setLimits(const RPoint& limits);

	/**
	* Set the Geometric Information.
	* param infos           Pointer to the Geometric Infos.
	*/
	inline void setInfos(RGeoInfos* infos) {Infos=infos;}

	/**
	* Set number of geometric information.
	* @param nbinfos        Number of geometric information.
	*/
	inline void setNbInfos(const size_t nbinfos) {NbInfos=nbinfos;}

	/**
	* Clear.
	*/
	inline void clear(void) {Last=0;FreePolygons=0;}

	/**
	* Get a pointer to a QPainter object.
	*/
	QPainter* begin(void);

	/**
	* Release a pointer to a QPainter object.
	*/
	void end(void);

protected:

	/**
	* Paint the connectors of a geometric information.
	* @param info           Geometric information to use.
	* @param Painter        Painter to paint in.
	*/
	void paintConnectors(RGeoInfo* info,QPainter* Painter);

	/**
	* Paint a connection.
	* @param con            Connection to paint.
	* @param Painter        Painter to paint in.
	* @param c              Must the connectors be drawed again.
	*/
	void paintConnection(RGeoInfoConnection* con,QPainter* Painter,bool c);

	/**
	* Paint event method.
	*/
	virtual void paintEvent(QPaintEvent*);

	/**
	* Resize event method.
	*/
	virtual void resizeEvent(QResizeEvent*);

	/**
	* Mouse button release event method.
	* @param e              MouseEvent info.
	*/
	virtual void mouseReleaseEvent(QMouseEvent* e);

	/**
	* Mouse button press event method.
	* @param e              MouseEvent info.
	*/
	virtual void mousePressEvent(QMouseEvent* e);

public:

	/**
	* Destruct the widget.
	*/
	~QDrawPolygons(void);
};


}  //------- End of namespace R ------------------------------------------------


//------------------------------------------------------------------------------
#endif
