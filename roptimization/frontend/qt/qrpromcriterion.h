/*
	R Project Library

	QRPromCriterion.h

	PROMETHEE Criterion Widget - Header.

Copyright 2016-2016 by Pascal Francq (pascal@francq.info).
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
#ifndef QRPromCriterionH
#define QRPromCriterionH


//------------------------------------------------------------------------------
// include files for R Project
#include <rpromcriterion.h>
#include <rparam.h>


//------------------------------------------------------------------------------
// include files for Qt/KDE
#include <QtGui/QFrame>


//------------------------------------------------------------------------------
namespace R{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
 * The QRPromCriterion class provides a widget to display the parameters of a
 * PROMETHEE criterion.
 * @short PROMETHEE Criterion Widget.
 */
class QRPromCriterion : public QFrame
{
	Q_OBJECT

	void* Ui;

public:

	/**
	* Construct the XML debug widget.
	* @param parent         Parent of the widget.
	*/
	QRPromCriterion(QWidget* parent=0);

	/**
	 * Set the parameters of a criterion to the widget.
	 * @param param          Parameter representing the criterion.
	 */
	void Set(RParamStruct* param);

	/**
	 * Set the parameters of the widget to a criterion.
	 * @param param          Parameter representing the criterion.
	 */
	void Get(RParamStruct* param);

	/**
	* Destruct the XML debug widget.
	*/
	~QRPromCriterion(void);
};


}  //------- End of namespace R ------------------------------------------------


//------------------------------------------------------------------------------
#endif
