/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <iomanip>      // std::setw, std::setprecision, ...
#include <cmath>
#include <string>

#include "mfPreprocessorSettings.h"

#include "mfAssert.h"
#include "mfServices.h"
#include "mfStringsHandling.h"
#include "mfIndentedTextOutput.h"

#include "msrAfterGraceNotes.h"
#include "msrArticulations.h"
#include "msrBarChecks.h"
#include "msrBarLines.h"
#include "msrBarNumberChecks.h"
#include "msrDoubleTremolos.h"
#include "msrGlissandos.h"
#include "msrLineBreaks.h"
#include "msrPageBreaks.h"
#include "msrRehearsalMarks.h"
#include "msrSlides.h"
#include "msrTablatures.h"
#include "msrTechnicals.h"
#include "msrTempos.h"
#include "msrTemposEnumTypes.h"
#include "msrVoiceStaffChanges.h"

#include "lpsrBarNumbers.h"
#include "lpsrLyrics.h"
#include "lpsrParts.h"
#include "lpsrStaves.h"
#include "lpsrScores.h"
#include "lpsrVoices.h"

#include "waeInterface.h"
#include "lpsr2lilypondWae.h"

#include "oahOah.h"

#include "waeOah.h"

#include "lpsr2lilypondOah.h"

#include "lpsrBrowsers.h"

#include "lpsr2lilypondTranslator.h"

#include "waeHandlers.h"


namespace MusicFormats
{

//______________________________________________________________________________
// for comments highlighting in the generated Lilypond code
constexpr int commentFieldWidth = 30;

//______________________________________________________________________________
S_lpsrRepeat lpsrRepeat::create (
  const S_msrRepeat& repeat,
  int                repeatEndingsNumber)
{
  lpsrRepeat* obj = new
    lpsrRepeat (
      repeat,
      repeatEndingsNumber);
  assert (obj != nullptr);
  return obj;
}

lpsrRepeat::lpsrRepeat (
  const S_msrRepeat& repeat,
  int                repeatEndingsNumber)
{
  fRepeat = repeat;

  fRepeatEndingsNumber = repeatEndingsNumber;
  fRepeatEndingsCounter = 0;

  fEndOfRepeatHasBeenGenerated = false; // superflous
}

lpsrRepeat::~lpsrRepeat ()
{}

std::string lpsrRepeat::asString ()
{
  std::stringstream ss;

  ss <<
    "[lpsrRepeat"
    ", fRepeatEndingsNumber: " <<
    fRepeatEndingsNumber <<
    ", fRepeatEndingsCounter: " <<
    fRepeatEndingsCounter <<
    ", fEndOfRepeatHasBeenGenerated: " <<
    fEndOfRepeatHasBeenGenerated <<
    ']';

  return ss.str ();
}

void lpsrRepeat::print (std::ostream& os)
{
  int fieldWidth = 29;

  os << std::left <<
    std::setw (fieldWidth) <<
    "fRepeatEndingsNumber" << ": " <<
    fRepeatEndingsNumber <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fRepeatEndingsCounter" << ": " <<
    fRepeatEndingsCounter <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fEndOfRepeatHasBeenGenerated" << ": " <<
    fEndOfRepeatHasBeenGenerated <<
    std::endl;
}

std::ostream& operator << (std::ostream& os, const S_lpsrRepeat& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NULL]" << std::endl;
  }

  return os;
}

//________________________________________________________________________
lpsr2lilypondTranslator::lpsr2lilypondTranslator (
  const S_msrOahGroup&  msrOpts,
  const S_lpsrOahGroup& lpsrOpts,
  std::ostream&         lilypondCodeStream)
  : fLilypondCodeStream (
      lilypondCodeStream)
{
  fMsrOahGroup = msrOpts;
  fLpsrOahGroup = lpsrOpts;

  initializeLilypondUsefulFragments ();

  // the private fields will be (re-)initialized each time
  // translateLpsrToLilypondCode() is called
};

lpsr2lilypondTranslator::~lpsr2lilypondTranslator ()
{}

void lpsr2lilypondTranslator::initializeLilypondUsefulFragments ()
{
  // spaces
  cLilyPondCommaAndSpace = ", ";
  if (gGlobalLpsr2lilypondOahGroup->getCommentLilypondStructure ()) {
    cLilyPondCommaAndSpace +=
      "%{ cLilyPondCommaAndSpace %} ";
  }

  // parts
  cLilypondPartOpener = "<< ";
  if (gGlobalLpsr2lilypondOahGroup->getCommentLilypondStructure ()) {
    cLilypondPartOpener +=
      "%{ cLilypondPartOpener %} ";
  }
  cLilypondPartCloser = ">> ";
  if (gGlobalLpsr2lilypondOahGroup->getCommentLilypondStructure ()) {
    cLilypondPartCloser +=
      "%{ cLilypondPartCloser %} ";
  }

  // staff groups
  cLilypondStaffBlockOpener = "<< ";
  if (gGlobalLpsr2lilypondOahGroup->getCommentLilypondStructure ()) {
    cLilypondStaffBlockOpener +=
      "%{ cLilypondStaffBlockOpener %} ";
  }
  cLilypondStaffBlockCloser = ">> ";
  if (gGlobalLpsr2lilypondOahGroup->getCommentLilypondStructure ()) {
    cLilypondStaffBlockCloser +=
      "%{ cLilypondStaffBlockCloser %} ";
  }

  cLilypondNewStaffGroup = "\\new StaffGroup ";
  if (gGlobalLpsr2lilypondOahGroup->getCommentLilypondStructure ()) {
    cLilypondNewStaffGroup +=
      "%{ cLilypondNewStaffGroup %} ";
  }
  cLilypondStaffGroupOpener = "<< ";
  if (gGlobalLpsr2lilypondOahGroup->getCommentLilypondStructure ()) {
    cLilypondStaffGroupOpener +=
      "%{ cLilypondStaffGroupOpener %} ";
  }
  cLilypondStaffGroupCloser = ">> ";
  if (gGlobalLpsr2lilypondOahGroup->getCommentLilypondStructure ()) {
    cLilypondStaffGroupCloser +=
      "%{ cLilypondStaffGroupCloser %} ";
  }

  // staves
  cLilypondNewStaff = "\\new Staff ";
  if (gGlobalLpsr2lilypondOahGroup->getCommentLilypondStructure ()) {
    cLilypondNewStaff +=
      "%{ cLilypondNewStaff %} ";
  }
  cLilypondNewTabStaff = "\\new TabStaff ";
  if (gGlobalLpsr2lilypondOahGroup->getCommentLilypondStructure ()) {
    cLilypondNewTabStaff +=
      "%{ cLilypondNewTabStaff %} ";
  }
  cLilypondNewHarmoniesStaff = "\\new HarmoniesStaff ";
  if (gGlobalLpsr2lilypondOahGroup->getCommentLilypondStructure ()) {
    cLilypondNewHarmoniesStaff +=
      "%{ cLilypondNewHarmoniesStaff %} ";
  }
  cLilypondNewFiguredBassStaff = "\\new FiguredBassStaff ";
  if (gGlobalLpsr2lilypondOahGroup->getCommentLilypondStructure ()) {
    cLilypondNewFiguredBassStaff +=
      "%{ cLilypondNewFiguredBassStaff %} ";
  }
  cLilypondNewDrumStaff = "\\new DrumStaff ";
  if (gGlobalLpsr2lilypondOahGroup->getCommentLilypondStructure ()) {
    cLilypondNewDrumStaff +=
      "%{ cLilypondNewDrumStaff %} ";
  }
  cLilypondNewRythmicStaff = "\\new RythmicStaff ";
  if (gGlobalLpsr2lilypondOahGroup->getCommentLilypondStructure ()) {
    cLilypondNewRythmicStaff +=
      "%{ cLilypondNewRythmicStaff %} ";
  }
  cLilypondNewJianpuStaff = "\\new JianpuStaff ";
  if (gGlobalLpsr2lilypondOahGroup->getCommentLilypondStructure ()) {
    cLilypondNewJianpuStaff +=
      "%{ cLilypondNewJianpuStaff %} ";
  }

  cLilypondNewGrandStaff = "\\new GrandStaff ";
  if (gGlobalLpsr2lilypondOahGroup->getCommentLilypondStructure ()) {
    cLilypondNewGrandStaff +=
      "%{ cLilypondNewGrandStaff %} ";
  }
  cLilypondNewPianoStaff = "\\new PianoStaff ";
  if (gGlobalLpsr2lilypondOahGroup->getCommentLilypondStructure ()) {
    cLilypondNewPianoStaff +=
      "%{ cLilypondNewPianoStaff %} ";
  }
  cLilypondNewChoirStaff = "\\new ChoirStaff ";
  if (gGlobalLpsr2lilypondOahGroup->getCommentLilypondStructure ()) {
    cLilypondNewChoirStaff +=
      "%{ cLilypondNewChoirStaff %} ";
  }

  // time signatures
  cLilypondTimeSignatureOpener = "(";
  if (gGlobalLpsr2lilypondOahGroup->getCommentLilypondStructure ()) {
    cLilypondTimeSignatureOpener +=
      "%{ cLilypondTimeSignatureOpener %} ";
  }
  cLilypondTimeSignatureCloser = ") ";
  if (gGlobalLpsr2lilypondOahGroup->getCommentLilypondStructure ()) {
    cLilypondTimeSignatureCloser +=
      "%{ cLilypondTimeSignatureCloser %} ";
  }

  // tempos
  cLilypondTempoBeatUnitsPerMinuteOpener = "(";
  if (gGlobalLpsr2lilypondOahGroup->getCommentLilypondStructure ()) {
    cLilypondTempoBeatUnitsPerMinuteOpener +=
      "%{ cLilypondTempoBeatUnitsPerMinuteOpener %} ";
  }
  cLilypondTempoBeatUnitsPerMinuteCloser = ")";
  if (gGlobalLpsr2lilypondOahGroup->getCommentLilypondStructure ()) {
    cLilypondTempoBeatUnitsPerMinuteCloser +=
      "%{ cLilypondTempoBeatUnitsPerMinuteCloser %} ";
  }

  // layout context
  cLilypondLayoutContextScoreOpener = "\\context { \\Score ";
  if (gGlobalLpsr2lilypondOahGroup->getCommentLilypondStructure ()) {
    cLilypondLayoutContextScoreOpener +=
      "%{ cLilypondLayoutContextScoreOpener %} ";
  }
  cLilypondLayoutContextScoreCloser = "} ";
  if (gGlobalLpsr2lilypondOahGroup->getCommentLilypondStructure ()) {
    cLilypondLayoutContextScoreCloser +=
      "%{ cLilypondLayoutContextScoreCloser %} ";
  }

  cLilypondLayoutContextStaffOpener = "\\context { \\Staff ";
  if (gGlobalLpsr2lilypondOahGroup->getCommentLilypondStructure ()) {
    cLilypondLayoutContextStaffOpener +=
      "%{ cLilypondLayoutContextStaffOpener %} ";
  }
  cLilypondLayoutContextStaffCloser = "} ";
  if (gGlobalLpsr2lilypondOahGroup->getCommentLilypondStructure ()) {
    cLilypondLayoutContextStaffCloser +=
      "%{ cLilypondLayoutContextStaffCloser %} ";
  }

  cLilypondLayoutContextVoiceOpener = "\\context { \\Voice ";
  if (gGlobalLpsr2lilypondOahGroup->getCommentLilypondStructure ()) {
    cLilypondLayoutContextVoiceOpener +=
      "%{ cLilypondLayoutContextVoiceOpener %} ";
  }
  cLilypondLayoutContextVoiceCloser = "} ";
  if (gGlobalLpsr2lilypondOahGroup->getCommentLilypondStructure ()) {
    cLilypondLayoutContextVoiceCloser +=
      "%{ cLilypondLayoutContextVoiceCloser %} ";
  }

  // voices usage
  cLilypondUseVoiceOpener = "\\context ";
  if (gGlobalLpsr2lilypondOahGroup->getCommentLilypondStructure ()) {
    cLilypondUseVoiceOpener +=
      "%{ cLilypondUseVoiceOpener %} ";
  }
  cLilypondUseVoiceCloser = ">> ";
  if (gGlobalLpsr2lilypondOahGroup->getCommentLilypondStructure ()) {
    cLilypondUseVoiceCloser +=
      "%{ cLilypondUseVoiceCloser %} ";
  }

  // parallel music
  cLilypondParallelMusicOpener = "<< ";
  if (gGlobalLpsr2lilypondOahGroup->getCommentLilypondStructure ()) {
    cLilypondParallelMusicOpener +=
      "%{ cLilypondParallelMusicOpener %} ";
  }
  cLilypondParallelMusicCloser = ">> ";
  if (gGlobalLpsr2lilypondOahGroup->getCommentLilypondStructure ()) {
    cLilypondParallelMusicCloser +=
      "%{ cLilypondParallelMusicCloser %} ";
  }

  // with
  cLilypondWithOpener = "{";
  if (gGlobalLpsr2lilypondOahGroup->getCommentLilypondStructure ()) {
    cLilypondWithOpener +=
      " %{ cLilypondWithOpener %} ";
  }
  cLilypondWithCloser = "}";
  if (gGlobalLpsr2lilypondOahGroup->getCommentLilypondStructure ()) {
    cLilypondWithCloser +=
      " %{ cLilypondWithCloser %} ";
  }

  // scordatura
  cLilypondScordaturaOpener = "<< ";
  if (gGlobalLpsr2lilypondOahGroup->getCommentLilypondStructure ()) {
    cLilypondScordaturaOpener +=
      "%{ cLilypondScordaturaOpener %} ";
  }
  cLilypondScordaturaCloser = ">> ";
  if (gGlobalLpsr2lilypondOahGroup->getCommentLilypondStructure ()) {
    cLilypondScordaturaCloser +=
      "%{ cLilypondScordaturaCloser %} ";
  }

  // beams
  cLilypondBeamsOpener = "[ ";
  if (gGlobalLpsr2lilypondOahGroup->getCommentLilypondStructure ()) {
    cLilypondBeamsOpener +=
      "%{ cLilypondBeamsOpener %} ";
  }
  cLilypondBeamsCloser = "] ";
  if (gGlobalLpsr2lilypondOahGroup->getCommentLilypondStructure ()) {
    cLilypondBeamsCloser +=
      "%{ cLilypondBeamsCloser %} ";
  }

  // slurs
  cLilypondSlurOpener = "( ";
  if (gGlobalLpsr2lilypondOahGroup->getCommentLilypondStructure ()) {
    cLilypondSlurOpener +=
      "%{ cLilypondSlurOpener %} ";
  }
  cLilypondSlurContinuer = "= ";
  if (gGlobalLpsr2lilypondOahGroup->getCommentLilypondStructure ()) {
    cLilypondSlurContinuer +=
      "%{ cLilypondSlurContinuer %} ";
  }
  cLilypondSlurCloser = ") ";
  if (gGlobalLpsr2lilypondOahGroup->getCommentLilypondStructure ()) {
    cLilypondSlurCloser +=
      "%{ cLilypondSlurCloser %} ";
  }

  // phrasing
  cLilypondPhrasingOpener = "\\( ";
  if (gGlobalLpsr2lilypondOahGroup->getCommentLilypondStructure ()) {
    cLilypondPhrasingOpener +=
      "%{ cLilypondPhrasingOpener %} ";
  }
  cLilypondPhrasingContinuer = "\\= "; // JMI 0.9.74 USELESS ???
  if (gGlobalLpsr2lilypondOahGroup->getCommentLilypondStructure ()) {
    cLilypondPhrasingContinuer +=
      "%{ cLilypondPhrasingContinuer %} ";
  }
  cLilypondPhrasingCloser = "\\) ";
  if (gGlobalLpsr2lilypondOahGroup->getCommentLilypondStructure ()) {
    cLilypondPhrasingCloser +=
      "%{ cLilypondPhrasingCloser %} ";
  }

  // grace notes groups
  cLilypondGraceNotesGroupOpener = " { ";
  if (gGlobalLpsr2lilypondOahGroup->getCommentLilypondStructure ()) {
    cLilypondGraceNotesGroupOpener +=
      "%{ cLilypondGraceNotesGroupOpener %} ";
  }
  cLilypondGraceNotesGroupCloser = "} ";
  if (gGlobalLpsr2lilypondOahGroup->getCommentLilypondStructure ()) {
    cLilypondGraceNotesGroupCloser +=
      "%{ cLilypondGraceNotesGroupCloser %} ";
  }

  // chords
  cLilypondChordOpener = "< ";
  if (gGlobalLpsr2lilypondOahGroup->getCommentLilypondStructure ()) {
    cLilypondChordOpener +=
      "%{ cLilypondChordOpener %} ";
  }
  cLilypondChordCloser = "> ";
  if (gGlobalLpsr2lilypondOahGroup->getCommentLilypondStructure ()) {
    cLilypondChordCloser +=
      "%{ cLilypondChordCloser %} ";
  }

  // tuplets
  cLilypondTupletOpener1 = "\\tuplet ";
  if (gGlobalLpsr2lilypondOahGroup->getCommentLilypondStructure ()) {
    cLilypondTupletOpener1 +=
      "%{ cLilypondTupletOpener1 %} ";
  }
  cLilypondTupletOpener2 = " { ";
  if (gGlobalLpsr2lilypondOahGroup->getCommentLilypondStructure ()) {
    cLilypondTupletOpener2 +=
      "%{ cLilypondTupletOpener2 %} ";
  }
  cLilypondTupletCloser = "} ";
  if (gGlobalLpsr2lilypondOahGroup->getCommentLilypondStructure ()) {
    cLilypondTupletCloser +=
      "%{ cLilypondTupletCloser %} ";
  }

  // ligatures
  cLilypondLigatureOpener = "\\[ ";
  if (gGlobalLpsr2lilypondOahGroup->getCommentLilypondStructure ()) {
    cLilypondLigatureOpener +=
      "%{ cLilypondLigatureOpener %} ";
  }
  cLilypondLigatureCloser = "\\] ";
  if (gGlobalLpsr2lilypondOahGroup->getCommentLilypondStructure ()) {
    cLilypondLigatureCloser +=
      "%{ cLilypondLigatureCloser %} ";
  }

  // hairpins
  cLilypondHairPinsCrescendo = "\\< ";
  if (gGlobalLpsr2lilypondOahGroup->getCommentLilypondStructure ()) {
    cLilypondHairPinsCrescendo +=
      "%{ cLilypondHairPinsCrescendo %} ";
  }
  cLilypondHairPinsDecrescendo = "\\> ";
  if (gGlobalLpsr2lilypondOahGroup->getCommentLilypondStructure ()) {
    cLilypondHairPinsDecrescendo +=
      "%{ cLilypondHairPinsDecrescendo %} ";
  }
  cLilypondHairPinsStop = "\\! ";
  if (gGlobalLpsr2lilypondOahGroup->getCommentLilypondStructure ()) {
    cLilypondHairPinsStop +=
      "%{ cLilypondHairPinsStop %} ";
  }

  // part groups
  cLilypondPartGroupOpener = "<< ";
  if (gGlobalLpsr2lilypondOahGroup->getCommentLilypondStructure ()) {
    cLilypondPartGroupOpener +=
      "%{ cLilypondPartGroupOpener %} ";
  }
  cLilypondPartGroupCloser = ">> ";
  if (gGlobalLpsr2lilypondOahGroup->getCommentLilypondStructure ()) {
    cLilypondPartGroupCloser +=
      "%{ cLilypondPartGroupCloser %} ";
  }

  // lyrics
  cLilypondNewLyrics = "\\new Lyrics ";
  if (gGlobalLpsr2lilypondOahGroup->getCommentLilypondStructure ()) {
    cLilypondNewLyrics +=
      "%{ cLilypondNewLyrics %} ";
  }

  // hide
  cLilypondHide = "\\hide ";
  if (gGlobalLpsr2lilypondOahGroup->getCommentLilypondStructure ()) {
    cLilypondHide +=
      "%{ cLilypondHide %} ";
  }
}

//________________________________________________________________________
void lpsr2lilypondTranslator::translateLpsrToLilypondCode (
  const S_lpsrScore& theLpsrScore)
{
  // the LPSR score we're visiting
  fVisitedLpsrScore = theLpsrScore;

  // get the msrScore
  fCurrentVisitedMsrScore =
    fVisitedLpsrScore->
      getEmbeddedMsrScore ();

  // JMI 0.9.70 these should be browsed to generated slurs...
//   // inhibit the browsing of grace notes groups before,
//   // since they are handled at the note level
//   fCurrentVisitedMsrScore->
//     setInhibitGraceNotesGroupsBeforeBrowsing ();
//
//   // inhibit the browsing of grace notes groups after,
//   // since they are handled at the note level
//   fCurrentVisitedMsrScore->
//     setInhibitGraceNotesGroupsAfterBrowsing ();

  // inhibit the browsing of chords in grace notes groups,
  // since they are handled at the note level
  /* JMI
if (false) // JMI
  fCurrentVisitedMsrScore->
      setInhibitChordsInGraceNotesGroupsBrowsing ();
*/

  // inhibit the browsing of tuplets in grace notes groups,
  // since they are handled at the note level
// not relevant, since there are not tuplets in grace notes groups ??? JMI
/* JMI
  fCurrentVisitedMsrScore->
    setInhibitTupletsInGraceNotesGroupsBrowsing ();
*/

  // inhibit the browsing of measure repeats replicas,
  // since Lilypond only needs the repeat measure
  fCurrentVisitedMsrScore->
    setInhibitMeasureRepeatReplicasBrowsing ();

  // inhibit the browsing of measures repeat replicas,
  // since Lilypond only needs the measure number
// if (false) // JMI 0.9.67
//   fVisitedLpsrScore->
//     getEmbeddedMsrScore ()->
//       setInhibitMultipleMeasureRestsBrowsing ();

  // octaves entry
  // ------------------------------------------------------
  /*
    the octaves entry reference note is:
      mobile in relative mode
      unused in absolute mode
      fixed  in fixed mode
  */

  switch (gGlobalLpsr2lilypondOahGroup->fetchOctaveEntryVariableValue ()) {
    case msrOctaveEntryKind::kOctaveEntryRelative:
      setCurrentOctaveEntryReferenceFromTheLilypondOah ();
      break;

    case msrOctaveEntryKind::kOctaveEntryAbsolute:
      // forget about the current octave entry reference
      fCurrentOctaveEntryReference = nullptr;
      break;

    case msrOctaveEntryKind::kOctaveEntryFixed:
#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
      mfAssert (
        __FILE__, mfInputLineNumber (__LINE__),
        gGlobalLpsr2lilypondOahGroup->
        getFixedOctaveEntrySemiTonesPitchAndOctave () != nullptr,
       "gGlobalLpsr2lilypondOahGroup->getFixedOctaveEntrySemiTonesPitchAndOctave () is NULL");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

      fCurrentOctaveEntryReference =
        msrNote::createNoteFromSemiTonesPitchAndOctave (
          K_MF_INPUT_LINE_UNKNOWN_,
          gGlobalLpsr2lilypondOahGroup->
            getFixedOctaveEntrySemiTonesPitchAndOctave ());
      break;
  } // switch

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceNotesOctaveEntry ()) {
    std::stringstream ss;

    ss <<
      "lpsr2lilypondTranslator()" <<
      ", octaveEntryKind is" <<
      msrOctaveEntryKindAsString (
        gGlobalLpsr2lilypondOahGroup->fetchOctaveEntryVariableValue ()) <<
      std::endl <<
      "Initial fCurrentOctaveEntryReference is ";

    if (fCurrentOctaveEntryReference) {
      ss <<
        '\'' <<
        fCurrentOctaveEntryReference->asString () <<
        "'";
    }
    else {
      ss << "[NULL]";
    }

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // figured bass
  fCurrentFiguredBassFiguresCounter = 0;

  // multiple measure rests
  fRemainingMeasureRestsNumber = 0;

  // measures
  fCurrentVoiceMeasuresCounter = -1;

  // durations
  fLastMetWholeNotes = K_WHOLE_NOTES_UNKNOWN_; // JMI 0.9.67

  // notes
  fCurrentNotePrinObjectKind =
    msrPrintObjectKind::kPrintObjectYes; // default value

  // articulations
  fCurrentArpeggioDirectionKind =
    msrDirectionKind::kDirectionNone;

  // stems
  fCurrentStemKind = msrStemKind::kStemKind_NONE; // default value

  // spanners
  fCurrentSpannerPlacementKind =
    msrPlacementKind::kPlacement_UNKNOWN_;

  // part group blocks
  fNumberOfPartGroupBlocks = -1;
  fPartGroupBlocksCounter =  0;

  // part blocks
  fNumberOfPartGroupBlockElements = -1;
  fPartGroupBlockElementsCounter =  0;

  // staff blocks
  fNumberOfStaffBlocksElements = -1;
  fStaffBlocksCounter =  0;

  if (fVisitedLpsrScore) {
    // set the parts browsing order
    fCurrentVisitedMsrScore->
      setStavesBrowingOrderKind (
        msrStavesBrowingOrderKind::kStavesBrowingOrderHarmoniesFiguredBassesRegulars);

    // set the clefKeyTimeSignature browsing order to musician's order
    setClefKeyTimeSignatureOrderKind (
        msrClefKeyTimeSignatureOrderKind::kClefKeyTimeSignatureOrderClefKeyTimeSignature);

    // browse the visited LPSR score
    lpsrBrowser<lpsrScore> browser (this);
    browser.browse (*fVisitedLpsrScore);
  }
}

//________________________________________________________________________
void lpsr2lilypondTranslator::setCurrentOctaveEntryReferenceFromTheLilypondOah ()
{
  if (
    gGlobalLpsr2lilypondOahGroup->
      getRelativeOctaveEntrySemiTonesPitchAndOctave ()
  ) {
    // option '-rel, -relative' has been used:
    fCurrentOctaveEntryReference =
      msrNote::createNoteFromSemiTonesPitchAndOctave (
        K_MF_INPUT_LINE_UNKNOWN_,
        gGlobalLpsr2lilypondOahGroup->
          getRelativeOctaveEntrySemiTonesPitchAndOctave ());
  }
  else {
    // forget about the current octave entry reference
    // the first note in the voice will become the initial reference
    fCurrentOctaveEntryReference = nullptr;
  }

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceNotesOctaveEntry ()) {
    std::stringstream ss;

    ss <<
      "setCurrentOctaveEntryReferenceFromTheLilypondOah()" <<
      ", octaveEntryKind is" <<
      msrOctaveEntryKindAsString (
        gGlobalLpsr2lilypondOahGroup->fetchOctaveEntryVariableValue ()) <<
      std::endl <<
      "The initial fCurrentOctaveEntryReference is ";

    if (fCurrentOctaveEntryReference) {
      ss <<
        '\'' <<
        fCurrentOctaveEntryReference->asString () <<
        "'";
    }
    else {
      ss << "[NULL]";
    }

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
std::string lpsr2lilypondTranslator::msrModeKindAsLilypondString (
  msrModeKind modeKind)
{
  std::string result;

  switch (modeKind) {
    case msrModeKind::kMode_UNKNOWN_:
      result = "kMode_UNKNOWN_";
      break;
    case msrModeKind::kModeMajor:
      result = "\\major";
      break;
    case msrModeKind::kModeMinor:
      result = "\\minor";
      break;
    case msrModeKind::kModeIonian:
      result = "\\ionian";
      break;
    case msrModeKind::kModeDorian:
      result = "\\dorian";
      break;
    case msrModeKind::kModePhrygian:
      result = "\\phrygian";
      break;
    case msrModeKind::kModeLydian:
      result = "\\lydian";
      break;
    case msrModeKind::kModeMixolydian:
      result = "\\mixolkModeMixolydianydian";
      break;
    case msrModeKind::kModeAeolian:
      result = "\\aeolian";
      break;
    case msrModeKind::kModeLocrian:
      result = "\\locrian";
      break;
  } // switch

  return result;
}

//________________________________________________________________________
std::string lpsr2lilypondTranslator::msrQuarterTonesPitchesLanguageKindAsLilypondString (
  msrQuarterTonesPitchesLanguageKind languageKind)
{
  std::string result;

  switch (languageKind) {
    case msrQuarterTonesPitchesLanguageKind::kQTPNederlands:
      result = "nederlands";
      break;
    case msrQuarterTonesPitchesLanguageKind::kQTPCatalan:
      result = "catalan";
      break;
    case msrQuarterTonesPitchesLanguageKind::kQTPDeutsch:
      result = "deutsch";
      break;
    case msrQuarterTonesPitchesLanguageKind::kQTPEnglish:
      result = "english";
      break;
    case msrQuarterTonesPitchesLanguageKind::kQTPEspanol:
      result = "espanol";
      break;
    case msrQuarterTonesPitchesLanguageKind::kQTPFrancais:
      result = "francais";
      break;
    case msrQuarterTonesPitchesLanguageKind::kQTPItaliano:
      result = "italiano";
      break;
    case msrQuarterTonesPitchesLanguageKind::kQTPNorsk:
      result = "norsk";
      break;
    case msrQuarterTonesPitchesLanguageKind::kQTPPortugues:
      result = "portugues";
      break;
    case msrQuarterTonesPitchesLanguageKind::kQTPSuomi:
      result = "suomi";
      break;
    case msrQuarterTonesPitchesLanguageKind::kQTPSvenska:
      result = "svenska";
      break;
    case msrQuarterTonesPitchesLanguageKind::kQTPVlaams:
      result = "vlaams";
      break;
    case msrQuarterTonesPitchesLanguageKind::kQTPArabic:
      result = "arabic";
      break;
  } // switch

  return result;
}

//________________________________________________________________________
std::string lpsrContextTypeKindAsLilypondString (
  lpsrContextTypeKind contextTypeKind)
{
  std::string result;

  switch (contextTypeKind) {
    case lpsrContextTypeKind::kContextChordNames:
      result = "ChordNames";
      break;
    case lpsrContextTypeKind::kContextFiguredBass:
      result = "FiguredBass";
      break;
  } // switch

  return result;
}

//________________________________________________________________________
std::string lpsr2lilypondTranslator::absoluteOctaveAsLilypondString (
  const mfInputLineNumber& inputLineNumber,
  msrOctaveKind absoluteOctaveKind)
{
  std::string result;

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalLpsr2lilypondOahGroup->getCommentLilypondNotes ()) {
    fLilypondCodeStream <<
      std::endl <<
      "%{ absoluteOctaveKind: " <<
      msrOctaveKindAsString (absoluteOctaveKind) <<
      " %}" <<
      std::endl;
  }
#endif // MF_TRACE_IS_ENABLED

  // generate LilyPond absolute octave
  switch (absoluteOctaveKind) {
    case msrOctaveKind::kOctave_UNKNOWN_:
      {
        std::stringstream ss;

        ss <<
          "absolute octave " <<
          msrOctaveKindAsString (absoluteOctaveKind) <<
          " cannot be translated to LilyPond";

//       result =
//         "%{\\override NoteHead.style = #'cross %} "; // JMI

//         lpsr2lilypondInternalError (
//           gServiceRunData->getInputSourceName (),
//           inputLineNumber,
//           __FILE__, mfInputLineNumber (__LINE__),
//           ss.str ());
      }
    case msrOctaveKind::kOctave0:
      result = ",,,";
      break;
    case msrOctaveKind::kOctave1:
      result = ",,";
      break;
    case msrOctaveKind::kOctave2:
      result = ",";
      break;
    case msrOctaveKind::kOctave3:
      result = "";
      break;
    case msrOctaveKind::kOctave4:
      result = "'";
      break;
    case msrOctaveKind::kOctave5:
      result = "''";
      break;
    case msrOctaveKind::kOctave6:
      result = "'''";
      break;
    case msrOctaveKind::kOctave7:
      result = "''''";
      break;
    case msrOctaveKind::kOctave8:
      result = "'''''";
      break;
    case msrOctaveKind::kOctave9:
      result = "''''''";
      break;
  } // switch

  return result;
}

//________________________________________________________________________
std::string lpsr2lilypondTranslator::alterationKindAsLilypondString (
  msrAlterationKind alterationKind)
{
  std::string result;

  switch (alterationKind) {
    case msrAlterationKind::kAlterationTripleFlat:
      result = "TRIPLE-FLAT";
      break;
    case msrAlterationKind::kAlterationDoubleFlat:
      result = "DOUBLE-FLAT";
      break;
    case msrAlterationKind::kAlterationSesquiFlat:
      result = "THREE-Q-FLAT";
      break;
    case msrAlterationKind::kAlterationFlat:
      result = "FLAT";
      break;
    case msrAlterationKind::kAlterationSemiFlat:
      result = "SEMI-FLAT";
      break;
    case msrAlterationKind::kAlterationNatural:
      result = "NATURAL";
      break;
    case msrAlterationKind::kAlterationSemiSharp:
      result = "SEMI-SHARP";
      break;
    case msrAlterationKind::kAlterationSharp:
      result = "SHARP";
      break;
    case msrAlterationKind::kAlterationSesquiSharp:
      result = "THREE-Q-SHARP";
      break;
    case msrAlterationKind::kAlterationDoubleSharp:
      result = "DOUBLE-SHARP";
      break;
    case msrAlterationKind::kAlterationTripleSharp:
      result = "TRIPLE-SHARP";
      break;
    case msrAlterationKind::kAlteration_UNKNOWN_:
      result = "kAlteration_UNKNOWN_";
      break;
  } // switch

  return result;
}

//________________________________________________________________________
std::string lpsr2lilypondTranslator::lilypondOctaveInRelativeEntryMode (
  const S_msrNote& note)
{
  // generate LilyPond octave relative to fCurrentOctaveEntryReference

  // in MusicXML, octave number is 4 for the octave starting with middle C
  msrOctaveKind
    noteAbsoluteOctave =
      note->getNoteOctaveKind ();

  msrDiatonicPitchKind
    noteDiatonicPitchKind =
      note->
        noteDiatonicPitchKind (
          note->getInputLineNumber ());

  msrDiatonicPitchKind
    referenceDiatonicPitchKind =
      fCurrentOctaveEntryReference->
        noteDiatonicPitchKind (
          note->getInputLineNumber ());

  std::string
    referenceDiatonicPitchKindAsString =
      fCurrentOctaveEntryReference->
        noteDiatonicPitchKindAsString (
          note->getInputLineNumber ());

  msrOctaveKind
    referenceAbsoluteOctave =
      fCurrentOctaveEntryReference->
        getNoteOctaveKind ();

  /*
    If no octave changing mark is used on a pitch, its octave is calculated
    so that the interval with the previous note is less than a fifth.
    This interval is determined without considering accidentals.
  */

  int
    noteAboluteDiatonicOrdinal =
      (int) noteAbsoluteOctave * 7 // JMI FOO
        +
      (int) noteDiatonicPitchKind - (int) msrDiatonicPitchKind::kDiatonicPitchC,

    referenceAboluteDiatonicOrdinal =
      (int) referenceAbsoluteOctave * 7 // JMI FOO
        +
      (int) referenceDiatonicPitchKind - (int) msrDiatonicPitchKind::kDiatonicPitchC;

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceNotesOctaveEntry ()) {
    int fieldWidth = 28;

    std::stringstream ss;

    ss << std::left <<
      "lilypondOctaveInRelativeEntryMode() 1" <<
      std::endl <<

      std::setw (fieldWidth) <<
      "% noteAboluteDiatonicOrdinal: " <<
      noteAboluteDiatonicOrdinal <<
      std::endl <<

      std::setw (fieldWidth) <<
      "% referenceDiatonicPitchAsString: " <<
      referenceDiatonicPitchKindAsString <<
      std::endl <<
      std::setw (fieldWidth) <<
      "% referenceAbsoluteOctave: " <<
      msrOctaveKindAsString (referenceAbsoluteOctave) <<
      std::endl <<
      std::setw (fieldWidth) <<
      "% referenceAboluteDiatonicOrdinal: " <<
      referenceAboluteDiatonicOrdinal <<
      std::endl << std::endl;

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  std::stringstream ss;

  // generate the octaves as needed
  if (noteAboluteDiatonicOrdinal >= referenceAboluteDiatonicOrdinal) {
    noteAboluteDiatonicOrdinal -= 4;
    while (noteAboluteDiatonicOrdinal >= referenceAboluteDiatonicOrdinal) {
      ss << "'";
      noteAboluteDiatonicOrdinal -= 7;
    } // while
  }

  else {
    noteAboluteDiatonicOrdinal += 4;
    while (noteAboluteDiatonicOrdinal <= referenceAboluteDiatonicOrdinal) {
      ss << ",";
      noteAboluteDiatonicOrdinal += 7;
    } // while
  }

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceNotesOctaveEntry ()) {
    std::stringstream ss;

    ss <<
      "lilypondOctaveInRelativeEntryMode() 2" <<
      ", result: " << ss.str () <<
      std::endl << std::endl;

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  return ss.str ();
}

//________________________________________________________________________
std::string lpsr2lilypondTranslator::lilypondOctaveInFixedEntryMode (
  const S_msrNote& note)
{
  // generate LilyPond octave relative to fCurrentOctaveEntryReference

  // in MusicXML, octave number is 4 for the octave starting with middle C

  /*
    Pitches in fixed octave entry mode only need commas or quotes
    when they are above or below fCurrentOctaveEntryReference,
    hence when their octave is different than the latter's.
  */

  msrOctaveKind
    noteAbsoluteOctave =
      note->getNoteOctaveKind ();

  msrOctaveKind
    referenceAbsoluteOctave =
      fCurrentOctaveEntryReference->
        getNoteOctaveKind ();

  int absoluteOctavesDifference =
    (int) noteAbsoluteOctave - (int) referenceAbsoluteOctave;

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceNotesOctaveEntry ()) {
    std::stringstream ss;

    ss <<
      "% noteAbsoluteOctave: " <<
      noteAbsoluteOctave <<
      ", referenceAbsoluteOctave: " <<
      referenceAbsoluteOctave <<
      ", referenceAbsoluteOctave: " <<
      absoluteOctavesDifference;

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  std::stringstream ss;

  // generate the octaves as needed
  switch (absoluteOctavesDifference) {
    case -12:
      ss << ",,,,,,,,,,,,";
      break;
    case -11:
      ss << ",,,,,,,,,,,";
      break;
    case -10:
      ss << ",,,,,,,,,,";
      break;
    case -9:
      ss << ",,,,,,,,,";
      break;
    case -8:
      ss << ",,,,,,,,";
      break;
    case -7:
      ss << ",,,,,,,";
      break;
    case -6:
      ss << ",,,,,,";
      break;
    case -5:
      ss << ",,,,,";
      break;
    case -4:
      ss << ",,,,";
      break;
    case -3:
      ss << ",,,";
      break;
    case -2:
      ss << ",,";
      break;
    case -1:
      ss << ",";
      break;

    case 0:
      break;

    case 1:
      ss << "'";
      break;
    case 2:
      ss << "''";
      break;
    case 3:
      ss << "'''";
      break;
    case 4:
      ss << "''''";
      break;
    case 5:
      ss << "'''''";
      break;
    case 6:
      ss << "''''''";
      break;
    case 7:
      ss << "'''''''";
      break;
    case 8:
      ss << "''''''''";
      break;
    case 9:
      ss << "'''''''''";
      break;
    case 10:
      ss << "''''''''''";
      break;
    case 11:
      ss << "'''''''''''";
      break;
    case 12:
      ss << "''''''''''''";
      break;
    default:
      ss << "!!!";
  } // switch

  return ss.str ();
}

//________________________________________________________________________
std::string lpsr2lilypondTranslator::stringTuningAsLilypondString (
  const mfInputLineNumber& inputLineNumber,
  const S_msrStringTuning& stringTuning)
{
  msrDiatonicPitchKind
    stringTuningDiatonicPitchKind =
      stringTuning->
        getStringTuningDiatonicPitchKind ();

  msrAlterationKind
    stringTuningAlterationKind =
      stringTuning->
       getStringTuningAlterationKind ();

  msrOctaveKind
    stringTuningOctave =
      stringTuning->
       getStringTuningOctaveKind ();

  // compute the quartertones pitch
  msrQuarterTonesPitchKind
    quarterTonesPitchKind =
      quarterTonesPitchKindFromDiatonicPitchAndAlteration (
        inputLineNumber,
        stringTuningDiatonicPitchKind,
        stringTuningAlterationKind);

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceScordaturas ()) {
    int
      getStringTuningNumber =
        stringTuning->
          getStringTuningNumber ();

    std::stringstream ss;

    ss <<
      std::endl <<
      "%getStringTuningNumber: " <<
      getStringTuningNumber <<
      std::endl <<
      "%stringTuningDiatonicPitchKind: " <<
      msrDiatonicPitchKindAsString (
        stringTuningDiatonicPitchKind) <<
      std::endl <<
      "%stringTuningAlterationKind: " <<
      alterationKindAsLilypondString (
        stringTuningAlterationKind) <<
      std::endl <<
      "%stringTuningOctave: " <<
      msrOctaveKindAsString (stringTuningOctave) <<
      std::endl <<
      "%quarterTonesPitchKind: " <<
      msrQuarterTonesPitchKindAsString (quarterTonesPitchKind) <<
      std::endl <<
      "%msrQuarterTonesPitchKindAsString: " <<
      msrQuarterTonesPitchKindAsStringInLanguage (
        quarterTonesPitchKind,
        gLpsrOahGroup->
          getLpsrQuarterTonesPitchesLanguageKind ()) <<
      std::endl << std::endl;

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  std::stringstream ss;

  ss <<
    msrQuarterTonesPitchKindAsStringInLanguage (
      quarterTonesPitchKind,
      gLpsrOahGroup->
        getLpsrQuarterTonesPitchesLanguageKind ()) <<

    absoluteOctaveAsLilypondString (
      inputLineNumber,
      stringTuningOctave);

  return ss.str ();
}

//________________________________________________________________________
std::string lpsr2lilypondTranslator::notePitchAsLilypondString (
  const S_msrNote& note)
{
  std::stringstream ss;

  // should an editorial accidental be generated?
  switch (note->getNoteEditorialAccidentalKind ()) {
    case msrEditorialAccidentalKind::kEditorialAccidentalYes:
      ss <<
        "\\editorialAccidental ";
      break;
    case msrEditorialAccidentalKind::kEditorialAccidentalNo:
      break;
  } // switch

  // get the note quarter tones pitch
  msrQuarterTonesPitchKind
    noteQuarterTonesPitchKind =
      note->
        getNoteQuarterTonesPitchKind ();

  // fetch the quarter tones pitch as string
  std::string
    noteQuarterTonesPitchKindAsString =
      msrQuarterTonesPitchKindAsStringInLanguage (
        noteQuarterTonesPitchKind,
        gLpsrOahGroup->
          getLpsrQuarterTonesPitchesLanguageKind ());

  // get the note quarter tones display pitch
  msrQuarterTonesPitchKind
    noteQuarterTonesDisplayPitchKind =
      note->
        getNoteQuarterTonesDisplayPitchKind ();

  // fetch the quarter tones display pitch as string
  std::string
    quarterTonesDisplayPitchKindAsString =
      msrQuarterTonesPitchKindAsStringInLanguage (
        noteQuarterTonesDisplayPitchKind,
        gLpsrOahGroup->
          getLpsrQuarterTonesPitchesLanguageKind ());

  // generate the pitch
  ss <<
    noteQuarterTonesPitchKindAsString;

  // in MusicXML, octave number is 4 for the octave
  // starting with middle C, LilyPond's c'
  msrOctaveKind
    noteAbsoluteOctave =
      note->getNoteOctaveKind ();

  // should an absolute octave be generated?
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceNotesOctaveEntry ()) {
    msrOctaveKind
      noteAbsoluteDisplayOctave =
        note->getNoteDisplayOctaveKind ();

    int fieldWidth = 39;

    std::stringstream ss;

    ss << std::left <<
      "notePitchAsLilypondString() 1" <<
      std::endl <<

      std::setw (fieldWidth) <<
      "% noteQuarterTonesPitchKindAsString: " <<
      noteQuarterTonesPitchKindAsString <<
      std::endl <<
      std::setw (fieldWidth) <<
      "% quarterTonesDisplayPitchKindAsString: " <<
      quarterTonesDisplayPitchKindAsString <<
      std::endl <<

      std::setw (fieldWidth) <<
      "% noteAbsoluteOctave: " <<
      msrOctaveKindAsString (noteAbsoluteOctave) <<
      std::endl <<
      std::setw (fieldWidth) <<
      "% noteAbsoluteDisplayOctave: " <<
      msrOctaveKindAsString (noteAbsoluteDisplayOctave) <<
      std::endl <<

      std::setw (fieldWidth) <<
      "% line " <<
      note->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  switch (gGlobalLpsr2lilypondOahGroup->fetchOctaveEntryVariableValue ()) {
    case msrOctaveEntryKind::kOctaveEntryRelative:
      if (! fCurrentOctaveEntryReference) {
#ifdef MF_TRACE_IS_ENABLED
        if (gTraceOahGroup->getTraceNotesOctaveEntry ()) {
          std::stringstream ss;

          ss <<
            "notePitchAsLilypondString() 2: fCurrentOctaveEntryReference is NULL" <<
            " upon note " << note->asString () <<
            ", line " << note->getInputLineNumber ();

          gWaeHandler->waeTrace (
            __FILE__, mfInputLineNumber (__LINE__),
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED

        // generate absolute octave
        ss <<
          absoluteOctaveAsLilypondString (
            note->getInputLineNumber (),
            noteAbsoluteOctave);

        // fCurrentOctaveEntryReference will be set to note later
      }
      else {
#ifdef MF_TRACE_IS_ENABLED
        if (gTraceOahGroup->getTraceNotesOctaveEntry ()) {
          std::stringstream ss;

          ss <<
            "notePitchAsLilypondString() 3: fCurrentOctaveEntryReference is '" <<
            fCurrentOctaveEntryReference->asString () <<
            "' upon note " << note->asString () <<
            ", line " << note->getInputLineNumber ();

          gWaeHandler->waeTrace (
            __FILE__, mfInputLineNumber (__LINE__),
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED

        // generate octave relative to mobile fCurrentOctaveEntryReference
        ss <<
          lilypondOctaveInRelativeEntryMode (note);
      }
      break;

    case msrOctaveEntryKind::kOctaveEntryAbsolute:
      // generate LilyPond absolute octave
      ss <<
        absoluteOctaveAsLilypondString (
          note->getInputLineNumber (),
          noteAbsoluteOctave);
      break;

    case msrOctaveEntryKind::kOctaveEntryFixed:
      // generate octave relative to fixed fCurrentOctaveEntryReference
      ss <<
        lilypondOctaveInFixedEntryMode (note);
      break;
  } // switch

  // should an accidental be generated? JMI this can be fine tuned with cautionary
  switch (note->getNoteAccidentalKind ()) {
    case msrAccidentalKind::kAccidentalNone:
      break;
    default:
      ss <<
        "!";
      break;
  } // switch

  // should an editorial accidental be generated?
  switch (note->getNoteEditorialAccidentalKind ()) {
    case msrEditorialAccidentalKind::kEditorialAccidentalYes:
      ss <<
        "?";
      break;
    case msrEditorialAccidentalKind::kEditorialAccidentalNo:
      break;
  } // switch

  // should a cautionary accidental be generated?
  switch (note->getNoteCautionaryAccidentalKind ()) {
    case msrCautionaryAccidentalKind::kCautionaryAccidentalYes:
      ss <<
        "?";
      break;
    case msrCautionaryAccidentalKind::kCautionaryAccidentalNo:
      break;
  } // switch

// #ifdef MF_TRACE_IS_ENABLED
//   if (gTraceOahGroup->getTraceNotesOctaveEntry ()) {
//     gLog << std::endl;
//   }
// #endif // MF_TRACE_IS_ENABLED

  return ss.str ();
}

//________________________________________________________________________
std::string lpsr2lilypondTranslator::durationAsLilypondStringIfItShouldBeGenerated (
  const mfInputLineNumber& inputLineNumber,
  const mfWholeNotes& wholeNotes)
{
  std::string result;

  Bool doGenerateExplicitNotesDuration (false);

  // should the duration be generated in all cases?
  if (gGlobalLpsr2lilypondOahGroup->getAllNotesDurations ()) {
    // yes
    doGenerateExplicitNotesDuration = true;
  }

  else {
    // let's see...
    Bool
      wholeNotesIsANewDuration =
        wholeNotes != fLastMetWholeNotes;

    if (wholeNotesIsANewDuration) {
      doGenerateExplicitNotesDuration = true;
      fLastMetWholeNotes = wholeNotes;
    }
    // else don't generate the duration
  }

  if (doGenerateExplicitNotesDuration) {
    result =
      wholeNotesAsLilypondString (
        inputLineNumber,
        wholeNotes);
  }

  return result;
}

Bool lpsr2lilypondTranslator::wholeNotesDurationShouldBeGenerated (
  const mfWholeNotes& wholeNotes)
{
  Bool result;

  result =
    gGlobalLpsr2lilypondOahGroup->getAllNotesDurations ()
      ||
    wholeNotes != fLastMetWholeNotes;

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalLpsr2lilypondOahGroup->getCommentLilypondDurations ()) {
    fLilypondCodeStream <<
      std::endl <<
      "%{ " <<
      "fLastMetWholeNotes: " << fLastMetWholeNotes.asFractionString () <<
      ", wholeNotes: " << wholeNotes.asFractionString () <<
      ", wholeNotesDurationShouldBeGenerated(): " << result <<
      " %}" <<
      std::endl;
  }
#endif // MF_TRACE_IS_ENABLED

  return result;
}

void lpsr2lilypondTranslator::generateWholeNotesDuration (
  const mfInputLineNumber& inputLineNumber,
  const mfWholeNotes& wholeNotes)
{
  fLilypondCodeStream <<
    wholeNotesAsLilypondString (
      inputLineNumber,
      wholeNotes);

  fLastMetWholeNotes = wholeNotes;
}

void lpsr2lilypondTranslator::generateWholeNotesDurationOnStream (
  const mfInputLineNumber& inputLineNumber,
  const mfWholeNotes& wholeNotes,
  std::ostream&       os)
{
  os <<
    wholeNotesAsLilypondString (
      inputLineNumber,
      wholeNotes);

  fLastMetWholeNotes = wholeNotes;
}

std::string lpsr2lilypondTranslator::notesDurationKindAsLilypondString (
  mfDurationKind notesDurationKind)
{
  std::string result;

  switch (notesDurationKind) {
    case mfDurationKind::kDuration_UNKNOWN_:
      result = "noNotesDuration";
      break;

    case mfDurationKind::kDuration1024th:
      result = "1024";
      break;
    case mfDurationKind::kDuration512th:
      result = "512";
      break;
    case mfDurationKind::kDuration256th:
      result = "256";
      break;
    case mfDurationKind::kDuration128th:
      result = "128";
      break;
    case mfDurationKind::kDuration64th:
      result = "64";
      break;
    case mfDurationKind::kDuration32nd:
      result = "32";
      break;
    case mfDurationKind::kDuration16th:
      result = "16";
      break;
    case mfDurationKind::kDurationEighth:
      result = "8";
      break;
    case mfDurationKind::kDurationQuarter:
      result = "4";
      break;
    case mfDurationKind::kDurationHalf:
      result = "2";
      break;
    case mfDurationKind::kDurationWhole:
      result = "1";
      break;
    case mfDurationKind::kDurationBreve:
      result = "\\breve";
      break;
    case mfDurationKind::kDurationLonga:
      result = "\\longa";
      break;
    case mfDurationKind::kDurationMaxima:
      result = "\\maxima";
      break;
  } // switch

  return result;
}

//________________________________________________________________________
std::string lpsr2lilypondTranslator::pitchedRestAsLilypondString (
  const S_msrNote& note)
{
  mfInputLineNumber
    noteInputLineNumber =
      note->getInputLineNumber ();

  std::stringstream ss;

  // get the note quarter tones pitch
  msrQuarterTonesPitchKind
    noteQuarterTonesPitchKind =
      note->
        getNoteQuarterTonesPitchKind ();

  // fetch the quarter tones pitch as string
  std::string
    noteQuarterTonesPitchKindAsString =
      msrQuarterTonesPitchKindAsStringInLanguage (
        noteQuarterTonesPitchKind,
        gLpsrOahGroup->getLpsrQuarterTonesPitchesLanguageKind ());

  // get the note quarter tones display pitch
  msrQuarterTonesPitchKind
    noteQuarterTonesDisplayPitchKind =
      note->
        getNoteQuarterTonesDisplayPitchKind ();

  // fetch the quarter tones display pitch as string
  std::string
    quarterTonesDisplayPitchKindAsString =
      msrQuarterTonesPitchKindAsStringInLanguage (
        noteQuarterTonesDisplayPitchKind,
        gLpsrOahGroup->
          getLpsrQuarterTonesPitchesLanguageKind ());

  // generate the display pitch
  ss <<
    note->
      noteDisplayPitchKindAsString ();
//    note->notePitchAsString (); JMI
//    quarterTonesDisplayPitchAsString;

  // should an absolute octave be generated?
  msrOctaveKind
    noteAbsoluteDisplayOctave =
      note->getNoteDisplayOctaveKind ();

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceNotes ()) {
    // in MusicXML, octave number is 4 for the octave starting with middle C
    msrOctaveKind
      noteAbsoluteOctave =
        note->getNoteOctaveKind ();

    int fieldWidth = 28;

    gLog << std::left <<
      "pitchedRestAsLilypondString()" <<
      std::endl <<

      std::setw (fieldWidth) <<
      "% noteQuarterTonesPitchKindAsString: " <<
      noteQuarterTonesPitchKindAsString <<
      std::endl <<
      std::setw (fieldWidth) <<
      "% quarterTonesDisplayPitch: " <<
      quarterTonesDisplayPitchKindAsString <<
      std::endl <<

      std::setw (fieldWidth) <<
      "% noteAbsoluteOctave: " <<
      msrOctaveKindAsString (noteAbsoluteOctave) <<
      std::endl <<
      std::setw (fieldWidth) <<
      "% noteAbsoluteDisplayOctave: " <<
      msrOctaveKindAsString (noteAbsoluteDisplayOctave) <<
      std::endl <<

      std::setw (fieldWidth) <<
      "% line: " <<
      noteInputLineNumber <<
      std::endl;  // JMI 0.9.70
  }
#endif // MF_TRACE_IS_ENABLED

  switch (gGlobalLpsr2lilypondOahGroup->fetchOctaveEntryVariableValue ()) {
    case msrOctaveEntryKind::kOctaveEntryRelative:
    // generate LilyPond octave relative to fCurrentOctaveEntryReference
    ss <<
      lilypondOctaveInRelativeEntryMode (note);
      break;
    case msrOctaveEntryKind::kOctaveEntryAbsolute:
      // generate LilyPond absolute octave
      ss <<
        absoluteOctaveAsLilypondString (
          noteInputLineNumber,
          noteAbsoluteDisplayOctave);
      break;
    case msrOctaveEntryKind::kOctaveEntryFixed:
      // generate LilyPond octave relative to fCurrentOctaveEntryReference
      ss <<
        lilypondOctaveInFixedEntryMode (note);
      break;
  } // switch

  // generate the skip duration if relevant
//   ss <<
//     durationAsLilypondStringIfItShouldBeGenerated (
//       noteInputLineNumber,
//       note->getMeasureElementSoundingWholeNotes ());
    mfWholeNotes
      pitchedRestSoundingWholeNotes =
        note->getMeasureElementSoundingWholeNotes ();

    if (wholeNotesDurationShouldBeGenerated (pitchedRestSoundingWholeNotes)) {
      generateWholeNotesDurationOnStream (
        noteInputLineNumber,
        pitchedRestSoundingWholeNotes,
        ss);
    }

  // generate the '\rest'
  ss <<
    "\\rest ";

  return ss.str ();
}

void lpsr2lilypondTranslator::generateNoteHeadColor (
  const S_msrNote& note)
{
  // has the note alpha RGB color been set?
 if (note->getNoteColorAlphaRGBHasBenSet ()) {
    // get note alpha RGB color
    const msrColorAlphaRGB&
      noteColorAlphaRGB =
        note->getNoteColorAlphaRGB ();

    // get note RGB color
    std::string noteRGB = noteColorAlphaRGB.getColorRGB ();

    // generate RGB color
    if (noteRGB.size () == 6) {
      std::string
        noteR = noteRGB.substr (0, 2),
        noteG = noteRGB.substr (2, 2),
        noteB = noteRGB.substr (4, 2);

      fLilypondCodeStream <<
        "\\once \\override NoteHead.color = #(map (lambda (x) (/ x 255)) "
        "'(" <<
        "#X" << noteRGB [0] << noteRGB [1] <<
        cLilyPondSpace <<
        "#X" << noteRGB [2] << noteRGB [3] <<
        cLilyPondSpace <<
        "#X" << noteRGB [4] << noteRGB [5] <<
        "))" <<
        std::endl;
      }
    else {
      std::stringstream ss;

      ss <<
        "note RGB color '" <<
        noteRGB <<
        "' is ill-formed" <<
        ", line " << note->getInputLineNumber ();

      lpsr2lilypondInternalError (
        gServiceRunData->getInputSourceName (),
        note->getInputLineNumber (),
        __FILE__, mfInputLineNumber (__LINE__),
        ss.str ());
    }
  }
}

void lpsr2lilypondTranslator::generateNoteLigaturesList (
  const S_msrNote& note)
{
  const std::list <S_msrLigature>&
    noteLigaturesList =
      note->getNoteLigaturesList ();

  if (! noteLigaturesList.empty ()) {
    for (const S_msrLigature& ligature : noteLigaturesList) {
      switch (ligature->getLigatureKind ()) {
        case msrLigatureKind::kLigatureNone:
          break;

        case msrLigatureKind::kLigatureStart:
          {
            /*
              the edge height is relative to the voice,
              i.e. a positive value points down in voices 1 and 3
              and it points up in voices 2 and 4,
            */

            // fetch note's voice
            S_msrVoice
              noteVoice =
                note->
                  fetchNoteUpLinkToVoice ();

            // determine vertical flipping factor
            int ligatureVerticalFlippingFactor = 0;

            switch (noteVoice->getRegularVoiceStaffSequentialNumber ()) {
              case 1:
              case 3:
                ligatureVerticalFlippingFactor = 1;
                break;
              case 2:
              case 4:
                ligatureVerticalFlippingFactor = -1;
                break;
              default:
                ;
            } // switch

#ifdef MF_TRACE_IS_ENABLED
            if (gTraceOahGroup->getTraceLigatures ()) {
              std::stringstream ss;

              ss <<
                "Ligature vertical flipping factore for note '" <<
                note->asString () <<
                "' in voice \"" <<
                noteVoice->getVoicePathLikeName () <<
                "\" is " <<
                ligatureVerticalFlippingFactor <<
                ", line " << ligature->getInputLineNumber () <<
                std::endl;

              gWaeHandler->waeTrace (
                __FILE__, mfInputLineNumber (__LINE__),
                ss.str ());
            }
#endif // MF_TRACE_IS_ENABLED

            // compute ligature start edge height
            float edgeHeightAbsValue = 0.75;

            float ligatureStartEdgeHeight = 0.0;

            switch (ligature->getLigatureLineEndKind ()) {
              case msrLigatureLineEndKind::kLigatureLineEndUp:
                ligatureStartEdgeHeight =
                  - ligatureVerticalFlippingFactor * edgeHeightAbsValue;
                break;

              case msrLigatureLineEndKind::kLigatureLineEndDown:
                ligatureStartEdgeHeight =
                  ligatureVerticalFlippingFactor * edgeHeightAbsValue;
                break;

              case msrLigatureLineEndKind::kLigatureLineEndBoth: // JMI
                ligatureStartEdgeHeight =
                  - ligatureVerticalFlippingFactor * edgeHeightAbsValue;
                break;

              case msrLigatureLineEndKind::kLigatureLineEndArrow: // JMI
                fLilypondCodeStream <<
                  "%{ ligatureLineEndArrow??? %} ";
                break;

              case msrLigatureLineEndKind::kLigatureLineEndNone:
                ligatureStartEdgeHeight = 0;
                break;
            } // switch

            // fetch ligature's other end
            S_msrLigature
              ligatureOtherEnd =
                ligature->
                  getLigatureSideLinkToOtherEnd ();

            // compute ligature end edge height
            float ligatureEndEdgeHeight = 0.0;

            switch (ligatureOtherEnd->getLigatureLineEndKind ()) {
              case msrLigatureLineEndKind::kLigatureLineEndUp:
                ligatureEndEdgeHeight =
                  - ligatureVerticalFlippingFactor * edgeHeightAbsValue;
                break;

              case msrLigatureLineEndKind::kLigatureLineEndDown:
                ligatureEndEdgeHeight =
                  ligatureVerticalFlippingFactor * edgeHeightAbsValue;
                break;

              case msrLigatureLineEndKind::kLigatureLineEndBoth: // JMI
                ligatureEndEdgeHeight =
                  - ligatureVerticalFlippingFactor * edgeHeightAbsValue;
                break;

              case msrLigatureLineEndKind::kLigatureLineEndArrow: // JMI
                fLilypondCodeStream <<
                  "%{ ligatureLineEndArrow??? %} ";
                break;

              case msrLigatureLineEndKind::kLigatureLineEndNone:
                ligatureEndEdgeHeight = 0;
                break;
            } // switch

            // generate the code the edge-height pair of values
            fLilypondCodeStream <<
              std::endl <<
              "\\once \\override Staff.LigatureBracket.edge-height = #'(" <<
              std::setprecision (2) <<
              ligatureStartEdgeHeight <<
              " . " <<
              std::setprecision (2) <<
              ligatureEndEdgeHeight <<
              ")";

            if (gGlobalLpsr2lilypondOahGroup->getCommentLilypondLigatures ()) {
              fLilypondCodeStream <<
                " %{ " <<
                ligature->getInputLineNumber () <<
                " %}" <<
                std::endl;
            }
          }

          // generate ligature line type if any
          switch (ligature->getLigatureLineTypeKind ()) {
            case msrLineTypeKind::kLineTypeSolid:
              break;
            case msrLineTypeKind::kLineTypeDashed:
              fLilypondCodeStream <<
                std::endl <<
                "\\once\\override LigatureBracket.style = #'dashed-line" <<
                std::endl;
              break;
            case msrLineTypeKind::kLineTypeDotted:
              fLilypondCodeStream <<
                std::endl <<
                "\\once\\override LigatureBracket.style = #'dotted-line" <<
                std::endl;
              break;
            case msrLineTypeKind::kLineTypeWavy:
              fLilypondCodeStream <<
                std::endl <<
                "\\once\\override LigatureBracket.style = #'zigzag" <<
                std::endl;
              break;
          } // switch

          fLilypondCodeStream << cLilypondLigatureOpener;
          break;

        case msrLigatureKind::kLigatureContinue:
          break;

        case msrLigatureKind::kLigatureStop:
   // JMI       fLilypondCodeStream << cLilypondLigatureCloser;
          break;
      } // switch
    } // for
  }
}

std::string lpsr2lilypondTranslator::stemAsLilypondString (
  msrStemKind stemKind)
{
  std::string result;

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceStems ()) {
    std::stringstream ss;

    ss <<
      "--> stemAsLilypondString() " <<
      ", stemKind: " <<
      stemKind;

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  switch (stemKind) {
    case msrStemKind::kStemKind_NONE:
      result = " %{ \\kStemKind_NONE %} ";
      break;
    case msrStemKind::kStemKindNeutral:
      result = "\\stemNeutral ";
      break;
    case msrStemKind::kStemKindUp:
      result = "\\stemUp ";
      break;
    case msrStemKind::kStemKindDown:
      result = "\\stemDown ";
      break;
    case msrStemKind::kStemKindDouble: // JMI ???
      break;
  } // switch

  return result;
}

void lpsr2lilypondTranslator::generateStemIfNeededAndUpdateCurrentStemKind (
  const S_msrStem& stem)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceStems ()) {
    std::stringstream ss;

    ss <<
      "--> generateStemIfNeededAndUpdateCurrentStemKind() 1" <<
      ", stem: " <<
      stem->asShortString () <<
      ", fCurrentStemKind: " <<
      fCurrentStemKind;

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (stem) {
    // should a stem direction command be generated?
    // otherwise, leave stems management to LilyPond
    msrStemKind
      stemKind =
        stem->getStemKind ();

    Bool doGenerateAStemDirection (false);

    if (
      gGlobalLpsr2lilypondOahGroup->getGenerateStemsDirections ()
    ) {
      doGenerateAStemDirection = true;
    }
    else {
      doGenerateAStemDirection =
        stemKind != fCurrentStemKind;
    }

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceStems ()) {
    std::stringstream ss;

    ss <<
      "--> generateStemIfNeededAndUpdateCurrentStemKind() 2" <<
      ", stem: " <<
      stem->asShortString () <<
      ", doGenerateAStemDirection: " <<
      doGenerateAStemDirection;

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

    if (doGenerateAStemDirection) {
      // JMI msrStemKind::kStemKindNeutral ??? JMI 0.9.72
#ifdef MF_TRACE_IS_ENABLED
      if (gTraceOahGroup->getTraceStems ()) {
        std::stringstream ss;

        ss <<
          "--> fCurrentStemKind switches from " <<
          fCurrentStemKind <<
          " to " <<
          stemKind <<
          ", line " << stem->getInputLineNumber ();

        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }
#endif // MF_TRACE_IS_ENABLED

      // generate the stem code
      fLilypondCodeStream <<
        stemAsLilypondString (stemKind);
    }

    // update fCurrentStemKind upon each stem occurrence
    fCurrentStemKind = stemKind;
  }
}

void lpsr2lilypondTranslator::generateNoteHeadSettingsBeforeNote (
  const S_msrNote& note)
{
  if (! note->fetchNoteIsARest ()) { // JMI ???
    msrNoteHeadKind
      noteHeadKind =
        note->getNoteHeadKind ();

    // these tweaks should occur right before the note itself
    switch (noteHeadKind) {
      case msrNoteHeadKind::kNoteHeadSlash:
        fLilypondCodeStream << "\\tweak style #'slash ";
        break;
      case msrNoteHeadKind::kNoteHeadTriangle:
        fLilypondCodeStream << "\\tweak style #'triangle ";
        break;
//       case msrNoteHeadKind::kNoteHeadDiamond: JMI 0.9.72
//    // JMI     fLilypondCodeStream << "\\tweak style #'diamond ";
//         fLilypondCodeStream << "\\harmonic ";
//         break;
      case msrNoteHeadKind::kNoteHeadSquare:
        fLilypondCodeStream << "\\tweak style #'la ";
        break;
      case msrNoteHeadKind::kNoteHeadCross:
        fLilypondCodeStream << "\\tweak style #'cross ";
        break;
      case msrNoteHeadKind::kNoteHeadX:
        fLilypondCodeStream << "\\tweak style #'cross ";
        break;
      case msrNoteHeadKind::kNoteHeadCircleX:
        fLilypondCodeStream << "\\tweak style #'xcircle ";
        break;
//       case msrNoteHeadKind::kNoteHeadInvertedTriangle:
//         fLilypondCodeStream << "%{ kNoteHeadInvertedTriangle %} ";
//         break;
//       case msrNoteHeadKind::kNoteHeadArrowDown:
//         fLilypondCodeStream << "%{ kNoteHeadArrowDown %} ";
//         break;
//       case msrNoteHeadKind::kNoteHeadArrowUp:
//         fLilypondCodeStream << "%{ kNoteHeadArrowUp %} ";
//         break;
//       case msrNoteHeadKind::kNoteHeadSlashed:
//         fLilypondCodeStream << "%{ kNoteHeadSlashed %} ";
//         break;
//       case msrNoteHeadKind::kNoteHeadBackSlashed:
//         fLilypondCodeStream << "%{ kNoteHeadBackSlashed %} ";
//         break;
//       case msrNoteHeadKind::kNoteHeadNormal:
//    // JMI     fLilypondCodeStream << "%{ msrNoteHeadKind::kNoteHeadNormal %} ";
//         break;
//       case msrNoteHeadKind::kNoteHeadCluster:
//         fLilypondCodeStream << "%{ kNoteHeadCluster %} ";
//         break;
//       case msrNoteHeadKind::kNoteHeadCircleDot:
//         fLilypondCodeStream << "%{ kNoteHeadCircleDot %} ";
//         break;
//       case msrNoteHeadKind::kNoteHeadLeftTriangle:
//         fLilypondCodeStream << "%{ kNoteHeadLeftTriangle %} ";
//         break;
//       case msrNoteHeadKind::kNoteHeadRectangle:
//         fLilypondCodeStream << "%{ kNoteHeadRectangle %} ";
//         break;
      case msrNoteHeadKind::kNoteHeadNone:
        fLilypondCodeStream << "\\once\\omit NoteHead ";
        break;
      case msrNoteHeadKind::kNoteHeadDo:
        fLilypondCodeStream << "\\tweak style #'do ";
        break;
      case msrNoteHeadKind::kNoteHeadRe:
        fLilypondCodeStream << "\\tweak style #'re ";
        break;
      case msrNoteHeadKind::kNoteHeadMi:
        fLilypondCodeStream << "\\tweak style #'mi ";
        break;
      case msrNoteHeadKind::kNoteHeadFa:
        fLilypondCodeStream << "\\tweak style #'fa ";
        break;
      case msrNoteHeadKind::kNoteHeadFaUp:
        fLilypondCodeStream << "\\tweak style #'triangle ";
        break;
      case msrNoteHeadKind::kNoteHeadSo:
        fLilypondCodeStream << "\\tweak style #'sol ";
        break;
      case msrNoteHeadKind::kNoteHeadLa:
        fLilypondCodeStream << "\\tweak style #'la ";
        break;
      case msrNoteHeadKind::kNoteHeadTi:
        fLilypondCodeStream << "\\tweak style #'ti ";
        break;

      default:
        ;
    } // switch
  }
}

void lpsr2lilypondTranslator::generateNoteHeadSettingsAfterNote (
  const S_msrNote& note)
{
  if (! note->fetchNoteIsARest ()) { // JMI ???
    msrNoteHeadKind
      noteHeadKind =
        note->getNoteHeadKind ();

    // these tweaks should occur right before the note itself
    switch (noteHeadKind) {
//       case msrNoteHeadKind::kNoteHeadSlash:
//         fLilypondCodeStream << "\\tweak style #'slash ";
//         break;
//       case msrNoteHeadKind::kNoteHeadTriangle:
//         fLilypondCodeStream << "\\tweak style #'triangle ";
//         break;
      case msrNoteHeadKind::kNoteHeadDiamond:
   // JMI     fLilypondCodeStream << "\\tweak style #'diamond ";
        fLilypondCodeStream << "\\harmonic ";
        break;
//       case msrNoteHeadKind::kNoteHeadSquare:
//         fLilypondCodeStream << "\\tweak style #'la ";
//         break;
//       case msrNoteHeadKind::kNoteHeadCross:
//         fLilypondCodeStream << "\\tweak style #'cross ";
//         break;
//       case msrNoteHeadKind::kNoteHeadX:
//         fLilypondCodeStream << "\\tweak style #'cross %{ x %} ";
//         break;
//       case msrNoteHeadKind::kNoteHeadCircleX:
//         fLilypondCodeStream << "\\tweak style #'xcircle ";
//         break;
      case msrNoteHeadKind::kNoteHeadInvertedTriangle:
        fLilypondCodeStream << "%{ kNoteHeadInvertedTriangle %} ";
        break;
      case msrNoteHeadKind::kNoteHeadArrowDown:
        fLilypondCodeStream << "%{ kNoteHeadArrowDown %} ";
        break;
      case msrNoteHeadKind::kNoteHeadArrowUp:
        fLilypondCodeStream << "%{ kNoteHeadArrowUp %} ";
        break;
      case msrNoteHeadKind::kNoteHeadSlashed:
        fLilypondCodeStream << "%{ kNoteHeadSlashed %} ";
        break;
      case msrNoteHeadKind::kNoteHeadBackSlashed:
        fLilypondCodeStream << "%{ kNoteHeadBackSlashed %} ";
        break;
      case msrNoteHeadKind::kNoteHeadNormal:
   // JMI     fLilypondCodeStream << "%{ msrNoteHeadKind::kNoteHeadNormal %} ";
        break;
      case msrNoteHeadKind::kNoteHeadCluster:
        fLilypondCodeStream << "%{ kNoteHeadCluster %} ";
        break;
      case msrNoteHeadKind::kNoteHeadCircleDot:
        fLilypondCodeStream << "%{ kNoteHeadCircleDot %} ";
        break;
      case msrNoteHeadKind::kNoteHeadLeftTriangle:
        fLilypondCodeStream << "%{ kNoteHeadLeftTriangle %} ";
        break;
      case msrNoteHeadKind::kNoteHeadRectangle:
        fLilypondCodeStream << "%{ kNoteHeadRectangle %} ";
        break;
//       case msrNoteHeadKind::kNoteHeadNone:
//         fLilypondCodeStream << "\\once\\omit NoteHead ";
//         break;
//       case msrNoteHeadKind::kNoteHeadDo:
//         fLilypondCodeStream << "\\tweak style #'do ";
//         break;
//       case msrNoteHeadKind::kNoteHeadRe:
//         fLilypondCodeStream << "\\tweak style #'re ";
//         break;
//       case msrNoteHeadKind::kNoteHeadMi:
//         fLilypondCodeStream << "\\tweak style #'mi ";
//         break;
//       case msrNoteHeadKind::kNoteHeadFa:
//         fLilypondCodeStream << "\\tweak style #'fa ";
//         break;
//       case msrNoteHeadKind::kNoteHeadFaUp:
//         fLilypondCodeStream << "\\tweak style #'triangle ";
//         break;
//       case msrNoteHeadKind::kNoteHeadSo:
//         fLilypondCodeStream << "\\tweak style #'sol ";
//         break;
//       case msrNoteHeadKind::kNoteHeadLa:
//         fLilypondCodeStream << "\\tweak style #'la ";
//         break;
//       case msrNoteHeadKind::kNoteHeadTi:
//         fLilypondCodeStream << "\\tweak style #'ti ";
//         break;

      default:
        ;
    } // switch
  }
}

void lpsr2lilypondTranslator::generateCoda (const S_msrCoda& coda)
{
  switch (coda->getCodaKind ()) {
    case msrCodaKind::kCodaFirst:
      // generate the coda
      fLilypondCodeStream <<
        "\\mark \\markup { \\musicglyph #\"scripts.coda\" }";

#ifdef MF_MAINTAINANCE_RUNS_ARE_ENABLED
      if (gWaeOahGroup->getMaintainanceRun ()) { // MAINTAINANCE_RUN
        fLilypondCodeStream << " %{ kCodaFirst %} ";
      }
#endif // MF_MAINTAINANCE_RUNS_ARE_ENABLED
      break;

    case msrCodaKind::kCodaSecond:
      // generate the coda
      fLilypondCodeStream <<
        "\\mark \\markup { \\musicglyph #\"scripts.coda\" }";

#ifdef MF_MAINTAINANCE_RUNS_ARE_ENABLED
      if (gWaeOahGroup->getMaintainanceRun ()) { // MAINTAINANCE_RUN
        fLilypondCodeStream << " %{ kCodaSecond %} ";
      }
#endif // MF_MAINTAINANCE_RUNS_ARE_ENABLED
      break;
  } // switch
  fLilypondCodeStream << std::endl;
}

void lpsr2lilypondTranslator::generateSegno (const S_msrSegno& segno)
{
  // generate the segno
  fLilypondCodeStream <<
    "\\mark \\markup { \\musicglyph #\"scripts.segno\" }" <<
    std::endl;
}

void lpsr2lilypondTranslator::generateCodeRightBeforeNote (
  const S_msrNote& note)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceNotesBasics ()) {
    std::stringstream ss;

    ss <<
      std::endl <<
      "% --> generateCodeRightBeforeNote()" <<
      ", note: " <<
      note->asString () <<
      ", line " << note->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());

    if (
      gGlobalLpsr2lilypondOahGroup->getGenerateLpsrVisitingInformation ()
    ) {
      fLilypondCodeStream << ss.str ();
    }
  }
#endif // MF_TRACE_IS_ENABLED

  if (! fOnGoingChord) { // JMI 0.9.66
    // generate the note codas if any
    const std::list <S_msrCoda>&
      noteCodasList =
        note->getNoteCodasList ();

    if (! noteCodasList.empty ()) {
      std::list <S_msrCoda>::const_iterator i;
      for (S_msrCoda coda : noteCodasList) {
        // generate only the second coda before the note
        switch (coda->getCodaKind ()) {
          case msrCodaKind::kCodaFirst:
            break;
          case msrCodaKind::kCodaSecond:
            // generate the coda
            generateCoda (coda);
            break;
        } // switch
      } // for
    }

    // generate the note segnos if any
    const std::list <S_msrSegno>&
      noteSegnosList =
        note->getNoteSegnosList ();

    if (! noteSegnosList.empty ()) {
      std::list <S_msrSegno>::const_iterator i;
      for (i = noteSegnosList.begin (); i != noteSegnosList.end (); ++i) {
        // JMI ??? 0.9.72
      } // for
    }
  }

  // generate the note head color
  generateNoteHeadColor (note);

  // generate the note ligatures if any
  const std::list <S_msrLigature>&
    noteLigaturesList =
      note->getNoteLigaturesList ();

  if (! noteLigaturesList.empty ()) {
    generateNoteLigaturesList (note);
  }


  // generate note stem kind if needed
  if (! fOnGoingChord) { // JMI 0.9.72 ???
    S_msrStem
      noteStem =
        note->getNoteStem ();

    if (noteStem) {
      switch (note->getNoteKind ()) {
        case msrNoteKind::kNote_UNKNOWN_:
          break;

        // in grace notes groups
        case msrNoteKind::kNoteRegularInGraceNotesGroup:
          // let LilyPond take care of the stem JMI 0.9.74
          break;

        default:
          generateStemIfNeededAndUpdateCurrentStemKind (noteStem);
      } // switch
    }
  }

  // generate the note slur direction if any,
  // unless the note is chord member
  Bool doGenerateSlurDirection (true);

  if (note->getNoteBelongsToAChord ()) {
     doGenerateSlurDirection = false;
  }
  if (doGenerateSlurDirection) {
    generateNoteSlurDirection (note);
  }

  // generating note head tweaks before note
  generateNoteHeadSettingsBeforeNote (note);

  // generate note spanners if any
  generateBeforeNoteSpannersListIfAny (note);
}

void lpsr2lilypondTranslator::generateTheNoteItself (
  const S_msrNote& note)
{
#ifdef MF_TRACE_IS_ENABLED
  {
    Bool
      doTraceNotes =
        gTraceOahGroup->getTraceNotesBasics (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (doTraceNotes) {
      std::stringstream ss;

      ss <<
        std::endl <<
        "% --> generateTheNoteItself()" <<
        ", note: " <<
        note->asString () <<
        ", line " << note->getInputLineNumber () <<
        std::endl;

      gWaeHandler->waeTrace (
        __FILE__, mfInputLineNumber (__LINE__),
        ss.str ());

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << ss.str ();
      }
    }
  }
#endif // MF_TRACE_IS_ENABLED

  // is this a solo note or rest?
  switch (note->getSoloNoteOrRestInStaffKind ()) {
    case msrSoloNoteOrRestInStaffKind::kSoloNoteOrRestInStaffYes:
      if (! fOnGoingChord) {
        fLilypondCodeStream << "\\once \\oneVoice ";
      }
      break;

    case msrSoloNoteOrRestInStaffKind::kSoloNoteOrRestInStaffPartially:
      {
        std::stringstream ss;

        ss <<
          "note " <<
          note->asShortString () <<
          "' has been finalized as being partially a solo note or rest, not supported yet" <<
          ", line " << note->getInputLineNumber ();

        lpsr2lilypondInternalError (
          gServiceRunData->getInputSourceName (),
          note->getInputLineNumber (),
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }
      break;

    case msrSoloNoteOrRestInStaffKind::kSoloNoteOrRestInStaffNo:
      break;
  } // switch


  if (gGlobalLpsr2lilypondOahGroup->getInputLineNumbers ()) {
    // generate information and line number as a comment
    fLilypondCodeStream <<
      " %{ line " <<
      note->getInputLineNumber () <<
      " --> %} ";
  }

  // generate the code for the note proper
  switch (note->getNoteKind ()) {
    case msrNoteKind::kNote_UNKNOWN_:
      break;

    // in measures
    case msrNoteKind::kNoteRegularInMeasure:
      generateRegularNoteInMeasure (note);
      break;

    case msrNoteKind::kNoteRestInMeasure:
      generateRestInMeasure (note);
      break;

    case msrNoteKind::kNoteSkipInMeasure:
      generateSkipInMeasure (note);
      break;

    case msrNoteKind::kNoteUnpitchedInMeasure:
      generateUnpitchedNoteInMeasure (note);
      break;

    // in chords
    case msrNoteKind::kNoteRegularInChord:
      generateRegularNoteInChord (note);
      {}
      break;

    // in tuplets
    case msrNoteKind::kNoteRegularInTuplet:
      generateRegularNoteInTuplet (note);
      break;

    case msrNoteKind::kNoteRestInTuplet:
      generateRestInTuplet (note);
      break;

    case msrNoteKind::kNoteUnpitchedInTuplet:
      generateNoteUnpitchedInTuplet (note);
      break;

    // in grace notes groups
    case msrNoteKind::kNoteRegularInGraceNotesGroup:
      generateRegularNoteInGraceNotesGroup (note);
      break;

    case msrNoteKind::kNoteRestInGraceNotesGroup:
      generateRestInGraceNotesGroup (note);
      break;

    case msrNoteKind::kNoteSkipInGraceNotesGroup:
      generateRestInGraceNotesGroup (note);
      break;

    // in chords in grace notes groups
    case msrNoteKind::kNoteInChordInGraceNotesGroup:
      generateNoteInChordInGraceNotesGroup (note);
      break;

    // in tuplets in grace notes groups
    case msrNoteKind::kNoteInTupletInGraceNotesGroup:
      generateNoteInTupletInGraceNotesGroup (note);
      break;

    // in double-tremolos
    case msrNoteKind::kNoteInDoubleTremolo:
      generateNoteInDoubleTremolo (note);
      break;
  } // switch

  fLilypondCodeStream << cLilyPondSpace;
}

void lpsr2lilypondTranslator::generateRegularNoteInMeasure (
  const S_msrNote& note)
{
#ifdef MF_TRACE_IS_ENABLED
  {
    Bool
      doTraceNotes =
        gTraceOahGroup->getTraceNotes (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (doTraceNotes || generateMsrVisitingInformation) {
      std::stringstream ss;

      ss <<
        std::endl <<
        "% --> generating code for noteRegularInMeasure " <<
        note->asString () <<
        ", line " << note->getInputLineNumber () <<
        std::endl;

      if (doTraceNotes) {
        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << ss.str ();
      }
    }
  }
#endif // MF_TRACE_IS_ENABLED

  // generate the note name
  fLilypondCodeStream <<
    notePitchAsLilypondString (note);

  // generate the note duration if relevant
  mfWholeNotes
    noteSoundingWholeNotes =
      note->getMeasureElementSoundingWholeNotes ();

  if (wholeNotesDurationShouldBeGenerated (noteSoundingWholeNotes)) {
    generateWholeNotesDuration (
      note->getInputLineNumber (),
      noteSoundingWholeNotes);
  }

  // handle delayed ornaments if any
  if (note->getNoteDelayedTurnOrnament ()) {
    // c2*2/3 ( s2*1/3\turn) JMI
    // we need the explicit duration in all cases,
    // regardless of gServiceRunData->getAllNotesDurations ()
    fLilypondCodeStream <<
    //* JMI TOO MUCH
      wholeNotesAsLilypondString (
        note->getInputLineNumber (),
        noteSoundingWholeNotes) <<
        //*/
      "*" <<
      gGlobalLpsr2lilypondOahGroup->getDelayedOrnamentsRational ();
  }

  // generate the ties if any
  const std::list <S_msrTie>& noteTiesList = note->getNoteTiesList ();

  if (! noteTiesList.empty ()) {
    for (S_msrTie noteTie : noteTiesList) {
      switch (noteTie->getTieKind ()) {
        case msrTieKind::kTieNone:
          break;
        case msrTieKind::kTieStart:
         fLilypondCodeStream << " ~ "; // JMI 0.9.72
          break;
        case msrTieKind::kTieContinue:
          break;
        case msrTieKind::kTieStop:
          break;
        case msrTieKind::kTieLetRing: // MusicXML 4.0
          fLilypondCodeStream << " \\laissezVibrer "; // JMI 0.9.72
          break;
      } // switch
    } // for
  }

  // this note is the new relative octave reference
  switch (gGlobalLpsr2lilypondOahGroup->fetchOctaveEntryVariableValue ()) {
    case msrOctaveEntryKind::kOctaveEntryRelative:
      fCurrentOctaveEntryReference = note;
      break;
    case msrOctaveEntryKind::kOctaveEntryAbsolute:
      break;
    case msrOctaveEntryKind::kOctaveEntryFixed:
      break;
  } // switch
}

void lpsr2lilypondTranslator::generatePitchedRestInMeasure (
  const S_msrNote& pitchedRest)
{
  fLilypondCodeStream <<
    pitchedRestAsLilypondString (pitchedRest);

  if (gGlobalLpsr2lilypondOahGroup->getInputLineNumbers ()) {
    // generate information and line number as a comment
    fLilypondCodeStream <<
      " %{ <-- line " <<
      pitchedRest->getInputLineNumber () <<
      " %} ";
  }

  if (gGlobalLpsr2lilypondOahGroup->getNotesComments ()) {
    // generate information and line number as a comment
    fLilypondCodeStream <<
      "%{ pitched rest %} ";
  }

  // this pitchedRest is the new relative octave reference
  // (the display quarter tone pitch and octave
  // have been copied to the pitchedRest octave in msrNote::msrNote(),
  // since the pitchedRest octave is used in relative code generation)
  switch (gGlobalLpsr2lilypondOahGroup->fetchOctaveEntryVariableValue ()) {
    case msrOctaveEntryKind::kOctaveEntryRelative:
      fCurrentOctaveEntryReference = pitchedRest;
      break;
    case msrOctaveEntryKind::kOctaveEntryAbsolute:
      break;
    case msrOctaveEntryKind::kOctaveEntryFixed:
      break;
  } // switch
}

void lpsr2lilypondTranslator::generateUnpitchedRestInMeasure (
  const S_msrNote& unpitchedRest)
{
  // get the unpitchedRest sounding whole notes
  mfWholeNotes
    unpitchedRestSoundingWholeNotes =
      unpitchedRest->getMeasureElementSoundingWholeNotes ();

  // get unpitchedRest's voice
  S_msrVoice
    noteVoice =
      unpitchedRest->
        fetchNoteUpLinkToVoice ();

  if (unpitchedRest->getNoteOccupiesAFullMeasure ()) {
    // unpitchedRest occupies a full measure
    // generate the unpitchedRest name and its whole notes duration if relevant
    // take voice kind into account shouldn't be necessary? JMI 0.9.69
    switch (noteVoice->getVoiceKind ()) {
      case msrVoiceKind::kVoiceKindRegular:
      case msrVoiceKind::kVoiceKindDynamics:
        fLilypondCodeStream <<
          'R';
        break;

      case msrVoiceKind::kVoiceKindHarmonies:
      case msrVoiceKind::kVoiceKindFiguredBass:
        fLilypondCodeStream << 's';
        if (gGlobalLpsr2lilypondOahGroup->getCommentLilypondRestNotes ()) {
          fLilypondCodeStream << " %{ s111 %} ";
        }
        break;
    } // switch

    // generate its whole notes duration if relevant JMI ALWAYS ??? 0.9.70
//       fLilypondCodeStream <<
//         durationAsLilypondStringIfItShouldBeGenerated (
//           unpitchedRest->getInputLineNumber (),
//           unpitchedRestSoundingWholeNotes);
    if (wholeNotesDurationShouldBeGenerated (unpitchedRestSoundingWholeNotes)) {
      generateWholeNotesDuration (
        unpitchedRest->getInputLineNumber (),
        unpitchedRestSoundingWholeNotes);
    }

    if (gGlobalLpsr2lilypondOahGroup->getInputLineNumbers ()) {
      // generate information and line number as a comment
      fLilypondCodeStream <<
        " %{ <-- line " <<
        unpitchedRest->getInputLineNumber () <<
        " %} ";
    }

    if (gGlobalLpsr2lilypondOahGroup->getNotesComments ()) {
      // generate information and line number as a comment
      fLilypondCodeStream <<
        " %{ full measure unpitched rest %} ";
    }
  }

  else {
    // unpitchedRest does not occupy a full measure
    // take voice kind into account JMI shouldn't be necessary? JMI 0.9.70
    msrPrintObjectKind
      notePrintObjectKind =
        unpitchedRest->getNotePrintObjectKind ();

    // generate the unpitchedRest name and its whole notes duration if relevant
    switch (notePrintObjectKind) {
      case msrPrintObjectKind::kPrintObjectYes:
        fLilypondCodeStream <<
          'r';
        break;
      case msrPrintObjectKind::kPrintObjectNo:
        fLilypondCodeStream <<
          "\\once\\omit Rest r";
        break;
    } // switch

    // generate the unpitchedRest whole notes duration if relevant
//       fLilypondCodeStream <<
//         durationAsLilypondStringIfItShouldBeGenerated (
//           unpitchedRest->getInputLineNumber (),
//           unpitchedRestSoundingWholeNotes);
    mfWholeNotes
      restSoundingWholeNotes =
        unpitchedRest->getMeasureElementSoundingWholeNotes ();

    if (wholeNotesDurationShouldBeGenerated (restSoundingWholeNotes)) {
      generateWholeNotesDuration (
        unpitchedRest->getInputLineNumber (),
        restSoundingWholeNotes);
    }

    if (gGlobalLpsr2lilypondOahGroup->getInputLineNumbers ()) {
      // generate information and line number as a comment
      fLilypondCodeStream <<
        " %{ <-- line " <<
        unpitchedRest->getInputLineNumber () <<
        " %} ";
    }

    if (gGlobalLpsr2lilypondOahGroup->getNotesComments ()) {
      // generate information and line number as a comment
      fLilypondCodeStream <<
        " %{ non-full measure unpitched rest %} ";
    }

/* JMI BOF
    if (fOnGoingVoiceCadenza) { // JMI
      if (noteSoundingWholeNotes != mfWholeNotes (1, 1)) {
        / * JMI
        // force the generation of the duration if needed
        if (! gGlobalLpsr2lilypondOahGroup->getAllNotesDurations ()) {
          fLilypondCodeStream << // JMI
            wholeNotesAsLilypondString (
              unpitchedRest->getInputLineNumber (),
              noteSoundingWholeNotes);
        }
        * /

        // generate the multiplying factor
        fLilypondCodeStream << // JMI
          "*" <<
          noteSoundingWholeNotes <<
          "";
      }
    }
          */
  }

  // an unpitched rest is no relative octave reference, // JMI ??? 0.9.72
  // the preceding one is kept
}

void lpsr2lilypondTranslator::generateRestInMeasure (
  const S_msrNote& rest)
{
#ifdef MF_TRACE_IS_ENABLED
  {
    Bool
      doTraceNotes =
        gTraceOahGroup->getTraceNotes (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (doTraceNotes || generateMsrVisitingInformation) {
      std::stringstream ss;

      ss <<
        std::endl <<
        "% --> generating code for noteRestInMeasure " <<
        rest->asString () <<
        ", line " << rest->getInputLineNumber () <<
        std::endl;

      if (doTraceNotes) {
        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << ss.str ();
      }
    }
  }
#endif // MF_TRACE_IS_ENABLED

  // get pitched rest status
  Bool
    noteIsAPitchedRest =
      rest->noteIsAPitchedRest ();

  if (noteIsAPitchedRest) {
    // pitched rest
    generatePitchedRestInMeasure (rest);
  }

  else {
    // unpitched rest
    generateUnpitchedRestInMeasure (rest);
  }
}

void lpsr2lilypondTranslator::generateSkipInMeasure (
  const S_msrNote& note)
{
#ifdef MF_TRACE_IS_ENABLED
  {
    Bool
      doTraceNotes =
        gTraceOahGroup->getTraceNotes (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (doTraceNotes || generateMsrVisitingInformation) {
      std::stringstream ss;

      ss <<
        std::endl <<
        "% --> generating code for noteSkipInMeasure " <<
        note->asString () <<
        ", line " << note->getInputLineNumber () <<
        std::endl;

      if (doTraceNotes) {
        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << ss.str ();
      }
    }
  }
#endif // MF_TRACE_IS_ENABLED

  if (gGlobalLpsr2lilypondOahGroup->getGeneratePositionInMeasures ()) {
    // generate a rest to help pin-point bugs
    fLilypondCodeStream << 'r';

#ifdef MF_MAINTAINANCE_RUNS_ARE_ENABLED
    if (gWaeOahGroup->getMaintainanceRun ()) { // MAINTAINANCE_RUN
      fLilypondCodeStream <<
        " %{ r244 %} ";
    }
#endif // MF_MAINTAINANCE_RUNS_ARE_ENABLED
  }
  else {
    // generate a skip
    fLilypondCodeStream << 's';
    if (gGlobalLpsr2lilypondOahGroup->getCommentLilypondSkipNotes ()) {
      fLilypondCodeStream << " %{ s222 %} ";
    }

#ifdef MF_MAINTAINANCE_RUNS_ARE_ENABLED
    if (gWaeOahGroup->getMaintainanceRun ()) { // MAINTAINANCE_RUN
      fLilypondCodeStream <<
      " %{ s447 %} ";
    }
#endif // MF_MAINTAINANCE_RUNS_ARE_ENABLED
  }

  // generate the skip duration if relevant
//   fLilypondCodeStream <<
//     durationAsLilypondStringIfItShouldBeGenerated (
//       note->getInputLineNumber (),
//       note->getMeasureElementSoundingWholeNotes ());
  mfWholeNotes
    skipSoundingWholeNotes =
      note->getMeasureElementSoundingWholeNotes ();

  if (wholeNotesDurationShouldBeGenerated (skipSoundingWholeNotes)) {
    generateWholeNotesDuration (
      note->getInputLineNumber (),
      skipSoundingWholeNotes);
  }

  if (gGlobalLpsr2lilypondOahGroup->getInputLineNumbers ()) {
    // generate information and line number as a comment
    fLilypondCodeStream <<
      " %{ <-- line " <<
      note->getInputLineNumber () <<
      " %} ";
  }

  if (gGlobalLpsr2lilypondOahGroup->getNotesComments ()) {
    // generate information and line number as a comment
    fLilypondCodeStream <<
      " %{ skip note in measure %} ";
  }

  // a skip is no relative octave reference,
  // thus the preceding one is kept
}

void lpsr2lilypondTranslator::generateUnpitchedNoteInMeasure (
  const S_msrNote& note)
{
#ifdef MF_TRACE_IS_ENABLED
  {
    Bool
      doTraceNotes =
        gTraceOahGroup->getTraceNotes (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (doTraceNotes || generateMsrVisitingInformation) {
      std::stringstream ss;

      ss <<
        std::endl <<
        "% --> generating code for noteUnpitchedInMeasure " <<
        note->asString () <<
        ", line " << note->getInputLineNumber () <<
        std::endl;

      if (doTraceNotes) {
        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << ss.str ();
      }
    }
  }
#endif // MF_TRACE_IS_ENABLED

  // generate the note name, "e" by convention
  fLilypondCodeStream <<
      "e";

  mfWholeNotes
    noteSoundingWholeNotes =
      note->getMeasureElementSoundingWholeNotes ();

  // generate the unpitched note duration if relevant
//   fLilypondCodeStream <<
//     durationAsLilypondStringIfItShouldBeGenerated (
//       note->getInputLineNumber (),
//       noteSoundingWholeNotes);
  if (wholeNotesDurationShouldBeGenerated (noteSoundingWholeNotes)) {
    generateWholeNotesDuration (
      note->getInputLineNumber (),
      noteSoundingWholeNotes);
  }

  // handle delayed ornaments if any
  if (note->getNoteDelayedTurnOrnament ()) {
    // c2*2/3 ( s2*1/3\turn) JMI
    // we need the explicit duration in all cases,
    // regardless of gServiceRunData->getAllNotesDurations ()
    fLilypondCodeStream <<
      wholeNotesAsLilypondString (
        note->getInputLineNumber (),
        noteSoundingWholeNotes) <<
      "*" <<
      gGlobalLpsr2lilypondOahGroup->getDelayedOrnamentsRational ();
  }

/* JMI
  // generate the tie if any
  {
    const std::list <S_msrTie>& noteTiesList = note->getNoteTiesList ();

    if (! noteTiesList.empty ()) {
      if (noteTie->getTieKind () == msrTieKind::kTieStart) {
        fLilypondCodeStream <<
          " %{ line " << note->getInputLineNumber () << "%} " <<
          " ~  %{ kUnpitchedNote %} "; // JMI spaces???
      }
    }
  }
  */
}

void lpsr2lilypondTranslator::generateCuedNoteInMeasure (
  const S_msrNote& note)
{
#ifdef MF_TRACE_IS_ENABLED
  {
    Bool
      doTraceNotes =
        gTraceOahGroup->getTraceNotes (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (doTraceNotes || generateMsrVisitingInformation) {
    std::stringstream ss;

    ss <<
      std::endl <<
      "% --> generating code for noteUnpitchedInMeasure " <<
      note->asString () <<
      ", line " << note->getInputLineNumber () <<
      std::endl;

      if (doTraceNotes) {
        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << ss.str ();
      }
    }
  }
#endif // MF_TRACE_IS_ENABLED

  // generate the note name, "e" by convention
  fLilypondCodeStream <<
      "e CUE_CUE ";

  mfWholeNotes
    noteSoundingWholeNotes =
      note->getMeasureElementSoundingWholeNotes ();

  std::string result; // JMI 0.9.71 CUE_CUE

//   switch (note->getNoteSizeTypeKind ()) {  // JMI 0.9.71 CUE_CUE
//     case msrNoteSizeTypeKind::kNoteSizeType_UNKNOWN_:
//       result = "kNoteSizeType_UNKNOWN_";
//       break;
//     case msrNoteSizeTypeKind::kNoteSizeTypeFull:
//       result = "kNoteSizeTypeFull";
//       break;
//     case msrNoteSizeTypeKind::kNoteSizeTypeGrace:
//       result = "kNoteSizeTypeGrace";
//       break;
//     case msrNoteSizeTypeKind::kNoteSizeTypeCue:
//       result = "kNoteSizeTypeCue";
//       break;
//     case msrNoteSizeTypeKind::kNoteSizeTypeGraceCue:
//       result = "kNoteSizeTypeGraceCue";
//       break;
//     case msrNoteSizeTypeKind::kNoteSizeTypeLarge:
//       result = "kNoteSizeTypeLarge";
//       break;
//   } // switch

  fLilypondCodeStream <<
    result <<
    cLilyPondSpace;
}

void lpsr2lilypondTranslator::generateRegularNoteInChord (
  const S_msrNote& note)
{
#ifdef MF_TRACE_IS_ENABLED
  {
    Bool
      doTraceNotes = // true ||
        gTraceOahGroup->getTraceNotes (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (doTraceNotes || generateMsrVisitingInformation) {
      std::stringstream ss;

      ss <<
        std::endl <<
        "% --> generating code for noteRegularInChord " <<
        note->asString () <<
        ", line " << note->getInputLineNumber () <<
        std::endl;

      if (doTraceNotes) {
        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << ss.str ();
      }
    }
  }
#endif // MF_TRACE_IS_ENABLED

  // generate the note name
  fLilypondCodeStream <<
    notePitchAsLilypondString (note);

  // don't print the note duration,
  // it will be printed for the chord itself

  // generate the ties if relevant // JMI only 1 tie at most? 0.9.72
  const std::list <S_msrTie>& noteTiesList = note->getNoteTiesList ();

  if (! noteTiesList.empty ()) {
    for (S_msrTie noteTie : noteTiesList) {
      switch (noteTie->getTieKind ()) {
        case msrTieKind::kTieNone:
          break;
        case msrTieKind::kTieStart:
         fLilypondCodeStream << " ~ "; // JMI 0.9.72
          break;
        case msrTieKind::kTieContinue:
          break;
        case msrTieKind::kTieStop:
          break;
        case msrTieKind::kTieLetRing: // MusicXML 4.0
          fLilypondCodeStream << " \\laissezVibrer "; // JMI 0.9.72
          break;
      } // switch
    } // for
  }

  // don't print the string number if any,
  // it should appear after the chord itself
  const std::list <S_msrTechnicalWithInteger>&
    chordMemberNoteTechnicalsListWithIntegers =
      note->getNoteTechnicalWithIntegersList ();

  if (! chordMemberNoteTechnicalsListWithIntegers.empty ()) {
    for (S_msrTechnicalWithInteger technicalWithInteger : chordMemberNoteTechnicalsListWithIntegers ) {
      switch (technicalWithInteger->getTechnicalWithIntegerKind ()) {
        case msrTechnicalWithIntegerKind::kFingering:
          break;
        case msrTechnicalWithIntegerKind::kFret:
          break;
        case msrTechnicalWithIntegerKind::kString:
          if (fOnGoingChord) {
            fPendingChordMemberNotesStringNumbers.push_back (
              technicalWithInteger->
                  getTechnicalWithIntegerValue ());
          }
          break;
      } // switch
    } // for
  }

  // inside chords, a note is relative to the preceding one
  switch (gGlobalLpsr2lilypondOahGroup->fetchOctaveEntryVariableValue ()) {
    case msrOctaveEntryKind::kOctaveEntryRelative:
      fCurrentOctaveEntryReference = note;
      break;
    case msrOctaveEntryKind::kOctaveEntryAbsolute:
      break;
    case msrOctaveEntryKind::kOctaveEntryFixed:
      break;
  } // switch
}

void lpsr2lilypondTranslator::generateRegularNoteInTuplet (
  const S_msrNote& note)
{
#ifdef MF_TRACE_IS_ENABLED
  {
    Bool
      doTraceNotes =
        gTraceOahGroup->getTraceNotes (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (doTraceNotes || generateMsrVisitingInformation) {
      std::stringstream ss;

      ss <<
        std::endl <<
        "% --> generating code for noteRegularInTuplet " <<
        note->asString () <<
        ", line " << note->getInputLineNumber () <<
        std::endl;

        if (doTraceNotes) {
          gWaeHandler->waeTrace (
            __FILE__, mfInputLineNumber (__LINE__),
            ss.str ());
        }

        if (generateMsrVisitingInformation) {
          fLilypondCodeStream << ss.str ();
      }
    }
  }
#endif // MF_TRACE_IS_ENABLED

  // generate the note name
  fLilypondCodeStream <<
    notePitchAsLilypondString (note);

  // generate the note display duration if relevant
//   fLilypondCodeStream <<
//     durationAsLilypondStringIfItShouldBeGenerated (
//       note->getInputLineNumber (),
//       note->getNoteDisplayWholeNotes ());
  mfWholeNotes
    noteDisplayWholeNotes =
      note->getNoteDisplayWholeNotes ();

  if (wholeNotesDurationShouldBeGenerated (noteDisplayWholeNotes)) {
    generateWholeNotesDuration (
      note->getInputLineNumber (),
      noteDisplayWholeNotes);
  }

/* JMI
  // generate the tie if any
  {
    const std::list <S_msrTie>& noteTiesList = note->getNoteTiesList ();

    if (! noteTiesList.empty ()) {
      if (noteTie->getTieKind () == msrTieKind::kTieStart) {
        fLilypondCodeStream <<
          " %{ <-- line " <<
          note->getInputLineNumber () <<
          " %} " <<
          "~  %{ kTupletMemberNote %} "; // JMI spaces???
      }
    }
  }
*/

  // this note is the new relative octave reference
  switch (gGlobalLpsr2lilypondOahGroup->fetchOctaveEntryVariableValue ()) {
    case msrOctaveEntryKind::kOctaveEntryRelative:
      fCurrentOctaveEntryReference = note;
      break;
    case msrOctaveEntryKind::kOctaveEntryAbsolute:
      break;
    case msrOctaveEntryKind::kOctaveEntryFixed:
      break;
  } // switch
}

void lpsr2lilypondTranslator::generateRestInTuplet (
  const S_msrNote& note)
{
#ifdef MF_TRACE_IS_ENABLED
  {
    Bool
      doTraceNotes =
        gTraceOahGroup->getTraceNotes (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (doTraceNotes || generateMsrVisitingInformation) {
    std::stringstream ss;

    ss <<
      std::endl <<
      "% --> generating code for noteRestInTuplet " <<
      note->asString () <<
      ", line " << note->getInputLineNumber () <<
      std::endl;

      if (doTraceNotes) {
        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << ss.str ();
      }
    }
  }
#endif // MF_TRACE_IS_ENABLED

  if (gGlobalLpsr2lilypondOahGroup->getIndentTuplets ()) {
    fLilypondCodeStream << std::endl;
  }

  // generate the note name
  fLilypondCodeStream <<
    char (
      note->getNoteOccupiesAFullMeasure ()
        ? 's' // JMI ??? 'R'
        : 'r');
  if (gGlobalLpsr2lilypondOahGroup->getCommentLilypondRestNotes ()) {
    fLilypondCodeStream << " %{ sr333 %} ";
  }

  // generate the note display duration if relevant
//   fLilypondCodeStream <<
//     durationAsLilypondStringIfItShouldBeGenerated (
//       note->getInputLineNumber (),
//       note-> getNoteDisplayWholeNotes ());
  mfWholeNotes
    noteDisplayWholeNotes =
      note->getNoteDisplayWholeNotes ();

  if (wholeNotesDurationShouldBeGenerated (noteDisplayWholeNotes)) {
    generateWholeNotesDuration (
      note->getInputLineNumber (),
      noteDisplayWholeNotes);
  }

  if (gGlobalLpsr2lilypondOahGroup->getInputLineNumbers ()) {
    // generate information and line number as a comment
    fLilypondCodeStream <<
      " %{ <-- line " <<
      note->getInputLineNumber () <<
      " %} ";
  }

  if (gGlobalLpsr2lilypondOahGroup->getNotesComments ()) {
    // generate information and line number as a comment
    fLilypondCodeStream <<
      " %{ rest in tuplet %} ";
  }

/* JMI
  // generate the tie if any
  {
    const std::list <S_msrTie>& noteTiesList = note->getNoteTiesList ();

    if (! noteTiesList.empty ()) {
      if (noteTie->getTieKind () == msrTieKind::kTieStart) {
        fLilypondCodeStream <<
          " %{ line " <<
          note->getInputLineNumber () <<
          " %} " <<
          "~  %{ kTupletMemberNote %} "; // JMI spaces???
      }
    }
  }
*/

  // a rest is no relative octave reference,
}

void lpsr2lilypondTranslator::generateNoteUnpitchedInTuplet (
  const S_msrNote& note)
{
#ifdef MF_TRACE_IS_ENABLED
  {
    Bool
      doTraceNotes =
        gTraceOahGroup->getTraceNotes (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (doTraceNotes || generateMsrVisitingInformation) {
      std::stringstream ss;

      ss <<
        std::endl <<
        "% --> generating code for noteUnpitchedInTuplet " <<
        note->asString () <<
        ", line " << note->getInputLineNumber () <<
        std::endl;

      if (doTraceNotes) {
        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << ss.str ();
      }
    }
  }
#endif // MF_TRACE_IS_ENABLED

  if (gGlobalLpsr2lilypondOahGroup->getIndentTuplets ()) {
    fLilypondCodeStream << std::endl;
  }

  // generate the note name
  fLilypondCodeStream <<
    "e"; // by convention JMI 0.9.67

  // generate the note display duration if relevant
//   fLilypondCodeStream <<
//     durationAsLilypondStringIfItShouldBeGenerated (
//       note->getInputLineNumber (),
//       note-> getNoteDisplayWholeNotes ());
  mfWholeNotes
    noteDisplayWholeNotes =
      note->getNoteDisplayWholeNotes ();

  if (wholeNotesDurationShouldBeGenerated (noteDisplayWholeNotes)) {
    generateWholeNotesDuration (
      note->getInputLineNumber (),
      noteDisplayWholeNotes);
  }

/* JMI
  // generate the tie if any
  {
    const std::list <S_msrTie>& noteTiesList = note->getNoteTiesList ();

    if (! noteTiesList.empty ()) {
      if (noteTie->getTieKind () == msrTieKind::kTieStart) {
        fLilypondCodeStream <<
          " %{ line " <<
          note->getInputLineNumber () <<
          " %} " <<
          "~  %{ kTupletUnpitchedMemberNote %} ";
      }
    }
  }
  */
}

void lpsr2lilypondTranslator::generateRegularNoteInGraceNotesGroup (
  const S_msrNote& note)
{
#ifdef MF_TRACE_IS_ENABLED
  {
    Bool
      doTraceNotes = // true ||
        gTraceOahGroup->getTraceNotes (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (doTraceNotes || generateMsrVisitingInformation) {
      std::stringstream ss;

      ss <<
        std::endl <<
        "% --> generating code for noteRegularInGraceNotesGroup " <<
        note->asString () <<
        ", line " << note->getInputLineNumber () <<
        std::endl;

      if (doTraceNotes) {
        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << ss.str ();
      }
    }
  }
#endif // MF_TRACE_IS_ENABLED

  // generate the note name
  fLilypondCodeStream <<
    notePitchAsLilypondString (note);

  // generate the grace note's graphic duration
  fLilypondCodeStream <<
    notesDurationKindAsLilypondString (
      note->
        getNoteGraphicNotesDurationKind ());

  // generate the dots if any JMI ???
  for (int i = 0; i < note->getNoteDotsNumber (); ++i) {
    fLilypondCodeStream << ".";
  } // for

  // don't print the tie if any, 'acciacattura takes care of it
  /*
  {
    const std::list <S_msrTie>& noteTiesList = note->getNoteTiesList ();

    if (! noteTiesList.empty ()) {
      if (noteTie->getTieKind () == msrTieKind::kTieStart) {
        fLilypondCodeStream <<
          " %{ line " <<
          note->getInputLineNumber () <<
          " %} " <<
          "~  %{ kGraceNote %} ";
      }
    }
  }
  */

  // this note is the new relative octave reference
  switch (gGlobalLpsr2lilypondOahGroup->fetchOctaveEntryVariableValue ()) {
    case msrOctaveEntryKind::kOctaveEntryRelative:
      fCurrentOctaveEntryReference = note;
      break;
    case msrOctaveEntryKind::kOctaveEntryAbsolute:
      break;
    case msrOctaveEntryKind::kOctaveEntryFixed:
      break;
  } // switch
}

void lpsr2lilypondTranslator::generateRestInGraceNotesGroup (
  const S_msrNote& note)
{
#ifdef MF_TRACE_IS_ENABLED
  {
    Bool
      doTraceNotes = // true ||
        gTraceOahGroup->getTraceNotes (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (doTraceNotes || generateMsrVisitingInformation) {
      std::stringstream ss;

      ss <<
        std::endl <<
        "% --> generating code for noteRestInGraceNotesGroup " <<
        note->asString () <<
        ", line " << note->getInputLineNumber () <<
        std::endl;

      if (doTraceNotes) {
        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << ss.str ();
      }
    }
  }
#endif // MF_TRACE_IS_ENABLED

  // generate the note name
  fLilypondCodeStream <<
    notePitchAsLilypondString (note);

  // generate the grace note's graphic duration
  fLilypondCodeStream <<
    notesDurationKindAsLilypondString (
      note->
        getNoteGraphicNotesDurationKind ());

  // generate the dots if any JMI ???
  for (int i = 0; i < note->getNoteDotsNumber (); ++i) {
    fLilypondCodeStream << ".";
  } // for

  // don't print the tie if any, 'acciacattura takes care of it
  /*
  {
    const std::list <S_msrTie>& noteTiesList = note->getNoteTiesList ();

    if (! noteTiesList.empty ()) {
      if (noteTie->getTieKind () == msrTieKind::kTieStart) {
        fLilypondCodeStream <<
          " %{ line " <<
          note->getInputLineNumber () <<
          " %} " <<
          "~  %{ kGraceNote %} ";
      }
    }
  }
  */

  // this note is the new relative octave reference
  switch (gGlobalLpsr2lilypondOahGroup->fetchOctaveEntryVariableValue ()) {
    case msrOctaveEntryKind::kOctaveEntryRelative:
      fCurrentOctaveEntryReference = note;
      break;
    case msrOctaveEntryKind::kOctaveEntryAbsolute:
      break;
    case msrOctaveEntryKind::kOctaveEntryFixed:
      break;
  } // switch
}

void lpsr2lilypondTranslator::generateSkipInGraceNotesGroup (
  const S_msrNote& note)
{
#ifdef MF_TRACE_IS_ENABLED
  {
    Bool
      doTraceNotes =
        gTraceOahGroup->getTraceNotes (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (doTraceNotes || generateMsrVisitingInformation) {
      std::stringstream ss;

      ss <<
        std::endl <<
        "% --> generating code for noteSkipInGraceNotesGroup " <<
        note->asString () <<
        ", line " << note->getInputLineNumber () <<
        std::endl;

      if (doTraceNotes) {
        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << ss.str ();
      }
    }
  }
#endif // MF_TRACE_IS_ENABLED

  // generate the note name
  if (gGlobalLpsr2lilypondOahGroup->getGeneratePositionInMeasures ()) {
    // generate the rest name to help pin-point bugs
    fLilypondCodeStream << 'r';
  }
  else {
    // generate the skip name
    fLilypondCodeStream << 's';
    if (gGlobalLpsr2lilypondOahGroup->getCommentLilypondGraceNotes ()) {
      fLilypondCodeStream << " %{ s444 %} ";
    }
  }

  // generate the skip duration if relevant
//   fLilypondCodeStream <<
//     durationAsLilypondStringIfItShouldBeGenerated (
//       note->getInputLineNumber (),
//       note->getNoteDisplayWholeNotes ());
  mfWholeNotes
    noteDisplayWholeNotes =
      note->getNoteDisplayWholeNotes ();

  if (wholeNotesDurationShouldBeGenerated (noteDisplayWholeNotes)) {
    generateWholeNotesDuration (
      note->getInputLineNumber (),
      noteDisplayWholeNotes);
  }

  if (gGlobalLpsr2lilypondOahGroup->getInputLineNumbers ()) {
    // generate information and line number as a comment
    fLilypondCodeStream <<
      " %{ <-- line " <<
      note->getInputLineNumber () <<
      " %} ";
  }

  if (gGlobalLpsr2lilypondOahGroup->getNotesComments ()) {
    // generate information and line number as a comment
    fLilypondCodeStream <<
      " %{ skip in grace notes group %} ";
  }

  // generate the dots if any JMI ???
  for (int i = 0; i < note->getNoteDotsNumber (); ++i) {
    fLilypondCodeStream << ".";
  } // for

  // a grace skip is no relative octave reference,
  // the preceding one is kept
}

void lpsr2lilypondTranslator::generateNoteInChordInGraceNotesGroup (
  const S_msrNote& note)
{
#ifdef MF_TRACE_IS_ENABLED
  {
    Bool
      doTraceNotes =
        gTraceOahGroup->getTraceNotes (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (doTraceNotes || generateMsrVisitingInformation) {
      std::stringstream ss;

      ss <<
        std::endl <<
        "% --> generating code for noteInChordInGraceNotesGroup " <<
        note->asString () <<
        ", line " << note->getInputLineNumber () <<
        std::endl;

      if (doTraceNotes) {
        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << ss.str ();
      }
    }
  }
#endif // MF_TRACE_IS_ENABLED

  // generate the note name
  fLilypondCodeStream <<
    notePitchAsLilypondString (note);

  // dont't print the grace note's graphic duration

  // generate the dots if any JMI ???
  for (int i = 0; i < note->getNoteDotsNumber (); ++i) {
    fLilypondCodeStream << ".";
  } // for

  // don't print the tie if any, 'acciacattura takes care of it
  /*
  {
    const std::list <S_msrTie>& noteTiesList = note->getNoteTiesList ();

    if (! noteTiesList.empty ()) {
      if (noteTie->getTieKind () == msrTieKind::kTieStart) {
        fLilypondCodeStream <<
          " %{ line " <<
          note->getInputLineNumber () <<
          " %} " <<
          "~  %{ msrNoteKind::kNoteInChordInGraceNotesGroup %} ";
      }
    }
  }
  */

  // inside chords, a note is relative to the preceding one
  switch (gGlobalLpsr2lilypondOahGroup->fetchOctaveEntryVariableValue ()) {
    case msrOctaveEntryKind::kOctaveEntryRelative:
      fCurrentOctaveEntryReference = note;
      break;
    case msrOctaveEntryKind::kOctaveEntryAbsolute:
      break;
    case msrOctaveEntryKind::kOctaveEntryFixed:
      break;
  } // switch
}

void lpsr2lilypondTranslator::generateNoteInTupletInGraceNotesGroup (
  const S_msrNote& note)
{
#ifdef MF_TRACE_IS_ENABLED
  {
    Bool
      doTraceNotes =
        gTraceOahGroup->getTraceNotes (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (doTraceNotes || generateMsrVisitingInformation) {
      std::stringstream ss;

      ss <<
        std::endl <<
        "% --> generating code for noteInTupletInGraceNotesGroup " <<
        note->asString () <<
        ", line " << note->getInputLineNumber () <<
        std::endl;

      if (doTraceNotes) {
        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << ss.str ();
      }
    }
  }
#endif // MF_TRACE_IS_ENABLED

  if (gGlobalLpsr2lilypondOahGroup->getIndentTuplets ()) {
    fLilypondCodeStream << std::endl;
  }

  // generate the note name
  if (note->fetchNoteIsARest ()) {
    fLilypondCodeStream <<
      char (
        note->getNoteOccupiesAFullMeasure ()
          ? 'R'
          : 'r');
  }
  else {
    fLilypondCodeStream <<
      notePitchAsLilypondString (note);
  }

  // generate the note display duration if relevant
//   fLilypondCodeStream <<
//     durationAsLilypondStringIfItShouldBeGenerated (
//       note->getInputLineNumber (),
//       note->getNoteDisplayWholeNotes ());
  mfWholeNotes
    noteDisplayWholeNotes =
      note->getNoteDisplayWholeNotes ();

  if (wholeNotesDurationShouldBeGenerated (noteDisplayWholeNotes)) {
    generateWholeNotesDuration (
      note->getInputLineNumber (),
      noteDisplayWholeNotes);
  }

  // generate the ties if any
  const std::list <S_msrTie>& noteTiesList = note->getNoteTiesList ();

  if (! noteTiesList.empty ()) {
    for (S_msrTie noteTie : noteTiesList) {
      if (noteTie->getTieKind () == msrTieKind::kTieStart) {
        fLilypondCodeStream <<
          "~  "; // JMI spaces???

        if (gGlobalLpsr2lilypondOahGroup->getInputLineNumbers ()) {
          // generate information and line number as a comment
          fLilypondCodeStream <<
            " %{ <-- line " <<
            note->getInputLineNumber () <<
            " %} ";
        }

        if (gGlobalLpsr2lilypondOahGroup->getNotesComments ()) {
          // generate information and line number as a comment
          fLilypondCodeStream <<
            " %{ note in tuplet in graceNotesGroup %} ";
        }
      }
    } // for
  }

  // this note is no new relative octave reference JMI ???
  // this note is the new relative octave reference
  switch (gGlobalLpsr2lilypondOahGroup->fetchOctaveEntryVariableValue ()) {
    case msrOctaveEntryKind::kOctaveEntryRelative:
      fCurrentOctaveEntryReference = note;
      break;
    case msrOctaveEntryKind::kOctaveEntryAbsolute:
      break;
    case msrOctaveEntryKind::kOctaveEntryFixed:
      break;
  } // switch
}

void lpsr2lilypondTranslator::generateNoteInDoubleTremolo (
  const S_msrNote& note)
{
#ifdef MF_TRACE_IS_ENABLED
  {
    Bool
      doTraceNotes =
        gTraceOahGroup->getTraceNotes (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (doTraceNotes || generateMsrVisitingInformation) {
      std::stringstream ss;

      ss <<
        std::endl <<
        "% --> generating code for noteInDoubleTremolo " <<
        note->asString () <<
        ", line " << note->getInputLineNumber () <<
        std::endl;

      if (doTraceNotes) {
        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << ss.str ();
      }
    }
  }
#endif // MF_TRACE_IS_ENABLED

  // generate the note name
  fLilypondCodeStream <<
    notePitchAsLilypondString (note);

  // generate the note duration if relevant
//   fLilypondCodeStream <<
//     durationAsLilypondStringIfItShouldBeGenerated (
//       note->getInputLineNumber (),
//       note->getMeasureElementSoundingWholeNotes ());
  mfWholeNotes
    noteSoundingWholeNotes =
      note->getMeasureElementSoundingWholeNotes ();

  if (wholeNotesDurationShouldBeGenerated (noteSoundingWholeNotes)) {
    generateWholeNotesDuration (
      note->getInputLineNumber (),
      noteSoundingWholeNotes);
  }

  // handle delayed ornaments if any
  if (note->getNoteDelayedTurnOrnament ()) {
    // c2*2/3 ( s2*1/3\turn JMI
    fLilypondCodeStream <<
      "*" <<
      gGlobalLpsr2lilypondOahGroup->getDelayedOrnamentsRational ();
  }

/* JMI
  // generate the tie if any
  {
    const std::list <S_msrTie>& noteTiesList = note->getNoteTiesList ();

    if (! noteTiesList.empty ()) {
      if (noteTie->getTieKind () == msrTieKind::kTieStart) {
        fLilypondCodeStream <<
          " %{ line " <<
          note->getInputLineNumber () <<
          " %} " <<
          " ~ %{ kDoubleTremoloMemberNote %} ";
      }
    }
  }
*/

  // this note is the new relative octave reference
  switch (gGlobalLpsr2lilypondOahGroup->fetchOctaveEntryVariableValue ()) {
    case msrOctaveEntryKind::kOctaveEntryRelative:
      fCurrentOctaveEntryReference = note;
      break;
    case msrOctaveEntryKind::kOctaveEntryAbsolute:
      break;
    case msrOctaveEntryKind::kOctaveEntryFixed:
      break;
  } // switch
}

void lpsr2lilypondTranslator::generateNoteWordsList (
  const std::list <S_msrWords>& noteWordsList)
{
  if (! noteWordsList.empty ()) {
    std::list <S_msrWords>::const_iterator i;
    for (S_msrWords word : noteWordsList) {
      msrPlacementKind
        wordsPlacementKind =
          word->getWordsPlacementKind ();

      std::string wordsContents =
        word->getWordsContents ();

      msrFontStyleKind
        wordsFontStyleKind =
          word->getWordsFontStyleKind ();

      S_msrFontSize
        wordsFontSize =
          word->getWordsFontSize ();

      msrFontWeightKind
        wordsFontWeightKind =
          word->getWordsFontWeightKind ();

      std::string markup;

      {
        // create markup apart to have its length available
        std::stringstream ss;

        switch (wordsPlacementKind) {
          case msrPlacementKind::kPlacement_UNKNOWN_:
            ss << "-";
            break;
          case msrPlacementKind::kPlacementAbove:
            ss << "^";
            break;
          case msrPlacementKind::kPlacementBelow:
            ss << "_";
            break;
        } // switch

        ss <<
          "\\markup" << " { ";

        switch (wordsFontStyleKind) {
          case msrFontStyleKind::kFontStyleNone:
            break;
          case msrFontStyleKind::kFontStyleNormal:
            // LilyPond produces 'normal style' text by default
            break;
          case msrFontStyleKind::KFontStyleItalic:
            ss <<
              "\\italic ";
            break;
        } // switch

        switch (wordsFontWeightKind) {
          case msrFontWeightKind::kFontWeightNone:
            break;
          case msrFontWeightKind::kFontWeightNormal:
            // LilyPond produces 'normal weight' text by default
            break;
          case msrFontWeightKind::kFontWeightBold:
            ss <<
              "\\bold ";
            break;
        } // switch

        switch (wordsFontSize->getFontSizeKind ()) {
          case msrFontSizeKind::kFontSizeNone:
            break;
          case msrFontSizeKind::kFontSizeXXSmall:
            ss <<
              "\\tiny ";
            break;
          case msrFontSizeKind::kFontSizeXSmall:
            ss <<
              "\\smaller ";
            break;
          case msrFontSizeKind::kFontSizeSmall:
            ss <<
              "\\small ";
            break;
          case msrFontSizeKind::kFontSizeMedium:
            ss <<
              "\\normalsize ";
            break;
          case msrFontSizeKind::kFontSizeLarge:
            ss <<
              "\\large ";
            break;
          case msrFontSizeKind::kFontSizeXLarge:
            ss <<
              "\\larger ";
            break;
          case msrFontSizeKind::kFontSizeXXLarge:
            ss <<
              "\\huge ";
            break;
          case msrFontSizeKind::kFontSizeNumeric:
          /* JMI
            ss <<
              " %{ " <<
              wordsFontSize->getFontNumericSize () <<
              " points %} ";
              */
            break;
        } // switch

        // does wordsContents contains end of lines?
        size_t endOfLineFound = wordsContents.find ("\n");

        if (endOfLineFound == std::string::npos) {
          ss <<
   // JMI         mfDoubleQuoteStringIfNonAlpha (wordsContents) <<
            "\"" << wordsContents << "\"";
          }
        else {
          ss <<
            gIndenter.indentMultiLineStringWithCurrentOffset (
              generateAColumnForMarkup (
                wordsContents,
                lilypondMarkupColumnKind::kMarkupColumnKindLeftAligned));
        }

        ss <<
          " } ";

        markup = ss.str ();
      }

      fLilypondCodeStream <<
        markup;
    } // for
  }
}

void lpsr2lilypondTranslator::generateCodeRightAfterNote (
  const S_msrNote& note)
{
/* JMI
  // should a stem direction command be generated?
  if (! fOnGoingChord) {
    S_msrStem
      noteStem =
        note->getNoteStem ();

    if (noteStem) {
      msrStemKind
        noteStemKind =
          noteStem->getStemKind ();

      // should a stem neutral direction command be generated?
      if (noteStemKind != msrStemKind::kStemKindNeutral) { JMI switch 0.9.70
        fLilypondCodeStream <<
          stemAsLilypondString (msrStemKind::kStemKindNeutral);

  // JMI      fCurrentStemKind = stemKind;
      }
    }
  }
*/

  // generate note head tweaks before note
  generateNoteHeadSettingsAfterNote (note);

  if (! fOnGoingChord) {
    // generate the note codas if any
    const std::list <S_msrCoda>&
      noteCodasList =
        note->getNoteCodasList ();

    if (! noteCodasList.empty ()) {
      std::list <S_msrCoda>::const_iterator i;
      for (S_msrCoda coda : noteCodasList) {
        // generate only the first coda before the note
        switch (coda->getCodaKind ()) {
          case msrCodaKind::kCodaFirst:
            // generate the coda
            generateCoda (coda);
            break;
          case msrCodaKind::kCodaSecond:
            break;
        } // switch
      } // for
    }

    // generate the note dal segnos if any
    const std::list <S_msrDalSegno>&
      noteDalSegnosList =
        note->getNoteDalSegnosList ();

    if (! noteDalSegnosList.empty ()) {
      std::list <S_msrDalSegno>::const_iterator i;
      for (S_msrDalSegno dalSegno : noteDalSegnosList) {
        // generate the dal segno
        fLilypondCodeStream <<
          std::endl <<
          "\\override Score.RehearsalMark.break-visibility = #begin-of-line-invisible" <<
          std::endl <<
          "\\tweak self-alignment-X #RIGHT" <<
          std::endl <<
          "\\mark \\markup { " <<
          dalSegno->getDalSegnoString () <<
          " }" <<
          std::endl;
      } // for
    }
  }
}

//________________________________________________________________________
void lpsr2lilypondTranslator::generateNoteArticulation (
  const S_msrArticulation& articulation)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceArticulations ()) {
    std::stringstream ss;

    ss <<
      "generateNoteArticulation()" <<
      ", articulation: " << articulation->asString () <<
      ", line " << articulation->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // should the placement be generated?
  Bool doGeneratePlacement (true);

  // generate note articulation preamble if any
  switch (articulation->getArticulationKind ()) {
    case msrArticulationKind::kArticulation_UNKNOWN_:
       break;

    case msrArticulationKind::kArticulationAccent:
       break;
    case msrArticulationKind::kArticulationBreathMark:
      doGeneratePlacement = false;
      break;
    case msrArticulationKind::kArticulationCaesura:
      doGeneratePlacement = false;
      break;
    case msrArticulationKind::kArticulationSpiccato:
      doGeneratePlacement = true; // JMI 0.9.69
      break;
    case msrArticulationKind::kArticulationStaccato:
      doGeneratePlacement = true;
      break;
    case msrArticulationKind::kArticulationStaccatissimo:
      doGeneratePlacement = true;
      break;
    case msrArticulationKind::kArticulationStress:
      doGeneratePlacement = false;
      break;
    case msrArticulationKind::kArticulationUnstress:
      doGeneratePlacement = false;
      break;
    case msrArticulationKind::kArticulationDetachedLegato:
      doGeneratePlacement = true;
      break;
    case msrArticulationKind::kArticulationStrongAccent:
      doGeneratePlacement = true;
      break;
    case msrArticulationKind::kArticulationTenuto:
      doGeneratePlacement = true;
      break;

    case msrArticulationKind::kArticulationFermata:
      doGeneratePlacement = true;
      break;

    case msrArticulationKind::kArticulationArpeggiato:
      // this is handled in chordArticulationAsLilypondString ()
      doGeneratePlacement = false;
      break;
    case msrArticulationKind::kArticulationNonArpeggiato:
      // this is handled in chordArticulationAsLilypondString ()
      doGeneratePlacement = false;
      break;

    case msrArticulationKind::kArticulationDoit:
      doGeneratePlacement = true;
      break;
    case msrArticulationKind::kArticulationFalloff:
      doGeneratePlacement = true;
      break;
    case msrArticulationKind::kArticulationPlop:
      doGeneratePlacement = false;
      break;
    case msrArticulationKind::kArticulationScoop:
      doGeneratePlacement = false;
      break;
  } // switch

  // JMI 0.9.61
  msrArticulationKind
    articulationKind =
      articulation->getArticulationKind ();

  if (doGeneratePlacement) {
    // dont generate a placement for breath marks JMI 0.9.61
//     switch (articulationKind) {
//       case msrArticulationKind::kArticulationBreathMark:
//         break;
//
//       default:
        switch (articulation->getArticulationPlacementKind ()) {
          case msrPlacementKind::kPlacement_UNKNOWN_:
            fLilypondCodeStream << "-";
            break;

          case msrPlacementKind::kPlacementAbove:
            fLilypondCodeStream << "^";
            break;

          case msrPlacementKind::kPlacementBelow:
            fLilypondCodeStream << "_";
        } // switch
//     } // switch
  }

  // generate note articulation itself
  switch (articulationKind) {
    case msrArticulationKind::kArticulation_UNKNOWN_:
      fLilypondCodeStream << ">";
      break;

    case msrArticulationKind::kArticulationAccent:
      fLilypondCodeStream << ">";
      break;
    case msrArticulationKind::kArticulationBreathMark:
      fLilypondCodeStream << "\\breathe";
      break;
    case msrArticulationKind::kArticulationCaesura:
    /* JMI 0.9.66
      fLilypondCodeStream <<
        std::endl <<
R"(\once\override BreathingSign.text = \markup {\musicglyph #"scripts.caesura.straight"} \breathe)" <<
        std::endl;
     */
      fLilypondCodeStream <<
        std::endl <<
        "\\override BreathingSign.text = \\markup {"
        "\\musicglyph #\"scripts.caesura.curved\"}" <<
        std::endl <<
        "\\breathe" <<
        std::endl;
      break;
    case msrArticulationKind::kArticulationSpiccato:
      // spiccato does not exist in LilyPond,
      // generate staccatissimo instead JMI %{ spiccato??? %}
      fLilypondCodeStream <<
        "!";
      break;
    case msrArticulationKind::kArticulationStaccato:
      fLilypondCodeStream <<
        ".";
      break;
    case msrArticulationKind::kArticulationStaccatissimo:
      fLilypondCodeStream <<
        "!";
      break;
    case msrArticulationKind::kArticulationStress:
      fLilypondCodeStream <<
        " %{ stress??? %} ";
      break;
    case msrArticulationKind::kArticulationUnstress:
      fLilypondCodeStream <<
        " %{ unstress??? %} ";
      break;
    case msrArticulationKind::kArticulationDetachedLegato:
      fLilypondCodeStream <<
        "_"; // portato
      break;
    case msrArticulationKind::kArticulationStrongAccent:
      fLilypondCodeStream <<
        "^"; // marcato
      break;
    case msrArticulationKind::kArticulationTenuto:
      fLilypondCodeStream <<
        "-";
      break;

    case msrArticulationKind::kArticulationFermata:
      if (
        // fermata?
        S_msrFermata
          fermata =
            dynamic_cast<msrFermata*>(&(*articulation))
        ) {
        switch (fermata->getFermataTypeKind ()) {
          case msrArticulationFermataType::kArticulationFermataTypeNone:
            // no placement needed
            break;
          case msrArticulationFermataType::kArticulationFermataTypeUpright:
            // no placement needed
            break;
          case msrArticulationFermataType::kArticulationFermataTypeInverted:
            fLilypondCodeStream << "_";
            break;
        } // switch

        switch (fermata->getFermataShapeKind ()) {
          case msrFermataShapeKind::kArticulationFermataNormal:
            fLilypondCodeStream << "\\fermata ";
            break;
          case msrFermataShapeKind::kArticulationFermataAngled:
            fLilypondCodeStream << "\\shortfermata ";
            break;
          case msrFermataShapeKind::kArticulationFermataSquare:
            fLilypondCodeStream << "\\longfermata ";
            break;
        } // switch
      }
      else {
        std::stringstream ss;

        ss <<
          "note articulation '" <<
          articulation->asString () <<
          "' has 'fermata' kind, but is not of type S_msrFermata" <<
          ", line " << articulation->getInputLineNumber ();

        lpsr2lilypondInternalError (
          gServiceRunData->getInputSourceName (),
          articulation->getInputLineNumber (),
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }
      break;

    case msrArticulationKind::kArticulationArpeggiato:
      // this is handled in chordArticulationAsLilypondString ()
      break;
    case msrArticulationKind::kArticulationNonArpeggiato:
      // this is handled in chordArticulationAsLilypondString ()
      break;
    case msrArticulationKind::kArticulationDoit:
      fLilypondCodeStream <<
        "\\bendAfter #+4";
      break;
    case msrArticulationKind::kArticulationFalloff:
      fLilypondCodeStream <<
        "\\bendAfter #-4";
      break;
    case msrArticulationKind::kArticulationPlop:
      fLilypondCodeStream <<
        " %{ plop??? %} ";
      break;
    case msrArticulationKind::kArticulationScoop:
      switch (articulation->getArticulationPlacementKind ()) {
        case msrPlacementKind::kPlacement_UNKNOWN_:
          fLilypondCodeStream <<
            "\\scoopAbove"; // JMI 0.9.63 meaningfull default ???
          break;

        case msrPlacementKind::kPlacementAbove:
          fLilypondCodeStream <<
            "\\scoopAbove";
          break;

        case msrPlacementKind::kPlacementBelow:
          fLilypondCodeStream <<
            "\\scoopBelow";
      } // switch
      break;
  } // switch
}

//________________________________________________________________________
void lpsr2lilypondTranslator::generateChordArticulation (
  const S_msrArticulation& articulation)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceArticulations ()) {
    std::stringstream ss;

    ss <<
      "generateChordArticulation()" <<
      ", articulation: " << articulation->asString () <<
      ", line " << articulation->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // JMI 0.9.62
  msrArticulationKind
    articulationKind =
      articulation->getArticulationKind ();

  switch (articulation->getArticulationPlacementKind ()) { // JMI 0.9.62
    case msrPlacementKind::kPlacement_UNKNOWN_:
      fLilypondCodeStream << "-";
      break;

    case msrPlacementKind::kPlacementAbove:
      fLilypondCodeStream << "^";
      break;

    case msrPlacementKind::kPlacementBelow:
      switch (articulationKind) {
        case msrArticulationKind::kArticulation_UNKNOWN_:
          break;

        case msrArticulationKind::kArticulationAccent:
          fLilypondCodeStream << ">";
          break;
        case msrArticulationKind::kArticulationBreathMark:
          //  JMI 0.9.62      fLilypondCodeStream << "\\breathe";
          break;
        default:
          fLilypondCodeStream << "_";
      } // switch
      break;
  } // switch

  switch (articulationKind) {
    case msrArticulationKind::kArticulation_UNKNOWN_:
      break;

    case msrArticulationKind::kArticulationAccent:
      fLilypondCodeStream << ">";
      break;
    case msrArticulationKind::kArticulationBreathMark:
//  JMI 0.9.62      fLilypondCodeStream << "\\breathe";
      break;
    case msrArticulationKind::kArticulationCaesura:
    /* JMI 0.9.66
      fLilypondCodeStream <<
        std::endl <<
R"(\once\override BreathingSign.text = \markup {\musicglyph #"scripts.caesura.straight"} \breathe)" <<
        std::endl;
     */
      fLilypondCodeStream <<
        std::endl <<
        "\\override BreathingSign.text = \\markup {"
        "\\musicglyph #\"scripts.caesura.curved\"}" <<
        std::endl <<
      "\\breathe" <<
        std::endl;
      break;
    case msrArticulationKind::kArticulationSpiccato:
      fLilypondCodeStream <<
        " %{ spiccato??? %} ";
      break;
    case msrArticulationKind::kArticulationStaccato:
      fLilypondCodeStream <<
        "\\staccato"; // JMI "-.";
      break;
    case msrArticulationKind::kArticulationStaccatissimo:
      fLilypondCodeStream << "!";
      break;
    case msrArticulationKind::kArticulationStress:
      fLilypondCodeStream <<
        " %{ stress??? %} ";
      break;
    case msrArticulationKind::kArticulationUnstress:
      fLilypondCodeStream <<
        " %{ unstress %} ";
      break;
    case msrArticulationKind::kArticulationDetachedLegato:
      fLilypondCodeStream <<
        "_"; // portato
      break;
    case msrArticulationKind::kArticulationStrongAccent:
      fLilypondCodeStream <<
        "^"; // marcato
      break;
    case msrArticulationKind::kArticulationTenuto:
      fLilypondCodeStream << "-";
      break;

    case msrArticulationKind::kArticulationFermata:
      if (
        // fermata?
        S_msrFermata
          fermata =
            dynamic_cast<msrFermata*>(&(*articulation))
        ) {
        switch (fermata->getFermataTypeKind ()) {
          case msrArticulationFermataType::kArticulationFermataTypeNone:
            // no placement needed
            break;
          case msrArticulationFermataType::kArticulationFermataTypeUpright:
            // no placement needed
            break;
          case msrArticulationFermataType::kArticulationFermataTypeInverted:
            fLilypondCodeStream << "_";
            break;
        } // switch

        switch (fermata->getFermataShapeKind ()) {
          case msrFermataShapeKind::kArticulationFermataNormal:
            fLilypondCodeStream << "\\fermata ";
            break;
          case msrFermataShapeKind::kArticulationFermataAngled:
            fLilypondCodeStream << "\\shortfermata ";
            break;
          case msrFermataShapeKind::kArticulationFermataSquare:
            fLilypondCodeStream << "\\longfermata ";
            break;
        } // switch
      }
      else {
        std::stringstream ss;

        ss <<
          "chord articulation '" <<
          articulation->asString () <<
          "' has 'fermata' kind, but is not of type S_msrFermata" <<
          ", line " << articulation->getInputLineNumber ();

        lpsr2lilypondInternalError (
          gServiceRunData->getInputSourceName (),
          articulation->getInputLineNumber (),
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }
      break;

    case msrArticulationKind::kArticulationArpeggiato:
      fLilypondCodeStream <<
        "\\arpeggio";
      break;
    case msrArticulationKind::kArticulationNonArpeggiato:
      fLilypondCodeStream <<
        "\\arpeggio";
      break;
    case msrArticulationKind::kArticulationDoit:
      fLilypondCodeStream <<
        "\\bendAfter #+4";
      break;
    case msrArticulationKind::kArticulationFalloff:
      fLilypondCodeStream <<
        "\\bendAfter #-4";
      break;
    case msrArticulationKind::kArticulationPlop:
      fLilypondCodeStream <<
        " %{ plop %} ";
      break;
    case msrArticulationKind::kArticulationScoop:
      switch (articulation->getArticulationPlacementKind ()) {
        case msrPlacementKind::kPlacement_UNKNOWN_:
          fLilypondCodeStream <<
            "\\scoopAbove"; // JMI 0.9.63 ???
          break;

        case msrPlacementKind::kPlacementAbove:
          fLilypondCodeStream <<
            "\\scoopAbove";
          break;

        case msrPlacementKind::kPlacementBelow:
          fLilypondCodeStream <<
            "\\scoopBelow";
      } // switch
      break;
  } // switch
}

//________________________________________________________________________
std::string lpsr2lilypondTranslator::technicalAsLilypondString (
  const S_msrTechnical& technical)
{
  std::string result;

  switch (technical->getTechnicalKind ()) {
    case msrTechnicalKind::kTechnicalArrow:
      result = "%{ \\Arrow??? %} ";
      break;
    case msrTechnicalKind::kTechnicalDoubleTongue:
      result = "-\\tongue #2";
      break;
    case msrTechnicalKind::kTechnicalDownBow:
      result = "\\downbow";
      break;
    case msrTechnicalKind::kTechnicalFingernails:
      result = "%{ \\Fingernails??? %} ";
      break;
    case msrTechnicalKind::kTechnicalHarmonic:
      result = "\\flageolet"; // JMI "\\once\\override Staff.NoteHead.style = #'harmonic-mixed";
      break;
    case msrTechnicalKind::kTechnicalHeel:
      result = "\\heel"; // rheel??? JMI
      break;
    case msrTechnicalKind::kTechnicalHole:
      result = "%{ \\Hole??? %} ";
      break;
    case msrTechnicalKind::kTechnicalOpenString:
      result = "\\open"; // halfopen ??? JMI
      break;
    case msrTechnicalKind::kTechnicalSnapPizzicato:
      result = "\\snappizzicato";
      break;
    case msrTechnicalKind::kTechnicalStopped:
      result = "\\stopped"; // or -+ JMI
      break;
    case msrTechnicalKind::kTechnicalTap:
      result = "%{ \\Tap??? %} ";
      break;
    case msrTechnicalKind::kTechnicalThumbPosition:
      result = "\\thumb";
      break;
    case msrTechnicalKind::kTechnicalToe:
      result = "\\ltoe"; // rtoe ??? JMI
      break;
    case msrTechnicalKind::kTechnicalTripleTongue:
      result = "-\\tongue #3";
      break;
    case msrTechnicalKind::kTechnicalUpBow:
      result = "\\upbow";
      break;
  } // switch

  return result;
}

//________________________________________________________________________
std::string lpsr2lilypondTranslator::technicalWithIntegerAsLilypondString (
  const S_msrTechnicalWithInteger& technicalWithInteger)
{
  std::stringstream ss;

  switch (technicalWithInteger->getTechnicalWithIntegerKind ()) {
     case msrTechnicalWithIntegerKind::kFingering:
      ss <<
        "-" <<
       technicalWithInteger->
          getTechnicalWithIntegerValue ();
      break;

    case msrTechnicalWithIntegerKind::kFret:
      // LilyPond will take care of that JMI 0.9.70
      break;

    case msrTechnicalWithIntegerKind::kString:
      ss << // no space is allowed between the backSlash and the number
        "\\" <<
       technicalWithInteger->
          getTechnicalWithIntegerValue ();
      break;
  } // switch

  return ss.str ();
}

//________________________________________________________________________
std::string lpsr2lilypondTranslator::technicalWithFloatAsLilypondString (
  const S_msrTechnicalWithFloat& technicalWithFloat)
{
  std::stringstream ss;

  switch (technicalWithFloat->getTechnicalWithFloatKind ()) {
    case msrTechnicalWithFloatKind::kTechnicalWithFloatBend:
      ss <<
        "\\bendAfter " <<
       technicalWithFloat->
          getTechnicalWithFloatValue ();
      break;
  } // switch

  return ss.str ();
}

//________________________________________________________________________
std::string lpsr2lilypondTranslator::technicalWithStringAsLilypondString (
  const S_msrTechnicalWithString& technicalWithString)
{
  std::string result;

  switch (technicalWithString->getTechnicalWithStringKind ()) {
    case msrTechnicalWithStringKind::kHammerOn:
      break;
    case msrTechnicalWithStringKind::kHandbell:
      result = "%{ kHandbell??? %} ";
      break;
    case msrTechnicalWithStringKind::kOtherTechnical:
      result = "%{ kHandbell??? %} ";
      break;
    case msrTechnicalWithStringKind::kPluck:
      result = "%{ kPluck??? %} ";
      break;
    case msrTechnicalWithStringKind::kPullOff:
      break;
  } // switch

  std::string stringValue =
    technicalWithString->
      getTechnicalWithStringValue ();

  if (! stringValue.empty ()) {
    result +=
      " -\\markup {\"" + stringValue + "\"}";
  }

  return result;
}

//________________________________________________________________________
void lpsr2lilypondTranslator::generateOrnament (
  const S_msrOrnament& ornament)
{
  S_msrNote
    ornamentUpLinkToNote =
      ornament->
        getOrnamentUpLinkToNote ();

  std::string
    upLinkToNoteNotesDuration =
      ornamentUpLinkToNote->
        noteSoundingWholeNotesPitchAndOctaveAsString ();

  switch (ornament->getOrnamentKind ()) {
    case msrOrnamentKind::kOrnamentTrill:
      if (! ornamentUpLinkToNote->getNoteWavyLineSpannerStart ()) {
        fLilypondCodeStream <<
          "\\trill ";
      }
      else {
        fLilypondCodeStream <<
          "\\startTrillSpan ";
      }
      break;

/* JMI 0.9.67
    case msrOrnamentKind::kOrnamentDashes:
      if (! ornamentUpLinkToNote->getNoteWavyLineSpannerStart ()) {
        fLilypondCodeStream <<
          "%{ \\dashes %} ";
      }
      break;
*/

    case msrOrnamentKind::kOrnamentTurn:
      fLilypondCodeStream <<
        "\\turn ";
      break;

    case msrOrnamentKind::kOrnamentInvertedTurn:
      fLilypondCodeStream <<
        "\\reverseturn ";
      break;

    case msrOrnamentKind::kOrnamentDelayedTurn:
      {
        // c2*2/3  s2*1/3\turn
        mfRational
          remainingFraction =
            mfRational (1, 1)
              -
            gGlobalLpsr2lilypondOahGroup->getDelayedOrnamentsRational ();

        int
          numerator =
            remainingFraction.getNumerator (),
          denominator =
            remainingFraction.getDenominator ();

        fLilypondCodeStream <<
          's';
        if (gGlobalLpsr2lilypondOahGroup->getCommentLilypondOrnaments ()) {
          fLilypondCodeStream <<  " %{ s555 %} ";
        }
        fLilypondCodeStream <<
          upLinkToNoteNotesDuration <<
          "*" <<
            denominator
            -
            numerator <<
          '/' <<
            denominator <<
          "\\turn ";

        if (gGlobalLpsr2lilypondOahGroup->getInputLineNumbers ()) {
          // generate information and line number as a comment
          fLilypondCodeStream <<
            " %{ <-- line " <<
            ornamentUpLinkToNote->getInputLineNumber () << // JMI ornamentUpLinkToNote ???
            " %} ";
        }

        if (gGlobalLpsr2lilypondOahGroup->getNotesComments ()) {
          // generate information and line number as a comment
          fLilypondCodeStream <<
            " %{ kOrnamentDelayedTur %} ";
        }

        // forget about the last found whole notes duration,
        // since the latter has been multipled by fDelayedOrnamentsRational
        fLastMetWholeNotes = K_WHOLE_NOTES_UNKNOWN_;
      }
      break;

    case msrOrnamentKind::kOrnamentDelayedInvertedTurn:
      {
/* JMI
        fLilypondCodeStream <<
          "delayed inverted turn is not supported, replaced by inverted turn," <<
          std::endl <<
          "see http://lilypond.org/doc/v2.18/Documentation/snippets/expressive-marks";

        lpsrWarning (
          note->getInputLineNumber (),
          ss.str ());

        result = "\\reverseturn %{ " + ss.str () + " %} ";
*/
        // c2*2/3 ( s2*1/3\turn

        fLilypondCodeStream <<
          's';
        if (gGlobalLpsr2lilypondOahGroup->getCommentLilypondOrnaments ()) {
          fLilypondCodeStream << " %{ s666 %} ";
        }
        fLilypondCodeStream <<
          upLinkToNoteNotesDuration <<
          "*1/3" "\\reverseturn ";

        if (gGlobalLpsr2lilypondOahGroup->getInputLineNumbers ()) {
          // generate information and line number as a comment
          fLilypondCodeStream <<
            " %{ <-- line " <<
            ornamentUpLinkToNote->getInputLineNumber () << // JMI ornamentUpLinkToNote ???
            " %} ";
        }

        if (gGlobalLpsr2lilypondOahGroup->getNotesComments ()) {
          // generate information and line number as a comment
          fLilypondCodeStream <<
            " %{ kOrnamentDelayedInvertedTurn %} ";
        }
      }
      break;

    case msrOrnamentKind::kOrnamentVerticalTurn:
      fLilypondCodeStream <<
        "^\\markup { \\rotate #90 \\musicglyph #\"scripts.turn\" } ";
          /* JMI
      {
        std::string message =
          "delayed vertical turn is not supported, ignored";

        lpsrWarning (
          note->getInputLineNumber (),
          message);

        result = "%{ " + message + " %} ";
      }
        */
      break;

    case msrOrnamentKind::kOrnamentMordent:
      fLilypondCodeStream <<
        "\\mordent ";
      break;

    case msrOrnamentKind::kOrnamentInvertedMordent:
      fLilypondCodeStream <<
        "\\prall ";
      break;
      \
    case msrOrnamentKind::kOrnamentSchleifer:
      fLilypondCodeStream <<
        "\\schleifer ";
      break;

    case msrOrnamentKind::kOrnamentShake:
      fLilypondCodeStream <<
        "%{ \\shake??? %} ";
      break;

    case msrOrnamentKind::kOrnamentAccidentalKind:
      switch (ornament->getOrnamentPlacementKind ()) {
        case msrPlacementKind::kPlacement_UNKNOWN_:
          fLilypondCodeStream << "-";
          break;
        case msrPlacementKind::kPlacementAbove:
          fLilypondCodeStream << "^";
          break;
        case msrPlacementKind::kPlacementBelow:
          fLilypondCodeStream << "_";
          break;
      } // switch

//std::string result; // JMI
      switch (ornament->getOrnamentAccidentalKind ()) {
        case msrAccidentalKind::kAccidentalNone:
          break;

        case msrAccidentalKind::kAccidentalSharp:
          fLilypondCodeStream << "\\markup { \\sharp } ";
          break;
        case msrAccidentalKind::kAccidentalNatural:
          fLilypondCodeStream << "\\markup { \\natural } ";
          break;
        case msrAccidentalKind::kAccidentalFlat:
          fLilypondCodeStream << "\\markup { \\flat } ";
          break;
        case msrAccidentalKind::kAccidentalDoubleSharp:
          fLilypondCodeStream << "\\markup { \\doublesharp } ";
          fLilypondCodeStream << "\\markup { \\doubleflat } ";
          break;
        case msrAccidentalKind::kAccidentalSharpSharp:
          fLilypondCodeStream << "\\markup { aSharpSharp } ";
          break;
        case msrAccidentalKind::kAccidentalFlatFlat:
          fLilypondCodeStream << "\\markup { aFlatFlat } ";
          break;
        case msrAccidentalKind::kAccidentalNaturalSharp:
          fLilypondCodeStream << "\\markup { aNaturalSharp } ";
          break;
        case msrAccidentalKind::kAccidentalNaturalFlat:
          fLilypondCodeStream << "\\markup { aNaturalFlat } ";
          break;
        case msrAccidentalKind::kAccidentalQuarterFlat:
          fLilypondCodeStream << "\\markup { \\semiflat } ";
          break;
        case msrAccidentalKind::kAccidentalQuarterSharp:
          fLilypondCodeStream << "\\markup { \\semisharp } ";
          break;
        case msrAccidentalKind::kAccidentalThreeQuartersFlat:
          fLilypondCodeStream << "\\markup { \\sesquiflat } ";
          break;
        case msrAccidentalKind::kAccidentalThreeQuartersSharp:
          fLilypondCodeStream << "\\markup { \\sesquisharp } ";
          break;

        case msrAccidentalKind::kAccidentalSharpDown:
          fLilypondCodeStream << "\\markup { aSharpDown } ";
          break;
        case msrAccidentalKind::kAccidentalSharpUp:
          fLilypondCodeStream << "\\markup { aSharpUp } ";
          break;
        case msrAccidentalKind::kAccidentalNaturalDown:
          fLilypondCodeStream << "\\markup { aNaturalDown } ";
          break;
        case msrAccidentalKind::kAccidentalNaturalUp:
          fLilypondCodeStream << "\\markup { aNaturalUp } ";
          break;
        case msrAccidentalKind::kAccidentalFlatDown:
          fLilypondCodeStream << "\\markup { aFlatDown } ";
          break;
        case msrAccidentalKind::kAccidentalFlatUp:
          fLilypondCodeStream << "\\markup { aFlatUp } ";
          break;
        case msrAccidentalKind::kAccidentalTripleSharp:
          fLilypondCodeStream << "\\markup { aTripleSharp } ";
          break;
        case msrAccidentalKind::kAccidentalTripleFlat:
          fLilypondCodeStream << "\\markup { aTripleFlat } ";
          break;
        case msrAccidentalKind::kAccidentalSlashQuarterSharp:
          fLilypondCodeStream << "\\markup { aSlashQuarterSharp } ";
          break;
        case msrAccidentalKind::kAccidentalSlashSharp:
          fLilypondCodeStream << "\\markup { aSlashSharp } ";
          break;
        case msrAccidentalKind::kAccidentalSlashFlat:
          fLilypondCodeStream << "\\markup { aSlashFlat } ";
          break;
        case msrAccidentalKind::kAccidentalDoubleSlashFlat:
          fLilypondCodeStream << "\\markup { adoubleSlashFlat } ";
          break;
        case msrAccidentalKind::kAccidentalSharp_1:
          fLilypondCodeStream << "\\markup { aSharp_1 } ";
          break;
        case msrAccidentalKind::kAccidentalSharp_2:
          fLilypondCodeStream << "\\markup { aSharp_2 } ";
          break;
        case msrAccidentalKind::kAccidentalSharp_3:
          fLilypondCodeStream << "\\markup { aSharp_3 } ";
          break;
        case msrAccidentalKind::kAccidentalSharp_5:
          fLilypondCodeStream << "\\markup { aSharp_5 } ";
          break;
        case msrAccidentalKind::kAccidentalFlat_1:
          fLilypondCodeStream << "\\markup { aFlat_1 } ";
          break;
        case msrAccidentalKind::kAccidentalFlat_2:
          fLilypondCodeStream << "\\markup { aFlat_2 } ";
          break;
        case msrAccidentalKind::kAccidentalFlat_3:
          fLilypondCodeStream << "\\markup { aFlat_3 } ";
          break;
        case msrAccidentalKind::kAccidentalFlat_4:
          fLilypondCodeStream << "\\markup { aFlat_4 } ";
          break;
        case msrAccidentalKind::kAccidentalSori:
          fLilypondCodeStream << "\\markup { aSori } ";
          break;
        case msrAccidentalKind::kAccidentalKoron:
          fLilypondCodeStream << "\\markup { aKoron } ";
          break;

        case msrAccidentalKind::kAccidentalOther:
          fLilypondCodeStream << "\\markup { AccidentalOther } ";
          break;
      } // switch
      break;
  } // switch
}

//________________________________________________________________________
void lpsr2lilypondTranslator::generateSpannerBeforeNote (
  const S_msrSpanner& spanner,
  const S_msrNote&    note)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceSpanners ()) {
    std::stringstream ss;

    ss <<
      "Generating code for spanner " <<
      spanner->getSpannerKind () <<
      " before note " <<
      note->asString () <<
      ", line " <<
      spanner->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  msrSpannerTypeKind
    spannerTypeKind =
      spanner->getSpannerTypeKind ();

  // should tweaks be generated for the various spanner texts?
  switch (spannerTypeKind) {
    case msrSpannerTypeKind::kSpannerTypeStart:
      {
        // handle spanner begin text if not empty
        std::string spannerBeginText = spanner->getSpannerBeginText ();

        if (! spannerBeginText.empty ()) {
          fLilypondCodeStream <<
             "-\\tweak bound-details.left.text \\markup { \"" <<
             spannerBeginText <<
             "\" }" <<
            std::endl;
        }

        // handle spanner middle text if not empty
        std::string spannerMiddleText = spanner->getSpannerMiddleText ();

        if (! spannerMiddleText.empty ()) {
          fLilypondCodeStream <<
             "\\TextSpannerWithCenteredText \"" << // JMI
             spannerMiddleText <<
             "\"" <<
            std::endl;
        }

        // handle spanner end text if not empty
        std::string spannerEndText = spanner->getSpannerEndText ();

        if (! spannerEndText.empty ()) {
          fLilypondCodeStream <<
             "-\\tweak bound-details.right.text \\markup { \"" <<
             spannerEndText <<
             "\" }" <<
            std::endl;
        }
      }
      break;

    case msrSpannerTypeKind::kSpannerTypeStop:
      break;
    case msrSpannerTypeKind::kSpannerTypeContinue:
      break;
    case msrSpannerTypeKind::kSpannerType_UNKNOWN_:
      break;
  } // switch

  switch (spanner->getSpannerKind ()) {
    case msrSpannerKind::kSpannerDashes:
      switch (spannerTypeKind) {
        case msrSpannerTypeKind::kSpannerTypeStart:
          fLilypondCodeStream <<
            "\\once \\override TextSpanner.style = #'dashed-line" <<
            std::endl;
          fOnGoingTrillSpanner = true;
          break;

        case msrSpannerTypeKind::kSpannerTypeStop:
          break;
        case msrSpannerTypeKind::kSpannerTypeContinue:
          break;
        case msrSpannerTypeKind::kSpannerType_UNKNOWN_:
          break;
      } // switch
      break;

    case msrSpannerKind::kSpannerWavyLine:
      switch (spannerTypeKind) {
        case msrSpannerTypeKind::kSpannerTypeStart:
          if (spanner->getSpannerUpLinkToNote ()->getNoteTrillOrnament ()) {
            // don't generate anything, the trill will display the wavy line
            fOnGoingTrillSpanner = true;
          }
          else {
            fLilypondCodeStream <<
              "\\once \\override TextSpanner.style = #'trill" <<
              std::endl;
          }
          break;

        case msrSpannerTypeKind::kSpannerTypeStop:
          break;
        case msrSpannerTypeKind::kSpannerTypeContinue:
          break;
        case msrSpannerTypeKind::kSpannerType_UNKNOWN_:
          break;
      } // switch

      msrPlacementKind
        spannerPlacementKind =
          spanner->getSpannerPlacementKind ();

      if (spannerPlacementKind != fCurrentSpannerPlacementKind) {
        switch (spannerPlacementKind) {
          case msrPlacementKind::kPlacement_UNKNOWN_:
            break;
          case msrPlacementKind::kPlacementAbove:
            fLilypondCodeStream <<
              "\\textSpannerUp ";
            break;
          case msrPlacementKind::kPlacementBelow:
            fLilypondCodeStream <<
              "\\textSpannerDown ";
            break;
          } // switch

        fCurrentSpannerPlacementKind = spannerPlacementKind;
      }
      break;
  } // switch
}

//________________________________________________________________________
void lpsr2lilypondTranslator::generateSpannerAfterNote (
  const S_msrSpanner& spanner,
  const S_msrNote&    note)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceSpanners ()) {
    std::stringstream ss;

    ss <<
      "Generating code for spanner " <<
      spanner->getSpannerKind () <<
      " after note " <<
      note->asString () <<
      ", line " <<
      spanner->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // handling the spanner type kind // JMI ??? 0.9.67
  msrSpannerTypeKind
    spannerTypeKind =
      spanner->getSpannerTypeKind ();

  switch (spannerTypeKind) {
    case msrSpannerTypeKind::kSpannerTypeStart:
      // JMI ??? 0.9.67
      break;

    case msrSpannerTypeKind::kSpannerTypeStop:
      // JMI ??? 0.9.67
      break;
    case msrSpannerTypeKind::kSpannerTypeContinue:
      // JMI ??? 0.9.67
      break;
    case msrSpannerTypeKind::kSpannerType_UNKNOWN_:
      // JMI ??? 0.9.67
      break;
  } // switch

  // handling the spanner kind
  msrSpannerKind
    spannerKind =
      spanner->getSpannerKind ();

  switch (spannerKind) {
    case msrSpannerKind::kSpannerDashes:
      switch (spannerTypeKind) {
        case msrSpannerTypeKind::kSpannerTypeStart:
          fLilypondCodeStream <<
            "\\startTextSpan" <<
            std::endl;
          fOnGoingTrillSpanner = true;
          break;

        case msrSpannerTypeKind::kSpannerTypeStop:
          fLilypondCodeStream <<
            "\\stopTextSpan" <<
            std::endl;
          fOnGoingTrillSpanner = false;
          break;

        case msrSpannerTypeKind::kSpannerTypeContinue:
          break;
        case msrSpannerTypeKind::kSpannerType_UNKNOWN_:
          break;
      } // switch
      break;

    case msrSpannerKind::kSpannerWavyLine:
      switch (spannerTypeKind) {
        case msrSpannerTypeKind::kSpannerTypeStart:
          if (spanner->getSpannerUpLinkToNote ()->getNoteTrillOrnament ()) {
            // don't generate anything, the trill will display the wavy line
            fOnGoingTrillSpanner = true;
          }
          else {
            fLilypondCodeStream <<
              "\\startTextSpan" <<
              std::endl;
          }
          break;

        case msrSpannerTypeKind::kSpannerTypeStop:
          {
            // get spanner start end
            S_msrSpanner
              spannerStartEnd =
                spanner->
                  getSpannerSideLinkToOtherEnd ();

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
            // sanity check
            mfAssert (
              __FILE__, mfInputLineNumber (__LINE__),
              spannerStartEnd != nullptr,
              "spannerStartEnd is NULL");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

            // has the start end a trill ornament?
            if (fPendingTrillSpannerForStop) { // JMI 0.9.67
#ifdef MF_TRACE_IS_ENABLED
              if (gTraceOahGroup->getTraceSpanners ()) {
                std::stringstream ss;

                ss <<
                  "Generating code for pending spanner " <<
                  fPendingTrillSpannerForStop->getSpannerKind () <<
                  std::endl;

                gWaeHandler->waeTraceWithoutInputLocation (
                  __FILE__, mfInputLineNumber (__LINE__),
                  ss.str ());
              }
#endif // MF_TRACE_IS_ENABLED

              if (spannerStartEnd->getSpannerUpLinkToNote ()->getNoteTrillOrnament ()) {
                fLilypondCodeStream <<
                  "\\stopTrillSpan ";
                if (gGlobalLpsr2lilypondOahGroup->getCommentLilypondSpanners ()) {
                  fLilypondCodeStream <<
                    " %{ generateSpannerAfterNote() AAA %} ";
                }
              }
              else {
                fLilypondCodeStream <<
                  "\\stopTextSpan ";
                if (gGlobalLpsr2lilypondOahGroup->getCommentLilypondSpanners ()) {
                  fLilypondCodeStream <<
                    " %{ generateSpannerAfterNote() BBB %} ";
                }
              }

              fPendingTrillSpannerForStop = nullptr;
            }
            else {
#ifdef MF_TRACE_IS_ENABLED
              if (gTraceOahGroup->getTraceSpanners ()) {
                std::stringstream ss;

                ss <<
                  "Spanner " <<
                  spanner->getSpannerKind () <<
                  " becomes pending" <<
                  std::endl;

                gWaeHandler->waeTraceWithoutInputLocation (
                  __FILE__, mfInputLineNumber (__LINE__),
                  ss.str ());
              }
#endif // MF_TRACE_IS_ENABLED

              fPendingTrillSpannerForStop = spanner;
            }

            fOnGoingTrillSpanner = false;
          }
          break;
        case msrSpannerTypeKind::kSpannerTypeContinue:
          break;
        case msrSpannerTypeKind::kSpannerType_UNKNOWN_:
          break;
      } // switch
      break;
  } // switch
}

//________________________________________________________________________
std::string lpsr2lilypondTranslator::tupletFactorAsLilypondString (
  const msrTupletFactor& tupletFactor)
{
  std::stringstream ss;

  ss <<
    tupletFactor.getTupletActualNotes () <<
    '/' <<
    tupletFactor.getTupletNormalNotes ();

  return ss.str ();
}

//________________________________________________________________________
std::string lpsr2lilypondTranslator::dynamicAsLilypondString (
  const S_msrDynamic& dynamic)
{
  std::string result =
    "\\" + msrDynamicKindAsString (dynamic->getDynamicKind ());

  return result;
}

//________________________________________________________________________
std::string lpsr2lilypondTranslator::harpPedalTuningAsLilypondString (
  msrAlterationKind alterationKind)
{
  std::string result;

  switch (alterationKind) {
    case msrAlterationKind::kAlterationTripleFlat:
      result = "%{ tripleFlat %} ";
      break;
    case msrAlterationKind::kAlterationDoubleFlat:
      result = "%{ doubleFlat %} ";
      break;
    case msrAlterationKind::kAlterationSesquiFlat:
      result = "%{ sesquiFlat %} ";
      break;
    case msrAlterationKind::kAlterationFlat:
      result = "^";
      break;
    case msrAlterationKind::kAlterationSemiFlat:
      result = "?";
      break;
    case msrAlterationKind::kAlterationNatural:
      result = "-";
      break;
    case msrAlterationKind::kAlterationSemiSharp:
      result = "?";
      break;
    case msrAlterationKind::kAlterationSharp:
      result = "v";
      break;
    case msrAlterationKind::kAlterationSesquiSharp:
      result = "%{ sesquiSharp %} ";
      break;
    case msrAlterationKind::kAlterationDoubleSharp:
      result = "%{ doubleSharp %} ";
      break;
    case msrAlterationKind::kAlterationTripleSharp:
      result = "%{ tripleSharp %} ";
      break;
    case msrAlterationKind::kAlteration_UNKNOWN_:
      result = "kAlteration_UNKNOWN_";
      break;
  } // switch

  return result;
}

//________________________________________________________________________
void lpsr2lilypondTranslator::transposeDiatonicError (
  const mfInputLineNumber& inputLineNumber,
  int transposeDiatonic,
  int transposeChromatic)
{
  std::stringstream ss;

  ss <<
    "diatonic '" << transposeDiatonic <<
    "' is not consistent with " <<
    "chromatic '" << transposeChromatic <<
    "'";

  lpsr2lilypondError (
    gServiceRunData->getInputSourceName (),
    inputLineNumber,
    __FILE__, mfInputLineNumber (__LINE__),
    ss.str ());
}

//________________________________________________________________________
std::string lpsr2lilypondTranslator::singleTremoloNotesDurationAsLilypondString (
  const S_msrSingleTremolo& singleTremolo)
{
  int
    singleTremoloMarksNumber =
      singleTremolo->
        getSingleTremoloMarksNumber ();

/* JMI
  S_msrNote
    singleTremoloUpLinkToNote =
      singleTremolo->
        getSingleTremoloUpLinkToNote ();
*/

  mfDurationKind
    singleTremoloNoteNotesDurationKind =
      singleTremolo->
        getSingleTremoloGraphicNotesDurationKind ();
    /*
      singleTremoloUpLinkToNote->
        getNoteGraphicNotesDurationKind ();
    */

  /*
  The same output can be obtained by adding :N after the note,
  where N indicates the duration of the subdivision (it must be at least 8).
  If N is 8, one beam is added to the note’s stem.
  */

  int durationToUse =
    singleTremoloMarksNumber; // JMI / singleTremoloNoteSoundingWholeNotes;

  if (singleTremoloNoteNotesDurationKind >= mfDurationKind::kDurationEighth) {
    durationToUse +=
      1 + ((int) singleTremoloNoteNotesDurationKind - (int) mfDurationKind::kDurationEighth);
  }

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceTremolos ()) {
    std::stringstream ss;

    ss <<
      "singleTremoloNotesDurationAsLilypondString()" <<
      ", line " << singleTremolo->getInputLineNumber () <<
      cLilyPondCommaAndSpace <<
      mfSingularOrPlural (
        singleTremoloMarksNumber, "mark", "marks") <<
      ", singleTremoloNoteNotesDurationKind: " <<
      mfDurationKindAsString (singleTremoloNoteNotesDurationKind) <<
      ", durationToUse: " <<
      durationToUse;

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  std::stringstream ss;

  ss <<
    ":" <<
 // JMI   int (pow (2, durationToUse + 2)) <<
    int (1 << (durationToUse + 2)) <<
    cLilyPondSpace;

  return ss.str ();
}

//________________________________________________________________________
std::string lpsr2lilypondTranslator::harmonyDegreeAlterationKindAsLilypondString (
  msrAlterationKind harmonyDegreeAlterationKind)
{
  std::string result;

  switch (harmonyDegreeAlterationKind) {
    case msrAlterationKind::kAlteration_UNKNOWN_:
      result = "?";
      break;
    case msrAlterationKind::kAlterationTripleFlat:
      result = "?";
      break;
    case msrAlterationKind::kAlterationDoubleFlat:
      result = "?";
      break;
    case msrAlterationKind::kAlterationSesquiFlat:
      result = "?";
      break;
    case msrAlterationKind::kAlterationFlat:
      result = "-";
      break;
    case msrAlterationKind::kAlterationSemiFlat:
      result = "?";
      break;
    case msrAlterationKind::kAlterationNatural:
      result = "";
      break;
    case msrAlterationKind::kAlterationSemiSharp:
      result = "?";
      break;
    case msrAlterationKind::kAlterationSharp:
      result = "+";
      break;
    case msrAlterationKind::kAlterationSesquiSharp:
      result = "?";
      break;
    case msrAlterationKind::kAlterationDoubleSharp:
      result = "?";
      break;
    case msrAlterationKind::kAlterationTripleSharp:
      result = "?";
      break;
  } // switch

  return result;
}

std::string lpsr2lilypondTranslator::harmonyAsLilypondString (
  const S_msrHarmony& harmony)
{
  std::stringstream ss;

  // should '\powerChords' be generated?
  switch (harmony->getHarmonyKind ()) {
    case msrHarmonyKind::kHarmonyPower:
      if (! fPowerChordHaveAlreadyBeenGenerated) {
        if (gGlobalLpsr2lilypondOahGroup->getCommentLilypondHarmonies ()) {
          ss << "%{ \\powerChords ??? %} ";
        }
        fPowerChordHaveAlreadyBeenGenerated = true;
      }
      break;
    default:
      ;
  } // switch

  // generate harmony pitch
  ss <<
    msrQuarterTonesPitchKindAsStringInLanguage (
      harmony->
        getHarmonyRootQuarterTonesPitchKind (),
      gMsrOahGroup->
        getMsrQuarterTonesPitchesLanguageKind ());

  // generate harmony duration
  const msrTupletFactor&
    harmonyTupletFactor =
      harmony->getHarmonyTupletFactor ();

  if (harmonyTupletFactor.isEqualToOne ()) {
    // generate harmony sounding whole notes if relevant
//     ss <<
//       durationAsLilypondStringIfItShouldBeGenerated (
//         harmony->getInputLineNumber (),
//         harmony->getMeasureElementSoundingWholeNotes ());
    mfWholeNotes
      harmonySoundingWholeNotes =
        harmony->getMeasureElementSoundingWholeNotes ();

    if (wholeNotesDurationShouldBeGenerated (harmonySoundingWholeNotes)) {
      generateWholeNotesDurationOnStream (
        harmony->getInputLineNumber (),
        harmonySoundingWholeNotes,
        ss);
    }
  }
  else {
    // generate harmony display whole notes if relevant and tuplet factor
//     ss <<
//       durationAsLilypondStringIfItShouldBeGenerated (
//         harmony->getInputLineNumber (),
//         harmony->getHarmonyDisplayWholeNotes ()) <<
    mfWholeNotes
      harmonyDisplayWholeNotes =
        harmony->getHarmonyDisplayWholeNotes ();

    if (wholeNotesDurationShouldBeGenerated (harmonyDisplayWholeNotes)) {
      generateWholeNotesDurationOnStream (
        harmony->getInputLineNumber (),
        harmonyDisplayWholeNotes,
        ss);
    }
    ss <<
      "*" <<
      harmonyTupletFactor.inverse ().asFractionString ();
  }

  // generate harmony kind
  switch (harmony->getHarmonyKind ()) {
    case msrHarmonyKind::kHarmony_UNKNOWN_:
      ss << "kHarmony_UNKNOWN_";
      break;

    // MusicXML harmonies

    case msrHarmonyKind::kHarmonyMajor:
      ss << ":5.3";
      break;
    case msrHarmonyKind::kHarmonyMinor:
      ss << ":m";
      break;
    case msrHarmonyKind::kHarmonyAugmented:
      ss << ":aug";
      break;
    case msrHarmonyKind::kHarmonyDiminished:
      ss << ":dim";
      break;

    case msrHarmonyKind::kHarmonyDominant:
      ss << ":7";
      break;
    case msrHarmonyKind::kHarmonyMajorSeventh:
      ss << ":maj7";
      break;
    case msrHarmonyKind::kHarmonyMinorSeventh:
      ss << ":m7";
      break;
    case msrHarmonyKind::kHarmonyDiminishedSeventh:
      ss << ":dim7";
      break;
    case msrHarmonyKind::kHarmonyAugmentedSeventh:
      ss << ":aug7";
      break;
    case msrHarmonyKind::kHarmonyHalfDiminished:
      ss << ":m7.5-";
      break;
    case msrHarmonyKind::kHarmonyMinorMajorSeventh:
      ss << ":m7+";
      break;

    case msrHarmonyKind::kHarmonyMajorSixth:
      ss << ":6";
      break;
    case msrHarmonyKind::kHarmonyMinorSixth:
      ss << ":m6";
      break;

    case msrHarmonyKind::kHarmonyDominantNinth:
      ss << ":9";
      break;
    case msrHarmonyKind::kHarmonyMajorNinth:
      ss << ":maj7.9";
      break;
    case msrHarmonyKind::kHarmonyMinorNinth:
      ss << ":m7.9";
      break;

    case msrHarmonyKind::kHarmonyDominantEleventh:
      ss << ":11";
      break;
    case msrHarmonyKind::kHarmonyMajorEleventh:
      ss << ":maj7.11";
      break;
    case msrHarmonyKind::kHarmonyMinorEleventh:
      ss << ":m7.11";
      break;

    case msrHarmonyKind::kHarmonyDominantThirteenth:
      ss << ":13";
      break;
    case msrHarmonyKind::kHarmonyMajorThirteenth:
      ss << ":maj7.13";
      break;
    case msrHarmonyKind::kHarmonyMinorThirteenth:
      ss << ":m7.13";
      break;

    case msrHarmonyKind::kHarmonySuspendedSecond:
      ss << ":sus2";
      break;
    case msrHarmonyKind::kHarmonySuspendedFourth:
      ss << ":sus4";
      break;

/*
 * kNeapolitan f aes des' in:
 *
 * c e g c' -> f f aes des' -> d g d b -> c e g c'

they are three different pre-dominant chords that are taught to American undergrads in a sophomore theory course.

in E major:
Italian = C E A#
French = C E F# A#
German = C E G A#
Tristan = C D# F# A#

in all of them, the C and A# in theory want to fan out to B (the dominant).  This is, of course, in theory - Wagner’s use of the Tristan chord, which he clearly named his opera after, has the A# moving down to A, or the 7th of the dominant (I’m transposing to fit w/ the example above).  Wagner obviously did not pay much attention during his sophomore music theory course…
*/
    case msrHarmonyKind::kHarmonyNeapolitan:
      ss << ":3-.6-";
      if (gGlobalLpsr2lilypondOahGroup->getCommentLilypondHarmonies ()) {
        ss << "%{ :Neapolitan %} ";
      }
      break;
    case msrHarmonyKind::kHarmonyItalian:
      ss << ":3.6+";
      if (gGlobalLpsr2lilypondOahGroup->getCommentLilypondHarmonies ()) {
        ss << "%{ :Italian %} ";
      }
      break;
    case msrHarmonyKind::kHarmonyFrench:
      ss << ":3.5+.6+";
      if (gGlobalLpsr2lilypondOahGroup->getCommentLilypondHarmonies ()) {
        ss << "%{ :French %} ";
      }
      break;
    case msrHarmonyKind::kHarmonyGerman:
      ss << ":3.5.6+";
      if (gGlobalLpsr2lilypondOahGroup->getCommentLilypondHarmonies ()) {
        ss << "%{ :German %} ";
      }
      break;

    case msrHarmonyKind::kHarmonyPedal:
      ss << "%{ :Pedal %} ";
      break;
    case msrHarmonyKind::kHarmonyPower:
      ss << ":5";
      if (gGlobalLpsr2lilypondOahGroup->getCommentLilypondHarmonies ()) {
        ss << "%{ power %} ";
      }
      break;
    case msrHarmonyKind::kHarmonyTristan:
      ss << ":2+.5+.6+";
      if (gGlobalLpsr2lilypondOahGroup->getCommentLilypondHarmonies () ){
        ss << "%{ :Tristan %} ";
      }
      break;

    // jazz-specific chords

    case msrHarmonyKind::kHarmonyMinorMajorNinth: // -maj9, minmaj9
      ss << ":m9";
      break;

    case msrHarmonyKind::kHarmonyDominantSuspendedFourth: // 7sus4, domsus4
      ss << ":7sus4";
      break;
    case msrHarmonyKind::kHarmonyDominantAugmentedFifth: // 7#5, domaug5
      ss << ":7.5+";
      break;
    case msrHarmonyKind::kHarmonyDominantMinorNinth: // 7b9, dommin9
      ss << ":7.9-";
      break;
    case msrHarmonyKind::kHarmonyDominantAugmentedNinthDiminishedFifth: // 7#9b5, domaug9dim5
      ss << ":7.9+.5-";
      break;
    case msrHarmonyKind::kIntervkDominantAugmentedNinthAugmentedFifthHarmonyalAugmentedThirteenth: // 7#9#5, domaug9aug5
      ss << ":7.9+.5+";
      break;
    case msrHarmonyKind::kHarmonyDominantAugmentedEleventh: // 7#11, domaug11
      ss << ":7.11+";
      break;

    case msrHarmonyKind::kHarmonyMajorSeventhAugmentedEleventh: // maj7#11, maj7aug11
      ss << ":maj7.11+";
      break;

    // other

    case msrHarmonyKind::kHarmonyOther:
      ss << "%{ :kHarmonyOther %} ";
      break;

    case msrHarmonyKind::kHarmonyNone:
      ss << "%{ :kHarmonyNone %} ";
      break;
  } // switch

  // generate harmony degrees if any
  const std::list <S_msrHarmonyDegree>&
    harmonyDegreesList =
      harmony->getHarmonyDegreesList ();

  if (! harmonyDegreesList.empty ()) {
    Bool thereAreDegreesToBeRemoved (false);

    // generate degrees to be added if any first
    for (S_msrHarmonyDegree harmonyDegree : harmonyDegreesList) {
      // get harmony degree information
      int
        harmonyDegreeValue =
          harmonyDegree->getHarmonyDegreeValue ();

      msrAlterationKind
        harmonyDegreeAlterationKind =
          harmonyDegree->
            getHarmonyDegreeAlterationKind ();

      msrHarmonyDegreeTypeKind
        harmonyDegreeTypeKind =
          harmonyDegree->
            getHarmonyDegreeTypeKind ();

      // generate the harmony degree
      switch (harmonyDegreeTypeKind) {
        case msrHarmonyDegreeTypeKind::kHarmonyDegreeTypeAdd:
          ss <<
//             "." << // JMI ??? 0.9.71
            harmonyDegreeValue <<
            harmonyDegreeAlterationKindAsLilypondString (
              harmonyDegreeAlterationKind);
          break;

        case msrHarmonyDegreeTypeKind::kHarmonyDegreeTypeAlter:
          ss <<
//             "." << // JMI ??? 0.9.71
            harmonyDegreeValue <<
            harmonyDegreeAlterationKindAsLilypondString (
              harmonyDegreeAlterationKind);
          break;

        case msrHarmonyDegreeTypeKind::kHarmonyDegreeTypeSubstract:
          thereAreDegreesToBeRemoved = true;
          break;
      } // switch
    } // for

    // then print harmony degrees to be removed if any
    if (thereAreDegreesToBeRemoved) {
      ss << "^";

//       int counter = 0; // JMI 0.9.67
      for (S_msrHarmonyDegree harmonyDegree: harmonyDegreesList) {
//         ++counter;

        // get harmony degree information
        int
          harmonyDegreeValue =
            harmonyDegree->getHarmonyDegreeValue ();

        msrAlterationKind
          harmonyDegreeAlterationKind =
            harmonyDegree->
              getHarmonyDegreeAlterationKind ();

        msrHarmonyDegreeTypeKind
          harmonyDegreeTypeKind =
            harmonyDegree->
              getHarmonyDegreeTypeKind ();

        // generate the harmony degree
        switch (harmonyDegreeTypeKind) {
          case msrHarmonyDegreeTypeKind::kHarmonyDegreeTypeAdd:
          case msrHarmonyDegreeTypeKind::kHarmonyDegreeTypeAlter:
            break;

          case msrHarmonyDegreeTypeKind::kHarmonyDegreeTypeSubstract:
   // JMI         if (counter > 1) {}
          ss <<
            harmonyDegreeValue <<
            harmonyDegreeAlterationKindAsLilypondString (
              harmonyDegreeAlterationKind);
            break;
        } // switch
      } // for
    }
  }

  // generate the harmony bass if relevant
  msrQuarterTonesPitchKind
    harmonyBassQuarterTonesPitchKind =
      harmony->
        getHarmonyBassQuarterTonesPitchKind ();

  if (
    harmonyBassQuarterTonesPitchKind
      !=
    msrQuarterTonesPitchKind::kQTP_UNKNOWN_
  ) {
    ss <<
      '/' <<
      msrQuarterTonesPitchKindAsStringInLanguage (
        harmonyBassQuarterTonesPitchKind,
        gMsrOahGroup->
          getMsrQuarterTonesPitchesLanguageKind ());
  }

  // generate the harmony inversion if relevant // JMI ???
  int harmonyInversion =
    harmony->getHarmonyInversion ();

  if (harmonyInversion != K_HARMONY_INVERSION_NONE) {
    if (gGlobalLpsr2lilypondOahGroup->getCommentLilypondHarmonies ()) {
      ss <<
        "%{ inversion: " << harmonyInversion << " %} ";
    }
  }

  return ss.str ();
}

std::string lpsr2lilypondTranslator::figureAsLilypondString (
  const S_msrBassFigure& bassFigure)
{
  std::stringstream ss;

  ++fCurrentFiguredBassFiguresCounter;

  // is the figured bass parenthesized?
  msrFiguredBassParenthesesKind
    figuredBassParenthesesKind =
      fCurrentFiguredBass->
        getFiguredBassParenthesesKind ();

  // generate the bass figure number
  switch (figuredBassParenthesesKind) {
    case msrFiguredBassParenthesesKind::kFiguredBassParenthesesYes:
      ss << '[';
      break;
    case msrFiguredBassParenthesesKind::kFiguredBassParenthesesNo:
      break;
  } // switch

  ss <<
    bassFigure->getFigureNumber ();

  switch (figuredBassParenthesesKind) {
    case msrFiguredBassParenthesesKind::kFiguredBassParenthesesYes:
      ss << ']';
      break;
    case msrFiguredBassParenthesesKind::kFiguredBassParenthesesNo:
      break;
  } // switch

  // handle the bass figure prefix
  switch (bassFigure->getFigurePrefixKind ()) {
    case msrBassFigurePrefixKind::kBassFigurePrefix_UNKNOWN_:
      break;
    case msrBassFigurePrefixKind::kBassFigurePrefixDoubleFlat:
      ss << "--";
      break;
    case msrBassFigurePrefixKind::kBassFigurePrefixFlat:
      ss << "-";
      break;
    case msrBassFigurePrefixKind::kBassFigurePrefixFlatFlat:
      ss << "flat flat";
      break;
    case msrBassFigurePrefixKind::kBassFigurePrefixNatural:
      ss << "!";
      break;
    case msrBassFigurePrefixKind::kBassFigurePrefixSharpSharp:
      ss << "sharp sharp";
      break;
    case msrBassFigurePrefixKind::kBassFigurePrefixSharp:
      ss << "+";
      break;
    case msrBassFigurePrefixKind::kBassFigurePrefixDoubleSharp:
      ss << "++";
      break;
  } // switch

  // handle the bass figure suffix
  switch (bassFigure->getFigureSuffixKind ()) {
    case msrBassFigureSuffixKind::kBassFigureSuffix_UNKNOWN_:
      break;
    case msrBassFigureSuffixKind::kBassFigureSuffixDoubleFlat:
      ss << "double flat";
      break;
    case msrBassFigureSuffixKind::kBassFigureSuffixFlat:
      ss << "flat";
      break;
    case msrBassFigureSuffixKind::kBassFigureSuffixFlatFlat:
      ss << "flat flat";
      break;
    case msrBassFigureSuffixKind::kBassFigureSuffixNatural:
      ss << "natural";
      break;
    case msrBassFigureSuffixKind::kBassFigureSuffixSharpSharp:
      ss << "sharp sharp";
      break;
    case msrBassFigureSuffixKind::kBassFigureSuffixSharp:
      ss << "sharp";
      break;
    case msrBassFigureSuffixKind::kBassFigureSuffixDoubleSharp:
      ss << "souble sharp";
      break;
    case msrBassFigureSuffixKind::kBassFigureSuffixSlash:
      ss << '/';
      break;
  } // switch

  // generate a space if not last bass figure in figured bass
  if (
    fCurrentFiguredBassFiguresCounter
      <
    fCurrentFiguredBass->getFiguredBassFiguresList ().size ()
  ) {
    ss << cLilyPondSpace;
  }

  return ss.str ();
}

std::string lpsr2lilypondTranslator::figuredBassAsLilypondString (
  const S_msrFiguredBass& figuredBass)
{
  std::stringstream ss;

  ss << "<";

  const std::list <S_msrBassFigure>&
    figuredBassFiguresList =
      figuredBass->
        getFiguredBassFiguresList ();

  if (! figuredBassFiguresList.empty ()) {
    std::list <S_msrBassFigure>::const_iterator
      iBegin = figuredBassFiguresList.begin (),
      iEnd   = figuredBassFiguresList.end (),
      i      = iBegin;

    for ( ; ; ) {
      ss << figureAsLilypondString ((*i));
      if (++i == iEnd) break;
      ss << cLilyPondSpace;
    } // for
  }

  ss << ">";

  // generate figured bass element duration
  const msrTupletFactor&
    figuredBassTupletFactor =
      figuredBass->getFiguredBassTupletFactor ();

  if (figuredBassTupletFactor.isEqualToOne ()) { // JMI ???
    // generate figured bass element sounding whole notes if relevant
//     ss <<
//       durationAsLilypondStringIfItShouldBeGenerated (
//         figuredBass->getInputLineNumber (),
//         figuredBass->getMeasureElementSoundingWholeNotes ());
    mfWholeNotes
      figuredBassSoundingWholeNotes =
        figuredBass->getMeasureElementSoundingWholeNotes ();

    if (wholeNotesDurationShouldBeGenerated (figuredBassSoundingWholeNotes)) {
      generateWholeNotesDurationOnStream (
        figuredBass->getInputLineNumber (),
        figuredBassSoundingWholeNotes,
        ss);
    }
  }
  else {
    // generate figured bass element display whole notes if relevant and tuplet factor
//     ss <<
//       durationAsLilypondStringIfItShouldBeGenerated (
//         figuredBass->getInputLineNumber (),
//         figuredBass->getFiguredBassDisplayWholeNotes ()) <<
    mfWholeNotes
      figuredBassDisplayWholeNotes =
        figuredBass->getFiguredBassDisplayWholeNotes ();

    if (wholeNotesDurationShouldBeGenerated (figuredBassDisplayWholeNotes)) {
      generateWholeNotesDurationOnStream (
        figuredBass->getInputLineNumber (),
        figuredBassDisplayWholeNotes,
        ss);
    }

    ss <<
      "*" <<
      figuredBassTupletFactor.asRational ();
  }

  ss << cLilyPondSpace;

  return ss.str ();
}

//________________________________________________________________________
std::string lpsr2lilypondTranslator::lpsrContextTypeKindAsLilypondString (
  lpsrContextTypeKind contextTypeKind)
{
  std::string result;

  switch (contextTypeKind) {
    case lpsrContextTypeKind::kContextChordNames:
      result = "ChordNames";
      break;
    case lpsrContextTypeKind::kContextFiguredBass:
      result = "FiguredBass";
      break;
  } // switch

  return result;
}

//________________________________________________________________________
bool compareFrameNotesByDecreasingStringNumber (
  S_msrFrameNote& first,
  S_msrFrameNote& second)
{
  return
    first->getFrameNoteStringNumber ()
      >
    second->getFrameNoteStringNumber ();
}

std::string lpsr2lilypondTranslator::frameAsLilypondString (
  const S_msrFrame& frame)
{
  std::stringstream ss;

  std::list <S_msrFrameNote> // not const&, it will be sorted below
    frameFrameNotesList =
      frame->getFrameFrameNotesList ();

  const std::list <msrBarre>&
    frameBarresList =
      frame->getFrameBarresList ();

  int frameStringsNumber =
    frame->getFrameStringsNumber ();
  int frameFretsNumber =
    frame->getFrameFretsNumber ();

  ss <<
    "^\\markup {\\fret-diagram #\"";

  // are there fingerings?
  if (frame->getFrameContainsFingerings ()) {
    ss <<
      "f:1;";
  }

  // strings number
  if (frameStringsNumber != 6) {
    ss <<
      "w:" <<
      frameStringsNumber <<
      ";";
  }

  // frets number
  ss <<
    "h:" <<
    frameFretsNumber <<
    ";";

  // frame barres
  if (! frameBarresList.empty ()) {
    std::list <msrBarre>::const_iterator
      iBegin = frameBarresList.begin (),
      iEnd   = frameBarresList.end (),
      i      = iBegin;

    for ( ; ; ) {
      msrBarre barre = (*i);

      ss <<
        "c:" <<
        barre.getBarreStartString () <<
        '-' <<
        barre.getBarreStopString () <<
        '-' <<
        barre.getBarreFretNumber () <<
        ";";

      if (++i == iEnd) break;
  // JMI    os << ";";
    } // for
  }

  // frame notes
  if (! frameFrameNotesList.empty ()) {
    // sort the frame notes,
    // necessary both for code generation
    // and the detection of muted (i.e. absent) frame notes
    frameFrameNotesList.sort (
      compareFrameNotesByDecreasingStringNumber);

    int currentStringNumber = frameStringsNumber;

    // generate the code
    std::list <S_msrFrameNote>::const_iterator
      iBegin = frameFrameNotesList.begin (),
      iEnd   = frameFrameNotesList.end (),
      i      = iBegin;

    for ( ; ; ) {
      S_msrFrameNote
        frameNote = (*i);

      int frameNoteStringNumber =
        frameNote->getFrameNoteStringNumber ();
      int frameNoteFretNumber =
        frameNote->getFrameNoteFretNumber ();
      int frameNoteFingering =
        frameNote->getFrameNoteFingering ();

      while (currentStringNumber > frameNoteStringNumber) {
        // currentStringNumber is missing,
        // hence it is a muted note
        ss <<
          currentStringNumber <<
          "-x;";

        --currentStringNumber;
      }

      // generate the frame note
      ss <<
        frameNoteStringNumber <<
        "-";

      if (frameNoteFretNumber == 0) {
        ss <<
          "o";
      }
      else {
        ss <<
          frameNoteFretNumber;
      }

      if (frameNoteFingering != -1) {
        ss <<
          '-' <<
          frameNoteFingering;
      }

      ss <<
        ";";

      --currentStringNumber;

      if (++i == iEnd) break;
  // JMI    os << ";";
    } // for

    // handling low-numbered muted notes
    while (currentStringNumber != 0) {
      // currentStringNumber is missing,
      // hence it is a muted note
      ss <<
        currentStringNumber <<
        "-x;";

      --currentStringNumber;
    }
  }

  ss <<
    "\" } ";

  return ss.str ();
}

void lpsr2lilypondTranslator::generateInputLineNumberAndOrPositionInMeasureAsAComment (
  const S_msrMeasureElement& measureElement)
{
  fLilypondCodeStream <<
    " %{ ";

  if (gGlobalLpsr2lilypondOahGroup->getInputLineNumbers ()) {
    // generate the input line number as a comment
    fLilypondCodeStream <<
      " %{ <-- line " <<
      measureElement->getInputLineNumber () <<
      " %} ";
//       cLilyPondSpace;
  }

  if (gGlobalLpsr2lilypondOahGroup->getGeneratePositionInMeasures ()) {
    // generate the measure position as a comment
    fLilypondCodeStream <<
      "memp: " <<
      measureElement->getMeasureElementPositionInMeasure () <<
      cLilyPondSpace;
  }

  fLilypondCodeStream <<
    "%} ";
}

//________________________________________________________________________
std::string lpsr2lilypondTranslator::generateAColumnForMarkup (
  const std::string&       theString,
  lilypondMarkupColumnKind columnKind)
{
  std::stringstream ss;

  switch (columnKind) {
    case lilypondMarkupColumnKind::kMarkupColumnKindLeftAligned:
      ss << "\\column { ";
      break;
    case lilypondMarkupColumnKind::kMarkupColumnKindLeftACentered:
      ss << "\\center-column { ";
      break;
  } // switch
  ss << std::endl;

  std::list <std::string> chunksList;

  mfSplitRegularStringAtEndOfLines (
    theString,
    chunksList);

  if (! chunksList.empty ()) {
    // generate a markup containing the chunks
    std::list <std::string>::const_iterator
      iBegin = chunksList.begin (),
      iEnd   = chunksList.end (),
      i      = iBegin;

    for ( ; ; ) {
      ss <<
        "\\line { \"" << (*i) << "\" }";
      if (++i == iEnd) break;
    // JMI ???  ss << cLilyPondSpace;
      ss << std::endl;
    } // for

    ss <<
      " } ";

    if (gGlobalLpsr2lilypondOahGroup->getCommentLilypondStructureBasics ()) {
      ss <<
        "% " <<
        mfSingularOrPlural (
          chunksList.size (), "chunk", "chunks");
    }

    ss << std::endl;
  }

  return ss.str ();
}

//________________________________________________________________________
std::string lpsr2lilypondTranslator::generateMultilineMarkup (
  const std::string&       theString,
  lilypondMarkupColumnKind columnKind)
{
  std::stringstream ss;

  ss <<
    "\\markup { " <<
    generateAColumnForMarkup (
      theString,
      columnKind) <<
    " } " <<
    std::endl;

  return ss.str ();
}

// the LyLuaTeX postamble and postamble
//______________________________________________________________________________
std::string pLyLuaTexPreamble =
R"(
% !TEX TS-program = LuaLaTeX+se

\documentclass[12pt,a4paper]{article}

\usepackage[nofragment, insert=systems, debug=true, program=/Applications/LilyPond.app/Contents/Resources/bin/lilypond]{lyluatex}

% -------------------------------------------------------------------------
\begin{document}
% -------------------------------------------------------------------------

% the LilyPond score

\begin{center}
\begin{lilypond}
)";

std::string pLyLuaTexPostamble =
R"(
\end{lilypond}
\end{center}

% -------------------------------------------------------------------------
\end{document}
% -------------------------------------------------------------------------
)";

//________________________________________________________________________
void lpsr2lilypondTranslator::visitStart (S_lpsrScore& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateLpsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateLpsrVisitingInformation) {
      std::stringstream ss;

      ss <<
        "% --> Start visiting lpsrScore" <<
        ", line " << elt->getInputLineNumber () <<
        std::endl;

      if (traceLpsrVisitors) {
        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }

      if (generateLpsrVisitingInformation) {
        fLilypondCodeStream << ss.str ();
      }
    }
  }
#endif // MF_TRACE_IS_ENABLED

  // generate a LyLuaTeX preamble if needed
  if (gGlobalLpsr2lilypondOahGroup->getLyLuaTexOutput ()) {
    fLilypondCodeStream <<
      pLyLuaTexPreamble;
  }

  // LilyPond version
  generateLilypondVersion ();

  // global staff size
  generateGlobalStaffSize ();

//   // generate myBreak if relevant JMI 0.9.67
//   if (! gGlobalLpsr2lilypondOahGroup->getIgnoreLpsrLineBreaks ()) {
//     fLilypondCodeStream <<
//       "% Pick your choice from the next two lines as needed" <<
//       std::endl <<
//       "%myBreak = { \\break }" <<
//       std::endl <<
//       "myBreak = {}" <<
//       std::endl << std::endl;
//   }
//
//   // generate myPageBreak if relevant
//   if (! gGlobalLpsr2lilypondOahGroup->getIgnoreLpsrPageBreaks ()) {
//     fLilypondCodeStream <<
//       "% Pick your choice from the next two lines as needed" <<
//       std::endl <<
//       "%myPageBreak = { \\pageBreak }" <<
//       std::endl <<
//       "myPageBreak = {}" <<
//       std::endl << std::endl;
//   }

  // generate a 'global' empty variable?
  if (gGlobalLpsr2lilypondOahGroup->getGlobalEmptyVariable ()) {
    fLilypondCodeStream <<
      "global = {" <<
      std::endl <<
      '}' <<
      std::endl << std::endl;
  }
}

void lpsr2lilypondTranslator::visitEnd (S_lpsrScore& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateLpsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateLpsrVisitingInformation) {
      std::stringstream ss;

      ss <<
        "% --> End visiting lpsrScore" <<
        ", line " << elt->getInputLineNumber () <<
      std::endl;

      if (traceLpsrVisitors) {
        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }

      if (generateLpsrVisitingInformation) {
        fLilypondCodeStream << ss.str ();
      }
    }
  }
#endif // MF_TRACE_IS_ENABLED

  // final empty line in LilyPond code
  // to help copy/paste it
// JMI  fLilypondCodeStream << std::endl;

  // generate a LyLuaTeX postamble if needed
  if (gGlobalLpsr2lilypondOahGroup->getLyLuaTexOutput ()) {
    fLilypondCodeStream <<
      pLyLuaTexPostamble;
  }
}

//________________________________________________________________________
void lpsr2lilypondTranslator::visitStart (S_msrIdentification& elt) // JMI
{
#ifdef MF_TRACE_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream ss;

      ss <<
        "% --> Start visiting msrIdentification" <<
        ", line " << elt->getInputLineNumber () <<
      std::endl;

      if (traceLpsrVisitors) {
        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << ss.str ();
      }
    }
  }
#endif // MF_TRACE_IS_ENABLED
}

void lpsr2lilypondTranslator::visitEnd (S_msrIdentification& elt) // JMI
{
#ifdef MF_TRACE_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream ss;

      ss <<
        "% --> End visiting msrIdentification" <<
        ", line " << elt->getInputLineNumber () <<
      std::endl;

      if (traceLpsrVisitors) {
        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << ss.str ();
      }
    }
  }
#endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
std::string lpsr2lilypondTranslator::nameAsLilypondString (
  const std::string& name)
{
  std::string result;

  size_t endOfLineFound = name.find ("\n");

  if (endOfLineFound != std::string::npos) {
    result =
      generateMultilineMarkup (
        name,
        lilypondMarkupColumnKind::kMarkupColumnKindLeftACentered); // JMI ???
  }
  else {
    result = "\"" + name + "\"";
  }

  return result;
}

//________________________________________________________________________
void lpsr2lilypondTranslator::visitStart (S_lpsrSchemeVariable& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateLpsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateLpsrVisitingInformation) {
      std::stringstream ss;

      ss <<
        "% --> Start visiting lpsrSchemeVariable" <<
        ", line " << elt->getInputLineNumber () <<
      std::endl;

      if (traceLpsrVisitors) {
        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }

      if (generateLpsrVisitingInformation) {
        fLilypondCodeStream << ss.str ();
      }
    }
  }
#endif // MF_TRACE_IS_ENABLED

  std::string
    comment =
      elt->getComment ();

  if (! comment.empty ()) {
    fLilypondCodeStream <<
      "% " << comment <<
      std::endl;
  }

  switch (elt->getCommentedKind ()) {
    case lpsrCommentedKind::kCommentedYes:
      fLilypondCodeStream << "% ";
      break;
    case lpsrCommentedKind::kCommentedNo:
      break;
  } // switch

  fLilypondCodeStream <<
    "#(" <<
    elt->getVariableName () <<
    cLilyPondSpace <<
    elt->getVariableValue () <<
    ")";

  switch (elt->getEndOfLineKind ()) {
    case lpsrEndOfLineKind::kEndOfLineNone:
      break;
    case lpsrEndOfLineKind::kEndOfLineOnce:
      fLilypondCodeStream << std::endl;
      break;
    case lpsrEndOfLineKind::kEndOfLineTwice:
      fLilypondCodeStream <<
        std::endl << std::endl;
      break;
  } // switch
}

void lpsr2lilypondTranslator::visitEnd (S_lpsrSchemeVariable& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateLpsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateLpsrVisitingInformation) {
      std::stringstream ss;

      ss <<
        "% --> End visiting lpsrSchemeVariable" <<
        ", line " << elt->getInputLineNumber () <<
      std::endl;

      if (traceLpsrVisitors) {
        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }

      if (generateLpsrVisitingInformation) {
        fLilypondCodeStream << ss.str ();
      }
    }
  }
#endif // MF_TRACE_IS_ENABLED
}

//______________________________________________________________________________
std::string lpsr2lilypondTranslator::lengthUnitAsLilypondString (
  msrLengthUnitKind lengthUnitKind)
{
  std::string result;

  switch (lengthUnitKind) {
    case msrLengthUnitKind::kUnitInch:
      result = "\\in";
      break;
    case msrLengthUnitKind::kUnitCentimeter:
      result = "\\cm";
      break;
    case msrLengthUnitKind::kUnitMillimeter:
      result = "\\mm";
      break;
  } // switch

  return result;
}

//________________________________________________________________________
void lpsr2lilypondTranslator::visitStart (S_lpsrHeader& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateLpsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateLpsrVisitingInformation) {
      std::stringstream ss;

      ss <<
        "% --> Start visiting lpsrHeader" <<
        ", line " << elt->getInputLineNumber () <<
      std::endl;

      if (traceLpsrVisitors) {
        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }

      if (generateLpsrVisitingInformation) {
        fLilypondCodeStream << ss.str ();
      }
    }
  }
#endif // MF_TRACE_IS_ENABLED

  if (gGlobalLpsr2lilypondOahGroup->getNoHeaderBlock ()) {
    return;
  }

  fLilypondCodeStream <<
    "\\header {" <<
    std::endl;

  ++gIndenter; // decremented in visitEnd (S_lpsrHeader&)

  // generate header
  generateHeader (elt);

  fOnGoingHeader = true;
}

void lpsr2lilypondTranslator::visitEnd (S_lpsrHeader& elt)
{
  if (gGlobalLpsr2lilypondOahGroup->getNoHeaderBlock ()) {
    return;
  }

  --gIndenter; // incremented in visitStart (S_lpsrHeader&)

#ifdef MF_TRACE_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateLpsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateLpsrVisitingInformation) {
      std::stringstream ss;

      ss <<
        "% --> End visiting lpsrHeader" <<
        ", line " << elt->getInputLineNumber () <<
      std::endl;

      if (traceLpsrVisitors) {
        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }

      if (generateLpsrVisitingInformation) {
        fLilypondCodeStream << ss.str ();
      }
    }
  }
#endif // MF_TRACE_IS_ENABLED

  fLilypondCodeStream <<
    '}';

  if (gGlobalLpsr2lilypondOahGroup->getCommentLilypondStructureBasics ()) {
    fLilypondCodeStream <<
      " % header";
  }

  fLilypondCodeStream <<
    std::endl << std::endl;

  fOnGoingHeader = false;
}

//________________________________________________________________________
void lpsr2lilypondTranslator::generateLilypondVersion ()
{
  // LilyPond version
  Bool
    lilypondVersionHasBeenSet =
      gGlobalLpsr2lilypondOahGroup->
        getLilypondVersionAtom ()->
          getSelected ();

  std::string
    lilypondVersion =
      lilypondVersionHasBeenSet
        ? gGlobalLpsr2lilypondOahGroup->
            getLilypondVersion ()
        : gGlobalLpsr2lilypondOahGroup->
           getLilypondVersionMinimumValue ();

  fLilypondCodeStream <<
    "\\version \"" <<
    lilypondVersion <<
    "\"" <<
    std::endl << std::endl;
}

//________________________________________________________________________
void lpsr2lilypondTranslator::generateGlobalStaffSize ()
{
  Bool
    generateCommentedOutVariables =
      gGlobalLpsr2lilypondOahGroup->
        getGenerateCommentedOutVariables ();

  // global staff size
  Bool
    globalStaffSizeHasBeenSet =
      gGlobalLpsr2lilypondOahGroup->
        getGlobalStaffSizeAtom ()->
          getSelected ();

  Bool
    doGenerateGlobalStaffSize =
      globalStaffSizeHasBeenSet || generateCommentedOutVariables;

  if (doGenerateGlobalStaffSize) {
    fLilypondCodeStream <<
      "% Comment or adapt next line as needed (default is " <<
      gGlobalLpsr2lilypondOahGroup->
        getStaffGlobalSizeDefaultValue () <<
      ")" <<
      std::endl;

    if (generateCommentedOutVariables) {
      fLilypondCodeStream << "% ";
    }

    float
      globalStaffSize =
        gGlobalLpsr2lilypondOahGroup->
          getGlobalStaffSize ();

    fLilypondCodeStream <<
      std::left <<
      "#(set-global-staff-size " <<
      std::setprecision (6) << globalStaffSize <<
      ")" <<
      std::endl << std::endl;
  }
}

//________________________________________________________________________
void lpsr2lilypondTranslator::generateHeader (S_lpsrHeader header)
{
  /*
    the values given through options have precedence
    over those found in the header
  */

  std::list <std::pair <std::string, std::string>> nameStringValuePairsList;

  S_msrIdentification
    headerIdentification =
      header->
        getHeaderIdentification ();

  if (headerIdentification) {
    generateHeaderIdentificationPart (
      headerIdentification,
      nameStringValuePairsList);
  }

  createLilypondHeaderStringValuePairs (
    header,
    nameStringValuePairsList);

  // compute field width
  size_t fieldWidth = 0;

  for (
    std::pair <std::string, std::string> thePair : nameStringValuePairsList
  ) {
    std::string name = thePair.first;

    size_t nameSize = name.size ();

    if (nameSize > fieldWidth) {
      fieldWidth = nameSize;
    }
  } // for

  fieldWidth = 20;

  // generate the name-value pairs
  for (std::pair <std::string, std::string> thePair : nameStringValuePairsList) {
    std::string name = thePair.first;
    std::string value = thePair.second;

    fLilypondCodeStream <<
      std::left <<
      std::setw (fieldWidth) <<
      name << "= " <<
      generateStringAsLilypondMarkupOrDoubleQuotedString (
        value) <<
      std::endl;
  } // for
}

//________________________________________________________________________
void lpsr2lilypondTranslator::generateHeaderIdentificationPart (
  const S_msrIdentification& identification,
  std::list <std::pair <std::string, std::string>>&
                             nameStringValuePairsList)
{
  /*
    the values given through options have precedence
    over those found in the identification
  */

#ifdef MF_TRACE_IS_ENABLED
  int fieldWidth =
    identification -> maxIdentificationNamesLength ();
#endif // MF_TRACE_IS_ENABLED

  // work number
  std::string
    workNumber =
      identification->getIdentificationWorkNumber ();

  if (! workNumber.empty ()) {
    nameStringValuePairsList.push_back (
      std::make_pair (
        "workNumber",
        workNumber));
  }

  // work title
  std::string
    workCreditTypeTitle =
      identification->getIdentificationWorkCreditTypeTitle ();

  if (! workCreditTypeTitle.empty ()) {
    nameStringValuePairsList.push_back (
      std::make_pair (
        "workCreditTypeTitle",
        workCreditTypeTitle));
  }

  // opus
  std::string
    opus =
      identification->getIdentificationOpus ();

  if (! opus.empty ()) {
    nameStringValuePairsList.push_back (
      std::make_pair (
        "opus",
        opus));
  }

  // movementNumber
  std::string
    movementNumber =
      identification->getIdentificationMovementNumber ();

  if (! movementNumber.empty ()) {
    nameStringValuePairsList.push_back (
      std::make_pair (
        "movementNumber",
        movementNumber));
  }

  // movementTitle
  std::string
    movementTitle =
      identification->getIdentificationMovementTitle ();

  if (! movementTitle.empty ()) {
    nameStringValuePairsList.push_back (
      std::make_pair (
        "movementTitle",
        movementTitle));
  }

  // encodingDate
  std::string
    encodingDate =
      identification->getIdentificationEncodingDate ();

  if (! encodingDate.empty ()) {
    nameStringValuePairsList.push_back (
      std::make_pair (
        "encodingDate",
        encodingDate));
  }

  // miscellaneousField
  std::string
    miscellaneousField =
      identification->getIdentificationMiscellaneousField ();

  if (! miscellaneousField.empty ()) {
    nameStringValuePairsList.push_back (
      std::make_pair (
        "miscellaneousField",
        miscellaneousField));
  }

  // scoreInstrument
  std::string
    scoreInstrument =
      identification->getIdentificationScoreInstrument ();

  if (! scoreInstrument.empty ()) {
    nameStringValuePairsList.push_back (
      std::make_pair (
        "scoreInstrument",
        scoreInstrument));
  }

  // composers
  const std::list <std::string>&
    composersList =
      identification->
        getIdentificationComposersList ();

  if (! composersList.empty ()) {
    std::list <std::string>::const_iterator i;

    for (i = composersList.begin (); i != composersList.end (); ++i) {
      std::string composer = (*i);

      nameStringValuePairsList.push_back (
        std::make_pair (
          "composer",
          composer));
    } // for
  }

  // lyricists
  const std::list <std::string>&
    lyricistsList =
      identification->
        getIdentificationLyricistsList ();

  if (! lyricistsList.empty ()) {
    std::list <std::string>::const_iterator i;

    for (i = lyricistsList.begin (); i != lyricistsList.end (); ++i) {
      std::string lyricist = (*i);

      nameStringValuePairsList.push_back (
        std::make_pair (
          "lyricist",
          lyricist));
    } // for
  }

  // arrangers
  const std::list <std::string>&
    arrangersList =
      identification->
        getIdentificationArrangersList ();

  if (! arrangersList.empty ()) {
    std::list <std::string>::const_iterator i;

    for (i = arrangersList.begin (); i != arrangersList.end (); ++i) {
      std::string arranger = (*i);

      nameStringValuePairsList.push_back (
        std::make_pair (
          "arranger",
          arranger));
    } // for
  }

  // poets
  const std::list <std::string>&
    poetsList =
      identification->
        getIdentificationPoetsList ();

  if (! poetsList.empty ()) {
    std::list <std::string>::const_iterator i;

    for (i = poetsList.begin (); i != poetsList.end (); ++i) {
      std::string poet = (*i);

      nameStringValuePairsList.push_back (
        std::make_pair (
          "poet",
          poet));
    } // for
  }

  // translators
  const std::list <std::string>&
    translatorsList =
      identification->
        getIdentificationTranslatorsList ();

  if (! translatorsList.empty ()) {
    std::list <std::string>::const_iterator i;

    for (i = translatorsList.begin (); i != translatorsList.end (); ++i) {
      std::string translator = (*i);

      nameStringValuePairsList.push_back (
        std::make_pair (
          "translator",
          translator));
    } // for
  }

  // artists
  const std::list <std::string>&
    artistsList =
      identification->
        getIdentificationArtistsList ();

  if (! artistsList.empty ()) {
    std::list <std::string>::const_iterator i;

    for (i = artistsList.begin (); i != artistsList.end (); ++i) {
      std::string artist = (*i);

      nameStringValuePairsList.push_back (
        std::make_pair (
          "artist",
          artist));
    } // for
  }

  // softwares
  const std::list <std::string>&
    softwaresList =
      identification->
        getIdentificationSoftwaresList ();

  if (! softwaresList.empty ()) {
    std::list <std::string>::const_iterator i;

    for (i = softwaresList.begin (); i != softwaresList.end (); ++i) {
      std::string software = (*i);

      nameStringValuePairsList.push_back (
        std::make_pair (
          "software",
          software));
    } // for
  }

  // rights
  const std::list <std::string>&
    rightsList =
      identification->
        getIdentificationRightsList ();

  if (! rightsList.empty ()) {
    std::list <std::string>::const_iterator i;

    for (i = rightsList.begin (); i != rightsList.end (); ++i) {
      std::string right = (*i);

      nameStringValuePairsList.push_back (
        std::make_pair (
          "right",
          right));
    } // for
  }

  // sources
  const std::list <std::string>&
    sourcesList =
      identification->
        getIdentificationSourcesList ();

  if (! sourcesList.empty ()) {
    std::list <std::string>::const_iterator i;

    for (i = sourcesList.begin (); i != sourcesList.end (); ++i) {
      std::string source = (*i);

      nameStringValuePairsList.push_back (
        std::make_pair (
          "source",
          source));
    } // for
  }

  // relations
  const std::list <std::string>&
    relationsList =
      identification->
        getIdentificationRelationsList ();

  if (! relationsList.empty ()) {
    std::list <std::string>::const_iterator i;

    for (i = relationsList.begin (); i != relationsList.end (); ++i) {
      std::string relation = (*i);

      nameStringValuePairsList.push_back (
        std::make_pair (
          "relation",
          relation));
    } // for
  }

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceIdentification ()) {
    fLilypondCodeStream <<
      "% --> The identification nameStringValuePairsList contains:" <<
      std::endl;

    for (
      std::list <std::pair <std::string, std::string>>::const_iterator i =
        nameStringValuePairsList.begin ();
      i != nameStringValuePairsList.end ();
      ++i
    ) {
      std::string name = (*i).first;
      std::string value = (*i).second;

      fLilypondCodeStream <<
        "% " << name << " ---> " << value <<
        std::endl;
    } // for

    fLilypondCodeStream <<
      "% ====> fieldWidth: " << fieldWidth <<
      std::endl;
  }
#endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
void lpsr2lilypondTranslator::createLilypondHeaderStringValuePairs (
  const S_lpsrHeader& header,
  std::list <std::pair <std::string, std::string>>&
                      nameStringValuePairsList)
{
  /*
    the values given through options have precedence
    over those found in the header
  */

  // dedication
  std::string
    dedicationFromOption =
      gGlobalLpsr2lilypondOahGroup->getDedication ();

  if (! dedicationFromOption.empty ()) {
    nameStringValuePairsList.push_back (
      std::make_pair (
        "dedication",
        dedicationFromOption));
  }
  else {
    std::string
      lilypondDedication =
        header->getLilypondDedication ();

    if (! lilypondDedication.empty ()) {
      nameStringValuePairsList.push_back (
        std::make_pair (
          "dedication",
          lilypondDedication));
    }

    else {
      if (gGlobalLpsr2lilypondOahGroup->getGenerateCommentedOutVariables ()) {
        nameStringValuePairsList.push_back (
          std::make_pair (
            "% dedication",
            ""));
      }
    }
  }

  // piece
  std::string
    pieceFromOption =
      gGlobalLpsr2lilypondOahGroup->getPiece ();

  if (! pieceFromOption.empty ()) {
    nameStringValuePairsList.push_back (
      std::make_pair (
        "piece",
        pieceFromOption));
  }
  else {
    std::string
      lilypondPiece =
        header->getLilypondPiece ();

    if (! lilypondPiece.empty ()) {
      nameStringValuePairsList.push_back (
        std::make_pair (
          "piece",
          lilypondPiece));
    }

    else {
      if (gGlobalLpsr2lilypondOahGroup->getGenerateCommentedOutVariables ()) {
        nameStringValuePairsList.push_back (
          std::make_pair (
            "% piece",
            ""));
      }
    }
  }

  // opus
  std::string
    opusFromOption =
      gGlobalLpsr2lilypondOahGroup->getOpus ();

  if (! opusFromOption.empty ()) {
      nameStringValuePairsList.push_back (
        std::make_pair (
          "opus",
          opusFromOption));
  }
  else {
    std::string
      lilypondOpus =
        header->getLilypondOpus ();

    if (! lilypondOpus.empty ()) {
      nameStringValuePairsList.push_back (
        std::make_pair (
          "opus",
          lilypondOpus));
    }

    else {
      if (gGlobalLpsr2lilypondOahGroup->getGenerateCommentedOutVariables ()) {
        nameStringValuePairsList.push_back (
          std::make_pair (
            "% opus",
            ""));
      }
    }
  }

  // title
  // JMI
  if (gMsrOahGroup->getUseFilenameAsWorkCreditTypeTitle ()) {
    nameStringValuePairsList.push_back (
      std::make_pair (
        "title",
        gServiceRunData->getInputSourceName ()));
  }
  else {
    std::string
      titleFromOption =
        gGlobalLpsr2lilypondOahGroup->getTitle ();

    if (! titleFromOption.empty ()) {
        nameStringValuePairsList.push_back (
          std::make_pair (
            "title",
            titleFromOption));
    }
    else {
      std::string
        lilypondTitle =
          header->getLilypondTitle ();

      if (! lilypondTitle.empty ()) {
        nameStringValuePairsList.push_back (
          std::make_pair (
            "title",
            lilypondTitle));
      }

      else {
        if (gGlobalLpsr2lilypondOahGroup->getGenerateCommentedOutVariables ()) {
          nameStringValuePairsList.push_back (
            std::make_pair (
              "% title",
              ""));
        }
      }
    }
  }

  // subtitle
  std::string
    subtitleFromOption =
      gGlobalLpsr2lilypondOahGroup->getSubTitle ();

  if (! subtitleFromOption.empty ()) {
      nameStringValuePairsList.push_back (
        std::make_pair (
          "subtitle",
          subtitleFromOption));
  }
  else {
    std::string
      lilypondSubTitle =
        header->getLilypondSubTitle ();

    if (! lilypondSubTitle.empty ()) {
      nameStringValuePairsList.push_back (
        std::make_pair (
          "subtitle",
          lilypondSubTitle));
    }

    else {
      if (gGlobalLpsr2lilypondOahGroup->getGenerateCommentedOutVariables ()) {
        nameStringValuePairsList.push_back (
          std::make_pair (
            "% subtitle",
            ""));
      }
    }
  }

  // subsubtitle
  std::string
    subsubtitleFromOption =
      gGlobalLpsr2lilypondOahGroup->getSubSubTitle ();

  if (! subsubtitleFromOption.empty ()) {
      nameStringValuePairsList.push_back (
        std::make_pair (
          "subsubtitle",
          subsubtitleFromOption));
  }
  else {
    std::string
      lilypondSubSubTitle =
        header->getLilypondSubSubTitle ();

    if (! lilypondSubSubTitle.empty ()) {
      nameStringValuePairsList.push_back (
        std::make_pair (
          "subsubtitle",
          lilypondSubSubTitle));
    }

    else {
      if (gGlobalLpsr2lilypondOahGroup->getGenerateCommentedOutVariables ()) {
        nameStringValuePairsList.push_back (
          std::make_pair (
            "% subsubtitle",
            ""));
      }
    }
  }

  // instrument
  std::string
    instrumentFromOption =
      gGlobalLpsr2lilypondOahGroup->getHeaderInstrument ();

  if (! instrumentFromOption.empty ()) {
      nameStringValuePairsList.push_back (
        std::make_pair (
          "instrument",
          instrumentFromOption));
  }
  else {
    std::string
      lilypondInstrument =
        header->getLilypondInstrument ();

    if (! lilypondInstrument.empty ()) {
      nameStringValuePairsList.push_back (
        std::make_pair (
          "instrument",
          lilypondInstrument));
    }

    else {
      if (gGlobalLpsr2lilypondOahGroup->getGenerateCommentedOutVariables ()) {
        nameStringValuePairsList.push_back (
          std::make_pair (
            "% instrument",
            ""));
      }
    }
  }

  // meter
  std::string
    meterFromOption =
      gGlobalLpsr2lilypondOahGroup->getHeaderMeter ();

  if (! meterFromOption.empty ()) {
      nameStringValuePairsList.push_back (
        std::make_pair (
          "meter",
          meterFromOption));
  }
  else {
    std::string
      lilypondMeter =
        header->getLilypondMeter ();

    if (! lilypondMeter.empty ()) {
      nameStringValuePairsList.push_back (
        std::make_pair (
          "meter",
          lilypondMeter));
    }

    else {
      if (gGlobalLpsr2lilypondOahGroup->getGenerateCommentedOutVariables ()) {
        nameStringValuePairsList.push_back (
          std::make_pair (
            "% meter",
            ""));
      }
    }
  }

  // copyright
  std::string
    copyrightFromOption =
      gGlobalLpsr2lilypondOahGroup->getCopyright ();

  if (! copyrightFromOption.empty ()) {
    nameStringValuePairsList.push_back (
      std::make_pair (
        "copyright",
        copyrightFromOption));
  }
  else {
    std::string
      lilypondCopyright =
        header->getLilypondCopyright ();

    if (! lilypondCopyright.empty ()) {
      nameStringValuePairsList.push_back (
        std::make_pair (
          "copyright",
          lilypondCopyright));
    }

    else {
      if (gGlobalLpsr2lilypondOahGroup->getGenerateCommentedOutVariables ()) {
        nameStringValuePairsList.push_back (
          std::make_pair (
            "% copyright",
            ""));
      }
    }
  }

  // tagline
  std::string
    taglineFromOption =
      gGlobalLpsr2lilypondOahGroup->getTagline ();

  if (! taglineFromOption.empty ()) {
    nameStringValuePairsList.push_back (
      std::make_pair (
        "tagline",
        taglineFromOption));
  }
  else {
    std::string
      lilypondTagline =
        header->getLilypondTagline ();

    if (! lilypondTagline.empty ()) {
      nameStringValuePairsList.push_back (
        std::make_pair (
          "tagline",
          lilypondTagline));
    }

    else {
      if (gGlobalLpsr2lilypondOahGroup->getGenerateCommentedOutVariables ()) {
        nameStringValuePairsList.push_back (
          std::make_pair (
            "% tagline",
            ""));
      }
    }
  }
}

//________________________________________________________________________
std::string lpsr2lilypondTranslator::generateStringAsLilypondMarkupOrDoubleQuotedString (
  const std::string& value)
{
  std::string result;

  if (value.find ("markup") != std::string::npos) {
    // no quoting for markups
    result = value;
  }
  else {
    result =
      mfDoubleQuoteString (value);
  }

  return result;
}

//________________________________________________________________________
void lpsr2lilypondTranslator::generatePaper (
  S_lpsrPaper paper)
{
  // get MSR page layout
  S_msrPageLayout
    pageLayout =
      paper->getPageLayout ();

  // default length unit
  msrLengthUnitKind
    defaultLengthUnit =
      msrLengthUnitKind::kUnitMillimeter; // JMI

  fLilypondCodeStream <<
    "\\paper {" <<
    std::endl;

  ++gIndenter;

  // fetch length values from paper the page size
  std::list <std::pair <std::string, msrLength>> nameLengthPairsList;

  fetchLengthValuesFromPaperPageSize (
    paper,
    nameLengthPairsList );

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceIdentification ()) {
  // fetch boolean on/off values from the LPSR options group
  std::list <std::pair <std::string, Bool>> nameBooleanPairsList;

  fetchOnOffValuesFromLpsrOptionsGroup (
    paper,
    nameBooleanPairsList);

    fLilypondCodeStream <<
      "% --> The paper nameLengthValuePairsList contains:" <<
      std::endl;

    for (std::pair <std::string, Bool> thePair : nameBooleanPairsList) {
      std::string name = thePair.first;

      Bool value = thePair.second;

      fLilypondCodeStream <<
        "% " << name << " ---> " << value <<
        std::endl;
    } // for
  }
#endif // MF_TRACE_IS_ENABLED

  // compute fieldWidth // JMI 0.9.69 INEFFECTIVE
  size_t fieldWidth = 0;

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceIdentification ()) {
    fLilypondCodeStream <<
      "% ====> fieldWidth: " << fieldWidth <<
      std::endl;
  }
#endif // MF_TRACE_IS_ENABLED

  for (std::pair <std::string, msrLength> thePair : nameLengthPairsList) {
    std::string name = thePair.first;

    gLog << "======> name: " << name << std::endl;

    size_t nameSize = name.size ();

    gLog << "======> nameSize: " << nameSize << std::endl;

    if (nameSize > fieldWidth) {
      fieldWidth = nameSize;
    }

    gLog << "======> fieldWidth: " << fieldWidth << std::endl << std::endl;
  } // for

  // this variable will be reset and set if relevant
  // by the following 'generate*()' methods
  Bool
    outputHasBeenProduced (false);

  // page size
  generatePaperPageSize (
    pageLayout,
    defaultLengthUnit,
    fieldWidth,
    outputHasBeenProduced);

  // separator if relevant
  if (outputHasBeenProduced) {
    fLilypondCodeStream << std::endl;
  }

  // margins
  generatePaperMargins (
    pageLayout,
    defaultLengthUnit,
    fieldWidth,
    outputHasBeenProduced);

  // separator if relevant
  if (outputHasBeenProduced) {
    fLilypondCodeStream << std::endl;
  }

  // indents
  generatePaperIndents (
    paper,
    defaultLengthUnit,
    fieldWidth,
    outputHasBeenProduced);

  // separator if relevant
  if (outputHasBeenProduced) {
    fLilypondCodeStream << std::endl;
  }

  // spaces
  generatePaperSpaces (
    paper,
    defaultLengthUnit,
    fieldWidth,
    outputHasBeenProduced);

  // separator if relevant
  if (outputHasBeenProduced) {
    fLilypondCodeStream << std::endl;
  }

  // counts
  generatePaperCounts (
    paper,
    defaultLengthUnit,
    fieldWidth,
    outputHasBeenProduced);

  // separator if relevant
  if (outputHasBeenProduced) {
    fLilypondCodeStream << std::endl;
  }

  // boolean
  generatePaperBooleans (
    paper,
    fieldWidth,
    outputHasBeenProduced);

  // separator if relevant
  if (outputHasBeenProduced) {
    fLilypondCodeStream << std::endl;
  }

  // headers and footers
  generatePaperHeadersAndFooters (
    paper,
    defaultLengthUnit,
    fieldWidth,
    outputHasBeenProduced);

  --gIndenter;

  fLilypondCodeStream <<
    '}';

  if (gGlobalLpsr2lilypondOahGroup->getCommentLilypondStructureBasics ()) {
    fLilypondCodeStream <<
      " % paper";
  }

  fLilypondCodeStream <<
    std::endl << std::endl;
}

//________________________________________________________________________
void lpsr2lilypondTranslator::fetchLengthValuesFromPaperPageSize (
  const S_lpsrPaper& paper,
  std::list <std::pair <std::string, msrLength>>&
                     nameLengthValuePairsList)
{
  Bool
    generateCommentedOutVariables =
      gGlobalLpsr2lilypondOahGroup->
        getGenerateCommentedOutVariables ();

  // paper height
  Bool
    paperHeightHasBeenSet =
      gLpsrOahGroup->
        getPaperHeightAtom ()->
          getSelected ();

  Bool
    doGeneratePaperHeight =
      paperHeightHasBeenSet || generateCommentedOutVariables;

  if (doGeneratePaperHeight) {
    msrLength
      paperHeight =
        gLpsrOahGroup->getPaperHeight ();

    nameLengthValuePairsList.push_back (
      std::make_pair (
        "paper-height",
        paperHeight));
  }

  // paper width
  Bool
    paperWidthHasBeenSet =
      gLpsrOahGroup->
        getPaperWidthAtom ()->
          getSelected ();

  Bool
    doGeneratePaperWidth =
      paperWidthHasBeenSet || generateCommentedOutVariables;

  if (doGeneratePaperWidth) {
    msrLength
      paperWidth =
        gLpsrOahGroup->getPaperWidth ();

    nameLengthValuePairsList.push_back (
      std::make_pair (
        "paper-width",
        paperWidth));
  }
}

void lpsr2lilypondTranslator::fetchOnOffValuesFromLpsrOptionsGroup (
  const S_lpsrPaper& paper,
  std::list <std::pair <std::string, Bool>>&
                     nameBooleanValuePairsList)
{
  // ragged-last
  if (gLpsrOahGroup->getRaggedLastAtom ()->getSelected ()) {
    mfOnOffKind
      raggedLast =
        paper->getRaggedLast ();

    switch (raggedLast) {
      case mfOnOffKind::kMfOnOffUnknown:
        break;

      case mfOnOffKind::kMfOnOffOn:
      case mfOnOffKind::kMfOnOffOff:
        nameBooleanValuePairsList.push_back (
          std::make_pair (
            "ragged-last",
            mfOnOffKindAsBool (raggedLast)));
        break;
    } // switch
  }

  // ragged-bottom
  if (gLpsrOahGroup->getRaggedBottomAtom ()->getSelected ()) {
    mfOnOffKind
      raggedBottom =
        paper->getRaggedBottom ();

    switch (raggedBottom) {
      case mfOnOffKind::kMfOnOffUnknown:
        break;

      case mfOnOffKind::kMfOnOffOn:
      case mfOnOffKind::kMfOnOffOff:
        nameBooleanValuePairsList.push_back (
          std::make_pair (
            "ragged-bottom",
            mfOnOffKindAsBool (raggedBottom)));
        break;
    } // switch
  }

  // ragged-last-bottom
  if (gLpsrOahGroup->getRaggedLastBottomAtom ()->getSelected ()) {
    mfOnOffKind
      raggedLastBottom =
        paper->getRaggedLastBottom ();

    switch (raggedLastBottom) {
      case mfOnOffKind::kMfOnOffUnknown:
        break;

      case mfOnOffKind::kMfOnOffOn:
      case mfOnOffKind::kMfOnOffOff:
        nameBooleanValuePairsList.push_back (
          std::make_pair (
            "ragged-last-bottom",
            mfOnOffKindAsBool (raggedLastBottom)));
        break;
    } // switch
  }

  // ragged-right
  if (gLpsrOahGroup->getRaggedRightAtom ()->getSelected ()) {
    mfOnOffKind
      raggedRight =
        paper->getRaggedRight ();

    switch (raggedRight) {
      case mfOnOffKind::kMfOnOffUnknown:
        break;

      case mfOnOffKind::kMfOnOffOn:
      case mfOnOffKind::kMfOnOffOff:
        nameBooleanValuePairsList.push_back (
          std::make_pair (
            "ragged-right",
            mfOnOffKindAsBool (raggedRight)));
        break;
    } // switch
  }
}

//________________________________________________________________________
void lpsr2lilypondTranslator::generatePaperPageSize (
  const S_msrPageLayout& pageLayout,
  msrLengthUnitKind      defaultLengthUnit,
  int                    fieldWidth,
  Bool&                  outputHasBeenProduced)
{
  Bool
    generateCommentedOutVariables =
      gGlobalLpsr2lilypondOahGroup->
        getGenerateCommentedOutVariables ();

  // reset outputHasBeenProduced
  outputHasBeenProduced = false;

  // paper height
  Bool
    paperHeightHasBeenSet =
      gLpsrOahGroup->
        getPaperHeightAtom ()->
          getSelected ();

  Bool
    doGeneratePaperHeight =
      paperHeightHasBeenSet || generateCommentedOutVariables;

  if (doGeneratePaperHeight) {
    if (generateCommentedOutVariables) {
      fLilypondCodeStream << "% ";
    }

    msrLength
      paperHeight =
        gLpsrOahGroup->getPaperHeight ();

    fLilypondCodeStream <<
      std::left <<
      std::setw (fieldWidth) <<
      "paper-height" << " = " <<
      std::setprecision (3) << paperHeight.getLengthValue () <<
      lengthUnitAsLilypondString (paperHeight.getLengthUnitKind ()) <<
      std::endl;

    outputHasBeenProduced = true;
  }

  // paper width
  Bool
    paperWidthHasBeenSet =
      gLpsrOahGroup->
        getPaperWidthAtom ()->
          getSelected ();

  Bool
    doGeneratePaperWidth =
      paperWidthHasBeenSet || generateCommentedOutVariables;

  if (doGeneratePaperWidth) {
    if (generateCommentedOutVariables) {
      fLilypondCodeStream << "% ";
    }

    msrLength
      paperWidth =
        gLpsrOahGroup->getPaperWidth ();

    fLilypondCodeStream <<
      std::left <<
      std::setw (fieldWidth) <<
      "paper-width" << " = " <<
      std::setprecision (3) << paperWidth.getLengthValue () <<
      lengthUnitAsLilypondString (paperWidth.getLengthUnitKind ()) <<
      std::endl;

    outputHasBeenProduced = true;
  }
}

//________________________________________________________________________
void lpsr2lilypondTranslator::generatePaperMargins (
  const S_msrPageLayout& pageLayout,
  msrLengthUnitKind      defaultLengthUnit,
  int                    fieldWidth,
  Bool&                  outputHasBeenProduced)
{
  Bool
    generateCommentedOutVariables =
      gGlobalLpsr2lilypondOahGroup->
        getGenerateCommentedOutVariables ();

  // reset outputHasBeenProduced
  outputHasBeenProduced = false;

  // left margin
  Bool
    leftMarginHasBeenSet =
      gLpsrOahGroup->
        getPaperLeftMarginAtom ()->
          getSelected ();

  Bool
    doGenerateLeftMargin =
      leftMarginHasBeenSet || generateCommentedOutVariables;

  if (doGenerateLeftMargin) {
    if (generateCommentedOutVariables) {
      fLilypondCodeStream << "% ";
    }

    msrLength
      leftMargin =
        gLpsrOahGroup->getPaperLeftMargin ();

    fLilypondCodeStream <<
      std::left <<
      std::setw (fieldWidth) <<
      "left-margin" << " = " <<
      std::setprecision (3) << leftMargin.getLengthValue () <<
      lengthUnitAsLilypondString (leftMargin.getLengthUnitKind ()) <<
      std::endl;

    outputHasBeenProduced = true;
  }

  // right margin
  Bool
    rightMarginHasBeenSet =
      gLpsrOahGroup->
        getPaperRightMarginAtom ()->
          getSelected ();

  Bool
    doGenerateRightMargin =
      rightMarginHasBeenSet || generateCommentedOutVariables;

  if (doGenerateRightMargin) {
    if (generateCommentedOutVariables) {
      fLilypondCodeStream << "% ";
    }

    msrLength
      rightMargin =
        gLpsrOahGroup->getPaperRightMargin ();

    fLilypondCodeStream <<
      std::left <<
      std::setw (fieldWidth) <<
      "right-margin" << " = " <<
      std::setprecision (3) << rightMargin.getLengthValue () <<
      lengthUnitAsLilypondString (rightMargin.getLengthUnitKind ()) <<
      std::endl;

    outputHasBeenProduced = true;
  }

  // top margin
  Bool
    topMarginHasBeenSet =
      gLpsrOahGroup->
        getPaperTopMarginAtom ()->
          getSelected ();

  Bool
    doGenerateTopMargin =
      topMarginHasBeenSet || generateCommentedOutVariables;

  if (doGenerateTopMargin) {
    if (generateCommentedOutVariables) {
      fLilypondCodeStream << "% ";
    }

    msrLength
      topMargin =
        gLpsrOahGroup->getPaperTopMargin ();

    fLilypondCodeStream <<
      std::left <<
      std::setw (fieldWidth) <<
      "top-margin" << " = " <<
      std::setprecision (3) << topMargin.getLengthValue () <<
      lengthUnitAsLilypondString (topMargin.getLengthUnitKind ()) <<
      std::endl;

    outputHasBeenProduced = true;
  }

  // bottom margin
  Bool
    bottomMarginHasBeenSet =
      gLpsrOahGroup->
        getPaperBottomMarginAtom ()->
          getSelected ();

  Bool
    doGenerateBottomMargin =
      bottomMarginHasBeenSet || generateCommentedOutVariables;

  if (doGenerateBottomMargin) {
    if (generateCommentedOutVariables) {
      fLilypondCodeStream << "% ";
    }

    msrLength
      bottomMargin =
        gLpsrOahGroup->getPaperBottomMargin ();

    fLilypondCodeStream <<
      std::left <<
      std::setw (fieldWidth) <<
      "bottom-margin" << " = " <<
      std::setprecision (3) << bottomMargin.getLengthValue () <<
      lengthUnitAsLilypondString (bottomMargin.getLengthUnitKind ()) <<
      std::endl;

    outputHasBeenProduced = true;
  }
}

//________________________________________________________________________
void lpsr2lilypondTranslator::generatePaperIndents (
  const S_lpsrPaper& pagePaper,
  msrLengthUnitKind defaultLengthUnit,
  int               fieldWidth,
  Bool&             outputHasBeenProduced)
{
  // reset outputHasBeenProduced
  outputHasBeenProduced = false;

  // horizontal shift
  S_msrLength
    horizontalShift =
      pagePaper->getHorizontalShift ();

  if (! horizontalShift) {
    fLilypondCodeStream << "% ";
  }
  fLilypondCodeStream <<
    std::left <<
    std::setw (fieldWidth) <<
    "horizontal-shift" << " = ";
  if (horizontalShift) {
    fLilypondCodeStream <<
      std::setprecision (3) << horizontalShift->getLengthValue () <<
      lengthUnitAsLilypondString (horizontalShift->getLengthUnitKind ());
  }
  else {
    fLilypondCodeStream <<
      "0.0" <<
      lengthUnitAsLilypondString (defaultLengthUnit);
  }
  fLilypondCodeStream << std::endl;
  outputHasBeenProduced = true;

  // indent
  S_msrLength
    indent =
      pagePaper->getIndent ();

  if (! indent) {
    fLilypondCodeStream << "% ";
  }
  fLilypondCodeStream <<
    std::left <<
    std::setw (fieldWidth) <<
    "indent" << " = ";
  if (indent) {
    fLilypondCodeStream <<
      std::setprecision (3) << indent->getLengthValue () <<
      lengthUnitAsLilypondString (indent->getLengthUnitKind ());
  }
  else {
    fLilypondCodeStream <<
      "0.0" <<
      lengthUnitAsLilypondString (defaultLengthUnit);
  }
  fLilypondCodeStream << std::endl;
  outputHasBeenProduced = true;

  // short indent
  S_msrLength
    shortIndent =
      pagePaper->getShortIndent ();

  if (! shortIndent) {
    fLilypondCodeStream << "% ";
  }
  fLilypondCodeStream <<
    std::left <<
    std::setw (fieldWidth) <<
    "short-indent" << " = ";
  if (shortIndent) {
    fLilypondCodeStream <<
      std::setprecision (3) << shortIndent->getLengthValue () <<
      lengthUnitAsLilypondString (shortIndent->getLengthUnitKind ());
  }
  else {
    fLilypondCodeStream <<
      "0.0" <<
      lengthUnitAsLilypondString (defaultLengthUnit);
  }
  fLilypondCodeStream << std::endl;
  outputHasBeenProduced = true;
}

//________________________________________________________________________
void lpsr2lilypondTranslator::generatePaperSpaces (
  const S_lpsrPaper& pagePaper,
  msrLengthUnitKind defaultLengthUnit,
  int               fieldWidth,
  Bool&             outputHasBeenProduced)
{
  // reset outputHasBeenProduced
  outputHasBeenProduced = false;

  // markup system spacing padding
  S_msrLength
    markupSystemPpacingPadding =
      pagePaper->getMarkupSystemSpacingPadding ();

  if (! markupSystemPpacingPadding) {
    fLilypondCodeStream << "% ";
  }
  fLilypondCodeStream <<
    std::left <<
    std::setw (fieldWidth) <<
    "markup-system-spacing-padding" << " = ";
  if (markupSystemPpacingPadding) {
    fLilypondCodeStream <<
      std::setprecision (3) << markupSystemPpacingPadding->getLengthValue () <<
      lengthUnitAsLilypondString (markupSystemPpacingPadding->getLengthUnitKind ());
  }
  else {
    fLilypondCodeStream <<
      "0.0" <<
      lengthUnitAsLilypondString (defaultLengthUnit);
  }
  fLilypondCodeStream << std::endl;
  outputHasBeenProduced = true;

  // between system space
  S_msrLength
    betweenSystemSpace =
      pagePaper->getBetweenSystemSpace ();

  if (! betweenSystemSpace) {
    fLilypondCodeStream << "% ";
  }
  fLilypondCodeStream <<
    std::left <<
    std::setw (fieldWidth) <<
    "between-system-space" << " = ";
  if (betweenSystemSpace) {
    fLilypondCodeStream <<
      std::setprecision (3) << betweenSystemSpace->getLengthValue () <<
      lengthUnitAsLilypondString (betweenSystemSpace->getLengthUnitKind ());
  }
  else {
    fLilypondCodeStream <<
      "0.0" <<
      lengthUnitAsLilypondString (defaultLengthUnit);
  }
  fLilypondCodeStream << std::endl;
  outputHasBeenProduced = true;

  // page top space
  S_msrLength
    pageTopSpace =
      pagePaper->getPageTopSpacing ();

  if (! pageTopSpace) {
    fLilypondCodeStream << "% ";
  }
  fLilypondCodeStream <<
    std::left <<
    std::setw (fieldWidth) <<
    "page-top-space" << " = ";
  if (pageTopSpace) {
    fLilypondCodeStream <<
      std::setprecision (3) << pageTopSpace->getLengthValue () <<
      lengthUnitAsLilypondString (pageTopSpace->getLengthUnitKind ());
  }
  else {
    fLilypondCodeStream <<
      "0.0" <<
      lengthUnitAsLilypondString (defaultLengthUnit);
  }
  fLilypondCodeStream << std::endl;
  outputHasBeenProduced = true;
}

//________________________________________________________________________
void lpsr2lilypondTranslator::generatePaperCounts (
  const S_lpsrPaper& pagePaper,
  msrLengthUnitKind defaultLengthUnit,
  int               fieldWidth,
  Bool&             outputHasBeenProduced)
{
  // reset outputHasBeenProduced
  outputHasBeenProduced = false;

  // page count
  int
    pageCount =
      pagePaper->getPageCount ();

  if (pageCount < 0) {
    fLilypondCodeStream << "% ";
  }
  fLilypondCodeStream <<
    std::left <<
    std::setw (fieldWidth) <<
    "page-count = " <<
    pageCount <<
    std::endl;
  outputHasBeenProduced = true;

  // system count
  int
    systemCount =
      pagePaper->getSystemCount ();

  if (systemCount < 0) {
    fLilypondCodeStream << "% ";
  }
  fLilypondCodeStream <<
    std::left <<
    std::setw (fieldWidth) <<
    "system-count" << " = " <<
    systemCount <<
    std::endl;
  outputHasBeenProduced = true;
}

//________________________________________________________________________
void lpsr2lilypondTranslator::generatePaperBooleans (
  const S_lpsrPaper& pagePaper,
  int               fieldWidth,
  Bool&             outputHasBeenProduced)
{
  // reset outputHasBeenProduced
  outputHasBeenProduced = false;

  // ragged last
  mfOnOffKind
    raggedLast =
      pagePaper->getRaggedLast ();

  switch (raggedLast) {
    case mfOnOffKind::kMfOnOffUnknown:
      break;

    case mfOnOffKind::kMfOnOffOn:
    case mfOnOffKind::kMfOnOffOff:
      fLilypondCodeStream <<
        std::left <<
        std::setw (fieldWidth) <<
        "ragged-last" << " = ";
      if (mfOnOffKindAsBool (raggedLast)) {
        fLilypondCodeStream << "##t";
      }
      else {
        fLilypondCodeStream << "##f";
      }

      outputHasBeenProduced = true;
      break;
  } // switch

  // ragged bottom
  mfOnOffKind
    raggedBottom =
      pagePaper->getRaggedBottom ();

  switch (raggedBottom) {
    case mfOnOffKind::kMfOnOffUnknown:
      break;

    case mfOnOffKind::kMfOnOffOn:
    case mfOnOffKind::kMfOnOffOff:
      fLilypondCodeStream <<
        std::left <<
        std::setw (fieldWidth) <<
        "ragged-bottom" << " = ";
      if (mfOnOffKindAsBool (raggedBottom)) {
        fLilypondCodeStream << "##t";
      }
      else {
        fLilypondCodeStream << "##f";
      }
      fLilypondCodeStream << std::endl;

      outputHasBeenProduced = true;
      break;
  } // switch

  // ragged last bottom
  mfOnOffKind
    raggedLastBottom =
      pagePaper->getRaggedLastBottom ();

  switch (raggedLastBottom) {
    case mfOnOffKind::kMfOnOffUnknown:
      break;

    case mfOnOffKind::kMfOnOffOn:
    case mfOnOffKind::kMfOnOffOff:
      fLilypondCodeStream <<
        std::left <<
        std::setw (fieldWidth) <<
        "ragged-last-bottom" << " = ";
      if (mfOnOffKindAsBool (raggedLastBottom)) {
        fLilypondCodeStream << "##t";
      }
      else {
        fLilypondCodeStream << "##f";
      }
      fLilypondCodeStream << std::endl;

      outputHasBeenProduced = true;
      break;
  } // switch

  // ragged right
  mfOnOffKind
    raggedRight =
      pagePaper->getRaggedRight ();

  switch (raggedRight) {
    case mfOnOffKind::kMfOnOffUnknown:
      break;

    case mfOnOffKind::kMfOnOffOn:
    case mfOnOffKind::kMfOnOffOff:
      fLilypondCodeStream <<
        std::left <<
        std::setw (fieldWidth) <<
        "ragged-right" << " = ";
      if (mfOnOffKindAsBool (raggedRight)) {
        fLilypondCodeStream << "##t";
      }
      else {
        fLilypondCodeStream << "##f";
      }
      fLilypondCodeStream << std::endl;

      outputHasBeenProduced = true;
      break;
  } // switch
}

//________________________________________________________________________
void lpsr2lilypondTranslator::generatePaperHeadersAndFooters (
  const S_lpsrPaper& pagePaper,
  msrLengthUnitKind defaultLengthUnit,
  int               fieldWidth,
  Bool&             outputHasBeenProduced)
{
  // reset outputHasBeenProduced
  outputHasBeenProduced = false;

  // odd header markup
  std::string oddHeaderMarkup =
    pagePaper->getOddHeaderMarkup ();

  if (! oddHeaderMarkup.empty ()) {
    fLilypondCodeStream <<
      std::left <<
      std::setw (fieldWidth) <<
      "oddHeaderMarkup" << " = " <<
      oddHeaderMarkup <<
      std::endl;
  }
  else {
    fLilypondCodeStream <<
      "% oddHeaderMarkup = \"\"" <<
      std::endl;
  }
  outputHasBeenProduced = true;

  // even header markup
  std::string evenHeaderMarkup =
    pagePaper->getEvenHeaderMarkup ();

  if (! evenHeaderMarkup.empty ()) {
    fLilypondCodeStream <<
      std::left <<
      std::setw (fieldWidth) <<
      "evenHeaderMarkup" << " = " <<
      evenHeaderMarkup <<
      std::endl;
  }
  else {
    fLilypondCodeStream <<
      "% evenHeaderMarkup = \"\"" <<
      std::endl;
  }
  outputHasBeenProduced = true;

  // odd footer markup
  std::string oddFooterMarkup =
    pagePaper->getOddFooterMarkup ();

  if (! oddFooterMarkup.empty ()) {
    fLilypondCodeStream <<
      std::left <<
      std::setw (fieldWidth) <<
      "oddFooterMarkup" << " = " <<
      oddFooterMarkup <<
      std::endl;
  }
  else {
    fLilypondCodeStream <<
      "% oddFooterMarkup = \"\"" <<
      std::endl;
  }
  outputHasBeenProduced = true;

  // even footer markup
  std::string evenFooterMarkup =
    pagePaper->getEvenFooterMarkup ();

  if (! evenFooterMarkup.empty ()) {
    fLilypondCodeStream <<
      std::left <<
      std::setw (fieldWidth) <<
      "evenFooterMarkup" << " = " <<
      evenFooterMarkup <<
      std::endl;
  }
  else {
    fLilypondCodeStream <<
      "% evenFooterMarkup = \"\"" <<
      std::endl;
  }
  outputHasBeenProduced = true;
}

//________________________________________________________________________
void lpsr2lilypondTranslator::visitStart (S_lpsrPaper& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateLpsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateLpsrVisitingInformation) {
      std::stringstream ss;

      ss <<
        "% --> Start visiting lpsrPaper" <<
        ", line " << elt->getInputLineNumber () <<
      std::endl;

      if (traceLpsrVisitors) {
        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }

      if (generateLpsrVisitingInformation) {
        fLilypondCodeStream << ss.str ();
      }
    }
  }
#endif // MF_TRACE_IS_ENABLED

  if (gGlobalLpsr2lilypondOahGroup->getNoPaperBlock ()) {
    return;
  }

  if (! fOnGoingBookPartBlock) {
    generatePaper (elt);
  }
}

void lpsr2lilypondTranslator::visitEnd (S_lpsrPaper& elt) // superflous ??? JMI
{
#ifdef MF_TRACE_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateLpsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateLpsrVisitingInformation) {
      std::stringstream ss;

      ss <<
        "% --> End visiting lpsrPaper" <<
        ", line " << elt->getInputLineNumber () <<
      std::endl;

      if (traceLpsrVisitors) {
        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }

      if (generateLpsrVisitingInformation) {
        fLilypondCodeStream << ss.str ();
      }
    }
  }
#endif // MF_TRACE_IS_ENABLED

  if (gGlobalLpsr2lilypondOahGroup->getNoPaperBlock ()) {
    return;
  }
}

//________________________________________________________________________
void lpsr2lilypondTranslator::visitStart (S_lpsrLayout& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateLpsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateLpsrVisitingInformation) {
      std::stringstream ss;

      ss <<
        "% --> Start visiting lpsrLayout" <<
        ", line " << elt->getInputLineNumber () <<
      std::endl;

      if (traceLpsrVisitors) {
        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }

      if (generateLpsrVisitingInformation) {
        fLilypondCodeStream << ss.str ();
      }
    }
  }
#endif // MF_TRACE_IS_ENABLED

  fLilypondCodeStream <<
    "\\layout {" <<
    std::endl;

  ++gIndenter; // decremented in visitEnd (S_lpsrLayout& elt)

  // score context?
  if (! gGlobalLpsr2lilypondOahGroup->getNoLayoutScoreContext ()) {
    fLilypondCodeStream <<
      cLilypondLayoutContextScoreOpener <<
      std::endl;

      ++gIndenter;

      fLilypondCodeStream <<
        "autoBeaming = ##f % to display tuplets brackets" <<
      std::endl;

    --gIndenter;

    // generate the end of the voice definition
    fLilypondCodeStream <<
      cLilypondLayoutContextScoreCloser;

    if (gGlobalLpsr2lilypondOahGroup->getCommentLilypondStructureBasics ()) {
      fLilypondCodeStream <<
        " % end of score context";
    }

    fLilypondCodeStream << std::endl;
  }

  // voice context?
  if (! gGlobalLpsr2lilypondOahGroup->getNoLayoutVoiceContext ()) {
    fLilypondCodeStream <<
      cLilypondLayoutContextVoiceOpener <<
      std::endl;
  }

  if (gGlobalLpsr2lilypondOahGroup->getAmbitusEngraver ()) {
    fLilypondCodeStream <<
        gTab << "\\consists \"Ambitus_engraver\"" <<
        std::endl;
  }

  if (gGlobalLpsr2lilypondOahGroup->getCustosEngraver ()) {
    fLilypondCodeStream <<
        gTab << "\\consists \"Custos_engraver\"" <<
        std::endl;
  }

  if (! gGlobalLpsr2lilypondOahGroup->getNoLayoutVoiceContext ()) {
    fLilypondCodeStream <<
      cLilypondLayoutContextVoiceCloser;

    if (gGlobalLpsr2lilypondOahGroup->getCommentLilypondStructureBasics ()) {
      fLilypondCodeStream <<
        " % end of voice context";
    }

    fLilypondCodeStream << std::endl;
  }

  // ChordNames context
  if (fVisitedLpsrScore->getJazzChordsDisplayIsNeeded ()) {
    gIndenter.indentMultiLineString (
R"(\context {
  \ChordNames
  chordNameExceptions = \chExceptions
  slashChordSeparator = \markup {
    % the \hspace commands simulate kerning
    \hspace #-.7
    \fontsize #1 \lower #1.2 \rotate #-10 '/'
    \hspace #-.1
  }
  chordNoteNamer = #lower-extension
})",
      fLilypondCodeStream);
  }
}

void lpsr2lilypondTranslator::visitEnd (S_lpsrLayout& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateLpsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateLpsrVisitingInformation) {
      std::stringstream ss;

      ss <<
        "% --> End visiting lpsrLayout" <<
        ", line " << elt->getInputLineNumber () <<
      std::endl;

      if (traceLpsrVisitors) {
        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }

      if (generateLpsrVisitingInformation) {
        fLilypondCodeStream << ss.str ();
      }
    }
  }
#endif // MF_TRACE_IS_ENABLED

  // should we set the repeat brackets type?
  if (gGlobalLpsr2lilypondOahGroup->getRepeatBrackets ()) {
    fLilypondCodeStream <<
      cLilypondLayoutContextScoreOpener <<
      std::endl;

    ++gIndenter;

    fLilypondCodeStream <<
      "% defaultBarType = #\"!\"" <<
      std::endl <<
      "startRepeatType = #\"[|:\"" <<
      std::endl <<
      "endRepeatType = #\":|]\"" <<
      std::endl <<
      "doubleRepeatType = #\":|][|:\"" <<
      std::endl;

    --gIndenter;

    fLilypondCodeStream <<
      cLilypondLayoutContextScoreCloser <<
      std::endl;
  }

  // should we set the bars number size?
  S_oahFloatAtom
    barNumbersSizeAtom =
      gGlobalLpsr2lilypondOahGroup->getBarNumbersSizeAtom ();

  if (barNumbersSizeAtom->getSelected ()) {
    fLilypondCodeStream <<
      cLilypondLayoutContextScoreOpener <<
      std::endl;

    ++gIndenter;

    fLilypondCodeStream <<
      "\\override BarNumber.font-size = " <<
      barNumbersSizeAtom->getFloatVariable () <<
      std::endl;

    --gIndenter;

    fLilypondCodeStream <<
      cLilypondLayoutContextScoreCloser <<
      std::endl;
  }

  // should we use an arpeggio engraver?
  if (gGlobalLpsr2lilypondOahGroup->getConnectArpeggios ()) {
    fLilypondCodeStream <<
      cLilypondLayoutContextScoreOpener <<
      std::endl;

    ++gIndenter;

    fLilypondCodeStream <<
      "\\consists \"Span_arpeggio_engraver\"" <<
      std::endl;

    --gIndenter;

    fLilypondCodeStream <<
      cLilypondLayoutContextStaffCloser <<
      " % end of staff context";

    if (gGlobalLpsr2lilypondOahGroup->getCommentLilypondStructureBasics ()) {
      fLilypondCodeStream <<
        " % end of layout";
    }

    fLilypondCodeStream << std::endl;
  }

  --gIndenter; // incremented in visitStart (S_lpsrLayout& elt)

  fLilypondCodeStream <<
    '}';

  if (gGlobalLpsr2lilypondOahGroup->getCommentLilypondStructureBasics ()) {
    fLilypondCodeStream <<
      " % layout";
  }

  fLilypondCodeStream <<
    std::endl << std::endl;
}

//________________________________________________________________________
void lpsr2lilypondTranslator::visitStart (S_lpsrBookBlock& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateLpsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateLpsrVisitingInformation) {
      std::stringstream ss;

      ss <<
        "% --> Start visiting lpsrBookBlock" <<
        ", line " << elt->getInputLineNumber () <<
      std::endl;

      if (traceLpsrVisitors) {
        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }

      if (generateLpsrVisitingInformation) {
        fLilypondCodeStream << ss.str ();
      }
    }
  }
#endif // MF_TRACE_IS_ENABLED

  if (gGlobalLpsr2lilypondOahGroup->getNoBookBlock ()) {
    return;
  }

  fLilypondCodeStream <<
    "\\book {" <<
    std::endl << std::endl;

  ++gIndenter; // decremented in visitEnd (S_lpsrBookBlock& elt)

  fOnGoingBookPartBlock = true;
}

void lpsr2lilypondTranslator::visitEnd (S_lpsrBookBlock& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateLpsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateLpsrVisitingInformation) {
      std::stringstream ss;

      ss <<
        "% --> End visiting lpsrBookBlock" <<
        ", line " << elt->getInputLineNumber () <<
      std::endl;

      if (traceLpsrVisitors) {
        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }

      if (generateLpsrVisitingInformation) {
        fLilypondCodeStream << ss.str ();
      }
    }
  }
#endif // MF_TRACE_IS_ENABLED

  if (gGlobalLpsr2lilypondOahGroup->getNoBookBlock ()) {
    return;
  }

  --gIndenter; // incremented in visitStart (S_lpsrBookBlock& elt)

  fLilypondCodeStream << '}';

  if (gGlobalLpsr2lilypondOahGroup->getCommentLilypondStructureBasics ()) {
    fLilypondCodeStream <<
      " % end of book";
  }

  else {
  }

  fLilypondCodeStream << std::endl;

  fOnGoingBookPartBlock = false;
}

//________________________________________________________________________
void lpsr2lilypondTranslator::visitStart (S_lpsrScoreBlock& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateLpsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateLpsrVisitingInformation) {
      std::stringstream ss;

      ss <<
        "% --> Start visiting lpsrScoreBlock" <<
        ", line " << elt->getInputLineNumber () <<
      std::endl;

      if (traceLpsrVisitors) {
        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }

      if (generateLpsrVisitingInformation) {
        fLilypondCodeStream << ss.str ();
      }
    }
  }
#endif // MF_TRACE_IS_ENABLED

  fLilypondCodeStream <<
    "\\score {" <<
    std::endl;

  ++gIndenter; // decremented in visitEnd (S_lpsrScoreBlock& elt)

  fOnGoingScoreBlock = true;
}

void lpsr2lilypondTranslator::visitEnd (S_lpsrScoreBlock& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateLpsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateLpsrVisitingInformation) {
      std::stringstream ss;

      ss <<
        "% --> End visiting lpsrScoreBlock" <<
        ", line " << elt->getInputLineNumber () <<
      std::endl;

      if (traceLpsrVisitors) {
        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }

      if (generateLpsrVisitingInformation) {
        fLilypondCodeStream << ss.str ();
      }
    }
  }
#endif // MF_TRACE_IS_ENABLED

  --gIndenter; // incremented in visitStart (S_lpsrScoreBlock& elt)

  fLilypondCodeStream << '}';

  if (gGlobalLpsr2lilypondOahGroup->getCommentLilypondStructureBasics ()) {
    fLilypondCodeStream <<
      " % end of score";
  }

  fLilypondCodeStream <<
    std::endl << std::endl; // JMI

  fOnGoingScoreBlock = false;
}

//________________________________________________________________________
void lpsr2lilypondTranslator::visitStart (S_lpsrBookPartBlock& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateLpsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateLpsrVisitingInformation) {
      std::stringstream ss;

      ss <<
        "% --> Start visiting lpsrBookPartBlock" <<
        ", line " << elt->getInputLineNumber () <<
      std::endl;

      if (traceLpsrVisitors) {
        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }

      if (generateLpsrVisitingInformation) {
        fLilypondCodeStream << ss.str ();
      }
    }
  }
#endif // MF_TRACE_IS_ENABLED

  fLilypondCodeStream <<
    "\\bookpart {" <<
    std::endl;

  ++gIndenter; //  // decremented in visitEnd (S_lpsrBookPartBlock& elt)

  fOnGoingBookPartBlock = true;
}

void lpsr2lilypondTranslator::visitEnd (S_lpsrBookPartBlock& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateLpsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateLpsrVisitingInformation) {
      std::stringstream ss;

      ss <<
        "% --> End visiting lpsrBookPartBlock" <<
        ", line " << elt->getInputLineNumber () <<
      std::endl;

      if (traceLpsrVisitors) {
        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }

      if (generateLpsrVisitingInformation) {
        fLilypondCodeStream << ss.str ();
      }
    }
  }
#endif // MF_TRACE_IS_ENABLED

  --gIndenter; // incremented in visitStart (S_lpsrBookPartBlock& elt)

  fLilypondCodeStream <<
    '}';

  if (gGlobalLpsr2lilypondOahGroup->getCommentLilypondStructureBasics ()) {
    fLilypondCodeStream <<
      " % bookpart";
  }

  fLilypondCodeStream <<
    std::endl << std::endl;

  fOnGoingBookPartBlock = false;
}

//________________________________________________________________________
void lpsr2lilypondTranslator::visitStart (S_lpsrParallelMusicBLock& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateLpsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateLpsrVisitingInformation) {
      std::stringstream ss;

      ss <<
        "% --> Start visiting lpsrParallelMusicBLock" <<
        ", line " << elt->getInputLineNumber () <<
      std::endl;

      if (traceLpsrVisitors) {
        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }

      if (generateLpsrVisitingInformation) {
        fLilypondCodeStream << ss.str ();
      }
    }
  }
#endif // MF_TRACE_IS_ENABLED

  fNumberOfPartGroupBlocks =
    elt->
      getParallelMusicBLockPartGroupBlocks ().size ();

  if (fNumberOfPartGroupBlocks > 0) {
    fLilypondCodeStream << cLilypondParallelMusicOpener;

    if (gGlobalLpsr2lilypondOahGroup->getCommentLilypondStructureBasics ()) {
      fLilypondCodeStream <<
        " % start of parallel music";
    }

    fLilypondCodeStream << std::endl;

    ++gIndenter; //  // decremented in visitEnd (S_lpsrParallelMusicBLock& elt)
  }

  fCurrentParallelMusicBLock = elt;
}

void lpsr2lilypondTranslator::visitEnd (S_lpsrParallelMusicBLock& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateLpsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateLpsrVisitingInformation) {
      std::stringstream ss;

      ss <<
        "% --> End visiting lpsrParallelMusicBLock" <<
        ", line " << elt->getInputLineNumber () <<
      std::endl;

      if (traceLpsrVisitors) {
        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }

      if (generateLpsrVisitingInformation) {
        fLilypondCodeStream << ss.str ();
      }
    }
  }
#endif // MF_TRACE_IS_ENABLED

  if (fNumberOfPartGroupBlocks > 0) {
    --gIndenter; // incremented in visitStart (S_lpsrParallelMusicBLock& elt)

    fLilypondCodeStream <<
      std::endl <<
      cLilypondParallelMusicCloser;

    if (gGlobalLpsr2lilypondOahGroup->getCommentLilypondStructureBasics ()) {
      fLilypondCodeStream <<
        " % end of parallel music";
    }

    fLilypondCodeStream << std::endl << std::endl;
  }
}

//________________________________________________________________________
void lpsr2lilypondTranslator::visitStart (S_lpsrPartGroupBlock& elt)
{
  // fetch part group
  S_msrPartGroup
    partGroup =
      elt->getPartGroup ();

#ifdef MF_TRACE_IS_ENABLED
  {
    Bool
      tracePartGroups =
        gTraceOahGroup->
          getTracePartGroups (),
      traceLpsrVisitors =
        gLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateLpsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (tracePartGroups || traceLpsrVisitors || generateLpsrVisitingInformation) {
      std::stringstream ss;

      ss <<
        "% --> Start visiting lpsrPartGroupBlock for '" <<
        partGroup->asShortString () <<
        '\'' <<
        ", line " << elt->getInputLineNumber () <<
      std::endl;

      if (traceLpsrVisitors) {
        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }

      if (generateLpsrVisitingInformation) {
        fLilypondCodeStream << ss.str ();
      }
    }
  }
#endif // MF_TRACE_IS_ENABLED

  ++fPartGroupBlocksCounter;

  fNumberOfPartGroupBlockElements =
    elt -> getPartGroupBlockElements ().size ();

// JMI  fLilypondCodeStream << std::endl << std::endl << partGroup << std::endl << std::endl;

  msrPartGroupImplicitKind
    partGroupImplicitKind =
      partGroup->
        getPartGroupImplicitKind ();

  msrPartGroupSymbolKind
    partGroupSymbolKind =
      partGroup->
        getPartGroupSymbolKind ();

  msrPartGroupBarLineKind
    partGroupBarLineKind =
      partGroup->
        getPartGroupBarLineKind ();

  std::string
    partGroupName =
      partGroup->
        getPartGroupName (),

    partGroupAbbreviation =
      partGroup->
        getPartGroupAbbreviation (),

    partGroupInstrumentName =
      partGroup->
        getPartGroupInstrumentName ();

  // LPNR, page 567 jMI ???

//   fLilypondCodeStream << std::endl; // JMI 0.9.74

  switch (partGroupImplicitKind) {
    case msrPartGroupImplicitKind::kPartGroupImplicitOuterMostYes:
      // don't generate the implicit outer-most part group block
      if (gGlobalLpsr2lilypondOahGroup->getCommentLilypondStructureBasics ()) {
        fLilypondCodeStream <<
          std::endl <<
          "% start of implicit part group block" <<
          std::endl;
      }
      break;

    case msrPartGroupImplicitKind::kPartGroupImplicitOuterMostNo:
      if (gGlobalLpsr2lilypondOahGroup->getCommentLilypondStructureBasics ()) {
        fLilypondCodeStream <<
          std::endl <<
          "% start of explicit part group block" <<
          std::endl;
      }

      // don't factorize '\\new' out,
      // to facilitate debugging
      switch (partGroupSymbolKind) {
        case msrPartGroupSymbolKind::kPartGroupSymbolNone:
          fLilypondCodeStream <<
            cLilypondNewStaffGroup;
          break;

        case msrPartGroupSymbolKind::kPartGroupSymbolBrace: // JMI
          switch (partGroupBarLineKind) {
            case msrPartGroupBarLineKind::kPartGroupBarLineYes:
              fLilypondCodeStream <<
                cLilypondNewPianoStaff;
              break;
            case msrPartGroupBarLineKind::kPartGroupBarLineNo:
              fLilypondCodeStream <<
                cLilypondNewGrandStaff;
              break;
          } // switch
          break;

        case msrPartGroupSymbolKind::kPartGroupSymbolBracket:
          switch (partGroupBarLineKind) {
            case msrPartGroupBarLineKind::kPartGroupBarLineYes:
              fLilypondCodeStream <<
                cLilypondNewStaffGroup;
              break;
            case msrPartGroupBarLineKind::kPartGroupBarLineNo:
              fLilypondCodeStream <<
                cLilypondNewChoirStaff;
              break;
          } // switch
          break;

        case msrPartGroupSymbolKind::kPartGroupSymbolLine:
          fLilypondCodeStream <<
            cLilypondNewStaffGroup;
          break;

        case msrPartGroupSymbolKind::kPartGroupSymbolSquare:
          fLilypondCodeStream <<
            cLilypondNewStaffGroup;
          break;
      } // switch

      if (gGlobalLpsr2lilypondOahGroup->getCommentLilypondStructureBasics ()) {
        fLilypondCodeStream <<
          " %{ partGroupImplicitKind: " <<
          partGroupImplicitKind <<
          " %} ";
      }


#ifdef MF_TRACE_IS_ENABLED
      if (gGlobalLpsr2lilypondOahGroup->getCommentLilypondPartGroups ()) {
         fLilypondCodeStream <<
          " %{ " <<
          partGroup->fetchPartGroupInformationForTrace () <<
          ", abs number: " <<
          partGroup->getPartGroupSequentialNumber () <<
          " %} ";
      }
#endif // MF_TRACE_IS_ENABLED

      // should a '\with' block be generated?
      Bool doGenerateAWithBlock (false);

      // generate the '\with' block if there's
      // a part group name or abbreviation to be generated
      if (
        ! partGroupName.empty ()
          ||
        ! partGroupAbbreviation.empty ()
      ) {
        doGenerateAWithBlock = true;
      }

      // generate the '\with' block
      // if the part group is not implicit
      switch (partGroupImplicitKind) {
        case msrPartGroupImplicitKind::kPartGroupImplicitOuterMostYes:
          break;
        case msrPartGroupImplicitKind::kPartGroupImplicitOuterMostNo:
          if (! partGroupName.empty ()) {
            doGenerateAWithBlock = true;
          }
          break;
      } // switch

      // generate the '\with' block
      // if the part group symbol is a line or square
      switch (partGroupSymbolKind) {
        case msrPartGroupSymbolKind::kPartGroupSymbolNone:
          break;

        case msrPartGroupSymbolKind::kPartGroupSymbolBrace: // JMI
          break;

        case msrPartGroupSymbolKind::kPartGroupSymbolBracket:
          break;

        case msrPartGroupSymbolKind::kPartGroupSymbolLine:
          doGenerateAWithBlock = true;
          break;

        case msrPartGroupSymbolKind::kPartGroupSymbolSquare:
          doGenerateAWithBlock = true;
          break;
      } // switch

      if (doGenerateAWithBlock) {
        fLilypondCodeStream <<
          std::endl <<
          "\\with " << cLilypondWithOpener <<
          std::endl;
      }

      ++gIndenter;

      if (doGenerateAWithBlock) { // JMI BLARK
        if (! partGroupName.empty ()) {
          fLilypondCodeStream <<
            "instrumentName = " <<
            nameAsLilypondString (partGroupName) <<
            std::endl;
        }
        if (! partGroupAbbreviation.empty ()) {
          fLilypondCodeStream <<
            "shortInstrumentName = " <<
            nameAsLilypondString (partGroupAbbreviation) <<
            std::endl;
        }
      }

      switch (partGroupSymbolKind) {
        case msrPartGroupSymbolKind::kPartGroupSymbolNone:
          break;

        case msrPartGroupSymbolKind::kPartGroupSymbolBrace: // JMI
          /*
           *
           * check whether individual part have instrument names JMI
           *
            if (! partGroupInstrumentName.empty ()) {
              fLilypondCodeStream << = cLilypondNewPianoStaff;
            }
            else {
              fLilypondCodeStream << = cLilypondNewGrandStaff;
            }
              */
          break;

        case msrPartGroupSymbolKind::kPartGroupSymbolBracket:
          break;

        case msrPartGroupSymbolKind::kPartGroupSymbolLine:
          fLilypondCodeStream <<
            "systemStartDelimiter = #'SystemStartBar" <<
            std::endl;
          break;

        case msrPartGroupSymbolKind::kPartGroupSymbolSquare:
          fLilypondCodeStream <<
            "systemStartDelimiter = #'SystemStartSquare" <<
            std::endl;
          break;
      } // switch

      --gIndenter;

      // generate the '\with' block ending
      // if the part group is not implicit
      if (doGenerateAWithBlock) {
        fLilypondCodeStream <<
          cLilypondWithCloser <<
          std::endl;
      }

//       fLilypondCodeStream << csvPartGroupOpener;

      if (gGlobalLpsr2lilypondOahGroup->getCommentLilypondStructureBasics ()) {
        fLilypondCodeStream <<
          " % start of part group block " <<
          partGroup->fetchPartGroupInformationForTraceWithoutEndOfLines ();
      }

      fLilypondCodeStream << std::endl;
      break;
  } // switch

  if (! partGroupInstrumentName.empty ()) { // JMI
    fLilypondCodeStream <<
      "instrumentName = \"" <<
      partGroupInstrumentName <<
      "\"" <<
      std::endl;
  }

  if (gGlobalLpsr2lilypondOahGroup->getConnectArpeggios ()) {
    fLilypondCodeStream <<
      "\\set PianoStaff.connectArpeggios = ##t" <<
      std::endl;
  }

/*
https://lilypond.org/doc/v2.25/Documentation/notation/arpeggio

Arpège distribué sur plusieurs voix
Affecter le graveur Span_arpeggio_engraver au contexte de la portée (Staff) permet de distribuer un arpège sur plusieurs voix.
\new Staff \with {
  \consists "Span_arpeggio_engraver"
}
\relative c' {
  \set Staff.connectArpeggios = ##t
  <<
    { <e' g>4\arpeggio <d f> <d f>2 }
    \\
    { <d, f>2\arpeggio <g b>2 }
  >>
}
*/

//   fLilypondCodeStream << std::endl;

//   if (elt->getPartGroupBlockElements ().size () > 1) {
//     ++gIndenter; //  // decremented in visitEnd (S_lpsrPartGroupBlock& elt)
//   }

  fLilypondCodeStream << std::endl; // JMI 0.9.72

  fLilypondCodeStream <<
    cLilypondStaffGroupOpener <<
    std::endl <<
    std::endl;

  // decremented in visitEnd (S_lpsrPartGroupBlock& elt)
  ++gIndenter;
}

void lpsr2lilypondTranslator::visitEnd (S_lpsrPartGroupBlock& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateLpsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateLpsrVisitingInformation) {
      std::stringstream ss;

      ss <<
        "% --> End visiting lpsrPartGroupBlock" <<
        ", line " << elt->getInputLineNumber () <<
      std::endl;

      if (traceLpsrVisitors) {
        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }

      if (generateLpsrVisitingInformation) {
        fLilypondCodeStream << ss.str ();
      }
    }
  }
#endif // MF_TRACE_IS_ENABLED

  // incremented in visitStart (S_lpsrPartGroupBlock& elt)
  --gIndenter;

  // fetch part group
  S_msrPartGroup
    partGroup =
      elt->getPartGroup ();

//   if (elt->getPartGroupBlockElements ().size () > 1) {
//     --gIndenter; // incremented in visitEnd (S_lpsrPartGroupBlock& elt)
//   }

  switch (partGroup->getPartGroupImplicitKind ()) {
    case msrPartGroupImplicitKind::kPartGroupImplicitOuterMostYes:
      // don't generate the implicit outer-most part group block
      if (gGlobalLpsr2lilypondOahGroup->getCommentLilypondStructureBasics ()) {
        fLilypondCodeStream <<
          " % end of implicit part group block " <<
          partGroup->fetchPartGroupInformationForTraceWithoutEndOfLines ();
      }

    case msrPartGroupImplicitKind::kPartGroupImplicitOuterMostNo:
//       fLilypondCodeStream << kPartGroupCloser;

      if (gGlobalLpsr2lilypondOahGroup->getCommentLilypondStructureBasics ()) {
        fLilypondCodeStream <<
          " % end of explicit part group block " <<
          partGroup->fetchPartGroupInformationForTraceWithoutEndOfLines ();
      }

      fLilypondCodeStream << std::endl;

      if (fPartGroupBlocksCounter != fNumberOfPartGroupBlocks) {
        fLilypondCodeStream << std::endl;
      }
      break;
  } // switch

  fLilypondCodeStream <<
    cLilypondPartGroupCloser <<
    std::endl;
}

//________________________________________________________________________
void lpsr2lilypondTranslator::visitStart (S_lpsrPartBlock& elt)
{
  // fetch part block's part
  S_msrPart part = elt->getPart ();

#ifdef MF_TRACE_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateLpsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateLpsrVisitingInformation) {
      std::stringstream ss;

      ss <<
        "% --> Start visiting lpsrPartBlock for '" <<
        part->asShortString () <<
        ", line " << elt->getInputLineNumber () <<
        std::endl;

      if (traceLpsrVisitors) {
        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }

      if (generateLpsrVisitingInformation) {
        fLilypondCodeStream << ss.str ();
      }
    }
  }
#endif // MF_TRACE_IS_ENABLED

  ++fPartGroupBlockElementsCounter;

  fNumberOfStaffBlocksElements =
    elt->getPartBlockElementsList ().size ();

  if (part->getPartStavesMap ().size () > 1) { // JMI
    // don't generate code here for a part with only one staff

    std::string
      partName =
        part->getPartPathLikeName (),
      partAbbreviation =
        part->getPartAbbreviation ();

 // JMI       gLog << "@@@@@@@@@@ partName: " << partName << ", partAbbreviation: " << partAbbreviation << std::endl;

        /*
    std::string
      partInstrumentName =  // JMI
        part->getPartInstrumentName (),
      partInstrumentAbbreviation =  // JMI
        part->getPartInstrumentAbbreviation ();
        */

    if (gGlobalLpsr2lilypondOahGroup->getCommentLilypondStructureBasics ()) {
      fLilypondCodeStream <<
        std::left <<
        std::setw (commentFieldWidth) <<
        cLilypondNewPianoStaff <<
        " % part " << part->fetchPartNameForTrace ();
    }
    else {
      fLilypondCodeStream <<
        cLilypondNewPianoStaff;
    }

    fLilypondCodeStream << std::endl;

    // generate the 'with' block beginning
    fLilypondCodeStream <<
      "\\with " << cLilypondWithOpener <<
      std::endl;

    ++gIndenter;

    if (! partName.empty ()) {
      fLilypondCodeStream <<
        "instrumentName = \"" <<
        partName <<
        "\"" <<
        std::endl;
    }
    if (! partAbbreviation.empty ()) {
      fLilypondCodeStream <<
        "shortInstrumentName = " <<
       nameAsLilypondString (partAbbreviation) <<
        std::endl;
    }

    if (gGlobalLpsr2lilypondOahGroup->getConnectArpeggios ()) {
      fLilypondCodeStream <<
        "connectArpeggios = ##t" <<
        std::endl;
    }

    --gIndenter;

    // generate the 'with' block ending
    fLilypondCodeStream <<
      cLilypondWithCloser <<
      std::endl;

    fLilypondCodeStream << // JMI ??? 0.9.62
      std::endl <<
      cLilypondPartOpener <<
      std::endl << std::endl;

    ++gIndenter;
  }
}

void lpsr2lilypondTranslator::visitEnd (S_lpsrPartBlock& elt)
{
  // fetch current part block's part
  S_msrPart
    part = elt->getPart ();

#ifdef MF_TRACE_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateLpsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateLpsrVisitingInformation) {
      std::stringstream ss;

      ss <<
        "% --> End visiting lpsrPartBlock" <<
        ", line " << elt->getInputLineNumber () <<
        std::endl;

      if (traceLpsrVisitors) {
        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }

      if (generateLpsrVisitingInformation) {
        fLilypondCodeStream << ss.str ();
      }
    }
  }
#endif // MF_TRACE_IS_ENABLED

  if (part->getPartStavesMap ().size () > 1) {
    --gIndenter;

    // don't generate a part with only one staff
    if (gGlobalLpsr2lilypondOahGroup->getCommentLilypondStructureBasics ()) {
      fLilypondCodeStream <<
        "% end of part blockpart " <<
        part->fetchPartNameForTrace ();
    }
    else {
      fLilypondCodeStream <<
        cLilypondPartCloser;
    }

    fLilypondCodeStream << std::endl;

    if (fPartGroupBlockElementsCounter != fNumberOfPartGroupBlockElements) {
      fLilypondCodeStream << std::endl;
    }
  }
}

//________________________________________________________________________
void lpsr2lilypondTranslator::visitStart (S_lpsrStaffBlock& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateLpsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateLpsrVisitingInformation) {
      std::stringstream ss;

      ss <<
        "% --> Start visiting lpsrStaffBlock" <<
        ", line " << elt->getInputLineNumber () <<
        std::endl;

      if (traceLpsrVisitors) {
        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }

      if (generateLpsrVisitingInformation) {
        fLilypondCodeStream << ss.str ();
      }
    }
  }
#endif // MF_TRACE_IS_ENABLED

  ++fStaffBlocksCounter;

  // get the staff
  S_msrStaff
    staff =
      elt->getStaff ();

  // generate the staff context
  switch (staff->getStaffKind ()) {
    case msrStaffKind::kStaffKindRegular:
      if (gGlobalLpsr2lilypondOahGroup->getJianpu ()) {
        fLilypondCodeStream << cLilypondNewJianpuStaff;
      }
      else {
        fLilypondCodeStream << cLilypondNewStaff;
      }
      break;

    case msrStaffKind::kStaffKindTablature:
      fLilypondCodeStream << cLilypondNewTabStaff;
      break;

    case msrStaffKind::kStaffKindHarmonies:
      fLilypondCodeStream << cLilypondNewHarmoniesStaff;
      break;

    case msrStaffKind::kStaffKindFiguredBass:
      fLilypondCodeStream << cLilypondNewFiguredBassStaff;
      break;

    case msrStaffKind::kStaffKindDrum:
      fLilypondCodeStream << cLilypondNewDrumStaff;
      break;

    case msrStaffKind::kStaffKindRythmic:
      fLilypondCodeStream << cLilypondNewRythmicStaff;
      break;
    } // switch

  fLilypondCodeStream <<
    " = \"" <<
    staff->getStaffPathLikeName () <<
    "\"";

  fLilypondCodeStream << std::endl;

  // generate the 'with' block beginning
  fLilypondCodeStream <<
    "\\with " << cLilypondWithOpener <<
    std::endl;

  ++gIndenter;

  // fetch part upLink
  S_msrPart
    staffUpLinkToPart =
      staff->getStaffUpLinkToPart ();

  // don't generate instrument names in the staves
  // if the containing part contains several of them
  if (staffUpLinkToPart->getPartStavesMap ().size () == 1) {
    // get the part upLink name to be used
    std::string
      partName =
        staffUpLinkToPart->
          getPartName ();

// JMI    gLog << "--> partName: \"" << partName << "\"" << std::endl;

    if (partName.empty ()) {
      partName =
        staffUpLinkToPart->
          getPartPathLikeName ();
    }

// JMI    gLog << "--> partName: \"" << partName << "\"" << std::endl;

    // generate the instrument name
    //* JMI BLARKBLARK
    if (
      ! partName.empty ()
        ||
      gLpsrOahGroup->
        getLpsrStavesInstrumentsNamesMapAtom ()->
          getSelected ()
    ) {
      fLilypondCodeStream <<
        "instrumentName = ";

      std::string staffInstrumentName = partName; // default value

      // fetch the staff instrument name
      if (
        gLpsrOahGroup->
          getLpsrStavesInstrumentsNamesMapAtom ()->
            getSelected ()
      ) {
        const std::map <std::string, std::string>&
          lpsrStavesInstrumentsNamesMap =
            gLpsrOahGroup->
              getLpsrStavesInstrumentsNamesMap ();

if (true)
        mfDisplayStringToStringMap ( // JMI 0.9.72 ???
          "--> lpsrStavesInstrumentsNamesMap",
          lpsrStavesInstrumentsNamesMap,
          gLog);

        std::string optionSuppliedInstrumentName;

        if (
          mfFetchValueFromStringToStringMap (
            partName,
            lpsrStavesInstrumentsNamesMap,
            optionSuppliedInstrumentName)
        ) {
          staffInstrumentName =
            optionSuppliedInstrumentName;
        }
      }

      // does the name contain hexadecimal end of lines?
      std::size_t found =
    // JMI    staffInstrumentName.find ("&#xd");
        staffInstrumentName.find ("\n");

      if (found == std::string::npos) {
        // no, escape quotes if any and generate the result
        fLilypondCodeStream <<
          generateStringAsLilypondMarkupOrDoubleQuotedString (
            staffInstrumentName) <<
          std::endl;
      }

      else {
        // yes, split the name into a chunks list
        // and generate a \markup{} // JMI ???
        fLilypondCodeStream <<
          std::endl <<
          generateMultilineMarkup (
            staffInstrumentName,
            lilypondMarkupColumnKind::kMarkupColumnKindLeftACentered) << // JMI ???
          std::endl;
      }
    }
    /* ??? JMI
    if (! partAbbreviation.empty ()) {
      fLilypondCodeStream <<
        "shortInstrumentName = " <<
        nameAsLilypondString (partAbbreviation) <<
        std::endl;
    }
    */

    // get the part upLink abbreviation display text to be used
    std::string partAbbreviation =
      staffUpLinkToPart->
        getPartAbbreviation ();

    if (partAbbreviation.empty ()) {
      partAbbreviation =
        staffUpLinkToPart->
          getPartAbbreviation ();
    }

    if (! partAbbreviation.empty ()) {
      fLilypondCodeStream <<
        "shortInstrumentName = ";

      // does the name contain hexadecimal end of lines?
      std::size_t found =
        partAbbreviation.find ("&#xd");

      if (found == std::string::npos) {
        // no, merely generate the name
        fLilypondCodeStream <<
          nameAsLilypondString (partAbbreviation) <<
          std::endl;
      }

      else {
        // yes, split the name into a chunks list
        // and generate a \markup{} // JMI ???
        fLilypondCodeStream <<
          std::endl <<
          generateMultilineMarkup (
            partAbbreviation,
            lilypondMarkupColumnKind::kMarkupColumnKindLeftACentered) <<
          std::endl;
      }
    }
  }

  --gIndenter;

  // generate the std::string tunings if any
  S_msrStaffDetails
    currentStaffDetails =
      staff->getCurrentStaffStaffDetails ();

  if (currentStaffDetails) {
    const std::list <S_msrStaffTuning>&
      staffTuningsList =
        currentStaffDetails->getStaffTuningsList ();

    if (! staffTuningsList.empty ()) {
      fLilypondCodeStream <<
  // JMI      "restrainOpenStrings = ##t" <<
  // JMI      std::endl <<
        "stringTunings = \\stringTuning <";

      std::list <S_msrStaffTuning>::const_iterator
        iBegin = staffTuningsList.begin (),
        iEnd   = staffTuningsList.end (),
        i      = iBegin;

      ++gIndenter;

      for ( ; ; ) {
        S_msrStaffTuning
          staffTuning = (*i);

        fLilypondCodeStream <<
          msrQuarterTonesPitchKindAsStringInLanguage (
            staffTuning->
              getStaffTuningQuarterTonesPitchKind (),
            gLpsrOahGroup->
              getLpsrQuarterTonesPitchesLanguageKind ()) <<

          absoluteOctaveAsLilypondString (
            elt->getInputLineNumber (),
            staffTuning->
              getStaffTuningOctaveKind ());

        if (++i == iEnd) break;

        fLilypondCodeStream << cLilyPondSpace;
      } // for

      fLilypondCodeStream <<
        ">" <<
        std::endl;

      --gIndenter;

      // should letters be used for frets?
      switch (currentStaffDetails->getShowFretsKind ()) {
        case msrShowFretsKind::kShowFretsNumbers:
          break;
        case msrShowFretsKind::kShowFretsLetters:
          fLilypondCodeStream <<
            "tablatureFormat = #fret-letter-tablature-format" <<
            std::endl;
          break;
      } // switch
    }
  }

  // generate ledger lines coloring code if needed
  if (fVisitedLpsrScore->getColoredLedgerLinesIsNeeded ()) {
    gIndenter.indentMultiLineString (
R"(  \override LedgerLineSpanner.stencil = #MyLedgerLineSpannerPrint
  \override LedgerLineSpanner.after-line-breaking = #grob::display-oects)",
      fLilypondCodeStream);
  }

  // generate merge rests if needed
  if (fVisitedLpsrScore->getMergeStaffCommonRestsIsNeeded ()) {
    fLilypondCodeStream <<
      gTab << "\\override RestCollision.positioning-done = #merge-rests-on-positioning" <<
      std::endl;
  }

  // generate the 'with' block ending
  fLilypondCodeStream <<
    cLilypondWithCloser <<
    std::endl;

  // generate the comment if relevant
  fLilypondCodeStream << cLilypondStaffBlockOpener;

  if (gGlobalLpsr2lilypondOahGroup->getCommentLilypondStructureBasics ()) {
    fLilypondCodeStream <<
      " % start of staff block for \"" << staff->getStaffPathLikeName () << "\"";
  }

  fLilypondCodeStream << std::endl;

  if (gGlobalLpsr2lilypondOahGroup->getJianpu ()) {
    fLilypondCodeStream <<
      " \\jianpuMusic" <<
      std::endl;
  }

  ++gIndenter; // decremented in visitEnd (S_lpsrStaffBlock& elt)
}

void lpsr2lilypondTranslator::visitEnd (S_lpsrStaffBlock& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateLpsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateLpsrVisitingInformation) {
      std::stringstream ss;

      ss <<
        "% --> End visiting lpsrStaffBlock" <<
        ", line " << elt->getInputLineNumber () <<
      std::endl;

      if (traceLpsrVisitors) {
        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }

      if (generateLpsrVisitingInformation) {
        fLilypondCodeStream << ss.str ();
      }
    }
  }
#endif // MF_TRACE_IS_ENABLED

  --gIndenter; // incremented in visitStart (S_lpsrStaffBlock& elt)

  fLilypondCodeStream <<  cLilypondStaffBlockCloser;

  if (gGlobalLpsr2lilypondOahGroup->getCommentLilypondStructureBasics ()) {
  // get the staff
    S_msrStaff
      staff =
        elt->getStaff ();

    fLilypondCodeStream <<
      " % end of staff block for \"" << staff->getStaffPathLikeName () << "\"";

    fLilypondCodeStream <<
      ", fStaffBlocksCounter: " << fStaffBlocksCounter <<
      ", fNumberOfStaffBlocksElements: " << fNumberOfStaffBlocksElements;
  }

  fLilypondCodeStream << std::endl;

  if (fStaffBlocksCounter < fNumberOfStaffBlocksElements) {
    fLilypondCodeStream << std::endl;
  }
}

/*
//________________________________________________________________________
void lpsr2lilypondTranslator::visitStart (S_lpsrNewStaffGroupBlock& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateLpsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateLpsrVisitingInformation) {
      std::stringstream ss;

      ss <<
        "% --> Start visiting lpsrNewStaffGroupBlock" <<
        ", line " << elt->getInputLineNumber () <<
      std::endl;

      if (traceLpsrVisitors) {
        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }

      if (generateLpsrVisitingInformation) {
        fLilypondCodeStream << ss.str ();
      }
    }
  }
#endif // MF_TRACE_IS_ENABLED

   fLilypondCodeStream <<
     cLilypondNewStaffGroup << cLilyPondSpace << "{" <<
      std::endl;

  ++gIndenter; // decremented in visitEnd (S_lpsrStaffBlock& elt)
}

void lpsr2lilypondTranslator::visitEnd (S_lpsrNewStaffGroupBlock& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateLpsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateLpsrVisitingInformation) {
      std::stringstream ss;

      ss <<
        "% --> End visiting lpsrNewStaffGroupBlock" <<
        ", line " << elt->getInputLineNumber () <<
      std::endl;

      if (traceLpsrVisitors) {
        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }

      if (generateLpsrVisitingInformation) {
        fLilypondCodeStream << ss.str ();
      }
    }
  }
#endif // MF_TRACE_IS_ENABLED

  --gIndenter; // incremented in visitStart (S_lpsrStaffBlock& elt)

  fLilypondCodeStream <<
    " }" <<
    std::endl << std::endl;
}

//________________________________________________________________________
void lpsr2lilypondTranslator::visitStart (S_lpsrNewStaffBlock& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateLpsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateLpsrVisitingInformation) {
      std::stringstream ss;

      ss <<
        "% --> Start visiting lpsrNewStaffBlock" <<
        ", line " << elt->getInputLineNumber () <<
      std::endl;

      if (traceLpsrVisitors) {
        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }

      if (generateLpsrVisitingInformation) {
        fLilypondCodeStream << ss.str ();
      }
    }
  }
#endif // MF_TRACE_IS_ENABLED

  ++gIndenter; // decremented in visitEnd (S_lpsrNewStaffBlock& elt)
}

void lpsr2lilypondTranslator::visitEnd (S_lpsrNewStaffBlock& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateLpsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateLpsrVisitingInformation) {
      std::stringstream ss;

      ss <<
        "% --> End visiting lpsrNewStaffBlock" <<
        ", line " << elt->getInputLineNumber () <<
      std::endl;

      if (traceLpsrVisitors) {
        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }

      if (generateLpsrVisitingInformation) {
        fLilypondCodeStream << ss.str ();
      }
    }
  }
#endif // MF_TRACE_IS_ENABLED

  --gIndenter; // incremented in visitStart (S_lpsrNewStaffBlock& elt)
}
*/

//________________________________________________________________________
void lpsr2lilypondTranslator::visitStart (S_lpsrUseVoiceCommand& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateLpsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateLpsrVisitingInformation) {
      std::stringstream ss;

      ss <<
        "% --> Start visiting lpsrUseVoiceCommand" <<
        ", line " << elt->getInputLineNumber () <<
      std::endl;

      if (traceLpsrVisitors) {
        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }

      if (generateLpsrVisitingInformation) {
        fLilypondCodeStream << ss.str ();
      }
    }
  }
#endif // MF_TRACE_IS_ENABLED

  S_msrVoice
    voice = elt->getVoice ();

  S_msrStaff
    staff = voice-> getVoiceUpLinkToStaff ();

  msrStaffKind
    staffKind = staff->getStaffKind ();

  std::string staffContextName;
  std::string voiceContextName;

  switch (staffKind) {
    case msrStaffKind::kStaffKindRegular:
      staffContextName = "Staff";
      voiceContextName = "Voice";
      break;

    case msrStaffKind::kStaffKindTablature:
      staffContextName = "TabStaff";
      voiceContextName = "TabVoice";
      break;

    case msrStaffKind::kStaffKindHarmonies:
      staffContextName = "ChordNames"; // JMI 0.9.68
      voiceContextName = "ChordNames???"; // JMI 0.9.68
      break;

    case msrStaffKind::kStaffKindFiguredBass:
      staffContextName = "FiguredBass";
      voiceContextName = "FiguredVoice???"; // JMI 0.9.68
      break;

    case msrStaffKind::kStaffKindDrum:
      staffContextName = "DrumStaff";
      voiceContextName = "DrumVoice";
        // the "DrumVoice" alias exists, let's use it
      break;

    case msrStaffKind::kStaffKindRythmic:
      staffContextName = "RhythmicStaff";
      voiceContextName = "RhythmicVoice";
        // no "RhythmicVoice" alias exists
      break;
  } // switch

// if (voice->getStaffRelativeVoiceNumber () > 0) { JMI
  fLilypondCodeStream <<
    cLilypondUseVoiceOpener <<
    voiceContextName << " = \"" <<
    voice->getVoicePathLikeName () << "\" <<" <<
    std::endl;

  ++gIndenter;

  if (gGlobalLpsr2lilypondOahGroup->getNoAutoBeaming ()) {
    fLilypondCodeStream <<
      "\\set " << staffContextName << ".autoBeaming = ##f" <<
      std::endl;
  }

  // should a voice indication be generated?
  switch (staffKind) {
    case msrStaffKind::kStaffKindRegular:
      {
        int staffRegularVoicesCounter =
          staff->getStaffRegularVoicesCounter ();

        int
          regularVoiceStaffSequentialNumber =
            voice->
              getRegularVoiceStaffSequentialNumber ();

        Bool doGenerateVoiceCommands (false);

        doGenerateVoiceCommands = staffRegularVoicesCounter > 1; // JMI SURE??? 0.9.70

        if (doGenerateVoiceCommands) {
          switch (regularVoiceStaffSequentialNumber) { // JMI 0.9.70
            case 1:
              fLilypondCodeStream << "\\voiceOne ";
              break;
            case 2:
              fLilypondCodeStream << "\\voiceTwo ";
              break;
            case 3:
              fLilypondCodeStream << "\\voiceThree ";
              break;
            case 4:
              fLilypondCodeStream << "\\voiceFour ";
              break;
            default: // should not occur
              fLilypondCodeStream <<
                "\\voice_" <<
                regularVoiceStaffSequentialNumber << cLilyPondSpace;
          } // switch

          fLilypondCodeStream <<
            "% out of " <<
            staffRegularVoicesCounter <<
            " regular voices" <<
            std::endl;
        }
      }
    break;

    default:
      ;
  } // switch

  // fetch the part and part name
  S_msrPart part =
    staff-> getStaffUpLinkToPart ();

  std::string
    partName =
      part->getPartPathLikeName ();

  // should a transposition be generated?
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceTranspositions ()) {
    std::stringstream ss;

    ss <<
      std::endl <<
      "Considering the generation of a voice transposition for part \"" <<
      partName <<
      "\"";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // should we transpose fCurrentPart?
  Bool doTransposeCurrentPart (false);

  std::string partNameFoundInTranspositionMap;
  std::string partIDFoundInTranspositionMap;

  S_msrSemiTonesPitchAndOctave
    semiTonesPitchAndOctave;

  // check by name
  if (! partName.empty ()) { // a part name is not mandatory in MusicXML
    if (! gGlobalLpsr2lilypondOahGroup->getPartNamesTranspositionMap ().empty ()) {
      std::map <std::string, S_msrSemiTonesPitchAndOctave>::const_iterator
        it =
          gGlobalLpsr2lilypondOahGroup->getPartNamesTranspositionMap ().find (
            partName);

      if (it != gGlobalLpsr2lilypondOahGroup->getPartNamesTranspositionMap ().end ()) {
        // partName is present in the map,
        // fetch the semitones pitch and octave
        semiTonesPitchAndOctave =
          (*it).second;

        partNameFoundInTranspositionMap = partName;
        doTransposeCurrentPart = true;
      }
    }
  }

  // check by partMusicXMLID
  std::string partMusicXMLID =
    part->getPartMusicXMLID ();

  if (! gGlobalLpsr2lilypondOahGroup->getPartMusicXMLIDsTranspositionMap ().empty ()) {
    std::map <std::string, S_msrSemiTonesPitchAndOctave>::const_iterator
      it =
        gGlobalLpsr2lilypondOahGroup->getPartMusicXMLIDsTranspositionMap ().find (
          partMusicXMLID);

    if (it != gGlobalLpsr2lilypondOahGroup->getPartMusicXMLIDsTranspositionMap ().end ()) {
      // partMusicXMLID is present in the map,
      // fetch the semitones pitch and octave
      semiTonesPitchAndOctave =
        (*it).second;

      partIDFoundInTranspositionMap = partMusicXMLID;
      doTransposeCurrentPart = true;
    }
  }

  if (doTransposeCurrentPart) {
    // generate the transposition
#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceTranspositions ()) {
      std::stringstream ss;

      ss <<
        std::endl <<
        "Generating a voice transposition for " <<
        semiTonesPitchAndOctave->asString () <<
        " in part \"" <<
        partName <<
        "\"" <<
        ", part ID \"" <<
        partMusicXMLID <<
        "\"";

      gWaeHandler->waeTrace (
        __FILE__, mfInputLineNumber (__LINE__),
        ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED

    // fetch the LilyPond pitch and octave
    std::string
      semiTonesPitchAndOctaveAsLilypondString =
        msrSemiTonesPitchAndOctaveAsLilypondString (
          gLpsrOahGroup->getLpsrQuarterTonesPitchesLanguageKind (),
          semiTonesPitchAndOctave);

    fLilypondCodeStream <<
       "\\transposition " <<
       semiTonesPitchAndOctaveAsLilypondString <<
       std::endl <<
       "\\transpose " <<
       semiTonesPitchAndOctaveAsLilypondString << cLilyPondSpace << "c'" <<
      std::endl;
  }

  else {
    if (
      ! partNameFoundInTranspositionMap.empty ()
        ||
      ! partIDFoundInTranspositionMap.empty ()
    ) {
      std::stringstream ss;

      if (! partNameFoundInTranspositionMap.empty ()) {
        ss <<
          "Sorry, part name \"" <<
          partNameFoundInTranspositionMap <<
          "\" not found in LPSR";
      }

      if (! partIDFoundInTranspositionMap.empty ()) {
        ss <<
          "Sorry, part ID \"" <<
          partIDFoundInTranspositionMap <<
          "\" not found in LPSR";
      }

      lpsr2lilypondWarning (
        __FILE__, mfInputLineNumber (__LINE__),
        ss.str ());
    }
  }

  // generate voice name
  fLilypondCodeStream <<
    "\\" << voice->getVoicePathLikeName () << std::endl;

  --gIndenter;

  fLilypondCodeStream <<
    cLilypondUseVoiceCloser;

  if (gGlobalLpsr2lilypondOahGroup->getCommentLilypondStructureBasics ()) {
    // get the voice
    S_msrVoice
      voice =
        elt->getVoice ();

    fLilypondCodeStream <<
      " % end of voice command for voice " << voice->getVoiceName ();
  }

  fLilypondCodeStream << std::endl;
// } JMI
}

void lpsr2lilypondTranslator::visitEnd (S_lpsrUseVoiceCommand& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateLpsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateLpsrVisitingInformation) {
      std::stringstream ss;

      ss <<
        "% --> End visiting lpsrUseVoiceCommand" <<
        ", line " << elt->getInputLineNumber () <<
      std::endl;

      if (traceLpsrVisitors) {
        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }

      if (generateLpsrVisitingInformation) {
        fLilypondCodeStream << ss.str ();
      }
    }
  }
#endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
void lpsr2lilypondTranslator::visitStart (S_lpsrNewLyricsBlock& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateLpsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateLpsrVisitingInformation) {
      std::stringstream ss;

      ss <<
        "% --> Start visiting lpsrNewLyricsBlock" <<
        ", line " << elt->getInputLineNumber () <<
      std::endl;

      if (traceLpsrVisitors) {
        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }

      if (generateLpsrVisitingInformation) {
        fLilypondCodeStream << ss.str ();
      }
    }
  }
#endif // MF_TRACE_IS_ENABLED

  if (! gGlobalLpsr2lilypondOahGroup->getNoLilypondLyrics ()) {
    S_msrStanza stanza = elt->getStanza ();

    fLilypondCodeStream <<
      cLilypondNewLyrics <<
      std::endl;

    ++gIndenter;

    fLilypondCodeStream <<
      "\\with " << cLilypondWithOpener <<
      std::endl;

    if (gGlobalLpsr2lilypondOahGroup->getAddLilypondStanzasNumbers ()) {
      fLilypondCodeStream <<
        gTab << "stanza = \"" <<
        stanza->getStanzaNumber () <<
        ".\"" <<
        std::endl;
    }

    fLilypondCodeStream <<
      cLilypondWithCloser <<
      std::endl;

    switch (gGlobalLpsr2lilypondOahGroup->getLyricsNotesDurationsKind ()) {
      case lpsrLyricsNotesDurationsKind::kLyricsNotesDurationsAutomatic:
        // no \lyricsto in that case
        fLilypondCodeStream <<
          "\\lyricsto \"" << elt->getVoice ()->getVoicePathLikeName () << "\" { " <<
          "\\" << stanza->getStanzaPathLikeName () <<
          " }" <<
          std::endl;
          break;

      case lpsrLyricsNotesDurationsKind::kLyricsNotesDurationsImplicit:
        // maybe we could use addlyrics optionally? JMI 0.9.70 BABASSE LPNR page 64
        fLilypondCodeStream <<
          "\\lyricsto \"" << elt->getVoice ()->getVoicePathLikeName () << "\" { " <<
          "\\" << stanza->getStanzaPathLikeName () <<
          " }" <<
          std::endl;
        break;

      case lpsrLyricsNotesDurationsKind::kLyricsNotesDurationsExplicit:
        // no \lyricsto in that case
        fLilypondCodeStream <<
          "\\" << stanza->getStanzaPathLikeName () <<
          std::endl;
        break;
    } // switch

    --gIndenter;
  }
}

void lpsr2lilypondTranslator::visitEnd (S_lpsrNewLyricsBlock& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateLpsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateLpsrVisitingInformation) {
      std::stringstream ss;

      ss <<
        "% --> End visiting lpsrNewLyricsBlock" <<
        ", line " << elt->getInputLineNumber () <<
      std::endl;

      if (traceLpsrVisitors) {
        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }

      if (generateLpsrVisitingInformation) {
        fLilypondCodeStream << ss.str ();
      }
    }
  }
#endif // MF_TRACE_IS_ENABLED

  if (! gGlobalLpsr2lilypondOahGroup->getNoLilypondLyrics ()) {
    // JMI
  }
}

//________________________________________________________________________
void lpsr2lilypondTranslator::visitStart (S_lpsrVariableUseCommand& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateLpsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateLpsrVisitingInformation) {
      std::stringstream ss;

      ss <<
        "% --> Start visiting lpsrVariableUseCommand" <<
        ", line " << elt->getInputLineNumber () <<
      std::endl;

      if (traceLpsrVisitors) {
        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }

      if (generateLpsrVisitingInformation) {
        fLilypondCodeStream << ss.str ();
      }
    }
  }
#endif // MF_TRACE_IS_ENABLED

  ++gIndenter; // decremented in visitEnd (S_lpsrVariableUseCommand& elt)
}

void lpsr2lilypondTranslator::visitEnd (S_lpsrVariableUseCommand& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateLpsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateLpsrVisitingInformation) {
      std::stringstream ss;

      ss <<
        "% --> End visiting lpsrVariableUseCommand" <<
        ", line " << elt->getInputLineNumber () <<
      std::endl;

      if (traceLpsrVisitors) {
        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }

      if (generateLpsrVisitingInformation) {
        fLilypondCodeStream << ss.str ();
      }
    }
  }
#endif // MF_TRACE_IS_ENABLED

  --gIndenter; // incremented in visitStart (S_lpsrVariableUseCommand& elt)
}

//________________________________________________________________________
void lpsr2lilypondTranslator::visitStart (S_lpsrChordNamesContext& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateLpsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateLpsrVisitingInformation) {
      std::stringstream ss;

      ss <<
        "% --> Start visiting lpsrChordNamesContext" <<
        ", line " << elt->getInputLineNumber () <<
      std::endl;

      if (traceLpsrVisitors) {
        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }

      if (generateLpsrVisitingInformation) {
        fLilypondCodeStream << ss.str ();
      }
    }
  }
#endif // MF_TRACE_IS_ENABLED

  switch (elt->getContextUseExistingKind ()) {
    case lpsrContextUseExistingKind::kUseExistingContextYes:
      if (gGlobalLpsr2lilypondOahGroup->getCommentLilypondChords ()) {
        fLilypondCodeStream <<
          "%{ Use existing context here %} " << // JMI ??? 0.9.66
          std::endl;
      }
      break;
    case lpsrContextUseExistingKind::kUseExistingContextNo:
      fLilypondCodeStream <<
        "\\context ";
      break;
  } // switch

  std::string
    contextPathLikeName =
      elt->getContextPathLikeName ();

  fLilypondCodeStream <<
//     "\\context " << // JMI ??? 0.9.66
    lpsrContextTypeKindAsLilypondString (
      elt->getContextTypeKind ()) <<
    " = \"" << contextPathLikeName << "\"" <<
    std::endl;

/* JMI
  if (false) { //option JMI
    fLilypondCodeStream <<
      "\\with " << cLilypondWithOpener <<
      std::endl;

    ++gIndenter;

    fLilypondCodeStream <<
      "\\override BarLine.bar-extent = #'(-2 . 2)" <<
      std::endl <<
      "\\consists \"Bar_engraver\"" <<
      std::endl;

    --gIndenter;

    fLilypondCodeStream <<
      cLilypondWithCloser <<
      std::endl;
  }
      */

  fLilypondCodeStream <<
    "\\" << contextPathLikeName <<
    std::endl << std::endl;
}

void lpsr2lilypondTranslator::visitEnd (S_lpsrChordNamesContext& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateLpsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateLpsrVisitingInformation) {
      std::stringstream ss;

      ss <<
        "% --> End visiting lpsrChordNamesContext" <<
        ", line " << elt->getInputLineNumber () <<
      std::endl;

      if (traceLpsrVisitors) {
        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }

      if (generateLpsrVisitingInformation) {
        fLilypondCodeStream << ss.str ();
      }
    }
  }
#endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
void lpsr2lilypondTranslator::visitStart (S_lpsrFiguredBassContext& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateLpsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateLpsrVisitingInformation) {
      std::stringstream ss;

      ss <<
        "% --> Start visiting lpsrFiguredBassContext" <<
        ", line " << elt->getInputLineNumber () <<
      std::endl;

      if (traceLpsrVisitors) {
        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }

      if (generateLpsrVisitingInformation) {
        fLilypondCodeStream << ss.str ();
      }
    }
  }
#endif // MF_TRACE_IS_ENABLED

  fLilypondCodeStream <<
    "%{" <<
    std::endl <<
    "visitStart (S_lpsrFiguredBassContext& elt), elt = " <<
    std::endl <<
    elt <<
    "%}" <<
    std::endl <<
    std::endl;

  switch (elt->getContextUseExistingKind ()) {
    case lpsrContextUseExistingKind::kUseExistingContextYes:
      if (gGlobalLpsr2lilypondOahGroup->getCommentLilypondFiguredBasses ()) {
        fLilypondCodeStream <<
          "%{ Use existing context here %} " << // JMI ??? 0.9.66
          std::endl;
      }
      break;
    case lpsrContextUseExistingKind::kUseExistingContextNo:
      fLilypondCodeStream <<
        "\\context ";
      break;
  } // switch

  std::string
    contextPathLikeName =
      elt->getContextPathLikeName ();

  fLilypondCodeStream <<
    lpsrContextTypeKindAsLilypondString (
      elt->getContextTypeKind ()) <<
    " = \"" << contextPathLikeName << "\"" <<
    std::endl;

/* JMI
  if (false) { //option JMI
    fLilypondCodeStream <<
      "\\with " << cLilypondWithOpener <<
      std::endl;

    ++gIndenter;

    fLilypondCodeStream <<
      "\\override BarLine.bar-extent = #'(-2 . 2)" <<
      std::endl <<
      "\\consists \"Bar_engraver\"" <<
      std::endl;

    --gIndenter;

    fLilypondCodeStream <<
      cLilypondWithCloser <<
      std::endl;
  }
  */

  fLilypondCodeStream <<
    "\\" << contextPathLikeName <<
    std::endl << std::endl;
}

void lpsr2lilypondTranslator::visitEnd (S_lpsrFiguredBassContext& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateLpsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateLpsrVisitingInformation) {
      std::stringstream ss;

      ss <<
        "% --> End visiting lpsrFiguredBassContext" <<
        ", line " << elt->getInputLineNumber () <<
      std::endl;

      if (traceLpsrVisitors) {
        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }

      if (generateLpsrVisitingInformation) {
        fLilypondCodeStream << ss.str ();
      }
    }
  }
#endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
void lpsr2lilypondTranslator::visitStart (S_lpsrBarCommand& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateLpsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateLpsrVisitingInformation) {
      std::stringstream ss;

      ss <<
        "% --> Start visiting lpsrBarCommand" <<
        ", line " << elt->getInputLineNumber () <<
      std::endl;

      if (traceLpsrVisitors) {
        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }

      if (generateLpsrVisitingInformation) {
        fLilypondCodeStream << ss.str ();
      }
    }
  }
#endif // MF_TRACE_IS_ENABLED
}

void lpsr2lilypondTranslator::visitEnd (S_lpsrBarCommand& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateLpsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateLpsrVisitingInformation) {
      std::stringstream ss;

      ss <<
        "% --> End visiting lpsrBarCommand" <<
        ", line " << elt->getInputLineNumber () <<
      std::endl;

      if (traceLpsrVisitors) {
        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }

      if (generateLpsrVisitingInformation) {
        fLilypondCodeStream << ss.str ();
      }
    }
  }
#endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
void lpsr2lilypondTranslator::visitStart (S_lpsrComment& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateLpsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateLpsrVisitingInformation) {
      std::stringstream ss;

      ss <<
        "% --> Start visiting lpsrComment" <<
        ", line " << elt->getInputLineNumber () <<
      std::endl;

      if (traceLpsrVisitors) {
        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }

      if (generateLpsrVisitingInformation) {
        fLilypondCodeStream << ss.str ();
      }
    }
  }
#endif // MF_TRACE_IS_ENABLED

  fLilypondCodeStream <<
    "% " << elt->getContents () <<
    std::endl;

  switch (elt->getCommentGapKind ()) {
    case lpsrCommentGapAfterwardsKind::kCommentGapAfterwardsYes:
      fLilypondCodeStream << std::endl;
      break;
    case lpsrCommentGapAfterwardsKind::kCommentGapAfterwardsNo:
      break;
  } // switch
}

void lpsr2lilypondTranslator::visitEnd (S_lpsrComment& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateLpsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateLpsrVisitingInformation) {
      std::stringstream ss;

      ss <<
        "% --> End visiting lpsrComment" <<
        ", line " << elt->getInputLineNumber () <<
      std::endl;

      if (traceLpsrVisitors) {
        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }

      if (generateLpsrVisitingInformation) {
        fLilypondCodeStream << ss.str ();
      }
    }
  }
#endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
void lpsr2lilypondTranslator::visitStart (S_lpsrSchemeFunction& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateLpsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateLpsrVisitingInformation) {
      std::stringstream ss;

      ss <<
        "% --> Start visiting lpsrSchemeFunction" <<
        ", line " << elt->getInputLineNumber () <<
      std::endl;

      if (traceLpsrVisitors) {
        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }

      if (generateLpsrVisitingInformation) {
        fLilypondCodeStream << ss.str ();
      }
    }
  }
#endif // MF_TRACE_IS_ENABLED

  fLilypondCodeStream <<
    std::endl <<
    "% Scheme function(s): \"" << elt->getFunctionName () << "\"" <<
    // std::endl is in the decription
    elt->getFunctionDescription () <<
    // std::endl is in the decription
    elt->getFunctionCode () <<
    std::endl << std::endl;
}

void lpsr2lilypondTranslator::visitEnd (S_lpsrSchemeFunction& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateLpsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateLpsrVisitingInformation) {
      std::stringstream ss;

      ss <<
        "% --> End visiting lpsrSchemeFunction" <<
        ", line " << elt->getInputLineNumber () <<
      std::endl;

      if (traceLpsrVisitors) {
        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }

      if (generateLpsrVisitingInformation) {
        fLilypondCodeStream << ss.str ();
      }
    }
  }
#endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
/* JMI
void lpsr2lilypondTranslator::visitStart (S_lpsrMelismaCommand& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateLpsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateLpsrVisitingInformation) {
      std::stringstream ss;

      ss <<
        "% --> Start visiting lpsrMelismaCommand" <<
        ", line " << elt->getInputLineNumber () <<
      std::endl;

      if (traceLpsrVisitors) {
        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }

      if (generateLpsrVisitingInformation) {
        fLilypondCodeStream << ss.str ();
      }
    }
  }
#endif // MF_TRACE_IS_ENABLED

  switch (elt->getMelismaKind ()) {
    case lpsrMelismaCommand::kMelismaStart:
// JMI      fLilypondCodeStream << "\\melisma ";
      break;
    case lpsrMelismaCommand::kMelismaEnd:
// JMI      fLilypondCodeStream << "\\melismaEnd ";
      break;
  } // switch
}

void lpsr2lilypondTranslator::visitEnd (S_lpsrMelismaCommand& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateLpsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateLpsrVisitingInformation) {
      std::stringstream ss;

      ss <<
        "% --> End visiting lpsrMelismaCommand" <<
        ", line " << elt->getInputLineNumber () <<
      std::endl;

      if (traceLpsrVisitors) {
        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }

      if (generateLpsrVisitingInformation) {
        fLilypondCodeStream << ss.str ();
      }
    }
  }
#endif // MF_TRACE_IS_ENABLED
}
*/

//________________________________________________________________________
void lpsr2lilypondTranslator::visitStart (S_msrScore& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream ss;

      ss <<
        "% --> Start visiting msrScore" <<
        ", line " << elt->getInputLineNumber () <<
      std::endl;

      if (traceLpsrVisitors) {
        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << ss.str ();
      }
    }
  }
#endif // MF_TRACE_IS_ENABLED
}

void lpsr2lilypondTranslator::visitEnd (S_msrScore& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream ss;

      ss <<
        "% --> End visiting msrScore" <<
        ", line " << elt->getInputLineNumber () <<
      std::endl;

      if (traceLpsrVisitors) {
        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << ss.str ();
      }
    }
  }
#endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
void lpsr2lilypondTranslator::visitStart (S_msrScaling& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream ss;

      ss <<
        "% --> Start visiting msrScaling" <<
        ", line " << elt->getInputLineNumber () <<
      std::endl;

      if (traceLpsrVisitors) {
        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << ss.str ();
      }
    }
  }
#endif // MF_TRACE_IS_ENABLED
}

void lpsr2lilypondTranslator::visitEnd (S_msrScaling& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream ss;

      ss <<
        "% --> End visiting msrScaling" <<
        ", line " << elt->getInputLineNumber () <<
      std::endl;

      if (traceLpsrVisitors) {
        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << ss.str ();
      }
    }
  }
#endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
void lpsr2lilypondTranslator::visitStart (S_msrPageLayout& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream ss;

      ss <<
        "% --> Start visiting msrPageLayout" <<
        ", line " << elt->getInputLineNumber () <<
      std::endl;

      if (traceLpsrVisitors) {
        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << ss.str ();
      }
    }
  }
#endif // MF_TRACE_IS_ENABLED

/* JMI
    // page size
    S_msrLength           fPageHeight;
    S_msrLength           fPageWidth;

    // margins
    S_msrMargin           fLeftMargin;
    S_msrMargin           fRightMargin;
    S_msrMargin           fTopMargin;
    S_msrMargin           fBottomMargin;
  */

  S_msrLength
    pageHeight =
      elt->getPageHeight ();

  if (pageHeight) {
    fLilypondCodeStream <<
      pageHeight->getLengthValue () <<
      lengthUnitAsLilypondString (pageHeight->getLengthUnitKind ()) <<
      std::endl;
  }
}

void lpsr2lilypondTranslator::visitEnd (S_msrPageLayout& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream ss;

      ss <<
        "% --> End visiting msrPageLayout" <<
        ", line " << elt->getInputLineNumber () <<
      std::endl;

      if (traceLpsrVisitors) {
        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << ss.str ();
      }
    }
  }
#endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
void lpsr2lilypondTranslator::visitStart (S_msrSystemLayout& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream ss;

      ss <<
        "% --> Start visiting msrSystemLayout" <<
        ", line " << elt->getInputLineNumber () <<
      std::endl;

      if (traceLpsrVisitors) {
        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << ss.str ();
      }
    }
  }
#endif // MF_TRACE_IS_ENABLED
}

void lpsr2lilypondTranslator::visitEnd (S_msrSystemLayout& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream ss;

      ss <<
        "% --> End visiting msrSystemLayout" <<
        ", line " << elt->getInputLineNumber () <<
      std::endl;

      if (traceLpsrVisitors) {
        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << ss.str ();
      }
    }
  }
#endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
void lpsr2lilypondTranslator::visitStart (S_msrStaffLayout& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream ss;

      ss <<
        "% --> Start visiting msrStaffLayout" <<
        ", line " << elt->getInputLineNumber () <<
      std::endl;

      if (traceLpsrVisitors) {
        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << ss.str ();
      }
    }
  }
#endif // MF_TRACE_IS_ENABLED
}

void lpsr2lilypondTranslator::visitEnd (S_msrStaffLayout& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream ss;

      ss <<
        "% --> End visiting msrStaffLayout" <<
        ", line " << elt->getInputLineNumber () <<
      std::endl;

      if (traceLpsrVisitors) {
        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << ss.str ();
      }
    }
  }
#endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
void lpsr2lilypondTranslator::visitStart (S_msrAppearance& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream ss;

      ss <<
        "% --> Start visiting msrAppearance" <<
        ", line " << elt->getInputLineNumber () <<
      std::endl;

      if (traceLpsrVisitors) {
        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << ss.str ();
      }
    }
  }
#endif // MF_TRACE_IS_ENABLED
}

void lpsr2lilypondTranslator::visitEnd (S_msrAppearance& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream ss;

      ss <<
        "% --> End visiting msrAppearance" <<
        ", line " << elt->getInputLineNumber () <<
      std::endl;

      if (traceLpsrVisitors) {
        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << ss.str ();
      }
    }
  }
#endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
void lpsr2lilypondTranslator::visitStart (S_msrCredit& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream ss;

      ss <<
        "% --> Start visiting msrCredit" <<
        ", line " << elt->getInputLineNumber () <<
      std::endl;

      if (traceLpsrVisitors) {
        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << ss.str ();
      }
    }
  }
#endif // MF_TRACE_IS_ENABLED
}

void lpsr2lilypondTranslator::visitEnd (S_msrCredit& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream ss;

      ss <<
        "% --> End visiting msrCredit" <<
        ", line " << elt->getInputLineNumber () <<
      std::endl;

      if (traceLpsrVisitors) {
        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << ss.str ();
      }
    }
  }
#endif // MF_TRACE_IS_ENABLED
}

void lpsr2lilypondTranslator::visitStart (S_msrCreditWords& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream ss;

      ss <<
        "% --> Start visiting msrCreditWords" <<
        ", line " << elt->getInputLineNumber () <<
      std::endl;

      if (traceLpsrVisitors) {
        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << ss.str ();
      }
    }
  }
#endif // MF_TRACE_IS_ENABLED
}

void lpsr2lilypondTranslator::visitEnd (S_msrCreditWords& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream ss;

      ss <<
        "% --> End visiting msrCreditWords" <<
        ", line " << elt->getInputLineNumber () <<
      std::endl;

      if (traceLpsrVisitors) {
        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << ss.str ();
      }
    }
  }
#endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
void lpsr2lilypondTranslator::visitStart (S_msrPartGroup& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream ss;

      ss <<
        "% --> Start visiting msrPartGroup" <<
        elt->asShortString () <<
        ", line " << elt->getInputLineNumber () <<
        std::endl;

      ss << // JMI 0.9.67
        elt->fetchPartGroupInformationForTrace () <<
        std::endl;

      if (traceLpsrVisitors) {
        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << ss.str ();
      }
    }
  }
#endif // MF_TRACE_IS_ENABLED
}

void lpsr2lilypondTranslator::visitEnd (S_msrPartGroup& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream ss;

      ss <<
        "% --> End visiting msrPartGroup" <<
        elt->fetchPartGroupInformationForTrace () <<
        ", line " << elt->getInputLineNumber () <<
      std::endl;

      if (traceLpsrVisitors) {
        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << ss.str ();
      }
    }
  }
#endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
void lpsr2lilypondTranslator::visitStart (S_msrPart& elt)
{
  std::string
    partdNameForTrace =
      elt->fetchPartNameForTrace ();

#ifdef MF_TRACE_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream ss;

      ss <<
        "% --> Start visiting msrPart" <<
        partdNameForTrace <<
        ", line " << elt->getInputLineNumber () <<
      std::endl;

      if (traceLpsrVisitors) {
        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << ss.str ();
      }
    }
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_MAINTAINANCE_RUNS_ARE_ENABLED
  if (
    gWaeOahGroup->getMaintainanceRun () // MAINTAINANCE_RUN
  ) {
#ifdef MF_TRACE_IS_ENABLED
    if (
      gTraceOahGroup->getTraceParts ()
    ) {
      std::stringstream ss;

      ss <<
  //       std::endl <<
        "<!--=== part \"" << partdNameForTrace << "\"" <<
        ", line " << elt->getInputLineNumber () << " ===-->";

      gWaeHandler->waeTrace (
        __FILE__, mfInputLineNumber (__LINE__),
        ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED
  }
#endif // MF_MAINTAINANCE_RUNS_ARE_ENABLED

  // remember current part
  fCurrentPart = elt;

  // register the current part in the service run data
  S_mfServiceRunData
    serviceRunData =
      gServiceRunData;

  serviceRunData->
    setCurrentPartIDAndName (
      elt->fetchPartIDAndName ());

  fRemainingMeasureRestsNumber = 0; // JMI
  fOnGoingMultipleMeasureRests = false; // JMI
}

void lpsr2lilypondTranslator::visitEnd (S_msrPart& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream ss;

      ss <<
        "% --> End visiting msrPart" <<
        elt->fetchPartNameForTrace () <<
        ", line " << elt->getInputLineNumber () <<
      std::endl;

      if (traceLpsrVisitors) {
        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << ss.str ();
      }
    }
  }
#endif // MF_TRACE_IS_ENABLED

  // forget about current part
  fCurrentPart = nullptr;
}

//________________________________________________________________________
void lpsr2lilypondTranslator::visitStart (S_msrStaff& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream ss;

      ss <<
        "% --> Start visiting msrStaff \"" <<
        elt->getStaffPathLikeName () <<
        "\"" <<
        ", line " << elt->getInputLineNumber () <<
      std::endl;

      if (traceLpsrVisitors) {
        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << ss.str ();
      }
    }
  }
#endif // MF_TRACE_IS_ENABLED

  // register the current staff number in the service run data
  S_mfServiceRunData
    serviceRunData =
      gServiceRunData;

  serviceRunData->
    setCurrentStaveNumber (
      elt->getStaffNumber ());

  fOnGoingStaff = true;
}

void lpsr2lilypondTranslator::visitEnd (S_msrStaff& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream ss;

      ss <<
        "% --> End visiting msrStaff \"" <<
        elt->getStaffPathLikeName () <<
        "\"" <<
        ", line " << elt->getInputLineNumber () <<
      std::endl;

      if (traceLpsrVisitors) {
        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << ss.str ();
      }
    }
  }
#endif // MF_TRACE_IS_ENABLED

  fOnGoingStaff = false;
}

void lpsr2lilypondTranslator::visitStart (S_msrStaffTuning& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream ss;

      ss <<
        "%--> Start visiting msrStaffTuning" <<
        std::endl;

      if (traceLpsrVisitors) {
        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << ss.str ();
      }
    }
  }
#endif // MF_TRACE_IS_ENABLED

/* JMI
  const std::list <S_msrStaffTuning>&
    staffTuningsList =
      staff->getStaffTuningsList ();

  if (! staffTuningsList.empty ()) {
    // \set TabStaff.stringTunings = \stringTuning <c' g' d'' a''>

    fLilypondCodeStream <<
      "\\set TabStaff.stringTunings = \\stringTuning <";

    std::list <S_msrStaffTuning>::const_iterator
      iBegin = staffTuningsList.begin (),
      iEnd   = staffTuningsList.end (),
      i      = iBegin;

    for ( ; ; ) {
      fLilypondCodeStream <<
        msrQuarterTonesPitchAsString (
          gLpsrOahGroup->getLpsrQuarterTonesPitchesLanguage (),
 // JMI            elt->getInputLineNumber (),
          ((*i)->getStaffTuningQuarterTonesPitch ())) <<
 // JMI       char (tolower ((*i)->getStaffTuningStep ())) <<
        absoluteOctaveAsLilypondString (
          elt->getInputLineNumber (),
          (*i)->getStaffTuningOctaveKind ());
      if (++i == iEnd) break;
      fLilypondCodeStream << cLilyPondSpace;
    } // for

    fLilypondCodeStream <<
      ">" <<
      std::endl;
  }
 */
}

void lpsr2lilypondTranslator::visitStart (S_msrStaffDetails& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream ss;

      ss <<
        "%--> Start visiting msrStaffDetails" <<
        std::endl;

      if (traceLpsrVisitors) {
        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << ss.str ();
      }
    }
  }
#endif // MF_TRACE_IS_ENABLED

  // fetch staff lines number
  int
    staffLinesNumber =
      elt->getStaffLinesNumber ();

  fLilypondCodeStream <<
    std::endl <<
    "\\stopStaff " <<
    std::endl <<
    "\\override Staff.StaffSymbol.line-count = " <<
    staffLinesNumber <<
    std::endl <<
    "\\startStaff" <<
    std::endl;
}

//________________________________________________________________________
void lpsr2lilypondTranslator::visitStart (S_msrVoice& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream ss;

      ss <<
        "% --> Start visiting msrVoice \"" <<
        elt->getVoicePathLikeName () <<
        "\"" <<
        ", line " << elt->getInputLineNumber () <<
      std::endl;

      if (traceLpsrVisitors) {
        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << ss.str ();
      }
    }
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceVoices ()) {
    std::stringstream ss;

    ss <<
//       std::endl <<
      "<!--=== voice \"" << elt->getVoicePathLikeName () << "\"" <<
      ", line " << elt->getInputLineNumber () << " ===-->";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // remember current voice
  fCurrentVoice = elt;

  // register the current voice number in the service run data
  S_mfServiceRunData
    serviceRunData =
      gServiceRunData;

  serviceRunData->
    setCurrentVoiceNumber (
      fCurrentVoice->getVoiceNumber ());

  fLilypondCodeStream <<
    fCurrentVoice->getVoicePathLikeName () <<
    " = ";

  // generate the beginning of the voice definition
  switch (fCurrentVoice->getVoiceKind ()) {
    case msrVoiceKind::kVoiceKindRegular:
      switch (gGlobalLpsr2lilypondOahGroup->fetchOctaveEntryVariableValue ()) {
        case msrOctaveEntryKind::kOctaveEntryRelative:
          fLilypondCodeStream <<
            "\\relative";
          if (
            gGlobalLpsr2lilypondOahGroup->
              getRelativeOctaveEntrySemiTonesPitchAndOctave ()
//      JMI         !=
//            gGlobalLpsr2lilypondOahGroup->getSemiTonesPitchAndOctaveDefaultValue ()
          ) {
            // option '-rel, -relative' has been used
            fLilypondCodeStream <<
              cLilyPondSpace <<
              msrSemiTonesPitchAndOctaveAsLilypondString (
                gLpsrOahGroup->
                  getLpsrQuarterTonesPitchesLanguageKind (),
                gGlobalLpsr2lilypondOahGroup->
                  getRelativeOctaveEntrySemiTonesPitchAndOctave ());
          }
          break;

        case msrOctaveEntryKind::kOctaveEntryAbsolute:
          fLilypondCodeStream <<
            "\\absolute";
          break;

        case msrOctaveEntryKind::kOctaveEntryFixed:
          fLilypondCodeStream <<
            "\\fixed " <<
            msrSemiTonesPitchAndOctaveAsLilypondString (
              gLpsrOahGroup->
                getLpsrQuarterTonesPitchesLanguageKind (),
              gGlobalLpsr2lilypondOahGroup->
                getFixedOctaveEntrySemiTonesPitchAndOctave ());
          break;
      } // switch

      fLilypondCodeStream <<
        " {" <<
        std::endl;
      break;

    case msrVoiceKind::kVoiceKindDynamics:
      fLilypondCodeStream <<
        "Dynamics {" << // JMI ???
        std::endl;
      break;

    case msrVoiceKind::kVoiceKindHarmonies:
      fLilypondCodeStream <<
        "\\chordmode {" <<
        std::endl;
      break;

    case msrVoiceKind::kVoiceKindFiguredBass:
      fLilypondCodeStream <<
        "\\figuremode {" <<
        std::endl;
      break;
  } // switch

  ++gIndenter; // decremented in visitEnd (S_msrVoice& elt)

  // generate a 'global' empty variable?
  if (gGlobalLpsr2lilypondOahGroup->getGlobalEmptyVariable ()) {
    fLilypondCodeStream <<
      "\\global" <<
      std::endl << std::endl;
  }

  // should \displayMusic be generated?
  if (gGlobalLpsr2lilypondOahGroup->getDisplayMusic ()) {
    fLilypondCodeStream <<
      "\\displayMusic {" <<
      std::endl;

    ++gIndenter; // decremented in visitEnd (S_msrVoice& elt)
  }

  // language?
  if (! gGlobalLpsr2lilypondOahGroup->getUseLilypondDefaultLanguages ()) { // JMI HasBeenSet???
    fLilypondCodeStream <<
      "\\language \"" <<
      msrQuarterTonesPitchesLanguageKindAsLilypondString (
        gLpsrOahGroup->
          getLpsrQuarterTonesPitchesLanguageKind ()) <<
      "\"" <<
      std::endl;
  }

  // chords language?
  if (! gGlobalLpsr2lilypondOahGroup->getUseLilypondDefaultLanguages ()) { // JMI HasBeenSet???
    if (
      gLpsrOahGroup->getLpsrChordsLanguageKind ()
        !=
      lpsrChordsLanguageKind::kChordsIgnatzek
    ) {
      fLilypondCodeStream <<
        "\\" <<
        lpsrChordsLanguageKindAsString (
          gLpsrOahGroup->
            getLpsrChordsLanguageKind ()) <<
        "Chords" <<
        std::endl;
    }
  }

  // show all bar numbers?
  if (gGlobalLpsr2lilypondOahGroup->getShowAllBarNumbers ()) {
    fLilypondCodeStream <<
      "\\set Score.barNumberVisibility = #all-bar-numbers-visible" <<
      std::endl <<
      "\\override Score.BarNumber.break-visibility = ##(#f #t #t)" <<
      std::endl << std::endl;
  }

  // center bar number?
  if (! gGlobalLpsr2lilypondOahGroup->getBoxAroundBarNumberSet ().empty ()) {
    // yes, center the boxed bar number
#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceMeasuresNumbers ()) {
      std::stringstream ss;

      ss <<
        std::endl <<
        "Centering boxed LilyPond measure numbers" <<
        ", line " << elt->getInputLineNumber () << " ===-->";

      gWaeHandler->waeTrace (
        __FILE__, mfInputLineNumber (__LINE__),
        ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED

    fLilypondCodeStream <<
      "\\override Score.BarNumber.self-alignment-X = #CENTER" <<
      std::endl;
  }

  // set dynamics text spanners style?
/* JMI
  \set crescendoText = \markup { \italic { cresc. poco } }
  \set crescendoSpanner = #'text
  \override DynamicTextSpanner.style = #'line
*/
  S_lilypondDynamicsTextSpannersStyleKindAtom
    dynamicsTextSpannersStyleKindAtom =
      gGlobalLpsr2lilypondOahGroup->
        getDynamicsTextSpannersStyleKindAtom ();

  if (dynamicsTextSpannersStyleKindAtom->getSelected ()) {
    fLilypondCodeStream <<
      "\\override DynamicTextSpanner.style = #'" <<
      lpsrDynamicsTextSpannersStyleKindAsString (
       dynamicsTextSpannersStyleKindAtom->
         getLpsrdDynamicsTextSpannersStyleKindVariable ()) <<
      std::endl << std::endl;
  }

  // compress multiple measure rests?
  if (gGlobalLpsr2lilypondOahGroup->getCompressMeasureRestsInLilypond ()) { // JMI 0.9.64
    fLilypondCodeStream << "\\compressMMRests";
    if (gGlobalLpsr2lilypondOahGroup->getCommentLilypondVoices ()) {
      fLilypondCodeStream <<
        " %{ JMI 0.9.64 ??? %} " <<
        std::endl;
    }
    fLilypondCodeStream <<
      "\\set restNumberThreshold = 0" <<
      std::endl << std::endl;
  }

// JMI   \set Score.alternativeNumberingStyle = #'numbers-with-letters

  if (
    gGlobalLpsr2lilypondOahGroup->getAccidentalStyleKind ()
      !=
    lpsrAccidentalStyleKind::kAccidentalStyleDefault
  ) {
    fLilypondCodeStream <<
      "\\accidentalStyle Score." <<
      lpsrAccidentalStyleKindAsString (
        gGlobalLpsr2lilypondOahGroup->getAccidentalStyleKind ()) <<
      std::endl << std::endl;
  }

  // reset fCurrentVoiceClef
  fCurrentVoiceClef = nullptr;

  // reset fCurrentOctaveEntryReference if relevant
  switch (gGlobalLpsr2lilypondOahGroup->fetchOctaveEntryVariableValue ()) {
    case msrOctaveEntryKind::kOctaveEntryRelative:
      // forget about the current reference:
      // it should be set from the LilyPond preferences here
      setCurrentOctaveEntryReferenceFromTheLilypondOah ();
      break;
    case msrOctaveEntryKind::kOctaveEntryAbsolute:
      break;
    case msrOctaveEntryKind::kOctaveEntryFixed:
      break;
  } // switch

  fOnGoingSenzaMisura = false;

  fOnGoingVoice = true;

  switch (fCurrentVoice->getVoiceKind ()) { // JMI
    case msrVoiceKind::kVoiceKindRegular:
    case msrVoiceKind::kVoiceKindDynamics:
      break;

    case msrVoiceKind::kVoiceKindHarmonies:
      fOnGoingHarmoniesVoice = true;
      break;

    case msrVoiceKind::kVoiceKindFiguredBass:
      fOnGoingFiguredBassVoice = true;
      break;
  } // switch

  // reset current voice measures counter
  fCurrentVoiceMeasuresCounter = 0; // none have been found

  // force durations to be displayed explicitly
  // at the beginning of the voice
  fLastMetWholeNotes = K_WHOLE_NOTES_UNKNOWN_;

  // reset current stem kind
  fCurrentStemKind = msrStemKind::kStemKind_NONE; // default value
}

void lpsr2lilypondTranslator::visitEnd (S_msrVoice& elt)
{
  --gIndenter; // incremented in visitStart (S_msrVoice& elt)

#ifdef MF_TRACE_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream ss;

      ss <<
        "% --> End visiting msrVoice \"" <<
        elt->getVoicePathLikeName () <<
        "\"" <<
        ", line " << elt->getInputLineNumber () <<
      std::endl;

      if (traceLpsrVisitors) {
        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << ss.str ();
      }
    }
  }
#endif // MF_TRACE_IS_ENABLED

  /* JMI
  if (
    fCurrentVoice->getVoiceContainsMultipleMeasureRests ()
      ||
    gGlobalLpsr2lilypondOahGroup->getCompressMeasureRestsInLilypond ()
  ) {
    fLilypondCodeStream <<
  // JMI    '}' <<
      std::endl;

    --gIndenter; // JMI ???
  */

  if (gGlobalLpsr2lilypondOahGroup->getDisplayMusic ()) {
    fLilypondCodeStream << '}';

    if (gGlobalLpsr2lilypondOahGroup->getCommentLilypondStructureBasics ()) {
      fLilypondCodeStream <<
        " % displayMusic";
    }

    fLilypondCodeStream << std::endl;

    --gIndenter; // incremented in visitStart (S_msrVoice& elt)
  }

  // generate the end of the voice definition
  fLilypondCodeStream << '}';

  if (gGlobalLpsr2lilypondOahGroup->getCommentLilypondStructureBasics ()) {
    fLilypondCodeStream <<
      " % end of voice " << elt->getVoiceName ();
  }

  fLilypondCodeStream << std::endl << std::endl;

  // set relevant 'ongoing' variables
  switch (elt->getVoiceKind ()) {
    case msrVoiceKind::kVoiceKindRegular:
    case msrVoiceKind::kVoiceKindDynamics:
      break;

    case msrVoiceKind::kVoiceKindHarmonies:
      fOnGoingHarmoniesVoice = false;
      break;

    case msrVoiceKind::kVoiceKindFiguredBass:
      fOnGoingFiguredBassVoice = false;
      break;
  } // switch

  // forget about current voice
  fCurrentVoice = nullptr;

  fOnGoingVoice = false;
}

//________________________________________________________________________
void lpsr2lilypondTranslator::visitStart (S_msrVoiceStaffChange& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream ss;

      ss <<
        "% --> Start visiting msrVoiceStaffChange '" <<
        elt->asString () << '\'' <<
        ", line " << elt->getInputLineNumber () <<
      std::endl;

      if (traceLpsrVisitors) {
        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << ss.str ();
      }
    }
  }
#endif // MF_TRACE_IS_ENABLED

  S_msrStaff
    landingStaff =
      elt->getLandingStaff ();

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceStaffChanges ()) { // JMI 0.9.67
    std::stringstream ss;

    ss <<
      "*** There is staff change" << // for chord member note '" <<
//       fCurrentNote->asShortString () <<
//       "' in voice \"" <<
//       voiceToInsertInto->getPartPathLikeName () <<
//       "\"" <<
//       " from staff " << fPreviousNoteMusicXMLStaffNumber <<
//       " to staff " << landingStaff->asShortString () <<
      ", to staff \"" << landingStaff->getStaffPathLikeName () << "\"" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fLilypondCodeStream << // DISABLED TEMP // JMI 0.9.71
    std::endl <<
    "\\change Staff = \"" <<
    landingStaff->getStaffPathLikeName () <<
    "\"";

  // generate the input line number as comment if relevant
  if (
    gGlobalLpsr2lilypondOahGroup->getInputLineNumbers ()
      ||
    gGlobalLpsr2lilypondOahGroup->getGeneratePositionInMeasures ()
  ) {
    generateInputLineNumberAndOrPositionInMeasureAsAComment (
      elt);
  }

  fLilypondCodeStream << std::endl;
}

//________________________________________________________________________
void lpsr2lilypondTranslator::visitStart (S_msrHarmony& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream ss;

      ss <<
        "% --> Start visiting msrHarmony '" <<
        elt->asString () <<
        '\'' <<
//         ", fCurrentTupletsStack.size (): " <<
//         fCurrentTupletsStack.size () <<
        ", fOnGoingChord: " <<
        fOnGoingChord <<
        ", fOnGoingHarmoniesVoice: " <<
        fOnGoingHarmoniesVoice <<
        ", line " << elt->getInputLineNumber () <<
      std::endl;

      if (traceLpsrVisitors) {
        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << ss.str ();
      }
    }
  }
#endif // MF_TRACE_IS_ENABLED

//   if (! fCurrentTupletsStack.empty ()) {
//   /* JMI 0.9.66
// #ifdef MF_TRACE_IS_ENABLED
//     if (gGlobalLpsr2lilypondOahGroup->getCommentLilypondHarmonies ()) {
//       std::stringstream ss;
//
//       ss <<
//         "%{ fCurrentTupletsStack.size () S_msrHarmony JMI " <<
//         elt->asString () <<
//         " %} ";
//
//       gWaeHandler->waeTrace (
//         __FILE__, mfInputLineNumber (__LINE__),
//         ss.str ());
//     }
// #endif // MF_TRACE_IS_ENABLED
// */
//   }

  if (fOnGoingChord) { // JMI
  }

  else if (fOnGoingHarmoniesVoice) {
    // actual LilyPond code generation
    fLilypondCodeStream <<
      harmonyAsLilypondString (elt) <<
      cLilyPondSpace;

    // generate the input line number as comment if relevant
    if (
      gGlobalLpsr2lilypondOahGroup->getInputLineNumbers ()
        ||
      gGlobalLpsr2lilypondOahGroup->getGeneratePositionInMeasures ()
    ) {
      generateInputLineNumberAndOrPositionInMeasureAsAComment (
        elt);
    }
  }
}

//________________________________________________________________________
void lpsr2lilypondTranslator::visitStart (S_msrFrame& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream ss;

      ss <<
        "% --> Start visiting msrHarmony " <<
        elt->asString () <<
        ", line " << elt->getInputLineNumber () <<
      std::endl;

      if (traceLpsrVisitors) {
        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << ss.str ();
      }
    }
  }
#endif // MF_TRACE_IS_ENABLED

//   if (! fCurrentTupletsStack.empty ()) {
// #ifdef MF_TRACE_IS_ENABLED
//     if (gTraceOahGroup->getTraceFrames ()) {
//       generateComment (elt->asString ());
//     }
// #endif // MF_TRACE_IS_ENABLED
//
//     fLilypondCodeStream <<
//       frameAsLilypondString (elt) <<
//       std::endl;
//   }
}

void lpsr2lilypondTranslator::generateComment (
  const std::string& comment)
{
  fLilypondCodeStream <<
    "%{ " <<comment << " %} ";
}

//________________________________________________________________________
void lpsr2lilypondTranslator::visitStart (S_msrFiguredBass& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream ss;

      ss <<
        "% --> Start visiting msrFiguredBass '" <<
        elt->asString () <<
        '\'' <<
//         ", fCurrentTupletsStack.size (): " <<
//         fCurrentTupletsStack.size () <<
        ", fOnGoingChord: " <<
        fOnGoingChord <<
        ", fOnGoingFiguredBassVoice: " <<
        fOnGoingFiguredBassVoice <<
        ", line " << elt->getInputLineNumber () <<
      std::endl;

      if (traceLpsrVisitors) {
        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << ss.str ();
      }
    }
  }
#endif // MF_TRACE_IS_ENABLED

  // handle the figured bass only when attached to a note in a regular voice,
  // since it needs to be attached to the latter
  Bool doHandleFiguredBass;

  switch (fCurrentVoice->getVoiceKind ()) {
    case msrVoiceKind::kVoiceKindRegular:
//       doHandleFiguredBass = true;
      break;

    case msrVoiceKind::kVoiceKindDynamics:
    case msrVoiceKind::kVoiceKindHarmonies:
      break;

    case msrVoiceKind::kVoiceKindFiguredBass:
      doHandleFiguredBass = true;
      break;
  } // switch

  if (doHandleFiguredBass) {
    fCurrentFiguredBass = elt;

//     if (! fCurrentTupletsStack.empty ()) {
//   #ifdef MF_TRACE_IS_ENABLED
//       if (gGlobalLpsr2lilypondOahGroup->getCommentLilypondFiguredBasses ()) {
//         fLilypondCodeStream <<
//           "%{ fCurrentTupletsStack.size () S_msrFiguredBass JMI " <<
//           fCurrentFiguredBass->asString () <<
//           " %}" <<
//           std::endl;
//       }
//   #endif // MF_TRACE_IS_ENABLED
//     }

    if (fOnGoingChord) { // JMI
    }

    else if (fOnGoingFiguredBassVoice) {
      fLilypondCodeStream <<
        figuredBassAsLilypondString (elt) <<
        cLilyPondSpace;

      // generate the input line number as comment if relevant
      if (
        gGlobalLpsr2lilypondOahGroup->getInputLineNumbers ()
          ||
        gGlobalLpsr2lilypondOahGroup->getGeneratePositionInMeasures ()
      ) {
        generateInputLineNumberAndOrPositionInMeasureAsAComment (
          fCurrentFiguredBass);
      }
    }

    fCurrentFiguredBassFiguresCounter = 0;
  }
}

void lpsr2lilypondTranslator::visitStart (S_msrBassFigure& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream ss;

      ss <<
        "% --> Start visiting msrBassFigure '" <<
        elt->asString () <<
        '\'' <<
        ", line " << elt->getInputLineNumber () <<
      std::endl;

      if (traceLpsrVisitors) {
        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << ss.str ();
      }
    }
  }
#endif // MF_TRACE_IS_ENABLED

  if (fOnGoingFiguredBassVoice) {
    ++fCurrentFiguredBassFiguresCounter;

    // is the figured bass parenthesized?
    msrFiguredBassParenthesesKind
      figuredBassParenthesesKind =
        fCurrentFiguredBass->
          getFiguredBassParenthesesKind ();

    // generate the bass figure number
    switch (figuredBassParenthesesKind) {
      case msrFiguredBassParenthesesKind::kFiguredBassParenthesesYes:
        fLilypondCodeStream << '[';
        break;
      case msrFiguredBassParenthesesKind::kFiguredBassParenthesesNo:
        break;
    } // switch

    fLilypondCodeStream <<
      elt->getFigureNumber ();

    switch (figuredBassParenthesesKind) {
      case msrFiguredBassParenthesesKind::kFiguredBassParenthesesYes:
        fLilypondCodeStream << ']';
        break;
      case msrFiguredBassParenthesesKind::kFiguredBassParenthesesNo:
        break;
    } // switch

    // handle the bass figure prefix
    switch (elt->getFigurePrefixKind ()) {
      case msrBassFigurePrefixKind::kBassFigurePrefix_UNKNOWN_:
        break;
      case msrBassFigurePrefixKind::kBassFigurePrefixDoubleFlat:
        fLilypondCodeStream << "--";
        break;
      case msrBassFigurePrefixKind::kBassFigurePrefixFlat:
        fLilypondCodeStream << "-";
        break;
      case msrBassFigurePrefixKind::kBassFigurePrefixFlatFlat:
        fLilypondCodeStream << "flat flat";
        break;
      case msrBassFigurePrefixKind::kBassFigurePrefixNatural:
        fLilypondCodeStream << "!";
        break;
      case msrBassFigurePrefixKind::kBassFigurePrefixSharpSharp:
        fLilypondCodeStream << "sharp sharp";
        break;
      case msrBassFigurePrefixKind::kBassFigurePrefixSharp:
        fLilypondCodeStream << "+";
        break;
      case msrBassFigurePrefixKind::kBassFigurePrefixDoubleSharp:
        fLilypondCodeStream << "++";
        break;
    } // switch

    // handle the bass figure suffix
    switch (elt->getFigureSuffixKind ()) {
      case msrBassFigureSuffixKind::kBassFigureSuffix_UNKNOWN_:
        break;
      case msrBassFigureSuffixKind::kBassFigureSuffixDoubleFlat:
        fLilypondCodeStream << "double flat";
        break;
      case msrBassFigureSuffixKind::kBassFigureSuffixFlat:
        fLilypondCodeStream << "flat";
        break;
      case msrBassFigureSuffixKind::kBassFigureSuffixFlatFlat:
        fLilypondCodeStream << "flat flat";
        break;
      case msrBassFigureSuffixKind::kBassFigureSuffixNatural:
        fLilypondCodeStream << "natural";
        break;
      case msrBassFigureSuffixKind::kBassFigureSuffixSharpSharp:
        fLilypondCodeStream << "sharp sharp";
        break;
      case msrBassFigureSuffixKind::kBassFigureSuffixSharp:
        fLilypondCodeStream << "sharp";
        break;
      case msrBassFigureSuffixKind::kBassFigureSuffixDoubleSharp:
        fLilypondCodeStream << "souble sharp";
        break;
      case msrBassFigureSuffixKind::kBassFigureSuffixSlash:
        fLilypondCodeStream << '/';
        break;
    } // switch

    // generate a space if not last bass figure in figured bass
    if (
      fCurrentFiguredBassFiguresCounter
        <
      fCurrentFiguredBass->getFiguredBassFiguresList ().size ()
    ) {
      fLilypondCodeStream << cLilyPondSpace;
    }
  }
}

void lpsr2lilypondTranslator::visitEnd (S_msrFiguredBass& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream ss;

      ss <<
        "% --> End visiting msrFiguredBass '" <<
        elt->asString () <<
        '\'' <<
        ", line " << elt->getInputLineNumber () <<
        std::endl;

      if (traceLpsrVisitors) {
        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << ss.str ();
      }
    }
  }
#endif // MF_TRACE_IS_ENABLED

  if (fOnGoingFiguredBassVoice) {
    fLilypondCodeStream <<
      ">";

    // generate figured bass duration
    const msrTupletFactor&
      figuredBassTupletFactor =
        elt->getFiguredBassTupletFactor ();

    if (figuredBassTupletFactor.isEqualToOne ()) {
      // generate figured bass sounding whole notes if relevant
      fLilypondCodeStream <<
        durationAsLilypondStringIfItShouldBeGenerated (
          elt->getInputLineNumber (),
          elt->getMeasureElementSoundingWholeNotes ());

      if (
        wholeNotesDurationShouldBeGenerated (
          elt->getMeasureElementSoundingWholeNotes ())
      ) {
        generateWholeNotesDurationOnStream (
          elt->getInputLineNumber (),
          elt->getMeasureElementSoundingWholeNotes (),
          fLilypondCodeStream);
      }
    }
    else {
      // generate figured bass display whole notes if relevant and tuplet factor
      fLilypondCodeStream <<
        durationAsLilypondStringIfItShouldBeGenerated (
          elt->getInputLineNumber (),
          elt->getFiguredBassDisplayWholeNotes ());

      if (
        wholeNotesDurationShouldBeGenerated (
          elt->getFiguredBassDisplayWholeNotes ())
      ) {
        generateWholeNotesDurationOnStream (
          elt->getInputLineNumber (),
          elt->getFiguredBassDisplayWholeNotes (),
          fLilypondCodeStream);
      }

      fLilypondCodeStream <<
        "*" <<
        figuredBassTupletFactor.asRational ();
    }

    fLilypondCodeStream <<
      cLilyPondSpace;
  }
}

//________________________________________________________________________
void lpsr2lilypondTranslator::visitStart (S_msrSegment& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream ss;

      ss <<
        "%--> Start visiting msrSegment '" <<
        elt->getSegmentAbsoluteNumber () << '\'' <<
        std::endl;

      if (traceLpsrVisitors) {
        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << ss.str ();
      }
    }
  }
#endif // MF_TRACE_IS_ENABLED

  if (gGlobalLpsr2lilypondOahGroup->getCommentLilypondStructureBasics ()) {
    // generate the end of the voice definition
    fLilypondCodeStream <<
      "% start of segment " <<
      elt->getSegmentAbsoluteNumber () <<
      ", line " <<
      elt->getInputLineNumber () <<
      std::endl;

//     ++gIndenter; // decremented in visitEnd (S_msrSegment& elt)
  }
}

void lpsr2lilypondTranslator::visitEnd (S_msrSegment& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream ss;

      ss <<
        "%--> End visiting msrSegment '" <<
        elt->getSegmentAbsoluteNumber () << '\'' <<
        std::endl;

      if (traceLpsrVisitors) {
        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << ss.str ();
      }
    }
  }
#endif // MF_TRACE_IS_ENABLED

  if (gGlobalLpsr2lilypondOahGroup->getCommentLilypondStructureBasics ()) {
//     --gIndenter; // incremented in visitStart (S_msrSegment& elt)

    fLilypondCodeStream <<
      "% end of segment " <<
      elt->getSegmentAbsoluteNumber () <<
      ", line " <<
      elt->getInputLineNumber () <<
      std::endl;
  }
}

//________________________________________________________________________
void lpsr2lilypondTranslator::visitStart (S_msrMeasure& elt)
{
  mfMeasureNumber
    fCurrentMeasureNumber =
      elt->getMeasureNumber ();

  msrMeasureKind
    measureKind =
      elt->getMeasureKind ();

//   int
//     measurePuristNumber =
//       elt->getMeasurePuristNumber ();

#ifdef MF_TRACE_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream ss;

      ss <<
        "% --> Start visiting msrMeasure '" << fCurrentMeasureNumber << '\'' <<
        ", measureKind: " << measureKind <<
        ", measureEndRegularKind: " <<
        elt-> getMeasureEndRegularKind () <<
        ", fCurrentMeasureNumber: '" << fCurrentMeasureNumber << '\'' <<
        ", fOnGoingMultipleMeasureRests: " << fOnGoingMultipleMeasureRests <<
        ", line " << elt->getInputLineNumber () <<
        std::endl;

      if (traceLpsrVisitors) {
        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << ss.str ();
      }
    }
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_MAINTAINANCE_RUNS_ARE_ENABLED
  if (gWaeOahGroup->getMaintainanceRun ()) { // MAINTAINANCE_RUN
    fLilypondCodeStream <<
      "%{ visitStart (S_msrMeasure&(), measureKind: " <<
      measureKind <<
      " %}" <<
      std::endl;
  }
#endif // MF_MAINTAINANCE_RUNS_ARE_ENABLED

  // register the current measure number in the service run data
  S_mfServiceRunData
    serviceRunData =
      gServiceRunData;

  serviceRunData->
    setCurrentMeasureNumber (
      fCurrentMeasureNumber);

  // generate measure start comment if relevant
  if (gGlobalLpsr2lilypondOahGroup->getCommentLilypondStructureBasics ()) {
    fLilypondCodeStream <<
      "% start of measure " <<
      fCurrentMeasureNumber <<
      " (" <<
      elt->getMeasureKind () <<
      ')' <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;

//     ++gIndenter; // decremented in visitEnd (S_msrMeasure& elt)
  }

  // should we generate a box around this bar number?
  {
    std::set <mfMeasureNumber>::const_iterator
      it =
        gGlobalLpsr2lilypondOahGroup->
          getBoxAroundBarNumberSet ().find (fCurrentMeasureNumber);

    if (
      gGlobalLpsr2lilypondOahGroup->getBoxAroundAllBarNumbers ()
        ||
      it != gGlobalLpsr2lilypondOahGroup->getBoxAroundBarNumberSet ().end ()
    ) {
      // yes, generate a box around the bar number
#ifdef MF_TRACE_IS_ENABLED
      if (gTraceOahGroup->getTraceMeasuresNumbers ()) {
        std::stringstream ss;

        ss <<
          std::endl <<
          "Generating a box around LilyPond measure number '" <<
          fCurrentMeasureNumber <<
          "', line " << elt->getInputLineNumber () << " ===-->";

        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }
#endif // MF_TRACE_IS_ENABLED

      fLilypondCodeStream <<
        "\\boxAroundNextBarNumber" <<
        std::endl;
    }
  }

  // should we reset the measure purist number?
  {
    std::map <mfMeasureNumber, mfMeasureNumber>::const_iterator
      it =
        gGlobalLpsr2lilypondOahGroup->
          getResetMeasureElementMeasureNumberMap ().find (fCurrentMeasureNumber);

    if (
      it
        !=
      gGlobalLpsr2lilypondOahGroup->getResetMeasureElementMeasureNumberMap ().end ()
    ) {
      // yes, reset measure number
      mfMeasureNumber lilypondMeasureNumber = (*it).second;

      if (lilypondMeasureNumber != fCurrentMeasureNumber) {
#ifdef MF_TRACE_IS_ENABLED
        if (gTraceOahGroup->getTraceMeasuresNumbers ()) {
          std::stringstream ss;

          ss <<
            std::endl <<
            "Resetting LilyPond measure number from '" <<
            fCurrentMeasureNumber <<
            "' to " <<
            lilypondMeasureNumber <<
            "', line " << elt->getInputLineNumber () << " ===-->";

          gWaeHandler->waeTrace (
            __FILE__, mfInputLineNumber (__LINE__),
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED

        fLilypondCodeStream <<
          "\\set Score.currentBarNumber = #" <<
          lilypondMeasureNumber <<
          std::endl;
      }
      else {
#ifdef MF_TRACE_IS_ENABLED
        if (gTraceOahGroup->getTraceMeasuresNumbers ()) {
          std::stringstream ss;

          ss <<
            std::endl <<
            "Cannot reset measure LilyPond number from '" <<
            fCurrentMeasureNumber <<
            "' to " <<
            lilypondMeasureNumber <<
            ": they're one and the same" <<
            "', line " << elt->getInputLineNumber () << " ===-->";

          gWaeHandler->waeTrace (
            __FILE__, mfInputLineNumber (__LINE__),
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
      }
    }
  }

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasures ()) {
    std::stringstream ss;

    ss <<
//       std::endl <<
      "<!--=== " <<
      "part ";

    if (fCurrentPart) {
      ss <<
        "\"" << fCurrentPart->getPartPathLikeName () << "\"" <<
        " (partMusicXMLID \"" << fCurrentPart->getPartMusicXMLID () << "\")";
    }
    else {
      ss << "fCurrentPart is [NULL]";
    }

    ss <<
      ", measure \"" << fCurrentMeasureNumber << "\"" <<
      ", measureEndRegularKind: " <<
      elt-> getMeasureEndRegularKind () <<
      ", fCurrentMeasureNumber: " << fCurrentMeasureNumber <<
      ", fOnGoingMultipleMeasureRests: " << fOnGoingMultipleMeasureRests <<
      ", line " << elt->getInputLineNumber () <<
      " ===-->";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // should we show the measure number???
  std::set <mfMeasureNumber>::const_iterator
    it =
      gGlobalLpsr2lilypondOahGroup->
        getShowMeasureNumbersAtMeasureSet ().find (fCurrentMeasureNumber); // JMI variable 0.9.75

  if (
    it
      !=
    gGlobalLpsr2lilypondOahGroup->getShowMeasureNumbersAtMeasureSet ().end ()
  ) {
    // yes, display the measure number
    fLilypondCodeStream <<
      "\\once \\override Score.BarNumber.break-visibility = ##f" <<
      std::endl;
  }

  // take this measure into account for counting
  switch (elt->getMeasureKind ()) {
    case msrMeasureKind::kMeasureKindUnknown:
      ++fCurrentVoiceMeasuresCounter;
      break;

    case msrMeasureKind::kMeasureKindRegular:
      ++fCurrentVoiceMeasuresCounter;
      break;

    case msrMeasureKind::kMeasureKindAnacrusis:
      // keep fCurrentVoiceMeasuresCounter at 0
      break;

    case msrMeasureKind::kMeasureKindIncompleteStandalone:
      ++fCurrentVoiceMeasuresCounter;
      break;

    case msrMeasureKind::kMeasureKindIncompleteLastMeasure:
      ++fCurrentVoiceMeasuresCounter;
      break;

    case msrMeasureKind::kMeasureKindIncompleteLastInRepeatCommonPart:
    case msrMeasureKind::kMeasureKindIncompleteLastInRepeatHookedEnding:
    case msrMeasureKind::kMeasureKindIncompleteLastInRepeatHooklessEnding:
    case msrMeasureKind::kMeasureKindIncompleteNextMeasureAfterCommonPart:
    case msrMeasureKind::kMeasureKindIncompleteNextMeasureAfterHookedEnding:
    case msrMeasureKind::kMeasureKindIncompleteNextMeasureAfterHooklessEnding:
      ++fCurrentVoiceMeasuresCounter;
      break;

    case msrMeasureKind::kMeasureKindOverFlowing:
      ++fCurrentVoiceMeasuresCounter;
      break;

    case msrMeasureKind::kMeasureKindCadenza:
      ++fCurrentVoiceMeasuresCounter;
      break;

    case msrMeasureKind::kMeasureKindMusicallyEmpty:
      ++fCurrentVoiceMeasuresCounter;
      break;
  } // switch

  // force durations to be displayed explicitly
  // for the notes at the beginning of the measure
  fLastMetWholeNotes = K_WHOLE_NOTES_UNKNOWN_;

  // is this the end of a cadenza?
  if (
    fOnGoingVoiceCadenza
      &&
    measureKind != msrMeasureKind::kMeasureKindOverFlowing
  ) {
    if (gGlobalLpsr2lilypondOahGroup->getCommentLilypondStructureBasics ()) {
      fLilypondCodeStream <<
        " % msrMeasureKind::kMeasureKindOverFlowing End" <<
        std::endl;
    }

    fLilypondCodeStream <<
      std::endl;

    fOnGoingVoiceCadenza = false;
  }

  // is this measure musically empty?
  switch (measureKind) {
    case msrMeasureKind::kMeasureKindUnknown:
      {
//         std::stringstream ss;
//
//         ss <<
//           "measure '" << fCurrentMeasureNumber <<
//           "' is of unknown kind in lpsr2lilypondTranslator";
//
// // if (false) // JMI
// //         lpsr2lilypondInternalError (
// //           gServiceRunData->getInputSourceName (),
// //           elt->getInputLineNumber (),
// //           __FILE__, mfInputLineNumber (__LINE__),
// //           ss.str ());
// // else
//         lpsr2lilypondInternalWarning (
//           gServiceRunData->getInputSourceName (),
//           elt->getInputLineNumber (),
//           ss.str ());
      }
      break;

    case msrMeasureKind::kMeasureKindRegular:
      break;

    case msrMeasureKind::kMeasureKindAnacrusis:
      // only generate '\partial' at the beginning of a voice // this code SUPERFLOUS??? JMI 0.9.73
      if (elt->getMeasureIsFirstInVoice ()) {//
        std::string
          upbeatNotesDuration =
            wholeNotesAsLilypondString (
              elt->getInputLineNumber (),
              elt->getMeasureCurrentPositionInMeasure ().asWholeNotes ());

        fLilypondCodeStream <<
          "\\partial " << upbeatNotesDuration <<
          std::endl;
      }
      break;

    case msrMeasureKind::kMeasureKindIncompleteStandalone:
//       if (
//         elt->getMeasureIsFirstInVoice ()
//           &&
//         elt->fetchMeasureUpLinkToScore ()->getScoreMeasuresNumber () > 1
//       ) {
//         // don't generate '\partial' at the beginning of a voice
//         std::string
//           upbeatNotesDuration =
//             wholeNotesAsLilypondString (
//               elt->getInputLineNumber (),
//               elt->getMeasureCurrentPositionInMeasure ());
//
//         fLilypondCodeStream <<
//           "\\partial " << upbeatNotesDuration <<
//           std::endl;
//       }
      break;

    case msrMeasureKind::kMeasureKindIncompleteLastMeasure:
//       if (
//         elt->getMeasureIsFirstInVoice ()
//           &&
//         elt->fetchMeasureUpLinkToScore ()->getScoreMeasuresNumber () > 1
//       ) {
//         // don't generate '\partial' at the beginning of a voice
//         std::string
//           upbeatNotesDuration =
//             wholeNotesAsLilypondString (
//               elt->getInputLineNumber (),
//               elt->getMeasureCurrentPositionInMeasure ());
//
//         fLilypondCodeStream <<
//           "\\partial " << upbeatNotesDuration <<
//           std::endl;
//       }
      break;

    case msrMeasureKind::kMeasureKindIncompleteLastInRepeatCommonPart:
    case msrMeasureKind::kMeasureKindIncompleteLastInRepeatHookedEnding:
    case msrMeasureKind::kMeasureKindIncompleteLastInRepeatHooklessEnding:
    case msrMeasureKind::kMeasureKindIncompleteNextMeasureAfterCommonPart:
    case msrMeasureKind::kMeasureKindIncompleteNextMeasureAfterHookedEnding:
    case msrMeasureKind::kMeasureKindIncompleteNextMeasureAfterHooklessEnding:
//       {
//         mfPositionInMeasure
//           measureCurrentPositionInMeasure =
//             elt->getMeasureCurrentPositionInMeasure ();
//
//         mfWholeNotes
//           fullMeasureWholeNotesDuration =
//             elt->getFullMeasureWholeNotesDuration ();
//
//         // we should set the score measure whole notes in this case
//         mfRational
//           ratioToFullMeasureWholeNotesDuration =
//             measureCurrentPositionInMeasure / fullMeasureWholeNotesDuration;
//
// #ifdef MF_TRACE_IS_ENABLED
//         if (gTraceOahGroup->getTraceMeasuresDetails ()) {
//           int fieldWidth = 27;
//
//           fLilypondCodeStream <<
//             std::left <<
//             "% Setting the measure whole notes for measure " <<
//             std::setw (fieldWidth) <<
//             fCurrentMeasureNumber <<
//             ", line: " << elt->getInputLineNumber () <<
//             std::endl <<
//             std::setw (fieldWidth) <<
//             "% measureCurrentPositionInMeasure: " <<
//             measureCurrentPositionInMeasure.asString () <<
//             std::endl <<
//             std::setw (fieldWidth) <<
//             "% fullMeasureWholeNotesDuration: " <<
//             fullMeasureWholeNotesDuration.asString () <<
//             std::endl <<
//             std::setw (fieldWidth) <<
//             "% ratioToFullMeasureWholeNotesDuration: " <<
//             ratioToFullMeasureWholeNotesDuration.asString () <<
//             std::endl << std::endl;
//         }
// #endif // MF_TRACE_IS_ENABLED
//
//         if (ratioToFullMeasureWholeNotesDuration == mfRational (1, 1)) {
//           std::stringstream ss;
//
//           ss <<
//             "underfull measure '" << fCurrentMeasureNumber <<
//             "' has actually the full measure whole notes";
//
//      // JMI       lpsr2lilypondInternalError (
//           lpsr2lilypondInternalWarning (
//             gServiceRunData->getInputSourceName (),
//             elt->getInputLineNumber (),
//     // JMI        __FILE__, mfInputLineNumber (__LINE__),
//             ss.str ());
//         }
//
//         else {
//           /* JMI
//           fLilypondCodeStream <<
//             "\\set Score.measureLength = #(ly:make-moment " <<
//             measureCurrentPositionInMeasure.toString () <<
//             ")" <<
//             std::endl;
//     */
//
//           // should we generate a line break?
//           if (gGlobalLpsr2lilypondOahGroup->getBreakLinesAtIncompleteRightMeasures ()) {
//             fLilypondCodeStream <<
//               "\\break" <<
//               std::endl;
//           }
//         }
//       }
      break;

    case msrMeasureKind::kMeasureKindOverFlowing:
      if (! fOnGoingVoiceCadenza) {
        mfPositionInMeasure
          measureCurrentPositionInMeasure =
            elt->getMeasureCurrentPositionInMeasure (); // JMI 0.9.72

        fLilypondCodeStream <<
          std::endl;
        if (gGlobalLpsr2lilypondOahGroup->getCommentLilypondMeasures ()) {
          fLilypondCodeStream <<
            "%{ begin kMeasureKindOverFlowing, measure " <<
            fCurrentMeasureNumber <<
            ", % measureCurrentPositionInMeasure: " <<
            measureCurrentPositionInMeasure <<
            " %}" <<
            std::endl;
        }
//           std::endl; // JMI 0.9.72 too much otherwise

//         if (gGlobalLpsr2lilypondOahGroup->getCommentLilypondStructureBasics ()) {
//           fLilypondCodeStream << " % msrMeasureKind::kMeasureKindOverFlowing Start";
//         }
//
//         fLilypondCodeStream << std::endl;

        fOnGoingVoiceCadenza = true;
      }
      break;

    case msrMeasureKind::kMeasureKindCadenza:
      if (! fOnGoingVoiceCadenza) {
        fLilypondCodeStream <<
          std::endl <<
          "\\cadenzaOn" <<
          std::endl;

        if (gGlobalLpsr2lilypondOahGroup->getCommentLilypondStructureBasics ()) {
          fLilypondCodeStream << " % msrMeasureKind::kMeasureKindCadenza Start";
        }

        fLilypondCodeStream << std::endl;

        fLilypondCodeStream <<
          "\\once\\omit Staff.TimeSignature" <<
          std::endl;

        fOnGoingVoiceCadenza = true;
      }
      break;

    case msrMeasureKind::kMeasureKindMusicallyEmpty:
      // DON'T generate a skip and bar check here,
      // this is done in lpsr2lilypondTranslator::visitEnd (S_msrMeasure& elt)
      // to have the msrClefKeyTimeSignatureGroup if any generated before it

      // the code for the rest or skip should be generated now,
      // because there can be a barline as the only element of the measure
      generateMusicallyEmptyMeasure (elt);
      break;
  } // switch
}

void lpsr2lilypondTranslator::generateMusicallyEmptyMeasure (
  S_msrMeasure& measure)
{ // JMI 0.9.72 ???
  // now, to have the msrClefKeyTimeSignatureGroup if any
  // being handled first
  if (gGlobalLpsr2lilypondOahGroup->getCommentLilypondStructureBasics ()) {
    fLilypondCodeStream <<
      "%{ kMeasureKindMusicallyEmpty" <<
      ", line " << measure->getInputLineNumber () <<
      " %} % " <<
      measure->getMeasureNumber () << // JMI 0.9.75    + 1 ???
      std::endl;
  }

  // fetch measure's voice
  S_msrVoice
    measureVoice =
      measure->
        fetchMeasureUpLinkToVoice ();

  // fetch measure's part
  S_msrPart
    measurePart =
      measureVoice->
        getVoiceUpLinkToStaff ()->
          getStaffUpLinkToPart ();

  mfWholeNotes
    partMeasureWholeNotes =
      measurePart->
        fetchPartMeasuresWholeNotesVectorAt (
          measure->getInputLineNumber (),
          measure->getMeasureOrdinalNumberInVoice () - 1);

  // generate the skip name
  // take voice kind into account may be useful for debug
  switch (measureVoice->getVoiceKind ()) {
    case msrVoiceKind::kVoiceKindRegular:
    case msrVoiceKind::kVoiceKindDynamics:
      fLilypondCodeStream << 's'; // 'R' ??? JMI
      if (gGlobalLpsr2lilypondOahGroup->getCommentLilypondMeasures ()) {
        fLilypondCodeStream << " %{ s999 %} ";
      }
      break;

    case msrVoiceKind::kVoiceKindHarmonies:
    case msrVoiceKind::kVoiceKindFiguredBass:
      fLilypondCodeStream << 's'; // 'R' ??? JMI
      if (gGlobalLpsr2lilypondOahGroup->getCommentLilypondMeasures ()) {
        fLilypondCodeStream << " %{ s888 %} ";
      }
      break;
  } // switch

//   mfAssertFalse ( // JMI for debug
//     __FILE__, mfInputLineNumber (__LINE__),
//     "generateMusicallyEmptyMeasure()");

  // generate the duration of the skip from the full measure whole notes JMI 0.9.68
  fLilypondCodeStream <<
    wholeNotesAsLilypondString (
      measure->getInputLineNumber (),
      measure->getFullMeasureWholeNotesDuration ());

  if (gGlobalLpsr2lilypondOahGroup->getInputLineNumbers ()) {
    // generate information and line number as a comment
    fLilypondCodeStream <<
      " %{ <-- line " <<
      measure->getInputLineNumber () <<
      " %} ";
  }

  if (gGlobalLpsr2lilypondOahGroup->getNotesComments ()) {
    // generate information and line number as a comment
    fLilypondCodeStream <<
      " %{ kMeasureKindMusicallyEmpty %} ";
  }
}

void lpsr2lilypondTranslator::visitEnd (S_msrMeasure& elt)
{
  msrMeasureKind
    measureKind =
      elt->getMeasureKind ();

//   int
//     measurePuristNumber =
//       elt->getMeasurePuristNumber ();

#ifdef MF_TRACE_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream ss;

      ss <<
        "% --> End visiting msrMeasure '" <<
        fCurrentMeasureNumber <<
        ", measureKind: '" << measureKind << '\'' <<
        ", measureEndRegularKind: '" <<
        elt-> getMeasureEndRegularKind () <<
        '\'' <<
        ", elt->getMeasureNumber (): '" << elt->getMeasureNumber () << '\'' <<
        ", fOnGoingMultipleMeasureRests: " << fOnGoingMultipleMeasureRests <<
        ", line " << elt->getInputLineNumber () <<
        std::endl;

      if (traceLpsrVisitors) {
        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << ss.str ();
      }
    }
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasures ()) {
    std::stringstream ss;

    ss <<
//       std::endl <<
      "<!--=== " <<
      "part ";

    if (fCurrentPart) {
      ss <<
        "\"" << fCurrentPart->getPartPathLikeName () << "\"" <<
        " (partMusicXMLID \"" << fCurrentPart->getPartMusicXMLID () << "\")";
    }
    else {
      ss << "fCurrentPart is [NULL]";
    }

    ss <<
      ", fCurrentMeasureNumber \"" << fCurrentMeasureNumber << "\"" <<
      ", measureEndRegularKind: '" <<
      elt-> getMeasureEndRegularKind () <<
      ", elt->getMeasureNumber (): " << elt->getMeasureNumber () << '\'' <<
      ", fOnGoingMultipleMeasureRests: " << fOnGoingMultipleMeasureRests <<
      ", line " << elt->getInputLineNumber () <<
      " ===-->";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // is there a pending trill spanner for stop?
  if (fPendingTrillSpannerForStop) { // JMI 0.9.67
#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceSpanners ()) {
      std::stringstream ss;

      ss <<
        "Generating code for pending spanner " <<
        fPendingTrillSpannerForStop->getSpannerKind () <<
        " upon the end of measure " <<
        fCurrentMeasureNumber <<
        ", line " << elt->getInputLineNumber ();

      gWaeHandler->waeTrace (
        __FILE__, mfInputLineNumber (__LINE__),
        ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED

    // get spanner start end
    S_msrSpanner
      spannerStartEnd =
        fPendingTrillSpannerForStop->
          getSpannerSideLinkToOtherEnd ();

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
    // sanity check
    mfAssert (
      __FILE__, mfInputLineNumber (__LINE__),
      spannerStartEnd != nullptr,
      "spannerStartEnd is NULL");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

    if (spannerStartEnd->getSpannerUpLinkToNote ()->getNoteTrillOrnament ()) {
      fLilypondCodeStream << "\\stopTrillSpan ";
      if (gGlobalLpsr2lilypondOahGroup->getCommentLilypondMeasures ()) {
        fLilypondCodeStream <<
          "%{ XXX lpsr2lilypondTranslator::visitEnd (S_msrMeasure& elt) %} ";
      }
    }
    else {
      fLilypondCodeStream << "\\stopTextSpan ";
      if (gGlobalLpsr2lilypondOahGroup->getCommentLilypondMeasures ()) {
        fLilypondCodeStream <<
          "%{ YYY lpsr2lilypondTranslator::visitEnd (S_msrMeasure& elt) %} ";
      }
    }

    fPendingTrillSpannerForStop = nullptr;
  }

  if (! fOnGoingMultipleMeasureRests) {
    // handle the measure
    switch (measureKind) {
      case msrMeasureKind::kMeasureKindUnknown: // should not occur
        if (gGlobalLpsr2lilypondOahGroup->getCommentLilypondMeasures ()) {
          fLilypondCodeStream <<
            "%{ measureKindUnknown, " <<
            elt->getMeasureNumber () << // JMI 0.9.75     + 1 ???
            " %}" <<
            std::endl;
        }
        break;

      case msrMeasureKind::kMeasureKindRegular:
        break;

      case msrMeasureKind::kMeasureKindAnacrusis:
        break;

      case msrMeasureKind::kMeasureKindIncompleteStandalone:
        break;

      case msrMeasureKind::kMeasureKindIncompleteLastMeasure:
        break;

      case msrMeasureKind::kMeasureKindIncompleteLastInRepeatCommonPart:
      case msrMeasureKind::kMeasureKindIncompleteLastInRepeatHookedEnding:
      case msrMeasureKind::kMeasureKindIncompleteLastInRepeatHooklessEnding:
      case msrMeasureKind::kMeasureKindIncompleteNextMeasureAfterCommonPart:
      case msrMeasureKind::kMeasureKindIncompleteNextMeasureAfterHookedEnding:
      case msrMeasureKind::kMeasureKindIncompleteNextMeasureAfterHooklessEnding:
        break;

      case msrMeasureKind::kMeasureKindOverFlowing:
        fLilypondCodeStream <<
          std::endl;
        if (gGlobalLpsr2lilypondOahGroup->getCommentLilypondMeasures ()) {
          fLilypondCodeStream <<
            "%{ end kMeasureKindOverFlowing, measure " <<
            fCurrentMeasureNumber <<
            " %}" <<
            std::endl;
        }
        fOnGoingVoiceCadenza = false;
        break;

      case msrMeasureKind::kMeasureKindCadenza:
        fLilypondCodeStream <<
          std::endl <<
          "\\cadenzaOff" <<
          std::endl <<
          "\\bar \"|\"" << // JMI ???
          std::endl;

        fOnGoingVoiceCadenza = false;
        break;

      case msrMeasureKind::kMeasureKindMusicallyEmpty:
//         {
//           // only now, to have the msrClefKeyTimeSignatureGroup if any
//           // being handled first // JMI 0.9.72 ???
//           if (gGlobalLpsr2lilypondOahGroup->getCommentLilypondStructureBasics ()) {
//             fLilypondCodeStream <<
//               "%{ kMeasureKindMusicallyEmpty" <<
//               ", line " << elt->getInputLineNumber () <<
//               " %} % " <<
//               measurePuristNumber + 1 <<
//               std::endl;
//           }
//
//           // fetch measure's voice
//           S_msrVoice
//             measureVoice =
//               elt->
//                 fetchMeasureUpLinkToVoice ();
//
//           // fetch measure's part
//           S_msrPart
//             measurePart =
//               measureVoice->
//                 getVoiceUpLinkToStaff ()->
//                   getStaffUpLinkToPart ();
//
//           mfWholeNotes
//             partMeasureWholeNotes =
//               measurePart->
//                 fetchPartMeasuresWholeNotesVectorAt (
//                   elt->getInputLineNumber (),
//                   elt->getMeasureOrdinalNumberInVoice () - 1);
//
//           // generate the skip name
//           // take voice kind into account may be useful for debug
//           switch (measureVoice->getVoiceKind ()) {
//             case msrVoiceKind::kVoiceKindRegular:
//             case msrVoiceKind::kVoiceKindDynamics:
//               fLilypondCodeStream <<
//   //               'R';
//                 's' << " %{ s999 %} ";
//               break;
//
//             case msrVoiceKind::kVoiceKindHarmonies:
//             case msrVoiceKind::kVoiceKindFiguredBass:
//               fLilypondCodeStream <<
//   //               'R';
//                 's' << " %{ s888 %} ";
//               break;
//           } // switch
//
//           // generate the duration of the skip from the full measure whole notes JMI 0.9.68
//           fLilypondCodeStream <<
//             wholeNotesAsLilypondString (
//               elt->getInputLineNumber (),
//               elt->getFullMeasureWholeNotesDuration ());
//
//           if (gGlobalLpsr2lilypondOahGroup->getInputLineNumbers ()) {
//             // generate information and line number as a comment
//             fLilypondCodeStream <<
//               " %{ <-- line " <<
//               elt->getInputLineNumber () <<
//               " %} ";
//           }
//
//           if (gGlobalLpsr2lilypondOahGroup->getNotesComments ()) {
//             // generate information and line number as a comment
//             fLilypondCodeStream <<
//               " %{ kMeasureKindMusicallyEmpty %} ";
//           }
//
          // generate a bar check
          fLilypondCodeStream <<
            " | % " <<
            elt->getNextMeasureNumber () <<
            std::endl;
//         }
        break;
    } // switch

    if (gGlobalLpsr2lilypondOahGroup->getCommentLilypondStructureBasics ()) {
 // JMI     --gIndenter; // incremented in visitStart (S_msrMeasure& elt)

      fLilypondCodeStream <<
        "% end of measure " <<
        fCurrentMeasureNumber <<
        " (" <<
        elt->getMeasureKind () <<
        ")" <<
        ", line " << elt->getInputLineNumber () <<
        std::endl << std::endl;
    }

    if (gGlobalLpsr2lilypondOahGroup->getSeparatorLineEveryNMeasures () > 0) {
      if (
        fCurrentVoiceMeasuresCounter
          %
        gGlobalLpsr2lilypondOahGroup->getSeparatorLineEveryNMeasures () == 0
      ) {
        fLilypondCodeStream <<
          std::endl <<
          "% ============================= " <<
          std::endl << std::endl;
      }
    }
  }

  // should we generate a line break?
  {
    std::set <mfMeasureNumber>::const_iterator
      it =
        gGlobalLpsr2lilypondOahGroup->
          getBreakLineAfterMeasureNumberSet ().find (fCurrentMeasureNumber);

    if (it != gGlobalLpsr2lilypondOahGroup->getBreakLineAfterMeasureNumberSet ().end ()) {
      // yes, generate a line break command
  #ifdef MF_TRACE_IS_ENABLED
        if (gTraceOahGroup->getTraceMeasuresNumbers ()) {
          std::stringstream ss;

          ss <<
            std::endl <<
            "Adding a LilyPond line break after measure number '" <<
            fCurrentMeasureNumber <<
            "', line " << elt->getInputLineNumber () << " ===-->";

          gWaeHandler->waeTrace (
            __FILE__, mfInputLineNumber (__LINE__),
            ss.str ());
        }
  #endif // MF_TRACE_IS_ENABLED

        fLilypondCodeStream <<
          "\\break" <<
          std::endl;
    }

  /* JMI
    else {
  #ifdef MF_TRACE_IS_ENABLED
     if (gTraceOahGroup->getTraceLineBreaks ()) { // JMI
        ss <<
          std::endl <<
          "Measure number '" <<
          fCurrentMeasureNumber <<
          "' not found in gGlobalLpsr2lilypondOahGroup->getBreakLineAfterMeasureNumberSet ()" <<
          ", line " << elt->getInputLineNumber ();

        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }
  #endif // MF_TRACE_IS_ENABLED
    }
    */
  }

  // should we generate a page break?
  {
    std::set <mfMeasureNumber>::const_iterator
      it =
        gGlobalLpsr2lilypondOahGroup->
          getBreakPageAfterMeasureNumberSet ().find (fCurrentMeasureNumber);

    if (it != gGlobalLpsr2lilypondOahGroup->getBreakPageAfterMeasureNumberSet ().end ()) {
      // yes, generate a page break command
  #ifdef MF_TRACE_IS_ENABLED
        if (gTraceOahGroup->getTracePageBreaks ()) {
          std::stringstream ss;

          ss <<
            std::endl <<
            "Adding a LilyPond page break after measure number '" <<
            fCurrentMeasureNumber <<
            "', line " << elt->getInputLineNumber () << " ===-->";

          gWaeHandler->waeTrace (
            __FILE__, mfInputLineNumber (__LINE__),
            ss.str ());
        }
  #endif // MF_TRACE_IS_ENABLED

        fLilypondCodeStream <<
          "\\pageBreak" <<
          std::endl;
    }

  /* JMI
    else {
  #ifdef MF_TRACE_IS_ENABLED
     if (gTraceOahGroup->getTracePageBreaks ()) { // JMI
        std::stringstream ss;

        ss <<
          std::endl <<
          "Measure number '" <<
          fCurrentMeasureNumber <<
          "' not found in gGlobalLpsr2lilypondOahGroup->getBreakPageAfterMeasureNumberSet ()" <<
          ", line " << elt->getInputLineNumber ();

        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }
  #endif // MF_TRACE_IS_ENABLED
    }
 */
  }

//   if (gGlobalLpsr2lilypondOahGroup->getCommentLilypondStructureBasics ()) {
// //     ++gIndenter; // incremented in visitStart (S_msrMeasure& elt)
//   }
}

//________________________________________________________________________
void lpsr2lilypondTranslator::visitStart (S_msrStanza& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream ss;

      ss <<
        "% --> Start visiting msrStanza \"" <<
        elt->getStanzaName () <<
        "\"" <<
        ", line " << elt->getInputLineNumber () <<
      std::endl;

      if (traceLpsrVisitors) {
        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << ss.str ();
      }
    }
  }
#endif // MF_TRACE_IS_ENABLED

  if (! gGlobalLpsr2lilypondOahGroup->getNoLilypondLyrics ()) {
    // don't generate the stanza inside the code for the voice
    gLog << "fOnGoingVoice: " << fOnGoingVoice << std::endl;
    gLog << "elt->getStanzaTextPresent (): " << elt->getStanzaTextPresent () << std::endl;

    fGenerateOngoingNonEmptyStanza =
      ! fOnGoingVoice
        &&
      elt->getStanzaTextPresent ();

    if (fGenerateOngoingNonEmptyStanza) {
      fLilypondCodeStream <<
        elt->getStanzaPathLikeName () << " = " << "\\lyricmode {" <<
        std::endl;

      ++gIndenter; // decremented in visitEnd (S_msrStanza& elt)

      switch (gGlobalLpsr2lilypondOahGroup->getLyricsNotesDurationsKind ()) {
        case lpsrLyricsNotesDurationsKind::kLyricsNotesDurationsAutomatic:
//           fLilypondCodeStream <<
//             "\\lyricsto \"" <<
//             elt->
//               getStanzaUpLinkToVoice ()->getVoicePathLikeName () <<
//             "\"" <<
//             std::endl;
          fLilypondCodeStream <<
            "\\set ignoreMelismata = ##t" <<
            std::endl;
          break;

        case lpsrLyricsNotesDurationsKind::kLyricsNotesDurationsImplicit:
          fLilypondCodeStream <<
            // set associatedVoice so that
            // both double hyphens and double underscores can be used
            // to draw hyphenated lines and extenders under melismata correctly
            "\\set associatedVoice = #\"" <<
            elt->
              getStanzaUpLinkToVoice ()->getVoicePathLikeName () <<
            "\"" <<
            std::endl <<
            "\\set ignoreMelismata = ##t" <<
            std::endl;
          break;

        case lpsrLyricsNotesDurationsKind::kLyricsNotesDurationsExplicit:
          fLilypondCodeStream <<
            // set associatedVoice so that
            // both double hyphens and double underscores can be used
            // to draw hyphenated lines and extenders under melismata correctly
            "\\set associatedVoice = #\"" <<
            elt->
              getStanzaUpLinkToVoice ()->getVoicePathLikeName () <<
            "\"" <<
            std::endl <<
            "\\set ignoreMelismata = ##t" <<
            std::endl;
          break;
      } // switch
    }
  }

  fCurrentStanza = elt;
  fOnGoingStanza = true;
}

void lpsr2lilypondTranslator::visitEnd (S_msrStanza& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream ss;

      ss <<
        "% --> End visiting msrStanza \"" <<
        elt->getStanzaName () <<
        "\"" <<
        ", line " << elt->getInputLineNumber () <<
      std::endl;

      if (traceLpsrVisitors) {
        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << ss.str ();
      }
    }
  }
#endif // MF_TRACE_IS_ENABLED

  if (! gGlobalLpsr2lilypondOahGroup->getNoLilypondLyrics ()) {
    if (fGenerateOngoingNonEmptyStanza) {
      --gIndenter; // incremented in visitStart (S_msrStanza& elt)

      fLilypondCodeStream <<
        std::endl <<
        "} ";
      if (gGlobalLpsr2lilypondOahGroup->getCommentLilypondLyrics ()) {
        fLilypondCodeStream <<
          "%{ FOFO FOFO %}";
      }
      fLilypondCodeStream <<
        std::endl << std::endl;
    }

    fGenerateOngoingNonEmptyStanza = false;
  }

  fOnGoingStanza = false;
}

//________________________________________________________________________
void lpsr2lilypondTranslator::visitStart (S_msrSyllable& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream ss;

      ss <<
        "% --> Start visiting msrSyllable '" <<
        elt->asString () <<
        '\'' <<
        ", line " << elt->getInputLineNumber () <<
      std::endl;

      if (traceLpsrVisitors) {
        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << ss.str ();
      }
    }
  }
#endif // MF_TRACE_IS_ENABLED

  if (! gGlobalLpsr2lilypondOahGroup->getNoLilypondLyrics ()) {
//     gLog << "fGenerateOngoingNonEmptyStanza: " << fGenerateOngoingNonEmptyStanza << std::endl;

    if (fGenerateOngoingNonEmptyStanza) { // JMI 0.9.73
      generateLilypondSyllable (elt);
    }
  }
}

void lpsr2lilypondTranslator::generateSyllableDescripionAsComment (
  const S_msrSyllable& syllable)
{
  // get the note the syllable is attached to
  S_msrNote
    noteTheSyllableIsAttachedTo =
      syllable->getSyllableUpLinkToNote ();

  constexpr int fieldWidth = 29;

  fLilypondCodeStream << std::left <<
    std::endl <<
    "%{ ================= SYLLABLE DESCRIPTION ================="  <<
    std::endl;

  gIndenter++;

  fLilypondCodeStream << std::left <<
    std::setw (fieldWidth) <<
    "getSyllableElementsList" << ": " <<
    syllableElementsListAsLilypondString (
      syllable->getSyllableElementsList ()) <<
    std::endl <<

    std::setw (fieldWidth) <<
    "getSyllableKind" << ": " <<
    syllable->getSyllableKind () <<
    std::endl <<

    std::endl <<

    std::setw (fieldWidth) <<
    "noteTheSyllableIsAttachedTo" << ": ";

  if (noteTheSyllableIsAttachedTo) {
    fLilypondCodeStream <<
      noteTheSyllableIsAttachedTo->
        noteCoreAsString () <<
        ", measure '" <<
      noteTheSyllableIsAttachedTo->
        getMeasureElementUpLinkToMeasure ()->
          getMeasureNumber () <<
      '\'' <<
      std::endl;

    // get the note ties list
    const std::list <S_msrTie>& noteTiesList =
      noteTheSyllableIsAttachedTo->getNoteTiesList ();

    if (! noteTiesList.empty ()) {
      for (S_msrTie noteTie : noteTiesList) {
        fLilypondCodeStream << std::left <<
          std::setw (fieldWidth) <<
          "noteTieKind" << ": " <<
          noteTie->getTieKind () <<
          std::endl;
      } // for
    }

    // get the note slurs list
    fLilypondCodeStream <<
      std::setw (fieldWidth) <<
      "noteSlursList" << ": ";

    const std::list <S_msrSlur>&
      noteSlursList =
        noteTheSyllableIsAttachedTo->getNoteSlursList ();

    // take note's slurs list into account if any
    if (! noteSlursList.empty ()) {
      fLilypondCodeStream <<
        noteSlursList.size ();
    }
    else {
      fLilypondCodeStream << "[EMPTY]";
    }
  }
  else {
    fLilypondCodeStream << "[NULL]";
  }
  fLilypondCodeStream << std::endl;

  fLilypondCodeStream << std::endl;

  fLilypondCodeStream << std::left <<
//     std::setw (fieldWidth) <<
//     "wholeNotes" << ": " <<
//     noteTheSyllableIsAttachedTo->getMeasureElementSoundingWholeNotes () <<
//     std::endl <<

    std::setw (fieldWidth) <<
    "getLyricsNotesDurationsKind" << ": " <<
    gGlobalLpsr2lilypondOahGroup->getLyricsNotesDurationsKind () <<
    std::endl <<

    std::setw (fieldWidth) <<
    "getSyllableExtendKind" << ": " <<
    syllable->getSyllableExtendKind () <<
    std::endl;

  fLilypondCodeStream <<
    std::setw (fieldWidth) <<
    "fOnGoingExtend" << ": " <<
    fOnGoingExtend <<
    std::endl;

  fLilypondCodeStream << std::left <<
    std::setw (fieldWidth) <<
    "line" << ": " << syllable->getInputLineNumber () <<
    std::endl;

  fLilypondCodeStream <<
    "========================================================" <<
    std::endl;

  gIndenter--;

  fLilypondCodeStream <<
    "%}"  <<
    std::endl;
}

void lpsr2lilypondTranslator::generateLilypondSyllable (
  const S_msrSyllable& syllable)
{
/*
From Lars Opfermann | soundnotation <Lars.Opfermann@soundnotation.com> :

June 2024 : Here are the files without cue, just normal notes. Implicit lyrics placement still missing skips, explicit missing hyphens.

Older:

The Finale version is correct. There are two words linked via the elision: tesoro and intanto.

It looks like recent changes in the handling of this causes the wrong result of xml2ly because I tested it with my currently installed version and it leads to the same result as in the Lilypond file I sent you before with the missing elision (te-so-in-tan-to).
I would say that in case of an elision this parameter needs special interpretation.

Since an elision links two words, there always is a <syllabic>end</syllabic> or a a <syllabic>single</syllabic> at first.

<lyric number='1' name='verse' default-y='-80'>
                <syllabic>end</syllabic>
                <text>ro</text>
                <elision>‿</elision>
                <syllabic>begin</syllabic>
                <text>in</text>
                <extend/>
</lyric>

If thus the last respective parameter <syllabic>begin</syllabic> would be interpreted as <syllabic>middle</syllabic> and <syllabic>single</syllabic> as<syllabic>end</syllabic> it would correspond to the handling of text without elision.
*/



/*
    for idx, elem in enumerate(voice._elements):
        lyrics = elem.get('lyric', []) # only <note> contains <lyric>
        found_matching_lyric = False
        for lyric in lyrics:
            if getattr(lyric, 'number', None) == lyric_key:
                found_matching_lyric = True
                text = musicxml_lyrics_to_text(lyric, None)
                result.append(text)
        if found_matching_lyric:
            pass
        elif len(lyrics) > 0: # any lyric
            result.append(r'\skip1 ')
        # Note does not have any lyric attached to it.
        elif is_chord(elem):
            # note without lyrics part of a chord. MusicXML format is
            # unclear if a chord element could contain a lyric, lets
            # asume that we do not want to put a skip here.
            continue
        elif is_note_and_not_rest(elem):
            result.append(r'\skip1 ')

    lyrics_dict[lyric_key].extend(result)
*/

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalLpsr2lilypondOahGroup->getCommentLilypondLyrics ()) {
    generateSyllableDescripionAsComment (
      syllable);
  }
#endif // MF_TRACE_IS_ENABLED

//   generateCodeBeforeSyllableIfRelevant (syllable);

  // get the note the syllable is attached to
  S_msrNote
    noteTheSyllableIsAttachedTo =
      syllable->getSyllableUpLinkToNote ();

  switch (syllable->getSyllableKind ()) {

    // ----------------------------------------------------
    case msrSyllableKind::kSyllableNone: // JMI 0.9.70
    // ----------------------------------------------------
#ifdef MF_TRACE_IS_ENABLED
          if (gGlobalLpsr2lilypondOahGroup->getCommentLilypondLyrics ()) {
            fLilypondCodeStream <<
              "%{ GENERATE_CODE_FOR_SYLLABLE_kSyllableNone" <<
              ", line " << syllable->getInputLineNumber () <<
              " %}" <<
              std::endl;
          }
#endif // MF_TRACE_IS_ENABLED
      break;

    // ----------------------------------------------------
    case msrSyllableKind::kSyllableSingle:
    // ----------------------------------------------------
      // generate the syllable elements
      fLilypondCodeStream <<
        syllableElementsListAsLilypondString (
          syllable->getSyllableElementsList ()) <<
        cLilyPondSpace;

      switch (gGlobalLpsr2lilypondOahGroup->getLyricsNotesDurationsKind ()) {
        case lpsrLyricsNotesDurationsKind::kLyricsNotesDurationsAutomatic:
          {
#ifdef MF_TRACE_IS_ENABLED
            if (gGlobalLpsr2lilypondOahGroup->getCommentLilypondLyrics ()) {
              fLilypondCodeStream <<
                "%{ CODE_FOR_SYLLABLE_DURATION_KIND_AUTOMATIC_kSyllableSingle" <<
                ", line " << syllable->getInputLineNumber () <<
                " %}" <<
                std::endl;
            }
#endif // MF_TRACE_IS_ENABLED
          }
          break;

        case lpsrLyricsNotesDurationsKind::kLyricsNotesDurationsImplicit:
#ifdef MF_TRACE_IS_ENABLED
          if (gGlobalLpsr2lilypondOahGroup->getCommentLilypondLyrics ()) {
            fLilypondCodeStream <<
              "%{ CODE_FOR_SYLLABLE_DURATION_KIND_AUTOMATIC_kSyllableSingle" <<
              ", line " << syllable->getInputLineNumber () <<
              " %}" <<
              std::endl;
          }
#endif // MF_TRACE_IS_ENABLED

          break;

        case lpsrLyricsNotesDurationsKind::kLyricsNotesDurationsExplicit:
          {
#ifdef MF_TRACE_IS_ENABLED
            if (gGlobalLpsr2lilypondOahGroup->getCommentLilypondLyrics ()) {
              fLilypondCodeStream <<
                "%{ CODE_FOR_SYLLABLE_DURATION_KIND_EXPLICIT_kSyllableSingle" <<
                ", line " << syllable->getInputLineNumber () <<
                " %}" <<
                std::endl;
            }
#endif // MF_TRACE_IS_ENABLED

            // get the syllable elements as a LilyPond string
//             std::string
//               elementsListAsLilypondString =
//                 syllableElementsListAsLilypondString (
//                   syllable->getSyllableElementsList ());
//
//             fLilypondCodeStream <<
//               syllableElementsListAsLilypondString (
//                 syllable->getSyllableElementsList ()) <<
//               cLilyPondSpace;

            if (noteTheSyllableIsAttachedTo) { // JMI 0.9.70 BABASSE
              fLilypondCodeStream <<
                durationAsLilypondStringIfItShouldBeGenerated (
                  noteTheSyllableIsAttachedTo->getInputLineNumber (),
                  noteTheSyllableIsAttachedTo->getMeasureElementSoundingWholeNotes ());
            }

            fLilypondCodeStream <<
              cLilyPondSpace;
          }
          break;
      } // switch

      fLilypondCodeStream <<
        cLilyPondSpace;
      break;

    // ----------------------------------------------------
    case msrSyllableKind::kSyllableBegin:
    // ----------------------------------------------------
      // generate the syllable elements
      fLilypondCodeStream <<
        syllableElementsListAsLilypondString (
          syllable->getSyllableElementsList ()) <<
        cLilyPondSpace;

      switch (gGlobalLpsr2lilypondOahGroup->getLyricsNotesDurationsKind ()) {
        case lpsrLyricsNotesDurationsKind::kLyricsNotesDurationsAutomatic:
#ifdef MF_TRACE_IS_ENABLED
          if (gGlobalLpsr2lilypondOahGroup->getCommentLilypondLyrics ()) {
            fLilypondCodeStream <<
              "%{ CODE_FOR_SYLLABLE_DURATION_KIND_AUTOMATIC_kSyllableBegin" <<
              ", line " << syllable->getInputLineNumber () <<
              " %}" <<
              std::endl;
          }
#endif // MF_TRACE_IS_ENABLED

//           fLilypondCodeStream <<
//             syllableElementsListAsLilypondString (
//               syllable->getSyllableElementsList ()) <<
//             cLilyPondSpace;
          break;

        case lpsrLyricsNotesDurationsKind::kLyricsNotesDurationsImplicit:
#ifdef MF_TRACE_IS_ENABLED
          if (gGlobalLpsr2lilypondOahGroup->getCommentLilypondLyrics ()) {
            fLilypondCodeStream <<
              "%{ CODE_FOR_SYLLABLE_DURATION_KIND_IMPLICIT_kSyllableBegin" <<
              ", line " << syllable->getInputLineNumber () <<
              " %}" <<
              std::endl;
          }
#endif // MF_TRACE_IS_ENABLED

//           fLilypondCodeStream <<
//             syllableElementsListAsLilypondString (
//               syllable->getSyllableElementsList ()) <<
//             cLilyPondSpace;
          break;

        case lpsrLyricsNotesDurationsKind::kLyricsNotesDurationsExplicit:
#ifdef MF_TRACE_IS_ENABLED
          if (gGlobalLpsr2lilypondOahGroup->getCommentLilypondLyrics ()) {
            fLilypondCodeStream <<
              "%{ CODE_FOR_SYLLABLE_DURATION_KIND_EXPLICIT_kSyllableBegin" <<
              ", line " << syllable->getInputLineNumber () <<
              " %}" <<
              std::endl;
          }
#endif // MF_TRACE_IS_ENABLED

          fLilypondCodeStream <<
//             syllableElementsListAsLilypondString (
//               syllable->getSyllableElementsList ()) <<
//             cLilyPondSpace <<
            durationAsLilypondStringIfItShouldBeGenerated (
              noteTheSyllableIsAttachedTo->getInputLineNumber (),
              noteTheSyllableIsAttachedTo->getMeasureElementSoundingWholeNotes ()) <<
            cLilyPondSpace;
          break;
      } // switch
      break;

    // ----------------------------------------------------
    case msrSyllableKind::kSyllableMiddle:
    // ----------------------------------------------------
      // generate the syllable elements
      fLilypondCodeStream <<
        syllableElementsListAsLilypondString (
          syllable->getSyllableElementsList ()) <<
        cLilyPondSpace;

      switch (gGlobalLpsr2lilypondOahGroup->getLyricsNotesDurationsKind ()) {
        case lpsrLyricsNotesDurationsKind::kLyricsNotesDurationsAutomatic:
#ifdef MF_TRACE_IS_ENABLED
          if (gGlobalLpsr2lilypondOahGroup->getCommentLilypondLyrics ()) {
            fLilypondCodeStream <<
              "%{ CODE_FOR_SYLLABLE_DURATION_KIND_AUTOMATIC_kSyllableMiddle" <<
              ", line " << syllable->getInputLineNumber () <<
              " %}" <<
              std::endl;
          }
#endif // MF_TRACE_IS_ENABLED

//           fLilypondCodeStream <<
//             syllableElementsListAsLilypondString (
//               syllable->getSyllableElementsList ()) <<
//             cLilyPondSpace;
          break;

        case lpsrLyricsNotesDurationsKind::kLyricsNotesDurationsImplicit:
#ifdef MF_TRACE_IS_ENABLED
          if (gGlobalLpsr2lilypondOahGroup->getCommentLilypondLyrics ()) {
            fLilypondCodeStream <<
              "%{ CODE_FOR_SYLLABLE_DURATION_KIND_IMPLICIT_kSyllableMiddle" <<
              ", line " << syllable->getInputLineNumber () <<
              " %}" <<
              std::endl;
          }
#endif // MF_TRACE_IS_ENABLED

//           fLilypondCodeStream << // JMI 0.9.70 FOOFPP
//             syllableElementsListAsLilypondString (
//               syllable->getSyllableElementsList ()) <<
//             cLilyPondSpace;
          break;

        case lpsrLyricsNotesDurationsKind::kLyricsNotesDurationsExplicit:
#ifdef MF_TRACE_IS_ENABLED
          if (gGlobalLpsr2lilypondOahGroup->getCommentLilypondLyrics ()) {
            fLilypondCodeStream <<
              "%{ CODE_FOR_SYLLABLE_DURATION_KIND_EXPLICIT_kSyllableMiddle" <<
              ", line " << syllable->getInputLineNumber () <<
              " %}" <<
              std::endl;
          }
#endif // MF_TRACE_IS_ENABLED

          fLilypondCodeStream <<
//             syllableElementsListAsLilypondString (
//               syllable->getSyllableElementsList ()) <<
//             cLilyPondSpace <<
            durationAsLilypondStringIfItShouldBeGenerated (
              noteTheSyllableIsAttachedTo->getInputLineNumber (),
              noteTheSyllableIsAttachedTo->getMeasureElementSoundingWholeNotes ()) <<
            cLilyPondSpace;
          break;
      } // switch
      break;

    // ----------------------------------------------------
    case msrSyllableKind::kSyllableEnd:
    // ----------------------------------------------------
      // generate the syllable elements
      fLilypondCodeStream <<
        syllableElementsListAsLilypondString (
          syllable->getSyllableElementsList ()) <<
        cLilyPondSpace;

      switch (gGlobalLpsr2lilypondOahGroup->getLyricsNotesDurationsKind ()) {
        case lpsrLyricsNotesDurationsKind::kLyricsNotesDurationsAutomatic:
#ifdef MF_TRACE_IS_ENABLED
          if (gGlobalLpsr2lilypondOahGroup->getCommentLilypondLyrics ()) {
            fLilypondCodeStream <<
              "%{ CODE_FOR_SYLLABLE_DURATION_KIND_AUTOMATIC_kSyllableEnd" <<
              ", line " << syllable->getInputLineNumber () <<
              " %}" <<
              std::endl;
          }
#endif // MF_TRACE_IS_ENABLED

//           fLilypondCodeStream <<
//             syllableElementsListAsLilypondString (
//               syllable->getSyllableElementsList ()) <<
//             cLilyPondSpace;
          break;

        case lpsrLyricsNotesDurationsKind::kLyricsNotesDurationsImplicit:
#ifdef MF_TRACE_IS_ENABLED
          if (gGlobalLpsr2lilypondOahGroup->getCommentLilypondLyrics ()) {
            fLilypondCodeStream <<
              "%{ CODE_FOR_SYLLABLE_DURATION_KIND_IMPLICIT_kSyllableEnd" <<
              ", line " << syllable->getInputLineNumber () <<
              " %}" <<
              std::endl;
          }
#endif // MF_TRACE_IS_ENABLED

//           fLilypondCodeStream <<
//             syllableElementsListAsLilypondString (
//               syllable->getSyllableElementsList ()) <<
//             cLilyPondSpace;

          // forget the last met whole notes duration,
          // to enforce the duration being generated
          // for the first syllable in the next measure
          fLastMetWholeNotes = K_WHOLE_NOTES_UNKNOWN_; // JMI 0.9.67
          break;

        case lpsrLyricsNotesDurationsKind::kLyricsNotesDurationsExplicit:
#ifdef MF_TRACE_IS_ENABLED
          if (gGlobalLpsr2lilypondOahGroup->getCommentLilypondLyrics ()) {
            fLilypondCodeStream <<
              "%{ CODE_FOR_SYLLABLE_DURATION_KIND_EXPLICIT_kSyllableEnd" <<
              ", line " << syllable->getInputLineNumber () <<
              " %}" <<
              std::endl;
          }
#endif // MF_TRACE_IS_ENABLED

//           fLilypondCodeStream <<
//             syllableElementsListAsLilypondString (
//               syllable->getSyllableElementsList ()) <<
//             cLilyPondSpace;

          fLilypondCodeStream <<
            durationAsLilypondStringIfItShouldBeGenerated (
              noteTheSyllableIsAttachedTo->getInputLineNumber (),
              noteTheSyllableIsAttachedTo->getMeasureElementSoundingWholeNotes ()) <<
            cLilyPondSpace;

          // forget the last met whole notes duration,
          // to enforce the duration being generated
          // for the first syllable in the next measure
          fLastMetWholeNotes = K_WHOLE_NOTES_UNKNOWN_; // JMI 0.9.67
          break;
      } // switch
      break;

    // ----------------------------------------------------
    case msrSyllableKind::kSyllableOnRestNote:
    // ----------------------------------------------------
#ifdef MF_TRACE_IS_ENABLED
          if (gGlobalLpsr2lilypondOahGroup->getCommentLilypondLyrics ()) {
            fLilypondCodeStream <<
              "%{ GENERATE_CODE_FOR_SYLLABLE_kSyllableOnRestNote" <<
              ", line " << syllable->getInputLineNumber () <<
              " %}" <<
              std::endl;
          }
#endif // MF_TRACE_IS_ENABLED

      // generate the syllable elements
      fLilypondCodeStream <<
        syllableElementsListAsLilypondString (
          syllable->getSyllableElementsList ()) <<
        cLilyPondSpace;

      switch (gGlobalLpsr2lilypondOahGroup->getLyricsNotesDurationsKind ()) {
        case lpsrLyricsNotesDurationsKind::kLyricsNotesDurationsAutomatic:
#ifdef MF_TRACE_IS_ENABLED
          if (gGlobalLpsr2lilypondOahGroup->getCommentLilypondLyrics ()) {
            fLilypondCodeStream <<
              "%{ CODE_FOR_SYLLABLE_DURATION_KIND_AUTOMATIC_kSyllableOnRestNote" <<
              ", line " << syllable->getInputLineNumber () <<
              " %}" <<
              std::endl;
          }
#endif // MF_TRACE_IS_ENABLED

//           fLilypondCodeStream <<
//             syllableElementsListAsLilypondString (
//               syllable->getSyllableElementsList ()) <<
//             cLilyPondSpace;
          break;

        case lpsrLyricsNotesDurationsKind::kLyricsNotesDurationsImplicit:
#ifdef MF_TRACE_IS_ENABLED
          if (gGlobalLpsr2lilypondOahGroup->getCommentLilypondLyrics ()) {
            fLilypondCodeStream <<
              "%{ CODE_FOR_SYLLABLE_DURATION_KIND_IMPLICIT_kSyllableOnRestNote" <<
              ", line " << syllable->getInputLineNumber () <<
              " %}" <<
              std::endl;
          }
#endif // MF_TRACE_IS_ENABLED

//           fLilypondCodeStream <<
//             syllableElementsListAsLilypondString (
//               syllable->getSyllableElementsList ()) <<
//             cLilyPondSpace;
          break;

        case lpsrLyricsNotesDurationsKind::kLyricsNotesDurationsExplicit:
#ifdef MF_TRACE_IS_ENABLED
          if (gGlobalLpsr2lilypondOahGroup->getCommentLilypondLyrics ()) {
            fLilypondCodeStream <<
              "%{ CODE_FOR_SYLLABLE_DURATION_KIND_EXPLICIT_kSyllableOnRestNote" <<
              ", line " << syllable->getInputLineNumber () <<
              " %}" <<
              std::endl;
          }
#endif // MF_TRACE_IS_ENABLED

//           fLilypondCodeStream <<
//             syllableElementsListAsLilypondString (
//               syllable->getSyllableElementsList ()) <<
//             cLilyPondSpace;

          fLilypondCodeStream <<
            durationAsLilypondStringIfItShouldBeGenerated (
              noteTheSyllableIsAttachedTo->getInputLineNumber (),
              noteTheSyllableIsAttachedTo->getMeasureElementSoundingWholeNotes ()) <<
            cLilyPondSpace;
          break;
      } // switch
      break;

    // ----------------------------------------------------
    case msrSyllableKind::kSyllableSkipOnRestNote:
    // ----------------------------------------------------
      switch (gGlobalLpsr2lilypondOahGroup->getLyricsNotesDurationsKind ()) {
        case lpsrLyricsNotesDurationsKind::kLyricsNotesDurationsAutomatic:
#ifdef MF_TRACE_IS_ENABLED
          if (gGlobalLpsr2lilypondOahGroup->getCommentLilypondLyrics ()) {
            fLilypondCodeStream <<
              "%{ CODE_FOR_SYLLABLE_DURATION_KIND_AUTOMATIC_kSyllableSkipOnRestNote" <<
              ", line " << syllable->getInputLineNumber () <<
              " %}" <<
              std::endl;
          }
#endif // MF_TRACE_IS_ENABLED
//
//             fLilypondCodeStream <<
//               "\\skip";
//             generateWholeNotesDuration (
//               syllable->getInputLineNumber (),
//               syllable->getSyllableWholeNotes ());
//             fLilypondCodeStream <<
//               cLilyPondSpace;
          break;

        case lpsrLyricsNotesDurationsKind::kLyricsNotesDurationsImplicit:
#ifdef MF_TRACE_IS_ENABLED
          if (gGlobalLpsr2lilypondOahGroup->getCommentLilypondLyrics ()) {
            fLilypondCodeStream <<
              "%{ CODE_FOR_SYLLABLE_DURATION_KIND_IMPLICIT_kSyllableSkipOnRestNote" <<
              ", line " << syllable->getInputLineNumber () <<
              " %}" <<
              std::endl;
          }
#endif // MF_TRACE_IS_ENABLED
          break;

        case lpsrLyricsNotesDurationsKind::kLyricsNotesDurationsExplicit:
#ifdef MF_TRACE_IS_ENABLED
          if (gGlobalLpsr2lilypondOahGroup->getCommentLilypondLyrics ()) {
            fLilypondCodeStream <<
              "%{ CODE_FOR_SYLLABLE_DURATION_KIND_EXPLICIT_kSyllableSkipOnRestNote" <<
              ", line " << syllable->getInputLineNumber () <<
              " %}" <<
              std::endl;
          }
#endif // MF_TRACE_IS_ENABLED

          // generate the empty syllable
          fLilypondCodeStream <<
            "\"\"";
          generateWholeNotesDuration (
            syllable->getInputLineNumber (),
            syllable->getSyllableWholeNotes ());
          fLilypondCodeStream <<
            cLilyPondSpace;
          break;
      } // switch

      break;

    // ----------------------------------------------------
    case msrSyllableKind::kSyllableSkipOnNonRestNote:
    // ----------------------------------------------------
      switch (gGlobalLpsr2lilypondOahGroup->getLyricsNotesDurationsKind ()) {
        case lpsrLyricsNotesDurationsKind::kLyricsNotesDurationsAutomatic:
#ifdef MF_TRACE_IS_ENABLED
          if (gGlobalLpsr2lilypondOahGroup->getCommentLilypondLyrics ()) {
            fLilypondCodeStream <<
              "%{ CODE_FOR_SYLLABLE_DURATION_KIND_AUTOMATIC_kSyllableSkipOnNonRestNote" <<
              ", line " << syllable->getInputLineNumber () <<
              " %}" <<
              std::endl;
          }
#endif // MF_TRACE_IS_ENABLED

//             fLilypondCodeStream <<
//               "\\skip";
//             generateWholeNotesDuration (
//               syllable->getInputLineNumber (),
//               syllable->getSyllableWholeNotes ());
//             fLilypondCodeStream <<
//               cLilyPondSpace;
          break;

        case lpsrLyricsNotesDurationsKind::kLyricsNotesDurationsImplicit:
#ifdef MF_TRACE_IS_ENABLED
          if (gGlobalLpsr2lilypondOahGroup->getCommentLilypondLyrics ()) {
            fLilypondCodeStream <<
              "%{ CODE_FOR_SYLLABLE_DURATION_KIND_IMPLICIT_kSyllableSkipOnNonRestNote" <<
              ", line " << syllable->getInputLineNumber () <<
              " %}" <<
              std::endl;
          }
#endif // MF_TRACE_IS_ENABLED

//             fLilypondCodeStream <<
//               "\\skip";
//             generateWholeNotesDuration (
//               syllable->getInputLineNumber (),
//               syllable->getSyllableWholeNotes ());
//             fLilypondCodeStream <<
//               cLilyPondSpace;
          break;

        case lpsrLyricsNotesDurationsKind::kLyricsNotesDurationsExplicit:
#ifdef MF_TRACE_IS_ENABLED
          if (gGlobalLpsr2lilypondOahGroup->getCommentLilypondLyrics ()) {
            fLilypondCodeStream <<
              "%{ CODE_FOR_SYLLABLE_DURATION_KIND_EXPLICIT_kSyllableSkipOnNonRestNote" <<
              ", line " << syllable->getInputLineNumber () <<
              " %}" <<
              std::endl;
          }
#endif // MF_TRACE_IS_ENABLED

          // generate the empty syllable
          fLilypondCodeStream <<
            "\"\"";
          generateWholeNotesDuration (
            syllable->getInputLineNumber (),
            syllable->getSyllableWholeNotes ());
          fLilypondCodeStream <<
            cLilyPondSpace;
          break;
      } // switch

      break;

    // ----------------------------------------------------
    case msrSyllableKind::kSyllableMeasureEnd:
    // ----------------------------------------------------
      {
#ifdef MF_TRACE_IS_ENABLED
        if (gGlobalLpsr2lilypondOahGroup->getCommentLilypondLyrics ()) {
          fLilypondCodeStream <<
            "%{ GENERATE_CODE_FOR_SYLLABLE_kSyllableMeasureEnd" <<
            ", line " << syllable->getInputLineNumber () <<
            " %}" <<
            std::endl;
        }
#endif // MF_TRACE_IS_ENABLED

        switch (gGlobalLpsr2lilypondOahGroup->getLyricsNotesDurationsKind ()) {
          case lpsrLyricsNotesDurationsKind::kLyricsNotesDurationsAutomatic:
            // don't generate a bar check:
            // the rests are not present in stanzas in aotomatic mode,
            // in which which case LilyPond complains about too short measures
            break;

          case lpsrLyricsNotesDurationsKind::kLyricsNotesDurationsImplicit:
            // a continue type extension is not mandatory
              fLilypondCodeStream <<
                "| ";
            break;

          case lpsrLyricsNotesDurationsKind::kLyricsNotesDurationsExplicit:
              fLilypondCodeStream <<
                "| ";
            break;
        } // switch

        fLilypondCodeStream <<
          "% " <<
          syllable->fetchSyllableMeasurePuristNumber () + 1<<
          cLilyPondSpace;

        if (noteTheSyllableIsAttachedTo) {
          S_msrMeasure
            noteUpLinkToMeasure =
              noteTheSyllableIsAttachedTo->getMeasureElementUpLinkToMeasure ();

          if (noteUpLinkToMeasure) {
            fLilypondCodeStream <<
              "% " <<
              noteUpLinkToMeasure->getNextMeasureNumber ();
          }
        }
//         else {
//           if (gGlobalLpsr2lilypondOahGroup->getCommentLilypondLyrics ()) {
//             fLilypondCodeStream <<
//               " %{ noteTheSyllableIsAttachedTo is NULL %} ";
//           }
//         }

        if (gGlobalLpsr2lilypondOahGroup->getInputLineNumbers ()) {
          // generate information and line number as a comment
          fLilypondCodeStream <<
            " %{ <-- line " <<
            syllable->getInputLineNumber () <<
            " %} ";
        }

        if (gGlobalLpsr2lilypondOahGroup->getNotesComments ()) {
          // generate information and line number as a comment
          fLilypondCodeStream <<
            " %{ kSyllableMeasureEnd %} ";
        }

        fLilypondCodeStream << std::endl;
      }
      break;

    // ----------------------------------------------------
    case msrSyllableKind::kSyllableLineBreak:
    // ----------------------------------------------------
#ifdef MF_TRACE_IS_ENABLED
          if (gGlobalLpsr2lilypondOahGroup->getCommentLilypondLyrics ()) {
            fLilypondCodeStream <<
              "%{ GENERATE_CODE_FOR_SYLLABLE_kSyllableLineBreak" <<
              ", line " << syllable->getInputLineNumber () <<
              " %}" <<
              std::endl;
          }
#endif // MF_TRACE_IS_ENABLED

      // generate information and line number as a comment if relevant
#ifdef MF_TRACE_IS_ENABLED
      if (gGlobalLpsr2lilypondOahGroup->getCommentLilypondLyrics ()) {
        fLilypondCodeStream <<
          "%{ kSyllableLineBreak, line " <<
          syllable->getInputLineNumber () <<
          " %}" <<
          std::endl;
      }
#endif // MF_TRACE_IS_ENABLED

      fLilypondCodeStream <<
        "\\break" <<
//         syllable->fetchSyllableMeasurePuristNumber () + 1 <<
        std::endl;
      break;

    // ----------------------------------------------------
    case msrSyllableKind::kSyllablePageBreak:
    // ----------------------------------------------------
#ifdef MF_TRACE_IS_ENABLED
          if (gGlobalLpsr2lilypondOahGroup->getCommentLilypondLyrics ()) {
            fLilypondCodeStream <<
              "%{ GENERATE_CODE_FOR_SYLLABLE_kSyllablePageBreak" <<
              ", line " << syllable->getInputLineNumber () <<
              " %}" <<
              std::endl;
          }
#endif // MF_TRACE_IS_ENABLED

      // generate information and line number as a comment
      // generate information and line number as a comment if relevant
#ifdef MF_TRACE_IS_ENABLED
      if (gGlobalLpsr2lilypondOahGroup->getCommentLilypondLyrics ()) {
        fLilypondCodeStream <<
          "%{ kSyllablePageBreak, line " <<
          syllable->getInputLineNumber () <<
          " %}" <<
          std::endl;
      }
#endif // MF_TRACE_IS_ENABLED

      fLilypondCodeStream <<
        "\\pageBreak" <<
        syllable->fetchSyllableMeasurePuristNumber () + 1 <<
        std::endl;
      break;
  } // switch

  generateCodeAfterSyllableIfRelevant (syllable);
}

void lpsr2lilypondTranslator::generateCodeBeforeSyllableIfRelevant (
  const S_msrSyllable& syllable)
{
// #ifdef MF_TRACE_IS_ENABLED
//   if (gGlobalLpsr2lilypondOahGroup->getCommentLilypondLyrics ()) {
//     fLilypondCodeStream <<
//       "%{ GENERATE_CODE_BEFORE_SYLLABLE \"" <<
//       syllable->asShortString () <<
//       "\" %}" <<
//       std::endl;
//   }
// #endif // MF_TRACE_IS_ENABLED
//
//   Bool doGenerateASingleUnderscore (false);
//
//   switch (syllable->getSyllableExtendKind ()) {
//
//     case msrSyllableExtendKind::kSyllableExtend_NONE:
//       switch (gGlobalLpsr2lilypondOahGroup->getLyricsNotesDurationsKind ()) {
//         case lpsrLyricsNotesDurationsKind::kLyricsNotesDurationsAutomatic:
//           break;
//
//         case lpsrLyricsNotesDurationsKind::kLyricsNotesDurationsAutomatic:
//           break;
//
//         case lpsrLyricsNotesDurationsKind::kLyricsNotesDurationsImplicit:
//           // a continue type extension is not mandatory
// //           doGenerateASingleUnderscore = true;
//           break;
//
//         case lpsrLyricsNotesDurationsKind::kLyricsNotesDurationsExplicit:
//           break;
//       } // switch
//       break;
//
//     case msrSyllableExtendKind::kSyllableExtendTypeLess:
//       switch (gGlobalLpsr2lilypondOahGroup->getLyricsNotesDurationsKind ()) {
//         case lpsrLyricsNotesDurationsKind::kLyricsNotesDurationsImplicit:
//           break;
//
//         case lpsrLyricsNotesDurationsKind::kLyricsNotesDurationsExplicit:
// //           generateLyricExtenderAndOrSkipWithExplicitDurations (
// //             syllable);
//           break;
//       } // switch
//       break;
//
//     case msrSyllableExtendKind::kSyllableExtendTypeStart:
//       fOnGoingExtend = true;
//
//       switch (gGlobalLpsr2lilypondOahGroup->getLyricsNotesDurationsKind ()) {
//         case lpsrLyricsNotesDurationsKind::kLyricsNotesDurationsAutomatic:
//           break;
//
//         case lpsrLyricsNotesDurationsKind::kLyricsNotesDurationsImplicit:
//           break;
//
//         case lpsrLyricsNotesDurationsKind::kLyricsNotesDurationsExplicit:
//           break;
//       } // switch
//       break;
//
//     case msrSyllableExtendKind::kSyllableExtendTypeContinue:
//       break;
//
//     case msrSyllableExtendKind::kSyllableExtendTypeStop:
//       fOnGoingExtend = true;
//       break;
//   } // switch
}

void lpsr2lilypondTranslator::generateCodeAfterSyllableIfRelevant (
  const S_msrSyllable& syllable)
{
/*
LilyPond syntax:

  In all three methods hyphens can be drawn between the syllables of a word
  and extender lines can be drawn beyond the end of a word.

  -  same syllable on a note tied to the preceding one
  -- new syllable in same word on a new note

  _  EFEF
  __ extender, new syllable on a note tied to the preceding one
*/

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalLpsr2lilypondOahGroup->getCommentLilypondLyrics ()) {
    fLilypondCodeStream <<
      std::endl <<
      "%{ GENERATE_CODE_AFTER_SYLLABLE \"" <<
      syllable->asShortString () <<
      "\" %}" <<
      std::endl;
  }
#endif // MF_TRACE_IS_ENABLED

  switch (syllable->getSyllableExtendKind ()) {
    case msrSyllableExtendKind::kSyllableExtend_NONE:
      switch (gGlobalLpsr2lilypondOahGroup->getLyricsNotesDurationsKind ()) {
        case lpsrLyricsNotesDurationsKind::kLyricsNotesDurationsAutomatic:
          generateLyricExtenderAndOrSkipWithAutomaticDurations (
            syllable);
          break;

        case lpsrLyricsNotesDurationsKind::kLyricsNotesDurationsImplicit:
          generateLyricExtenderAndOrSkipWithImplicitDurations (
            syllable);
          break;

        case lpsrLyricsNotesDurationsKind::kLyricsNotesDurationsExplicit:
          generateLyricExtenderAndOrSkipWithExplicitDurations (
            syllable);
          break;
      } // switch
      break;

    case msrSyllableExtendKind::kSyllableExtendTypeLess:
      switch (gGlobalLpsr2lilypondOahGroup->getLyricsNotesDurationsKind ()) {
        case lpsrLyricsNotesDurationsKind::kLyricsNotesDurationsAutomatic:
          generateLyricExtenderAndOrSkipWithAutomaticDurations (
            syllable);
          break;

        case lpsrLyricsNotesDurationsKind::kLyricsNotesDurationsImplicit:
          generateLyricExtenderAndOrSkipWithImplicitDurations (
            syllable);
          break;

        case lpsrLyricsNotesDurationsKind::kLyricsNotesDurationsExplicit:
          generateLyricExtenderAndOrSkipWithExplicitDurations (
            syllable);
          break;
      } // switch
      break;

    case msrSyllableExtendKind::kSyllableExtendTypeStart:
      switch (gGlobalLpsr2lilypondOahGroup->getLyricsNotesDurationsKind ()) {
        case lpsrLyricsNotesDurationsKind::kLyricsNotesDurationsAutomatic:
          generateLyricExtenderAndOrSkipWithAutomaticDurations (
            syllable);
          break;

        case lpsrLyricsNotesDurationsKind::kLyricsNotesDurationsImplicit:
          generateLyricExtenderAndOrSkipWithImplicitDurations (
            syllable);
          break;

        case lpsrLyricsNotesDurationsKind::kLyricsNotesDurationsExplicit:
          generateLyricExtenderAndOrSkipWithExplicitDurations (
            syllable);
          break;
      } // switch
      break;

    case msrSyllableExtendKind::kSyllableExtendTypeContinue:
      break;

    case msrSyllableExtendKind::kSyllableExtendTypeStop:
      break;
  } // switch

  // is an extend starting or stopping?
  // a continue type extension is not mandatory inbetween
  Bool doGenerateADoubleUnderscore (false);
  Bool doGenerateASingleUnderscore (false);

  switch (syllable->getSyllableExtendKind ()) {

    case msrSyllableExtendKind::kSyllableExtend_NONE:
      break;

    case msrSyllableExtendKind::kSyllableExtendTypeLess:
//       doGenerateADoubleUnderscore = true;
      break;

    case msrSyllableExtendKind::kSyllableExtendTypeStart:
//       doGenerateADoubleUnderscore = true;

      switch (gGlobalLpsr2lilypondOahGroup->getLyricsNotesDurationsKind ()) {
        case lpsrLyricsNotesDurationsKind::kLyricsNotesDurationsAutomatic:
          break;

        case lpsrLyricsNotesDurationsKind::kLyricsNotesDurationsImplicit:
          break;

        case lpsrLyricsNotesDurationsKind::kLyricsNotesDurationsExplicit:
//           doGenerateASingleUnderscore = true;
          break;
      } // switch

      fOnGoingExtend = true;
      break;

    case msrSyllableExtendKind::kSyllableExtendTypeContinue:
//       doGenerateADoubleUnderscore = true;
//       doGenerateASingleUnderscore = true;
      break;

    case msrSyllableExtendKind::kSyllableExtendTypeStop:
      fOnGoingExtend = false;
      break;
  } // switch


  // generate and extender if relevant
  if (doGenerateADoubleUnderscore) {
    fLilypondCodeStream <<
      "__ " <<
      std::endl;
  }

  if (doGenerateASingleUnderscore) {
    fLilypondCodeStream <<
      "_ " <<
      std::endl;
  }
}

void lpsr2lilypondTranslator::generateLyricExtenderAndOrSkipWithAutomaticDurations (
  const S_msrSyllable& syllable)
{
/*
Multiple notes to one syllable:

Sometimes, particularly in Medieval and baroque music, several notes are sung on one syllable; this is called melisma, see Section “melisma” in Music Glossary. The syllable to a melisma is usually left-aligned with the first note of the melisma.

When a melisma occurs on a syllable *** other than the last one *** in a word, that syllable is usually joined to the following one with a hyphenated line. This is indicated by placing a double hyphen, --, immediately after the syllable.

Alternatively, when a melisma occurs on the *** last or only syllable in a word *** an extender line is usually drawn from the end of the syllable to the last note of the melisma. This is indicated by placing a double underscore, __, immediately after the word.
*/

  // the way lyrics are presented in the LilyPond documentation
  // is not organized acccording to an automatic/implicit/explicit trichotomy,
  // so let's make things clear in the automatic case:
  Bool doGenerateADoubleUnderscore (false);
  Bool doGenerateASingleUnderscore (false);

  Bool doGenerateASingleHyphen (false);
  Bool doGenerateADoubleHyphen (false);

  Bool doGenerateASkip (false);

  // get the note the syllable is attached to
  S_msrNote
    noteTheSyllableIsAttachedTo =
      syllable->getSyllableUpLinkToNote ();

  // what is the syllable kind?
  switch (syllable->getSyllableKind ()) {

    // ----------------------------------------------------
    case msrSyllableKind::kSyllableNone:
    // ----------------------------------------------------
#ifdef MF_TRACE_IS_ENABLED
      if (gGlobalLpsr2lilypondOahGroup->getCommentLilypondLyrics ()) {
        fLilypondCodeStream <<
          std::endl <<
          "%{ AUTOMATIC_DURATIONS_kSyllableNone" <<
          ", line " << syllable->getInputLineNumber () <<
          " %}" <<
          std::endl;
      }
#endif // MF_TRACE_IS_ENABLED
      break;

    // ----------------------------------------------------
    case msrSyllableKind::kSyllableSingle:
    // ----------------------------------------------------
#ifdef MF_TRACE_IS_ENABLED
      if (gGlobalLpsr2lilypondOahGroup->getCommentLilypondLyrics ()) {
        fLilypondCodeStream <<
          std::endl <<
          "%{ AUTOMATIC_DURATIONS_kSyllableSingle" <<
          ", line " << syllable->getInputLineNumber () <<
          " %}" <<
          std::endl;
      }
#endif // MF_TRACE_IS_ENABLED

      if (noteTheSyllableIsAttachedTo) {
        // take note's tie into account if any
        const std::list <S_msrTie>& noteTiesList =
          noteTheSyllableIsAttachedTo->getNoteTiesList ();

        if (! noteTiesList.empty ()) {
          for (S_msrTie noteTie : noteTiesList) {
            switch (noteTie->getTieKind ()) {
              case msrTieKind::kTieNone:
                break;
              case msrTieKind::kTieStart: // PAS CLAIR, VOIR GOAL
                doGenerateASingleHyphen = true;
                break;
              case msrTieKind::kTieContinue:
                doGenerateASingleHyphen = true;
                break;
              case msrTieKind::kTieStop:
                break;
              case msrTieKind::kTieLetRing:
                break;
            } // switch
          } // for
        }
      }
      break;

    // ----------------------------------------------------
    case msrSyllableKind::kSyllableBegin:
    // ----------------------------------------------------
#ifdef MF_TRACE_IS_ENABLED
      if (gGlobalLpsr2lilypondOahGroup->getCommentLilypondLyrics ()) {
        fLilypondCodeStream <<
          std::endl <<
          "%{ AUTOMATIC_DURATIONS_kSyllableBegin" <<
          ", line " << syllable->getInputLineNumber () <<
          " %}" <<
          std::endl;
      }
#endif // MF_TRACE_IS_ENABLED

      doGenerateADoubleHyphen = true;

      if (noteTheSyllableIsAttachedTo) {
        // take note's tie into account if any
        const std::list <S_msrTie>& noteTiesList =
          noteTheSyllableIsAttachedTo->getNoteTiesList ();

        if (! noteTiesList.empty ()) {
          for (S_msrTie noteTie : noteTiesList) {
            switch (noteTie->getTieKind ()) {
              case msrTieKind::kTieNone:
                break;
              case msrTieKind::kTieStart: // PAS CLAIR, VOIR GOAL
                doGenerateASingleHyphen = true;
                break;
              case msrTieKind::kTieContinue:
                doGenerateASingleHyphen = true;
                break;
              case msrTieKind::kTieStop:
                break;
              case msrTieKind::kTieLetRing:
                break;
            } // switch
          } // for
        }
      }
      break;

    // ----------------------------------------------------
    case msrSyllableKind::kSyllableMiddle:
    // ----------------------------------------------------
#ifdef MF_TRACE_IS_ENABLED
      if (gGlobalLpsr2lilypondOahGroup->getCommentLilypondLyrics ()) {
        fLilypondCodeStream <<
          std::endl <<
          "%{ AUTOMATIC_DURATIONS_kSyllableMiddle" <<
          ", line " << syllable->getInputLineNumber () <<
          " %}" <<
          std::endl;
      }
#endif // MF_TRACE_IS_ENABLED

      doGenerateADoubleHyphen = true;

      if (noteTheSyllableIsAttachedTo) {
        // take note's tie into account if any
        const std::list <S_msrTie>& noteTiesList =
          noteTheSyllableIsAttachedTo->getNoteTiesList ();

        if (! noteTiesList.empty ()) {
          for (S_msrTie noteTie : noteTiesList) {
            switch (noteTie->getTieKind ()) {
              case msrTieKind::kTieNone:
                break;
              case msrTieKind::kTieStart: // PAS CLAIR, VOIR GOAL
                doGenerateASingleHyphen = true;
                break;
              case msrTieKind::kTieContinue:
                doGenerateASingleHyphen = true;
                break;
              case msrTieKind::kTieStop:
                break;
              case msrTieKind::kTieLetRing:
                break;
            } // switch
          } // for
        }
      }
      break;

    // ----------------------------------------------------
    case msrSyllableKind::kSyllableEnd:
    // ----------------------------------------------------
#ifdef MF_TRACE_IS_ENABLED
      if (gGlobalLpsr2lilypondOahGroup->getCommentLilypondLyrics ()) {
        fLilypondCodeStream <<
          std::endl <<
          "%{ AUTOMATIC_DURATIONS_kSyllableEnd" <<
          ", line " << syllable->getInputLineNumber () <<
          " %}" <<
          std::endl;
      }
#endif // MF_TRACE_IS_ENABLED

      if (noteTheSyllableIsAttachedTo) {
        // take note's tie into account if any
        const std::list <S_msrTie>& noteTiesList =
          noteTheSyllableIsAttachedTo->getNoteTiesList ();

        if (! noteTiesList.empty ()) {
          for (S_msrTie noteTie : noteTiesList) {
            switch (noteTie->getTieKind ()) {
              case msrTieKind::kTieNone:
                break;
              case msrTieKind::kTieStart: // PAS CLAIR, VOIR GOAL
                doGenerateASingleHyphen = true;
                break;
              case msrTieKind::kTieContinue:
                doGenerateASingleHyphen = true;
                break;
              case msrTieKind::kTieStop:
                break;
              case msrTieKind::kTieLetRing:
                break;
            } // switch
          } // for
        }
      }
      break;

    // ----------------------------------------------------
    case msrSyllableKind::kSyllableOnRestNote:
    // ----------------------------------------------------
#ifdef MF_TRACE_IS_ENABLED
      if (gGlobalLpsr2lilypondOahGroup->getCommentLilypondLyrics ()) {
        fLilypondCodeStream <<
          std::endl <<
          "%{ AUTOMATIC_DURATIONS_kSyllableOnRestNote" <<
          ", line " << syllable->getInputLineNumber () <<
          " %}" <<
          std::endl;
      }
#endif // MF_TRACE_IS_ENABLED
      break;

    // ----------------------------------------------------
    case msrSyllableKind::kSyllableSkipOnRestNote:
    // ----------------------------------------------------
#ifdef MF_TRACE_IS_ENABLED
      if (gGlobalLpsr2lilypondOahGroup->getCommentLilypondLyrics ()) {
        fLilypondCodeStream <<
          std::endl <<
          "%{ AUTOMATIC_DURATIONS_kSyllableSkipOnRestNote" <<
          ", line " << syllable->getInputLineNumber () <<
          " %}" <<
          std::endl;
      }
#endif // MF_TRACE_IS_ENABLED
      break;

    // ----------------------------------------------------
    case msrSyllableKind::kSyllableSkipOnNonRestNote:
    // ----------------------------------------------------
#ifdef MF_TRACE_IS_ENABLED
      if (gGlobalLpsr2lilypondOahGroup->getCommentLilypondLyrics ()) {
        fLilypondCodeStream <<
          std::endl <<
          "%{ AUTOMATIC_DURATIONS_kSyllableSkipOnNonRestNote" <<
          ", line " << syllable->getInputLineNumber () <<
          " %}" <<
          std::endl;
      }
#endif // MF_TRACE_IS_ENABLED

      doGenerateASkip = true;
      break;

    // ----------------------------------------------------
    case msrSyllableKind::kSyllableMeasureEnd:
    // ----------------------------------------------------
      break;

    // ----------------------------------------------------
    case msrSyllableKind::kSyllableLineBreak:
    // ----------------------------------------------------
      break;

    // ----------------------------------------------------
    case msrSyllableKind::kSyllablePageBreak:
    // ----------------------------------------------------
      break;
  } // switch

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalLpsr2lilypondOahGroup->getCommentLilypondLyrics ()) {
    fLilypondCodeStream <<
      "%{ =======>" <<
      std::endl;

    gIndenter++;

    constexpr int fieldWidth = 28;

//     syllable->printForTrace (
//       fLilypondCodeStream,
//       fieldWidth);

    fLilypondCodeStream <<
      syllable <<
      std::endl;

    fLilypondCodeStream << std::left <<
      std::setw (fieldWidth) <<
      "doGenerateASingleUnderscore" << ": " <<
      doGenerateASingleUnderscore <<
      std::endl <<
      std::setw (fieldWidth) <<
      "doGenerateADoubleUnderscore" << ": " <<
      doGenerateADoubleUnderscore <<
      std::endl <<

      std::setw (fieldWidth) <<
      "syllableUpLinkToNote" << ": " <<
      syllable->syllableUpLinkToNoteAsString () <<
      std::endl <<

      std::setw (fieldWidth) <<
      "doGenerateASingleHyphen" << ": " <<
      doGenerateASingleHyphen <<
      std::endl <<
      std::setw (fieldWidth) <<
      "doGenerateADoubleHyphen" << ": " <<
      doGenerateADoubleHyphen <<
      std::endl <<

      std::setw (fieldWidth) <<
      "doGenerateASkip" << ": " <<
      doGenerateASkip <<
      std::endl;

    gIndenter--;

    fLilypondCodeStream <<
      "%}" <<
      std::endl;
  }
#endif // MF_TRACE_IS_ENABLED

  // should a single hyphen be generated?
  // ----------------------------------------------------
  if (doGenerateASingleHyphen) {
    fLilypondCodeStream <<
      "- ";
  }

  // should a double hyphen be generated?
  // ----------------------------------------------------
  if (doGenerateADoubleHyphen) {
    fLilypondCodeStream <<
      "-- ";
  }

  // should a double underscore be generated? (before a single underscore if relevant)
  // ----------------------------------------------------
  if (doGenerateADoubleUnderscore) {
    fLilypondCodeStream <<
      "__ ";
  }

  // should a single underscore be generated?
  // ----------------------------------------------------
  if (doGenerateASingleUnderscore) {
    fLilypondCodeStream <<
      "_ ";
  }

  // should a \skip be generated?
  // ----------------------------------------------------
  if (doGenerateASkip) {
    fLilypondCodeStream <<
      " \\skip" <<
//       durationAsLilypondStringIfItShouldBeGenerated (
//         noteTheSyllableIsAttachedTo->getInputLineNumber (),
//         noteTheSyllableIsAttachedTo->getMeasureElementSoundingWholeNotes ()) <<

      // durationAsLilypondStringIfItShouldBeGenerated is not adequate it seems,
      // so let's generate it the hardwired way... // JMI 0.9.71
      wholeNotesAsLilypondString (
        noteTheSyllableIsAttachedTo->getInputLineNumber (),
        noteTheSyllableIsAttachedTo->getMeasureElementSoundingWholeNotes ()) <<
      cLilyPondSpace;
  }

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalLpsr2lilypondOahGroup->getCommentLilypondLyrics ()) {
    fLilypondCodeStream <<
      std::endl;
  }
#endif // MF_TRACE_IS_ENABLED
}

void lpsr2lilypondTranslator::generateLyricExtenderAndOrSkipWithImplicitDurations (
  const S_msrSyllable& syllable)
{
/*
Multiple notes to one syllable:

Sometimes, particularly in Medieval and baroque music, several notes are sung on one syllable; this is called melisma, see Section “melisma” in Music Glossary. The syllable to a melisma is usually left-aligned with the first note of the melisma.

When a melisma occurs on a syllable *** other than the last one *** in a word, that syllable is usually joined to the following one with a hyphenated line. This is indicated by placing a double hyphen, --, immediately after the syllable.

Alternatively, when a melisma occurs on the *** last or only syllable in a word *** an extender line is usually drawn from the end of the syllable to the last note of the melisma. This is indicated by placing a double underscore, __, immediately after the word.
*/

  // the way lyrics are presented in the LilyPond documentation
  // is not organized acccording to an implicit/explicit dichotomy,
  // so let's make things clear in the implicit case:
  Bool doGenerateADoubleUnderscore (false);
  Bool doGenerateASingleUnderscore (false);

  Bool doGenerateASingleHyphen (false);
  Bool doGenerateADoubleHyphen (false);

  Bool doGenerateASkip (false);

  // get the note the syllable is attached to
  S_msrNote
    noteTheSyllableIsAttachedTo =
      syllable->getSyllableUpLinkToNote ();

  // what is the syllable kind?
  switch (syllable->getSyllableKind ()) {

    // ----------------------------------------------------
    case msrSyllableKind::kSyllableNone:
    // ----------------------------------------------------
#ifdef MF_TRACE_IS_ENABLED
      if (gGlobalLpsr2lilypondOahGroup->getCommentLilypondLyrics ()) {
        fLilypondCodeStream <<
          std::endl <<
          "%{ IMPLICIT_DURATIONS_kSyllableNone" <<
          ", line " << syllable->getInputLineNumber () <<
          " %}" <<
          std::endl;
      }
#endif // MF_TRACE_IS_ENABLED
      break;

    // ----------------------------------------------------
    case msrSyllableKind::kSyllableSingle:
    // ----------------------------------------------------
      {
#ifdef MF_TRACE_IS_ENABLED
      if (gGlobalLpsr2lilypondOahGroup->getCommentLilypondLyrics ()) {
        fLilypondCodeStream <<
          std::endl <<
          "%{ IMPLICIT_DURATIONS_kSyllableSingle" <<
          ", line " << syllable->getInputLineNumber () <<
          " %}" <<
          std::endl;
      }
#endif // MF_TRACE_IS_ENABLED

      if (noteTheSyllableIsAttachedTo) {
        // take note's tie into account if any
        const std::list <S_msrTie>& noteTiesList =
          noteTheSyllableIsAttachedTo->getNoteTiesList ();

        if (! noteTiesList.empty ()) {
          for (S_msrTie noteTie : noteTiesList) {
            switch (noteTie->getTieKind ()) {
              case msrTieKind::kTieNone:
                break;
              case msrTieKind::kTieStart: // PAS CLAIR, VOIR GOAL
                doGenerateASingleHyphen = true;
                break;
              case msrTieKind::kTieContinue:
                doGenerateASingleHyphen = true;
                break;
              case msrTieKind::kTieStop:
                break;
              case msrTieKind::kTieLetRing:
                break;
            } // switch
          } // for
        }
      }

      // what it the syllable's extend kind?
      switch (syllable->getSyllableExtendKind ()) {
        case msrSyllableExtendKind::kSyllableExtend_NONE:
          // a continue type extension is not mandatory
          if (fOnGoingExtend) {
//             doGenerateASingleUnderscore = true;
          }
          break;

        case msrSyllableExtendKind::kSyllableExtendTypeLess:
          doGenerateADoubleUnderscore = true;
          break;

        case msrSyllableExtendKind::kSyllableExtendTypeStart:
          fOnGoingExtend = true;
          doGenerateADoubleUnderscore = true;
          break;

        case msrSyllableExtendKind::kSyllableExtendTypeContinue:
          doGenerateADoubleUnderscore = true;
          break;

        case msrSyllableExtendKind::kSyllableExtendTypeStop:
//           if (fOnGoingExtend) { // JMI 0.9.70
//             doGenerateASingleHyphen = true;
//           }

          fOnGoingExtend = false;
          break;
      } // switch
      }
      break;

    // ----------------------------------------------------
    case msrSyllableKind::kSyllableBegin:
    // ----------------------------------------------------
#ifdef MF_TRACE_IS_ENABLED
      if (gGlobalLpsr2lilypondOahGroup->getCommentLilypondLyrics ()) {
        fLilypondCodeStream <<
          std::endl <<
          "%{ IMPLICIT_DURATIONS_kSyllableBegin" <<
          ", line " << syllable->getInputLineNumber () <<
          " %}" <<
          std::endl;
      }
#endif // MF_TRACE_IS_ENABLED

      doGenerateADoubleHyphen = true;

      {
        if (noteTheSyllableIsAttachedTo) {
          // take note's tie into account if any
          const std::list <S_msrTie>& noteTiesList =
            noteTheSyllableIsAttachedTo->getNoteTiesList ();

          if (! noteTiesList.empty ()) {
            for (S_msrTie noteTie : noteTiesList) {
              switch (noteTie->getTieKind ()) {
                case msrTieKind::kTieNone:
                  break;
                case msrTieKind::kTieStart:
                doGenerateASingleHyphen = true;
                  break;
                case msrTieKind::kTieContinue:
                doGenerateASingleHyphen = true;
                  break;
                case msrTieKind::kTieStop:
                  break;
                case msrTieKind::kTieLetRing:
                  break;
              } // switch
            } // for
          }
        }
      }

        {
          // get the note slurs
          const std::list <S_msrSlur>&
            noteSlursList =
              noteTheSyllableIsAttachedTo->getNoteSlursList ();

          // take note's tie into account if any
          if (! noteSlursList.empty ()) {
//             doGenerateASkip = true;
          }
        }

      // what it the syllable's extend kind?
      switch (syllable->getSyllableExtendKind ()) {
        case msrSyllableExtendKind::kSyllableExtend_NONE:
          break;

        case msrSyllableExtendKind::kSyllableExtendTypeLess:
          doGenerateASkip = true;
          break;

        case msrSyllableExtendKind::kSyllableExtendTypeStart:
          fOnGoingExtend = true;
          doGenerateADoubleUnderscore = true;
          break;

        case msrSyllableExtendKind::kSyllableExtendTypeContinue:
          doGenerateADoubleUnderscore = true;
          break;

        case msrSyllableExtendKind::kSyllableExtendTypeStop:
          fOnGoingExtend = false;
          break;
      } // switch
      break;

    // ----------------------------------------------------
    case msrSyllableKind::kSyllableMiddle:
    // ----------------------------------------------------
#ifdef MF_TRACE_IS_ENABLED
      if (gGlobalLpsr2lilypondOahGroup->getCommentLilypondLyrics ()) {
        fLilypondCodeStream <<
          std::endl <<
          "%{ IMPLICIT_DURATIONS_kSyllableMiddle" <<
          ", line " << syllable->getInputLineNumber () <<
          " %}" <<
          std::endl;
      }
#endif // MF_TRACE_IS_ENABLED

      doGenerateADoubleHyphen = true;

      {
        // take note's tie into account if any
        const std::list <S_msrTie>&
          noteTiesList =
            noteTheSyllableIsAttachedTo->getNoteTiesList ();

        if (! noteTiesList.empty ()) {
          for (S_msrTie noteTie : noteTiesList) {
            if (noteTie->getTieKind () == msrTieKind::kTieStart) {
              fLilypondCodeStream << " ~ ";
              if (gGlobalLpsr2lilypondOahGroup->getCommentLilypondLyrics ()) {
                fLilypondCodeStream <<
                   " %{ msrNoteKind::kNoteRegularInMeasure %} "; // JMI 0.9.72 ZOZO
              }
            }
          } // for
        }

//         S_msrTie
//           noteTie =
//             noteTheSyllableIsAttachedTo->getNoteTie ();
//
//         if (noteTie) {
//           switch (noteTie->getTieKind ()) {
//             case msrTieKind::kTieNone:
//               break;
//             case msrTieKind::kTieStart:
//                doGenerateASingleHyphen = true;
//               break;
//             case msrTieKind::kTieContinue:
//               break;
//             case msrTieKind::kTieStop:
//               break;
//             case msrTieKind::kTieLetRing:
//               break;
//           } // switch
//         }
      }

      {
        // get the note slurs
        const std::list <S_msrSlur>&
          noteSlursList =
            noteTheSyllableIsAttachedTo->getNoteSlursList ();

        // take note's tie into account if any
        if (! noteSlursList.empty ()) {
//             doGenerateASkip = true;
        }
      }

      // what it the syllable's extend kind?
      switch (syllable->getSyllableExtendKind ()) {
        case msrSyllableExtendKind::kSyllableExtend_NONE:
          break;

        case msrSyllableExtendKind::kSyllableExtendTypeLess:
          doGenerateASkip = true;
          break;

        case msrSyllableExtendKind::kSyllableExtendTypeStart:
          doGenerateADoubleUnderscore = true;
          break;

        case msrSyllableExtendKind::kSyllableExtendTypeContinue:
          doGenerateADoubleUnderscore = true;
          break;

        case msrSyllableExtendKind::kSyllableExtendTypeStop:
          break;
      } // switch
      break;

    // ----------------------------------------------------
    case msrSyllableKind::kSyllableEnd:
    // ----------------------------------------------------
#ifdef MF_TRACE_IS_ENABLED
      if (gGlobalLpsr2lilypondOahGroup->getCommentLilypondLyrics ()) {
        fLilypondCodeStream <<
          std::endl <<
          "%{ IMPLICIT_DURATIONS_kSyllableEnd" <<
          ", line " << syllable->getInputLineNumber () <<
          " %}" <<
          std::endl;
      }
#endif // MF_TRACE_IS_ENABLED

      // what it the syllable's extend kind?
      switch (syllable->getSyllableExtendKind ()) {
        case msrSyllableExtendKind::kSyllableExtend_NONE:
          break;

        case msrSyllableExtendKind::kSyllableExtendTypeLess:
          doGenerateADoubleUnderscore = true;
          doGenerateASkip = true;
          break;

        case msrSyllableExtendKind::kSyllableExtendTypeStart:
          doGenerateASingleUnderscore = true;
          break;

        case msrSyllableExtendKind::kSyllableExtendTypeContinue:
          doGenerateASingleUnderscore = true;
          break;

        case msrSyllableExtendKind::kSyllableExtendTypeStop:
          break;
      } // switch
      break;

    // ----------------------------------------------------
    case msrSyllableKind::kSyllableOnRestNote:
    // ----------------------------------------------------
#ifdef MF_TRACE_IS_ENABLED
      if (gGlobalLpsr2lilypondOahGroup->getCommentLilypondLyrics ()) {
        fLilypondCodeStream <<
          std::endl <<
          "%{ IMPLICIT_DURATIONS_kSyllableOnRestNote" <<
          ", line " << syllable->getInputLineNumber () <<
          " %}" <<
          std::endl;
      }
#endif // MF_TRACE_IS_ENABLED
      break;

    // ----------------------------------------------------
    case msrSyllableKind::kSyllableSkipOnRestNote:
    // ----------------------------------------------------
#ifdef MF_TRACE_IS_ENABLED
      if (gGlobalLpsr2lilypondOahGroup->getCommentLilypondLyrics ()) {
        fLilypondCodeStream <<
          std::endl <<
          "%{ IMPLICIT_DURATIONS_kSyllableSkipOnRestNote" <<
          ", line " << syllable->getInputLineNumber () <<
          " %}" <<
          std::endl;
      }
#endif // MF_TRACE_IS_ENABLED
      break;

    // ----------------------------------------------------
    case msrSyllableKind::kSyllableSkipOnNonRestNote:
    // ----------------------------------------------------
#ifdef MF_TRACE_IS_ENABLED
      if (gGlobalLpsr2lilypondOahGroup->getCommentLilypondLyrics ()) {
        fLilypondCodeStream <<
          std::endl <<
          "%{ IMPLICIT_DURATIONS_kSyllableSkipOnNonRestNote" <<
          ", line " << syllable->getInputLineNumber () <<
          " %}" <<
          std::endl;
      }
#endif // MF_TRACE_IS_ENABLED

      doGenerateASkip = true;
      break;

    // ----------------------------------------------------
    case msrSyllableKind::kSyllableMeasureEnd:
    // ----------------------------------------------------
      // forget about the last whole notes,
      // to enforce a duration being generated
      // for the first syllable in the next measure
      fLastMetWholeNotes = K_WHOLE_NOTES_UNKNOWN_; // JMI 0.9.70
      break;

    // ----------------------------------------------------
    case msrSyllableKind::kSyllableLineBreak:
    // ----------------------------------------------------
      break;

    // ----------------------------------------------------
    case msrSyllableKind::kSyllablePageBreak:
    // ----------------------------------------------------
      break;
  } // switch

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalLpsr2lilypondOahGroup->getCommentLilypondLyrics ()) {
    fLilypondCodeStream <<
      "%{ =======>" <<
      std::endl;

    gIndenter++;

    constexpr int fieldWidth = 28;

    fLilypondCodeStream << std::left <<
      std::setw (fieldWidth) <<
      "doGenerateASingleUnderscore" << ": " <<
      doGenerateASingleUnderscore <<
      std::endl <<
      std::setw (fieldWidth) <<
      "doGenerateADoubleUnderscore" << ": " <<
      doGenerateADoubleUnderscore <<
      std::endl <<

      std::setw (fieldWidth) <<
      "doGenerateASingleHyphen" << ": " <<
      doGenerateASingleHyphen <<
      std::endl <<
      std::setw (fieldWidth) <<
      "doGenerateADoubleHyphen" << ": " <<
      doGenerateADoubleHyphen <<
      std::endl <<

      std::setw (fieldWidth) <<
      "doGenerateASkip" << ": " <<
      doGenerateASkip <<
      std::endl;

    gIndenter--;

    fLilypondCodeStream <<
      "%}" <<
      std::endl;
  }
#endif // MF_TRACE_IS_ENABLED

  // should a single hyphen be generated?
  // ----------------------------------------------------
  if (doGenerateASingleHyphen) {
    fLilypondCodeStream <<
      "- ";
  }

  // should a double hyphen be generated?
  // ----------------------------------------------------
  if (doGenerateADoubleHyphen) {
    fLilypondCodeStream <<
      "-- ";
  }

  // should a double underscore be generated? (before a single underscore if relevant)
  // ----------------------------------------------------
  if (doGenerateADoubleUnderscore) {
    fLilypondCodeStream <<
      "__ ";
  }

  // should a single underscore be generated?
  // ----------------------------------------------------
  if (doGenerateASingleUnderscore) {
    fLilypondCodeStream <<
      "_ ";
  }

  // should a \skip be generated?
  // ----------------------------------------------------
  if (doGenerateASkip) {
    fLilypondCodeStream <<
      " \\skip" <<
//       durationAsLilypondStringIfItShouldBeGenerated (
//         noteTheSyllableIsAttachedTo->getInputLineNumber (),
//         noteTheSyllableIsAttachedTo->getMeasureElementSoundingWholeNotes ()) <<

      // durationAsLilypondStringIfItShouldBeGenerated is not adequate it seems,
      // so let's generate it the hardwired way... // JMI 0.9.71
      wholeNotesAsLilypondString (
        noteTheSyllableIsAttachedTo->getInputLineNumber (),
        noteTheSyllableIsAttachedTo->getMeasureElementSoundingWholeNotes ()) <<
      cLilyPondSpace;
  }

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalLpsr2lilypondOahGroup->getCommentLilypondLyrics ()) {
    fLilypondCodeStream <<
      std::endl;
  }
#endif // MF_TRACE_IS_ENABLED
}

void lpsr2lilypondTranslator::generateLyricExtenderAndOrSkipWithExplicitDurations (
  const S_msrSyllable& syllable)
{
/*
Multiple notes to one syllable:

Sometimes, particularly in Medieval and baroque music, several notes are sung on one syllable; this is called melisma, see Section “melisma” in Music Glossary. The syllable to a melisma is usually left-aligned with the first note of the melisma.

When a melisma occurs on a syllable *** other than the last one *** in a word, that syllable is usually joined to the following one with a hyphenated line. This is indicated by placing a double hyphen, --, immediately after the syllable.

Alternatively, when a melisma occurs on the *** last or only syllable in a word *** an extender line is usually drawn from the end of the syllable to the last note of the melisma. This is indicated by placing a double underscore, __, immediately after the word.
*/

  // the way lyrics are presented in the LilyPond documentation
  // is not organized acccording to an implicit/explicit dichotomy,
  // so let's make things clear in the explicit case:
  Bool doGenerateASingleUnderscore (false);
  Bool doGenerateADoubleUnderscore (false);

  Bool doGenerateASingleHyphen (false);
  Bool doGenerateADoubleHyphen (false);

  Bool doGenerateASkip (false);

  // get the note the syllable is attached to
  S_msrNote
    noteTheSyllableIsAttachedTo =
      syllable->getSyllableUpLinkToNote ();

  // what is the syllable kind?
  switch (syllable->getSyllableKind ()) {

    // ----------------------------------------------------
    case msrSyllableKind::kSyllableNone:
    // ----------------------------------------------------
#ifdef MF_TRACE_IS_ENABLED
      if (gGlobalLpsr2lilypondOahGroup->getCommentLilypondLyrics ()) {
        fLilypondCodeStream <<
          std::endl <<
          "%{ EXPLICIT_DURATIONS_kSyllableNone" <<
          ", line " << syllable->getInputLineNumber () <<
          " %}" <<
          std::endl;
      }
#endif // MF_TRACE_IS_ENABLED
      break;

    // ----------------------------------------------------
    case msrSyllableKind::kSyllableSingle:
    // ----------------------------------------------------
#ifdef MF_TRACE_IS_ENABLED
      if (gGlobalLpsr2lilypondOahGroup->getCommentLilypondLyrics ()) {
        fLilypondCodeStream <<
          std::endl <<
          "%{ EXPLICIT_DURATIONS_kSyllableSingle" <<
          ", line " << syllable->getInputLineNumber () <<
          " %}" <<
          std::endl;
      }
#endif // MF_TRACE_IS_ENABLED

      if (noteTheSyllableIsAttachedTo) {
        // take note's tie into account if any
        const std::list <S_msrTie>& noteTiesList =
          noteTheSyllableIsAttachedTo->getNoteTiesList ();

        if (! noteTiesList.empty ()) {
          for (S_msrTie noteTie : noteTiesList) {
            switch (noteTie->getTieKind ()) {
              case msrTieKind::kTieNone:
                break;
              case msrTieKind::kTieStart: // PAS CLAIR, VOIR GOAL
                doGenerateASingleHyphen = true;
                break;
              case msrTieKind::kTieContinue:
                doGenerateASingleHyphen = true;
                break;
              case msrTieKind::kTieStop:
                break;
              case msrTieKind::kTieLetRing:
                break;
            } // switch
          } // for
        }
      }

//       // what it the syllable's extend kind?
//       switch (syllable->getSyllableExtendKind ()) {
//         case msrSyllableExtendKind::kSyllableExtend_NONE:
//           break;
//
//         case msrSyllableExtendKind::kSyllableExtendTypeLess:
//           doGenerateADoubleUnderscore = true;
//           break;
//
//         case msrSyllableExtendKind::kSyllableExtendTypeStart:
//           fOnGoingExtend = true;
//           doGenerateADoubleUnderscore = true;
//           break;
//
//         case msrSyllableExtendKind::kSyllableExtendTypeContinue:
//           doGenerateADoubleUnderscore = true;
//           break;
//
//         case msrSyllableExtendKind::kSyllableExtendTypeStop:
//           fOnGoingExtend = false;
//           break;
//       } // switch
      break;

    // ----------------------------------------------------
    case msrSyllableKind::kSyllableBegin:
    // ----------------------------------------------------
#ifdef MF_TRACE_IS_ENABLED
      if (gGlobalLpsr2lilypondOahGroup->getCommentLilypondLyrics ()) {
        fLilypondCodeStream <<
          std::endl <<
          "%{ EXPLICIT_DURATIONS_kSyllableBegin" <<
          ", line " << syllable->getInputLineNumber () <<
          " %}" <<
          std::endl;
      }
#endif // MF_TRACE_IS_ENABLED

        // doGenerateADoubleHyphen = true;

        {
          if (noteTheSyllableIsAttachedTo) {
            // take note's tie into account if any
            const std::list <S_msrTie>& noteTiesList =
              noteTheSyllableIsAttachedTo->getNoteTiesList ();

            if (! noteTiesList.empty ()) {
              for (S_msrTie noteTie : noteTiesList) {
                switch (noteTie->getTieKind ()) {
                  case msrTieKind::kTieNone:
                    break;
                  case msrTieKind::kTieStart:
                    doGenerateASingleHyphen = true;
                    break;
                  case msrTieKind::kTieContinue:
                    doGenerateASingleHyphen = true;
                    break;
                  case msrTieKind::kTieStop:
                    break;
                  case msrTieKind::kTieLetRing:
                    break;
                } // switch
              } // for
            }
          }
        }

//         {
//           // get the note slurs
//           const std::list <S_msrSlur>&
//             noteSlursList =
//               noteTheSyllableIsAttachedTo->getNoteSlursList ();
//
//           // take note's tie into account if any
//           if (! noteSlursList.empty ()) {
// //             doGenerateASkip = true;
//           }
//         }

      // what it the syllable's extend kind?
      switch (syllable->getSyllableExtendKind ()) {
        case msrSyllableExtendKind::kSyllableExtend_NONE:
          break;

        case msrSyllableExtendKind::kSyllableExtendTypeLess:
          doGenerateADoubleHyphen = true;
          break;

        case msrSyllableExtendKind::kSyllableExtendTypeStart:
          fOnGoingExtend = true;
//           doGenerateASingleUnderscore = true;
          break;

        case msrSyllableExtendKind::kSyllableExtendTypeContinue:
//           doGenerateASingleUnderscore = true;
          break;

        case msrSyllableExtendKind::kSyllableExtendTypeStop:
          fOnGoingExtend = false;
          break;
      } // switch
      break;

    // ----------------------------------------------------
    case msrSyllableKind::kSyllableMiddle:
    // ----------------------------------------------------
      {
#ifdef MF_TRACE_IS_ENABLED
        if (gGlobalLpsr2lilypondOahGroup->getCommentLilypondLyrics ()) {
          fLilypondCodeStream <<
            std::endl <<
            "%{ EXPLICIT_DURATIONS_kSyllableMiddle" <<
            ", line " << syllable->getInputLineNumber () <<
            " %}" <<
            std::endl;
        }
#endif // MF_TRACE_IS_ENABLED

//        doGenerateASingleHyphen = true;

        const std::list <S_msrTie>& noteTiesList =
          noteTheSyllableIsAttachedTo->getNoteTiesList ();

        if (! noteTiesList.empty ()) {
          for (S_msrTie noteTie : noteTiesList) {
            switch (noteTie->getTieKind ()) {
              case msrTieKind::kTieNone:
                break;
              case msrTieKind::kTieStart:
                doGenerateASingleHyphen = true;
                break;
              case msrTieKind::kTieContinue:
                doGenerateASingleHyphen = true;
                break;
              case msrTieKind::kTieStop:
                break;
              case msrTieKind::kTieLetRing:
                break;
             } // switch
          } // for
        }

//         {
//           // get the note slurs
//           const std::list <S_msrSlur>&
//             noteSlursList =
//               noteTheSyllableIsAttachedTo->getNoteSlursList ();
//
//           // take note's tie into account if any
//           if (! noteSlursList.empty ()) {
//             doGenerateASingleHyphen = true;
//           }
//         }

        // what it the syllable's extend kind?
        switch (syllable->getSyllableExtendKind ()) {
          case msrSyllableExtendKind::kSyllableExtend_NONE:
            break;

          case msrSyllableExtendKind::kSyllableExtendTypeLess:
            {
  //             // get the note slurs
  //             const std::list <S_msrSlur>&
  //               noteSlursList =
  //                 noteTheSyllableIsAttachedTo->getNoteSlursList ();
  //
  //             // take note's tie into account if any
  //             if (! noteSlursList.empty ()) {
  //               doGenerateADoubleHyphen = true;
  //             }
            }

            doGenerateASingleHyphen = true;
            break;

          case msrSyllableExtendKind::kSyllableExtendTypeStart:
  //           doGenerateADoubleUnderscore = true;
            break;

          case msrSyllableExtendKind::kSyllableExtendTypeContinue:
  //           doGenerateADoubleUnderscore = true;
            break;

          case msrSyllableExtendKind::kSyllableExtendTypeStop:
            break;
        } // switch
      }
      break;

    // ----------------------------------------------------
    case msrSyllableKind::kSyllableEnd:
    // ----------------------------------------------------
#ifdef MF_TRACE_IS_ENABLED
      if (gGlobalLpsr2lilypondOahGroup->getCommentLilypondLyrics ()) {
        fLilypondCodeStream <<
          std::endl <<
          "%{ EXPLICIT_DURATIONS_kSyllableEnd" <<
          ", line " << syllable->getInputLineNumber () <<
          " %}" <<
          std::endl;
      }
#endif // MF_TRACE_IS_ENABLED

//       // what it the syllable's extend kind?
//       switch (syllable->getSyllableExtendKind ()) {
//         case msrSyllableExtendKind::kSyllableExtend_NONE:
//           break;
//
//         case msrSyllableExtendKind::kSyllableExtendTypeLess:
// //           doGenerateADoubleUnderscore = true;
//           break;
//
//         case msrSyllableExtendKind::kSyllableExtendTypeStart:
//           doGenerateASingleUnderscore = true;
//           break;
//
//         case msrSyllableExtendKind::kSyllableExtendTypeContinue:
//           doGenerateASingleUnderscore = true;
//           break;
//
//         case msrSyllableExtendKind::kSyllableExtendTypeStop:
//           break;
//       } // switch
      break;

    // ----------------------------------------------------
    case msrSyllableKind::kSyllableOnRestNote:
    // ----------------------------------------------------
#ifdef MF_TRACE_IS_ENABLED
      if (gGlobalLpsr2lilypondOahGroup->getCommentLilypondLyrics ()) {
        fLilypondCodeStream <<
          std::endl <<
          "%{ EXPLICIT_DURATIONS_kSyllableOnRestNote" <<
          ", line " << syllable->getInputLineNumber () <<
          " %}" <<
          std::endl;
      }
#endif // MF_TRACE_IS_ENABLED
      break;

    // ----------------------------------------------------
    case msrSyllableKind::kSyllableSkipOnRestNote:
    // ----------------------------------------------------
#ifdef MF_TRACE_IS_ENABLED
      if (gGlobalLpsr2lilypondOahGroup->getCommentLilypondLyrics ()) {
        fLilypondCodeStream <<
          std::endl <<
          "%{ EXPLICIT_DURATIONS_kSyllableSkipOnRestNote" <<
          ", line " << syllable->getInputLineNumber () <<
          " %}" <<
          std::endl;
      }
#endif // MF_TRACE_IS_ENABLED

//         doGenerateASkip = true;
      break;

    // ----------------------------------------------------
    case msrSyllableKind::kSyllableSkipOnNonRestNote:
    // ----------------------------------------------------
#ifdef MF_TRACE_IS_ENABLED
      if (gGlobalLpsr2lilypondOahGroup->getCommentLilypondLyrics ()) {
        fLilypondCodeStream <<
          std::endl <<
          "%{ EXPLICIT_DURATIONS_kSyllableSkipOnNonRestNote" <<
          ", line " << syllable->getInputLineNumber () <<
          " %}" <<
          std::endl;
      }
#endif // MF_TRACE_IS_ENABLED

      doGenerateASkip = true;
      break;

    // ----------------------------------------------------
    case msrSyllableKind::kSyllableMeasureEnd:
    // ----------------------------------------------------
      // forget about the last whole notes,
      // to enforce a duration being generated
      // for the first syllable in the next measure
      fLastMetWholeNotes = K_WHOLE_NOTES_UNKNOWN_; // JMI 0.9.70
      break;

    // ----------------------------------------------------
    case msrSyllableKind::kSyllableLineBreak:
    // ----------------------------------------------------
      break;

    // ----------------------------------------------------
    case msrSyllableKind::kSyllablePageBreak:
    // ----------------------------------------------------
      break;
  } // switch

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalLpsr2lilypondOahGroup->getCommentLilypondLyrics ()) {
    fLilypondCodeStream <<
      "%{ =======>" <<
      std::endl;

    gIndenter++;

    constexpr int fieldWidth = 28;

    fLilypondCodeStream << std::left <<
      std::setw (fieldWidth) <<
      "doGenerateASingleUnderscore" << ": " <<
      doGenerateASingleUnderscore <<
      std::endl <<
      std::setw (fieldWidth) <<
      "doGenerateADoubleUnderscore" << ": " <<
      doGenerateADoubleUnderscore <<
      std::endl <<

      std::setw (fieldWidth) <<
      "doGenerateASingleHyphen" << ": " <<
      doGenerateASingleHyphen <<
      std::endl <<
      std::setw (fieldWidth) <<
      "doGenerateADoubleHyphen" << ": "  <<
      doGenerateADoubleHyphen <<
      std::endl <<

      std::setw (fieldWidth) <<
      "doGenerateASkip: " << ": "  <<
      doGenerateASkip <<
      std::endl;

    gIndenter--;

    fLilypondCodeStream <<
      "%}" <<
      std::endl;
  }
#endif // MF_TRACE_IS_ENABLED

  // should a single hyphen be generated?
  // ----------------------------------------------------
  if (doGenerateASingleHyphen) {
    fLilypondCodeStream <<
      "- ";
  }

  // should a double hyphen be generated?
  // ----------------------------------------------------
  if (doGenerateADoubleHyphen) {
    fLilypondCodeStream <<
      "-- ";
  }

  // should a single underscore be generated?
  // ----------------------------------------------------
  if (doGenerateASingleUnderscore) {
    fLilypondCodeStream <<
      "_ " <<
      std::endl;
  }

  // should a double underscore be generated?
  // ----------------------------------------------------
  if (doGenerateADoubleUnderscore) {
    fLilypondCodeStream <<
      "__ ";
  }

  // should a skip with duratoin be generated?
  // ----------------------------------------------------
  if (doGenerateASkip) {
    fLilypondCodeStream <<
      " \\skip" <<
      durationAsLilypondStringIfItShouldBeGenerated (
        noteTheSyllableIsAttachedTo->getInputLineNumber (),
        noteTheSyllableIsAttachedTo->getMeasureElementSoundingWholeNotes ()) <<
      cLilyPondSpace;
  }

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalLpsr2lilypondOahGroup->getCommentLilypondLyrics ()) {
    fLilypondCodeStream <<
      std::endl;
  }
#endif // MF_TRACE_IS_ENABLED
}

void lpsr2lilypondTranslator::visitEnd (S_msrSyllable& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream ss;

      ss <<
        "% --> End visiting msrSyllable '" <<
        elt->asString () <<
        '\'' <<
        ", line " << elt->getInputLineNumber () <<
      std::endl;

      if (traceLpsrVisitors) {
        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << ss.str ();
      }
    }
  }
#endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
void lpsr2lilypondTranslator::visitStart (S_msrClefKeyTimeSignatureGroup& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream ss;

      ss <<
        "% --> Start visiting msrClefKeyTimeSignatureGroup '" <<
        elt->asString () <<
        '\'' <<
        ", line " << elt->getInputLineNumber () <<
      std::endl;

      if (traceLpsrVisitors) {
        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }

      if (generateMsrVisitingInformation) {
//         fLilypondCodeStream << ss.str ();
      }
    }
  }
#endif // MF_TRACE_IS_ENABLED
}

void lpsr2lilypondTranslator::visitEnd (S_msrClefKeyTimeSignatureGroup& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream ss;

      ss <<
        "% --> End visiting msrClefKeyTimeSignatureGroup '" <<
        elt->asString () <<
        '\'' <<
        ", line " << elt->getInputLineNumber () <<
      std::endl;

      if (traceLpsrVisitors) {
        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }

      if (generateMsrVisitingInformation) {
//         fLilypondCodeStream << ss.str ();
      }
    }
  }
#endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
void lpsr2lilypondTranslator::visitStart (S_msrClef& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream ss;

      ss <<
        "% --> Start visiting msrClef '" <<
        elt->asString () <<
        '\'' <<
        ", line " << elt->getInputLineNumber () <<
      std::endl;

      if (traceLpsrVisitors) {
        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << ss.str ();
      }
    }
  }
#endif // MF_TRACE_IS_ENABLED

  // get the clef kind
  msrClefKind
    clefKind =
      elt->getClefKind ();

  Bool doGenerateClef (true);

  if (! fCurrentVoiceClef) {
    // this is the first clef in this voice
    if (
      clefKind == msrClefKind::kClefTreble
        &&
      gGlobalLpsr2lilypondOahGroup->getNoInitialTrebleClef ()
    ) {
      doGenerateClef = false;
    }
  }

  if (doGenerateClef) {
    fLilypondCodeStream << std::endl;

    if (gGlobalLpsr2lilypondOahGroup->getCommentClefChanges ()) {
    /* JMI
      S_msrClef
        currentVoiceCurrentClef =
          fCurrentVoice->getVoiceCurrentClef ();
  */

      if (
        fCurrentVoiceClef
          &&
        fCurrentVoiceClef->getClefKind () != clefKind
      ) {
        // this is a clef change, comment it

  #ifdef MF_TRACE_IS_ENABLED
        if (gTraceOahGroup->getTraceClefs ()) {
          std::stringstream ss;

          ss <<
            "Commenting clef change from " <<
            fCurrentVoiceClef->asShortString () <<
            " to " <<
            elt->asShortString ();

          gWaeHandler->waeTrace (
            __FILE__, mfInputLineNumber (__LINE__),
            ss.str ());
        }
  #endif // MF_TRACE_IS_ENABLED

        fLilypondCodeStream << "% ";
      }
    }

    if (clefKind != msrClefKind::kClef_UNKNOWN_) {
      fLilypondCodeStream <<
        "\\clef ";

      switch (clefKind) {
        case msrClefKind::kClef_UNKNOWN_:
          break;
        case msrClefKind::kClefTreble:
          fLilypondCodeStream << "\"treble\"";
          break;
        case msrClefKind::kClefSoprano:
          fLilypondCodeStream << "\"soprano\"";
          break;
        case msrClefKind::kClefMezzoSoprano:
          fLilypondCodeStream << "\"mezzosoprano\"";
          break;
        case msrClefKind::kClefAlto:
          fLilypondCodeStream << "\"alto\"";
          break;
        case msrClefKind::kClefTenor:
          fLilypondCodeStream << "\"tenor\"";
          break;
        case msrClefKind::kClefBaritone:
          fLilypondCodeStream << "\"baritone\"";
          break;
        case msrClefKind::kClefBass:
          fLilypondCodeStream << "\"bass\"";
          break;
        case msrClefKind::kClefTrebleLine1:
          fLilypondCodeStream << "\"french\"";
          break;
        case msrClefKind::kClefTrebleMinus15:
          fLilypondCodeStream << "\"treble_15\"";
          break;
        case msrClefKind::kClefTrebleMinus8:
          fLilypondCodeStream << "\"treble_8\"";
          break;
        case msrClefKind::kClefTreblePlus8:
          fLilypondCodeStream << "\"treble^8\"";
          break;
        case msrClefKind::kClefTreblePlus15:
          fLilypondCodeStream << "\"treble^15\"";
          break;
        case msrClefKind::kClefBassMinus15:
          fLilypondCodeStream << "\"bass_15\"";
          break;
        case msrClefKind::kClefBassMinus8:
          fLilypondCodeStream << "\"bass_8\"";
          break;
        case msrClefKind::kClefBassPlus8:
          fLilypondCodeStream << "\"bass^8\"";
          break;
        case msrClefKind::kClefBassPlus15:
          fLilypondCodeStream << "\"bass^15\"";
          break;
        case msrClefKind::kClefVarbaritone:
          fLilypondCodeStream << "\"varbaritone\"";
          break;

        case msrClefKind::kClefTablature4:
        case msrClefKind::kClefTablature5:
        case msrClefKind::kClefTablature6:
        case msrClefKind::kClefTablature7:
          if (gGlobalLpsr2lilypondOahGroup->getModernTab ()) {
            fLilypondCodeStream <<
              "\"moderntab\"" <<
              std::endl;
          }
          else {
            fLilypondCodeStream <<
              "\"tab\"" <<
              std::endl;
          }
          if (gGlobalLpsr2lilypondOahGroup->getTabFullNotation ()) {
            fLilypondCodeStream <<
              "\\tabFullNotation" <<
              std::endl;
          }
          break;

        case msrClefKind::kClefPercussion:
          fLilypondCodeStream << "\"percussion\"";
          break;
        case msrClefKind::kClefJianpu:
          fLilypondCodeStream << "\"%{ jianpuClef??? %} \"";
          break;
      } // switch

      if (gGlobalLpsr2lilypondOahGroup->getInputLineNumbers ()) {
        fLilypondCodeStream <<
          " %{ <-- line " <<
          elt->getInputLineNumber () <<
          " %} ";
      }

      fLilypondCodeStream << std::endl;
    }
  }

  // register current voice clef
  fCurrentVoiceClef = elt;
}

void lpsr2lilypondTranslator::visitEnd (S_msrClef& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream ss;

      ss <<
        "% --> End visiting msrClef '" <<
        elt->asString () <<
        '\'' <<
        ", line " << elt->getInputLineNumber () <<
      std::endl;

      if (traceLpsrVisitors) {
        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << ss.str ();
      }
    }
  }
#endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
void lpsr2lilypondTranslator::visitStart (S_msrKey& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream ss;

      ss <<
        "% --> Start visiting msrKey '" <<
        elt->asString () <<
        '\'' <<
        ", line " << elt->getInputLineNumber () <<
      std::endl;

      if (traceLpsrVisitors) {
        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << ss.str ();
      }
    }
  }
#endif // MF_TRACE_IS_ENABLED

  msrKeyKind
    keyKind =
      elt->getKeyKind ();

  Bool doGenerateKey (true);

  if (! fCurrentVoiceKey) {
    // this is the first clef in this voice
    if (
// add Bool msrKey::isCMajor () method JM or create an CMajorKey variable // JMI 0.9.70
//       keyKind == kCommonKey
//         &&
      gGlobalLpsr2lilypondOahGroup->getNoInitialCMajorKey ()
    ) {
      doGenerateKey = false;
    }
  }

  if (doGenerateKey) {
    switch (fCurrentVoice->getVoiceKind ()) {
      case msrVoiceKind::kVoiceKindDynamics:
        break;

      case msrVoiceKind::kVoiceKindRegular:
      case msrVoiceKind::kVoiceKindHarmonies:
        switch (keyKind) {
          case msrKeyKind::kKeyTraditional:
            {
              fLilypondCodeStream <<
                "\\key " <<
                msrQuarterTonesPitchKindAsStringInLanguage (
                  elt->getKeyTonicQuarterTonesPitchKind (),
                  gLpsrOahGroup->
                    getLpsrQuarterTonesPitchesLanguageKind ());

                msrModeKind
                  modeKind = elt->getModeKind (),
                  modeKindToBeUsed = msrModeKind::kMode_UNKNOWN_;

                switch (modeKind) {
                  case msrModeKind::kMode_UNKNOWN_:
                    modeKindToBeUsed = msrModeKind::kModeMajor; // JMI
                    break;
                  default:
                    modeKindToBeUsed = modeKind;
                } // switch

              fLilypondCodeStream <<
                cLilyPondSpace <<
                msrModeKindAsLilypondString (
                  modeKindToBeUsed);

                if (gGlobalLpsr2lilypondOahGroup->getInputLineNumbers ()) {
                  fLilypondCodeStream <<
                    " %{ <-- line " <<
                    elt->getInputLineNumber () <<
                    " %} ";
                }

                fLilypondCodeStream << std::endl;
              }
            break;

          case msrKeyKind::kKeyHumdrumScot:
            {
              const std::vector <S_msrHumdrumScotKeyItem>&
                humdrumScotKeyItemsVector =
                  elt->getHumdrumScotKeyItemsVector ();

              if (! humdrumScotKeyItemsVector.empty ()) {
                fLilypondCodeStream <<
                  std::endl <<
                  "\\set Staff.keyAlterations = #`(";

                std::vector <S_msrHumdrumScotKeyItem>::const_iterator
                  iBegin = humdrumScotKeyItemsVector.begin (),
                  iEnd   = humdrumScotKeyItemsVector.end (),
                  i      = iBegin;

                for ( ; ; ) {
                  S_msrHumdrumScotKeyItem item = (*i);

                  if (elt->getKeyItemsOctavesAreSpecified ()) {
                    //   JMI   "((octave . step) . alter) ((octave . step) . alter) ...)";
                    //\set Staff.keyAlterations = #`(((3 . 3) . 7) ((3 . 5) . 3) ((3 . 6) . 3))"  \time 2/4

                      // in MusicXML, octave number is 4 for the octave
                      // starting with middle C,
                      // and the latter is c' in LilyPond
                      fLilypondCodeStream <<
                        '(' <<
                          '(' <<
                          (int) item->getKeyItemOctaveKind () - 3 <<
                          " . " <<
                          (int) item->getKeyItemDiatonicPitchKind () << // JMI ???
                          ")" <<
                        " . ," <<
                        alterationKindAsLilypondString (
                          item->getKeyItemAlterationKind ()) <<
                        ")";
                  }

                  else {
                    // Alternatively, for each item in the list, using the more concise format (step . alter) specifies that the same alteration should hold in all octaves.

                      fLilypondCodeStream <<
                        '(' <<
                        (int) item->getKeyItemDiatonicPitchKind () << // JMI ???
                        " . ," <<
                        alterationKindAsLilypondString (
                          item->getKeyItemAlterationKind ()) <<
                        ")";
                  }

                  if (++i == iEnd) break;

                  fLilypondCodeStream << cLilyPondSpace;
                } // for

                fLilypondCodeStream <<
                  ")";
              }

              else {
                lpsr2lilypondInternalError (
                  gServiceRunData->getInputSourceName (),
                  elt->getInputLineNumber (),
                  __FILE__, mfInputLineNumber (__LINE__),
                  "Humdrum/Scot key items vector is empty");
              }
            }
            break;
        } // switch
        break;

      case msrVoiceKind::kVoiceKindFiguredBass:
        // not \key should be generated in \figuremode
        break;
    } // switch
  }
}

void lpsr2lilypondTranslator::visitEnd (S_msrKey& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream ss;

      ss <<
        "% --> End visiting msrKey '" <<
        elt->asString () <<
        "'"  <<
        ", line " << elt->getInputLineNumber () <<
      std::endl;

      if (traceLpsrVisitors) {
        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << ss.str ();
      }
    }
  }
#endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
void lpsr2lilypondTranslator::visitStart (S_msrTimeSignature& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gLpsrOahGroup->
          getTraceLpsrVisitors (),

      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream ss;

      ss <<
        "% --> Start visiting msrTimeSignature " <<
        elt->asString () <<
        ", line " << elt->getInputLineNumber () <<
      std::endl;

      if (traceLpsrVisitors) {
        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << ss.str ();
      }
    }
  }
#endif // MF_TRACE_IS_ENABLED

//   gLog <<
//     "---> visitStart (S_msrTimeSignature& elt): " <<
//     ", elt: " <<
//     elt << // JMI  0.9.73
//     std::endl;

//   msrTimeSignatureSymbolKind
//     timeSignatureSymbolKind =
//       elt->getTimeSignatureSymbolKind ();
//
//   gLog <<
//     "---> timeSignatureSymbolKind: " << timeSignatureSymbolKind << // JMI  0.9.70
//     std::endl;

  Bool doGenerateTimeSignature (true);

  if (! fCurrentVoiceTimeSignature) { // JMI  0.9.70
    // this is the first clef in this voice
    if (
      gGlobalLpsr2lilypondOahGroup->getNoInitialCommonTime ()
    ) {
//       doGenerateTimeSignature = false;
    }
  }

  if (doGenerateTimeSignature) {
    // is this the end of a senza misura fragment?
    if (fOnGoingSenzaMisura) {

//       timeSignatureSymbolKind != msrTimeSignatureSymbolKind::kTimeSignatureSymbolSenzaMisura) {
//       fLilypondCodeStream <<
//         "\\undo\\omit Staff.TimeSignature" <<

      fLilypondCodeStream <<
        "\\cadenzaOff" <<
        std::endl;

      fOnGoingSenzaMisura = false;
    }

    // handle the time
    switch (elt->getTimeSignatureSymbolKind ()) {
      case msrTimeSignatureSymbolKind::kTimeSignatureSymbolSenzaMisura:
        // senza misura time

        /* JMI
        fLilypondCodeStream <<
          "\\omit Staff.TimeSignature" <<
          std::endl;
    */

        fLilypondCodeStream <<
          "\\cadenzaOn" <<
          std::endl;

        fOnGoingSenzaMisura = true; // JMI  0.9.70
        break;

      default:
        // regular time signature
        generateRegularTimeSignature (elt);
    } // switch
  }

  fCurrentVoiceTimeSignature = elt;
}

void lpsr2lilypondTranslator::generateRegularTimeSignature (
  const S_msrTimeSignature& timeSignature)
{
  const std::vector <S_msrTimeSignatureItem>&
    timeSignatureItemsVector =
      timeSignature->getTimeSignatureItemsVector ();

  int timesItemsNumber =
    timeSignatureItemsVector.size ();

  if (timesItemsNumber) {
    // should there be a single number?
    switch (timeSignature->getTimeSignatureSymbolKind ()) {
      case msrTimeSignatureSymbolKind::kTimeSignatureSymbolCommon:
        break;
      case msrTimeSignatureSymbolKind::kTimeSignatureSymbolCut:
        break;
      case msrTimeSignatureSymbolKind::kTimeSignatureSymbolNote:
        break;
      case msrTimeSignatureSymbolKind::kTimeSignatureSymbolDottedNote:
        break;
      case msrTimeSignatureSymbolKind::kTimeSignatureSymbolSingleNumber:
        fLilypondCodeStream <<
          "\\once\\override Staff.TimeSignature.style = #'single-digit" <<
          std::endl;
        break;
      case msrTimeSignatureSymbolKind::kTimeSignatureSymbolSenzaMisura:
        break;
      case msrTimeSignatureSymbolKind::kTimeSignatureSymbolNone:
        break;
    } // switch

    if (! timeSignature->getTimeIsCompound ()) {
      // simple time
      // \time "3/4" for 3/4
      // or senza misura

      S_msrTimeSignatureItem
        timeSignatureItem =
          timeSignatureItemsVector.at (0); // the only element;

      // fetch the time signature item beat numbers vector
      const std::vector <int>&
        beatsNumbersVector =
          timeSignatureItem->
            getTimeSignatureBeatsNumbersVector ();

      // should the time be numeric?
      if (
        timeSignature->getTimeSignatureSymbolKind ()
          ==
        msrTimeSignatureSymbolKind::kTimeSignatureSymbolNone
          ||
        gGlobalLpsr2lilypondOahGroup->getNumericalTime ()
      ) {
        fLilypondCodeStream <<
          "\\numericTimeSignature ";
      }

      fLilypondCodeStream <<
        "\\time " <<
        beatsNumbersVector.at (0) << // the only element
        '/' <<
        timeSignatureItem->getTimeSignatureBeatValue ();
    }

    else {
      // compound time
      // \compoundMeter #'(3 2 8) for 3+2/8
      // \compoundMeter #'((3 8) (2 8) (3 4)) for 3/8+2/8+3/4
      // \compoundMeter #'((3 2 8) (3 4)) for 3+2/8+3/4

      fLilypondCodeStream <<
        "\\compoundMeter #`(";

      // handle all the time signature items in the vector
      for (int i = 0; i < timesItemsNumber; ++i) {
        S_msrTimeSignatureItem
          timeSignatureItem =
            timeSignatureItemsVector.at (i);

        // fetch the time signature item beat numbers vector
        const std::vector <int>&
          beatsNumbersVector =
            timeSignatureItem->
              getTimeSignatureBeatsNumbersVector ();

        int beatsNumbersNumber =
          beatsNumbersVector.size ();

        // first generate the opening parenthesis
        fLilypondCodeStream <<
          cLilypondTimeSignatureOpener;

        // then generate all beats numbers in the vector
        for (int j = 0; j < beatsNumbersNumber; ++j) {
          fLilypondCodeStream <<
            beatsNumbersVector.at (j) <<
            cLilyPondSpace;
        } // for

        // then generate the beat type
        fLilypondCodeStream <<
          timeSignatureItem->getTimeSignatureBeatValue ();

        // and finally generate the closing parenthesis
        fLilypondCodeStream <<
          ")";

        if (i != timesItemsNumber - 1) {
          fLilypondCodeStream <<
            cLilyPondSpace;
        }
      } // for

      fLilypondCodeStream <<
        cLilypondTimeSignatureCloser;
    }

    if (gGlobalLpsr2lilypondOahGroup->getInputLineNumbers ()) {
      fLilypondCodeStream <<
        " %{ <-- line " <<
        timeSignature->getInputLineNumber () <<
         " %} ";
    }

    fLilypondCodeStream << std::endl;
  }

  else {
    // there are no time signature items
    if (
      timeSignature->getTimeSignatureSymbolKind ()
        !=
      msrTimeSignatureSymbolKind::kTimeSignatureSymbolSenzaMisura
    ) {
      lpsr2lilypondInternalError (
        gServiceRunData->getInputSourceName (),
        timeSignature->getInputLineNumber (),
        __FILE__, mfInputLineNumber (__LINE__),
        "time signature items vector is empty");
    }
  }
}

void lpsr2lilypondTranslator::visitEnd (S_msrTimeSignature& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream ss;

      ss <<
        "% --> End visiting msrTimeSignature " <<
        elt->asString () <<
        ", line " << elt->getInputLineNumber () <<
      std::endl;

      if (traceLpsrVisitors) {
        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << ss.str ();
      }
    }
  }
#endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
void lpsr2lilypondTranslator::visitStart (S_msrTransposition& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream ss;

      ss <<
        "% --> Start visiting msrTransposition" <<
        ", line " << elt->getInputLineNumber () <<
      std::endl;

      if (traceLpsrVisitors) {
        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << ss.str ();
      }
    }
  }
#endif // MF_TRACE_IS_ENABLED

  int  transposeDiatonic = elt->getTranspositionDiatonic ();
  int  transposeChromatic = elt->getTranspositionChromatic ();
  int  transposeOctaveChange = elt->getTranspositionOctaveChange ();
  Bool transposeDouble = elt->getTranspositionDouble ();

/*
  // transposition in LilyPond is relative to c',
  // i.e. the C in the middle of the piano keyboard

The diatonic element specifies the number of pitch steps needed to go from written to sounding pitch. This allows for correct spelling of enharmonic transpositions.

The chromatic element represents the number of semitones needed to get from written to sounding pitch. This value does not include octave-change values; the values for both elements need to be added to the written pitch to get the correct sounding pitch.

The octave-change element indicates how many octaves to add to get from written pitch to sounding pitch.

If the double element is present, it indicates that the music is doubled one octave down from what is currently written (as is the case for mixed cello / bass parts in orchestral literature).
*/

  // determine transposition pitch
  msrQuarterTonesPitchKind
    transpositionPitchKind = msrQuarterTonesPitchKind::kQTP_UNKNOWN_;

  switch (transposeChromatic) {
    case -11:
      switch (transposeDiatonic) {
        case -7:
          transpositionPitchKind = msrQuarterTonesPitchKind::kQTP_C_Sharp;
          break;

        case -6:
          transpositionPitchKind = msrQuarterTonesPitchKind::kQTP_D_Flat;
          break;

        default:
          transposeDiatonicError (
            elt->getInputLineNumber (),
            transposeDiatonic,
            transposeChromatic);
      } // switch
      break;

    case -10:
      switch (transposeDiatonic) {
        case -6:
          transpositionPitchKind = msrQuarterTonesPitchKind::kQTP_D_Natural;
          break;

        default:
          transposeDiatonicError (
            elt->getInputLineNumber (),
            transposeDiatonic,
            transposeChromatic);
      } // switch
      break;

    case -9:
      switch (transposeDiatonic) {
        case -6:
          transpositionPitchKind = msrQuarterTonesPitchKind::kQTP_D_Sharp;
          break;

        case -5:
          transpositionPitchKind = msrQuarterTonesPitchKind::kQTP_E_Flat;
          break;

        default:
          transposeDiatonicError (
            elt->getInputLineNumber (),
            transposeDiatonic,
            transposeChromatic);
      } // switch
      break;

    case -8:
      switch (transposeDiatonic) {
        case -5:
          transpositionPitchKind = msrQuarterTonesPitchKind::kQTP_E_Natural;
          break;

        default:
          transposeDiatonicError (
            elt->getInputLineNumber (),
            transposeDiatonic,
            transposeChromatic);
      } // switch
      break;

    case -7:
      switch (transposeDiatonic) {
        case -4:
          transpositionPitchKind = msrQuarterTonesPitchKind::kQTP_F_Natural;
          break;

        default:
          transposeDiatonicError (
            elt->getInputLineNumber (),
            transposeDiatonic,
            transposeChromatic);
      } // switch
      break;

    case -6:
      switch (transposeDiatonic) {
        case -4:
          transpositionPitchKind = msrQuarterTonesPitchKind::kQTP_F_Sharp;
          break;

        case -3:
          transpositionPitchKind = msrQuarterTonesPitchKind::kQTP_G_Flat;
          break;

        default:
          transposeDiatonicError (
            elt->getInputLineNumber (),
            transposeDiatonic,
            transposeChromatic);
      } // switch
      break;

    case -5:
      switch (transposeDiatonic) {
        case -3:
          transpositionPitchKind = msrQuarterTonesPitchKind::kQTP_G_Natural;
          break;

        default:
          transposeDiatonicError (
            elt->getInputLineNumber (),
            transposeDiatonic,
            transposeChromatic);
      } // switch
      break;

    case -4:
      switch (transposeDiatonic) {
        case -3:
          transpositionPitchKind = msrQuarterTonesPitchKind::kQTP_G_Sharp;
          break;

        case -2:
          transpositionPitchKind = msrQuarterTonesPitchKind::kQTP_A_Flat;
          break;

        default:
          transposeDiatonicError (
            elt->getInputLineNumber (),
            transposeDiatonic,
            transposeChromatic);
      } // switch
      break;

    case -3:
      switch (transposeDiatonic) {
        case -2:
          transpositionPitchKind = msrQuarterTonesPitchKind::kQTP_A_Natural;
          break;

        default:
          transposeDiatonicError (
            elt->getInputLineNumber (),
            transposeDiatonic,
            transposeChromatic);
      } // switch
      break;

    case -2:
      switch (transposeDiatonic) {
        case -2:
          transpositionPitchKind = msrQuarterTonesPitchKind::kQTP_A_Sharp;
          break;

        case -1:
          transpositionPitchKind = msrQuarterTonesPitchKind::kQTP_B_Flat;
          break;

        default:
          transposeDiatonicError (
            elt->getInputLineNumber (),
            transposeDiatonic,
            transposeChromatic);
      } // switch
      break;

    case -1:
      switch (transposeDiatonic) {
        case -1:
          transpositionPitchKind = msrQuarterTonesPitchKind::kQTP_B_Natural;
          break;

        default:
          transposeDiatonicError (
            elt->getInputLineNumber (),
            transposeDiatonic,
            transposeChromatic);
      } // switch
      break;

    case 0:
      switch (transposeDiatonic) {
        case 0:
          transpositionPitchKind = msrQuarterTonesPitchKind::kQTP_C_Natural;
          break;

        default:
          transposeDiatonicError (
            elt->getInputLineNumber (),
            transposeDiatonic,
            transposeChromatic);
      } // switch
      break;

    case 1:
      switch (transposeDiatonic) {
        case 0:
          transpositionPitchKind = msrQuarterTonesPitchKind::kQTP_C_Sharp;
          break;

        case 1:
          transpositionPitchKind = msrQuarterTonesPitchKind::kQTP_D_Flat;
          break;

        default:
          transposeDiatonicError (
            elt->getInputLineNumber (),
            transposeDiatonic,
            transposeChromatic);
      } // switch
      break;

    case 2:
      switch (transposeDiatonic) {
        case 1:
          transpositionPitchKind = msrQuarterTonesPitchKind::kQTP_D_Natural;
          break;

        default:
          transposeDiatonicError (
            elt->getInputLineNumber (),
            transposeDiatonic,
            transposeChromatic);
      } // switch
      break;

    case 3:
      switch (transposeDiatonic) {
        case 1:
          transpositionPitchKind = msrQuarterTonesPitchKind::kQTP_D_Sharp;
          break;

        case 2:
          transpositionPitchKind = msrQuarterTonesPitchKind::kQTP_E_Flat;
          break;

        default:
          transposeDiatonicError (
            elt->getInputLineNumber (),
            transposeDiatonic,
            transposeChromatic);
      } // switch
      break;

    case 4:
      switch (transposeDiatonic) {
        case 2:
          transpositionPitchKind = msrQuarterTonesPitchKind::kQTP_E_Natural;
          break;

        default:
          transposeDiatonicError (
            elt->getInputLineNumber (),
            transposeDiatonic,
            transposeChromatic);
      } // switch
      break;

    case 5:
      switch (transposeDiatonic) {
        case 3:
          transpositionPitchKind = msrQuarterTonesPitchKind::kQTP_F_Natural;
          break;

        default:
          transposeDiatonicError (
            elt->getInputLineNumber (),
            transposeDiatonic,
            transposeChromatic);
      } // switch
      break;

    case 6:
      switch (transposeDiatonic) {
        case 3:
          transpositionPitchKind = msrQuarterTonesPitchKind::kQTP_F_Sharp;
          break;

        case 4:
          transpositionPitchKind = msrQuarterTonesPitchKind::kQTP_G_Flat;
          break;

        default:
          transposeDiatonicError (
            elt->getInputLineNumber (),
            transposeDiatonic,
            transposeChromatic);
      } // switch
      break;

    case 7:
      switch (transposeDiatonic) {
        case 4:
          transpositionPitchKind = msrQuarterTonesPitchKind::kQTP_G_Natural;
          break;

        default:
          transposeDiatonicError (
            elt->getInputLineNumber (),
            transposeDiatonic,
            transposeChromatic);
      } // switch
      break;

    case 8:
      switch (transposeDiatonic) {
        case 4:
          transpositionPitchKind = msrQuarterTonesPitchKind::kQTP_G_Sharp;
          break;

        case 5:
          transpositionPitchKind = msrQuarterTonesPitchKind::kQTP_A_Flat;
          break;

        default:
          transposeDiatonicError (
            elt->getInputLineNumber (),
            transposeDiatonic,
            transposeChromatic);
      } // switch
      break;

    case 9:
      switch (transposeDiatonic) {
        case 5:
          transpositionPitchKind = msrQuarterTonesPitchKind::kQTP_A_Natural;
          break;

        default:
          transposeDiatonicError (
            elt->getInputLineNumber (),
            transposeDiatonic,
            transposeChromatic);
      } // switch
      break;

    case 10:
      switch (transposeDiatonic) {
        case 5:
          transpositionPitchKind = msrQuarterTonesPitchKind::kQTP_A_Sharp;
          break;

        case 6:
          transpositionPitchKind = msrQuarterTonesPitchKind::kQTP_B_Flat;
          break;

        default:
          transposeDiatonicError (
            elt->getInputLineNumber (),
            transposeDiatonic,
            transposeChromatic);
      } // switch
      break;

    case 11:
      switch (transposeDiatonic) {
        case 6:
          transpositionPitchKind = msrQuarterTonesPitchKind::kQTP_B_Natural;
          break;

        default:
          transposeDiatonicError (
            elt->getInputLineNumber (),
            transposeDiatonic,
            transposeChromatic);
      } // switch
      break;

    default:
      {
        std::stringstream ss;

        ss <<
          "fTransposition chromatic '" << transposeChromatic <<
          "' is not between -12 and 12, ignored";

        lpsr2lilypondError (
          gServiceRunData->getInputSourceName (),
          elt->getInputLineNumber (),
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }
  } // switch

  // 4 is the octave starting with middle C
  msrOctaveKind transpositionOctaveKind;

  if (transposeChromatic < 0) {
    transpositionOctaveKind = msrOctaveKind::kOctave3;
  }
  else {
    transpositionOctaveKind = msrOctaveKind::kOctave4;
  }

  // take the transpose octave change into account
  for (int i = 1; i <= transposeOctaveChange; ++i) {
    ++transpositionOctaveKind;
  } // for

  // take the transpose double if any into account
  if (transposeDouble) {
    /*
      If the double element is present,
      it indicates that the music is doubled one octave down
      from what is currently written
      (as is the case for mixed cello / bass parts in orchestral literature).
    */

    --transpositionOctaveKind;
  }

  std::string
    transpositionPitchKindAsString =
      msrQuarterTonesPitchKindAsStringInLanguage (
        transpositionPitchKind,
        gLpsrOahGroup->
          getLpsrQuarterTonesPitchesLanguageKind ());

  std::string
    transpositionOctaveKindAsString =
      absoluteOctaveAsLilypondString (
        elt->getInputLineNumber (),
        transpositionOctaveKind);

/* JMI
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceTranspositions ()) {
    fLilypondCodeStream << // JMI
      "Handlling transpose '" <<
      elt->transposeAsString () <<
      "' ignored because it is already present in voice \"" <<
      fCurrentVoice->getVoicePathLikeName () <<
      "\"" <<
      / * JMI
      getStaffPathLikeName () <<
      "\" in part " <<
      fStaffUpLinkToPart->fetchPartNameForTrace () <<
      * /
      std::endl <<
      ", transpositionPitch: " <<
      transpositionPitchAsString <<
      ", transpositionOctaveKind: " <<
      transpositionOctaveKindAsString <<
      '(' << transpositionOctaveKind << ")" <<
      std::endl;
    }
#endif // MF_TRACE_IS_ENABLED
*/

  // now we can generate the transpostion command
  fLilypondCodeStream <<
    "\\transposition " <<
    transpositionPitchKindAsString <<
    transpositionOctaveKindAsString <<
    cLilyPondSpace <<
    std::endl;
}

void lpsr2lilypondTranslator::visitEnd (S_msrTransposition& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream ss;

      ss <<
        "% --> End visiting msrTransposition" <<
        ", line " << elt->getInputLineNumber () <<
      std::endl;

      if (traceLpsrVisitors) {
        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << ss.str ();
      }
    }
  }
#endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
void lpsr2lilypondTranslator::visitStart (S_msrTempo& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream ss;

      ss <<
        "% --> Start visiting msrTempo" <<
        ", line " << elt->getInputLineNumber () <<
      std::endl;

      if (traceLpsrVisitors) {
        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << ss.str ();
      }
    }
  }
#endif // MF_TRACE_IS_ENABLED

  switch (elt->getTempoPlacementKind ()) {
    case msrPlacementKind::kPlacement_UNKNOWN_:
      break;
    case msrPlacementKind::kPlacementAbove:
      // by default, so nothing to do
      break;
    case msrPlacementKind::kPlacementBelow:
      fLilypondCodeStream <<
        "\\once\\override Score.MetronomeMark.direction = #DOWN" <<
        std::endl;
      break;
    } // switch

  switch (elt->getTempoKind ()) {
    case msrTempoKBeatUnitsKind::kTempoBeatUnits_UNKNOWN_:
      break;

    case msrTempoKBeatUnitsKind::kTempoBeatUnitsWordsOnly:
      generateTempoBeatUnitsWordsOnly (elt);
      break;

    case msrTempoKBeatUnitsKind::kTempoBeatUnitsPerMinute:
      generateTempoBeatUnitsPerMinute (elt);
      break;

    case msrTempoKBeatUnitsKind::kTempoBeatUnitsEquivalence:
      generateTempoBeatUnitsEquivalence (elt);
      break;

    case msrTempoKBeatUnitsKind::kTempoNotesRelationship:
      generateTempoNotesRelationship (elt);
      break;
  } // switch
}

void lpsr2lilypondTranslator::generateTempoBeatUnitsWordsOnly (
  const S_msrTempo& tempo)
{
  fLilypondCodeStream <<
    "\\tempo ";

  const std::list <S_msrWords>&
    tempoWordsList =
      tempo->getTempoWordsList ();

  size_t tempoWordsListSize = tempoWordsList.size ();

  if (tempoWordsListSize) {
    std::list <S_msrWords>::const_iterator
      iBegin = tempoWordsList.begin (),
      iEnd   = tempoWordsList.end (),
      i      = iBegin;

    for ( ; ; ) {
      S_msrWords words = (*i);

      fLilypondCodeStream <<
        "\"" << words->getWordsContents () << "\"";

      if (++i == iEnd) break;

      fLilypondCodeStream <<
        cLilyPondSpace;
    } // for

    fLilypondCodeStream <<
      std::endl;
  }
}

void lpsr2lilypondTranslator::generateTempoBeatUnitsPerMinute (
  const S_msrTempo& tempo)
{
  msrTempoParenthesizedKind
    tempoParenthesizedKind =
      tempo->getTempoParenthesizedKind ();

  const std::list <S_msrWords>&
    tempoWordsList =
      tempo->getTempoWordsList ();

  size_t tempoWordsListSize = tempoWordsList.size ();

  mfDottedNotesDuration tempoBeatUnit = tempo->getTempoBeatUnit ();
  std::string           tempoPerMinute = tempo->getTempoPerMinute ();

  switch (tempoParenthesizedKind) {
    case msrTempoParenthesizedKind::kTempoParenthesizedYes:
      fLilypondCodeStream <<
        "\\tempo " <<
        "\\markup {" <<
        std::endl;

      ++gIndenter;

      fLilypondCodeStream <<
        "\\concat {" <<
        std::endl;

      ++gIndenter;

      if (tempoWordsListSize) {
        std::list <S_msrWords>::const_iterator
          iBegin = tempoWordsList.begin (),
          iEnd   = tempoWordsList.end (),
          i      = iBegin;

        for ( ; ; ) {
          S_msrWords words = (*i);

          fLilypondCodeStream <<
            "\"" << words->getWordsContents () << "\"";

          if (++i == iEnd) break;

          fLilypondCodeStream <<
            cLilyPondSpace;
        } // for
      }

      fLilypondCodeStream <<
        cLilypondTempoBeatUnitsPerMinuteOpener;
      if (
        versionNumberGreaterThanOrEqualTo (
          gGlobalLpsr2lilypondOahGroup->getLilypondVersion (),
          "2.21.0")
      ) {
        fLilypondCodeStream <<
          " \\smaller \\general-align #Y #DOWN \\note {";
      }
      else {
        fLilypondCodeStream <<
          "\\smaller \\general-align #Y #DOWN \\note #\"";
      }

      fLilypondCodeStream <<
        dottedNotesDurationAsLilypondString (
          tempo->getInputLineNumber (),
          tempoBeatUnit);

      if (
        versionNumberGreaterThanOrEqualTo (
          gGlobalLpsr2lilypondOahGroup->getLilypondVersion (),
          "2.21.0")
      ) {
        fLilypondCodeStream <<
          "} #UP";
      }
      else {
        fLilypondCodeStream <<
          "\" #UP";
      }

      fLilypondCodeStream <<
        std::endl <<
        "\" = \"" <<
        std::endl <<
        tempoPerMinute <<
        cLilypondTempoBeatUnitsPerMinuteCloser <<
        std::endl;

      --gIndenter;

      fLilypondCodeStream << std::endl;

      fLilypondCodeStream <<
        "} " << "% concat" <<
        std::endl;

      --gIndenter;

      fLilypondCodeStream <<
        '}' <<
        std::endl;
    break;

  case msrTempoParenthesizedKind::kTempoParenthesizedNo:
    fLilypondCodeStream <<
      "\\tempo " <<
      "\\markup {" <<
      std::endl;

    ++gIndenter;

    if (tempoWordsListSize) {
      std::list <S_msrWords>::const_iterator
        iBegin = tempoWordsList.begin (),
        iEnd   = tempoWordsList.end (),
        i      = iBegin;

      for ( ; ; ) {
        S_msrWords words = (*i);

        fLilypondCodeStream <<
          "\"" << words->getWordsContents () << "\"";

        if (++i == iEnd) break;

        fLilypondCodeStream <<
          cLilyPondSpace;
      } // for
    }

    fLilypondCodeStream <<
      "\\concat {" <<
      std::endl;

    ++gIndenter;

    if (
      versionNumberGreaterThanOrEqualTo (
        gGlobalLpsr2lilypondOahGroup->getLilypondVersion (),
        "2.21.0")
    ) {
      fLilypondCodeStream <<
        " \\smaller \\general-align #Y #DOWN \\note {";
    }
    else {
      fLilypondCodeStream <<
        " \\smaller \\general-align #Y #DOWN \\note #\"";
    }

    fLilypondCodeStream <<
      dottedNotesDurationAsLilypondString (
        tempo->getInputLineNumber (),
        tempoBeatUnit);

    if (
      versionNumberGreaterThanOrEqualTo (
        gGlobalLpsr2lilypondOahGroup->getLilypondVersion (),
        "2.21.0")
    ) {
      fLilypondCodeStream <<
        "} #UP";
    }
    else {
      fLilypondCodeStream <<
        "\" #UP";
    }

    fLilypondCodeStream <<
      std::endl <<
      "\" = \"" <<
      std::endl <<
      tempoPerMinute <<
    std::endl;

  --gIndenter;

  fLilypondCodeStream <<
    "} " << "% concat" <<
    std::endl;

  --gIndenter;

  fLilypondCodeStream <<
    '}' << // \\markup
    std::endl;
  } // switch
}

void lpsr2lilypondTranslator::generateTempoBeatUnitsEquivalence (
  const S_msrTempo& tempo)
{
  msrTempoParenthesizedKind
    tempoParenthesizedKind =
      tempo->getTempoParenthesizedKind ();

  const std::list <S_msrWords>&
    tempoWordsList =
      tempo->getTempoWordsList ();

  size_t tempoWordsListSize = tempoWordsList.size ();

  mfDottedNotesDuration tempoBeatUnit = tempo->getTempoBeatUnit ();
  std::string           tempoPerMinute = tempo->getTempoPerMinute ();

  fLilypondCodeStream <<
    "\\tempo ";

  if (tempoWordsListSize) {
    std::list <S_msrWords>::const_iterator
      iBegin = tempoWordsList.begin (),
      iEnd   = tempoWordsList.end (),
      i      = iBegin;

    for ( ; ; ) {
      S_msrWords words = (*i);

      fLilypondCodeStream <<
        "\"" << words->getWordsContents () << "\"";

      if (++i == iEnd) break;

      fLilypondCodeStream <<
        cLilyPondSpace;
    } // for
  }

  fLilypondCodeStream <<
    cLilyPondSpace <<
    "\\markup {" <<
    std::endl;

  ++gIndenter;

  fLilypondCodeStream <<
    "\\concat {" <<
    std::endl;

  ++gIndenter;

  switch (tempoParenthesizedKind) {
    case msrTempoParenthesizedKind::kTempoParenthesizedYes:
      fLilypondCodeStream <<
        '(' <<
        std::endl;
      break;
    case msrTempoParenthesizedKind::kTempoParenthesizedNo:
      break;
  } // switch

  ++gIndenter;

  if (
    versionNumberGreaterThanOrEqualTo (
      gGlobalLpsr2lilypondOahGroup->getLilypondVersion (),
      "2.21.0")
  ) {
    fLilypondCodeStream <<
      " \\smaller \\general-align #Y #DOWN \\note {";
  }
  else {
    fLilypondCodeStream <<
      " \\smaller \\general-align #Y #DOWN \\note #\"";
  }

  fLilypondCodeStream <<
    dottedNotesDurationAsLilypondString (
      tempo->getInputLineNumber (),
      tempoBeatUnit);

  if (
    versionNumberGreaterThanOrEqualTo (
      gGlobalLpsr2lilypondOahGroup->getLilypondVersion (),
      "2.21.0")
  ) {
    fLilypondCodeStream <<
      "} #UP";
  }
  else {
    fLilypondCodeStream <<
      "\" #UP";
  }

  fLilypondCodeStream <<
    std::endl <<
    "\" = \"" <<
    std::endl;

  if (
    versionNumberGreaterThanOrEqualTo (
      gGlobalLpsr2lilypondOahGroup->getLilypondVersion (),
      "2.21.0")
  ) {
    fLilypondCodeStream <<
      " \\smaller \\general-align #Y #DOWN \\note {";
  }
  else {
    fLilypondCodeStream <<
      " \\smaller \\general-align #Y #DOWN \\note #\"";
  }

  fLilypondCodeStream <<
    dottedNotesDurationAsLilypondString (
      tempo->getInputLineNumber (),
      tempo->getTempoEquivalentBeatUnit ());

  if (
    versionNumberGreaterThanOrEqualTo (
      gGlobalLpsr2lilypondOahGroup->getLilypondVersion (),
      "2.21.0")
  ) {
    fLilypondCodeStream <<
      "} #UP";
  }
  else {
    fLilypondCodeStream <<
      "\" #UP";
  }

  fLilypondCodeStream << std::endl;

  --gIndenter;

  switch (tempoParenthesizedKind) {
    case msrTempoParenthesizedKind::kTempoParenthesizedYes:
      fLilypondCodeStream <<
        ")" <<
        std::endl;
      break;
    case msrTempoParenthesizedKind::kTempoParenthesizedNo:
      break;
  } // switch

  --gIndenter;

  fLilypondCodeStream <<
    '}' <<
    std::endl;

  --gIndenter;

  fLilypondCodeStream <<
    '}' <<
    std::endl;
}

void lpsr2lilypondTranslator::generateTempoNotesRelationship (
  const S_msrTempo& tempo)
{
  msrTempoParenthesizedKind
    tempoParenthesizedKind =
      tempo->getTempoParenthesizedKind ();

  const std::list <S_msrWords>&
    tempoWordsList =
      tempo->getTempoWordsList ();

  size_t tempoWordsListSize = tempoWordsList.size ();

  fLilypondCodeStream <<
    "\\tempoNotesRelationship #\"";

  if (tempoWordsListSize) {
    std::list <S_msrWords>::const_iterator
      iBegin = tempoWordsList.begin (),
      iEnd   = tempoWordsList.end (),
      i      = iBegin;

    for ( ; ; ) {
      S_msrWords words = (*i);

      fLilypondCodeStream <<
 // JMI       "\"" <<
        words->getWordsContents (); // JMI <<
  // JMI      "\"";

      if (++i == iEnd) break;

      fLilypondCodeStream <<
        cLilyPondSpace;
    } // for
  }

  fLilypondCodeStream <<
    "\"";

  switch (tempoParenthesizedKind) {
    case msrTempoParenthesizedKind::kTempoParenthesizedYes:
      fLilypondCodeStream <<
        " ##t";
      break;
    case msrTempoParenthesizedKind::kTempoParenthesizedNo:
      fLilypondCodeStream <<
        " ##f";
      break;
  } // switch

  fLilypondCodeStream << std::endl;
}

void lpsr2lilypondTranslator::visitStart (S_msrTempoNotesRelationshipElements& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream ss;

      ss <<
        "% --> Start visiting msrTempoNotesRelationshipElements" <<
        ", line " << elt->getInputLineNumber () <<
      std::endl;

      if (traceLpsrVisitors) {
        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << ss.str ();
      }
    }
  }
#endif // MF_TRACE_IS_ENABLED

  fLilypondCodeStream <<
    "\\fixed b' {" <<
    std::endl;

  ++gIndenter; // decremented in visitEnd (S_msrTempoNotesRelationshipElements& elt)
}

void lpsr2lilypondTranslator::visitEnd (S_msrTempoNotesRelationshipElements& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream ss;

      ss <<
        "% --> End visiting msrTempoNotesRelationshipElements" <<
        ", line " << elt->getInputLineNumber () <<
      std::endl;

      if (traceLpsrVisitors) {
        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << ss.str ();
      }
    }
  }
#endif // MF_TRACE_IS_ENABLED

  --gIndenter; // incremented in visitStart (S_msrTempoNotesRelationshipElements& elt)

  fLilypondCodeStream <<
    std::endl <<
    '}' <<
    std::endl;
}

void lpsr2lilypondTranslator::visitStart (S_msrTempoNote& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream ss;

      ss <<
        "% --> Start visiting msrTempoNote" <<
        ", line " << elt->getInputLineNumber () <<
      std::endl;

      if (traceLpsrVisitors) {
        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << ss.str ();
      }
    }
  }
#endif // MF_TRACE_IS_ENABLED

  fLilypondCodeStream <<
    "b" << // JMI convention 0.9.70
    wholeNotesAsLilypondString (
      elt->getInputLineNumber (),
      elt->getTempoNoteWholeNotes ()) <<
      cLilyPondSpace;
}

void lpsr2lilypondTranslator::visitStart (S_msrTempoTuplet& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream ss;

      ss <<
        "% --> Start visiting msrTempoTuplet" <<
        ", line " << elt->getInputLineNumber () <<
      std::endl;

      if (traceLpsrVisitors) {
        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << ss.str ();
      }
    }
  }
#endif // MF_TRACE_IS_ENABLED

  const msrTupletFactor&
    tempoTupletFactor =
      elt->getTempoTupletFactor ();

  fLilypondCodeStream <<
    cLilypondTupletOpener1 <<
    tupletFactorAsLilypondString (tempoTupletFactor) <<
    cLilypondTupletOpener2;
}

void lpsr2lilypondTranslator::visitEnd (S_msrTempoTuplet& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream ss;

      ss <<
        "% --> End visiting msrTempoTuplet" <<
        ", line " << elt->getInputLineNumber () <<
      std::endl;

      if (traceLpsrVisitors) {
        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << ss.str ();
      }
    }
  }
#endif // MF_TRACE_IS_ENABLED

  fLilypondCodeStream <<
    cLilypondTupletCloser;
}

void lpsr2lilypondTranslator::visitEnd (S_msrTempo& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream ss;

      ss <<
        "% --> End visiting msrTempo" <<
        ", line " << elt->getInputLineNumber () <<
      std::endl;

      if (traceLpsrVisitors) {
        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << ss.str ();
      }
    }
  }
#endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
void lpsr2lilypondTranslator::visitStart (S_msrArticulation& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream ss;

      ss <<
        "% --> Start visiting msrArticulation" <<
        ", line " << elt->getInputLineNumber () <<
      std::endl;

      if (traceLpsrVisitors) {
        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << ss.str ();
      }
    }
  }
#endif // MF_TRACE_IS_ENABLED

  // don't generate the articulation here,
  // the note or chord will do it in its visitEnd () method
}

void lpsr2lilypondTranslator::visitEnd (S_msrArticulation& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream ss;

      ss <<
        "% --> End visiting msrArticulation" <<
        ", line " << elt->getInputLineNumber () <<
      std::endl;

      if (traceLpsrVisitors) {
        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << ss.str ();
      }
    }
  }
#endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
void lpsr2lilypondTranslator::visitStart (S_msrFermata& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream ss;

      ss <<
        "% --> Start visiting msrFermata" <<
        ", line " << elt->getInputLineNumber () <<
      std::endl;

      if (traceLpsrVisitors) {
        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << ss.str ();
      }
    }
  }
#endif // MF_TRACE_IS_ENABLED

/*
Articulations can be attached to rests as well as notes but they cannot be attached to multiple measure rests. A special predefined command, \fermataMarkup, is available for at- taching a fermata to a multiple measure rest (and only a multiple measure rest). This creates a MultipleMeasureRestText object.
*/

/* JMI
  switch (elt->getArticulationPlacement ()) {
    case msrPlacementKind::kPlacement_UNKNOWN_:
      // nothing needed JMI
      break;
    case msrPlacementKind::kPlacementAbove:
      fLilypondCodeStream << "^";
      break;
    case msrPlacementKind::kPlacementBelow:
      fLilypondCodeStream << "_";
      break;
  } // switch

  // don't generate fermatas for chord member notes
  if (false && ! fCurrentTupletsStack.empty ()) { // JMI
    switch (elt->getFermataTypeKind ()) {
      case msrArticulationFermataType::kArticulationFermataTypeNone:
        // no placement needed
        break;
      case msrArticulationFermataType::kArticulationFermataTypeUpright:
        // no placement needed
        break;
      case msrArticulationFermataType::kArticulationFermataTypeInverted:
        fLilypondCodeStream << "_";
        break;
    } // switch

    switch (elt->getFermataShapeKind ()) {
      case msrFermataShapeKind::kArticulationFermataNormal:
        fLilypondCodeStream << "\\fermata ";
        break;
      case msrFermataShapeKind::kArticulationFermataAngled:
        fLilypondCodeStream << "\\shortfermata ";
        break;
      case msrFermataShapeKind::kArticulationFermataSquare:
        fLilypondCodeStream << "\\longfermata ";
        break;
    } // switch
  }
*/
}

void lpsr2lilypondTranslator::visitEnd (S_msrFermata& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream ss;

      ss <<
        "% --> End visiting msrFermata" <<
        ", line " << elt->getInputLineNumber () <<
      std::endl;

      if (traceLpsrVisitors) {
        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << ss.str ();
      }
    }
  }
#endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
void lpsr2lilypondTranslator::visitStart (S_msrArpeggiato& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream ss;

      ss <<
        "% --> Start visiting msrArpeggiato" <<
        ", line " << elt->getInputLineNumber () <<
      std::endl;

      if (traceLpsrVisitors) {
        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << ss.str ();
      }
    }
  }
#endif // MF_TRACE_IS_ENABLED

/* VIRER JMI
  */
}

void lpsr2lilypondTranslator::visitEnd (S_msrArpeggiato& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream ss;

      ss <<
        "% --> End visiting msrArpeggiato" <<
        ", line " << elt->getInputLineNumber () <<
      std::endl;

      if (traceLpsrVisitors) {
        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << ss.str ();
      }
    }
  }
#endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
void lpsr2lilypondTranslator::visitStart (S_msrNonArpeggiato& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream ss;

      ss <<
        "% --> Start visiting msrNonArpeggiato" <<
        ", line " << elt->getInputLineNumber () <<
      std::endl;

      if (traceLpsrVisitors) {
        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << ss.str ();
      }
    }
  }
#endif // MF_TRACE_IS_ENABLED

/* VIRER JMI
  */
}

void lpsr2lilypondTranslator::visitEnd (S_msrNonArpeggiato& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream ss;

      ss <<
        "% --> End visiting msrNonArpeggiato" <<
        ", line " << elt->getInputLineNumber () <<
      std::endl;

      if (traceLpsrVisitors) {
        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << ss.str ();
      }
    }
  }
#endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
void lpsr2lilypondTranslator::visitStart (S_msrTechnical& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream ss;

      ss <<
        "% --> Start visiting msrTechnical" <<
        ", line " << elt->getInputLineNumber () <<
      std::endl;

      if (traceLpsrVisitors) {
        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << ss.str ();
      }
    }
  }
#endif // MF_TRACE_IS_ENABLED

  // don't generate the technical here,
  // the note or chord will do it in its visitEnd () method
}

void lpsr2lilypondTranslator::visitEnd (S_msrTechnical& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream ss;

      ss <<
        "% --> End visiting msrTechnical" <<
        ", line " << elt->getInputLineNumber () <<
      std::endl;

      if (traceLpsrVisitors) {
        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << ss.str ();
      }
    }
  }
#endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
void lpsr2lilypondTranslator::visitStart (S_msrTechnicalWithInteger& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream ss;

      ss <<
        "% --> Start visiting msrTechnicalWithInteger" <<
        ", fOnGoingChord: " <<
        fOnGoingChord <<
        ", line " << elt->getInputLineNumber () <<
      std::endl;

      if (traceLpsrVisitors) {
        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << ss.str ();
      }
    }
  }
#endif // MF_TRACE_IS_ENABLED

  // don't generate the technicalWithInteger here,
  // the note or chord will do it in its visitEnd () method
}

void lpsr2lilypondTranslator::visitEnd (S_msrTechnicalWithInteger& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream ss;

      ss <<
        "% --> End visiting msrTechnicalWithInteger" <<
        ", line " << elt->getInputLineNumber () <<
      std::endl;

      if (traceLpsrVisitors) {
        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << ss.str ();
      }
    }
  }
#endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
void lpsr2lilypondTranslator::visitStart (S_msrTechnicalWithFloat& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream ss;

      ss <<
        "% --> Start visiting msrTechnicalWithFloat" <<
        ", fOnGoingChord: " <<
        fOnGoingChord <<
        ", line " << elt->getInputLineNumber () <<
      std::endl;

      if (traceLpsrVisitors) {
        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << ss.str ();
      }
    }
  }
#endif // MF_TRACE_IS_ENABLED

  // don't generate the technicalWithFloat here,
  // the note or chord will do it in its visitEnd () method
}

void lpsr2lilypondTranslator::visitEnd (S_msrTechnicalWithFloat& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream ss;

      ss <<
        "% --> End visiting msrTechnicalWithFloat" <<
        ", line " << elt->getInputLineNumber () <<
      std::endl;

      if (traceLpsrVisitors) {
        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << ss.str ();
      }
    }
  }
#endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
void lpsr2lilypondTranslator::visitStart (S_msrTechnicalWithString& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream ss;

      ss <<
        "% --> Start visiting msrTechnicalWithString" <<
        ", line " << elt->getInputLineNumber () <<
      std::endl;

      if (traceLpsrVisitors) {
        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << ss.str ();
      }
    }
  }
#endif // MF_TRACE_IS_ENABLED

  // don't generate the technicalWithString here,
  // the note or chord will do it in its visitEnd () method
}

void lpsr2lilypondTranslator::visitEnd (S_msrTechnicalWithString& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream ss;

      ss <<
        "% --> End visiting msrTechnicalWithString" <<
        ", line " << elt->getInputLineNumber () <<
      std::endl;

      if (traceLpsrVisitors) {
        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << ss.str ();
      }
    }
  }
#endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
void lpsr2lilypondTranslator::visitStart (S_msrOrnament& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream ss;

      ss <<
        "% --> Start visiting msrOrnament" <<
        ", line " << elt->getInputLineNumber () <<
      std::endl;

      if (traceLpsrVisitors) {
        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << ss.str ();
      }
    }
  }
#endif // MF_TRACE_IS_ENABLED

  // don't generate the ornament here,
  // the note or chord will do it in its visitEnd () method
}

void lpsr2lilypondTranslator::visitEnd (S_msrOrnament& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream ss;

      ss <<
        "% --> End visiting msrOrnament" <<
        ", line " << elt->getInputLineNumber () <<
      std::endl;

      if (traceLpsrVisitors) {
        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << ss.str ();
      }
    }
  }
#endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
void lpsr2lilypondTranslator::visitStart (S_msrGlissando& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream ss;

      ss <<
        "% --> Start visiting msrGlissando" <<
        ", line " << elt->getInputLineNumber () <<
      std::endl;

      if (traceLpsrVisitors) {
        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << ss.str ();
      }
    }
  }
#endif // MF_TRACE_IS_ENABLED

  // don't generate the glissando here,
  // the note or chord will do it in its visitEnd () method
}

void lpsr2lilypondTranslator::visitEnd (S_msrGlissando& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream ss;

      ss <<
        "% --> End visiting msrGlissando" <<
        ", line " << elt->getInputLineNumber () <<
      std::endl;

      if (traceLpsrVisitors) {
        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << ss.str ();
      }
    }
  }
#endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
void lpsr2lilypondTranslator::visitStart (S_msrSlide& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream ss;

      ss <<
        "% --> Start visiting msrSlide" <<
        ", line " << elt->getInputLineNumber () <<
      std::endl;

      if (traceLpsrVisitors) {
        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << ss.str ();
      }
    }
  }
#endif // MF_TRACE_IS_ENABLED

  // don't generate the slide here,
  // the note or chord will do it in its visitEnd () method
}

void lpsr2lilypondTranslator::visitEnd (S_msrSlide& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream ss;

      ss <<
        "% --> End visiting msrSlide" <<
        ", line " << elt->getInputLineNumber () <<
      std::endl;

      if (traceLpsrVisitors) {
        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << ss.str ();
      }
    }
  }
#endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
void lpsr2lilypondTranslator::visitStart (S_msrSingleTremolo& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream ss;

      ss <<
        "% --> Start visiting msrSingleTremolo" <<
        ", line " << elt->getInputLineNumber () <<
      std::endl;

      if (traceLpsrVisitors) {
        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << ss.str ();
      }
    }
  }
#endif // MF_TRACE_IS_ENABLED

  // don't generate the singleTremolo here,
  // the note or chord will do it in its visitEnd () method
}

void lpsr2lilypondTranslator::visitEnd (S_msrSingleTremolo& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream ss;

      ss <<
        "% --> End visiting msrSingleTremolo" <<
        ", line " << elt->getInputLineNumber () <<
      std::endl;

      if (traceLpsrVisitors) {
        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << ss.str ();
      }
    }
  }
#endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
void lpsr2lilypondTranslator::visitStart (S_msrDoubleTremolo& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream ss;

      ss <<
        "% --> Start visiting msrDoubleTremolo" <<
        ", line " << elt->getInputLineNumber () <<
      std::endl;

      if (traceLpsrVisitors) {
        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << ss.str ();
      }
    }
  }
#endif // MF_TRACE_IS_ENABLED

  // get double tremolo number of repeats
  int numberOfRepeats =
    elt->getDoubleTremoloNumberOfRepeats ();

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceTremolos ()) {
    fLilypondCodeStream <<
      "% visitStart (S_msrDoubleTremolo&)" <<
      std::endl;

    ++gIndenter;

    fLilypondCodeStream <<
      "% doubleTremoloSoundingWholeNotes: " <<
      elt->getMeasureElementSoundingWholeNotes ().asFractionString () <<
      std::endl <<

      "% gdoubleTremoloElementsNotesDuration: " <<
      elt->getDoubleTremoloElementsNotesDuration () <<
      std::endl <<

      "% doubleTremoloMarksNumber: " <<
      elt->getDoubleTremoloMarksNumber () <<
      std::endl <<

      "% numberOfRepeats: \"" <<
      numberOfRepeats <<
      std::endl;

    --gIndenter;
  }
#endif // MF_TRACE_IS_ENABLED

  fLilypondCodeStream <<
    "\\repeat tremolo " << numberOfRepeats << " {";

  ++gIndenter; // decremented in visitEnd (S_msrDoubleTremolo& elt)
}

void lpsr2lilypondTranslator::visitEnd (S_msrDoubleTremolo& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream ss;

      ss <<
        "% --> End visiting msrDoubleTremolo" <<
        ", line " << elt->getInputLineNumber () <<
      std::endl;

      if (traceLpsrVisitors) {
        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << ss.str ();
      }
    }
  }
#endif // MF_TRACE_IS_ENABLED

  --gIndenter; // incremented in visitStart (S_msrDoubleTremolo& elt)

  fLilypondCodeStream <<
    '}' <<
    std::endl;
}

//________________________________________________________________________
void lpsr2lilypondTranslator::visitStart (S_msrDynamic& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream ss;

      ss <<
        "% --> Start visiting msrDynamic" <<
        ", line " << elt->getInputLineNumber () <<
      std::endl;

      if (traceLpsrVisitors) {
        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << ss.str ();
      }
    }
  }
#endif // MF_TRACE_IS_ENABLED
}

void lpsr2lilypondTranslator::visitEnd (S_msrDynamic& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream ss;

      ss <<
        "% --> End visiting msrDynamic" <<
        ", line " << elt->getInputLineNumber () <<
      std::endl;

      if (traceLpsrVisitors) {
        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << ss.str ();
      }
    }
  }
#endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
void lpsr2lilypondTranslator::visitStart (S_msrOtherDynamic& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream ss;

      ss <<
        "% --> Start visiting msrOtherDynamic" <<
        ", line " << elt->getInputLineNumber () <<
      std::endl;

      if (traceLpsrVisitors) {
        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << ss.str ();
      }
    }
  }
#endif // MF_TRACE_IS_ENABLED
}

void lpsr2lilypondTranslator::visitEnd (S_msrOtherDynamic& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream ss;

      ss <<
        "% --> End visiting msrOtherDynamic" <<
        ", line " << elt->getInputLineNumber () <<
      std::endl;

      if (traceLpsrVisitors) {
        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << ss.str ();
      }
    }
  }
#endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
void lpsr2lilypondTranslator::visitStart (S_msrWords& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream ss;

      ss <<
        "% --> Start visiting msrWords" <<
        ", line " << elt->getInputLineNumber () <<
      std::endl;

      if (traceLpsrVisitors) {
        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << ss.str ();
      }
    }
  }
#endif // MF_TRACE_IS_ENABLED
}

void lpsr2lilypondTranslator::visitEnd (S_msrWords& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream ss;

      ss <<
        "% --> End visiting msrWords" <<
        ", line " << elt->getInputLineNumber () <<
      std::endl;

      if (traceLpsrVisitors) {
        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << ss.str ();
      }
    }
  }
#endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
void lpsr2lilypondTranslator::visitStart (S_msrSlur& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream ss;

      ss <<
        "% --> Start visiting msrSlur " <<
        elt->asShortString () <<
        ", line " << elt->getInputLineNumber () <<
      std::endl;

      if (traceLpsrVisitors) {
        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << ss.str ();
      }
    }
  }
#endif // MF_TRACE_IS_ENABLED
}

void lpsr2lilypondTranslator::visitEnd (S_msrSlur& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream ss;

      ss <<
        "% --> End visiting msrSlur " <<
        elt->asShortString () <<
        ", line " << elt->getInputLineNumber () <<
      std::endl;

      if (traceLpsrVisitors) {
        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << ss.str ();
      }
    }
  }
#endif // MF_TRACE_IS_ENABLED
}

/*
void lpsr2lilypondTranslator::visitStart (S_msrChordSlurLink& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream ss;

      ss <<
        "% --> Start visiting msrChordSlurLink " <<
        elt->asShortString () <<
        ", line " << elt->getInputLineNumber () <<
      std::endl;

      if (traceLpsrVisitors) {
        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << ss.str ();
      }
    }
  }
#endif // MF_TRACE_IS_ENABLED
}

void lpsr2lilypondTranslator::visitEnd (S_msrChordSlurLink& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream ss;

      ss <<
        "% --> End visiting msrChordSlurLink " <<
        elt->asShortString () <<
        ", line " << elt->getInputLineNumber () <<
      std::endl;

      if (traceLpsrVisitors) {
        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << ss.str ();
      }
    }
  }
#endif // MF_TRACE_IS_ENABLED
}
*/

//________________________________________________________________________
void lpsr2lilypondTranslator::visitStart (S_msrLigature& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream ss;

      ss <<
        "% --> Start visiting msrLigature" <<
        ", line " << elt->getInputLineNumber () <<
      std::endl;

      if (traceLpsrVisitors) {
        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << ss.str ();
      }
    }
  }
#endif // MF_TRACE_IS_ENABLED
}

void lpsr2lilypondTranslator::visitEnd (S_msrLigature& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream ss;

      ss <<
        "% --> End visiting msrLigature" <<
        ", line " << elt->getInputLineNumber () <<
      std::endl;

      if (traceLpsrVisitors) {
        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << ss.str ();
      }
    }
  }
#endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
void lpsr2lilypondTranslator::visitStart (S_msrCrescDecresc& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream ss;

      ss <<
        "% --> Start visiting msrCrescDecresc" <<
        ", line " << elt->getInputLineNumber () <<
      std::endl;

      if (traceLpsrVisitors) {
        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << ss.str ();
      }
    }
  }
#endif // MF_TRACE_IS_ENABLED

  switch (elt->getCrescDecrescKind ()) {
    case msrCrescDecrescKind::kCrescDecrescCrescendo:
      fLilypondCodeStream <<
        "\\cresc ";
      break;
    case msrCrescDecrescKind::kCrescDecrescDecrescendo:
      fLilypondCodeStream <<
        "\\decresc ";
      break;
  } // switch
}

void lpsr2lilypondTranslator::visitEnd (S_msrCrescDecresc& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream ss;

      ss <<
        "% --> End visiting msrCrescDecresc" <<
        ", line " << elt->getInputLineNumber () <<
      std::endl;

      if (traceLpsrVisitors) {
        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << ss.str ();
      }
    }
  }
#endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
void lpsr2lilypondTranslator::visitStart (S_msrWedge& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream ss;

      ss <<
        "% --> Start visiting msrWedge" <<
        ", line " << elt->getInputLineNumber () <<
      std::endl;

      if (traceLpsrVisitors) {
        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << ss.str ();
      }
    }
  }
#endif // MF_TRACE_IS_ENABLED
}

void lpsr2lilypondTranslator::visitEnd (S_msrWedge& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream ss;

      ss <<
        "% --> End visiting msrWedge" <<
        ", line " << elt->getInputLineNumber () <<
      std::endl;

      if (traceLpsrVisitors) {
        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << ss.str ();
      }
    }
  }
#endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
void lpsr2lilypondTranslator::generateNoteBeamsAfterNote (
  const S_msrNote& note)
{
  const std::list <S_msrBeam>&
    noteBeamsList =
      note->getNoteBeamsList ();

  if (! noteBeamsList.empty ()) {
    for (S_msrBeam beam : noteBeamsList) {
 #ifdef MF_TRACE_IS_ENABLED
      if (gTraceOahGroup->getTraceBeams ()) {
        std::stringstream ss;

        ss <<
          "Considering to generate LilyPond code for beam " <<
          beam->asShortString () <<
          " in note " <<
          note->asShortString ();

        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }
#endif // MF_TRACE_IS_ENABLED

      // in MusicXML, the beam number is the number of simultaneous beams,
      // from 1 to 8, i.e. from 8th to 1024th notes

      // in LilyPond, the '[' follows the first beamed note
      // and the ']' follows the last beamed one

      // LilyPond will take care of multiple beams automatically,
      // so we need only generate the first number (level) JMI 0.9.72

/*
      <!--
        Beam types include begin, continue, end, forward hook, and
        backward hook. Up to eight concurrent beams are available to
        cover up to 1024th notes, using an enumerated type defined
        in the common.mod file. Each beam in a note is represented
        with a separate beam element, starting with the eighth note
        beam using a number attribute of 1.

        Note that the beam number does not distinguish sets of
        beams that overlap, as it does for slur and other elements.
        Beaming groups are distinguished by being in different
        voices and/or the presence or absence of grace and cue
        elements.

        Beams that have a begin value can also have a fan attribute to
        indicate accelerandos and ritardandos using fanned beams. The
        fan attribute may also be used with a continue value if the
        fanning direction changes on that note. The value is "none"
        if not specified.

        The repeater attribute has been deprecated in MusicXML 3.0.
        Formerly used for tremolos, it needs to be specified with a
        "yes" value for each beam using it.
      -->
      <!ELEMENT beam (#PCDATA)>
      <!ATTLIST beam
          number %beam-level; "1"
          repeater %yes-no; #IMPLIED
          fan (accel | rit | none) #IMPLIED
          %color;
      >
*/

      mfInputLineNumber inputStartLineNumber = beam->getInputLineNumber ();

      int beamNumber = beam->getBeamNumber ();

      msrBeamKind beamKind = beam->getBeamKind ();

#ifdef MF_TRACE_IS_ENABLED
      if (gTraceOahGroup->getTraceBeams ()) {
        std::stringstream ss;

        ss <<
          "% --> generateNoteBeamsAfterNote()" <<
          ", beamNumber: " << beamNumber <<
          ", beamKind: " << beamKind <<
          ", line " << inputStartLineNumber ;

        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }
#endif // MF_TRACE_IS_ENABLED

      switch (beamKind) {

        case msrBeamKind::kBeamBegin:
          // ------------------------------------------
          // beam begin
          // ------------------------------------------

          if (! gGlobalLpsr2lilypondOahGroup->getNoBeams ()) {
            if (beamNumber == 1) {
              fLilypondCodeStream <<
                "[ ";
              if (gGlobalLpsr2lilypondOahGroup->getCommentLilypondBeams ()) {
                fLilypondCodeStream <<
                  " %{ beam " <<
                  beamNumber <<
                  ", line " <<
                  inputStartLineNumber <<
                  " %}";
              }
              fLilypondCodeStream <<
                std::endl;
            }
          }

#ifdef MF_MAINTAINANCE_RUNS_ARE_ENABLED
          if (gWaeOahGroup->getMaintainanceRun ()) { // MAINTAINANCE_RUN
            fLilypondCodeStream << "%{ kBeamBegin 2 %} ";
          }
#endif // MF_MAINTAINANCE_RUNS_ARE_ENABLED

          if (gGlobalLpsr2lilypondOahGroup->getInputLineNumbers ()) {
            // generate the input line number as a comment
            fLilypondCodeStream <<
              " %{ <-- line " <<
              inputStartLineNumber <<
              " %} ";
          }
          break;

        case msrBeamKind::kBeamContinue:
          // ------------------------------------------
          // beam continue
          // ------------------------------------------
          break;

        case msrBeamKind::kBeamEnd:
          // ------------------------------------------
           // beam end
          // ------------------------------------------

          if (! gGlobalLpsr2lilypondOahGroup->getNoBeams ()) {
            if (beamNumber == 1) {
              fLilypondCodeStream <<
                "] ";
              if (gGlobalLpsr2lilypondOahGroup->getCommentLilypondBeams ()) {
                fLilypondCodeStream <<
                  " %{ beam " <<
                  beamNumber <<
                  ", line " <<
                  inputStartLineNumber <<
                  " %}";
              }
              fLilypondCodeStream <<
                std::endl;
            }

// #ifdef MF_MAINTAINANCE_RUNS_ARE_ENABLED
//             if (gWaeOahGroup->getMaintainanceRun ()) { // MAINTAINANCE_RUN
//               fLilypondCodeStream << "%{ kBeamEnd 1 %} ";
//             }
// #endif // MF_MAINTAINANCE_RUNS_ARE_ENABLED

//             if (gGlobalLpsr2lilypondOahGroup->getInputLineNumbers ()) {
//               // generate the input line number as a comment
//               fLilypondCodeStream <<
//                   " %{ <-- line " <<
//                   beam->getInputLineNumber () <<
//                   " %} ";
//             }
          }

// #ifdef MF_MAINTAINANCE_RUNS_ARE_ENABLED
//           if (gWaeOahGroup->getMaintainanceRun ()) { // MAINTAINANCE_RUN
//             fLilypondCodeStream << "%{ kBeamEnd 2 %} ";
//           }
// #endif // MF_MAINTAINANCE_RUNS_ARE_ENABLED

//           if (gGlobalLpsr2lilypondOahGroup->getInputLineNumbers ()) {
//             // generate the input line number as a comment
//             fLilypondCodeStream <<
//                   " %{ <-- line " <<
//                   beam->getInputLineNumber () <<
//                   " %} ";
//           }
          break;

        case msrBeamKind::kBeamForwardHook:
          // JMI ???
          break;

        case msrBeamKind::kBeamBackwardHook:
          // JMI ???
          break;

        case msrBeamKind::kBeam_UNKNOWN_:
           // JMI ???
         break;
      } // switch
    } // for
  }
}

//________________________________________________________________________
void lpsr2lilypondTranslator::generateNoteSlurDirection (
  const S_msrNote& note)
{
  const std::list <S_msrSlur>&
    noteSlursList =
      note->getNoteSlursList ();

  if (! noteSlursList.empty ()) {
    std::list <S_msrSlur>::const_iterator i;

    for (S_msrSlur slur : noteSlursList) {
 #ifdef MF_TRACE_IS_ENABLED
      if (gTraceOahGroup->getTraceSlurs ()) {
        std::stringstream ss;

        ss <<
          "Considering to generate LilyPond code for slur direction " <<
          slur->asShortString () <<
          " for note " <<
          note->asShortString () <<
          ", line " << slur->getInputLineNumber ();

        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }
#endif // MF_TRACE_IS_ENABLED

      // should a stem direction command be generated?
      // otherwise, leave stems management to LilyPond
      msrPlacementKind
        slurPlacementKind =
          slur->getSlurPlacementKind ();

      Bool doGenerateASlurDirection (false);

      if (
        gGlobalLpsr2lilypondOahGroup->getGenerateSlursDirections ()
      ) {
        doGenerateASlurDirection = true;
      }
      else {
        doGenerateASlurDirection =
          slurPlacementKind != fCurrentSlurPlacementKind;
      }

#ifdef MF_TRACE_IS_ENABLED
      if (gTraceOahGroup->getTraceSlurs ()) {
        std::stringstream ss;

        ss <<
          "--> generateNoteSlurDirection() 2" <<
          ", slur: " <<
          slur->asShortString () <<
          ", doGenerateAStemDirection: " <<
          doGenerateASlurDirection;

        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }
#endif // MF_TRACE_IS_ENABLED

      if (doGenerateASlurDirection) {
        // JMI msrPlacementKind::kPlacement_UNKNOWN_ ??? JMI 0.9.72
  #ifdef MF_TRACE_IS_ENABLED
        if (gTraceOahGroup->getTraceSlurs ()) {
          std::stringstream ss;

          ss <<
            "--> fCurrentSlurPlacementKind switches from " <<
            fCurrentSlurPlacementKind <<
            " to " <<
            slurPlacementKind <<
            ", line " << slur->getInputLineNumber ();

          gWaeHandler->waeTrace (
            __FILE__, mfInputLineNumber (__LINE__),
            ss.str ());
        }
  #endif // MF_TRACE_IS_ENABLED

        switch (slurPlacementKind) {
          case msrPlacementKind::kPlacement_UNKNOWN_:
            break;
          case msrPlacementKind::kPlacementAbove:
            fLilypondCodeStream << "\\slurUp ";
            break;
          case msrPlacementKind::kPlacementBelow:
            fLilypondCodeStream << "\\slurDown ";
            break;
        } // switch
      }

      // update fCurrentSlurPlacementKind upon each slur occurrence
      fCurrentSlurPlacementKind = slurPlacementKind;
    } // for
  }
}

//________________________________________________________________________
void lpsr2lilypondTranslator::generateNoteSlursList (
  const S_msrNote& note)
{
  const std::list <S_msrSlur>&
    noteSlursList =
      note->getNoteSlursList ();

  size_t
    noteSlursListSize =
      noteSlursList.size ();

  int
    noteSlurStartsNumber =
      note->fetchNoteSlurStartsNumber (),
    noteSlurStopsNumber =
      note->fetchNoteSlurStopsNumber ();

  if (noteSlursListSize) {
    std::list <S_msrSlur>::const_iterator i;

    for (S_msrSlur slur : noteSlursList) {
 #ifdef MF_TRACE_IS_ENABLED
      if (gTraceOahGroup->getTraceSlurs ()) {
        std::stringstream ss;

        ss <<
          "Considering to generate LilyPond code for slur " <<
          slur->asShortString () <<
          " for note " <<
          note->asShortString () <<
          ", noteSlurStartsNumber: " << noteSlurStartsNumber <<
          ", noteSlurStopsNumber: " << noteSlurStopsNumber <<
          ", line " << slur->getInputLineNumber ();

        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }
#endif // MF_TRACE_IS_ENABLED

      /* JMI ???
        \slurDashed, \slurDotted, \slurHalfDashed,
        \slurHalfSolid, \slurDashPattern, \slurSolid
      */

      // generate the slur, leveraging the LilyPond 2.24 and above
      // labelled multiple slurs matching
      switch (slur->getSlurTypeKind ()) {
        case msrSlurTypeKind::kSlurType_UNKNOWN_:
          break;

        case msrSlurTypeKind::kSlurTypeRegularStart:
#ifdef MF_TRACE_IS_ENABLED
          if (gTraceOahGroup->getTraceSlurs ()) {
            std::stringstream ss;

            ss <<
              "Generating LilyPond code for slur regular start " <<
              slur->asShortString () <<
              " in note " <<
              note->asShortString () <<
              ", line " << slur->getInputLineNumber ();

            gWaeHandler->waeTrace (
              __FILE__, mfInputLineNumber (__LINE__),
              ss.str ());
          }
#endif // MF_TRACE_IS_ENABLED

//           gLog << note << std::endl;

          switch (note->getNoteKind ()) {
            case msrNoteKind::kNote_UNKNOWN_:
              break;

            // in grace notes groups
            case msrNoteKind::kNoteRegularInGraceNotesGroup:
              // let LilyPond take care of the slur JMI 0.9.74
              fLilypondWillHandleTheNextSlurCloser = true;
              break;

            default:
              if (noteSlurStartsNumber != 1) {
                fLilypondCodeStream <<
                  cLilypondPhrasingContinuer <<
                  slur->getSlurNumber (); // JMI 0.9.68
              }
              fLilypondCodeStream << cLilypondSlurOpener;

              if (gGlobalLpsr2lilypondOahGroup->getInputLineNumbers ()) {
                // generate the input line number as a comment
                fLilypondCodeStream <<
                  " %{ <-- line " <<
                  slur->getInputLineNumber () <<
                  " AAAAAAA %} ";
              }
          } // switch
          break;

        case msrSlurTypeKind::kSlurTypeRegularContinue:
          // nothing to do
          break;

        case msrSlurTypeKind::kSlurTypeRegularStop:
#ifdef MF_TRACE_IS_ENABLED
          if (gTraceOahGroup->getTraceSlurs ()) {
            std::stringstream ss;

            ss <<
              "Generating LilyPond code for slur regular continue " <<
              slur->asShortString () <<
              " in note " <<
              note->asShortString () <<
              ", line " << slur->getInputLineNumber ();

            gWaeHandler->waeTrace (
              __FILE__, mfInputLineNumber (__LINE__),
              ss.str ());
          }
#endif // MF_TRACE_IS_ENABLED

//           switch (note->getNoteKind ()) {
//             case msrNoteKind::kNote_UNKNOWN_:
//               break;
//
//             // in grace notes groups
//             case msrNoteKind::kNoteRegularInGraceNotesGroup:
//               // let LilyPond take care of the slur JMI 0.9.74
//               break;
//
//             default:
              if (fLilypondWillHandleTheNextSlurCloser) {
                // let LilyPond take care of the slur JMI 0.9.74
                fLilypondWillHandleTheNextSlurCloser = false;

                // there is one slur stopper to be handled
                noteSlurStopsNumber -= 1; // JMI 0.9.74
              }
              else {
                if (noteSlurStopsNumber != 1) {
                  fLilypondCodeStream <<
                    cLilypondSlurContinuer <<
                    slur->getSlurNumber ();
                }
                fLilypondCodeStream << cLilypondSlurCloser;

                if (gGlobalLpsr2lilypondOahGroup->getInputLineNumbers ()) {
                  // generate the input line number as a comment
                  fLilypondCodeStream <<
                    " %{ <-- line " <<
                    slur->getInputLineNumber () <<
                    " BBBBBBB %} ";
                }
              }
//           } // switch

          switch (slur->getSlurPlacementKind ()) {
            case msrPlacementKind::kPlacement_UNKNOWN_:
              break;
            case msrPlacementKind::kPlacementAbove:
            case msrPlacementKind::kPlacementBelow:
              fLilypondCodeStream << "\\slurNeutral ";
              break;
          } // switch
          break;

        case msrSlurTypeKind::kSlurTypePhrasingStart:
#ifdef MF_TRACE_IS_ENABLED
          if (gTraceOahGroup->getTraceSlurs ()) {
            std::stringstream ss;

            ss <<
              "Generating LilyPond code for slur phrasing start " <<
              slur->asShortString () <<
              " in note " <<
              note->asShortString ();

            gWaeHandler->waeTrace (
              __FILE__, mfInputLineNumber (__LINE__),
              ss.str ());
          }
#endif // MF_TRACE_IS_ENABLED

          fLilypondCodeStream << cLilypondPhrasingOpener;

          if (gGlobalLpsr2lilypondOahGroup->getInputLineNumbers ()) {
            // generate the input line number as a comment
            fLilypondCodeStream <<
              " %{ <-- line " <<
              slur->getInputLineNumber () <<
              " CC %} ";
          }
          break;

        case msrSlurTypeKind::kSlurTypePhrasingContinue:
          // nothing to do
          break;

        case msrSlurTypeKind::kSlurTypePhrasingStop:
#ifdef MF_TRACE_IS_ENABLED
          if (gTraceOahGroup->getTraceSlurs ()) {
            std::stringstream ss;

            ss <<
              "Generating LilyPond code for slur phrasing stop " <<
              slur->asShortString () <<
              " in note " <<
              note->asShortString () <<
              std::endl;

            gWaeHandler->waeTrace (
              __FILE__, mfInputLineNumber (__LINE__),
              ss.str ());
          }
#endif // MF_TRACE_IS_ENABLED

          fLilypondCodeStream << cLilypondPhrasingCloser;

          if (gGlobalLpsr2lilypondOahGroup->getInputLineNumbers ()) {
            // generate the input line number as a comment
            fLilypondCodeStream <<
              " %{ <-- line " <<
              slur->getInputLineNumber () <<
              "DD %} ";
          }
          break;
      } // switch
    } // for
  }
}

//________________________________________________________________________
void lpsr2lilypondTranslator::generateGraceNotesGroup (
  const S_msrGraceNotesGroup& graceNotesGroup)
{
  /*
    1. no slash, no slur: \grace
    2. slash and slur:    \acciaccatura, LilyPond will slur it // JMI 0.9.74
    3. slash but no slur: \slashedGrace
    4. no slash but slur: \appoggiatura, LilyPond will slur it // JMI 0.9.74
  */

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceGraceNotes ()) {
    fLilypondCodeStream <<
      std::endl <<
      "% --> generating code for grace notes group " <<
      graceNotesGroup->asString () <<
      ", line " << graceNotesGroup->getInputLineNumber () <<
      std::endl;
  }
#endif // MF_TRACE_IS_ENABLED

  msrGraceNotesGroupKind
    graceNotesGroupKind =
      graceNotesGroup->getGraceNotesGroupKind ();

  Bool
    graceNotesGroupIsSlashed =
      graceNotesGroup->getGraceNotesGroupIsSlashed (),
    graceNotesGroupIsBeamed =
      graceNotesGroup->getGraceNotesGroupIsBeamed (),

    graceNotesGroupIsTied =
      graceNotesGroup->getGraceNotesGroupIsTied (),
    graceNotesGroupIsSlurred =
      graceNotesGroup->getGraceNotesGroupIsSlurred ();

#ifdef MF_TRACE_IS_ENABLED
  if ( gTraceOahGroup->getTraceGraceNotes ()) {
    std::stringstream ss;

    ss <<
      "% --> generateGraceNotesGroup()" << // JMI BOF
      ", graceNotesGroupKind: " << graceNotesGroupKind <<
      ", graceNotesGroupIsSlashed: " << graceNotesGroupIsSlashed <<
      ", graceNotesGroupIsBeamed: " << graceNotesGroupIsBeamed <<
      ", graceNotesGroupIsTied: " << graceNotesGroupIsTied <<
      ", graceNotesGroupIsSlurred: " << graceNotesGroupIsSlurred;

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  Bool doGenerateASlurIfPresent (false);

  switch (graceNotesGroupKind) {
    case msrGraceNotesGroupKind::kGraceNotesGroupBefore:
      if (graceNotesGroupIsBeamed) {
        // JMI ??? 0.9.70
      }

      if (graceNotesGroupIsSlashed) {
        if (graceNotesGroupIsTied || graceNotesGroupIsSlurred) {
          fLilypondCodeStream <<
            "\\acciaccatura";
            /* JMI NO see :
\version "2.23.3"

% thanks to ksnortum@gmail.com and lfm@gmx.de

\language english

% This is controlled by the stroke-style property of the Flag:

slash = \tweak Flag.stroke-style grace \etc

\relative c
{
  \clef bass
  <<
    {
      <b' fs>2 \afterGrace <as e>2 \slash <as e>8( |
      <fs d>1)
    }
    \\
    {
      <d b>2 \afterGrace <cs fs,> \slash <cs fs,>8( |
      <b d,>4)( <as cs,>4 <fs b,>2)
    }
  >>
}

% I removed the 3/4, as this is the standard anyway as long as you don't modify afterGraceFraction.

% original:
\relative c
{
  \clef bass
  <<
    { <b' fs>2 \afterGrace 3/4 <as e>2  <as e>8( |  <fs d>1) }
    \\
    {<d b>2  \afterGrace 3/4 <cs fs,>  <cs fs,>8( | <b d,>4)( <as cs,>4 <fs b,>2) }
  >>
}
            */
        }
        else {
          fLilypondCodeStream <<
            "\\slashedGrace";
          doGenerateASlurIfPresent = true;
        }
      }

      else {
        if (graceNotesGroupIsTied || graceNotesGroupIsSlurred) {
          fLilypondCodeStream <<
            "\\appoggiatura";
        }
        else {
          fLilypondCodeStream <<
            "\\grace";
          doGenerateASlurIfPresent = true;
        }
      }
      break;

    case msrGraceNotesGroupKind::kGraceNotesGroupAfter:
      // don't generate anything here
      break;
  } // switch

  fLilypondCodeStream <<
    cLilypondGraceNotesGroupOpener;

  // force durations to be displayed explicitly
  // at the beginning of the grace notes
  fLastMetWholeNotes = K_WHOLE_NOTES_UNKNOWN_;

  // generate the notes in the grace notes group
  const std::list <S_msrMeasureElement>&
    graceNotesGroupElementsList =
      graceNotesGroup->
        getGraceNotesGroupElementsList ();

  size_t
    graceNotesGroupElementsListSize =
      graceNotesGroupElementsList.size ();

  if (graceNotesGroupElementsListSize) {
    std::list <S_msrMeasureElement>::const_iterator
      iBegin = graceNotesGroupElementsList.begin (),
      iEnd   = graceNotesGroupElementsList.end (),
      i      = iBegin;

    size_t elementNumber = 0;

    for ( ; ; ) {
      S_msrElement element = (*i);

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
      // sanity check
      mfAssert (
        __FILE__, mfInputLineNumber (__LINE__),
        element != nullptr,
        "element is NULL");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

      elementNumber += 1;

      if (
        // note?
        S_msrNote
          graceNotesGroupNote =
            dynamic_cast<msrNote*>(&(*element))
      ) {
        // generate things before the note
        generateCodeRightBeforeNote (graceNotesGroupNote);

        // generate the graceNotesGroupNote itself
        generateTheNoteItself (graceNotesGroupNote);

        // generate the input line number as comment if relevant
        if ( // JMI
          gGlobalLpsr2lilypondOahGroup->getInputLineNumbers ()
            ||
          gGlobalLpsr2lilypondOahGroup->getGeneratePositionInMeasures ()
        ) {
          generateInputLineNumberAndOrPositionInMeasureAsAComment (
            graceNotesGroupNote);
        }

        // generate things after the graceNotesGroupNote
        generateCodeRightAfterNote (graceNotesGroupNote);

        // generate the graceNotesGroupNote beams if any,
        // unless the graceNotesGroupNote is chord member
        if (! graceNotesGroupNote->getNoteBelongsToAChord ()) {
//             generateNoteBeamsAfterNote (graceNotesGroupNote); // JMI fixes [ [ ... ] ] issue 0.9.70
        }

        if (graceNotesGroupIsBeamed) { // JMI fixed [ ] issue 0.9.70
          if (elementNumber == 1) {
            fLilypondCodeStream << cLilypondBeamsOpener;
          }
          else if (elementNumber == graceNotesGroupElementsListSize) {
            fLilypondCodeStream << cLilypondBeamsCloser;
          }

          if (gGlobalLpsr2lilypondOahGroup->getCommentLilypondGraceNotes ()) {
            fLilypondCodeStream <<
              " %{ line " <<
              graceNotesGroupNote->getInputLineNumber () <<
              ", elementNumber: " <<
              elementNumber <<
              " %}" <<
              std::endl; // JMI 0.9.70
          }
        }

        // generate the graceNotesGroupNote slurs if any,
        // unless the graceNotesGroupNote is chord member
        // or LilyPond will take care of that

        // see gracenotes/SlurredNoteWithGraceNotes.xml for
        // extraneous '( ' JMI
//          if (false && // JMI
        if (false &&
          ! graceNotesGroupNote->getNoteBelongsToAChord ()
            &&
          doGenerateASlurIfPresent
        ) {
          generateNoteSlursList (graceNotesGroupNote);
        }
      }

      else if (
        // chord?
        S_msrChord
          chord =
            dynamic_cast<msrChord*>(&(*element))
        ) {
//           generateChordInGraceNotesGroup (chord);
        }

      else {
        std::stringstream ss;

        fLilypondCodeStream <<
          "grace notes group elements list in '" <<
          graceNotesGroup->asString () <<
          "' is empty" <<
          ", line " << graceNotesGroup->getInputLineNumber ();

        lpsr2lilypondInternalError (
          gServiceRunData->getInputSourceName (),
          graceNotesGroup->getInputLineNumber (),
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }

      if (++i == iEnd) break;
      fLilypondCodeStream <<
        cLilyPondSpace;
    } // for

    fLilypondCodeStream << cLilypondGraceNotesGroupCloser;
  }

  else {
    std::stringstream ss;

    fLilypondCodeStream <<
      "grace notes group elements list in '" <<
      graceNotesGroup->asString () <<
      "' is empty" <<
      ", line " << graceNotesGroup->getInputLineNumber ();

    lpsr2lilypondInternalError (
      gServiceRunData->getInputSourceName (),
      graceNotesGroup->getInputLineNumber (),
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }

  // force durations to be displayed explicitly
  // at the end of the grace notes
  fLastMetWholeNotes = K_WHOLE_NOTES_UNKNOWN_;
}

//________________________________________________________________________
void lpsr2lilypondTranslator::visitStart (S_msrGraceNotesGroup& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream ss;

      ss <<
        "% --> Start visiting msrGraceNotesGroup " <<
        elt->asShortString () <<
        ", fOnGoingChord: " <<
        fOnGoingChord <<
        ", fOnGoingGraceNotesGroup: " <<
        fOnGoingGraceNotesGroup <<
        ", fOnGoingChordGraceNotesGroupLink: " <<
        fOnGoingChordGraceNotesGroupLink <<
        ", line " << elt->getInputLineNumber () <<
      std::endl;

      if (traceLpsrVisitors) {
        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << ss.str ();
      }
    }
  }
#endif // MF_TRACE_IS_ENABLED

  fCurrentGraceNotesGroup = elt;

  fOnGoingGraceNotesGroup = true;
}

void lpsr2lilypondTranslator::visitEnd (S_msrGraceNotesGroup& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream ss;

      ss <<
        "% --> End visiting msrGraceNotesGroup " <<
        elt->asShortString () <<
        ", line " << elt->getInputLineNumber () <<
      std::endl;

      if (traceLpsrVisitors) {
        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << ss.str ();
      }
    }
  }
#endif // MF_TRACE_IS_ENABLED

  // forget about current grace notes group
  fCurrentGraceNotesGroup = nullptr;

  fOnGoingGraceNotesGroup = false;
}

//________________________________________________________________________
void lpsr2lilypondTranslator::visitStart (S_msrChordGraceNotesGroupLink& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream ss;

      ss <<
        "% --> Start visiting msrChordGraceNotesGroupLink " <<
        elt->asShortString () <<
        ", line " << elt->getInputLineNumber () <<
      std::endl;

      if (traceLpsrVisitors) {
        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << ss.str ();
      }
    }
  }
#endif // MF_TRACE_IS_ENABLED

  fOnGoingChordGraceNotesGroupLink = true;
}

void lpsr2lilypondTranslator::visitEnd (S_msrChordGraceNotesGroupLink& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream ss;

      ss <<
        "% --> End visiting msrChordGraceNotesGroupLink " <<
        elt->asShortString () <<
        ", line " << elt->getInputLineNumber () <<
      std::endl;

      if (traceLpsrVisitors) {
        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << ss.str ();
      }
    }
  }
#endif // MF_TRACE_IS_ENABLED

  fOnGoingChordGraceNotesGroupLink = false;
}

//________________________________________________________________________
void lpsr2lilypondTranslator::visitStart (S_msrAfterGraceNotesGroup& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream ss;

      ss <<
        "% --> Start visiting msrAfterGraceNotesGroup " <<
        elt->asShortString () <<
        ", line " << elt->getInputLineNumber () <<
      std::endl;

      if (traceLpsrVisitors) {
        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << ss.str ();
      }
    }
  }
#endif // MF_TRACE_IS_ENABLED

 // JMI exists? if (elt->getGraceNotesGroupIsSlashed ()) {}
  fLilypondCodeStream <<
    "\\afterGrace { ";
  if (gGlobalLpsr2lilypondOahGroup->getCommentLilypondGraceNotes ()) {
    fLilypondCodeStream <<
      "%{ visitStart (S_msrAfterGraceNotesGroup&) %} ";
  }
}

void lpsr2lilypondTranslator::visitStart (S_msrAfterGraceNotesGroupContents& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream ss;

      ss <<
        "% --> Start visiting msrAfterGraceNotesGroupContents " <<
        elt->asShortString () <<
        ", line " << elt->getInputLineNumber () <<
      std::endl;

      if (traceLpsrVisitors) {
        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << ss.str ();
      }
    }
  }
#endif // MF_TRACE_IS_ENABLED

  // write a first closing right bracket right now
  fLilypondCodeStream <<
    "} { ";

  // force durations to be displayed explicitly
  // at the beginning of the after grace notes contents
  fLastMetWholeNotes = K_WHOLE_NOTES_UNKNOWN_;
}

void lpsr2lilypondTranslator::visitEnd (S_msrAfterGraceNotesGroupContents& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream ss;

      ss <<
        "% --> End visiting msrAfterGraceNotesGroupContents " <<
        elt->asShortString () <<
        ", line " << elt->getInputLineNumber () <<
      std::endl;

      if (traceLpsrVisitors) {
        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << ss.str ();
      }
    }
  }
#endif // MF_TRACE_IS_ENABLED

  fLilypondCodeStream <<
    "} ";
}

void lpsr2lilypondTranslator::visitEnd (S_msrAfterGraceNotesGroup& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream ss;

      ss <<
        "% --> End visiting msrAfterGraceNotesGroup " <<
        elt->asShortString () <<
        ", line " << elt->getInputLineNumber () <<
      std::endl;

      if (traceLpsrVisitors) {
        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << ss.str ();
      }
    }
  }
#endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
void lpsr2lilypondTranslator::generateBeforeNoteSpannersListIfAny (
  const S_msrNote& note)
{
  const std::list <S_msrSpanner>&
    noteSpannersList =
      note->getNoteSpannersList ();

  if (! noteSpannersList.empty ()) {
    for (S_msrSpanner spanner : noteSpannersList) {
      generateSpannerBeforeNote (
        spanner,
        note);
    } // for
  }
}

//________________________________________________________________________
void lpsr2lilypondTranslator::generateAfterNoteSpannersListIfAny (
  const S_msrNote& note)
{
  const std::list <S_msrSpanner>&
    noteSpannersList =
      note->getNoteSpannersList ();

  if (! noteSpannersList.empty ()) {
    for (S_msrSpanner spanner : noteSpannersList) {
      generateSpannerAfterNote (
        spanner,
        note);
    } // for
  }
}

//________________________________________________________________________
void lpsr2lilypondTranslator::generateStuffBeforeTheNoteIself (
  const S_msrNote& note)
{
  // generate the note scordaturas if any
  const std::list <S_msrScordatura>&
    noteScordaturasList =
      note->getNoteScordaturasList ();

  if (! noteScordaturasList.empty ()) {
    generateNoteScordaturasList (note);
  }

  // should the note actually be printed? // JMI
  msrPrintObjectKind
    notePrintObjectKind =
      note->getNotePrintObjectKind ();

  if (notePrintObjectKind != fCurrentNotePrinObjectKind) {
    generateNoteHeadAndStem (note);
  }

  // generate the note slashes if any
  const std::list <S_msrSlash>&
    noteSlashesList =
      note->getNoteSlashesList ();

  if (! noteSlashesList.empty ()) {
    generateNoteSlashesList (note);
  }

  // generate the note wedges circled tips if any
  const std::list <S_msrWedge>&
    noteWedgesList =
      note->getNoteWedgesList ();

  if (! noteWedgesList.empty ()) {
    generateNoteWedgesList (note);
  }

  // generate the note slurs line types if any,
  // unless the note is chord member
  if (! note->getNoteBelongsToAChord ()) {
    const std::list <S_msrSlur>&
      noteSlursList =
        note->getNoteSlursList ();

    if (! noteSlursList.empty ()) {
      generateNoteSlurLineTypes (note);
    }
  }


  // get the note glissandos list
  const std::list <S_msrGlissando>&
    noteGlissandosList =
      note->getNoteGlissandosList ();

  // generate the note glissandos if any, BEFORE the note glissando styles
  if (! noteGlissandosList.empty ()) {
    generateNoteGlissandos (
      noteGlissandosList);
  }

  // generate the note glissandos styles if any, AFTER the note glissandos
  if (! noteGlissandosList.empty ()) {
    generateNoteGlissandoStyles (note);
  }

  // generate the note glissandos with text if any
  // just got noteGlissandosList above
  if (! noteGlissandosList.empty ()) {
    generateNoteGlissandosListWithText (note);
  }


  // generate the note slides line styles if any, implemented as glissandos
  const std::list <S_msrSlide>&
    noteSlidesList =
      note->getNoteSlidesList ();

  if (! noteSlidesList.empty ()) {
    generateNoteSlideLineStyles (note);
  }

  // generate the note slides with text if any
  // just got noteSlidesList above
  if (! noteSlidesList.empty ()) {
    generateNoteSlidesListWithText (note);
  }

  // should the note be parenthesized?
  msrNoteHeadParenthesesKind
    noteHeadParenthesesKind =
      note->getNoteHeadParenthesesKind ();

  switch (noteHeadParenthesesKind) {
    case msrNoteHeadParenthesesKind::kNoteHeadParenthesesYes:
      fLilypondCodeStream << "\\parenthesize ";
      break;
    case msrNoteHeadParenthesesKind::kNoteHeadParenthesesNo:
      break;
  } // switch

  // generate the note technicals with string if any
  const std::list <S_msrTechnicalWithString>&
    noteTechnicalWithStringsList =
      note->getNoteTechnicalWithStringsList ();

  if (! noteTechnicalWithStringsList.empty ()) {
    generateNoteTechnicalsListWithStrings (note);
  }

  // is the note a cue note?
  switch (note->getNoteIsACueNoteKind ()) {
    case msrNoteIsACueNoteKind::kNoteIsACueNoteYes:
      fLilypondCodeStream <<
        "\\once \\override NoteHead.font-size = -3 ";
      break;
    case msrNoteIsACueNoteKind::kNoteIsACueNoteNo:
      break;
  } // switch

  // has the note an octave shift up or down?
  if (! fOnGoingChord) {
    // the octave shift for the chord if any has already been generated
    S_msrOctaveShift
      noteOctaveShift =
        note->
          getNoteOctaveShift ();

    if (noteOctaveShift) {
      generateOctaveShiftBeforeNote (
        noteOctaveShift);
    }
  }
}

//________________________________________________________________________
// void lpsr2lilypondTranslator::handleRestInTuplet (
//   const S_msrNote& rest)
// {
// }
//
// void lpsr2lilypondTranslator::handleRestInMeasure (
//   const S_msrNote& rest)
// {
// }
//
// void lpsr2lilypondTranslator::handleSkipInMeasure (
//   const S_msrNote& skip)
// {
// }
//
// void lpsr2lilypondTranslator::handleRegularNoteInChord (
//   const S_msrNote& regularNote)
// {
//   generateRegularNoteInChord (regularNote);
// }
//
// void lpsr2lilypondTranslator::handleNoteInDoubleTremolo (
//   const S_msrNote& regularNote)
// {
// }
//
// void lpsr2lilypondTranslator::handleRegularNoteInTuplet (
//   const S_msrNote& regularNote)
// {
//   generateRegularNoteInTuplet (regularNote);
// }
//
// void lpsr2lilypondTranslator::handleRegularNoteInMeasure (
//   const S_msrNote& regularNote)
// {
//   generateRegularNoteInMeasure (regularNote);
// }
//
// // notes in grace notes groups
// void lpsr2lilypondTranslator::handleNoteInGraceNotesGroup (
//   const S_msrNote& note)
// {
//   generateRegularNoteInGraceNotesGroup (note);
// }
//
// // rests in grace notes groups
// void lpsr2lilypondTranslator::handleNoteInGraceNotesGroup (
//   const S_msrNote& note)
// {
//   generateRestNoteInGraceNotesGroup (note);
// }
//
// // skips in grace notes groups
// void lpsr2lilypondTranslator::handleSkipInGraceNotesGroup (
//   const S_msrNote& note)
// {
// }
//
// void lpsr2lilypondTranslator::handleUnpitchedNoteInTuplet (
//   const S_msrNote& unpitchedNote)
// {
// }
//
// void lpsr2lilypondTranslator::handleUnpitchedNoteInMeasure (
//   const S_msrNote& unpitchedNote)
// {
// }
//
// void lpsr2lilypondTranslator::handleNoteInTupletInGraceNotesGroup (
//   const S_msrNote& regularNote)
// {
// }
//
// void lpsr2lilypondTranslator::handleNoteInChordInGraceNotesGroup (
//   const S_msrNote& regularNote)
// {
// }

//________________________________________________________________________
void lpsr2lilypondTranslator::visitStart (S_msrNote& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || traceLpsrVisitors || generateMsrVisitingInformation) { // JMI
      std::stringstream ss;

      ss <<
        "% --> Start visiting msrNote " <<
        elt->asShortString () <<
        ", fOnGoingChord: " <<
        fOnGoingChord <<
        ", fOnGoingGraceNotesGroup: " <<
        fOnGoingGraceNotesGroup <<
        ", fOnGoingChordGraceNotesGroupLink: " <<
        fOnGoingChordGraceNotesGroupLink <<
        ", fOnGoingMultipleMeasureRests: " <<
        fOnGoingMultipleMeasureRests <<
        ", line " << elt->getInputLineNumber () <<
        std::endl;

      if (traceLpsrVisitors) {
        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << ss.str ();
      }
    }
  }
#endif // MF_TRACE_IS_ENABLED

  // register the note as on-going note
//   fCurrentNotesTupletsStack.push_front (elt); // JMI will be popped in visitEnd (S_msrNote&)

//   switch (elt->getNoteKind ()) {
//     case msrNoteKind::kNote_UNKNOWN_:
//       break;
//
//     // in measures
//     case msrNoteKind::kNoteRegularInMeasure:
//       handleRegularNoteInMeasure (elt);
//       break;
//
//     case msrNoteKind::kNoteRestInMeasure:
//       handleRestInMeasure (elt);
//       break;
//
//     case msrNoteKind::kNoteSkipInMeasure:
//       handleSkipInMeasure (elt);
//       break;
//
//     case msrNoteKind::kNoteUnpitchedInMeasure:
//       handleUnpitchedNoteInMeasure (elt);
//       break;
//
//     // in chords
//     case msrNoteKind::kNoteRegularInChord:
//       handleRegularNoteInChord (elt);
//       break;
//
//     // in tuplets
//     case msrNoteKind::kNoteRegularInTuplet:
//       handleRegularNoteInTuplet (elt);
//       break;
//
//     case msrNoteKind::kNoteRestInTuplet:
//       handleRestInTuplet (elt);
//       break;
//
//     case msrNoteKind::kNoteUnpitchedInTuplet:
//       handleUnpitchedNoteInTuplet (elt);
//       break;
//
//     // in grace notes groups
//     case msrNoteKind::kNoteRegularInGraceNotesGroup:
//       handleNoteInGraceNotesGroup (elt);
//       break;
//
//     // in grace notes groups
//     case msrNoteKind::kNoteRestInGraceNotesGroup:
//       handleNoteInGraceNotesGroup (elt);
//       break;
//
//     case msrNoteKind::kNoteSkipInGraceNotesGroup:
//       handleSkipInGraceNotesGroup (elt);
//       {
//       // don't generate the grace notes here, that's done thru
//       // the note's graceNotesGroupBeforeNote and graceNotesGroupAfterNote fields
// //         noteIsToBeIgnored = true;
// /* JMI
//         S_msrNote
//           noteTheGraceNotesGroupIsAttachedTo =
//             elt->getNoteShortcutUpLinkToGraceNotesGroup ()->
//               getGraceNotesGroupUpLinkToNote ();
//
//         // don't generate note if the notes it's grace notes group is attached to
//         // has a ??? JMI
//         if (
//           noteTheGraceNotesGroupIsAttachedTo->getGraceNotesGroupBeforeNote ()
//             ||
//           noteTheGraceNotesGroupIsAttachedTo->getGraceNotesGroupAfterNote ()
//         ) {
//           noteIsToBeIgnored = true;
//         }
// */
//       }
//       break;
//
//     // in chords in grace notes groups
//     case msrNoteKind::kNoteInChordInGraceNotesGroup:
//       handleNoteInChordInGraceNotesGroup (elt);
//
// #ifdef MF_TRACE_IS_ENABLED
//       if (
//         gLpsrOahGroup->getTraceLpsrVisitors ()
//           ||
//         gTraceOahGroup->getTraceGraceNotes ()
//       ) {
//         S_msrGraceNotesGroup
//           upLinkToGraceNotesGroup =
//             elt->getNoteShortcutUpLinkToGraceNotesGroup ();
//
//         std::stringstream ss;
//
//         ss <<
//           "% ==> upLinkToGraceNotesGroup: ";
//         if (upLinkToGraceNotesGroup) {
//           ss <<
//             upLinkToGraceNotesGroup->asString ();
//         }
//         else {
//           ss << "[NULL]";
//         }
//
//         gWaeHandler->waeTrace (
//           __FILE__, mfInputLineNumber (__LINE__),
//           ss.str ());
//       }
// #endif // MF_TRACE_IS_ENABLED
//       break;
//
//    // in tuplets in grace notes groups
//    case msrNoteKind::kNoteInTupletInGraceNotesGroup:
//       handleNoteInTupletInGraceNotesGroup (elt);
//       break;
//
//     // in double-tremolos
//     case msrNoteKind::kNoteInDoubleTremolo:
//       handleNoteInDoubleTremolo (elt);
//       break;
//
//   } // switch

  // is this note to be ignored?
  Bool noteIsToBeIgnored (false);

  if (false && fOnGoingGraceNotesGroup) { // JMI
    if (fOnGoingChord) {
      noteIsToBeIgnored = true;
    }
    else {
//       handleNoteInGraceNotesGroup (elt);

      switch (elt->getNoteKind ()) {
        case msrNoteKind::kNote_UNKNOWN_:
          break;

        // in measures
        case msrNoteKind::kNoteRegularInMeasure:
          break;
        case msrNoteKind::kNoteRestInMeasure:
          break;
        case msrNoteKind::kNoteSkipInMeasure:
          break;
        case msrNoteKind::kNoteUnpitchedInMeasure:
          break;

        // in chords
        case msrNoteKind::kNoteRegularInChord:
          break;

        // in tuplets
        case msrNoteKind::kNoteRegularInTuplet:
          break;
        case msrNoteKind::kNoteRestInTuplet:
          break;
        case msrNoteKind::kNoteUnpitchedInTuplet:
          break;

        // in grace notes groups
        case msrNoteKind::kNoteRegularInGraceNotesGroup:
        case msrNoteKind::kNoteRestInGraceNotesGroup:
        case msrNoteKind::kNoteSkipInGraceNotesGroup:
          {
          // don't generate the grace notes here, that's done thru
          // the note's graceNotesGroupBeforeNote and graceNotesGroupAfterNote fields
            noteIsToBeIgnored = true;
/* JMI
            S_msrNote
              noteTheGraceNotesGroupIsAttachedTo =
                elt->getNoteShortcutUpLinkToGraceNotesGroup ()->
                  getGraceNotesGroupUpLinkToNote ();

            // don't generate note if the notes it's grace notes group is attached to
            // has a ??? JMI
            if (
              noteTheGraceNotesGroupIsAttachedTo->getGraceNotesGroupBeforeNote ()
                ||
              noteTheGraceNotesGroupIsAttachedTo->getGraceNotesGroupAfterNote ()
            ) {
              noteIsToBeIgnored = true;
            }
*/
          }
          break;

        // in chords in grace notes groups
        case msrNoteKind::kNoteInChordInGraceNotesGroup:
#ifdef MF_TRACE_IS_ENABLED
          if (
            gLpsrOahGroup->getTraceLpsrVisitors ()
              ||
            gTraceOahGroup->getTraceGraceNotes ()
          ) {
            S_msrGraceNotesGroup
              upLinkToGraceNotesGroup =
                elt->getNoteShortcutUpLinkToGraceNotesGroup ();

            std::stringstream ss;

            ss <<
              "% ==> upLinkToGraceNotesGroup: ";
            if (upLinkToGraceNotesGroup) {
              ss <<
                upLinkToGraceNotesGroup->asString ();
            }
            else {
              ss << "[NULL]";
            }

            gWaeHandler->waeTrace (
              __FILE__, mfInputLineNumber (__LINE__),
              ss.str ());
          }
#endif // MF_TRACE_IS_ENABLED
          break;

       // in tuplets in grace notes groups
       case msrNoteKind::kNoteInTupletInGraceNotesGroup:
          break;

        // in double-tremolos
        case msrNoteKind::kNoteInDoubleTremolo:
          break;

      } // switch
    }
  }

  // is there an ongoing multiple measure rest?
  if (false && fOnGoingMultipleMeasureRests) {
    switch (elt->getNoteKind ()) {
      case msrNoteKind::kNoteRestInMeasure:
        // don't handle multiple measure rests, that's done in visitEnd (S_msrMultipleMeasureRest&)
          /*
          if (elt->getNoteOccupiesAFullMeasure ()) {
            Bool inhibitMultipleMeasureRestsBrowsing =
              fVisitedLpsrScore->
                getEmbeddedMsrScore ()->
                  getInhibitMultipleMeasureRestsBrowsing ();

            if (inhibitMultipleMeasureRestsBrowsing) {
              if (
                gLpsrOahGroup->getTraceLpsrVisitors ()
                  ||
                gTraceOahGroup->getTraceMultipleMeasureRests ()
              ) {
                std::stringstream ss;

                ss <<
                  "% ==> visiting multiple measure rests is ignored" <<
                  std::endl;

                gWaeHandler->waeTrace (
                  __FILE__, mfInputLineNumber (__LINE__),
                  ss.str ());
              }

  #ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceNotesDetails ()) {
      std::stringstream ss;

      ss <<
        "% ==> returning from visitStart (S_msrNote&)" <<
      std::endl;
    }
  #endif // MF_TRACE_IS_ENABLED

            noteIsToBeIgnored = true;
            }
          }
          */

#ifdef MF_TRACE_IS_ENABLED
          if (
            gLpsrOahGroup->getTraceLpsrVisitors ()
              ||
            gTraceOahGroup->getTraceMultipleMeasureRests ()
          ) {
            std::stringstream ss;

            ss <<
              "% ==> start visiting rest notes is ignored upon note " <<
              elt->asString () <<
              std::endl;

            gWaeHandler->waeTrace (
              __FILE__, mfInputLineNumber (__LINE__),
              ss.str ());
          }
#endif // MF_TRACE_IS_ENABLED

          noteIsToBeIgnored = true;
        break;

      case msrNoteKind::kNoteSkipInMeasure:
        if (elt->getNoteShortcutUpLinkToGraceNotesGroup ()) {
#ifdef MF_TRACE_IS_ENABLED
          if (
            gLpsrOahGroup->getTraceLpsrVisitors ()
              ||
            gTraceOahGroup->getTraceNotes ()
          ) {
            std::stringstream ss;

            ss <<
              "% ==> start visiting skip notes is ignored upon note " <<
              elt->asString ();

            gWaeHandler->waeTrace (
              __FILE__, mfInputLineNumber (__LINE__),
              ss.str ());
          }
#endif // MF_TRACE_IS_ENABLED

          noteIsToBeIgnored = true;
        }
        break;

      case msrNoteKind::kNoteRegularInGraceNotesGroup:
      case msrNoteKind::kNoteRestInGraceNotesGroup:
      case msrNoteKind::kNoteSkipInGraceNotesGroup:
#ifdef MF_TRACE_IS_ENABLED
          if (
            gLpsrOahGroup->getTraceLpsrVisitors ()
              ||
            gTraceOahGroup->getTraceGraceNotes ()
          ) {
            std::stringstream ss;

            ss <<
              "% ==> start visiting grace notes is ignored upon note " <<
              elt->asString ();

            gWaeHandler->waeTrace (
              __FILE__, mfInputLineNumber (__LINE__),
              ss.str ());
          }
#endif // MF_TRACE_IS_ENABLED

          noteIsToBeIgnored = true;
        break;

      case msrNoteKind::kNoteInChordInGraceNotesGroup:
#ifdef MF_TRACE_IS_ENABLED
          if (
            gLpsrOahGroup->getTraceLpsrVisitors ()
              ||
            gTraceOahGroup->getTraceGraceNotes ()
          ) {
            std::stringstream ss;

            ss <<
              "% ==> start visiting chord grace notes is ignored upon note " <<
              elt->asString ();

            gWaeHandler->waeTrace (
              __FILE__, mfInputLineNumber (__LINE__),
              ss.str ());
          }
#endif // MF_TRACE_IS_ENABLED

          noteIsToBeIgnored = true;
        break;

      default:
        ;
    } // switch
  }

  // is this note to be ignored?
  if (false && noteIsToBeIgnored) {
#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceNotes ()) {
      std::stringstream ss;

      ss <<
        "% ==> Start visiting notes is ignored inside grace notes groups " << // JMI BLARK
        elt->asShortString ();

      lpsr2lilypondInternalWarning (
        gServiceRunData->getInputSourceName (),
        elt->getInputLineNumber (),
        ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED

    return;
  }

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceNotes ()) {
    std::stringstream ss;

    ss <<
      "% --> Actually handling note " <<
      elt->asShortString () <<
      ", fOnGoingChord: " <<
      fOnGoingChord <<
      ", fOnGoingGraceNotesGroup: " <<
      fOnGoingGraceNotesGroup <<
      ", fOnGoingChordGraceNotesGroupLink: " <<
      fOnGoingChordGraceNotesGroupLink <<
      ", fOnGoingMultipleMeasureRests: " <<
      fOnGoingMultipleMeasureRests <<
      ", line " << elt->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // get the grace notes group after note
  S_msrGraceNotesGroup
    graceNotesGroupAfterNote =
      elt->getGraceNotesGroupAfterNote ();

  // generate the grace notes group after note opener if any
  if (graceNotesGroupAfterNote) {
    fLilypondCodeStream <<
      "\\afterGrace { ";
  }

  // generate the grace notes group before note if any,
  // unless the note belongs to a chord
  Bool doGenerateGraceNotesGroupBeforeNote (true);

  if (fOnGoingChord && fOnGoingGraceNotesGroup) {
    doGenerateGraceNotesGroupBeforeNote = false;
  }

  if (doGenerateGraceNotesGroupBeforeNote) {
    S_msrGraceNotesGroup
      graceNotesGroupBeforeNote =
        elt->getGraceNotesGroupBeforeNote ();

    if (graceNotesGroupBeforeNote) {
      generateGraceNotesGroup (
        graceNotesGroupBeforeNote);
    }
  }

  generateStuffBeforeTheNoteIself (elt);


  // generate things before the note
  generateCodeRightBeforeNote (elt);


  ////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////
  // generate the note itself as a LilyPond string
  ////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////

  switch (elt->getNoteKind ()) {
    case msrNoteKind::kNote_UNKNOWN_:
    case msrNoteKind::kNoteRestInMeasure:
    case msrNoteKind::kNoteSkipInMeasure:
    case msrNoteKind::kNoteRegularInMeasure:
    case msrNoteKind::kNoteInDoubleTremolo:
      generateTheNoteItself (elt);
      break;

    case msrNoteKind::kNoteRegularInGraceNotesGroup:
    case msrNoteKind::kNoteRestInGraceNotesGroup:
    case msrNoteKind::kNoteSkipInGraceNotesGroup:
      // don't generate the grace notes here,
      // that is done in generateRegularNoteInGraceNotesGroup()
      break;

    case msrNoteKind::kNoteInChordInGraceNotesGroup:
    case msrNoteKind::kNoteRegularInChord:
    case msrNoteKind::kNoteRegularInTuplet:
    case msrNoteKind::kNoteRestInTuplet:
      generateTheNoteItself (elt);
      break;

    case msrNoteKind::kNoteInTupletInGraceNotesGroup:
      // don't generate the grace notes here,
      // that is done in generateRegularNoteInGraceNotesGroup()
      break;

    case msrNoteKind::kNoteUnpitchedInMeasure:
      generateTheNoteItself (elt);
      break;

    case msrNoteKind::kNoteUnpitchedInTuplet:
      generateTheNoteItself (elt);
      break;
  } // switch

  // generate things after the note
  generateAfterNoteSpannersListIfAny (elt);

  generateCodeRightAfterNote (elt);

  // has the note an octave shift up or down?
  if (! fOnGoingChord) {
    // the octave shift for the chord if any has already been generated
    S_msrOctaveShift
      noteOctaveShift =
        elt->
          getNoteOctaveShift ();

    if (noteOctaveShift) {
      generateOctaveShiftAfterNote (
        noteOctaveShift);
    }
  }

/* JMI
  // get the grace notes group after note ??? JMI
  S_msrGraceNotesGroup
    graceNotesGroupAfterNote =
      elt->getGraceNotesGroupAfterNote ();

  // generate the grace notes group after note opener if any
  if (graceNotesGroupAfterNote) {
    fLilypondCodeStream <<
      "\\afterGrace { ";
  }
*/

  // generate the input line number as comment if relevant
  if (
    gGlobalLpsr2lilypondOahGroup->getInputLineNumbers ()
      ||
    gGlobalLpsr2lilypondOahGroup->getGeneratePositionInMeasures ()
  ) {
    generateInputLineNumberAndOrPositionInMeasureAsAComment (
      elt);
  }
}

void lpsr2lilypondTranslator::generateNoteHeadAndStem (
  S_msrNote note)
{
  msrPrintObjectKind
    notePrintObjectKind =
      note->getNotePrintObjectKind ();

  switch (notePrintObjectKind) {
    case msrPrintObjectKind::kPrintObjectYes:
      break;
    case msrPrintObjectKind::kPrintObjectNo:
      {
        // get the print notes head RGB color atom
        S_oahColorRGBAtom
          nonPrintNotesHeadColorRGBAtom =
            gGlobalLpsr2lilypondOahGroup->
              getNonPrintNotesHeadColorRGBAtom ();

        // has the note color been set? // JMI 0.9.65
        if (nonPrintNotesHeadColorRGBAtom->getSelected ()) {
          // yes
          const msrColorRGB&
            theColorRGB =
              gGlobalLpsr2lilypondOahGroup->
                getNonPrintNotesHeadColorRGB ();

          fLilypondCodeStream <<
            std::endl <<
            "\\once\\override NoteHead.color = #(rgb-color " <<
            theColorRGB.asSpaceSeparatedString (4) <<
            ")" <<
            std::endl;
        }
        else {
          // no
//             if (gGlobalLpsr2lilypondOahGroup->getCommentLilypondBasics ()) {
//               fLilypondCodeStream <<
//                 "%{ " <<
//                 gServiceRunData->getInputSourceName () <<
//                 ":" <<
//                 tempo->getInputLineNumber () <<
//                 ": " <<
//                 "ignoring 'msrPrintObjectKind::kPrintObjectNo'" <<
//                 " %}" <<
//                 std::endl;
//             }

          // should the noted and/or stem be hidden JMI 0.9.67
          Bool hideNoteHeadAndOrStem (true);

          switch (note->getNoteKind ()) {
            case msrNoteKind::kNote_UNKNOWN_:
              break;

            // in measures
            case msrNoteKind::kNoteRegularInMeasure:
              break;
            case msrNoteKind::kNoteRestInMeasure:
              hideNoteHeadAndOrStem = false;
              break;
            case msrNoteKind::kNoteSkipInMeasure:
              break;
            case msrNoteKind::kNoteUnpitchedInMeasure:
              break;

            // in chords
            case msrNoteKind::kNoteRegularInChord:
              break;

            // in tuplets
            case msrNoteKind::kNoteRegularInTuplet:
              break;
            case msrNoteKind::kNoteRestInTuplet:
              hideNoteHeadAndOrStem = false;
              break;
            case msrNoteKind::kNoteUnpitchedInTuplet:
              break;

            // in grace notes groups
            case msrNoteKind::kNoteRegularInGraceNotesGroup:
            case msrNoteKind::kNoteRestInGraceNotesGroup:
            case msrNoteKind::kNoteSkipInGraceNotesGroup:
              break;

            // in tuplets in grace notes groups
            case msrNoteKind::kNoteInChordInGraceNotesGroup:
              break;

           // in tuplets in grace notes groups
           case msrNoteKind::kNoteInTupletInGraceNotesGroup:
              break;

            // in double-tremolos
            case msrNoteKind::kNoteInDoubleTremolo:
              break;

          } // switch

          if (hideNoteHeadAndOrStem) {
            fLilypondCodeStream <<
              "\\once\\hide NoteHead \\once\\hide Stem ";
          }
        }
      }
      break;
  } // switch

  fCurrentNotePrinObjectKind = notePrintObjectKind;
}

void lpsr2lilypondTranslator::generateNoteScordaturasList (
  const S_msrNote& note)
{
  const std::list <S_msrScordatura>&
    noteScordaturasList =
      note->getNoteScordaturasList ();

  if (! noteScordaturasList.empty ()) {
    fLilypondCodeStream <<
      cLilypondScordaturaOpener <<
      std::endl;

    ++gIndenter;

    std::list <S_msrScordatura>::const_iterator
      iBegin = noteScordaturasList.begin (),
      iEnd   = noteScordaturasList.end (),
      i      = iBegin;
    for ( ; ; ) {
      S_msrScordatura
        scordatura = (*i);

      const std::list <S_msrStringTuning>&
        scordaturaStringTuningsList =
          scordatura->
            getScordaturaStringTuningsList ();

      ++gIndenter;

      fLilypondCodeStream <<
        cLilypondNewStaff <<
        " \\with { alignAboveContext = \"" <<
        note->
          fetchUpLinkToNoteToStaff ()->
            getStaffPathLikeName () <<
        "\" } {" <<
        std::endl;

      ++gIndenter;

      fLilypondCodeStream <<
        cLilypondHide << " Staff.Stem" <<
        std::endl <<
        cLilypondHide << " Staff.TimeSignature" <<
        std::endl <<
        "\\small" <<
        std::endl <<
        "\\once \\override Score.RehearsalMark.self-alignment-X = #LEFT" <<
        std::endl <<
        "\\mark\\markup {\\small\\bold \"Scordatura\"}" <<
        std::endl <<
        "<";

      if (! scordaturaStringTuningsList.empty ()) {
        std::list <S_msrStringTuning>::const_iterator
          iBegin = scordaturaStringTuningsList.begin (),
          iEnd   = scordaturaStringTuningsList.end (),
          i      = iBegin;
        for ( ; ; ) {
          S_msrStringTuning
            stringTuning = (*i);

          fLilypondCodeStream <<
            stringTuningAsLilypondString (
              note->getInputLineNumber (),
              stringTuning);

          if (++i == iEnd) break;

          fLilypondCodeStream << cLilyPondSpace;
        } // for
      }

      fLilypondCodeStream <<
        ">4" <<
        std::endl;

      --gIndenter;

      fLilypondCodeStream <<
        '}' <<
        std::endl;

      --gIndenter;

      fLilypondCodeStream <<
        "{" <<
        std::endl;

      ++gIndenter;

      if (++i == iEnd) break;
    } // for
  }
}

void lpsr2lilypondTranslator::generateNoteSlashesList (
  const S_msrNote& note)
{
  const std::list <S_msrSlash>&
    noteSlashesList =
      note->getNoteSlashesList ();

  if (! noteSlashesList.empty ()) {
    std::list <S_msrSlash>::const_iterator i;
    for (
      i = noteSlashesList.begin ();
      i != noteSlashesList.end ();
      ++i
    ) {
      S_msrSlash slash = (*i);

      switch (slash->getSlashTypeKind ()) {
        case msrSlashTypeKind::kSlashType_UNKNOWN_:
          break;

        case msrSlashTypeKind::kSlashTypeStart:
          fLilypondCodeStream <<
            std::endl <<
            "\\override Staff.NoteHead.style = #'slash " <<
            std::endl;
          break;

        case msrSlashTypeKind::kSlashTypeStop:
          fLilypondCodeStream <<
            std::endl <<
            "\\revert Staff.NoteHead.style " <<
            std::endl;
          break;
      } // switch

      switch (slash->getUseDotsKind ()) {
        case msrUseDotsKind::kUseDots_UNKNOWN_:
          break;

        case msrUseDotsKind::kUseDotsYes:
          fLilypondCodeStream <<
            std::endl <<
            "\\override Staff.NoteHead.style = #'slash " <<
            std::endl;
          break;

        case msrUseDotsKind::kUseDotsNo:
          fLilypondCodeStream <<
            std::endl <<
            "\\revert Staff.NoteHead.style " <<
            std::endl;
          break;
      } // switch

      switch (slash->getSlashUseStemsKind ()) {
        case msrSlashUseStemsKind::kSlashUseStems_UNKNOWN_:
          break;

        case msrSlashUseStemsKind::kSlashUseStemsYes:
          fLilypondCodeStream <<
            std::endl <<
            "\\undo \\hide Staff.Stem " <<
            std::endl;
          break;

        case msrSlashUseStemsKind::kSlashUseStemsNo:
          fLilypondCodeStream <<
            std::endl <<
            cLilypondHide << " Staff.Stem " <<
            std::endl;
          break;
      } // switch
    } // for
  }
}

void lpsr2lilypondTranslator::generateNoteWedgesList (
  const S_msrNote& note)
{
  const std::list <S_msrWedge>&
    noteWedgesList =
      note->getNoteWedgesList ();

  if (! noteWedgesList.empty ()) {
    std::list <S_msrWedge>::const_iterator i;
    for (
      i = noteWedgesList.begin ();
      i != noteWedgesList.end ();
      ++i
    ) {
      S_msrWedge wedge = (*i);

      switch (wedge->getWedgeKind ()) {
        case msrWedgeKind::kWedgeKindNone:
          break;

        case msrWedgeKind::kWedgeCrescendo:
          switch (wedge->getWedgeNienteKind ()) {
            case msrWedgeNienteKind::kWedgeNienteYes:
              fLilypondCodeStream <<
                std::endl <<
                "\\once\\override Hairpin.circled-tip = ##t " <<
                std::endl;
              break;
            case msrWedgeNienteKind::kWedgeNienteNo:
              break;
            } // switch
          break;

        case msrWedgeKind::kWedgeDecrescendo:
          switch (wedge->getWedgeNienteKind ()) {
            case msrWedgeNienteKind::kWedgeNienteYes:
              fLilypondCodeStream <<
                std::endl <<
                "\\once\\override Hairpin.circled-tip = ##t " <<
                std::endl;
              break;
            case msrWedgeNienteKind::kWedgeNienteNo:
              break;
            } // switch
          break;

        case msrWedgeKind::kWedgeStop:
          break;
      } // switch
    } // for
  }
}

void lpsr2lilypondTranslator::generateNoteSlurLineTypes (
  const S_msrNote& note)
{
  const std::list <S_msrSlur>&
    noteSlursList =
      note->getNoteSlursList ();

  if (! noteSlursList.empty ()) {
    std::list <S_msrSlur>::const_iterator i;
    for (S_msrSlur slur : noteSlursList) {
      /* JMI 0.9.68
      \slurDashed, \slurDotted, \slurHalfDashed,
      \slurHalfSolid, \slurDashPattern, \slurSolid
      */

      switch (slur->getSlurTypeKind ()) {
        case msrSlurTypeKind::kSlurTypeRegularStart:
        case msrSlurTypeKind::kSlurTypePhrasingStart:
          switch (slur->getSlurLineTypeKind ()) {
            case msrLineTypeKind::kLineTypeSolid:
              /* JMI ???
              fLilypondCodeStream <<
                "\\once\\slurSolid ";
              */
              break;
            case msrLineTypeKind::kLineTypeDashed:
              fLilypondCodeStream <<
                "\\once\\slurDashed ";
              break;
            case msrLineTypeKind::kLineTypeDotted:
              fLilypondCodeStream <<
                "\\once\\slurDotted ";
              break;
            case msrLineTypeKind::kLineTypeWavy:
              fLilypondCodeStream <<
                "\\once\\slurWavy "; // JMI
              break;
          } // switch
          break;
        default:
          ;
      } // switch
    } // for
  }
}

void lpsr2lilypondTranslator::generateNoteGlissandoStyles (
  const S_msrNote& note)
{
  const std::list <S_msrGlissando>&
    noteGlissandosList =
      note->getNoteGlissandosList ();

  if (! noteGlissandosList.empty ()) {
    std::list <S_msrGlissando>::const_iterator i;
    for (
      i = noteGlissandosList.begin ();
      i != noteGlissandosList.end ();
      ++i
    ) {
      S_msrGlissando glissando = (*i);

      switch (glissando->getGlissandoTypeKind ()) {
        case msrGlissandoTypeKind::kGlissandoTypeNone:
          break;

        case msrGlissandoTypeKind::kGlissandoTypeStart:
          // generate the glissando style
          switch (glissando->getGlissandoLineTypeKind ()) {
            case msrLineTypeKind::kLineTypeSolid:
              break;
            case msrLineTypeKind::kLineTypeDashed:
              fLilypondCodeStream <<
                std::endl <<
                "\\once\\override Glissando.style = #'dashed-line" <<
                std::endl;
              break;
            case msrLineTypeKind::kLineTypeDotted:
              fLilypondCodeStream <<
                std::endl <<
                "\\once\\override Glissando.style = #'dotted-line" <<
                std::endl;
              break;
            case msrLineTypeKind::kLineTypeWavy:
              fLilypondCodeStream <<
                std::endl <<
                "\\once\\override Glissando.style = #'zigzag" <<
                std::endl;
              break;
          } // switch
          break;

        case msrGlissandoTypeKind::kGlissandoTypeStop:
          break;
      } // switch
    } // for
  }
}

void lpsr2lilypondTranslator::generateNoteGlissandosListWithText (
  const S_msrNote& note)
{
  const std::list <S_msrGlissando>&
    noteGlissandosList =
      note->getNoteGlissandosList ();

  if (! noteGlissandosList.empty ()) {
    std::list <S_msrGlissando>::const_iterator i;
    for (
      i = noteGlissandosList.begin ();
      i != noteGlissandosList.end ();
      ++i
    ) {
      S_msrGlissando glissando = (*i);

      switch (glissando->getGlissandoTypeKind ()) {
        case msrGlissandoTypeKind::kGlissandoTypeNone:
          break;

        case msrGlissandoTypeKind::kGlissandoTypeStart:
          {
            std::string
              glissandoTextValue =
                glissando->getGlissandoTextValue ();

            if (! glissandoTextValue.empty ()) {
              // generate the glissando text on itself
              fLilypondCodeStream <<
                std::endl <<
                "\\once\\override Glissando.details.glissando-text = \"" <<
                glissandoTextValue <<
                "\"" <<
                std::endl <<
                "\\glissandoTextOn" <<
                std::endl;
            }
          }
          break;

        case msrGlissandoTypeKind::kGlissandoTypeStop:
          break;
      } // switch
    } // for
  }
}

void lpsr2lilypondTranslator::generateNoteSlideLineStyles (
  const S_msrNote& note)
{
  const std::list <S_msrSlide>&
    noteSlidesList =
      note->getNoteSlidesList ();

  if (! noteSlidesList.empty ()) {
    std::list <S_msrSlide>::const_iterator i;
    for (
      i = noteSlidesList.begin ();
      i != noteSlidesList.end ();
      ++i
    ) {
      S_msrSlide slide = (*i);

      switch (slide->getSlideTypeKind ()) {
        case msrSlideTypeKind::kSlideTypeNone:
          break;

        case msrSlideTypeKind::kSlideTypeStart:
          // generate the glissando style
          switch (slide->getSlideLineTypeKind ()) {
            case msrLineTypeKind::kLineTypeSolid:
              break;
            case msrLineTypeKind::kLineTypeDashed:
              fLilypondCodeStream <<
                std::endl <<
                "\\once\\override Glissando.style = #'dashed-line" <<
                std::endl;
              break;
            case msrLineTypeKind::kLineTypeDotted:
              fLilypondCodeStream <<
                std::endl <<
                "\\once\\override Glissando.style = #'dotted-line" <<
                std::endl;
              break;
            case msrLineTypeKind::kLineTypeWavy:
              fLilypondCodeStream <<
                std::endl <<
                "\\once\\override Glissando.style = #'zigzag" <<
                std::endl;
              break;
          } // switch
          break;

        case msrSlideTypeKind::kSlideTypeStop:
          break;
      } // switch
    } // for
  }
}

void lpsr2lilypondTranslator::generateNoteSlidesListWithText (
  const S_msrNote& note)
{
  const std::list <S_msrSlide>&
    noteSlidesList =
      note->getNoteSlidesList ();

  if (! noteSlidesList.empty ()) {
    std::list <S_msrSlide>::const_iterator i;
    for (
      i = noteSlidesList.begin ();
      i != noteSlidesList.end ();
      ++i
    ) {
      S_msrSlide slide = (*i);

      switch (slide->getSlideTypeKind ()) {
        case msrSlideTypeKind::kSlideTypeNone:
          break;

        case msrSlideTypeKind::kSlideTypeStart:
          {
            std::string
              slideTextValue =
                slide->getSlideTextValue ();

            if (! slideTextValue.empty ()) {
              // generate the slide text on itself
              fLilypondCodeStream <<
                std::endl <<
                "\\once\\override Glissando.details.glissando-text = \"" <<
                slideTextValue <<
                "\"" <<
                std::endl <<
                "\\glissandoTextOn" <<
                std::endl;
            }
          }
          break;

        case msrSlideTypeKind::kSlideTypeStop:
          break;
      } // switch
    } // for
  }
}

void lpsr2lilypondTranslator::generateNoteTechnicalsListWithStrings (
  const S_msrNote& note)
{
  const std::list <S_msrTechnicalWithString>&
    noteTechnicalWithStringsList =
      note->getNoteTechnicalWithStringsList ();

  if (! noteTechnicalWithStringsList.empty ()) {
    std::list <S_msrTechnicalWithString>::const_iterator i;
    for (
      i = noteTechnicalWithStringsList.begin ();
      i != noteTechnicalWithStringsList.end ();
      ++i
    ) {
      S_msrTechnicalWithString technicalWithString = (*i);

      switch (technicalWithString->getTechnicalWithStringKind ()) {
        case msrTechnicalWithStringKind::kHammerOn:
          switch (technicalWithString->getTechnicalWithStringTypeKind ()) {
            case msrTechnicalTypeKind::kTechnicalTypeStart:
              {
                mfWholeNotes
                  noteSoundingWholeNotes =
                    note->getMeasureElementSoundingWholeNotes ();

                mfWholeNotes
                  halfWholeNotes =
                    noteSoundingWholeNotes / 2;

                fLilypondCodeStream <<
                  "\\after " <<
                  wholeNotesAsLilypondString (
                    note->getInputLineNumber (),
                    halfWholeNotes) <<
                  " ^\"H\" ";
              }
              break;
            case msrTechnicalTypeKind::kTechnicalTypeStop:
              break;
            case msrTechnicalTypeKind::kTechnicalType_UNKNOWN_:
              break;
          } // switch
          break;
        case msrTechnicalWithStringKind::kHandbell: // JMI
          break;
        case msrTechnicalWithStringKind::kOtherTechnical: // JMI
          break;
        case msrTechnicalWithStringKind::kPluck: // JMI
          break;
        case msrTechnicalWithStringKind::kPullOff:
          switch (technicalWithString->getTechnicalWithStringTypeKind ()) {
            case msrTechnicalTypeKind::kTechnicalTypeStart:
              {
                mfWholeNotes
                  noteSoundingWholeNotes =
                    note->getMeasureElementSoundingWholeNotes ();

                mfWholeNotes
                  halfWholeNotes =
                    noteSoundingWholeNotes /2;

                fLilypondCodeStream <<
                  "\\after " <<
                  wholeNotesAsLilypondString (
                    note->getInputLineNumber (),
                    halfWholeNotes) <<
                  " ^\"P\" ";
              }
              break;
            case msrTechnicalTypeKind::kTechnicalTypeStop:
              break;
            case msrTechnicalTypeKind::kTechnicalType_UNKNOWN_:
              break;
          } // switch
          break;
      } // switch
    } // for
  }
}

void lpsr2lilypondTranslator::generateTechnicals (
  const std::list <S_msrTechnical>& noteTechnicalsList)
{
  for (S_msrTechnical technical : noteTechnicalsList) {
    fLilypondCodeStream <<
      technicalAsLilypondString (technical);

    switch (technical->getTechnicalPlacementKind ()) {
      case msrPlacementKind::kPlacement_UNKNOWN_:
        break;
      case msrPlacementKind::kPlacementAbove:
        fLilypondCodeStream << "^";
        break;
      case msrPlacementKind::kPlacementBelow:
        fLilypondCodeStream << "_";
        break;
    } // switch

    fLilypondCodeStream << cLilyPondSpace;
  } // for
}

void lpsr2lilypondTranslator::generateTechnicalsWithInteger (
  const std::list <S_msrTechnicalWithInteger>&
    technicalWithIntegersList)
{
  std::list <S_msrTechnicalWithInteger>::const_iterator i;
  for (S_msrTechnicalWithInteger technicalWithInteger : technicalWithIntegersList) {
    fLilypondCodeStream <<
      technicalWithIntegerAsLilypondString (
        technicalWithInteger);

    switch (technicalWithInteger->getTechnicalWithIntegerPlacementKind ()) {
      case msrPlacementKind::kPlacement_UNKNOWN_:
        break;
      case msrPlacementKind::kPlacementAbove:
        fLilypondCodeStream << "^";
        break;
      case msrPlacementKind::kPlacementBelow:
        fLilypondCodeStream << "_";
        break;
    } // switch

    fLilypondCodeStream << cLilyPondSpace;
  } // for
}

void lpsr2lilypondTranslator::generateTechnicalsWithFloat (
  const std::list <S_msrTechnicalWithFloat>&
    technicalWithFloatsList)
{
  for (S_msrTechnicalWithFloat technicalWithFloat : technicalWithFloatsList) {
    fLilypondCodeStream <<
      technicalWithFloatAsLilypondString (
        technicalWithFloat);

    switch (technicalWithFloat->getTechnicalWithFloatPlacementKind ()) {
      case msrPlacementKind::kPlacement_UNKNOWN_:
        break;
      case msrPlacementKind::kPlacementAbove:
        fLilypondCodeStream << "^";
        break;
      case msrPlacementKind::kPlacementBelow:
        fLilypondCodeStream << "_";
        break;
    } // switch

    fLilypondCodeStream << cLilyPondSpace;
  } // for
}

void lpsr2lilypondTranslator::generateTechnicalsWithStringsList (
  const std::list <S_msrTechnicalWithString>&
    technicalWithStringsList)
{
  for (S_msrTechnicalWithString technicalWithString : technicalWithStringsList) {
    fLilypondCodeStream <<
      technicalWithStringAsLilypondString (technicalWithString);

    switch (technicalWithString->getTechnicalWithStringPlacementKind ()) {
      case msrPlacementKind::kPlacement_UNKNOWN_:
        break;
      case msrPlacementKind::kPlacementAbove:
        fLilypondCodeStream << "^";
        break;
      case msrPlacementKind::kPlacementBelow:
        fLilypondCodeStream << "_";
        break;
    } // switch

    fLilypondCodeStream << cLilyPondSpace;
  } // for
}

void lpsr2lilypondTranslator::generateDynamics (
  const std::list <S_msrDynamic>&
    dynamicsList)
{
  for (S_msrDynamic dynamic : dynamicsList) {
    switch (dynamic->getDynamicPlacementKind ()) {
      case msrPlacementKind::kPlacement_UNKNOWN_:
 // JMI       fLilypondCodeStream << "-3";
        break;
      case msrPlacementKind::kPlacementAbove:
        fLilypondCodeStream << "^";
        break;
      case msrPlacementKind::kPlacementBelow:
        // this is done by LilyPond by default
        break;
    } // switch

    fLilypondCodeStream <<
      dynamicAsLilypondString (dynamic) << cLilyPondSpace;
  } // for
}

void lpsr2lilypondTranslator::generateOtherDynamics (
  const std::list <S_msrOtherDynamic>&
    otherDynamicsList)
{
  for (S_msrOtherDynamic otherDynamic : otherDynamicsList) {
    switch (otherDynamic->getOtherDynamicPlacementKind ()) {
      case msrPlacementKind::kPlacement_UNKNOWN_:
        fLilypondCodeStream << "-";
        break;
      case msrPlacementKind::kPlacementAbove:
        fLilypondCodeStream << "^";
        break;
      case msrPlacementKind::kPlacementBelow:
        // this is done by LilyPond by default
        break;
    } // switch

    fLilypondCodeStream <<
      "\\otherDynamic \"" <<
      otherDynamic->getOtherDynamicsString () <<
      "\" ";
  } // for
}

void lpsr2lilypondTranslator::generateLigatures (
  const std::list <S_msrLigature>&
    ligaturesList)
{
  for (S_msrLigature ligature : ligaturesList) {
    switch (ligature->getLigatureKind ()) {
      case msrLigatureKind::kLigatureNone:
        break;
      case msrLigatureKind::kLigatureStart:
        fLilypondCodeStream << cLilypondLigatureOpener; // JMI 0.9.70
        break;
      case msrLigatureKind::kLigatureContinue:
        break;
      case msrLigatureKind::kLigatureStop:
        fLilypondCodeStream << cLilypondLigatureCloser;
        break;
    } // switch
  } // for
}

void lpsr2lilypondTranslator::generateWedges (
  const std::list <S_msrWedge>&
    wedgesList)
{
  for (S_msrWedge wedge : wedgesList) {
    switch (wedge->getWedgeKind ()) {
      case msrWedgeKind::kWedgeKindNone:
        break;

      case msrWedgeKind::kWedgeCrescendo:
        switch (wedge->getWedgePlacementKind ()) {
          case msrPlacementKind::kPlacement_UNKNOWN_:
            break;
          case msrPlacementKind::kPlacementAbove:
            fLilypondCodeStream <<
              "^";
            break;
          case msrPlacementKind::kPlacementBelow:
            fLilypondCodeStream <<
              "_";
            break;
          } // switch
        fLilypondCodeStream <<
          cLilypondHairPinsCrescendo;
        break;

      case msrWedgeKind::kWedgeDecrescendo:
        switch (wedge->getWedgePlacementKind ()) {
          case msrPlacementKind::kPlacement_UNKNOWN_:
            break;
          case msrPlacementKind::kPlacementAbove:
            fLilypondCodeStream <<
              "^";
            break;
          case msrPlacementKind::kPlacementBelow:
            fLilypondCodeStream <<
              "_";
            break;
          } // switch
        fLilypondCodeStream <<
          cLilypondHairPinsDecrescendo;
        break;

      case msrWedgeKind::kWedgeStop:
        fLilypondCodeStream <<
          cLilypondHairPinsStop;
        break;
    } // switch
  } // for
}

void lpsr2lilypondTranslator:: generateArticulations (
  const S_msrNote note)
{
  const std::list <S_msrArticulation>&
    articulationsList =
      note->getNoteArticulationsList ();

  for (S_msrArticulation articulation : articulationsList ) {
    switch (articulation->getArticulationKind ()) {
      case msrArticulationKind::kArticulationFermata: // handle this better JMI
        if (
          // fermata?
          S_msrFermata
            fermata =
              dynamic_cast<msrFermata*>(&(*articulation))
          ) {
          switch (fermata->getFermataTypeKind ()) {
            case msrArticulationFermataType::kArticulationFermataTypeNone:
              // no placement needed
              break;
            case msrArticulationFermataType::kArticulationFermataTypeUpright:
              // no placement needed
              break;
            case msrArticulationFermataType::kArticulationFermataTypeInverted:
              fLilypondCodeStream << "_";
              break;
          } // switch

          switch (fermata->getFermataShapeKind ()) {
            case msrFermataShapeKind::kArticulationFermataNormal:
              if (
                note->getNoteOccupiesAFullMeasure ()
                  &&
                note->fetchNoteIsARest ()
              ) {
//                     fLilypondCodeStream << "\\fermataMarkup ";
                fLilypondCodeStream << "\\fermata "; // 0.9.63, since LilyPond 2.23
              }
              else {
                fLilypondCodeStream << "\\fermata ";
              }
              break;
            case msrFermataShapeKind::kArticulationFermataAngled:
              fLilypondCodeStream << "\\shortfermata ";
              break;
            case msrFermataShapeKind::kArticulationFermataSquare:
              fLilypondCodeStream << "\\longfermata ";
              break;
          } // switch
        }
        else {
          std::stringstream ss;

          ss <<
            "note articulation '" <<
            articulation->asString () <<
            "' has 'fermata' kind, but is not of type S_msrFermata" <<
            ", line " << articulation->getInputLineNumber ();

          lpsr2lilypondInternalError (
            gServiceRunData->getInputSourceName (),
            articulation->getInputLineNumber (),
            __FILE__, mfInputLineNumber (__LINE__),
            ss.str ());
        }
        break;

      default:
        generateNoteArticulation (articulation);
        fLilypondCodeStream <<
          cLilyPondSpace;
    } // switch
  } // for
}

void lpsr2lilypondTranslator::generateNoteGlissandos (
  const std::list <S_msrGlissando>&
    glissandosList)
{
  for (S_msrGlissando glissando : glissandosList) {
    switch (glissando->getGlissandoTypeKind ()) {
      case msrGlissandoTypeKind::kGlissandoTypeNone:
        break;

      case msrGlissandoTypeKind::kGlissandoTypeStart:
        // generate the glissando itself
        fLilypondCodeStream <<
          "\\glissando ";

        if (! glissando->getGlissandoTextValue().empty ()) {
          fLilypondCodeStream <<
          "\\glissandoTextOff ";
        }
        break;

      case msrGlissandoTypeKind::kGlissandoTypeStop:
        break;
    } // switch
  } // for
}

void lpsr2lilypondTranslator::generateNoteSlides (
  const std::list <S_msrSlide>&
    slidesList)
{
  for (S_msrSlide slide : slidesList) {
    switch (slide->getSlideTypeKind ()) {
      case msrSlideTypeKind::kSlideTypeNone:
        break;

      case msrSlideTypeKind::kSlideTypeStart:
        // generate the glissando itself
        fLilypondCodeStream <<
          "\\glissando " <<
          "\\glissandoTextOff ";
        break;

      case msrSlideTypeKind::kSlideTypeStop:
        break;
    } // switch
  } // for
}

void lpsr2lilypondTranslator::visitEnd (S_msrNote& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream ss;

      ss <<
        "% --> End visiting note " <<
        elt->asShortString () <<
        ", line " << elt->getInputLineNumber () <<
        std::endl;

      if (traceLpsrVisitors) {
        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << ss.str ();
      }
    }
  }
#endif // MF_TRACE_IS_ENABLED

  Bool noteIsToBeIgnored (false);

  // is this note to be ignored?
  if (fOnGoingChord && fOnGoingGraceNotesGroup) { // JMI
    noteIsToBeIgnored = true;
  }

  if (fOnGoingMultipleMeasureRests) {
    switch (elt->getNoteKind ()) {
      case msrNoteKind::kNoteRestInMeasure:
        // don't handle multiple measure restss, that's done in visitEnd (S_msrMultipleMeasureRest&)
        if (elt->getNoteOccupiesAFullMeasure ()) {
          Bool
            inhibitMultipleMeasureRestsBrowsing =
              fVisitedLpsrScore->
                getEmbeddedMsrScore ()->
                  getInhibitMultipleMeasureRestsBrowsing ();

          if (inhibitMultipleMeasureRestsBrowsing) {
#ifdef MF_TRACE_IS_ENABLED
            if (
              gTraceOahGroup->getTraceNotes ()
                ||
              gTraceOahGroup->getTraceMultipleMeasureRests ()
            ) {
              std::stringstream ss;

              ss <<
                "% ==> end visiting multiple measure rests is ignored";

            gWaeHandler->waeTrace (
              __FILE__, mfInputLineNumber (__LINE__),
              ss.str ());
            }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_TRACE_IS_ENABLED
            if (gTraceOahGroup->getTraceNotesDetails ()) {
              std::stringstream ss;

              ss <<
                "% ==> returning from visitEnd (S_msrNote&)";

            gWaeHandler->waeTrace (
              __FILE__, mfInputLineNumber (__LINE__),
              ss.str ());
            }
#endif // MF_TRACE_IS_ENABLED

            noteIsToBeIgnored = true;
          }
        }
        break;

      case msrNoteKind::kNoteSkipInMeasure:
        if (elt->getNoteShortcutUpLinkToGraceNotesGroup ()) {
#ifdef MF_TRACE_IS_ENABLED
          if (
            gLpsrOahGroup->getTraceLpsrVisitors ()
              ||
            gTraceOahGroup->getTraceNotes ()
          ) {
            std::stringstream ss;

            ss <<
              "% ==> end visiting skip notes is ignored";

            gWaeHandler->waeTrace (
              __FILE__, mfInputLineNumber (__LINE__),
              ss.str ());
          }
#endif // MF_TRACE_IS_ENABLED

          noteIsToBeIgnored = true;
        }
        break;

      case msrNoteKind::kNoteRegularInGraceNotesGroup:
      case msrNoteKind::kNoteRestInGraceNotesGroup:
      case msrNoteKind::kNoteSkipInGraceNotesGroup:
#ifdef MF_TRACE_IS_ENABLED
          if (
            gLpsrOahGroup->getTraceLpsrVisitors ()
              ||
            gTraceOahGroup->getTraceGraceNotes ()
          ) {
            std::stringstream ss;

            ss <<
              "% ==> end visiting grace notes is ignored";

            gWaeHandler->waeTrace (
              __FILE__, mfInputLineNumber (__LINE__),
              ss.str ());
          }
#endif // MF_TRACE_IS_ENABLED

          noteIsToBeIgnored = true;
        break;

      default:
        ;
    } // switch
  }

  if (noteIsToBeIgnored) {
#ifdef MF_TRACE_IS_ENABLED
    if (gTraceOahGroup->getTraceNotes ()) {
      std::stringstream ss;

      ss <<
        "% ==> End visiting notes is ignored inside grace notes groups " << // JMI BLARK
        elt->asShortString ();

      lpsr2lilypondInternalWarning (
        gServiceRunData->getInputSourceName (),
        elt->getInputLineNumber (),
        ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED

    return;
  }

  // unregister the note as on-going note
//   fCurrentNotesTupletsStack.pop_front (); // JMI was pushed in visitStart (S_msrNote&)

  S_msrSingleTremolo
    noteSingleTremolo =
      elt->getNoteSingleTremolo ();

  if (noteSingleTremolo) {
    // generate the single tremolo only
    // if note doesn't belong to a chord,
    // otherwise it will be generated for the chord itself
    if (! elt->getNoteBelongsToAChord ()) {
      fLilypondCodeStream <<
        singleTremoloNotesDurationAsLilypondString (
          noteSingleTremolo);
    }
  }

  // generate the note words if any,
  // which should precede the articulations in LilyPond
  // generate the words only
  // if note doesn't belong to a chord,
  // otherwise it will be generated for the chord itself
  if (
 //   fOnGoingStanza // JMI ???
 //     &&
    ! elt->getNoteBelongsToAChord ()) {
    const std::list <S_msrWords>&
      noteWordsList =
        elt->getNoteWordsList ();

    if (! noteWordsList.empty ()) {
      generateNoteWordsList (noteWordsList);
    }
  }

  // generate the note technicals if any
  const std::list <S_msrTechnical>&
    noteTechnicalsList =
      elt->getNoteTechnicalsList ();

  if (! noteTechnicalsList.empty ()) {
    generateTechnicals (
      noteTechnicalsList);
  }

  // generate the note technicals with integer if any,
  // but not for chord member notes strings:
  // they should appear after the chord itself
  switch (elt->getNoteKind ()) {
    case msrNoteKind::kNoteRegularInChord:
       break;

    default:
      {
        const std::list <S_msrTechnicalWithInteger>&
          noteTechnicalWithIntegersList =
            elt->getNoteTechnicalWithIntegersList ();

        if (! noteTechnicalWithIntegersList.empty ()) {
          generateTechnicalsWithInteger (
            noteTechnicalWithIntegersList);
        }
      }
  } // switch

  // generate the note technicals with float if any,
  // but not for chord member notes strings:
  // they should appear after the chord itself
  switch (elt->getNoteKind ()) {
    case msrNoteKind::kNoteRegularInChord:
       break;

    default:
      {
        const std::list <S_msrTechnicalWithFloat>&
          noteTechnicalWithFloatsList =
            elt->getNoteTechnicalWithFloatsList ();

        if (! noteTechnicalWithFloatsList.empty ()) {
          generateTechnicalsWithFloat (
            noteTechnicalWithFloatsList);
        }
      }
  } // switch

  // generate the note technicals with string if any
  const std::list <S_msrTechnicalWithString>&
    noteTechnicalWithStringsList =
      elt->getNoteTechnicalWithStringsList ();

  if (! noteTechnicalWithStringsList.empty ()) {
    generateTechnicalsWithStringsList (
      noteTechnicalWithStringsList);
  }

  // generate the note ornaments if any
  const std::list <S_msrOrnament>&
    noteOrnamentsList =
      elt->getNoteOrnamentsList ();

  if (! noteOrnamentsList.empty ()) {
    std::list <S_msrOrnament>::const_iterator i;
    for (
      i = noteOrnamentsList.begin ();
      i != noteOrnamentsList.end ();
      ++i
    ) {
      S_msrOrnament
        ornament = (*i);

      generateOrnament (ornament); // some ornaments are not yet supportedb JMI 0.9.67
    } // for
  }

  // generate the note dynamics if any
  if (! fOnGoingChord) {
    const std::list <S_msrDynamic>&
      noteDynamicsList =
        elt->getNoteDynamicsList ();

    if (! noteDynamicsList.empty ()) {
      generateDynamics (
        noteDynamicsList);
    }
  }

  // generate the note other dynamics if any
  if (! fOnGoingChord) {
    const std::list <S_msrOtherDynamic>&
      noteOtherDynamicsList =
        elt->getNoteOtherDynamicsList ();

    if (! noteOtherDynamicsList.empty ()) {
      generateOtherDynamics (
        noteOtherDynamicsList);
    }
  }

  // generate the note beams if any,
  // unless the note is a grace group or chord member
  Bool doGenerateBeams (true);

  if (elt->getNoteBelongsToAChord ()) { // JMI 0.9.72
    doGenerateBeams = false;
  }
  else if (elt->getNoteIsAGraceNote ()) { // JMI 0.9.72
    doGenerateBeams = false;
  }
  else {
    const std::list <S_msrBeam>&
      noteBeamsList =
        elt->getNoteBeamsList ();

    if (! noteBeamsList.empty ()) {
      doGenerateBeams = true;
    }

    else {
      doGenerateBeams =
        gGlobalLpsr2lilypondOahGroup->getgenerateNoteBeamsAfterNote ();
          // JMI fixes superflous[ [ ... ] ] issue,
          // asked by Lars Opfermann upon 0.9.71
          // but then, when should they be generated ??? JMI
    }
  }

  if (doGenerateBeams) {
    generateNoteBeamsAfterNote (elt);
  }

  // generate the note ligatures if any
  const std::list <S_msrLigature>&
    noteLigaturesList =
      elt->getNoteLigaturesList ();

  if (! noteLigaturesList.empty ()) {
    generateLigatures (
      noteLigaturesList);
  }

  // generate the note wedges if any
  const std::list <S_msrWedge>&
    noteWedgesList =
      elt->getNoteWedgesList ();

  if (! noteWedgesList.empty ()) {
    generateWedges (
      noteWedgesList);
  }

  // generate the note slurs if any,
  // unless the note is chord member
  Bool doGenerateSlurs (true);

  if (elt->getNoteBelongsToAChord ()) {
     doGenerateSlurs = false;
  }

  if (doGenerateSlurs) {
    generateNoteSlursList (elt);
  }

  // generate the note articulations if any,
  // which should follow the dynamics and wedges in LilyPond
  if (! fOnGoingChord) {
    const std::list <S_msrArticulation>&
      noteArticulationsList =
        elt->getNoteArticulationsList ();

    if (! noteArticulationsList.empty ()) {
      generateArticulations (
        elt);
    }
  }

//   // generate the note glissandos if any
//   const std::list <S_msrGlissando>&
//     noteGlissandosList =
//       elt->getNoteGlissandosList ();
//
//   if (! noteGlissandosList.empty ()) {
//     generateNoteGlissandos (
//       noteGlissandosList);
//   }
//
  // generate the note slides if any, implemented as glissandos
  const std::list <S_msrSlide>&
    noteSlidesList =
      elt->getNoteSlidesList ();

  if (! noteSlidesList.empty ()) {
    generateNoteSlides (
      noteSlidesList);
  }

  // the note spanners if any are handled in visitStart (S_msrNote&)

  // are there note scordaturas?
  const std::list <S_msrScordatura>&
    noteScordaturasList =
      elt->getNoteScordaturasList ();

  if (! noteScordaturasList.empty ()) { // g_Indenter ??? JMI
    --gIndenter;

    fLilypondCodeStream <<
      std::endl <<
      '}' <<
      std::endl;

    --gIndenter;

    fLilypondCodeStream <<
      cLilypondScordaturaCloser <<
      std::endl;
  }

  if (elt->getNoteIsFollowedByGraceNotesGroup ()) { // JMI
    if (gGlobalLpsr2lilypondOahGroup->getCommentLilypondGraceNotes ()) {
      if (! elt->fetchNoteIsARest ()) {
        fLilypondCodeStream <<
         " % noteIsFollowedByGraceNotesGroup" <<
          std::endl; // JMI ???
      }
    }
  }

  // get the note's grace notes now,
  // unless the note belongs to a grace notes group link
  if (! fOnGoingChordGraceNotesGroupLink) {
    S_msrGraceNotesGroup
      graceNotesGroupAfterNote =
        elt->getGraceNotesGroupAfterNote ();

    // generate the note's grace notes after group closer if any
    if (graceNotesGroupAfterNote) {
      fLilypondCodeStream <<
        "} ";
//       generateGraceNotesGroup (
//         graceNotesGroupAfterNote);
    }
  }
}

//________________________________________________________________________
void lpsr2lilypondTranslator::visitStart (S_msrOctaveShift& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream ss;

      ss <<
        "% --> Start visiting msrOctaveShift" <<
        ", line " << elt->getInputLineNumber () <<
      std::endl;

      if (traceLpsrVisitors) {
        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << ss.str ();
      }
    }
  }
#endif // MF_TRACE_IS_ENABLED
}

void lpsr2lilypondTranslator::visitEnd (S_msrOctaveShift& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream ss;

      ss <<
        "% --> End visiting msrOctaveShift" <<
        ", line " << elt->getInputLineNumber () <<
      std::endl;

      if (traceLpsrVisitors) {
        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << ss.str ();
      }
    }
  }
#endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
void lpsr2lilypondTranslator::visitStart (S_msrAccordionRegistration& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream ss;

      ss <<
        "% --> Start visiting msrAccordionRegistration" <<
        ", line " << elt->getInputLineNumber () <<
      std::endl;

      if (traceLpsrVisitors) {
        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << ss.str ();
      }
    }
  }
#endif // MF_TRACE_IS_ENABLED

  int highDotsNumber =
    elt->getHighDotsNumber ();
  int middleDotsNumber =
    elt->getMiddleDotsNumber ();
  int lowDotsNumber =
    elt->getLowDotsNumber ();

  std::string numbersToBeUsed;
  Bool   nonZeroNumberHasBeenIssued (false);

  // the numbers should be written in the order 'high, middle, low'
  // and 0 ahead of the specification is forbidden

  if (highDotsNumber > 0) {
    numbersToBeUsed +=
      std::to_string (highDotsNumber);
    nonZeroNumberHasBeenIssued = true;
  }

  if (middleDotsNumber > 0) {
    numbersToBeUsed +=
      std::to_string (middleDotsNumber);
    nonZeroNumberHasBeenIssued = true;
  }
  else {
    if (nonZeroNumberHasBeenIssued) {
      numbersToBeUsed +=
        std::to_string (middleDotsNumber);
    }
  }

  numbersToBeUsed +=
    std::to_string (lowDotsNumber);

  fLilypondCodeStream <<
    "\\discant \"" <<
    numbersToBeUsed <<
    "\" ";
}

void lpsr2lilypondTranslator::visitStart (S_msrHarpPedalsTuning& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream ss;

      ss <<
        "% --> Start visiting msrHarpPedalsTuning" <<
        ", line " << elt->getInputLineNumber () <<
      std::endl;

      if (traceLpsrVisitors) {
        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << ss.str ();
      }
    }
  }
#endif // MF_TRACE_IS_ENABLED

  std::map <msrDiatonicPitchKind, msrAlterationKind>
    harpPedalsAlterationKindsMap =
      elt->getHarpPedalsAlterationKindsMap ();

  if (! harpPedalsAlterationKindsMap.empty ()) {
    ++gIndenter;

    fLilypondCodeStream <<
      "_\\markup { \\harp-pedal #\"" <<
      harpPedalTuningAsLilypondString (
        harpPedalsAlterationKindsMap [msrDiatonicPitchKind::kDiatonicPitchD]) <<
      harpPedalTuningAsLilypondString (
        harpPedalsAlterationKindsMap [msrDiatonicPitchKind::kDiatonicPitchC]) <<
      harpPedalTuningAsLilypondString (
        harpPedalsAlterationKindsMap [msrDiatonicPitchKind::kDiatonicPitchB]) <<
      "|" << // JMI 0.9.70
      harpPedalTuningAsLilypondString (
        harpPedalsAlterationKindsMap [msrDiatonicPitchKind::kDiatonicPitchE]) <<
      harpPedalTuningAsLilypondString (
        harpPedalsAlterationKindsMap [msrDiatonicPitchKind::kDiatonicPitchF]) <<
      harpPedalTuningAsLilypondString (
        harpPedalsAlterationKindsMap [msrDiatonicPitchKind::kDiatonicPitchG]) <<
      harpPedalTuningAsLilypondString (
        harpPedalsAlterationKindsMap [msrDiatonicPitchKind::kDiatonicPitchA]) <<
      "\" } " <<
      std::endl;

    --gIndenter;
  }
  else {
    fLilypondCodeStream <<
      "%{empty harp pedals tuning???%} "; // JMI
  }
}

//________________________________________________________________________
void lpsr2lilypondTranslator::visitStart (S_msrStem& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream ss;

      ss <<
        "% --> Start visiting msrStem" <<
        ", line " << elt->getInputLineNumber () <<
      std::endl;

      if (traceLpsrVisitors) {
        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << ss.str ();
      }
    }
  }
#endif // MF_TRACE_IS_ENABLED
}

void lpsr2lilypondTranslator::visitEnd (S_msrStem& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream ss;

      ss <<
        "% --> End visiting msrStem" <<
        ", line " << elt->getInputLineNumber () <<
      std::endl;

      if (traceLpsrVisitors) {
        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << ss.str ();
      }
    }
  }
#endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
void lpsr2lilypondTranslator::visitStart (S_msrBeam& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream ss;

      ss <<
        "% --> Start visiting msrBeam" <<
        ", line " << elt->getInputLineNumber () <<
      std::endl;

      if (traceLpsrVisitors) {
        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << ss.str ();
      }
    }
  }
#endif // MF_TRACE_IS_ENABLED

  // don't generate '[ ... ] here, this will be done in generateNoteBeamsAfterNote()
}

void lpsr2lilypondTranslator::visitEnd (S_msrBeam& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream ss;

      ss <<
        "% --> End visiting msrBeam" <<
        ", line " << elt->getInputLineNumber () <<
      std::endl;

      if (traceLpsrVisitors) {
        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << ss.str ();
      }
    }
  }
#endif // MF_TRACE_IS_ENABLED

  // don't generate '[ ... ] here, this will be done in generateNoteBeamsAfterNote()
}

//________________________________________________________________________
void lpsr2lilypondTranslator::generateOctaveShiftBeforeNote (
  const S_msrOctaveShift& octaveShift)
{
  msrOctaveShiftKind
    octaveShiftKind =
      octaveShift->
        getOctaveShiftKind ();

  int
    octaveShiftSize =
      octaveShift->
        getOctaveShiftSize ();

  switch (octaveShiftKind) {
    case msrOctaveShiftKind::kOctaveShiftNone:
      break;

    case msrOctaveShiftKind::kOctaveShiftUp:
      fLilypondCodeStream <<
      "\\ottava #" <<
        '-' << (octaveShiftSize - 1) / 7 << // 1 or 2
        cLilyPondSpace;
      break;

    case msrOctaveShiftKind::kOctaveShiftDown:
      fLilypondCodeStream <<
        "\\ottava #" <<
        (octaveShiftSize - 1) / 7 << // 1 or 2
        cLilyPondSpace;
      break;

    case msrOctaveShiftKind::kOctaveShiftStop:
      fLilypondCodeStream <<
        "\\ottava #0 ";
      break;

    case msrOctaveShiftKind::kOctaveShiftContinue:
      break;
  } // switch
}

void lpsr2lilypondTranslator::generateOctaveShiftAfterNote ( // JMI USELESS??? 0.9.66
  const S_msrOctaveShift& octaveShift)
{
  msrOctaveShiftKind
    octaveShiftKind =
      octaveShift->
        getOctaveShiftKind ();

  switch (octaveShiftKind) {
    case msrOctaveShiftKind::kOctaveShiftNone:
      break;

    case msrOctaveShiftKind::kOctaveShiftUp:
      break;

    case msrOctaveShiftKind::kOctaveShiftDown:
      break;

    case msrOctaveShiftKind::kOctaveShiftStop:
//       fLilypondCodeStream <<
//         "\\ottava #0 ";
      break;

    case msrOctaveShiftKind::kOctaveShiftContinue:
      break;
  } // switch
}

//________________________________________________________________________
void lpsr2lilypondTranslator::generateChordGlissandos (
  const std::list <S_msrGlissando>& chordGlissandosList)
{
  for (S_msrGlissando glissando : chordGlissandosList) {
    switch (glissando->getGlissandoTypeKind ()) {
      case msrGlissandoTypeKind::kGlissandoTypeNone:
        break;

      case msrGlissandoTypeKind::kGlissandoTypeStart:
        // generate the glissando style
        switch (glissando->getGlissandoLineTypeKind ()) {
          case msrLineTypeKind::kLineTypeSolid:
            break;
          case msrLineTypeKind::kLineTypeDashed:
            fLilypondCodeStream <<
              "\\once\\override Glissando.style = #'dashed-line ";
            break;
          case msrLineTypeKind::kLineTypeDotted:
            fLilypondCodeStream <<
              "\\once\\override Glissando.style = #'dotted-line ";
            break;
          case msrLineTypeKind::kLineTypeWavy:
            fLilypondCodeStream <<
              "\\once\\override Glissando.style = #'zigzag ";
            break;
        } // switch
        break;

      case msrGlissandoTypeKind::kGlissandoTypeStop:
        break;
    } // switch
  } // for
}

void lpsr2lilypondTranslator::generateChordSlides (
  const std::list <S_msrSlide>& chordSlidesList)
{
  for ( S_msrSlide slide : chordSlidesList) {
    switch (slide->getSlideTypeKind ()) {
      case msrSlideTypeKind::kSlideTypeNone:
        break;

      case msrSlideTypeKind::kSlideTypeStart:
        // generate the glissando style
        switch (slide->getSlideLineTypeKind ()) {
          case msrLineTypeKind::kLineTypeSolid:
            break;
          case msrLineTypeKind::kLineTypeDashed:
            fLilypondCodeStream <<
              "\\once\\override Glissando.style = #'dashed-line ";
            break;
          case msrLineTypeKind::kLineTypeDotted:
            fLilypondCodeStream <<
              "\\once\\override Glissando.style = #'dotted-line ";
            break;
          case msrLineTypeKind::kLineTypeWavy:
            fLilypondCodeStream <<
              "\\once\\override Glissando.style = #'zigzag ";
            break;
        } // switch
        break;

      case msrSlideTypeKind::kSlideTypeStop:
        break;
    } // switch
  } // for
}

void lpsr2lilypondTranslator::generateChordLigatures (
  const std::list <S_msrLigature>& chordLigaturesList)
{
  std::list <S_msrLigature>::const_iterator i;
  for (S_msrLigature ligature : chordLigaturesList) {
    switch (ligature->getLigatureKind ()) {
      case msrLigatureKind::kLigatureNone:
        break;
      case msrLigatureKind::kLigatureStart:
        fLilypondCodeStream << cLilypondLigatureOpener;
        break;
      case msrLigatureKind::kLigatureContinue:
        break;
      case msrLigatureKind::kLigatureStop:
// JMI        fLilypondCodeStream << cLilypondLigatureCloser;
        break;
    } // switch
  } // for
}

void lpsr2lilypondTranslator::generateChordStems (
  const S_msrChord&            chord,
  const std::list <S_msrStem>& chordStemsList)
{
  // only the first stem is used, since they all have the same kind
  S_msrStem firstChordStem = chordStemsList.front ();

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
// sanity check
  mfAssert (
    __FILE__, mfInputLineNumber (__LINE__),
    firstChordStem != nullptr,
    "firstChordStem is NULL");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceStems ()) {
    fLilypondCodeStream <<
      std::endl <<
      "% --> generateCodeBeforeChordBegin() for chord " <<
      chord->asShortString () <<
      ", firstChordStem: " << firstChordStem->asShortString () <<
      ", line " << chord->getInputLineNumber () <<
      std::endl;
  }
#endif // MF_TRACE_IS_ENABLED

  generateStemIfNeededAndUpdateCurrentStemKind (firstChordStem);

  fLilypondCodeStream <<
    cLilyPondSpace;
}

void lpsr2lilypondTranslator::generateCodeBeforeChordBegin (
  const S_msrChord& chord)
{
#ifdef MF_TRACE_IS_ENABLED
//   if (gGlobalLpsr2lilypondOahGroup->getCommentLilypondChords ()) { // JMI 0.9.67
//     ss <<
//       "%{ --> generateCodeBeforeChordBegin() for chord " <<
//       chord->asShortString () <<
//       ", line " << chord->getInputLineNumber () <<
//       " %}";
//
//     gWaeHandler->waeTrace (
//       __FILE__, mfInputLineNumber (__LINE__),
//       ss.str ());
//   }
#endif // MF_TRACE_IS_ENABLED

/* JMI
  ss <<
    "% chordGraceNotesGroupBefore = ";
  if (chordGraceNotesGroupBefore) {
    ss <<
      chordGraceNotesGroupBefore;
  }
  else {
    ss <<
      "[NULL]";
  }

  gWaeHandler->waeTrace (
    __FILE__, mfInputLineNumber (__LINE__),
    ss.str ());
*/

  // generate the chord codas if any
  const std::list <S_msrCoda>&
    chordCodasList =
      chord->getChordCodasList ();

  if (! chordCodasList.empty ()) {
    for (S_msrCoda coda : chordCodasList) {
      // generate the coda
      generateCoda (coda);
    } // for
  }

  // generate the chord segnos if any
  const std::list <S_msrSegno>&
    chordSegnosList =
      chord->getChordSegnosList ();

  if (! chordSegnosList.empty ()) {
    for (S_msrSegno segno : chordSegnosList) {
      // generate the segno
      generateSegno (segno);
    } // for
  }
  // generate the chord's grace notes before if any,
  S_msrChordGraceNotesGroupLink
    chordGraceNotesGroupLinkBefore =
      chord->getChordGraceNotesGroupLinkBefore ();

  if (chordGraceNotesGroupLinkBefore) {
    generateGraceNotesGroup (
      chordGraceNotesGroupLinkBefore->
        getOriginalGraceNotesGroup ());
  }

  // get the chord glissandos
  const std::list <S_msrGlissando>&
    chordGlissandosList =
      chord->getChordGlissandosList ();

  if (! chordGlissandosList.empty ()) {
    generateChordGlissandos (
      chordGlissandosList);
  }

  // get the chord slides
  const std::list <S_msrSlide>&
    chordSlidesList =
      chord->getChordSlidesList ();

  // generate the chord slides styles if any, implemented as glissandos
  if (! chordSlidesList.empty ()) {
    generateChordSlides (
      chordSlidesList);
  }

  // get the chord ligatures
  const std::list <S_msrLigature>&
    chordLigaturesList =
      chord->getChordLigaturesList ();

  // generate the chord ligatures if any
  if (! chordLigaturesList.empty ()) {
    generateChordLigatures (
      chordLigaturesList);
  }

  // don't take the chord into account for line breaking ??? JMI

  // get the chord articulations
  const std::list <S_msrArticulation>&
    chordArticulationsList =
      chord->getChordArticulationsList ();

  // generate the chord arpeggios directions if any
  if (! chordArticulationsList.empty ()) {
    std::list <S_msrArticulation>::const_iterator i;
    for (
      i = chordArticulationsList.begin ();
      i != chordArticulationsList.end ();
      ++i
    ) {
      S_msrArticulation articulation = (*i);

      if (
        // arpeggiato?
        S_msrArpeggiato
          arpeggiato =
            dynamic_cast<msrArpeggiato*>(&(*articulation))
      ) {
        msrDirectionKind
          directionKind =
            arpeggiato->getArpeggiatoDirectionKind ();

        switch (directionKind) {
          case msrDirectionKind::kDirectionNone:
            fLilypondCodeStream <<
              std::endl <<
              "\\arpeggioNormal";
            break;
          case msrDirectionKind::kDirectionUp:
            fLilypondCodeStream <<
              std::endl <<
              "\\arpeggioArrowUp";
            break;
          case msrDirectionKind::kDirectionDown:
            fLilypondCodeStream <<
              std::endl <<
              "\\arpeggioArrowDown";
            break;
        } // switch

        fLilypondCodeStream << cLilyPondSpace;

        fCurrentArpeggioDirectionKind = directionKind;
      }

      else if (
        // non arpeggiato?
        S_msrNonArpeggiato
          nonArpeggiato =
            dynamic_cast<msrNonArpeggiato*>(&(*articulation))
      ) {
        fLilypondCodeStream <<
          std::endl <<
          "\\arpeggioBracket";

        switch (nonArpeggiato->getNonArpeggiatoTypeKind ()) {
          case msrNonArpeggiatoTypeKind::kArticulationNonArpeggiatoTypeNone:
            fLilypondCodeStream << " %{ \\kArticulationNonArpeggiatoTypeNone??? %} ";
            break;
          case msrNonArpeggiatoTypeKind::kArticulationNonArpeggiatoTypeTop:
            fLilypondCodeStream << " %{ \\kArticulationNonArpeggiatoTypeTop??? %} ";
            break;
          case msrNonArpeggiatoTypeKind::kArticulationNonArpeggiatoTypeBottom:
            fLilypondCodeStream << " %{ \\kArticulationNonArpeggiatoTypeBottom??? %} ";
            break;
        } // switch

        fLilypondCodeStream << std::endl;
      }
   } // for
  }

  // should a stem direction command be generated?
  const std::list <S_msrStem>&
    chordStemsList =
      chord->getChordStemsList ();

  if (! chordStemsList.empty ()) {
    generateChordStems (
      chord,
      chordStemsList);
  }

  // should an octave shift be generated?
  S_msrOctaveShift
    chordOctaveShift =
      chord->getChordOctaveShift ();

  if (chordOctaveShift) {
    generateOctaveShiftBeforeNote (
      chordOctaveShift);
  }
}

void lpsr2lilypondTranslator::generateChordInGraceNotesGroupContents (
  const S_msrChord& chord)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceChords ()) {
    fLilypondCodeStream <<
      "% --> generateChordInGraceNotesGroupContents() for chord " <<
      chord->asShortString () <<
      ", line " << chord->getInputLineNumber () <<
      std::endl;
  }
#endif // MF_TRACE_IS_ENABLED

  // get the chord notes vector
  const std::vector <S_msrNote>&
    chordNotesVector =
      chord->getChordNotesVector ();

  // generate the chord notes KOF JMI 0.9.70
  if (! chordNotesVector.empty ()) {
    std::vector <S_msrNote>::const_iterator
      iBegin = chordNotesVector.begin (),
      iEnd   = chordNotesVector.end (),
      i      = iBegin;
    for ( ; ; ) {
      S_msrNote
        chordNote = (*i);

      // generate things before the chordNote
//       generateCodeRightBeforeNote (chordNote); // VITAL

      // generate the chordNote itself
      generateTheNoteItself (chordNote);

      // generate things after the chordNote
      generateCodeRightAfterNote (chordNote);

      if (++i == iEnd) break;
      fLilypondCodeStream <<
        cLilyPondSpace;
    } // for
  }
}

void lpsr2lilypondTranslator::generateCodeAfterChordEnd (
  const S_msrChord& chord)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceChords ()) {
    fLilypondCodeStream <<
      std::endl <<
      "% --> generateCodeAfterChordEnd() for chord " <<
      chord->asShortString () <<
      ", line " << chord->getInputLineNumber () <<
      std::endl;
  }
#endif // MF_TRACE_IS_ENABLED

  // get the chord notes vector
  const std::vector <S_msrNote>&
    chordNotesVector =
      chord->getChordNotesVector ();

  // if the preceding item is a chord, the first note of the chord
  // is used as the reference point for the octave placement
  // of a following note or chord
  switch (gGlobalLpsr2lilypondOahGroup->fetchOctaveEntryVariableValue ()) {
    case msrOctaveEntryKind::kOctaveEntryRelative:
      fCurrentOctaveEntryReference =
       chordNotesVector.at (0);
      break;
    case msrOctaveEntryKind::kOctaveEntryAbsolute:
      break;
    case msrOctaveEntryKind::kOctaveEntryFixed:
      break;
  } // switch

  // generate the chord duration if relevant
  if (
    chord->getChordIsFirstChordInADoubleTremolo ()
      ||
    chord->getChordIsSecondChordInADoubleTremolo ()) {
      // generate chord duration
      fLilypondCodeStream <<
        chord->getMeasureElementSoundingWholeNotes ();
  }

  else {
    mfInputLineNumber
      chordInputLineNumber =
        chord->getInputLineNumber ();

    // generate the chord duration if relevant
    mfWholeNotes
      chordSoundingWholeNotes =
        chord->getChordDisplayWholeNotes (); // JMI test wether chord is in a tuplet? 0.9.70

    if (wholeNotesDurationShouldBeGenerated (chordSoundingWholeNotes)) {
      generateWholeNotesDuration (
        chordInputLineNumber,
        chordSoundingWholeNotes);
    }
  }

/* JMI
  ss <<
    "% chordGraceNotesGroupAfter = ";
  if (chordGraceNotesGroupAfter) {
    ss <<
      chordGraceNotesGroupAfter;
  }
  else {
    ss <<
      "[NULL]";
  }

  gWaeHandler->waeTrace (
    __FILE__, mfInputLineNumber (__LINE__),
    ss.str ());

  if (chordGraceNotesGroupAfter) {
    generateGraceNotesGroup (
      chordGraceNotesGroupAfter);
  }
*/

  // generate the chord's grace notes after if any
  S_msrChordGraceNotesGroupLink
    chordGraceNotesGroupLinkAfter =
      chord->getChordGraceNotesGroupLinkAfter ();

  if (chordGraceNotesGroupLinkAfter) {
    generateGraceNotesGroup (
      chordGraceNotesGroupLinkAfter->
        getOriginalGraceNotesGroup ());
  }

  // are there pending chord member notes string numbers?
  if (! fPendingChordMemberNotesStringNumbers.empty ()) {
    std::list <int>::const_iterator
      iBegin = fPendingChordMemberNotesStringNumbers.begin (),
      iEnd   = fPendingChordMemberNotesStringNumbers.end (),
      i      = iBegin;

    for ( ; ; ) {
      int stringNumber = (*i);

      fLilypondCodeStream <<
        "\\" <<
        stringNumber;

      if (++i == iEnd) break;
      fLilypondCodeStream <<
        cLilyPondSpace;
    } // for
    fLilypondCodeStream <<
      cLilyPondSpace;

    // forget about the pending string numbers
    fPendingChordMemberNotesStringNumbers.clear ();
  }

  // fetch the chord single tremolo
  S_msrSingleTremolo
    chordSingleTremolo =
      chord->getChordSingleTremolo ();

  if (chordSingleTremolo) {
    // generate the chord single tremolo
    fLilypondCodeStream <<
      singleTremoloNotesDurationAsLilypondString (
        chordSingleTremolo);
  }

  fLilypondCodeStream <<
    cLilyPondSpace;

//   // get the chord articulations
//   const std::list <S_msrArticulation>&
//     chordArticulationsList =
//       chord->getChordArticulationsList ();
//
//   // generate the chord articulations if any
//   if (! chordArticulationsList.empty ()) {
//     std::list <S_msrArticulation>::const_iterator i;
//     for (
//       i = chordArticulationsList.begin ();
//       i != chordArticulationsList.end ();
//       ++i
//     ) {
//       generateChordArticulation ((*i));
//
//       fLilypondCodeStream <<
//         cLilyPondSpace;
//     } // for
//   }
//
  // generate the chord technicals if any
  const std::list <S_msrTechnical>&
    chordTechnicalsList =
      chord->getChordTechnicalsList ();

  if (! chordTechnicalsList.empty ()) {
    std::list <S_msrTechnical>::const_iterator i;
    for (
      i = chordTechnicalsList.begin ();
      i != chordTechnicalsList.end ();
      ++i
    ) {
      fLilypondCodeStream <<
        technicalAsLilypondString ((*i)) <<
        cLilyPondSpace; // JMI
    } // for
  }

  // generate the chord technicals with integer if any
  const std::list <S_msrTechnicalWithInteger>&
    chordTechnicalWithIntegersList =
      chord->getChordTechnicalWithIntegersList ();

  if (! chordTechnicalWithIntegersList.empty ()) {
    std::list <S_msrTechnicalWithInteger>::const_iterator i;
    for (
      i = chordTechnicalWithIntegersList.begin ();
      i != chordTechnicalWithIntegersList.end ();
      ++i
    ) {
      fLilypondCodeStream <<
        technicalWithIntegerAsLilypondString ((*i)) <<
        cLilyPondSpace; // JMI
    } // for
  }

  // generate the chord technicals with float if any
  const std::list <S_msrTechnicalWithFloat>&
    chordTechnicalWithFloatsList =
      chord->getChordTechnicalWithFloatsList ();

  if (! chordTechnicalWithFloatsList.empty ()) {
    std::list <S_msrTechnicalWithFloat>::const_iterator i;
    for (
      i = chordTechnicalWithFloatsList.begin ();
      i != chordTechnicalWithFloatsList.end ();
      ++i
    ) {
      fLilypondCodeStream <<
        technicalWithFloatAsLilypondString ((*i)) <<
        cLilyPondSpace; // JMI
    } // for
  }

  // generate the chord technicals with string if any
  const std::list <S_msrTechnicalWithString>&
    chordTechnicalWithStringsList =
      chord->getChordTechnicalWithStringsList ();

  if (! chordTechnicalWithStringsList.empty ()) {
    std::list <S_msrTechnicalWithString>::const_iterator i;
    for (
      i = chordTechnicalWithStringsList.begin ();
      i != chordTechnicalWithStringsList.end ();
      ++i
    ) {
      fLilypondCodeStream <<
        technicalWithStringAsLilypondString ((*i)) <<
        cLilyPondSpace; // JMI
    } // for
  }

  // generate the chord ornaments if any
  const std::list <S_msrOrnament>&
    chordOrnamentsList =
      chord->getChordOrnamentsList ();

  if (! chordOrnamentsList.empty ()) {
    std::list <S_msrOrnament>::const_iterator i;
    for (
      i = chordOrnamentsList.begin ();
      i != chordOrnamentsList.end ();
      ++i
    ) {
      S_msrOrnament
        ornament = (*i);

      switch (ornament->getOrnamentPlacementKind ()) {
        case msrPlacementKind::kPlacement_UNKNOWN_:
          fLilypondCodeStream << "-";
          break;
        case msrPlacementKind::kPlacementAbove:
          fLilypondCodeStream << "^";
          break;
        case msrPlacementKind::kPlacementBelow:
          fLilypondCodeStream << "-";
          break;
      } // switch

      generateOrnament (ornament); // some ornaments are not yet supported
    } // for
  }

  // generate the chord dynamics if any
  const std::list <S_msrDynamic>&
    chordDynamicsList =
      chord->getChordDynamicsList ();

  if (! chordDynamicsList.empty ()) {
    std::list <S_msrDynamic>::const_iterator i;
    for (
      i = chordDynamicsList.begin ();
      i != chordDynamicsList.end ();
      ++i
    ) {
      S_msrDynamic
        dynamic = (*i);

      switch (dynamic->getDynamicPlacementKind ()) {
        case msrPlacementKind::kPlacement_UNKNOWN_:
          fLilypondCodeStream << "-";
          break;
        case msrPlacementKind::kPlacementAbove:
          fLilypondCodeStream << "^";
          break;
        case msrPlacementKind::kPlacementBelow:
          // this is done by LilyPond by default
          break;
      } // switch

      fLilypondCodeStream <<
        dynamicAsLilypondString (dynamic) << cLilyPondSpace;
    } // for
  }

  // generate the chord other dynamics if any
  const std::list <S_msrOtherDynamic>&
    chordOtherDynamicsList =
      chord->getChordOtherDynamicsList ();

  if (! chordOtherDynamicsList.empty ()) {
    std::list <S_msrOtherDynamic>::const_iterator i;
    for (
      i = chordOtherDynamicsList.begin ();
      i != chordOtherDynamicsList.end ();
      ++i
    ) {
      S_msrOtherDynamic
        otherDynamic = (*i);

      switch (otherDynamic->getOtherDynamicPlacementKind ()) {
        case msrPlacementKind::kPlacement_UNKNOWN_:
          fLilypondCodeStream << "-";
          break;
        case msrPlacementKind::kPlacementAbove:
          fLilypondCodeStream << "^";
          break;
        case msrPlacementKind::kPlacementBelow:
          // this is done by LilyPond by default
          break;
      } // switch

      fLilypondCodeStream <<
        "\\otherDynamic \"" <<
        otherDynamic->getOtherDynamicsString () <<
        "\" ";
    } // for
  }

  // JMI 0.9.61, only now after the various dynamics
  // get the chord articulations
  const std::list <S_msrArticulation>&
    chordArticulationsList =
      chord->getChordArticulationsList ();

  // generate the chord articulations if any
  if (! chordArticulationsList.empty ()) {
    std::list <S_msrArticulation>::const_iterator i;
    for (
      i = chordArticulationsList.begin ();
      i != chordArticulationsList.end ();
      ++i
    ) {
      generateChordArticulation ((*i));

      fLilypondCodeStream <<
        cLilyPondSpace;
    } // for
  }

  // generate the chord words if any
  const std::list <S_msrWords>&
    chordWordsList =
      chord->getChordWordsList ();

  if (! chordWordsList.empty ()) {
    std::list <S_msrWords>::const_iterator i;
    for (
      i = chordWordsList.begin ();
      i != chordWordsList.end ();
      ++i
    ) {

      msrPlacementKind
        wordsPlacementKind =
          (*i)->getWordsPlacementKind ();

      std::string wordsContents =
        (*i)->getWordsContents ();

/*
<!--
	The words element specifies a standard text direction.
	Left justification is assumed if not specified.
	Language is Italian ("it") by default. Enclosure
	is none by default.
-->
<!ELEMENT words (#PCDATA)>
<!ATTLIST words
    %text-formatting;
    %optional-unique-id;
>

<!--
	The placement attribute indicates whether something is
	above or below another element, such as a note or a
	notation.
-->
<!ENTITY % placement
	"placement %above-below; #IMPLIED">
*/

      switch (wordsPlacementKind) {
        case msrPlacementKind::kPlacement_UNKNOWN_:
          fLilypondCodeStream << "-"; // JMI 0.9.69
          break;
        case msrPlacementKind::kPlacementAbove:
          fLilypondCodeStream << "^";
          break;
        case msrPlacementKind::kPlacementBelow:
          fLilypondCodeStream << "_";
          break;
      } // switch

      fLilypondCodeStream <<
        "\\markup" << " { " <<
        mfDoubleQuoteStringIfNonAlpha (wordsContents) <<
        " } ";
    } // for
  }

  // generate the chord beams if any
  const std::list <S_msrBeam>&
    chordBeamsList =
      chord->getChordBeamsList ();

  if (! chordBeamsList.empty ()) {
    // LilyPond will take care of multiple beams automatically,
    // so we need only generate the first number (level)
    for ( S_msrBeam chordBeam : chordBeamsList) {
      switch (chordBeam->getBeamKind ()) {

        case msrBeamKind::kBeamBegin:
          if (chordBeam->getBeamNumber () == 1)
            fLilypondCodeStream << "[ ";
            if (gGlobalLpsr2lilypondOahGroup->getCommentLilypondChords ()) {
              fLilypondCodeStream << " %{ generateCodeAfterChordEnd() %}";
            }

// #ifdef MF_MAINTAINANCE_RUNS_ARE_ENABLED
//               if (gWaeOahGroup->getMaintainanceRun ()) { // MAINTAINANCE_RUN
//                 fLilypondCodeStream << "%{ kBeamBegin 2 %} ";
//               }
// #endif // MF_MAINTAINANCE_RUNS_ARE_ENABLED

            if (gGlobalLpsr2lilypondOahGroup->getInputLineNumbers ()) {
              // generate the input line number as a comment
              fLilypondCodeStream <<
                " %{ <-- line " <<
                chordBeam->getInputLineNumber () <<
                " %} ";
            }
          break;

        case msrBeamKind::kBeamContinue:
          break;

        case msrBeamKind::kBeamEnd:
          if (chordBeam->getBeamNumber () == 1)
            fLilypondCodeStream << "] ";

#ifdef MF_MAINTAINANCE_RUNS_ARE_ENABLED
              if (gWaeOahGroup->getMaintainanceRun ()) { // MAINTAINANCE_RUN
                fLilypondCodeStream << "%{ kBeamEnd 2 %} ";
              }
#endif // MF_MAINTAINANCE_RUNS_ARE_ENABLED

            if (gGlobalLpsr2lilypondOahGroup->getInputLineNumbers ()) {
              // generate the input line number as a comment
              fLilypondCodeStream <<
                " %{ line " << chordBeam->getInputLineNumber () << " %} ";
            }
          break;

        case msrBeamKind::kBeamForwardHook:
          // JMI ???
          break;

        case msrBeamKind::kBeamBackwardHook:
          // JMI ???
          break;

        case msrBeamKind::kBeam_UNKNOWN_:
          // JMI ???
          break;
      } // switch
    } // for
  }

  // generate the chord slur links if any
  const std::list <S_msrChordSlurLink>&
    chordSlurLinksList =
      chord->getChordSlurLinksList ();

  if (! chordSlurLinksList.empty ()) {
    std::list <S_msrChordSlurLink>::const_iterator i;
    for (
      i = chordSlurLinksList.begin ();
      i != chordSlurLinksList.end ();
      ++i
    ) {
      S_msrChordSlurLink chordSlurLink = (*i);

      S_msrSlur originalSlur = chordSlurLink->getOriginalSlur ();

      switch (originalSlur->getSlurTypeKind ()) {
        case msrSlurTypeKind::kSlurType_UNKNOWN_:
          break;

        case msrSlurTypeKind::kSlurTypeRegularStart:
          fLilypondCodeStream << cLilypondSlurOpener;
          break;
        case msrSlurTypeKind::kSlurTypeRegularContinue:
          break;
        case msrSlurTypeKind::kSlurTypeRegularStop:
          fLilypondCodeStream << cLilypondSlurCloser;
          break;

        case msrSlurTypeKind::kSlurTypePhrasingStart:
          fLilypondCodeStream << cLilypondPhrasingOpener;
          break;
        case msrSlurTypeKind::kSlurTypePhrasingContinue:
          break;
        case msrSlurTypeKind::kSlurTypePhrasingStop:
          fLilypondCodeStream << cLilypondPhrasingCloser;
          break;
      } // switch

      if (gGlobalLpsr2lilypondOahGroup->getInputLineNumbers ()) {
        // generate the input line number as a comment
        fLilypondCodeStream <<
          " %{ <-- line " <<
          originalSlur->getInputLineNumber () <<
          " chord %} ";
      }
    } // for
  }

/* Don't print the chord ties, rely only on its notes's ties // JMI
  // thus using LilyPond's partially tied chords // JMI
  // generate the chord ties if any
  const std::list <S_msrTie>&
    chordTiesList =
      chord->getChordTiesList ();

  if (! chordTiesList.empty ()) {
    std::list <S_msrTie>::const_iterator i;
    for (
      i = chordTiesList.begin ();
      i != chordTiesList.end ();
      ++i
    ) {
      fLilypondCodeStream <<
        "%{line " << elt->getInputLineNumber () << "%} " <<
        "~ %{S_msrChord}"; // JMI spaces???
    } // for
  }
*/

/* JMI
  // generate the tie if any
  {
    S_msrTie chordTie = chord->getChordTie ();

    if (chordTie) {
      if (chordTie->getTieKind () == msrTieKind::kTieStart) {
        fLilypondCodeStream <<
          "%{line " << elt->getInputLineNumber () << "%} " <<
          "~ ";
      }
    }
  }
*/

  // get the chord ligatures
  const std::list <S_msrLigature>&
    chordLigaturesList =
      chord->getChordLigaturesList ();

  // generate the chord ligatures if any
  if (! chordLigaturesList.empty ()) {
    std::list <S_msrLigature>::const_iterator i;
    for (
      i = chordLigaturesList.begin ();
      i != chordLigaturesList.end ();
      ++i
    ) {
      switch ((*i)->getLigatureKind ()) {
        case msrLigatureKind::kLigatureNone:
          break;
        case msrLigatureKind::kLigatureStart:
          fLilypondCodeStream << cLilypondLigatureOpener;
          break;
        case msrLigatureKind::kLigatureContinue:
          break;
        case msrLigatureKind::kLigatureStop:
          fLilypondCodeStream << cLilypondLigatureCloser;
          break;
      } // switch
    } // for
  }

  // generate the chord wedges if any
  const std::list <S_msrWedge>&
    chordWedgesList =
      chord->getChordWedgesList ();

  if (! chordWedgesList.empty ()) {
    std::list <S_msrWedge>::const_iterator i;
    for (
      i = chordWedgesList.begin ();
      i != chordWedgesList.end ();
      ++i
      ) {
      switch ((*i)->getWedgeKind ()) {
        case msrWedgeKind::kWedgeKindNone:
          break;
        case msrWedgeKind::kWedgeCrescendo:
          fLilypondCodeStream << cLilypondHairPinsCrescendo;
          break;
        case msrWedgeKind::kWedgeDecrescendo:
          fLilypondCodeStream << cLilypondHairPinsDecrescendo;
          break;
        case msrWedgeKind::kWedgeStop:
          fLilypondCodeStream << cLilypondHairPinsStop;
          break;
      } // switch
    } // for
  }

  // get the chord glissandos
  const std::list <S_msrGlissando>&
    chordGlissandosList =
      chord->getChordGlissandosList ();

  // generate the chord glissandos if any
  if (! chordGlissandosList.empty ()) {
    std::list <S_msrGlissando>::const_iterator i;
    for (
      i = chordGlissandosList.begin ();
      i != chordGlissandosList.end ();
      ++i
    ) {
      S_msrGlissando glissando = (*i);

      switch (glissando->getGlissandoTypeKind ()) {
        case msrGlissandoTypeKind::kGlissandoTypeNone:
          break;

        case msrGlissandoTypeKind::kGlissandoTypeStart:
          // generate the glissando itself
          fLilypondCodeStream <<
            "\\glissando ";
          break;

        case msrGlissandoTypeKind::kGlissandoTypeStop:
          break;
      } // switch
    } // for
  }

  // get the chord slides
  const std::list <S_msrSlide>&
    chordSlidesList =
      chord->getChordSlidesList ();

  // generate the chord slides if any, implemented as glissandos
  if (! chordSlidesList.empty ()) {
    std::list <S_msrSlide>::const_iterator i;
    for (
      i = chordSlidesList.begin ();
      i != chordSlidesList.end ();
      ++i
    ) {
      S_msrSlide slide = (*i);

      switch (slide->getSlideTypeKind ()) {
        case msrSlideTypeKind::kSlideTypeNone:
          break;

        case msrSlideTypeKind::kSlideTypeStart:
          // generate the glissando itself
          fLilypondCodeStream <<
            "\\glissando ";
          break;

        case msrSlideTypeKind::kSlideTypeStop:
          break;
      } // switch
    } // for
  }

  // generate the chord dal segnos if any
  const std::list <S_msrDalSegno>&
    chordDalSegnosList =
      chord->getChordDalSegnosList ();

  if (! chordDalSegnosList.empty ()) {
    std::list <S_msrDalSegno>::const_iterator i;
    for (i = chordDalSegnosList.begin (); i != chordDalSegnosList.end (); ++i) {
      // generate the dal segno
      fLilypondCodeStream <<
        std::endl <<
        "\\override Score.RehearsalMark.break-visibility = #begin-of-line-invisible" <<
        std::endl <<
        "\\tweak self-alignment-X #RIGHT" <<
        std::endl <<
        "\\mark \\markup { " <<
        (*i)->getDalSegnoString () <<
        " }" <<
        std::endl;
    } // for
  }

  // should an octave shift be generated?
  S_msrOctaveShift
    chordOctaveShift =
      chord->getChordOctaveShift ();

  if (chordOctaveShift) {
    generateOctaveShiftAfterNote (
      chordOctaveShift);
  }
}

void lpsr2lilypondTranslator::generateChordInGraceNotesGroup (const S_msrChord& chord)
{
//   generateCodeBeforeChordBegin (chord);
//
//   // generate the start of the chord
//   fLilypondCodeStream <<
//    cLilypondChordOpener;
//
//   generateChordInGraceNotesGroupContents (chord);
//
//   // generate the end of the chord
//   fLilypondCodeStream <<
//     cLilypondChordCloser;
//
//   generateCodeAfterChordEnd (chord);
}

//________________________________________________________________________
void lpsr2lilypondTranslator::visitStart (S_msrChord& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream ss;

      ss <<
        "% --> Start visiting msrChord " <<
        elt->asShortString () <<
        ", fOnGoingChord: " <<
        fOnGoingChord <<
        ", fOnGoingGraceNotesGroup: " <<
        fOnGoingGraceNotesGroup <<
        ", fOnGoingChordGraceNotesGroupLink: " <<
        fOnGoingChordGraceNotesGroupLink <<
        ", line " << elt->getInputLineNumber () <<
      std::endl;

      if (traceLpsrVisitors) {
        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << ss.str ();
      }
    }
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_TRACE_IS_ENABLED
  if (fOnGoingGraceNotesGroup) {
    mfInputLineNumber
      chordInputLineNumber =
        elt->getInputLineNumber ();

    if (gTraceOahGroup->getTraceNotes ()) {
        std::stringstream ss;

        ss <<
          "% ==> Start visiting grace chords is ignored inside grace notes groups " <<
          elt->asShortString ();

      lpsr2lilypondInternalWarning (
        gServiceRunData->getInputSourceName (),
        chordInputLineNumber,
        ss.str ());
    }

    return;
  }
#endif // MF_TRACE_IS_ENABLED

  fCurrentChord = elt;
  fOnGoingChord = true;

  generateCodeBeforeChordBegin (elt);

  // generate the start of the chord
  if (gGlobalLpsr2lilypondOahGroup->getInputLineNumbers ()) {
    fLilypondCodeStream <<
      " %{ <-- line " <<
      elt->getInputLineNumber () <<
      " %}" <<
      std::endl;
  }

  fLilypondCodeStream <<
    cLilypondChordOpener;
}

void lpsr2lilypondTranslator::visitEnd (S_msrChord& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream ss;

      ss <<
        "% --> End visiting msrChord " <<
        elt->asShortString () <<
        ", line " << elt->getInputLineNumber () <<
        std::endl;

      if (traceLpsrVisitors) {
        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << ss.str ();
      }
    }
  }

  if (fOnGoingGraceNotesGroup) {
    if (gTraceOahGroup->getTraceNotes ()) {
      lpsr2lilypondInternalWarning (
        gServiceRunData->getInputSourceName (),
        elt->getInputLineNumber (),
        "% ==> End visiting grace chords is ignored inside grace notes groups");
    }

    return;
  }
#endif // MF_TRACE_IS_ENABLED

  // generate the end of the chord
  fLilypondCodeStream <<
    cLilypondChordCloser;

  generateCodeAfterChordEnd (elt);

  // if the preceding item is a chord, the first note of the chord
  // is used as the reference point for the octave placement
  // of a following note or chord
  switch (gGlobalLpsr2lilypondOahGroup->fetchOctaveEntryVariableValue ()) {
    case msrOctaveEntryKind::kOctaveEntryRelative:
      fCurrentOctaveEntryReference =
        elt->getChordNotesVector () [0];
      break;
    case msrOctaveEntryKind::kOctaveEntryAbsolute:
      break;
    case msrOctaveEntryKind::kOctaveEntryFixed:
      break;
  } // switch

  // forget about the current chord
  fCurrentChord = nullptr;

  fOnGoingChord = false;
}

//________________________________________________________________________
void lpsr2lilypondTranslator::visitStart (S_msrTuplet& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  {
    Bool
      traceLpsrVisitors = // true ||
        gLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

//     gLog << // JMI
//       "++++++++++++ visitStart (S_msrTuplet& elt)" <<
//       ", elt: " <<
//       std::endl;
//     ++gIndenter;
//     elt->print (gLog);
//     --gIndenter;

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream ss;

      ss <<
        "% --> Start visiting msrTuplet " <<
        elt->asShortString () <<
        ", fOnGoingChord: " <<
        fOnGoingChord <<
        ", fOnGoingChord: " <<
        fOnGoingChord <<
        ", fOnGoingGraceNotesGroup: " <<
        fOnGoingGraceNotesGroup <<
        ", fOnGoingChordGraceNotesGroupLink: " <<
        fOnGoingChordGraceNotesGroupLink <<
        ", line " << elt->getInputLineNumber () <<
        std::endl;

      if (traceLpsrVisitors) {
        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }

      gLog <<
        "------>>>>> tuplet is:" <<
        std::endl;
      ++gIndenter;
      elt->print (gLog);
      --gIndenter;
      gLog <<
        "<<<<<<------" <<
        std::endl;

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << ss.str ();
      }
    }
  }
#endif // MF_TRACE_IS_ENABLED

//   if (! fCurrentTupletsStack.empty ()) { // JMI 0.9.72 CHENIT INTEGRAL!!!
//     // elt is a nested tuplet
//
//     S_msrTuplet
//       containingTuplet =
//         fCurrentTupletsStack.front ();
//
//     // unapply containing tuplet factor,
//     // i.e 3/2 inside 5/4 becomes 15/8 in MusicXML...
//     elt->
//       unapplySoundingFactorToTupletMembers (
//         containingTuplet->
//           getTupletFactor ());
//   }

//   if (gGlobalLpsr2lilypondOahGroup->getIndentTuplets ()) { // JMI ??? 0.9.67
  if (gGlobalLpsr2lilypondOahGroup->getInputLineNumbers ()) {
    fLilypondCodeStream <<
      " %{ <-- line " <<
      elt->getInputLineNumber () <<
     " %}" <<
      std::endl;
  }

  // get bracket kind
  msrTupletBracketKind
    tupletBracketKind =
      elt->getTupletBracketKind ();

  switch (tupletBracketKind) {
    case msrTupletBracketKind::kTupletBracketYes:
      fLilypondCodeStream <<
        std::endl <<
        "\\once\\override TupletBracket.bracket-visibility = ##t" <<
        std::endl;
      break;
    case msrTupletBracketKind::kTupletBracketNo:
      fLilypondCodeStream <<
        "\\once \\omit TupletBracket" <<
        std::endl;
      break;
  } // switch

  // get bracket bracket placement kind
  msrPlacementKind
    tupletBracketPlacementKind  =
      elt->getTupletBracketPlacementKind  ();

  switch (tupletBracketPlacementKind ) {
    case msrPlacementKind::kPlacement_UNKNOWN_:
      break;

    case msrPlacementKind::kPlacementAbove:
      fLilypondCodeStream <<
        std::endl <<
        "\\once\\override TupletBracket.direction = -1 % DOWN" <<
        std::endl;
      break;

    case msrPlacementKind::kPlacementBelow:
      fLilypondCodeStream <<
        "\\once\\override TupletBracket.direction = 1 % UP" <<
        std::endl;
      break;
  } // switch

  // get line shape kind
  msrTupletLineShapeKind
    tupletLineShapeKind =
      elt->getTupletLineShapeKind ();

  switch (tupletLineShapeKind) {
    case msrTupletLineShapeKind::kTupletLineShapeStraight:
      break;
    case msrTupletLineShapeKind::kTupletLineShapeCurved:
      fLilypondCodeStream <<
        "\\temporary \\tupletsCurvedBrackets" <<
        std::endl;
      break;
  } // switch

  // get show number kind
  msrTupletShowNumberKind
    tupletShowNumberKind =
      elt->getTupletShowNumberKind ();

  switch (tupletShowNumberKind) {
    case msrTupletShowNumberKind::kTupletShowNumberActual:
    /* JMI
      if (gGlobalLpsr2lilypondOahGroup->getCommentLilypondTuplets ()) {
        fLilypondCodeStream <<
          "%{ tupletShowNumberActual %}" <<
          std::endl;
      }
    */
      break;
    case msrTupletShowNumberKind::kTupletShowNumberBoth:
      fLilypondCodeStream <<
        "\\once \\override TupletNumber.text = #tuplet-number::calc-fraction-text" <<
        std::endl;
      break;
    case msrTupletShowNumberKind::kTupletShowNumberNone:
      fLilypondCodeStream <<
        "\\once \\omit TupletNumber" <<
        std::endl;
      break;
  } // switch

//   // get show type kind JMI 0.9.71
//   msrTupletShowTypeKind
//     tupletShowTypeKind =
//       elt->getTupletShowTypeKind ();
//
//   mfWholeNotes
//     memberNoteDisplayWholeNotes =
//       elt->getMemberNotesDisplayWholeNotes ();
//
//   switch (tupletShowTypeKind) {
//     case msrTupletShowTypeKind::kTupletShowTypeActual:
//       fLilypondCodeStream <<
//      // JMI ???   "\\once \\override TupletNumber.text = #(tuplet-number::append-note-wrapper tuplet-number::calc-fraction-text \"" <<
//         "\\once \\override TupletNumber.text = #(tuplet-number::append-note-wrapper tuplet-number::calc-denominator-text #{ " <<
// //         wholeNotesAsLilypondMakeDuration ( // JMI 0.9.67
//         wholeNotesAsLilypondString ( // JMI 0.9.67
//           elt->getInputLineNumber (),
//           memberNoteDisplayWholeNotes) <<
//         " #})" <<
//         std::endl;
//       break;
//
//     case msrTupletShowTypeKind::kTupletShowTypeBoth:
//       fLilypondCodeStream <<
//         "\\once \\override TupletNumber.text = #(tuplet-number::fraction-with-notes" <<
//         " #{ " <<
//         wholeNotesAsLilypondString (
//           elt->getInputLineNumber (),
//           memberNoteDisplayWholeNotes) <<
//         " #} #{ " <<
//         wholeNotesAsLilypondString (
//           elt->getInputLineNumber (),
//           memberNoteDisplayWholeNotes) <<
//         " #})" <<
//         std::endl;
//       break;
//
//     case msrTupletShowTypeKind::kTupletShowTypeNone:
//     /* JMI
//       if (gGlobalLpsr2lilypondOahGroup->getCommentLilypondTuplets ()) {
//         fLilypondCodeStream <<
  //         "%{ tupletShowTypeNone %}" <<
  //         std::endl;
//       }
//     */
//       break;
//   } // switch

  // should a \tupletDown be generated? // JMI ??? 0.9.71
//   S_msrNote
//     tupletVoiceNumber =
//       elt->getTupletFactor ()->
//         getSingleTremoloUpLinkToNote ();

  // generate the \tuplet command
  fLilypondCodeStream <<
    cLilypondTupletOpener1 <<
//     tupletFactorAsLilypondString (elt->getTupletFactor ()) <<
    elt->getTupletFactor ().getTupletActualNotes () <<
    '/' <<
    elt->getTupletFactor ().getTupletNormalNotes () <<
    cLilypondTupletOpener2;

  if (gGlobalLpsr2lilypondOahGroup->getCommentLilypondTuplets ()) {
    fLilypondCodeStream <<
      " %{ tupletNumber: " << elt->getTupletNumber () <<
      ", tupleFactor: " << elt->getTupletFactor ().asFractionString () <<
      ", line " << elt->getInputLineNumber () <<
      " %} ";
  }

  if (gGlobalLpsr2lilypondOahGroup->getIndentTuplets ()) {
    fLilypondCodeStream << std::endl;
    ++gIndenter; // decremented in visitEnd (S_msrTuplet& elt)
  }
  else {
    fLilypondCodeStream << cLilyPondSpace;
  }

  // push the tuplet onto the ongoing tuplets stack
//   fCurrentTupletsStack.push_front (elt);

  // force durations to be displayed explicitly
  // at the beginning of the tuplet
  fLastMetWholeNotes = K_WHOLE_NOTES_UNKNOWN_;
}

void lpsr2lilypondTranslator::visitEnd (S_msrTuplet& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  {
    Bool
      traceLpsrVisitors = // true ||
        gLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream ss;

      ss <<
        "% --> End visiting msrTuplet " <<
        elt->asShortString () <<
        ", line " << elt->getInputLineNumber () <<
      std::endl;

      if (traceLpsrVisitors) {
        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }

      gLog <<
        "------>>>>> tuplet is:" <<
        std::endl;
      ++gIndenter;
      elt->print (gLog);
      --gIndenter;
      gLog <<
        "<<<<<<------" <<
        std::endl;

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << ss.str ();
      }
    }
  }
#endif // MF_TRACE_IS_ENABLED

  if (gGlobalLpsr2lilypondOahGroup->getIndentTuplets ()) {
    fLilypondCodeStream << std::endl;
    --gIndenter; // incremented in visitStart (S_msrTuplet& elt)
      // JMI 0.9.67 space in case this followed by a '|' in the output
  }

  fLilypondCodeStream <<
    cLilypondTupletCloser;

  // get line shape kind
  msrTupletLineShapeKind
    tupletLineShapeKind =
      elt->getTupletLineShapeKind ();

  switch (tupletLineShapeKind) {
    case msrTupletLineShapeKind::kTupletLineShapeStraight:
      break;
    case msrTupletLineShapeKind::kTupletLineShapeCurved:
      fLilypondCodeStream <<
        "\\undo \\tupletsCurvedBrackets" <<
        std::endl;
      break;
  } // switch

  // pop the tuplet from the ongoing tuplets stack
//   fCurrentTupletsStack.pop_front ();

/* JMI
  // forget about the current octave entry reference
  // the first note after the tuplet will become the new reference
 ?????? fCurrentOctaveEntryReference = nullptr;
  */
}

//________________________________________________________________________
void lpsr2lilypondTranslator::visitStart (S_msrTie& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream ss;

      ss <<
        "% --> Start visiting msrTie" <<
        ", line " << elt->getInputLineNumber () <<
        std::endl;

      if (traceLpsrVisitors) {
        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << ss.str ();
      }
    }
  }
#endif // MF_TRACE_IS_ENABLED

//   switch (elt->getTieKind ()) {
//     case msrTieKind::kTieNone:
//       break;
//     case msrTieKind::kTieStart:
//      fLilypondCodeStream << " ~ "; // JMI 0.9.72
//       break;
//     case msrTieKind::kTieContinue:
//       break;
//     case msrTieKind::kTieStop:
//       break;
//     case msrTieKind::kTieLetRing: // MusicXML 4.0
//       fLilypondCodeStream << " \\laissezVibrer "; // JMI 0.9.72
//       break;
//   } // switch
}

void lpsr2lilypondTranslator::visitEnd (S_msrTie& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream ss;

      ss <<
        "% --> End visiting msrTie" <<
        ", line " << elt->getInputLineNumber () <<
      std::endl;

      if (traceLpsrVisitors) {
        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << ss.str ();
      }
    }
  }
#endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
void lpsr2lilypondTranslator::visitStart (S_msrSegno& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream ss;

      ss <<
        "% --> Start visiting msrSegno" <<
        ", line " << elt->getInputLineNumber () <<
      std::endl;

      if (traceLpsrVisitors) {
        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << ss.str ();
      }
    }
  }
#endif // MF_TRACE_IS_ENABLED
}

void lpsr2lilypondTranslator::visitStart (S_msrHiddenMeasureAndBarLine& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream ss;

      ss <<
        "% --> Start visiting msrHiddenMeasureAndBarLine" <<
        ", line " << elt->getInputLineNumber () <<
      std::endl;

      if (traceLpsrVisitors) {
        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << ss.str ();
      }
    }
  }
#endif // MF_TRACE_IS_ENABLED

  fLilypondCodeStream <<
    "\\HiddenMeasureAndBarLine " <<
    "\\time 4/4"; // JMI ??? 0.9.67

  if (gGlobalLpsr2lilypondOahGroup->getInputLineNumbers ()) {
    fLilypondCodeStream <<
      " %{ <-- line " <<
      elt->getInputLineNumber () <<
      " %} ";
  }

  fLilypondCodeStream << std::endl;
}

void lpsr2lilypondTranslator::visitStart (S_msrCoda& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream ss;

      ss <<
        "% --> Start visiting msrCoda" <<
        ", line " << elt->getInputLineNumber () <<
      std::endl;

      if (traceLpsrVisitors) {
        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << ss.str ();
      }
    }
  }
#endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
void lpsr2lilypondTranslator::visitStart (S_msrEyeGlasses& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream ss;

      ss <<
        "% --> Start visiting eyeGlasses" <<
        ", line " << elt->getInputLineNumber () <<
      std::endl;

      if (traceLpsrVisitors) {
        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << ss.str ();
      }
    }
  }
#endif // MF_TRACE_IS_ENABLED

  fLilypondCodeStream <<
    "^\\markup {\\eyeglasses} ";
}

void lpsr2lilypondTranslator::visitStart (S_msrScordatura& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream ss;

      ss <<
        "% --> Start visiting scordatura" <<
        ", line " << elt->getInputLineNumber () <<
      std::endl;

      if (traceLpsrVisitors) {
        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << ss.str ();
      }
    }
  }
#endif // MF_TRACE_IS_ENABLED

/* JMI
  const std::list <S_msrStringTuning>&
    scordaturaStringTuningsList =
      elt->getScordaturaStringTuningsList ();

  ++gIndenter;

  fLilypondCodeStream <<
    cLilypondNewStaff << " {" <<
    std::endl;

  ++gIndenter;

  fLilypondCodeStream <<
    cLilypondHide << " Staff.Stem" <<
    std::endl <<
    cLilypondHide << " Staff.TimeSignature" <<
    std::endl <<
    "\\small" <<
    std::endl <<
    "<";

  if (! scordaturaStringTuningsList..empty ()) {
    std::list <S_msrStringTuning>::const_iterator
      iBegin = scordaturaStringTuningsList.begin (),
      iEnd   = scordaturaStringTuningsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      S_msrStringTuning
        stringTuning = (*i);

      fLilypondCodeStream <<
        stringTuningAsLilypondString (
          elt->getInputLineNumber (),
          stringTuning);

      if (++i == iEnd) break;

      fLilypondCodeStream << cLilyPondSpace;
    } // for
  }

  fLilypondCodeStream <<
    ">4" <<
    std::endl <<
    '}' <<
    std::endl;

  --gIndenter;

  fLilypondCodeStream <<
    "{ c'4 }" <<
    std::endl <<

  --gIndenter;
  */
}

void lpsr2lilypondTranslator::visitStart (S_msrPedal& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream ss;

      ss <<
        "% --> Start visiting pedal" <<
        ", line " << elt->getInputLineNumber () <<
        std::endl;

      if (traceLpsrVisitors) {
        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << ss.str ();
      }
    }
  }
#endif // MF_TRACE_IS_ENABLED

  fLilypondCodeStream << std::endl;

  switch (elt->getPedalTypeKind ()) {
    case msrPedalTypeKind::kPedalType_UNKNOWN_:
      {
        // should not occur
        std::stringstream ss;

        ss <<
          "msrPedal '" <<
          elt->asShortString () <<
          "' has no pedal type";

        lpsr2lilypondInternalError (
          gServiceRunData->getInputSourceName (),
          elt->getInputLineNumber (),
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }
      break;
    case msrPedalTypeKind::kPedalTypeStart:
      fLilypondCodeStream <<
        "\\sustainOn";
      break;
    case msrPedalTypeKind::kPedalTypeContinue:
      fLilypondCodeStream <<
        "%{ kPedalTypeContinue %} ";
//         "\\sustainOff\\sustainOn"; // JMI 0.9.70
      break;
    case msrPedalTypeKind::kPedalTypeChange:
      fLilypondCodeStream <<
        "%{ kPedalTypeChange %} ";
//         "\\sustainOff\\sustainOn"; // JMI 0.9.70
      break;
    case msrPedalTypeKind::kPedalTypeStop:
      fLilypondCodeStream <<
        "\\sustainOff";
      break;
  } // switch

  fLilypondCodeStream << std::endl;
}

void lpsr2lilypondTranslator::visitStart (S_msrDamp& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream ss;

      ss <<
        "% --> Start visiting damp" <<
        ", line " << elt->getInputLineNumber () <<
      std::endl;

      if (traceLpsrVisitors) {
        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << ss.str ();
      }
    }
  }
#endif // MF_TRACE_IS_ENABLED

  fLilypondCodeStream <<
    "^\\markup {\\damp} ";
}

void lpsr2lilypondTranslator::visitStart (S_msrDampAll& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream ss;

      ss <<
        "% --> Start visiting dampAll" <<
        ", line " << elt->getInputLineNumber () <<
      std::endl;

      if (traceLpsrVisitors) {
        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << ss.str ();
      }
    }
  }
#endif // MF_TRACE_IS_ENABLED

  fLilypondCodeStream <<
    "^\\markup {\\dampAll} ";
}

//________________________________________________________________________
void lpsr2lilypondTranslator::visitStart (S_msrBarLine& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream ss;

      ss <<
        std::endl <<
        "% --> Start visiting msrBarLine" <<
        ", line " << elt->getInputLineNumber () <<
      std::endl;

      if (traceLpsrVisitors) {
        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << ss.str ();
      }
    }
  }
#endif // MF_TRACE_IS_ENABLED

  switch (elt->getBarLineCategory ()) {
    case msrBarLineCategoryKind::kBarLineCategoryStandalone:
      switch (elt->getBarLineStyleKind ()) {
        case msrBarLineStyleKind::kBarLineStyleNone:
          break;
        case msrBarLineStyleKind::kBarLineStyleRegular:
          fLilypondCodeStream << "\\bar \"|\" ";
          break;
        case msrBarLineStyleKind::kBarLineStyleDotted:
          fLilypondCodeStream << "\\bar \";\" ";
          break;
        case msrBarLineStyleKind::kBarLineStyleDashed:
          fLilypondCodeStream << "\\bar \"!\" ";
          break;
        case msrBarLineStyleKind::kBarLineStyleHeavy:
          fLilypondCodeStream << "\\bar \".\" ";
          break;
        case msrBarLineStyleKind::kBarLineStyleLightLight:
          fLilypondCodeStream << "\\bar \"||\" ";
          break;
        case msrBarLineStyleKind::kBarLineStyleLightHeavy:
          fLilypondCodeStream <<
            std::endl <<
            "\\bar \"|.\" ";
          break;
        case msrBarLineStyleKind::kBarLineStyleHeavyLight:
          fLilypondCodeStream << "\\bar \".|\" ";
          break;
        case msrBarLineStyleKind::kBarLineStyleHeavyHeavy:
          fLilypondCodeStream << "\\bar \"..\" ";
          break;
        case msrBarLineStyleKind::kBarLineStyleTick:
          fLilypondCodeStream << "\\bar \"'\" ";
          break;
        case msrBarLineStyleKind::kBarLineStyleShort:
          // \bar '/' is the custom short barLine
          fLilypondCodeStream << "\\bar \"/\" ";
          break;
          /* JMI
        case msrBarLineStyleKind::kBarLineStyleNone:
          fLilypondCodeStream << "\\bar \"\" ";
          break;
          */
      } // switch
      if (gGlobalLpsr2lilypondOahGroup->getCommentLilypondBarLines ()) {
        fLilypondCodeStream <<
          " %{ b333 visitStart (S_msrBarLine& elt) %}";
      }

      if (gGlobalLpsr2lilypondOahGroup->getInputLineNumbers ()) {
        // generate the barLine line number as a comment
        fLilypondCodeStream <<
          " %{ <-- line " <<
          elt->getInputLineNumber () <<
          " %} ";
      }

      fLilypondCodeStream << std::endl;

/* JMI BOF ???
      switch (elt->getBarLineHasSegnoKind ()) {
        case msrBarLineHasSegnoKind::kBarLineHasSegnoYes:
          // generate the segno
          generateSegno ((*i));
          break;
        case msrBarLineHasSegnoKind::kBarLineHasSegnoNo:
          break;
      } // switch

      switch (elt->getBarLineHasCodaKind ()) {
        case msrBarLineHasCodaKind::kBarLineHasCodaYes:
          fLilypondCodeStream <<
            "\\mark \\markup { \\musicglyph #\"scripts.coda\" } ";
          if (gGlobalLpsr2lilypondOahGroup->getInputLineNumbers ()) {
            // generate the input line number as a comment
            fLilypondCodeStream <<
              " %{ <-- line " <<
              note->getInputLineNumber () <<
              " %} ";
          }
          fLilypondCodeStream << std::endl;
          break;
        case msrBarLineHasCodaKind::kBarLineHasCodaNo:
          break;
      } // switch
*/
      break;

    case msrBarLineCategoryKind::kBarLineCategoryRepeatStart:
      if (gGlobalLpsr2lilypondOahGroup->getKeepRepeatBarLines ()) {
      /*
        if (gGlobalLpsr2lilypondOahGroup->getRepeatBrackets ()) {
          if (! fRepeatDescrsStack.empty ()) {
            S_msrRepeat
              currentRepeat =
                fRepeatDescrsStack.back ()->getRepeat (),
              precedingRepeat =
                currentRepeat->getImmediatelyPrecedingRepeat ();

            if (precedingRepeat && ! precedingRepeat-> getRepeatEndings ().empty ()) {
              // JMI 0.9.72
            }
          }
          else {
            // JMI
          }
          fLilypondCodeStream << "\\bar \"[|:\" ";
        }
      */
        fLilypondCodeStream << "\\bar \".|:\" ";
      }
      else {
        // JMI
      }
      break;

    case msrBarLineCategoryKind::kBarLineCategoryRepeatEnd:
      if (gGlobalLpsr2lilypondOahGroup->getKeepRepeatBarLines ()) {
        if (gGlobalLpsr2lilypondOahGroup->getRepeatBrackets ()) {
          if (! fRepeatDescrsStack.empty ()) {
            S_msrRepeat
              currentRepeat =
                fRepeatDescrsStack.back ()->getRepeat (),
              followingRepeat =
                currentRepeat->getImmediatelyPrecedingRepeat ();

      // JMI      if (followingRepeat && followingRepeat-> getRepeatEndings ().empty ()) {
            if (followingRepeat) {
              fLilypondCodeStream << "\\bar \":|][|:\" ";
            }
          }
          else {
            fLilypondCodeStream << "\\bar \":|.\" ";
          }
        }
        else {
          fLilypondCodeStream << "\\bar \":|.\" ";
        }
        fLilypondCodeStream << " { b666 }";
      }
      else {
        // JMI
      }
      break;

    case msrBarLineCategoryKind::kBarLineCategoryHookedEndingStart:
    case msrBarLineCategoryKind::kBarLineCategoryHookedEndingEnd:
    case msrBarLineCategoryKind::kBarLineCategoryHooklessEndingStart:
    case msrBarLineCategoryKind::kBarLineCategoryHooklessEndingEnd:
      // should not occur, since
      // LilyPond will take care of displaying the repeat
      break;

    case msrBarLineCategoryKind::kBarLineCategory_UNKNOWN_:
      {
        std::stringstream ss;

        ss <<
          "barLine category has not been set" <<
          ", line " << elt->getInputLineNumber ();

  // JMI      lpsr2lilypondInternalError (
        lpsr2lilypondInternalWarning (
          gServiceRunData->getInputSourceName (),
          elt->getInputLineNumber (),
  // JMI        __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }
      break;
  } // switch

  if (gGlobalLpsr2lilypondOahGroup->getInputLineNumbers ()) {
    fLilypondCodeStream <<
      " %{ <-- line " <<
      elt->getInputLineNumber () <<
      " %} ";
  }
}

void lpsr2lilypondTranslator::visitEnd (S_msrBarLine& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream ss;

      ss <<
        std::endl <<
        "% --> End visiting msrBarLine" <<
        ", line " << elt->getInputLineNumber () <<
      std::endl;

      if (traceLpsrVisitors) {
        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << ss.str ();
      }
    }
  }
#endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
void lpsr2lilypondTranslator::visitStart (S_msrBarCheck& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream ss;

      ss <<
        "% --> Start visiting msrBarCheck" <<
        ", nextBarNumber: " <<
        elt->getNextBarPuristNumber () <<
        ", line " << elt->getInputLineNumber () <<
      std::endl;

      if (traceLpsrVisitors) {
        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << ss.str ();
      }
    }
  }
#endif // MF_TRACE_IS_ENABLED

  // should bar number checks be generated?
  if (gGlobalLpsr2lilypondOahGroup->getNoBarCheckComments ()) {
    return;
  }

  int nextBarPuristNumber =
    elt->getNextBarPuristNumber ();

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalLpsr2lilypondOahGroup->getCommentLilypondMultipleMeasureRests ()) {
    fLilypondCodeStream <<
      "% nextBarPuristNumber: " <<
      nextBarPuristNumber <<
      ", fOnGoingMultipleMeasureRests: " <<
      fOnGoingMultipleMeasureRests <<
      "fOnGoingVoiceCadenza: " <<
      fOnGoingVoiceCadenza <<
      ", nextBarPuristNumber: " <<
      nextBarPuristNumber <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif // MF_TRACE_IS_ENABLED

  if (
    !
      (
        fOnGoingVoiceCadenza
            // should be tested in msr2lpsrTranslator.cpp JMI visitEnd (S_msrMeasure&)
            // MusicXML bar numbers cannot be relied upon for a LilyPond bar number check
          ||
        fOnGoingMultipleMeasureRests
      )
  ) {
    // don't generate a bar check before the end of measure 1 // JMI ??? 0.9.70
    fLilypondCodeStream <<
      " | % " << nextBarPuristNumber;

    if (gGlobalLpsr2lilypondOahGroup->getInputLineNumbers ()) {
      // generate the line number as a comment
      fLilypondCodeStream <<
        " %{ <-- line " <<
        elt->getInputLineNumber () <<
        " %} ";
    }

    if (gGlobalLpsr2lilypondOahGroup->getOriginalMeasureNumbers ()) {
      // generate the original MusicXML measure number as a comment
      fLilypondCodeStream <<
        " % nbon: " << elt->getNextBarOriginalNumber () << " %} ";
    }

    fLilypondCodeStream <<
      std::endl;
  }
}

void lpsr2lilypondTranslator::visitEnd (S_msrBarCheck& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream ss;

      ss <<
        "% --> End visiting msrBarCheck" <<
        ", line " << elt->getInputLineNumber () <<
      std::endl;

      if (traceLpsrVisitors) {
        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << ss.str ();
      }
    }
  }
#endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
void lpsr2lilypondTranslator::visitStart (S_msrBarNumberCheck& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream ss;

      ss <<
        "% --> Start visiting msrBarNumberCheck" <<
        ", line " << elt->getInputLineNumber () <<
        std::endl;

      if (traceLpsrVisitors) {
        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << ss.str ();
      }
    }
  }
#endif // MF_TRACE_IS_ENABLED

  // should bar number checks be generated?
  if (gGlobalLpsr2lilypondOahGroup->getNoBarNumberChecks ()) {
    return;
  }

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalLpsr2lilypondOahGroup->getCommentLilypondMultipleMeasureRests ()) {
    fLilypondCodeStream <<
      "%, fOnGoingMultipleMeasureRests: " <<
      fOnGoingMultipleMeasureRests <<
      "% fOnGoingVoiceCadenza: " <<
      fOnGoingVoiceCadenza <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif // MF_TRACE_IS_ENABLED

  if (
    !
      (
        fOnGoingVoiceCadenza
          // should be tested in msr2lpsrTranslator.cpp JMI visitEnd (S_msrMeasure&)
          // MusicXML bar numbers cannot be relied upon for a LilyPond bar number check
          ||
        fOnGoingMultipleMeasureRests
      )
  ) {
    mfMeasureNumber
      nextBarOriginalNumber =
        elt->getNextBarOriginalNumber ();

    int nextBarPuristNumber =
      elt->getNextBarPuristNumber ();

//     gLog << // JMI 0.9.71
//       "nextBarOriginalNumber: " << nextBarOriginalNumber <<
//       ", nextBarPuristNumber: " << nextBarPuristNumber <<
//       std::endl;

    fLilypondCodeStream <<
      "\\barNumberCheck #" <<
      nextBarPuristNumber;

    if (gGlobalLpsr2lilypondOahGroup->getInputLineNumbers ()) {
      // generate the line number as a comment
      fLilypondCodeStream <<
        " %{ <-- line " <<
        elt->getInputLineNumber () <<
        " %} ";
    }

    if (gGlobalLpsr2lilypondOahGroup->getOriginalMeasureNumbers ()) {
      fLilypondCodeStream <<
        " %{ nbon: " <<
        elt->getNextBarOriginalNumber () <<
        " %}";
    }

    fLilypondCodeStream <<
      std::endl;
  }
}

void lpsr2lilypondTranslator::visitEnd (S_msrBarNumberCheck& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream ss;

      ss <<
        "% --> End visiting msrBarNumberCheck" <<
        ", line " << elt->getInputLineNumber () <<
      std::endl;

      if (traceLpsrVisitors) {
        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << ss.str ();
      }
    }
  }
#endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
void lpsr2lilypondTranslator::visitStart (S_msrLineBreak& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream ss;

      ss <<
        "% --> Start visiting msrLineBreak" <<
        ", line " << elt->getInputLineNumber () <<
      std::endl;

      if (traceLpsrVisitors) {
        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << ss.str ();
      }
    }
  }
#endif // MF_TRACE_IS_ENABLED

  // generate a page break
  fLilypondCodeStream <<
    "\\break | % " << elt->getNextBarPuristNumber ();

  if (gGlobalLpsr2lilypondOahGroup->getInputLineNumbers ()) {
    // generate the line number as a comment
    fLilypondCodeStream <<
      " %{ <-- line " <<
      elt->getNextBarPuristNumber () <<
      " %} ";
  }

  if (gGlobalLpsr2lilypondOahGroup->getOriginalMeasureNumbers ()) {
    // generate the original MusicXML measure number as a comment
    fLilypondCodeStream <<
      " (line " << elt->getInputLineNumber () << ")";
  }

//   // enforce a line break here, not using \myLineBreak,
//   // if this is a user selected line break // JMI TO BE SEEN 0.9.74 abcde
//   switch (elt->getUserSelectedLineBreakKind ()) {
//     case msrUserSelectedLineBreakKind::kUserSelectedLineBreakYes:
//       fLilypondCodeStream <<
//         " \\lineBreak ";
//       break;
//     case msrUserSelectedLineBreakKind::kUserSelectedLineBreakNo:
//       fLilypondCodeStream <<
//         " \\myLineBreak ";
//       break;
//   } // switch

  if (gGlobalLpsr2lilypondOahGroup->getInputLineNumbers ()) {
    // generate the line number as a comment
    fLilypondCodeStream <<
      " %{ <-- line " <<
      elt->getInputLineNumber () <<
      " %} ";
  }

  fLilypondCodeStream <<
    std::endl << std::endl;
}

void lpsr2lilypondTranslator::visitEnd (S_msrLineBreak& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream ss;

      ss <<
        "% --> End visiting msrLineBreak" <<
        ", line " << elt->getInputLineNumber () <<
      std::endl;

      if (traceLpsrVisitors) {
        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << ss.str ();
      }
    }
  }
#endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
void lpsr2lilypondTranslator::visitStart (S_msrPageBreak& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream ss;

      ss <<
        "% --> Start visiting msrPageBreak" <<
        ", line " << elt->getInputLineNumber () <<
      std::endl;

      if (traceLpsrVisitors) {
        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << ss.str ();
      }
    }
  }
#endif // MF_TRACE_IS_ENABLED

  // enforce a page break here, not using \myPageBreak,
  // if this is a user selected page break
  switch (elt->getUserSelectedPageBreakKind ()) {
    case msrUserSelectedPageBreakKind::kUserSelectedPageBreakYes:
      fLilypondCodeStream <<
        "\\pageBreak | % " << elt->getNextBarPuristNumber ();
      break;
    case msrUserSelectedPageBreakKind::kUserSelectedPageBreakNo:
      fLilypondCodeStream <<
//         "\\myPageBreak | % " << elt->getNextBarPuristNumber ();
        "\\pageBreak | % " << elt->getNextBarPuristNumber ();
      break;
  } // switch

  if (gGlobalLpsr2lilypondOahGroup->getInputLineNumbers ()) {
    // generate the line number as a comment
    fLilypondCodeStream <<
      " %{ <-- line " <<
      elt->getInputLineNumber () <<
      " %} ";
  }

  fLilypondCodeStream <<
    std::endl << std::endl;
}

void lpsr2lilypondTranslator::visitEnd (S_msrPageBreak& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream ss;

      ss <<
        "% --> End visiting msrPageBreak" <<
        ", line " << elt->getInputLineNumber () <<
      std::endl;

      if (traceLpsrVisitors) {
        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << ss.str ();
      }
    }
  }
#endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
void lpsr2lilypondTranslator::visitStart (S_msrRepeat& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream ss;

      ss <<
        "% --> Start visiting msrRepeat" <<
        ", line " << elt->getInputLineNumber () <<
      std::endl;

      if (traceLpsrVisitors) {
        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << ss.str ();
      }
    }
  }
#endif // MF_TRACE_IS_ENABLED

  int repeatEndingsNumber =
    elt->getRepeatEndings ().size ();

  if (repeatEndingsNumber == 0)
    repeatEndingsNumber = 2; // implicitly JMI 0.9.70

  fRepeatDescrsStack.push_front (
    lpsrRepeat::create (
      elt,
      repeatEndingsNumber));

  int
    repeatTimes =
      elt->getRepeatTimes ();

  std::stringstream ss;

  fLilypondCodeStream << std::endl;

  ss <<
    "\\repeat volta " <<
    repeatTimes <<
// JMI 0.9.70   fRepeatDescrsStack.back ()->getRepeatEndingsNumber () <<
    " {";

  fLilypondCodeStream <<
    ss.str ();

  if (gGlobalLpsr2lilypondOahGroup->getCommentLilypondStructureBasics ()) {
    fLilypondCodeStream <<
      " % start of repeat";
  }

  fLilypondCodeStream << std::endl;

  ++gIndenter; // decremented in visitEnd (S_msrRepeat& elt)

  if (repeatTimes > 2) {
    fLilypondCodeStream <<
      "<>^\"" << repeatTimes << " times\"" << // JMI 0.9.70
      std::endl;
  }
}

void lpsr2lilypondTranslator::visitEnd (S_msrRepeat& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gLpsrOahGroup->
          getTraceLpsrVisitors ();

    Bool
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream ss;

      ss <<
        "% --> End visiting msrRepeat" <<
        ", line " << elt->getInputLineNumber () <<
      std::endl;

      if (traceLpsrVisitors) {
        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << ss.str ();
      }
    }
  }
#endif // MF_TRACE_IS_ENABLED

  /*
    CAUTION:
      the end of the repeat has to be generated
      before the endings are handled
  */

  if (! fRepeatDescrsStack.back ()->getEndOfRepeatHasBeenGenerated ()) {
    // the end of the repeat has not been generated yet JMI

    --gIndenter;

    fLilypondCodeStream << "}";

    if (gGlobalLpsr2lilypondOahGroup->getCommentLilypondStructureBasics ()) {
      fLilypondCodeStream <<
        " % end of repeat";
    }

    fLilypondCodeStream <<
      std::endl;
  }

  //   --gIndenter; // incremented in visitStart (S_msrRepeat& elt) JMI ???

  fRepeatDescrsStack.pop_front ();
}

//________________________________________________________________________
void lpsr2lilypondTranslator::visitStart (S_msrRepeatCommonPart& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gLpsrOahGroup->
          getTraceLpsrVisitors ();

    Bool
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream ss;

      ss <<
        "% --> Start visiting msrRepeatCommonPart" <<
        ", line " << elt->getInputLineNumber () <<
      std::endl;

      if (traceLpsrVisitors) {
        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << ss.str ();
      }
    }
  }
#endif // MF_TRACE_IS_ENABLED
}

void lpsr2lilypondTranslator::visitEnd (S_msrRepeatCommonPart& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gLpsrOahGroup->
          getTraceLpsrVisitors ();

    Bool
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream ss;

      ss <<
        "% --> End visiting msrRepeatCommonPart" <<
        ", line " << elt->getInputLineNumber () <<
      std::endl;

      if (traceLpsrVisitors) {
        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << ss.str ();
      }
    }
  }
#endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
void lpsr2lilypondTranslator::visitStart (S_msrRepeatEnding& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gLpsrOahGroup->
          getTraceLpsrVisitors ();

    Bool
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream ss;

      ss <<
        "% --> Start visiting msrRepeatEnding" <<
        ", line " << elt->getInputLineNumber () <<
      std::endl;

      if (traceLpsrVisitors) {
        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << ss.str ();
      }
    }
  }
#endif // MF_TRACE_IS_ENABLED

  fRepeatDescrsStack.back ()->
    incrementRepeatEndingsCounter ();

  int
    repeatEndingInternalNumber =
      elt->getRepeatEndingInternalNumber ();

  if (repeatEndingInternalNumber == 1) {
    --gIndenter;

    // the first repeat ending is in charge of
    // outputting the end of the repeat
    fLilypondCodeStream << '}';

    if (gGlobalLpsr2lilypondOahGroup->getCommentLilypondStructureBasics ()) {
      fLilypondCodeStream <<
        " % end of repeat" <<
        std::endl;
    }

    fRepeatDescrsStack.back ()->
      setEndOfRepeatHasBeenGenerated ();

    // the first repeat ending is in charge of
    // outputting the start of the alternative
    fLilypondCodeStream <<
      std::endl << std::endl <<
      "\\alternative {";

    if (gGlobalLpsr2lilypondOahGroup->getCommentLilypondStructureBasics ()) {
      fLilypondCodeStream <<
        " % start of alternative";
    }

    fLilypondCodeStream <<
      std::endl;

    ++gIndenter;
  }

  // output the start of the ending
  switch (elt->getRepeatEndingKind ()) {
    case msrRepeatEndingKind::kRepeatEndingHooked:
      fLilypondCodeStream <<
        '{';

      if (gGlobalLpsr2lilypondOahGroup->getCommentLilypondStructureBasics ()) {
        fLilypondCodeStream <<
          " % start of repeat hooked ending";
      }

      fLilypondCodeStream <<
        std::endl;
      break;

    case msrRepeatEndingKind::kRepeatEndingHookless:
      fLilypondCodeStream <<
        '{';

      if (gGlobalLpsr2lilypondOahGroup->getCommentLilypondStructureBasics ()) {
        fLilypondCodeStream <<
          " % start of repeat hookless ending";
      }

    fLilypondCodeStream <<
      std::endl;
      break;
  } // switch

  ++gIndenter; // decremented in visitEnd (S_msrRepeatEnding&)

  // generate the repeat ending number if any
  std::string
    repeatEndingNumber =
      elt->getRepeatEndingNumber ();

  if (! repeatEndingNumber.empty ()) {
  /* only if numbers differ JMI ???
    if (repeatEndingInternalNumber == 1) {
      fLilypondCodeStream <<
        "\\set Score.repeatCommands = #'((volta \"" <<
        repeatEndingNumber <<
        "\"))" <<
        std::endl;
    }
    else {
      fLilypondCodeStream <<
        "\\set Score.repeatCommands = #'(end-repeat (volta \"" <<
        repeatEndingNumber <<
        "\"))" <<
        std::endl;
    }
    */
  }
}

void lpsr2lilypondTranslator::visitEnd (S_msrRepeatEnding& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gLpsrOahGroup->
          getTraceLpsrVisitors ();

    Bool
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream ss;

      ss <<
        "% --> End visiting msrRepeatEnding" <<
        ", line " << elt->getInputLineNumber () <<
      std::endl;

      if (traceLpsrVisitors) {
        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << ss.str ();
      }
    }
  }
#endif // MF_TRACE_IS_ENABLED

  --gIndenter;  // incremented in visitStart (S_msrRepeatEnding&)

  // output the end of the ending
  fLilypondCodeStream << std::endl;

  switch (elt->getRepeatEndingKind ()) {
    case msrRepeatEndingKind::kRepeatEndingHooked:
      fLilypondCodeStream <<
        '}' << " % HAHA_1";

      if (gGlobalLpsr2lilypondOahGroup->getCommentLilypondStructureBasics ()) {
        fLilypondCodeStream <<
          " % end of repeat hooked ending";
      }
      break;

    case msrRepeatEndingKind::kRepeatEndingHookless:
      fLilypondCodeStream <<
        '}' << " % HAHA_2";

      if (gGlobalLpsr2lilypondOahGroup->getCommentLilypondStructureBasics ())   {
        fLilypondCodeStream <<
          " % end of repeat hookless ending";
      }

      fLilypondCodeStream <<
        std::endl << std::endl;
      break;
  } // switch

  fLilypondCodeStream <<
    std::endl;

/* JMI 0.9.70
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalLpsr2lilypondOahGroup->getCommentLilypondRepeats ()) {
    fLilypondCodeStream <<
      "% ===**** fRepeatDescrsStack.back (): '" <<
      fRepeatDescrsStack.back ()->asString () <<
      '\'' <<
      std::endl;

      if (traceLpsrVisitors) {
        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << ss.str ();
      }
    }
  }
#endif // MF_TRACE_IS_ENABLED
*/

  if (
    fRepeatDescrsStack.back ()->getRepeatEndingsCounter ()
      ==
    fRepeatDescrsStack.back ()->getRepeatEndingsNumber ()
  ) {

     --gIndenter; // JMI 0.9.70

    // the last repeat ending is in charge of
    // outputting the end of the alternative
    fLilypondCodeStream <<
      '}';

    if (gGlobalLpsr2lilypondOahGroup->getCommentLilypondStructureBasics ())   {
      fLilypondCodeStream <<
        " % end of alternative";
    }

    fLilypondCodeStream <<
      std::endl << std::endl;
  }
}

//________________________________________________________________________
void lpsr2lilypondTranslator::visitStart (S_msrRehearsalMark& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream ss;

      ss <<
        "% --> Start visiting msrRehearsalMark" <<
        ", line " << elt->getInputLineNumber () <<
      std::endl;

      if (traceLpsrVisitors) {
        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << ss.str ();
      }
    }
  }
#endif // MF_TRACE_IS_ENABLED

  fLilypondCodeStream << std::endl;

  switch (elt->getRehearsalMarkPlacementKind ()) {
    case msrPlacementKind::kPlacement_UNKNOWN_:
      break;
    case msrPlacementKind::kPlacementAbove:
      break;
    case msrPlacementKind::kPlacementBelow:
      fLilypondCodeStream <<
        "\\once\\override Score.RehearsalMark.direction = #DOWN";
      break;
    } // switch

  fLilypondCodeStream <<
    std::endl <<
    "\\mark\\markup { ";

  switch (elt->getRehearsalMarkKind ()) { // JMI 0.9.70
    case msrRehearsalMarkKind::kRehearsalMarkNone:
      fLilypondCodeStream <<
        "\\box"; // default value
      break;
    case msrRehearsalMarkKind::kRehearsalMarkRectangle:
      fLilypondCodeStream <<
        "\\box";
      break;
    case msrRehearsalMarkKind::kRehearsalMarkSquare:
      fLilypondCodeStream <<
        "\\box";
      break;
    case msrRehearsalMarkKind::kRehearsalMarkOval:
      fLilypondCodeStream <<
        "\\oval";
      break;
    case msrRehearsalMarkKind::kRehearsalMarkCircle:
      fLilypondCodeStream <<
        "\\circle";
      break;
    case msrRehearsalMarkKind::kRehearsalMarkBracket:
      fLilypondCodeStream <<
        "\\bracket";
      break;
    case msrRehearsalMarkKind::kRehearsalMarkTriangle:
      fLilypondCodeStream <<
        "%{ \\triangle??? %} ";
      break;
    case msrRehearsalMarkKind::kRehearsalMarkDiamond:
      fLilypondCodeStream <<
        "%{ \\diamond??? %} ";
      break;
    case msrRehearsalMarkKind::kRehearsalMarkPentagon:
      fLilypondCodeStream <<
        "%{ \\pentagon??? %} ";
      break;
    case msrRehearsalMarkKind::kRehearsalMarkHexagon:
      fLilypondCodeStream <<
        "%{ \\hexagon??? %} ";
      break;
    case msrRehearsalMarkKind::kRehearsalMarkHeptagon:
      fLilypondCodeStream <<
        "%{ \\heptagon??? %} ";
      break;
    case msrRehearsalMarkKind::kRehearsalMarkOctagon:
      fLilypondCodeStream <<
        "%{ \\octagon??? %} ";
      break;
    case msrRehearsalMarkKind::kRehearsalMarkNonagon:
      fLilypondCodeStream <<
        "%{ \\nonagon??? %} ";
      break;
    case msrRehearsalMarkKind::kRehearsalMarkDecagon:
      fLilypondCodeStream <<
        "%{ \\decagon??? %} ";
      break;
  } // switch

  fLilypondCodeStream <<
    " { \"" <<
    elt->getRehearsalMarkText () <<
    "\"" " } }" <<
    std::endl;
}

void lpsr2lilypondTranslator::visitEnd (S_msrRehearsalMark& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream ss;

      ss <<
        "% --> End visiting msrRehearsalMark" <<
        ", line " << elt->getInputLineNumber () <<
      std::endl;

      if (traceLpsrVisitors) {
        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << ss.str ();
      }
    }
  }
#endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
void lpsr2lilypondTranslator::visitStart (S_msrMeasureRepeat& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream ss;

      ss <<
        "% --> Start visiting msrMeasureRepeat" <<
        ", line " << elt->getInputLineNumber () <<
      std::endl;

      if (traceLpsrVisitors) {
        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << ss.str ();
      }
    }
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_TRACE_IS_ENABLED
  int repeatReplicasMeasuresNumber =
    elt->fetchMeasureRepeatReplicasMeasuresNumber ();
#endif // MF_TRACE_IS_ENABLED

  int replicasNumber =
    elt->fetchMeasureRepeatReplicasNumber ();

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalLpsr2lilypondOahGroup->getCommentLilypondMeasureRepeats ()) {
    int repeatPatternMeasuresNumber =
      elt->fetchMeasureRepeatPatternMeasuresNumber ();

    fLilypondCodeStream <<
      "% measures repeat, line " << elt->getInputLineNumber () << ":" <<
      std::endl;

    int fieldWidth = 24;

    fLilypondCodeStream <<
      std::left <<
      std::setw (fieldWidth) <<
      "% repeatPatternMeasuresNumber" << ": " << repeatPatternMeasuresNumber <<
      std::endl <<
      std::setw (fieldWidth) <<
      "% repeatReplicasMeasuresNumber" << ": " << repeatReplicasMeasuresNumber <<
      std::endl <<
      std::setw (fieldWidth) <<
      "% replicasNumber" << ": " << replicasNumber <<
      std::endl;
  }
#endif // MF_TRACE_IS_ENABLED

  if (gGlobalLpsr2lilypondOahGroup->getCommentLilypondStructureBasics ()) {
    fLilypondCodeStream <<
      " % start of measures repeat" <<
      mfSingularOrPlural (
        elt->fetchMeasureRepeatReplicasNumber (),
        "replica",
        "replicas") <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }

  fLilypondCodeStream <<
    std::endl << std::endl <<
    "\\repeat percent " <<
    replicasNumber + 1 <<
     " { " <<
    std::endl;

  ++gIndenter; // decremented in visitEnd (S_msrMeasureRepeat&)
}

void lpsr2lilypondTranslator::visitEnd (S_msrMeasureRepeat& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream ss;

      ss <<
        "% --> End visiting msrMeasureRepeat" <<
        ", line " << elt->getInputLineNumber () <<
      std::endl;

      if (traceLpsrVisitors) {
        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << ss.str ();
      }
    }
  }
#endif // MF_TRACE_IS_ENABLED

  --gIndenter; // incremented in visitStart (S_msrMeasureRepeat&)

  fLilypondCodeStream <<
    std::endl << std::endl <<
    " }" <<
    std::endl;

  if (gGlobalLpsr2lilypondOahGroup->getCommentLilypondStructureBasics ()) {
    fLilypondCodeStream <<
      "% end of measures repeat" <<
      mfSingularOrPlural (
        elt->fetchMeasureRepeatReplicasNumber (),
        "replica",
        "replicas") <<
      ", line " << elt->getInputLineNumber () <<
      std::endl << std::endl;
  }
}

void lpsr2lilypondTranslator::visitStart (S_msrMeasureRepeatPattern& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream ss;

      ss <<
        "%--> Start visiting msrMeasureRepeatPattern" <<
        std::endl;

      if (traceLpsrVisitors) {
        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << ss.str ();
      }
    }
  }
#endif // MF_TRACE_IS_ENABLED
}

void lpsr2lilypondTranslator::visitEnd (S_msrMeasureRepeatPattern& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream ss;

      ss <<
        "%--> End visiting msrMeasureRepeatPattern" <<
        std::endl;

      if (traceLpsrVisitors) {
        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << ss.str ();
      }
    }
  }
#endif // MF_TRACE_IS_ENABLED
}

void lpsr2lilypondTranslator::visitStart (S_msrMeasureRepeatReplicas& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream ss;

      ss <<
        "%--> Start visiting msrMeasureRepeatReplicas" <<
        std::endl;

      if (traceLpsrVisitors) {
        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << ss.str ();
      }
    }
  }
#endif // MF_TRACE_IS_ENABLED

  // output the start of the ending
  fLilypondCodeStream << "{";

  if (gGlobalLpsr2lilypondOahGroup->getCommentLilypondStructureBasics ()) {
    fLilypondCodeStream <<
      " % start of measures repeat replicas" <<
      std::endl;
  }

  fLilypondCodeStream << std::endl;

  ++gIndenter; // decremented in visitEnd (S_msrMeasureRepeat&)
}

void lpsr2lilypondTranslator::visitEnd (S_msrMeasureRepeatReplicas& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream ss;

      ss <<
        "%--> End visiting msrMeasureRepeatReplicas" <<
        std::endl;

      if (traceLpsrVisitors) {
        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << ss.str ();
      }
    }
  }
#endif // MF_TRACE_IS_ENABLED

  --gIndenter; // incremented in visitStart (S_msrMeasureRepeatReplicas&)
}

//________________________________________________________________________
void lpsr2lilypondTranslator::visitStart (S_msrMultipleMeasureRest& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream ss;

      ss <<
        "% --> Start visiting msrMultipleMeasureRest" <<
        ", line " << elt->getInputLineNumber () <<
      std::endl;

      if (traceLpsrVisitors) {
        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << ss.str ();
      }
    }
  }
#endif // MF_TRACE_IS_ENABLED

  int measuresNumber =
    elt->getMeasuresNumber ();

  if (gGlobalLpsr2lilypondOahGroup->getCommentLilypondStructureBasics ()) {
    fLilypondCodeStream <<
      "% start of multiple measure rests" <<
      mfSingularOrPlural (
        measuresNumber,
        "measure",
        "measures") <<
      ", line " << elt->getInputLineNumber () <<
      std::endl << std::endl;

    ++gIndenter; // decremented in visitEnd (S_msrMultipleMeasureRest&)
  }

  if (
    gGlobalLpsr2lilypondOahGroup->
      getMultipleMeasureRestsExpandLimitAtom ()->getSelected ()
  ) {
    fLilypondCodeStream <<
      "\\override MultipleMeasureRest.expand-limit = " <<
      gGlobalLpsr2lilypondOahGroup->getMultipleMeasureRestsExpandLimit () <<
      std::endl;
  }

  fLilypondCodeStream <<
    "\\compressMMRests { ";
    if (gGlobalLpsr2lilypondOahGroup->getCommentLilypondMeasures ()) {
      fLilypondCodeStream << "%{ CC %} "; // JMI 0.9.64
    }
  fLilypondCodeStream <<
    std::endl;
  ++gIndenter;

  fOnGoingMultipleMeasureRests = true;
}

void lpsr2lilypondTranslator::visitEnd (S_msrMultipleMeasureRest& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream ss;

      ss <<
        "% --> End visiting msrMultipleMeasureRest" <<
        ", line " << elt->getInputLineNumber () <<
      std::endl;

      if (traceLpsrVisitors) {
        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << ss.str ();
      }
    }
  }
#endif // MF_TRACE_IS_ENABLED

  int measuresNumber =
    elt->getMeasuresNumber ();

  // start counting measures
  fRemainingMeasureRestsNumber =
    elt->getMeasuresNumber ();

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMultipleMeasureRests ()) {
    std::stringstream ss;

    ss <<
      std::endl <<
      "--> fRemainingMeasureRestsNumber: " <<
      fRemainingMeasureRestsNumber;

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // get multiple measure rests sounding notes JMI USELESS 0.9.63
  mfWholeNotes
    multipleMeasureRestsMeasureSoundingNotes =
      elt->fetchMultipleMeasureRestMeasureSoundingNotes ();

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMultipleMeasureRests ()) {
    std::stringstream ss;

    ss <<
      std::endl <<
      "--> multipleMeasureRestsMeasureSoundingNotes: " <<
      multipleMeasureRestsMeasureSoundingNotes;

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // generate multiple measure rests only now, in case there are
  // clef, keys or times before them in the first measure
  std::string
    measureRestsWholeNoteAsLilypondString =
      multipleMeasureRestsWholeNoteAsLilypondString (
        elt->getInputLineNumber (),
        multipleMeasureRestsMeasureSoundingNotes);

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMultipleMeasureRests ()) {
    std::stringstream ss;

    ss <<
      std::endl <<
      "--> measureRestsWholeNoteAsLilypondString: " <<
      measureRestsWholeNoteAsLilypondString;

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fLilypondCodeStream <<
    'R' <<
    measureRestsWholeNoteAsLilypondString;

  if (gGlobalLpsr2lilypondOahGroup->getInputLineNumbers ()) {
    // generate information and line number as a comment
    fLilypondCodeStream <<
      " %{ <-- line " <<
      elt->getInputLineNumber () <<
      " %} ";
  }

  if (gGlobalLpsr2lilypondOahGroup->getNotesComments ()) {
    // generate information and line number as a comment
    fLilypondCodeStream <<
      "%{ multiple measure rest %} ";
  }

  // wait until all measures have be visited
  // before the bar check is generated // JMI ???

  // now we can generate the bar check
  fLilypondCodeStream <<
    " | % ";
//     " | % " <<
//     elt->getMultipleMeasureRestLastMeasurePuristMeasureNumber () + 1;

/* TO BE FINALIZED JMI
    if (gGlobalLpsr2lilypondOahGroup->getOriginalMeasureNumbers ()) {
      // generate the original MusicXML measure number as a comment
      fLilypondCodeStream <<
        " (line: " << measureElement->getInputLineNumber () << ")";
    }
*/

  fLilypondCodeStream <<
    std::endl;

  if (gGlobalLpsr2lilypondOahGroup->getCommentLilypondStructureBasics ()) {
    --gIndenter; // incremented in visitStart (S_msrMultipleMeasureRest&)

    fLilypondCodeStream <<
      "% end of multiple measure rests" <<
      mfSingularOrPlural (
        measuresNumber,
        "measure",
        "measures") <<
      ", line " << elt->getInputLineNumber () <<
      std::endl << std::endl;
  }

  --gIndenter;

  fLilypondCodeStream << "}";
  if (gGlobalLpsr2lilypondOahGroup->getCommentLilypondMeasures ()) {
    fLilypondCodeStream <<
      " %{ compressMMRests %}";
  }
  fLilypondCodeStream <<
    std::endl;

  fOnGoingMultipleMeasureRests = false;
}

//________________________________________________________________________
void lpsr2lilypondTranslator::visitStart (S_msrMidiTempo& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gLpsrOahGroup->
          getTraceLpsrVisitors (),
      traceMidi =
        gTraceOahGroup->
          getTraceMidi (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || traceMidi || generateMsrVisitingInformation) {
      std::stringstream ss;

      ss <<
        "% --> Start visiting msrMidiTempo" <<
        ", line " << elt->getInputLineNumber () <<
      std::endl;

      if (traceLpsrVisitors) {
        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << ss.str ();
      }
    }
  }
#endif // MF_TRACE_IS_ENABLED

  if (gGlobalLpsr2lilypondOahGroup->getNoMidi ()) {
    fLilypondCodeStream <<
      "%{" <<
      std::endl;

    ++gIndenter;
  }

  fLilypondCodeStream <<
    "\\midi {" <<
    std::endl;

  ++gIndenter;

  if (gGlobalLpsr2lilypondOahGroup->getNoMidi ()) {
    fLilypondCodeStream <<
      "% ";
  }

  fLilypondCodeStream <<
    "\\tempo " <<
    elt->getMidiTempoNotesDuration () << // BLARK 0.9.71
    " = " <<
    elt->getMidiTempoPerSecond () <<
    std::endl;

  --gIndenter;

  fLilypondCodeStream <<
    '}' <<
    std::endl;

  if (gGlobalLpsr2lilypondOahGroup->getNoMidi ()) {
    --gIndenter;

    fLilypondCodeStream <<
      "%}" <<
      std::endl;
  }
}

void lpsr2lilypondTranslator::visitEnd (S_msrMidiTempo& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream ss;

      ss <<
        "% --> End visiting msrMidiTempo" <<
        ", line " << elt->getInputLineNumber () <<
      std::endl;

      if (traceLpsrVisitors) {
        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << ss.str ();
      }
    }
  }
#endif // MF_TRACE_IS_ENABLED
}


} // namespace


/* TOO EARLY FOR ALL OF THEM??? JMI
  // generate the note articulations if any
  if (! fOnGoingChord) {
    const std::list <S_msrArticulation>&
      noteArticulationsList =
        elt->getNoteArticulationsList ();

    if (! noteArticulationsList.empty ()) {
      std::list <S_msrArticulation>::const_iterator i;
      for (
        i = noteArticulationsList.begin ();
        i != noteArticulationsList.end ();
        ++i
      ) {
        S_msrArticulation articulation = (*i);
        switch (articulation->getArticulationKind ()) {
          case msrArticulationKind::kArticulationFermata: // handle this better JMI
            if (
              // fermata?
              S_msrFermata
                fermata =
                  dynamic_cast<msrFermata*>(&(*articulation))
              ) {
              switch (fermata->getFermataTypeKind ()) {
                case msrArticulationFermataType::kArticulationFermataTypeNone:
                  // no placement needed
                  break;
                case msrArticulationFermataType::kArticulationFermataTypeUpright:
                  // no placement needed
                  break;
                case msrArticulationFermataType::kArticulationFermataTypeInverted:
                  fLilypondCodeStream << "_";
                  break;
              } // switch

              switch (fermata->getFermataShapeKind ()) {
                case msrFermataShapeKind::kArticulationFermataNormal:
                  fLilypondCodeStream << "\\fermata ";
                  break;
                case msrFermataShapeKind::kArticulationFermataAngled:
                  fLilypondCodeStream << "\\shortfermata ";
                  break;
                case msrFermataShapeKind::kArticulationFermataSquare:
                  fLilypondCodeStream << "\\longfermata ";
                  break;
              } // switch
            }
            else {
              std::stringstream ss;

              ss <<
                "note articulation '" <<
                articulation->asString () <<
                "' has 'fermata' kind, but is not of type S_msrFermata" <<
                ", line " << articulation->getInputLineNumber ();

              lpsr2lilypondInternalError (
                gServiceRunData->getInputSourceName (),
                articulation->getInputLineNumber (),
                __FILE__, mfInputLineNumber (__LINE__),
                ss.str ());
            }
            break;

          default:
            generateNoteArticulation ((*i));
            fLilypondCodeStream <<
              cLilyPondSpace;
        } // switch
      } // for
    }
  }
  */
