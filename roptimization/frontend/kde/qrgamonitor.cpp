/*

	R Project Library

	QRGAMonitor.cpp

	Widget to represents statistics about a running GA - Implementation.

	Copyright 2000-2012 by Pascal Francq (pascal@francq.info).
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
// include files for ANSI C/C++
#include <stdio.h>
#include <iostream>


//------------------------------------------------------------------------------
// include files for Qt
#include <QtGui/QPainter>
#include <QtGui/QMenu>
#include <QtGui/QMouseEvent>


//------------------------------------------------------------------------------
// include files for R Project
#include <qrgamonitor.h>
#include <ui_qrgamonitor.h>
using namespace R;



//------------------------------------------------------------------------------
//
// QInfoBox
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
/**
* The QInfoBox class provides a popupmenu that display information about a
* specific object and geometric information.
* @author Pascal Francq
* @short Popup object information
*/
class QInfoBox : public QMenu
{
	/**
	* Widget that must have the focus after.
	*/
	QWidget* afterFocus;

public:

	/**
	* Constructor of the popup.
	* @param parent        Parent of the widget.
	* @param gen           Generation.
	*/
	QInfoBox(QWidget* parent,size_t gen);

	/**
	* Constructor of the popup.
	* @param parent        Parent of the widget.
	* @param gen           Generation.
	* @param fit           Fitness.
	*/
	QInfoBox(QWidget* parent,size_t gen,double fit);

protected:

	/**
	* Mouse release event method. When the mouse bouton is released, the popup
	* is closed.
	*/
	virtual void mouseReleaseEvent(QMouseEvent*);
};


//------------------------------------------------------------------------------
QInfoBox::QInfoBox(QWidget* parent,size_t gen)
	: QMenu(0)
{
	addAction("Generation "+QString::number(gen)+" not executed");
	afterFocus=parent;
	afterFocus->parentWidget()->setFocus();
}


//------------------------------------------------------------------------------
QInfoBox::QInfoBox(QWidget* parent,size_t gen,double fit)
	: QMenu(0)
{
	addAction("Generation "+QString::number(gen));
	addAction("Fitness="+QString::number(fit));
	afterFocus=parent;
	afterFocus->parentWidget()->setFocus();
}


//------------------------------------------------------------------------------
void QInfoBox::mouseReleaseEvent(QMouseEvent*)
{
	afterFocus->parentWidget()->setFocus();
	delete(this);
}



//------------------------------------------------------------------------------
//
// QRGAMonitorGraph
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
QRGAMonitorGraph::QRGAMonitorGraph(QWidget* parent)
	: QWidget(parent),values(),LastValue(0),pixmap(0)
{
	QRect r;

	vScale=1.0;
	hScale=1;
	receivedValues=0;
//	setBackgroundRole(QPalette(Qt::black));
	setMinimumSize(200,100);
	r=rect();
	FactorX=(static_cast<double>(r.width()))/(static_cast<double>(hScale));
	FactorY=(static_cast<double>(r.height()))/(static_cast<double>(vScale));
	Changed=false;
}

//------------------------------------------------------------------------------
QSize QRGAMonitorGraph::sizeHint() const
{
	return(minimumSize());
}


//------------------------------------------------------------------------------
void QRGAMonitorGraph::clear()
{
//	erase();
	values.clear();
	receivedValues=0;
	LastValue=0;
	Changed=true;
	repaint();
}


//------------------------------------------------------------------------------
void QRGAMonitorGraph::setVScale(const double v)
{
	QRect r=rect();
	double i=r.height();
	double tmp;

	vScale=v;
	Changed=true;
	tmp=i/vScale;
	FactorY=tmp;
	repaint();
}


//------------------------------------------------------------------------------
void QRGAMonitorGraph::setHScale(const size_t h)
{
	QRect r=rect();
	hScale=h;
	Changed=true;
	FactorX=(static_cast<double>(r.width()))/(static_cast<double>(hScale));
	repaint();
}


//------------------------------------------------------------------------------
void QRGAMonitorGraph::AddValue(const double value)
{
	int NewX,NewY;
	QRect r=rect();
	values.append(value);
	QPainter *painter=new QPainter(pixmap);
	painter->setPen(Qt::red);
	NewX=static_cast<int>(static_cast<double>(receivedValues)*FactorX);
	NewY=r.height()-static_cast<int>(value*FactorY);
	if(LastValue)
		painter->drawLine(static_cast<int>((static_cast<double>(receivedValues)-1)*FactorX),
				          r.height()-static_cast<int>(LastValue*FactorY),
		                  NewX,NewY);
	else
		painter->drawPoint(NewX,NewY);
	delete painter;
	receivedValues++;
	LastValue=value;
	render(pixmap);
}


