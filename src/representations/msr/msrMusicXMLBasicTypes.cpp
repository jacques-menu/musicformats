/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <iomanip>      // std::setw, std::setprecision, ...

#include "visitor.h"

#include "mfAssert.h"
#include "mfServices.h"
#include "mfStringsHandling.h"

#include "msrWae.h"

#include "mfStaticSettings.h"

#include "msrBrowsers.h"
#include "msrMeasureConstants.h"
#include "msrMusicXMLBasicTypes.h"

#include "oahEarlyOptions.h"

#include "msrOah.h"

#include "waeHandlers.h"


namespace MusicFormats
{

//______________________________________________________________________________
// XMLLang
msrXMLLangKind msrXMLLangKindFromString (
  int           inputLineNumber,
  const std::string& XMLLangString)
{
  msrXMLLangKind
    result = msrXMLLangKind::kXMLLangIt; // default value

  if      (XMLLangString == "it")
    result = msrXMLLangKind::kXMLLangIt;
  else if (XMLLangString == "en")
    result = msrXMLLangKind::kXMLLangEn;
  else if (XMLLangString == "de")
    result = msrXMLLangKind::kXMLLangDe;
  else if (XMLLangString == "fr")
    result = msrXMLLangKind::kXMLLangFr;
  else if (XMLLangString == "ja")
    result = msrXMLLangKind::kXMLLangJa;
  else if (XMLLangString == "la")
    result = msrXMLLangKind::kXMLLangLa;
  else {
    if (XMLLangString.size ()) {
      std::stringstream ss;

      ss <<
        "xml:lang value '" << XMLLangString <<
        "' should be 'it', 'en', 'de' or 'fr'";

      msrError (
        gServiceRunData->getInputSourceName (),
        inputLineNumber,
        __FILE__, __LINE__,
        ss.str ());
    }
  }

  return result;
}

std::string msrXMLLangKindAsString (
  msrXMLLangKind XMLLangKind)
{
  std::string result;

  switch (XMLLangKind) {
    case msrXMLLangKind::kXMLLangIt:
      result = "it";
      break;
    case msrXMLLangKind::kXMLLangEn:
      result = "en";
      break;
    case msrXMLLangKind::kXMLLangDe:
      result = "de";
      break;
    case msrXMLLangKind::kXMLLangFr:
      result = "fr";
      break;
    case msrXMLLangKind::kXMLLangJa:
      result = "ja";
      break;
    case msrXMLLangKind::kXMLLangLa:
      result = "la";
      break;
  } // switch

  return result;
}

std::ostream& operator << (std::ostream& os, const msrXMLLangKind& elt)
{
  os << msrXMLLangKindAsString (elt);
  return os;
}

std::string msrOtherAppearanceTypeKindAsString (
  msrOtherAppearanceTypeKind otherAppearanceTypeKind)
{
  std::string result;

  switch (otherAppearanceTypeKind) {
    case msrOtherAppearanceTypeKind::kOtherAppearanceType_UNKNOWN:
      result = "kOtherAppearanceType_UNKNOWN";
      break;
  } // switch

  return result;
}

std::ostream& operator << (std::ostream& os, const msrOtherAppearanceTypeKind& elt)
{
  os << msrOtherAppearanceTypeKindAsString (elt);
  return os;
}

std::string msrLineWidthTypeKindAsString (
  msrLineWidthTypeKind lineWidthTypeKind)
{
  std::string result;

  switch (lineWidthTypeKind) {
    case msrLineWidthTypeKind::kLineWidthType_UNKNOWN:
      result = "kLineWidthType_UNKNOWN";
      break;
    case msrLineWidthTypeKind::kLineWidthTypeBeam:
      result = "kLineWidthTypeBeam";
      break;
    case msrLineWidthTypeKind::kLineWidthTypeBracket:
      result = "kLineWidthTypeBracket";
      break;
    case msrLineWidthTypeKind::kLineWidthTypeDashes:
      result = "kLineWidthTypeDashes";
      break;
    case msrLineWidthTypeKind::kLineWidthTypeEnclosure:
      result = "kLineWidthTypeEnclosure";
      break;
    case msrLineWidthTypeKind::kLineWidthTypeEnding:
      result = "kLineWidthTypeEnding";
      break;
    case msrLineWidthTypeKind::kLineWidthTypeExtend:
      result = "kLineWidthTypeExtend";
      break;
    case msrLineWidthTypeKind::kLineWidthTypeHeavyBarLine:
      result = "kLineWidthTypeHeavyBarLine";
      break;
    case msrLineWidthTypeKind::kLineWidthTypeHeavyLeger:
      result = "kLineWidthTypeHeavyLeger";
      break;
    case msrLineWidthTypeKind::kLineWidthTypeLightBarLine:
      result = "kLineWidthTypeLightBarLine";
      break;
    case msrLineWidthTypeKind::kLineWidthTypeOctaveShift:
      result = "kLineWidthTypeOctaveShift";
      break;
    case msrLineWidthTypeKind::kLineWidthTypePedal:
      result = "kLineWidthTypePedal";
      break;
    case msrLineWidthTypeKind::kLineWidthTypeSlurMiddle:
      result = "kLineWidthTypeSlurMiddle";
      break;
    case msrLineWidthTypeKind::kLineWidthTypeSlurTip:
      result = "kLineWidthTypeSlurTip";
      break;
    case msrLineWidthTypeKind::kLineWidthTypeStaff:
      result = "kLineWidthTypeStaff";
      break;
    case msrLineWidthTypeKind::kLineWidthTypeStem:
      result = "kLineWidthTypeStem";
      break;
    case msrLineWidthTypeKind::kLineWidthTypeTieMiddle:
      result = "kLineWidthTypeTieMiddle";
      break;
    case msrLineWidthTypeKind::kLineWidthTypeTieTip:
      result = "kLineWidthTypeTieTip";
      break;
    case msrLineWidthTypeKind::kLineWidthTypeTupletBracket:
      result = "kLineWidthTypeTupletBracket";
      break;
    case msrLineWidthTypeKind::kLineWidthTypeWedge:
      result = "kLineWidthTypeWedge";
      break;
  } // switch

  return result;
}

std::ostream& operator << (std::ostream& os, const msrLineWidthTypeKind& elt)
{
  os << msrLineWidthTypeKindAsString (elt);
  return os;
}

std::string msrNoteSizeTypeKindAsString (
  msrNoteSizeTypeKind noteSizeTypeKind)
{
  std::string result;

  switch (noteSizeTypeKind) {
    case msrNoteSizeTypeKind::kNoteSizeType_UNKNOWN:
      result = "kNoteSizeType_UNKNOWN";
      break;
    case msrNoteSizeTypeKind::kNoteSizeTypeCue:
      result = "kNoteSizeTypeCue";
      break;
    case msrNoteSizeTypeKind::kNoteSizeTypeGrace:
      result = "kNoteSizeTypeGrace";
      break;
    case msrNoteSizeTypeKind::kNoteSizeTypeLarge:
      result = "kNoteSizeTypeLarge";
      break;
  } // switch

  return result;
}

std::ostream& operator << (std::ostream& os, const msrNoteSizeTypeKind& elt)
{
  os << msrNoteSizeTypeKindAsString (elt);
  return os;
}

std::string msrDistanceTypeKindAsString (
  msrDistanceTypeKind distanceTypeKind)
{
  std::string result;

  switch (distanceTypeKind) {
    case msrDistanceTypeKind::kDistanceType_UNKNOWN:
      result = "kDistanceType_UNKNOWN";
      break;
    case msrDistanceTypeKind::kDistanceTypeHyphen:
      result = "kDistanceTypeHyphen";
      break;
    case msrDistanceTypeKind::kDistanceTypeBeam:
      result = "kDistanceTypeBeam";
      break;
  } // switch

  return result;
}

std::ostream& operator << (std::ostream& os, const msrDistanceTypeKind& elt)
{
  os << msrDistanceTypeKindAsString (elt);
  return os;
}

std::string msrGlyphTypeKindAsString (
  msrGlyphTypeKind glyphTypeKind)
{
  std::string result;

  switch (glyphTypeKind) {
    case msrGlyphTypeKind::kGlyphType_UNKNOWN:
      result = "kGlyphType_UNKNOWN";
      break;
    case msrGlyphTypeKind::kGlyphTypeQuarterRest:
      result = "kGlyphTypeQuarterRest";
      break;
    case msrGlyphTypeKind::kGlyphTypeGClefOttavaBassa:
      result = "kGlyphTypeGClefOttavaBassa";
      break;
    case msrGlyphTypeKind::kGlyphTypeCClef:
      result = "kGlyphTypeCClef";
      break;
    case msrGlyphTypeKind::kGlyphTypeFClef:
      result = "kGlyphTypeFClef";
      break;
    case msrGlyphTypeKind::kGlyphTypePercussionClef:
      result = "msrClefKind::kGlyphTypePercussionClef";
      break;
    case msrGlyphTypeKind::kGlyphTypeOctaveShiftUp8:
      result = "kGlyphTypeOctaveShiftUp8";
      break;
    case msrGlyphTypeKind::kGlyphTypeOctaveShiftDown8:
      result = "kGlyphTypeOctaveShiftDown8";
      break;
    case msrGlyphTypeKind::kGlyphTypeOctaveShiftContinue8:
      result = "kGlyphTypeOctaveShiftContinue8";
      break;
    case msrGlyphTypeKind::kGlyphTypeOctaveShiftDown15:
      result = "kGlyphTypeOctaveShiftDown15";
      break;
    case msrGlyphTypeKind::kGlyphTypeOctaveShiftUp15:
      result = "kGlyphTypeOctaveShiftUp15";
      break;
    case msrGlyphTypeKind::kGlyphTypeOctaveShiftContinue15:
      result = "kGlyphTypeOctaveShiftContinue15";
      break;
    case msrGlyphTypeKind::kGlyphTypeOctaveShiftDown22:
      result = "kGlyphTypeOctaveShiftDown22";
      break;
    case msrGlyphTypeKind::kGlyphTypeOctaveShiftUp22:
      result = "kGlyphTypeOctaveShiftUp22";
      break;
    case msrGlyphTypeKind::kGlyphTypeOctaveShiftContinue22:
      result = "kGlyphTypeOctaveShiftContinue22";
      break;
  } // switch

  return result;
}

std::ostream& operator << (std::ostream& os, const msrGlyphTypeKind& elt)
{
  os << msrGlyphTypeKindAsString (elt);
  return os;
}

//______________________________________________________________________________
S_msrPageLayout msrPageLayout::create (
  int inputLineNumber)
{
  msrPageLayout* o =
    new msrPageLayout (
      inputLineNumber);
  assert (o != nullptr);
  return o;
}

msrPageLayout::msrPageLayout (
  int inputLineNumber)
    : msrElement (inputLineNumber)
{}

msrPageLayout::~msrPageLayout ()
{}

void msrPageLayout::setOddMarginsGroup (
  int               inputLineNumber,
  const S_msrMarginsGroup& val)
{
  if (fBothMarginsGroup) {
    std::stringstream ss;

    ss <<
      "setting an odd margins group when there is already a both margins group in " <<
      this->asString ();

    msrError (
      gServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      ss.str ());
  }

  fOddMarginsGroup = val;
}

void msrPageLayout::setEvenMarginsGroup (
  int               inputLineNumber,
  const S_msrMarginsGroup& val)
{
  if (fBothMarginsGroup) {
    std::stringstream ss;

    ss <<
      "setting an even margins group when there is already a both margins group in " <<
      this->asString ();

    msrError (
      gServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      ss.str ());
  }

  fEvenMarginsGroup = val;
}

void msrPageLayout::setBothMarginsGroup (
  int               inputLineNumber,
  const S_msrMarginsGroup& val)
{
  if (fOddMarginsGroup || fEvenMarginsGroup) {
    std::stringstream ss;

    ss <<
      "setting a both margins group when there is already an odd or even margins group in " <<
      this->asString ();

    msrError (
      gServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      ss.str ());
  }

  fBothMarginsGroup = val;
}


S_msrMargin msrPageLayout::getSingleLeftMargin () const
{
  // heuristic, could be refined with suitable options JMI
  S_msrMargin result;

  if (fBothMarginsGroup) {
    result = fBothMarginsGroup->getLeftMargin ();
  }
  else if (fOddMarginsGroup) {
    result = fOddMarginsGroup->getLeftMargin ();
  }
  else if (fEvenMarginsGroup) {
    result = fEvenMarginsGroup->getLeftMargin ();
  }

  return result;
}

S_msrMargin msrPageLayout::getSingleRightMargin () const
{
  // heuristic, could be refined with suitable options JMI
  S_msrMargin result;

  if (fBothMarginsGroup) {
    result = fBothMarginsGroup->getRightMargin ();
  }
  else if (fOddMarginsGroup) {
    result = fOddMarginsGroup->getRightMargin ();
  }
  else if (fEvenMarginsGroup) {
    result = fEvenMarginsGroup->getRightMargin ();
  }

  return result;
}

S_msrMargin msrPageLayout::getSingleTopMargin () const
{
  // heuristic, could be refined with suitable options JMI
  S_msrMargin result;

  if (fBothMarginsGroup) {
    result = fBothMarginsGroup->getTopMargin ();
  }
  else if (fOddMarginsGroup) {
    result = fOddMarginsGroup->getTopMargin ();
  }
  else if (fEvenMarginsGroup) {
    result = fEvenMarginsGroup->getTopMargin ();
  }

  return result;
}

S_msrMargin msrPageLayout::getSingleBottomMargin () const
{
  // heuristic, could be refined with suitable options JMI
  S_msrMargin result;

  if (fBothMarginsGroup) {
    result = fBothMarginsGroup->getBottomMargin ();
  }
  else if (fOddMarginsGroup) {
    result = fOddMarginsGroup->getBottomMargin ();
  }
  else if (fEvenMarginsGroup) {
    result = fEvenMarginsGroup->getBottomMargin ();
  }

  return result;
}

void msrPageLayout::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrPageLayout::acceptIn ()" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif

