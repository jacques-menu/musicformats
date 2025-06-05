/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <iostream>
#include <sstream>
#include <iomanip>      // std::setw, std::setprecision, ...

#include "visitor.h"

#include "msrSlashes.h"

#include "oahOah.h"

#include "msrOah.h"

#include "msrWae.h"

#include "waeHandlers.h"


namespace MusicFormats
{

// measure style
//______________________________________________________________________________
msrUseDotsKind msrUseDotsFromString (
  const mfInputLineNumber& inputLineNumber,
  const std::string& useDotsString)
{
  msrUseDotsKind result = msrUseDotsKind::kUseDotsNo; // default value

  if      (useDotsString == "yes")
    result = msrUseDotsKind::kUseDotsYes;
  else if (useDotsString == "no")
    result = msrUseDotsKind::kUseDotsNo;
  else {
    if (useDotsString.size ()) {
      std::stringstream ss;

      ss <<
        "use-dots \"" << useDotsString <<
        "\" is unknown";

      msrError (
        gServiceRunData->getInputSourceName (),
        inputLineNumber,
        __FILE__, __LINE__,
        ss.str ());
    }
  }

  return result;
}

std::string msrSlashTypeKindAsString (
  msrSlashTypeKind slashTypeKind)
{
  std::string result;

  switch (slashTypeKind) {
    case msrSlashTypeKind::kSlashType_UNKNOWN_:
      result = "kSlashType_UNKNOWN_";
      break;
    case msrSlashTypeKind::kSlashTypeStart:
      result = "kSlashTypeStart";
      break;
    case msrSlashTypeKind::kSlashTypeStop:
      result = "kSlashTypeStop";
      break;
  } // switch

  return result;
}

std::ostream& operator << (std::ostream& os, const msrSlashTypeKind& elt)
{
  os << msrSlashTypeKindAsString (elt);
  return os;
}

std::string msrUseDotsKindAsString (
  msrUseDotsKind useDotsKind)
{
  std::string result;

  switch (useDotsKind) {
    case msrUseDotsKind::kUseDots_UNKNOWN_:
      result = "kUseDots_UNKNOWN_";
      break;
    case msrUseDotsKind::kUseDotsYes:
      result = "kUseDotsYes";
      break;
    case msrUseDotsKind::kUseDotsNo:
      result = "kUseDotsNo";
      break;
  } // switch

  return result;
}

std::ostream& operator << (std::ostream& os, const msrUseDotsKind& elt)
{
  os << msrUseDotsKindAsString (elt);
  return os;
}

std::string msrSlashUseStemsKindAsString (
  msrSlashUseStemsKind slashUseStemsKind)
{
  std::string result;

  switch (slashUseStemsKind) {
    case msrSlashUseStemsKind::kSlashUseStems_UNKNOWN_:
      result = "kSlashUseStems_UNKNOWN_";
      break;
    case msrSlashUseStemsKind::kSlashUseStemsYes:
      result = "kSlashUseStemsYes";
      break;
    case msrSlashUseStemsKind::kSlashUseStemsNo:
      result = "kSlashUseStemsNo";
      break;
  } // switch

  return result;
}

std::ostream& operator << (std::ostream& os, const msrSlashUseStemsKind& elt)
{
  os << msrSlashUseStemsKindAsString (elt);
  return os;
}

//______________________________________________________________________________
S_msrSlash msrSlash::create (
  const mfInputLineNumber& inputLineNumber,
  msrSlashTypeKind     slashTypeKind,
  msrUseDotsKind  useDotsKind,
  msrSlashUseStemsKind slashUseStemsKind)
{
  msrSlash* obj =
    new msrSlash (
      inputLineNumber,
      slashTypeKind,
      useDotsKind,
      slashUseStemsKind);
  assert (obj != nullptr);
  return obj;
}

msrSlash::msrSlash (
  const mfInputLineNumber& inputLineNumber,
  msrSlashTypeKind     slashTypeKind,
  msrUseDotsKind  useDotsKind,
  msrSlashUseStemsKind slashUseStemsKind)
    : msrElement (inputLineNumber)
{
  fSlashTypeKind = slashTypeKind;
  fUseDotsKind = useDotsKind;
  fSlashUseStemsKind = slashUseStemsKind;
}

msrSlash::~msrSlash ()
{}

void msrSlash::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrSlash::acceptIn ()";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msrSlash>*
    p =
      dynamic_cast<visitor<S_msrSlash>*> (v)) {
        S_msrSlash elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrSlash::visitStart ()";

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void msrSlash::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrSlash::acceptOut ()";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msrSlash>*
    p =
      dynamic_cast<visitor<S_msrSlash>*> (v)) {
        S_msrSlash elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrSlash::visitEnd ()";

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif
        p->visitEnd (elem);
  }
}

void msrSlash::browseData (basevisitor* v)
{}

std::string msrSlash::asString () const
{
  std::stringstream ss;

  ss <<
    "Slash" <<
    ", line " << fInputLineNumber <<
    ", slashTypeKind:" <<
    msrSlashTypeKindAsString (
      fSlashTypeKind) <<
    ", useDotsKind:" <<
    msrUseDotsKindAsString (
      fUseDotsKind) <<
    ", slashUseStemsKind:" <<
    msrSlashUseStemsKindAsString (
      fSlashUseStemsKind);

  return ss.str ();
}

void msrSlash::print (std::ostream& os) const
{
  os <<
    "[Slash" <<
    ", line " << fInputLineNumber <<
    std::endl;

  ++gIndenter;

  constexpr int fieldWidth = 18;

  // print the voice upLink
  os << std::left <<
    std::setw (fieldWidth) <<
    "slashTypeKind" << ": " <<
    msrSlashTypeKindAsString (
      fSlashTypeKind) <<
    std::endl <<
    std::setw (fieldWidth) <<
    "useDotsKind" << ": " <<
    msrUseDotsKindAsString (
      fUseDotsKind) <<
    std::endl <<
    std::setw (fieldWidth) <<
    "slashUseStemsKind" << ": " <<
    msrSlashUseStemsKindAsString (
      fSlashUseStemsKind) <<
    std::endl;

  --gIndenter;

  os << ']' << std::endl;
}

std::ostream& operator << (std::ostream& os, const S_msrSlash& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NULL]" << std::endl;
  }

  return os;
}


}
