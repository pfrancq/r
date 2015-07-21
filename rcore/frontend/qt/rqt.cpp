/*

	R Project Library

	RQt.cpp

	Generic function used for Qt compatibility - Implementation.

	Copyright 2004-2015 by Pascal Francq (pascal@francq.info).
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
	New.SetLen(maxlen+10);
	New.SetLen(0);

	RCharCursor Cur(str);
	for(Cur.Start();(!Cur.End())&&maxlen;)
	{
		// Skip Spaces
		if(Cur().IsSpace())
		{
			// Add a space
			if(!New.IsEmpty())
				New+=' ';
			while((!Cur.End())&&Cur().IsSpace())
				Cur.Next();
		}
		if(Cur.End())
			break;

		if(Cur()=='<')
		{
			// Skip HTML codes
			RString Skip;
			int nb(0);
			while((!Cur.End())&&(Cur()!='>'))
			{
				if(nb<6)
					Skip+=Cur();
				Cur.Next();
			}
			Skip.ToLower();
			if(RChar::StrNCmp(Skip(),"<style",6)==0)
			{
				// Skip everything until </style>
				while((!Cur.End())&&(Cur()!='<'))
					Cur.Next();
			}
			else if(!Cur.End())
				Cur.Next();
		}
		else
		{
			maxlen--;
			New+=Cur();
			Cur.Next();
		}
	}

	// If maxlen is not null -> add (...) at the end
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
RString R::ExtractHTMLBody(const QTextEdit* edit)
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


//-----------------------------------------------------------------------------
R::RString R::ToSimpleHTML(const QTextEdit* edit)
{
	if(edit->toPlainText().isEmpty())
		return(RString::Null);
	QString str(edit->toHtml());
	bool Body(false);

	RString New;
	New.SetLen(200);
	New.SetLen(0);

	// Parse the string
	for(int i=0;i<str.size();++i)
	{
		if(str.at(i)=='<')
		{
			bool End(false);
			i++;
			if((i<str.size())&&(str.at(i)=='/'))
			{
				End=true;
				i++;
			}

			// Look for the HTML code
			RString Skip;
			int nb(0);
			while((i<str.size())&&(str.at(i).isLetterOrNumber()))
			{
				if(nb<6)
					Skip+=str.at(i).unicode();
				i++;
			}
			Skip.ToLower();

			// Look what is allowed (b,i,br,p,h1,h2,h3,h4,h5,h6)
			if(Skip=="body")
			{
				Body=!End;

				// NO -> Skip until >
				while((i<str.size())&&(str.at(i)!='>'))
					i++;
			}
			else if((RChar::StrNCmp(Skip(),"h",1)==0)&&(Skip.GetLen()==2))
			{
				if((Skip[1]>='1')&&(Skip[1]<='6'))
				{
					// OK
					if(Body)
					{
						New+='<';
						if(End)
							New+='/';
						New+=Skip+">";
					}
				}

				// Skip until >
				while((i<str.size())&&(str.at(i)!='>'))
					i++;
			}
			else
			{
				if((Skip.Compare("b")==0)||(Skip.Compare("i")==0)||(Skip.Compare("br")==0)||(Skip.Compare("p")==0))
				{
					// OK
					if(Body)
					{
						New+='<';
						if(End)
							New+='/';
						New+=Skip+">";
					}
				}

				// Skip until >
				while((i<str.size())&&(str.at(i)!='>'))
					i++;
			}
		}
		else
		{
			if(Body)
				New+=str.at(i).unicode();
		}
   }

	return(New.Trim());
}