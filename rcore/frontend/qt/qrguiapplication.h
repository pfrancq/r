/*

	R Project Library

	QRGUIApplication.h

	Generic QT-based GUI Application - Header.

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
#ifndef QRGUIApplication_H
#define QRGUIApplication_H


//-----------------------------------------------------------------------------
// include files for R Project
#include <rapplication.h>


//-----------------------------------------------------------------------------
// include files for Qt
#include <QWidget>


//-----------------------------------------------------------------------------
namespace R{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
//	Convenient macros
#define mRetrieveSize(class)                                 \
	GUIApp->retrieveSize(#class,this)

#define mStoreSize(class)                                     \
	GUIApp->storeSize(#class,this);


//-----------------------------------------------------------------------------
/**
* The QRGUIApplication provides a class to represent a GUI application based
* on Qt. It provides a mechanism to store and retrieve the size of a
* window.
*
* Here is an example:
* @code
*
* class MainWidget : public QWidget
* {
* public:
*	MainWidget(void) : QWidget()
*	{
*		mRretrieveSize(MainWidget);
*	}
*	~MainWidget(void)
*	{
*		mStoreSize(MainWidget);
*	}
* };
*
* class MyApplication : public QRGUIApplication
* {
* public:
*   MyApplication(int argc, char** argv)
*   	: QRGUIApplication("Test",argc,argv)
*   {
*		 addWidgetClass("MainWidget");
*   }
*   virtual void Run(void)
*   {
*      Main=new MainWidget();
*	    Main->show();
*	    QApplication::instance()->exec();
*   }
* };
*
* //------------------------------------------------------------------------------
* int main(int argc, char *argv[])
* {
*	QApplication Super(argc,argv);
*	MyApplication App(argc,argv);
*	App.Execute();
*	return(EXIT_SUCCESS);
* }
* @endcode
* @short Generic QT-based GUI Application
*/
class QRGUIApplication : public RApplication
{
	/**
	 * All the widget classes for which the size must be managed.
	 */
	R::RContainer<R::RString,true,false> WidgetClasses;

public:

	/**
	* Construct a generic Qt-based GUI application.
	* @param name            Name of the application.
	* @param argc            Number of arguments of the program (received from
	*                        main).
	* @param argv            Arguments of the program (received from main).
	*/
	QRGUIApplication(const RString& name,int argc, char** argv);

	/**
	 * Create the configuration parameters. It practice, for each widget classes
	 * registered, it creates the two parameters (Class::Width and
	 * Class::Height).
    */
	virtual void CreateConfig(void);

	/**
	 * Add a widget class for which the size must be managed.
    * @param classname      Name of the class.
    */
	void addWidgetClass(const R::RString& classname);

	/**
	 * Retrieve the size for a given class name and assign it to a widget.
	 *
	 * This method can be called in the constructor of a widget.
    * @param classname      Name of the class.
    * @param widget         Widget.
    */
	void retrieveSize(const R::RString& classname,QWidget* widget);

	/**
	 * Store the size for a given class name from a given widget.
	 *
	 * This method is typically called from the destructor of a widget.
    * @param classname      Name of the class.
    * @param widget         Widget.
    */
	void storeSize(const R::RString& classname,QWidget* widget);

	/**
	 * Destructor.
	 */
	virtual ~QRGUIApplication(void);
};


//-----------------------------------------------------------------------------
/**
* Pointer to the current application. Only one application can be run at once.
*/
extern QRGUIApplication* GUIApp;


}  //-------- End of namespace R ----------------------------------------------


//-----------------------------------------------------------------------------
#endif

