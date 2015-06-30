/*

	R Project Library

	QRGUIApplication.h

	Generic QT-based GUI Application - Implementation.

	Copyright 2015-2015 by Pascal Francq (pascal@francq.info).

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



//-----------------------------------------------------------------------------
// include files for R Project
#include <qrguiapplication.h>
using namespace std;
using namespace R;


//-----------------------------------------------------------------------------
// Static variables
QRGUIApplication* R::GUIApp=0;



//-----------------------------------------------------------------------------
//
// class QRGUIApplication
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
QRGUIApplication::QRGUIApplication(const RString& name,int argc, char** argv)
	: RApplication(name,argc,argv), WidgetClasses(20)
{
	if(GUIApp)
		mThrowRException("Already one application running");
	GUIApp=this;

}


//-----------------------------------------------------------------------------
void QRGUIApplication::CreateConfig(void)
{
	RApplication::CreateConfig();
	RCursor<RString> Widget(WidgetClasses);
	for(Widget.Start();!Widget.End();Widget.Next())
	{
		Config.InsertParam(new RParamValue("Width",-1),(*Widget()));
		Config.InsertParam(new RParamValue("Height",-1),(*Widget()));
	}
}


//-----------------------------------------------------------------------------
void QRGUIApplication::addWidgetClass(const R::RString& classname)
{
	WidgetClasses.InsertPtr(new RString(classname));
}


//-----------------------------------------------------------------------------
void QRGUIApplication::retrieveSize(const R::RString& classname,QWidget* widget)
{
	int width(Config.GetInt("Width",classname));
	int height(Config.GetInt("Height",classname));
	if(width==-1)
	{
		QSize Size(widget->sizeHint());
		width=Size.width();
		height=Size.height();
	}
	widget->resize(width,height);
}


//-----------------------------------------------------------------------------
void QRGUIApplication::storeSize(const R::RString& classname,QWidget* widget)
{
	Config.SetInt("Width",widget->width(),classname);
	Config.SetInt("Height",widget->height(),classname);
}


//-----------------------------------------------------------------------------
QRGUIApplication::~QRGUIApplication(void)
{
	GUIApp=0;
}