/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include "visitor.h"

#include "lpsrLyrics.h"

#include "oahOah.h"

#include "lpsrOah.h"


namespace MusicFormats
{

//______________________________________________________________________________
S_lpsrNewLyricsBlock lpsrNewLyricsBlock::create (
  int         inputLineNumber,
  S_msrStanza stanza,
  S_msrVoice  voice)
{
  lpsrNewLyricsBlock* o =
    new lpsrNewLyricsBlock (
      inputLineNumber,
      stanza, voice);
  assert (o != nullptr);
  return o;
}

lpsrNewLyricsBlock::lpsrNewLyricsBlock (
  int         inputLineNumber,
  S_msrStanza stanza,
  S_msrVoice  voice)
    : lpsrElement (inputLineNumber)
{
  fStanza = stanza;
  fVoice  = voice;
}

lpsrNewLyricsBlock::~lpsrNewLyricsBlock ()
{}

void lpsrNewLyricsBlock::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalLpsrOahGroup->getTraceLpsrVisitors ()) {
    gLogStream <<
      "% ==> lpsrNewLyricsBlock::acceptIn ()" <<
      std::endl;
  }
#endif

  if (visitor<S_lpsrNewLyricsBlock>*
    p =
      dynamic_cast<visitor<S_lpsrNewLyricsBlock>*> (v)) {
        S_lpsrNewLyricsBlock elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalLpsrOahGroup->getTraceLpsrVisitors ()) {
          gLogStream <<
            "% ==> Launching lpsrNewLyricsBlock::visitStart ()" <<
            std::endl;
        }
#endif
        p->visitStart (elem);
  }
}

void lpsrNewLyricsBlock::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalLpsrOahGroup->getTraceLpsrVisitors ()) {
    gLogStream <<
      "% ==> lpsrNewLyricsBlock::acceptOut ()" <<
      std::endl;
  }
#endif

  if (visitor<S_lpsrNewLyricsBlock>*
    p =
      dynamic_cast<visitor<S_lpsrNewLyricsBlock>*> (v)) {
        S_lpsrNewLyricsBlock elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalLpsrOahGroup->getTraceLpsrVisitors ()) {
          gLogStream <<
            "% ==> Launching lpsrNewLyricsBlock::visitEnd ()" <<
            std::endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void lpsrNewLyricsBlock::browseData (basevisitor* v)
{}

void lpsrNewLyricsBlock::print (std::ostream& os) const
{
  os <<
    "NewLyricsBlock" << ' ' <<
    fStanza->getStanzaName () << ' ' <<
    fVoice->getVoiceName () <<
    std::endl;
}

std::ostream& operator << (std::ostream& os, const S_lpsrNewLyricsBlock& nlc)
{
  nlc->print (os);
  return os;
}


}