  if (visitor<S_msrPageLayout>*
    p =
      dynamic_cast<visitor<S_msrPageLayout>*> (v)) {
        S_msrPageLayout elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrPageLayout::visitStart ()" <<
            std::endl;

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif
        p->visitStart (elem);
  }
}

void msrPageLayout::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrPageLayout::acceptOut ()" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif

  if (visitor<S_msrPageLayout>*
    p =
      dynamic_cast<visitor<S_msrPageLayout>*> (v)) {
        S_msrPageLayout elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrPageLayout::visitEnd ()" <<
            std::endl;

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif        
        p->visitEnd (elem);
  }
}

void msrPageLayout::browseData (basevisitor* v)
{
/* JMI
  if (fOddMarginsGroup) {
    // browse the odd margins group
    msrBrowser<msrMarginsGroup> browser (v);
    browser.browse (*fOddMarginsGroup);
  }

  if (fEvenMarginsGroup) {
    // browse the even margins group
    msrBrowser<msrMarginsGroup> browser (v);
    browser.browse (*fEvenMarginsGroup);
  }

  if (fBothMarginsGroup) {
    // browse the both margins group
    msrBrowser<msrMarginsGroup> browser (v);
    browser.browse (*fBothMarginsGroup);
  }
  */
}

std::string msrPageLayout::asString () const
{
  std::stringstream ss;

  ss <<
    "[PageLayout" <<
    ", line " << fInputLineNumber <<
    ']';

  return ss.str ();
}