//------------------------------------------------------------------------------
void QRGAMonitorGraph::paintEvent(QPaintEvent*)
{
	double tmp;
	QRect r;
	double Last;

	r=rect();
	if(!pixmap)
		pixmap=new QPixmap(r.size());
	if(Changed)
	{
		pixmap->fill(this,r.topLeft());
		Changed=false;
		QPainter *painter=new QPainter(pixmap);
		painter->setPen(Qt::red);
		tmp=values.first();
		if(tmp)
		{
			painter->drawPoint(0,static_cast<int>((vScale-tmp)*FactorY));
			Last=tmp;
			for(int i=1;tmp;i++)
			{
				tmp=values.at(i);
				painter->drawLine(static_cast<int>((i-1)*FactorX),static_cast<int>((vScale-Last)*FactorY),
				                  static_cast<int>(i*FactorX),static_cast<int>((vScale-tmp)*FactorY));
				Last=tmp;
			}
		}
		delete painter;
	}
	render(pixmap);
}


//------------------------------------------------------------------------------
void QRGAMonitorGraph::resizeEvent(QResizeEvent*)
{
	QRect r=rect();
	double i=r.height();
	double tmp;

	if(pixmap)
		pixmap->scaled(rect().size());
	Changed=true;
	FactorX=(static_cast<double>(r.width()))/(static_cast<double>(hScale));
	tmp=i/vScale;
	FactorY=tmp;
	repaint();
}


//------------------------------------------------------------------------------
void QRGAMonitorGraph::mousePressEvent(QMouseEvent* e)
{
	QInfoBox* InfoBox;

	if(e->button()==Qt::RightButton)
	{
		size_t gen=static_cast<size_t>(e->x()/FactorX);
		if(gen<receivedValues)
			InfoBox=new QInfoBox(this,gen,values.at(gen));
		else
			InfoBox = new QInfoBox(this,gen);
		InfoBox->popup(e->globalPos());
	}
	else
		QWidget::mousePressEvent(e);
}


//------------------------------------------------------------------------------
QRGAMonitorGraph::~QRGAMonitorGraph(void)
{
	values.clear();
	if(pixmap) delete pixmap;
}



//------------------------------------------------------------------------------
//
// QRGAMonitor
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
QRGAMonitor::QRGAMonitor(QWidget* parent)
	: QWidget(parent), Ui(new Ui_QRGAMonitor())
{
	static_cast<Ui_QRGAMonitor*>(Ui)->setupUi(this);
	static_cast<Ui_QRGAMonitor*>(Ui)->Graph->setScene(&Scene);
	Scene.setBackgroundBrush(Qt::black);
	clear();
}


//------------------------------------------------------------------------------
void QRGAMonitor::setParams(size_t maxgen,double minfit,double maxfit)
{
	MaxGen=maxgen;
	MinFit=minfit;
	MaxFit=maxfit;
}


//------------------------------------------------------------------------------
void QRGAMonitor::setGenInfo(const size_t gen,const size_t best,const double value)
{
	static_cast<Ui_QRGAMonitor*>(Ui)->Gen->setText("Generation: "+QString::number(gen));
	static_cast<Ui_QRGAMonitor*>(Ui)->Best->setText("Best age: "+QString::number(best));
	static_cast<Ui_QRGAMonitor*>(Ui)->BestFit->setText("Best fitness: "+QString::number(value));
	Values.insert(gen,value);
	if(gen)
		Scene.addLine(x(gen-1),y(Values.at(gen-1)),x(gen),y(Values.at(gen)),QPen(Qt::red));
}


//------------------------------------------------------------------------------
void QRGAMonitor::clear(void)
{
	static_cast<Ui_QRGAMonitor*>(Ui)->Gen->setText("No generation");
	static_cast<Ui_QRGAMonitor*>(Ui)->Best->setText("No best solution computed");
	Scene.clear();
}


//------------------------------------------------------------------------------
void QRGAMonitor::repaint(void)
{
	QGraphicsView* Graph(static_cast<Ui_QRGAMonitor*>(Ui)->Graph);
	XScale=static_cast<double>(Graph->width())/static_cast<double>(MaxGen);
	YScale=static_cast<double>(Graph->height())/(MaxFit-MinFit);
	Scene.clear();
	for(int i=1;i<Values.size();++i)
		Scene.addLine(x(i-1),y(Values.at(i-1)),x(i),y(Values.at(i)),QPen(Qt::red));
}


//------------------------------------------------------------------------------
void QRGAMonitor::resizeEvent(QResizeEvent* event)
{
	QWidget::resizeEvent(event);
	repaint();
}


//------------------------------------------------------------------------------
QRGAMonitor::~QRGAMonitor(void)
{
	delete static_cast<Ui_QRGAMonitor*>(Ui);
}
