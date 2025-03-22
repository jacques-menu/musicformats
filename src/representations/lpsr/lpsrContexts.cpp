/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include "lpsrContexts.h"

#include "oahOah.h"

#include "lpsrOah.h"

#include "waeHandlers.h"


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
  int                        inputLineNumber,
  lpsrContextUseExistingKind contextUseExistingKind,
  lpsrContextTypeKind        contextTypeKind,
  const std::string&         contextPathLikeName)
{
  lpsrContext* obj =
    new lpsrContext (
      inputLineNumber,
      contextUseExistingKind,
      contextTypeKind,
      contextPathLikeName);
  assert (obj != nullptr);
  return obj;
}

lpsrContext::lpsrContext (
  int                        inputLineNumber,
  lpsrContextUseExistingKind contextUseExistingKind,
  lpsrContextTypeKind        contextTypeKind,
  const std::string&         contextPathLikeName)
    : lpsrElement (inputLineNumber)
{
  fContextUseExistingKind = contextUseExistingKind;
  fContextTypeKind = contextTypeKind;
  fContextPathLikeName = contextPathLikeName;
}

lpsrContext::~lpsrContext ()
{}

void lpsrContext::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gLpsrOahGroup->getTraceLpsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> lpsrContext::acceptIn ()";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_lpsrContext>*
    p =
      dynamic_cast<visitor<S_lpsrContext>*> (v)) {
        S_lpsrContext elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gLpsrOahGroup->getTraceLpsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching lpsrContext::visitStart ()";

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void lpsrContext::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gLpsrOahGroup->getTraceLpsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> lpsrContext::acceptOut ()";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_lpsrContext>*
    p =
      dynamic_cast<visitor<S_lpsrContext>*> (v)) {
        S_lpsrContext elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gLpsrOahGroup->getTraceLpsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching lpsrContext::visitEnd ()";

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
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

  if (fContextPathLikeName.size ()) {
    os << ' ' << fContextPathLikeName;
  }
  os << std::endl;

  ++gIndenter;

  os <<
    "fContextElementsList";

  if (fContextElementsList.size ()) {
    os << std::endl;

    ++gIndenter;

    std::list <S_msrElement>::const_iterator
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
    os << "[EMPTY]" <<
    std::endl;
  }

  --gIndenter;
}

//______________________________________________________________________________
S_lpsrChordNamesContext lpsrChordNamesContext::create (
  int                        inputLineNumber,
  lpsrContextUseExistingKind contextUseExistingKind,
  const std::string&         contextPathLikeName,
  const S_msrVoice&          contextVoice)
{
  lpsrChordNamesContext* obj =
    new lpsrChordNamesContext (
      inputLineNumber,
      contextUseExistingKind,
      contextPathLikeName,
      contextVoice);
  assert (obj != nullptr);
  return obj;
}

lpsrChordNamesContext::lpsrChordNamesContext (
  int                        inputLineNumber,
  lpsrContextUseExistingKind contextUseExistingKind,
  const std::string&         contextPathLikeName,
  const S_msrVoice&          contextVoice)
    : lpsrContext (
      inputLineNumber,
      contextUseExistingKind,
      lpsrContextTypeKind::kContextChordNames,
      contextPathLikeName)
{
  fContextUseExistingKind = contextUseExistingKind;

  fContextPathLikeName = contextPathLikeName;

  fContextVoice = contextVoice;
}

lpsrChordNamesContext::~lpsrChordNamesContext ()
{}

