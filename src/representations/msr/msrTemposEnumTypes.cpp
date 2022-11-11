/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

// #include <iostream>
// #include <sstream>
// #include <iomanip>      // setw, setprecision, ...
//
// #include "visitor.h"
//
// #include "enableTracingIfDesired.h"
// #ifdef TRACING_IS_ENABLED
//   #include "tracingOah.h"
// #endif
//
// #include "mfAssert.h"
//
// #include "mfServiceRunData.h"
//
// #include "mfStringsHandling.h"
//
// #include "msrWae.h"
//
// #include "msrTemposEnumTypes.h"
//
// #include "oahOah.h"
//
// #include "msrOah.h"
//
// #include "msrBrowsers.h"

#include "msrTemposEnumTypes.h"


using namespace std;

namespace MusicFormats
{

//______________________________________________________________________________
string msrTempoKBeatUnitsKindAsString (
  msrTempoKBeatUnitsKind tempoKind)
{
  string result;

  switch (tempoKind) {
    case msrTempoKBeatUnitsKind::kTempoBeatUnits_NO_:
      result = "kTempoBeatUnits_NO_";
      break;
    case msrTempoKBeatUnitsKind::kTempoBeatUnitsWordsOnly:
      result = "kTempoBeatUnitsWordsOnly";
      break;
    case msrTempoKBeatUnitsKind::kTempoBeatUnitsPerMinute:
      result = "kTempoBeatUnitsPerMinute";
      break;
    case msrTempoKBeatUnitsKind::kTempoBeatUnitsEquivalence:
      result = "kTempoBeatUnitsEquivalence";
      break;
    case msrTempoKBeatUnitsKind::kTempoNotesRelationship:
      result = "kTempoNotesRelationship";
      break;
  } // switch

  return result;
}

ostream& operator << (ostream& os, const msrTempoKBeatUnitsKind& elt)
{
  os << msrTempoKBeatUnitsKindAsString (elt);
  return os;
}

string msrTempoParenthesizedKindAsString (
  msrTempoParenthesizedKind tempoParenthesizedKind)
{
  string result;

  switch (tempoParenthesizedKind) {
    case msrTempoParenthesizedKind::kTempoParenthesizedYes:
      result = "kTempoParenthesizedYes";
      break;
    case msrTempoParenthesizedKind::kTempoParenthesizedNo:
      result = "kTempoParenthesizedNo";
      break;
  } // switch

  return result;
}

ostream& operator << (ostream& os, const msrTempoParenthesizedKind& elt)
{
  os << msrTempoParenthesizedKindAsString (elt);
  return os;
}

string msrTempoNotesRelationshipKindAsString (
  msrTempoNotesRelationshipKind tempoNotesRelationshipKind)
{
  string result;

  switch (tempoNotesRelationshipKind) {
    case msrTempoNotesRelationshipKind::kTempoNotesRelationshipEquals:
      result = "kTempoNotesRelationshipEquals";
      break;
    case msrTempoNotesRelationshipKind::kTempoNotesRelationshipNone:
      result = "kTempoNotesRelationshipNone";
      break;
  } // switch

  return result;
}

ostream& operator << (ostream& os, const msrTempoNotesRelationshipKind& elt)
{
  os << msrTempoNotesRelationshipKindAsString (elt);
  return os;
}


}
