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


// using namespace std;

// namespace MusicFormats
// {

//________________________________________________________________________
// no class here, an int type id needed for yylex()
namespace MfslToken {
  enum mfslTokenKind {
//     kMFSL_EOF,
//       //	0, will be returned by yylex () when 'yywrap ()'
//       //	retuns a non-null value itself

// Bison 3.8.1 generates:
// enum yysymbol_kind_t
// {
//   YYSYMBOL_YYEMPTY = -2,
//   YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
//   YYSYMBOL_YYerror = 1,                    /* error  */
//   YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
//   YYSYMBOL_kTOOL = 3,                      /* kTOOL  */
// so our own token integers should start at 3

    // keywords
    kTOOL = 3,
    kINPUT,

    kBOOK,
    kSCORE,

    kCASE,

    // constants
    kSINGLE_QUOTED_STRING,
    kDOUBLE_QUOTED_STRING,

    kINTEGER_NUMBER,
    kDOUBLE_NUMBER,

    kNAME,

    // others
    kLEFT_PARENTHESIS,
    kRIGHT_PARENTHESIS,

    kEQUALS,
    kCOMMA,
    kPLUS,
  //   kMINUS,
    kSTAR,
    kSLASH,
    kCOLON,
    kSEMI_COLON,

    kBAR,

    kOPTION
  };
}

EXP string mfslTokenKindAsString (
  MfslToken::mfslTokenKind tokenKind);

ostream& operator<< (ostream& os, const MfslToken::mfslTokenKind& elt);


// }


#endif
