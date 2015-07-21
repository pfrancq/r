/*

	R Project Library

	RQt.h

	Generic function used for Qt compatibility - Header.

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
#ifndef RQT_H
#define RQT_H


//------------------------------------------------------------------------------
// include files for R
#include <rstring.h>


//------------------------------------------------------------------------------
// include files for Qt
#include <QtCore/QString>
#include <QtGui/QTextEdit>


//------------------------------------------------------------------------------
namespace R{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* This function returns a QString (Qt toolkit) corresponding to RString.
* @param str                String to transform.
* @returns QString.
*/
QString ToQString(const RString& str);


//------------------------------------------------------------------------------
/**
* This function returns a RString corresponding to a QString (Qt toolkit).
* @param str                String to transform.
* @returns RString.
*/
RString FromQString(const QString& str);


//------------------------------------------------------------------------------
/**
 * Convert a string that contains HTML codes and multiple end of lines into
 * a single line string (eventually limited to a maximum number of characters).
 *
 * This function can be useful when a given rich content editable by a QTextEdit
 * must be shown in a list (such as in a QListWidget).
 * @param str               String to convert.
 * @param maxlen            Maximum length of the resulted string.
 * @return a QString.
 */
QString ToPlainText(const R::RString& str,int maxlen=100);


//------------------------------------------------------------------------------
/**
 * Convert a string that contains HTML codes and multiple end of lines into
 * a single line string (eventually limited to a maximum number of characters).
 *
 * This function can be useful when a given rich content editable by a QTextEdit
 * must be shown in a list (such as in a QListWidget).
 * @param str               String to convert.
 * @param maxlen            Maximum length of the resulted string.
 * @return a QString.
 */
QString ToPlainText(const QString& str,int maxlen=100);


//------------------------------------------------------------------------------
/**
 * When QTextEdit manipulates a rich content (using HTML tags), the resulting
 * string represents a whole HTML document (starting with the <html> tag). This
 * function construct a string with the content extract from the <body> tag
 * (excluding the tag).
 * @param edit              QTextEdit from which the string must be extracted.
 * @return a R::RString.
 */
R::RString ExtractHTMLBody(const QTextEdit* edit);


//------------------------------------------------------------------------------
/**
 * When QTextEdit manipulates a rich content (using HTML tags), the resulting
 * string represents a whole HTML document (starting with the <html> tag). This
 * function construct a string with the content that contains only basic HTML
 * tags (in particular <style> is excluded).
 * @param edit              QTextEdit from which the string must be extracted.
 * @return a R::RString.
 */
R::RString ToSimpleHTML(const QTextEdit* edit);


}  //-------- End of namespace R -----------------------------------------------


//------------------------------------------------------------------------------
#endif

