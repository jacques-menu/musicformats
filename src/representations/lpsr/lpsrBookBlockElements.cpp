/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include "visitor.h"

#include "lpsrBookBlockElements.h"


#include "oahEnableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "tracingOah.h"
#endif

#include "oahOah.h"

#include "lpsrOah.h"
#include "lpsr2lilypondOah.h"

#include "msrBrowsers.h"


namespace MusicFormats
{

//______________________________________________________________________________
S_lpsrBookBlockElement lpsrBookBlockElement::create (
  int inputLineNumber)
{
  lpsrBookBlockElement* o = new lpsrBookBlockElement (
    inputLineNumber);
  assert (o != nullptr);
  return o;
}

lpsrBookBlockElement::lpsrBookBlockElement (
  int inputLineNumber)
    : lpsrElement (inputLineNumber)
{
  // create the book block element parallel music
  fBookBlockElementParallelMusicBlock =
    lpsrParallelMusicBLock::create (
      inputLineNumber,
      lpsrElementsSeparatorKind::kElementsSeparatorEndOfLine);
}

lpsrBookBlockElement::~lpsrBookBlockElement ()
{}

void lpsrBookBlockElement::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalLpsrOahGroup->getTraceLpsrVisitors ()) {
    gLogStream <<
      "% ==> lpsrBookBlockElement::acceptIn ()" <<
      std::endl;
  }
#endif

  if (visitor<S_lpsrBookBlockElement>*
    p =
      dynamic_cast<visitor<S_lpsrBookBlockElement>*> (v)) {
        S_lpsrBookBlockElement elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalLpsrOahGroup->getTraceLpsrVisitors ()) {
          gLogStream <<
            "% ==> Launching lpsrBookBlockElement::visitStart ()" <<
            std::endl;
        }
#endif
        p->visitStart (elem);
  }
}

void lpsrBookBlockElement::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalLpsrOahGroup->getTraceLpsrVisitors ()) {
    gLogStream <<
      "% ==> lpsrBookBlockElement::acceptOut ()" <<
      std::endl;
  }
#endif

  if (visitor<S_lpsrBookBlockElement>*
    p =
      dynamic_cast<visitor<S_lpsrBookBlockElement>*> (v)) {
        S_lpsrBookBlockElement elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalLpsrOahGroup->getTraceLpsrVisitors ()) {
          gLogStream <<
            "% ==> Launching lpsrBookBlockElement::visitEnd ()" <<
            std::endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void lpsrBookBlockElement::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalLpsrOahGroup->getTraceLpsrVisitors ()) {
    gLogStream <<
      "% ==> lpsrBookBlockElement::browseData ()" <<
      std::endl;
  }
#endif

  {
    // browse the book block element parallel music
    msrBrowser<lpsrParallelMusicBLock> browser (v);
    browser.browse (*fBookBlockElementParallelMusicBlock);
  }

/* JMI
  for (
    std::vector<S_msrElement>::const_iterator i = fBlockElements.begin ();
    i != fBlockElements.end ();
    ++i
  ) {
    // browse the element
 //   msrBrowser<msrElement> browser (v);
 //   browser.browse (*(*i));
  } // for
*/

#ifdef TRACING_IS_ENABLED
  if (gGlobalLpsrOahGroup->getTraceLpsrVisitors ()) {
    gLogStream <<
      "% <== lpsrBookBlockElement::browseData ()" <<
      std::endl;
  }
#endif
}

void lpsrBookBlockElement::print (std::ostream& os) const
{
  // will never be called since it is overridden in sub-classes
}

std::ostream& operator << (std::ostream& os, const S_lpsrBookBlockElement& scr)
{
  scr->print (os);
  return os;
}

//______________________________________________________________________________
S_lpsrScoreBlock lpsrScoreBlock::create (
  int inputLineNumber)
{
  lpsrScoreBlock* o = new lpsrScoreBlock (
    inputLineNumber);
  assert (o != nullptr);
  return o;
}

