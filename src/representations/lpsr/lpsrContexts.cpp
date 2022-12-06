/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include "lpsrContexts.h"

#include "oahOah.h"

#include "lpsrOah.h"


namespace MusicFormats
{

//______________________________________________________________________________
std::string lpsrContextTypeKindAsString (
  lpsrContextTypeKind contextTypeKind)
{
  std::string result;

  switch (contextTypeKind) {
    case lpsrContextTypeKind::kContextChordNames:
      result = "kContextChordNames";
      break;
    case lpsrContextTypeKind::kContextFiguredBass:
      result = "kContextFiguredBass";
      break;
  } // switch

  return result;
}

std::ostream& operator << (std::ostream& os, const lpsrContextTypeKind& elt)
{
  os << lpsrContextTypeKindAsString (elt);
  return os;
}

std::string lpsrContextUseExistingKindAsString (
  lpsrContextUseExistingKind contextUseExistingKind)
{
  std::string result;

  switch (contextUseExistingKind) {
    case lpsrContextUseExistingKind::kUseExistingContextYes:
      result = "kUseExistingContextYes";
      break;
    case lpsrContextUseExistingKind::kUseExistingContextNo:
      result = "kUseExistingContextNo";
      break;
  } // switch

  return result;
}

std::ostream& operator << (std::ostream& os, const lpsrContextUseExistingKind& elt)
{
  os << lpsrContextUseExistingKindAsString (elt);
  return os;
}

//______________________________________________________________________________
S_lpsrContext lpsrContext::create (
  int                     inputLineNumber,
  lpsrContextUseExistingKind contextUseExistingKind,
  lpsrContextTypeKind     contextTypeKind,
  const std::string&      contextName)
{
  lpsrContext* o =
    new lpsrContext (
      inputLineNumber,
      contextUseExistingKind,
      contextTypeKind,
      contextName);
  assert (o != nullptr);
  return o;
}

lpsrContext::lpsrContext (
  int                     inputLineNumber,
  lpsrContextUseExistingKind contextUseExistingKind,
  lpsrContextTypeKind     contextTypeKind,
  const std::string&      contextName)
    : lpsrElement (inputLineNumber)
{
  fContextUseExistingKind = contextUseExistingKind;
  fContextTypeKind = contextTypeKind;
  fContextName = contextName;
}

lpsrContext::~lpsrContext ()
{}

void lpsrContext::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalLpsrOahGroup->getTraceLpsrVisitors ()) {
    gLogStream <<
      "% ==> lpsrContext::acceptIn ()" <<
      std::endl;
  }
#endif

  if (visitor<S_lpsrContext>*
    p =
      dynamic_cast<visitor<S_lpsrContext>*> (v)) {
        S_lpsrContext elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalLpsrOahGroup->getTraceLpsrVisitors ()) {
          gLogStream <<
            "% ==> Launching lpsrContext::visitStart ()" <<
            std::endl;
        }
#endif
        p->visitStart (elem);
  }
}

void lpsrContext::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalLpsrOahGroup->getTraceLpsrVisitors ()) {
    gLogStream <<
      "% ==> lpsrContext::acceptOut ()" <<
      std::endl;
  }
#endif

  if (visitor<S_lpsrContext>*
    p =
      dynamic_cast<visitor<S_lpsrContext>*> (v)) {
        S_lpsrContext elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalLpsrOahGroup->getTraceLpsrVisitors ()) {
          gLogStream <<
            "% ==> Launching lpsrContext::visitEnd ()" <<
            std::endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void lpsrContext::browseData (basevisitor* v)
{}

void lpsrContext::print (std::ostream& os) const
{
  os <<
    "[Context" <<
    ", fContextTypeKind: " << fContextTypeKind <<
    ", fContextUseExistingKind: " << fContextUseExistingKind;

  if (fContextName.size ()) {
    os << ' ' << fContextName;
  }
  os << std::endl;

  ++gIndenter;

  os <<
    "fContextElementsList";

  if (fContextElementsList.size ()) {
    os << std::endl;

    ++gIndenter;

    std::list<S_msrElement>::const_iterator
      iBegin = fContextElementsList.begin (),
      iEnd   = fContextElementsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
 // JMI     os << std::endl;
    } // for

    --gIndenter;
  }
  else {
    os << "[NONE]" <<
    std::endl;
  }

  --gIndenter;
}

