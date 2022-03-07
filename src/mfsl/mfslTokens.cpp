/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include "enableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "tracingOah.h"
#endif

#include "mfslTokens.h"


using namespace std;

namespace MusicFormats
{

//________________________________________________________________________
string mfslTokenKindAsString (
  mfslTokenKind tokenKind)
{
  stringstream s;

  switch (tokenKind) {
    case kMfslEOF:
      s << "kMfslEOF";
      break;

    case kMfslTool:
      s << "kMfslTool";
      break;
    case kMfslInput:
      s << "kMfslInput";
      break;
    case kMfslCase:
      s << "kMfslCase";
      break;

    case kMfslString:
      s << "kMfslString";
      break;

    case kMfslIntegerNumber:
      s << "kMfslIntegerNumber";
      break;
    case kMfslDoubleNumber:
      s << "kMfslDoubleNumber";
      break;

    case kMfslName:
      s << "kMfslName";
      break;

    case kMfslLeftParenthesis:
      s << "kMfslLeftParenthesis";
      break;
    case kMfslRightParenthesis:
      s << "kMfslRightParenthesis";
      break;

    case kMfslEquals:
      s << "kMfslEquals";
      break;
    case kMfslComma:
      s << "kMfslComma";
      break;
    case kMfslPlus:
      s << "kMfslPlus";
      break;
    case kMfslMinus:
      s << "kMfslMinus";
      break;
    case kMfslTimes:
      s << "kMfslTimes";
      break;
    case kMfslSlash:
      s << "kMfslSlash";
      break;
    case kMfslColon:
      s << "kMfslColon";
      break;
    case kMfslSemicolon:
      s << "kMfslSemicolon";
  } // switch

  return s.str ();
}

ostream& operator<< (ostream& os, const mfslTokenKind& elt){
  os << mfslTokenKindAsString (elt);
  return os;
}


} // namespace
