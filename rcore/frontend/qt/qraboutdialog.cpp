/*

	R Project Library

	QRAboutDialog.cpp

	About Dialog Box - Implementation.

	Copyright 2015-2015 by Pascal Francq (pascal@francq.info).

	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU Library General Public
	License as published by the Free Software Foundation; either
	version 2.0 of the License, or (at your option) any later version.

	This library is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
	Library General Public License for more details.

	You should have received a copy of the GNU Library General Public
	License along with this library, as a file COPYING.LIB; if not, write
	to the Free Software Foundation, Inc., 59 Temple Place, Suite 330,
	Boston, MA  02111-1307  USA

*/


//------------------------------------------------------------------------------
// include files for Qt
//#include <QtGui/QScrollArea>


//------------------------------------------------------------------------------
// include files for R Project
#include <qraboutdialog.h>
#include <ui_qraboutdialog.h>
#include <rqt.h>
using namespace R;
using namespace std;



//------------------------------------------------------------------------------
//
// class QRAboutDlg
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
QRAboutDialog::QRAboutDialog(const QString& name,const QString& version,QWidget* parent)
	: QDialog(parent), Ui(new Ui_QRAboutDialog()), License(License_Unknown)
{
	QVBoxLayout* verticalLayout = new QVBoxLayout(this);
	QWidget* widget=new QWidget(this);
	verticalLayout->addWidget(widget);
	Ui->setupUi(widget);
	Ui->Application->setText(name);
	Ui->Version->setText("Version "+version);
	connect(Ui->OK,SIGNAL(pressed()),this,SLOT(accept()));
}


//------------------------------------------------------------------------------
void QRAboutDialog::setDescription(const QString& desc)
{
	sDescription=desc;
}


//------------------------------------------------------------------------------
void QRAboutDialog::setCopyright(const QString& copyright)
{
	sCopyright=copyright;
}


//------------------------------------------------------------------------------
void QRAboutDialog::setURL(const QString& url)
{
	sURL=url;
}


//------------------------------------------------------------------------------
void QRAboutDialog::setLicense(tLicense license)
{
	License=license;
}


//------------------------------------------------------------------------------
void QRAboutDialog::addAuthor(const QString& name,const QString& role,const QString& email)
{
	bool NewBR(false);
	QString Author;
	if(!name.isEmpty())
	{
		Author+=name;
		NewBR=true;
		if(!email.isEmpty())
			Author+=" ("+email+")";
	}
	if(!role.isEmpty())
	{
		if(NewBR)
			Author+=" ";
		Author+="<i>"+role+"</i>";
	}
	if(!Author.isEmpty())
		sAuthors.append(Author);
}


//------------------------------------------------------------------------------
void QRAboutDialog::exec(void)
{
	// Generate About
	bool NewBR(false);
	QString sAbout;
	if(!sDescription.isEmpty())
	{
		sAbout+=sDescription;
		NewBR=true;
	}
	if(!sCopyright.isEmpty())
	{
		if(NewBR)
			sAbout+="<br/><br/>";
		sAbout+=sCopyright;
		NewBR=true;
	}
	if(!sURL.isEmpty())
	{
		if(NewBR)
			sAbout+="<br/><br/>";
		sAbout+="<a href=\""+sURL+"\"/>"+sURL+"</a>";
		NewBR=true;
	}
	switch(License)
	{
		case License_GPL:
			if(NewBR)
				sAbout+="<br/><br/>";
			sAbout+="License: GNU General Public License Version 2";
			break;
	}
   Ui->About->setWordWrap(true);
   Ui->About->setOpenExternalLinks(true);
	Ui->About->setText(sAbout);
	Ui->About->setTextInteractionFlags(Qt::TextBrowserInteraction);

	// Generate Author
	QString sAuthor;
	NewBR=false;
	for(int i=0;i<sAuthors.size();++i)
	{
		if(NewBR)
			sAuthor+="<br/>";
		else
			NewBR=true;
		sAuthor+=sAuthors.at(i);
	}
   Ui->Authors->setWordWrap(true);
   Ui->Authors->setOpenExternalLinks(true);
	Ui->Authors->setText(sAuthor);
	Ui->Authors->setTextInteractionFlags(Qt::TextBrowserInteraction);

	adjustSize();
	layout()->setSizeConstraint(QLayout::SetFixedSize);
	QDialog::exec();
}


//------------------------------------------------------------------------------
QRAboutDialog::~QRAboutDialog(void)
{

}
