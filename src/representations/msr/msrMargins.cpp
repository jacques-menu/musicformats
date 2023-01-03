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

#include "mfServiceRunData.h"

#include "msrMargins.h"

#include "msrWae.h"


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

std::string existingMsrMarginTypeKinds (size_t namesListMaxLength)
{
  std::stringstream s;

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
        s << std::endl << gIndenter.getSpacer ();
        cumulatedLength = 0;
      }

      if (count == 1) {
        s << gIndenter.getSpacer ();
      }
      s << theString;

      if (count == nextToLast) {
        s << " and ";
      }
      else if (count != msrMarginTypeKindsMapSize) {
        s << ", ";
      }
    } // for
  }

  return s.str ();
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
  std::stringstream s;

  s <<
    "[Margin, " <<
    std::setprecision (4) <<
    fMarginLength.asString () <<
    ' ' <<
    msrMarginTypeKindAsString (fMarginTypeKind) <<
    ']';

  return s.str ();
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
    std::stringstream s;

    s <<
      "setting a " <<
      msrMarginTypeKindAsString (marginTypeKind) <<
      " left margin in a " <<
      msrMarginTypeKindAsString (fMarginsGroupTypeKind) <<
      " margins group";

    msrInternalError (
      gGlobalServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
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
    std::stringstream s;

    s <<
      "setting a " <<
      msrMarginTypeKindAsString (marginTypeKind) <<
      " right margin in a " <<
      msrMarginTypeKindAsString (fMarginsGroupTypeKind) <<
      " margins group";

    msrInternalError (
      gGlobalServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
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
    std::stringstream s;

    s <<
      "setting a " <<
      msrMarginTypeKindAsString (marginTypeKind) <<
      " top margin in a " <<
      msrMarginTypeKindAsString (fMarginsGroupTypeKind) <<
      " margins group";

    msrInternalError (
      gGlobalServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
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
    std::stringstream s;

    s <<
      "setting a " <<
      msrMarginTypeKindAsString (marginTypeKind) <<
      " bottom margin in a " <<
      msrMarginTypeKindAsString (fMarginsGroupTypeKind) <<
      " margins group";

    msrInternalError (
      gGlobalServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }

  fBottomMargin = val;
}

/* JMI
void msrMarginsGroup::acceptIn (basevisitor* v) {
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrMarginsGroup::acceptIn ()" <<
      std::endl;
  }

  if (visitor<S_msrMarginsGroup>*
    p =
      dynamic_cast<visitor<S_msrMarginsGroup>*> (v)) {
        S_msrMarginsGroup elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrMarginsGroup::visitStart ()" <<
             std::endl;
        p->visitStart (elem);
  }
}

void msrMarginsGroup::acceptOut (basevisitor* v) {
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrMarginsGroup::acceptOut ()" <<
      std::endl;
  }

  if (visitor<S_msrMarginsGroup>*
    p =
      dynamic_cast<visitor<S_msrMarginsGroup>*> (v)) {
        S_msrMarginsGroup elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
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
  std::stringstream s;

  s <<
    "[MarginsGroup, " <<
    msrMarginTypeKindAsString (fMarginsGroupTypeKind) <<
    ", leftMargin: " << fLeftMargin->asString () <<
    ", rightMargin: " << fRightMargin->asString () <<
    ", topMargin: " << fTopMargin->asString () <<
    ", bottomMargin: " << fBottomMargin->asString () <<
    ']';

  return s.str ();
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

