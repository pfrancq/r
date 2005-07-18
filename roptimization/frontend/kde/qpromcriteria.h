/*

	R Project Library

	QPromCriteria.h

	Qt Widget to configure a list of Promethee Criterion - Header.

	(C) 2002 by P. Francq.

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
#ifndef QPromCriteriaH
#define QPromCriteriaH


//------------------------------------------------------------------------------
// include files for R Project
#include <rcontainer.h>
#include <rpromcriterion.h>


//------------------------------------------------------------------------------
// include files for Qt
#include <qgroupbox.h>
class QComboBox;
class QVBoxLayout;
class QLineEdit;


//------------------------------------------------------------------------------
namespace R{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* The QPromCriteria class provides a QT widget to configure a list of Prom�h�
* Criterion.
* @short Configuration of PROMETHEE Criteria.
* @author Pascal Francq.
*/
class QPromCriteria : public QGroupBox
{
	class QPromCriterion;

	Q_OBJECT

	/**
	* Direct access to the criteria.
	*/
	QComboBox* Direct;

	/**
	* Index of the current criteria;
	*/
	int Current;

	/**
	* Preference threshold (p) for the criterion.
	*/
	QLineEdit* P;

	/**
	* Indifference threshold (q) for the criterion.
	*/
	QLineEdit* Q;

	/**
	* Weight for the criterion.
	*/
	QLineEdit* W;

	/**
	* All criteria handled by this widget.
	*/
	R::RContainer<QPromCriterion,true,false> Crits;

public:

	/**
	* Construct a criterion.
	* @param parent         Parent widget.
	* @param name           Name of the widget.
	*/
	QPromCriteria(QWidget* parent,const char* name);

	/**
	* Insert a criterion.
	* @param crit           Name of the criterion.
	*/
	void InsertCriterion(const char* crit);

	/**
	* Set the parameters for a particular criterion of PROMETHEE.
	* @param crit           Name of the criterion.
	* @param p              Preference's threshold.
	* @param q              Indifference's threshold.
	* @param w              Weight of the criterion.
	*/
	void SetCriterionParam(const char* crit,double p,double q,double w);

	/**
	* Set the parameters for a particular criterion of PROMETHEE.
	* @param crit           Name of the criterion.
	* @param p              Parameters.
	*/
	void SetCriterionParam(const char* crit,R::RPromCriterionParams& p);

	/**
	* Set the parameters for a particular criterion of PROMETHEE.
	* @param crit           Name of the criterion.
	* @param p              Preference's threshold.
	* @param q              Indifference's threshold.
	* @param w              Weight of the criterion.
	*/
	void GetCriterionParam(const char* crit,double& p,double& q,double& w);

	/**
	* Set the parameters for a particular criterion of PROMETHEE.
	* @param crit           Name of the criterion.
	* @param p              Parameters.	*/
	void GetCriterionParam(const char* crit,R::RPromCriterionParams& p);

public slots:

	/**
	* Slot called when a new criterion was selected
	* @param index          Index of the selected criterion.
	*/
	void slotActivated(int index);

	/**
	* Slot called when a value was changed.
	*/
	void slotChanged(void);

signals:

	/**
	* Signal called when a value is changed
	*/
	void valueChanged(void);

public:

	/**
	* Destruct the Criterion.
	*/
	~QPromCriteria(void);
};


}  //-------- End of namespace R -----------------------------------------------


//------------------------------------------------------------------------------
#endif
