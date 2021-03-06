/*

	R Project Library

	QRDrawGeoInfos.h

	Qt Widget to draw a set of geometric information - Header.

	Copyright 2000-2015 by Pascal Francq (pascal@francq.info).

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
#ifndef QRDrawGeoInfosH
#define QRDrawGeoInfosH


//------------------------------------------------------------------------------
// include files for R Project
#include <rpoint.h>
#include <rlayout.h>


//------------------------------------------------------------------------------
// include files for Qt
#include <QtCore/QVector>
#include <QtGui/QWidget>
#include <QtGui/QGraphicsScene>
#include <QtGui/QGraphicsPolygonItem>


//-----------------------------------------------------------------------------
namespace R{
//-----------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
 * The QRDrawGeoInfos provides a widget that paints a layout.
  * @short Layout Widget.
 */
class QRDrawGeoInfos : public QWidget
{
	class MyItem;

	Q_OBJECT

	void* Ui;

	/**
	* Pointer to free polygons to paint.
	*/
	RFreePolygons* FreePolygons;

	/**
	* Layout.
	*/
	RLayout* Layout;

	/**
	 * The scene.
	 */
	QGraphicsScene Scene;

	/**
	* A blue brush.
	*/
	QBrush BlueBrush;

	/**
	* A red brush.
	*/
	QBrush RedBrush;

	/**
	* A black brush.
	*/
	QBrush BlackBrush;

	/**
	* An yellow brush.
	*/
	QBrush YellowBrush;

	/**
	* A green brush.
	*/
	QBrush GreenBrush;

	/**
	 * A cyan brush.
	 */
	QBrush CyanBrush;

	/**
	 * Elements added to paint the asked connections.
	 */
	QVector<QGraphicsLineItem*> Nets;

	/**
	 * Last item inserted.
	 */
	MyItem* LastItem;

	/**
	 * Ratio to adapt the widths of the objects of the scene to the actual
	 * width of the widget.
	 */
	double XScale;

	/**
	 * Ratio to adapt the heights of the objects of the scene to the actual
	 * height of the widget.
	 */
	double YScale;

	/**
	 * Draw in a step mode (last object is drawn in a different colour).
	 */
	bool StepMode;

	/**
	 * Should the objects identifiers be showed?
	 */
	bool ShowId;

public:

	/**
	* Constructor.
	* @param parent          Parent of the widget.
	* @param showid          Should the objects identifiers be showed?
	*/
	QRDrawGeoInfos(QWidget* parent=0,bool showid=true);

	/**
	* Set the layout.
	* @param layout          Layout.
	*/
	void setLayout(RLayout* layout);

	/**
	* Set the free polygons.
	* @param polys           Free polygons.
	*/
	void setPolys(RFreePolygons* polys);

	/**
	 * Set the step mode.
	 * @param step           Step mode.
	 */
	void setStepMode(bool step);

	/**
	* The widget has changed and has to be repainted.
	*/
	void repaint(void);

	/**
	* Add a geometric information.
	* @param info           Geometric Info to add.
	* @param fly            Is it added on the fly ?
	*/
	void paintInfo(const RGeoInfo* info,bool fly);

	/**
	* Add a free polygon.
	* @param poly           Free Polygon to add.
	* @param fly            Is it added on the fly ?
	*/
	void paintFree(const RFreePolygon* poly,bool fly);

protected:

	/**
	 * @return the X-coordinate in the scene.
	 * @param x              X-coordinate in the problem.
	 */
	inline double x(tCoord x)
	{
		return(static_cast<double>((x*XScale)+5.0));
	}

	/**
	 * @return the Y-coordinate in the scene.
	 * @param y              Y-coordinate in the problem.
	 */
	inline double y(tCoord y)
	{
		return((static_cast<double>(Layout->GetProblem()->GetLimits().GetHeight()-y)*YScale)-5.0);
	}

	/**
	 * Paint a given connection.
	 * @param connect        Connection to paint.
	 */
	void paintConnection(const RGeoInfoConnection* connect);

	/**
	 * Remove the connections from the board.
	 */
	void removeConnections(void);

	/**
	* Paint the connectors of a geometric information.
	* @param info           Geometric information to use.
	*/
	void paintConnectors(const RGeoInfo* info);

	/**
	 * Repaint the widget with the new sizes.
	 * @param event          The paint event.
	 */
	virtual void resizeEvent(QResizeEvent* event);

public:

	/**
	 * Destructor.
	 */
	virtual ~QRDrawGeoInfos(void);

	friend class MyItem;
};


} //------- End of namespace R ------------------------------------------------


//------------------------------------------------------------------------------
#endif
