/*

	R Project Library

	RXMLParser.cpp

	XML parser - Implementation.

	Copyright 2000-2012 by Pascal Francq (pascal@francq.info).
	Copyright 2000-2008 by the Université Libre de Bruxelles (ULB).

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
// include files for R Project
#include <rxmlparser.h>
#include <rcursor.h>
#include <rxmlattr.h>
#include <rstringbuilder.h>
using namespace R;
using namespace std;


//------------------------------------------------------------------------------
// HTML codes
struct HTMLCode
{
	RString Code;
	RChar Car;

	HTMLCode(const RString& code,const unsigned int car) : Code(code), Car(car) {}
	int Compare(const HTMLCode& code) const {return(Code.Compare(code.Code));}
	int Compare(const RString& code) const {return(Code.Compare(code));}
};
RContainer<HTMLCode,true,true> TheHTMLCodes(270);


//------------------------------------------------------------------------------
// Skip ending '/' if necessary
RString AdaptNamespace(const RString& name)
{
	if((name.GetLen())&&(name[name.GetLen()-1]=='/'))
	{
		return(name.Mid(0,name.GetLen()-1));
	}
	else
		return(name);
}



//------------------------------------------------------------------------------
//
// class RXMLParser::Attribute
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
class RXMLParser::Attribute
{
public:
	RString Name;
	RContainer<RString,true,false> Values;
	size_t NbValues;

	Attribute(void) : Values(20) {}
	int Compare(const Attribute&) const {return(-1);}
};


//------------------------------------------------------------------------------
//
// class RXMLParser::Namespace
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
class RXMLParser::Namespace
{
public:
	RString Prefix;
	RStack<RString,true,true,true> URI;

	Namespace(const RString& prefix) : Prefix(prefix), URI(5)
	{
	}
	int Compare(const Namespace& sp) const {return(Prefix.Compare(sp.Prefix));}
	int Compare(const Namespace* sp) const {return(Prefix.Compare(sp->Prefix));}
	int Compare(const RString& sp) const {return(Prefix.Compare(sp));}
};


//------------------------------------------------------------------------------
//
// class RXMLParser::Tag
//
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
RXMLParser::HTMLTag::HTMLTag(const RString& name,int level,bool single,bool self)
		: Name(name), Level(level), Single(single), SelfContained(self)
{
}


//------------------------------------------------------------------------------
int RXMLParser::HTMLTag::Compare(const RXMLParser::HTMLTag& t) const
{
	return(Name.Compare(t.Name));
}


//------------------------------------------------------------------------------
int RXMLParser::HTMLTag::Compare(const RString& t) const
{
	return(Name.Compare(t));
}



//------------------------------------------------------------------------------
// Global variables
RContainer<RXMLParser::HTMLTag,true,true> RXMLParser::Tags(200,10);



//------------------------------------------------------------------------------
//
// class RXMLParser
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
RXMLParser::RXMLParser(void)
	: RTextFile(), Namespaces(20), DefaultNamespace(5), Attributes(20),
	  AvoidSpaces(false), HTMLCodes(false), HTMLMode(false)
{
	SetRemStyle(MultiLineComment);
	SetRem("<!--","-->");
	if(!Tags.GetNb())
		InitValidTags();
}


//------------------------------------------------------------------------------
RXMLParser::RXMLParser(const RURI& uri,const RCString& encoding)
 : RTextFile(uri,encoding), Namespaces(20), DefaultNamespace(5), Attributes(20),
   AvoidSpaces(false), HTMLCodes(false), HTMLMode(false)
{
	SetRemStyle(MultiLineComment);
	SetRem("<!--","-->");
	if(!Tags.GetNb())
		InitValidTags();
}


//------------------------------------------------------------------------------
RXMLParser::RXMLParser(RIOFile& file,const RCString& encoding)
 : RTextFile(file,encoding), Namespaces(20), DefaultNamespace(5), Attributes(20),
   AvoidSpaces(false), HTMLCodes(false), HTMLMode(false)
{
	SetRemStyle(MultiLineComment);
	SetRem("<!--","-->");
	if(!Tags.GetNb())
		InitValidTags();
}


//------------------------------------------------------------------------------
void RXMLParser::InitValidTags(void)
{
	// Global Tags
	Tags.InsertPtr(new HTMLTag("html",0,false));
	Tags.InsertPtr(new HTMLTag("body",10,false));
	Tags.InsertPtr(new HTMLTag("head",10,false));

	// Heading Tags
	Tags.InsertPtr(new HTMLTag("meta",500,true));
	Tags.InsertPtr(new HTMLTag("title",500,false));
	Tags.InsertPtr(new HTMLTag("base",500,false));
	Tags.InsertPtr(new HTMLTag("link",500,true));
	Tags.InsertPtr(new HTMLTag("style",500,false));

	// Division Tags
	Tags.InsertPtr(new HTMLTag("div",150,false,true));
	Tags.InsertPtr(new HTMLTag("span",160,false,true));

	// Frame
	Tags.InsertPtr(new HTMLTag("frameset",100,false));
	Tags.InsertPtr(new HTMLTag("frame",360,false));
	Tags.InsertPtr(new HTMLTag("noframe",360,false));
	Tags.InsertPtr(new HTMLTag("iframe",360,false));

	//info to contact author
	Tags.InsertPtr(new HTMLTag("address",360,false));

	// Forms
	Tags.InsertPtr(new HTMLTag("form",360,false));
	Tags.InsertPtr(new HTMLTag("input",500,false));
	Tags.InsertPtr(new HTMLTag("button",500,false));
	Tags.InsertPtr(new HTMLTag("label",500,false));
	Tags.InsertPtr(new HTMLTag("select",500,false));
	Tags.InsertPtr(new HTMLTag("option",500,false));
	Tags.InsertPtr(new HTMLTag("optgroup",500,false));
	Tags.InsertPtr(new HTMLTag("textarea",500,false));
	Tags.InsertPtr(new HTMLTag("isindex",500,false));
	Tags.InsertPtr(new HTMLTag("fieldset",500,false));
	Tags.InsertPtr(new HTMLTag("legend",500,false));

	// Structure Tags
	Tags.InsertPtr(new HTMLTag("table",200,false));
	Tags.InsertPtr(new HTMLTag("caption",210,false));
	Tags.InsertPtr(new HTMLTag("thead",210,false));
	Tags.InsertPtr(new HTMLTag("tfoot",210,false));
	Tags.InsertPtr(new HTMLTag("tbody",210,false));
	Tags.InsertPtr(new HTMLTag("tr",220,false));
	Tags.InsertPtr(new HTMLTag("th",220,false));
	Tags.InsertPtr(new HTMLTag("td",230,false));
	Tags.InsertPtr(new HTMLTag("ul",240,false));
	Tags.InsertPtr(new HTMLTag("ol",240,false));
	Tags.InsertPtr(new HTMLTag("dl",240,false));
	Tags.InsertPtr(new HTMLTag("li",250,false));
	Tags.InsertPtr(new HTMLTag("dt",250,false));
	Tags.InsertPtr(new HTMLTag("dd",250,false));
	Tags.InsertPtr(new HTMLTag("colgroup",500,false));
	Tags.InsertPtr(new HTMLTag("col",500,false));


	// Paragraph Tag
	Tags.InsertPtr(new HTMLTag("h1",300,false));
	Tags.InsertPtr(new HTMLTag("h2",310,false));
	Tags.InsertPtr(new HTMLTag("h3",320,false));
	Tags.InsertPtr(new HTMLTag("h4",330,false));
	Tags.InsertPtr(new HTMLTag("h5",340,false));
	Tags.InsertPtr(new HTMLTag("h6",350,false));
	Tags.InsertPtr(new HTMLTag("p",360,false));


	// Low level tags
	Tags.InsertPtr(new HTMLTag("sup",500,false));
	Tags.InsertPtr(new HTMLTag("sub",500,false));
	Tags.InsertPtr(new HTMLTag("br",500,true));
	Tags.InsertPtr(new HTMLTag("nobr",500,false));
	Tags.InsertPtr(new HTMLTag("hr",500,true));
	Tags.InsertPtr(new HTMLTag("font",500,false));
	Tags.InsertPtr(new HTMLTag("pre",500,false));
	Tags.InsertPtr(new HTMLTag("img",500,true)); //? Depreciated
	Tags.InsertPtr(new HTMLTag("object",500,false));
	Tags.InsertPtr(new HTMLTag("param",500,false));
	Tags.InsertPtr(new HTMLTag("embed",500,false));
	Tags.InsertPtr(new HTMLTag("noembed",500,false));
	Tags.InsertPtr(new HTMLTag("map",500,false));
	Tags.InsertPtr(new HTMLTag("area",500,false));
	Tags.InsertPtr(new HTMLTag("center",500,false));
	Tags.InsertPtr(new HTMLTag("em",500,false));
	Tags.InsertPtr(new HTMLTag("strong",500,false));
	Tags.InsertPtr(new HTMLTag("dfn",500,false));
	Tags.InsertPtr(new HTMLTag("code",500,false));
	Tags.InsertPtr(new HTMLTag("samp",500,false));
	Tags.InsertPtr(new HTMLTag("kbd",500,false));
	Tags.InsertPtr(new HTMLTag("var",500,false));
	Tags.InsertPtr(new HTMLTag("cite",500,false));
	Tags.InsertPtr(new HTMLTag("abbr",500,false));
	Tags.InsertPtr(new HTMLTag("acronym",500,false));
	Tags.InsertPtr(new HTMLTag("i",500,false));
	Tags.InsertPtr(new HTMLTag("tt",500,false));
	Tags.InsertPtr(new HTMLTag("big",500,false));
	Tags.InsertPtr(new HTMLTag("small",500,false));
	Tags.InsertPtr(new HTMLTag("strike",500,false));
	Tags.InsertPtr(new HTMLTag("b",500,false));
	Tags.InsertPtr(new HTMLTag("u",500,false));  //Depreciated
	Tags.InsertPtr(new HTMLTag("s",500,false));  //Depreciated
	Tags.InsertPtr(new HTMLTag("applet",500,false));  //Depreciated
	Tags.InsertPtr(new HTMLTag("a",500,false));
	Tags.InsertPtr(new HTMLTag("blockquote",500,false));
	Tags.InsertPtr(new HTMLTag("q",500,false));
	Tags.InsertPtr(new HTMLTag("script",500,false));
	Tags.InsertPtr(new HTMLTag("noscript",500,false));
}


//------------------------------------------------------------------------------
void RXMLParser::StopAnalysis(void)
{
	Break=true;
}


//------------------------------------------------------------------------------
size_t RXMLParser::GetCurrentDepth(void) const
{
	if(Mode!=RIO::Read)
		ThrowRIOException(this,"File not in read mode");
	return(CurDepth);
}


//------------------------------------------------------------------------------
size_t RXMLParser::GetLastTokenPos(void) const
{
	if(Mode!=RIO::Read)
		ThrowRIOException(this,"File not in read mode");
	return(LastTokenPos);
}


//------------------------------------------------------------------------------
void RXMLParser::InitParser(void)
{
	Break=false;
	TreatEncoding=false;
	CurHTMLTag=0;
	Namespaces.Clear();
	DefaultNamespace.Clear();
	Namespace* ptr=Namespaces.GetInsertPtr("xmlns");
	ptr->URI.Push(new RString("http://www.w3.org/2000/xmlns"));
	FoundClosingHTML=false;
	if(HTMLMode)
	{
		Namespace* ptr=Namespaces.GetInsertPtr("xml");
		ptr->URI.Push(new RString("http://www.w3.org/1999/xhtml"));
	}
	else
	{
		Namespace* ptr=Namespaces.GetInsertPtr("xml");
		ptr->URI.Push(new RString("http://www.w3.org/XML/1998/namespace"));
	}
}


//------------------------------------------------------------------------------
void RXMLParser::SetHTMLMode(bool html)
{
	HTMLMode=html;
	SetAcceptHTMLCodes(html);
}


//------------------------------------------------------------------------------
void RXMLParser::Open(RIO::ModeType mode)
{
	RTextFile::Open(mode);

	// Create default namespaces
	InitParser();

	// Treat the different modes
	switch(Mode)
	{
		case RIO::Read:
			CurDepth=0;
			LoadHeader();
			if(Break)
				return;
			TreatEncoding=false;
			Section=Body;
			LoadNextTag();
			if(Break)
				return;
			CurDepth=0;
			break;

		case RIO::Append:
		case RIO::Create:
			ThrowRIOException(this,"A XML Parser is read-only");
			break;

		default:
			ThrowRIOException(this,"No Valid Mode");
	};
}


//------------------------------------------------------------------------------
void RXMLParser::Open(const RURI& uri,RIO::ModeType mode,const RCString& encoding)
{
	RTextFile::Open(uri,mode,encoding);
}


//------------------------------------------------------------------------------
RChar RXMLParser::CodeToChar(const RString& str,bool html)
{
	if(!str.Compare("quot"))
		return('"');
	if(!str.Compare("amp"))
		return('&');
	if(!str.Compare("apos"))
		return('\'');
	if(!str.Compare("lt"))
		return('<');
	if(!str.Compare("gt"))
		return('>');
	if(str[0]=='#')
	{
		RString Code(str.Mid(1));
		bool Valid;
		unsigned int Num(Code.ToUInt(Valid));
		if(Valid)
			return(RChar(Num));
		if(html)
		{
			if(!TheHTMLCodes.GetNb())
			{
				// Insert all codes
				TheHTMLCodes.InsertPtr(new HTMLCode("AElig",0x00C6));
				TheHTMLCodes.InsertPtr(new HTMLCode("Aacute",0x00C1));
				TheHTMLCodes.InsertPtr(new HTMLCode("Acirc",0x00C2));
				TheHTMLCodes.InsertPtr(new HTMLCode("Agrave",0x00C0));
				TheHTMLCodes.InsertPtr(new HTMLCode("Alpha",0x0391));
				TheHTMLCodes.InsertPtr(new HTMLCode("Aring",0x00C5));
				TheHTMLCodes.InsertPtr(new HTMLCode("Atilde",0x00C3));
				TheHTMLCodes.InsertPtr(new HTMLCode("Auml",0x00C4));
				TheHTMLCodes.InsertPtr(new HTMLCode("Beta",0x0392));
				TheHTMLCodes.InsertPtr(new HTMLCode("Ccedil",0x00C7));
				TheHTMLCodes.InsertPtr(new HTMLCode("Chi",0x03A7));
				TheHTMLCodes.InsertPtr(new HTMLCode("Dagger",0x2021));
				TheHTMLCodes.InsertPtr(new HTMLCode("Delta",0x0394));
				TheHTMLCodes.InsertPtr(new HTMLCode("ETH",0x00D0));
				TheHTMLCodes.InsertPtr(new HTMLCode("Eacute",0x00C9));
				TheHTMLCodes.InsertPtr(new HTMLCode("Ecirc",0x00CA));
				TheHTMLCodes.InsertPtr(new HTMLCode("Egrave",0x00C8));
				TheHTMLCodes.InsertPtr(new HTMLCode("Epsilon",0x0395));
				TheHTMLCodes.InsertPtr(new HTMLCode("Eta",0x0397));
				TheHTMLCodes.InsertPtr(new HTMLCode("Euml",0x00CB));
				TheHTMLCodes.InsertPtr(new HTMLCode("Gamma",0x0393));
				TheHTMLCodes.InsertPtr(new HTMLCode("Iacute",0x00CD));
				TheHTMLCodes.InsertPtr(new HTMLCode("Icirc",0x00CE));
				TheHTMLCodes.InsertPtr(new HTMLCode("Igrave",0x00CC));
				TheHTMLCodes.InsertPtr(new HTMLCode("Iota",0x0399));
				TheHTMLCodes.InsertPtr(new HTMLCode("Iuml",0x00CF));
				TheHTMLCodes.InsertPtr(new HTMLCode("Kappa",0x039A));
				TheHTMLCodes.InsertPtr(new HTMLCode("Lambda",0x039B));
				TheHTMLCodes.InsertPtr(new HTMLCode("Mu",0x039C));
				TheHTMLCodes.InsertPtr(new HTMLCode("Ntilde",0x00D1));
				TheHTMLCodes.InsertPtr(new HTMLCode("Nu",0x039D));
				TheHTMLCodes.InsertPtr(new HTMLCode("OElig",0x0152));
				TheHTMLCodes.InsertPtr(new HTMLCode("Oacute",0x00D3));
				TheHTMLCodes.InsertPtr(new HTMLCode("Ocirc",0x00D4));
				TheHTMLCodes.InsertPtr(new HTMLCode("Ograve",0x00D2));
				TheHTMLCodes.InsertPtr(new HTMLCode("Omega",0x03A9));
				TheHTMLCodes.InsertPtr(new HTMLCode("Omicron",0x039F));
				TheHTMLCodes.InsertPtr(new HTMLCode("Oslash",0x00D8));
				TheHTMLCodes.InsertPtr(new HTMLCode("Otilde",0x00D5));
				TheHTMLCodes.InsertPtr(new HTMLCode("Ouml",0x00D6));
				TheHTMLCodes.InsertPtr(new HTMLCode("Phi",0x03A6));
				TheHTMLCodes.InsertPtr(new HTMLCode("Pi",0x03A0));
				TheHTMLCodes.InsertPtr(new HTMLCode("Prime",0x2033));
				TheHTMLCodes.InsertPtr(new HTMLCode("Psi",0x03A8));
				TheHTMLCodes.InsertPtr(new HTMLCode("Rho",0x03A1));
				TheHTMLCodes.InsertPtr(new HTMLCode("Scaron",0x0160));
				TheHTMLCodes.InsertPtr(new HTMLCode("Sigma",0x03A3));
				TheHTMLCodes.InsertPtr(new HTMLCode("THORN",0x00DE));
				TheHTMLCodes.InsertPtr(new HTMLCode("Tau",0x03A4));
				TheHTMLCodes.InsertPtr(new HTMLCode("Theta",0x0398));
				TheHTMLCodes.InsertPtr(new HTMLCode("Uacute",0x00DA));
				TheHTMLCodes.InsertPtr(new HTMLCode("Ucirc",0x00DB));
				TheHTMLCodes.InsertPtr(new HTMLCode("Ugrave",0x00D9));
				TheHTMLCodes.InsertPtr(new HTMLCode("Upsilon",0x03A5));
				TheHTMLCodes.InsertPtr(new HTMLCode("Uuml",0x00DC));
				TheHTMLCodes.InsertPtr(new HTMLCode("Xi",0x039E));
				TheHTMLCodes.InsertPtr(new HTMLCode("Yacute",0x00DD));
				TheHTMLCodes.InsertPtr(new HTMLCode("Yuml",0x0178));
				TheHTMLCodes.InsertPtr(new HTMLCode("Zeta",0x0396));
				TheHTMLCodes.InsertPtr(new HTMLCode("aacute",0x00E1));
				TheHTMLCodes.InsertPtr(new HTMLCode("acirc",0x00E2));
				TheHTMLCodes.InsertPtr(new HTMLCode("acute",0x00E1));
				TheHTMLCodes.InsertPtr(new HTMLCode("aelig",0x00E6));
				TheHTMLCodes.InsertPtr(new HTMLCode("agrave",0x00E0));
				TheHTMLCodes.InsertPtr(new HTMLCode("alefsym",0x2135));
				TheHTMLCodes.InsertPtr(new HTMLCode("alpha",0x03B1));
				TheHTMLCodes.InsertPtr(new HTMLCode("amp",0x0026));
				TheHTMLCodes.InsertPtr(new HTMLCode("and",0x2227));
				TheHTMLCodes.InsertPtr(new HTMLCode("ang",0x2220));
				TheHTMLCodes.InsertPtr(new HTMLCode("apos",0x0027));
				TheHTMLCodes.InsertPtr(new HTMLCode("aring",0x00E5));
				TheHTMLCodes.InsertPtr(new HTMLCode("asymp",0x2248));
				TheHTMLCodes.InsertPtr(new HTMLCode("atilde",0x00E3));
				TheHTMLCodes.InsertPtr(new HTMLCode("auml",0x00E4));
				TheHTMLCodes.InsertPtr(new HTMLCode("bdquo",0x201E));
				TheHTMLCodes.InsertPtr(new HTMLCode("beta",0x03B2));
				TheHTMLCodes.InsertPtr(new HTMLCode("brvbar",0x00A6));
				TheHTMLCodes.InsertPtr(new HTMLCode("bull",0x2022));
				TheHTMLCodes.InsertPtr(new HTMLCode("cap",0x2229));
				TheHTMLCodes.InsertPtr(new HTMLCode("ccedil",0x00E7));
				TheHTMLCodes.InsertPtr(new HTMLCode("cedil",0x00B8));
				TheHTMLCodes.InsertPtr(new HTMLCode("cent",0x00A2));
				TheHTMLCodes.InsertPtr(new HTMLCode("chi",0x03C7));
				TheHTMLCodes.InsertPtr(new HTMLCode("circ",0x02C6));
				TheHTMLCodes.InsertPtr(new HTMLCode("clubs",0x2663));
				TheHTMLCodes.InsertPtr(new HTMLCode("cong",0x2245));
				TheHTMLCodes.InsertPtr(new HTMLCode("copy",0x00A9));
				TheHTMLCodes.InsertPtr(new HTMLCode("crarr",0x21B5));
				TheHTMLCodes.InsertPtr(new HTMLCode("cup",0x222A));
				TheHTMLCodes.InsertPtr(new HTMLCode("curren",0x00A4));
				TheHTMLCodes.InsertPtr(new HTMLCode("dArr",0x21D3));
				TheHTMLCodes.InsertPtr(new HTMLCode("dagger",0x2020));
				TheHTMLCodes.InsertPtr(new HTMLCode("darr",0x2193));
				TheHTMLCodes.InsertPtr(new HTMLCode("deg",0x00B0));
				TheHTMLCodes.InsertPtr(new HTMLCode("delta",0x03B4));
				TheHTMLCodes.InsertPtr(new HTMLCode("diams",0x2666));
				TheHTMLCodes.InsertPtr(new HTMLCode("divide",0x00F7));
				TheHTMLCodes.InsertPtr(new HTMLCode("eacute",0x00E9));
				TheHTMLCodes.InsertPtr(new HTMLCode("ecirc",0x00EA));
				TheHTMLCodes.InsertPtr(new HTMLCode("egrave",0x00E8));
				TheHTMLCodes.InsertPtr(new HTMLCode("empty",0x2205));
				TheHTMLCodes.InsertPtr(new HTMLCode("emsp",0x2003));
				TheHTMLCodes.InsertPtr(new HTMLCode("ensp",0x2002));
				TheHTMLCodes.InsertPtr(new HTMLCode("epsilon",0x03B5));
				TheHTMLCodes.InsertPtr(new HTMLCode("equiv",0x2261));
				TheHTMLCodes.InsertPtr(new HTMLCode("eta",0x03B7));
				TheHTMLCodes.InsertPtr(new HTMLCode("eth",0x00F0));
				TheHTMLCodes.InsertPtr(new HTMLCode("euml",0x00E4));
				TheHTMLCodes.InsertPtr(new HTMLCode("euro",0x20AC));
				TheHTMLCodes.InsertPtr(new HTMLCode("exist",0x2203));
				TheHTMLCodes.InsertPtr(new HTMLCode("forall",0x2200));
				TheHTMLCodes.InsertPtr(new HTMLCode("frac12",0x00BD));
				TheHTMLCodes.InsertPtr(new HTMLCode("frac14",0x00BC));
				TheHTMLCodes.InsertPtr(new HTMLCode("frac34",0x00B3));
				TheHTMLCodes.InsertPtr(new HTMLCode("frasl",0x002F));
				TheHTMLCodes.InsertPtr(new HTMLCode("gamma",0x03B3));
				TheHTMLCodes.InsertPtr(new HTMLCode("ge",0x2265));
				TheHTMLCodes.InsertPtr(new HTMLCode("gt",0x003E));
				TheHTMLCodes.InsertPtr(new HTMLCode("hArr",0x21D4));
				TheHTMLCodes.InsertPtr(new HTMLCode("harr",0x2194));
				TheHTMLCodes.InsertPtr(new HTMLCode("hearts",0x2665));
				TheHTMLCodes.InsertPtr(new HTMLCode("hellip",0x2026));
				TheHTMLCodes.InsertPtr(new HTMLCode("iacute",0x00ED));
				TheHTMLCodes.InsertPtr(new HTMLCode("icirc",0x00EE));
				TheHTMLCodes.InsertPtr(new HTMLCode("iexcl",0x00A1));
				TheHTMLCodes.InsertPtr(new HTMLCode("igrave",0x00EC));
				TheHTMLCodes.InsertPtr(new HTMLCode("image",0x2111));
				TheHTMLCodes.InsertPtr(new HTMLCode("infin",0x221E));
				TheHTMLCodes.InsertPtr(new HTMLCode("int",0x222B));
				TheHTMLCodes.InsertPtr(new HTMLCode("iota",0x03B9));
				TheHTMLCodes.InsertPtr(new HTMLCode("iquest",0x00BF));
				TheHTMLCodes.InsertPtr(new HTMLCode("isin",0x2208));
				TheHTMLCodes.InsertPtr(new HTMLCode("iuml",0x00EF));
				TheHTMLCodes.InsertPtr(new HTMLCode("kappa",0x03BA));
				TheHTMLCodes.InsertPtr(new HTMLCode("lArr",0x21D0));
				TheHTMLCodes.InsertPtr(new HTMLCode("lang",0x2329));
				TheHTMLCodes.InsertPtr(new HTMLCode("lambda",0x03BB));
				TheHTMLCodes.InsertPtr(new HTMLCode("laquo",0x00AB));
				TheHTMLCodes.InsertPtr(new HTMLCode("larr",0x2190));
				TheHTMLCodes.InsertPtr(new HTMLCode("lceil",0x2308));
				TheHTMLCodes.InsertPtr(new HTMLCode("ldquo",0x201C));
				TheHTMLCodes.InsertPtr(new HTMLCode("le",0x2264));
				TheHTMLCodes.InsertPtr(new HTMLCode("lfloor",0x230A));
				TheHTMLCodes.InsertPtr(new HTMLCode("lowast",0x2217));
				TheHTMLCodes.InsertPtr(new HTMLCode("loz",0x25CA));
				TheHTMLCodes.InsertPtr(new HTMLCode("lrm",0x200E));
				TheHTMLCodes.InsertPtr(new HTMLCode("lsaquo",0x2039));
				TheHTMLCodes.InsertPtr(new HTMLCode("lsquo",0x2018));
				TheHTMLCodes.InsertPtr(new HTMLCode("lt",0x003C));
				TheHTMLCodes.InsertPtr(new HTMLCode("macr",0x00AF));
				TheHTMLCodes.InsertPtr(new HTMLCode("mdash",0x2014));
				TheHTMLCodes.InsertPtr(new HTMLCode("micro",0x00B5));
				TheHTMLCodes.InsertPtr(new HTMLCode("middot",0x00B7));
				TheHTMLCodes.InsertPtr(new HTMLCode("minus",0x2212));
				TheHTMLCodes.InsertPtr(new HTMLCode("mu",0x03BC));
				TheHTMLCodes.InsertPtr(new HTMLCode("nabla",0x2207));
				TheHTMLCodes.InsertPtr(new HTMLCode("nbsp",0x0020));
				TheHTMLCodes.InsertPtr(new HTMLCode("ndash",0x2013));
				TheHTMLCodes.InsertPtr(new HTMLCode("ne",0x2260));
				TheHTMLCodes.InsertPtr(new HTMLCode("ni",0x220B));
				TheHTMLCodes.InsertPtr(new HTMLCode("not",0x00AC));
				TheHTMLCodes.InsertPtr(new HTMLCode("notin",0x2209));
				TheHTMLCodes.InsertPtr(new HTMLCode("nsub",0x2284));
				TheHTMLCodes.InsertPtr(new HTMLCode("ntilde",0x00F1));
				TheHTMLCodes.InsertPtr(new HTMLCode("nu",0x03BD));
				TheHTMLCodes.InsertPtr(new HTMLCode("oacute",0x00F3));
				TheHTMLCodes.InsertPtr(new HTMLCode("ocirc",0x00F4));
				TheHTMLCodes.InsertPtr(new HTMLCode("oelig",0x0153));
				TheHTMLCodes.InsertPtr(new HTMLCode("ograve",0x00F2));
				TheHTMLCodes.InsertPtr(new HTMLCode("oline",0x203E));
				TheHTMLCodes.InsertPtr(new HTMLCode("omega",0x03C9));
				TheHTMLCodes.InsertPtr(new HTMLCode("omicron",0x03BF));
				TheHTMLCodes.InsertPtr(new HTMLCode("oplus",0x2295));
				TheHTMLCodes.InsertPtr(new HTMLCode("or",0x2228));
				TheHTMLCodes.InsertPtr(new HTMLCode("ordf",0x00AA));
				TheHTMLCodes.InsertPtr(new HTMLCode("ordm",0x00BA));
				TheHTMLCodes.InsertPtr(new HTMLCode("oslash",0x00F8));
				TheHTMLCodes.InsertPtr(new HTMLCode("otilde",0x00F5));
				TheHTMLCodes.InsertPtr(new HTMLCode("otimes",0x2297));
				TheHTMLCodes.InsertPtr(new HTMLCode("ouml",0x00F6));
				TheHTMLCodes.InsertPtr(new HTMLCode("para",0x00B6));
				TheHTMLCodes.InsertPtr(new HTMLCode("part",0x2202));
				TheHTMLCodes.InsertPtr(new HTMLCode("permil",0x2030));
				TheHTMLCodes.InsertPtr(new HTMLCode("perp",0x22A5));
				TheHTMLCodes.InsertPtr(new HTMLCode("phi",0x03C6));
				TheHTMLCodes.InsertPtr(new HTMLCode("pi",0x03C0));
				TheHTMLCodes.InsertPtr(new HTMLCode("piv",0x03D6));
				TheHTMLCodes.InsertPtr(new HTMLCode("plusmn",0x00B1));
				TheHTMLCodes.InsertPtr(new HTMLCode("pound",0x00A3));
				TheHTMLCodes.InsertPtr(new HTMLCode("prime",0x2032));
				TheHTMLCodes.InsertPtr(new HTMLCode("prod",0x220F));
				TheHTMLCodes.InsertPtr(new HTMLCode("prop",0x221D));
				TheHTMLCodes.InsertPtr(new HTMLCode("psi",0x03C8));
				TheHTMLCodes.InsertPtr(new HTMLCode("quot",0x0022));
				TheHTMLCodes.InsertPtr(new HTMLCode("rArr",0x21D2));
				TheHTMLCodes.InsertPtr(new HTMLCode("radic",0x221A));
				TheHTMLCodes.InsertPtr(new HTMLCode("rang",0x232A));
				TheHTMLCodes.InsertPtr(new HTMLCode("raquo",0x00BB));
				TheHTMLCodes.InsertPtr(new HTMLCode("rarr",0x2192));
				TheHTMLCodes.InsertPtr(new HTMLCode("rceil",0x2309));
				TheHTMLCodes.InsertPtr(new HTMLCode("rdquo",0x201D));
				TheHTMLCodes.InsertPtr(new HTMLCode("real",0x211C));
				TheHTMLCodes.InsertPtr(new HTMLCode("reg",0x00AE));
				TheHTMLCodes.InsertPtr(new HTMLCode("rfloor",0x230B));
				TheHTMLCodes.InsertPtr(new HTMLCode("rho",0x03C1));
				TheHTMLCodes.InsertPtr(new HTMLCode("rlm",0x200F));
				TheHTMLCodes.InsertPtr(new HTMLCode("rsaquo",0x203A));
				TheHTMLCodes.InsertPtr(new HTMLCode("rsquo",0x2019));
				TheHTMLCodes.InsertPtr(new HTMLCode("sbquo",0x201A));
				TheHTMLCodes.InsertPtr(new HTMLCode("scaron",0x0161));
				TheHTMLCodes.InsertPtr(new HTMLCode("sdot",0x22C5));
				TheHTMLCodes.InsertPtr(new HTMLCode("sect",0x00A7));
				TheHTMLCodes.InsertPtr(new HTMLCode("shy",0x00AF));
				TheHTMLCodes.InsertPtr(new HTMLCode("sigma",0x03C3));
				TheHTMLCodes.InsertPtr(new HTMLCode("sigmaf",0x03C2));
				TheHTMLCodes.InsertPtr(new HTMLCode("sim",0x223C));
				TheHTMLCodes.InsertPtr(new HTMLCode("spades",0x2660));
				TheHTMLCodes.InsertPtr(new HTMLCode("sub",0x2282));
				TheHTMLCodes.InsertPtr(new HTMLCode("sube",0x2286));
				TheHTMLCodes.InsertPtr(new HTMLCode("sum",0x2211));
				TheHTMLCodes.InsertPtr(new HTMLCode("sup",0x2283));
				TheHTMLCodes.InsertPtr(new HTMLCode("sup1",0x00B9));
				TheHTMLCodes.InsertPtr(new HTMLCode("sup2",0x00B2));
				TheHTMLCodes.InsertPtr(new HTMLCode("sup3",0x00B3));
				TheHTMLCodes.InsertPtr(new HTMLCode("supe",0x2287));
				TheHTMLCodes.InsertPtr(new HTMLCode("szlig",0x00DF));
				TheHTMLCodes.InsertPtr(new HTMLCode("tau",0x03C4));
				TheHTMLCodes.InsertPtr(new HTMLCode("there4",0x2234));
				TheHTMLCodes.InsertPtr(new HTMLCode("theta",0x03B8));
				TheHTMLCodes.InsertPtr(new HTMLCode("thetasym",0x03D1));
				TheHTMLCodes.InsertPtr(new HTMLCode("thinsp",0x2009));
				TheHTMLCodes.InsertPtr(new HTMLCode("thorn",0x00FE));
				TheHTMLCodes.InsertPtr(new HTMLCode("tilde",0x007E));
				TheHTMLCodes.InsertPtr(new HTMLCode("times",0x00D7));
				TheHTMLCodes.InsertPtr(new HTMLCode("trade",0x2122));
				TheHTMLCodes.InsertPtr(new HTMLCode("uArr",0x21D1));
				TheHTMLCodes.InsertPtr(new HTMLCode("uacute",0x00FA));
				TheHTMLCodes.InsertPtr(new HTMLCode("uarr",0x2191));
				TheHTMLCodes.InsertPtr(new HTMLCode("ucirc",0x00FB));
				TheHTMLCodes.InsertPtr(new HTMLCode("ugrave",0x00F9));
				TheHTMLCodes.InsertPtr(new HTMLCode("uml",0x00A8));
				TheHTMLCodes.InsertPtr(new HTMLCode("upsih",0x03D2));
				TheHTMLCodes.InsertPtr(new HTMLCode("upsilon",0x03C5));
				TheHTMLCodes.InsertPtr(new HTMLCode("uuml",0x00FC));
				TheHTMLCodes.InsertPtr(new HTMLCode("weierp",0x2118));
				TheHTMLCodes.InsertPtr(new HTMLCode("xi",0x03BE));
				TheHTMLCodes.InsertPtr(new HTMLCode("yacute",0x00FD));
				TheHTMLCodes.InsertPtr(new HTMLCode("yen",0x00A5));
				TheHTMLCodes.InsertPtr(new HTMLCode("yuml",0x00FF));
				TheHTMLCodes.InsertPtr(new HTMLCode("zeta",0x03B6));
				TheHTMLCodes.InsertPtr(new HTMLCode("zwj",0x200D));
				TheHTMLCodes.InsertPtr(new HTMLCode("zwnj",0x200C));
			}
			HTMLCode* code(TheHTMLCodes.GetPtr(Code));
			if(code)
				return(code->Car);
		}
	}
	return(0);
}


//------------------------------------------------------------------------------
RString RXMLParser::CharToCode(RChar car,bool strict)
{
	RString res;

	switch(car.Unicode())
	{
		case '"':
			if(strict)
				res="&quo;";
			else
				res="\"";
			break;
		case '&':
			res="&amp;";
			break;
		case '\'':
			if(strict)
				res="&apos;";
			else
				res="'";
			break;
		case '<':
			res="&lt;";
			break;
		case '>':
			if(strict)
				res="&gt;";
			else
				res=">";
			break;
		default:
			res+=car;
	}

	return(res);
}


//------------------------------------------------------------------------------
RString RXMLParser::XMLToString(const RString& str,bool html)
{
	RStringBuilder Res;
	RCharCursor Cur(str);
	RStringBuilder Code;
	RChar What;

	while(!Cur.End())
	{
		// Verify if it is not a code
		if(Cur()==RChar('&'))
		{
			// Skip &
			Cur.Next();

			// Read what could be a code
			Code.Clear();
			while((!Cur.End())&&(Cur()!=RChar(';'))&&(!Cur().IsSpace()))
			{
				Code+=Cur();
				Cur.Next();
			}

			// Is it a code (len<10 and last character is ";'))
			if((!Cur.End())&&(Cur()==RChar(';'))&&(Code().GetLen()<10))
			{
				What=CodeToChar(Code(),html);
				if(What.IsNull())
				{
					if(html)
						return(Code());
					ThrowRException("Invalid XML/HTML code \""+Code()+"\"");
				}
				else
					Res+=What;

				// Skip ;
				if(!Cur.End())
					Cur.Next();
			}
			else
			{
				if(html)
					return(Code());
				ThrowRException("Invalid XML/HTML code \""+Code()+"\"");
			}
		}
		else
		{
			Res+=Cur();
			Cur.Next();
		}
	}

	return(Res());
}


//------------------------------------------------------------------------------
RString RXMLParser::XMLToString(const RString& str)
{
	try
	{
		return(XMLToString(str,HTMLCodes));
	}
	catch(RException& e)
	{
		ThrowRIOException(this,e.GetMsg());
	}
}


//------------------------------------------------------------------------------
RString RXMLParser::StringToXML(const RString& str,bool strict)
{
	RCharCursor Cur(str);
	RStringBuilder Res;

	if(str.FindStr("<![CDATA[")!=-1)
		return(str);
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		Res+=CharToCode(Cur(),strict);
	}
	return(Res());
}


//------------------------------------------------------------------------------
const RXMLParser::HTMLTag* RXMLParser::GetHTMLTag(const RString& name) const
{
	HTMLTag* tag(Tags.GetPtr(name));
	if(!tag)
		ThrowRIOException(this,"Unknown HTML Tag '"+name+"'.");
	return(tag);
}


//------------------------------------------------------------------------------
void RXMLParser::LoadHeader(void)
{
	// Search after "<? xml"
	if(CurString("<?"))
	{
		Section=Header;
		SkipSpaces();
		bool XMLHeader(CurString("xml",false));
		if(XMLHeader)
		{
			SkipSpaces();

			// Search for parameters until ?> is found
			bool PopDefault;
			RContainer<Namespace,false,false> PopURI(5);
			LoadAttributes(PopDefault,PopURI,'?','>');
			if(Break)
				return;

			// Skip '>' and the spaces after that
			Next();
			SkipSpaces();
		}
		else
		{
			// A HTML file doesn't need it.
			if(!HTMLMode)
				ThrowRIOException(this,"Wrong XML header");
		}
	}

	// Search after "<?xml-stylesheet"
	if(CurString("<?xml-stylesheet"))
	{
		Section=StyleSheet;
		SkipSpaces();

		// Search for parameters until ?> is found
		bool PopDefault;
		RContainer<Namespace,false,false> PopURI(5);
		LoadAttributes(PopDefault,PopURI,'?','>');
		if(Break)
			return;

		// Skip '>' and the spaces after that
		Next(); SkipSpaces();
	}


	// Search after "!DOCTYPE"
	if(CurString("<!"))
	{
		Section=DOCTYPE;
		if(!CurString("DOCTYPE",false))
			ThrowRIOException(this,"Wrong DOCTYPE command");

		RString Content(GetToken(">["));
		if(HTMLMode)
			Content=Content.ToLower();
		SetDocType(XMLToString(Content));
		if(Break)
			return;
		SkipSpaces();

		if(Content=="html")
		{
			// Skip Everything until end of the header
			while((!End())&&(GetNextChar()!='>'))
				GetChar();
		}
		else
		{
			for(bool Cont=true;Cont;Cont=false)
			{
				if(GetNextChar()=='[')
				{
					GetChar(); // Skip [
					SkipSpaces();

					// Read Entities
					while((!End())&&(GetNextChar()!=']'))
					{
						// Get next name
						RString cmd(GetWord());
						if(cmd!="<!ENTITY")
							ThrowRIOException(this,"Wrong entities formating");

						RString ns(GetWord());
						Cur=GetChar();
						if(Cur!='"')
							ThrowRIOException(this,"Wrong entities formating");
						RString uri(GetToken(RString('"')));
						Cur=GetChar();
						if(Cur!='"')
							ThrowRIOException(this,"Wrong entities formating");

						// Add the entity
						AddEntity(ns,uri);

						// Verify if the attribute is a namespace
						int i=ns.Find(':');
						if(i!=-1)
						{
							Namespace* ptr=Namespaces.GetInsertPtr(ns.Mid(i+1,ns.GetLen()-i));
							ptr->URI.Push(new RString(AdaptNamespace(uri)));
						}
						else
						{
							// If not : -> default namespace
							DefaultNamespace.Push(new RString(AdaptNamespace(uri)));
						}

						// Skip >
						SkipSpaces();
						Next();
						SkipSpaces();
					}

					if(End())
						ThrowRIOException(this,"Wrong entities formating");

					// Skip ] and spaces
					Next();
					SkipSpaces();
					Cont=true;
				}
				else if(CurString("SYSTEM"))
				{
					SkipSpaces();
					RChar What=GetChar();
					bool Quotes=((What==RChar('\''))||(What==RChar('"')));
					if(!Quotes)
						ThrowRIOException(this,"Bad XML file");
					SetDTD(GetToken(RString(What)));
					if(Break)
						return;
					SkipSpaces();
					if(GetChar()!=What)
						ThrowRIOException(this,"Bad XML file");
					SkipSpaces();
					Cont=true;
				}
				else
				{
					// Perhaps a string defining directly the DTD
					RChar What=GetNextChar();
					bool Quotes=((What==RChar('\''))||(What==RChar('"')));
					if(Quotes)
					{
						Next();  // Skip the quote
						SetDTD(GetToken(RString(What)));
						if(Break)
							return;
						SkipSpaces();
						if(GetChar()!=What)
							ThrowRIOException(this,"Bad XML file");
						SkipSpaces();
						Cont=true;
					}
				}
			}
		}
		if(GetChar()!='>') // Skip >
			ThrowRIOException(this,"Bad XML file");
		SkipSpaces();
	}
}


//------------------------------------------------------------------------------
void RXMLParser::LoadNextTag(void)
{
	RString attrn,attrv;
	RString TagName;
	RString Contains;
	RChar What;
	RString Code;
	bool PopDefault;
	RContainer<Namespace,false,false> PopURI(5);
	bool CDATA;
	bool Resolve(false);

	// If not a tag -> Error
	if(GetChar()!='<')
		ThrowRIOException(this,"Not a tag");

	// Read name of the tag
	LastTokenPos=GetPos();
	size_t OpenTagLine(GetLineNb());
	TagName=GetToken(">/");
	CurTagClosing=false;  // Suppose it is not a closing tag

	// Treat the tag
	// Search if it has a namespace
	RString uri,lname;
	RString prefix;
	int i=TagName.Find(':');
	if(i!=-1)
	{
		prefix=TagName.Mid(0,i);
		Namespace* ptr=Namespaces.GetPtr(prefix);
		if(!ptr)
			Resolve=true;       // Search after the attribute
		else
			uri=(*ptr->URI());
		lname=TagName.Mid(i+1,TagName.GetLen()-i);
	}
	else
	{
		if(DefaultNamespace.GetNb())
			uri=(*DefaultNamespace()); // Default namespace.
		else
			Resolve=true;    // Search after the attribute
		lname=TagName;
	}

	if(HTMLMode)
	{
		lname=lname.ToLower();
		TagName=TagName.ToLower();

		// Valid HTML Tag?
		CurHTMLTag=Tags.GetPtr(lname);
		if(!CurHTMLTag)
			ThrowRIOException(this,"Unknown opening HTML Tag '"+lname+"'.");
	}
	BeginTag(uri,lname,TagName);
	if(Break)
		return;
	if(HTMLMode)
	{
		// Verify that is not a single tag in HTML
		if(CurHTMLTag->Single)
			CurTagClosing=true;
		CurHTMLTag=0;
	}

	// Read Attributes
	LoadAttributes(PopDefault,PopURI);
	if(Break)
		return;
	if(Resolve)
	{
		if(!prefix.IsEmpty())
		{
			Namespace* ptr=Namespaces.GetPtr(prefix);
			if(!ptr)
				ThrowRIOException(this,"Namespace with prefix '"+prefix+"' no defined");
			uri=(*ptr->URI());
			ResolveNamespace(uri);
			if(Break)
				return;
		}
		else if(DefaultNamespace.GetNb())
		{
			uri=(*DefaultNamespace()); // Default namespace.
			ResolveNamespace(uri);
			if(Break)
				return;
		}
	}
	BeginTagParsed(uri,lname,TagName);
	if(Break)
		return;

	// If the parser does not now if it is a closing tag -> test it.
	if(!CurTagClosing)
	{
		// It is a closing tag? -> Skip / and >
		CurTagClosing=(Cur==RChar('/'));
		if(CurTagClosing)
			Next();  // Skip / from "/>"
	}
	SkipSpaces();

	// Verify if it has sub-tags
	if(CurTagClosing)
	{
		if(HTMLMode)
		{
			lname=lname.ToLower();
			TagName=TagName.ToLower();

			// Valid HTML Tag?
			CurHTMLTag=Tags.GetPtr(lname);
			if(!CurHTMLTag)
				ThrowRIOException(this,"Unknown closing HTML Tag '"+lname+"'.");
		}
		EndTag(uri,lname,TagName);
		if(Break)
			return;
		if(HTMLMode)
		{
			CurHTMLTag=0;
			if(lname=="html")
				FoundClosingHTML=true;
		}
		return;
	}

	// Treat sub-tags
	while((!End())&&(!CurString("</",false)))
	{
		LastTokenPos=GetPos();
		// If only '<' -> new tag
		if(GetNextChar()==RChar('<'))
		{
			// It is a tag -> read it
			CDATA=CurString("<![CDATA[",true);
			if(CDATA)
			{
				CurDepth++;   	// Increase the depth
				Text("<![CDATA[");
				SkipSpaces();
				SetParseSpace(RTextFile::LeaveSpaces);

				while(!CurString("]]>",false))
				{
					if(End())
						ThrowRIOException(this,"Invalid '[CDATA[]]>'.");
					LastTokenPos=GetPos();
					RString tmp(GetTokenString("]]>"));
					if(tmp.GetLen())
					{
						Text(tmp);
						if(Break)
							return;
					}

					// Add all spaces
					while(GetNextChar().IsSpace())
					{
						LastTokenPos=GetPos();
						Text(RString(GetChar()));
						if(Break)
							return;
					}
				}
				LastTokenPos=GetPos();
				Text("]]>");
				SetParseSpace(RTextFile::SkipAllSpaces);
				SkipSpaces();
				CurDepth--;   	// Decrease the depth
			}
			else
			{
				CurDepth++;   	// Increase the depth
				LoadNextTag();
				if(Break)
					return;
				CurDepth--;   	// Decrease the depth
			}
		}
		else
		{
			// It is content -> read it as long as there is no open tag.
			Contains.SetLen(0);
			CDATA=true; // Suppose that first '<' found is a "<![CDATA["
			SetParseSpace(RTextFile::LeaveSpaces);
			CurDepth++;   	// Increase the depth
			while(CDATA)
			{
				// Get the next word
				LastTokenPos=GetPos();
				RString tmp(GetToken("<"));
				if(tmp.GetLen())
				{
					Text(XMLToString(tmp));
					if(Break)
					return;
				}

				// Add all spaces
				while(GetNextChar().IsSpace())
				{
					LastTokenPos=GetPos();
					Text(RString(GetChar()));
					if(Break)
						return;
				}

				// Look if the next '<' is the beginning of "<![CDATA["
				CDATA=CurString("<![CDATA[",true);
				if(CDATA)
				{
					LastTokenPos=GetPos();
					Text("<![CDATA[");
					SkipSpaces();
					SetParseSpace(RTextFile::LeaveSpaces);

					while(!CurString("]]>"))
					{
						if(End())
							ThrowRIOException(this,"Invalid '[CDATA[]]>'.");
						LastTokenPos=GetPos();
						RString tmp(GetTokenString("]]>"));
						if(tmp.GetLen())
						{
							Text(tmp);
							if(Break)
								return;
						}

						// Add all spaces
						while(GetNextChar().IsSpace())
						{
							LastTokenPos=GetPos();
							Text(RString(GetChar()));
							if(Break)
								return;
						}
					}
					LastTokenPos=GetPos();
					Text("]]>");
					SetParseSpace(RTextFile::SkipAllSpaces);
					SkipSpaces();
				}
			}
			CurDepth--;   	// Decrease the depth
		}
		SetParseSpace(RTextFile::SkipAllSpaces);
		SkipSpaces();
	}

	// Read the close tag
	CurTagClosing=true;
	SkipSpaces();
	LastTokenPos=GetPos();
	RString EndTagName(GetToken(RString('>')));
	if((EndTagName!=TagName))
		ThrowRIOException(this,"Found closing tag '"+EndTagName+"' while closing tag '"+TagName+"' ("+RString::Number(OpenTagLine)+") was expected.");
	SkipSpaces();
	if(HTMLMode&&(lname=="html"))
			FoundClosingHTML=true;
	else
		Next(); // Skip >
	if(HTMLMode)
	{
		lname=lname.ToLower();
		TagName=TagName.ToLower();

		// Valid HTML Tag?
		CurHTMLTag=Tags.GetPtr(lname);
		if(!CurHTMLTag)
			ThrowRIOException(this,"Unknown closing HTML Tag '"+lname+"'.");
	}
	EndTag(uri,lname,TagName);
	if(Break)
		return;
	if(HTMLMode)
		CurHTMLTag=0;
	if(PopDefault)
		DefaultNamespace.Pop();
	while(PopURI.GetNb())
	{
		RCursor<Namespace> Cur(PopURI);
		for(Cur.Start();!Cur.End();Cur.Next())
			Cur()->URI.Pop();
		PopURI.Clear();
	}
	SkipSpaces();
}


//------------------------------------------------------------------------------
void RXMLParser::LoadAttributes(bool& popdefault,RContainer<Namespace,false,false>& popuri,RChar EndTag1,RChar EndTag2)
{
	RChar What;
	bool Quotes;

	CurDepth++;

	// No namespace
	popdefault=false;

	while((!End())&&(GetNextChar()!=EndTag1)&&(GetNextChar()!=EndTag2))
	{
		// Clear name, value and uri
		RString ns;
		RString uri;
		RString lname;
		bool GetNs(false);

		// Read the name of the attribute
		LastTokenPos=GetPos();
		RString attrn(GetToken("=>"));

		// Verify if the attribute name has a namespace
		int i=attrn.Find(':');
		if(i!=-1)
		{
			// Namespace defined
			RString prefix(attrn.Mid(0,i));
			lname=AdaptNamespace(attrn.Mid(i+1,attrn.GetLen()-i-1));
			if(prefix=="xmlns")
			{
				uri="http://www.w3.org/2000/xmlns";  // New namespace declared
				GetNs=true;
			}
			else
			{
				Namespace* ptr=Namespaces.GetPtr(prefix);
				if(!ptr)
					ThrowRIOException(this,"Namespace with prefix '"+prefix+"' no defined");
				uri=(*ptr->URI());
			}
		}
		else
		{
			lname=attrn;
			if(attrn=="xmlns")
			{
				uri="http://www.w3.org/2000/xmlns";  // New namespace declared
				GetNs=true;
			}
			else
			{
				if(DefaultNamespace.GetNb())
					uri=(*DefaultNamespace()); // Default namespace.
			}
		}

		// Add the attribute
		if(Section==Header)
			HeaderAttribute(uri,lname,attrn);
		else
			AddAttribute(uri,lname,attrn);
		if(Break)
			return;

		// Determine if a value is assign
		Next();
		if(Cur==RChar('='))
		{
			SkipSpaces();

			// Determine if the parameter is delimited by quotes
			Next();
			What=Cur;
			Quotes=((What==RChar('\''))||(What==RChar('"')));

			// Read the value
			if(Quotes)
			{
				SetParseSpace(RTextFile::LeaveSpaces);
				while((!End())&&(GetNextChar()!=What))
				{
					// Get the next word
					LastTokenPos=GetPos();
					RString tmp(GetToken(RString(What)));
					if(tmp.GetLen())
					{
						if(Section==Header)
							HeaderValue(XMLToString(tmp));
						else
							Value(XMLToString(tmp));
						if(Break)
							return;
					}
					if(GetNs)
						ns+=tmp;

					// Add all spaces
					while(GetNextChar().IsSpace())
					{
						if(Section==Header)
							HeaderValue(tmp);
						else
							Value(RString(GetChar()));
						if(Break)
							return;
					}
				}
				SetParseSpace(RTextFile::SkipAllSpaces);
				Next();
			}
			else
			{
				// If Quote must be used -> generate an exception
				if(OnlyQuote())
					ThrowRIOException(this,"Quote must be used to delimit the parameter value in a tag.");
				RString tmp(What+GetToken(">"));
				if(Section==Header)
					HeaderValue(tmp);
				else
					Value(tmp);
				if(Break)
					return;
			}

			// Verify if the attribute is a namespace
			if(GetNs)
			{
				uri=AdaptNamespace(XMLToString(ns));
				int i=attrn.Find(':');
				if(i!=-1)
				{
					Namespace* ptr=Namespaces.GetInsertPtr(AdaptNamespace(attrn.Mid(i+1,attrn.GetLen()-i)));
					ptr->URI.Push(new RString(uri));
					popuri.InsertPtr(ptr);
				}
				else
				{
					// If not : -> default namespace
					DefaultNamespace.Push(new RString(uri));
					popdefault=true;
				}
			}
			SkipSpaces();
		}
	}

	CurDepth--;
	Next(); // Put the internal pointer to the first ending character found
}


//------------------------------------------------------------------------------
void RXMLParser::HeaderAttribute(const RString&,const RString& lName,const RString&)
{
	if(lName=="encoding")
		TreatEncoding=true;
	else
		TreatEncoding=false;
}


//------------------------------------------------------------------------------
void RXMLParser::HeaderValue(const RString& value)
{
	if(TreatEncoding)
		SetEncoding(value.ToLatin1());
}


//------------------------------------------------------------------------------
void RXMLParser::SetDocType(const RString& docType)
{
	DocType=docType;
}


//------------------------------------------------------------------------------
void RXMLParser::AddEntity(const RString&,const RString&)
{
}


//------------------------------------------------------------------------------
void RXMLParser::BeginTag(const RString&,const RString&,const RString&)
{
}


//------------------------------------------------------------------------------
void RXMLParser::BeginTagParsed(const RString&,const RString&,const RString&)
{
}


//------------------------------------------------------------------------------
void RXMLParser::ResolveNamespace(const RString&)
{
}


//------------------------------------------------------------------------------
void RXMLParser::AddAttribute(const RString&,const RString&,const RString&)
{
}


//------------------------------------------------------------------------------
void RXMLParser::Value(const RString&)
{
}


//------------------------------------------------------------------------------
void RXMLParser::EndTag(const RString&, const RString&, const RString&)
{
}


//------------------------------------------------------------------------------
void RXMLParser::Text(const RString&)
{
}


//------------------------------------------------------------------------------
void RXMLParser::SetDTD(const RString&)
{
}


//------------------------------------------------------------------------------
bool RXMLParser::OnlyQuote(void)
{
	return(true);
}


//------------------------------------------------------------------------------
RXMLParser::~RXMLParser(void)
{
}
