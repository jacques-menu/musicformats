/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <sstream>
#include <iomanip>      // setw()), set::precision(), ...

#include "mfIndentedTextOutput.h"
#include "mfServices.h"
#include "msrWae.h"

#include "msrMargins.h"


namespace MusicFormats
{

// margins types
//______________________________________________________________________________

std::map<std::string, msrMarginTypeKind>
  gGlobalMsrMarginTypeKindsMap;

std::string msrMarginTypeKindAsString (
  msrMarginTypeKind marginTypeKind)
{
  std::string result;

  // no CamelCase here, these strings are used in the command line options

  switch (marginTypeKind) {
    case msrMarginTypeKind::kMarginOdd:
      result = "odd";
      break;
    case msrMarginTypeKind::kMarginEven:
      result = "even";
      break;
    case msrMarginTypeKind::kMarginBoth: // default value
      result = "both";
      break;
  } // switch

  return result;
}

std::ostream& operator << (std::ostream& os, const msrMarginTypeKind& elt)
{
  os << msrMarginTypeKindAsString (elt);
  return os;
}

void initializeMsrMarginTypeKindsMap ()
{
  // register the LilyPond score output kinds
  // --------------------------------------

  // no CamelCase here, these strings are used in the command line options

  gGlobalMsrMarginTypeKindsMap ["odd"] = msrMarginTypeKind::kMarginOdd;
  gGlobalMsrMarginTypeKindsMap ["even"] = msrMarginTypeKind::kMarginEven;
  gGlobalMsrMarginTypeKindsMap ["both"] = msrMarginTypeKind::kMarginBoth;
}

std::string availableMsrMarginTypeKinds (size_t namesListMaxLength)
{
  std::stringstream ss;

  size_t msrMarginTypeKindsMapSize =
    gGlobalMsrMarginTypeKindsMap.size ();

  if (msrMarginTypeKindsMapSize) {
    size_t nextToLast =
      msrMarginTypeKindsMapSize - 1;

    size_t count = 0;
    size_t cumulatedLength = 0;

    for (
      std::map<std::string, msrMarginTypeKind>::const_iterator i =
        gGlobalMsrMarginTypeKindsMap.begin ();
      i != gGlobalMsrMarginTypeKindsMap.end ();
      ++i
    ) {
      std::string theString = (*i).first;

      ++count;

      cumulatedLength += theString.size ();
      if (cumulatedLength >= namesListMaxLength) {
        ss << std::endl << gIndenter.getSpacer ();
        cumulatedLength = 0;
      }

      if (count == 1) {
        ss << gIndenter.getSpacer ();
      }
      ss << theString;

      if (count == nextToLast) {
        ss << " and ";
      }
      else if (count != msrMarginTypeKindsMapSize) {
        ss << ", ";
      }
    } // for
  }

  return ss.str ();
}

// margins
//______________________________________________________________________________
S_msrMargin msrMargin::create (
  msrMarginTypeKind marginTypeKind,
  msrLength         marginLength)
{
  msrMargin * o =
    new msrMargin (
      marginTypeKind,
      marginLength);
  assert (o != nullptr);
  return o;
}

msrMargin::msrMargin (
  msrMarginTypeKind marginTypeKind,
  msrLength         marginLength)
{
  fMarginTypeKind = marginTypeKind;
  fMarginLength   = marginLength;
}

msrMargin::~msrMargin ()
{}

std::string msrMargin::asString () const
{
  std::stringstream ss;

  ss <<
    "[Margin, " <<
    std::setprecision (4) <<
    fMarginLength.asString () <<
    ' ' <<
    msrMarginTypeKindAsString (fMarginTypeKind) <<
    ']';

  return ss.str ();
}

void msrMargin::print (std::ostream& os) const
{
  os <<
    asString ();
};

std::ostream& operator << (std::ostream& os, const S_msrMargin& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NONE]" << std::endl;
  }

  return os;
}

// margins groups
//______________________________________________________________________________
S_msrMarginsGroup msrMarginsGroup::create (
  msrMarginTypeKind marginTypeKind)
{
  msrMarginsGroup * o =
    new msrMarginsGroup (
      marginTypeKind);
  assert (o != nullptr);
  return o;
}

msrMarginsGroup::msrMarginsGroup (
  msrMarginTypeKind marginTypeKind)
{
  fMarginsGroupTypeKind = marginTypeKind;
}

msrMarginsGroup::~msrMarginsGroup ()
{}

void msrMarginsGroup::setLeftMargin (
  int         inputLineNumber,
  const S_msrMargin& val)
{
  msrMarginTypeKind
    marginTypeKind =
      val->getMarginTypeKind ();

  if (marginTypeKind != fMarginsGroupTypeKind) {
    std::stringstream ss;

    ss <<
      "setting a " <<
      msrMarginTypeKindAsString (marginTypeKind) <<
      " left margin in a " <<
      msrMarginTypeKindAsString (fMarginsGroupTypeKind) <<
      " margins group";

    msrInternalError (
      gServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      ss.str ());
  }

  fLeftMargin = val;
}

