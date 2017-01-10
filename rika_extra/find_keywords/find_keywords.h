// find_keywords function declaration -- A. Amiruddin -- 30/12/2016

// REVISION HISTORY
// None

//=================================================================================================
//    Copyright (C) 2016  Afeeq Amiruddin
//
//    This program is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program.  If not, see <http://www.gnu.org/licenses/>.
//=================================================================================================

#ifndef FIND_KEYWORDS_H_INCLUDED
#define FIND_KEYWORDS_H_INCLUDED

using namespace std;

//*************************************************************************************************

int32_t FindKeywords(string pszSource, string pszKeyword1);
// Checks for keywords in a string. Returns 1 if all keywords present, 0 if not all keywords present or error.

int32_t FindKeywords(string pszSource, string pszKeyword1, string pszKeyword2);
// Checks for keywords in a string. Returns 1 if all keywords present, 0 if not all keywords present or error.

int32_t FindKeywords(string pszSource, string pszKeyword1, string pszKeyword2, string pszKeyword3);
// Checks for keywords in a string. Returns 1 if all keywords present, 0 if not all keywords present or error.

//*************************************************************************************************

#endif // FIND_KEYWORDS_H_INCLUDED
