/*

	QDrawPolygons.cpp

	Qt Widget to draw polygons - Implementation.

	(c) 2000-2001 by P. Francq.

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


//------------------------------------------------------------------------------
// include files for Qt
#include <qpainter.h>


//------------------------------------------------------------------------------
// include files for Qt widgets
#include <qdrawpolygons.h>
#include <frontend/kde/rqt.h>
using namespace R;



//------------------------------------------------------------------------------
//
// QInfoBox
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
QInfoBox::QInfoBox(QWidget* parent,RGeoInfo* info)
	: QPopupMenu(0,"Info Box"), Empty(true), pixmap(0), tmppixmap(0)
{
	QString Tmp;

	Tmp="Object '"+ToQString(info->GetObj()->Name)+"'";
	insertItem(Tmp);
	if(info->GetOrder()!=NoObject)
	{
		Tmp="Order: "+QString::number(info->GetOrder());
		insertItem(Tmp);
	}
	insertItem("Points:");

	RRelPointCursor Cur(info);
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		Tmp="    ("+QString::number(Cur().X)+","+QString::number(Cur().Y)+")";
		insertItem(Tmp);
	}
	afterFocus=parent;
	afterFocus->parentWidget()->setFocus();
}


//------------------------------------------------------------------------------
QInfoBox::QInfoBox(QWidget* parent,RGeoInfoConnector* con)
	: QPopupMenu(0,"Info Box"), Empty(true), pixmap(0), tmppixmap(0)
{
	QString Tmp;

	Tmp="Connector '"+ToQString(con->Con->Name)+"'";
	insertItem(Tmp);
	afterFocus=parent;
	afterFocus->parentWidget()->setFocus();
}


//------------------------------------------------------------------------------
QInfoBox::QInfoBox(QWidget* parent, QPixmap* p,QPixmap* pt)
	: QPopupMenu(0,"Info Box"), Empty(true), pixmap(p), tmppixmap(pt)
{
	afterFocus=parent;
	afterFocus->parentWidget()->setFocus();
}


//------------------------------------------------------------------------------
void QInfoBox::AddConnectionInfo(RGeoInfoConnection* con)
{
	QString Tmp;

	if(Empty)
		Empty=false;
	else
		insertSeparator();
	for(con->Con->Connect.Start();!con->Con->Connect.End();con->Con->Connect.Next())
	{
		Tmp="Connector '"+ToQString(con->Con->Connect()->Owner->Name)+"\t|\t"+ToQString(con->Con->Connect()->Name)+"'";
		insertItem(Tmp);
	}
}


//------------------------------------------------------------------------------
void QInfoBox::mouseReleaseEvent(QMouseEvent*)
{
	if(pixmap)
	{
		bitBlt(afterFocus,afterFocus->rect().topLeft(),pixmap);
		delete tmppixmap;
	}
	afterFocus->parentWidget()->setFocus();
	delete(this);
}



//------------------------------------------------------------------------------
//
// QDrawPolygons
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
QDrawPolygons::QDrawPolygons(QWidget* parent,const char* name)
	: QWidget(parent,name), Problem(0), Limits(1,1), pixmap(0), tmppixmap(0),Changed(false), Last(0),
	  NbInfos(0), Infos(0), Painter(0), FreePolygons(0), brBlack(black,BDiagPattern),
	  brGreen(green,BDiagPattern),brYellow(yellow,BDiagPattern),
	  brBlue(blue,BDiagPattern), brRed(red,SolidPattern)
{
	Translation.Set(10,10);
}


//------------------------------------------------------------------------------
QDrawPolygons::QDrawPolygons(RProblem2D* prob,QWidget* parent,const char* name)
	: QWidget(parent,name), Problem(prob), Limits(1,1), pixmap(0), tmppixmap(0),Changed(false),
	  Last(0), NbInfos(0), Infos(0), Painter(0), FreePolygons(0), brBlack(black,BDiagPattern),
	  brGreen(green,BDiagPattern),brYellow(yellow,BDiagPattern),
	  brBlue(blue,BDiagPattern), brRed(red,SolidPattern)
{
	Limits.Set(prob->GlobalLimits.X+20,prob->GlobalLimits.Y+20);
	Translation.Set(prob->Translation.X+10,prob->Translation.Y+10);
}


//------------------------------------------------------------------------------
void QDrawPolygons::addInfo(RGeoInfo* info)
{
	unsigned int j;
	QPoint Null(0,0);
	QPointArray Pts;
	RPoint Pt;
	RRelPointCursor Cur;

	if(!pixmap)
	{
		QRect r=rect();
		pixmap=new QPixmap(r.size());
		FactorX=((double)r.width())/((double)Limits.X);
		FactorY=((double)r.height())/((double)Limits.Y);
		Changed=true;
	}
	Painter=new QPainter(pixmap);
	CHECK_PTR(Painter);
	Painter->setPen(black);
	if(Last&&Last->IsValid())
	{
		Painter->setBrush(brBlue);
		Pts.fill(Null,Last->NbPoints());
		Cur.Set(Last);
		for(Cur.Start(),j=0;!Cur.End();Cur.Next(),j++)
		{
			Pt=Cur();
			Pts.setPoint(j,RealToScreenX(Pt.X+Translation.X),RealToScreenY(Pt.Y+Translation.Y));
		}
		Painter->drawPolygon(Pts);
	}
	Painter->setBrush(brBlack);
	Pts.fill(Null,info->NbPoints());
	Cur.Set(info);
	for(Cur.Start(),j=0;!Cur.End();Cur.Next(),j++)
	{
		Pt=Cur();
		Pts.setPoint(j,RealToScreenX(Pt.X+Translation.X),RealToScreenY(Pt.Y+Translation.Y));
	}
	Painter->drawPolygon(Pts);
	paintConnectors(info,Painter);
	delete Painter;
	Painter=0;
	Last=info;
	repaint();
}


//------------------------------------------------------------------------------
void QDrawPolygons::addFree(RFreePolygon* poly)
{
	unsigned int j;
	QPoint Null(0,0);
	QPointArray Pts;
	RPoint *Pt,Pos;

	if(!pixmap)
	{
		QRect r=rect();
		pixmap=new QPixmap(r.size());
		FactorX=((double)r.width())/((double)Limits.X);
		FactorY=((double)r.height())/((double)Limits.Y);
		Changed=true;
	}
	Painter=new QPainter(pixmap);
	CHECK_PTR(Painter);
	Painter->setPen(black);
	Painter->setBrush(brYellow);
	Pts.fill(Null,poly->NbPtr);
	for(poly->Start(),j=0;!poly->End();poly->Next(),j++)
	{
		Pt=(*poly)();
		Pos=poly->GetPos();
		Pts.setPoint(j,RealToScreenX(Pt->X+Translation.X),RealToScreenY(Pt->Y+Translation.Y));
	}
	Painter->drawPolygon(Pts);
	delete Painter;
	Painter=0;
	repaint();
}


//------------------------------------------------------------------------------
void QDrawPolygons::setLimits(const RPoint& limits)
{
	Limits.Set(limits.X+20,limits.Y+20);
	if(pixmap)
	{
		QRect r=rect();
		pixmap->resize(r.size());
		FactorX=((double)r.width())/((double)Limits.X);
		FactorY=((double)r.height())/((double)Limits.Y);
		Changed=true;
	}
}


//------------------------------------------------------------------------------
QPainter* QDrawPolygons::begin(void)
{
	Painter=new QPainter(pixmap);
	CHECK_PTR(Painter);
	return(Painter);
}


//------------------------------------------------------------------------------
void QDrawPolygons::end(void)
{
	delete Painter;
	Painter=0;
}


//------------------------------------------------------------------------------
void QDrawPolygons::paintConnectors(RGeoInfo* info,QPainter* Painter)
{
	unsigned i,j;
	RGeoInfoConnector** tab;
	RCoord x,y;

	Painter->setPen(black);
	Painter->setBrush(SolidPattern);
	for(i=info->Connectors.NbPtr+1,tab=info->Connectors.Tab;--i;tab++)
	{
		for(j=0;j<(*tab)->NbPos;j++)
		{
			x=RealToScreenX((*tab)->Pos[j].X+Translation.X+info->GetPos().X-1);
			y=RealToScreenY((*tab)->Pos[j].Y+Translation.Y+info->GetPos().Y+1);
			Painter->drawRect(x,y,static_cast<int>(3*FactorX),static_cast<int>(3*FactorY));
		}
	}
}


//------------------------------------------------------------------------------
void QDrawPolygons::TransformExternCon(RCoord& x,RCoord& y)
{
	// X-Axis
	if(x<Translation.X)
	{
		// Left Extern
		x+=5;
	}
	else
	{
		if(x>Problem->Limits.X+Problem->Translation.X)
		{
			// Right Extern
			x+=15;
		}
		else
		{
			// Middle
			x+=10;
		}
	}

	// Y-Axis
	if(y<Translation.Y)
	{
		// Bottom Extern
		y+=5;
	}
	else
	{
		if(y>Problem->Limits.Y+Problem->Translation.Y)
		{
			// Top Extern
			y+=15;
		}
		else
		{
			// Middle
			y+=10;
		}
	}
}


//------------------------------------------------------------------------------
void QDrawPolygons::AdaptExternCon(RCoord& x,RCoord& y)
{
	// Absolute Adaptation
	x=x+Translation.X-Problem->Translation.X;
	y=y+Translation.Y-Problem->Translation.Y;

	// X-Axis
	if(x<Translation.X)
	{
		// Left Extern
		x-=5;
	}
	else
	{
		if(x>Problem->Limits.X+Problem->Translation.X)
		{
			// Right Extern
			x-=15;
		}
		else
		{
			// Middle
			x-=10;
		}
	}

	// Y-Axis
	if(y<Translation.Y)
	{
		// Bottom Extern
		y-=5;
	}
	else
	{
		if(y>Problem->Limits.Y+Problem->Translation.Y)
		{
			// Top Extern
			y-=15;
		}
		else
		{
			// Middle
			y-=10;
		}
	}
}


//------------------------------------------------------------------------------
void QDrawPolygons::paintConnection(RGeoInfoConnection* con,QPainter* Painter,bool c)
{
	RGeoInfoConnectionPart *p;
	int X1,Y1,X2,Y2;

	for(con->Start();!con->End();con->Next())
	{
		p=(*con)();
		X1=p->PosCon1.X;
		Y1=p->PosCon1.Y;
		X2=p->PosCon2.X;
		Y2=p->PosCon2.Y;

		if(p->Id1==Problem->Problem.GetId())
		{
			X1+=Problem->Translation.X;
			Y1+=Problem->Translation.Y;
			TransformExternCon(X1,Y1);
		}
		else
		{
			X1+=Translation.X;
			Y1+=Translation.Y;
		}

		if(p->Id2==Problem->Problem.GetId())
		{
			X2+=Problem->Translation.X;
			Y2+=Problem->Translation.Y;
			TransformExternCon(X2,Y2);
		}
		else
		{
			X2+=Translation.X;
			Y2+=Translation.Y;
		}

		if(c)
		{
			Painter->drawRect(RealToScreenX(X1-2),RealToScreenY(Y1+2),static_cast<int>(5*FactorX),static_cast<int>(5*FactorY));
			Painter->drawRect(RealToScreenX(X2-2),RealToScreenY(Y2+2),static_cast<int>(5*FactorX),static_cast<int>(5*FactorY));
		}
		Painter->drawLine(RealToScreenX(X1),RealToScreenY(Y1),RealToScreenX(X1),RealToScreenY(Y2));
		Painter->drawLine(RealToScreenX(X1),RealToScreenY(Y2),RealToScreenX(X2),RealToScreenY(Y2));
	}
}


//------------------------------------------------------------------------------
void QDrawPolygons::paintEvent(QPaintEvent*)
{
	QRect r=rect();
	if(!pixmap)
	{
		pixmap=new QPixmap(r.size());
		if(Problem)
		FactorX=((double)r.width())/((double)Limits.X);
		FactorY=((double)r.height())/((double)Limits.Y);
		Changed=true;
	}
	if(Changed)
	{
		unsigned int i,j;
		QPoint Null(0,0);
		QPointArray Pts;
		RGeoInfo **o;
		RFreePolygon *p;
		RPoint *Pt,Pos;

		pixmap->fill(this,r.topLeft());
		Painter=new QPainter(pixmap);
		CHECK_PTR(Painter);
		Painter->setPen(black);
		if(Infos)
		{
			for(i=Infos->RealNb+1,o=Infos->Tab;--i;o++)
			{
				if((*o)->IsSelect())
					Painter->setBrush(brGreen);
				else
				{
					if((*o)==Last)
						Painter->setBrush(brBlack);
					else
						Painter->setBrush(brBlue);
				}
				if((*o)->IsValid())
				{
					Pts.fill(Null,(*o)->NbPoints());
					RRelPointCursor Cur(*o);
					for(Cur.Start(),j=0;!Cur.End();Cur.Next(),j++)
					{
						Pos=Cur();
						Pts.setPoint(j,RealToScreenX(Pos.X+Translation.X),RealToScreenY(Pos.Y+Translation.Y));
					}
					Painter->drawPolygon(Pts);
					paintConnectors(*o,Painter);
				}
			}
		}
		if(FreePolygons)
		{
			Painter->setBrush(brYellow);
			for(FreePolygons->Start();!FreePolygons->End();FreePolygons->Next())
			{
				p=(*FreePolygons)();
				Pts.fill(Null,p->NbPtr);
				for(p->Start(),j=0;!p->End();p->Next(),j++)
				{
					Pt=(*p)();
					Pos=p->GetPos();
					Pts.setPoint(j,RealToScreenX(Pt->X+Translation.X),RealToScreenY(Pt->Y+Translation.Y));
				}
				Painter->drawPolygon(Pts);
			}
		}
		Painter->setBrush(NoBrush);
		Painter->setPen(red);
		Painter->drawRect(0,0,r.width(),r.height());
		if(Problem&&Infos)
		{
			Painter->drawRect(RealToScreenX(Translation.X),RealToScreenY(Translation.Y+Problem->Limits.Y),
				static_cast<int>((Problem->Limits.X+1)*FactorX),static_cast<int>((Problem->Limits.Y+1)*FactorY));
			unsigned i,j;
			RGeoInfoConnector** tab;
			RGeoInfo* info;
			RCoord x,y;
			Painter->setPen(black);
			Painter->setBrush(SolidPattern);
			info=Infos->GetPtr<unsigned int>(Problem->Problem.GetId());
			if(info)
			{
				for(i=info->Connectors.NbPtr+1,tab=info->Connectors.Tab;--i;tab++)
				{
					for(j=0;j<(*tab)->NbPos;j++)
					{
						x=(*tab)->Pos[j].X;
						y=(*tab)->Pos[j].Y;
						TransformExternCon(x,y);
						Painter->drawRect(RealToScreenX(x-1),RealToScreenY(y+1),static_cast<int>(3*FactorX),static_cast<int>(3*FactorY));
					}
				}
			}
		}
		delete Painter;
		Painter=0;
		Changed=false;
	}
	bitBlt(this,r.topLeft(),pixmap);
}


//------------------------------------------------------------------------------
void QDrawPolygons::resizeEvent(QResizeEvent*)
{
	if(pixmap)
	{
		QRect r=rect();
		pixmap->resize(r.size());
		FactorX=((double)r.width())/((double)Limits.X);
		FactorY=((double)r.height())/((double)Limits.Y);
		Changed=true;
	}
}


//------------------------------------------------------------------------------
void QDrawPolygons::mouseReleaseEvent(QMouseEvent* e)
{
	if((tmppixmap)&&(e->button()==RightButton))
	{
		QRect r(rect());

		bitBlt(this,r.topLeft(),pixmap);
		delete tmppixmap;
		tmppixmap=0;
	}
}


//------------------------------------------------------------------------------
void QDrawPolygons::mousePressEvent(QMouseEvent* e)
{
	if(e->button()==RightButton)
	{
		RGeoInfoConnector* con;
		RGeoInfo** info;
		unsigned int i;
		RPoint Pos,Pos2;
		QRect r;
		QInfoBox *InfoBox;

		r=rect();
		if(e->state()==ControlButton)
		{
			// Click with Left Button + Ctrl -> Show all connections
 			tmppixmap=new QPixmap(*pixmap);
 			Painter=new QPainter(tmppixmap);
 			CHECK_PTR(Painter);
 			Painter->setPen(red);
			for(Infos->Cons.Start();!Infos->Cons.End();Infos->Cons.Next())
			{
				paintConnection(Infos->Cons(),Painter,false);
			}
			delete Painter;
			Painter=0;
			bitBlt(this,r.topLeft(),tmppixmap);
		}
		else
		{
			Pos2.X=static_cast<RCoord>(e->x()/FactorX)-Translation.X;
			Pos2.Y=Limits.Y-static_cast<RCoord>(e->y()/FactorY)-Translation.Y;
			for(i=Infos->NbPtr+1,info=Infos->Tab;--i;info++)
			{
				Pos=Pos2;
				if((*info)->GetObj()->GetId()==Problem->Problem.GetId())
				{
					AdaptExternCon(Pos.X,Pos.Y);
				}
				if((*info)->IsIn(Pos))
				{
					con=(*info)->GetConnector(Pos);
					if(e->state()==ShiftButton)
					{
						// Click with Left Button -> Show the Connections
						tmppixmap=new QPixmap(*pixmap);
						Painter=new QPainter(tmppixmap);
						CHECK_PTR(Painter);
						Painter->setPen(red);
						Painter->setBrush(brRed);
						InfoBox=0;
						for(Infos->Cons.Start();!Infos->Cons.End();Infos->Cons.Next())
						{
							if(con)
							{
								if(Infos->Cons()->IsIn(con))
								{
									if(!InfoBox)
										InfoBox=new QInfoBox(this,pixmap,tmppixmap);
									paintConnection(Infos->Cons(),Painter,true);
									InfoBox->AddConnectionInfo(Infos->Cons());
								}
							}
							else
							{
								if(Infos->Cons()->IsIn(*info))
								{
									if(!InfoBox)
										InfoBox=new QInfoBox(this,pixmap,tmppixmap);
									paintConnection(Infos->Cons(),Painter,true);
									InfoBox->AddConnectionInfo(Infos->Cons());
								}
							}
						}
						delete Painter;
						Painter=0;
						bitBlt(this,r.topLeft(),tmppixmap);
						if(InfoBox)
						{
							QPoint pt(0,0);
							InfoBox->popup(pt);
							tmppixmap=0;
						}
					}
					else
					{
						if(con)
						{
							// Click with Left Button -> Call the Info box
							// for a connector.
							InfoBox = new QInfoBox(this,con);
							InfoBox->popup(e->globalPos());
						}
						else
						{
							// Click with Left Button -> Call the Info box
							// for a geometric information.
							InfoBox = new QInfoBox(this,*info);
							InfoBox->popup(e->globalPos());
						}
					}

					// Geometric information found.
					break;
				}
			}
		}
	}
	else
		QWidget::mousePressEvent(e);
}


//------------------------------------------------------------------------------
QDrawPolygons::~QDrawPolygons(void)
{
	if(pixmap) delete pixmap;
}
