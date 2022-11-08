/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

// #include <climits>      // INT_MIN, INT_MAX
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

#include "msrStavesEnumTypes.h"

// #include "oahOah.h"
//
// #include "oahOah.h"
// #include "msrOah.h"
// #include "mxsr2msrOah.h"
// #include "msr2msrOah.h"
//
// #include "msrBrowsers.h"


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


}
