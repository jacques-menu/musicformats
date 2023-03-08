/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include "visitor.h"

#include "mfStringsHandling.h"

#include "lpsrVoices.h"

#include "oahOah.h"

#include "lpsrOah.h"

#include "waeHandlers.h"


namespace MusicFormats
{

//______________________________________________________________________________
S_lpsrUseVoiceCommand lpsrUseVoiceCommand::create (
  int        inputLineNumber,
  const S_msrVoice& voice)
{
  lpsrUseVoiceCommand* obj =
    new lpsrUseVoiceCommand (
      inputLineNumber,
      voice);
  assert (obj != nullptr);
  return obj;
}

lpsrUseVoiceCommand::lpsrUseVoiceCommand (
  int        inputLineNumber,
  const S_msrVoice& voice)
    : lpsrElement (inputLineNumber)
{
  fVoice  = voice;
}

lpsrUseVoiceCommand::~lpsrUseVoiceCommand ()
{}

void lpsrUseVoiceCommand::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gLpsrOahGroup->getTraceLpsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> lpsrUseVoiceCommand::acceptIn ()" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_lpsrUseVoiceCommand>*
    p =
      dynamic_cast<visitor<S_lpsrUseVoiceCommand>*> (v)) {
        S_lpsrUseVoiceCommand elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gLpsrOahGroup->getTraceLpsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching lpsrUseVoiceCommand::visitStart ()" <<
            std::endl;

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void lpsrUseVoiceCommand::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gLpsrOahGroup->getTraceLpsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> lpsrUseVoiceCommand::acceptOut ()" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_lpsrUseVoiceCommand>*
    p =
      dynamic_cast<visitor<S_lpsrUseVoiceCommand>*> (v)) {
        S_lpsrUseVoiceCommand elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gLpsrOahGroup->getTraceLpsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching lpsrUseVoiceCommand::visitEnd ()" <<
            std::endl;

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void lpsrUseVoiceCommand::browseData (basevisitor* v)
{}

void lpsrUseVoiceCommand::print (std::ostream& os) const
{
  os <<
    "UseVoiceCommand" << " \"" <<
    fVoice->getVoiceName () <<
    "\", " <<
    mfSingularOrPlural (
      fVoice->getVoiceStanzasMap ().size (), "stanza", "stanzas") <<
    std::endl;
}

std::ostream& operator << (std::ostream& os, const S_lpsrUseVoiceCommand& nlc)
{
  nlc->print (os);
  return os;
}


}
