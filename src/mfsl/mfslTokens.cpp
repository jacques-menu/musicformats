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

    case kTOOL:
      s << "kTOOL";
      break;
    case kINPUT:
      s << "kINPUT";
      break;

    case kBOOK:
      s << "kBOOK";
      break;

    case kCASE:
      s << "kCASE";
      break;

    case kSINGLE_QUOTED_STRING:
      s << "kSINGLE_QUOTED_STRING";
      break;
    case kDOUBLE_QUOTED_STRING:
      s << "kDOUBLE_QUOTED_STRING";
      break;

    case kINTEGER_NUMBER:
      s << "kINTEGER_NUMBER";
      break;
    case kDOUBLE_NUMBER:
      s << "kDOUBLE_NUMBER";
      break;

    case kNAME:
      s << "kNAME";
      break;

    case kLEFT_PARENTHESIS:
      s << "kLEFT_PARENTHESIS";
      break;
    case kRIGHT_PARENTHESIS:
      s << "kRIGHT_PARENTHESIS";
      break;

    case kEQUALS:
      s << "kEQUALS";
      break;
    case kCOMMA:
      s << "kCOMMA";
      break;
    case kPLUS:
      s << "kPLUS";
      break;
    case kMINUS:
      s << "kMINUS";
      break;
    case kSTAR:
      s << "kSTAR";
      break;
    case kSLASH:
      s << "kSLASH";
      break;
    case kCOLON:
      s << "kCOLON";
      break;
    case kSEMI_COLON:
      s << "kSEMI_COLON";
      break;

    case kBAR:
      s << "kBAR";
      break;

    case kDASH:
      s << "kDASH";
      break;
    case kDASH_DASH:
      s << "kDASH_DASH";
      break;
  } // switch

  return s.str ();
}

ostream& operator<< (ostream& os, const mfslTokenKind& elt){
  os << mfslTokenKindAsString (elt);
  return os;
}


} // namespace