void msrPageLayout::print (std::ostream& os) const
{
  os << "[PageLayout" << std::endl;

  const int fieldWidth = 17;

  ++gIndenter;

  // page size
  os << std::left <<
    std::setw (fieldWidth) <<
    "fPageHeight" << ": ";
    if (fPageHeight) {
      os << fPageHeight;
    }
    else {
      os << "[NONE]";
    }
  os << std::endl;

  os << std::left <<
    std::setw (fieldWidth) <<
    "fPageWidth" << ": ";
    if (fPageWidth) {
      os << fPageWidth;
    }
    else {
      os << "[NONE]";
    }
  os << std::endl;

  // margins groups
  os << std::left <<
    std::setw (fieldWidth) <<
    "fOddMarginsGroup" << ": ";
    if (fOddMarginsGroup) {
      os << fOddMarginsGroup;
    }
    else {
      os << "[NONE]" << std::endl;
    }

  os << std::left <<
    std::setw (fieldWidth) <<
    "fEvenMarginsGroup" << ": ";
    if (fEvenMarginsGroup) {
      os << fEvenMarginsGroup;
    }
    else {
      os << "[NONE]" << std::endl;
    }

  os << std::left <<
    std::setw (fieldWidth) <<
    "fBothMarginsGroup" << ": ";
    if (fBothMarginsGroup) {
      os << fBothMarginsGroup;
    }
    else {
      os << "[NONE]" << std::endl;
    }

  --gIndenter;

  os << ']' << std::endl;
}

