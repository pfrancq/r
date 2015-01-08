/*

	R Project Library

	QRDrawGeoInfos.h

	Qt Widget to draw a set of geometric information - Implementation.

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
// include files for R project
#include <rproblem2d.h>
#include <qrdrawgeoinfos.h>
#include <ui_qrdrawgeoinfos.h>
#include <robj2dconfig.h>
#include <rqt.h>
using namespace R;
using namespace std;


//------------------------------------------------------------------------------
// include files for Qt/KDe
#include <QtGui/QGraphicsSceneMouseEvent>
#include <QtGui/QMouseEvent>



//------------------------------------------------------------------------------
//
// class MyItem
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
class QRDrawGeoInfos::MyItem : public QGraphicsPolygonItem
{
	QRDrawGeoInfos* Widget;
	const RGeoInfo* Info;
	RObj2DConfigPin* Pin;

public:
	MyItem(const RGeoInfo* info,QRDrawGeoInfos* widget,const QBrush& brush);
	MyItem(const RGeoInfo* info,RObj2DConfigPin* pin,QRDrawGeoInfos* widget,const QBrush& brush);
	virtual void mousePressEvent(QGraphicsSceneMouseEvent* event);
	virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent* event);
};


//------------------------------------------------------------------------------
QRDrawGeoInfos::MyItem::MyItem(const RGeoInfo* info,QRDrawGeoInfos* widget,const QBrush& brush)
	: QGraphicsPolygonItem(), Widget(widget), Info(info), Pin(0)
{
	Widget->paintConnectors(info);

	QString Tmp;
	if(Info!=widget->Layout->GetBoard())
	{
		// Set the polygon
		RPolygon Obj(info->GetPlacedPolygon());
		QPolygonF Polygon;
		RCursor<RPoint> Cur(Obj.GetVertices());
		for(Cur.Start();!Cur.End();Cur.Next())
			Polygon<<QPointF(Widget->x(Cur()->X),Widget->y(Cur()->Y));
		setPolygon(Polygon);

		setZValue(3.0);
		setBrush(brush);
		Tmp="Object '"+ToQString(info->GetObj()->GetName())+"'<br>";
		if(info->GetOrder()!=cNoRef)
			Tmp+="Order: "+QString::number(info->GetOrder())+"<br>";
		Tmp+="Points:<br>";

		bool AddBr(false);
		for(Cur.Start();!Cur.End();Cur.Next())
		{
			if(AddBr)
				Tmp+="<br>";
			else
				AddBr=true;
			Tmp+="&nbsp;&nbsp;&nbsp;("+QString::number(Cur()->X)+","+QString::number(Cur()->Y)+")";
		}
	}
	else
	{
		// Set the polygon
		QPolygonF Polygon;
		Polygon<<QPointF(Widget->x(0),Widget->y(0))
			   <<QPointF(Widget->x(Widget->Layout->GetProblem()->GetLimits().GetWidth()),Widget->y(0))
		       <<QPointF(Widget->x(Widget->Layout->GetProblem()->GetLimits().GetWidth()),Widget->y(Widget->Layout->GetProblem()->GetLimits().GetHeight()))
		       <<QPointF(Widget->x(0),Widget->y(Widget->Layout->GetProblem()->GetLimits().GetHeight()));
		setPolygon(Polygon);
		setZValue(2.0);
		Tmp="Circuit";
	}

	setPen(QPen(Qt::black));
	setToolTip(Tmp);
}


//------------------------------------------------------------------------------
QRDrawGeoInfos::MyItem::MyItem(const RGeoInfo* info,RObj2DConfigPin* pin,QRDrawGeoInfos* widget,const QBrush& brush)
	: QGraphicsPolygonItem(), Widget(widget), Info(info), Pin(pin)
{
	RRect Rect(Pin->GetRect());
	if(Info!=widget->Layout->GetBoard())
		Rect+=Info->GetPos();
	QPolygonF Polygon;
	Polygon<<QPointF(Widget->x(Rect.GetX1()),Widget->y(Rect.GetY1()))
		   <<QPointF(Widget->x(Rect.GetX2()),Widget->y(Rect.GetY1()))
		   <<QPointF(Widget->x(Rect.GetX2()),Widget->y(Rect.GetY2()))
		   <<QPointF(Widget->x(Rect.GetX1()),Widget->y(Rect.GetY2()));
	setPolygon(Polygon);
	setBrush(brush);
	setPen(QPen(Qt::black));
	setZValue(4.0);

	QString Tmp("Connector '"+ToQString(Pin->GetPin()->GetConnector()->GetName())+"'<br>");
	Tmp+="Pin '"+Pin->GetPin()->GetName()+"'";
	setToolTip(Tmp);
}


//------------------------------------------------------------------------------
void QRDrawGeoInfos::MyItem::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
	if(event->button()!=Qt::LeftButton)
		return;
	if(Pin)
	{
		RCursor<RGeoInfoConnection> Cur(Widget->Layout->GetConnections());
		for(Cur.Start();!Cur.End();Cur.Next())
		{
			if(!Cur()->IsIn(Pin))
				continue;
			Widget->paintConnection(Cur());
		}
	}
	else
	{
		RCursor<RGeoInfoConnection> Cur(Widget->Layout->GetConnections());
		for(Cur.Start();!Cur.End();Cur.Next())
		{
			if((Info!=Widget->Layout->GetBoard())&&(!Cur()->IsIn(Info)))
				continue;
			Widget->paintConnection(Cur());
		}
	}
}


//------------------------------------------------------------------------------
void QRDrawGeoInfos::MyItem::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
	if(event->button()!=Qt::LeftButton)
		return;
	Widget->removeConnections();
}



//------------------------------------------------------------------------------
//
// class QRDrawGeoInfos
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
QRDrawGeoInfos::QRDrawGeoInfos(QWidget* parent)
	: QWidget(parent), Ui(new Ui_QRDrawGeoInfos()), FreePolygons(0), Layout(0),
	  BlueBrush(Qt::blue,Qt::BDiagPattern), RedBrush(Qt::red,Qt::SolidPattern),
	  BlackBrush(Qt::black,Qt::SolidPattern), YellowBrush(Qt::yellow,Qt::BDiagPattern),
	  GreenBrush(Qt::green,Qt::BDiagPattern), CyanBrush(Qt::cyan,Qt::BDiagPattern),
	  Nets(), LastItem(0), XScale(1), YScale(1), StepMode(false)
{
	static_cast<Ui_QRDrawGeoInfos*>(Ui)->setupUi(this);
	static_cast<Ui_QRDrawGeoInfos*>(Ui)->Draw->setScene(&Scene);
}


//------------------------------------------------------------------------------
void QRDrawGeoInfos::setLayout(RLayout* layout)
{
	if(layout==Layout)
		return;
	Layout=layout;
	repaint();
}


//------------------------------------------------------------------------------
void QRDrawGeoInfos::setPolys(RFreePolygons* polys)
{
	FreePolygons=polys;
}


//------------------------------------------------------------------------------
void QRDrawGeoInfos::setStepMode(bool step)
{
	StepMode=step;
	LastItem=0;
}


//------------------------------------------------------------------------------
void QRDrawGeoInfos::repaint(void)
{
	if(!Layout)
		return;
	QWidget* Draw((static_cast<Ui_QRDrawGeoInfos*>(Ui)->Draw)->viewport());
	RRect Board(Layout->GetProblem()->GetBoard());
	RSize Limits(Board.GetSize());
	XScale=((static_cast<double>(Draw->width()-10)/static_cast<double>(Limits.GetWidth())));
	YScale=((static_cast<double>(Draw->height()-10)/static_cast<double>(Limits.GetHeight())));
	Scene.clear();

	// The board
	QPointF Pt1(x(Board.GetX1()),y(Board.GetY1()));
	QPointF Pt2(x(Board.GetX2()),y(Board.GetY2()));
	QRectF Rect(Pt1,Pt2);
	Scene.addRect(Rect,QPen(Qt::black))->setZValue(1.0);

	// The object representing the limits
	paintInfo(Layout->GetBoard(),false);

	// The object already placed
	RCursor<RGeoInfo> Cur(Layout->GetInfos());
	for(Cur.Start();!Cur.End();Cur.Next())
		if(Cur()->IsValid())
			paintInfo(Cur(),false);

	// The polygons
	if(FreePolygons)
	{
		RCursor<RFreePolygon> Free(*FreePolygons);
		for(Free.Start();!Free.End();Free.Next())
			paintFree(Free(),false);
	}
}


//------------------------------------------------------------------------------
void QRDrawGeoInfos::paintInfo(const RGeoInfo* info,bool fly)
{
	if(info==Layout->GetBoard())
	{
		Scene.addItem(new MyItem(info,this,BlueBrush));
		return;
	}
	if(info->GetContainer())
		Scene.addItem(new MyItem(info,this,GreenBrush));
	else
	{
		if(StepMode)
		{
			if(fly)
			{
				if(LastItem)
					LastItem->setBrush(BlueBrush);
				Scene.addItem(LastItem=new MyItem(info,this,CyanBrush));
			}
			else
			{
				if(LastItem)
					Scene.addItem(LastItem=new MyItem(info,this,CyanBrush));
				else
					Scene.addItem(LastItem=new MyItem(info,this,BlueBrush));
			}
		}
		else
			Scene.addItem(new MyItem(info,this,BlueBrush));
	}
}


//------------------------------------------------------------------------------
void QRDrawGeoInfos::paintFree(const RFreePolygon* poly,bool)
{
	if(!poly)
		return;
	QPolygonF Polygon;
	RCursor<RPoint> Pt(poly->GetVertices());
	for(Pt.Start();!Pt.End();Pt.Next())
		Polygon<<QPointF(x(Pt()->X),y(Pt()->Y));
	QGraphicsPolygonItem* ptr=Scene.addPolygon(Polygon,QPen(Qt::black),YellowBrush);
	ptr->setZValue(3.0);
}


//------------------------------------------------------------------------------
void QRDrawGeoInfos::paintConnection(const RGeoInfoConnection* connect)
{
	RCursor<RGeoInfoPin> p(connect->GetPins());
	if(!p.GetNb())
		return;
	p.Start();
	RPoint Pt1(p()->GetPos());
	for(p.Next();!p.End();p.Next())
	{
		QGraphicsLineItem* ptr;
		RPoint Pt2(p()->GetPos());
		Nets.append(ptr=Scene.addLine(x(Pt1.X),y(Pt1.Y),x(Pt2.X),y(Pt1.Y),QPen(Qt::red)));
		ptr->setZValue(5.0);
		Nets.append(ptr=Scene.addLine(x(Pt2.X),y(Pt1.Y),x(Pt2.X),y(Pt2.Y),QPen(Qt::red)));
		ptr->setZValue(5.0);
		Pt1=Pt2;
	}
}


//------------------------------------------------------------------------------
void QRDrawGeoInfos::removeConnections(void)
{
	if(!Nets.size())
		return;
	for(int i=0;i<Nets.size();++i)
		Scene.removeItem(Nets.at(i));
	Nets.clear();
}


//------------------------------------------------------------------------------
void QRDrawGeoInfos::paintConnectors(const RGeoInfo* info)
{
	RCursor<RObj2DConfigConnector> Cur(info->GetConfig()->GetConnectors());
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		RCursor<RObj2DConfigPin> Cur2(*Cur());
		for(Cur2.Start();!Cur2.End();Cur2.Next())
			Scene.addItem(new MyItem(info,Cur2(),this,BlackBrush));
	}
}


//------------------------------------------------------------------------------
void QRDrawGeoInfos::resizeEvent(QResizeEvent* event)
{
	QWidget::resizeEvent(event);
	repaint();
}


//------------------------------------------------------------------------------
QRDrawGeoInfos::~QRDrawGeoInfos(void)
{
	delete static_cast<Ui_QRDrawGeoInfos*>(Ui);
}
