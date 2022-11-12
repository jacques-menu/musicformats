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

std::string lpsrContextExistingKindAsString (
  lpsrContextExistingKind contextExistingKind)
{
  std::string result;

  switch (contextExistingKind) {
    case lpsrContextExistingKind::kExistingContextYes:
      result = "kExistingContextYes";
      break;
    case lpsrContextExistingKind::kExistingContextNo:
      result = "kExistingContextNo";
      break;
  } // switch

  return result;
}

std::ostream& operator << (std::ostream& os, const lpsrContextExistingKind& elt)
{
  os << lpsrContextExistingKindAsString (elt);
  return os;
}

//______________________________________________________________________________
S_lpsrContext lpsrContext::create (
  int                     inputLineNumber,
  lpsrContextExistingKind contextExistingKind,
  lpsrContextTypeKind     contextTypeKind,
  const std::string&           contextName)
{
  lpsrContext* o =
    new lpsrContext (
      inputLineNumber,
      contextExistingKind,
      contextTypeKind,
      contextName);
  assert (o != nullptr);
  return o;
}

lpsrContext::lpsrContext (
  int                     inputLineNumber,
  lpsrContextExistingKind contextExistingKind,
  lpsrContextTypeKind     contextTypeKind,
  const std::string&           contextName)
    : lpsrElement (inputLineNumber)
{
  fContextExistingKind = contextExistingKind;
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
    ", fContextExistingKind: " << fContextExistingKind;

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
  lpsrContextExistingKind contextExistingKind,
  const std::string&           contextName,
  S_msrVoice              contextVoice)
{
  lpsrChordNamesContext* o =
    new lpsrChordNamesContext (
      inputLineNumber,
      contextExistingKind,
      contextName,
      contextVoice);
  assert (o != nullptr);
  return o;
}

lpsrChordNamesContext::lpsrChordNamesContext (
  int                     inputLineNumber,
  lpsrContextExistingKind contextExistingKind,
  const std::string&           contextName,
  S_msrVoice              contextVoice)
    : lpsrContext (
      inputLineNumber,
      contextExistingKind,
      lpsrContextTypeKind::kContextChordNames,
      contextName)
{
  fContextExistingKind = contextExistingKind;

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
    "contextExistingKind" << " : " <<
    lpsrContextExistingKindAsString (
      fContextExistingKind) <<
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
      " : " << "[NONE]" <<
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
  lpsrContextExistingKind contextExistingKind,
  const std::string&           contextName,
  S_msrStaff              contextStaff)
{
  lpsrFiguredBassContext* o =
    new lpsrFiguredBassContext (
      inputLineNumber,
      contextExistingKind,
      contextName,
      contextStaff);
  assert (o != nullptr);
  return o;
}

lpsrFiguredBassContext::lpsrFiguredBassContext (
  int                     inputLineNumber,
  lpsrContextExistingKind contextExistingKind,
  const std::string&           contextName,
  S_msrStaff              contextStaff)
    : lpsrContext (
      inputLineNumber,
      contextExistingKind,
      lpsrContextTypeKind::kContextFiguredBass,
      contextName)
{
  fContextExistingKind = contextExistingKind;

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
    "fContextExistingKind" << " : " <<
    lpsrContextExistingKindAsString (
      fContextExistingKind) <<
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
      " : " << "[EMPTY]" <<
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