void msrMarginsGroup::setRightMargin (
  int         inputLineNumber,
  const S_msrMargin& val)
{
  msrMarginTypeKind
    marginTypeKind =
      val->getMarginTypeKind ();

  if (marginTypeKind != fMarginsGroupTypeKind) {
    std::stringstream ss;

    ss <<
      "setting a " <<
      msrMarginTypeKindAsString (marginTypeKind) <<
      " right margin in a " <<
      msrMarginTypeKindAsString (fMarginsGroupTypeKind) <<
      " margins group";

    msrInternalError (
      gServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      ss.str ());
  }

  fRightMargin = val;
}

void msrMarginsGroup::setTopMargin (
  int         inputLineNumber,
  const S_msrMargin& val)
{
  msrMarginTypeKind
    marginTypeKind =
      val->getMarginTypeKind ();

  if (marginTypeKind != fMarginsGroupTypeKind) {
    std::stringstream ss;

    ss <<
      "setting a " <<
      msrMarginTypeKindAsString (marginTypeKind) <<
      " top margin in a " <<
      msrMarginTypeKindAsString (fMarginsGroupTypeKind) <<
      " margins group";

    msrInternalError (
      gServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      ss.str ());
  }

  fTopMargin = val;
}

void msrMarginsGroup::setBottomMargin (
  int         inputLineNumber,
  const S_msrMargin& val)
{
  msrMarginTypeKind
    marginTypeKind =
      val->getMarginTypeKind ();

  if (marginTypeKind != fMarginsGroupTypeKind) {
    std::stringstream ss;

    ss <<
      "setting a " <<
      msrMarginTypeKindAsString (marginTypeKind) <<
      " bottom margin in a " <<
      msrMarginTypeKindAsString (fMarginsGroupTypeKind) <<
      " margins group";

    msrInternalError (
      gServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      ss.str ());
  }

  fBottomMargin = val;
}

/* JMI
void msrMarginsGroup::acceptIn (basevisitor* v) {
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrMarginsGroup::acceptIn ()" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msrMarginsGroup>*
    p =
      dynamic_cast<visitor<S_msrMarginsGroup>*> (v)) {
        S_msrMarginsGroup elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrMarginsGroup::visitStart ()" <<
             std::endl;
        p->visitStart (elem);
  }
}

void msrMarginsGroup::acceptOut (basevisitor* v) {
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrMarginsGroup::acceptOut ()" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msrMarginsGroup>*
    p =
      dynamic_cast<visitor<S_msrMarginsGroup>*> (v)) {
        S_msrMarginsGroup elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrMarginsGroup::visitEnd ()" <<
            std::endl;
        p->visitEnd (elem);
  }
}

void msrMarginsGroup::browseData (basevisitor* v)
{}
*/

std::string msrMarginsGroup::asString () const
{
  std::stringstream ss;

  ss <<
    "[MarginsGroup, " <<
    msrMarginTypeKindAsString (fMarginsGroupTypeKind) <<
    ", leftMargin: " << fLeftMargin->asString () <<
    ", rightMargin: " << fRightMargin->asString () <<
    ", topMargin: " << fTopMargin->asString () <<
    ", bottomMargin: " << fBottomMargin->asString () <<
    ']';

  return ss.str ();
}

void msrMarginsGroup::print (std::ostream& os) const
{

  os <<
    "MarginsGroup" <<
    ", marginsGroupTypeKind: " <<
    msrMarginTypeKindAsString (fMarginsGroupTypeKind) <<
    std::endl;

  const int fieldWidth = 13;

  ++gIndenter;

  // margins
  os << std::left <<
    std::setw (fieldWidth) <<
    "leftMargin" << ": ";
    if (fLeftMargin) {
      os << fLeftMargin;
    }
    else {
      os << "[NONE]";
    }
  os << std::endl;

  os << std::left <<
    std::setw (fieldWidth) <<
    "rightMargin" << ": ";
    if (fRightMargin) {
      os << fRightMargin;
    }
    else {
      os << "[NONE]";
    }
  os << std::endl;

  os << std::left <<
    std::setw (fieldWidth) <<
    "topMargin" << ": ";
    if (fTopMargin) {
      os << fTopMargin;
    }
    else {
      os << "[NONE]";
    }
  os << std::endl;

  os << std::left <<
    std::setw (fieldWidth) <<
    "bottomMargin" << ": ";
    if (fBottomMargin) {
      os << fBottomMargin;
    }
    else {
      os << "[NONE]";
    }
  os << std::endl;

  --gIndenter;
};

std::ostream& operator << (std::ostream& os, const S_msrMarginsGroup& elt)
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

