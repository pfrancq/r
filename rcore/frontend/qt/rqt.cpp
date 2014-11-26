/*

	R Project Library

	RQt.cpp

	Generic function used for Qt compatibility - Implementation.

	Copyright 2004-2014 by Pascal Francq (pascal@francq.info).
	Copyright 2004-2008 by the Université Libre de Bruxelles (ULB).

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
// include files for R
#include <rqt.h>
using namespace R;



//------------------------------------------------------------------------------
//
// Generic Functions
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
QString R::ToQString(const RString& str)
{
/*	const RChar* ptr;
	size_t i;
	uint len(static_cast<uint>(str.GetLen()));
	QString ret;

	for(i=0,ptr=str();i<len;ptr++,i++)
		ret+=ptr->Unicode();
	return(ret);*/
	return(QString::fromUtf8(str));
}


//------------------------------------------------------------------------------
RString R::FromQString(const QString& str)
{
	RString ret;
	const RChar* ptr;
	size_t i;
	uint len(str.length());

	ret.SetLen(len);
	for(i=0,ptr=ret();i<len;i++,ptr++)
		(*const_cast<RChar*>(ptr))=str.at(i).unicode();
	return(ret);
}


//-----------------------------------------------------------------------------
QString R::ToPlainText(const R::RString& str,int maxlen)
{
	RString New;
	New.SetLen(110);
	New.SetLen(0);
	RChar Last(0);

	RCharCursor Cur(str);
	for(Cur.Start();(!Cur.End())&&maxlen;Cur.Next())
	{
		// Skip Spaces
		if(Last.IsSpace())
		{
			while((!Cur.End())&&Cur().IsSpace())
				Cur.Next();
		}
		if(Cur.End())
			break;

		if(Cur()=='<')
		{
			// Skip HTML codes
			RChar Skip[6];
			int nb(0);
			while((!Cur.End())&&(Cur()!='>'))
			{
				if(nb<6)
					Skip[nb++]+=Cur();
				Cur.Next();
			}
			if(RChar::StrNCmp(Skip,"<style",6)==0)
			{
				// Skip everything until </style>
				while((!Cur.End())&&(Cur()!='<'))
					Cur.Next();
				if(!Cur.End())
					Cur.Prev();
			}
		}
		else
		{
			maxlen--;
			New+=Cur();
		}
		Last=Cur();
	}

	// If maxlen is null -> add (...) at the end
	if(!maxlen)
		New+=" (...)";

	return(ToQString(New.Trim()));
}


//-----------------------------------------------------------------------------
QString R::ToPlainText(const QString& str,int maxlen)
{
	return(ToPlainText(R::FromQString(str),maxlen));
}


//-----------------------------------------------------------------------------
RString R::ToSimpleHTML(const QTextEdit* edit)
{
	if(edit->toPlainText().isEmpty())
		return(RString::Null);
	QString str(edit->toHtml());
	// Find <body and </body>
	int BeginBody(str.indexOf("<body"));
	int EndBody(str.indexOf("</body>"));
	QString Text("<div"+str.mid(BeginBody+5,EndBody-BeginBody-6)+"</div");
	RString New(FromQString(Text));
	return(New);
}
