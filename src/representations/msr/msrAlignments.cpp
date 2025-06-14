/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <sstream>

#include "mfServices.h"

#include "msrAlignments.h"

#include "msrWae.h"


namespace MusicFormats
{

// alignement
//______________________________________________________________________________
msrJustifyKind msrJustifyKindFromString (
  const mfInputLineNumber& inputLineNumber,
  const std::string& justifyString)
{
  msrJustifyKind result = msrJustifyKind::kJustifyNone; // default value

  if      (justifyString == "left")
    result = msrJustifyKind::kJustifyLeft;
  else if (justifyString == "center")
    result = msrJustifyKind::kJustifyCenter;
  else if (justifyString == "right")
    result = msrJustifyKind::kJustifyRight;
  else {
    if (justifyString.size ()) {
      std::stringstream ss;

      ss <<
        "justify value " << justifyString <<
        " should be 'left', 'center' or 'right'";

      msrError (
        gServiceRunData->getInputSourceName (),
        inputLineNumber,
        __FILE__, mfInputLineNumber (__LINE__),
        ss.str ());
    }
  }

  return result;
}

std::string msrJustifyKindAsString (
  msrJustifyKind justifyKind)
{
  std::string result;

  switch (justifyKind) {
    case msrJustifyKind::kJustifyNone:
      result = "kJustifyNone***";
      break;
    case msrJustifyKind::kJustifyLeft:
      result = "kJustifyLeft";
      break;
    case msrJustifyKind::kJustifyCenter:
      result = "kJustifyCenter";
      break;
    case msrJustifyKind::kJustifyRight:
      result = "kJustifyRight";
      break;
    } // switch

  return result;
}

std::ostream& operator << (std::ostream& os, const msrJustifyKind& elt)
{
  os << msrJustifyKindAsString (elt);
  return os;
}

msrHorizontalAlignmentKind msrHorizontalAlignmentKindFromString (
  const mfInputLineNumber& inputLineNumber,
  const std::string& horizontalAlignmentString)
{
  msrHorizontalAlignmentKind
    result = msrHorizontalAlignmentKind::kHorizontalAlignmentNone; // default value

  if      (horizontalAlignmentString == "left")
    result = msrHorizontalAlignmentKind::kHorizontalAlignmentLeft;
  else if (horizontalAlignmentString == "center")
    result = msrHorizontalAlignmentKind::kHorizontalAlignmentCenter;
  else if (horizontalAlignmentString == "right")
    result = msrHorizontalAlignmentKind::kHorizontalAlignmentRight;
  else {
    if (horizontalAlignmentString.size ()) {
      std::stringstream ss;

      ss <<
        "halign value " << horizontalAlignmentString <<
        " should be 'left', 'center' or 'right'";

      msrError (
        gServiceRunData->getInputSourceName (),
        inputLineNumber,
        __FILE__, mfInputLineNumber (__LINE__),
        ss.str ());
    }
  }

  return result;
}

std::string msrHorizontalAlignmentKindAsString (
  msrHorizontalAlignmentKind horizontalAlignmentKind)
{
  std::string result;

  switch (horizontalAlignmentKind) {
    case msrHorizontalAlignmentKind::kHorizontalAlignmentNone:
      result = "kHorizontalAlignmentNone***";
      break;
    case msrHorizontalAlignmentKind::kHorizontalAlignmentLeft:
      result = "kHorizontalAlignmentLeft";
      break;
    case msrHorizontalAlignmentKind::kHorizontalAlignmentCenter:
      result = "kHorizontalAlignmentCenter";
      break;
    case msrHorizontalAlignmentKind::kHorizontalAlignmentRight:
      result = "kHorizontalAlignmentRight";
      break;
    } // switch

  return result;
}

std::ostream& operator << (std::ostream& os, const msrHorizontalAlignmentKind& elt)
{
  os << msrHorizontalAlignmentKindAsString (elt);
  return os;
}

msrVerticalAlignmentKind msrVerticalAlignmentKindFromString (
  const mfInputLineNumber& inputLineNumber,
  const std::string& verticalAlignmentString)
{
  msrVerticalAlignmentKind
    result = msrVerticalAlignmentKind::kVerticalAlignmentNone; // default value

  if      (verticalAlignmentString == "top")
    result = msrVerticalAlignmentKind::kVerticalAlignmentTop;
  else if (verticalAlignmentString == "middle")
    result = msrVerticalAlignmentKind::kVerticalAlignmentMiddle;
  else if (verticalAlignmentString == "bottom")
    result = msrVerticalAlignmentKind::kVerticalAlignmentBottom;
  else {
    if (verticalAlignmentString.size ()) {
      std::stringstream ss;

      ss <<
        "valign value " << verticalAlignmentString <<
        " should be 'top', 'middle' or 'bottom'";

      msrError (
        gServiceRunData->getInputSourceName (),
        inputLineNumber,
        __FILE__, mfInputLineNumber (__LINE__),
        ss.str ());
    }
  }

  return result;
}

std::string msrVerticalAlignmentKindAsString (
  msrVerticalAlignmentKind verticalAlignmentKind)
{
  std::string result;

  switch (verticalAlignmentKind) {
    case msrVerticalAlignmentKind::kVerticalAlignmentNone:
      result = "kVerticalAlignmentNone***";
      break;
    case msrVerticalAlignmentKind::kVerticalAlignmentTop:
      result = "kVerticalAlignmentTop";
      break;
    case msrVerticalAlignmentKind::kVerticalAlignmentMiddle:
      result = "kVerticalAlignmentMiddle";
      break;
    case msrVerticalAlignmentKind::kVerticalAlignmentBottom:
      result = "kVerticalAlignmentBottom";
      break;
    } // switch

  return result;
}

std::ostream& operator << (std::ostream& os, const msrVerticalAlignmentKind& elt)
{
  os << msrVerticalAlignmentKindAsString (elt);
  return os;
}


}

