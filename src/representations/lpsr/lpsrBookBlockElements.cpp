/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include "visitor.h"

#include "mfPreprocessorSettings.h"

#include "oahOah.h"

#include "lpsrBookBlockElements.h"

#include "lpsrOah.h"
#include "lpsr2lilypondOah.h"

#include "msrBrowsers.h"

#include "waeHandlers.h"


namespace MusicFormats
{

//______________________________________________________________________________
S_lpsrBookBlockElement lpsrBookBlockElement::create (
  int inputLineNumber)
{
  lpsrBookBlockElement* obj = new lpsrBookBlockElement (
    inputLineNumber);
  assert (obj != nullptr);
  return obj;
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
#ifdef MF_TRACE_IS_ENABLED
  if (gLpsrOahGroup->getTraceLpsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> lpsrBookBlockElement::acceptIn ()";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_lpsrBookBlockElement>*
    p =
      dynamic_cast<visitor<S_lpsrBookBlockElement>*> (v)) {
        S_lpsrBookBlockElement elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gLpsrOahGroup->getTraceLpsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching lpsrBookBlockElement::visitStart ()";

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void lpsrBookBlockElement::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gLpsrOahGroup->getTraceLpsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> lpsrBookBlockElement::acceptOut ()";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_lpsrBookBlockElement>*
    p =
      dynamic_cast<visitor<S_lpsrBookBlockElement>*> (v)) {
        S_lpsrBookBlockElement elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gLpsrOahGroup->getTraceLpsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching lpsrBookBlockElement::visitEnd ()";

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void lpsrBookBlockElement::browseData (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gLpsrOahGroup->getTraceLpsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> lpsrBookBlockElement::browseData ()";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  {
    // browse the book block element parallel music
    msrBrowser<lpsrParallelMusicBLock> browser (v);
    browser.browse (*fBookBlockElementParallelMusicBlock);
  }

/* JMI
  for (
    std::vector <S_msrElement>::const_iterator i = fBlockElements.begin ();
    i != fBlockElements.end ();
    ++i
  ) {
    // browse the element
 //   msrBrowser<msrElement> browser (v);
 //   browser.browse (*(*i));
  } // for
*/

#ifdef MF_TRACE_IS_ENABLED
  if (gLpsrOahGroup->getTraceLpsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% <== lpsrBookBlockElement::browseData ()";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
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
  lpsrScoreBlock* obj = new lpsrScoreBlock (
    inputLineNumber);
  assert (obj != nullptr);
  return obj;
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

  std::string midiTempoNotesDuration = "16";
  int         midiTempoPerSecond = 360;

  if (midiTempoAtom->getSelected ()) {
    const msrMidiTempo&
      midiTempo =
       gGlobalLpsr2lilypondOahGroup->getMidiTempo ();

    midiTempoNotesDuration = midiTempo.getMidiTempoNotesDuration ();
    midiTempoPerSecond = midiTempo.getMidiTempoPerSecond ();
  }
  else {
    // JMI use hard-wired value set above
  }

  fScoreBlockMidiTempo =
    msrMidiTempo::create (
      inputLineNumber,
      midiTempoNotesDuration,
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
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTracePartGroups ()) {
    std::stringstream ss;

    ss <<
      "Appending part group block " <<
       partGroupBlock-> getPartGroup ()-> fetchPartGroupCombinedName () <<
       " to LPSR score block" <<
       std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fBookBlockElementParallelMusicBlock->
    appendPartGroupBlockToParallelMusicBLock (
      partGroupBlock);

//               fBlockElements.push_back(partGroupBlock);
}

/* JMI
void lpsrScoreBlock::appendVoiceUseToParallelMusicBLock (
  S_lpsrUseVoiceCommand voiceUse)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceVoices ()) {
    std::stringstream ss;

    ss <<
      "Appending the use of voice \"" <<
       voiceUse-> getVoice ()-> getVoiceName () <<
       "\" to LPSR score" <<
       std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fBookBlockElementParallelMusicBlock->
    addElementToParallelMusicBLock (voiceUse);

//     JMI             fBlockElements.push_back(voiceUse);
}

void lpsrScoreBlock::appendLyricsUseToParallelMusicBLock (
  S_lpsrNewLyricsBlock lyricsUse)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceLyrics ()) {
    std::stringstream ss;

    ss <<
      "Appending the use of stanza " <<
       lyricsUse-> getStanza ()-> getStanzaName () <<
       " to LPSR score" <<
       std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fBookBlockElementParallelMusicBlock->
    addElementToParallelMusicBLock (lyricsUse);
}
*/

void lpsrScoreBlock::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gLpsrOahGroup->getTraceLpsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> lpsrScoreBlock::acceptIn ()";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_lpsrScoreBlock>*
    p =
      dynamic_cast<visitor<S_lpsrScoreBlock>*> (v)) {
        S_lpsrScoreBlock elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gLpsrOahGroup->getTraceLpsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching lpsrScoreBlock::visitStart ()";

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void lpsrScoreBlock::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gLpsrOahGroup->getTraceLpsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> lpsrScoreBlock::acceptOut ()";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_lpsrScoreBlock>*
    p =
      dynamic_cast<visitor<S_lpsrScoreBlock>*> (v)) {
        S_lpsrScoreBlock elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gLpsrOahGroup->getTraceLpsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching lpsrScoreBlock::visitEnd ()";

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void lpsrScoreBlock::browseData (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gLpsrOahGroup->getTraceLpsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> lpsrScoreBlock::browseData ()";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  {
    // browse the score block parallel music
    msrBrowser<lpsrParallelMusicBLock> browser (v);
    browser.browse (*fBookBlockElementParallelMusicBlock);
  }

/* JMI
  for (
    std::vector <S_msrElement>::const_iterator i = fBlockElements.begin ();
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

#ifdef MF_TRACE_IS_ENABLED
  if (gLpsrOahGroup->getTraceLpsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% <== lpsrScoreBlock::browseData ()";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
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
  lpsrBookPartBlock* obj = new lpsrBookPartBlock (
    inputLineNumber);
  assert (obj != nullptr);
  return obj;
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
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTracePartGroups ()) {
    std::stringstream ss;

    ss <<
      "Appending part group block " <<
       partGroupBlock-> getPartGroup ()-> fetchPartGroupCombinedName () <<
       " to LPSR book part block" <<
       std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fBookBlockElementParallelMusicBlock->
    appendPartGroupBlockToParallelMusicBLock (
      partGroupBlock);

//               fBlockElements.push_back(partGroupBlock);
}

/* JMI
void lpsrBookPartBlock::appendVoiceUseToParallelMusicBLock (
  S_lpsrUseVoiceCommand voiceUse)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceVoices ()) {
    std::stringstream ss;

    ss <<
      "Appending the use of voice \"" <<
       voiceUse-> getVoice ()-> getVoiceName () <<
       "\" to LPSR score" <<
       std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fBookBlockElementParallelMusicBlock->
    addElementToParallelMusicBLock (voiceUse);

//     JMI             fBlockElements.push_back(voiceUse);
}

void lpsrBookPartBlock::appendLyricsUseToParallelMusicBLock (
  S_lpsrNewLyricsBlock lyricsUse)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceLyrics ()) {
    std::stringstream ss;

    ss <<
      "Appending the use of stanza " <<
       lyricsUse-> getStanza ()-> getStanzaName () <<
       " to LPSR score" <<
       std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fBookBlockElementParallelMusicBlock->
    addElementToParallelMusicBLock (lyricsUse);
}
*/

void lpsrBookPartBlock::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gLpsrOahGroup->getTraceLpsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> lpsrBookPartBlock::acceptIn ()";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_lpsrBookPartBlock>*
    p =
      dynamic_cast<visitor<S_lpsrBookPartBlock>*> (v)) {
        S_lpsrBookPartBlock elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gLpsrOahGroup->getTraceLpsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching lpsrBookPartBlock::visitStart ()";

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void lpsrBookPartBlock::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gLpsrOahGroup->getTraceLpsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> lpsrBookPartBlock::acceptOut ()";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_lpsrBookPartBlock>*
    p =
      dynamic_cast<visitor<S_lpsrBookPartBlock>*> (v)) {
        S_lpsrBookPartBlock elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gLpsrOahGroup->getTraceLpsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching lpsrBookPartBlock::visitEnd ()";

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void lpsrBookPartBlock::browseData (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gLpsrOahGroup->getTraceLpsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> lpsrBookPartBlock::browseData ()";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  {
    // browse the bookpart block parallel music
    msrBrowser<lpsrParallelMusicBLock> browser (v);
    browser.browse (*fBookBlockElementParallelMusicBlock);
  }

#ifdef MF_TRACE_IS_ENABLED
  if (gLpsrOahGroup->getTraceLpsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% <== lpsrBookPartBlock::browseData ()";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
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
  lpsrBookBlock* obj = new lpsrBookBlock (
    inputLineNumber);
  assert (obj != nullptr);
  return obj;
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
  // it will be created as a newborn clone of the one in the lpsrScore
  // when the scaling is handled in visitStart (S_msrScaling&)
}

lpsrBookBlock::~lpsrBookBlock ()
{}

/* JMI
void lpsrBookBlock::appendPartGroupBlockToBlock (
  const S_lpsrPartGroupBlock& partGroupBlock)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTracePartGroups ()) {
    std::stringstream ss;

    ss <<
      "Appending part group block " <<
       partGroupBlock-> getPartGroup ()-> fetchPartGroupCombinedName () <<
       " to LPSR score" <<
       std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

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
#ifdef MF_TRACE_IS_ENABLED
  if (gLpsrOahGroup->getTraceLpsrBlocks ()) {
    std::stringstream ss;

    ss <<
      "Appending book block element  " <<
       bookBlockElement-> asString () <<
       " to LPSR book block" <<
       std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
*/
  fBookBlockElementsList.push_back (
      bookBlockElement);
}

void lpsrBookBlock::appendLpsrScoreBlockToBookBlockElementsList (
  const S_lpsrScoreBlock& scoreBlock)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gLpsrOahGroup->getTraceLpsrBlocks ()) {
    std::stringstream ss;

    ss <<
      "Appending score block element  " <<
       scoreBlock-> asString () <<
       " to LPSR book block elements list" <<
       std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  appendBookBlockElementToBookBlockElementsList (
    scoreBlock);
}

void lpsrBookBlock::appendLpsrBookPartBlockToBookBlockElementsList (
  const S_lpsrBookPartBlock& bookPartBlock)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gLpsrOahGroup->getTraceLpsrBlocks ()) {
    std::stringstream ss;

    ss <<
      "Appending book part block element  " <<
       bookPartBlock-> asString () <<
       " to LPSR book block elements list" <<
       std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  appendBookBlockElementToBookBlockElementsList (
      bookPartBlock);
}

/* JMI
void lpsrBookBlock::appendVoiceUseToParallelMusicBLock (
  S_lpsrUseVoiceCommand voiceUse)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceVoices ()) {
    std::stringstream ss;

    ss <<
      "Appending the use of voice \"" <<
       voiceUse-> getVoice ()-> getVoiceName () <<
       "\" to LPSR score" <<
       std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fBookBlockElementParallelMusicBlock->
    addElementToParallelMusicBLock (voiceUse);

//     JMI             fBlockElements.push_back(voiceUse);
}

void lpsrBookBlock::appendLyricsUseToParallelMusicBLock (
  S_lpsrNewLyricsBlock lyricsUse)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceLyrics ()) {
    std::stringstream ss;

    ss <<
      "Appending the use of stanza " <<
       lyricsUse-> getStanza ()-> getStanzaName () <<
       " to LPSR score" <<
       std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fBookBlockElementParallelMusicBlock->
    addElementToParallelMusicBLock (lyricsUse);
}
*/

void lpsrBookBlock::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gLpsrOahGroup->getTraceLpsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> lpsrBookBlock::acceptIn ()";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_lpsrBookBlock>*
    p =
      dynamic_cast<visitor<S_lpsrBookBlock>*> (v)) {
        S_lpsrBookBlock elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gLpsrOahGroup->getTraceLpsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching lpsrBookBlock::visitStart ()";

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void lpsrBookBlock::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gLpsrOahGroup->getTraceLpsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> lpsrBookBlock::acceptOut ()";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_lpsrBookBlock>*
    p =
      dynamic_cast<visitor<S_lpsrBookBlock>*> (v)) {
        S_lpsrBookBlock elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gLpsrOahGroup->getTraceLpsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching lpsrBookBlock::visitEnd ()";

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void lpsrBookBlock::browseData (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gLpsrOahGroup->getTraceLpsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> lpsrBookBlock::browseData ()";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (fBookBlockHeader) {
    // browse the book block header
    msrBrowser<lpsrHeader> browser (v);
    browser.browse (*fBookBlockHeader);
  }

  for (
    std::list <S_lpsrBookBlockElement>::const_iterator i = fBookBlockElementsList.begin ();
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

#ifdef MF_TRACE_IS_ENABLED
  if (gLpsrOahGroup->getTraceLpsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% <== lpsrBookBlock::browseData ()";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
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
      std::list <S_lpsrBookBlockElement>::const_iterator i = fBookBlockElementsList.begin ();
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
      " : [EMPTY]" <<
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