//______________________________________________________________________________
S_lpsrChordNamesContext lpsrChordNamesContext::create (
  int                     inputLineNumber,
  lpsrContextUseExistingKind contextUseExistingKind,
  const std::string&      contextName,
  const S_msrVoice&       contextVoice)
{
  lpsrChordNamesContext* o =
    new lpsrChordNamesContext (
      inputLineNumber,
      contextUseExistingKind,
      contextName,
      contextVoice);
  assert (o != nullptr);
  return o;
}

lpsrChordNamesContext::lpsrChordNamesContext (
  int                     inputLineNumber,
  lpsrContextUseExistingKind contextUseExistingKind,
  const std::string&      contextName,
  const S_msrVoice&       contextVoice)
    : lpsrContext (
      inputLineNumber,
      contextUseExistingKind,
      lpsrContextTypeKind::kContextChordNames,
      contextName)
{
  fContextUseExistingKind = contextUseExistingKind;

  fContextName = contextName;

  fContextVoice = contextVoice;
}

lpsrChordNamesContext::~lpsrChordNamesContext ()
{}

void lpsrChordNamesContext::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalLpsrOahGroup->getTraceLpsrVisitors ()) {
    gLogStream <<
      "% ==> lpsrChordNamesContext::acceptIn ()" <<
      std::endl;
  }
#endif

  if (visitor<S_lpsrChordNamesContext>*
    p =
      dynamic_cast<visitor<S_lpsrChordNamesContext>*> (v)) {
        S_lpsrChordNamesContext elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalLpsrOahGroup->getTraceLpsrVisitors ()) {
          gLogStream <<
            "% ==> Launching lpsrChordNamesContext::visitStart ()" <<
            std::endl;
        }
#endif
        p->visitStart (elem);
  }
}

void lpsrChordNamesContext::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalLpsrOahGroup->getTraceLpsrVisitors ()) {
    gLogStream <<
      "% ==> lpsrChordNamesContext::acceptOut ()" <<
      std::endl;
  }
#endif

  if (visitor<S_lpsrChordNamesContext>*
    p =
      dynamic_cast<visitor<S_lpsrChordNamesContext>*> (v)) {
        S_lpsrChordNamesContext elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalLpsrOahGroup->getTraceLpsrVisitors ()) {
          gLogStream <<
            "% ==> Launching lpsrChordNamesContext::visitEnd ()" <<
            std::endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void lpsrChordNamesContext::browseData (basevisitor* v)
{}

void lpsrChordNamesContext::print (std::ostream& os) const
{
  os <<
  /* JMI
    "ChordNamesContext, \"" <<
    fContextTypeKind <<
    "\"" <<
    */
    "ChordNamesContext" <<
    std::endl;

  ++gIndenter;

  const int fieldWidth = 20;

  os << std::left <<
    std::setw (fieldWidth) <<
    "contextUseExistingKind" << ": " <<
    lpsrContextUseExistingKindAsString (
      fContextUseExistingKind) <<
    std::endl <<
    std::setw (fieldWidth) <<
    "contextName" << " : \"" << fContextName << "\"" <<
    std::endl <<
    std::setw (fieldWidth) <<
    "contextVoice" << " : \"" << fContextVoice->getVoiceName () << "\"" <<
    std::endl;

  os <<
    std::setw (fieldWidth) <<
    "ContextElements";

  if (fContextElementsList.size ()) {
    os << std::endl;

    ++gIndenter;

    std::list<S_msrElement>::const_iterator
      iBegin = fContextElementsList.begin (),
      iEnd   = fContextElementsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
 // JMI     os << std::endl;
    } // for

    --gIndenter;
  }
  else {
    os <<
      ": " << "[NONE]" <<
    std::endl;
  }

  --gIndenter;
}

