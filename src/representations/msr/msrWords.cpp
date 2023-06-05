/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <iostream>
#include <sstream>
#include <iomanip>      // std::setw, std::setprecision, ...

#include "visitor.h"

#include "msrWords.h"

#include "oahOah.h"

#include "msrOah.h"

#include "waeHandlers.h"


namespace MusicFormats
{

//______________________________________________________________________________
S_msrWords msrWords::create (
  int                        inputLineNumber,
  msrPlacementKind           wordsPlacementKind,
  const std::string&         wordsContents,
  msrJustifyKind             wordsJustifyKind,
  msrHorizontalAlignmentKind wordsHorizontalAlignmentKind,
  msrVerticalAlignmentKind   wordsVerticalAlignmentKind,
  msrFontStyleKind           wordsFontStyleKind,
  const S_msrFontSize&       wordsFontSize,
  msrFontWeightKind          wordsFontWeightKind,
  msrXMLLangKind             wordsXMLLangKind,
  int                        wordsStaffNumber)
{
  msrWords* obj =
    new msrWords (
      inputLineNumber,
      wordsPlacementKind,
      wordsContents,
      wordsJustifyKind,
      wordsHorizontalAlignmentKind,
      wordsVerticalAlignmentKind,
      wordsFontStyleKind,
      wordsFontSize,
      wordsFontWeightKind,
      wordsXMLLangKind,
      wordsStaffNumber);
  assert (obj != nullptr);
  return obj;
}

msrWords::msrWords (
  int                        inputLineNumber,
  msrPlacementKind           wordsPlacementKind,
  const std::string&         wordsContents,
  msrJustifyKind             wordsJustifyKind,
  msrHorizontalAlignmentKind wordsHorizontalAlignmentKind,
  msrVerticalAlignmentKind   wordsVerticalAlignmentKind,
  msrFontStyleKind           wordsFontStyleKind,
  const S_msrFontSize&       wordsFontSize,
  msrFontWeightKind          wordsFontWeightKind,
  msrXMLLangKind             wordsXMLLangKind,
  int                        wordsStaffNumber)
    : msrElement (inputLineNumber)
{
  fWordsPlacementKind  = wordsPlacementKind;

  fWordsContents = wordsContents;

  fWordsJustifyKind = wordsJustifyKind;

  fWordsHorizontalAlignmentKind = wordsHorizontalAlignmentKind;
  fWordsVerticalAlignmentKind   = wordsVerticalAlignmentKind;

  fWordsFontStyleKind  = wordsFontStyleKind;
  fWordsFontSize       = wordsFontSize;
  fWordsFontWeightKind = wordsFontWeightKind;

  fWordsXMLLangKind = wordsXMLLangKind;

  fWordsStaffNumber = wordsStaffNumber;
}

msrWords::~msrWords ()
{}

void msrWords::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrWords::acceptIn ()";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msrWords>*
    p =
      dynamic_cast<visitor<S_msrWords>*> (v)) {
        S_msrWords elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrWords::visitStart ()";

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void msrWords::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrWords::acceptOut ()";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msrWords>*
    p =
      dynamic_cast<visitor<S_msrWords>*> (v)) {
        S_msrWords elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrWords::visitEnd ()";

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif
        p->visitEnd (elem);
  }
}

void msrWords::browseData (basevisitor* v)
{}

std::string msrWords::wordsPlacementKindAsString () const
{
  return
    msrPlacementKindAsString (
      fWordsPlacementKind);
}

std::string msrWords::wordsJustifyKindAsString () const
{
  return msrJustifyKindAsString (fWordsJustifyKind);
}

std::string msrWords::wordsHorizontalAlignmentKindAsString () const
{
  return
    msrHorizontalAlignmentKindAsString (
      fWordsHorizontalAlignmentKind);
}

std::string msrWords::wordsVerticalAlignmentKindAsString () const
{
  return
    msrVerticalAlignmentKindAsString (
      fWordsVerticalAlignmentKind);
}

std::string msrWords::msrWordsFontStyleKindAsString () const
{
  return msrFontStyleKindAsString (fWordsFontStyleKind);
}

std::string msrWords::wordsFontSizeAsString () const
{
  return fWordsFontSize->fontSizeAsString ();
}

std::string msrWords::wordsFontWeightKindAsString () const
{
  return msrFontWeightKindAsString (fWordsFontWeightKind);
}

std::string msrWords::asString () const
{
  std::stringstream ss;

  ss <<
    "Words" <<
    ", line " << fInputStartLineNumber <<
    ' ' <<
    fWordsContents << ", fWordsPlacementKind: " <<
    msrPlacementKindAsString (fWordsPlacementKind);

  return ss.str ();
}

void msrWords::print (std::ostream& os) const
{
// JMI  os << asString () << std::endl;

  os <<
    "[Words" <<
    ", line " << fInputStartLineNumber <<
    std::endl;

  ++gIndenter;

  const int fieldWidth = 29;

  os << std::left <<
    std::setw (fieldWidth) <<
    "fWordsContents" << ": \"" << fWordsContents << "\"" <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fWordsPlacementKind" << ": " <<
    msrPlacementKindAsString (fWordsPlacementKind) <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fWordsJustifyKind" << ": " <<
    msrJustifyKindAsString (fWordsJustifyKind) <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fWordsHorizontalAlignmentKind" << ": " <<
    msrHorizontalAlignmentKindAsString (fWordsHorizontalAlignmentKind) <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fWordsVerticalAlignmentKind" << ": " <<
    msrVerticalAlignmentKindAsString (fWordsVerticalAlignmentKind) <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fWordsVerticalAlignmentKind" << ": " <<
    msrFontStyleKindAsString (fWordsFontStyleKind) <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fWordsFontSize" << ": " <<
    fWordsFontSize->fontSizeAsString () <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fWordsFontWeightKind" << ": " <<
    msrFontWeightKindAsString (fWordsFontWeightKind) <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fWordsXMLLangKind" << ": " <<
    msrXMLLangKindAsString (fWordsXMLLangKind) <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fWordsStaffNumber" << ": " <<
    fWordsStaffNumber <<
    std::endl;

  --gIndenter;

  os << ']' << std::endl;
}

std::ostream& operator << (std::ostream& os, const S_msrWords& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NULL]" << std::endl;
  }

  return os;
}

mfIndentedStringStream& operator << (mfIndentedStringStream& iss, const S_msrWords& elt)
{
  if (elt) {
    iss.getStringstream () << elt;
  }
  else {
    iss << "[NULL]" << '\n';
  }

  return iss;
}


}
