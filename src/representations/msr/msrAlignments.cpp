/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <sstream>

#include "mfServiceRunData.h"

#include "msrAlignments.h"

#include "msrWae.h"


namespace MusicFormats
{

// alignement
//______________________________________________________________________________
msrJustifyKind msrJustifyKindFromString (
  int           inputLineNumber,
  const string& justifyString)
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
      stringstream s;

      s <<
        "justify value " << justifyString <<
        " should be 'left', 'center' or 'right'";

      msrError (
        gGlobalServiceRunData->getInputSourceName (),
        inputLineNumber,
        __FILE__, __LINE__,
        s.str ());
    }
  }

  return result;
}

string msrJustifyKindAsString (
  msrJustifyKind justifyKind)
{
  string result;

  switch (justifyKind) {
    case msrJustifyKind::kJustifyNone:
      result = "***kJustifyNone***";
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

ostream& operator << (ostream& os, const msrJustifyKind& elt)
{
  os << msrJustifyKindAsString (elt);
  return os;
}

msrHorizontalAlignmentKind msrHorizontalAlignmentKindFromString (
  int           inputLineNumber,
  const string& horizontalAlignmentString)
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
      stringstream s;

      s <<
        "halign value " << horizontalAlignmentString <<
        " should be 'left', 'center' or 'right'";

      msrError (
        gGlobalServiceRunData->getInputSourceName (),
        inputLineNumber,
        __FILE__, __LINE__,
        s.str ());
    }
  }

  return result;
}

string msrHorizontalAlignmentKindAsString (
  msrHorizontalAlignmentKind horizontalAlignmentKind)
{
  string result;

  switch (horizontalAlignmentKind) {
    case msrHorizontalAlignmentKind::kHorizontalAlignmentNone:
      result = "***kHorizontalAlignmentNone***";
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

ostream& operator << (ostream& os, const msrHorizontalAlignmentKind& elt)
{
  os << msrHorizontalAlignmentKindAsString (elt);
  return os;
}

msrVerticalAlignmentKind msrVerticalAlignmentKindFromString (
  int           inputLineNumber,
  const string& verticalAlignmentString)
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
      stringstream s;

      s <<
        "valign value " << verticalAlignmentString <<
        " should be 'top', 'middle' or 'bottom'";

      msrError (
        gGlobalServiceRunData->getInputSourceName (),
        inputLineNumber,
        __FILE__, __LINE__,
        s.str ());
    }
  }

  return result;
}

string msrVerticalAlignmentKindAsString (
  msrVerticalAlignmentKind verticalAlignmentKind)
{
  string result;

  switch (verticalAlignmentKind) {
    case msrVerticalAlignmentKind::kVerticalAlignmentNone:
      result = "***kVerticalAlignmentNone***";
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

ostream& operator << (ostream& os, const msrVerticalAlignmentKind& elt)
{
  os << msrVerticalAlignmentKindAsString (elt);
  return os;
}


}