std::ostream& operator << (std::ostream& os, const S_lpsrChordNamesContext& elt)
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
S_lpsrFiguredBassContext lpsrFiguredBassContext::create (
  int                     inputLineNumber,
  lpsrContextUseExistingKind contextUseExistingKind,
  const std::string&      contextName,
  const S_msrStaff&       contextStaff)
{
  lpsrFiguredBassContext* o =
    new lpsrFiguredBassContext (
      inputLineNumber,
      contextUseExistingKind,
      contextName,
      contextStaff);
  assert (o != nullptr);
  return o;
}

lpsrFiguredBassContext::lpsrFiguredBassContext (
  int                     inputLineNumber,
  lpsrContextUseExistingKind contextUseExistingKind,
  const std::string&      contextName,
  const S_msrStaff&       contextStaff)
    : lpsrContext (
      inputLineNumber,
      contextUseExistingKind,
      lpsrContextTypeKind::kContextFiguredBass,
      contextName)
{
  fContextUseExistingKind = contextUseExistingKind;

  fContextName = contextName;

  fContextStaff = contextStaff;
}

lpsrFiguredBassContext::~lpsrFiguredBassContext ()
{}

void lpsrFiguredBassContext::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalLpsrOahGroup->getTraceLpsrVisitors ()) {
    gLogStream <<
      "% ==> lpsrFiguredBassContext::acceptIn ()" <<
      std::endl;
  }
#endif

  if (visitor<S_lpsrFiguredBassContext>*
    p =
      dynamic_cast<visitor<S_lpsrFiguredBassContext>*> (v)) {
        S_lpsrFiguredBassContext elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalLpsrOahGroup->getTraceLpsrVisitors ()) {
          gLogStream <<
            "% ==> Launching lpsrFiguredBassContext::visitStart ()" <<
            std::endl;
        }
#endif
        p->visitStart (elem);
  }
}

void lpsrFiguredBassContext::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalLpsrOahGroup->getTraceLpsrVisitors ()) {
    gLogStream <<
      "% ==> lpsrFiguredBassContext::acceptOut ()" <<
      std::endl;
  }
#endif

  if (visitor<S_lpsrFiguredBassContext>*
    p =
      dynamic_cast<visitor<S_lpsrFiguredBassContext>*> (v)) {
        S_lpsrFiguredBassContext elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalLpsrOahGroup->getTraceLpsrVisitors ()) {
          gLogStream <<
            "% ==> Launching lpsrFiguredBassContext::visitEnd ()" <<
            std::endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void lpsrFiguredBassContext::browseData (basevisitor* v)
{}

void lpsrFiguredBassContext::print (std::ostream& os) const
{
  os <<
  /* JMI
    "FiguredBassContext, \"" <<
    fContextTypeKind <<
    "\"" <<
    */
    "FiguredBassContext" <<
    std::endl;

  ++gIndenter;

  const int fieldWidth = 20;

  os << std::left <<
    std::setw (fieldWidth) <<
    "fContextUseExistingKind" << ": " <<
    lpsrContextUseExistingKindAsString (
      fContextUseExistingKind) <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fContextName" << " : \"" << fContextName << "\"" <<
    std::endl <<
    std::setw (fieldWidth) <<
    "contextStaff" << " : \"" << fContextStaff->getStaffName () << "\"" <<
    std::endl;

  os <<
    std::setw (fieldWidth) <<
    "fContextElementsList";

  if (fContextElementsList.size ()) {
    os << std::endl;

    ++gIndenter;

    std::list<S_msrElement>::const_iterator
      iBegin = fContextElementsList.begin (),
      iEnd   = fContextElementsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
 // JMI     os << std::endl;
    } // for

    --gIndenter;
  }
  else {
    os <<
      ": " << "[EMPTY]" <<
    std::endl;
  }

  --gIndenter;
}

std::ostream& operator << (std::ostream& os, const S_lpsrFiguredBassContext& elt)
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
