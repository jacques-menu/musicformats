/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include "mfWaeInterface.h"

#include "mfOnOff.h"


namespace MusicFormats
{

//______________________________________________________________________________
Bool mfOnOffKindAsBool (
  mfOnOffKind onOffKind)
{
  Bool result;

  switch (onOffKind) {
    case mfOnOffKind::kMfOnOffUnknown:
      {
        mfError (
          __FILE__, __LINE__,
          "mfOnOffKind::kMfOnOffUnknown cannot be converted to Bool");
      }
      break;
    case mfOnOffKind::kMfOnOffOn:
      result = true;
      break;
    case mfOnOffKind::kMfOnOffOff:
      result = false;
      break;
  } // switch

  return result;
}

std::string mfOnOffKindAsString (
  mfOnOffKind onOffKind)
{
  std::string result;

  // no CamelCase here, these strings are used in the command line options

  switch (onOffKind) {
    case mfOnOffKind::kMfOnOffUnknown:
      result = "unknown";
      break;
    case mfOnOffKind::kMfOnOffOn:
      result = "on";
      break;
    case mfOnOffKind::kMfOnOffOff:
      result = "off";
      break;
  } // switch

  return result;
}

std::ostream& operator << (std::ostream& os, const mfOnOffKind elt)
{
  os << mfOnOffKindAsString (elt);
  return os;
}


}

