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


// using namespace std;

// namespace MusicFormats
// {

//________________________________________________________________________
string mfslTokenKindAsString (
  MfslToken::mfslTokenKind tokenKind)
{
  stringstream s;

  switch (tokenKind) {
//     case MfslToken::kMFSL_EOF:
//       s << "kMFSL_EOF";
//       break;
//
    case MfslToken::kTOOL:
      s << "kTOOL";
      break;
    case MfslToken::kINPUT:
      s << "kINPUT";
      break;

    case MfslToken::kBOOK:
      s << "kBOOK";
      break;
    case MfslToken::kSCORE:
      s << "kSCORE";
      break;

    case MfslToken::kCASE:
      s << "kCASE";
      break;

    case MfslToken::kSINGLE_QUOTED_STRING:
      s << "kSINGLE_QUOTED_STRING";
      break;
    case MfslToken::kDOUBLE_QUOTED_STRING:
      s << "kDOUBLE_QUOTED_STRING";
      break;

    case MfslToken::kINTEGER_NUMBER:
      s << "kINTEGER_NUMBER";
      break;
    case MfslToken::kDOUBLE_NUMBER:
      s << "kDOUBLE_NUMBER";
      break;

    case MfslToken::kNAME:
      s << "kNAME";
      break;

    case MfslToken::kLEFT_PARENTHESIS:
      s << "kLEFT_PARENTHESIS";
      break;
    case MfslToken::kRIGHT_PARENTHESIS:
      s << "kRIGHT_PARENTHESIS";
      break;

    case MfslToken::kEQUALS:
      s << "kEQUALS";
      break;
    case MfslToken::kCOMMA:
      s << "kCOMMA";
      break;
    case MfslToken::kPLUS:
      s << "kPLUS";
      break;
//     case MfslToken::kMINUS:
//       s << "kMINUS";
//       break;
    case MfslToken::kSTAR:
      s << "kSTAR";
      break;
    case MfslToken::kSLASH:
      s << "kSLASH";
      break;
    case MfslToken::kCOLON:
      s << "kCOLON";
      break;
    case MfslToken::kSEMI_COLON:
      s << "kSEMI_COLON";
      break;

    case MfslToken::kBAR:
      s << "kBAR";
      break;

    case MfslToken::kOPTION:
      s << "kOPTION";
      break;
  } // switch

  return s.str ();
}

ostream& operator<< (ostream& os, const MfslToken::mfslTokenKind& elt){
  os << mfslTokenKindAsString (elt);
  return os;
}


// } // namespace
