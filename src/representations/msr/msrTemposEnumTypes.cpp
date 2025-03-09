/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include "msrTemposEnumTypes.h"


namespace MusicFormats
{

//______________________________________________________________________________
std::string msrTempoBeatUnitsKindAsString (
  msrTempoKBeatUnitsKind tempoKind)
{
  std::string result;

  switch (tempoKind) {
    case msrTempoKBeatUnitsKind::kTempoBeatUnits_UNKNOWN_:
      result = "kTempoBeatUnits_UNKNOWN_";
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

std::ostream& operator << (std::ostream& os, const msrTempoKBeatUnitsKind& elt)
{
  os << msrTempoBeatUnitsKindAsString (elt);
  return os;
}

mfIndentedStringStream& operator << (mfIndentedStringStream& iss, const msrTempoKBeatUnitsKind& elt)
{
  iss << msrTempoBeatUnitsKindAsString (elt);
  return iss;
}

std::string msrTempoParenthesizedKindAsString (
  msrTempoParenthesizedKind tempoParenthesizedKind)
{
  std::string result;

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

std::ostream& operator << (std::ostream& os, const msrTempoParenthesizedKind& elt)
{
  os << msrTempoParenthesizedKindAsString (elt);
  return os;
}

mfIndentedStringStream& operator << (mfIndentedStringStream& iss, const msrTempoParenthesizedKind& elt)
{
  iss << msrTempoParenthesizedKindAsString (elt);
  return iss;
}

std::string msrTempoNotesRelationshipKindAsString (
  msrTempoNotesRelationshipKind tempoNotesRelationshipKind)
{
  std::string result;

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

std::ostream& operator << (std::ostream& os, const msrTempoNotesRelationshipKind& elt)
{
  os << msrTempoNotesRelationshipKindAsString (elt);
  return os;
}


}
