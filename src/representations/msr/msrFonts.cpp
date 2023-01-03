/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <sstream>

#include "mfConstants.h"
#include "mfServiceRunData.h"

#include "msrFonts.h"

#include "msrWae.h"


namespace MusicFormats
{

// font size
//______________________________________________________________________________
std::string msrFontSizeKindAsString (
  msrFontSizeKind fontSizeKind)
{
  std::string result;

  switch (fontSizeKind) {
    case msrFontSizeKind::kFontSizeNone:
      result = "kFontSizeNone";
      break;
    case msrFontSizeKind::kFontSizeXXSmall:
      result = "kFontSizeXXSmall";
      break;
    case msrFontSizeKind::kFontSizeXSmall:
      result = "kFontSizeXSmall";
      break;
    case msrFontSizeKind::kFontSizeSmall:
      result = "kFontSizeSmall";
      break;
    case msrFontSizeKind::kFontSizeMedium:
      result = "kFontSizeMedium";
      break;
    case msrFontSizeKind::kFontSizeLarge:
      result = "kFontSizeLarge";
      break;
    case msrFontSizeKind::kFontSizeXLarge:
      result = "kFontSizeXLarge";
      break;
    case msrFontSizeKind::kFontSizeXXLarge:
      result = "kFontSizeXXLarge";
      break;
    case msrFontSizeKind::kFontSizeNumeric:
      result = "kFontSizeNumeric";
      break;
    } // switch

  return result;
}

std::ostream& operator << (std::ostream& os, const  msrFontSizeKind& elt)
{
  os <<  msrFontSizeKindAsString (elt);
  return os;
}

// font style
//______________________________________________________________________________
msrFontStyleKind msrFontStyleKindFromString (
  int                inputLineNumber,
  const std::string& fontStyleString)
{
  msrFontStyleKind result = msrFontStyleKind::kFontStyleNone; // default value

  if      (fontStyleString == "normal")
    result = msrFontStyleKind::kFontStyleNormal;
  else if (fontStyleString == "italic")
    result = msrFontStyleKind::KFontStyleItalic;
  else {
    if (fontStyleString.size ()) {
      std::stringstream s;

      s <<
        "style value " << fontStyleString <<
        " should be 'normal' or 'italic'";

      msrError (
        gGlobalServiceRunData->getInputSourceName (),
        inputLineNumber,
        __FILE__, __LINE__,
        s.str ());
    }
  }

  return result;
}

std::string msrFontStyleKindAsString (
  msrFontStyleKind fontStyleKind)
{
  std::string result;

  switch (fontStyleKind) {
    case msrFontStyleKind::kFontStyleNone:
      result = "kFontStyleNone***";
      break;
    case msrFontStyleKind::kFontStyleNormal:
      result = "kFontStyleNormal";
      break;
    case msrFontStyleKind::KFontStyleItalic:
      result = "KFontStyleItalic";
      break;
    } // switch

  return result;
}

std::ostream& operator << (std::ostream& os, const msrFontStyleKind& elt)
{
  os << msrFontStyleKindAsString (elt);
  return os;
}

// font weight
//______________________________________________________________________________
msrFontWeightKind msrFontWeightKindFromString (
  int                inputLineNumber,
  const std::string& fontWeightString)
{
  msrFontWeightKind result = msrFontWeightKind::kFontWeightNone; // default value

  if      (fontWeightString == "normal")
    result = msrFontWeightKind::kFontWeightNormal;
  else if (fontWeightString == "bold")
    result = msrFontWeightKind::kFontWeightBold;
  else {
    if (fontWeightString.size ()) {
      std::stringstream s;

      s <<
        "font weight value " << fontWeightString <<
        " should be 'normal' or 'bold'";

      msrError (
        gGlobalServiceRunData->getInputSourceName (),
        inputLineNumber,
        __FILE__, __LINE__,
        s.str ());
    }
  }

  return result;
}

std::string msrFontWeightKindAsString (
  msrFontWeightKind fontWeightKind)
{
  std::string result;

  switch (fontWeightKind) {
    case msrFontWeightKind::kFontWeightNone:
      result = "kFontWeightNone";
      break;
    case msrFontWeightKind::kFontWeightNormal:
      result = "kFontWeightNormal";
      break;
    case msrFontWeightKind::kFontWeightBold:
      result = "kFontWeightBold";
      break;
    } // switch

  return result;
}

std::ostream& operator << (std::ostream& os, const msrFontWeightKind& elt)
{
  os << msrFontWeightKindAsString (elt);
  return os;
}

//______________________________________________________________________________
S_msrFontSize msrFontSize::create (
  msrFontSizeKind fontSizeKind)
{
  msrFontSize * o =
    new msrFontSize (
      fontSizeKind);
  assert (o != nullptr);
  return o;
}

S_msrFontSize msrFontSize::create (
  float numericFontSize)
{
  msrFontSize * o =
    new msrFontSize (
      numericFontSize);
  assert (o != nullptr);
  return o;
}

msrFontSize::msrFontSize (
  msrFontSizeKind fontSizeKind)
{
  fFontSizeKind = fontSizeKind;
}

msrFontSize::msrFontSize (
  float fontNumericSize)
{
  fFontSizeKind    = msrFontSizeKind::kFontSizeNumeric;
  fFontNumericSize = fontNumericSize;
}

msrFontSize::~msrFontSize ()
{}

std::string msrFontSize::fontSizeAsString () const
{
  std::string result;

  switch (fFontSizeKind) {
    case msrFontSizeKind::kFontSizeNone:
    case msrFontSizeKind::kFontSizeXXSmall:
    case msrFontSizeKind::kFontSizeXSmall:
    case msrFontSizeKind::kFontSizeSmall:
    case msrFontSizeKind::kFontSizeMedium:
    case msrFontSizeKind::kFontSizeLarge:
    case msrFontSizeKind::kFontSizeXLarge:
    case msrFontSizeKind::kFontSizeXXLarge:
      result = msrFontSizeKindAsString (fFontSizeKind);
      break;

    case msrFontSizeKind::kFontSizeNumeric:
      result =
        std::to_string (fFontNumericSize);
      break;
    } // switch

  return result;
}

float msrFontSize::getFontNumericSize () const
{
  float result = 12; // JMI

  switch (fFontSizeKind) {
    case msrFontSizeKind::kFontSizeNone:
    case msrFontSizeKind::kFontSizeXXSmall:
    case msrFontSizeKind::kFontSizeXSmall:
    case msrFontSizeKind::kFontSizeSmall:
    case msrFontSizeKind::kFontSizeMedium:
    case msrFontSizeKind::kFontSizeLarge:
    case msrFontSizeKind::kFontSizeXLarge:
    case msrFontSizeKind::kFontSizeXXLarge:
      {
        std::stringstream s;

        s <<
          "attempting to get font numeric size for a " <<
          msrFontSizeKindAsString (fFontSizeKind);

        msrInternalError (
          gGlobalServiceRunData->getInputSourceName (),
          K_MF_INPUT_LINE_UNKNOWN, // JMI
          __FILE__, __LINE__,
          s.str ());
      }
      break;

    case msrFontSizeKind::kFontSizeNumeric:
      result = fFontNumericSize;
      break;
    } // switch

  return result;
}

std::string msrFontSize::asString () const
{
  std::string result;

  switch (fFontSizeKind) {
    case msrFontSizeKind::kFontSizeNone:
    case msrFontSizeKind::kFontSizeXXSmall:
    case msrFontSizeKind::kFontSizeXSmall:
    case msrFontSizeKind::kFontSizeSmall:
    case msrFontSizeKind::kFontSizeMedium:
    case msrFontSizeKind::kFontSizeLarge:
    case msrFontSizeKind::kFontSizeXLarge:
    case msrFontSizeKind::kFontSizeXXLarge:
      result =
        msrFontSizeKindAsString (fFontSizeKind);
      break;

    case msrFontSizeKind::kFontSizeNumeric:
      result =
        fFontNumericSize;
      break;
  } // switch

  return result;
}

void msrFontSize::print (std::ostream& os) const
{
  os << asString () << std::endl;
}

std::ostream& operator << (std::ostream& os, const S_msrFontSize& elt)
{
  os << elt->asString ();
  return os;
}


}

