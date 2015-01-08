/*

	R Project Library

	QRGAMonitor.h

	Widget to represents statistics about a running GA - Header.

	Copyright 2000-2015 by Pascal Francq (pascal@francq.info).
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
#ifndef QRGAMonitorH
#define QRGAMonitorH


//------------------------------------------------------------------------------
// include files for Qt
#include <QtGui/QGroupBox>
#include <QtGui/QLabel>
#include <QtGui/QSplitter>
#include <QtGui/QPixmap>
#include <QtGui/QGraphicsScene>


//------------------------------------------------------------------------------
namespace R{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* The QRGAMonitorGraph provides the widget to display a graph with the evolution
* of the fitness function represent by double.
* @author Pascal Francq
* @short GA graph display for Qt.
*/
class QRGAMonitorGraph : public QWidget
{
	Q_OBJECT

	/**
	* The maximal number of generations.
	*/
	size_t hScale;

	/**
	* The maximal value of the fitness.
	*/
	double vScale;

	/**
	* Number of values received actually.
	*/
	size_t receivedValues;

	/**
	* The list of values received.
	*/
	QList<double> values;

	/**
	* The last value added.
	*/
	double LastValue;

	/**
	* The values representing the graph have changed.
	*/
	bool Changed;

	/**
	* The QPixmap representing the graph.
	*/
	QPixmap* pixmap;

	/**
	* X Factor.
	*/
	double FactorX;

	/**
	* Y Factor.
	*/
	double FactorY;

public:

	/**
	* Constructor of the graph display.
	* @param parent         Parent of the widget.
	*/
	QRGAMonitorGraph(QWidget* parent=0);

	/**
	* Returns a recommended size for the graph.
	*/
	virtual QSize sizeHint() const;

	/**
	* Clear the values and the graph.
	*/
	void clear();

	/**
	* Set the maximal value for the Y axis.
	* @param v              Maximal Y value.
	*/
	void setVScale(const double v);

	/**
	* Set the maximal value for the X axis.
	* @param h              Maximal X value.
	*/
	void setHScale(const size_t h);

	/**
	* Add a new value.
	* @param value          Value to add.
	*/
	void AddValue(const double value);

protected:

	/**
	* Paint event method.
	*/
	virtual void paintEvent(QPaintEvent*);

	/**
	* Resize event method.
	*/
	virtual void resizeEvent(QResizeEvent* event);

	/**
	* Mouse button press event method.
	* @param e              MouseEvent info.
	*/
	virtual void mousePressEvent(QMouseEvent* e);

public:

	/**
	* Destructor of the graph display.
	*/
	virtual ~QRGAMonitorGraph(void);
};


//------------------------------------------------------------------------------
/**
* The QRGAMonitor class provides a widget to display information about the GA
* who is working, like the number of generation or a graph with the evolution
* of the fitnesses.
* @author Pascal Francq
* @short GA Information display.
*/
class QRGAMonitor : public QWidget
{
	Q_OBJECT

	void* Ui;

	/**
	 * The scene.
	 */
	QGraphicsScene Scene;

	/**
	 * Maximal number of generations.
	 */
	size_t MaxGen;

	/**
	 * Maximal fitness.
	 */
	double MinFit;

	/**
	 * Maximal fitness.
	 */
	double MaxFit;

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
	* The list of values received.
	*/
	QList<double> Values;

public:

	/**
	* Construct the information display.
	* @param parent         Parent of the widget.
	*/
	QRGAMonitor(QWidget* parent=0);

	/**
	 * Set the parameters.
	 * @param maxgen         Maximum number of generations.
	 * @param minfit         Minimum value of the fitness.
	 * @param maxfit         Maximum value of the fitness.
	 */
	void setParams(size_t maxgen,double minfit,double maxfit);

	/**
	* Receive generation information.
	* @param gen            Actual Generation.
	* @param best           Age of the best chromosome.
	* @param value          Value of the best chromosome.
	*/
	void setGenInfo(const size_t gen,const size_t best,const double value);

	/**
	* Clear all the information displayed.
	*/
	void clear(void);

protected:

	/**
	 * @return the X-coordinate in the scene.
	 * @param x              X-coordinate in the problem.
	 */
	inline double x(size_t x)
	{
		return(static_cast<double>(x)*XScale);
	}

	/**
	 * @return the Y-coordinate in the scene.
	 * @param y              Y-coordinate in the problem.
	 */
	inline double y(double y)
	{
		return((MaxFit-MinFit-y)*YScale);
	}

	/**
	* The widget has changed and has to be repainted.
	*/
	void repaint(void);

	/**
	* Resize event method.
	* @param event           Event.
	*/
	virtual void resizeEvent(QResizeEvent* event);

public:

	/**
	 * Destructor.
	 */
	virtual ~QRGAMonitor(void);
};


}  //------- End of namespace R ------------------------------------------------


//------------------------------------------------------------------------------
#endif
