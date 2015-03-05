/*

	R Project Library

	QRAboutDialog.h

	About Dialog Box - Header.

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
#ifndef QRAboutDialog_H
#define QRAboutDialog_H


//------------------------------------------------------------------------------
// include files for Qt
#include <QtCore/QList>
#include <QtGui/QDialog>
class Ui_QRAboutDialog; // Generate by Qt (moc)


//------------------------------------------------------------------------------
namespace R{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* The QRAboutDialog provides a representation for a dialog box presenting
* "About information" for a given object (for example related to an
* application). It is inspired by the KAboutApplicationDialog of KDE.
*
* Several information can be specified (description, an URL, a list of authors,
* etc.).
*
* Here is an example of use :
* @code
*	QRAboutDialog dlg("Test R","1.0");
*	dlg.setDescription("This is a simple application using the R Libraries.");
*	dlg.setCopyright(QWidget::trUtf8("(C) 2010-2015 by the Paul Otlet Institute"));
*	dlg.setURL("http://www.otlet-institute.org");
*	dlg.setLicense(QRAboutDialog::License_GPL);
*	dlg.addAuthor(QWidget::trUtf8("Pascal Francq"),QWidget::trUtf8("Maintainer"), "pascal@francq.info");
*	dlg.exec();
* @endcode
* @short About Dialog Box
*/
class QRAboutDialog : QDialog
{
	Q_OBJECT

public:

	/**
	 * Type of license recognised.
	 */
	enum tLicense
	{
		License_Unknown,   /** Unknown license.*/
		License_GPL        /** The GNU GPL license. */
	};

private:

	/**
	 * Main Widget.
	 */
	Ui_QRAboutDialog* Ui;

	/**
	 * Description of the object.
	 */
	QString sDescription;

	/**
	 * Copyright information.
	 */
	QString sCopyright;

	/**
	 * URL that can be associated.
	 */
	QString sURL;

	/**
	 * License associated.
	 */
	tLicense License;

	/**
	 * List of authors.
	 */
	QList<QString> sAuthors;

public:

	/**
	 * Construct the about dialog box.
    * @param name           Name of the object.
    * @param version        Version of the object.
    * @param parent         Parent widget.
    */
	QRAboutDialog(const QString& name,const QString& version=QString::null,QWidget* parent=0);

	/**
	 * Set the description of the object.
    * @param desc           Description.
    */
	void setDescription(const QString& desc);

	/**
	 * Set the copyright information of the object.
    * @param copyright      Copyright information.
    */
	void setCopyright(const QString& copyright);

	/**
	 * Set the URL associated to the object.
    * @param url            URL.
    */
	void setURL(const QString& url);

	/**
	 * Set the type of license associated to the object.
    * @param license        The license.
    */
	void setLicense(tLicense license);

	/**
	 * Add an author of the object.
    * @param name           Name of the author.
    * @param role           Role of the author.
    * @param email          E-mail of the author.
    */
	void addAuthor(const QString& name,const QString& role=QString::null,const QString& email=QString::null);

	/**
	 * Show the dialog box.
    */
	void exec(void);

	/**
	 * Destructor.
    */
	~QRAboutDialog(void);
};


}  //-------- End of namespace R -----------------------------------------------


//------------------------------------------------------------------------------
#endif
