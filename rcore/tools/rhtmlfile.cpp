/*

	R Project Library

	RHTMLFile.cpp

	HTML file - Implementation.

	Copyright 2004-2005 by the Universit�Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).

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
#include <rstd/rhtmlfile.h>
using namespace R;
using namespace std;



//------------------------------------------------------------------------------
//
// class RHTMLFile::Tag
//
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
class RHTMLFile::Tag
{
public:
	RString Name;      // Name of the HTML Tag
	int Level;         // Integer representing the depth of the tag in HTML structure.
	bool Single;       // A single Tag (ex: br)?

	Tag(const char* n,int l,bool s)
		: Name(n), Level(l), Single(s) {}
	int Compare(const Tag* t) const {return(Name.Compare(t->Name));}
	int Compare(const Tag& t) const {return(Name.Compare(t.Name));}
	int Compare(const char* t) const {return(Name.Compare(t));}
	int Compare(const RString& t) const {return(Name.Compare(t));}
};


//------------------------------------------------------------------------------
//
// struct CodeToChar
//
//------------------------------------------------------------------------------

struct RCharCode
{
	RString Code;
	RChar Car;
};

//------------------------------------------------------------------------------
// All the supported codes from HTML or XML to normal.
const RCharCode FromXML[]={
{"#13" ,0x000D},
{"#32" ,0x0020},
{"#33" ,0x0021},
{"#34" ,0x0022},
{"#35" ,0x0023},
{"#36" ,0x0024},
{"#37" ,0x0025},
{"#38" ,0x0026},
{"#39" ,0x0027},
{"#40" ,0x0028},
{"#41" ,0x0029},
{"#42" ,0x002A},
{"#43" ,0x002B},
{"#44" ,0x002C},
{"#45" ,0x002D},
{"#46" ,0x002E},
{"#47" ,0x002F},
{"#58" ,0x003A},
{"#59" ,0x003B},
{"#60" ,0x003C},
{"#61" ,0x003D},
{"#62" ,0x003E},
{"#63" ,0x003F},
{"#64" ,0x0040},
{"#91" ,0x005B},
{"#92" ,0x005C},
{"#93" ,0x005D},
{"#94" ,0x005E},
{"#95" ,0x005F},
{"#96" ,0x0060},
{"#123",0x007B},
{"#124",0x007C},
{"#125",0x007D},
{"#126",0x007E},
{"#128",0x0080},
{"#130",0x0082},
{"#131",0x0083},
{"#132",0x0084},
{"#133",0x0085},
{"#134",0x0086},
{"#135",0x0087},
{"#136",0x0088},
{"#137",0x0089},
{"#138",0x008A},
{"#139",0x008B},
{"#140",0x008C},
{"#142",0x008E},
{"#145",0x0091},
{"#146",0x0092},
{"#147",0x0093},
{"#148",0x0094},
{"#149",0x0095},
{"#150",0x0096},
{"#151",0x0097},
{"#152",0x0098},
{"#153",0x0099},
{"#154",0x009A},
{"#155",0x009B},
{"#156",0x009C},
{"#158",0x009E},
{"#159",0x009F},
{"#160",0x00A0},
{"#161",0x00A1},
{"#162",0x00A2},
{"#163",0x00A3},
{"#164",0x00A4},
{"#165",0x00A5},
{"#166",0x00A6},
{"#167",0x00A7},
{"#168",0x00A8},
{"#169",0x00A9},
{"#170",0x00AA},
{"#171",0x00AB},
{"#172",0x00AC},
{"#173",0x00AD},
{"#174",0x00AE},
{"#175",0x00AF},
{"#176",0x00B0},
{"#177",0x00B1},
{"#178",0x00B2},
{"#179",0x00B3},
{"#180",0x00B4},
{"#181",0x00B5},
{"#182",0x00B6},
{"#183",0x00B7},
{"#184",0x00B8},
{"#185",0x00B9},
{"#186",0x00BA},
{"#187",0x00BB},
{"#188",0x00BC},
{"#189",0x00BD},
{"#190",0x00BE},
{"#191",0x00BF},
{"#192",0x00C0},
{"#193",0x00C1},
{"#194",0x00C2},
{"#195",0x00C3},
{"#196",0x00C4},
{"#197",0x00C5},
{"#198",0x00C6},
{"#199",0x00C7},
{"#200",0x00C8},
{"#201",0x00C9},
{"#202",0x00CA},
{"#203",0x00CB},
{"#204",0x00CC},
{"#205",0x00CD},
{"#206",0x00CE},
{"#207",0x00CF},
{"#208",0x00D0},
{"#209",0x00D1},
{"#210",0x00D2},
{"#211",0x00D3},
{"#212",0x00D4},
{"#213",0x00D5},
{"#214",0x00D6},
{"#215",0x00D7},
{"#216",0x00D8},
{"#217",0x00D9},
{"#218",0x00DA},
{"#219",0x00DB},
{"#220",0x00DC},
{"#221",0x00DD},
{"#222",0x00DE},
{"#223",0x00DF},
{"#224",0x00E0},
{"#225",0x00E1},
{"#226",0x00E2},
{"#227",0x00E3},
{"#228",0x00E4},
{"#229",0x00E5},
{"#230",0x00E6},
{"#231",0x00E7},
{"#232",0x00E8},
{"#233",0x00E9},
{"#234",0x00EA},
{"#235",0x00EB},
{"#236",0x00EC},
{"#237",0x00ED},
{"#238",0x00EE},
{"#239",0x00EF},
{"#240",0x00F0},
{"#241",0x00F1},
{"#242",0x00F2},
{"#243",0x00F3},
{"#244",0x00F4},
{"#245",0x00F5},
{"#246",0x00F6},
{"#247",0x00F7},
{"#248",0x00F8},
{"#249",0x00F9},
{"#250",0x00FA},
{"#251",0x00FB},
{"#252",0x00FC},
{"#253",0x00FD},
{"#254",0x00FE},
{"#255",0x00FF},
{"#710",0x02C6},
{"#732",0x02DC},
{"#913",0x0391},
{"#914",0x0392},
{"#915",0x0393},
{"#916",0x0394},
{"#917",0x0395},
{"#918",0x0396},
{"#919",0x0397},
{"#920",0x0398},
{"#921",0x0399},
{"#922",0x039A},
{"#923",0x039B},
{"#924",0x039C},
{"#925",0x039D},
{"#926",0x039E},
{"#927",0x039F},
{"#928",0x03A0},
{"#929",0x03A1},
{"#931",0x03A3},
{"#932",0x03A4},
{"#933",0x03A5},
{"#934",0x03A6},
{"#935",0x03A7},
{"#936",0x03A8},
{"#937",0x03A9},
{"#945",0x03B1},
{"#946",0x03B2},
{"#947",0x03B3},
{"#948",0x03B4},
{"#949",0x03B5},
{"#950",0x03B6},
{"#951",0x03B7},
{"#952",0x03B8},
{"#953",0x03B9},
{"#954",0x03BA},
{"#955",0x03BB},
{"#956",0x03BC},
{"#957",0x03BD},
{"#958",0x03BE},
{"#959",0x03BF},
{"#959",0x03BF},
{"#960",0x03C0},
{"#961",0x03C1},
{"#962",0x03C2},
{"#963",0x03C3},
{"#964",0x03C4},
{"#965",0x03C5},
{"#966",0x03C6},
{"#967",0x03C7},
{"#968",0x03C8},
{"#969",0x03C9},
{"#977",0x03D1},
{"#978",0x03D2},
{"#982",0x03D6},
{"#8194",0x2002},
{"#8195",0x2003},
{"#8201",0x2009},
{"#8204",0x200C},
{"#8205",0x200D},
{"#8206",0x200E},
{"#8207",0x200F},
{"#8211",0x2013},
{"#8212",0x2014},
{"#8216",0x2018},
{"#8217",0x2019},
{"#8218",0x201A},
{"#8220",0x201C},
{"#8221",0x201D},
{"#8222",0x201E},
{"#8224",0x2020},
{"#8225",0x2021},
{"#8226",0x2022},
{"#8230",0x2026},
{"#8240",0x2030},
{"#8242",0x2032},
{"#8243",0x2033},
{"#8249",0x2039},
{"#8250",0x203A},
{"#8254",0x203E},
{"#8260",0x2044},
{"#8364",0x20AC},
{"#8472",0x2118},
{"#8465",0x2111},
{"#8476",0x211C},
{"#8482",0x2122},
{"#8501",0x2135},
{"#8592",0x2190},
{"#8593",0x2191},
{"#8594",0x2192},
{"#8595",0x2193},
{"#8596",0x2194},
{"#8629",0x21B5},
{"#8656",0x21D0},
{"#8657",0x21D1},
{"#8658",0x21D2},
{"#8659",0x21D3},
{"#8660",0x21D4},
{"#8704",0x2200},
{"#8706",0x2202},
{"#8707",0x2203},
{"#8709",0x2205},
{"#8711",0x2207},
{"#8712",0x2208},
{"#8713",0x2209},
{"#8715",0x220B},
{"#8719",0x220F},
{"#8721",0x2211},
{"#8722",0x2212},
{"#8727",0x2217},
{"#8730",0x221A},
{"#8733",0x221D},
{"#8734",0x221E},
{"#8736",0x2220},
{"#8743",0x2227},
{"#8744",0x2228},
{"#8745",0x2229},
{"#8746",0x222A},
{"#8747",0x222B},
{"#8756",0x2234},
{"#8764",0x223C},
{"#8773",0x2245},
{"#8776",0x2248},
{"#8800",0x2260},
{"#8801",0x2261},
{"#8804",0x2264},
{"#8805",0x2265},
{"#8834",0x2282},
{"#8835",0x2283},
{"#8836",0x2284},
{"#8838",0x2286},
{"#8839",0x2287},
{"#8853",0x2295},
{"#8855",0x2297},
{"#8869",0x22A5},
{"#8901",0x22C5},
{"#8968",0x2308},
{"#8969",0x2309},
{"#8970",0x230A},
{"#8971",0x230B},
{"#9001",0x2329},
{"#9002",0x232A},
{"#9674",0x25CA},
{"#9824",0x2660},
{"#9827",0x2663},
{"#9829",0x2665},
{"#9830",0x2666},
{"AElig",0x00C6},
{"Aacute",0x00C1},
{"Acirc",0x00C2},
{"Agrave",0x00C0},
{"Alpha",0x0391},
{"Aring",0x00C5},
{"Atilde",0x00C3},
{"Auml",0x00C4},
{"Beta",0x0392},
{"Ccedil",0x00C7},
{"Chi",0x03A7},
{"Dagger",0x2021},
{"Delta",0x0394},
{"ETH",0x00D0},
{"Eacute",0x00C9},
{"Ecirc",0x00CA},
{"Egrave",0x00C8},
{"Epsilon",0x0395},
{"Eta",0x0397},
{"Euml",0x00CB},
{"Gamma",0x0393},
{"Iacute",0x00CD},
{"Icirc",0x00CE},
{"Igrave",0x00CC},
{"Iota",0x0399},
{"Iuml",0x00CF},
{"Kappa",0x039A},
{"Lambda",0x039B},
{"Mu",0x039C},
{"Ntilde",0x00D1},
{"Nu",0x039D},
{"OElig",0x0152},
{"Oacute",0x00D3},
{"Ocirc",0x00D4},
{"Ograve",0x00D2},
{"Omega",0x03A9},
{"Omicron",0x039F},
{"Oslash",0x00D8},
{"Otilde",0x00D5},
{"Ouml",0x00D6},
{"Phi",0x03A6},
{"Pi",0x03A0},
{"Prime",0x2033},
{"Psi",0x03A8},
{"Rho",0x03A1},
{"Scaron",0x0160},
{"Sigma",0x03A3},
{"THORN",0x00DE},
{"Tau",0x03A4},
{"Theta",0x0398},
{"Uacute",0x00DA},
{"Ucirc",0x00DB},
{"Ugrave",0x00D9},
{"Upsilon",0x03A5},
{"Uuml",0x00DC},
{"Xi",0x039E},
{"Yacute",0x00DD},
{"Yuml",0x0178},
{"Zeta",0x0396},
{"aacute",0x00E1},
{"acirc",0x00E2},
{"acute",0x00E1},
{"aelig",0x00E6},
{"agrave",0x00E0},
{"alefsym",0x2135},
{"alpha",0x03B1},
{"amp",0x0026},
{"and",0x2227},
{"ang",0x2220},
{"aring",0x00E5},
{"asymp",0x2248},
{"atilde",0x00E3},
{"auml",0x00E4},
{"bdquo",0x201E},
{"beta",0x03B2},
{"brvbar",0x00A6},
{"bull",0x2022},
{"cap",0x2229},
{"ccedil",0x00E7},
{"cedil",0x00B8},
{"cent",0x00A2},
{"chi",0x03C7},
{"circ",0x02C6},
{"clubs",0x2663},
{"cong",0x2245},
{"copy",0x00A9},
{"crarr",0x21B5},
{"cup",0x222A},
{"curren",0x00A4},
{"dArr",0x21D3},
{"dagger",0x2020},
{"darr",0x2193},
{"deg",0x00B0},
{"delta",0x03B4},
{"diams",0x2666},
{"divide",0x00F7},
{"eacute",0x00E9},
{"ecirc",0x00EA},
{"egrave",0x00E8},
{"empty",0x2205},
{"emsp",0x2003},
{"ensp",0x2002},
{"epsilon",0x03B5},
{"equiv",0x2261},
{"eta",0x03B7},
{"eth",0x00F0},
{"euml",0x00E4},
{"euro",0x20AC},
{"exist",0x2203},
{"forall",0x2200},
{"frac12",0x00BD},
{"frac14",0x00BC},
{"frac34",0x00B3},
{"frasl",0x002F},
{"gamma",0x03B3},
{"ge",0x2265},
{"gt",0x003E},
{"hArr",0x21D4},
{"harr",0x2194},
{"hearts",0x2665},
{"hellip",0x2026},
{"iacute",0x00ED},
{"icirc",0x00EE},
{"iexcl",0x00A1},
{"igrave",0x00EC},
{"image",0x2111},
{"infin",0x221E},
{"int",0x222B},
{"iota",0x03B9},
{"iquest",0x00BF},
{"isin",0x2208},
{"iuml",0x00EF},
{"kappa",0x03BA},
{"lArr",0x21D0},
{"lang",0x2329},
{"lambda",0x03BB},
{"laquo",0x00AB},
{"larr",0x2190},
{"lceil",0x2308},
{"ldquo",0x201C},
{"le",0x2264},
{"lfloor",0x230A},
{"lowast",0x2217},
{"loz",0x25CA},
{"lrm",0x200E},
{"lsaquo",0x2039},
{"lsquo",0x2018},
{"lt",0x003C},
{"macr",0x00AF},
{"mdash",0x2014},
{"micro",0x00B5},
{"middot",0x00B7},
{"minus",0x2212},
{"mu",0x03BC},
{"nabla",0x2207},
{"nbsp",0x0020},
{"ndash",0x2013},
{"ne",0x2260},
{"ni",0x220B},
{"not",0x00AC},
{"notin",0x2209},
{"nsub",0x2284},
{"ntilde",0x00F1},
{"nu",0x03BD},
{"oacute",0x00F3},
{"ocirc",0x00F4},
{"oelig",0x0153},
{"ograve",0x00F2},
{"oline",0x203E},
{"omega",0x03C9},
{"omicron",0x03BF},
{"oplus",0x2295},
{"or",0x2228},
{"ordf",0x00AA},
{"ordm",0x00BA},
{"oslash",0x00F8},
{"otilde",0x00F5},
{"otimes",0x2297},
{"ouml",0x00F6},
{"para",0x00B6},
{"part",0x2202},
{"permil",0x2030},
{"perp",0x22A5},
{"phi",0x03C6},
{"pi",0x03C0},
{"piv",0x03D6},
{"plusmn",0x00B1},
{"pound",0x00A3},
{"prime",0x2032},
{"prod",0x220F},
{"prop",0x221D},
{"psi",0x03C8},
{"quot",0x0022},
{"rArr",0x21D2},
{"radic",0x221A},
{"rang",0x232A},
{"raquo",0x00BB},
{"rarr",0x2192},
{"rceil",0x2309},
{"rdquo",0x201D},
{"real",0x211C},
{"reg",0x00AE},
{"rfloor",0x230B},
{"rho",0x03C1},
{"rlm",0x200F},
{"rsaquo",0x203A},
{"rsquo",0x2019},
{"sbquo",0x201A},
{"scaron",0x0161},
{"sdot",0x22C5},
{"sect",0x00A7},
{"shy",0x00AF},
{"sigma",0x03C3},
{"sigmaf",0x03C2},
{"sim",0x223C},
{"spades",0x2660},
{"sub",0x2282},
{"sube",0x2286},
{"sum",0x2211},
{"sup",0x2283},
{"sup1",0x00B9},
{"sup2",0x00B2},
{"sup3",0x00B3},
{"supe",0x2287},
{"szlig",0x00DF},
{"tau",0x03C4},
{"there4",0x2234},
{"theta",0x03B8},
{"thetasym",0x03D1},
{"thinsp",0x2009},
{"thorn",0x00FE},
{"tilde",0x007E},
{"times",0x00D7},
{"trade",0x2122},
{"uArr",0x21D1},
{"uacute",0x00FA},
{"uarr",0x2191},
{"ucirc",0x00FB},
{"ugrave",0x00F9},
{"uml",0x00A8},
{"upsih",0x03D2},
{"upsilon",0x03C5},
{"uuml",0x00FC},
{"weierp",0x2118},
{"xi",0x03BE},
{"yacute",0x00FD},
{"yen",0x00A5},
{"yuml",0x00FF},
{"zeta",0x03B6},
{"zwj",0x200D},
{"zwnj",0x200C}
};


//------------------------------------------------------------------------------
//
// class RHTMLFile
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
RHTMLFile::RHTMLFile(const RString& name,RXMLStruct* xmlstruct,const RString& encoding)
 : RXMLFile(name,xmlstruct,encoding), Tags(200,10), FoundClosingHTML(false)
{
	InitValidTags();
}


//------------------------------------------------------------------------------
RHTMLFile::RHTMLFile(const RString& name,RXMLStruct& xmlstruct,const RString& encoding)
 : RXMLFile(name,xmlstruct,encoding), Tags(200,10), FoundClosingHTML(false)
{
	InitValidTags();
}


//------------------------------------------------------------------------------
RHTMLFile::RHTMLFile(RIOFile& file,RXMLStruct* xmlstruct,const RString& encoding)
 : RXMLFile(file,xmlstruct,encoding), Tags(200,10), FoundClosingHTML(false)
{
	InitValidTags();
}


//------------------------------------------------------------------------------
RHTMLFile::RHTMLFile(RIOFile& file,RXMLStruct& xmlstruct,const RString& encoding)
 : RXMLFile(file,xmlstruct,encoding), Tags(200,10), FoundClosingHTML(false)
{
	InitValidTags();
}


//------------------------------------------------------------------------------
void RHTMLFile::SetDocType(const RString& docType)
{
	RString name;

	name=docType.ToLower();
	if((name=="xhtml")||(name=="html"))
		RXMLFile::SetDocType("html");
	else
		throw RIOException(this,"DOCTYPE of a HTML file must be either HTML or XHTML");
}


//------------------------------------------------------------------------------
void RHTMLFile::InitValidTags(void)
{
	// Global Tags
	Tags.InsertPtr(new Tag("html",0,false));
	Tags.InsertPtr(new Tag("body",10,false));
	Tags.InsertPtr(new Tag("head",10,false));

	// Heading Tags
	Tags.InsertPtr(new Tag("meta",500,true));
	Tags.InsertPtr(new Tag("title",500,false));
	Tags.InsertPtr(new Tag("base",500,false));
	Tags.InsertPtr(new Tag("link",500,true));
	Tags.InsertPtr(new Tag("style",500,false));

	// Division Tags
	Tags.InsertPtr(new Tag("div",150,false));
	Tags.InsertPtr(new Tag("span",160,false));

	// Frame
	Tags.InsertPtr(new Tag("frameset",100,false));
	Tags.InsertPtr(new Tag("frame",360,false));
	Tags.InsertPtr(new Tag("noframe",360,false));
	Tags.InsertPtr(new Tag("iframe",360,false));

	//info to contact author
	Tags.InsertPtr(new Tag("address",360,false));

	// Forms
	Tags.InsertPtr(new Tag("form",360,false));
	Tags.InsertPtr(new Tag("input",500,false));
	Tags.InsertPtr(new Tag("button",500,false));
	Tags.InsertPtr(new Tag("label",500,false));
	Tags.InsertPtr(new Tag("select",500,false));
	Tags.InsertPtr(new Tag("option",500,false));
	Tags.InsertPtr(new Tag("optgroup",500,false));
	Tags.InsertPtr(new Tag("textarea",500,false));
	Tags.InsertPtr(new Tag("isindex",500,false));
	Tags.InsertPtr(new Tag("fieldset",500,false));
	Tags.InsertPtr(new Tag("legend",500,false));

	// Structure Tags
	Tags.InsertPtr(new Tag("table",200,false));
	Tags.InsertPtr(new Tag("caption",210,false));
	Tags.InsertPtr(new Tag("thead",210,false));
	Tags.InsertPtr(new Tag("tfoot",210,false));
	Tags.InsertPtr(new Tag("tbody",210,false));
	Tags.InsertPtr(new Tag("tr",220,false));
	Tags.InsertPtr(new Tag("th",220,false));
	Tags.InsertPtr(new Tag("td",230,false));
	Tags.InsertPtr(new Tag("ul",240,false));
	Tags.InsertPtr(new Tag("ol",240,false));
	Tags.InsertPtr(new Tag("dl",240,false));
	Tags.InsertPtr(new Tag("li",250,false));
	Tags.InsertPtr(new Tag("dt",250,false));
	Tags.InsertPtr(new Tag("dd",250,false));
	Tags.InsertPtr(new Tag("colgroup",500,false));
	Tags.InsertPtr(new Tag("col",500,false));


	// Paragraph Tag
	Tags.InsertPtr(new Tag("h1",300,false));
	Tags.InsertPtr(new Tag("h2",310,false));
	Tags.InsertPtr(new Tag("h3",320,false));
	Tags.InsertPtr(new Tag("h4",330,false));
	Tags.InsertPtr(new Tag("h5",340,false));
	Tags.InsertPtr(new Tag("h6",350,false));
	Tags.InsertPtr(new Tag("p",360,false));


	// Low level tags
	Tags.InsertPtr(new Tag("sup",500,false));
	Tags.InsertPtr(new Tag("sub",500,false));
	Tags.InsertPtr(new Tag("br",500,true));
	Tags.InsertPtr(new Tag("nobr",500,false));
	Tags.InsertPtr(new Tag("hr",500,true));
	Tags.InsertPtr(new Tag("font",500,false));
	Tags.InsertPtr(new Tag("pre",500,false));
	Tags.InsertPtr(new Tag("img",500,true)); //? deprecated
	Tags.InsertPtr(new Tag("object",500,false));
	Tags.InsertPtr(new Tag("param",500,false));
	Tags.InsertPtr(new Tag("embed",500,false));
	Tags.InsertPtr(new Tag("noembed",500,false));
	Tags.InsertPtr(new Tag("map",500,false));
	Tags.InsertPtr(new Tag("area",500,false));
	Tags.InsertPtr(new Tag("center",500,false));
	Tags.InsertPtr(new Tag("em",500,false));
	Tags.InsertPtr(new Tag("strong",500,false));
	Tags.InsertPtr(new Tag("dfn",500,false));
	Tags.InsertPtr(new Tag("code",500,false));
	Tags.InsertPtr(new Tag("samp",500,false));
	Tags.InsertPtr(new Tag("kbd",500,false));
	Tags.InsertPtr(new Tag("var",500,false));
	Tags.InsertPtr(new Tag("cite",500,false));
	Tags.InsertPtr(new Tag("abbr",500,false));
	Tags.InsertPtr(new Tag("acronym",500,false));
	Tags.InsertPtr(new Tag("i",500,false));
	Tags.InsertPtr(new Tag("tt",500,false));
	Tags.InsertPtr(new Tag("big",500,false));
	Tags.InsertPtr(new Tag("small",500,false));
	Tags.InsertPtr(new Tag("strike",500,false));
	Tags.InsertPtr(new Tag("b",500,false));
	Tags.InsertPtr(new Tag("u",500,false));  //deprecated
	Tags.InsertPtr(new Tag("s",500,false));  //deprecated
	Tags.InsertPtr(new Tag("applet",500,false));  //deprecated
	Tags.InsertPtr(new Tag("a",500,false));
	Tags.InsertPtr(new Tag("blockquote",500,false));
	Tags.InsertPtr(new Tag("q",500,false));
	Tags.InsertPtr(new Tag("script",500,false));
	Tags.InsertPtr(new Tag("noscript",500,false));
}


//------------------------------------------------------------------------------
void RHTMLFile::BeginTag(const RString& namespaceURI, const RString& lName, const RString& name,RContainer<RXMLAttr,true,true>& attrs) throw(RIOException)
{
	RString htmlName=name;
	Tag* tag;
	Tag* tag2;
	RXMLTag* Search;

	// if HTML closing tag found -> Nothing to do
	if(FoundClosingHTML)
		return;

	// HTML is not case sensitive
	htmlName=htmlName.ToLower();

	// Valid HTML Tag?
	tag=Tags.GetPtr<const RString>(htmlName);
	if(!tag)
		throw RIOException(this,"Unknown opening HTML Tag '"+htmlName+"'.");

	// If the current tag is deeper than this one -> close it
	if(CurTag)
	{
		tag2=Tags.GetPtr<const RString>(CurTag->GetName());
		if((tag->Level<tag2->Level)||((tag==tag2)&&(tag->Level!=500)))
		{
			// Find the first parent with a higher Level
			Search=CurTag;
			while(Search&&(tag->Level<=tag2->Level))
			{
				Search=Search->GetParent();
				if(Search)
					tag2=Tags.GetPtr<const RString>(Search->GetName());
			}
			if(Search)
				CurTag=Search;
		}

	}

	// Treat the beginning tag
	RXMLFile::BeginTag(namespaceURI,lName,htmlName,attrs);

	//Skip content if cur tag == 'SCRIPT'
	if(!htmlName.Compare("script"))
	{
		SkipTagContent(htmlName);
		CurTagClosing=true;
	}

	// Verify that is not a single tag in HTML
	if(tag->Single)
		CurTagClosing=true;
}


//------------------------------------------------------------------------------
void RHTMLFile::EndTag(const RString& namespaceURI, const RString& lName, const RString& name) throw(RIOException)
{
	RString htmlName=name;
	Tag* tag;
	Tag* tag2;
	RXMLTag* Search;

	// if HTML closing tag found -> Nothing to do
	if(FoundClosingHTML)
		return;

	// A Current tag must exist
	if(!CurTag)
		throw RIOException(this,"No current tag");

	// HTML is not case sensitive
	htmlName=htmlName.ToLower();

	// Valid HTML Tag?
	tag2=Tags.GetPtr<const RString>(CurTag->GetName());
	tag=Tags.GetPtr<const RString>(htmlName);
	if(!tag)
		throw RIOException(this,"Unknown closing HTML Tag '"+htmlName+"'.");

	// If the closing tag is the not same as the current open one
	// -> find the right tag to close
	if((tag!=tag2)&&CurTagClosing)
	{
		// If the founding tag is deeper that the the one to close -> Skip it
		// Else -> Find the tag to close
		if(tag->Level<tag2->Level)
		{
			// Find the first parent with a higher Level
			Search=CurTag;
			while(Search&&(tag->Level<=tag2->Level))
			{
				Search=Search->GetParent();
				if(Search)
					tag2=Tags.GetPtr<const RString>(Search->GetName());
			}
			if(Search)
				CurTag=Search;
		}
		else
		{
			// The name of the tag to close is the current one.
			htmlName=CurTag->GetName();
			Search=0;
		}
	}
	else
		Search=0;

	// Treat the ending tag
	if(!Search)
	{
		if(htmlName=="html")
			FoundClosingHTML=true;
		RXMLFile::EndTag(namespaceURI,lName,htmlName);
	}
}


//------------------------------------------------------------------------------
void RHTMLFile::Text(const RString& text) throw(RIOException)
{
	// if HTML closing tag found -> Nothing to do
	if(FoundClosingHTML)
		return;

	// A Current tag must exist
	if(!CurTag)
		throw RIOException(this,"No current tag");

	RXMLFile::Text(text);
}


//------------------------------------------------------------------------------
void RHTMLFile::SkipTagContent(const RString& tag)throw(RIOException)
{
	RString tagToFind("</");

	tagToFind+=tag;
	tagToFind+=">";
	while((!Cur.IsNull()&&(!CurString(tagToFind,false))))
	{
		Next();
	}

	//The End Tag is found ->skip it ( /tag> )
	unsigned int skiplen=tag.GetLen()+2;
	while((!Cur.IsNull())&&(skiplen--))
	{
		Next();
	}
}


//------------------------------------------------------------------------------
bool RHTMLFile::OnlyQuote(void)
{
	return(false);
}


//------------------------------------------------------------------------------
bool RHTMLFile::InvalidXMLCodeAccept(void)
{
	return(true);
}


//------------------------------------------------------------------------------
RHTMLFile::~RHTMLFile(void)
{
}