lpsrScoreBlock::lpsrScoreBlock (
  int inputLineNumber)
    : lpsrBookBlockElement (inputLineNumber)
{
  // create the score block parallel music block
  fBookBlockElementParallelMusicBlock =
    lpsrParallelMusicBLock::create (
      inputLineNumber,
      lpsrElementsSeparatorKind::kElementsSeparatorEndOfLine);

  // create the score block layout?
  if (! gGlobalLpsr2lilypondOahGroup->getNoTopLevelLayoutBlock ()) {
    fScoreBlockLayout =
      lpsrLayout::create (
        inputLineNumber);
  }

  // create the score block midi tempo
  S_oahMidiTempoAtom
    midiTempoAtom =
      gGlobalLpsr2lilypondOahGroup ->
        getMidiTempoAtom ();

  std::string midiTempoDuration  = "16";
  int    midiTempoPerSecond = 360;

  if (midiTempoAtom->getSetByAnOption ()) {
    const msrMidiTempo&
      midiTempo =
       gGlobalLpsr2lilypondOahGroup->getMidiTempo ();

    midiTempoDuration  = midiTempo.getMidiTempoDuration ();
    midiTempoPerSecond = midiTempo.getMidiTempoPerSecond ();
  }
  else {
    // JMI use hard-wired value set above
  }

  fScoreBlockMidiTempo =
    msrMidiTempo::create (
      inputLineNumber,
      midiTempoDuration,
      midiTempoPerSecond);

  // create the score block midi tempo // JMI
//   fScoreBlockMidiTempo =
//     gGlobalLpsr2lilypondOahGroup->
//       getMidiTempo ().createMsrMidiTempoNewbornClone ();
}

lpsrScoreBlock::~lpsrScoreBlock ()
{}

void lpsrScoreBlock::appendPartGroupBlockToScoreBlock (
  const S_lpsrPartGroupBlock& partGroupBlock)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTracePartGroups ()) {
    gLogStream <<
      "Appending part group block " <<
       partGroupBlock-> getPartGroup ()-> getPartGroupCombinedName () <<
       " to LPSR score block" <<
       std::endl;
  }
#endif

  fBookBlockElementParallelMusicBlock->
    appendPartGroupBlockToParallelMusicBLock (
      partGroupBlock);

//               fBlockElements.push_back(partGroupBlock);
}

/* JMI
void lpsrScoreBlock::appendVoiceUseToParallelMusicBLock (
  S_lpsrUseVoiceCommand voiceUse)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceVoices ()) {
    gLogStream <<
      "Appending the use of voice \"" <<
       voiceUse-> getVoice ()-> getVoiceName () <<
       "\" to LPSR score" <<
       std::endl;
  }
#endif

  fBookBlockElementParallelMusicBlock->
    addElementToParallelMusicBLock (voiceUse);

//     JMI             fBlockElements.push_back(voiceUse);
}

void lpsrScoreBlock::appendLyricsUseToParallelMusicBLock (
  S_lpsrNewLyricsBlock lyricsUse)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceLyrics ()) {
    gLogStream <<
      "Appending the use of stanza " <<
       lyricsUse-> getStanza ()-> getStanzaName () <<
       " to LPSR score" <<
       std::endl;
  }
#endif

  fBookBlockElementParallelMusicBlock->
    addElementToParallelMusicBLock (lyricsUse);
}
*/

void lpsrScoreBlock::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalLpsrOahGroup->getTraceLpsrVisitors ()) {
    gLogStream <<
      "% ==> lpsrScoreBlock::acceptIn ()" <<
      std::endl;
  }
#endif

  if (visitor<S_lpsrScoreBlock>*
    p =
      dynamic_cast<visitor<S_lpsrScoreBlock>*> (v)) {
        S_lpsrScoreBlock elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalLpsrOahGroup->getTraceLpsrVisitors ()) {
          gLogStream <<
            "% ==> Launching lpsrScoreBlock::visitStart ()" <<
            std::endl;
        }
#endif
        p->visitStart (elem);
  }
}

void lpsrScoreBlock::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalLpsrOahGroup->getTraceLpsrVisitors ()) {
    gLogStream <<
      "% ==> lpsrScoreBlock::acceptOut ()" <<
      std::endl;
  }
#endif

  if (visitor<S_lpsrScoreBlock>*
    p =
      dynamic_cast<visitor<S_lpsrScoreBlock>*> (v)) {
        S_lpsrScoreBlock elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalLpsrOahGroup->getTraceLpsrVisitors ()) {
          gLogStream <<
            "% ==> Launching lpsrScoreBlock::visitEnd ()" <<
            std::endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void lpsrScoreBlock::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalLpsrOahGroup->getTraceLpsrVisitors ()) {
    gLogStream <<
      "% ==> lpsrScoreBlock::browseData ()" <<
      std::endl;
  }
#endif

  {
    // browse the score block parallel music
    msrBrowser<lpsrParallelMusicBLock> browser (v);
    browser.browse (*fBookBlockElementParallelMusicBlock);
  }