std::ostream& operator << (std::ostream& os, const S_msrPageLayout& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NONE]" << std::endl;
  }

  return os;
}

//______________________________________________________________________________
S_msrSystemLayout msrSystemLayout::create (
  int inputLineNumber)
{
  msrSystemLayout* o =
    new msrSystemLayout (
      inputLineNumber);
  assert (o != nullptr);
  return o;
}

msrSystemLayout::msrSystemLayout (
  int inputLineNumber)
    : msrElement (inputLineNumber)
{}

msrSystemLayout::~msrSystemLayout ()
{}

void msrSystemLayout::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrSystemLayout::acceptIn ()" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif

  if (visitor<S_msrSystemLayout>*
    p =
      dynamic_cast<visitor<S_msrSystemLayout>*> (v)) {
        S_msrSystemLayout elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrSystemLayout::visitStart ()" <<
            std::endl;

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif
        p->visitStart (elem);
  }
}

void msrSystemLayout::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrSystemLayout::acceptOut ()" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif

  if (visitor<S_msrSystemLayout>*
    p =
      dynamic_cast<visitor<S_msrSystemLayout>*> (v)) {
        S_msrSystemLayout elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrSystemLayout::visitEnd ()" <<
            std::endl;

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif        
        p->visitEnd (elem);
  }
}

void msrSystemLayout::browseData (basevisitor* v)
{
/* JMI
    // margins
    S_msrMargin           fLeftMargin;
    S_msrMargin           fRightMargin;

    // distances
    S_msrLength           fSystemDistance;
    S_msrLength           fTopSystemDistance;
    */
}

std::string msrSystemLayout::asString () const
{
  std::stringstream ss;

  ss <<
    "[SystemLayout" <<
    ", line " << fInputLineNumber <<
    ']';

  return ss.str ();
}

void msrSystemLayout::print (std::ostream& os) const
{
  os << "[SystemLayout" << std::endl;

  const int fieldWidth = 18;

  ++gIndenter;

  // margins
  os << std::left <<
    std::setw (fieldWidth) <<
    "fLeftMargin" << ": ";
    if (fLeftMargin) {
      os << fLeftMargin;
    }
    else {
      os << "[NONE]";
    }
  os << std::endl;

  os << std::left <<
    std::setw (fieldWidth) <<
    "fRightMargin" << ": ";
    if (fRightMargin) {
      os << fRightMargin;
    }
    else {
      os << "[NONE]";
    }
  os << std::endl;

  // distances
  os << std::left <<
    std::setw (fieldWidth) <<
    "fSystemDistance" << ": ";
    if (fSystemDistance) {
      os << fSystemDistance;
    }
    else {
      os << "[NONE]";
    }
  os << std::endl;

  os << std::left <<
    std::setw (fieldWidth) <<
    "fTopSystemDistance" << ": ";
    if (fTopSystemDistance) {
      os << fTopSystemDistance;
    }
    else {
      os << "[NONE]";
    }
  os << std::endl;

  --gIndenter;

  os << ']' << std::endl;
}

std::ostream& operator << (std::ostream& os, const S_msrSystemLayout& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NONE]" << std::endl;
  }

  return os;
}

//______________________________________________________________________________
S_msrSystemDividers msrSystemDividers::create (
  int inputLineNumber)
{
  msrSystemDividers* o =
    new msrSystemDividers (
      inputLineNumber);
  assert (o != nullptr);
  return o;
}

msrSystemDividers::msrSystemDividers (
  int inputLineNumber)
    : msrElement (inputLineNumber)
{
  fLeftDivider  = false;
  fRightDivider = false;
}

msrSystemDividers::~msrSystemDividers ()
{}

void msrSystemDividers::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrSystemDividers::acceptIn ()" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif

  if (visitor<S_msrSystemDividers>*
    p =
      dynamic_cast<visitor<S_msrSystemDividers>*> (v)) {
        S_msrSystemDividers elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrSystemDividers::visitStart ()" <<
            std::endl;

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif
        p->visitStart (elem);
  }
}

void msrSystemDividers::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrSystemDividers::acceptOut ()" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif

  if (visitor<S_msrSystemDividers>*
    p =
      dynamic_cast<visitor<S_msrSystemDividers>*> (v)) {
        S_msrSystemDividers elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrSystemDividers::visitEnd ()" <<
            std::endl;

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif        
        p->visitEnd (elem);
  }
}

void msrSystemDividers::browseData (basevisitor* v)
{}

std::string msrSystemDividers::asString () const
{
  std::stringstream ss;

  ss <<
    "[SystemDividers" <<
    ", line " << fInputLineNumber <<
    ']';

  return ss.str ();
}

void msrSystemDividers::print (std::ostream& os) const
{
  os << "[SystemDividers" << std::endl;

  const int fieldWidth = 13;

  ++gIndenter;

  os << std::left <<
    std::setw (fieldWidth) <<
    "fLeftDivider" << ": " << fLeftDivider <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fRightDivider" << ": " << fRightDivider <<
    std::endl;

  --gIndenter;

  os << ']' << std::endl;
}

std::ostream& operator << (std::ostream& os, const S_msrSystemDividers& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NONE]" << std::endl;
  }

  return os;
}

//______________________________________________________________________________
S_msrStaffLayout msrStaffLayout::create (
  int inputLineNumber,
  int staffNumber)
{
  msrStaffLayout* o =
    new msrStaffLayout (
      inputLineNumber,
      staffNumber);
  assert (o != nullptr);
  return o;
}

msrStaffLayout::msrStaffLayout (
  int inputLineNumber,
  int staffNumber)
    : msrElement (inputLineNumber)
{
  fStaffNumber = staffNumber;
}

msrStaffLayout::~msrStaffLayout ()
{}

