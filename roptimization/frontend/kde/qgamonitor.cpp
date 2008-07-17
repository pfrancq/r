/*

	QGAMonitor.cpp

	Widget to represents statistics about a running GA - Implementation.

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
// include files for ANSI C/C++
#include <stdio.h>
#include <iostream>


//------------------------------------------------------------------------------
// include files for Qt
#include <qpainter.h>
#include <qpopupmenu.h>


//------------------------------------------------------------------------------
// include files for Qt Widgets
#include <qgamonitor.h>
using namespace R;



//------------------------------------------------------------------------------
//
// QInfoBox
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
/**
* The QInfoBox class provides a popupmenu that display informartion about a
* specific object and geometric information.
* @author Pascal Francq
* @short Popup object information
*/
class QInfoBox : public QPopupMenu
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
	QInfoBox(QWidget* parent,int gen);

	/**
	* Constructor of the popup.
	* @param parent        Parent of the widget.
	* @param gen           Generation.
	* @param fit           Fitness.
	*/
	QInfoBox(QWidget* parent,int gen,double fit);

protected:

	/**
	* Mouse release event method. When the mouse bouton is released, the popup
	* is closed.
	*/
	virtual void mouseReleaseEvent(QMouseEvent*);
};


//------------------------------------------------------------------------------
QInfoBox::QInfoBox(QWidget* parent,int gen)
	: QPopupMenu(0,"Info Box")
{
	char Tmp[50];

	sprintf(Tmp,"Generation %u not executed",gen);
	insertItem(Tmp);
	afterFocus=parent;
	afterFocus->parentWidget()->setFocus();
}


//------------------------------------------------------------------------------
QInfoBox::QInfoBox(QWidget* parent,int gen,double fit)
	: QPopupMenu(0,"Info Box")
{
	char Tmp[50];

	sprintf(Tmp,"Generation %u",gen);
	insertItem(Tmp);
	sprintf(Tmp,"Fitness=%f",fit);
	insertItem(Tmp);
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
// QGAMonitorGroupBox
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
QGAMonitorStats::QGAMonitorStats(QWidget* parent,const char* name)
	: QGroupBox(parent,name)
{
	QLabel *l;

	l=new QLabel("Generations:",this);
	l->setGeometry(10,15,80,20);
	Gen = new QLabel("0",this);
	Gen->setGeometry(90,15,100,20);
	l=new QLabel("Best Age:",this);
	l->setGeometry(10,35,80,20);
	Best = new QLabel("0",this);
	Best->setGeometry(90,35,100,20);
	setTitle("Statistics");
}



//------------------------------------------------------------------------------
//
// QGAMonitorGraph
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
QGAMonitorGraph::QGAMonitorGraph(QWidget* parent,const char* name)
	: QWidget(parent,name),values(),LastValue(0),pixmap(0)
{
	QRect r;

	vScale=1.0;
	hScale=1;
	receivedValues=0;
	values.setAutoDelete(true);
	setBackgroundColor(black);
	setMinimumSize(200,100);
	r=rect();
	FactorX=(static_cast<double>(r.width()))/(static_cast<double>(hScale));
	FactorY=(static_cast<double>(r.height()))/(static_cast<double>(vScale));
	Changed=false;
}

//------------------------------------------------------------------------------
QSize QGAMonitorGraph::sizeHint() const
{
	return(minimumSize());
}


//------------------------------------------------------------------------------
void QGAMonitorGraph::clear()
{
	erase();
	values.clear();
	receivedValues=0;
	LastValue=0;
	Changed=true;
	repaint();
}


//------------------------------------------------------------------------------
void QGAMonitorGraph::setVScale(const double v)
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
void QGAMonitorGraph::setHScale(const size_t h)
{
	QRect r=rect();
	hScale=h;
	Changed=true;
	FactorX=(static_cast<double>(r.width()))/(static_cast<double>(hScale));
	repaint();
}


//------------------------------------------------------------------------------
void QGAMonitorGraph::AddValue(const double value)
{
	int NewX,NewY;
	QRect r=rect();
	double *tmp=new double(value);
	CHECK_PTR(tmp);
	values.append(tmp);
	QPainter *painter=new QPainter(pixmap);
	CHECK_PTR(painter);
	painter->setPen(red);
	NewX=static_cast<int>(receivedValues*FactorX);
	NewY=r.height()-static_cast<int>(value*FactorY);
	if(LastValue)
		painter->drawLine(static_cast<int>((receivedValues-1)*FactorX),r.height()-static_cast<int>((*LastValue)*FactorY),
		                  NewX,NewY);
	else
		painter->drawPoint(NewX,NewY);
	delete painter;
	receivedValues++;
	LastValue=tmp;
	bitBlt(this,r.topLeft(),pixmap);
}


//------------------------------------------------------------------------------
void QGAMonitorGraph::paintEvent(QPaintEvent*)
{
	double *tmp;
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
		CHECK_PTR(painter);
		painter->setPen(red);
		tmp=values.first();
		if(tmp)
		{
			painter->drawPoint(0,static_cast<int>((vScale-(*tmp))*FactorY));
			Last=(*tmp);
			tmp=values.next();
			for(int i=1;tmp;tmp=values.next(),i++)
			{
				painter->drawLine(static_cast<int>((i-1)*FactorX),static_cast<int>((vScale-Last)*FactorY),
				                  static_cast<int>(i*FactorX),static_cast<int>((vScale-(*tmp))*FactorY));
				Last=(*tmp);
			}
		}
		delete painter;
	}
	bitBlt(this,r.topLeft(),pixmap);
}


//------------------------------------------------------------------------------
void QGAMonitorGraph::resizeEvent(QResizeEvent*)
{
	QRect r=rect();
	double i=r.height();
	double tmp;

	if(pixmap)
		pixmap->resize(rect().size());
	Changed=true;
	FactorX=(static_cast<double>(r.width()))/(static_cast<double>(hScale));
	tmp=i/vScale;
	FactorY=tmp;
	repaint();
}


//------------------------------------------------------------------------------
void QGAMonitorGraph::mousePressEvent(QMouseEvent* e)
{
	QInfoBox* InfoBox;

	if(e->button()==RightButton)
	{
		size_t gen=static_cast<size_t>(e->x()/FactorX);
		if(gen<receivedValues)
			InfoBox=new QInfoBox(this,gen,*values.at(gen));
		else
			InfoBox = new QInfoBox(this,gen);
		InfoBox->popup(e->globalPos());
	}
	else
		QWidget::mousePressEvent(e);
}


//------------------------------------------------------------------------------
QGAMonitorGraph::~QGAMonitorGraph(void)
{
	values.clear();
	if(pixmap) delete pixmap;
}



//------------------------------------------------------------------------------
//
// QGAMonitor
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
QGAMonitor::QGAMonitor(QWidget* parent,const char* name)
	: QSplitter(parent,name)
{
	setBackgroundMode(PaletteBase);
	Stats = new QGAMonitorStats(this);
	Graph = new QGAMonitorGraph(this);
	Graph->setMinimumSize(Graph->sizeHint());
}


//------------------------------------------------------------------------------
void QGAMonitor::slotSetGen(const size_t gen,const size_t best,const double value)
{
	static char Txt[20];

	sprintf(Txt,"%u",gen);
	Stats->setGen(Txt);
	sprintf(Txt,"%u",best);
	Stats->setBest(Txt);
	Graph->AddValue(value);
	repaint();
}


//------------------------------------------------------------------------------
void QGAMonitor::Clear(void)
{
	Stats->setGen("0");
	Stats->setBest("0");
	Graph->clear();
}
