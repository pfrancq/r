/*

	QGAMonitor.h

	Widget to represents statistics about a running GA - Header.

	(c) 2000-2002 by P. Francq.

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
#ifndef QGAMonitorH
#define QGAMonitorH


//------------------------------------------------------------------------------
// include files for Qt
#include <qlist.h>
#include <qwidget.h>
#include <qlabel.h>
#include <qgroupbox.h>
#include <qsplitter.h>
#include <qpixmap.h>



//------------------------------------------------------------------------------
namespace R{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* The QGAMonitorStats provides the widget that will display statistics about
* the running GA.
* @author Pascal Francq
* @short GA statistics viewer for Qt.
*/
class QGAMonitorStats : public QGroupBox
{
	/**
	* Label to display the number of generations calculated.
	*/
	QLabel* Gen;

	/**
	* Label to display the age of the best chromosome.
	*/
	QLabel* Best;

public:

	/**
	* Constructor for the statistics viewer.
	* @param parent         Parent of the widget.
	* @param name           Name of the instance.
	*/
	QGAMonitorStats(QWidget* parent=0,const char* name=0);

	/**
	* Set the Gen label.
	* @param text          Text.
	*/
	void setGen(const char* text) {Gen->setText(text);}

	/**
	* Set the Best label.
	* @param text          Text.
	*/
	void setBest(const char* text) {Best->setText(text);}
};


//------------------------------------------------------------------------------
/**
* The QGAMonitorGraph provides the widget to display a graph with the evolution
* of the fitness function represent by double.
* @author Pascal Francq
* @short GA graph display for Qt.
*/
class QGAMonitorGraph : public QWidget
{
	Q_OBJECT

	/**
	* The maximal number of generations.
	*/
	unsigned int hScale;

	/**
	* The maximal value of the fitness.
	*/
	double vScale;

	/**
	* Number of values received actually.
	*/
	unsigned int receivedValues;

	/**
	* The list of values received.
	*/
	QList<double> values;

	/**
	* The last value added.
	*/
	double* LastValue;

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
	* @param name           Name of the instance.
	*/
	QGAMonitorGraph(QWidget* parent=0,const char* name=0);

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
	void setHScale(const unsigned int h);

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
	virtual void resizeEvent(QResizeEvent*);

	/**
	* Mouse button press event method.
	* @param e              MouseEvent info.
	*/
	virtual void mousePressEvent(QMouseEvent* e);

public:

	/**
	* Destructor of the graph display.
	*/
	virtual ~QGAMonitorGraph(void);
};


//------------------------------------------------------------------------------
/**
* The QGAMonitor class provides a widget to display information about the GA
* who is working, like the number of generation or a graph with the evolution
* of the fitnesses.
* @author Pascal Francq
* @short GA Information display.
*/
class QGAMonitor : public QSplitter
{
	Q_OBJECT

	/**
	* Pointer to the GA graph display.
	*/
	QGAMonitorGraph* Graph;

	/**
	* Pointer to the GA statistics display.
	*/
	QGAMonitorStats* Stats;

public:

	/**
	* Construct the information display.
	* @param parent         Parent of the widget.
	* @param name           Name of the instance.
	*/
	QGAMonitor(QWidget* parent=0,const char* name=0);

	/**
	* Set the maximal number of generations.
	* @param m              Maximum number of generation.
	*/
	void setMaxGen(const unsigned int m) { Graph->setHScale(m); }

	/**
	* Set the maximal value of the fitness function.
	* @param m              Maximum number of generation.
	*/
	void setMaxFitness(const double m) {Graph->setVScale(m);}

	/**
	* Clear all the information displayed.
	*/
	void Clear(void);

public slots:

	/**
	* Receive generation information.
	* @param gen            Actual Generation.
	* @param best           Age of the best chromosome.
	* @param value          Value of the best chromosmome.
	*/
	void slotSetGen(const unsigned int gen,const unsigned int best,const double value);
};


}  //------- End of namespace R ------------------------------------------------


//------------------------------------------------------------------------------
#endif
