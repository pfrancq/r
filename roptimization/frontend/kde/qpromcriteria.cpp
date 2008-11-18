/*

	R Project Library

	QPromCriteria.cpp

	Qt Widget to configure a list of Promethee Criterion - Implementation.

	Copyright 2002-2008 by the Université Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).

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
// include files for R Project
#include <rpromcriterion.h>
#include <qpromcriteria.h>
using namespace R;


//------------------------------------------------------------------------------
// include files for Qt
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QToolTip>
#include <QtGui/QWhatsThis>
#include <QtGui/QValidator>
#include <QtGui/QComboBox>
#include <QtGui/QLayout>


//------------------------------------------------------------------------------
// include files for KDE
#include <klocale.h>



//------------------------------------------------------------------------------
//
// class QPromCriterion
//
//------------------------------------------------------------------------------

class QPromCriteria::QPromCriterion
{
public:
	RString Name;
	double P;
	double Q;
	double W;
	int Idx;

	QPromCriterion(const char* name,int idx) : Name(name), Idx(idx) {}
	int Compare(const QPromCriterion& crit) const {return(Name.Compare(crit.Name));}
	int Compare(const QPromCriterion* crit) const {return(Name.Compare(crit->Name));}
	int Compare(const RString& name) const {return(Name.Compare(name));}
	int Compare(const char* name) const {return(Name.Compare(name));}
};



//------------------------------------------------------------------------------
//
// class QPromCriteria
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
QPromCriteria::QPromCriteria(QWidget* parent, QString& name)
	: QGroupBox(parent), Current(-1), Crits(10,5)
{
	setTitle(name);

	QVBoxLayout* WLayout= new QVBoxLayout(this);
	WLayout->addSpacing(10);

	QHBoxLayout* Layout2 = new QHBoxLayout();
	Layout2->addSpacing(5);
	Direct=new QComboBox(this);
	Direct->setToolTip(i18n("Select the criterion to configure."));
	Direct->setWhatsThis(i18n("With the combo box, the user chooses the criterion to configure below."));
	Layout2->addWidget(Direct);
	Layout2->addSpacing(5);
	WLayout->addLayout(Layout2);
	WLayout->addSpacing(5);

	QHBoxLayout* Layout1 = new QHBoxLayout();
	Layout1->addSpacing(5);

	// Parameter "P".
	QLabel* txtP=new QLabel(this);
	txtP->setText(i18n("p"));
	Layout1->addWidget(txtP);
	Layout1->addSpacing(5);
	P = new QLineEdit(this);
	P->setValidator(new QDoubleValidator(0.0,1.0,3,P));
	Layout1->addWidget(P);
	Layout1->addSpacing(10);
	P->setToolTip(i18n("Set the preference threshold for the criterion"));
	P->setWhatsThis(i18n("When the relativ difference for two solutions for the criterion"
	                       " is greather than <b>p</p>, the best solution is prefered."));
	Layout1->addItem(new QSpacerItem(10,0,QSizePolicy::Expanding, QSizePolicy::Minimum));

	// Parameter "Q".
	QLabel* txtQ=new QLabel(this);
	txtQ->setText(i18n("q"));
	Layout1->addWidget(txtQ);
	Layout1->addSpacing(5);
	Q = new QLineEdit(this);
	Q->setValidator(new QDoubleValidator(0.0,1.0,3,Q));
	Layout1->addWidget(Q);
	Layout1->addSpacing(10);
	Q->setToolTip(i18n("Set the indeference threshold for the criterion"));
	Q->setWhatsThis(i18n("When the relativ difference for two solutions for the criterion"
	                       " is smaller than <b>q</p>, the two solutions are considered to be similar."));
	Layout1->addItem(new QSpacerItem(10,0,QSizePolicy::Expanding, QSizePolicy::Minimum));

	// Parameter Weight.
	QLabel* txtW=new QLabel(this);
	txtW->setText(i18n("W"));
	Layout1->addWidget(txtW);
	Layout1->addSpacing(5);
	W = new QLineEdit(this);
	Layout1->addWidget(W);
	Layout1->addSpacing(5);
	W->setToolTip(i18n("Set the weight for the criterion"));
	W->setWhatsThis(i18n("The <b>weight</b> for the criterion"
	                       " specify its importance in comparison to the others criteria."));
	WLayout->addLayout(Layout1);
	WLayout->addItem(new QSpacerItem(0,0,QSizePolicy::Minimum, QSizePolicy::Expanding));

	connect(P,SIGNAL(textChanged(const QString&)),this,SLOT(slotChanged()));
	connect(Q,SIGNAL(textChanged(const QString&)),this,SLOT(slotChanged()));
	connect(W,SIGNAL(textChanged(const QString&)),this,SLOT(slotChanged()));

	connect(Direct,SIGNAL(activated(int)),this,SLOT(slotActivated(int)));
}


//------------------------------------------------------------------------------
void QPromCriteria::InsertCriterion(const char* crit)
{
	QPromCriterion* ptr;

	ptr=new QPromCriterion(crit,static_cast<int>(Crits.GetNb()));
	Direct->addItem(crit,static_cast<int>(Crits.GetNb()));
	Crits.InsertPtr(ptr);
	Current=static_cast<int>(Crits.GetNb()-1);
}


//------------------------------------------------------------------------------
void QPromCriteria::SetCriterionParam(const char* crit,double p,double q,double w)
{
	QPromCriterion* ptr;
	QString tmp;

	ptr=Crits.GetPtr<const char*>(crit);
	if(!ptr) return;
	ptr->P=p;
	ptr->Q=q;
	ptr->W=w;
	if(ptr->Idx==Current)
	{
		tmp.setNum(ptr->P);
		P->setText(tmp);
		tmp.setNum(ptr->Q);
		Q->setText(tmp);
		tmp.setNum(ptr->W);
		W->setText(tmp);
	}
}


//------------------------------------------------------------------------------
void QPromCriteria::SetCriterionParam(const char* crit,RParam* p)
{
	QPromCriterion* ptr;
	QString tmp;

	ptr=Crits.GetPtr(crit);
	if(!ptr) return;
	ptr->P=dynamic_cast<RParamStruct*>(p)->Get<RParamValue>("P")->GetDouble();
	ptr->Q=dynamic_cast<RParamStruct*>(p)->Get<RParamValue>("Q")->GetDouble();
	ptr->W=dynamic_cast<RParamStruct*>(p)->Get<RParamValue>("Weight")->GetDouble();
	if(ptr->Idx==Current)
	{
		tmp.setNum(ptr->P);
		P->setText(tmp);
		tmp.setNum(ptr->Q);
		Q->setText(tmp);
		tmp.setNum(ptr->W);
		W->setText(tmp);
	}
}


//------------------------------------------------------------------------------
void QPromCriteria::GetCriterionParam(const char* crit,double& p,double& q,double& w)
{
	QPromCriterion* ptr;
	double d;

	ptr=Crits.GetPtr<const char*>(crit);
	if(!ptr) return;
	if(ptr->Idx==Current)
	{
		d=P->text().toDouble();
		if((d>=0.0)&&(d<=1.0))
			ptr->P=d;
		d=Q->text().toDouble();
		if((d>=0.0)&&(d<=1.0))
			ptr->Q=d;
		ptr->W=W->text().toDouble();
	}
	p=ptr->P;
	q=ptr->Q;
	w=ptr->W;

}


//------------------------------------------------------------------------------
void QPromCriteria::GetCriterionParam(const char* crit,RParam* p)
{
	QPromCriterion* ptr;
	double d;

	ptr=Crits.GetPtr<const char*>(crit);
	if(!ptr) return;
	if(ptr->Idx==Current)
	{
		d=P->text().toDouble();
		if((d>=0.0)&&(d<=1.0))
			ptr->P=d;
		d=Q->text().toDouble();
		if((d>=0.0)&&(d<=1.0))
			ptr->Q=d;
		ptr->W=W->text().toDouble();
	}
	dynamic_cast<RParamStruct*>(p)->Get<RParamValue>("P")->SetDouble(ptr->P);
	dynamic_cast<RParamStruct*>(p)->Get<RParamValue>("Q")->SetDouble(ptr->Q);
	dynamic_cast<RParamStruct*>(p)->Get<RParamValue>("Weight")->SetDouble(ptr->W);
}


//------------------------------------------------------------------------------
void QPromCriteria::slotActivated(int index)
{
	QString tmp;
	QPromCriterion* ptr;
	double d;

	if(Current!=-1)
	{
		ptr=Crits[Current];
		d=P->text().toDouble();
		if((d>=0.0)&&(d<=1.0))
			ptr->P=d;
		d=Q->text().toDouble();
		if((d>=0.0)&&(d<=1.0))
			ptr->Q=d;
		ptr->W=W->text().toDouble();
	}
	Current=index;
	ptr=Crits[Current];
	tmp.setNum(ptr->P);
	P->setText(tmp);
	tmp.setNum(ptr->Q);
	Q->setText(tmp);
	tmp.setNum(ptr->W);
	W->setText(tmp);
}


//------------------------------------------------------------------------------
void QPromCriteria::slotChanged(void)
{
	emit valueChanged();
}


//------------------------------------------------------------------------------
QPromCriteria::~QPromCriteria(void)
{
}
