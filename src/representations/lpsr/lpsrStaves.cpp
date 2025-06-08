/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include "mfPreprocessorSettings.h"

#include "visitor.h"

#include "mfAssert.h"
#include "mfStringsHandling.h"

#include "lpsrStaves.h"

#include "lpsrVoices.h"
#include "lpsrLyrics.h"

#include "oahOah.h"

#include "lpsrOah.h"

#include "msrBrowsers.h"

#include "waeHandlers.h"


namespace MusicFormats
{

//______________________________________________________________________________
S_lpsrNewStaffGroupBlock lpsrNewStaffGroupBlock::create (
  const mfInputLineNumber& inputLineNumber)
{
  lpsrNewStaffGroupBlock* obj =
    new lpsrNewStaffGroupBlock (
      inputLineNumber);
  assert (obj != nullptr);
  return obj;
}

lpsrNewStaffGroupBlock::lpsrNewStaffGroupBlock (
  const mfInputLineNumber& inputLineNumber)
    : lpsrElement (inputLineNumber)
{}

lpsrNewStaffGroupBlock::~lpsrNewStaffGroupBlock ()
{}

void lpsrNewStaffGroupBlock::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gLpsrOahGroup->getTraceLpsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> lpsrNewStaffGroupBlock::acceptIn ()";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_lpsrNewStaffGroupBlock>*
    p =
      dynamic_cast<visitor<S_lpsrNewStaffGroupBlock>*> (v)) {
        S_lpsrNewStaffGroupBlock elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gLpsrOahGroup->getTraceLpsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching lpsrNewStaffGroupBlock::visitStart ()";

          gWaeHandler->waeTrace (
            __FILE__, mfInputLineNumber (__LINE__),
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void lpsrNewStaffGroupBlock::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gLpsrOahGroup->getTraceLpsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> lpsrNewStaffGroupBlock::acceptOut ()";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_lpsrNewStaffGroupBlock>*
    p =
      dynamic_cast<visitor<S_lpsrNewStaffGroupBlock>*> (v)) {
        S_lpsrNewStaffGroupBlock elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gLpsrOahGroup->getTraceLpsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching lpsrNewStaffGroupBlock::visitEnd ()";

          gWaeHandler->waeTrace (
            __FILE__, mfInputLineNumber (__LINE__),
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void lpsrNewStaffGroupBlock::browseData (basevisitor* v)
{}

void lpsrNewStaffGroupBlock::print (std::ostream& os) const
{
  os << "NewStaffGroupBlock" << std::endl;

  ++gIndenter;

  int size = fNewStaffGroupElementsVector.size ();

  for (int i = 0; i < size; ++i ) {
    os << fNewStaffGroupElementsVector[i];
  } // for

  --gIndenter;
}

std::ostream& operator << (std::ostream& os, const S_lpsrNewStaffGroupBlock& nstf)
{
  nstf->print (os);
  return os;
}

//______________________________________________________________________________
S_lpsrNewStaffTuningBlock lpsrNewStaffTuningBlock::create (
  const mfInputLineNumber& inputLineNumber,
  const S_msrStaffTuning& staffTuning)
{
  lpsrNewStaffTuningBlock* obj =
    new lpsrNewStaffTuningBlock (
      inputLineNumber, staffTuning);
  assert (obj != nullptr);
  return obj;
}

lpsrNewStaffTuningBlock::lpsrNewStaffTuningBlock (
  const mfInputLineNumber& inputLineNumber,
  const S_msrStaffTuning& staffTuning)
    : lpsrElement (inputLineNumber)
{
  fStaffTuning = staffTuning;
}

lpsrNewStaffTuningBlock::~lpsrNewStaffTuningBlock ()
{}

void lpsrNewStaffTuningBlock::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gLpsrOahGroup->getTraceLpsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> lpsrNewStaffTuningBlock::acceptIn ()";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_lpsrNewStaffTuningBlock>*
    p =
      dynamic_cast<visitor<S_lpsrNewStaffTuningBlock>*> (v)) {
        S_lpsrNewStaffTuningBlock elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gLpsrOahGroup->getTraceLpsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching lpsrNewStaffTuningBlock::visitStart ()";

          gWaeHandler->waeTrace (
            __FILE__, mfInputLineNumber (__LINE__),
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void lpsrNewStaffTuningBlock::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gLpsrOahGroup->getTraceLpsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> lpsrNewStaffTuningBlock::acceptOut ()";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_lpsrNewStaffTuningBlock>*
    p =
      dynamic_cast<visitor<S_lpsrNewStaffTuningBlock>*> (v)) {
        S_lpsrNewStaffTuningBlock elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gLpsrOahGroup->getTraceLpsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching lpsrNewStaffTuningBlock::visitEnd ()";

          gWaeHandler->waeTrace (
            __FILE__, mfInputLineNumber (__LINE__),
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void lpsrNewStaffTuningBlock::browseData (basevisitor* v)
{}

void lpsrNewStaffTuningBlock::print (std::ostream& os) const
{
  os << "NewStaffTuningBlock" << std::endl;

  ++gIndenter;

  os <<
    fStaffTuning;

  --gIndenter;
}

std::ostream& operator << (std::ostream& os, const S_lpsrNewStaffTuningBlock& nstf)
{
  nstf->print (os);
  return os;
}

//______________________________________________________________________________
S_lpsrNewStaffBlock lpsrNewStaffBlock::create (
  const mfInputLineNumber& inputLineNumber)
{
  lpsrNewStaffBlock* obj =
    new lpsrNewStaffBlock (
      inputLineNumber);
  assert (obj != nullptr);
  return obj;
}

lpsrNewStaffBlock::lpsrNewStaffBlock (
  const mfInputLineNumber& inputLineNumber)
    : lpsrElement (inputLineNumber)
{}

lpsrNewStaffBlock::~lpsrNewStaffBlock ()
{}

void lpsrNewStaffBlock::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gLpsrOahGroup->getTraceLpsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> lpsrNewStaffBlock::acceptIn ()";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_lpsrNewStaffBlock>*
    p =
      dynamic_cast<visitor<S_lpsrNewStaffBlock>*> (v)) {
        S_lpsrNewStaffBlock elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gLpsrOahGroup->getTraceLpsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching lpsrNewStaffBlock::visitStart ()";

          gWaeHandler->waeTrace (
            __FILE__, mfInputLineNumber (__LINE__),
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void lpsrNewStaffBlock::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gLpsrOahGroup->getTraceLpsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> lpsrNewStaffBlock::acceptOut ()";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_lpsrNewStaffBlock>*
    p =
      dynamic_cast<visitor<S_lpsrNewStaffBlock>*> (v)) {
        S_lpsrNewStaffBlock elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gLpsrOahGroup->getTraceLpsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching lpsrNewStaffBlock::visitEnd ()";

          gWaeHandler->waeTrace (
            __FILE__, mfInputLineNumber (__LINE__),
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void lpsrNewStaffBlock::browseData (basevisitor* v)
{}

void lpsrNewStaffBlock::print (std::ostream& os) const
{
  os << "NewStaffBlock" << std::endl;

  ++gIndenter;

  int size = fNewStaffElementsVector.size ();

  for (int i = 0; i < size; ++i ) {
    os << fNewStaffElementsVector[i];
  } // for

  --gIndenter;
}

std::ostream& operator << (std::ostream& os, const S_lpsrNewStaffBlock& nstf)
{
  nstf->print (os);
  return os;
}

//______________________________________________________________________________
S_lpsrStaffBlock lpsrStaffBlock::create (
  const S_msrStaff& staff)
{
  lpsrStaffBlock* obj = new lpsrStaffBlock (
    staff);
  assert (obj != nullptr);
  return obj;
}

lpsrStaffBlock::lpsrStaffBlock (
  const S_msrStaff& staff)
    : lpsrElement (0) // JMI
{
#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, mfInputLineNumber (__LINE__),
    staff != nullptr,
    "staff is NULL");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  fStaff = staff;

  // set staff block instrument names
  fStaffBlockInstrumentName =
    fStaff->
      getStaffInstrumentName ();
  fStaffBlockShortInstrumentName =
    fStaff->
      getStaffInstrumentAbbreviation ();
}

lpsrStaffBlock::~lpsrStaffBlock ()
{}

void lpsrStaffBlock::appendVoiceUseToStaffBlock (const S_msrVoice& voice)
{
  S_lpsrUseVoiceCommand
    useVoiceCommand =
      lpsrUseVoiceCommand::create (
        fInputLineNumber,
        voice);

  fStaffBlockElements.push_back (useVoiceCommand);
}

void lpsrStaffBlock::appendLyricsUseToStaffBlock (const S_msrStanza& stanza)
{
  S_lpsrNewLyricsBlock
    newLyricsCommand =
      lpsrNewLyricsBlock::create (
        fInputLineNumber,
        stanza,
        stanza->getStanzaUpLinkToVoice ());

  fStaffBlockElements.push_back (newLyricsCommand);
}

void lpsrStaffBlock::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gLpsrOahGroup->getTraceLpsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> lpsrStaffBlock::acceptIn ()";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_lpsrStaffBlock>*
    p =
      dynamic_cast<visitor<S_lpsrStaffBlock>*> (v)) {
        S_lpsrStaffBlock elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gLpsrOahGroup->getTraceLpsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching lpsrStaffBlock::visitStart ()";

          gWaeHandler->waeTrace (
            __FILE__, mfInputLineNumber (__LINE__),
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void lpsrStaffBlock::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gLpsrOahGroup->getTraceLpsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> lpsrStaffBlock::acceptOut ()";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_lpsrStaffBlock>*
    p =
      dynamic_cast<visitor<S_lpsrStaffBlock>*> (v)) {
        S_lpsrStaffBlock elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gLpsrOahGroup->getTraceLpsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching lpsrStaffBlock::visitEnd ()";

          gWaeHandler->waeTrace (
            __FILE__, mfInputLineNumber (__LINE__),
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void lpsrStaffBlock::browseData (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gLpsrOahGroup->getTraceLpsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> lpsrStaffBlock::browseData ()";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  for (
    std::list <S_msrElement>::const_iterator i = fStaffBlockElements.begin ();
    i != fStaffBlockElements.end ();
    ++i
  ) {
    // browse the element
    msrBrowser<msrElement> browser (v);
    browser.browse (*(*i));
  } // for

#ifdef MF_TRACE_IS_ENABLED
  if (gLpsrOahGroup->getTraceLpsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% <== lpsrStaffBlock::browseData ()";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

void lpsrStaffBlock::print (std::ostream& os) const
{
  os <<
    "StaffBlock for staff \"" <<
    fStaff->getStaffPathLikeName () <<
    "\" (" << msrStaffKindAsString (fStaff->getStaffKind ()) <<
    "), " <<
    mfSingularOrPlural (
      fStaffBlockElements.size (), "element", "elements") <<
    std::endl;

  ++gIndenter;

  constexpr int fieldWidth = 31;

  os << std::left <<
    std::setw (fieldWidth) <<
    "(StaffBlockInstrumentName" << " = \"" << fStaffBlockInstrumentName <<
    "\")" <<
    std::endl <<
    std::setw (fieldWidth) <<
    "(StaffBlockShortInstrumentName" << " = \"" << fStaffBlockShortInstrumentName <<
    "\")" <<
    std::endl << std::endl;

  if (fStaffBlockElements.size ()) {
    std::list <S_msrElement>::const_iterator
      iBegin = fStaffBlockElements.begin (),
      iEnd   = fStaffBlockElements.end (),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
 // JMI     os << std::endl;
    } // for
  }

  --gIndenter;
}

std::ostream& operator << (std::ostream& os, const S_lpsrStaffBlock& scr)
{
  scr->print (os);
  return os;
}


}
