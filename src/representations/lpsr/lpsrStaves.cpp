/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include "mfStaticSettings.h"

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
S_lpsrNewStaffgroupBlock lpsrNewStaffgroupBlock::create (
  int inputLineNumber)
{
  lpsrNewStaffgroupBlock* o =
    new lpsrNewStaffgroupBlock (
      inputLineNumber);
  assert (o != nullptr);
  return o;
}

lpsrNewStaffgroupBlock::lpsrNewStaffgroupBlock (
  int inputLineNumber)
    : lpsrElement (inputLineNumber)
{}

lpsrNewStaffgroupBlock::~lpsrNewStaffgroupBlock ()
{}

void lpsrNewStaffgroupBlock::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalLpsrOahGroup->getTraceLpsrVisitors ()) {
		std::stringstream ss;

    ss <<
      "% ==> lpsrNewStaffgroupBlock::acceptIn ()" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif

  if (visitor<S_lpsrNewStaffgroupBlock>*
    p =
      dynamic_cast<visitor<S_lpsrNewStaffgroupBlock>*> (v)) {
        S_lpsrNewStaffgroupBlock elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gGlobalLpsrOahGroup->getTraceLpsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching lpsrNewStaffgroupBlock::visitStart ()" <<
            std::endl;

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif
        p->visitStart (elem);
  }
}

void lpsrNewStaffgroupBlock::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalLpsrOahGroup->getTraceLpsrVisitors ()) {
		std::stringstream ss;

    ss <<
      "% ==> lpsrNewStaffgroupBlock::acceptOut ()" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif

  if (visitor<S_lpsrNewStaffgroupBlock>*
    p =
      dynamic_cast<visitor<S_lpsrNewStaffgroupBlock>*> (v)) {
        S_lpsrNewStaffgroupBlock elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gGlobalLpsrOahGroup->getTraceLpsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching lpsrNewStaffgroupBlock::visitEnd ()" <<
            std::endl;

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif
        p->visitEnd (elem);
  }
}

void lpsrNewStaffgroupBlock::browseData (basevisitor* v)
{}

void lpsrNewStaffgroupBlock::print (std::ostream& os) const
{
  os << "NewStaffgroupBlock" << std::endl;

  ++gIndenter;

  int size = fNewStaffgroupElements.size ();

  for (int i = 0; i < size; ++i ) {
    os << fNewStaffgroupElements[i];
  } // for

  --gIndenter;
}

std::ostream& operator << (std::ostream& os, const S_lpsrNewStaffgroupBlock& nstf)
{
  nstf->print (os);
  return os;
}

//______________________________________________________________________________
S_lpsrNewStaffTuningBlock lpsrNewStaffTuningBlock::create (
  int                     inputLineNumber,
  const S_msrStaffTuning& staffTuning)
{
  lpsrNewStaffTuningBlock* o =
    new lpsrNewStaffTuningBlock (
      inputLineNumber, staffTuning);
  assert (o != nullptr);
  return o;
}

lpsrNewStaffTuningBlock::lpsrNewStaffTuningBlock (
  int                     inputLineNumber,
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
  if (gGlobalLpsrOahGroup->getTraceLpsrVisitors ()) {
		std::stringstream ss;

    ss <<
      "% ==> lpsrNewStaffTuningBlock::acceptIn ()" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif

  if (visitor<S_lpsrNewStaffTuningBlock>*
    p =
      dynamic_cast<visitor<S_lpsrNewStaffTuningBlock>*> (v)) {
        S_lpsrNewStaffTuningBlock elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gGlobalLpsrOahGroup->getTraceLpsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching lpsrNewStaffTuningBlock::visitStart ()" <<
            std::endl;

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif
        p->visitStart (elem);
  }
}

void lpsrNewStaffTuningBlock::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalLpsrOahGroup->getTraceLpsrVisitors ()) {
		std::stringstream ss;

    ss <<
      "% ==> lpsrNewStaffTuningBlock::acceptOut ()" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif

  if (visitor<S_lpsrNewStaffTuningBlock>*
    p =
      dynamic_cast<visitor<S_lpsrNewStaffTuningBlock>*> (v)) {
        S_lpsrNewStaffTuningBlock elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gGlobalLpsrOahGroup->getTraceLpsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching lpsrNewStaffTuningBlock::visitEnd ()" <<
            std::endl;

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif
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
  int inputLineNumber)
{
  lpsrNewStaffBlock* o =
    new lpsrNewStaffBlock (
      inputLineNumber);
  assert (o != nullptr);
  return o;
}

lpsrNewStaffBlock::lpsrNewStaffBlock (
  int inputLineNumber)
    : lpsrElement (inputLineNumber)
{}

lpsrNewStaffBlock::~lpsrNewStaffBlock ()
{}

void lpsrNewStaffBlock::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalLpsrOahGroup->getTraceLpsrVisitors ()) {
		std::stringstream ss;

    ss <<
      "% ==> lpsrNewStaffBlock::acceptIn ()" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif

  if (visitor<S_lpsrNewStaffBlock>*
    p =
      dynamic_cast<visitor<S_lpsrNewStaffBlock>*> (v)) {
        S_lpsrNewStaffBlock elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gGlobalLpsrOahGroup->getTraceLpsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching lpsrNewStaffBlock::visitStart ()" <<
            std::endl;

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif
        p->visitStart (elem);
  }
}

