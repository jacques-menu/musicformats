/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/


#include "msrRepeatsEnumTypes.h"


using namespace std;

namespace MusicFormats
{

// repeat endings
//______________________________________________________________________________
string msrRepeatEndingKindAsString (
  msrRepeatEndingKind repeatEndingKind)
{
  string result;

  switch (repeatEndingKind) {
    case msrRepeatEndingKind::kRepeatEndingHooked:
      result = "kRepeatEndingHooked";
      break;
    case msrRepeatEndingKind::kRepeatEndingHookless:
      result = "kRepeatEndingHookless";
      break;
  } // switch

  return result;
}

ostream& operator << (ostream& os, const msrRepeatEndingKind& elt)
{
  os << msrRepeatEndingKindAsString (elt);
  return os;
}

string msrRepeatExplicitStartKindAsString (
  msrRepeatExplicitStartKind repeatExplicitStartKind)
{
  string result;

  switch (repeatExplicitStartKind) {
    case msrRepeatExplicitStartKind::kRepeatExplicitStartNo:
      result = "kRepeatExplicitStartNo";
      break;
    case msrRepeatExplicitStartKind::kRepeatExplicitStartYes:
      result = "kRepeatExplicitStartYes";
      break;
  } // switch

  return result;
}

ostream& operator << (ostream& os, const msrRepeatExplicitStartKind& elt)
{
  os << msrRepeatExplicitStartKindAsString (elt);
  return os;
}

string repeatBuildPhaseKindAsString (
  msrRepeatBuildPhaseKind repeatBuildPhaseKind)
{
  string result;

  switch (repeatBuildPhaseKind) {
    case msrRepeatBuildPhaseKind::kRepeatBuildPhaseJustCreated:
      result = "kRepeatBuildPhaseJustCreated";
      break;
    case msrRepeatBuildPhaseKind::kRepeatBuildPhaseInCommonPart:
      result = "kRepeatBuildPhaseInCommonPart";
      break;
    case msrRepeatBuildPhaseKind::kRepeatBuildPhaseInEndings:
      result = "kRepeatBuildPhaseInEndings";
      break;
    case msrRepeatBuildPhaseKind::kRepeatBuildPhaseCompleted:
      result = "kRepeatBuildPhaseCompleted";
      break;
  } // switch

  return result;
}



}