/* JMI
  for (
    std::vector<S_msrElement>::const_iterator i = fBlockElements.begin ();
    i != fBlockElements.end ();
    ++i
  ) {
    // browse the element
 //   msrBrowser<msrElement> browser (v);
 //   browser.browse (*(*i));
  } // for
*/

  if (fScoreBlockLayout) {
    // browse the score block layout
    msrBrowser<lpsrLayout> browser (v);
    browser.browse (*fScoreBlockLayout);
  }

  if (fScoreBlockMidiTempo) {
    msrBrowser<msrMidiTempo> browser (v);
    browser.browse (*fScoreBlockMidiTempo);
  }

#ifdef TRACING_IS_ENABLED
  if (gGlobalLpsrOahGroup->getTraceLpsrVisitors ()) {
    gLogStream <<
      "% <== lpsrScoreBlock::browseData ()" <<
      std::endl;
  }
#endif
}

void lpsrScoreBlock::print (std::ostream& os) const
{
  os << "ScoreBlock" << std::endl << std::endl;

  ++gIndenter;

  os <<
    fBookBlockElementParallelMusicBlock <<
    std::endl;

  if (fScoreBlockLayout) {
    os <<
      fScoreBlockLayout <<
      std::endl;
  }

  os <<
    fScoreBlockMidiTempo <<
    std::endl;

  --gIndenter;
}

std::ostream& operator << (std::ostream& os, const S_lpsrScoreBlock& scr)
{
  scr->print (os);
  return os;
}

//______________________________________________________________________________
S_lpsrBookPartBlock lpsrBookPartBlock::create (
  int inputLineNumber)
{
  lpsrBookPartBlock* o = new lpsrBookPartBlock (
    inputLineNumber);
  assert (o != nullptr);
  return o;
}

lpsrBookPartBlock::lpsrBookPartBlock (
  int            inputLineNumber)
    : lpsrBookBlockElement (inputLineNumber)
{
  // create the bookpart block parallel music
  fBookBlockElementParallelMusicBlock =
    lpsrParallelMusicBLock::create (
      inputLineNumber,
      lpsrElementsSeparatorKind::kElementsSeparatorEndOfLine);
}

lpsrBookPartBlock::~lpsrBookPartBlock ()
{}

void lpsrBookPartBlock::appendPartGroupBlockToBookPartBlock (
  const S_lpsrPartGroupBlock& partGroupBlock)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTracePartGroups ()) {
    gLogStream <<
      "Appending part group block " <<
       partGroupBlock-> getPartGroup ()-> getPartGroupCombinedName () <<
       " to LPSR book part block" <<
       std::endl;
  }
#endif

  fBookBlockElementParallelMusicBlock->
    appendPartGroupBlockToParallelMusicBLock (
      partGroupBlock);

//               fBlockElements.push_back(partGroupBlock);
}

/* JMI
void lpsrBookPartBlock::appendVoiceUseToParallelMusicBLock (
  S_lpsrUseVoiceCommand voiceUse)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceVoices ()) {
    gLogStream <<
      "Appending the use of voice \"" <<
       voiceUse-> getVoice ()-> getVoiceName () <<
       "\" to LPSR score" <<
       std::endl;
  }
#endif

  fBookBlockElementParallelMusicBlock->
    addElementToParallelMusicBLock (voiceUse);

//     JMI             fBlockElements.push_back(voiceUse);
}

void lpsrBookPartBlock::appendLyricsUseToParallelMusicBLock (
  S_lpsrNewLyricsBlock lyricsUse)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceLyrics ()) {
    gLogStream <<
      "Appending the use of stanza " <<
       lyricsUse-> getStanza ()-> getStanzaName () <<
       " to LPSR score" <<
       std::endl;
  }
#endif

  fBookBlockElementParallelMusicBlock->
    addElementToParallelMusicBLock (lyricsUse);
}
*/

void lpsrBookPartBlock::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalLpsrOahGroup->getTraceLpsrVisitors ()) {
    gLogStream <<
      "% ==> lpsrBookPartBlock::acceptIn ()" <<
      std::endl;
  }
#endif

  if (visitor<S_lpsrBookPartBlock>*
    p =
      dynamic_cast<visitor<S_lpsrBookPartBlock>*> (v)) {
        S_lpsrBookPartBlock elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalLpsrOahGroup->getTraceLpsrVisitors ()) {
          gLogStream <<
            "% ==> Launching lpsrBookPartBlock::visitStart ()" <<
            std::endl;
        }
#endif
        p->visitStart (elem);
  }
}