void msrStaffLayout::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrStaffLayout::acceptIn ()" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif

  if (visitor<S_msrStaffLayout>*
    p =
      dynamic_cast<visitor<S_msrStaffLayout>*> (v)) {
        S_msrStaffLayout elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrStaffLayout::visitStart ()" <<
            std::endl;

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif
        p->visitStart (elem);
  }
}

void msrStaffLayout::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrStaffLayout::acceptOut ()" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif

  if (visitor<S_msrStaffLayout>*
    p =
      dynamic_cast<visitor<S_msrStaffLayout>*> (v)) {
        S_msrStaffLayout elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrStaffLayout::visitEnd ()" <<
            std::endl;

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif        
        p->visitEnd (elem);
  }
}

void msrStaffLayout::browseData (basevisitor* v)
{}

std::string msrStaffLayout::asString () const
{
  std::stringstream ss;

  ss <<
    "[StaffLayout" <<
    ", fStaffDistance: " << fStaffDistance <<
    ", line " << fInputLineNumber <<
    ']';

  return ss.str ();
}

void msrStaffLayout::print (std::ostream& os) const
{
  os << "[StaffLayout" << std::endl;

  const int fieldWidth = 14;

  ++gIndenter;

  // staff number
  os << std::left <<
    std::setw (fieldWidth) <<
    "fStaffNumber" << ": " << fStaffNumber <<
    std::endl;

  // staff distance
  os << std::left <<
    std::setw (fieldWidth) <<
    "fStaffDistance" << ": ";
  if (fStaffDistance) {
    os << fStaffDistance;
  }
  os << std::endl;

  --gIndenter;

  os << ']' << std::endl;
}

std::ostream& operator << (std::ostream& os, const S_msrStaffLayout& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NONE]" << std::endl;
  }

  return os;
}

//______________________________________________________________________________
S_msrMeasureLayout msrMeasureLayout::create (
  int inputLineNumber)
{
  msrMeasureLayout* o =
    new msrMeasureLayout (
      inputLineNumber);
  assert (o != nullptr);
  return o;
}

msrMeasureLayout::msrMeasureLayout (
  int inputLineNumber)
    : msrElement (inputLineNumber)
{}

msrMeasureLayout::~msrMeasureLayout ()
{}

void msrMeasureLayout::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrMeasureLayout::acceptIn ()" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif

  if (visitor<S_msrMeasureLayout>*
    p =
      dynamic_cast<visitor<S_msrMeasureLayout>*> (v)) {
        S_msrMeasureLayout elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrMeasureLayout::visitStart ()" <<
            std::endl;

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif
        p->visitStart (elem);
  }
}

void msrMeasureLayout::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrMeasureLayout::acceptOut ()" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif

  if (visitor<S_msrMeasureLayout>*
    p =
      dynamic_cast<visitor<S_msrMeasureLayout>*> (v)) {
        S_msrMeasureLayout elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrMeasureLayout::visitEnd ()" <<
            std::endl;

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif        
        p->visitEnd (elem);
  }
}

void msrMeasureLayout::browseData (basevisitor* v)
{}

std::string msrMeasureLayout::asString () const
{
  std::stringstream ss;

  ss <<
    "[MeasureLayout" <<
    ", fMeasureDistance: " << fMeasureDistance <<
    ", line " << fInputLineNumber <<
    ']';

  return ss.str ();
}

void msrMeasureLayout::print (std::ostream& os) const
{
  os << "[MeasureLayout" << std::endl;

  const int fieldWidth = 14;

  ++gIndenter;

  // measure distance
  os << std::left <<
    std::setw (fieldWidth) <<
    "fMeasureDistance" << ": " << fMeasureDistance <<
    std::endl;

  --gIndenter;

  os << ']' << std::endl;
}

std::ostream& operator << (std::ostream& os, const S_msrMeasureLayout& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NONE]" << std::endl;
  }

  return os;
}

//______________________________________________________________________________
S_msrLineWidth msrLineWidth::create (
  int                  inputLineNumber,
  msrLineWidthTypeKind lineWidthTypeKind,
  const S_msrLength&   lineWidthValue)
{
  msrLineWidth* o =
    new msrLineWidth (
      inputLineNumber,
      lineWidthTypeKind,
      lineWidthValue);
  assert (o != nullptr);
  return o;
}

msrLineWidth::msrLineWidth (
  int                  inputLineNumber,
  msrLineWidthTypeKind lineWidthTypeKind,
  const S_msrLength&   lineWidthValue)
    : msrElement (inputLineNumber)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceMusicXMLPrintLayouts ()) {
		std::stringstream ss;

    ss <<
      "Creating a print layout " <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif

  fLineWidthTypeKind = lineWidthTypeKind;
  fLineWidthValue    = lineWidthValue;
}

msrLineWidth::~msrLineWidth ()
{}

void msrLineWidth::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrLineWidth::acceptIn ()" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif

  if (visitor<S_msrLineWidth>*
    p =
      dynamic_cast<visitor<S_msrLineWidth>*> (v)) {
        S_msrLineWidth elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrLineWidth::visitStart ()" <<
            std::endl;

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif
        p->visitStart (elem);
  }
}

void msrLineWidth::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrLineWidth::acceptOut ()" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif

  if (visitor<S_msrLineWidth>*
    p =
      dynamic_cast<visitor<S_msrLineWidth>*> (v)) {
        S_msrLineWidth elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrLineWidth::visitEnd ()" <<
            std::endl;

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif        
        p->visitEnd (elem);
  }
}

void msrLineWidth::browseData (basevisitor* v)
{}

std::string msrLineWidth::asString () const
{
  std::stringstream ss;

  ss <<
    "[LineWidth" <<
    ", fLineWidthTypeKind: " << fLineWidthTypeKind <<
     ", lineWidthValue: ";
    if (fLineWidthValue) {
      ss << fLineWidthValue->asString();
    }
    else {
      ss << "[NONE]";
    }
  ss <<
    ", line " << fInputLineNumber <<
    ']';

  return ss.str ();
}

