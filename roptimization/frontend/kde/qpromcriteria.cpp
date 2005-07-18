/*

	R Project Library

	QPromCriteria.cpp

	Qt Widget to configure a list of Promethee Criterion - Implementation.

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
// include files for R Project
#include <rpromcriterion.h>
#include <qpromcriteria.h>
using namespace R;


//------------------------------------------------------------------------------
// include files for Qt
#include <qlabel.h>
#include <qlineedit.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qvalidator.h>
#include <qcombobox.h>
#include <qlayout.h>


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
QPromCriteria::QPromCriteria(QWidget* parent,const char* name)
	: QGroupBox(parent,name), Current(-1), Crits(10,5)
{
	setTitle(name);

	QVBoxLayout* WLayout= new QVBoxLayout(this,5,5,"Promethee Criteria");
	WLayout->addSpacing(10);

	QHBoxLayout* Layout2 = new QHBoxLayout(0,0,0);
	Layout2->addSpacing(5);
	Direct=new QComboBox(this,"Direct");
	QToolTip::add(Direct,i18n("Select the criterion to configure."));
	QWhatsThis::add(Direct,i18n("With the combo box, the user chooses the criterion to configure below."));
	Layout2->addWidget(Direct);
	Layout2->addSpacing(5);
	WLayout->addLayout(Layout2);
	WLayout->addSpacing(5);

	QHBoxLayout* Layout1 = new QHBoxLayout(0,0,0,"Promethee Layout");
	Layout1->addSpacing(5);

	// Parameter "P".
	QLabel* txtP=new QLabel(this,"txtP");
	txtP->setText(i18n("p"));
	Layout1->addWidget(txtP);
	Layout1->addSpacing(5);
	P = new QLineEdit(this,"Preference threshold");
	P->setValidator(new QDoubleValidator(0.0,1.0,3,P));
	Layout1->addWidget(P);
	Layout1->addSpacing(10);
	QToolTip::add(P,i18n("Set the preference threshold for the criterion"));
	QWhatsThis::add(P,i18n("When the relativ difference for two solutions for the criterion"
	                       " is greather than <b>p</p>, the best solution is prefered."));
	Layout1->addItem(new QSpacerItem(10,0,QSizePolicy::Expanding, QSizePolicy::Minimum));

	// Parameter "Q".
	QLabel* txtQ=new QLabel(this,"txtQ");
	txtQ->setText(i18n("q"));
	Layout1->addWidget(txtQ);
	Layout1->addSpacing(5);
	Q = new QLineEdit(this,"Indeference threshold");
	Q->setValidator(new QDoubleValidator(0.0,1.0,3,Q));
	Layout1->addWidget(Q);
	Layout1->addSpacing(10);
	QToolTip::add(Q,i18n("Set the indeference threshold for the criterion"));
	QWhatsThis::add(Q,i18n("When the relativ difference for two solutions for the criterion"
	                       " is smaller than <b>q</p>, the two solutions are considered to be similar."));
	Layout1->addItem(new QSpacerItem(10,0,QSizePolicy::Expanding, QSizePolicy::Minimum));

	// Parameter Weight.
	QLabel* txtW=new QLabel(this,"txtW");
	txtW->setText(i18n("W"));
	Layout1->addWidget(txtW);
	Layout1->addSpacing(5);
	W = new QLineEdit(this,"W");
	Layout1->addWidget(W);
	Layout1->addSpacing(5);
	QToolTip::add(W,i18n("Set the weight for the criterion"));
	QWhatsThis::add(W,i18n("The <b>weight</b> for the criterion"
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

	ptr=new QPromCriterion(crit,Crits.GetNb());
	Direct->insertItem(crit,Crits.GetNb());
	Crits.InsertPtr(ptr);
	Current=Crits.GetNb()-1;
}


//------------------------------------------------------------------------------
void QPromCriteria::SetCriterionParam(const char* crit,double p,double q,double w)
{
	QPromCriterion* ptr;
	QString tmp;

	ptr=Crits.GetPtr<const char*>(crit);
	if(!ptr) return;
/*	tmp.setNum(p);
	ptr->P->setText(tmp);
	tmp.setNum(q);
	ptr->Q->setText(tmp);
	tmp.setNum(w);
	ptr->W->setText(tmp);*/
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
void QPromCriteria::SetCriterionParam(const char* crit,RPromCriterionParams& p)
{
	QPromCriterion* ptr;
	QString tmp;

	ptr=Crits.GetPtr<const char*>(crit);
	if(!ptr) return;
/*	tmp.setNum(p.P);
	ptr->P->setText(tmp);
	tmp.setNum(p.Q);
	ptr->Q->setText(tmp);
	tmp.setNum(p.Weight);
	ptr->W->setText(tmp);*/
	ptr->P=p.P;
	ptr->Q=p.Q;
	ptr->W=p.Weight;
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
/*	d=ptr->P->text().toDouble();
	if((d>=0.0)&&(d<=1.0))
		p=d;
	d=ptr->Q->text().toDouble();
	if((d>=0.0)&&(d<=1.0))
		q=d;
	w=ptr->W->text().toDouble();*/
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
void QPromCriteria::GetCriterionParam(const char* crit,RPromCriterionParams& p)
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
	p.P=ptr->P;
	p.Q=ptr->Q;
	p.Weight=ptr->W;
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