void lpsrBookPartBlock::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalLpsrOahGroup->getTraceLpsrVisitors ()) {
    gLogStream <<
      "% ==> lpsrBookPartBlock::acceptOut ()" <<
      std::endl;
  }
#endif

  if (visitor<S_lpsrBookPartBlock>*
    p =
      dynamic_cast<visitor<S_lpsrBookPartBlock>*> (v)) {
        S_lpsrBookPartBlock elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalLpsrOahGroup->getTraceLpsrVisitors ()) {
          gLogStream <<
            "% ==> Launching lpsrBookPartBlock::visitEnd ()" <<
            std::endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void lpsrBookPartBlock::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalLpsrOahGroup->getTraceLpsrVisitors ()) {
    gLogStream <<
      "% ==> lpsrBookPartBlock::browseData ()" <<
      std::endl;
  }
#endif

  {
    // browse the bookpart block parallel music
    msrBrowser<lpsrParallelMusicBLock> browser (v);
    browser.browse (*fBookBlockElementParallelMusicBlock);
  }

#ifdef TRACING_IS_ENABLED
  if (gGlobalLpsrOahGroup->getTraceLpsrVisitors ()) {
    gLogStream <<
      "% <== lpsrBookPartBlock::browseData ()" <<
      std::endl;
  }
#endif
}

void lpsrBookPartBlock::print (std::ostream& os) const
{
  os << "BookPartBlock" << std::endl << std::endl;

  ++gIndenter;

  os <<
    fBookBlockElementParallelMusicBlock <<
    std::endl;

  --gIndenter;
}

std::ostream& operator << (std::ostream& os, const S_lpsrBookPartBlock& scr)
{
  scr->print (os);
  return os;
}

//______________________________________________________________________________
S_lpsrBookBlock lpsrBookBlock::create (
  int inputLineNumber)
{
  lpsrBookBlock* o = new lpsrBookBlock (
    inputLineNumber);
  assert (o != nullptr);
  return o;
}

lpsrBookBlock::lpsrBookBlock (
  int inputLineNumber)
    : lpsrElement (inputLineNumber)
{
  /* JMI
  // create the score block element parallel music
  fBookBlockElementParallelMusicBlock =
    lpsrParallelMusicBLock::create (
      inputLineNumber,
      lpsrElementsSeparatorKind::kElementsSeparatorEndOfLine);
*/

  // create the book block element header
  fBookBlockHeader =
    lpsrHeader::create (
      inputLineNumber);

  // don't create the book block element paper
  // it will be created as a new born clone of the one in the lpsrScore
  // when the scaling is handled in visitStart (S_msrScaling&)
}

lpsrBookBlock::~lpsrBookBlock ()
{}

/* JMI
void lpsrBookBlock::appendPartGroupBlockToBlock (
  const S_lpsrPartGroupBlock& partGroupBlock)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTracePartGroups ()) {
    gLogStream <<
      "Appending part group block " <<
       partGroupBlock-> getPartGroup ()-> getPartGroupCombinedName () <<
       " to LPSR score" <<
       std::endl;
  }
#endif

  fBookBlockElementParallelMusicBlock->
    appendPartGroupBlockToParallelMusicBLock (
      partGroupBlock);

//               fBlockElements.push_back(partGroupBlock);
}
*/

void lpsrBookBlock::appendBookBlockElementToBookBlockElementsList (
  const S_lpsrBookBlockElement& bookBlockElement)
{
/* JMI
#ifdef TRACING_IS_ENABLED
  if (gGlobalLpsrOahGroup->getTraceLpsrBlocks ()) {
    gLogStream <<
      "Appending book block element  " <<
       bookBlockElement-> asString () <<
       " to LPSR book block" <<
       std::endl;
  }
#endif
*/
  fBookBlockElementsList.push_back (
      bookBlockElement);
}

void lpsrBookBlock::appendLpsrScoreBlockToBookBlockElementsList (
  const S_lpsrScoreBlock& scoreBlock)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalLpsrOahGroup->getTraceLpsrBlocks ()) {
    gLogStream <<
      "Appending score block element  " <<
       scoreBlock-> asString () <<
       " to LPSR book block elements list" <<
       std::endl;
  }
#endif

  appendBookBlockElementToBookBlockElementsList (
    scoreBlock);
}

void lpsrBookBlock::appendLpsrBookPartBlockToBookBlockElementsList (
  const S_lpsrBookPartBlock& bookPartBlock)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalLpsrOahGroup->getTraceLpsrBlocks ()) {
    gLogStream <<
      "Appending book part block element  " <<
       bookPartBlock-> asString () <<
       " to LPSR book block elements list" <<
       std::endl;
  }