void msrLineWidth::print (std::ostream& os) const
{
  os <<
    "[LineWidth" <<
    ", line " << fInputLineNumber <<
    std::endl;

  ++gIndenter;

  const int fieldWidth = 18;

  os << std::left <<
    std::setw (fieldWidth) <<
    "fLineWidthTypeKind" << ": " << fLineWidthTypeKind <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fLineWidthValue" <<  ": ";
    if (fLineWidthValue) {
      os << fLineWidthValue;
    }
    else {
      os << "[NONE]";
    }
  os << std::endl;

  --gIndenter;

  os << ']' << std::endl;
}

std::ostream& operator << (std::ostream& os, const S_msrLineWidth& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NONE]" << std::endl;
  }

  return os;
}

//______________________________________________________________________________
S_msrNoteSize msrNoteSize::create (
  int                 inputLineNumber,
  msrNoteSizeTypeKind noteSizeTypeKind,
  float               noteSizeValue)
{
  msrNoteSize* o =
    new msrNoteSize (
      inputLineNumber,
      noteSizeTypeKind,
      noteSizeValue);
  assert (o != nullptr);
  return o;
}

msrNoteSize::msrNoteSize (
  int                 inputLineNumber,
  msrNoteSizeTypeKind noteSizeTypeKind,
  float               noteSizeValue)
    : msrElement (inputLineNumber)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceMusicXMLPrintLayouts ()) {
		std::stringstream ss;

    ss <<
      "Creating a print layout " <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif

  fNoteSizeTypeKind = noteSizeTypeKind;
  fNoteSizeValue    = noteSizeValue;
}

msrNoteSize::~msrNoteSize ()
{}

void msrNoteSize::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrNoteSize::acceptIn ()" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif

  if (visitor<S_msrNoteSize>*
    p =
      dynamic_cast<visitor<S_msrNoteSize>*> (v)) {
        S_msrNoteSize elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrNoteSize::visitStart ()" <<
            std::endl;

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif
        p->visitStart (elem);
  }
}

void msrNoteSize::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrNoteSize::acceptOut ()" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif

  if (visitor<S_msrNoteSize>*
    p =
      dynamic_cast<visitor<S_msrNoteSize>*> (v)) {
        S_msrNoteSize elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrNoteSize::visitEnd ()" <<
            std::endl;

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif        
        p->visitEnd (elem);
  }
}

void msrNoteSize::browseData (basevisitor* v)
{}

std::string msrNoteSize::asString () const
{
  std::stringstream ss;

  ss <<
    "[NoteSize" <<
    ", fNoteSizeTypeKind: " << fNoteSizeTypeKind <<
    ", fNoteSizeValue: " << fNoteSizeValue <<
    ", line " << fInputLineNumber <<
    ']';

  return ss.str ();
}

void msrNoteSize::print (std::ostream& os) const
{
  os <<
    "[NoteSize" <<
    ", line " << fInputLineNumber <<
    std::endl;

  ++gIndenter;

  const int fieldWidth = 18;

  os << std::left <<
    std::setw (fieldWidth) <<
    "fNoteSizeTypeKind" << ": " << fNoteSizeTypeKind <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fNoteSizeValue" <<  ": " << fNoteSizeValue <<
    std::endl;

  --gIndenter;

  os << ']' << std::endl;
}

std::ostream& operator << (std::ostream& os, const S_msrNoteSize& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NONE]" << std::endl;
  }

  return os;
}

//______________________________________________________________________________
S_msrDistance msrDistance::create (
  int                 inputLineNumber,
  msrDistanceTypeKind distanceTypeKind,
  S_msrLength         distanceValue)
{
  msrDistance* o =
    new msrDistance (
      inputLineNumber,
      distanceTypeKind,
      distanceValue);
  assert (o != nullptr);
  return o;
}

msrDistance::msrDistance (
  int                 inputLineNumber,
  msrDistanceTypeKind distanceTypeKind,
  S_msrLength         distanceValue)
    : msrElement (inputLineNumber)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceMusicXMLPrintLayouts ()) {
		std::stringstream ss;

    ss <<
      "Creating a print layout " <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif

  fDistanceTypeKind = distanceTypeKind;
  fDistanceValue    = distanceValue;
}

msrDistance::~msrDistance ()
{}

void msrDistance::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrDistance::acceptIn ()" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif

  if (visitor<S_msrDistance>*
    p =
      dynamic_cast<visitor<S_msrDistance>*> (v)) {
        S_msrDistance elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrDistance::visitStart ()" <<
            std::endl;

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif
        p->visitStart (elem);
  }
}

void msrDistance::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrDistance::acceptOut ()" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif

  if (visitor<S_msrDistance>*
    p =
      dynamic_cast<visitor<S_msrDistance>*> (v)) {
        S_msrDistance elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrDistance::visitEnd ()" <<
            std::endl;

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif        
        p->visitEnd (elem);
  }
}

void msrDistance::browseData (basevisitor* v)
{}

std::string msrDistance::asString () const
{
  std::stringstream ss;

  ss <<
    "[Distance" <<
    ", fDistanceTypeKind: " << fDistanceTypeKind <<
    ", fDistanceValue: " << fDistanceValue <<
    ", line " << fInputLineNumber <<
    ']';

  return ss.str ();
}

void msrDistance::print (std::ostream& os) const
{
  os <<
    "[Distance" <<
    ", line " << fInputLineNumber <<
    std::endl;

  ++gIndenter;

  const int fieldWidth = 18;

  os << std::left <<
    std::setw (fieldWidth) <<
    "fDistanceTypeKind" << ": " << fDistanceTypeKind <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fDistanceValue" <<  ": " << fDistanceValue <<
    std::endl;

  --gIndenter;

  os << ']' << std::endl;
}

