/*
 	R Project Library

	QRPromCriterion.cpp

	PROMETHEE Criterion Widget - Implementation.

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
	Boston, MA  02111-1307  USA*

*/



//------------------------------------------------------------------------------
// include files for R Library
#include <qrpromcriterion.h>
#include <ui_qrpromcriterion.h>
#include <rqt.h>
using namespace R;
using namespace std;



//------------------------------------------------------------------------------
//
// QRPromCriterion
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
QRPromCriterion::QRPromCriterion(QWidget* parent)
	: QFrame(parent), Ui(new Ui_QRPromCriterion())
{
	Ui_QRPromCriterion* ptr(static_cast<Ui_QRPromCriterion*>(Ui));
	ptr->setupUi(this);
}


//------------------------------------------------------------------------------
void QRPromCriterion::Set(RParamStruct* param)
{
	Ui_QRPromCriterion* ptr(static_cast<Ui_QRPromCriterion*>(Ui));
	bool Active(param->Get<RParamValue>("Active")->GetBool());
	ptr->Active->setText(ToQString(param->GetName()));
	ptr->Active->setChecked(Active);
	ptr->P->setValue(param->Get<RParamValue>("P")->GetDouble());
	ptr->P->setEnabled(Active);
	ptr->Q->setValue(param->Get<RParamValue>("Q")->GetDouble());
	ptr->Q->setEnabled(Active);
	ptr->Weight->setValue(param->Get<RParamValue>("Weight")->GetDouble());
	ptr->Weight->setEnabled(Active);
}


//------------------------------------------------------------------------------
void QRPromCriterion::Get(RParamStruct* param)
{
	Ui_QRPromCriterion* ptr(static_cast<Ui_QRPromCriterion*>(Ui));
	param->Get<RParamValue>("Active")->SetBool(ptr->Active->isChecked());
	param->Get<RParamValue>("P")->SetDouble(ptr->P->value());
	param->Get<RParamValue>("Q")->SetDouble(ptr->Q->value());
	param->Get<RParamValue>("Weight")->SetDouble(ptr->Weight->value());
}


//------------------------------------------------------------------------------
QRPromCriterion::~QRPromCriterion(void)
{
}