void lpsrNewStaffBlock::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalLpsrOahGroup->getTraceLpsrVisitors ()) {
		std::stringstream ss;

    ss <<
      "% ==> lpsrNewStaffBlock::acceptOut ()" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif

  if (visitor<S_lpsrNewStaffBlock>*
    p =
      dynamic_cast<visitor<S_lpsrNewStaffBlock>*> (v)) {
        S_lpsrNewStaffBlock elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gGlobalLpsrOahGroup->getTraceLpsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching lpsrNewStaffBlock::visitEnd ()" <<
            std::endl;

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif
        p->visitEnd (elem);
  }
}

void lpsrNewStaffBlock::browseData (basevisitor* v)
{}

void lpsrNewStaffBlock::print (std::ostream& os) const
{
  os << "NewStaffBlock" << std::endl;

  ++gIndenter;

  int size = fNewStaffElements.size ();

  for (int i = 0; i < size; ++i ) {
    os << fNewStaffElements[i];
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
  lpsrStaffBlock* o = new lpsrStaffBlock (
    staff);
  assert (o != nullptr);
  return o;
}

lpsrStaffBlock::lpsrStaffBlock (
  const S_msrStaff& staff)
    : lpsrElement (0) // JMI
{
#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    staff != nullptr,
    "staff is null");
#endif

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
  if (gGlobalLpsrOahGroup->getTraceLpsrVisitors ()) {
		std::stringstream ss;

    ss <<
      "% ==> lpsrStaffBlock::acceptIn ()" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif

  if (visitor<S_lpsrStaffBlock>*
    p =
      dynamic_cast<visitor<S_lpsrStaffBlock>*> (v)) {
        S_lpsrStaffBlock elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gGlobalLpsrOahGroup->getTraceLpsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching lpsrStaffBlock::visitStart ()" <<
            std::endl;

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif
        p->visitStart (elem);
  }
}

void lpsrStaffBlock::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalLpsrOahGroup->getTraceLpsrVisitors ()) {
		std::stringstream ss;

    ss <<
      "% ==> lpsrStaffBlock::acceptOut ()" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif

  if (visitor<S_lpsrStaffBlock>*
    p =
      dynamic_cast<visitor<S_lpsrStaffBlock>*> (v)) {
        S_lpsrStaffBlock elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gGlobalLpsrOahGroup->getTraceLpsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching lpsrStaffBlock::visitEnd ()" <<
            std::endl;

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif
        p->visitEnd (elem);
  }
}

void lpsrStaffBlock::browseData (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalLpsrOahGroup->getTraceLpsrVisitors ()) {
		std::stringstream ss;

    ss <<
      "% ==> lpsrStaffBlock::browseData ()" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif

  for (
    std::list<S_msrElement>::const_iterator i = fStaffBlockElements.begin ();
    i != fStaffBlockElements.end ();
    ++i
  ) {
    // browse the element
    msrBrowser<msrElement> browser (v);
    browser.browse (*(*i));
  } // for

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalLpsrOahGroup->getTraceLpsrVisitors ()) {
		std::stringstream ss;

    ss <<
      "% <== lpsrStaffBlock::browseData ()" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif
}

void lpsrStaffBlock::print (std::ostream& os) const
{
  os <<
    "StaffBlock for staff \"" <<
    fStaff->getStaffName () <<
    "\" (" << msrStaffKindAsString (fStaff->getStaffKind ()) <<
    "), " <<
    mfSingularOrPlural (
      fStaffBlockElements.size (), "element", "elements") <<
    std::endl;

  ++gIndenter;

  const int fieldWidth = 31;

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
    std::list<S_msrElement>::const_iterator
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
