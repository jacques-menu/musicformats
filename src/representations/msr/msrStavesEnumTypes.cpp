/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include "msrStavesEnumTypes.h"


namespace MusicFormats
{

// staves
//______________________________________________________________________________
std::string msrStaffKindAsString (
  msrStaffKind staffKind)
{
  std::string result;

  switch (staffKind) {
    case msrStaffKind::kStaffKindRegular:
      result = "kStaffKindRegular";
      break;
    case msrStaffKind::kStaffKindTablature:
      result = "kStaffKindTablature";
      break;
    case msrStaffKind::kStaffKindHarmonies:
      result = "kStaffKindHarmonies";
      break;
    case msrStaffKind::kStaffKindFiguredBass:
      result = "kStaffKindFiguredBass";
      break;
    case msrStaffKind::kStaffKindDrum:
      result = "kStaffKindDrum";
      break;
    case msrStaffKind::kStaffKindRythmic:
      result = "kStaffKindRythmic";
      break;
  } // switch

  return result;
}

std::ostream& operator << (std::ostream& os, const msrStaffKind& elt)
{
  os << msrStaffKindAsString (elt);
  return os;
}

std::string msrStaffKindAsStringForPrint (
  msrStaffKind staffKind)
{
  std::string result;

  switch (staffKind) {
    case msrStaffKind::kStaffKindRegular:
      result = "kStaffKindRegular";
      break;
    case msrStaffKind::kStaffKindTablature:
      result = "kStaffKindTablature";
      break;
    case msrStaffKind::kStaffKindHarmonies:
      result = "kStaffKindHarmonies";
      break;
    case msrStaffKind::kStaffKindFiguredBass:
      result = "kStaffKindFiguredBass";
      break;
    case msrStaffKind::kStaffKindDrum:
      result = "kStaffKindDrum";
      break;
    case msrStaffKind::kStaffKindRythmic:
      result = "kStaffKindRythmic";
      break;
  } // switch

  return result;
}

std::string msrStaffChangeKindAsString (
  msrStaffChangeKind staffChangeKind)
{
  std::string result;

  switch (staffChangeKind) {
    case msrStaffChangeKind::kStaffChange_UNKNOWN_:
      result = "kStaffChange_UNKNOWN_";
      break;
    case msrStaffChangeKind::kStaffChangeChordMemberNote:
      result = "kStaffChangeChordMemberNote";
      break;
    case msrStaffChangeKind::kStaffChangeOtherNote:
      result = "kStaffChangeOtherNote";
      break;
  } // switch

  return result;
}

std::ostream& operator << (std::ostream& os, const msrStaffChangeKind& elt)
{
  os << msrStaffChangeKindAsString (elt);
  return os;
}



}