std::ostream& operator << (std::ostream& os, const S_msrDistance& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NONE]" << std::endl;
  }

  return os;
}

//______________________________________________________________________________
S_msrGlyph msrGlyph::create (
  int              inputLineNumber,
  msrGlyphTypeKind glyphTypeKind,
  const std::string&    glyphValue)
{
  msrGlyph* o =
    new msrGlyph (
      inputLineNumber,
      glyphTypeKind,
      glyphValue);
  assert (o != nullptr);
  return o;
}

msrGlyph::msrGlyph (
  int              inputLineNumber,
  msrGlyphTypeKind glyphTypeKind,
  const std::string&    glyphValue)
    : msrElement (inputLineNumber)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceMusicXMLPrintLayouts ()) {
		std::stringstream ss;

    ss <<
      "Creating a print layout " <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif

  fGlyphTypeKind = glyphTypeKind;
  fGlyphValue    = glyphValue;
}

msrGlyph::~msrGlyph ()
{}

void msrGlyph::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrGlyph::acceptIn ()" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif

  if (visitor<S_msrGlyph>*
    p =
      dynamic_cast<visitor<S_msrGlyph>*> (v)) {
        S_msrGlyph elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrGlyph::visitStart ()" <<
            std::endl;

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif
        p->visitStart (elem);
  }
}

void msrGlyph::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrGlyph::acceptOut ()" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif

  if (visitor<S_msrGlyph>*
    p =
      dynamic_cast<visitor<S_msrGlyph>*> (v)) {
        S_msrGlyph elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrGlyph::visitEnd ()" <<
            std::endl;

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif        
        p->visitEnd (elem);
  }
}

void msrGlyph::browseData (basevisitor* v)
{}

std::string msrGlyph::asString () const
{
  std::stringstream ss;

  ss <<
    "[Glyph" <<
    ", fGlyphTypeKind: " << fGlyphTypeKind <<
     ", fGlyphValue: " << fGlyphValue <<
    ", line " << fInputLineNumber <<
    ']';

  return ss.str ();
}

void msrGlyph::print (std::ostream& os) const
{
  os <<
    "[Glyph" <<
    ", line " << fInputLineNumber <<
    std::endl;

  ++gIndenter;

  const int fieldWidth = 18;

  os << std::left <<
    std::setw (fieldWidth) <<
    "glyphTypeKind" << ": " << fGlyphTypeKind <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fGlyphValue" <<  ": \"" << fGlyphValue << "\"" <<
    std::endl;

  --gIndenter;

  os << ']' << std::endl;
}

std::ostream& operator << (std::ostream& os, const S_msrGlyph& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NONE]" << std::endl;
  }

  return os;
}

//______________________________________________________________________________
S_msrOtherAppearance msrOtherAppearance::create (
  int                        inputLineNumber,
  msrOtherAppearanceTypeKind otherAppearanceTypeKind,
  const std::string&    otherAppearanceValue)
{
  msrOtherAppearance* o =
    new msrOtherAppearance (
      inputLineNumber,
      otherAppearanceTypeKind,
      otherAppearanceValue);
  assert (o != nullptr);
  return o;
}

msrOtherAppearance::msrOtherAppearance (
  int                        inputLineNumber,
  msrOtherAppearanceTypeKind otherAppearanceTypeKind,
  const std::string&    otherAppearanceValue)
    : msrElement (inputLineNumber)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceMusicXMLPrintLayouts ()) {
		std::stringstream ss;

    ss <<
      "Creating a print layout " <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif

  fOtherAppearanceTypeKind = otherAppearanceTypeKind;
  fOtherAppearanceValue    = otherAppearanceValue;
}

msrOtherAppearance::~msrOtherAppearance ()
{}

void msrOtherAppearance::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrOtherAppearance::acceptIn ()" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif

  if (visitor<S_msrOtherAppearance>*
    p =
      dynamic_cast<visitor<S_msrOtherAppearance>*> (v)) {
        S_msrOtherAppearance elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrOtherAppearance::visitStart ()" <<
            std::endl;

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif
        p->visitStart (elem);
  }
}

void msrOtherAppearance::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrOtherAppearance::acceptOut ()" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif

  if (visitor<S_msrOtherAppearance>*
    p =
      dynamic_cast<visitor<S_msrOtherAppearance>*> (v)) {
        S_msrOtherAppearance elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrOtherAppearance::visitEnd ()" <<
            std::endl;

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif        
        p->visitEnd (elem);
  }
}

void msrOtherAppearance::browseData (basevisitor* v)
{}

std::string msrOtherAppearance::asString () const
{
  std::stringstream ss;

  ss <<
    "[OtherAppearance" <<
    ", otherAppearanceTypeKind: " << fOtherAppearanceTypeKind <<
    ", fOtherAppearanceValue: " << fOtherAppearanceValue <<
    ", line " << fInputLineNumber <<
    ']';

  return ss.str ();
}

void msrOtherAppearance::print (std::ostream& os) const
{
  os <<
    "[OtherAppearance" <<
    ", line " << fInputLineNumber <<
    std::endl;

  ++gIndenter;

  const int fieldWidth = 18;

  os << std::left <<
    std::setw (fieldWidth) <<
    "fOtherAppearanceTypeKind" << ": " << fOtherAppearanceTypeKind <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fOtherAppearanceValue" <<  ": " << fOtherAppearanceValue <<
    std::endl;

  --gIndenter;

  os << ']' << std::endl;
}

std::ostream& operator << (std::ostream& os, const S_msrOtherAppearance& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NONE]" << std::endl;
  }

  return os;
}

