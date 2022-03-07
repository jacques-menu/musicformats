/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___mfslTokens___
#define ___mfslTokens___

#include <string>
#include <ostream>

#include "exports.h"


using namespace std;

namespace MusicFormats
{

//________________________________________________________________________
// no class here, an int type id needed for yylex()
enum mfslTokenKind {
	kMfslEOF,	//	0, will be returned by yylex () when 'yywrap ()'
						//	retuns a non-null valu itself

  kMfslTool,
  kMfslInput,
  kMfslCase,

  kMfslString,

  kMfslIntegerNumber,
  kMfslDoubleNumber,

  kMfslName,

  kMfslLeftParenthesis,
  kMfslRightParenthesis,

  kMfslEquals,
  kMfslComma,
  kMfslPlus,
  kMfslMinus,
  kMfslTimes,
  kMfslSlash,
  kMfslColon,
  kMfslSemicolon
};

EXP string mfslTokenKindAsString (
  mfslTokenKind tokenKind);

ostream& operator<< (ostream& os, const mfslTokenKind& elt);


}


#endif
