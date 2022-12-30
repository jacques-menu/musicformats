/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include "visitor.h"

#include "mfStringsHandling.h"

#include "lpsrVoices.h"

#include "oahOah.h"

#include "lpsrOah.h"


namespace MusicFormats
{

//______________________________________________________________________________
S_lpsrUseVoiceCommand lpsrUseVoiceCommand::create (
  int        inputLineNumber,
  const S_msrVoice& voice)
{
  lpsrUseVoiceCommand* o =
    new lpsrUseVoiceCommand (
      inputLineNumber,
      voice);
  assert (o != nullptr);
  return o;
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
#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalLpsrOahGroup->getTraceLpsrVisitors ()) {
    gLogStream <<
      "% ==> lpsrUseVoiceCommand::acceptIn ()" <<
      std::endl;
  }
#endif

  if (visitor<S_lpsrUseVoiceCommand>*
    p =
      dynamic_cast<visitor<S_lpsrUseVoiceCommand>*> (v)) {
        S_lpsrUseVoiceCommand elem = this;

#ifdef OAH_TRACING_IS_ENABLED
        if (gGlobalLpsrOahGroup->getTraceLpsrVisitors ()) {
          gLogStream <<
            "% ==> Launching lpsrUseVoiceCommand::visitStart ()" <<
            std::endl;
        }
#endif
        p->visitStart (elem);
  }
}

void lpsrUseVoiceCommand::acceptOut (basevisitor* v)
{
#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalLpsrOahGroup->getTraceLpsrVisitors ()) {
    gLogStream <<
      "% ==> lpsrUseVoiceCommand::acceptOut ()" <<
      std::endl;
  }
#endif

  if (visitor<S_lpsrUseVoiceCommand>*
    p =
      dynamic_cast<visitor<S_lpsrUseVoiceCommand>*> (v)) {
        S_lpsrUseVoiceCommand elem = this;

#ifdef OAH_TRACING_IS_ENABLED
        if (gGlobalLpsrOahGroup->getTraceLpsrVisitors ()) {
          gLogStream <<
            "% ==> Launching lpsrUseVoiceCommand::visitEnd ()" <<
            std::endl;
        }
#endif
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