//______________________________________________________________________________
S_msrAppearance msrAppearance::create (
  int inputLineNumber)
{
  msrAppearance* o =
    new msrAppearance (
      inputLineNumber);
  assert (o != nullptr);
  return o;
}

msrAppearance::msrAppearance (
  int inputLineNumber)
    : msrElement (inputLineNumber)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceMusicXMLPrintLayouts ()) {
		std::stringstream ss;

    ss <<
      "Creating an appearance" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif
}

msrAppearance::~msrAppearance ()
{}

void msrAppearance::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrAppearance::acceptIn ()" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif

  if (visitor<S_msrAppearance>*
    p =
      dynamic_cast<visitor<S_msrAppearance>*> (v)) {
        S_msrAppearance elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrAppearance::visitStart ()" <<
            std::endl;

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif
        p->visitStart (elem);
  }
}

void msrAppearance::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrAppearance::acceptOut ()" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif

  if (visitor<S_msrAppearance>*
    p =
      dynamic_cast<visitor<S_msrAppearance>*> (v)) {
        S_msrAppearance elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrAppearance::visitEnd ()" <<
            std::endl;

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif        
        p->visitEnd (elem);
  }
}

void msrAppearance::browseData (basevisitor* v)
{
/*
    std::list<S_msrLineWidth>  fLineWidthsList;

    std::list<S_msrNoteSize>   fNoteSizesList;

    std::list<S_msrDistance>   fDistancesList;

    std::list<S_msrGlyph>      fGlyphsList;

    std::list<S_msrOtherAppearance>
                          fOtherAppearancesList;


  if (fPageLayout) {
    // browse the page layout
    msrBrowser<msrPageLayout> browser (v);
    browser.browse (*fPageLayout);
  }

  if (fSystemLayout) {
    // browse the system layout
    msrBrowser<msrSystemLayout> browser (v);
    browser.browse (*fSystemLayout);
  }

  if (fSystemDividers) {
    // browse the system dividers
    msrBrowser<msrSystemDividers> browser (v);
    browser.browse (*fSystemDividers);
  }
  */
}

std::string msrAppearance::asString () const
{
  std::stringstream ss;

  ss <<
    "[Appearance" <<
    /*
    ", otherAppearanceTypeKind: " <<
    msrOtherAppearanceTypeKindAsString (fOtherAppearanceTypeKind) <<
    ", otherAppearanceValue: " <<  fOtherAppearanceValue <<
    */
    ", line " << fInputLineNumber <<
    ']';

  return ss.str ();
}

void msrAppearance::print (std::ostream& os) const
{
  os <<
    "[Appearance" <<
    ", line " << fInputLineNumber <<
    std::endl;

  ++gIndenter;

  const int fieldWidth = 18;

  // line widths
  size_t
    lineWidthsListSize =
      fLineWidthsList.size ();

  os <<
    std::setw (fieldWidth) <<
    "lineWidthsList" << ": " <<
    mfSingularOrPlural (
      lineWidthsListSize, "element", "elements") <<
    std::endl;

  if (lineWidthsListSize) {
    ++gIndenter;

    std::list<S_msrLineWidth>::const_iterator
      iBegin = fLineWidthsList.begin (),
      iEnd   = fLineWidthsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
 // JMI     os << std::endl;
    } // for

    --gIndenter;
  }

  // note sizes
  size_t
    noteSizesListSize =
      fNoteSizesList.size ();

  os <<
    std::setw (fieldWidth) <<
    "noteSizesList" << ": " <<
    mfSingularOrPlural (
      noteSizesListSize, "element", "elements") <<
    std::endl;

  if (noteSizesListSize) {
    ++gIndenter;

    std::list<S_msrNoteSize>::const_iterator
      iBegin = fNoteSizesList.begin (),
      iEnd   = fNoteSizesList.end (),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
 // JMI     os << std::endl;
    } // for

    --gIndenter;
  }

  // distances
  size_t
    distancesListSize =
      fDistancesList.size ();

  os <<
    std::setw (fieldWidth) <<
    "distancesList" << ": " <<
    mfSingularOrPlural (
      distancesListSize, "element", "elements") <<
    std::endl;

  if (distancesListSize) {
    ++gIndenter;

    std::list<S_msrDistance>::const_iterator
      iBegin = fDistancesList.begin (),
      iEnd   = fDistancesList.end (),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
 // JMI     os << std::endl;
    } // for

    --gIndenter;
  }

  // glyphs
  size_t
    glyphsListSize =
      fGlyphsList.size ();

  os <<
    std::setw (fieldWidth) <<
    "glyphsList" << ": " <<
    mfSingularOrPlural (
      glyphsListSize, "element", "elements") <<
    std::endl;

  if (glyphsListSize) {
    ++gIndenter;

    std::list<S_msrGlyph>::const_iterator
      iBegin = fGlyphsList.begin (),
      iEnd   = fGlyphsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
 // JMI     os << std::endl;
    } // for

    --gIndenter;
  }

  // other appearances
  size_t
    otherAppearancesListSize =
      fOtherAppearancesList.size ();

  os <<
    std::setw (fieldWidth) <<
    "otherAppearancesList" << ": " <<
    mfSingularOrPlural (
      otherAppearancesListSize, "element", "elements") <<
    std::endl;

  if (otherAppearancesListSize) {
    ++gIndenter;

    std::list<S_msrOtherAppearance>::const_iterator
      iBegin = fOtherAppearancesList.begin (),
      iEnd   = fOtherAppearancesList.end (),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
 // JMI     os << std::endl;
    } // for

    --gIndenter;
  }

  --gIndenter;

  os << ']' << std::endl;
}

std::ostream& operator << (std::ostream& os, const S_msrAppearance& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NONE]" << std::endl;
  }

  return os;
}


}
