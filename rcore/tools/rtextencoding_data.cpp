/*

	R Project Library

	RTextEnconding_data.cpp

	Genereic Text Encoding Scheme - Data.

	Copyright 2004 by the Universit� Libre de Bruxelles.

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


/*
	These are the official names for character sets that may be used in
	the Internet and may be referred to in Internet documentation.
	These tables are generated from the reference file
	http://www.iana.org/assignments/character-sets
*/


//------------------------------------------------------------------------------
// All official encoding names and their code
const EncodingName EncodingNames[]={
	{"adobe-standard-encoding",2005},
	{"adobe-symbol-encoding",2020},
	{"amiga-1251",2104},
	{"ansi_x3.110-1983",74},
	{"ansi_x3.4-1968",3},
	{"asmo_449",65},
	{"big5",2026},
	{"big5-hkscs",2101},
	{"bocu-1",1020},
	{"bs_4730",20},
	{"bs_viewdata",50},
	{"cesu-8",1016},
	{"csa_z243.4-1985-1",78},
	{"csa_z243.4-1985-2",79},
	{"csa_z243.4-1985-gr",80},
	{"csn_369103",86},
	{"dec-mcs",2008},
	{"din_66003",24},
	{"dk-us",101},
	{"ds_2089",99},
	{"ebcdic-at-de",2064},
	{"ebcdic-at-de-a",2065},
	{"ebcdic-ca-fr",2066},
	{"ebcdic-dk-no",2067},
	{"ebcdic-dk-no-a",2068},
	{"ebcdic-es",2074},
	{"ebcdic-es-a",2075},
	{"ebcdic-es-s",2076},
	{"ebcdic-fi-se",2069},
	{"ebcdic-fi-se-a",2070},
	{"ebcdic-fr",2071},
	{"ebcdic-it",2072},
	{"ebcdic-pt",2073},
	{"ebcdic-uk",2077},
	{"ebcdic-us",2078},
	{"ecma-cyrillic",77},
	{"es",23},
	{"es2",61},
	{"euc-kr",38},
	{"extended_unix_code_fixed_width_for_japanese",19},
	{"extended_unix_code_packed_format_for_japanese",18},
	{"gb18030",114},
	{"gb2312",2025},
	{"gb_1988-80",56},
	{"gb_2312-80",57},
	{"gbk",113},
	{"gost_19768-74",94},
	{"greek-ccitt",91},
	{"greek7",64},
	{"greek7-old",44},
	{"hp-desktop",2021},
	{"hp-legal",2017},
	{"hp-math8",2019},
	{"hp-pi-font",2018},
	{"hp-roman8",2004},
	{"hz-gb-2312",2085},
	{"ibm-symbols",2015},
	{"ibm-thai",2016},
	{"ibm00858",2089},
	{"ibm00924",2090},
	{"ibm01140",2091},
	{"ibm01141",2092},
	{"ibm01142",2093},
	{"ibm01143",2094},
	{"ibm01144",2095},
	{"ibm01145",2096},
	{"ibm01146",2097},
	{"ibm01147",2098},
	{"ibm01148",2099},
	{"ibm01149",2100},
	{"ibm037",2028},
	{"ibm038",2029},
	{"ibm1026",2063},
	{"ibm1047",2102},
	{"ibm273",2030},
	{"ibm274",2031},
	{"ibm275",2032},
	{"ibm277",2033},
	{"ibm278",2034},
	{"ibm280",2035},
	{"ibm281",2036},
	{"ibm284",2037},
	{"ibm285",2038},
	{"ibm290",2039},
	{"ibm297",2040},
	{"ibm420",2041},
	{"ibm423",2042},
	{"ibm424",2043},
	{"ibm437",2011},
	{"ibm500",2044},
	{"ibm775",2087},
	{"ibm850",2009},
	{"ibm851",2045},
	{"ibm852",2010},
	{"ibm855",2046},
	{"ibm857",2047},
	{"ibm860",2048},
	{"ibm861",2049},
	{"ibm862",2013},
	{"ibm863",2050},
	{"ibm864",2051},
	{"ibm865",2052},
	{"ibm866",2086},
	{"ibm868",2053},
	{"ibm869",2054},
	{"ibm870",2055},
	{"ibm871",2056},
	{"ibm880",2057},
	{"ibm891",2058},
	{"ibm903",2059},
	{"ibm904",2060},
	{"ibm905",2061},
	{"ibm918",2062},
	{"iec_p27-1",88},
	{"inis",51},
	{"inis-8",52},
	{"inis-cyrillic",53},
	{"invariant",29},
	{"iso-10646-j-1",0},
	{"iso-10646-ucs-2",1000},
	{"iso-10646-ucs-4",1001},
	{"iso-10646-ucs-basic",1002},
	{"iso-10646-unicode-latin1",1003},
	{"iso-10646-utf-1",27},
	{"iso-2022-cn",104},
	{"iso-2022-cn-ext",105},
	{"iso-2022-jp",39},
	{"iso-2022-jp-2",40},
	{"iso-2022-kr",37},
	{"iso-8859-1-windows-3.0-latin-1",2000},
	{"iso-8859-1-windows-3.1-latin-1",2001},
	{"iso-8859-10",13},
	{"iso-8859-13",109},
	{"iso-8859-14",110},
	{"iso-8859-15",111},
	{"iso-8859-16",112},
	{"iso-8859-2-windows-latin-2",2002},
	{"iso-8859-9-windows-latin-5",2003},
	{"iso-ir-90",66},
	{"iso-unicode-ibm-1261",1005},
	{"iso-unicode-ibm-1264",1008},
	{"iso-unicode-ibm-1265",1009},
	{"iso-unicode-ibm-1268",1006},
	{"iso-unicode-ibm-1276",1007},
	{"iso_10367-box",96},
	{"iso_2033-1983",73},
	{"iso_5427",48},
	{"iso_5427:1981",54},
	{"iso_5428:1980",55},
	{"iso_646.basic:1983",28},
	{"iso_646.irv:1983",30},
	{"iso_6937-2-25",93},
	{"iso_6937-2-add",14},
	{"iso_8859-1:1987",4},
	{"iso_8859-2:1987",5},
	{"iso_8859-3:1988",6},
	{"iso_8859-4:1988",7},
	{"iso_8859-5:1988",8},
	{"iso_8859-6-e",81},
	{"iso_8859-6-i",82},
	{"iso_8859-6:1987",9},
	{"iso_8859-7:1987",10},
	{"iso_8859-8-e",84},
	{"iso_8859-8-i",85},
	{"iso_8859-8:1988",11},
	{"iso_8859-9:1989",12},
	{"iso_8859-supp",95},
	{"it",22},
	{"jis_c6220-1969-jp",41},
	{"jis_c6220-1969-ro",42},
	{"jis_c6226-1978",49},
	{"jis_c6226-1983",63},
	{"jis_c6229-1984-a",67},
	{"jis_c6229-1984-b",68},
	{"jis_c6229-1984-b-add",69},
	{"jis_c6229-1984-hand",70},
	{"jis_c6229-1984-hand-add",71},
	{"jis_c6229-1984-kana",72},
	{"jis_encoding",16},
	{"jis_x0201",15},
	{"jis_x0212-1990",98},
	{"jus_i.b1.002",87},
	{"jus_i.b1.003-mac",90},
	{"jus_i.b1.003-serb",89},
	{"koi7-switched",2105},
	{"koi8-r",2084},
	{"koi8-u",2088},
	{"ks_c_5601-1987",36},
	{"ksc5636",102},
	{"latin-greek",45},
	{"latin-greek-1",47},
	{"latin-lap",97},
	{"macintosh",2027},
	{"microsoft-publishing",2023},
	{"mnem",2081},
	{"mnemonic",2080},
	{"msz_7795.3",62},
	{"nats-dano",33},
	{"nats-dano-add",34},
	{"nats-sefi",31},
	{"nats-sefi-add",32},
	{"nc_nc00-10:81",92},
	{"nf_z_62-010",26},
	{"nf_z_62-010_(1973)",46},
	{"ns_4551-1",25},
	{"ns_4551-2",58},
	{"osd_ebcdic_df03_irv",116},
	{"osd_ebcdic_df04_1",117},
	{"osd_ebcdic_df04_15",115},
	{"pc8-danish-norwegian",2012},
	{"pc8-turkish",2014},
	{"pt",43},
	{"pt2",60},
	{"ptcp154",2103},
	{"scsu",1011},
	{"sen_850200_b",35},
	{"sen_850200_c",21},
	{"shift_jis",17},
	{"t.101-g2",83},
	{"t.61-7bit",75},
	{"t.61-8bit",76},
	{"tis-620",2259},
	{"unicode-1-1",1010},
	{"unicode-1-1-utf-7",103},
	{"unknown-8bit",2079},
	{"us-dk",100},
	{"utf-16",1015},
	{"utf-16be",1013},
	{"utf-16le",1014},
	{"utf-32",1017},
	{"utf-32be",1018},
	{"utf-32le",1019},
	{"utf-7",1012},
	{"utf-8",106},
	{"ventura-international",2007},
	{"ventura-math",2022},
	{"ventura-us",2006},
	{"videotex-suppl",59},
	{"viqr",2083},
	{"viscii",2082},
	{"windows-1250",2250},
	{"windows-1251",2251},
	{"windows-1252",2252},
	{"windows-1253",2253},
	{"windows-1254",2254},
	{"windows-1255",2255},
	{"windows-1256",2256},
	{"windows-1257",2257},
	{"windows-1258",2258},
	{"windows-31j",2024}
};