void lpsrChordNamesContext::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gLpsrOahGroup->getTraceLpsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> lpsrChordNamesContext::acceptIn ()";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_lpsrChordNamesContext>*
    p =
      dynamic_cast<visitor<S_lpsrChordNamesContext>*> (v)) {
        S_lpsrChordNamesContext elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gLpsrOahGroup->getTraceLpsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching lpsrChordNamesContext::visitStart ()";

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void lpsrChordNamesContext::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gLpsrOahGroup->getTraceLpsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> lpsrChordNamesContext::acceptOut ()";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_lpsrChordNamesContext>*
    p =
      dynamic_cast<visitor<S_lpsrChordNamesContext>*> (v)) {
        S_lpsrChordNamesContext elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gLpsrOahGroup->getTraceLpsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching lpsrChordNamesContext::visitEnd ()";

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
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

  constexpr int fieldWidth = 20;

  os << std::left <<
    std::setw (fieldWidth) <<
    "contextUseExistingKind" << ": " <<
    lpsrContextUseExistingKindAsString (
      fContextUseExistingKind) <<
    std::endl <<
    std::setw (fieldWidth) <<
    "contextPathLikeName" << ": \"" << fContextPathLikeName << "\"" <<
    std::endl <<
    std::setw (fieldWidth) <<
    "contextVoice" << ": \"" << fContextVoice->getVoiceName () << "\"" <<
    std::endl;

  os <<
    std::setw (fieldWidth) <<
    "ContextElements";

  if (fContextElementsList.size ()) {
    os << std::endl;

    ++gIndenter;

    std::list <S_msrElement>::const_iterator
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

std::ostream& operator << (std::ostream& os, const S_lpsrChordNamesContext& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NULL]" << std::endl;
  }

  return os;
}

//______________________________________________________________________________
S_lpsrFiguredBassContext lpsrFiguredBassContext::create (
  int                        inputLineNumber,
  lpsrContextUseExistingKind contextUseExistingKind,
  const std::string&         contextPathLikeName,
  const S_msrStaff&          contextStaff)
{
  lpsrFiguredBassContext* obj =
    new lpsrFiguredBassContext (
      inputLineNumber,
      contextUseExistingKind,
      contextPathLikeName,
      contextStaff);
  assert (obj != nullptr);
  return obj;
}

lpsrFiguredBassContext::lpsrFiguredBassContext (
  int                        inputLineNumber,
  lpsrContextUseExistingKind contextUseExistingKind,
  const std::string&         contextPathLikeName,
  const S_msrStaff&          contextStaff)
    : lpsrContext (
      inputLineNumber,
      contextUseExistingKind,
      lpsrContextTypeKind::kContextFiguredBass,
      contextPathLikeName)
{
  fContextUseExistingKind = contextUseExistingKind;

  fContextPathLikeName = contextPathLikeName;

  fContextStaff = contextStaff;
}

lpsrFiguredBassContext::~lpsrFiguredBassContext ()
{}

void lpsrFiguredBassContext::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gLpsrOahGroup->getTraceLpsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> lpsrFiguredBassContext::acceptIn ()";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_lpsrFiguredBassContext>*
    p =
      dynamic_cast<visitor<S_lpsrFiguredBassContext>*> (v)) {
        S_lpsrFiguredBassContext elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gLpsrOahGroup->getTraceLpsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching lpsrFiguredBassContext::visitStart ()";

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void lpsrFiguredBassContext::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gLpsrOahGroup->getTraceLpsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> lpsrFiguredBassContext::acceptOut ()";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_lpsrFiguredBassContext>*
    p =
      dynamic_cast<visitor<S_lpsrFiguredBassContext>*> (v)) {
        S_lpsrFiguredBassContext elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gLpsrOahGroup->getTraceLpsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching lpsrFiguredBassContext::visitEnd ()";

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
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

  constexpr int fieldWidth = 20;

  os << std::left <<
    std::setw (fieldWidth) <<
    "fContextUseExistingKind" << ": " <<
    lpsrContextUseExistingKindAsString (
      fContextUseExistingKind) <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fContextPathLikeName" << ": \"" << fContextPathLikeName << "\"" <<
    std::endl <<
    std::setw (fieldWidth) <<
    "contextStaff" << ": \"" << fContextStaff->getStaffPathLikeName () << "\"" <<
    std::endl;

  os <<
    std::setw (fieldWidth) <<
    "fContextElementsList";

  if (fContextElementsList.size ()) {
    os << std::endl;

    ++gIndenter;

    std::list <S_msrElement>::const_iterator
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
    os << "[NULL]" << std::endl;
  }

  return os;
}


}