#endif

  appendBookBlockElementToBookBlockElementsList (
      bookPartBlock);
}

/* JMI
void lpsrBookBlock::appendVoiceUseToParallelMusicBLock (
  S_lpsrUseVoiceCommand voiceUse)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceVoices ()) {
    gLogStream <<
      "Appending the use of voice \"" <<
       voiceUse-> getVoice ()-> getVoiceName () <<
       "\" to LPSR score" <<
       std::endl;
  }
#endif

  fBookBlockElementParallelMusicBlock->
    addElementToParallelMusicBLock (voiceUse);

//     JMI             fBlockElements.push_back(voiceUse);
}

void lpsrBookBlock::appendLyricsUseToParallelMusicBLock (
  S_lpsrNewLyricsBlock lyricsUse)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceLyrics ()) {
    gLogStream <<
      "Appending the use of stanza " <<
       lyricsUse-> getStanza ()-> getStanzaName () <<
       " to LPSR score" <<
       std::endl;
  }
#endif

  fBookBlockElementParallelMusicBlock->
    addElementToParallelMusicBLock (lyricsUse);
}
*/

void lpsrBookBlock::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalLpsrOahGroup->getTraceLpsrVisitors ()) {
    gLogStream <<
      "% ==> lpsrBookBlock::acceptIn ()" <<
      std::endl;
  }
#endif

  if (visitor<S_lpsrBookBlock>*
    p =
      dynamic_cast<visitor<S_lpsrBookBlock>*> (v)) {
        S_lpsrBookBlock elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalLpsrOahGroup->getTraceLpsrVisitors ()) {
          gLogStream <<
            "% ==> Launching lpsrBookBlock::visitStart ()" <<
            std::endl;
        }
#endif
        p->visitStart (elem);
  }
}

void lpsrBookBlock::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalLpsrOahGroup->getTraceLpsrVisitors ()) {
    gLogStream <<
      "% ==> lpsrBookBlock::acceptOut ()" <<
      std::endl;
  }
#endif

  if (visitor<S_lpsrBookBlock>*
    p =
      dynamic_cast<visitor<S_lpsrBookBlock>*> (v)) {
        S_lpsrBookBlock elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalLpsrOahGroup->getTraceLpsrVisitors ()) {
          gLogStream <<
            "% ==> Launching lpsrBookBlock::visitEnd ()" <<
            std::endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void lpsrBookBlock::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalLpsrOahGroup->getTraceLpsrVisitors ()) {
    gLogStream <<
      "% ==> lpsrBookBlock::browseData ()" <<
      std::endl;
  }
#endif

  if (fBookBlockHeader) {
    // browse the book block header
    msrBrowser<lpsrHeader> browser (v);
    browser.browse (*fBookBlockHeader);
  }

  for (
    std::list<S_lpsrBookBlockElement>::const_iterator i = fBookBlockElementsList.begin ();
    i != fBookBlockElementsList.end ();
    ++i
  ) {
    // browse block the element
    msrBrowser<lpsrBookBlockElement> browser (v);
    browser.browse (*(*i));
  } // for

  if (fBookBlockPaper) {
    // browse the book block paper
    msrBrowser<lpsrPaper> browser (v);
    browser.browse (*fBookBlockPaper);
  }

#ifdef TRACING_IS_ENABLED
  if (gGlobalLpsrOahGroup->getTraceLpsrVisitors ()) {
    gLogStream <<
      "% <== lpsrBookBlock::browseData ()" <<
      std::endl;
  }
#endif
}

void lpsrBookBlock::print (std::ostream& os) const
{
  os << "BookBlock" << std::endl << std::endl;

  ++gIndenter;

  if (fBookBlockHeader) {
    os <<
      fBookBlockHeader <<
      std::endl;
  }

  os <<
    "BookBlockElements";

  if (fBookBlockElementsList.size ()) {
    os << std::endl;

    ++gIndenter;

    for (
      std::list<S_lpsrBookBlockElement>::const_iterator i = fBookBlockElementsList.begin ();
      i != fBookBlockElementsList.end ();
      ++i
    ) {
      // print the book block element
      os << (*i);
    } // for

    --gIndenter;
  }
  else {
    os <<
      " : [NONE]" <<
      std::endl;
  }

  if (fBookBlockPaper) {
    os <<
      fBookBlockPaper <<
      std::endl;
  }

  --gIndenter;
}

std::ostream& operator << (std::ostream& os, const S_lpsrBookBlock& scr)
{
  scr->print (os);
  return os;
}


}
