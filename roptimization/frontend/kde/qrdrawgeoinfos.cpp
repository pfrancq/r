/*

	R Project Library

	QRDrawGeoInfos.h

	Qt Widget to draw a set of geometric information - Implementation.

	Copyright 2000-2009 by Pascal Francq (pascal@francq.info).

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
	RGeoInfo* Info;
	RGeoInfoConnector* Con;
	size_t Pos;

public:
	MyItem(RGeoInfo* info,QRDrawGeoInfos* widget,const QBrush& brush);
	MyItem(RGeoInfo* info,RGeoInfoConnector* con,size_t pos,QRDrawGeoInfos* widget,const QBrush& brush);
	virtual void mousePressEvent(QGraphicsSceneMouseEvent* event);
	virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent* event);
};


//------------------------------------------------------------------------------
QRDrawGeoInfos::MyItem::MyItem(RGeoInfo* info,QRDrawGeoInfos* widget,const QBrush& brush)
	: QGraphicsPolygonItem(), Widget(widget), Info(info), Con(0), Pos(cNoRef)
{
	Widget->paintConnectors(info);

	// Set the polygon
	QPolygonF Polygon;
	RPoint Pos(info->GetPos());
	RCursor<RPoint> Cur(*info->GetBound());
	for(Cur.Start();!Cur.End();Cur.Next())
		Polygon<<QPointF(Widget->x(Cur()->X+Pos.X),Widget->y(Cur()->Y+Pos.Y));
	setPolygon(Polygon);

	QString Tmp;
	if(Info->GetObj()->GetId()!=Widget->Infos->RealNb)
	{
		setZValue(3.0);
		setBrush(brush);
		Tmp="Object '"+ToQString(info->GetObj()->Name)+"'<br>";
		if(info->GetOrder()!=cNoRef)
			Tmp+="Order: "+QString::number(info->GetOrder())+"<br>";
		Tmp+="Points:<br>";

		RRelPointCursor Cur2(*info);
		bool AddBr(false);
		for(Cur2.Start();!Cur2.End();Cur2.Next())
		{
			if(AddBr)
				Tmp+="<br>";
			else
				AddBr=true;
			Tmp+="&nbsp;&nbsp;&nbsp;("+QString::number(Cur2().X)+","+QString::number(Cur2().Y)+")";
		}
	}
	else
	{
		setZValue(2.0);
		Tmp="Circuit";
	}

	setPen(QPen(Qt::black));
	setToolTip(Tmp);
}


//------------------------------------------------------------------------------
QRDrawGeoInfos::MyItem::MyItem(RGeoInfo* info,RGeoInfoConnector* con,size_t pos,QRDrawGeoInfos* widget,const QBrush& brush)
	: QGraphicsPolygonItem(), Widget(widget), Info(info), Con(con), Pos(pos)
{
	tCoord X(Con->Pos[Pos].X+Info->GetPos().X-1),Y(Con->Pos[Pos].Y+Info->GetPos().Y-1);
	QPolygonF Polygon;
	Polygon<<QPointF(Widget->x(X),Widget->y(Y))<<QPointF(Widget->x(X+3),Widget->y(Y))<<QPointF(Widget->x(X+3),Widget->y(Y+3))<<QPointF(Widget->x(X),Widget->y(Y+3));
	setPolygon(Polygon);
	setBrush(brush);
	setPen(QPen(Qt::black));
	setZValue(4.0);

	QString Tmp("Connector '"+ToQString(con->Con->Name)+"'<br>");
	Tmp+="Pin "+QString::number(pos);
	setToolTip(Tmp);
}


//------------------------------------------------------------------------------
void QRDrawGeoInfos::MyItem::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
	if(event->button()!=Qt::LeftButton)
		return;
	if(Con)
	{
		RCursor<RGeoInfoConnection> Cur(Widget->Infos->Cons);
		for(Cur.Start();!Cur.End();Cur.Next())
		{
			if(!Cur()->IsIn(Con))
				continue;
			Widget->paintConnection(Cur());
		}
	}
	else
	{
		RCursor<RGeoInfoConnection> Cur(Widget->Infos->Cons);
		for(Cur.Start();!Cur.End();Cur.Next())
		{
			if((Info->GetObj()->GetId()!=Widget->Infos->RealNb)&&(!Cur()->IsIn(Info)))
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
	: QWidget(parent), Ui(new Ui_QRDrawGeoInfos()), FreePolygons(0), Infos(0),
	  BlueBrush(Qt::blue,Qt::BDiagPattern), RedBrush(Qt::red,Qt::SolidPattern),
	  BlackBrush(Qt::black,Qt::SolidPattern), YellowBrush(Qt::yellow,Qt::BDiagPattern),
	  GreenBrush(Qt::green,Qt::BDiagPattern), CyanBrush(Qt::cyan,Qt::BDiagPattern),
	  Nets(), LastItem(0), XScale(1), YScale(1), StepMode(false)
{
	static_cast<Ui_QRDrawGeoInfos*>(Ui)->setupUi(this);
	static_cast<Ui_QRDrawGeoInfos*>(Ui)->Draw->setScene(&Scene);
}


//------------------------------------------------------------------------------
void QRDrawGeoInfos::setInfos(RGeoInfos* infos,const RPoint& limits,const RPoint& translation)
{
	if(infos==Infos)
		return;
	Infos=infos;
	Limits=limits;
	Translation=translation;
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
	if(!Infos)
		return;
	QGraphicsView* Draw(static_cast<Ui_QRDrawGeoInfos*>(Ui)->Draw);
	XScale=((static_cast<double>(Draw->width()-10)/static_cast<double>(Limits.X)));
	YScale=((static_cast<double>(Draw->height()-10)/static_cast<double>(Limits.Y)));
	Scene.clear();

	// The board
	QPointF Pt1(x(0),y(0));
	QPointF Pt2(x(Infos->Problem->Limits.X),y(Infos->Problem->Limits.Y));
	QRectF Rect(Pt1,Pt2);
	Scene.addRect(Rect,QPen(Qt::black))->setZValue(1.0);

	// The object already place
	RCursor<RGeoInfo> Cur(*Infos);
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
void QRDrawGeoInfos::paintInfo(RGeoInfo* info,bool fly)
{
	if(!info)
		return;
	if(info->IsSelect())
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
void QRDrawGeoInfos::paintFree(RFreePolygon* poly,bool)
{
	if(!poly)
		return;
	QPolygonF Polygon;
	RCursor<RPoint> Pt(*poly);
	for(Pt.Start();!Pt.End();Pt.Next())
		Polygon<<QPointF(x(Pt()->X),y(Pt()->Y));
	Scene.addPolygon(Polygon,QPen(Qt::black),YellowBrush);
}


//------------------------------------------------------------------------------
void QRDrawGeoInfos::paintConnection(RGeoInfoConnection* connect)
{
	RCursor<RGeoInfoConnectionPart> p(*connect);
	p.Start();
	for(p.Start();!p.End();p.Next())
	{
		QGraphicsLineItem* ptr;
		Nets.append(ptr=Scene.addLine(x(p()->PosCon1.X),y(p()->PosCon1.Y),x(p()->PosCon2.X),y(p()->PosCon1.Y),QPen(Qt::red)));
		ptr->setZValue(4.0);
		Nets.append(ptr=Scene.addLine(x(p()->PosCon2.X),y(p()->PosCon1.Y),x(p()->PosCon2.X),y(p()->PosCon2.Y),QPen(Qt::red)));
		ptr->setZValue(4.0);
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
void QRDrawGeoInfos::paintConnectors(RGeoInfo* info)
{
	if(!info)
		return;

	RCursor<RGeoInfoConnector> Cur(info->Connectors);
	for(Cur.Start();!Cur.End();Cur.Next())
		for(size_t j=0;j<Cur()->NbPos;j++)
			Scene.addItem(new MyItem(info,Cur(),j,this,BlackBrush));
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