//------------------------------------------------------------------------------
// Different aliases for the official encoding names
const EncodingAlias EncodingAliases[]={
	{"437","ibm437"},
	{"850","ibm850"},
	{"851","ibm851"},
	{"852","ibm852"},
	{"855","ibm855"},
	{"857","ibm857"},
	{"860","ibm860"},
	{"861","ibm861"},
	{"862","ibm862"},
	{"863","ibm863"},
	{"865","ibm865"},
	{"866","ibm866"},
	{"869","ibm869"},
	{"904","ibm904"},
	{":","koi7-switched"},
	{"adobe-standard-encoding","adobe-standard-encoding"},
	{"adobe-symbol-encoding","adobe-symbol-encoding"},
	{"ami-1251","amiga-1251"},
	{"ami1251","amiga-1251"},
	{"amiga-1251","amiga-1251"},
	{"amiga1251","amiga-1251"},
	{"ansi_x3.110-1983","ansi_x3.110-1983"},
	{"ansi_x3.4-1968","ansi_x3.4-1968"},
	{"ansi_x3.4-1986","ansi_x3.4-1968"},
	{"arabic","iso_8859-6:1987"},
	{"arabic7","asmo_449"},
	{"ascii","ansi_x3.4-1968"},
	{"asmo-708","iso_8859-6:1987"},
	{"asmo_449","asmo_449"},
	{"big5","big5"},
	{"big5-hkscs","big5-hkscs"},
	{"bocu-1","bocu-1"},
	{"bs_4730","bs_4730"},
	{"bs_viewdata","bs_viewdata"},
	{"ca","csa_z243.4-1985-1"},
	{"ccsid00858","ibm00858"},
	{"ccsid00924","ibm00924"},
	{"ccsid01140","ibm01140"},
	{"ccsid01141","ibm01141"},
	{"ccsid01142","ibm01142"},
	{"ccsid01143","ibm01143"},
	{"ccsid01144","ibm01144"},
	{"ccsid01145","ibm01145"},
	{"ccsid01146","ibm01146"},
	{"ccsid01147","ibm01147"},
	{"ccsid01148","ibm01148"},
	{"ccsid01149","ibm01149"},
	{"cesu-8","cesu-8"},
	{"chinese","gb_2312-80"},
	{"cn","gb_1988-80"},
	{"cp-ar","ibm868"},
	{"cp-gr","ibm869"},
	{"cp-is","ibm861"},
	{"cp00858","ibm00858"},
	{"cp00924","ibm00924"},
	{"cp01140","ibm01140"},
	{"cp01141","ibm01141"},
	{"cp01142","ibm01142"},
	{"cp01143","ibm01143"},
	{"cp01144","ibm01144"},
	{"cp01145","ibm01145"},
	{"cp01146","ibm01146"},
	{"cp01147","ibm01147"},
	{"cp01148","ibm01148"},
	{"cp01149","ibm01149"},
	{"cp037","ibm037"},
	{"cp038","ibm038"},
	{"cp1026","ibm1026"},
	{"cp154","ptcp154"},
	{"cp273","ibm273"},
	{"cp274","ibm274"},
	{"cp275","ibm275"},
	{"cp278","ibm278"},
	{"cp280","ibm280"},
	{"cp281","ibm281"},
	{"cp284","ibm284"},
	{"cp285","ibm285"},
	{"cp290","ibm290"},
	{"cp297","ibm297"},
	{"cp367","ansi_x3.4-1968"},
	{"cp420","ibm420"},
	{"cp423","ibm423"},
	{"cp424","ibm424"},
	{"cp437","ibm437"},
	{"cp500","ibm500"},
	{"cp775","ibm775"},
	{"cp819","iso_8859-1:1987"},
	{"cp850","ibm850"},
	{"cp851","ibm851"},
	{"cp852","ibm852"},
	{"cp855","ibm855"},
	{"cp857","ibm857"},
	{"cp860","ibm860"},
	{"cp861","ibm861"},
	{"cp862","ibm862"},
	{"cp863","ibm863"},
	{"cp864","ibm864"},
	{"cp865","ibm865"},
	{"cp866","ibm866"},
	{"cp868","ibm868"},
	{"cp869","ibm869"},
	{"cp870","ibm870"},
	{"cp871","ibm871"},
	{"cp880","ibm880"},
	{"cp891","ibm891"},
	{"cp903","ibm903"},
	{"cp904","ibm904"},
	{"cp905","ibm905"},
	{"cp918","ibm918"},
	{"cp936","gbk"},
	{"csa7-1","csa_z243.4-1985-1"},
	{"csa7-2","csa_z243.4-1985-2"},
	{"csa_t500-1983","ansi_x3.110-1983"},
	{"csa_z243.4-1985-1","csa_z243.4-1985-1"},
	{"csa_z243.4-1985-2","csa_z243.4-1985-2"},
	{"csa_z243.4-1985-gr","csa_z243.4-1985-gr"},
	{"csadobestandardencoding","adobe-standard-encoding"},
	{"csascii","ansi_x3.4-1968"},
	{"csbig5","big5"},
	{"csbocu-1","bocu-1"},
	{"cscesu-8","cesu-8"},
	{"csdecmcs","dec-mcs"},
	{"csdkus","dk-us"},
	{"csebcdicatdea","ebcdic-at-de-a"},
	{"csebcdiccafr","ebcdic-ca-fr"},
	{"csebcdicdkno","ebcdic-dk-no"},
	{"csebcdicdknoa","ebcdic-dk-no-a"},
	{"csebcdices","ebcdic-es"},
	{"csebcdicesa","ebcdic-es-a"},
	{"csebcdicess","ebcdic-es-s"},
	{"csebcdicfise","ebcdic-fi-se"},
	{"csebcdicfisea","ebcdic-fi-se-a"},
	{"csebcdicfr","ebcdic-fr"},
	{"csebcdicit","ebcdic-it"},
	{"csebcdicpt","ebcdic-pt"},
	{"csebcdicuk","ebcdic-uk"},
	{"csebcdicus","ebcdic-us"},
	{"cseucfixwidjapanese","extended_unix_code_fixed_width_for_japanese"},
	{"cseuckr","euc-kr"},
	{"cseucpkdfmtjapanese","extended_unix_code_packed_format_for_japanese"},
	{"csgb2312","gb2312"},
	{"cshalfwidthkatakana","jis_x0201"},
	{"cshpdesktop","hp-desktop"},
	{"cshplegal","hp-legal"},
	{"cshpmath8","hp-math8"},
	{"cshppifont","hp-pi-font"},
	{"cshppsmath","adobe-symbol-encoding"},
	{"cshproman8","hp-roman8"},
	{"csibbm904","ibm904"},
	{"csibm037","ibm037"},
	{"csibm038","ibm038"},
	{"csibm1026","ibm1026"},
	{"csibm273","ibm273"},
	{"csibm274","ibm274"},
	{"csibm275","ibm275"},
	{"csibm277","ibm277"},
	{"csibm278","ibm278"},
	{"csibm280","ibm280"},
	{"csibm281","ibm281"},
	{"csibm284","ibm284"},
	{"csibm285","ibm285"},
	{"csibm290","ibm290"},
	{"csibm297","ibm297"},
	{"csibm420","ibm420"},
	{"csibm423","ibm423"},
	{"csibm424","ibm424"},
	{"csibm500","ibm500"},
	{"csibm851","ibm851"},
	{"csibm855","ibm855"},
	{"csibm857","ibm857"},
	{"csibm860","ibm860"},
	{"csibm861","ibm861"},
	{"csibm863","ibm863"},
	{"csibm864","ibm864"},
	{"csibm865","ibm865"},
	{"csibm866","ibm866"},
	{"csibm868","ibm868"},
	{"csibm869","ibm869"},
	{"csibm870","ibm870"},
	{"csibm871","ibm871"},
	{"csibm880","ibm880"},
	{"csibm891","ibm891"},
	{"csibm903","ibm903"},
	{"csibm905","ibm905"},
	{"csibm918","ibm918"},
	{"csibmebcdicatde","ebcdic-at-de"},
	{"csibmsymbols","ibm-symbols"},
	{"csibmthai","ibm-thai"},
	{"csinvariant","invariant"},
	{"csiso102t617bit","t.61-7bit"},
	{"csiso10367box","iso_10367-box"},
	{"csiso103t618bit","t.61-8bit"},
	{"csiso10646utf1","iso-10646-utf-1"},
	{"csiso10swedish","sen_850200_b"},
	{"csiso111ecmacyrillic","ecma-cyrillic"},
	{"csiso11swedishfornames","sen_850200_c"},
	{"csiso121canadian1","csa_z243.4-1985-1"},
	{"csiso122canadian2","csa_z243.4-1985-2"},
	{"csiso123csaz24341985gr","csa_z243.4-1985-gr"},
	{"csiso128t101g2","t.101-g2"},
	{"csiso139csn369103","csn_369103"},
	{"csiso13jisc6220jp","jis_c6220-1969-jp"},
	{"csiso141jusib1002","jus_i.b1.002"},
	{"csiso143iecp271","iec_p27-1"},
	{"csiso146serbian","jus_i.b1.003-serb"},
	{"csiso147macedonian","jus_i.b1.003-mac"},
	{"csiso14jisc6220ro","jis_c6220-1969-ro"},
	{"csiso150","greek-ccitt"},
	{"csiso150greekccitt","greek-ccitt"},
	{"csiso151cuba","nc_nc00-10:81"},
	{"csiso153gost1976874","gost_19768-74"},
	{"csiso158lap","latin-lap"},
	{"csiso159jisx02121990","jis_x0212-1990"},
	{"csiso15italian","it"},
	{"csiso16portuguese","pt"},
	{"csiso17spanish","es"},
	{"csiso18greek7old","greek7-old"},
	{"csiso19latingreek","latin-greek"},
	{"csiso2022jp","iso-2022-jp"},
	{"csiso2022jp2","iso-2022-jp-2"},
	{"csiso2022kr","iso-2022-kr"},
	{"csiso2033","iso_2033-1983"},
	{"csiso21german","din_66003"},
	{"csiso25french","nf_z_62-010_(1973)"},
	{"csiso27latingreek1","latin-greek-1"},
	{"csiso2intlrefversion","iso_646.irv:1983"},
	{"csiso42jisc62261978","jis_c6226-1978"},
	{"csiso47bsviewdata","bs_viewdata"},
	{"csiso49inis","inis"},
	{"csiso4unitedkingdom","bs_4730"},
	{"csiso50inis8","inis-8"},
	{"csiso51iniscyrillic","inis-cyrillic"},
	{"csiso5427cyrillic","iso_5427"},
	{"csiso5428greek","iso_5428:1980"},
	{"csiso57gb1988","gb_1988-80"},
	{"csiso58gb231280","gb_2312-80"},
	{"csiso60danishnorwegian","ns_4551-1"},
	{"csiso60norwegian1","ns_4551-1"},
	{"csiso61norwegian2","ns_4551-2"},
	{"csiso646basic1983","iso_646.basic:1983"},
	{"csiso646danish","ds_2089"},
	{"csiso6937add","iso_6937-2-25"},
	{"csiso69french","nf_z_62-010"},
	{"csiso70videotexsupp1","videotex-suppl"},
	{"csiso84portuguese2","pt2"},
	{"csiso85spanish2","es2"},
	{"csiso86hungarian","msz_7795.3"},
	{"csiso87jisx0208","jis_c6226-1983"},
	{"csiso88596e","iso_8859-6-e"},
	{"csiso88596i","iso_8859-6-i"},
	{"csiso88598e","iso_8859-8-e"},
	{"csiso88598i","iso_8859-8-i"},
	{"csiso8859supp","iso_8859-supp"},
	{"csiso88greek7","greek7"},
	{"csiso89asmo449","asmo_449"},
	{"csiso90","iso-ir-90"},
	{"csiso91jisc62291984a","jis_c6229-1984-a"},
	{"csiso92jisc62991984b","jis_c6229-1984-b"},
	{"csiso93jis62291984badd","jis_c6229-1984-b-add"},
	{"csiso94jis62291984hand","jis_c6229-1984-hand"},
	{"csiso95jis62291984handadd","jis_c6229-1984-hand-add"},
	{"csiso96jisc62291984kana","jis_c6229-1984-kana"},
	{"csiso99naplps","ansi_x3.110-1983"},
	{"csisolatin1","iso_8859-1:1987"},
	{"csisolatin2","iso_8859-2:1987"},
	{"csisolatin3","iso_8859-3:1988"},
	{"csisolatin4","iso_8859-4:1988"},
	{"csisolatin5","iso_8859-9:1989"},
	{"csisolatin6","iso-8859-10"},
	{"csisolatinarabic","iso_8859-6:1987"},
	{"csisolatincyrillic","iso_8859-5:1988"},
	{"csisolatingreek","iso_8859-7:1987"},
	{"csisolatinhebrew","iso_8859-8:1988"},
	{"csisotextcomm","iso_6937-2-add"},
	{"csjisencoding","jis_encoding"},
	{"cskoi8r","koi8-r"},
	{"csksc56011987","ks_c_5601-1987"},
	{"csksc5636","ksc5636"},
	{"csmacintosh","macintosh"},
	{"csmicrosoftpublishing","microsoft-publishing"},
	{"csmnem","mnem"},
	{"csmnemonic","mnemonic"},
	{"csn_369103","csn_369103"},
	{"csnatsdano","nats-dano"},
	{"csnatsdanoadd","nats-dano-add"},
	{"csnatssefi","nats-sefi"},
	{"csnatssefiadd","nats-sefi-add"},
	{"cspc775baltic","ibm775"},
	{"cspc850multilingual","ibm850"},
	{"cspc862latinhebrew","ibm862"},
	{"cspc8codepage437","ibm437"},
	{"cspc8danishnorwegian","pc8-danish-norwegian"},
	{"cspc8turkish","pc8-turkish"},
	{"cspcp852","ibm852"},
	{"csptcp154","ptcp154"},
	{"csshiftjis","shift_jis"},
	{"csucs4","iso-10646-ucs-4"},
	{"csunicode","iso-10646-ucs-2"},
	{"csunicode11","unicode-1-1"},
	{"csunicode11utf7","unicode-1-1-utf-7"},
	{"csunicodeascii","iso-10646-ucs-basic"},
	{"csunicodeibm1261","iso-unicode-ibm-1261"},
	{"csunicodeibm1264","iso-unicode-ibm-1264"},
	{"csunicodeibm1265","iso-unicode-ibm-1265"},
	{"csunicodeibm1268","iso-unicode-ibm-1268"},
	{"csunicodeibm1276","iso-unicode-ibm-1276"},
	{"csunicodelatin1","iso-10646-unicode-latin1"},
	{"csunknown8bit","unknown-8bit"},
	{"csusdk","us-dk"},
	{"csventurainternational","ventura-international"},
	{"csventuramath","ventura-math"},
	{"csventuraus","ventura-us"},
	{"csviqr","viqr"},
	{"csviscii","viscii"},
	{"cswindows30latin1","iso-8859-1-windows-3.0-latin-1"},
	{"cswindows31j","windows-31j"},
	{"cswindows31latin1","iso-8859-1-windows-3.1-latin-1"},
	{"cswindows31latin2","iso-8859-2-windows-latin-2"},
	{"cswindows31latin5","iso-8859-9-windows-latin-5"},
	{"cuba","nc_nc00-10:81"},
	{"cyrillic","iso_8859-5:1988"},
	{"cyrillic-asian","ptcp154"},
	{"de","din_66003"},
	{"dec","dec-mcs"},
	{"dec-mcs","dec-mcs"},
	{"din_66003","din_66003"},
	{"dk","ds_2089"},
	{"dk-us","dk-us"},
	{"ds2089","ds_2089"},
	{"ds_2089","ds_2089"},
	{"e13b","iso_2033-1983"},
	{"ebcdic-at-de","ebcdic-at-de"},
	{"ebcdic-at-de-a","ebcdic-at-de-a"},
	{"ebcdic-be","ibm274"},
	{"ebcdic-br","ibm275"},
	{"ebcdic-ca-fr","ebcdic-ca-fr"},
	{"ebcdic-cp-ar1","ibm420"},
	{"ebcdic-cp-ar2","ibm918"},
	{"ebcdic-cp-be","ibm500"},
	{"ebcdic-cp-ca","ibm037"},
	{"ebcdic-cp-ch","ibm500"},
	{"ebcdic-cp-dk","ibm277"},
	{"ebcdic-cp-es","ibm284"},
	{"ebcdic-cp-fi","ibm278"},
	{"ebcdic-cp-fr","ibm297"},
	{"ebcdic-cp-gb","ibm285"},
	{"ebcdic-cp-gr","ibm423"},
	{"ebcdic-cp-he","ibm424"},
	{"ebcdic-cp-is","ibm871"},
	{"ebcdic-cp-it","ibm280"},
	{"ebcdic-cp-nl","ibm037"},
	{"ebcdic-cp-no","ibm277"},
	{"ebcdic-cp-roece","ibm870"},
	{"ebcdic-cp-se","ibm278"},
	{"ebcdic-cp-tr","ibm905"},
	{"ebcdic-cp-us","ibm037"},
	{"ebcdic-cp-wt","ibm037"},
	{"ebcdic-cp-yu","ibm870"},
	{"ebcdic-cyrillic","ibm880"},
	{"ebcdic-de-273+euro","ibm01141"},
	{"ebcdic-dk-277+euro","ibm01142"},
	{"ebcdic-dk-no","ebcdic-dk-no"},
	{"ebcdic-dk-no-a","ebcdic-dk-no-a"},
	{"ebcdic-es","ebcdic-es"},
	{"ebcdic-es-284+euro","ibm01145"},
	{"ebcdic-es-a","ebcdic-es-a"},
	{"ebcdic-es-s","ebcdic-es-s"},
	{"ebcdic-fi-278+euro","ibm01143"},
	{"ebcdic-fi-se","ebcdic-fi-se"},
	{"ebcdic-fi-se-a","ebcdic-fi-se-a"},
	{"ebcdic-fr","ebcdic-fr"},
	{"ebcdic-fr-297+euro","ibm01147"},
	{"ebcdic-gb-285+euro","ibm01146"},
	{"ebcdic-int","ibm038"},
	{"ebcdic-international-500+euro","ibm01148"},
	{"ebcdic-is-871+euro","ibm01149"},
	{"ebcdic-it","ebcdic-it"},
	{"ebcdic-it-280+euro","ibm01144"},
	{"ebcdic-jp-e","ibm281"},
	{"ebcdic-jp-kana","ibm290"},
	{"ebcdic-latin9--euro","ibm00924"},
	{"ebcdic-no-277+euro","ibm01142"},
	{"ebcdic-pt","ebcdic-pt"},
	{"ebcdic-se-278+euro","ibm01143"},
	{"ebcdic-uk","ebcdic-uk"},
	{"ebcdic-us","ebcdic-us"},
	{"ebcdic-us-37+euro","ibm01140"},
	{"ecma-114","iso_8859-6:1987"},
	{"ecma-118","iso_8859-7:1987"},
	{"ecma-cyrillic","ecma-cyrillic"},
	{"elot_928","iso_8859-7:1987"},
	{"es","es"},
	{"es2","es2"},
	{"euc-jp","extended_unix_code_packed_format_for_japanese"},
	{"euc-kr","euc-kr"},
	{"extended_unix_code_fixed_width_for_japanese","extended_unix_code_fixed_width_for_japanese"},
	{"extended_unix_code_packed_format_for_japanese","extended_unix_code_packed_format_for_japanese"},
	{"fi","sen_850200_b"},
	{"fr","nf_z_62-010"},
	{"gb","bs_4730"},
	{"gb18030","gb18030"},
	{"gb2312","gb2312"},
	{"gb_1988-80","gb_1988-80"},
	{"gb_2312-80","gb_2312-80"},
	{"gbk","gbk"},
	{"gost_19768-74","gost_19768-74"},
	{"greek","iso_8859-7:1987"},
	{"greek-ccitt","greek-ccitt"},
	{"greek7","greek7"},
	{"greek7-old","greek7-old"},
	{"greek8","iso_8859-7:1987"},
	{"hebrew","iso_8859-8:1988"},
	{"hp-desktop","hp-desktop"},
	{"hp-legal","hp-legal"},
	{"hp-math8","hp-math8"},
	{"hp-pi-font","hp-pi-font"},
	{"hp-roman8","hp-roman8"},
	{"hu","msz_7795.3"},
	{"hz-gb-2312","hz-gb-2312"},
	{"ibm-1047","ibm1047"},
	{"ibm-symbols","ibm-symbols"},
	{"ibm-thai","ibm-thai"},
	{"ibm00858","ibm00858"},
	{"ibm00924","ibm00924"},
	{"ibm01140","ibm01140"},
	{"ibm01141","ibm01141"},
	{"ibm01142","ibm01142"},
	{"ibm01143","ibm01143"},
	{"ibm01144","ibm01144"},
	{"ibm01145","ibm01145"},
	{"ibm01146","ibm01146"},
	{"ibm01147","ibm01147"},
	{"ibm01148","ibm01148"},
	{"ibm01149","ibm01149"},
	{"ibm037","ibm037"},
	{"ibm038","ibm038"},
	{"ibm1026","ibm1026"},
	{"ibm1047","ibm1047"},
	{"ibm273","ibm273"},
	{"ibm274","ibm274"},
	{"ibm275","ibm275"},
	{"ibm277","ibm277"},
	{"ibm278","ibm278"},
	{"ibm280","ibm280"},
	{"ibm281","ibm281"},
	{"ibm284","ibm284"},
	{"ibm285","ibm285"},
	{"ibm290","ibm290"},
	{"ibm297","ibm297"},
	{"ibm367","ansi_x3.4-1968"},
	{"ibm420","ibm420"},
	{"ibm423","ibm423"},
	{"ibm424","ibm424"},
	{"ibm437","ibm437"},
	{"ibm500","ibm500"},
	{"ibm775","ibm775"},
	{"ibm819","iso_8859-1:1987"},
	{"ibm850","ibm850"},
	{"ibm851","ibm851"},
	{"ibm852","ibm852"},
	{"ibm855","ibm855"},
	{"ibm857","ibm857"},
	{"ibm860","ibm860"},
	{"ibm861","ibm861"},
	{"ibm862","ibm862"},
	{"ibm863","ibm863"},
	{"ibm864","ibm864"},
	{"ibm865","ibm865"},
	{"ibm866","ibm866"},
	{"ibm868","ibm868"},
	{"ibm869","ibm869"},
	{"ibm870","ibm870"},
	{"ibm871","ibm871"},
	{"ibm880","ibm880"},
	{"ibm891","ibm891"},
	{"ibm903","ibm903"},
	{"ibm904","ibm904"},
	{"ibm905","ibm905"},
	{"ibm918","ibm918"},
	{"iec_p27-1","iec_p27-1"},
	{"inis","inis"},
	{"inis-8","inis-8"},
	{"inis-cyrillic","inis-cyrillic"},
	{"invariant","invariant"},
	{"irv","iso_646.irv:1983"},
	{"iso-10646","iso-10646-unicode-latin1"},
	{"iso-10646-j-1","iso-10646-j-1"},
	{"iso-10646-ucs-2","iso-10646-ucs-2"},
	{"iso-10646-ucs-4","iso-10646-ucs-4"},
	{"iso-10646-ucs-basic","iso-10646-ucs-basic"},
	{"iso-10646-unicode-latin1","iso-10646-unicode-latin1"},
	{"iso-10646-utf-1","iso-10646-utf-1"},
	{"iso-2022-cn","iso-2022-cn"},
	{"iso-2022-cn-ext","iso-2022-cn-ext"},
	{"iso-2022-jp","iso-2022-jp"},
	{"iso-2022-jp-2","iso-2022-jp-2"},
	{"iso-2022-kr","iso-2022-kr"},
	{"iso-8859-1","iso_8859-1:1987"},
	{"iso-8859-1-windows-3.0-latin-1","iso-8859-1-windows-3.0-latin-1"},
	{"iso-8859-1-windows-3.1-latin-1","iso-8859-1-windows-3.1-latin-1"},
	{"iso-8859-10","iso-8859-10"},
	{"iso-8859-13","iso-8859-13"},
	{"iso-8859-14","iso-8859-14"},
	{"iso-8859-15","iso-8859-15"},
	{"iso-8859-16","iso-8859-16"},
	{"iso-8859-2","iso_8859-2:1987"},
	{"iso-8859-2-windows-latin-2","iso-8859-2-windows-latin-2"},
	{"iso-8859-3","iso_8859-3:1988"},
	{"iso-8859-4","iso_8859-4:1988"},
	{"iso-8859-5","iso_8859-5:1988"},
	{"iso-8859-6","iso_8859-6:1987"},
	{"iso-8859-6-e","iso_8859-6-e"},
	{"iso-8859-6-i","iso_8859-6-i"},
	{"iso-8859-7","iso_8859-7:1987"},
	{"iso-8859-8","iso_8859-8:1988"},
	{"iso-8859-8-e","iso_8859-8-e"},
	{"iso-8859-8-i","iso_8859-8-i"},
	{"iso-8859-9","iso_8859-9:1989"},
	{"iso-8859-9-windows-latin-5","iso-8859-9-windows-latin-5"},
	{"iso-celtic","iso-8859-14"},
	{"iso-ir-10","sen_850200_b"},
	{"iso-ir-100","iso_8859-1:1987"},
	{"iso-ir-101","iso_8859-2:1987"},
	{"iso-ir-102","t.61-7bit"},
	{"iso-ir-103","t.61-8bit"},
	{"iso-ir-109","iso_8859-3:1988"},
	{"iso-ir-11","sen_850200_c"},
	{"iso-ir-110","iso_8859-4:1988"},
	{"iso-ir-111","ecma-cyrillic"},
	{"iso-ir-121","csa_z243.4-1985-1"},
	{"iso-ir-122","csa_z243.4-1985-2"},
	{"iso-ir-123","csa_z243.4-1985-gr"},
	{"iso-ir-126","iso_8859-7:1987"},
	{"iso-ir-127","iso_8859-6:1987"},
	{"iso-ir-128","t.101-g2"},
	{"iso-ir-13","jis_c6220-1969-jp"},
	{"iso-ir-138","iso_8859-8:1988"},
	{"iso-ir-139","csn_369103"},
	{"iso-ir-14","jis_c6220-1969-ro"},
	{"iso-ir-141","jus_i.b1.002"},
	{"iso-ir-142","iso_6937-2-add"},
	{"iso-ir-143","iec_p27-1"},
	{"iso-ir-144","iso_8859-5:1988"},
	{"iso-ir-146","jus_i.b1.003-serb"},
	{"iso-ir-147","jus_i.b1.003-mac"},
	{"iso-ir-148","iso_8859-9:1989"},
	{"iso-ir-149","ks_c_5601-1987"},
	{"iso-ir-15","it"},
	{"iso-ir-150","greek-ccitt"},
	{"iso-ir-151","nc_nc00-10:81"},
	{"iso-ir-152","iso_6937-2-25"},
	{"iso-ir-153","gost_19768-74"},
	{"iso-ir-154","iso_8859-supp"},
	{"iso-ir-155","iso_10367-box"},
	{"iso-ir-157","iso-8859-10"},
	{"iso-ir-158","latin-lap"},
	{"iso-ir-159","jis_x0212-1990"},
	{"iso-ir-16","pt"},
	{"iso-ir-17","es"},
	{"iso-ir-18","greek7-old"},
	{"iso-ir-19","latin-greek"},
	{"iso-ir-199","iso-8859-14"},
	{"iso-ir-2","iso_646.irv:1983"},
	{"iso-ir-21","din_66003"},
	{"iso-ir-226","iso-8859-16"},
	{"iso-ir-25","nf_z_62-010_(1973)"},
	{"iso-ir-27","latin-greek-1"},
	{"iso-ir-37","iso_5427"},
	{"iso-ir-4","bs_4730"},
	{"iso-ir-42","jis_c6226-1978"},
	{"iso-ir-47","bs_viewdata"},
	{"iso-ir-49","inis"},
	{"iso-ir-50","inis-8"},
	{"iso-ir-51","inis-cyrillic"},
	{"iso-ir-54","iso_5427:1981"},
	{"iso-ir-55","iso_5428:1980"},
	{"iso-ir-57","gb_1988-80"},
	{"iso-ir-58","gb_2312-80"},
	{"iso-ir-6","ansi_x3.4-1968"},
	{"iso-ir-60","ns_4551-1"},
	{"iso-ir-61","ns_4551-2"},
	{"iso-ir-69","nf_z_62-010"},
	{"iso-ir-70","videotex-suppl"},
	{"iso-ir-8-1","nats-sefi"},
	{"iso-ir-8-2","nats-sefi-add"},
	{"iso-ir-84","pt2"},
	{"iso-ir-85","es2"},
	{"iso-ir-86","msz_7795.3"},
	{"iso-ir-87","jis_c6226-1983"},
	{"iso-ir-88","greek7"},
	{"iso-ir-89","asmo_449"},
	{"iso-ir-9-1","nats-dano"},
	{"iso-ir-9-2","nats-dano-add"},
	{"iso-ir-90","iso-ir-90"},
	{"iso-ir-91","jis_c6229-1984-a"},
	{"iso-ir-92","jis_c6229-1984-b"},
	{"iso-ir-93","jis_c6229-1984-b-add"},
	{"iso-ir-94","jis_c6229-1984-hand"},
	{"iso-ir-95","jis_c6229-1984-hand-add"},
	{"iso-ir-96","jis_c6229-1984-kana"},
	{"iso-ir-98","iso_2033-1983"},
	{"iso-ir-99","ansi_x3.110-1983"},
	{"iso-unicode-ibm-1261","iso-unicode-ibm-1261"},
	{"iso-unicode-ibm-1264","iso-unicode-ibm-1264"},
	{"iso-unicode-ibm-1265","iso-unicode-ibm-1265"},
	{"iso-unicode-ibm-1268","iso-unicode-ibm-1268"},
	{"iso-unicode-ibm-1276","iso-unicode-ibm-1276"},
	{"iso5427cyrillic1981","iso_5427:1981"},
	{"iso646-ca","csa_z243.4-1985-1"},
	{"iso646-ca2","csa_z243.4-1985-2"},
	{"iso646-cn","gb_1988-80"},
	{"iso646-cu","nc_nc00-10:81"},
	{"iso646-de","din_66003"},
	{"iso646-dk","ds_2089"},
	{"iso646-es","es"},
	{"iso646-es2","es2"},
	{"iso646-fi","sen_850200_b"},
	{"iso646-fr","nf_z_62-010"},
	{"iso646-fr1","nf_z_62-010_(1973)"},
	{"iso646-gb","bs_4730"},
	{"iso646-hu","msz_7795.3"},
	{"iso646-it","it"},
	{"iso646-jp","jis_c6220-1969-ro"},
	{"iso646-jp-ocr-b","jis_c6229-1984-b"},
	{"iso646-kr","ksc5636"},
	{"iso646-no","ns_4551-1"},
	{"iso646-no2","ns_4551-2"},
	{"iso646-pt","pt"},
	{"iso646-pt2","pt2"},
	{"iso646-se","sen_850200_b"},
	{"iso646-se2","sen_850200_c"},
	{"iso646-us","ansi_x3.4-1968"},
	{"iso646-yu","jus_i.b1.002"},
	{"iso_10367-box","iso_10367-box"},
	{"iso_2033-1983","iso_2033-1983"},
	{"iso_5427","iso_5427"},
	{"iso_5427:1981","iso_5427:1981"},
	{"iso_5428:1980","iso_5428:1980"},
	{"iso_646.basic:1983","iso_646.basic:1983"},
	{"iso_646.irv:1983","iso_646.irv:1983"},
	{"iso_646.irv:1991","ansi_x3.4-1968"},
	{"iso_6937-2-25","iso_6937-2-25"},
	{"iso_6937-2-add","iso_6937-2-add"},
	{"iso_8859-1","iso_8859-1:1987"},
	{"iso_8859-10:1992","iso-8859-10"},
	{"iso_8859-14","iso-8859-14"},
	{"iso_8859-14:1998","iso-8859-14"},
	{"iso_8859-15","iso-8859-15"},
	{"iso_8859-16","iso-8859-16"},
	{"iso_8859-16:2001","iso-8859-16"},
	{"iso_8859-1:1987","iso_8859-1:1987"},
	{"iso_8859-2","iso_8859-2:1987"},
	{"iso_8859-2:1987","iso_8859-2:1987"},
	{"iso_8859-3","iso_8859-3:1988"},
	{"iso_8859-3:1988","iso_8859-3:1988"},
	{"iso_8859-4","iso_8859-4:1988"},
	{"iso_8859-4:1988","iso_8859-4:1988"},
	{"iso_8859-5","iso_8859-5:1988"},
	{"iso_8859-5:1988","iso_8859-5:1988"},
	{"iso_8859-6","iso_8859-6:1987"},
	{"iso_8859-6-e","iso_8859-6-e"},
	{"iso_8859-6-i","iso_8859-6-i"},
	{"iso_8859-6:1987","iso_8859-6:1987"},
	{"iso_8859-7","iso_8859-7:1987"},
	{"iso_8859-7:1987","iso_8859-7:1987"},
	{"iso_8859-8","iso_8859-8:1988"},
	{"iso_8859-8-e","iso_8859-8-e"},
	{"iso_8859-8-i","iso_8859-8-i"},
	{"iso_8859-8:1988","iso_8859-8:1988"},
	{"iso_8859-9","iso_8859-9:1989"},
	{"iso_8859-9:1989","iso_8859-9:1989"},
	{"iso_8859-supp","iso_8859-supp"},
	{"iso_9036","asmo_449"},
	{"it","it"},
	{"jis_c6220-1969","jis_c6220-1969-jp"},
	{"jis_c6220-1969-jp","jis_c6220-1969-jp"},
	{"jis_c6220-1969-ro","jis_c6220-1969-ro"},
	{"jis_c6226-1978","jis_c6226-1978"},
	{"jis_c6226-1983","jis_c6226-1983"},
	{"jis_c6229-1984-a","jis_c6229-1984-a"},
	{"jis_c6229-1984-b","jis_c6229-1984-b"},
	{"jis_c6229-1984-b-add","jis_c6229-1984-b-add"},
	{"jis_c6229-1984-hand","jis_c6229-1984-hand"},
	{"jis_c6229-1984-hand-add","jis_c6229-1984-hand-add"},
	{"jis_c6229-1984-kana","jis_c6229-1984-kana"},
	{"jis_encoding","jis_encoding"},
	{"jis_x0201","jis_x0201"},
	{"jis_x0208-1983","jis_c6226-1983"},
	{"jis_x0212-1990","jis_x0212-1990"},
	{"jp","jis_c6220-1969-ro"},
	{"jp-ocr-a","jis_c6229-1984-a"},
	{"jp-ocr-b","jis_c6229-1984-b"},
	{"jp-ocr-b-add","jis_c6229-1984-b-add"},
	{"jp-ocr-hand","jis_c6229-1984-hand"},
	{"jp-ocr-hand-add","jis_c6229-1984-hand-add"},
	{"js","jus_i.b1.002"},
	{"jus_i.b1.002","jus_i.b1.002"},
	{"jus_i.b1.003-mac","jus_i.b1.003-mac"},
	{"jus_i.b1.003-serb","jus_i.b1.003-serb"},
	{"katakana","jis_c6220-1969-jp"},
	{"koi7-switched","koi7-switched"},
	{"koi8-e","ecma-cyrillic"},
	{"koi8-r","koi8-r"},
	{"koi8-u","koi8-u"},
	{"korean","ks_c_5601-1987"},
	{"ks_c_5601-1987","ks_c_5601-1987"},
	{"ks_c_5601-1989","ks_c_5601-1987"},
	{"ksc5636","ksc5636"},
	{"ksc_5601","ks_c_5601-1987"},
	{"l1","iso_8859-1:1987"},
	{"l10","iso-8859-16"},
	{"l2","iso_8859-2:1987"},
	{"l3","iso_8859-3:1988"},
	{"l4","iso_8859-4:1988"},
	{"l5","iso_8859-9:1989"},
	{"l6","iso-8859-10"},
	{"l8","iso-8859-14"},
	{"lap","latin-lap"},
	{"latin-9","iso-8859-15"},
	{"latin-greek","latin-greek"},
	{"latin-greek-1","latin-greek-1"},
	{"latin-lap","latin-lap"},
	{"latin1","iso_8859-1:1987"},
	{"latin1-2-5","iso_8859-supp"},
	{"latin10","iso-8859-16"},
	{"latin2","iso_8859-2:1987"},
	{"latin3","iso_8859-3:1988"},
	{"latin4","iso_8859-4:1988"},
	{"latin5","iso_8859-9:1989"},
	{"latin6","iso-8859-10"},
	{"latin8","iso-8859-14"},
	{"mac","macintosh"},
	{"macedonian","jus_i.b1.003-mac"},
	{"macintosh","macintosh"},
	{"microsoft-publishing","microsoft-publishing"},
	{"mnem","mnem"},
	{"mnemonic","mnemonic"},
	{"ms936","gbk"},
	{"ms_kanji","shift_jis"},
	{"msz_7795.3","msz_7795.3"},
	{"naplps","ansi_x3.110-1983"},
	{"nats-dano","nats-dano"},
	{"nats-dano-add","nats-dano-add"},
	{"nats-sefi","nats-sefi"},
	{"nats-sefi-add","nats-sefi-add"},
	{"nc_nc00-10:81","nc_nc00-10:81"},
	{"nf_z_62-010","nf_z_62-010"},
	{"nf_z_62-010_(1973)","nf_z_62-010_(1973)"},
	{"no","ns_4551-1"},
	{"no2","ns_4551-2"},
	{"ns_4551-1","ns_4551-1"},
	{"ns_4551-2","ns_4551-2"},
	{"osd_ebcdic_df03_irv","osd_ebcdic_df03_irv"},
	{"osd_ebcdic_df04_1","osd_ebcdic_df04_1"},
	{"osd_ebcdic_df04_15","osd_ebcdic_df04_15"},
	{"pc-multilingual-850+euro","ibm00858"},
	{"pc8-danish-norwegian","pc8-danish-norwegian"},
	{"pc8-turkish","pc8-turkish"},
	{"pt","pt"},
	{"pt154","ptcp154"},
	{"pt2","pt2"},
	{"ptcp154","ptcp154"},
	{"r8","hp-roman8"},
	{"ref","iso_646.basic:1983"},
	{"roman8","hp-roman8"},
	{"scsu","scsu"},
	{"se","sen_850200_b"},
	{"se2","sen_850200_c"},
	{"sen_850200_b","sen_850200_b"},
	{"sen_850200_c","sen_850200_c"},
	{"serbian","jus_i.b1.003-serb"},
	{"shift_jis","shift_jis"},
	{"st_sev_358-88","gost_19768-74"},
	{"t.101-g2","t.101-g2"},
	{"t.61","t.61-8bit"},
	{"t.61-7bit","t.61-7bit"},
	{"t.61-8bit","t.61-8bit"},
	{"tis-620","tis-620"},
	{"uk","bs_4730"},
	{"unicode-1-1","unicode-1-1"},
	{"unicode-1-1-utf-7","unicode-1-1-utf-7"},
	{"unknown-8bit","unknown-8bit"},
	{"us","ansi_x3.4-1968"},
	{"us-ascii","ansi_x3.4-1968"},
	{"us-dk","us-dk"},
	{"utf-16","utf-16"},
	{"utf-16be","utf-16be"},
	{"utf-16le","utf-16le"},
	{"utf-32","utf-32"},
	{"utf-32be","utf-32be"},
	{"utf-32le","utf-32le"},
	{"utf-7","utf-7"},
	{"utf-8","utf-8"},
	{"ventura-international","ventura-international"},
	{"ventura-math","ventura-math"},
	{"ventura-us","ventura-us"},
	{"videotex-suppl","videotex-suppl"},
	{"viqr","viqr"},
	{"viscii","viscii"},
	{"windows-1250","windows-1250"},
	{"windows-1251","windows-1251"},
	{"windows-1252","windows-1252"},
	{"windows-1253","windows-1253"},
	{"windows-1254","windows-1254"},
	{"windows-1255","windows-1255"},
	{"windows-1256","windows-1256"},
	{"windows-1257","windows-1257"},
	{"windows-1258","windows-1258"},
	{"windows-31j","windows-31j"},
	{"windows-936","gbk"},
	{"x0201","jis_x0201"},
	{"x0201-7","jis_c6220-1969-jp"},
	{"x0208","jis_c6226-1983"},
	{"x0212","jis_x0212-1990"},
	{"yu","jus_i.b1.002"}
};