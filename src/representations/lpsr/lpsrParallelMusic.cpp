/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include "mfStringsHandling.h"

#include "lpsrParallelMusic.h"

#include "lpsrComments.h"

#include "lpsrVariables.h"

#include "lpsrBarNumbers.h"

#include "lpsrVoices.h"
#include "lpsrLyrics.h"

#include "lpsrStaves.h"

#include "lpsrContexts.h"

#include "oahOah.h"

#include "lpsrOah.h"

#include "msrBrowsers.h"

#include "waeHandlers.h"


namespace MusicFormats
{

//______________________________________________________________________________
std::string lpsrElementsSeparatorKindAsString (
  lpsrElementsSeparatorKind elementsSeparatorKind)
{
  std::string result;

  switch (elementsSeparatorKind) {
    case lpsrElementsSeparatorKind::kElementsSeparatorEndOfLine:
      result = "kElementsSeparatorEndOfLine";
      break;
    case lpsrElementsSeparatorKind::kElementsSeparatorSpace:
      result = "kElementsSeparatorSpace";
      break;
  } // switch

  return result;
}

std::ostream& operator << (std::ostream& os, const lpsrElementsSeparatorKind& elt)
{
  os << lpsrElementsSeparatorKindAsString (elt);
  return os;
}

//______________________________________________________________________________
S_lpsrParallelMusicBLock lpsrParallelMusicBLock::create (
  const mfInputLineNumber& inputLineNumber,
  lpsrElementsSeparatorKind elementsSeparatorKind)
{
  lpsrParallelMusicBLock* obj =
    new lpsrParallelMusicBLock (
      inputLineNumber,
      elementsSeparatorKind);
  assert (obj != nullptr);
  return obj;
}

lpsrParallelMusicBLock::lpsrParallelMusicBLock (
  const mfInputLineNumber& inputLineNumber,
  lpsrElementsSeparatorKind elementsSeparatorKind)
    : lpsrElement (inputLineNumber)
{
  fElementsSeparatorKind = elementsSeparatorKind;
}

lpsrParallelMusicBLock::~lpsrParallelMusicBLock ()
{}

void lpsrParallelMusicBLock::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gLpsrOahGroup->getTraceLpsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> lpsrParallelMusicBLock::acceptIn ()";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_lpsrParallelMusicBLock>*
    p =
      dynamic_cast<visitor<S_lpsrParallelMusicBLock>*> (v)) {
        S_lpsrParallelMusicBLock elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gLpsrOahGroup->getTraceLpsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching lpsrParallelMusicBLock::visitStart ()";

          gWaeHandler->waeTrace (
            __FILE__, mfInputLineNumber (__LINE__),
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void lpsrParallelMusicBLock::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gLpsrOahGroup->getTraceLpsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> lpsrParallelMusicBLock::acceptOut ()";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_lpsrParallelMusicBLock>*
    p =
      dynamic_cast<visitor<S_lpsrParallelMusicBLock>*> (v)) {
        S_lpsrParallelMusicBLock elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gLpsrOahGroup->getTraceLpsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching lpsrParallelMusicBLock::visitEnd ()";

          gWaeHandler->waeTrace (
            __FILE__, mfInputLineNumber (__LINE__),
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void lpsrParallelMusicBLock::browseData (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gLpsrOahGroup->getTraceLpsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> lpsrParallelMusicBLock::browseData ()";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  for (
    std::list <S_lpsrPartGroupBlock>::const_iterator i =
      fParallelMusicBLockPartGroupBlocks.begin ();
    i != fParallelMusicBLockPartGroupBlocks.end ();
    ++i
  ) {
    // browse the element browser
    msrBrowser<msrElement> browser (v);
    browser.browse (*(*i));
  } // for

#ifdef MF_TRACE_IS_ENABLED
  if (gLpsrOahGroup->getTraceLpsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% <== lpsrParallelMusicBLock::browseData ()";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

void lpsrParallelMusicBLock::print (std::ostream& os) const
{
  os <<
    "ParallelMusicBLock" <<
    ", " <<
    mfSingularOrPlural (
      fParallelMusicBLockPartGroupBlocks.size (), "part group", "part groups") <<
    std::endl;

  ++gIndenter;

  for (
    std::list <S_lpsrPartGroupBlock>::const_iterator i =
      fParallelMusicBLockPartGroupBlocks.begin ();
    i != fParallelMusicBLockPartGroupBlocks.end ();
    ++i
  ) {
    os << (*i);
  } // for

  --gIndenter;
}

std::ostream& operator << (std::ostream& os, const S_lpsrParallelMusicBLock& elt)
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
