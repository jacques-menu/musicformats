/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include "msrStavesEnumTypes.h"


using namespace std;

namespace MusicFormats
{

// staves
//______________________________________________________________________________
string msrStaffKindAsString (
  msrStaffKind staffKind)
{
  string result;

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

ostream& operator << (ostream& os, const msrStaffKind& elt)
{
  os << msrStaffKindAsString (elt);
  return os;
}

string msrStaffKindAsStringForPrint (
  msrStaffKind staffKind)
{
  string result;

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

string msrStaffChangeKindAsString (
  msrStaffChangeKind staffChangeKind)
{
  string result;

  switch (staffChangeKind) {
    case msrStaffChangeKind::kStaffChange_NO_:
      result = "kStaffChange_NO_";
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

ostream& operator << (ostream& os, const msrStaffChangeKind& elt)
{
  os << msrStaffChangeKindAsString (elt);
  return os;
}



}
