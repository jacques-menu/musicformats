/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <iomanip>      // std::setw, std::setprecision, ...
#include <cmath>
#include <string>

#include "mfAssert.h"
#include "mfServiceRunData.h"
#include "mfStringsHandling.h"

#include "msrAfterGraceNotes.h"
#include "msrArticulations.h"
#include "msrBarChecks.h"
#include "msrBarLines.h"
#include "msrBarNumberChecks.h"
#include "msrBreaks.h"
#include "msrDoubleTremolos.h"
#include "msrGlissandos.h"
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

#include "wae.h"
#include "lpsr2lilypondWae.h"

#include "oahEnableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "tracingOah.h"
#endif

#include "mfConstants.h"
#include "mfIndentedTextOutput.h"

#include "oahOah.h"

#include "lpsr2lilypondOah.h"

#include "lpsrBrowsers.h"

#include "lpsr2lilypondTranslator.h"


namespace MusicFormats
{

// for comments highlighting in the generated Lilypond code
const int commentFieldWidth = 30;

//______________________________________________________________________________
S_lpsrRepeatDescr lpsrRepeatDescr::create (
  const S_msrRepeat& repeat,
  int         repeatEndingsNumber)
{
  lpsrRepeatDescr* o = new
    lpsrRepeatDescr (
      repeat,
      repeatEndingsNumber);
  assert (o != nullptr);
  return o;
}

lpsrRepeatDescr::lpsrRepeatDescr (
  const S_msrRepeat& repeat,
  int         repeatEndingsNumber)
{
  fRepeat = repeat;

  fRepeatEndingsNumber  = repeatEndingsNumber;
  fRepeatEndingsCounter = 0;

  fEndOfRepeatHasBeenGenerated = false; // superflous
}

lpsrRepeatDescr::~lpsrRepeatDescr ()
{}

std::string lpsrRepeatDescr::asString ()
{
  std::stringstream s;

  s <<
  	"[lpsrRepeatDescr"
    ", fRepeatEndingsNumber: " <<
    fRepeatEndingsNumber <<
    ", fRepeatEndingsCounter: " <<
    fRepeatEndingsCounter <<
    ", fEndOfRepeatHasBeenGenerated: " <<
    fEndOfRepeatHasBeenGenerated <<
    ']';

  return s.str ();
}

void lpsrRepeatDescr::print (std::ostream& os)
{
  int fieldWidth = 29;

  os << std::left <<
    std::setw (fieldWidth) <<
    "fRepeatEndingsNumber" << " : " <<
    fRepeatEndingsNumber <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fRepeatEndingsCounter" << " : " <<
    fRepeatEndingsCounter <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fEndOfRepeatHasBeenGenerated" << " : " <<
    fEndOfRepeatHasBeenGenerated <<
    std::endl;
}

std::ostream& operator << (std::ostream& os, const S_lpsrRepeatDescr& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NONE]" << std::endl;
  }

  return os;
}

//________________________________________________________________________
lpsr2lilypondTranslator::lpsr2lilypondTranslator (
  const S_msrOahGroup&  msrOpts,
  const S_lpsrOahGroup& lpsrOpts,
  std::ostream&        lilypondCodeStream)
  : fLilypondCodeStream (
      lilypondCodeStream)
{
  fMsrOahGroup  = msrOpts;
  fLpsrOahGroup = lpsrOpts;

  // the private fields will be (re-)initialized each time
  // translateLpsrToLilypondCode() is called
};

lpsr2lilypondTranslator::~lpsr2lilypondTranslator ()
{}

//________________________________________________________________________
void lpsr2lilypondTranslator::translateLpsrToLilypondCode (
  const S_lpsrScore& theLpsrScore)
{
  // the LPSR score we're visiting
  fVisitedLpsrScore = theLpsrScore;

  // get the msrScore
  S_msrScore
    theMsrScore =
      fVisitedLpsrScore->
        getMsrScore ();

  // inhibit the browsing of grace notes groups before,
  // since they are handled at the note level
  theMsrScore->
    setInhibitGraceNotesGroupsBeforeBrowsing ();

  // inhibit the browsing of grace notes groups after,
  // since they are handled at the note level
  theMsrScore->
    setInhibitGraceNotesGroupsAfterBrowsing ();

  // inhibit the browsing of chords in grace notes groups,
  // since they are handled at the note level
  /* JMI
if (false) // JMI
  theMsrScore->
      setInhibitChordsInGraceNotesGroupsBrowsing ();
*/

  // inhibit the browsing of tuplets in grace notes groups,
  // since they are handled at the note level
// not relevant, since there are not tuplets in grace notes groups ??? JMI
/* JMI
  theMsrScore->
    setInhibitTupletsInGraceNotesGroupsBrowsing ();
*/

  // inhibit the browsing of measure repeats replicas,
  // since Lilypond only needs the repeat measure
  theMsrScore->
    setInhibitMeasureRepeatReplicasBrowsing ();

  // inhibit the browsing of measures repeat replicas,
  // since Lilypond only needs the measure number
// if (false) // JMI CAFE
//   fVisitedLpsrScore->
//     getMsrScore ()->
//       setInhibitMultipleFullBarRestsBrowsing ();

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
      fCurrentOctaveEntryReference = nullptr;
      break;

    case msrOctaveEntryKind::kOctaveEntryFixed:
      // sanity check
      mfAssert (
        __FILE__, __LINE__,
        gGlobalLpsr2lilypondOahGroup->
        getFixedOctaveEntrySemiTonesPitchAndOctave () != nullptr,
       "gGlobalLpsr2lilypondOahGroup->getFixedOctaveEntrySemiTonesPitchAndOctave () is null");

      fCurrentOctaveEntryReference =
        msrNote::createNoteFromSemiTonesPitchAndOctave (
          K_MF_NO_INPUT_LINE_NUMBER,
          gGlobalLpsr2lilypondOahGroup->
            getFixedOctaveEntrySemiTonesPitchAndOctave ());
      break;
  } // switch

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceNotesOctaveEntry ()) {
    gLogStream <<
      "lpsr2lilypondTranslator()" <<
      ", octaveEntryKind is" <<
      msrOctaveEntryKindAsString (
        gGlobalLpsr2lilypondOahGroup->fetchOctaveEntryVariableValue ()) <<
      std::endl <<
      "Initial fCurrentOctaveEntryReference is ";

    if (fCurrentOctaveEntryReference) {
      gLogStream <<
        "'" <<
        fCurrentOctaveEntryReference->asString () <<
        "'";
    }
    else {
      gLogStream << "[NONE]";
    }
    gLogStream << std::endl;
  }
#endif

  // figured bass
  fCurrentFiguredBassFiguresCounter = 0;

  // multiple full-bar rests
  fRemainingMultipleFullBarRestsNumber = 0;

  // measures
  fCurrentVoiceMeasuresCounter = -1;

  // durations
  fLastMetWholeNotes = Rational (0, 1);

  // notes
  fCurrentNotePrinObjectKind =
    msrPrintObjectKind::kPrintObjectYes; // default value

  // articulations
  fCurrentArpeggioDirectionKind =
    msrDirectionKind::kDirectionNone;

  // stems
  fCurrentStemKind = msrStemKind::kStemNeutral; // default value

  // spanners
  fCurrentSpannerPlacementKind =
    msrPlacementKind::kPlacement_NO_;

  // part group blocks
  fNumberOfPartGroupBlocks = -1;
  fPartGroupBlocksCounter  =  0;

  // part blocks
  fNumberOfPartGroupBlockElements = -1;
  fPartGroupBlockElementsCounter  =  0;

  // staff blocks
  fNumberOfStaffBlocksElements = -1;
  fStaffBlocksCounter          =  0;

  if (fVisitedLpsrScore) {
    // browse a msrScore browser
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
        K_MF_NO_INPUT_LINE_NUMBER,
        gGlobalLpsr2lilypondOahGroup->
          getRelativeOctaveEntrySemiTonesPitchAndOctave ());
  }
  else {
    fCurrentOctaveEntryReference = nullptr;
    // the first note in the voice will become the initial reference
  }

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceNotesOctaveEntry ()) {
    gLogStream <<
      "setCurrentOctaveEntryReferenceFromTheLilypondOah()" <<
      ", octaveEntryKind is" <<
      msrOctaveEntryKindAsString (
        gGlobalLpsr2lilypondOahGroup->fetchOctaveEntryVariableValue ()) <<
      std::endl <<
      "Initial fCurrentOctaveEntryReference is ";

    if (fCurrentOctaveEntryReference) {
      gLogStream <<
        "'" <<
        fCurrentOctaveEntryReference->asString () <<
        "'";
    }
    else {
      gLogStream << "[NONE]";
    }
    gLogStream << std::endl;
  }
#endif
}

//________________________________________________________________________
std::string lpsr2lilypondTranslator::msrModeKindAsLilypondString (
  msrModeKind modeKind)
{
  std::string result;

  switch (modeKind) {
    case msrModeKind::kMode_NO_:
      result = "kMode_NO_";
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
  int           inputLineNumber,
  msrOctaveKind absoluteOctaveKind)
{
  std::string result;

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceNotes ()) {
    fLilypondCodeStream <<
      std::endl <<
      "%{ absoluteOctaveKind = " <<
      msrOctaveKindAsString (absoluteOctaveKind) <<
      " %} " <<
      std::endl;
  }
#endif

  // generate LilyPond absolute octave
  switch (absoluteOctaveKind) {
    case msrOctaveKind::kOctave_NO_:
      {
        std::stringstream s;

        s <<
          "absolute octave " <<
          msrOctaveKindAsString (absoluteOctaveKind) <<
          " cannot be translated to LilyPond";

//       result =
//         "%{\\override NoteHead.style = #'cross %}"; // JMI

//         lpsr2lilypondInternalError (
//           gGlobalServiceRunData->getInputSourceName (),
//           inputLineNumber,
//           __FILE__, __LINE__,
//           s.str ());
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
    case msrAlterationKind::kAlteration_NO_:
      result = "alteration???";
      break;
  } // switch

  return result;
}

//________________________________________________________________________
std::string lpsr2lilypondTranslator::lilypondOctaveInRelativeEntryMode (
  const S_msrNote& note)
{
  int inputLineNumber =
    note->getInputLineNumber ();

  // generate LilyPond octave relative to fCurrentOctaveEntryReference

  // in MusicXML, octave number is 4 for the octave starting with middle C
  msrOctaveKind
    noteAbsoluteOctave =
      note->getNoteOctaveKind ();

  msrDiatonicPitchKind
    noteDiatonicPitchKind =
      note->
        noteDiatonicPitchKind (
          inputLineNumber);

  msrDiatonicPitchKind
    referenceDiatonicPitchKind =
      fCurrentOctaveEntryReference->
        noteDiatonicPitchKind (
          inputLineNumber);

  std::string
    referenceDiatonicPitchKindAsString =
      fCurrentOctaveEntryReference->
        noteDiatonicPitchKindAsString (
          inputLineNumber);

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

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceNotesOctaveEntry ()) {
    int fieldWidth = 28;

    gLogStream << std::left <<
      "lilypondOctaveInRelativeEntryMode() 1" <<
      std::endl <<

      std::setw (fieldWidth) <<
      "% noteAboluteDiatonicOrdinal" <<
      " = " <<
      noteAboluteDiatonicOrdinal <<
      std::endl <<

      std::setw (fieldWidth) <<
      "% referenceDiatonicPitchAsString" <<
      " = " <<
      referenceDiatonicPitchKindAsString <<
      std::endl <<
      std::setw (fieldWidth) <<
      "% referenceAbsoluteOctave" <<
       " = " <<
      msrOctaveKindAsString (referenceAbsoluteOctave) <<
      std::endl <<
      std::setw (fieldWidth) <<
      "% referenceAboluteDiatonicOrdinal" <<
      " = " <<
      referenceAboluteDiatonicOrdinal <<
      std::endl << std::endl;
  }
#endif

  std::stringstream s;

  // generate the octaves as needed
  if (noteAboluteDiatonicOrdinal >= referenceAboluteDiatonicOrdinal) {
    noteAboluteDiatonicOrdinal -= 4;
    while (noteAboluteDiatonicOrdinal >= referenceAboluteDiatonicOrdinal) {
      s << "'";
      noteAboluteDiatonicOrdinal -= 7;
    } // while
  }

  else {
    noteAboluteDiatonicOrdinal += 4;
    while (noteAboluteDiatonicOrdinal <= referenceAboluteDiatonicOrdinal) {
      s << ",";
      noteAboluteDiatonicOrdinal += 7;
    } // while
  }

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceNotesOctaveEntry ()) {
    gLogStream <<
      "lilypondOctaveInRelativeEntryMode() 2" <<
      ", result = " << s.str () <<
      std::endl << std::endl;
  }
#endif

  return s.str ();
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

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceNotesOctaveEntry ()) {
    gLogStream << std::left <<
      "% noteAbsoluteOctave = " <<
      msrOctaveKindAsString (noteAbsoluteOctave) <<
      ", referenceAbsoluteOctave = " <<
      msrOctaveKindAsString (referenceAbsoluteOctave) <<
      ", referenceAbsoluteOctave = " <<
      absoluteOctavesDifference <<
      std::endl;
  }
#endif

  std::stringstream s;

  // generate the octaves as needed
  switch (absoluteOctavesDifference) {
    case -12:
      s << ",,,,,,,,,,,,";
      break;
    case -11:
      s << ",,,,,,,,,,,";
      break;
    case -10:
      s << ",,,,,,,,,,";
      break;
    case -9:
      s << ",,,,,,,,,";
      break;
    case -8:
      s << ",,,,,,,,";
      break;
    case -7:
      s << ",,,,,,,";
      break;
    case -6:
      s << ",,,,,,";
      break;
    case -5:
      s << ",,,,,";
      break;
    case -4:
      s << ",,,,";
      break;
    case -3:
      s << ",,,";
      break;
    case -2:
      s << ",,";
      break;
    case -1:
      s << ",";
      break;

    case 0:
      break;

    case 1:
      s << "'";
      break;
    case 2:
      s << "''";
      break;
    case 3:
      s << "'''";
      break;
    case 4:
      s << "''''";
      break;
    case 5:
      s << "'''''";
      break;
    case 6:
      s << "''''''";
      break;
    case 7:
      s << "'''''''";
      break;
    case 8:
      s << "''''''''";
      break;
    case 9:
      s << "'''''''''";
      break;
    case 10:
      s << "''''''''''";
      break;
    case 11:
      s << "'''''''''''";
      break;
    case 12:
      s << "''''''''''''";
      break;
    default:
      s << "!!!";
  } // switch

  return s.str ();
}

//________________________________________________________________________
std::string lpsr2lilypondTranslator::stringTuningAsLilypondString (
  int               inputLineNumber,
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

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceScordaturas ()) {
    int
      getStringTuningNumber =
        stringTuning->
          getStringTuningNumber ();

    gLogStream <<
      std::endl <<
      "%getStringTuningNumber = " <<
      getStringTuningNumber <<
      std::endl <<
      "%stringTuningDiatonicPitchKind = " <<
      msrDiatonicPitchKindAsString (
        stringTuningDiatonicPitchKind) <<
      std::endl <<
      "%stringTuningAlterationKind = " <<
      alterationKindAsLilypondString (
        stringTuningAlterationKind) <<
      std::endl <<
      "%stringTuningOctave = " <<
      msrOctaveKindAsString (stringTuningOctave) <<
      std::endl <<
      "%quarterTonesPitchKind = " <<
      msrQuarterTonesPitchKindAsString (quarterTonesPitchKind) <<
      std::endl <<
      "%msrQuarterTonesPitchKindAsString: " <<
      msrQuarterTonesPitchKindAsStringInLanguage (
        quarterTonesPitchKind,
        gGlobalLpsrOahGroup->
          getLpsrQuarterTonesPitchesLanguageKind ()) <<
      std::endl << std::endl;
  }
#endif

  std::stringstream s;

  s <<
    msrQuarterTonesPitchKindAsStringInLanguage (
      quarterTonesPitchKind,
      gGlobalLpsrOahGroup->
        getLpsrQuarterTonesPitchesLanguageKind ()) <<

    absoluteOctaveAsLilypondString (
      inputLineNumber,
      stringTuningOctave);

  return s.str ();
}

//________________________________________________________________________
std::string lpsr2lilypondTranslator::notePitchAsLilypondString (
  const S_msrNote& note)
{
  int inputLineNumber =
    note->getInputLineNumber ();

std::stringstream s;

  // should an editorial accidental be generated?
  switch (note->getNoteEditorialAccidentalKind ()) {
    case msrEditorialAccidentalKind::kEditorialAccidentalYes:
      s <<
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

  // fetch the quarter tones pitch as std::string
  std::string
    noteQuarterTonesPitchKindAsString =
      msrQuarterTonesPitchKindAsStringInLanguage (
        noteQuarterTonesPitchKind,
        gGlobalLpsrOahGroup->
          getLpsrQuarterTonesPitchesLanguageKind ());

  // get the note quarter tones display pitch
  msrQuarterTonesPitchKind
    noteQuarterTonesDisplayPitchKind =
      note->
        getNoteQuarterTonesDisplayPitchKind ();

  // fetch the quarter tones display pitch as std::string
  std::string
    quarterTonesDisplayPitchKindAsString =
      msrQuarterTonesPitchKindAsStringInLanguage (
        noteQuarterTonesDisplayPitchKind,
        gGlobalLpsrOahGroup->
          getLpsrQuarterTonesPitchesLanguageKind ());

  // generate the pitch
  s <<
    noteQuarterTonesPitchKindAsString;

  // in MusicXML, octave number is 4 for the octave
  // starting with middle C, LilyPond's c'
  msrOctaveKind
    noteAbsoluteOctave =
      note->getNoteOctaveKind ();

  // should an absolute octave be generated?
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceNotesOctaveEntry ()) {
    msrOctaveKind
      noteAbsoluteDisplayOctave =
        note->getNoteDisplayOctaveKind ();

    int fieldWidth = 39;

    gLogStream << std::left <<
      "notePitchAsLilypondString() 1" <<
      std::endl <<

      std::setw (fieldWidth) <<
      "% noteQuarterTonesPitchKindAsString" <<
      " = " <<
      noteQuarterTonesPitchKindAsString <<
      std::endl <<
      std::setw (fieldWidth) <<
      "% quarterTonesDisplayPitchKindAsString" <<
      " = " <<
      quarterTonesDisplayPitchKindAsString <<
      std::endl <<

      std::setw (fieldWidth) <<
      "% noteAbsoluteOctave" <<
      " = " <<
      msrOctaveKindAsString (noteAbsoluteOctave) <<
      std::endl <<
      std::setw (fieldWidth) <<
      "% noteAbsoluteDisplayOctave" <<
      " = " <<
      msrOctaveKindAsString (noteAbsoluteDisplayOctave) <<
      std::endl <<

      std::setw (fieldWidth) <<
      "% line" <<
      " = " <<
      inputLineNumber <<
      std::endl;
  }
#endif

  switch (gGlobalLpsr2lilypondOahGroup->fetchOctaveEntryVariableValue ()) {
    case msrOctaveEntryKind::kOctaveEntryRelative:
      if (! fCurrentOctaveEntryReference) {
#ifdef TRACING_IS_ENABLED
        if (gGlobalTracingOahGroup->getTraceNotesOctaveEntry ()) {
          gLogStream <<
            "notePitchAsLilypondString() 2: fCurrentOctaveEntryReference is null" <<
            " upon note " << note->asString () <<
            ", line " << inputLineNumber <<
            std::endl;
        }
#endif

        // generate absolute octave
        s <<
          absoluteOctaveAsLilypondString (
            inputLineNumber,
            noteAbsoluteOctave);

        // fCurrentOctaveEntryReference will be set to note later
      }
      else {
#ifdef TRACING_IS_ENABLED
        if (gGlobalTracingOahGroup->getTraceNotesOctaveEntry ()) {
          gLogStream <<
            "notePitchAsLilypondString() 3: fCurrentOctaveEntryReference is '" <<
            fCurrentOctaveEntryReference->asString () <<
            "' upon note " << note->asString () <<
            ", line " << inputLineNumber <<
            std::endl;
        }
#endif

        // generate octave relative to mobile fCurrentOctaveEntryReference
        s <<
          lilypondOctaveInRelativeEntryMode (note);
      }
      break;

    case msrOctaveEntryKind::kOctaveEntryAbsolute:
      // generate LilyPond absolute octave
      s <<
        absoluteOctaveAsLilypondString (
          inputLineNumber,
          noteAbsoluteOctave);
      break;

    case msrOctaveEntryKind::kOctaveEntryFixed:
      // generate octave relative to fixed fCurrentOctaveEntryReference
      s <<
        lilypondOctaveInFixedEntryMode (note);
      break;
  } // switch

  // should an accidental be generated? JMI this can be fine tuned with cautionary
  switch (note->getNoteAccidentalKind ()) {
    case msrAccidentalKind::kAccidentalNone:
      break;
    default:
      s <<
        "!";
      break;
  } // switch

  // should an editorial accidental be generated?
  switch (note->getNoteEditorialAccidentalKind ()) {
    case msrEditorialAccidentalKind::kEditorialAccidentalYes:
      s <<
        "?";
      break;
    case msrEditorialAccidentalKind::kEditorialAccidentalNo:
      break;
  } // switch

  // should a cautionary accidental be generated?
  switch (note->getNoteCautionaryAccidentalKind ()) {
    case msrCautionaryAccidentalKind::kCautionaryAccidentalYes:
      s <<
        "?";
      break;
    case msrCautionaryAccidentalKind::kCautionaryAccidentalNo:
      break;
  } // switch

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceNotesOctaveEntry ()) {
    gLogStream << std::endl;
  }
#endif

  return s.str ();
}

//________________________________________________________________________
std::string lpsr2lilypondTranslator::durationAsLilypondString (
  int             inputLineNumber,
  const Rational& wholeNotes)
{
  std::string result; // JMI v0.9.66

  Bool generateExplicitDuration;

  if (wholeNotes != fLastMetWholeNotes) {
    generateExplicitDuration = true;
    fLastMetWholeNotes = wholeNotes;
  }
  else {
    generateExplicitDuration =
      gGlobalLpsr2lilypondOahGroup->
        getAllDurations ();
  }

  if (generateExplicitDuration) {
    result =
      wholeNotesAsLilypondString (
        inputLineNumber,
        wholeNotes);
  }

  return result;
}

std::string lpsr2lilypondTranslator::msrDurationKindAsLilypondString (
  msrDurationKind durationKind)
{
  std::string result;

  switch (durationKind) {
    case msrDurationKind::kDuration_NO_:
      result = "noDuration";
      break;

    case msrDurationKind::kDuration1024th:
      result = "1024";
      break;
    case msrDurationKind::kDuration512th:
      result = "512";
      break;
    case msrDurationKind::kDuration256th:
      result = "256";
      break;
    case msrDurationKind::kDuration128th:
      result = "128";
      break;
    case msrDurationKind::kDuration64th:
      result = "64";
      break;
    case msrDurationKind::kDuration32nd:
      result = "32";
      break;
    case msrDurationKind::kDuration16th:
      result = "16";
      break;
    case msrDurationKind::kDurationEighth:
      result = "8";
      break;
    case msrDurationKind::kDurationQuarter:
      result = "4";
      break;
    case msrDurationKind::kDurationHalf:
      result = "2";
      break;
    case msrDurationKind::kDurationWhole:
      result = "1";
      break;
    case msrDurationKind::kDurationBreve:
      result = "\\breve";
      break;
    case msrDurationKind::kDurationLonga:
      result = "\\longa";
      break;
    case msrDurationKind::kDurationMaxima:
      result = "\\maxima";
      break;
  } // switch

  return result;
}

//________________________________________________________________________
std::string lpsr2lilypondTranslator::pitchedRestAsLilypondString (
  const S_msrNote& note)
{
  int noteInputLineNumber =
    note->getInputLineNumber ();

  std::stringstream s;

  // get the note quarter tones pitch
  msrQuarterTonesPitchKind
    noteQuarterTonesPitchKind =
      note->
        getNoteQuarterTonesPitchKind ();

  // fetch the quarter tones pitch as std::string
  std::string
    noteQuarterTonesPitchKindAsString =
      msrQuarterTonesPitchKindAsStringInLanguage (
        noteQuarterTonesPitchKind,
        gGlobalLpsrOahGroup->getLpsrQuarterTonesPitchesLanguageKind ());

  // get the note quarter tones display pitch
  msrQuarterTonesPitchKind
    noteQuarterTonesDisplayPitchKind =
      note->
        getNoteQuarterTonesDisplayPitchKind ();

  // fetch the quarter tones display pitch as std::string
  std::string
    quarterTonesDisplayPitchKindAsString =
      msrQuarterTonesPitchKindAsStringInLanguage (
        noteQuarterTonesDisplayPitchKind,
        gGlobalLpsrOahGroup->
          getLpsrQuarterTonesPitchesLanguageKind ());

  // generate the display pitch
  s <<
    note->
      noteDisplayPitchKindAsString ();
//    note->notePitchAsString (); JMI
//    quarterTonesDisplayPitchAsString;

  // should an absolute octave be generated?
  msrOctaveKind
    noteAbsoluteDisplayOctave =
      note->getNoteDisplayOctaveKind ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceNotes ()) {
    // in MusicXML, octave number is 4 for the octave starting with middle C
    msrOctaveKind
      noteAbsoluteOctave =
        note->getNoteOctaveKind ();

    int fieldWidth = 28;

    gLogStream << std::left <<
      "pitchedRestAsLilypondString()" <<
      std::endl <<

      std::setw (fieldWidth) <<
      "% noteQuarterTonesPitchKindAsString" <<
      " = " <<
      noteQuarterTonesPitchKindAsString <<
      std::endl <<
      std::setw (fieldWidth) <<
      "% quarterTonesDisplayPitch" <<
      " = " <<
      quarterTonesDisplayPitchKindAsString <<
      std::endl <<

      std::setw (fieldWidth) <<
      "% noteAbsoluteOctave" <<
      " = " <<
      msrOctaveKindAsString (noteAbsoluteOctave) <<
      std::endl <<
      std::setw (fieldWidth) <<
      "% noteAbsoluteDisplayOctave" <<
      " = " <<
      msrOctaveKindAsString (noteAbsoluteDisplayOctave) <<
      std::endl <<

      std::setw (fieldWidth) <<
      "% line" <<
      " = " <<
      noteInputLineNumber <<
      std::endl;
  }
#endif

  switch (gGlobalLpsr2lilypondOahGroup->fetchOctaveEntryVariableValue ()) {
    case msrOctaveEntryKind::kOctaveEntryRelative:
    // generate LilyPond octave relative to fCurrentOctaveEntryReference
    s <<
      lilypondOctaveInRelativeEntryMode (note);
      break;
    case msrOctaveEntryKind::kOctaveEntryAbsolute:
      // generate LilyPond absolute octave
      s <<
        absoluteOctaveAsLilypondString (
          noteInputLineNumber,
          noteAbsoluteDisplayOctave);
      break;
    case msrOctaveEntryKind::kOctaveEntryFixed:
      // generate LilyPond octave relative to fCurrentOctaveEntryReference
      s <<
        lilypondOctaveInFixedEntryMode (note);
      break;
  } // switch

  // generate the skip duration
  s <<
    durationAsLilypondString (
      noteInputLineNumber,
      note->
        getMeasureElementSoundingWholeNotes ());

  // generate the '\rest'
  s <<
    "\\rest ";

  return s.str ();
}

void lpsr2lilypondTranslator::generateNoteHeadColor (
  const S_msrNote& note)
{
  int inputLineNumber =
    note->getInputLineNumber ();

  // has the note alpha RGB color been set?
 if (note->getNoteColorAlphaRGBHasBenSet ()) {
    // get note alpha RGB color
    const msrColorAlphaRGB&
      noteColorAlphaRGB =
        note->getNoteColorAlphaRGB ();

    // get note RGB color
    std::string noteRGB = noteColorAlphaRGB.getColorRGB ();

    // generate code for RGB color
    if (noteRGB.size () == 6) {
      std::string
        noteR = noteRGB.substr (0, 2),
        noteG = noteRGB.substr (2, 2),
        noteB = noteRGB.substr (4, 2);

      fLilypondCodeStream <<
        "\\once \\override NoteHead.color = #(std::map (lambda (x) (/ x 255)) "
        "'(" <<
        "#X" << noteRGB [0] << noteRGB [1] <<
        ' ' <<
        "#X" << noteRGB [2] << noteRGB [3] <<
        ' ' <<
        "#X" << noteRGB [4] << noteRGB [5] <<
        "))" <<
        std::endl;
      }
    else {
      std::stringstream s;

      s <<
        "note RGB color '" <<
        noteRGB <<
        "' is ill-formed" <<
        ", line " << inputLineNumber;

      lpsr2lilypondInternalError (
        gGlobalServiceRunData->getInputSourceName (),
        inputLineNumber,
        __FILE__, __LINE__,
        s.str ());
    }
  }
}

void lpsr2lilypondTranslator::generateNoteLigatures (
  const S_msrNote& note)
{
  const std::list<S_msrLigature>&
    noteLigatures =
      note->getNoteLigatures ();

  if (noteLigatures.size ()) {
    std::list<S_msrLigature>::const_iterator i;
    for (
      i=noteLigatures.begin ();
      i!=noteLigatures.end ();
      ++i
    ) {
      S_msrLigature ligature = (*i);

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

#ifdef TRACING_IS_ENABLED
            if (gGlobalTracingOahGroup->getTraceLigatures ()) {
              gLogStream <<
                "Ligature vertical flipping factore for note '" <<
                note->asString () <<
                "' in voice \"" <<
                noteVoice->getVoiceName () <<
                "\" is " <<
                ligatureVerticalFlippingFactor <<
                ", line " << ligature->getInputLineNumber () <<
                std::endl;
            }
#endif

            // compute ligature start edge height
            float edgeHeightAbsValue = 0.75;

            float       ligatureStartEdgeHeight = 0.0;

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

            // generate the code the edge-height std::pair of values
            fLilypondCodeStream <<
              std::endl <<
              "\\once \\override Staff.LigatureBracket.edge-height = #'(" <<
              std::setprecision (2) <<
              ligatureStartEdgeHeight <<
              " . " <<
              std::setprecision (2) <<
              ligatureEndEdgeHeight <<
              ")" <<
              " %{ " <<
              ligature->getInputLineNumber () <<
              " %}" <<
              std::endl;
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

          fLilypondCodeStream << "\\[ ";
          break;

        case msrLigatureKind::kLigatureContinue:
          break;

        case msrLigatureKind::kLigatureStop:
   // JMI       fLilypondCodeStream << "\\] ";
          break;
      } // switch
    } // for
  }
}

std::string lpsr2lilypondTranslator::stemAsLilypondString (
  msrStemKind stemKind)
{
  std::string result;

  switch (stemKind) {
    case msrStemKind::kStemNeutral:
      result = "\\stemNeutral ";
      break;
    case msrStemKind::kStemUp:
      result = "\\stemUp ";
      break;
    case msrStemKind::kStemDown:
      result = "\\stemDown ";
      break;
    case msrStemKind::kStemDouble: // JMI ???
      break;
  } // switch

  return result;
}

void lpsr2lilypondTranslator::generateStemIfNeededAndUpdateCurrentStemKind (
  const S_msrStem& stem)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceStems ()) {
    gLogStream <<
      "--> generateStemIfNeededAndUpdateCurrentStemKind" <<
      ", stem = " <<
      stem->asShortString () <<
      ", fCurrentStemKind = " <<
      msrStemKindAsString (fCurrentStemKind) <<
      std::endl;
  }
#endif

  if (stem) {
    msrStemKind
      stemKind =
        stem->getStemKind ();

    // should a stem direction command be generated?
    if (
      stemKind != fCurrentStemKind
        &&
      gGlobalLpsr2lilypondOahGroup->getGenerateStemsDirections ()
       // JMI msrStemKind::kStemNeutral ???
    ) {
#ifdef TRACING_IS_ENABLED
      if (gGlobalTracingOahGroup->getTraceStems ()) {
        gLogStream <<
          "--> fCurrentStemKind switches from " <<
          msrStemKindAsString (fCurrentStemKind) <<
          " to " <<
          msrStemKindAsString (stemKind) <<
          std::endl;
      }
#endif

      fLilypondCodeStream <<
        stemAsLilypondString (stemKind);

      fCurrentStemKind = stemKind;
    }
  }
}

void lpsr2lilypondTranslator::generateNoteHead (
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
      case msrNoteHeadKind::kNoteHeadDiamond:
   // JMI     fLilypondCodeStream << "\\tweak style #'diamond ";
        fLilypondCodeStream << "\\harmonic ";
        break;
      case msrNoteHeadKind::kNoteHeadSquare:
        fLilypondCodeStream << "\\tweak style #'la ";
        break;
      case msrNoteHeadKind::kNoteHeadCross:
        fLilypondCodeStream << "\\tweak style #'cross ";
        break;
      case msrNoteHeadKind::kNoteHeadX:
        fLilypondCodeStream << "\\tweak style #'cross %{ x %} ";
        break;
      case msrNoteHeadKind::kNoteHeadCircleX:
        fLilypondCodeStream << "\\tweak style #'xcircle ";
        break;
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
    } // switch
  }
}

void lpsr2lilypondTranslator::generateCoda (const S_msrCoda& coda)
{
  switch (coda->getCodaKind ()) {
    case msrCodaKind::kCodaFirst:
      // generate the coda
      fLilypondCodeStream <<
        "\\mark \\markup { \\musicglyph #\"scripts.coda\" }%{ first %}";
      break;
    case msrCodaKind::kCodaSecond:
      // generate the coda
      fLilypondCodeStream <<
        "\\mark \\markup { \\musicglyph #\"scripts.coda\" }%{ second %}";
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
  if (! fOnGoingChord) {
    // generate the note codas if any
    const std::list<S_msrCoda>&
      noteCodas =
        note->getNoteCodas ();

    if (noteCodas.size ()) {
      std::list<S_msrCoda>::const_iterator i;
      for (i=noteCodas.begin (); i!=noteCodas.end (); ++i) {
        S_msrCoda coda = (*i);

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
    const std::list<S_msrSegno>&
      noteSegnos =
        note->getNoteSegnos ();

    if (noteSegnos.size ()) {
      std::list<S_msrSegno>::const_iterator i;
      for (i=noteSegnos.begin (); i!=noteSegnos.end (); ++i) {
      } // for
    }
  }

  // generate the note head color
  generateNoteHeadColor (note);

  // generate the note ligatures if any
  const std::list<S_msrLigature>&
    noteLigatures =
      note->getNoteLigatures ();

  if (noteLigatures.size ()) {
    generateNoteLigatures (note);
  }

  // generate note stem kind if needed
  if (! fOnGoingChord) {
    S_msrStem
      noteStem =
        note->getNoteStem ();

    if (noteStem) {
      generateStemIfNeededAndUpdateCurrentStemKind (noteStem);
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

  // handling note head
  generateNoteHead (note);

  // generate note spanners if any
  generateBeforeNoteSpannersIfAny (note);
}

void lpsr2lilypondTranslator::generateCodeForNote (
  const S_msrNote& note)
{
  int inputLineNumber =
    note->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  {
    Bool
      traceNotes =
        gGlobalTracingOahGroup->
          getTraceNotes (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (gGlobalTracingOahGroup->getTraceNotes ()) {
      std::stringstream s;

      s <<
        std::endl <<
        "% --> generating code for note " <<
        note->asString () <<
        ", line " << inputLineNumber <<
        std::endl;

        if (traceNotes) {
          gLogStream << s.str ();
        }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << s.str ();
      }
    }
  }
#endif

  ////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////
  // generate the note itself
  ////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////

  ++gIndenter;

  // is this a solo note or rest?
  switch (note->getSoloNoteOrRestInStaffKind ()) {
    case msrSoloNoteOrRestInStaffKind::kSoloNoteOrRestInStaffYes:
      if (! fOnGoingChord) {
        fLilypondCodeStream << "\\once \\oneVoice ";
      }
      break;

    case msrSoloNoteOrRestInStaffKind::kSoloNoteOrRestInStaffPartially:
      {
        std::stringstream s;

        s <<
          "note " <<
          note->asShortStringForTimeView () <<
          "' has been finalized as being partially a solo note or rest, not supported yet" <<
          ", line " << inputLineNumber;

        lpsr2lilypondInternalError (
          gGlobalServiceRunData->getInputSourceName (),
          inputLineNumber,
          __FILE__, __LINE__,
          s.str ());
      }
      break;

    case msrSoloNoteOrRestInStaffKind::kSoloNoteOrRestInStaffNo:
      break;
  } // switch

  // generate the code for the note proper
  switch (note->getNoteKind ()) {
    case msrNoteKind::kNote_NO_:
      break;

    // in measures
    case msrNoteKind::kNoteRegularInMeasure:
      generateCodeForNoteRegularInMeasure (note);
      break;

    case msrNoteKind::kNoteRestInMeasure:
      generateCodeForNoteRestInMeasure (note);
      break;

    case msrNoteKind::kNoteSkipInMeasure:
      generateCodeForNoteSkipInMeasure (note);
      break;

    case msrNoteKind::kNoteUnpitchedInMeasure:
      generateCodeForNoteUnpitchedInMeasure (note);
      break;

    // in chords
    case msrNoteKind::kNoteRegularInChord:
      generateCodeForNoteRegularInChord (note);
      {}
      break;

    // in tuplets
    case msrNoteKind::kNoteRegularInTuplet:
      generateCodeForNoteRegularInTuplet (note);
      break;

    case msrNoteKind::kNoteRestInTuplet:
      generateCodeForNoteRestInTuplet (note);
      break;

    case msrNoteKind::kNoteUnpitchedInTuplet:
      generateCodeForNoteUnpitchedInTuplet (note);
      break;

    // in grace notes groups
    case msrNoteKind::kNoteRegularInGraceNotesGroup:
      generateCodeForNoteRegularInGraceNotesGroup (note);
      break;

    case msrNoteKind::kNoteSkipInGraceNotesGroup:
      generateCodeForNoteSkipInGraceNotesGroup (note);
      break;

    // in chords in grace notes groups
    case msrNoteKind::kNoteInChordInGraceNotesGroup:
      generateCodeForNoteInChordInGraceNotesGroup (note);
      break;

    // in tuplets in grace notes groups
    case msrNoteKind::kNoteInTupletInGraceNotesGroup:
      generateCodeForNoteInTupletInGraceNotesGroup (note);
      break;

    // in double-tremolos
    case msrNoteKind::kNoteInDoubleTremolo:
      generateCodeForNoteInDoubleTremolo (note);
      break;
  } // switch

  fLilypondCodeStream << ' ';

  --gIndenter;
}

void lpsr2lilypondTranslator::generateCodeForNoteRegularInMeasure (
  const S_msrNote& note)
{
  int inputLineNumber =
    note->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  {
    Bool
      traceNotes =
        gGlobalTracingOahGroup->
          getTraceNotes (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceNotes || generateMsrVisitingInformation) {
      std::stringstream s;

      s <<
        std::endl <<
        "% --> generating code for noteRegularInMeasure " <<
        note->asString () <<
        ", line " << inputLineNumber <<
        std::endl;

      if (traceNotes) {
        gLogStream << s.str ();
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << s.str ();
      }
    }
  }
#endif

  // generate the note name
  fLilypondCodeStream <<
    notePitchAsLilypondString (note);

  Rational
    noteSoundingWholeNotes =
      note->
        getMeasureElementSoundingWholeNotes ();

  // generate the note duration
  fLilypondCodeStream <<
    durationAsLilypondString (
      inputLineNumber,
      noteSoundingWholeNotes);

  // handle delayed ornaments if any
  if (note->getNoteDelayedTurnOrnament ()) {
    // c2*2/3 ( s2*1/3\turn) JMI
    // we need the explicit duration in all cases,
    // regardless of gGlobalServiceRunData->getAllDurations ()
    fLilypondCodeStream <<
    //* JMI TOO MUCH
      wholeNotesAsLilypondString (
        inputLineNumber,
        noteSoundingWholeNotes) <<
        //*/
      "*" <<
      gGlobalLpsr2lilypondOahGroup->getDelayedOrnamentsFraction ();
  }

  // generate the tie if any
  {
    S_msrTie noteTie = note->getNoteTie ();

    if (noteTie) {
      if (noteTie->getTieKind () == msrTieKind::kTieStart) {
//        fLilypondCodeStream << " ~ %{ msrNoteKind::kNoteRegularInMeasure %}"; // JMI
      }
    }
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

void lpsr2lilypondTranslator::generateCodeForNoteRestInMeasure (
  const S_msrNote& note)
{
  int inputLineNumber =
    note->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  {
    Bool
      traceNotes =
        gGlobalTracingOahGroup->
          getTraceNotes (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceNotes || generateMsrVisitingInformation) {
      std::stringstream s;

      s <<
        std::endl <<
        "% --> generating code for noteRestInMeasure " <<
        note->asString () <<
        ", line " << inputLineNumber <<
        std::endl;

        if (traceNotes) {
          gLogStream << s.str ();
        }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << s.str ();
      }
    }
  }
#endif

  // get pitched rest status
  Bool noteIsAPitchedRest =
    note->noteIsAPitchedRest ();

  if (noteIsAPitchedRest) {
    // pitched rest
    fLilypondCodeStream <<
      pitchedRestAsLilypondString (note);

    if (gGlobalLpsr2lilypondOahGroup->getInputLineNumbers ()) {
      // generate information and line number as a comment
      fLilypondCodeStream <<
        " %{ line " <<
        note->getInputLineNumber () <<
        " %}";
    }

    if (gGlobalLpsr2lilypondOahGroup->getNotesComments ()) {
      // generate information and line number as a comment
      fLilypondCodeStream <<
        " %{ pitched rest %}";
    }

    // this note is the new relative octave reference
    // (the display quarter tone pitch and octave
    // have been copied to the note octave
    // in the msrNote::msrNote ()ructor,
    // since the note octave is used in relative code generation)
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

  else {
    // unpitched rest
    // get the note sounding whole notes
    Rational
      noteSoundingWholeNotes =
        note->getMeasureElementSoundingWholeNotes ();

    // get note's voice
    S_msrVoice
      noteVoice =
        note->
          fetchNoteUpLinkToVoice ();

    // generate the rest name and duration
    if (note->getNoteOccupiesAFullMeasure ()) {
      // take voice kind into account JMI shouldn't be necessary?
      switch (noteVoice->getVoiceKind ()) {
        case msrVoiceKind::kVoiceKindRegular:
        case msrVoiceKind::kVoiceKindDynamics:
          fLilypondCodeStream <<
            "R";
          break;

        case msrVoiceKind::kVoiceKindHarmonies:
        case msrVoiceKind::kVoiceKindFiguredBass:
          fLilypondCodeStream <<
            "s";
          break;
      } // switch

      fLilypondCodeStream <<
        durationAsLilypondString (
          inputLineNumber,
          noteSoundingWholeNotes);

      if (gGlobalLpsr2lilypondOahGroup->getInputLineNumbers ()) {
        // generate information and line number as a comment
        fLilypondCodeStream <<
          " %{ line " <<
          note->getInputLineNumber () <<
          " %}";
      }

      if (gGlobalLpsr2lilypondOahGroup->getNotesComments ()) {
        // generate information and line number as a comment
        fLilypondCodeStream <<
          " %{ full measure unpitched rest %}";
      }
    }

    else {
      // note does not occupy a full measure
      // take voice kind into account JMI shouldn't be necessary?

      switch (noteVoice->getVoiceKind ()) {
        case msrVoiceKind::kVoiceKindRegular:
        case msrVoiceKind::kVoiceKindDynamics:
          fLilypondCodeStream <<
            "r";
          break;

        case msrVoiceKind::kVoiceKindHarmonies:
        case msrVoiceKind::kVoiceKindFiguredBass:
          fLilypondCodeStream <<
            "s";
          break;
      } // switch

      fLilypondCodeStream <<
        durationAsLilypondString (
          inputLineNumber,
          noteSoundingWholeNotes);

      if (gGlobalLpsr2lilypondOahGroup->getInputLineNumbers ()) {
        // generate information and line number as a comment
        fLilypondCodeStream <<
          " %{ line " <<
          note->getInputLineNumber () <<
          " %}";
      }

      if (gGlobalLpsr2lilypondOahGroup->getNotesComments ()) {
        // generate information and line number as a comment
        fLilypondCodeStream <<
          " %{ non-full measure unpitched rest %}";
      }

/* JMI BOF
      if (fOnGoingVoiceCadenza) { // JMI
        if (noteSoundingWholeNotes != Rational (1, 1)) {
          / * JMI
          // force the generation of the duration if needed
          if (! gGlobalLpsr2lilypondOahGroup->getAllDurations ()) {
            fLilypondCodeStream << // JMI
              wholeNotesAsLilypondString (
                inputLineNumber,
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

    // an unpitched rest is no relative octave reference,
    // the preceding one is kept
  }
}

void lpsr2lilypondTranslator::generateCodeForNoteSkipInMeasure (
  const S_msrNote& note)
{
  int inputLineNumber =
    note->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  {
    Bool
      traceNotes =
        gGlobalTracingOahGroup->
          getTraceNotes (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceNotes || generateMsrVisitingInformation) {
      std::stringstream s;

      s <<
        std::endl <<
        "% --> generating code for noteSkipInMeasure " <<
        note->asString () <<
        ", line " << inputLineNumber <<
        std::endl;

      if (traceNotes) {
        gLogStream << s.str ();
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << s.str ();
      }
    }
  }
#endif

  if (gGlobalLpsr2lilypondOahGroup->getGenerateMeasurePositions ()) {
    // generate the rest name to help pin-point bugs
    fLilypondCodeStream << "r";
  }
  else {
    // generate the skip name
    fLilypondCodeStream << "s";
  }

  // generate the skip duration
  fLilypondCodeStream <<
    durationAsLilypondString (
      inputLineNumber,
      note->
        getMeasureElementSoundingWholeNotes ());

  if (gGlobalLpsr2lilypondOahGroup->getInputLineNumbers ()) {
    // generate information and line number as a comment
    fLilypondCodeStream <<
      " %{ line " <<
      note->getInputLineNumber () <<
      " %}";
  }

  if (gGlobalLpsr2lilypondOahGroup->getNotesComments ()) {
    // generate information and line number as a comment
    fLilypondCodeStream <<
      " %{ note skip in measure %}";
  }

  // a skip is no relative octave reference,
  // the preceding one is kept
}

void lpsr2lilypondTranslator::generateCodeForNoteUnpitchedInMeasure (
  const S_msrNote& note)
{
  int inputLineNumber =
    note->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  {
    Bool
      traceNotes =
        gGlobalTracingOahGroup->
          getTraceNotes (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceNotes || generateMsrVisitingInformation) {
    std::stringstream s;

    s <<
      std::endl <<
      "% --> generating code for noteUnpitchedInMeasure " <<
      note->asString () <<
      ", line " << inputLineNumber <<
      std::endl;

      if (traceNotes) {
        gLogStream << s.str ();
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << s.str ();
      }
    }
  }
#endif

  // generate the note name, "e" by convention
  fLilypondCodeStream <<
      "e";

  Rational
    noteSoundingWholeNotes =
      note->
        getMeasureElementSoundingWholeNotes ();

  // generate the note duration
  fLilypondCodeStream <<
    durationAsLilypondString (
      inputLineNumber,
      noteSoundingWholeNotes);

  // handle delayed ornaments if any
  if (note->getNoteDelayedTurnOrnament ()) {
    // c2*2/3 ( s2*1/3\turn) JMI
    // we need the explicit duration in all cases,
    // regardless of gGlobalServiceRunData->getAllDurations ()
    fLilypondCodeStream <<
      wholeNotesAsLilypondString (
        inputLineNumber,
        noteSoundingWholeNotes) <<
      "*" <<
      gGlobalLpsr2lilypondOahGroup->getDelayedOrnamentsFraction ();
  }

/* JMI
  // generate the tie if any
  {
    S_msrTie noteTie = note->getNoteTie ();

    if (noteTie) {
      if (noteTie->getTieKind () == msrTieKind::kTieStart) {
        fLilypondCodeStream <<
          " %{ line " << inputLineNumber << "%}" <<
          " ~  %{ kUnpitchedNote %}"; // JMI spaces???
      }
    }
  }
  */
}

void lpsr2lilypondTranslator::generateCodeForNoteRegularInChord (
  const S_msrNote& note)
{
#ifdef TRACING_IS_ENABLED
  int inputLineNumber =
    note->getInputLineNumber ();

  {
    Bool
      traceNotes =
        gGlobalTracingOahGroup->
          getTraceNotes (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceNotes || generateMsrVisitingInformation) {
      std::stringstream s;

      s <<
        std::endl <<
        "% --> generating code for noteRegularInChord " <<
        note->asString () <<
        ", line " << inputLineNumber <<
        std::endl;

      if (traceNotes) {
        gLogStream << s.str ();
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << s.str ();
      }
    }
  }
#endif

  // generate the note name
  fLilypondCodeStream <<
    notePitchAsLilypondString (note);

  // don't print the note duration,
  // it will be printed for the chord itself

  // don't print the std::string number if any,
  // it should appear after the chord itself
  const std::list<S_msrTechnicalWithInteger>&
    chordMemberNoteTechnicalsWithIntegers =
      note->getNoteTechnicalWithIntegers ();

  if (chordMemberNoteTechnicalsWithIntegers.size ()) {
    std::list<S_msrTechnicalWithInteger>::const_iterator i;
    for (
      i=chordMemberNoteTechnicalsWithIntegers.begin ();
      i!=chordMemberNoteTechnicalsWithIntegers.end ();
      ++i
    ) {
      S_msrTechnicalWithInteger
        technicalWithInteger = (*i);

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

void lpsr2lilypondTranslator::generateCodeForNoteRegularInTuplet (
  const S_msrNote& note)
{
  int inputLineNumber =
    note->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  {
    Bool
      traceNotes =
        gGlobalTracingOahGroup->
          getTraceNotes (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceNotes || generateMsrVisitingInformation) {
			std::stringstream s;

			s <<
				std::endl <<
				"% --> generating code for noteRegularInTuplet " <<
				note->asString () <<
				", line " << inputLineNumber <<
				std::endl;

				if (traceNotes) {
					gLogStream << s.str ();
				}

				if (generateMsrVisitingInformation) {
					fLilypondCodeStream << s.str ();
      }
    }
  }
#endif

  if (gGlobalLpsr2lilypondOahGroup->getIndentTuplets ()) {
    fLilypondCodeStream << std::endl;
  }

  // generate the note name
  fLilypondCodeStream <<
    notePitchAsLilypondString (note);

  // generate the note display duration
  fLilypondCodeStream <<
    durationAsLilypondString (
      inputLineNumber,
      note->
        getNoteDisplayWholeNotes ());

/* JMI
  // generate the tie if any
  {
    S_msrTie noteTie = note->getNoteTie ();

    if (noteTie) {
      if (noteTie->getTieKind () == msrTieKind::kTieStart) {
        fLilypondCodeStream <<
          " %{ l ine " << inputLineNumber << " %}" <<
          "~  %{ kTupletMemberNote %}"; // JMI spaces???
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

void lpsr2lilypondTranslator::generateCodeForNoteRestInTuplet (
  const S_msrNote& note)
{
  int inputLineNumber =
    note->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  {
    Bool
      traceNotes =
        gGlobalTracingOahGroup->
          getTraceNotes (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceNotes || generateMsrVisitingInformation) {
    std::stringstream s;

    s <<
      std::endl <<
      "% --> generating code for noteRestInTuplet " <<
      note->asString () <<
      ", line " << inputLineNumber <<
      std::endl;

      if (traceNotes) {
        gLogStream << s.str ();
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << s.str ();
      }
    }
  }
#endif

  if (gGlobalLpsr2lilypondOahGroup->getIndentTuplets ()) {
    fLilypondCodeStream << std::endl;
  }

  // generate the note name
  fLilypondCodeStream <<
    std::string (
      note->getNoteOccupiesAFullMeasure ()
        ? "s" // JMI ??? "R"
        : "r");

  // generate the note display duration
  fLilypondCodeStream <<
    durationAsLilypondString (
      inputLineNumber,
      note->
        getNoteDisplayWholeNotes ());

  if (gGlobalLpsr2lilypondOahGroup->getInputLineNumbers ()) {
    // generate information and line number as a comment
    fLilypondCodeStream <<
      " %{ line " <<
      note->getInputLineNumber () <<
      " %}";
  }

  if (gGlobalLpsr2lilypondOahGroup->getNotesComments ()) {
    // generate information and line number as a comment
    fLilypondCodeStream <<
      " %{ rest in tuplet %}";
  }

/* JMI
  // generate the tie if any
  {
    S_msrTie noteTie = note->getNoteTie ();

    if (noteTie) {
      if (noteTie->getTieKind () == msrTieKind::kTieStart) {
        fLilypondCodeStream <<
          " %{ line " << inputLineNumber << " %}" <<
          "~  %{ kTupletMemberNote %}"; // JMI spaces???
      }
    }
  }
*/

  // a rest is no relative octave reference,
}

void lpsr2lilypondTranslator::generateCodeForNoteUnpitchedInTuplet (
  const S_msrNote& note)
{
  int inputLineNumber =
    note->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  {
    Bool
      traceNotes =
        gGlobalTracingOahGroup->
          getTraceNotes (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceNotes || generateMsrVisitingInformation) {
      std::stringstream s;

      s <<
        std::endl <<
        "% --> generating code for noteUnpitchedInTuplet " <<
        note->asString () <<
        ", line " << inputLineNumber <<
        std::endl;

      if (traceNotes) {
        gLogStream << s.str ();
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << s.str ();
      }
    }
  }
#endif

  if (gGlobalLpsr2lilypondOahGroup->getIndentTuplets ()) {
    fLilypondCodeStream << std::endl;
  }

  // generate the note name
  fLilypondCodeStream <<
    "e"; // by convention

  // generate the note (display) duration
  fLilypondCodeStream <<
    durationAsLilypondString (
      inputLineNumber,
      note->
        getNoteDisplayWholeNotes ());

/* JMI
  // generate the tie if any
  {
    S_msrTie noteTie = note->getNoteTie ();

    if (noteTie) {
      if (noteTie->getTieKind () == msrTieKind::kTieStart) {
        fLilypondCodeStream <<
          " %{ line " << inputLineNumber << " %}" <<
          "~  %{ kTupletUnpitchedMemberNote %}";
      }
    }
  }
  */
}

void lpsr2lilypondTranslator::generateCodeForNoteRegularInGraceNotesGroup (
  const S_msrNote& note)
{
#ifdef TRACING_IS_ENABLED
  int inputLineNumber =
    note->getInputLineNumber ();

  {
    Bool
      traceNotes =
        gGlobalTracingOahGroup->
          getTraceNotes (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceNotes || generateMsrVisitingInformation) {
      std::stringstream s;

      s <<
        std::endl <<
        "% --> generating code for noteRegularInGraceNotesGroup " <<
        note->asString () <<
        ", line " << inputLineNumber <<
        std::endl;

      if (traceNotes) {
        gLogStream << s.str ();
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << s.str ();
      }
    }
  }
#endif

  // generate the note name
  fLilypondCodeStream <<
    notePitchAsLilypondString (note);

  // generate the grace note's graphic duration
  fLilypondCodeStream <<
    msrDurationKindAsLilypondString (
      note->
        getNoteGraphicDurationKind ());

  // generate the dots if any JMI ???
  for (int i = 0; i < note->getNoteDotsNumber (); ++i) {
    fLilypondCodeStream << ".";
  } // for

  // don't print the tie if any, 'acciacattura takes care of it
  /*
  {
    S_msrTie noteTie = note->getNoteTie ();

    if (noteTie) {
      if (noteTie->getTieKind () == msrTieKind::kTieStart) {
        fLilypondCodeStream <<
          " %{ line " << inputLineNumber << " %}" <<
          "~  %{ kGraceNote %}";
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

void lpsr2lilypondTranslator::generateCodeForNoteSkipInGraceNotesGroup (
  const S_msrNote& note)
{
  int inputLineNumber =
    note->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  {
    Bool
      traceNotes =
        gGlobalTracingOahGroup->
          getTraceNotes (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceNotes || generateMsrVisitingInformation) {
      std::stringstream s;

      s <<
        std::endl <<
        "% --> generating code for noteSkipInGraceNotesGroup " <<
        note->asString () <<
        ", line " << inputLineNumber <<
        std::endl;

      if (traceNotes) {
        gLogStream << s.str ();
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << s.str ();
      }
    }
  }
#endif

  // generate the note name
  if (gGlobalLpsr2lilypondOahGroup->getGenerateMeasurePositions ()) {
    // generate the rest name to help pin-point bugs
    fLilypondCodeStream << "r";
  }
  else {
    // generate the skip name
    fLilypondCodeStream << "s";
  }

  // generate the skip duration
  fLilypondCodeStream <<
    durationAsLilypondString (
      inputLineNumber,
      note->
        getNoteDisplayWholeNotes ());

  if (gGlobalLpsr2lilypondOahGroup->getInputLineNumbers ()) {
    // generate information and line number as a comment
    fLilypondCodeStream <<
      " %{ line " <<
      note->getInputLineNumber () <<
      " %}";
  }

  if (gGlobalLpsr2lilypondOahGroup->getNotesComments ()) {
    // generate information and line number as a comment
    fLilypondCodeStream <<
      " %{ skip in grace notes group %}";
  }

  // generate the dots if any JMI ???
  for (int i = 0; i < note->getNoteDotsNumber (); ++i) {
    fLilypondCodeStream << ".";
  } // for

  // a grace skip is no relative octave reference,
  // the preceding one is kept
}

void lpsr2lilypondTranslator::generateCodeForNoteInChordInGraceNotesGroup (
  const S_msrNote& note)
{
#ifdef TRACING_IS_ENABLED
  int inputLineNumber =
    note->getInputLineNumber ();

  {
    Bool
      traceNotes =
        gGlobalTracingOahGroup->
          getTraceNotes (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceNotes || generateMsrVisitingInformation) {
      std::stringstream s;

      s <<
        std::endl <<
        "% --> generating code for noteInChordInGraceNotesGroup " <<
        note->asString () <<
        ", line " << inputLineNumber <<
        std::endl;

      if (traceNotes) {
        gLogStream << s.str ();
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << s.str ();
      }
    }
  }
#endif

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
    S_msrTie noteTie = note->getNoteTie ();

    if (noteTie) {
      if (noteTie->getTieKind () == msrTieKind::kTieStart) {
        fLilypondCodeStream <<
          " %{ line " << inputLineNumber << " %}" <<
          "~  %{ msrNoteKind::kNoteInChordInGraceNotesGroup %}";
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

void lpsr2lilypondTranslator::generateCodeForNoteInTupletInGraceNotesGroup (
  const S_msrNote& note)
{
  int inputLineNumber =
    note->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  {
    Bool
      traceNotes =
        gGlobalTracingOahGroup->
          getTraceNotes (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceNotes || generateMsrVisitingInformation) {
      std::stringstream s;

      s <<
        std::endl <<
        "% --> generating code for noteInTupletInGraceNotesGroup " <<
        note->asString () <<
        ", line " << inputLineNumber <<
        std::endl;

      if (traceNotes) {
        gLogStream << s.str ();
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << s.str ();
      }
    }
  }
#endif

  if (gGlobalLpsr2lilypondOahGroup->getIndentTuplets ()) {
    fLilypondCodeStream << std::endl;
  }

  // generate the note name
  if (note->fetchNoteIsARest ()) {
    fLilypondCodeStream <<
      std::string (
        note->getNoteOccupiesAFullMeasure ()
          ? "R"
          : "r");
  }
  else {
    fLilypondCodeStream <<
      notePitchAsLilypondString (note);
  }

  // generate the note display duration
  fLilypondCodeStream <<
    durationAsLilypondString (
      inputLineNumber,
      note->
        getNoteDisplayWholeNotes ());

  // generate the tie if any
  {
    S_msrTie noteTie = note->getNoteTie ();

    if (noteTie) {
      if (noteTie->getTieKind () == msrTieKind::kTieStart) {
        fLilypondCodeStream <<
          "~  "; // JMI spaces???

        if (gGlobalLpsr2lilypondOahGroup->getInputLineNumbers ()) {
          // generate information and line number as a comment
          fLilypondCodeStream <<
            " %{ line " <<
            note->getInputLineNumber () <<
            " %}";
        }

        if (gGlobalLpsr2lilypondOahGroup->getNotesComments ()) {
          // generate information and line number as a comment
          fLilypondCodeStream <<
            " %{ note in tuplet in graceNotesGroup %}";
        }
      }
    }
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

void lpsr2lilypondTranslator::generateCodeForNoteInDoubleTremolo (
  const S_msrNote& note)
{
  int inputLineNumber =
    note->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  {
    Bool
      traceNotes =
        gGlobalTracingOahGroup->
          getTraceNotes (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceNotes || generateMsrVisitingInformation) {
      std::stringstream s;

      s <<
        std::endl <<
        "% --> generating code for noteInDoubleTremolo " <<
        note->asString () <<
        ", line " << inputLineNumber <<
        std::endl;

      if (traceNotes) {
        gLogStream << s.str ();
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << s.str ();
      }
    }
  }
#endif

  // generate the note name
  fLilypondCodeStream <<
    notePitchAsLilypondString (note);

  // generate the note duration
  fLilypondCodeStream <<
    durationAsLilypondString (
      inputLineNumber,
      note->getMeasureElementSoundingWholeNotes ());

  // handle delayed ornaments if any
  if (note->getNoteDelayedTurnOrnament ()) {
    // c2*2/3 ( s2*1/3\turn JMI
    fLilypondCodeStream <<
      "*" <<
      gGlobalLpsr2lilypondOahGroup->getDelayedOrnamentsFraction ();
  }

/* JMI
  // generate the tie if any
  {
    S_msrTie noteTie = note->getNoteTie ();

    if (noteTie) {
      if (noteTie->getTieKind () == msrTieKind::kTieStart) {
        fLilypondCodeStream <<
          " %{ line " << inputLineNumber << " %}" <<
          " ~ %{ kDoubleTremoloMemberNote %}";
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

void lpsr2lilypondTranslator::generateCodeForNoteWords (
  const std::list<S_msrWords>& noteWords)
{
  if (noteWords.size ()) {
    std::list<S_msrWords>::const_iterator i;
    for (
      i=noteWords.begin ();
      i!=noteWords.end ();
      ++i
    ) {
      msrPlacementKind
        wordsPlacementKind =
          (*i)->getWordsPlacementKind ();

      std::string wordsContents =
        (*i)->getWordsContents ();

      msrFontStyleKind
        wordsFontStyleKind =
          (*i)->getWordsFontStyleKind ();

      S_msrFontSize
        wordsFontSize =
          (*i)->getWordsFontSize ();

      msrFontWeightKind
        wordsFontWeightKind =
          (*i)->getWordsFontWeightKind ();

      std::string markup;

      {
        // create markup apart to have its length available
        std::stringstream s;

        switch (wordsPlacementKind) {
          case msrPlacementKind::kPlacement_NO_:
            s << "-";
            break;
          case msrPlacementKind::kPlacementAbove:
            s << "^";
            break;
          case msrPlacementKind::kPlacementBelow:
            s << "_";
            break;
        } // switch

        s <<
          "\\markup" << " { ";

        switch (wordsFontStyleKind) {
          case msrFontStyleKind::kFontStyleNone:
            break;
          case msrFontStyleKind::kFontStyleNormal:
            // LilyPond produces 'normal style' text by default
            break;
          case msrFontStyleKind::KFontStyleItalic:
            s <<
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
            s <<
              "\\bold ";
            break;
        } // switch

        switch (wordsFontSize->getFontSizeKind ()) {
          case msrFontSizeKind::kFontSizeNone:
            break;
          case msrFontSizeKind::kFontSizeXXSmall:
            s <<
              "\\tiny ";
            break;
          case msrFontSizeKind::kFontSizeXSmall:
            s <<
              "\\smaller ";
            break;
          case msrFontSizeKind::kFontSizeSmall:
            s <<
              "\\small ";
            break;
          case msrFontSizeKind::kFontSizeMedium:
            s <<
              "\\normalsize ";
            break;
          case msrFontSizeKind::kFontSizeLarge:
            s <<
              "\\large ";
            break;
          case msrFontSizeKind::kFontSizeXLarge:
            s <<
              "\\larger ";
            break;
          case msrFontSizeKind::kFontSizeXXLarge:
            s <<
              "\\huge ";
            break;
          case msrFontSizeKind::kFontSizeNumeric:
          /* JMI
            s <<
              " %{ " <<
              wordsFontSize->getFontNumericSize () <<
              " points %} ";
              */
            break;
        } // switch

        // does wordsContents contains end of lines?
        size_t endOfLineFound = wordsContents.find ("\n");

        if (endOfLineFound == std::string::npos) {
          s <<
   // JMI         mfDoubleQuoteStringIfNonAlpha (wordsContents) <<
            "\"" << wordsContents << "\"";
          }
        else {
          s <<
						gIndenter.indentMultiLineStringWithCurrentOffset (
              generateAColumnForMarkup (
                wordsContents,
                lilypondMarkupColumnKind::kMarkupColumnKindLeftAligned));
        }

        s <<
          " } ";

        markup = s.str ();
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
      if (noteStemKind != msrStemKind::kStemNeutral) {
        fLilypondCodeStream <<
          stemAsLilypondString (msrStemKind::kStemNeutral);

  // JMI      fCurrentStemKind = stemKind;
      }
    }
  }
*/

  if (! fOnGoingChord) {
    // generate the note codas if any
    const std::list<S_msrCoda>&
      noteCodas =
        note->getNoteCodas ();

    if (noteCodas.size ()) {
      std::list<S_msrCoda>::const_iterator i;
      for (i=noteCodas.begin (); i!=noteCodas.end (); ++i) {
        S_msrCoda coda = (*i);

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
    const std::list<S_msrDalSegno>&
      noteDalSegnos =
        note->getNoteDalSegnos ();

    if (noteDalSegnos.size ()) {
      std::list<S_msrDalSegno>::const_iterator i;
      for (i=noteDalSegnos.begin (); i!=noteDalSegnos.end (); ++i) {
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
  }
}

//________________________________________________________________________
void lpsr2lilypondTranslator::generateNoteArticulation (
  const S_msrArticulation& articulation)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceArticulations ()) {
    gLogStream <<
      "generateNoteArticulation()" <<
      ", articulation: " << articulation->asString () <<
      ", line " << articulation->getInputLineNumber () <<
      std::endl;
  }
#endif

  // should the placement be generated?
  Bool doGeneratePlacement (true);

  // generate note articulation preamble if any
  switch (articulation->getArticulationKind ()) {
    case msrArticulationKind::kArticulation_NO_:
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
      doGeneratePlacement = false;
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
      // this is handled in chordArticulationAsLilyponString ()
      doGeneratePlacement = false;
      break;
    case msrArticulationKind::kArticulationNonArpeggiato:
      // this is handled in chordArticulationAsLilyponString ()
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

  // JMI v0.9.61
  msrArticulationKind
    articulationKind =
      articulation->getArticulationKind ();

  if (doGeneratePlacement) {
    // dont generate a placement for breath marks JMI v0.9.61
//     switch (articulationKind) {
//       case msrArticulationKind::kArticulationBreathMark:
//         break;
//
//       default:
        switch (articulation->getArticulationPlacementKind ()) {
          case msrPlacementKind::kPlacement_NO_:
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
    case msrArticulationKind::kArticulation_NO_:
      fLilypondCodeStream << ">";
      break;

    case msrArticulationKind::kArticulationAccent:
      fLilypondCodeStream << ">";
      break;
    case msrArticulationKind::kArticulationBreathMark:
      fLilypondCodeStream << "\\breathe";
      break;
    case msrArticulationKind::kArticulationCaesura:
    /* JMI v0.9.66
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
      // does not exist in LilyPond, generate staccatissimo instead JMI v0.9.64
      fLilypondCodeStream <<
        "! %{ spiccato %}";
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
        " %{ stress??? %}";
      break;
    case msrArticulationKind::kArticulationUnstress:
      fLilypondCodeStream <<
        " %{ unstress??? %}";
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
        std::stringstream s;

        s <<
          "note articulation '" <<
          articulation->asString () <<
          "' has 'fermata' kind, but is not of type S_msrFermata" <<
          ", line " << articulation->getInputLineNumber ();

        lpsr2lilypondInternalError (
          gGlobalServiceRunData->getInputSourceName (),
          articulation->getInputLineNumber (),
          __FILE__, __LINE__,
          s.str ());
      }
      break;

    case msrArticulationKind::kArticulationArpeggiato:
      // this is handled in chordArticulationAsLilyponString ()
      break;
    case msrArticulationKind::kArticulationNonArpeggiato:
      // this is handled in chordArticulationAsLilyponString ()
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
        " %{ plop??? %}";
      break;
    case msrArticulationKind::kArticulationScoop:
      switch (articulation->getArticulationPlacementKind ()) {
        case msrPlacementKind::kPlacement_NO_:
          fLilypondCodeStream <<
            "\\scoopAbove"; // JMI v0.9.63 meaningfull default ???
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
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceArticulations ()) {
    gLogStream <<
      "generateChordArticulation()" <<
      ", articulation: " << articulation->asString () <<
      ", line " << articulation->getInputLineNumber () <<
      std::endl;
  }
#endif

  // JMI v0.9.62
  msrArticulationKind
    articulationKind =
      articulation->getArticulationKind ();

  switch (articulation->getArticulationPlacementKind ()) { // JMI v0.9.62
    case msrPlacementKind::kPlacement_NO_:
      fLilypondCodeStream << "-";
      break;

    case msrPlacementKind::kPlacementAbove:
      fLilypondCodeStream << "^";
      break;

    case msrPlacementKind::kPlacementBelow:
      switch (articulationKind) {
        case msrArticulationKind::kArticulation_NO_:
          break;

        case msrArticulationKind::kArticulationAccent:
          fLilypondCodeStream << ">";
          break;
        case msrArticulationKind::kArticulationBreathMark:
          //  JMI v0.9.62      fLilypondCodeStream << "\\breathe";
          break;
        default:
          fLilypondCodeStream << "_";
      } // switch
      break;
  } // switch

  switch (articulationKind) {
    case msrArticulationKind::kArticulation_NO_:
      break;

    case msrArticulationKind::kArticulationAccent:
      fLilypondCodeStream << ">";
      break;
    case msrArticulationKind::kArticulationBreathMark:
//  JMI v0.9.62      fLilypondCodeStream << "\\breathe";
      break;
    case msrArticulationKind::kArticulationCaesura:
    /* JMI v0.9.66
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
        " %{ spiccato??? %}";
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
        " %{ stress??? %}";
      break;
    case msrArticulationKind::kArticulationUnstress:
      fLilypondCodeStream <<
        " %{ unstress %}";
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
        std::stringstream s;

        s <<
          "chord articulation '" <<
          articulation->asString () <<
          "' has 'fermata' kind, but is not of type S_msrFermata" <<
          ", line " << articulation->getInputLineNumber ();

        lpsr2lilypondInternalError (
          gGlobalServiceRunData->getInputSourceName (),
          articulation->getInputLineNumber (),
          __FILE__, __LINE__,
          s.str ());
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
        " %{ plop %}";
      break;
    case msrArticulationKind::kArticulationScoop:
      switch (articulation->getArticulationPlacementKind ()) {
        case msrPlacementKind::kPlacement_NO_:
          fLilypondCodeStream <<
            "\\scoopAbove"; // JMI v0.9.63 ???
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
      result = "%{ \\Arrow??? %}";
      break;
    case msrTechnicalKind::kTechnicalDoubleTongue:
      result = "-\\tongue #2";
      break;
    case msrTechnicalKind::kTechnicalDownBow:
      result = "\\downbow";
      break;
    case msrTechnicalKind::kTechnicalFingernails:
      result = "%{ \\Fingernails??? %}";
      break;
    case msrTechnicalKind::kTechnicalHarmonic:
      result = "\\flageolet"; // JMI "\\once\\override Staff.NoteHead.style = #'harmonic-mixed";
      break;
    case msrTechnicalKind::kTechnicalHeel:
      result = "\\lheel"; // rheel??? JMI
      break;
    case msrTechnicalKind::kTechnicalHole:
      result = "%{ \\Hole??? %}";
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
      result = "%{ \\Tap??? %}";
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
  std::stringstream s;

  switch (technicalWithInteger->getTechnicalWithIntegerKind ()) {
     case msrTechnicalWithIntegerKind::kFingering:
      s <<
        "- " <<
       technicalWithInteger->
          getTechnicalWithIntegerValue ();
      break;
    case msrTechnicalWithIntegerKind::kFret:
      // LilyPond will take care of that JMI
      break;
    case msrTechnicalWithIntegerKind::kString:
      s << // no space is allowed between the backSlash and the number
        "\\" <<
       technicalWithInteger->
          getTechnicalWithIntegerValue ();
      break;
  } // switch

  return s.str ();
}

//________________________________________________________________________
std::string lpsr2lilypondTranslator::technicalWithFloatAsLilypondString (
  const S_msrTechnicalWithFloat& technicalWithFloat)
{
  std::stringstream s;

  switch (technicalWithFloat->getTechnicalWithFloatKind ()) {
    case msrTechnicalWithFloatKind::kTechnicalWithFloatBend:
      s <<
        "\\bendAfter " <<
       technicalWithFloat->
          getTechnicalWithFloatValue ();
      break;
  } // switch

  return s.str ();
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
      result = "%{ kHandbell??? %}";
      break;
    case msrTechnicalWithStringKind::kOtherTechnical:
      result = "%{ kHandbell??? %}";
      break;
    case msrTechnicalWithStringKind::kPluck:
      result = "%{ kPluck??? %}";
      break;
    case msrTechnicalWithStringKind::kPullOff:
      break;
  } // switch

  std::string stringValue =
    technicalWithString->
      getTechnicalWithStringValue ();

  if (stringValue.size ()) {
    result +=
      std::string (" ") +
      "-\\markup {\"" + stringValue + "\"}";
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
    upLinkToNoteDuration =
      ornamentUpLinkToNote->
        noteSoundingWholeNotesAsMsrString ();

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

/* JMI
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
        Rational
          remainingFraction =
            Rational (1, 1)
              -
            gGlobalLpsr2lilypondOahGroup->getDelayedOrnamentsFraction ();

        int
          numerator =
            remainingFraction.getNumerator (),
          denominator =
            remainingFraction.getDenominator ();

        fLilypondCodeStream <<
          "s" <<
          upLinkToNoteDuration <<
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
            " %{ line " <<
            ornamentUpLinkToNote->getInputLineNumber () << // JMI ornamentUpLinkToNote ???
            " %}";
        }

        if (gGlobalLpsr2lilypondOahGroup->getNotesComments ()) {
          // generate information and line number as a comment
          fLilypondCodeStream <<
            " %{ kOrnamentDelayedTur %}";
        }

        // forget about the last found whole notes duration,
        // since the latter has been multipled by fDelayedOrnamentsFraction
        fLastMetWholeNotes = Rational (0, 1);
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
          inputLineNumber,
          s.str ());

        result = "\\reverseturn %{ " + s.str () + " %}";
*/
        // c2*2/3 ( s2*1/3\turn

        fLilypondCodeStream <<
          "s" <<
          upLinkToNoteDuration <<
          "*1/3" "\\reverseturn ";

        if (gGlobalLpsr2lilypondOahGroup->getInputLineNumbers ()) {
          // generate information and line number as a comment
          fLilypondCodeStream <<
            " %{ line " <<
            ornamentUpLinkToNote->getInputLineNumber () << // JMI ornamentUpLinkToNote ???
            " %}";
        }

        if (gGlobalLpsr2lilypondOahGroup->getNotesComments ()) {
          // generate information and line number as a comment
          fLilypondCodeStream <<
            " %{ kOrnamentDelayedInvertedTurn %}";
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
          inputLineNumber,
          message);

        result = "%{ " + message + " %}";
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
        case msrPlacementKind::kPlacement_NO_:
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
void lpsr2lilypondTranslator::generateCodeForSpannerBeforeNote (
  const S_msrSpanner& spanner)
{
  msrSpannerTypeKind
    spannerTypeKind =
      spanner->getSpannerTypeKind ();

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
        case msrSpannerTypeKind::kSpannerType_NO_:
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
        case msrSpannerTypeKind::kSpannerType_NO_:
          break;
      } // switch

      msrPlacementKind
        spannerPlacementKind =
          spanner->getSpannerPlacementKind ();

      if (spannerPlacementKind != fCurrentSpannerPlacementKind) {
        switch (spannerPlacementKind) {
          case msrPlacementKind::kPlacement_NO_:
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
void lpsr2lilypondTranslator::generateCodeForSpannerAfterNote (
  const S_msrSpanner& spanner)
{
  msrSpannerTypeKind
    spannerTypeKind =
      spanner->getSpannerTypeKind ();

  // should tweaks be generated for the various spanner texts?
  switch (spannerTypeKind) {
    case msrSpannerTypeKind::kSpannerTypeStart:
      {
        // handle spanner begin text if not empty
        std::string spannerBeginText = spanner->getSpannerBeginText ();

        if (spannerBeginText.size ()) {
          fLilypondCodeStream <<
             "-\\tweak bound-details.left.text \\markup { \"" <<
             spannerBeginText <<
             "\" }" <<
            std::endl;
        }

        // handle spanner middle text if not empty
        std::string spannerMiddleText = spanner->getSpannerMiddleText ();

        if (spannerMiddleText.size ()) {
          fLilypondCodeStream <<
             "\\TextSpannerWithCenteredText \"" << // JMI
             spannerMiddleText <<
             "\"" <<
            std::endl;
        }

        // handle spanner end text if not empty
        std::string spannerEndText = spanner->getSpannerEndText ();

        if (spannerEndText.size ()) {
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
    case msrSpannerTypeKind::kSpannerType_NO_:
      break;
  } // switch

  switch (spanner->getSpannerKind ()) {
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
        case msrSpannerTypeKind::kSpannerType_NO_:
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

            // sanity check
            mfAssert (
              __FILE__, __LINE__,
              spannerStartEnd != nullptr,
              "spannerStartEnd is null");

            // has the start end a trill ornament?
            if (spannerStartEnd->getSpannerUpLinkToNote ()->getNoteTrillOrnament ()) {
              fLilypondCodeStream <<
                "\\stopTrillSpan ";
            }
            else {
              fLilypondCodeStream <<
                "\\stopTextSpan ";
            }
            fOnGoingTrillSpanner = false;
          }
          break;
        case msrSpannerTypeKind::kSpannerTypeContinue:
          break;
        case msrSpannerTypeKind::kSpannerType_NO_:
          break;
      } // switch
      break;
  } // switch

/*
\version "2.19.25"

{
  c' d' ees' fis'
  \once \override TextSpanner.direction = #DOWN

  \TextSpannerWithCenteredText "6\""
  g' \startTextSpan
  a' bes' c'' \stopTextSpan

  \TextSpannerWithCenteredText "x3"
  bes'\startTextSpan a' g' c' | r1 \stopTextSpan
}
*/
}

//________________________________________________________________________
std::string lpsr2lilypondTranslator::tupletFactorAsLilypondString (
	const msrTupletFactor& tupletFactor)
{
  std::stringstream s;

  s <<
    tupletFactor.getTupletActualNotes () <<
		'/' <<
		tupletFactor.getTupletNormalNotes ();

  return s.str ();
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
    case msrAlterationKind::kAlteration_NO_:
      result = "alteration???";
      break;
  } // switch

  return result;
}

//________________________________________________________________________
void lpsr2lilypondTranslator::transposeDiatonicError (
  int inputLineNumber,
  int transposeDiatonic,
  int transposeChromatic)
{
  std::stringstream s;

  s <<
    "diatonic '" << transposeDiatonic <<
    "' is not consistent with " <<
    "chromatic '" << transposeChromatic <<
    "'";

  lpsr2lilypondError (
    gGlobalServiceRunData->getInputSourceName (),
    inputLineNumber,
    __FILE__, __LINE__,
    s.str ());
}

//________________________________________________________________________
std::string lpsr2lilypondTranslator::singleTremoloDurationAsLilypondString (
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

  msrDurationKind
    singleTremoloNoteDurationKind =
      singleTremolo->
        getSingleTremoloGraphicDurationKind ();
    /*
      singleTremoloUpLinkToNote->
        getNoteGraphicDurationKind ();
    */

  /*
  The same output can be obtained by adding :N after the note,
  where N indicates the duration of the subdivision (it must be at least 8).
  If N is 8, one beam is added to the note’s stem.
  */

  int durationToUse =
    singleTremoloMarksNumber; // JMI / singleTremoloNoteSoundingWholeNotes;

  if (singleTremoloNoteDurationKind >= msrDurationKind::kDurationEighth) {
    durationToUse +=
      1 + ((int) singleTremoloNoteDurationKind - (int) msrDurationKind::kDurationEighth);
  }

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceTremolos ()) {
    gLogStream <<
      "singleTremoloDurationAsLilypondString()" <<
      ", line " << singleTremolo->getInputLineNumber () <<
      ", " <<
      mfSingularOrPlural (
        singleTremoloMarksNumber, "mark", "marks") <<
      ", singleTremoloNoteDurationKind: " <<
      msrDurationKindAsString (singleTremoloNoteDurationKind) <<
      ", durationToUse: " <<
      durationToUse <<
      std::endl;
  }
#endif

  std::stringstream s;

  s <<
    ":" <<
 // JMI   int (pow (2, durationToUse + 2)) <<
    int (1 << (durationToUse + 2)) <<
    ' ';

  return s.str ();
}

//________________________________________________________________________
std::string lpsr2lilypondTranslator::harmonyDegreeAlterationKindAsLilypondString (
  msrAlterationKind harmonyDegreeAlterationKind)
{
  std::string result;

  switch (harmonyDegreeAlterationKind) {
    case msrAlterationKind::kAlteration_NO_:
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
  int inputLineNumber =
    harmony->getInputLineNumber ();

  std::stringstream s;

  // should '\powerChords' be generated?
  switch (harmony->getHarmonyKind ()) {
    case msrHarmonyKind::kHarmonyPower:
      if (! fPowerChordHaveAlreadyBeenGenerated) {
        s << "%{ \\powerChords ??? %} ";
        fPowerChordHaveAlreadyBeenGenerated = true;
      }
      break;
    default:
      ;
  } // switch

  // generate harmony pitch
  s <<
    msrQuarterTonesPitchKindAsStringInLanguage (
      harmony->
        getHarmonyRootQuarterTonesPitchKind (),
      gGlobalMsrOahGroup->
        getMsrQuarterTonesPitchesLanguageKind ());

  // generate harmony duration
  msrTupletFactor
    harmonyTupletFactor =
      harmony->getHarmonyTupletFactor ();

  if (harmonyTupletFactor.isEqualToOne ()) {
    // use harmony sounding whole notes
    s <<
      durationAsLilypondString (
        inputLineNumber,
        harmony->
          getMeasureElementSoundingWholeNotes ());
  }
  else {
    // use harmony display whole notes and tuplet factor
    s <<
      durationAsLilypondString (
        inputLineNumber,
        harmony->
          getHarmonyDisplayWholeNotes ()) <<
      "*" <<
      harmonyTupletFactor.inverse ().asFractionString ();
  }

  // generate harmony kind
  switch (harmony->getHarmonyKind ()) {
    case msrHarmonyKind::kHarmony_NO_:
      s << "Harmony???";
      break;

    // MusicXML harmonies

    case msrHarmonyKind::kHarmonyMajor:
      s << ":5.3";
      break;
    case msrHarmonyKind::kHarmonyMinor:
      s << ":m";
      break;
    case msrHarmonyKind::kHarmonyAugmented:
      s << ":aug";
      break;
    case msrHarmonyKind::kHarmonyDiminished:
      s << ":dim";
      break;

    case msrHarmonyKind::kHarmonyDominant:
      s << ":7";
      break;
    case msrHarmonyKind::kHarmonyMajorSeventh:
      s << ":maj7";
      break;
    case msrHarmonyKind::kHarmonyMinorSeventh:
      s << ":m7";
      break;
    case msrHarmonyKind::kHarmonyDiminishedSeventh:
      s << ":dim7";
      break;
    case msrHarmonyKind::kHarmonyAugmentedSeventh:
      s << ":aug7";
      break;
    case msrHarmonyKind::kHarmonyHalfDiminished:
      s << ":m7.5-";
      break;
    case msrHarmonyKind::kHarmonyMinorMajorSeventh:
      s << ":m7+";
      break;

    case msrHarmonyKind::kHarmonyMajorSixth:
      s << ":6";
      break;
    case msrHarmonyKind::kHarmonyMinorSixth:
      s << ":m6";
      break;

    case msrHarmonyKind::kHarmonyDominantNinth:
      s << ":9";
      break;
    case msrHarmonyKind::kHarmonyMajorNinth:
      s << ":maj7.9";
      break;
    case msrHarmonyKind::kHarmonyMinorNinth:
      s << ":m7.9";
      break;

    case msrHarmonyKind::kHarmonyDominantEleventh:
      s << ":11";
      break;
    case msrHarmonyKind::kHarmonyMajorEleventh:
      s << ":maj7.11";
      break;
    case msrHarmonyKind::kHarmonyMinorEleventh:
      s << ":m7.11";
      break;

    case msrHarmonyKind::kHarmonyDominantThirteenth:
      s << ":13";
      break;
    case msrHarmonyKind::kHarmonyMajorThirteenth:
      s << ":maj7.13";
      break;
    case msrHarmonyKind::kHarmonyMinorThirteenth:
      s << ":m7.13";
      break;

    case msrHarmonyKind::kHarmonySuspendedSecond:
      s << ":sus2";
      break;
    case msrHarmonyKind::kHarmonySuspendedFourth:
      s << ":sus4";
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
      s << ":3-.6-%{ :Neapolitan %}";
      break;
    case msrHarmonyKind::kHarmonyItalian:
      s << ":3.6+%{ :Italian %}";
      break;
    case msrHarmonyKind::kHarmonyFrench:
      s << ":3.5+.6+%{ :French %}";
      break;
    case msrHarmonyKind::kHarmonyGerman:
      s << ":3.5.6+%{ :German %}";
      break;

    case msrHarmonyKind::kHarmonyPedal:
      s << "%{ :Pedal %}";
      break;
    case msrHarmonyKind::kHarmonyPower:
      s << ":5 %{ power %}";
      break;
    case msrHarmonyKind::kHarmonyTristan:
      s << ":2+.5+.6+%{ :Tristan %}";
      break;

    // jazz-specific chords

    case msrHarmonyKind::kHarmonyMinorMajorNinth: // -maj9, minmaj9
      s << ":m9";
      break;

    case msrHarmonyKind::kHarmonyDominantSuspendedFourth: // 7sus4, domsus4
      s << ":7sus4";
      break;
    case msrHarmonyKind::kHarmonyDominantAugmentedFifth: // 7#5, domaug5
      s << ":7.5+";
      break;
    case msrHarmonyKind::kHarmonyDominantMinorNinth: // 7b9, dommin9
      s << ":7.9-";
      break;
    case msrHarmonyKind::kHarmonyDominantAugmentedNinthDiminishedFifth: // 7#9b5, domaug9dim5
      s << ":7.9+.5-";
      break;
    case msrHarmonyKind::kIntervkDominantAugmentedNinthAugmentedFifthHarmonyalAugmentedThirteenth: // 7#9#5, domaug9aug5
      s << ":7.9+.5+";
      break;
    case msrHarmonyKind::kHarmonyDominantAugmentedEleventh: // 7#11, domaug11
      s << ":7.11+";
      break;

    case msrHarmonyKind::kHarmonyMajorSeventhAugmentedEleventh: // maj7#11, maj7aug11
      s << ":maj7.11+";
      break;

    // other

    case msrHarmonyKind::kHarmonyOther:
      s << "%{ :kHarmonyOther %}";
      break;

    case msrHarmonyKind::kHarmonyNone:
      s << "%{ :kHarmonyNone %}";
      break;
  } // switch

  // generate harmony degrees if any
  const std::list<S_msrHarmonyDegree>&
    harmonyDegreesList =
      harmony->getHarmonyDegreesList ();

  if (harmonyDegreesList.size ()) {
    Bool thereAreDegreesToBeRemoved (false);

    // generate degrees to be added if any first
    for (
      std::list<S_msrHarmonyDegree>::const_iterator i = harmonyDegreesList.begin ();
      i != harmonyDegreesList.end ();
      ++i
    ) {
      S_msrHarmonyDegree harmonyDegree = (*i);

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
          s <<
      // JMI ???      "." <<
            harmonyDegreeValue <<
            harmonyDegreeAlterationKindAsLilypondString (
              harmonyDegreeAlterationKind);
          break;

        case msrHarmonyDegreeTypeKind::kHarmonyDegreeTypeAlter:
          s <<
            "." <<
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
      s << "^";

      int counter = 0;
      for (
        std::list<S_msrHarmonyDegree>::const_iterator i = harmonyDegreesList.begin ();
        i != harmonyDegreesList.end ();
        ++i
      ) {
        ++counter;

        S_msrHarmonyDegree
          harmonyDegree = (*i);

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
          s <<
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
    msrQuarterTonesPitchKind::kQTP_NO_
  ) {
    s <<
      '/' <<
      msrQuarterTonesPitchKindAsStringInLanguage (
        harmonyBassQuarterTonesPitchKind,
        gGlobalMsrOahGroup->
          getMsrQuarterTonesPitchesLanguageKind ());
  }

  // generate the harmony inversion if relevant // JMI ???
  int harmonyInversion =
    harmony->getHarmonyInversion ();

  if (harmonyInversion != K_HARMONY_NO_INVERSION) {
    s <<
      "%{ inversion: " << harmonyInversion << " %}";
  }

  return s.str ();
}

std::string lpsr2lilypondTranslator::figureAsLilypondString (
  const S_msrBassFigure& bassFigure)
{
  std::stringstream s;

  ++fCurrentFiguredBassFiguresCounter;

  // is the figured bass parenthesized?
  msrFiguredBassParenthesesKind
    figuredBassParenthesesKind =
      fCurrentFiguredBass->
        getFiguredBassParenthesesKind ();

  // generate the bass figure number
  switch (figuredBassParenthesesKind) {
    case msrFiguredBassParenthesesKind::kFiguredBassParenthesesYes:
      s << '[';
      break;
    case msrFiguredBassParenthesesKind::kFiguredBassParenthesesNo:
      break;
  } // switch

  s <<
    bassFigure->getFigureNumber ();

  switch (figuredBassParenthesesKind) {
    case msrFiguredBassParenthesesKind::kFiguredBassParenthesesYes:
      s << ']';
      break;
    case msrFiguredBassParenthesesKind::kFiguredBassParenthesesNo:
      break;
  } // switch

  // handle the bass figure prefix
  switch (bassFigure->getFigurePrefixKind ()) {
    case msrBassFigurePrefixKind::kBassFigurePrefix_NO_:
      break;
    case msrBassFigurePrefixKind::kBassFigurePrefixDoubleFlat:
      s << "--";
      break;
    case msrBassFigurePrefixKind::kBassFigurePrefixFlat:
      s << "-";
      break;
    case msrBassFigurePrefixKind::kBassFigurePrefixFlatFlat:
      s << "flat flat";
      break;
    case msrBassFigurePrefixKind::kBassFigurePrefixNatural:
      s << "!";
      break;
    case msrBassFigurePrefixKind::kBassFigurePrefixSharpSharp:
      s << "sharp sharp";
      break;
    case msrBassFigurePrefixKind::kBassFigurePrefixSharp:
      s << "+";
      break;
    case msrBassFigurePrefixKind::kBassFigurePrefixDoubleSharp:
      s << "++";
      break;
  } // switch

  // handle the bass figure suffix
  switch (bassFigure->getFigureSuffixKind ()) {
    case msrBassFigureSuffixKind::kBassFigureSuffix_NO_:
      break;
    case msrBassFigureSuffixKind::kBassFigureSuffixDoubleFlat:
      s << "double flat";
      break;
    case msrBassFigureSuffixKind::kBassFigureSuffixFlat:
      s << "flat";
      break;
    case msrBassFigureSuffixKind::kBassFigureSuffixFlatFlat:
      s << "flat flat";
      break;
    case msrBassFigureSuffixKind::kBassFigureSuffixNatural:
      s << "natural";
      break;
    case msrBassFigureSuffixKind::kBassFigureSuffixSharpSharp:
      s << "sharp sharp";
      break;
    case msrBassFigureSuffixKind::kBassFigureSuffixSharp:
      s << "sharp";
      break;
    case msrBassFigureSuffixKind::kBassFigureSuffixDoubleSharp:
      s << "souble sharp";
      break;
    case msrBassFigureSuffixKind::kBassFigureSuffixSlash:
      s << '/';
      break;
  } // switch

  // generate a space if not last bass figure in figured bass
  if (
    fCurrentFiguredBassFiguresCounter
      <
    fCurrentFiguredBass->getFiguredBassFiguresList ().size ()
  ) {
    s << ' ';
  }

  return s.str ();
}

std::string lpsr2lilypondTranslator::figuredBassAsLilypondString (
  const S_msrFiguredBass& figuredBass)
{
  int inputLineNumber =
    figuredBass->getInputLineNumber ();

  std::stringstream s;

  s << "<";

  const std::list<S_msrBassFigure>&
    figuredBassFiguresList =
      figuredBass->
        getFiguredBassFiguresList ();

  if (figuredBassFiguresList.size ()) {
    std::list<S_msrBassFigure>::const_iterator
      iBegin = figuredBassFiguresList.begin (),
      iEnd   = figuredBassFiguresList.end (),
      i      = iBegin;

    for ( ; ; ) {
      s << figureAsLilypondString ((*i));
      if (++i == iEnd) break;
      s << ' ';
    } // for
  }

  s << ">";

  // generate figured bass element duration
  msrTupletFactor
    figuredBassTupletFactor =
      figuredBass->getFiguredBassTupletFactor ();

  if (figuredBassTupletFactor.isEqualToOne ()) { // JMI ???
    // use figured bass element sounding whole notes
    s <<
      durationAsLilypondString (
        inputLineNumber,
        figuredBass->
          getMeasureElementSoundingWholeNotes ());
  }
  else {
    // use figured bass element display whole notes and tuplet factor
    s <<
      durationAsLilypondString (
        inputLineNumber,
        figuredBass->
          getFiguredBassDisplayWholeNotes ()) <<
      "*" <<
      figuredBassTupletFactor.asRational ();
  }

  s << ' ';

  return s.str ();
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
/* JMI
  int inputLineNumber =
    frame->getInputLineNumber ();
  */

  std::stringstream s;

  std::list<S_msrFrameNote> // not const&, it will be sorted below
    frameFrameNotesList =
      frame->getFrameFrameNotesList ();

  const std::list<msrBarre>&
    frameBarresList =
      frame->getFrameBarresList ();

  int frameStringsNumber =
    frame->getFrameStringsNumber ();
  int frameFretsNumber =
    frame->getFrameFretsNumber ();

  s <<
    "^\\markup {\\fret-diagram #\"";

  // are there fingerings?
  if (frame->getFrameContainsFingerings ()) {
    s <<
      "f:1;";
  }

  // strings number
  if (frameStringsNumber != 6) {
    s <<
      "w:" <<
      frameStringsNumber <<
      ";";
  }

  // frets number
  s <<
    "h:" <<
    frameFretsNumber <<
    ";";

  // frame barres
  if (frameBarresList.size ()) {
    std::list<msrBarre>::const_iterator
      iBegin = frameBarresList.begin (),
      iEnd   = frameBarresList.end (),
      i      = iBegin;

    for ( ; ; ) {
      msrBarre barre = (*i);

      s <<
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
  if (frameFrameNotesList.size ()) {
    // sort the frame notes,
    // necessary both for code generation
    // and the detection of muted (i.e. absent) frame notes
    frameFrameNotesList.sort (
      compareFrameNotesByDecreasingStringNumber);

    int currentStringNumber = frameStringsNumber;

    // generate the code
    std::list<S_msrFrameNote>::const_iterator
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
        s <<
          currentStringNumber <<
          "-x;";

        --currentStringNumber;
      }

      // generate code for the frame note
      s <<
        frameNoteStringNumber <<
        "-";

      if (frameNoteFretNumber == 0) {
        s <<
          "o";
      }
      else {
        s <<
          frameNoteFretNumber;
      }

      if (frameNoteFingering != -1) {
        s <<
          '-' <<
          frameNoteFingering;
      }

      s <<
        ";";

      --currentStringNumber;

      if (++i == iEnd) break;
  // JMI    os << ";";
    } // for

    // handling low-numbered muted notes
    while (currentStringNumber != 0) {
      // currentStringNumber is missing,
      // hence it is a muted note
      s <<
        currentStringNumber <<
        "-x;";

      --currentStringNumber;
    }
  }

  s <<
    "\" } ";

  return s.str ();
}

void lpsr2lilypondTranslator::generateInputLineNumberAndOrMeasurePositionAsAComment (
  const S_msrMeasureElement& measureElement)
{
  fLilypondCodeStream <<
    " %{ ";

  if (gGlobalLpsr2lilypondOahGroup->getInputLineNumbers ()) {
    // generate the input line number as a comment
    fLilypondCodeStream <<
      "line " << measureElement->getInputLineNumber () << ' ';
  }

  if (gGlobalLpsr2lilypondOahGroup->getGenerateMeasurePositions ()) {
    // generate the measure position as a comment
    fLilypondCodeStream <<
      "memp: " <<
      measureElement->getMeasureElementMeasurePosition () <<
      ' ';
  }

  fLilypondCodeStream <<
    "%} ";
}

//________________________________________________________________________
std::string lpsr2lilypondTranslator::generateAColumnForMarkup (
  const std::string&    theString,
  lilypondMarkupColumnKind columnKind)
{
  std::stringstream s;

  switch (columnKind) {
    case lilypondMarkupColumnKind::kMarkupColumnKindLeftAligned:
      s << "\\column { ";
      break;
    case lilypondMarkupColumnKind::kMarkupColumnKindLeftACentered:
      s << "\\center-column { ";
      break;
  } // switch
  s << std::endl;

  std::list<std::string> chunksList;

  mfSplitRegularStringAtEndOfLines (
    theString,
    chunksList);

  if (chunksList.size ()) {
    // generate a markup containing the chunks
    std::list<std::string>::const_iterator
      iBegin = chunksList.begin (),
      iEnd   = chunksList.end (),
      i      = iBegin;

    for ( ; ; ) {
      s <<
        "\\line { \"" << (*i) << "\" }";
      if (++i == iEnd) break;
    // JMI ???  s << ' ';
      s << std::endl;
    } // for

    s <<
      " } ";

    if (gGlobalLpsr2lilypondOahGroup->getLilypondComments ()) {
      s <<
        "% " <<
        mfSingularOrPlural (
          chunksList.size (), "chunk", "chunks");
    }

    s << std::endl;
  }

  return s.str ();
}

//________________________________________________________________________
std::string lpsr2lilypondTranslator::generateMultilineMarkup (
  const std::string&    theString,
  lilypondMarkupColumnKind columnKind)
{
  std::stringstream s;

  s <<
    "\\markup { " <<
    generateAColumnForMarkup (
      theString,
      columnKind) <<
    " } " <<
    std::endl;

  return s.str ();
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
#ifdef TRACING_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gGlobalLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateLpsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateLpsrVisitingInformation) {
      std::stringstream s;

      s <<
        "% --> Start visiting lpsrScore" <<
        ", line " << elt->getInputLineNumber () <<
        std::endl;

      if (traceLpsrVisitors) {
        gLogStream << s.str ();
      }

      if (generateLpsrVisitingInformation) {
        fLilypondCodeStream << s.str ();
      }
    }
  }
#endif

  // generate a LyLuaTeX preamble if needed
  if (gGlobalLpsr2lilypondOahGroup->getLyLuaTexOutput ()) {
    fLilypondCodeStream <<
      pLyLuaTexPreamble;
  }

  // LilyPond version
  generateLilypondVersion ();

  // global staff size
  generateGlobalStaffSize ();

  // generate myBreak if relevant
  if (! gGlobalLpsr2lilypondOahGroup->getIgnoreLpsrLineBreaks ()) {
    fLilypondCodeStream <<
      "% Pick your choice from the next two lines as needed" <<
      std::endl <<
      "%myBreak = { \\break }" <<
      std::endl <<
      "myBreak = {}" <<
      std::endl << std::endl;
  }

  // generate myPageBreak if relevant
  if (! gGlobalLpsr2lilypondOahGroup->getIgnoreLpsrPageBreaks ()) {
    fLilypondCodeStream <<
      "% Pick your choice from the next two lines as needed" <<
      std::endl <<
      "%myPageBreak = { \\pageBreak }" <<
      std::endl <<
      "myPageBreak = {}" <<
      std::endl << std::endl;
  }

  // generate a 'global' variable?
  if (gGlobalLpsr2lilypondOahGroup->getGlobal ()) {
    fLilypondCodeStream <<
      "global = {" <<
      std::endl <<
      '}' <<
      std::endl << std::endl;
  }
}

void lpsr2lilypondTranslator::visitEnd (S_lpsrScore& elt)
{
#ifdef TRACING_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gGlobalLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateLpsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateLpsrVisitingInformation) {
      std::stringstream s;

      s <<
        "% --> End visiting lpsrScore" <<
        ", line " << elt->getInputLineNumber () <<
        std::endl;

      if (traceLpsrVisitors) {
        gLogStream << s.str ();
      }

      if (generateLpsrVisitingInformation) {
        fLilypondCodeStream << s.str ();
      }
    }
  }
#endif

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
#ifdef TRACING_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gGlobalLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream s;

      s <<
        "% --> Start visiting msrIdentification" <<
        ", line " << elt->getInputLineNumber () <<
        std::endl;

      if (traceLpsrVisitors) {
        gLogStream << s.str ();
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << s.str ();
      }
    }
  }
#endif
}

void lpsr2lilypondTranslator::visitEnd (S_msrIdentification& elt) // JMI
{
#ifdef TRACING_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gGlobalLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream s;

      s <<
        "% --> End visiting msrIdentification" <<
        ", line " << elt->getInputLineNumber () <<
        std::endl;

      if (traceLpsrVisitors) {
        gLogStream << s.str ();
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << s.str ();
      }
    }
  }
#endif
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
#ifdef TRACING_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gGlobalLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateLpsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateLpsrVisitingInformation) {
      std::stringstream s;

      s <<
        "% --> Start visiting lpsrSchemeVariable" <<
        ", line " << elt->getInputLineNumber () <<
        std::endl;

      if (traceLpsrVisitors) {
        gLogStream << s.str ();
      }

      if (generateLpsrVisitingInformation) {
        fLilypondCodeStream << s.str ();
      }
    }
  }
#endif

  std::string
    comment =
      elt->getComment ();

  if (comment.size ()) {
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
    ' ' <<
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
#ifdef TRACING_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gGlobalLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateLpsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateLpsrVisitingInformation) {
      std::stringstream s;

      s <<
        "% --> End visiting lpsrSchemeVariable" <<
        ", line " << elt->getInputLineNumber () <<
        std::endl;

      if (traceLpsrVisitors) {
        gLogStream << s.str ();
      }

      if (generateLpsrVisitingInformation) {
        fLilypondCodeStream << s.str ();
      }
    }
  }
#endif
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
#ifdef TRACING_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gGlobalLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateLpsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateLpsrVisitingInformation) {
      std::stringstream s;

      s <<
        "% --> Start visiting lpsrHeader" <<
        ", line " << elt->getInputLineNumber () <<
        std::endl;

      if (traceLpsrVisitors) {
        gLogStream << s.str ();
      }

      if (generateLpsrVisitingInformation) {
        fLilypondCodeStream << s.str ();
      }
    }
  }
#endif

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

#ifdef TRACING_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gGlobalLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateLpsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateLpsrVisitingInformation) {
      std::stringstream s;

      s <<
        "% --> End visiting lpsrHeader" <<
        ", line " << elt->getInputLineNumber () <<
        std::endl;

      if (traceLpsrVisitors) {
        gLogStream << s.str ();
      }

      if (generateLpsrVisitingInformation) {
        fLilypondCodeStream << s.str ();
      }
    }
  }
#endif

  fLilypondCodeStream <<
    '}';

  if (gGlobalLpsr2lilypondOahGroup->getLilypondComments ()) {
    fLilypondCodeStream << std::left <<
      std::setw (commentFieldWidth) <<
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
          getSetByAnOption ();

  std::string
    lilypondVersion =
      lilypondVersionHasBeenSet
        ? gGlobalLpsr2lilypondOahGroup->
            getLilypondVersion ()
        : gGlobalLpsr2lilypondOahGroup->
           getLilypondVersionDefaultValue ();

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
          getSetByAnOption ();

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

    fLilypondCodeStream << std::left <<
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

  std::list<std::pair<std::string, std::string> > nameStringValuePairsList;

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
    std::list<std::pair<std::string, std::string> >::const_iterator i =
      nameStringValuePairsList.begin ();
    i != nameStringValuePairsList.end ();
    ++i
  ) {
    std::string name = (*i).first;

    size_t nameSize = name.size ();

    if (nameSize > fieldWidth) {
      fieldWidth = nameSize;
    }
  } // for

  fieldWidth = 20;

  // title
  std::string title = header->getLilypondTitle ();

  fLilypondCodeStream << std::left <<
    std::setw (fieldWidth) <<
    "title" << " = " << "\"" << title << "\"" <<
    std::endl;

  // generate the name-value pairs
  for (
    std::list<std::pair<std::string, std::string> >::const_iterator i =
      nameStringValuePairsList.begin ();
    i != nameStringValuePairsList.end ();
    ++i
  ) {
    std::string name  = (*i).first;
    std::string value = (*i).second;

    fLilypondCodeStream << std::left <<
      std::setw (fieldWidth) <<
      name << " = " <<
      generateStringAsLilypondMarkupOrDoubleQuotedString (
        value) <<
      std::endl;
  } // for
}

//________________________________________________________________________
void lpsr2lilypondTranslator::generateHeaderIdentificationPart (
  const S_msrIdentification&          identification,
  std::list<std::pair<std::string, std::string> >& nameStringValuePairsList)
{
  /*
    the values given through options have precedence
    over those found in the identification
  */

#ifdef TRACING_IS_ENABLED
  int fieldWidth =
    identification -> maxIdentificationNamesLength ();
#endif

  // work number
  std::string
    workNumber =
      identification->getIdentificationWorkNumber ();

  if (workNumber.size ()) {
    nameStringValuePairsList.push_back (
      std::make_pair (
        "workNumber",
        workNumber));
  }

  // work title
  std::string
    workCreditTypeTitle =
      identification->getIdentificationWorkCreditTypeTitle ();

  if (workCreditTypeTitle.size ()) {
    nameStringValuePairsList.push_back (
      std::make_pair (
        "workCreditTypeTitle",
        workCreditTypeTitle));
  }

  // opus
  std::string
    opus =
      identification->getIdentificationOpus ();

  if (opus.size ()) {
    nameStringValuePairsList.push_back (
      std::make_pair (
        "opus",
        opus));
  }

  // movementNumber
  std::string
    movementNumber =
      identification->getIdentificationMovementNumber ();

  if (movementNumber.size ()) {
    nameStringValuePairsList.push_back (
      std::make_pair (
        "movementNumber",
        movementNumber));
  }

  // movementTitle
  std::string
    movementTitle =
      identification->getIdentificationMovementTitle ();

  if (movementTitle.size ()) {
    nameStringValuePairsList.push_back (
      std::make_pair (
        "movementTitle",
        movementTitle));
  }

  // encodingDate
  std::string
    encodingDate =
      identification->getIdentificationEncodingDate ();

  if (encodingDate.size ()) {
    nameStringValuePairsList.push_back (
      std::make_pair (
        "encodingDate",
        encodingDate));
  }

  // miscellaneousField
  std::string
    miscellaneousField =
      identification->getIdentificationMiscellaneousField ();

  if (miscellaneousField.size ()) {
    nameStringValuePairsList.push_back (
      std::make_pair (
        "miscellaneousField",
        miscellaneousField));
  }

  // scoreInstrument
  std::string
    scoreInstrument =
      identification->getIdentificationScoreInstrument ();

  if (scoreInstrument.size ()) {
    nameStringValuePairsList.push_back (
      std::make_pair (
        "scoreInstrument",
        scoreInstrument));
  }

  // composers
  const std::list<std::string>&
    composersList =
      identification->
        getIdentificationComposersList ();

  if (composersList.size ()) {
    std::list<std::string>::const_iterator i;

    for (i = composersList.begin (); i != composersList.end (); ++i) {
      std::string composer = (*i);

      nameStringValuePairsList.push_back (
        std::make_pair (
          "composer",
          composer));
    } // for
  }

  // lyricists
  const std::list<std::string>&
    lyricistsList =
      identification->
        getIdentificationLyricistsList ();

  if (lyricistsList.size ()) {
    std::list<std::string>::const_iterator i;

    for (i = lyricistsList.begin (); i != lyricistsList.end (); ++i) {
      std::string lyricist = (*i);

      nameStringValuePairsList.push_back (
        std::make_pair (
          "lyricist",
          lyricist));
    } // for
  }

  // arrangers
  const std::list<std::string>&
    arrangersList =
      identification->
        getIdentificationArrangersList ();

  if (arrangersList.size ()) {
    std::list<std::string>::const_iterator i;

    for (i = arrangersList.begin (); i != arrangersList.end (); ++i) {
      std::string arranger = (*i);

      nameStringValuePairsList.push_back (
        std::make_pair (
          "arranger",
          arranger));
    } // for
  }

  // poets
  const std::list<std::string>&
    poetsList =
      identification->
        getIdentificationPoetsList ();

  if (poetsList.size ()) {
    std::list<std::string>::const_iterator i;

    for (i = poetsList.begin (); i != poetsList.end (); ++i) {
      std::string poet = (*i);

      nameStringValuePairsList.push_back (
        std::make_pair (
          "poet",
          poet));
    } // for
  }

  // translators
  const std::list<std::string>&
    translatorsList =
      identification->
        getIdentificationTranslatorsList ();

  if (translatorsList.size ()) {
    std::list<std::string>::const_iterator i;

    for (i = translatorsList.begin (); i != translatorsList.end (); ++i) {
      std::string translator = (*i);

      nameStringValuePairsList.push_back (
        std::make_pair (
          "translator",
          translator));
    } // for
  }

  // artists
  const std::list<std::string>&
    artistsList =
      identification->
        getIdentificationArtistsList ();

  if (artistsList.size ()) {
    std::list<std::string>::const_iterator i;

    for (i = artistsList.begin (); i != artistsList.end (); ++i) {
      std::string artist = (*i);

      nameStringValuePairsList.push_back (
        std::make_pair (
          "artist",
          artist));
    } // for
  }

  // softwares
  const std::list<std::string>&
    softwaresList =
      identification->
        getIdentificationSoftwaresList ();

  if (softwaresList.size ()) {
    std::list<std::string>::const_iterator i;

    for (i = softwaresList.begin (); i != softwaresList.end (); ++i) {
      std::string software = (*i);

      nameStringValuePairsList.push_back (
        std::make_pair (
          "software",
          software));
    } // for
  }

  // rights
  const std::list<std::string>&
    rightsList =
      identification->
        getIdentificationRightsList ();

  if (rightsList.size ()) {
    std::list<std::string>::const_iterator i;

    for (i = rightsList.begin (); i != rightsList.end (); ++i) {
      std::string right = (*i);

      nameStringValuePairsList.push_back (
        std::make_pair (
          "right",
          right));
    } // for
  }

  // sources
  const std::list<std::string>&
    sourcesList =
      identification->
        getIdentificationSourcesList ();

  if (sourcesList.size ()) {
    std::list<std::string>::const_iterator i;

    for (i = sourcesList.begin (); i != sourcesList.end (); ++i) {
      std::string source = (*i);

      nameStringValuePairsList.push_back (
        std::make_pair (
          "source",
          source));
    } // for
  }

  // relations
  const std::list<std::string>&
    relationsList =
      identification->
        getIdentificationRelationsList ();

  if (relationsList.size ()) {
    std::list<std::string>::const_iterator i;

    for (i = relationsList.begin (); i != relationsList.end (); ++i) {
      std::string relation = (*i);

      nameStringValuePairsList.push_back (
        std::make_pair (
          "relation",
          relation));
    } // for
  }

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceIdentification ()) {
    fLilypondCodeStream <<
      "% --> The identification nameStringValuePairsList contains:" <<
      std::endl;

    for (
      std::list<std::pair<std::string, std::string> >::const_iterator i =
        nameStringValuePairsList.begin ();
      i != nameStringValuePairsList.end ();
      ++i
    ) {
      std::string name  = (*i).first;
      std::string value = (*i).second;

      fLilypondCodeStream <<
        "% " << name << " ---> " << value <<
        std::endl;
    } // for

    fLilypondCodeStream <<
      "% ====> fieldWidth: " << fieldWidth <<
      std::endl;
  }
#endif
}

//________________________________________________________________________
void lpsr2lilypondTranslator::createLilypondHeaderStringValuePairs (
  const S_lpsrHeader&                 header,
  std::list<std::pair<std::string, std::string> >& nameStringValuePairsList)
{
  /*
    the values given through options have precedence
    over those found in the header
  */

  // dedication
  std::string
    dedicationFromOption =
      gGlobalLpsr2lilypondOahGroup->getDedication ();

  if (dedicationFromOption.size ()) {
      nameStringValuePairsList.push_back (
        std::make_pair (
          "dedication",
          dedicationFromOption));
  }
  else {
    std::string
      lilypondDedication =
        header->getLilypondDedication ();

    if (lilypondDedication.size ()) {
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

  if (pieceFromOption.size ()) {
      nameStringValuePairsList.push_back (
        std::make_pair (
          "piece",
          pieceFromOption));
  }
  else {
    std::string
      lilypondPiece =
        header->getLilypondPiece ();

    if (lilypondPiece.size ()) {
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

  if (opusFromOption.size ()) {
      nameStringValuePairsList.push_back (
        std::make_pair (
          "opus",
          opusFromOption));
  }
  else {
    std::string
      lilypondOpus =
        header->getLilypondOpus ();

    if (lilypondOpus.size ()) {
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
  if (gGlobalMsrOahGroup->getUseFilenameAsWorkCreditTypeTitle ()) {
    nameStringValuePairsList.push_back (
      std::make_pair (
        "title",
        gGlobalServiceRunData->getInputSourceName ()));
  }
  else {
    std::string
      titleFromOption =
        gGlobalLpsr2lilypondOahGroup->getTitle ();

    if (titleFromOption.size ()) {
        nameStringValuePairsList.push_back (
          std::make_pair (
            "title",
            titleFromOption));
    }
    else {
      std::string
        lilypondTitle =
          header->getLilypondTitle ();

      if (lilypondTitle.size ()) {
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

  if (subtitleFromOption.size ()) {
      nameStringValuePairsList.push_back (
        std::make_pair (
          "subtitle",
          subtitleFromOption));
  }
  else {
    std::string
      lilypondSubTitle =
        header->getLilypondSubTitle ();

    if (lilypondSubTitle.size ()) {
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

  if (subsubtitleFromOption.size ()) {
      nameStringValuePairsList.push_back (
        std::make_pair (
          "subsubtitle",
          subsubtitleFromOption));
  }
  else {
    std::string
      lilypondSubSubTitle =
        header->getLilypondSubSubTitle ();

    if (lilypondSubSubTitle.size ()) {
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

  if (instrumentFromOption.size ()) {
      nameStringValuePairsList.push_back (
        std::make_pair (
          "instrument",
          instrumentFromOption));
  }
  else {
    std::string
      lilypondInstrument =
        header->getLilypondInstrument ();

    if (lilypondInstrument.size ()) {
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

  if (meterFromOption.size ()) {
      nameStringValuePairsList.push_back (
        std::make_pair (
          "meter",
          meterFromOption));
  }
  else {
    std::string
      lilypondMeter =
        header->getLilypondMeter ();

    if (lilypondMeter.size ()) {
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

  if (copyrightFromOption.size ()) {
      nameStringValuePairsList.push_back (
        std::make_pair (
          "copyright",
          copyrightFromOption));
  }
  else {
    std::string
      lilypondCopyright =
        header->getLilypondCopyright ();

    if (lilypondCopyright.size ()) {
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

  if (taglineFromOption.size ()) {
      nameStringValuePairsList.push_back (
        std::make_pair (
          "tagline",
          taglineFromOption));
  }
  else {
    std::string
      lilypondTagline =
        header->getLilypondTagline ();

    if (lilypondTagline.size ()) {
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
      mfDoubleQuoteString (value); // JMI  extensive tests???
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

  // fetch length values from paper page size
  std::list<std::pair<std::string, msrLength> > nameLengthValuePairsList;

  fetchLengthValuesFromPaperPageSize (
    paper,
    nameLengthValuePairsList );

  // fetch boolean values from LPSR options group
  std::list<std::pair<std::string, Bool> > nameBooleanValuePairsList;

  fetchOnOffValuesFromLpsrOptionsGroup (
    paper,
    nameBooleanValuePairsList);

  // compute fieldWidth
  size_t fieldWidth = 0;

  for (
    std::list<std::pair<std::string, msrLength> >::const_iterator i =
      nameLengthValuePairsList.begin ();
    i != nameLengthValuePairsList.end ();
    ++i
  ) {
    std::string name = (*i).first;

    size_t nameSize = name.size ();

    if (nameSize > fieldWidth) {
      fieldWidth = nameSize;
    }
  } // for

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceIdentification ()) {
    fLilypondCodeStream <<
      "% --> The paper nameLengthValuePairsList contains:" <<
      std::endl;

    for (
      std::list<std::pair<std::string, msrLength> >::const_iterator i =
        nameLengthValuePairsList.begin ();
      i != nameLengthValuePairsList.end ();
      ++i
    ) {
      std::string    name  = (*i).first;
      msrLength value = (*i).second;

      fLilypondCodeStream <<
        "% " << name << " ---> " << value <<
        std::endl;
    } // for

    fLilypondCodeStream <<
      "% ====> fieldWidth: " << fieldWidth <<
      std::endl;
  }
#endif

  for (
    std::list<std::pair<std::string, Bool> >::const_iterator i =
      nameBooleanValuePairsList.begin ();
    i != nameBooleanValuePairsList.end ();
    ++i
  ) {
    std::string name = (*i).first;

    size_t nameSize = name.size ();

    if (nameSize > fieldWidth) {
      fieldWidth = nameSize;
    }
  } // for

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceIdentification ()) {
    fLilypondCodeStream <<
      "% --> The paper nameLengthValuePairsList contains:" <<
      std::endl;

    for (
      std::list<std::pair<std::string, Bool> >::const_iterator i =
        nameBooleanValuePairsList.begin ();
      i != nameBooleanValuePairsList.end ();
      ++i
    ) {
      std::string name  = (*i).first;
      Bool   value = (*i).second;

      fLilypondCodeStream <<
        "% " << name << " ---> " << value <<
        std::endl;
    } // for

    fLilypondCodeStream <<
      "% ====> fieldWidth: " << fieldWidth <<
      std::endl;
  }
#endif

  // page size
  generatePaperPageSize (
    pageLayout,
    defaultLengthUnit,
    fieldWidth);

  // separator
// JMI    fLilypondCodeStream << std::endl;

  // margins
  generatePaperMargins (
    pageLayout,
    defaultLengthUnit,
    fieldWidth);

  // separator
// JMI    fLilypondCodeStream << std::endl;

  // indents
  generatePaperIndents (
    paper,
    defaultLengthUnit,
    fieldWidth);

  // separator
// JMI    fLilypondCodeStream << std::endl;

  // spaces
  generatePaperSpaces (
    paper,
    defaultLengthUnit,
    fieldWidth);

  // separator
// JMI    fLilypondCodeStream << std::endl;

  // counts
  generatePaperCounts (
    paper,
    defaultLengthUnit,
    fieldWidth);

  // separator
// JMI    fLilypondCodeStream << std::endl;

  // boolean
  generatePaperBooleans (
    paper,
    fieldWidth);

  // separator
// JMI    fLilypondCodeStream << std::endl;

  // headers and footers
  generatePaperHeadersAndFooters (
    paper,
    defaultLengthUnit,
    fieldWidth);

  --gIndenter;

  fLilypondCodeStream <<
    '}';

  if (gGlobalLpsr2lilypondOahGroup->getLilypondComments ()) {
    fLilypondCodeStream << std::left <<
      std::setw (commentFieldWidth) <<
      " % paper";
  }

  fLilypondCodeStream <<
    std::endl << std::endl;
}

//________________________________________________________________________
void lpsr2lilypondTranslator::fetchLengthValuesFromPaperPageSize (
  const S_lpsrPaper&                     paper,
  std::list<std::pair<std::string, msrLength> >& nameLengthValuePairsList)
{
  Bool
    generateCommentedOutVariables =
      gGlobalLpsr2lilypondOahGroup->
        getGenerateCommentedOutVariables ();

  // paper height
  Bool
    paperHeightHasBeenSet =
      gGlobalLpsrOahGroup->
        getPaperHeightAtom ()->
          getSetByAnOption (),
    doGeneratePaperHeight =
      paperHeightHasBeenSet || generateCommentedOutVariables;

  if (doGeneratePaperHeight) {
    msrLength
      paperHeight =
        gGlobalLpsrOahGroup->getPaperHeight ();

    nameLengthValuePairsList.push_back (
      std::make_pair (
        "paper-height",
        paperHeight));
  }

  // paper width
  Bool
    paperWidthHasBeenSet =
      gGlobalLpsrOahGroup->
        getPaperWidthAtom ()->
          getSetByAnOption (),
    doGeneratePaperWidth =
      paperWidthHasBeenSet || generateCommentedOutVariables;

  if (doGeneratePaperWidth) {
    msrLength
      paperWidth =
        gGlobalLpsrOahGroup->getPaperWidth ();

    nameLengthValuePairsList.push_back (
      std::make_pair (
        "paper-width",
        paperWidth));
  }
}

void lpsr2lilypondTranslator::fetchOnOffValuesFromLpsrOptionsGroup (
  const S_lpsrPaper&                paper,
  std::list<std::pair<std::string, Bool> >& nameBooleanValuePairsList)
{
  // ragged-last
  if (gGlobalLpsrOahGroup->getRaggedLastAtom ()->getSetByAnOption ()) {
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
  if (gGlobalLpsrOahGroup->getRaggedBottomAtom ()->getSetByAnOption ()) {
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
  if (gGlobalLpsrOahGroup->getRaggedLastBottomAtom ()->getSetByAnOption ()) {
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
  if (gGlobalLpsrOahGroup->getRaggedRightAtom ()->getSetByAnOption ()) {
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
  const S_msrPageLayout&   pageLayout,
  msrLengthUnitKind defaultLengthUnit,
  int               fieldWidth)
{
  Bool
    generateCommentedOutVariables =
      gGlobalLpsr2lilypondOahGroup->
        getGenerateCommentedOutVariables ();

  // paper height
  Bool
    paperHeightHasBeenSet =
      gGlobalLpsrOahGroup->
        getPaperHeightAtom ()->
          getSetByAnOption (),
    doGeneratePaperHeight =
      paperHeightHasBeenSet || generateCommentedOutVariables;

  if (doGeneratePaperHeight) {
    if (generateCommentedOutVariables) {
      fLilypondCodeStream << "% ";
    }

    msrLength
      paperHeight =
        gGlobalLpsrOahGroup->getPaperHeight ();

    fLilypondCodeStream << std::left <<
      std::setw (fieldWidth) <<
      "paper-height" <<
      " = " <<
      std::setprecision (3) << paperHeight.getLengthValue () <<
      lengthUnitAsLilypondString (paperHeight.getLengthUnitKind ()) <<
      std::endl;
  }

  // paper width
  Bool
    paperWidthHasBeenSet =
      gGlobalLpsrOahGroup->
        getPaperWidthAtom ()->
          getSetByAnOption (),
    doGeneratePaperWidth =
      paperWidthHasBeenSet || generateCommentedOutVariables;

  if (doGeneratePaperWidth) {
    if (generateCommentedOutVariables) {
      fLilypondCodeStream << "% ";
    }

    msrLength
      paperWidth =
        gGlobalLpsrOahGroup->getPaperWidth ();

    fLilypondCodeStream << std::left <<
      std::setw (fieldWidth) <<
      "paper-width" <<
      " = " <<
      std::setprecision (3) << paperWidth.getLengthValue () <<
      lengthUnitAsLilypondString (paperWidth.getLengthUnitKind ()) <<
      std::endl;
  }
}

//________________________________________________________________________
void lpsr2lilypondTranslator::generatePaperMargins (
  const S_msrPageLayout&   pageLayout,
  msrLengthUnitKind defaultLengthUnit,
  int               fieldWidth)
{
  Bool
    generateCommentedOutVariables =
      gGlobalLpsr2lilypondOahGroup->
        getGenerateCommentedOutVariables ();

  // left margin
  Bool
    leftMarginHasBeenSet =
      gGlobalLpsrOahGroup->
        getPaperLeftMarginAtom ()->
          getSetByAnOption (),
    doGenerateLeftMargin =
      leftMarginHasBeenSet || generateCommentedOutVariables;

  if (doGenerateLeftMargin) {
    if (generateCommentedOutVariables) {
      fLilypondCodeStream << "% ";
    }

    msrLength
      leftMargin =
        gGlobalLpsrOahGroup->getPaperLeftMargin ();

    fLilypondCodeStream << std::left <<
      std::setw (fieldWidth) <<
      "left-margin" <<
      " = " <<
      std::setprecision (3) << leftMargin.getLengthValue () <<
      lengthUnitAsLilypondString (leftMargin.getLengthUnitKind ()) <<
      std::endl;
  }

  // right margin
  Bool
    rightMarginHasBeenSet =
      gGlobalLpsrOahGroup->
        getPaperRightMarginAtom ()->
          getSetByAnOption (),
    doGenerateRightMargin =
      rightMarginHasBeenSet || generateCommentedOutVariables;

  if (doGenerateRightMargin) {
    if (generateCommentedOutVariables) {
      fLilypondCodeStream << "% ";
    }

    msrLength
      rightMargin =
        gGlobalLpsrOahGroup->getPaperRightMargin ();

    fLilypondCodeStream << std::left <<
      std::setw (fieldWidth) <<
      "right-margin" <<
      " = " <<
      std::setprecision (3) << rightMargin.getLengthValue () <<
      lengthUnitAsLilypondString (rightMargin.getLengthUnitKind ()) <<
      std::endl;
  }

  // top margin
  Bool
    topMarginHasBeenSet =
      gGlobalLpsrOahGroup->
        getPaperTopMarginAtom ()->
          getSetByAnOption (),
    doGenerateTopMargin =
      topMarginHasBeenSet || generateCommentedOutVariables;

  if (doGenerateTopMargin) {
    if (generateCommentedOutVariables) {
      fLilypondCodeStream << "% ";
    }

    msrLength
      topMargin =
        gGlobalLpsrOahGroup->getPaperTopMargin ();

    fLilypondCodeStream << std::left <<
      std::setw (fieldWidth) <<
      "top-margin" <<
      " = " <<
      std::setprecision (3) << topMargin.getLengthValue () <<
      lengthUnitAsLilypondString (topMargin.getLengthUnitKind ()) <<
      std::endl;
  }

  // bottom margin
  Bool
    bottomMarginHasBeenSet =
      gGlobalLpsrOahGroup->
        getPaperBottomMarginAtom ()->
          getSetByAnOption (),
    doGenerateBottomMargin =
      bottomMarginHasBeenSet || generateCommentedOutVariables;

  if (doGenerateBottomMargin) {
    if (generateCommentedOutVariables) {
      fLilypondCodeStream << "% ";
    }

    msrLength
      bottomMargin =
        gGlobalLpsrOahGroup->getPaperBottomMargin ();

    fLilypondCodeStream << std::left <<
      std::setw (fieldWidth) <<
      "bottom-margin" <<
      " = " <<
      std::setprecision (3) << bottomMargin.getLengthValue () <<
      lengthUnitAsLilypondString (bottomMargin.getLengthUnitKind ()) <<
      std::endl;
  }
}

//________________________________________________________________________
void lpsr2lilypondTranslator::generatePaperIndents (
  const S_lpsrPaper& pagePaper,
  msrLengthUnitKind defaultLengthUnit,
  int               fieldWidth)
{
  // horizontal shift
  S_msrLength
    horizontalShift =
      pagePaper->getHorizontalShift ();

  if (! horizontalShift) {
    fLilypondCodeStream << "% ";
  }
  fLilypondCodeStream << std::left <<
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

  // indent
  S_msrLength
    indent =
      pagePaper->getIndent ();

  if (! indent) {
    fLilypondCodeStream << "% ";
  }
  fLilypondCodeStream << std::left <<
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

  // short indent
  S_msrLength
    shortIndent =
      pagePaper->getShortIndent ();

  if (! shortIndent) {
    fLilypondCodeStream << "% ";
  }
  fLilypondCodeStream << std::left <<
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
}

//________________________________________________________________________
void lpsr2lilypondTranslator::generatePaperSpaces (
  const S_lpsrPaper& pagePaper,
  msrLengthUnitKind defaultLengthUnit,
  int               fieldWidth)
{
  // markup system spacing padding
  S_msrLength
    markupSystemPpacingPadding =
      pagePaper->getMarkupSystemSpacingPadding ();

  if (! markupSystemPpacingPadding) {
    fLilypondCodeStream << "% ";
  }
  fLilypondCodeStream << std::left <<
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

  // between system space
  S_msrLength
    betweenSystemSpace =
      pagePaper->getBetweenSystemSpace ();

  if (! betweenSystemSpace) {
    fLilypondCodeStream << "% ";
  }
  fLilypondCodeStream << std::left <<
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

  // page top space
  S_msrLength
    pageTopSpace =
      pagePaper->getPageTopSpacing ();

  if (! pageTopSpace) {
    fLilypondCodeStream << "% ";
  }
  fLilypondCodeStream << std::left <<
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
}

//________________________________________________________________________
void lpsr2lilypondTranslator::generatePaperCounts (
  const S_lpsrPaper& pagePaper,
  msrLengthUnitKind defaultLengthUnit,
  int               fieldWidth)
{
  // page count
  int
    pageCount =
      pagePaper->getPageCount ();

  if (pageCount < 0) {
    fLilypondCodeStream << "% ";
  }
  fLilypondCodeStream << std::left <<
    std::setw (fieldWidth) <<
    "page-count" << " = " <<
    pageCount <<
    std::endl;

  // system count
  int
    systemCount =
      pagePaper->getSystemCount ();

  if (systemCount < 0) {
    fLilypondCodeStream << "% ";
  }
  fLilypondCodeStream << std::left <<
    std::setw (fieldWidth) <<
    "system-count" << " = " <<
    systemCount <<
    std::endl;
}

//________________________________________________________________________
void lpsr2lilypondTranslator::generatePaperBooleans (
  const S_lpsrPaper& pagePaper,
  int               fieldWidth)
{
  // ragged last
  mfOnOffKind
    raggedLast =
      pagePaper->getRaggedLast ();

  switch (raggedLast) {
    case mfOnOffKind::kMfOnOffUnknown:
      break;

    case mfOnOffKind::kMfOnOffOn:
    case mfOnOffKind::kMfOnOffOff:
      fLilypondCodeStream << std::left <<
        std::setw (fieldWidth) <<
        "ragged-last" << " = ";
      if (mfOnOffKindAsBool (raggedLast)) {
        fLilypondCodeStream << "##t";
      }
      else {
        fLilypondCodeStream << "##f";
      }
      break;
  } // switch
  fLilypondCodeStream << std::endl;

  // ragged bottom
  mfOnOffKind
    raggedBottom =
      pagePaper->getRaggedBottom ();

  switch (raggedBottom) {
    case mfOnOffKind::kMfOnOffUnknown:
      break;

    case mfOnOffKind::kMfOnOffOn:
    case mfOnOffKind::kMfOnOffOff:
      fLilypondCodeStream << std::left <<
        std::setw (fieldWidth) <<
        "ragged-bottom" << " = ";
      if (mfOnOffKindAsBool (raggedBottom)) {
        fLilypondCodeStream << "##t";
      }
      else {
        fLilypondCodeStream << "##f";
      }
      break;
  } // switch
  fLilypondCodeStream << std::endl;

  // ragged last bottom
  mfOnOffKind
    raggedLastBottom =
      pagePaper->getRaggedLastBottom ();

  switch (raggedLastBottom) {
    case mfOnOffKind::kMfOnOffUnknown:
      break;

    case mfOnOffKind::kMfOnOffOn:
    case mfOnOffKind::kMfOnOffOff:
      fLilypondCodeStream << std::left <<
        std::setw (fieldWidth) <<
        "ragged-last-bottom" << " = ";
      if (mfOnOffKindAsBool (raggedLastBottom)) {
        fLilypondCodeStream << "##t";
      }
      else {
        fLilypondCodeStream << "##f";
      }
      break;
  } // switch
  fLilypondCodeStream << std::endl;

  // ragged right
  mfOnOffKind
    raggedRight =
      pagePaper->getRaggedRight ();

  switch (raggedRight) {
    case mfOnOffKind::kMfOnOffUnknown:
      break;

    case mfOnOffKind::kMfOnOffOn:
    case mfOnOffKind::kMfOnOffOff:
      fLilypondCodeStream << std::left <<
        std::setw (fieldWidth) <<
        "ragged-right" << " = ";
      if (mfOnOffKindAsBool (raggedRight)) {
        fLilypondCodeStream << "##t";
      }
      else {
        fLilypondCodeStream << "##f";
      }
      break;
  } // switch

  fLilypondCodeStream << std::endl;
}

//________________________________________________________________________
void lpsr2lilypondTranslator::generatePaperHeadersAndFooters (
  const S_lpsrPaper& pagePaper,
  msrLengthUnitKind defaultLengthUnit,
  int               fieldWidth)
{
  std::string oddHeaderMarkup =
    pagePaper->getOddHeaderMarkup ();

  fLilypondCodeStream << std::left <<
    std::setw (fieldWidth) <<
    "oddHeaderMarkup" << " = ";
  if (oddHeaderMarkup.size ()) {
    fLilypondCodeStream <<
      oddHeaderMarkup;
  }
  else {
    fLilypondCodeStream << "\"\"";
  }
  fLilypondCodeStream << std::endl;

  std::string evenHeaderMarkup =
    pagePaper->getEvenHeaderMarkup ();

  fLilypondCodeStream << std::left <<
    std::setw (fieldWidth) <<
    "evenHeaderMarkup" << " = ";
  if (evenHeaderMarkup.size ()) {
    fLilypondCodeStream <<
      evenHeaderMarkup;
  }
  else {
    fLilypondCodeStream << "\"\"";
  }
  fLilypondCodeStream << std::endl;

  std::string oddFooterMarkup =
    pagePaper->getOddFooterMarkup ();

  fLilypondCodeStream << std::left <<
    std::setw (fieldWidth) <<
    "oddFooterMarkup" << " = ";
  if (oddFooterMarkup.size ()) {
    fLilypondCodeStream <<
      oddFooterMarkup;
  }
  else {
    fLilypondCodeStream << "\"\"";
  }
  fLilypondCodeStream << std::endl;

  std::string evenFooterMarkup =
    pagePaper->getEvenFooterMarkup ();

  fLilypondCodeStream << std::left <<
    std::setw (fieldWidth) <<
    "evenFooterMarkup" << " = ";
  if (evenFooterMarkup.size ()) {
    fLilypondCodeStream <<
      evenFooterMarkup;
  }
  else {
    fLilypondCodeStream << "\"\"";
  }
  fLilypondCodeStream << std::endl;
}

//________________________________________________________________________
void lpsr2lilypondTranslator::visitStart (S_lpsrPaper& elt)
{
#ifdef TRACING_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gGlobalLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateLpsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateLpsrVisitingInformation) {
      std::stringstream s;

      s <<
        "% --> Start visiting lpsrPaper" <<
        ", line " << elt->getInputLineNumber () <<
        std::endl;

      if (traceLpsrVisitors) {
        gLogStream << s.str ();
      }

      if (generateLpsrVisitingInformation) {
        fLilypondCodeStream << s.str ();
      }
    }
  }
#endif

  if (gGlobalLpsr2lilypondOahGroup->getNoPaperBlock ()) {
    return;
  }

  if (! fOnGoingBookPartBlock) {
    generatePaper (elt);
  }
}

void lpsr2lilypondTranslator::visitEnd (S_lpsrPaper& elt) // superflous ??? JMI
{
#ifdef TRACING_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gGlobalLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateLpsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateLpsrVisitingInformation) {
      std::stringstream s;

      s <<
        "% --> End visiting lpsrPaper" <<
        ", line " << elt->getInputLineNumber () <<
        std::endl;

      if (traceLpsrVisitors) {
        gLogStream << s.str ();
      }

      if (generateLpsrVisitingInformation) {
        fLilypondCodeStream << s.str ();
      }
    }
  }
#endif

  if (gGlobalLpsr2lilypondOahGroup->getNoPaperBlock ()) {
    return;
  }
}

//________________________________________________________________________
void lpsr2lilypondTranslator::visitStart (S_lpsrLayout& elt)
{
#ifdef TRACING_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gGlobalLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateLpsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateLpsrVisitingInformation) {
      std::stringstream s;

      s <<
        "% --> Start visiting lpsrLayout" <<
        ", line " << elt->getInputLineNumber () <<
        std::endl;

      if (traceLpsrVisitors) {
        gLogStream << s.str ();
      }

      if (generateLpsrVisitingInformation) {
        fLilypondCodeStream << s.str ();
      }
    }
  }
#endif

  fLilypondCodeStream <<
    "\\layout {" <<
    std::endl;

  ++gIndenter; // decremented in visitEnd (S_lpsrLayout& elt)

  // score context?
  if (! gGlobalLpsr2lilypondOahGroup->getNoLayoutScoreContext ()) {
    fLilypondCodeStream <<
      "\\context {" <<
      std::endl <<
      gTab << "\\Score" <<
      std::endl <<
      gTab << "autoBeaming = ##f % to display tuplets brackets" <<
      std::endl;

    // generate the end of the voice definition
    if (gGlobalLpsr2lilypondOahGroup->getLilypondComments ()) {
      fLilypondCodeStream << std::left <<
        std::setw (commentFieldWidth) <<
        '}' <<
        "% score context";
    }
    else {
      fLilypondCodeStream <<
        '}';
    }
    fLilypondCodeStream << std::endl;
  }

  // voice context?
  if (! gGlobalLpsr2lilypondOahGroup->getNoLayoutVoiceContext ()) {
    fLilypondCodeStream <<
      "\\context {" <<
      std::endl <<
      gTab << "\\Voice" <<
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
    if (gGlobalLpsr2lilypondOahGroup->getLilypondComments ()) {
      fLilypondCodeStream << std::left <<
        std::setw (commentFieldWidth) <<
        '}' <<
        "% voice context";
    }
    else {
      fLilypondCodeStream <<
        '}';
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
#ifdef TRACING_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gGlobalLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateLpsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateLpsrVisitingInformation) {
      std::stringstream s;

      s <<
        "% --> End visiting lpsrLayout" <<
        ", line " << elt->getInputLineNumber () <<
        std::endl;

      if (traceLpsrVisitors) {
        gLogStream << s.str ();
      }

      if (generateLpsrVisitingInformation) {
        fLilypondCodeStream << s.str ();
      }
    }
  }
#endif

  // should we set the repeat brackets type?
  if (gGlobalLpsr2lilypondOahGroup->getRepeatBrackets ()) {
    fLilypondCodeStream <<
      "\\context {" <<
      std::endl;

    ++gIndenter;

    fLilypondCodeStream <<
      "\\Score" <<
      std::endl <<
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
      '}' <<
      std::endl;
  }

  // should we set the bars number size?
  S_oahFloatAtom
    barNumbersSizeAtom =
      gGlobalLpsr2lilypondOahGroup->getBarNumbersSizeAtom ();

  if (barNumbersSizeAtom->getSetByAnOption ()) {
    fLilypondCodeStream <<
      "\\context {" <<
      std::endl;

    ++gIndenter;

    fLilypondCodeStream <<
      "\\Score" <<
      std::endl <<
      "\\override BarNumber.font-size = " <<
      barNumbersSizeAtom->getFloatVariable () <<
      std::endl;

    --gIndenter;

    fLilypondCodeStream <<
      '}' <<
      std::endl;
  }

  // should we use an arpeggio engraver?
  if (gGlobalLpsr2lilypondOahGroup->getConnectArpeggios ()) {
    fLilypondCodeStream <<
      "\\context {" <<
      std::endl;

    ++gIndenter;

    fLilypondCodeStream <<
      "\\Staff" <<
      std::endl <<
      "\\consists \"Span_arpeggio_engraver\"" <<
      std::endl;

    --gIndenter;

    fLilypondCodeStream <<
      "}" " % staff context";

    if (gGlobalLpsr2lilypondOahGroup->getLilypondComments ()) {
      fLilypondCodeStream << std::left <<
        std::setw (commentFieldWidth) <<
        " % layout";
    }

    fLilypondCodeStream << std::endl;
  }

  --gIndenter; // incremented in visitStart (S_lpsrLayout& elt)

  fLilypondCodeStream <<
    '}';

  if (gGlobalLpsr2lilypondOahGroup->getLilypondComments ()) {
    fLilypondCodeStream << std::left <<
      std::setw (commentFieldWidth) <<
      " % layout";
  }

  fLilypondCodeStream <<
    std::endl << std::endl;
}

//________________________________________________________________________
void lpsr2lilypondTranslator::visitStart (S_lpsrBookBlock& elt)
{
#ifdef TRACING_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gGlobalLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateLpsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateLpsrVisitingInformation) {
      std::stringstream s;

      s <<
        "% --> Start visiting lpsrBookBlock" <<
        ", line " << elt->getInputLineNumber () <<
        std::endl;

      if (traceLpsrVisitors) {
        gLogStream << s.str ();
      }

      if (generateLpsrVisitingInformation) {
        fLilypondCodeStream << s.str ();
      }
    }
  }
#endif

  if (gGlobalLpsr2lilypondOahGroup->getNoBookBlock ()) {
    return;
  }

  fLilypondCodeStream <<
    "\\book {" <<
    std::endl;

  ++gIndenter; // decremented in visitEnd (S_lpsrBookBlock& elt)

  fOnGoingBookPartBlock = true;
}

void lpsr2lilypondTranslator::visitEnd (S_lpsrBookBlock& elt)
{
#ifdef TRACING_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gGlobalLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateLpsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateLpsrVisitingInformation) {
      std::stringstream s;

      s <<
        "% --> End visiting lpsrBookBlock" <<
        ", line " << elt->getInputLineNumber () <<
        std::endl;

      if (traceLpsrVisitors) {
        gLogStream << s.str ();
      }

      if (generateLpsrVisitingInformation) {
        fLilypondCodeStream << s.str ();
      }
    }
  }
#endif

  if (gGlobalLpsr2lilypondOahGroup->getNoBookBlock ()) {
    return;
  }

  --gIndenter; // incremented in visitStart (S_lpsrBookBlock& elt)

  if (gGlobalLpsr2lilypondOahGroup->getLilypondComments ()) {
    fLilypondCodeStream << std::left <<
      std::setw (commentFieldWidth) <<
      '}' <<
      "% book";
  }

  else {
    fLilypondCodeStream <<
      '}';
  }

  fLilypondCodeStream << std::endl;

  fOnGoingBookPartBlock = false;
}

//________________________________________________________________________
void lpsr2lilypondTranslator::visitStart (S_lpsrScoreBlock& elt)
{
#ifdef TRACING_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gGlobalLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateLpsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateLpsrVisitingInformation) {
      std::stringstream s;

      s <<
        "% --> Start visiting lpsrScoreBlock" <<
        ", line " << elt->getInputLineNumber () <<
        std::endl;

      if (traceLpsrVisitors) {
        gLogStream << s.str ();
      }

      if (generateLpsrVisitingInformation) {
        fLilypondCodeStream << s.str ();
      }
    }
  }
#endif

  fLilypondCodeStream <<
    "\\score {" <<
    std::endl;

  ++gIndenter; // decremented in visitEnd (S_lpsrScoreBlock& elt)

  fOnGoingScoreBlock = true;
}

void lpsr2lilypondTranslator::visitEnd (S_lpsrScoreBlock& elt)
{
#ifdef TRACING_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gGlobalLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateLpsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateLpsrVisitingInformation) {
      std::stringstream s;

      s <<
        "% --> End visiting lpsrScoreBlock" <<
        ", line " << elt->getInputLineNumber () <<
        std::endl;

      if (traceLpsrVisitors) {
        gLogStream << s.str ();
      }

      if (generateLpsrVisitingInformation) {
        fLilypondCodeStream << s.str ();
      }
    }
  }
#endif

  --gIndenter; // incremented in visitStart (S_lpsrScoreBlock& elt)

  if (gGlobalLpsr2lilypondOahGroup->getLilypondComments ()) {
    fLilypondCodeStream << std::left <<
      std::setw (commentFieldWidth) <<
      '}' <<
      "% score";
  }

  else {
    fLilypondCodeStream <<
      '}';
  }

  fLilypondCodeStream <<
    std::endl << std::endl; // JMI

  fOnGoingScoreBlock = false;
}

//________________________________________________________________________
void lpsr2lilypondTranslator::visitStart (S_lpsrBookPartBlock& elt)
{
#ifdef TRACING_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gGlobalLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateLpsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateLpsrVisitingInformation) {
      std::stringstream s;

      s <<
        "% --> Start visiting lpsrBookPartBlock" <<
        ", line " << elt->getInputLineNumber () <<
        std::endl;

      if (traceLpsrVisitors) {
        gLogStream << s.str ();
      }

      if (generateLpsrVisitingInformation) {
        fLilypondCodeStream << s.str ();
      }
    }
  }
#endif

  fLilypondCodeStream <<
    "\\bookpart {" <<
    std::endl;

  ++gIndenter; //  // decremented in visitEnd (S_lpsrBookPartBlock& elt)

  fOnGoingBookPartBlock = true;
}

void lpsr2lilypondTranslator::visitEnd (S_lpsrBookPartBlock& elt)
{
#ifdef TRACING_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gGlobalLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateLpsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateLpsrVisitingInformation) {
      std::stringstream s;

      s <<
        "% --> End visiting lpsrBookPartBlock" <<
        ", line " << elt->getInputLineNumber () <<
        std::endl;

      if (traceLpsrVisitors) {
        gLogStream << s.str ();
      }

      if (generateLpsrVisitingInformation) {
        fLilypondCodeStream << s.str ();
      }
    }
  }
#endif

  --gIndenter; // incremented in visitStart (S_lpsrBookPartBlock& elt)

  fLilypondCodeStream <<
    '}';

  if (gGlobalLpsr2lilypondOahGroup->getLilypondComments ()) {
    fLilypondCodeStream << std::left <<
      std::setw (commentFieldWidth) <<
      " % bookpart";
  }

  fLilypondCodeStream <<
    std::endl << std::endl;

  fOnGoingBookPartBlock = false;
}

//________________________________________________________________________
void lpsr2lilypondTranslator::visitStart (S_lpsrParallelMusicBLock& elt)
{
#ifdef TRACING_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gGlobalLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateLpsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateLpsrVisitingInformation) {
      std::stringstream s;

      s <<
        "% --> Start visiting lpsrParallelMusicBLock" <<
        ", line " << elt->getInputLineNumber () <<
        std::endl;

      if (traceLpsrVisitors) {
        gLogStream << s.str ();
      }

      if (generateLpsrVisitingInformation) {
        fLilypondCodeStream << s.str ();
      }
    }
  }
#endif

  fNumberOfPartGroupBlocks =
    elt->
      getParallelMusicBLockPartGroupBlocks ().size ();

  if (fNumberOfPartGroupBlocks > 0) {
    if (gGlobalLpsr2lilypondOahGroup->getLilypondComments ()) {
      fLilypondCodeStream << std::left <<
        std::setw (commentFieldWidth) <<
        "<<" <<
        "% parallel music";
    }

    else {
      fLilypondCodeStream <<
        "<<";
    }

    fLilypondCodeStream << std::endl;

    ++gIndenter; //  // decremented in visitEnd (S_lpsrParallelMusicBLock& elt)
  }

  fCurrentParallelMusicBLock = elt;
}

void lpsr2lilypondTranslator::visitEnd (S_lpsrParallelMusicBLock& elt)
{
#ifdef TRACING_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gGlobalLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateLpsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateLpsrVisitingInformation) {
      std::stringstream s;

      s <<
        "% --> End visiting lpsrParallelMusicBLock" <<
        ", line " << elt->getInputLineNumber () <<
        std::endl;

      if (traceLpsrVisitors) {
        gLogStream << s.str ();
      }

      if (generateLpsrVisitingInformation) {
        fLilypondCodeStream << s.str ();
      }
    }
  }
#endif

  fLilypondCodeStream << std::endl;

  if (fNumberOfPartGroupBlocks > 0) {
    --gIndenter; // incremented in visitStart (S_lpsrParallelMusicBLock& elt)

    if (gGlobalLpsr2lilypondOahGroup->getLilypondComments ()) {
      fLilypondCodeStream << std::left <<
        std::setw (commentFieldWidth) <<
        ">>" <<
        "% parallel music";
    }

    else {
      fLilypondCodeStream <<
        ">>";
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

#ifdef TRACING_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gGlobalLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateLpsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateLpsrVisitingInformation) {
      std::stringstream s;

      s <<
        "% --> Start visiting lpsrPartGroupBlock for '" <<
        partGroup->asShortString () <<
        "'" <<
        ", line " << elt->getInputLineNumber () <<
        std::endl;

      if (traceLpsrVisitors) {
        gLogStream << s.str ();
      }

      if (generateLpsrVisitingInformation) {
        fLilypondCodeStream << s.str ();
      }
    }
  }
#endif

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

  switch (partGroupImplicitKind) {
    case msrPartGroupImplicitKind::kPartGroupImplicitYes:
      // don't generate code for an implicit top-most part group block
      break;

    case msrPartGroupImplicitKind::kPartGroupImplicitNo:
      if (gGlobalLpsr2lilypondOahGroup->getLilypondComments ()) {
        fLilypondCodeStream << std::left <<
          std::setw (commentFieldWidth);
      }

      switch (partGroupSymbolKind) {
        case msrPartGroupSymbolKind::kPartGroupSymbolNone:
          fLilypondCodeStream <<
            "\\new StaffGroup";
          break;

        case msrPartGroupSymbolKind::kPartGroupSymbolBrace: // JMI
          switch (partGroupBarLineKind) {
            case msrPartGroupBarLineKind::kPartGroupBarLineYes:
              fLilypondCodeStream <<
                "\\new PianoStaff";
              break;
            case msrPartGroupBarLineKind::kPartGroupBarLineNo:
              fLilypondCodeStream <<
                "\\new GrandStaff";
              break;
          } // switch
          break;

        case msrPartGroupSymbolKind::kPartGroupSymbolBracket:
          switch (partGroupBarLineKind) {
            case msrPartGroupBarLineKind::kPartGroupBarLineYes:
              fLilypondCodeStream <<
                "\\new StaffGroup";
              break;
            case msrPartGroupBarLineKind::kPartGroupBarLineNo:
              fLilypondCodeStream <<
                "\\new ChoirStaff";
              break;
          } // switch
          break;

        case msrPartGroupSymbolKind::kPartGroupSymbolLine:
          fLilypondCodeStream <<
            "\\new StaffGroup";
          break;

        case msrPartGroupSymbolKind::kPartGroupSymbolSquare:
          fLilypondCodeStream <<
            "\\new StaffGroup";
          break;
      } // switch

#ifdef TRACING_IS_ENABLED
      if (gGlobalTracingOahGroup->getTracePartGroups ()) {
         fLilypondCodeStream <<
          " %{ " <<
          partGroup->getPartGroupCombinedName () <<
          ", abs number: " <<
          partGroup->getPartGroupAbsoluteNumber () <<
          " %} ";
      }
#endif

      // should a '\with' block be generated?
      Bool doGenerateAWithBlock (false);

      // generate the '\with' block if there's
      // a part group name or abbreviation to be generated
      if (
        partGroupName.size ()
          ||
        partGroupAbbreviation.size ()
      ) {
        doGenerateAWithBlock = true;
      }

      // generate the '\with' block
      // if the part group is not implicit
      switch (partGroupImplicitKind) {
        case msrPartGroupImplicitKind::kPartGroupImplicitYes:
          break;
        case msrPartGroupImplicitKind::kPartGroupImplicitNo:
          if (partGroupName.size ()) {
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
          "\\with {" <<
          std::endl;
      }

      ++gIndenter;

      if (doGenerateAWithBlock) { // JMI BLARK
        if (partGroupName.size ()) {
          fLilypondCodeStream <<
            "instrumentName = " <<
            nameAsLilypondString (partGroupName) <<
            std::endl;
        }
        if (partGroupAbbreviation.size ()) {
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
            if (partGroupInstrumentName.size ()) {
              fLilypondCodeStream << = "\\new PianoStaff";
            }
            else {
              fLilypondCodeStream << = "\\new GrandStaff";
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
          '}' <<
          std::endl;
      }

      if (gGlobalLpsr2lilypondOahGroup->getLilypondComments ()) {
        fLilypondCodeStream << std::left <<
          std::setw (commentFieldWidth) <<
          "<<" << "% part group " <<
          partGroup->getPartGroupCombinedNameWithoutEndOfLines ();
      }
      else {
        fLilypondCodeStream <<
          "<<";
      }

      fLilypondCodeStream << std::endl;
      break;
  } // switch

  if (partGroupInstrumentName.size ()) { // JMI
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

  fLilypondCodeStream << std::endl;

  if (elt->getPartGroupBlockElements ().size () > 1) {
    ++gIndenter; //  // decremented in visitEnd (S_lpsrPartGroupBlock& elt)
  }
}

void lpsr2lilypondTranslator::visitEnd (S_lpsrPartGroupBlock& elt)
{
#ifdef TRACING_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gGlobalLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateLpsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateLpsrVisitingInformation) {
      std::stringstream s;

      s <<
        "% --> End visiting lpsrPartGroupBlock" <<
        ", line " << elt->getInputLineNumber () <<
        std::endl;

      if (traceLpsrVisitors) {
        gLogStream << s.str ();
      }

      if (generateLpsrVisitingInformation) {
        fLilypondCodeStream << s.str ();
      }
    }
  }
#endif

  // fetch part group
  S_msrPartGroup
    partGroup =
      elt->getPartGroup ();

  if (elt->getPartGroupBlockElements ().size () > 1) {
    --gIndenter; // incremented in visitEnd (S_lpsrPartGroupBlock& elt)
  }

  switch (partGroup->getPartGroupImplicitKind ()) {
    case msrPartGroupImplicitKind::kPartGroupImplicitYes:
      // don't generate code for an implicit top-most part group block
      break;

    case msrPartGroupImplicitKind::kPartGroupImplicitNo:
      if (gGlobalLpsr2lilypondOahGroup->getLilypondComments ()) {
        fLilypondCodeStream << std::left <<
          std::setw (commentFieldWidth) << ">>" <<
          "% part group " <<
          partGroup->getPartGroupCombinedNameWithoutEndOfLines ();
      }
      else {
        fLilypondCodeStream <<
          ">>";
      }

      fLilypondCodeStream << std::endl;

      if (fPartGroupBlocksCounter != fNumberOfPartGroupBlocks) {
        fLilypondCodeStream << std::endl;
      }
      break;
  } // switch
}

//________________________________________________________________________
void lpsr2lilypondTranslator::visitStart (S_lpsrPartBlock& elt)
{
  // fetch part block's part
  S_msrPart
    part =
      elt->getPart ();

#ifdef TRACING_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gGlobalLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateLpsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateLpsrVisitingInformation) {
      std::stringstream s;

      s <<
        "% --> Start visiting lpsrPartBlock for '" <<
        part->asShortString () <<
        ", line " << elt->getInputLineNumber () <<
        std::endl;

      if (traceLpsrVisitors) {
        gLogStream << s.str ();
      }

      if (generateLpsrVisitingInformation) {
        fLilypondCodeStream << s.str ();
      }
    }
  }
#endif

  ++fPartGroupBlockElementsCounter;

  fNumberOfStaffBlocksElements =
    elt->getPartBlockElementsList ().size ();

  if (part->getPartStaveNumbersToStavesMap ().size () > 1) { // JMI
    // don't generate code here for a part with only one stave

    std::string
      partName =
        part->getPartName (),
      partAbbreviation =
        part->getPartAbbreviation ();

 // JMI       gLogStream << "@@@@@@@@@@ partName: " << partName << ", partAbbreviation: " << partAbbreviation << std::endl;

        /*
    std::string
      partInstrumentName =  // JMI
        part->getPartInstrumentName (),
      partInstrumentAbbreviation =  // JMI
        part->getPartInstrumentAbbreviation ();
        */

    if (gGlobalLpsr2lilypondOahGroup->getLilypondComments ()) {
      fLilypondCodeStream << std::left <<
        std::setw (commentFieldWidth) <<
        "\\new PianoStaff" <<
        " % part " << part->getPartCombinedName ();
    }
    else {
      fLilypondCodeStream <<
        "\\new PianoStaff";
    }
    fLilypondCodeStream << std::endl;

    // generate the 'with' block beginning
    fLilypondCodeStream <<
      "\\with {" <<
      std::endl;

    ++gIndenter;

    if (partName.size ()) {
      fLilypondCodeStream <<
        "instrumentName = \"" <<
        partName <<
        "\"" <<
        std::endl;
    }
    if (partAbbreviation.size ()) {
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
      '}' <<
      std::endl;

    fLilypondCodeStream << // JMI ??? v0.9.62
      " <<" <<
      std::endl;
  }
}

void lpsr2lilypondTranslator::visitEnd (S_lpsrPartBlock& elt)
{
  // fetch current part block's part
  S_msrPart
    part =
      elt->getPart ();

#ifdef TRACING_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gGlobalLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateLpsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateLpsrVisitingInformation) {
      std::stringstream s;

      s <<
        "% --> End visiting lpsrPartBlock" <<
        ", line " << elt->getInputLineNumber () <<
        std::endl;

      if (traceLpsrVisitors) {
        gLogStream << s.str ();
      }

      if (generateLpsrVisitingInformation) {
        fLilypondCodeStream << s.str ();
      }
    }
  }
#endif

  if (part->getPartStaveNumbersToStavesMap ().size () > 1) {
    // don't generate code for a part with only one stave
    if (gGlobalLpsr2lilypondOahGroup->getLilypondComments ()) {
      fLilypondCodeStream << std::left <<
        std::setw (commentFieldWidth) << ">>" <<
        "% part " <<
        part->getPartCombinedName ();
    }
    else {
      fLilypondCodeStream <<
        ">>";
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
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gGlobalLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateLpsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateLpsrVisitingInformation) {
      std::stringstream s;

      s <<
        "% --> Start visiting lpsrStaffBlock" <<
        ", line " << inputLineNumber <<
        std::endl;

      if (traceLpsrVisitors) {
        gLogStream << s.str ();
      }

      if (generateLpsrVisitingInformation) {
        fLilypondCodeStream << s.str ();
      }
    }
  }
#endif

  ++fStaffBlocksCounter;

  S_msrStaff
    staff =
      elt->getStaff ();

  // generate the staff context
  switch (staff->getStaffKind ()) {
    case msrStaffKind::kStaffKindRegular:
      if (gGlobalLpsr2lilypondOahGroup->getJianpu ()) {
        fLilypondCodeStream << "\\new JianpuStaff";
      }
      else {
        fLilypondCodeStream << "\\new Staff";
      }
      break;

    case msrStaffKind::kStaffKindTablature:
      fLilypondCodeStream << "\\new TabStaff";
      break;

    case msrStaffKind::kStaffKindHarmonies:
      fLilypondCodeStream << "\\new msrStaffKind::kStaffKindHarmonies???";
      break;

    case msrStaffKind::kStaffKindFiguredBass:
      fLilypondCodeStream << "\\new FiguredBassStaff???";
      break;

    case msrStaffKind::kStaffKindDrum:
      fLilypondCodeStream << "\\new DrumStaff";
      break;

    case msrStaffKind::kStaffKindRythmic:
      fLilypondCodeStream << "\\new RhythmicStaff";
      break;
    } // switch

  fLilypondCodeStream <<
    " = \"" <<
    staff->getStaffName () <<
    "\"";

  fLilypondCodeStream << std::endl;

  // generate the 'with' block beginning
  fLilypondCodeStream <<
    "\\with {" <<
    std::endl;

  ++gIndenter;

  // fetch part upLink
  S_msrPart
    staffUpLinkToPart =
      staff->getStaffUpLinkToPart ();

  // don't generate instrument names in the staves
  // if the containing part contains several of them
  if (staffUpLinkToPart->getPartStaveNumbersToStavesMap ().size () == 1) {
    // get the part upLink name to be used
    std::string partName =
      staffUpLinkToPart->
        getPartNameDisplayText ();

// JMI    gLogStream << "--> partName: \"" << partName << "\"" << std::endl;

    if (partName.size () == 0) {
      partName =
        staffUpLinkToPart->
          getPartName ();
    }

// JMI    gLogStream << "--> partName: \"" << partName << "\"" << std::endl;

    // generate the instrument name
    //* JMI BLARKBLARK
    if (
      partName.size ()
        ||
      gGlobalLpsrOahGroup->
        getLpsrStavesInstrumentsNamesMapAtom ()->
          getSetByAnOption ()
    ) {
      fLilypondCodeStream <<
        "instrumentName = ";

      std::string staffInstrumentName = partName; // default value

      // fetch the staff instrument name
      if (
        gGlobalLpsrOahGroup->
          getLpsrStavesInstrumentsNamesMapAtom ()->
            getSetByAnOption ()
      ) {
        const std::map<std::string, std::string>&
          lpsrStavesInstrumentsNamesMap =
            gGlobalLpsrOahGroup->
              getLpsrStavesInstrumentsNamesMap ();

if (true)
        mfDisplayStringToStringMap ( // JMI
          "--> lpsrStavesInstrumentsNamesMap",
          lpsrStavesInstrumentsNamesMap,
          gLogStream);

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
        // yes, split the name into a chunks std::list
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
    if (partAbbreviation.size ()) {
      fLilypondCodeStream <<
        "shortInstrumentName = " <<
        nameAsLilypondString (partAbbreviation) <<
        std::endl;
    }
    */

    // get the part upLink abbreviation display text to be used
    std::string partAbbreviation =
      staffUpLinkToPart->
        getPartAbbreviationDisplayText ();

    if (partAbbreviation.size () == 0) {
      partAbbreviation =
        staffUpLinkToPart->
          getPartAbbreviation ();
    }

/* JMI ???
    if (partAbbreviation.size ()) {
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
        // yes, split the name into a chunks std::list
        // and generate a \markup{} // JMI ???
        fLilypondCodeStream <<
          std::endl <<
          generateMultilineMarkup (partAbbreviation) <<
          std::endl;
      }
    }
    */
  }

  --gIndenter;

  // generate the std::string tunings if any
  S_msrStaffDetails
    currentStaffDetails =
      staff->getCurrentStaffStaffDetails ();

  if (currentStaffDetails) {
    const std::list<S_msrStaffTuning>&
      staffTuningsList =
        currentStaffDetails->getStaffTuningsList ();

    if (staffTuningsList.size ()) {
      fLilypondCodeStream <<
  // JMI      "restrainOpenStrings = ##t" <<
  // JMI      std::endl <<
        "stringTunings = \\stringTuning <";

      std::list<S_msrStaffTuning>::const_iterator
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
            gGlobalLpsrOahGroup->
              getLpsrQuarterTonesPitchesLanguageKind ()) <<

          absoluteOctaveAsLilypondString (
            inputLineNumber,
            staffTuning->
              getStaffTuningOctaveKind ());

        if (++i == iEnd) break;

        fLilypondCodeStream << ' ';
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
  \override LedgerLineSpanner.after-line-breaking = #grob::display-objects)",
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
    '}' <<
    std::endl;

  // generate the comment if relevant
  if (gGlobalLpsr2lilypondOahGroup->getLilypondComments ()) {
    fLilypondCodeStream << std::left <<
        std::setw (commentFieldWidth) <<
        "<<" <<
        " % staff \"" << staff->getStaffName () << "\"";
  }
  else {
    fLilypondCodeStream <<
      "<<";
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
#ifdef TRACING_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gGlobalLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateLpsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateLpsrVisitingInformation) {
      std::stringstream s;

      s <<
        "% --> End visiting lpsrStaffBlock" <<
        ", line " << elt->getInputLineNumber () <<
        std::endl;

      if (traceLpsrVisitors) {
        gLogStream << s.str ();
      }

      if (generateLpsrVisitingInformation) {
        fLilypondCodeStream << s.str ();
      }
    }
  }
#endif

  --gIndenter; // incremented in visitStart (S_lpsrStaffBlock& elt)

  if (gGlobalLpsr2lilypondOahGroup->getLilypondComments ()) {
    fLilypondCodeStream << std::left <<
      std::setw (commentFieldWidth) << ">>" <<
      "% staff " <<
      elt->getStaff ()->getStaffName ();
  }
  else {
    fLilypondCodeStream <<
      ">>";
  }

  fLilypondCodeStream << std::endl;

  if (fStaffBlocksCounter != fNumberOfStaffBlocksElements) {
    fLilypondCodeStream << std::endl;
  }
}

/*
//________________________________________________________________________
void lpsr2lilypondTranslator::visitStart (S_lpsrNewStaffgroupBlock& elt)
{
#ifdef TRACING_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gGlobalLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateLpsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateLpsrVisitingInformation) {
      std::stringstream s;

      s <<
        "% --> Start visiting lpsrNewStaffgroupBlock" <<
        ", line " << elt->getInputLineNumber () <<
        std::endl;

      if (traceLpsrVisitors) {
        gLogStream << s.str ();
      }

      if (generateLpsrVisitingInformation) {
        fLilypondCodeStream << s.str ();
      }
    }
  }
#endif

   fLilypondCodeStream <<
     "\\new StaffGroup" << ' ' << "{" <<
      std::endl;

  ++gIndenter; // decremented in visitEnd (S_lpsrStaffBlock& elt)
}

void lpsr2lilypondTranslator::visitEnd (S_lpsrNewStaffgroupBlock& elt)
{
#ifdef TRACING_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gGlobalLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateLpsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateLpsrVisitingInformation) {
      std::stringstream s;

      s <<
        "% --> End visiting lpsrNewStaffgroupBlock" <<
        ", line " << elt->getInputLineNumber () <<
        std::endl;

      if (traceLpsrVisitors) {
        gLogStream << s.str ();
      }

      if (generateLpsrVisitingInformation) {
        fLilypondCodeStream << s.str ();
      }
    }
  }
#endif

  --gIndenter; // incremented in visitStart (S_lpsrStaffBlock& elt)

  fLilypondCodeStream <<
    " }" <<
    std::endl << std::endl;
}

//________________________________________________________________________
void lpsr2lilypondTranslator::visitStart (S_lpsrNewStaffBlock& elt)
{
#ifdef TRACING_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gGlobalLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateLpsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateLpsrVisitingInformation) {
      std::stringstream s;

      s <<
        "% --> Start visiting lpsrNewStaffBlock" <<
        ", line " << elt->getInputLineNumber () <<
        std::endl;

      if (traceLpsrVisitors) {
        gLogStream << s.str ();
      }

      if (generateLpsrVisitingInformation) {
        fLilypondCodeStream << s.str ();
      }
    }
  }
#endif

  ++gIndenter; // decremented in visitEnd (S_lpsrNewStaffBlock& elt)
}

void lpsr2lilypondTranslator::visitEnd (S_lpsrNewStaffBlock& elt)
{
#ifdef TRACING_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gGlobalLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateLpsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateLpsrVisitingInformation) {
      std::stringstream s;

      s <<
        "% --> End visiting lpsrNewStaffBlock" <<
        ", line " << elt->getInputLineNumber () <<
        std::endl;

      if (traceLpsrVisitors) {
        gLogStream << s.str ();
      }

      if (generateLpsrVisitingInformation) {
        fLilypondCodeStream << s.str ();
      }
    }
  }
#endif

  --gIndenter; // incremented in visitStart (S_lpsrNewStaffBlock& elt)
}
*/

//________________________________________________________________________
void lpsr2lilypondTranslator::visitStart (S_lpsrUseVoiceCommand& elt)
{
#ifdef TRACING_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gGlobalLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateLpsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateLpsrVisitingInformation) {
      std::stringstream s;

      s <<
        "% --> Start visiting lpsrUseVoiceCommand" <<
        ", line " << elt->getInputLineNumber () <<
        std::endl;

      if (traceLpsrVisitors) {
        gLogStream << s.str ();
      }

      if (generateLpsrVisitingInformation) {
        fLilypondCodeStream << s.str ();
      }
    }
  }
#endif

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
      staffContextName = "ChordNames2"; // JMI
      voiceContextName = "???"; // JMI
      break;

    case msrStaffKind::kStaffKindFiguredBass:
      staffContextName = "FiguredBass";
      voiceContextName = "???"; // JMI
      break;

    case msrStaffKind::kStaffKindDrum:
      staffContextName = "DrumStaff";
      voiceContextName = "DrumVoice";
        // the "DrumVoice" alias exists, let's use it
      break;

    case msrStaffKind::kStaffKindRythmic:
      staffContextName = "RhythmicStaff";
      voiceContextName = "Voice";
        // no "RhythmicVoice" alias exists
      break;
  } // switch

// if (voice->getStaffRelativeVoiceNumber () > 0) { JMI
  fLilypondCodeStream <<
    "\\context " << voiceContextName << " = \"" <<
    voice->getVoiceName () << "\" <<" <<
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

        if (staffRegularVoicesCounter > 1) {
          switch (regularVoiceStaffSequentialNumber) {
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
                regularVoiceStaffSequentialNumber << ' ';
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

  std::string partName =
    part->getPartName ();

  // should a transposition be generated?
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceTranspositions ()) {
    gLogStream <<
      std::endl <<
      "Considering the generation of a voice transposition for part \"" <<
      partName <<
      "\"" <<
      std::endl;
  }
#endif

  // should we transpose fCurrentPart?
  Bool doTransposeCurrentPart (false);

  std::string partNameFoundInTranspositionMap;
  std::string partIDFoundInTranspositionMap;

  S_msrSemiTonesPitchAndOctave
    semiTonesPitchAndOctave;

  // check by name
  if (partName.size ()) { // a part name is not mandatory in MusicXML
    if (gGlobalLpsr2lilypondOahGroup->getPartNamesTranspositionMap ().size ()) {
      std::map<std::string, S_msrSemiTonesPitchAndOctave>::const_iterator
        it =
          gGlobalLpsr2lilypondOahGroup->getPartNamesTranspositionMap ().find (
            partName);

      if (it != gGlobalLpsr2lilypondOahGroup->getPartNamesTranspositionMap ().end ()) {
        // partName is present in the std::map,
        // fetch the semitones pitch and octave
        semiTonesPitchAndOctave =
          (*it).second;

        partNameFoundInTranspositionMap = partName;
        doTransposeCurrentPart = true;
      }
    }
  }

  // check by ID
  std::string partID =
    part->getPartID ();

  if (gGlobalLpsr2lilypondOahGroup->getPartIDsTranspositionMap ().size ()) {
    std::map<std::string, S_msrSemiTonesPitchAndOctave>::const_iterator
      it =
        gGlobalLpsr2lilypondOahGroup->getPartIDsTranspositionMap ().find (
          partID);

    if (it != gGlobalLpsr2lilypondOahGroup->getPartIDsTranspositionMap ().end ()) {
      // partID is present in the std::map,
      // fetch the semitones pitch and octave
      semiTonesPitchAndOctave =
        (*it).second;

      partIDFoundInTranspositionMap = partID;
      doTransposeCurrentPart = true;
    }
  }

  if (doTransposeCurrentPart) {
    // generate the transposition
#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceTranspositions ()) {
      gLogStream <<
        std::endl <<
        "Generating a voice transposition for " <<
        semiTonesPitchAndOctave->asString () <<
        " in part \"" <<
        partName <<
        "\"" <<
        ", part ID \"" <<
        partID <<
        "\"" <<
        std::endl;
    }
#endif

    // fetch the LilyPond pitch and octave
    std::string
      semiTonesPitchAndOctaveAsLilypondString =
        msrSemiTonesPitchAndOctaveAsLilypondString (
          gGlobalLpsrOahGroup->getLpsrQuarterTonesPitchesLanguageKind (),
          semiTonesPitchAndOctave);

    fLilypondCodeStream <<
       "\\transposition " <<
       semiTonesPitchAndOctaveAsLilypondString <<
       std::endl <<
       "\\transpose " <<
       semiTonesPitchAndOctaveAsLilypondString << ' ' << "c'" <<
      std::endl;
  }

  else {
    if (
      partNameFoundInTranspositionMap.size ()
        ||
      partIDFoundInTranspositionMap.size ()
    ) {
      std::stringstream s;

      if (partNameFoundInTranspositionMap.size ()) {
        s <<
          "Sorry, part name \"" <<
          partNameFoundInTranspositionMap <<
          "\" not found in LPSR";
      }

      if (partIDFoundInTranspositionMap.size ()) {
        s <<
          "Sorry, part ID \"" <<
          partIDFoundInTranspositionMap <<
          "\" not found in LPSR";
      }

      lpsr2lilypondWarning (
        __FILE__, __LINE__,
        s.str ());
    }
  }

  // generate voice name
  fLilypondCodeStream <<
    "\\" << voice->getVoiceName () << std::endl;

  --gIndenter;

  fLilypondCodeStream <<
    ">>" <<
    std::endl;
// } JMI
}

void lpsr2lilypondTranslator::visitEnd (S_lpsrUseVoiceCommand& elt)
{
#ifdef TRACING_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gGlobalLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateLpsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateLpsrVisitingInformation) {
      std::stringstream s;

      s <<
        "% --> End visiting lpsrUseVoiceCommand" <<
        ", line " << elt->getInputLineNumber () <<
        std::endl;

      if (traceLpsrVisitors) {
        gLogStream << s.str ();
      }

      if (generateLpsrVisitingInformation) {
        fLilypondCodeStream << s.str ();
      }
    }
  }
#endif
}

//________________________________________________________________________
void lpsr2lilypondTranslator::visitStart (S_lpsrNewLyricsBlock& elt)
{
#ifdef TRACING_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gGlobalLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateLpsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateLpsrVisitingInformation) {
      std::stringstream s;

      s <<
        "% --> Start visiting lpsrNewLyricsBlock" <<
        ", line " << elt->getInputLineNumber () <<
        std::endl;

      if (traceLpsrVisitors) {
        gLogStream << s.str ();
      }

      if (generateLpsrVisitingInformation) {
        fLilypondCodeStream << s.str ();
      }
    }
  }
#endif

  if (! gGlobalLpsr2lilypondOahGroup->getNoLilypondLyrics ()) {
    const S_msrStanza& stanza = elt->getStanza ();

    fLilypondCodeStream <<
      "\\new Lyrics" <<
      std::endl;

    ++gIndenter;

    fLilypondCodeStream <<
      "\\with {" <<
      std::endl;

    if (gGlobalLpsr2lilypondOahGroup->getAddLilypondStanzasNumbers ()) {
      fLilypondCodeStream <<
        gTab << "stanza = \"" <<
        stanza->getStanzaNumber () <<
        ".\"" <<
        std::endl;
    }

    fLilypondCodeStream <<
      '}' <<
      std::endl;

    switch (gGlobalLpsr2lilypondOahGroup->getLyricsDurationsKind ()) {
      case lpsrLyricsDurationsKind::kLyricsDurationsImplicit:
        fLilypondCodeStream <<
          "\\lyricsto \"" << elt->getVoice ()->getVoiceName () << "\" {" <<
          "\\" << stanza->getStanzaName () <<
          '}' <<
          std::endl;
        break;
      case lpsrLyricsDurationsKind::kLyricsDurationsExplicit:
        // no \lyricsto in that case
        fLilypondCodeStream <<
          "\\" << stanza->getStanzaName () <<
          std::endl;
        break;
    } // switch

    --gIndenter;
  }
}

void lpsr2lilypondTranslator::visitEnd (S_lpsrNewLyricsBlock& elt)
{
#ifdef TRACING_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gGlobalLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateLpsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateLpsrVisitingInformation) {
      std::stringstream s;

      s <<
        "% --> End visiting lpsrNewLyricsBlock" <<
        ", line " << elt->getInputLineNumber () <<
        std::endl;

      if (traceLpsrVisitors) {
        gLogStream << s.str ();
      }

      if (generateLpsrVisitingInformation) {
        fLilypondCodeStream << s.str ();
      }
    }
  }
#endif

  if (! gGlobalLpsr2lilypondOahGroup->getNoLilypondLyrics ()) {
    // JMI
  }
}

//________________________________________________________________________
void lpsr2lilypondTranslator::visitStart (S_lpsrVariableUseCommand& elt)
{
#ifdef TRACING_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gGlobalLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateLpsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateLpsrVisitingInformation) {
      std::stringstream s;

      s <<
        "% --> Start visiting lpsrVariableUseCommand" <<
        ", line " << elt->getInputLineNumber () <<
        std::endl;

      if (traceLpsrVisitors) {
        gLogStream << s.str ();
      }

      if (generateLpsrVisitingInformation) {
        fLilypondCodeStream << s.str ();
      }
    }
  }
#endif

  ++gIndenter; // decremented in visitEnd (S_lpsrVariableUseCommand& elt)
}

void lpsr2lilypondTranslator::visitEnd (S_lpsrVariableUseCommand& elt)
{
#ifdef TRACING_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gGlobalLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateLpsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateLpsrVisitingInformation) {
      std::stringstream s;

      s <<
        "% --> End visiting lpsrVariableUseCommand" <<
        ", line " << elt->getInputLineNumber () <<
        std::endl;

      if (traceLpsrVisitors) {
        gLogStream << s.str ();
      }

      if (generateLpsrVisitingInformation) {
        fLilypondCodeStream << s.str ();
      }
    }
  }
#endif

  --gIndenter; // incremented in visitStart (S_lpsrVariableUseCommand& elt)
}

//________________________________________________________________________
void lpsr2lilypondTranslator::visitStart (S_lpsrChordNamesContext& elt)
{
#ifdef TRACING_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gGlobalLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateLpsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateLpsrVisitingInformation) {
      std::stringstream s;

      s <<
        "% --> Start visiting lpsrChordNamesContext" <<
        ", line " << elt->getInputLineNumber () <<
        std::endl;

      if (traceLpsrVisitors) {
        gLogStream << s.str ();
      }

      if (generateLpsrVisitingInformation) {
        fLilypondCodeStream << s.str ();
      }
    }
  }
#endif

  switch (elt->getContextUseExistingKind ()) {
    case lpsrContextUseExistingKind::kUseExistingContextYes:
			fLilypondCodeStream <<
				"%{ Use existing context here %}" << // JMI ??? v0.9.66
				std::endl;
      break;
    case lpsrContextUseExistingKind::kUseExistingContextNo:
			fLilypondCodeStream <<
				"\\context ";
      break;
  } // switch

  std::string
    contextName =
      elt->getContextName ();

  fLilypondCodeStream <<
//     "\\context " << // JMI ??? v0.9.66
		lpsrContextTypeKindAsLilypondString (
			elt->getContextTypeKind ()) <<
    " = \"" << contextName << "\"" <<
    std::endl;

/* JMI
  if (false) { //option JMI
    fLilypondCodeStream <<
      "\\with {" <<
      std::endl;

    ++gIndenter;

    fLilypondCodeStream <<
      "\\override BarLine.bar-extent = #'(-2 . 2)" <<
      std::endl <<
      "\\consists \"Bar_engraver\"" <<
      std::endl;

    --gIndenter;

    fLilypondCodeStream <<
      '}' <<
      std::endl;
  }
      */

  fLilypondCodeStream <<
    "\\" << contextName <<
    std::endl << std::endl;
}

void lpsr2lilypondTranslator::visitEnd (S_lpsrChordNamesContext& elt)
{
#ifdef TRACING_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gGlobalLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateLpsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateLpsrVisitingInformation) {
      std::stringstream s;

      s <<
        "% --> End visiting lpsrChordNamesContext" <<
        ", line " << elt->getInputLineNumber () <<
        std::endl;

      if (traceLpsrVisitors) {
        gLogStream << s.str ();
      }

      if (generateLpsrVisitingInformation) {
        fLilypondCodeStream << s.str ();
      }
    }
  }
#endif
}

//________________________________________________________________________
void lpsr2lilypondTranslator::visitStart (S_lpsrFiguredBassContext& elt)
{
#ifdef TRACING_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gGlobalLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateLpsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateLpsrVisitingInformation) {
      std::stringstream s;

      s <<
        "% --> Start visiting lpsrFiguredBassContext" <<
        ", line " << elt->getInputLineNumber () <<
        std::endl;

      if (traceLpsrVisitors) {
        gLogStream << s.str ();
      }

      if (generateLpsrVisitingInformation) {
        fLilypondCodeStream << s.str ();
      }
    }
  }
#endif

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
			fLilypondCodeStream <<
				"%{ Use existing context here %}" << // JMI ??? v0.9.66
				std::endl;
      break;
    case lpsrContextUseExistingKind::kUseExistingContextNo:
			fLilypondCodeStream <<
				"\\context ";
      break;
  } // switch

  std::string
    contextName =
      elt->getContextName ();

  fLilypondCodeStream <<
		lpsrContextTypeKindAsLilypondString (
			elt->getContextTypeKind ()) <<
    " = \"" << contextName << "\"" <<
    std::endl;

/* JMI
  if (false) { //option JMI
    fLilypondCodeStream <<
      "\\with {" <<
      std::endl;

    ++gIndenter;

    fLilypondCodeStream <<
      "\\override BarLine.bar-extent = #'(-2 . 2)" <<
      std::endl <<
      "\\consists \"Bar_engraver\"" <<
      std::endl;

    --gIndenter;

    fLilypondCodeStream <<
      '}' <<
      std::endl;
  }
  */

  fLilypondCodeStream <<
    "\\" << contextName <<
    std::endl << std::endl;
}

void lpsr2lilypondTranslator::visitEnd (S_lpsrFiguredBassContext& elt)
{
#ifdef TRACING_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gGlobalLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateLpsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateLpsrVisitingInformation) {
      std::stringstream s;

      s <<
        "% --> End visiting lpsrFiguredBassContext" <<
        ", line " << elt->getInputLineNumber () <<
        std::endl;

      if (traceLpsrVisitors) {
        gLogStream << s.str ();
      }

      if (generateLpsrVisitingInformation) {
        fLilypondCodeStream << s.str ();
      }
    }
  }
#endif
}

//________________________________________________________________________
void lpsr2lilypondTranslator::visitStart (S_lpsrBarCommand& elt)
{
#ifdef TRACING_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gGlobalLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateLpsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateLpsrVisitingInformation) {
      std::stringstream s;

      s <<
        "% --> Start visiting lpsrBarCommand" <<
        ", line " << elt->getInputLineNumber () <<
        std::endl;

      if (traceLpsrVisitors) {
        gLogStream << s.str ();
      }

      if (generateLpsrVisitingInformation) {
        fLilypondCodeStream << s.str ();
      }
    }
  }
#endif
}

void lpsr2lilypondTranslator::visitEnd (S_lpsrBarCommand& elt)
{
#ifdef TRACING_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gGlobalLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateLpsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateLpsrVisitingInformation) {
      std::stringstream s;

      s <<
        "% --> End visiting lpsrBarCommand" <<
        ", line " << elt->getInputLineNumber () <<
        std::endl;

      if (traceLpsrVisitors) {
        gLogStream << s.str ();
      }

      if (generateLpsrVisitingInformation) {
        fLilypondCodeStream << s.str ();
      }
    }
  }
#endif
}

//________________________________________________________________________
void lpsr2lilypondTranslator::visitStart (S_lpsrComment& elt)
{
#ifdef TRACING_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gGlobalLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateLpsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateLpsrVisitingInformation) {
      std::stringstream s;

      s <<
        "% --> Start visiting lpsrComment" <<
        ", line " << elt->getInputLineNumber () <<
        std::endl;

      if (traceLpsrVisitors) {
        gLogStream << s.str ();
      }

      if (generateLpsrVisitingInformation) {
        fLilypondCodeStream << s.str ();
      }
    }
  }
#endif

  fLilypondCodeStream <<
    "% " << elt->getContents () <<
    std::endl;

  if (elt->getCommentGapKind () == lpsrCommentGapAfterwardsKind::kCommentGapAfterwardsYes)
    fLilypondCodeStream << std::endl;
}

void lpsr2lilypondTranslator::visitEnd (S_lpsrComment& elt)
{
#ifdef TRACING_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gGlobalLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateLpsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateLpsrVisitingInformation) {
      std::stringstream s;

      s <<
        "% --> End visiting lpsrComment" <<
        ", line " << elt->getInputLineNumber () <<
        std::endl;

      if (traceLpsrVisitors) {
        gLogStream << s.str ();
      }

      if (generateLpsrVisitingInformation) {
        fLilypondCodeStream << s.str ();
      }
    }
  }
#endif
}

//________________________________________________________________________
void lpsr2lilypondTranslator::visitStart (S_lpsrSchemeFunction& elt)
{
#ifdef TRACING_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gGlobalLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateLpsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateLpsrVisitingInformation) {
      std::stringstream s;

      s <<
        "% --> Start visiting lpsrSchemeFunction" <<
        ", line " << elt->getInputLineNumber () <<
        std::endl;

      if (traceLpsrVisitors) {
        gLogStream << s.str ();
      }

      if (generateLpsrVisitingInformation) {
        fLilypondCodeStream << s.str ();
      }
    }
  }
#endif

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
#ifdef TRACING_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gGlobalLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateLpsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateLpsrVisitingInformation) {
      std::stringstream s;

      s <<
        "% --> End visiting lpsrSchemeFunction" <<
        ", line " << elt->getInputLineNumber () <<
        std::endl;

      if (traceLpsrVisitors) {
        gLogStream << s.str ();
      }

      if (generateLpsrVisitingInformation) {
        fLilypondCodeStream << s.str ();
      }
    }
  }
#endif
}

//________________________________________________________________________
/* JMI
void lpsr2lilypondTranslator::visitStart (S_lpsrMelismaCommand& elt)
{
#ifdef TRACING_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gGlobalLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateLpsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateLpsrVisitingInformation) {
      std::stringstream s;

      s <<
        "% --> Start visiting lpsrMelismaCommand" <<
        ", line " << elt->getInputLineNumber () <<
        std::endl;

      if (traceLpsrVisitors) {
        gLogStream << s.str ();
      }

      if (generateLpsrVisitingInformation) {
        fLilypondCodeStream << s.str ();
      }
    }
  }
#endif

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
#ifdef TRACING_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gGlobalLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateLpsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateLpsrVisitingInformation) {
      std::stringstream s;

      s <<
        "% --> End visiting lpsrMelismaCommand" <<
        ", line " << elt->getInputLineNumber () <<
        std::endl;

      if (traceLpsrVisitors) {
        gLogStream << s.str ();
      }

      if (generateLpsrVisitingInformation) {
        fLilypondCodeStream << s.str ();
      }
    }
  }
#endif
}
*/

//________________________________________________________________________
void lpsr2lilypondTranslator::visitStart (S_msrScore& elt)
{
#ifdef TRACING_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gGlobalLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream s;

      s <<
        "% --> Start visiting msrScore" <<
        ", line " << elt->getInputLineNumber () <<
        std::endl;

      if (traceLpsrVisitors) {
        gLogStream << s.str ();
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << s.str ();
      }
    }
  }
#endif
}

void lpsr2lilypondTranslator::visitEnd (S_msrScore& elt)
{
#ifdef TRACING_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gGlobalLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream s;

      s <<
        "% --> End visiting msrScore" <<
        ", line " << elt->getInputLineNumber () <<
        std::endl;

      if (traceLpsrVisitors) {
        gLogStream << s.str ();
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << s.str ();
      }
    }
  }
#endif
}

//________________________________________________________________________
void lpsr2lilypondTranslator::visitStart (S_msrScaling& elt)
{
#ifdef TRACING_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gGlobalLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream s;

      s <<
        "% --> Start visiting msrScaling" <<
        ", line " << elt->getInputLineNumber () <<
        std::endl;

      if (traceLpsrVisitors) {
        gLogStream << s.str ();
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << s.str ();
      }
    }
  }
#endif
}

void lpsr2lilypondTranslator::visitEnd (S_msrScaling& elt)
{
#ifdef TRACING_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gGlobalLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream s;

      s <<
        "% --> End visiting msrScaling" <<
        ", line " << elt->getInputLineNumber () <<
        std::endl;

      if (traceLpsrVisitors) {
        gLogStream << s.str ();
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << s.str ();
      }
    }
  }
#endif
}

//________________________________________________________________________
void lpsr2lilypondTranslator::visitStart (S_msrPageLayout& elt)
{
#ifdef TRACING_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gGlobalLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream s;

      s <<
        "% --> Start visiting msrPageLayout" <<
        ", line " << elt->getInputLineNumber () <<
        std::endl;

      if (traceLpsrVisitors) {
        gLogStream << s.str ();
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << s.str ();
      }
    }
  }
#endif

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
#ifdef TRACING_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gGlobalLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream s;

      s <<
        "% --> End visiting msrPageLayout" <<
        ", line " << elt->getInputLineNumber () <<
        std::endl;

      if (traceLpsrVisitors) {
        gLogStream << s.str ();
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << s.str ();
      }
    }
  }
#endif
}

//________________________________________________________________________
void lpsr2lilypondTranslator::visitStart (S_msrSystemLayout& elt)
{
#ifdef TRACING_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gGlobalLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream s;

      s <<
        "% --> Start visiting msrSystemLayout" <<
        ", line " << elt->getInputLineNumber () <<
        std::endl;

      if (traceLpsrVisitors) {
        gLogStream << s.str ();
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << s.str ();
      }
    }
  }
#endif
}

void lpsr2lilypondTranslator::visitEnd (S_msrSystemLayout& elt)
{
#ifdef TRACING_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gGlobalLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream s;

      s <<
        "% --> End visiting msrSystemLayout" <<
        ", line " << elt->getInputLineNumber () <<
        std::endl;

      if (traceLpsrVisitors) {
        gLogStream << s.str ();
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << s.str ();
      }
    }
  }
#endif
}

//________________________________________________________________________
void lpsr2lilypondTranslator::visitStart (S_msrStaffLayout& elt)
{
#ifdef TRACING_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gGlobalLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream s;

      s <<
        "% --> Start visiting msrStaffLayout" <<
        ", line " << elt->getInputLineNumber () <<
        std::endl;

      if (traceLpsrVisitors) {
        gLogStream << s.str ();
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << s.str ();
      }
    }
  }
#endif
}

void lpsr2lilypondTranslator::visitEnd (S_msrStaffLayout& elt)
{
#ifdef TRACING_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gGlobalLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream s;

      s <<
        "% --> End visiting msrStaffLayout" <<
        ", line " << elt->getInputLineNumber () <<
        std::endl;

      if (traceLpsrVisitors) {
        gLogStream << s.str ();
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << s.str ();
      }
    }
  }
#endif
}

//________________________________________________________________________
void lpsr2lilypondTranslator::visitStart (S_msrAppearance& elt)
{
#ifdef TRACING_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gGlobalLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream s;

      s <<
        "% --> Start visiting msrAppearance" <<
        ", line " << elt->getInputLineNumber () <<
        std::endl;

      if (traceLpsrVisitors) {
        gLogStream << s.str ();
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << s.str ();
      }
    }
  }
#endif
}

void lpsr2lilypondTranslator::visitEnd (S_msrAppearance& elt)
{
#ifdef TRACING_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gGlobalLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream s;

      s <<
        "% --> End visiting msrAppearance" <<
        ", line " << elt->getInputLineNumber () <<
        std::endl;

      if (traceLpsrVisitors) {
        gLogStream << s.str ();
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << s.str ();
      }
    }
  }
#endif
}

//________________________________________________________________________
void lpsr2lilypondTranslator::visitStart (S_msrCredit& elt)
{
#ifdef TRACING_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gGlobalLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream s;

      s <<
        "% --> Start visiting msrCredit" <<
        ", line " << elt->getInputLineNumber () <<
        std::endl;

      if (traceLpsrVisitors) {
        gLogStream << s.str ();
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << s.str ();
      }
    }
  }
#endif
}

void lpsr2lilypondTranslator::visitEnd (S_msrCredit& elt)
{
#ifdef TRACING_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gGlobalLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream s;

      s <<
        "% --> End visiting msrCredit" <<
        ", line " << elt->getInputLineNumber () <<
        std::endl;

      if (traceLpsrVisitors) {
        gLogStream << s.str ();
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << s.str ();
      }
    }
  }
#endif
}

void lpsr2lilypondTranslator::visitStart (S_msrCreditWords& elt)
{
#ifdef TRACING_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gGlobalLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream s;

      s <<
        "% --> Start visiting msrCreditWords" <<
        ", line " << elt->getInputLineNumber () <<
        std::endl;

      if (traceLpsrVisitors) {
        gLogStream << s.str ();
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << s.str ();
      }
    }
  }
#endif
}

void lpsr2lilypondTranslator::visitEnd (S_msrCreditWords& elt)
{
#ifdef TRACING_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gGlobalLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream s;

      s <<
        "% --> End visiting msrCreditWords" <<
        ", line " << elt->getInputLineNumber () <<
        std::endl;

      if (traceLpsrVisitors) {
        gLogStream << s.str ();
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << s.str ();
      }
    }
  }
#endif
}

//________________________________________________________________________
void lpsr2lilypondTranslator::visitStart (S_msrPartGroup& elt)
{
#ifdef TRACING_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gGlobalLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream s;

      s <<
        "% --> Start visiting msrPartGroup" <<
        elt->asShortString () <<
        ", line " << elt->getInputLineNumber () <<
        elt->getPartGroupCombinedName () <<
        std::endl;

      if (traceLpsrVisitors) {
        gLogStream << s.str ();
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << s.str ();
      }
    }
  }
#endif
}

void lpsr2lilypondTranslator::visitEnd (S_msrPartGroup& elt)
{
#ifdef TRACING_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gGlobalLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream s;

      s <<
        "% --> End visiting msrPartGroup" <<
        elt->getPartGroupCombinedName () <<
        ", line " << elt->getInputLineNumber () <<
        std::endl;

      if (traceLpsrVisitors) {
        gLogStream << s.str ();
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << s.str ();
      }
    }
  }
#endif
}

//________________________________________________________________________
void lpsr2lilypondTranslator::visitStart (S_msrPart& elt)
{
  std::string
    partCombinedName =
      elt->getPartCombinedName ();

#ifdef TRACING_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gGlobalLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream s;

      s <<
        "% --> Start visiting msrPart" <<
        partCombinedName <<
        ", line " << elt->getInputLineNumber () <<
        std::endl;

      if (traceLpsrVisitors) {
        gLogStream << s.str ();
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << s.str ();
      }
    }
  }
#endif

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceParts ()) {
    gLogStream <<
      std::endl <<
      "<!--=== part \"" << partCombinedName << "\"" <<
      ", line " << elt->getInputLineNumber () << " ===-->" <<
      std::endl;
  }
#endif

  // remember current part
  fCurrentPart = elt;

  fRemainingMultipleFullBarRestsNumber = 0; // JMI
  fOnGoingMultipleFullBarRests = false; // JMI
}

void lpsr2lilypondTranslator::visitEnd (S_msrPart& elt)
{
#ifdef TRACING_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gGlobalLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream s;

      s <<
        "% --> End visiting msrPart" <<
        elt->getPartCombinedName () <<
        ", line " << elt->getInputLineNumber () <<
        std::endl;

      if (traceLpsrVisitors) {
        gLogStream << s.str ();
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << s.str ();
      }
    }
  }
#endif

  // forget about current part
  fCurrentPart = nullptr;
}

//________________________________________________________________________
void lpsr2lilypondTranslator::visitStart (S_msrStaff& elt)
{
#ifdef TRACING_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gGlobalLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream s;

      s <<
        "% --> Start visiting msrStaff \"" <<
        elt->getStaffName () <<
        "\"" <<
        ", line " << elt->getInputLineNumber () <<
        std::endl;

      if (traceLpsrVisitors) {
        gLogStream << s.str ();
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << s.str ();
      }
    }
  }
#endif

  fOnGoingStaff = true;
}

void lpsr2lilypondTranslator::visitEnd (S_msrStaff& elt)
{
#ifdef TRACING_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gGlobalLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream s;

      s <<
        "% --> End visiting msrStaff \"" <<
        elt->getStaffName () <<
        "\"" <<
        ", line " << elt->getInputLineNumber () <<
        std::endl;

      if (traceLpsrVisitors) {
        gLogStream << s.str ();
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << s.str ();
      }
    }
  }
#endif

  fOnGoingStaff = false;
}

void lpsr2lilypondTranslator::visitStart (S_msrStaffTuning& elt)
{
#ifdef TRACING_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gGlobalLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream s;

      s <<
        "%--> Start visiting msrStaffTuning" <<
        std::endl;

        if (traceLpsrVisitors) {
          gLogStream << s.str ();
        }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << s.str ();
      }
    }
  }
#endif

/* JMI
  const std::list<S_msrStaffTuning>&
    staffTuningsList =
      staff->getStaffTuningsList ();

  if (staffTuningsList.size ()) {
    // \set TabStaff.stringTunings = \stringTuning <c' g' d'' a''>

    fLilypondCodeStream <<
      "\\set TabStaff.stringTunings = \\stringTuning <";

    std::list<S_msrStaffTuning>::const_iterator
      iBegin = staffTuningsList.begin (),
      iEnd   = staffTuningsList.end (),
      i      = iBegin;

    for ( ; ; ) {
      fLilypondCodeStream <<
        msrQuarterTonesPitchAsString (
          gGlobalLpsrOahGroup->getLpsrQuarterTonesPitchesLanguage (),
 // JMI            elt->getInputLineNumber (),
          ((*i)->getStaffTuningQuarterTonesPitch ())) <<
 // JMI       char (tolower ((*i)->getStaffTuningStep ())) <<
        absoluteOctaveAsLilypondString (
          inputLineNumber,
          (*i)->getStaffTuningOctaveKind ());
      if (++i == iEnd) break;
      fLilypondCodeStream << ' ';
    } // for

    fLilypondCodeStream <<
      ">" <<
      std::endl;
  }
 */
}

void lpsr2lilypondTranslator::visitStart (S_msrStaffDetails& elt)
{
#ifdef TRACING_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gGlobalLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream s;

      s <<
        "%--> Start visiting msrStaffDetails" <<
        std::endl;

      if (traceLpsrVisitors) {
        gLogStream << s.str ();
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << s.str ();
      }
    }
  }
#endif

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
#ifdef TRACING_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gGlobalLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream s;

      s <<
        "% --> Start visiting msrVoice \"" <<
        elt->getVoiceName () <<
        "\"" <<
        ", line " << elt->getInputLineNumber () <<
        std::endl;

      if (traceLpsrVisitors) {
        gLogStream << s.str ();
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << s.str ();
      }
    }
  }
#endif

  fCurrentVoice = elt;

  fLilypondCodeStream <<
    fCurrentVoice->getVoiceName () <<
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
              ' ' <<
              msrSemiTonesPitchAndOctaveAsLilypondString (
                gGlobalLpsrOahGroup->
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
              gGlobalLpsrOahGroup->
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

  // use a 'global' macro?
  if (gGlobalLpsr2lilypondOahGroup->getGlobal ()) {
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
        gGlobalLpsrOahGroup->
          getLpsrQuarterTonesPitchesLanguageKind ()) <<
      "\"" <<
      std::endl;
  }

  // chords language?
  if (! gGlobalLpsr2lilypondOahGroup->getUseLilypondDefaultLanguages ()) { // JMI HasBeenSet???
    if (
      gGlobalLpsrOahGroup->getLpsrChordsLanguageKind ()
        !=
      lpsrChordsLanguageKind::kChordsIgnatzek
    ) {
      fLilypondCodeStream <<
        "\\" <<
        lpsrChordsLanguageKindAsString (
          gGlobalLpsrOahGroup->
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
  if (gGlobalLpsr2lilypondOahGroup->getBoxAroundBarNumberSet ().size ()) {
    // yes, center the boxed bar number
#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceMeasuresNumbers ()) {
      gLogStream <<
        std::endl <<
        "Centering boxed LilyPond measure numbers" <<
        ", line " << elt->getInputLineNumber () << " ===-->" <<
        std::endl;
    }
#endif

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

  if (dynamicsTextSpannersStyleKindAtom->getSetByAnOption ()) {
    fLilypondCodeStream <<
      "\\override DynamicTextSpanner.style = #'" <<
      lpsrDynamicsTextSpannersStyleKindAsString (
       dynamicsTextSpannersStyleKindAtom->
         getLpsrdDynamicsTextSpannersStyleKindVariable ()) <<
      std::endl << std::endl;
  }

  // compress multiple full-bar rests?
  if (gGlobalLpsr2lilypondOahGroup->getCompressFullBarRestsInLilypond ()) { // JMI v0.9.64
    fLilypondCodeStream <<
      "\\compressMMRests" << " %{ JMI v0.9.64 ??? %}" <<
      std::endl <<
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

  fVoiceIsCurrentlySenzaMisura = false;

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
  fLastMetWholeNotes = Rational (0, 1);

  // reset current stem kind
  fCurrentStemKind = msrStemKind::kStemNeutral; // default value
}

void lpsr2lilypondTranslator::visitEnd (S_msrVoice& elt)
{
  --gIndenter; // incremented in visitStart (S_msrVoice& elt)

#ifdef TRACING_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gGlobalLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream s;

      s <<
        "% --> End visiting msrVoice \"" <<
        elt->getVoiceName () <<
        "\"" <<
        ", line " << elt->getInputLineNumber () <<
        std::endl;

      if (traceLpsrVisitors) {
        gLogStream << s.str ();
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << s.str ();
      }
    }
  }
#endif

  /* JMI
  if (
    fCurrentVoice->getVoiceContainsMultipleFullBarRests ()
      ||
    gGlobalLpsr2lilypondOahGroup->getCompressFullBarRestsInLilypond ()
  ) {
    fLilypondCodeStream <<
  // JMI    '}' <<
      std::endl;

    --g Indenter; // JMI ???
  */

  if (gGlobalLpsr2lilypondOahGroup->getDisplayMusic ()) {
    fLilypondCodeStream <<
      '}';

    if (gGlobalLpsr2lilypondOahGroup->getLilypondComments ()) {
      fLilypondCodeStream << std::left <<
        std::setw (commentFieldWidth) <<
        " % displayMusic";
    }

    fLilypondCodeStream << std::endl;

    --gIndenter; // incremented in visitStart (S_msrVoice& elt)
  }

  // generate the end of the voice definition
  if (gGlobalLpsr2lilypondOahGroup->getLilypondComments ()) {
    fLilypondCodeStream << std::left <<
      std::setw (commentFieldWidth) <<
      '}' <<
      "% voice";
  }

  else {
    fLilypondCodeStream <<
      '}';
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

  fCurrentVoice = nullptr;
  fOnGoingVoice = false;
}

//________________________________________________________________________
void lpsr2lilypondTranslator::visitStart (S_msrVoiceStaffChange& elt)
{
#ifdef TRACING_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gGlobalLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream s;

      s <<
        "% --> Start visiting msrVoiceStaffChange '" <<
        elt->asString () << "'" <<
        ", line " << elt->getInputLineNumber () <<
        std::endl;

      if (traceLpsrVisitors) {
        gLogStream << s.str ();
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << s.str ();
      }
    }
  }
#endif

  fLilypondCodeStream <<
    std::endl <<
    "\\change Staff=\"" <<
    elt->getStaffToChangeTo ()->getStaffName () <<
    "\"";

  // generate the input line number as comment if relevant
  if (
    gGlobalLpsr2lilypondOahGroup->getInputLineNumbers ()
      ||
    gGlobalLpsr2lilypondOahGroup->getGenerateMeasurePositions ()
  ) {
    generateInputLineNumberAndOrMeasurePositionAsAComment (
      elt);
  }

  fLilypondCodeStream << std::endl;
}

//________________________________________________________________________
void lpsr2lilypondTranslator::visitStart (S_msrHarmony& elt)
{
#ifdef TRACING_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gGlobalLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream s;

      s <<
        "% --> Start visiting msrHarmony '" <<
        elt->asString () <<
        "'" <<
        ", fOnGoingNotesStack.size () = " <<
        fOnGoingNotesStack.size () <<
        ", fOnGoingChord = " <<
        fOnGoingChord <<
        ", fOnGoingHarmoniesVoice = " <<
        fOnGoingHarmoniesVoice <<
        ", line " << elt->getInputLineNumber () <<
        std::endl;

      if (traceLpsrVisitors) {
        gLogStream << s.str ();
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << s.str ();
      }
    }
  }
#endif

  if (fOnGoingNotesStack.size () > 0) {
  /* JMI
#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceHarmonies ()) {
      fLilypondCodeStream <<
        "%{ fOnGoingNotesStack.size () S_msrHarmony JMI " <<
        elt->asString () <<
        " %}" <<
        std::endl;
    }
#endif
*/
  }

  else if (fOnGoingChord) { // JMI
  }

  else if (fOnGoingHarmoniesVoice) {
    // actual LilyPond code generation
    fLilypondCodeStream <<
      harmonyAsLilypondString (elt) <<
      ' ';

    // generate the input line number as comment if relevant
    if (
      gGlobalLpsr2lilypondOahGroup->getInputLineNumbers ()
        ||
      gGlobalLpsr2lilypondOahGroup->getGenerateMeasurePositions ()
    ) {
      generateInputLineNumberAndOrMeasurePositionAsAComment (
        elt);
    }
  }
}

//________________________________________________________________________
void lpsr2lilypondTranslator::visitStart (S_msrFrame& elt)
{
#ifdef TRACING_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gGlobalLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream s;

      s <<
        "% --> Start visiting msrHarmony " <<
        elt->asString () <<
        ", line " << elt->getInputLineNumber () <<
        std::endl;

      if (traceLpsrVisitors) {
        gLogStream << s.str ();
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << s.str ();
      }
    }
  }
#endif

  if (fOnGoingNotesStack.size () > 0) {
#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceFrames ()) {
      fLilypondCodeStream <<
        "%{ " << elt->asString () << " %}" <<
        std::endl;
    }
#endif

    fLilypondCodeStream <<
      frameAsLilypondString (elt) <<
      std::endl;
  }
}

//________________________________________________________________________
void lpsr2lilypondTranslator::visitStart (S_msrFiguredBass& elt)
{
#ifdef TRACING_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gGlobalLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream s;

      s <<
        "% --> Start visiting msrFiguredBass '" <<
        elt->asString () <<
        "'" <<
        ", fOnGoingNotesStack.size () = " <<
        fOnGoingNotesStack.size () <<
        ", fOnGoingChord = " <<
        fOnGoingChord <<
        ", fOnGoingFiguredBassVoice = " <<
        fOnGoingFiguredBassVoice <<
        ", line " << elt->getInputLineNumber () <<
        std::endl;

      if (traceLpsrVisitors) {
        gLogStream << s.str ();
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << s.str ();
      }
    }
  }
#endif

  fCurrentFiguredBass = elt;

  if (fOnGoingNotesStack.size () > 0) {
#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceFiguredBass ()) {
      fLilypondCodeStream <<
        "%{ fOnGoingNotesStack.size () S_msrFiguredBass JMI " <<
        fCurrentFiguredBass->asString () <<
        " %}" <<
        std::endl;
    }
#endif
  }

  else if (fOnGoingChord) { // JMI
  }

  else if (fOnGoingFiguredBassVoice) {
    fLilypondCodeStream <<
      figuredBassAsLilypondString (elt) <<
      ' ';

    // generate the input line number as comment if relevant
    if (
      gGlobalLpsr2lilypondOahGroup->getInputLineNumbers ()
        ||
      gGlobalLpsr2lilypondOahGroup->getGenerateMeasurePositions ()
    ) {
      generateInputLineNumberAndOrMeasurePositionAsAComment (
        fCurrentFiguredBass);
    }
  }

  fCurrentFiguredBassFiguresCounter = 0;
}

void lpsr2lilypondTranslator::visitStart (S_msrBassFigure& elt)
{
/* JMI
#ifdef TRACING_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gGlobalLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream s;

      s <<
        "% --> Start visiting msrBassFigure '" <<
        elt->asString () <<
        "'" <<
        ", line " << elt->getInputLineNumber () <<
        std::endl;

      if (traceLpsrVisitors) {
        gLogStream << s.str ();
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << s.str ();
      }
    }
  }
#endif

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
      case msrBassFigurePrefixKind::kBassFigurePrefix_NO_:
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
      case msrBassFigureSuffixKind::kBassFigureSuffix_NO_:
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
      fLilypondCodeStream << ' ';
    }
  }
  */
}

void lpsr2lilypondTranslator::visitEnd (S_msrFiguredBass& elt)
{
#ifdef TRACING_IS_ENABLED
  int inputLineNumber =
    elt->getInputLineNumber ();

  {
    Bool
      traceLpsrVisitors =
        gGlobalLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream s;

      s <<
        "% --> End visiting msrFiguredBass '" <<
        elt->asString () <<
        "'" <<
        ", line " << inputLineNumber <<
        std::endl;

      if (traceLpsrVisitors) {
        gLogStream << s.str ();
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << s.str ();
      }
    }
  }
#endif

/* JMI
  if (fOnGoingFiguredBassVoice) {
    fLilypondCodeStream <<
      ">";

    // generate figured bass duration
    msrTupletFactor
      figuredBassTupletFactor =
        elt->getFiguredBassTupletFactor ();

    if (figuredBassTupletFactor.isEqualToOne ()) {
      // use figured bass sounding whole notes
      fLilypondCodeStream <<
        durationAsLilypondString (
          inputLineNumber,
          elt->
            getFiguredBassSoundingWholeNotes ());
    }
    else {
      // use figured bass display whole notes and tuplet factor
      fLilypondCodeStream <<
        durationAsLilypondString (
          inputLineNumber,
          elt->
            getFiguredBassDisplayWholeNotes ()) <<
        "*" <<
        figuredBassTupletFactor.asRational ();
    }

    fLilypondCodeStream <<
      ' ';
  }
  */
}

//________________________________________________________________________
void lpsr2lilypondTranslator::visitStart (S_msrSegment& elt)
{
#ifdef TRACING_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gGlobalLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream s;

      s <<
        "%--> Start visiting msrSegment '" <<
        elt->getSegmentAbsoluteNumber () << "'" <<
        std::endl;

      if (traceLpsrVisitors) {
        gLogStream << s.str ();
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << s.str ();
      }
    }
  }
#endif

  if (gGlobalLpsr2lilypondOahGroup->getLilypondComments ()) {
    // generate the end of the voice definition
    fLilypondCodeStream << std::left <<
      std::setw (commentFieldWidth) <<
      "" <<
      "% start of segment " <<
      elt->getSegmentAbsoluteNumber () <<
      ", line " <<
      elt->getInputLineNumber () <<
      std::endl;

    ++gIndenter; // decremented in visitEnd (S_msrSegment& elt)
  }
}

void lpsr2lilypondTranslator::visitEnd (S_msrSegment& elt)
{
#ifdef TRACING_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gGlobalLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream s;

      s <<
        "%--> End visiting msrSegment '" <<
        elt->getSegmentAbsoluteNumber () << "'" <<
        std::endl;

      if (traceLpsrVisitors) {
        gLogStream << s.str ();
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << s.str ();
      }
    }
  }
#endif

  if (gGlobalLpsr2lilypondOahGroup->getLilypondComments ()) {
    --gIndenter; // incremented in visitStart (S_msrSegment& elt)

    fLilypondCodeStream << std::left <<
      std::setw (commentFieldWidth) <<
      "" <<
      "% end of segment" <<
      elt->getSegmentAbsoluteNumber () <<
      ", line " <<
      elt->getInputLineNumber () <<
      std::endl;
  }
}

//________________________________________________________________________
void lpsr2lilypondTranslator::visitStart (S_msrMeasure& elt)
{
  int
    inputLineNumber =
      elt->getInputLineNumber ();

  std::string
    measureNumber =
      elt->getMeasureNumber ();

  msrMeasureKind
    measureKind =
      elt->getMeasureKind ();

  int
    measurePuristNumber =
      elt->getMeasurePuristNumber ();

#ifdef TRACING_IS_ENABLED
  msrMeasureEndRegularKind
    measureEndRegularKind =
      elt-> getMeasureEndRegularKind ();
#endif

#ifdef TRACING_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gGlobalLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream s;

      s <<
        "% --> Start visiting msrMeasure '" << measureNumber << '\'' <<
        ", measureKind: " << measureKind <<
        ", measureEndRegularKind: " << measureEndRegularKind <<
        ", measurePuristNumber: '" << measurePuristNumber << '\'' <<
        ", fOnGoingMultipleFullBarRests: " << fOnGoingMultipleFullBarRests <<
        ", line " << inputLineNumber <<
        std::endl;

      if (traceLpsrVisitors) {
        gLogStream << s.str ();
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << s.str ();
      }
    }
  }
#endif

  // generate measure start comment if relevant
  if (gGlobalLpsr2lilypondOahGroup->getLilypondComments ()) {
    fLilypondCodeStream << std::left <<
      std::setw (commentFieldWidth) <<
      "" <<
      "% start of " <<
      elt->getMeasureKind () <<
      " measure " <<
      measureNumber <<
      ", line " << inputLineNumber <<
      std::endl;

    ++gIndenter; // decremented in visitEnd (S_msrMeasure& elt)
  }

  // should we generate a box around this bar number?
  {
    std::set<int>::const_iterator
      it =
        gGlobalLpsr2lilypondOahGroup->
          getBoxAroundBarNumberSet ().find (measurePuristNumber);

    if (
      gGlobalLpsr2lilypondOahGroup->getBoxAroundAllBarNumbers ()
        ||
      it != gGlobalLpsr2lilypondOahGroup->getBoxAroundBarNumberSet ().end ()
    ) {
      // yes, generate a box around the bar number
  #ifdef TRACING_IS_ENABLED
      if (gGlobalTracingOahGroup->getTraceMeasuresNumbers ()) {
        gLogStream <<
          std::endl <<
          "Generating a box around LilyPond measure purist number '" <<
          measurePuristNumber <<
          "', line " << inputLineNumber << " ===-->" <<
          std::endl;
      }
  #endif

      fLilypondCodeStream <<
        "\\boxAroundNextBarNumber" <<
        std::endl;
    }
  }

  // should we reset the measure purist number?
  {
    std::map<std::string, int>::const_iterator
      it =
        gGlobalLpsr2lilypondOahGroup->
          getResetMeasureElementMeasureNumberMap ().find (measureNumber);

    if (
      it
        !=
      gGlobalLpsr2lilypondOahGroup->getResetMeasureElementMeasureNumberMap ().end ()
    ) {
      // yes, reset measure number
      int lilypondMeasureNumber = (*it).second;

      if (std::to_string (lilypondMeasureNumber) != measureNumber) {
  #ifdef TRACING_IS_ENABLED
        if (gGlobalTracingOahGroup->getTraceMeasuresNumbers ()) {
          gLogStream <<
            std::endl <<
            "Resetting LilyPond measure number from '" <<
            measureNumber <<
            "' to " <<
            lilypondMeasureNumber <<
            "', line " << inputLineNumber << " ===-->" <<
            std::endl;
        }
  #endif

        fLilypondCodeStream <<
          "\\set Score.currentBarNumber = #" <<
          lilypondMeasureNumber <<
          std::endl;
      }
      else {
  #ifdef TRACING_IS_ENABLED
        if (gGlobalTracingOahGroup->getTraceMeasuresNumbers ()) {
          gLogStream <<
            std::endl <<
            "Cannot reset measure LilyPond number from '" <<
            measureNumber <<
            "' to " <<
            lilypondMeasureNumber <<
            ": they're one and the same" <<
            "', line " << inputLineNumber << " ===-->" <<
            std::endl;
        }
  #endif
      }
    }
  }

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasures ()) {
    gLogStream <<
      std::endl <<
      "% <!--=== measure '" << measureNumber << "' start, " <<
      ", measureEndRegularKind = '" << measureEndRegularKind <<
      ", measurePuristNumber = '" << measurePuristNumber <<
      ", fOnGoingMultipleFullBarRests = " << fOnGoingMultipleFullBarRests <<
      ", line " << inputLineNumber <<
      " ===-->" <<
      std::endl;
  }
#endif

  // should we show the measure number???
  std::set<std::string, int>::const_iterator
    it =
      gGlobalLpsr2lilypondOahGroup->
        getShowNumbersAtMeasureSet ().find (measureNumber); //JMI variable

  if (
    it
      !=
    gGlobalLpsr2lilypondOahGroup->getShowNumbersAtMeasureSet ().end ()
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

    case msrMeasureKind::kMeasureKindIncompleteStandalone: // JMI v0.9.63
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

    case msrMeasureKind::kMeasureKindOvercomplete:
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
  fLastMetWholeNotes = Rational (0, 1);

  // is this the end of a cadenza?
  if (
    fOnGoingVoiceCadenza
      &&
    measureKind != msrMeasureKind::kMeasureKindOvercomplete
  ) {
  /* JMI
    fLilypondCodeStream <<
      std::endl <<
      "\\cadenzaOff" <<
      std::endl <<
 // JMI     "\\undo \\omit Staff.TimeSignature" <<
      std::endl <<
      "\\bar \"|\" "; // JMI ???
      */

    if (gGlobalLpsr2lilypondOahGroup->getLilypondComments ()) {
      fLilypondCodeStream <<
        " % msrMeasureKind::kMeasureKindOvercomplete End";
    }

    fLilypondCodeStream <<
      std::endl;

    fOnGoingVoiceCadenza = false;
  }

  switch (measureKind) {
    case msrMeasureKind::kMeasureKindUnknown:
      {
        std::stringstream s;

        s <<
          "measure '" << measureNumber <<
          "' is of unknown kind in lpsr2lilypondTranslator";

// if (false) // JMI
//         lpsr2lilypondInternalError (
//           gGlobalServiceRunData->getInputSourceName (),
//           inputLineNumber,
//           __FILE__, __LINE__,
//           s.str ());
// else
        lpsr2lilypondInternalWarning (
          gGlobalServiceRunData->getInputSourceName (),
          inputLineNumber,
          s.str ());
      }
      break;

    case msrMeasureKind::kMeasureKindRegular:
      break;

    case msrMeasureKind::kMeasureKindAnacrusis:
      if (elt->getMeasureIsFirstInVoice ()) {
        // only generate '\partial' at the beginning of a voice // this code SUPERFLOUS??? JMI v0.9.66
        std::string
          upbeatDuration =
            wholeNotesAsLilypondString (
              inputLineNumber,
              elt->getMeasureWholeNotesDuration ());

        fLilypondCodeStream <<
          "\\partial " << upbeatDuration <<
          std::endl;
      }
      break;

    case msrMeasureKind::kMeasureKindIncompleteStandalone:
      if (
      	elt->getMeasureIsFirstInVoice ()
          &&
        elt->fetchMeasureUpLinkToScore ()->getScoreNumberOfMeasures () > 1
      ) {
        // don't generate '\partial' at the beginning of a voice
        std::string
          upbeatDuration =
            wholeNotesAsLilypondString (
              inputLineNumber,
              elt->getMeasureWholeNotesDuration ());

        fLilypondCodeStream <<
          "\\partial " << upbeatDuration <<
          std::endl;
      }
      break;

    case msrMeasureKind::kMeasureKindIncompleteLastInRepeatCommonPart:
    case msrMeasureKind::kMeasureKindIncompleteLastInRepeatHookedEnding:
    case msrMeasureKind::kMeasureKindIncompleteLastInRepeatHooklessEnding:
    case msrMeasureKind::kMeasureKindIncompleteNextMeasureAfterCommonPart:
    case msrMeasureKind::kMeasureKindIncompleteNextMeasureAfterHookedEnding:
    case msrMeasureKind::kMeasureKindIncompleteNextMeasureAfterHooklessEnding:
      {
        Rational
          measureWholeNotesDuration =
            elt->getMeasureWholeNotesDuration ();

        Rational
          fullMeasureWholeNotesDuration =
            elt->getFullMeasureWholeNotesDuration ();

        // we should set the score measure whole notes in this case
        Rational
          ratioToFullMeasureWholeNotesDuration =
            measureWholeNotesDuration / fullMeasureWholeNotesDuration;

#ifdef TRACING_IS_ENABLED
        if (gGlobalTracingOahGroup->getTraceMeasuresDetails ()) {
          int fieldWidth = 27;

          fLilypondCodeStream << std::left <<
            "% Setting the measure whole notes for measure " <<
            std::setw (fieldWidth) <<
            measureNumber <<
            ", line = " << inputLineNumber <<
            std::endl <<
            std::setw (fieldWidth) <<
            "% measureWholeNotesDuration" << " = " <<
            measureWholeNotesDuration <<
            std::endl <<
            std::setw (fieldWidth) <<
            "% fullMeasureWholeNotesDuration" << " = " <<
            fullMeasureWholeNotesDuration <<
            std::endl <<
            std::setw (fieldWidth) <<
            "% ratioToFullMeasureWholeNotesDuration" << " = " <<
            ratioToFullMeasureWholeNotesDuration <<
            std::endl << std::endl;
        }
#endif

        if (ratioToFullMeasureWholeNotesDuration == Rational (1, 1)) {
          std::stringstream s;

          s <<
            "underfull measure '" << measureNumber <<
            "' has actually the full measure whole notes";

     // JMI       lpsr2lilypondInternalError (
          lpsr2lilypondInternalWarning (
            gGlobalServiceRunData->getInputSourceName (),
            inputLineNumber,
    // JMI        __FILE__, __LINE__,
            s.str ());
        }

        else {
          /* JMI
          fLilypondCodeStream <<
            "\\set Score.measureLength = #(ly:make-moment " <<
            measureWholeNotesDuration.toString () <<
            ")" <<
            std::endl;
    */

          // should we generate a line break?
          if (gGlobalLpsr2lilypondOahGroup->getBreakLinesAtIncompleteRightMeasures ()) {
            fLilypondCodeStream <<
              "\\break" <<
              std::endl;
          }
        }
      }
      break;

    case msrMeasureKind::kMeasureKindOvercomplete:
      if (! fOnGoingVoiceCadenza) {
        fLilypondCodeStream <<
          std::endl <<
          "\\cadenzaOn" <<
          std::endl <<
          "\\omit Staff.TimeSignature";

        if (gGlobalLpsr2lilypondOahGroup->getLilypondComments ()) {
          fLilypondCodeStream << " % msrMeasureKind::kMeasureKindOvercomplete Start";
        }

        fLilypondCodeStream << std::endl;

        fOnGoingVoiceCadenza = true;
      }
      break;

    case msrMeasureKind::kMeasureKindCadenza:
      if (! fOnGoingVoiceCadenza) {
        fLilypondCodeStream <<
          std::endl <<
          "\\cadenzaOn" <<
          std::endl;

        if (gGlobalLpsr2lilypondOahGroup->getLilypondComments ()) {
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
      {
        // fetch measure's voice
        S_msrVoice
          measureVoice =
            elt->
              fetchMeasureUpLinkToVoice ();

        // fetch measure's part
        S_msrPart
          measurePart =
            measureVoice->
              getVoiceUpLinkToStaff ()->
                getStaffUpLinkToPart ();

        Rational
          measureWholeNotesDuration =
            measurePart->
              fetchPartMeasuresWholeNotesDurationsVectorAt (
                inputLineNumber,
                elt->getMeasureOrdinalNumberInVoice () - 1);

        // generate the skip name
        // take voice kind into account may be useful for debug
        switch (measureVoice->getVoiceKind ()) {
          case msrVoiceKind::kVoiceKindRegular:
          case msrVoiceKind::kVoiceKindDynamics:
            fLilypondCodeStream <<
              "s";
            break;

          case msrVoiceKind::kVoiceKindHarmonies:
          case msrVoiceKind::kVoiceKindFiguredBass:
            fLilypondCodeStream <<
              "s";
            break;
        } // switch

        // generate the duration of the skip
        fLilypondCodeStream <<
          wholeNotesAsLilypondString (
            inputLineNumber,
            measureWholeNotesDuration);

        if (gGlobalLpsr2lilypondOahGroup->getInputLineNumbers ()) {
          // generate information and line number as a comment
          fLilypondCodeStream <<
            " %{ line " <<
            elt->getInputLineNumber () <<
            " %}";
        }

        if (gGlobalLpsr2lilypondOahGroup->getNotesComments ()) {
          // generate information and line number as a comment
          fLilypondCodeStream <<
            " %{ kMeasureKindMusicallyEmpty %}";
        }

        // generate a bar check if this makes the measure full
        if (
          measureWholeNotesDuration
            ==
          elt->getFullMeasureWholeNotesDuration ()
        ) {
          fLilypondCodeStream << " | " << std::endl;
        }
      }
      break;
  } // switch
}

void lpsr2lilypondTranslator::visitEnd (S_msrMeasure& elt)
{
  int
    inputLineNumber =
      elt->getInputLineNumber ();

  std::string
    measureNumber =
      elt->getMeasureNumber ();

  msrMeasureKind
    measureKind =
      elt->getMeasureKind ();

  int
    measurePuristNumber =
      elt->getMeasurePuristNumber ();

#ifdef TRACING_IS_ENABLED
  msrMeasureEndRegularKind
    measureEndRegularKind =
      elt-> getMeasureEndRegularKind ();
#endif

#ifdef TRACING_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gGlobalLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream s;

      s <<
        "% --> End visiting msrMeasure '" <<
        measureNumber <<
        ", measureKind: '" << measureKind << '\'' <<
        ", measureEndRegularKind: '" << measureEndRegularKind << '\'' <<
        ", measurePuristNumber: '" << measurePuristNumber << '\'' <<
        ", fOnGoingMultipleFullBarRests: " << fOnGoingMultipleFullBarRests <<
        ", line " << inputLineNumber <<
        std::endl;

      if (traceLpsrVisitors) {
        gLogStream << s.str ();
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << s.str ();
      }
    }
  }
#endif

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasures ()) {
    gLogStream <<
      std::endl <<
      "% <!--=== measure '" << measureNumber << "' end, " <<
			", measureEndRegularKind: " << measureEndRegularKind <<
      ", measurePuristNumber: '" << measurePuristNumber << "'" <<
      ", fOnGoingMultipleFullBarRests = " << fOnGoingMultipleFullBarRests <<
      "', line " << inputLineNumber <<
      " ===-->" <<
      std::endl;
  }
#endif

  if (! fOnGoingMultipleFullBarRests) {
    // handle the measure
    switch (measureKind) {
      case msrMeasureKind::kMeasureKindUnknown: // should not occur
        fLilypondCodeStream <<
          "%{ measureKindUnknown, " <<
          measurePuristNumber + 1 <<
          " %}" <<
          std::endl;
        break;

      case msrMeasureKind::kMeasureKindRegular:
        {
        }
        break;

      case msrMeasureKind::kMeasureKindAnacrusis:
        break;

      case msrMeasureKind::kMeasureKindIncompleteStandalone:
        break;

      case msrMeasureKind::kMeasureKindIncompleteLastInRepeatCommonPart:
      case msrMeasureKind::kMeasureKindIncompleteLastInRepeatHookedEnding:
      case msrMeasureKind::kMeasureKindIncompleteLastInRepeatHooklessEnding:
      case msrMeasureKind::kMeasureKindIncompleteNextMeasureAfterCommonPart:
      case msrMeasureKind::kMeasureKindIncompleteNextMeasureAfterHookedEnding:
      case msrMeasureKind::kMeasureKindIncompleteNextMeasureAfterHooklessEnding:
        break;

      case msrMeasureKind::kMeasureKindOvercomplete:
        fLilypondCodeStream <<
          std::endl <<
          "\\undo \\omit Staff.TimeSignature |" <<
          std::endl <<
          "\\cadenzaOff" <<
          std::endl;

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
        if (gGlobalLpsr2lilypondOahGroup->getLilypondComments ()) {
          fLilypondCodeStream <<
            "%{ emptyMeasureKind" <<
            ", line " << inputLineNumber <<
            " %} % " <<
            measurePuristNumber + 1 <<
            std::endl;
        }
        break;
    } // switch

    if (gGlobalLpsr2lilypondOahGroup->getLilypondComments ()) {
 // JMI     --gIn denter; // incremented in visitStart (S_msrMeasure& elt)

      fLilypondCodeStream << std::left <<
        std::setw (commentFieldWidth) <<
        "" <<
        "% end of " <<
        elt->getMeasureKind () <<
        " measure " <<
        measureNumber <<
        ", line " << inputLineNumber <<
        std::endl << std::endl;
    }

    if (gGlobalLpsr2lilypondOahGroup->getSeparatorLineEveryNMeasures () > 0) {
      if (
        fCurrentVoiceMeasuresCounter
          %
        gGlobalLpsr2lilypondOahGroup->getSeparatorLineEveryNMeasures ()
          ==
        0)
        fLilypondCodeStream <<
          std::endl <<
          "% ============================= " <<
          std::endl << std::endl;
    }
  }

  // should we generate a line break?
  {
    std::set<std::string>::const_iterator
      it =
        gGlobalLpsr2lilypondOahGroup->
          getBreakLineAfterMeasureNumberSet ().find (measureNumber);

    if (it != gGlobalLpsr2lilypondOahGroup->getBreakLineAfterMeasureNumberSet ().end ()) {
      // yes, generate a line break command
  #ifdef TRACING_IS_ENABLED
        if (gGlobalTracingOahGroup->getTraceLineBreaks ()) {
          gLogStream <<
            std::endl <<
            "Adding a LilyPond line break after measure number '" <<
            measureNumber <<
            "', line " << inputLineNumber << " ===-->" <<
            std::endl;
        }
  #endif

        fLilypondCodeStream <<
          "\\break" <<
          std::endl;
    }

  /* JMI
    else {
  #ifdef TRACING_IS_ENABLED
     if (gGlobalTracingOahGroup->getTraceLineBreaks ()) { // JMI
        gLogStream <<
          std::endl <<
          "Measure number '" <<
          measureNumber <<
          "' not found in gGlobalLpsr2lilypondOahGroup->getBreakLineAfterMeasureNumberSet ()" <<
          ", line " << inputLineNumber <<
          std::endl;
      }
  #endif
    }
    */
  }

  // should we generate a page break?
  {
    std::set<std::string>::const_iterator
      it =
        gGlobalLpsr2lilypondOahGroup->
          getBreakPageAfterMeasureNumberSet ().find (measureNumber);

    if (it != gGlobalLpsr2lilypondOahGroup->getBreakPageAfterMeasureNumberSet ().end ()) {
      // yes, generate a page break command
  #ifdef TRACING_IS_ENABLED
        if (gGlobalTracingOahGroup->getTracePageBreaks ()) {
          gLogStream <<
            std::endl <<
            "Adding a LilyPond page break after measure number '" <<
            measureNumber <<
            "', line " << inputLineNumber << " ===-->" <<
            std::endl;
        }
  #endif

        fLilypondCodeStream <<
          "\\pageBreak" <<
          std::endl;
    }

  /* JMI
    else {
  #ifdef TRACING_IS_ENABLED
     if (gGlobalTracingOahGroup->getTracePageBreaks ()) { // JMI
        gLogStream <<
          std::endl <<
          "Measure number '" <<
          measureNumber <<
          "' not found in gGlobalLpsr2lilypondOahGroup->getBreakPageAfterMeasureNumberSet ()" <<
          ", line " << inputLineNumber <<
          std::endl;
      }
  #endif
    }
 */
  }

  if (gGlobalLpsr2lilypondOahGroup->getLilypondComments ()) {
    ++gIndenter; // incremented in visitStart (S_msrMeasure& elt)
  }
}

//________________________________________________________________________
void lpsr2lilypondTranslator::visitStart (S_msrStanza& elt)
{
#ifdef TRACING_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gGlobalLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream s;

      s <<
        "% --> Start visiting msrStanza \"" <<
        elt->getStanzaName () <<
        "\"" <<
        ", line " << elt->getInputLineNumber () <<
        std::endl;

      if (traceLpsrVisitors) {
        gLogStream << s.str ();
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << s.str ();
      }
    }
  }
#endif

  if (! gGlobalLpsr2lilypondOahGroup->getNoLilypondLyrics ()) {
    // don't generate code for the stanza inside the code for the voice
    fGenerateCodeForOngoingNonEmptyStanza =
      ! fOnGoingVoice
        &&
      elt->getStanzaTextPresent ();

    if (fGenerateCodeForOngoingNonEmptyStanza) {
      fLilypondCodeStream <<
        elt->getStanzaName () << " = " << "\\lyricmode {" <<
        std::endl;

      ++gIndenter; // decremented in visitEnd (S_msrStanza& elt)

      fLilypondCodeStream <<
        // set associatedVoice so that
        // both double hyphens and double underscores can be used
        // to draw hyphenated lines and extenders under melismata correctly
        "\\set associatedVoice = #\"" <<
        elt->
          getStanzaUpLinkToVoice ()->
            getVoiceName () <<
        "\"" <<
        std::endl <<
        "\\set ignoreMelismata = ##t" <<
        std::endl;
    }
  }

  fCurrentStanza = elt;
  fOnGoingStanza = true;
}

void lpsr2lilypondTranslator::visitEnd (S_msrStanza& elt)
{
#ifdef TRACING_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gGlobalLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream s;

      s <<
        "% --> End visiting msrStanza \"" <<
        elt->getStanzaName () <<
        "\"" <<
        ", line " << elt->getInputLineNumber () <<
        std::endl;

      if (traceLpsrVisitors) {
        gLogStream << s.str ();
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << s.str ();
      }
    }
  }
#endif

  if (! gGlobalLpsr2lilypondOahGroup->getNoLilypondLyrics ()) {
    if (fGenerateCodeForOngoingNonEmptyStanza) {
      --gIndenter; // incremented in visitStart (S_msrStanza& elt)

      fLilypondCodeStream <<
        std::endl <<
        '}' <<
        std::endl << std::endl;
    }

    fGenerateCodeForOngoingNonEmptyStanza = false;
  }

  fOnGoingStanza = false;
}

//________________________________________________________________________
void lpsr2lilypondTranslator::visitStart (S_msrSyllable& elt)
{
#ifdef TRACING_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gGlobalLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream s;

      s <<
        "% --> Start visiting msrSyllable '" <<
        elt->asString () <<
        "'" <<
        ", line " << elt->getInputLineNumber () <<
        std::endl;

        if (traceLpsrVisitors) {
          gLogStream << s.str ();
        }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << s.str ();
      }
    }
  }
#endif

  if (
    ! gGlobalLpsr2lilypondOahGroup->getNoLilypondLyrics ()
  ) {
    if (fGenerateCodeForOngoingNonEmptyStanza) {
      switch (elt->getSyllableKind ()) {
        case msrSyllableKind::kSyllableNone: // JMI
          break;

        case msrSyllableKind::kSyllableSingle:
          writeTextsListAsLilypondString (
            elt->getSyllableTextsList (),
            fLilypondCodeStream);

          switch (gGlobalLpsr2lilypondOahGroup->getLyricsDurationsKind ()) {
            case lpsrLyricsDurationsKind::kLyricsDurationsImplicit:
              // don't generate a duration for automatic lyrics durations
              break;
            case lpsrLyricsDurationsKind::kLyricsDurationsExplicit:
              fLilypondCodeStream <<
                elt->syllableWholeNotesAsMsrString ();
              break;
          } // switch

          fLilypondCodeStream <<
            ' ';

#ifdef TRACING_IS_ENABLED
          if (gGlobalTracingOahGroup->getTraceLyrics ()) {
            fLilypondCodeStream <<
              "%{ kSyllableSingle %} ";
          }
#endif
          break;

        case msrSyllableKind::kSyllableBegin:
          writeTextsListAsLilypondString (
            elt->getSyllableTextsList (),
            fLilypondCodeStream);

          switch (gGlobalLpsr2lilypondOahGroup->getLyricsDurationsKind ()) {
            case lpsrLyricsDurationsKind::kLyricsDurationsImplicit:
              // don't generate a duration for automatic lyrics durations
              break;
            case lpsrLyricsDurationsKind::kLyricsDurationsExplicit:
              fLilypondCodeStream <<
                elt->syllableWholeNotesAsMsrString ();
              break;
          } // switch

          fLilypondCodeStream <<
            " -- ";

#ifdef TRACING_IS_ENABLED
          if (gGlobalTracingOahGroup->getTraceLyrics ()) {
            fLilypondCodeStream <<
              "%{ kSyllableBegin %} ";
          }
#endif
          break;

        case msrSyllableKind::kSyllableMiddle:
          writeTextsListAsLilypondString (
            elt->getSyllableTextsList (),
            fLilypondCodeStream);

          switch (gGlobalLpsr2lilypondOahGroup->getLyricsDurationsKind ()) {
            case lpsrLyricsDurationsKind::kLyricsDurationsImplicit:
              // don't generate a duration for automatic lyrics durations
              break;
            case lpsrLyricsDurationsKind::kLyricsDurationsExplicit:
              fLilypondCodeStream <<
                elt->syllableWholeNotesAsMsrString ();
              break;
          } // switch

          fLilypondCodeStream <<
            " -- ";

#ifdef TRACING_IS_ENABLED
          if (gGlobalTracingOahGroup->getTraceLyrics ()) {
            fLilypondCodeStream <<
              "%{ kSyllableMiddle %} ";
          }
#endif
          break;

        case msrSyllableKind::kSyllableEnd:
          writeTextsListAsLilypondString (
            elt->getSyllableTextsList (),
            fLilypondCodeStream);

          switch (gGlobalLpsr2lilypondOahGroup->getLyricsDurationsKind ()) {
            case lpsrLyricsDurationsKind::kLyricsDurationsImplicit:
              // don't generate a duration for automatic lyrics durations
              break;
            case lpsrLyricsDurationsKind::kLyricsDurationsExplicit:
              fLilypondCodeStream <<
                elt->syllableWholeNotesAsMsrString ();
              break;
          } // switch

          fLilypondCodeStream <<
            ' ';

#ifdef TRACING_IS_ENABLED
          if (gGlobalTracingOahGroup->getTraceLyrics ()) {
            fLilypondCodeStream <<
              "%{ kSyllableEnd %} ";
          }
#endif
          break;

        case msrSyllableKind::kSyllableOnRestNote:
          // generate the syllable in lyrics for rests with syllables
          writeTextsListAsLilypondString (
            elt->getSyllableTextsList (),
            fLilypondCodeStream);
          fLilypondCodeStream <<
            elt->syllableWholeNotesAsMsrString () <<
            ' ';
#ifdef TRACING_IS_ENABLED
          if (gGlobalTracingOahGroup->getTraceLyrics ()) {
            fLilypondCodeStream <<
            " %{ kSyllableOnRestNote %} ";
          }
#endif
          break;

        case msrSyllableKind::kSyllableSkipRestNote:
          switch (gGlobalLpsr2lilypondOahGroup->getLyricsDurationsKind ()) {
            case lpsrLyricsDurationsKind::kLyricsDurationsImplicit:
              // LilyPond ignores the skip durations when \lyricsto is used
#ifdef TRACING_IS_ENABLED
              if (gGlobalTracingOahGroup->getTraceLyrics ()) {
                fLilypondCodeStream <<
                  " %{ NOTHING for kSyllableSkipRestNote " <<
                  elt->syllableWholeNotesAsMsrString () <<
                  " %} ";
              }
#endif
              break;
            case lpsrLyricsDurationsKind::kLyricsDurationsExplicit:
              fLilypondCodeStream <<
                "\\skip" <<
                elt->syllableWholeNotesAsMsrString () <<
                ' ';
#ifdef TRACING_IS_ENABLED
              if (gGlobalTracingOahGroup->getTraceLyrics ()) {
                fLilypondCodeStream <<
                " %{ lpsrLyricsDurationsKind::kLyricsDurationsExplicit %} ";
              }
#endif
              break;
          } // switch
          break;

        case msrSyllableKind::kSyllableSkipNonRestNote:
          switch (gGlobalLpsr2lilypondOahGroup->getLyricsDurationsKind ()) {
            case lpsrLyricsDurationsKind::kLyricsDurationsImplicit:
              // LilyPond ignores the skip durations when \lyricsto is used
              fLilypondCodeStream <<
                "\\skip" <<
                elt->syllableWholeNotesAsMsrString () <<
                ' ';
#ifdef TRACING_IS_ENABLED
              if (gGlobalTracingOahGroup->getTraceLyrics ()) {
                fLilypondCodeStream <<
                  " %{ syllableSkipNonRestNote lyricsDurationsImplicit %} ";
              }
#endif
              break;
            case lpsrLyricsDurationsKind::kLyricsDurationsExplicit:
              fLilypondCodeStream <<
                "\\skip" <<
                elt->syllableWholeNotesAsMsrString () <<
                ' ';
#ifdef TRACING_IS_ENABLED
              if (gGlobalTracingOahGroup->getTraceLyrics ()) {
                fLilypondCodeStream <<
                " %{ syllableSkipNonRestNote lyricsDurationsExplicit %} ";
              }
#endif
              break;
          } // switch
          break;

        case msrSyllableKind::kSyllableMeasureEnd:
      // JMI      "| " <<
          if (gGlobalLpsr2lilypondOahGroup->getInputLineNumbers ()) {
            // generate information and line number as a comment
            fLilypondCodeStream <<
              " %{ line " <<
              elt->getInputLineNumber () <<
              " %}";
          }

          if (gGlobalLpsr2lilypondOahGroup->getNotesComments ()) {
            // generate information and line number as a comment
            fLilypondCodeStream <<
              "%{ kSyllableMeasureEnd %}";
          }

      // JMI    fLilypondCodeStream << std::endl;
          break;

        case msrSyllableKind::kSyllableLineBreak:
          // generate information and line number as a comment
          fLilypondCodeStream <<
            "%{ syllableLineBreak, line " <<
            elt->getInputLineNumber () <<
            " %} " <<
            "%| % " << // JMI BLARK TO BE FIXED
            elt->getSyllableNextMeasurePuristNumber () <<
            std::endl;
          break;

        case msrSyllableKind::kSyllablePageBreak:
          // generate information and line number as a comment
          fLilypondCodeStream <<
            "%{ kSyllablePageBreak, line " <<
            elt->getInputLineNumber () <<
            " %}" <<
            std::endl;
          break;
      } // switch

      switch (elt->getSyllableExtendKind ()) {
        case msrSyllableExtendKind::kSyllableExtendNone:
#ifdef TRACING_IS_ENABLED
          if (gGlobalTracingOahGroup->getTraceLyrics ()) {
            fLilypondCodeStream <<
              "%{ kSyllableExtendNone %} ";
          }
#endif
          break;

        case msrSyllableExtendKind::kSyllableExtendEmpty:
#ifdef TRACING_IS_ENABLED
          if (gGlobalTracingOahGroup->getTraceLyrics ()) {
            fLilypondCodeStream <<
              "%{ kSyllableExtendEmpty %} ";
          }
#endif
          break;

        case msrSyllableExtendKind::kSyllableExtendSingle:
          switch (gGlobalLpsr2lilypondOahGroup->getLyricsDurationsKind ()) {
            case lpsrLyricsDurationsKind::kLyricsDurationsImplicit:
              // generate a lyric extender, i.e. a melisma, after this syllable
              fLilypondCodeStream <<
                "__ ";
              break;
            case lpsrLyricsDurationsKind::kLyricsDurationsExplicit:
              // generate a lyric extender, i.e. a melisma, after this syllable ??? JMI
              fLilypondCodeStream <<
                "__ ";
              break;
          } // switch
#ifdef TRACING_IS_ENABLED
          if (gGlobalTracingOahGroup->getTraceLyrics ()) {
            fLilypondCodeStream <<
              "%{ kSyllableExtendSingle %} ";
          }
#endif
          break;

        case msrSyllableExtendKind::kSyllableExtendStart:
          switch (gGlobalLpsr2lilypondOahGroup->getLyricsDurationsKind ()) {
            case lpsrLyricsDurationsKind::kLyricsDurationsImplicit:
              // generate a lyric extender, i.e. a melisma, after this syllable
              fLilypondCodeStream <<
                "__ ";
              break;
            case lpsrLyricsDurationsKind::kLyricsDurationsExplicit:
              // generate a lyric extender, i.e. a melisma, after this syllable ??? JMI
              fLilypondCodeStream <<
                "__ ";
              break;
          } // switch
#ifdef TRACING_IS_ENABLED
          if (gGlobalTracingOahGroup->getTraceLyrics ()) {
            fLilypondCodeStream <<
              "%{ kSyllableExtendStart %} ";
          }
#endif
          break;

        case msrSyllableExtendKind::kSyllableExtendContinue:
#ifdef TRACING_IS_ENABLED
          if (gGlobalTracingOahGroup->getTraceLyrics ()) {
            fLilypondCodeStream <<
              "%{ kSyllableExtendContinue %} ";
          }
#endif
          break;

        case msrSyllableExtendKind::kSyllableExtendStop:
#ifdef TRACING_IS_ENABLED
          if (gGlobalTracingOahGroup->getTraceLyrics ()) {
            fLilypondCodeStream <<
              "%{ kSyllableExtendStop %} ";
          }
#endif
          break;
      } // switch

    // generate the input line number as comment if relevant
    if (
      gGlobalLpsr2lilypondOahGroup->getInputLineNumbers ()
        ||
      gGlobalLpsr2lilypondOahGroup->getGenerateMeasurePositions ()
    ) {
      generateInputLineNumberAndOrMeasurePositionAsAComment (
        elt);
      }
    }
  }
}

void lpsr2lilypondTranslator::visitEnd (S_msrSyllable& elt)
{
#ifdef TRACING_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gGlobalLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream s;

      s <<
        "% --> End visiting msrSyllable '" <<
        elt->asString () <<
        "'" <<
        ", line " << elt->getInputLineNumber () <<
        std::endl;

      if (traceLpsrVisitors) {
        gLogStream << s.str ();
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << s.str ();
      }
    }
  }
#endif
}

//________________________________________________________________________
void lpsr2lilypondTranslator::visitStart (S_msrClef& elt)
{
#ifdef TRACING_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gGlobalLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream s;

      s <<
        "% --> Start visiting msrClef '" <<
        elt->asString () <<
        "'" <<
        ", line " << elt->getInputLineNumber () <<
        std::endl;

      if (traceLpsrVisitors) {
        gLogStream << s.str ();
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << s.str ();
      }
    }
  }
#endif

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

  #ifdef TRACING_IS_ENABLED
        if (gGlobalTracingOahGroup->getTraceClefs ()) {
          gLogStream <<
            "Commenting clef change from " <<
            fCurrentVoiceClef->asShortString () <<
            " to " <<
            elt->asShortString () <<
            std::endl;
        }
  #endif

        fLilypondCodeStream << "% ";
      }
    }

    if (clefKind != msrClefKind::kClef_NO_) {
      fLilypondCodeStream <<
        "\\clef ";

      switch (clefKind) {
        case msrClefKind::kClef_NO_:
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
          fLilypondCodeStream << "\"%{ jianpuClef??? %}\"";
          break;
      } // switch

      if (gGlobalLpsr2lilypondOahGroup->getInputLineNumbers ()) {
        fLilypondCodeStream <<
          " %{ " << elt->getInputLineNumber () << " %}";
      }

      fLilypondCodeStream << std::endl;
    }
  }

  // register current voice clef
  fCurrentVoiceClef = elt;
}

void lpsr2lilypondTranslator::visitEnd (S_msrClef& elt)
{
#ifdef TRACING_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gGlobalLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream s;

      s <<
        "% --> End visiting msrClef '" <<
        elt->asString () <<
        "'" <<
        ", line " << elt->getInputLineNumber () <<
        std::endl;

      if (traceLpsrVisitors) {
        gLogStream << s.str ();
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << s.str ();
      }
    }
  }
#endif
}

//________________________________________________________________________
void lpsr2lilypondTranslator::visitStart (S_msrKey& elt)
{
#ifdef TRACING_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gGlobalLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream s;

      s <<
        "% --> Start visiting msrKey '" <<
        elt->asString () <<
        "'" <<
        ", line " << elt->getInputLineNumber () <<
        std::endl;

      if (traceLpsrVisitors) {
        gLogStream << s.str ();
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << s.str ();
      }
    }
  }
#endif

  msrKeyKind
    keyKind =
      elt->getKeyKind ();

  Bool doGenerateKey (true);

  if (! fCurrentVoiceKey) {
    // this is the first clef in this voice
    if (
// add Bool msrKey::isCMajor () method JM or create an CMajorKey variable
//      keyKind == kCommonKey
//        &&
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
                  gGlobalLpsrOahGroup->
                    getLpsrQuarterTonesPitchesLanguageKind ());

                msrModeKind
                  modeKind         = elt->getModeKind (),
                  modeKindToBeUsed = msrModeKind::kMode_NO_;

                switch (modeKind) {
                  case msrModeKind::kMode_NO_:
                    modeKindToBeUsed = msrModeKind::kModeMajor; // JMI
                    break;
                  default:
                    modeKindToBeUsed = modeKind;
                } // switch

              fLilypondCodeStream <<
                ' ' <<
                msrModeKindAsLilypondString (
                  modeKindToBeUsed);

                if (gGlobalLpsr2lilypondOahGroup->getInputLineNumbers ()) {
                  fLilypondCodeStream <<
                    " %{ " << elt->getInputLineNumber () << " %}";
                }

                fLilypondCodeStream << std::endl;
              }
            break;

          case msrKeyKind::kKeyHumdrumScot:
            {
              const std::vector<S_msrHumdrumScotKeyItem>&
                humdrumScotKeyItemsVector =
                  elt->getHumdrumScotKeyItemsVector ();

              if (humdrumScotKeyItemsVector.size ()) {
                fLilypondCodeStream <<
                  std::endl <<
                  "\\set Staff.keyAlterations = #`(";

                std::vector<S_msrHumdrumScotKeyItem>::const_iterator
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
                    // Alternatively, for each item in the std::list, using the more concise format (step . alter) specifies that the same alteration should hold in all octaves.

                      fLilypondCodeStream <<
                        '(' <<
                        (int) item->getKeyItemDiatonicPitchKind () << // JMI ???
                        " . ," <<
                        alterationKindAsLilypondString (
                          item->getKeyItemAlterationKind ()) <<
                        ")";
                  }

                  if (++i == iEnd) break;

                  fLilypondCodeStream << ' ';
                } // for

                fLilypondCodeStream <<
                  ")";
              }

              else {
                lpsr2lilypondInternalError (
                  gGlobalServiceRunData->getInputSourceName (),
                  elt->getInputLineNumber (),
                  __FILE__, __LINE__,
                  "Humdrum/Scot key items std::vector is empty");
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
#ifdef TRACING_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gGlobalLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream s;

      s <<
        "% --> End visiting msrKey '" <<
        elt->asString () <<
        "'"  <<
        ", line " << elt->getInputLineNumber () <<
        std::endl;

      if (traceLpsrVisitors) {
        gLogStream << s.str ();
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << s.str ();
      }
    }
  }
#endif
}

//________________________________________________________________________
void lpsr2lilypondTranslator::visitStart (S_msrTimeSignature& elt)
{
#ifdef TRACING_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gGlobalLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream s;

      s <<
        "% --> Start visiting msrTimeSignature " <<
        elt->asString () <<
        ", line " << elt->getInputLineNumber () <<
        std::endl;

      if (traceLpsrVisitors) {
        gLogStream << s.str ();
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << s.str ();
      }
    }
  }
#endif

  msrTimeSignatureSymbolKind
    timeSignatureSymbolKind =
      elt->getTimeSignatureSymbolKind ();

  Bool doGenerateTimeSignature (true);

  if (! fCurrentVoiceTimeSignature) {
    // this is the first clef in this voice
    if (
      timeSignatureSymbolKind == msrTimeSignatureSymbolKind::kTimeSignatureSymbolCommon
        &&
      gGlobalLpsr2lilypondOahGroup->getNoInitialCommonTime ()
    ) {
      doGenerateTimeSignature = false;
    }
  }

  if (doGenerateTimeSignature) {
  /* JMI
    // is this the end of a senza misura fragment?
    if (
      fVoiceIsCurrentlySenzaMisura
        &&
      timeSignatureSymbolKind != msrTimeSignatureSymbolKind::kTimeSignatureSymbolSenzaMisura) {
      fLilypondCodeStream <<
        "\\undo\\omit Staff.TimeSignature" <<
        std::endl;

      fVoiceIsCurrentlySenzaMisura = false;
    }
  */

    // handle the time
    if (timeSignatureSymbolKind == msrTimeSignatureSymbolKind::kTimeSignatureSymbolSenzaMisura) {
      // senza misura time

      /* JMI
      fLilypondCodeStream <<
        "\\omit Staff.TimeSignature" <<
        std::endl;
  */

      fVoiceIsCurrentlySenzaMisura = true;
    }

    else {
      // con misura time

      const std::vector<S_msrTimeSignatureItem>&
        timeSignatureItemsVector =
          elt->getTimeSignatureItemsVector ();

      int timesItemsNumber =
        timeSignatureItemsVector.size ();

      if (timesItemsNumber) {
        // should there be a single number?
        switch (timeSignatureSymbolKind) {
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

        if (! elt->getTimeIsCompound ()) {
          // simple time
          // \time "3/4" for 3/4
          // or senza misura

          S_msrTimeSignatureItem
            timeSignatureItem =
              timeSignatureItemsVector [0]; // the only element;

          // fetch the time signature item beat numbers std::vector
          const std::vector<int>&
            beatsNumbersVector =
              timeSignatureItem->
                getTimeSignatureBeatsNumbersVector ();

          // should the time be numeric?
          if (
            timeSignatureSymbolKind == msrTimeSignatureSymbolKind::kTimeSignatureSymbolNone
              ||
            gGlobalLpsr2lilypondOahGroup->getNumericalTime ()) {
            fLilypondCodeStream <<
              "\\numericTimeSignature ";
          }

          fLilypondCodeStream <<
            "\\time " <<
            beatsNumbersVector [0] << // the only element
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

          // handle all the time signature items in the std::vector
          for (int i = 0; i < timesItemsNumber; ++i) {
            S_msrTimeSignatureItem
              timeSignatureItem =
                timeSignatureItemsVector [i];

            // fetch the time signature item beat numbers std::vector
            const std::vector<int>&
              beatsNumbersVector =
                timeSignatureItem->
                  getTimeSignatureBeatsNumbersVector ();

            int beatsNumbersNumber =
              beatsNumbersVector.size ();

            // first generate the opening parenthesis
            fLilypondCodeStream <<
              "(";

            // then generate all beats numbers in the std::vector
            for (int j = 0; j < beatsNumbersNumber; ++j) {
              fLilypondCodeStream <<
                beatsNumbersVector [j] <<
                ' ';
            } // for

            // then generate the beat type
            fLilypondCodeStream <<
              timeSignatureItem->getTimeSignatureBeatValue ();

            // and finally generate the closing parenthesis
            fLilypondCodeStream <<
              ")";

            if (i != timesItemsNumber - 1) {
              fLilypondCodeStream <<
                ' ';
            }
          } // for

          fLilypondCodeStream <<
            ")";
        }

        if (gGlobalLpsr2lilypondOahGroup->getInputLineNumbers ()) {
          fLilypondCodeStream <<
            " %{ " << elt->getInputLineNumber () << " %}";
        }

        fLilypondCodeStream << std::endl;
      }

      else {
        // there are no time signature items
        if (timeSignatureSymbolKind != msrTimeSignatureSymbolKind::kTimeSignatureSymbolSenzaMisura) {
          lpsr2lilypondInternalError (
            gGlobalServiceRunData->getInputSourceName (),
            elt->getInputLineNumber (),
            __FILE__, __LINE__,
            "time signature items std::vector is empty");
        }
      }
    }
  }

  fCurrentVoiceTimeSignature = elt;
}

void lpsr2lilypondTranslator::visitEnd (S_msrTimeSignature& elt)
{
#ifdef TRACING_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gGlobalLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream s;

      s <<
        "% --> End visiting msrTimeSignature " <<
        elt->asString () <<
        ", line " << elt->getInputLineNumber () <<
        std::endl;

      if (traceLpsrVisitors) {
        gLogStream << s.str ();
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << s.str ();
      }
    }
  }
#endif
}

//________________________________________________________________________
void lpsr2lilypondTranslator::visitStart (S_msrTransposition& elt)
{
#ifdef TRACING_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gGlobalLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream s;

      s <<
        "% --> Start visiting msrTransposition" <<
        ", line " << elt->getInputLineNumber () <<
        std::endl;

      if (traceLpsrVisitors) {
        gLogStream << s.str ();
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << s.str ();
      }
    }
  }
#endif

  int inputLineNumber =
    elt->getInputLineNumber ();

  int  transposeDiatonic     = elt->getTranspositionDiatonic ();
  int  transposeChromatic    = elt->getTranspositionChromatic ();
  int  transposeOctaveChange = elt->getTranspositionOctaveChange ();
  Bool transposeDouble       = elt->getTranspositionDouble ();

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
    transpositionPitchKind = msrQuarterTonesPitchKind::kQTP_NO_;

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
            inputLineNumber,
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
            inputLineNumber,
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
            inputLineNumber,
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
            inputLineNumber,
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
            inputLineNumber,
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
            inputLineNumber,
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
            inputLineNumber,
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
            inputLineNumber,
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
            inputLineNumber,
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
            inputLineNumber,
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
            inputLineNumber,
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
            inputLineNumber,
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
            inputLineNumber,
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
            inputLineNumber,
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
            inputLineNumber,
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
            inputLineNumber,
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
            inputLineNumber,
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
            inputLineNumber,
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
            inputLineNumber,
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
            inputLineNumber,
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
            inputLineNumber,
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
            inputLineNumber,
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
            inputLineNumber,
            transposeDiatonic,
            transposeChromatic);
      } // switch
      break;

    default:
      {
        std::stringstream s;

        s <<
          "fTransposition chromatic '" << transposeChromatic <<
          "' is not between -12 and 12, ignored";

        lpsr2lilypondError (
          gGlobalServiceRunData->getInputSourceName (),
          elt->getInputLineNumber (),
          __FILE__, __LINE__,
          s.str ());
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
        gGlobalLpsrOahGroup->
          getLpsrQuarterTonesPitchesLanguageKind ());

  std::string
    transpositionOctaveKindAsString =
      absoluteOctaveAsLilypondString (
        inputLineNumber,
        transpositionOctaveKind);

/* JMI
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceTranspositions ()) {
    fLilypondCodeStream << // JMI
      "Handlling transpose '" <<
      elt->transposeAsString () <<
      "' ignored because it is already present in voice \"" <<
      fCurrentVoice->getVoiceName () <<
      "\"" <<
      / * JMI
      getStaffName () <<
      "\" in part " <<
      fStaffUpLinkToPart->getPartCombinedName () <<
      * /
      std::endl <<
      ", transpositionPitch: " <<
      transpositionPitchAsString <<
      ", transpositionOctaveKind: " <<
      transpositionOctaveKindAsString <<
      '(' << transpositionOctaveKind << ")" <<
      std::endl;
    }
#endif
*/

  // now we can generate the transpostion command
  fLilypondCodeStream <<
    "\\transposition " <<
    transpositionPitchKindAsString <<
    transpositionOctaveKindAsString <<
    ' ' <<
    std::endl;
}

void lpsr2lilypondTranslator::visitEnd (S_msrTransposition& elt)
{
#ifdef TRACING_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gGlobalLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream s;

      s <<
        "% --> End visiting msrTransposition" <<
        ", line " << elt->getInputLineNumber () <<
        std::endl;

      if (traceLpsrVisitors) {
        gLogStream << s.str ();
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << s.str ();
      }
    }
  }
#endif
}

//________________________________________________________________________
void lpsr2lilypondTranslator::visitStart (S_msrTempo& elt)
{
#ifdef TRACING_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gGlobalLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream s;

      s <<
        "% --> Start visiting msrTempo" <<
        ", line " << elt->getInputLineNumber () <<
        std::endl;

      if (traceLpsrVisitors) {
        gLogStream << s.str ();
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << s.str ();
      }
    }
  }
#endif

  switch (elt->getTempoPlacementKind ()) {
    case msrPlacementKind::kPlacement_NO_:
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
    case msrTempoKBeatUnitsKind::kTempoBeatUnits_NO_:
      break;

    case msrTempoKBeatUnitsKind::kTempoBeatUnitsWordsOnly:
      generateCodeForTempoBeatUnitsWordsOnly (elt);
      break;

    case msrTempoKBeatUnitsKind::kTempoBeatUnitsPerMinute:
      generateCodeForTempoBeatUnitsPerMinute (elt);
      break;

    case msrTempoKBeatUnitsKind::kTempoBeatUnitsEquivalence:
      generateCodeForTempoBeatUnitsEquivalence (elt);
      break;

    case msrTempoKBeatUnitsKind::kTempoNotesRelationship:
      generateCodeForTempoNotesRelationship (elt);
      break;
  } // switch
}

void lpsr2lilypondTranslator::generateCodeForTempoBeatUnitsWordsOnly (
  const S_msrTempo& tempo)
{
  fLilypondCodeStream <<
    "\\tempo ";

  const std::list<S_msrWords>&
    tempoWordsList =
      tempo->getTempoWordsList ();

  size_t tempoWordsListSize = tempoWordsList.size ();

  if (tempoWordsListSize) {
    std::list<S_msrWords>::const_iterator
      iBegin = tempoWordsList.begin (),
      iEnd   = tempoWordsList.end (),
      i      = iBegin;

    for ( ; ; ) {
      S_msrWords words = (*i);

      fLilypondCodeStream <<
        "\"" << words->getWordsContents () << "\"";

      if (++i == iEnd) break;

      fLilypondCodeStream <<
        ' ';
    } // for

    fLilypondCodeStream <<
      std::endl;
  }
}

void lpsr2lilypondTranslator::generateCodeForTempoBeatUnitsPerMinute (
  const S_msrTempo& tempo)
{
  int inputLineNumber =
    tempo->getInputLineNumber ();

  msrTempoParenthesizedKind
    tempoParenthesizedKind =
      tempo->getTempoParenthesizedKind ();

  const std::list<S_msrWords>&
    tempoWordsList =
      tempo->getTempoWordsList ();

  size_t tempoWordsListSize = tempoWordsList.size ();

  msrDottedDuration tempoBeatUnit  = tempo->getTempoBeatUnit ();
  std::string       tempoPerMinute = tempo->getTempoPerMinute ();

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
        std::list<S_msrWords>::const_iterator
          iBegin = tempoWordsList.begin (),
          iEnd   = tempoWordsList.end (),
          i      = iBegin;

        for ( ; ; ) {
          S_msrWords words = (*i);

          fLilypondCodeStream <<
            "\"" << words->getWordsContents () << "\"";

          if (++i == iEnd) break;

          fLilypondCodeStream <<
            ' ';
        } // for
      }

      fLilypondCodeStream <<
        "(";
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
        dottedDurationAsLilypondString (
          inputLineNumber,
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
        ")" <<
        std::endl;

      --gIndenter;

      fLilypondCodeStream << std::endl;

      fLilypondCodeStream <<
        '}' << "% concat" <<
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
      std::list<S_msrWords>::const_iterator
        iBegin = tempoWordsList.begin (),
        iEnd   = tempoWordsList.end (),
        i      = iBegin;

      for ( ; ; ) {
        S_msrWords words = (*i);

        fLilypondCodeStream <<
          "\"" << words->getWordsContents () << "\"";

        if (++i == iEnd) break;

        fLilypondCodeStream <<
          ' ';
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
      dottedDurationAsLilypondString (
        inputLineNumber,
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
    '}' << "% concat" <<
    std::endl;

  --gIndenter;

  fLilypondCodeStream <<
    '}' <<
    std::endl;
  } // switch
}

void lpsr2lilypondTranslator::generateCodeForTempoBeatUnitsEquivalence (
  const S_msrTempo& tempo)
{
  int inputLineNumber =
    tempo->getInputLineNumber ();

  msrTempoParenthesizedKind
    tempoParenthesizedKind =
      tempo->getTempoParenthesizedKind ();

  const std::list<S_msrWords>&
    tempoWordsList =
      tempo->getTempoWordsList ();

  size_t tempoWordsListSize = tempoWordsList.size ();

  msrDottedDuration tempoBeatUnit  = tempo->getTempoBeatUnit ();
  std::string       tempoPerMinute = tempo->getTempoPerMinute ();

  fLilypondCodeStream <<
    "\\tempo ";

  if (tempoWordsListSize) {
    std::list<S_msrWords>::const_iterator
      iBegin = tempoWordsList.begin (),
      iEnd   = tempoWordsList.end (),
      i      = iBegin;

    for ( ; ; ) {
      S_msrWords words = (*i);

      fLilypondCodeStream <<
        "\"" << words->getWordsContents () << "\"";

      if (++i == iEnd) break;

      fLilypondCodeStream <<
        ' ';
    } // for
  }

  fLilypondCodeStream <<
    ' ' <<
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
    dottedDurationAsLilypondString (
      inputLineNumber,
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
    dottedDurationAsLilypondString (
      inputLineNumber,
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

void lpsr2lilypondTranslator::generateCodeForTempoNotesRelationship (
  const S_msrTempo& tempo)
{
  msrTempoParenthesizedKind
    tempoParenthesizedKind =
      tempo->getTempoParenthesizedKind ();

  const std::list<S_msrWords>&
    tempoWordsList =
      tempo->getTempoWordsList ();

  size_t tempoWordsListSize = tempoWordsList.size ();

  fLilypondCodeStream <<
    "\\tempoNotesRelationship #\"";

  if (tempoWordsListSize) {
    std::list<S_msrWords>::const_iterator
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
        ' ';
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
#ifdef TRACING_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gGlobalLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream s;

      s <<
        "% --> Start visiting msrTempoNotesRelationshipElements" <<
        ", line " << elt->getInputLineNumber () <<
        std::endl;

      if (traceLpsrVisitors) {
        gLogStream << s.str ();
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << s.str ();
      }
    }
  }
#endif

  fLilypondCodeStream <<
    "\\fixed b' {" <<
    std::endl;

  ++gIndenter; // decremented in visitEnd (S_msrTempoNotesRelationshipElements& elt)
}

void lpsr2lilypondTranslator::visitEnd (S_msrTempoNotesRelationshipElements& elt)
{
#ifdef TRACING_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gGlobalLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream s;

      s <<
        "% --> End visiting msrTempoNotesRelationshipElements" <<
        ", line " << elt->getInputLineNumber () <<
        std::endl;

      if (traceLpsrVisitors) {
        gLogStream << s.str ();
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << s.str ();
      }
    }
  }
#endif

  --gIndenter; // incremented in visitStart (S_msrTempoNotesRelationshipElements& elt)

  fLilypondCodeStream <<
    std::endl <<
    '}' <<
    std::endl;
}

void lpsr2lilypondTranslator::visitStart (S_msrTempoNote& elt)
{
#ifdef TRACING_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gGlobalLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream s;

      s <<
        "% --> Start visiting msrTempoNote" <<
        ", line " << elt->getInputLineNumber () <<
        std::endl;

      if (traceLpsrVisitors) {
        gLogStream << s.str ();
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << s.str ();
      }
    }
  }
#endif

  fLilypondCodeStream <<
    "b" <<
    wholeNotesAsLilypondString (
      elt->getInputLineNumber (),
      elt->getTempoNoteWholeNotes ()) <<
      ' ';
}

void lpsr2lilypondTranslator::visitStart (S_msrTempoTuplet& elt)
{
#ifdef TRACING_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gGlobalLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream s;

      s <<
        "% --> Start visiting msrTempoTuplet" <<
        ", line " << elt->getInputLineNumber () <<
        std::endl;

      if (traceLpsrVisitors) {
        gLogStream << s.str ();
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << s.str ();
      }
    }
  }
#endif

  const msrTupletFactor&
    tempoTupletFactor =
      elt->getTempoTupletFactor ();

  fLilypondCodeStream <<
    "\\tuplet " <<
    tempoTupletFactor.getTupletActualNotes () <<
    '/' <<
    tempoTupletFactor.getTupletNormalNotes () << " { ";
}

void lpsr2lilypondTranslator::visitEnd (S_msrTempoTuplet& elt)
{
#ifdef TRACING_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gGlobalLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream s;

      s <<
        "% --> End visiting msrTempoTuplet" <<
        ", line " << elt->getInputLineNumber () <<
        std::endl;

      if (traceLpsrVisitors) {
        gLogStream << s.str ();
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << s.str ();
      }
    }
  }
#endif

  fLilypondCodeStream <<
    '}';
}

void lpsr2lilypondTranslator::visitEnd (S_msrTempo& elt)
{
#ifdef TRACING_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gGlobalLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream s;

      s <<
        "% --> End visiting msrTempo" <<
        ", line " << elt->getInputLineNumber () <<
        std::endl;

      if (traceLpsrVisitors) {
        gLogStream << s.str ();
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << s.str ();
      }
    }
  }
#endif
}

//________________________________________________________________________
void lpsr2lilypondTranslator::visitStart (S_msrArticulation& elt)
{
#ifdef TRACING_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gGlobalLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream s;

      s <<
        "% --> Start visiting msrArticulation" <<
        ", line " << elt->getInputLineNumber () <<
        std::endl;

      if (traceLpsrVisitors) {
        gLogStream << s.str ();
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << s.str ();
      }
    }
  }
#endif

  // don't generate the articulation here,
  // the note or chord will do it in its visitEnd () method
}

void lpsr2lilypondTranslator::visitEnd (S_msrArticulation& elt)
{
#ifdef TRACING_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gGlobalLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream s;

      s <<
        "% --> End visiting msrArticulation" <<
        ", line " << elt->getInputLineNumber () <<
        std::endl;

      if (traceLpsrVisitors) {
        gLogStream << s.str ();
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << s.str ();
      }
    }
  }
#endif
}

//________________________________________________________________________
void lpsr2lilypondTranslator::visitStart (S_msrFermata& elt)
{
#ifdef TRACING_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gGlobalLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream s;

      s <<
        "% --> Start visiting msrFermata" <<
        ", line " << elt->getInputLineNumber () <<
        std::endl;

      if (traceLpsrVisitors) {
        gLogStream << s.str ();
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << s.str ();
      }
    }
  }
#endif

/*
Articulations can be attached to rests as well as notes but they cannot be attached to multiple full-bar rests. A special predefined command, \fermataMarkup, is available for at- taching a fermata to a multiple full-bar rest (and only a multiple full-bar rest). This creates a MultiMeasureRestText object.
*/

/* JMI
  switch (elt->getArticulationPlacement ()) {
    case msrPlacementKind::kPlacement_NO_:
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
  if (false && fOnGoingNotesStack.size () > 0) { // JMI
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
#ifdef TRACING_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gGlobalLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream s;

      s <<
        "% --> End visiting msrFermata" <<
        ", line " << elt->getInputLineNumber () <<
        std::endl;

      if (traceLpsrVisitors) {
        gLogStream << s.str ();
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << s.str ();
      }
    }
  }
#endif
}

//________________________________________________________________________
void lpsr2lilypondTranslator::visitStart (S_msrArpeggiato& elt)
{
#ifdef TRACING_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gGlobalLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream s;

      s <<
        "% --> Start visiting msrArpeggiato" <<
        ", line " << elt->getInputLineNumber () <<
        std::endl;

      if (traceLpsrVisitors) {
        gLogStream << s.str ();
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << s.str ();
      }
    }
  }
#endif

/* VIRER JMI
  */
}

void lpsr2lilypondTranslator::visitEnd (S_msrArpeggiato& elt)
{
#ifdef TRACING_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gGlobalLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream s;

      s <<
        "% --> End visiting msrArpeggiato" <<
        ", line " << elt->getInputLineNumber () <<
        std::endl;

      if (traceLpsrVisitors) {
        gLogStream << s.str ();
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << s.str ();
      }
    }
  }
#endif
}

//________________________________________________________________________
void lpsr2lilypondTranslator::visitStart (S_msrNonArpeggiato& elt)
{
#ifdef TRACING_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gGlobalLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream s;

      s <<
        "% --> Start visiting msrNonArpeggiato" <<
        ", line " << elt->getInputLineNumber () <<
        std::endl;

      if (traceLpsrVisitors) {
        gLogStream << s.str ();
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << s.str ();
      }
    }
  }
#endif

/* VIRER JMI
  */
}

void lpsr2lilypondTranslator::visitEnd (S_msrNonArpeggiato& elt)
{
#ifdef TRACING_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gGlobalLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream s;

      s <<
        "% --> End visiting msrNonArpeggiato" <<
        ", line " << elt->getInputLineNumber () <<
        std::endl;

      if (traceLpsrVisitors) {
        gLogStream << s.str ();
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << s.str ();
      }
    }
  }
#endif
}

//________________________________________________________________________
void lpsr2lilypondTranslator::visitStart (S_msrTechnical& elt)
{
#ifdef TRACING_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gGlobalLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream s;

      s <<
        "% --> Start visiting msrTechnical" <<
        ", line " << elt->getInputLineNumber () <<
        std::endl;

      if (traceLpsrVisitors) {
        gLogStream << s.str ();
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << s.str ();
      }
    }
  }
#endif

  // don't generate the technical here,
  // the note or chord will do it in its visitEnd () method
}

void lpsr2lilypondTranslator::visitEnd (S_msrTechnical& elt)
{
#ifdef TRACING_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gGlobalLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream s;

      s <<
        "% --> End visiting msrTechnical" <<
        ", line " << elt->getInputLineNumber () <<
        std::endl;

      if (traceLpsrVisitors) {
        gLogStream << s.str ();
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << s.str ();
      }
    }
  }
#endif
}

//________________________________________________________________________
void lpsr2lilypondTranslator::visitStart (S_msrTechnicalWithInteger& elt)
{
#ifdef TRACING_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gGlobalLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream s;

      s <<
        "% --> Start visiting msrTechnicalWithInteger" <<
        ", fOnGoingChord = " <<
        fOnGoingChord <<
        ", line " << elt->getInputLineNumber () <<
        std::endl;

      if (traceLpsrVisitors) {
        gLogStream << s.str ();
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << s.str ();
      }
    }
  }
#endif

  // don't generate the technicalWithInteger here,
  // the note or chord will do it in its visitEnd () method
}

void lpsr2lilypondTranslator::visitEnd (S_msrTechnicalWithInteger& elt)
{
#ifdef TRACING_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gGlobalLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream s;

      s <<
        "% --> End visiting msrTechnicalWithInteger" <<
        ", line " << elt->getInputLineNumber () <<
        std::endl;

      if (traceLpsrVisitors) {
        gLogStream << s.str ();
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << s.str ();
      }
    }
  }
#endif
}

//________________________________________________________________________
void lpsr2lilypondTranslator::visitStart (S_msrTechnicalWithFloat& elt)
{
#ifdef TRACING_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gGlobalLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream s;

      s <<
        "% --> Start visiting msrTechnicalWithFloat" <<
        ", fOnGoingChord = " <<
        fOnGoingChord <<
        ", line " << elt->getInputLineNumber () <<
        std::endl;

      if (traceLpsrVisitors) {
        gLogStream << s.str ();
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << s.str ();
      }
    }
  }
#endif

  // don't generate the technicalWithFloat here,
  // the note or chord will do it in its visitEnd () method
}

void lpsr2lilypondTranslator::visitEnd (S_msrTechnicalWithFloat& elt)
{
#ifdef TRACING_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gGlobalLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream s;

      s <<
        "% --> End visiting msrTechnicalWithFloat" <<
        ", line " << elt->getInputLineNumber () <<
        std::endl;

      if (traceLpsrVisitors) {
        gLogStream << s.str ();
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << s.str ();
      }
    }
  }
#endif
}

//________________________________________________________________________
void lpsr2lilypondTranslator::visitStart (S_msrTechnicalWithString& elt)
{
#ifdef TRACING_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gGlobalLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream s;

      s <<
        "% --> Start visiting msrTechnicalWithString" <<
        ", line " << elt->getInputLineNumber () <<
        std::endl;

      if (traceLpsrVisitors) {
        gLogStream << s.str ();
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << s.str ();
      }
    }
  }
#endif

  // don't generate the technicalWithString here,
  // the note or chord will do it in its visitEnd () method
}

void lpsr2lilypondTranslator::visitEnd (S_msrTechnicalWithString& elt)
{
#ifdef TRACING_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gGlobalLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream s;

      s <<
        "% --> End visiting msrTechnicalWithString" <<
        ", line " << elt->getInputLineNumber () <<
        std::endl;

      if (traceLpsrVisitors) {
        gLogStream << s.str ();
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << s.str ();
      }
    }
  }
#endif
}

//________________________________________________________________________
void lpsr2lilypondTranslator::visitStart (S_msrOrnament& elt)
{
#ifdef TRACING_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gGlobalLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream s;

      s <<
        "% --> Start visiting msrOrnament" <<
        ", line " << elt->getInputLineNumber () <<
        std::endl;

      if (traceLpsrVisitors) {
        gLogStream << s.str ();
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << s.str ();
      }
    }
  }
#endif

  // don't generate the ornament here,
  // the note or chord will do it in its visitEnd () method
}

void lpsr2lilypondTranslator::visitEnd (S_msrOrnament& elt)
{
#ifdef TRACING_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gGlobalLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream s;

      s <<
        "% --> End visiting msrOrnament" <<
        ", line " << elt->getInputLineNumber () <<
        std::endl;

      if (traceLpsrVisitors) {
        gLogStream << s.str ();
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << s.str ();
      }
    }
  }
#endif
}

//________________________________________________________________________
void lpsr2lilypondTranslator::visitStart (S_msrGlissando& elt)
{
#ifdef TRACING_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gGlobalLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream s;

      s <<
        "% --> Start visiting msrGlissando" <<
        ", line " << elt->getInputLineNumber () <<
        std::endl;

      if (traceLpsrVisitors) {
        gLogStream << s.str ();
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << s.str ();
      }
    }
  }
#endif

  // don't generate the glissando here,
  // the note or chord will do it in its visitEnd () method
}

void lpsr2lilypondTranslator::visitEnd (S_msrGlissando& elt)
{
#ifdef TRACING_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gGlobalLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream s;

      s <<
        "% --> End visiting msrGlissando" <<
        ", line " << elt->getInputLineNumber () <<
        std::endl;

      if (traceLpsrVisitors) {
        gLogStream << s.str ();
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << s.str ();
      }
    }
  }
#endif
}

//________________________________________________________________________
void lpsr2lilypondTranslator::visitStart (S_msrSlide& elt)
{
#ifdef TRACING_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gGlobalLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream s;

      s <<
        "% --> Start visiting msrSlide" <<
        ", line " << elt->getInputLineNumber () <<
        std::endl;

      if (traceLpsrVisitors) {
        gLogStream << s.str ();
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << s.str ();
      }
    }
  }
#endif

  // don't generate the slide here,
  // the note or chord will do it in its visitEnd () method
}

void lpsr2lilypondTranslator::visitEnd (S_msrSlide& elt)
{
#ifdef TRACING_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gGlobalLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream s;

      s <<
        "% --> End visiting msrSlide" <<
        ", line " << elt->getInputLineNumber () <<
        std::endl;

      if (traceLpsrVisitors) {
        gLogStream << s.str ();
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << s.str ();
      }
    }
  }
#endif
}

//________________________________________________________________________
void lpsr2lilypondTranslator::visitStart (S_msrSingleTremolo& elt)
{
#ifdef TRACING_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gGlobalLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream s;

      s <<
        "% --> Start visiting msrSingleTremolo" <<
        ", line " << elt->getInputLineNumber () <<
        std::endl;

      if (traceLpsrVisitors) {
        gLogStream << s.str ();
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << s.str ();
      }
    }
  }
#endif

  // don't generate the singleTremolo here,
  // the note or chord will do it in its visitEnd () method
}

void lpsr2lilypondTranslator::visitEnd (S_msrSingleTremolo& elt)
{
#ifdef TRACING_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gGlobalLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream s;

      s <<
        "% --> End visiting msrSingleTremolo" <<
        ", line " << elt->getInputLineNumber () <<
        std::endl;

      if (traceLpsrVisitors) {
        gLogStream << s.str ();
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << s.str ();
      }
    }
  }
#endif
}

//________________________________________________________________________
void lpsr2lilypondTranslator::visitStart (S_msrDoubleTremolo& elt)
{
#ifdef TRACING_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gGlobalLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream s;

      s <<
        "% --> Start visiting msrDoubleTremolo" <<
        ", line " << elt->getInputLineNumber () <<
        std::endl;

      if (traceLpsrVisitors) {
        gLogStream << s.str ();
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << s.str ();
      }
    }
  }
#endif

  // get double tremolo number of repeats
  int numberOfRepeats =
    elt->getDoubleTremoloNumberOfRepeats ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceTremolos ()) {
    fLilypondCodeStream <<
      "% visitStart (S_msrDoubleTremolo&)" <<
      std::endl;

    ++gIndenter;

    fLilypondCodeStream <<
      "% doubleTremoloSoundingWholeNotes = " <<
      elt->getMeasureElementSoundingWholeNotes () <<
      std::endl <<

      "% gdoubleTremoloElementsDuration = " <<
      elt->getDoubleTremoloElementsDuration () <<
      std::endl <<

      "% doubleTremoloMarksNumber = " <<
      elt->getDoubleTremoloMarksNumber () <<
      std::endl <<

      "% numberOfRepeats = " <<
      numberOfRepeats <<
      std::endl;

    --gIndenter;
  }
#endif

  fLilypondCodeStream <<
    "\\repeat tremolo " << numberOfRepeats << " {";

  ++gIndenter; // decremented in visitEnd (S_msrDoubleTremolo& elt)
}

void lpsr2lilypondTranslator::visitEnd (S_msrDoubleTremolo& elt)
{
#ifdef TRACING_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gGlobalLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream s;

      s <<
        "% --> End visiting msrDoubleTremolo" <<
        ", line " << elt->getInputLineNumber () <<
        std::endl;

      if (traceLpsrVisitors) {
        gLogStream << s.str ();
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << s.str ();
      }
    }
  }
#endif

  --gIndenter; // incremented in visitStart (S_msrDoubleTremolo& elt)

  fLilypondCodeStream <<
    '}' <<
    std::endl;
}

//________________________________________________________________________
void lpsr2lilypondTranslator::visitStart (S_msrDynamic& elt)
{
#ifdef TRACING_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gGlobalLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream s;

      s <<
        "% --> Start visiting msrDynamic" <<
        ", line " << elt->getInputLineNumber () <<
        std::endl;

      if (traceLpsrVisitors) {
        gLogStream << s.str ();
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << s.str ();
      }
    }
  }
#endif
}

void lpsr2lilypondTranslator::visitEnd (S_msrDynamic& elt)
{
#ifdef TRACING_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gGlobalLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream s;

      s <<
        "% --> End visiting msrDynamic" <<
        ", line " << elt->getInputLineNumber () <<
        std::endl;

      if (traceLpsrVisitors) {
        gLogStream << s.str ();
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << s.str ();
      }
    }
  }
#endif
}

//________________________________________________________________________
void lpsr2lilypondTranslator::visitStart (S_msrOtherDynamic& elt)
{
#ifdef TRACING_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gGlobalLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream s;

      s <<
        "% --> Start visiting msrOtherDynamic" <<
        ", line " << elt->getInputLineNumber () <<
        std::endl;

      if (traceLpsrVisitors) {
        gLogStream << s.str ();
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << s.str ();
      }
    }
  }
#endif
}

void lpsr2lilypondTranslator::visitEnd (S_msrOtherDynamic& elt)
{
#ifdef TRACING_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gGlobalLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream s;

      s <<
        "% --> End visiting msrOtherDynamic" <<
        ", line " << elt->getInputLineNumber () <<
        std::endl;

      if (traceLpsrVisitors) {
        gLogStream << s.str ();
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << s.str ();
      }
    }
  }
#endif
}

//________________________________________________________________________
void lpsr2lilypondTranslator::visitStart (S_msrWords& elt)
{
#ifdef TRACING_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gGlobalLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream s;

      s <<
        "% --> Start visiting msrWords" <<
        ", line " << elt->getInputLineNumber () <<
        std::endl;

      if (traceLpsrVisitors) {
        gLogStream << s.str ();
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << s.str ();
      }
    }
  }
#endif
}

void lpsr2lilypondTranslator::visitEnd (S_msrWords& elt)
{
#ifdef TRACING_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gGlobalLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream s;

      s <<
        "% --> End visiting msrWords" <<
        ", line " << elt->getInputLineNumber () <<
        std::endl;

      if (traceLpsrVisitors) {
        gLogStream << s.str ();
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << s.str ();
      }
    }
  }
#endif
}

//________________________________________________________________________
void lpsr2lilypondTranslator::visitStart (S_msrSlur& elt)
{
#ifdef TRACING_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gGlobalLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream s;

      s <<
        "% --> Start visiting msrSlur " <<
        elt->asShortString () <<
        ", line " << elt->getInputLineNumber () <<
        std::endl;

      if (traceLpsrVisitors) {
        gLogStream << s.str ();
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << s.str ();
      }
    }
  }
#endif
}

void lpsr2lilypondTranslator::visitEnd (S_msrSlur& elt)
{
#ifdef TRACING_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gGlobalLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream s;

      s <<
        "% --> End visiting msrSlur " <<
        elt->asShortString () <<
        ", line " << elt->getInputLineNumber () <<
        std::endl;

      if (traceLpsrVisitors) {
        gLogStream << s.str ();
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << s.str ();
      }
    }
  }
#endif
}

/*
void lpsr2lilypondTranslator::visitStart (S_msrChordSlurLink& elt)
{
#ifdef TRACING_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gGlobalLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream s;

      s <<
        "% --> Start visiting msrChordSlurLink " <<
        elt->asShortString () <<
        ", line " << elt->getInputLineNumber () <<
        std::endl;

      if (traceLpsrVisitors) {
        gLogStream << s.str ();
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << s.str ();
      }
    }
  }
#endif
}

void lpsr2lilypondTranslator::visitEnd (S_msrChordSlurLink& elt)
{
#ifdef TRACING_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gGlobalLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream s;

      s <<
        "% --> End visiting msrChordSlurLink " <<
        elt->asShortString () <<
        ", line " << elt->getInputLineNumber () <<
        std::endl;

      if (traceLpsrVisitors) {
        gLogStream << s.str ();
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << s.str ();
      }
    }
  }
#endif
}
*/

//________________________________________________________________________
void lpsr2lilypondTranslator::visitStart (S_msrLigature& elt)
{
#ifdef TRACING_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gGlobalLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream s;

      s <<
        "% --> Start visiting msrLigature" <<
        ", line " << elt->getInputLineNumber () <<
        std::endl;

      if (traceLpsrVisitors) {
        gLogStream << s.str ();
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << s.str ();
      }
    }
  }
#endif
}

void lpsr2lilypondTranslator::visitEnd (S_msrLigature& elt)
{
#ifdef TRACING_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gGlobalLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream s;

      s <<
        "% --> End visiting msrLigature" <<
        ", line " << elt->getInputLineNumber () <<
        std::endl;

      if (traceLpsrVisitors) {
        gLogStream << s.str ();
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << s.str ();
      }
    }
  }
#endif
}

//________________________________________________________________________
void lpsr2lilypondTranslator::visitStart (S_msrCrescDecresc& elt)
{
#ifdef TRACING_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gGlobalLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream s;

      s <<
        "% --> Start visiting msrCrescDecresc" <<
        ", line " << elt->getInputLineNumber () <<
        std::endl;

      if (traceLpsrVisitors) {
        gLogStream << s.str ();
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << s.str ();
      }
    }
  }
#endif

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
#ifdef TRACING_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gGlobalLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream s;

      s <<
        "% --> End visiting msrCrescDecresc" <<
        ", line " << elt->getInputLineNumber () <<
        std::endl;

      if (traceLpsrVisitors) {
        gLogStream << s.str ();
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << s.str ();
      }
    }
  }
#endif
}

//________________________________________________________________________
void lpsr2lilypondTranslator::visitStart (S_msrWedge& elt)
{
#ifdef TRACING_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gGlobalLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream s;

      s <<
        "% --> Start visiting msrWedge" <<
        ", line " << elt->getInputLineNumber () <<
        std::endl;

      if (traceLpsrVisitors) {
        gLogStream << s.str ();
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << s.str ();
      }
    }
  }
#endif
}

void lpsr2lilypondTranslator::visitEnd (S_msrWedge& elt)
{
#ifdef TRACING_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gGlobalLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream s;

      s <<
        "% --> End visiting msrWedge" <<
        ", line " << elt->getInputLineNumber () <<
        std::endl;

      if (traceLpsrVisitors) {
        gLogStream << s.str ();
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << s.str ();
      }
    }
  }
#endif
}

//________________________________________________________________________
void lpsr2lilypondTranslator::generateNoteBeams (
  const S_msrNote& note)
{
  const std::list<S_msrBeam>&
    noteBeams =
      note->getNoteBeams ();

  if (noteBeams.size ()) {
    for (
      std::list<S_msrBeam>::const_iterator i = noteBeams.begin ();
      i != noteBeams.end ();
      ++i
    ) {
      S_msrBeam beam = (*i);

 #ifdef TRACING_IS_ENABLED
      if (gGlobalTracingOahGroup->getTraceBeams ()) {
        gLogStream <<
          "Considering to generate LilyPond code for beam " <<
          beam->asShortString () <<
          " in note " <<
          note->asShortString () <<
          std::endl;
      }
#endif

      // LilyPond will take care of multiple beams automatically,
      // so we need only generate code for the first number (level)
      switch (beam->getBeamKind ()) {

        case msrBeamKind::kBeamBegin:
          if (beam->getBeamNumber () == 1) {
            if (! gGlobalLpsr2lilypondOahGroup->getNoBeams ()) {
 #ifdef TRACING_IS_ENABLED
              if (gGlobalTracingOahGroup->getTraceBeams ()) {
                gLogStream <<
                  "Generating LilyPond code for beam " <<
                  beam->asShortString () <<
                  " in note " <<
                  note->asShortString () <<
                  std::endl;
              }
#endif

              fLilypondCodeStream << "[ ";

              if (gGlobalLpsr2lilypondOahGroup->getInputLineNumbers ()) {
                // generate the input line number as a comment
                fLilypondCodeStream <<
                  " %{ line " << beam->getInputLineNumber () << " kBeamBegin %} ";
              }
            }
          }
          break;

        case msrBeamKind::kBeamContinue:
          break;

        case msrBeamKind::kBeamEnd:
          if (beam->getBeamNumber () == 1) {
            if (! gGlobalLpsr2lilypondOahGroup->getNoBeams ()) {
              fLilypondCodeStream << "] ";

              if (gGlobalLpsr2lilypondOahGroup->getInputLineNumbers ()) {
                // generate the input line number as a comment
                fLilypondCodeStream <<
                  " %{ line " << beam->getInputLineNumber () << " kBeamEnd %} ";
              }
            }
          }
          break;

        case msrBeamKind::kBeamForwardHook:
          break;

        case msrBeamKind::kBeamBackwardHook:
          break;

        case msrBeamKind::kBeam_NO_:
          break;
      } // switch
    } // for
  }
}

//________________________________________________________________________
void lpsr2lilypondTranslator::generateNoteSlurDirection (
  const S_msrNote& note)
{
  const std::list<S_msrSlur>&
    noteSlurs =
      note->getNoteSlurs ();

  if (noteSlurs.size ()) {
    std::list<S_msrSlur>::const_iterator i;

    for (
      i=noteSlurs.begin ();
      i!=noteSlurs.end ();
      ++i
    ) {
      S_msrSlur slur = (*i);

 #ifdef TRACING_IS_ENABLED
      if (gGlobalTracingOahGroup->getTraceSlurs ()) {
        gLogStream <<
          "Considering to generate LilyPond code for slur direction " <<
          slur->asShortString () <<
          " in note " <<
          note->asShortString () <<
          std::endl;
      }
#endif

      switch (slur->getSlurPlacementKind ()) {
        case msrPlacementKind::kPlacement_NO_:
          break;
        case msrPlacementKind::kPlacementAbove:
          fLilypondCodeStream << "\\slurUp ";
          break;
        case msrPlacementKind::kPlacementBelow:
          fLilypondCodeStream << "\\slurDown ";
          break;
      } // switch
    } // for
  }
}

//________________________________________________________________________
void lpsr2lilypondTranslator::generateNoteSlurs (
  const S_msrNote& note)
{
  const std::list<S_msrSlur>&
    noteSlurs =
      note->getNoteSlurs ();

  if (noteSlurs.size ()) {
    std::list<S_msrSlur>::const_iterator i;

    for (
      i=noteSlurs.begin ();
      i!=noteSlurs.end ();
      ++i
    ) {
      S_msrSlur slur = (*i);

 #ifdef TRACING_IS_ENABLED
      if (gGlobalTracingOahGroup->getTraceSlurs ()) {
        gLogStream <<
          "Considering to generate LilyPond code for slur " <<
          slur->asShortString () <<
          " in note " <<
          note->asShortString () <<
          std::endl;
      }
#endif

      /* JMI ???
        \slurDashed, \slurDotted, \slurHalfDashed,
        \slurHalfSolid, \slurDashPattern, \slurSolid
      */

      switch (slur->getSlurTypeKind ()) {
        case msrSlurTypeKind::kSlurType_NO_:
          break;

        case msrSlurTypeKind::kSlurTypeRegularStart:
#ifdef TRACING_IS_ENABLED
          if (gGlobalTracingOahGroup->getTraceSlurs ()) {
            gLogStream <<
              "Generating LilyPond code for slur regular start " <<
              slur->asShortString () <<
              " in note " <<
              note->asShortString () <<
              std::endl;
          }
#endif

          fLilypondCodeStream << "( ";

          if (gGlobalLpsr2lilypondOahGroup->getInputLineNumbers ()) {
            // generate the input line number as a comment
            fLilypondCodeStream <<
              " %{ line " << slur->getInputLineNumber () << " AA %} ";
          }
          break;

        case msrSlurTypeKind::kSlurTypeRegularContinue:
          // nothing to do
          break;

        case msrSlurTypeKind::kSlurTypeRegularStop:
#ifdef TRACING_IS_ENABLED
          if (gGlobalTracingOahGroup->getTraceSlurs ()) {
            gLogStream <<
              "Generating LilyPond code for slur regular stop " <<
              slur->asShortString () <<
              " in note " <<
              note->asShortString () <<
              std::endl;
          }
#endif

          fLilypondCodeStream << ") ";

          if (gGlobalLpsr2lilypondOahGroup->getInputLineNumbers ()) {
            // generate the input line number as a comment
            fLilypondCodeStream <<
              " %{ line " << slur->getInputLineNumber () << " BB %} ";
          }

          switch (slur->getSlurPlacementKind ()) {
            case msrPlacementKind::kPlacement_NO_:
              break;
            case msrPlacementKind::kPlacementAbove:
            case msrPlacementKind::kPlacementBelow:
              fLilypondCodeStream << "\\slurNeutral ";
              break;
          } // switch
          break;

        case msrSlurTypeKind::kSlurTypePhrasingStart:
#ifdef TRACING_IS_ENABLED
          if (gGlobalTracingOahGroup->getTraceSlurs ()) {
            gLogStream <<
              "Generating LilyPond code for slur phrasing start " <<
              slur->asShortString () <<
              " in note " <<
              note->asShortString () <<
              std::endl;
          }
#endif

          fLilypondCodeStream << "\\( ";

          if (gGlobalLpsr2lilypondOahGroup->getInputLineNumbers ()) {
            // generate the input line number as a comment
            fLilypondCodeStream <<
              " %{ line " << slur->getInputLineNumber () << " CC %} ";
          }
          break;

        case msrSlurTypeKind::kSlurTypePhrasingContinue:
          // nothing to do
          break;

        case msrSlurTypeKind::kSlurTypePhrasingStop:
#ifdef TRACING_IS_ENABLED
          if (gGlobalTracingOahGroup->getTraceSlurs ()) {
            gLogStream <<
              "Generating LilyPond code for slur phrasing stop " <<
              slur->asShortString () <<
              " in note " <<
              note->asShortString () <<
              std::endl;
          }
#endif

          fLilypondCodeStream << "\\) ";

          if (gGlobalLpsr2lilypondOahGroup->getInputLineNumbers ()) {
            // generate the input line number as a comment
            fLilypondCodeStream <<
              " %{ line " << slur->getInputLineNumber () << "DD %} ";
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
    2. slash and slur: \acciaccatura, LilyPond will slur it JMI
    3. slash but no slur: \slashedGrace
    4. no slash but slur: \appoggiatura, LilyPond will slur it JMI
  */

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceGraceNotes ()) {
    fLilypondCodeStream <<
      "% --> generating code for grace notes group " <<
      graceNotesGroup->asString () <<
      ", line " << graceNotesGroup->getInputLineNumber () <<
      std::endl;
  }
#endif

  Bool doGenerateASlurIfPresent (false);

  switch (graceNotesGroup->getGraceNotesGroupKind ()) {
    case msrGraceNotesGroupKind::kGraceNotesGroupBefore:
      if (graceNotesGroup->getGraceNotesGroupIsSlashed ()) {
        if (graceNotesGroup->getGraceNotesGroupIsTied ()) {
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
        if (graceNotesGroup->getGraceNotesGroupIsTied ()) {
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
    " { ";

  // force durations to be displayed explicitly
  // at the beginning of the grace notes
  fLastMetWholeNotes = Rational (0, 1);

  // generate the notes in the grace notes group
  const std::list<S_msrMeasureElement>&
    graceNotesGroupElementsList =
      graceNotesGroup->
        getGraceNotesGroupElementsList ();

  size_t
    graceNotesGroupElementsListSize =
      graceNotesGroupElementsList.size ();

  if (graceNotesGroupElementsListSize) {
    std::list<S_msrMeasureElement>::const_iterator
      iBegin = graceNotesGroupElementsList.begin (),
      iEnd   = graceNotesGroupElementsList.end (),
      i      = iBegin;

    size_t elementNumber = 0;

    for ( ; ; ) {
      S_msrElement element = (*i);

      // sanity check
      mfAssert (
        __FILE__, __LINE__,
        element != nullptr,
        "element is null");

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
          generateCodeForNote (graceNotesGroupNote);

          // generate the input line number as comment if relevant
          if ( // JMI
            gGlobalLpsr2lilypondOahGroup->getInputLineNumbers ()
              ||
            gGlobalLpsr2lilypondOahGroup->getGenerateMeasurePositions ()
          ) {
            generateInputLineNumberAndOrMeasurePositionAsAComment (
              graceNotesGroupNote);
          }

          // generate things after the graceNotesGroupNote
          generateCodeRightAfterNote (graceNotesGroupNote);

          // generate the graceNotesGroupNote beams if any,
          // unless the graceNotesGroupNote is chord member
          if (! graceNotesGroupNote->getNoteBelongsToAChord ()) {
            generateNoteBeams (graceNotesGroupNote);
          }

          if (graceNotesGroup->getGraceNotesGroupIsBeamed ()) {
            if (elementNumber == 1) {
              fLilypondCodeStream << "[ ";
            }
            else if (elementNumber == graceNotesGroupElementsListSize) {
              fLilypondCodeStream << "] ";
            }
          }

          // generate the graceNotesGroupNote slurs if any,
          // unless the graceNotesGroupNote is chord member
          // or LilyPond will take care of that

          // see gracenotes/SlurredNoteWithGraceNotes.xml for
          // extraneous '( ' JMI
//          if (false && // JMI
          if (
            ! graceNotesGroupNote->getNoteBelongsToAChord ()
              &&
            doGenerateASlurIfPresent
          ) {
            generateNoteSlurs (graceNotesGroupNote);
          }
        }

      else if (
        // chord?
        S_msrChord
          chord =
            dynamic_cast<msrChord*>(&(*element))
        ) {
          generateChordInGraceNotesGroup (chord);
        }

      else {
        std::stringstream s;

        fLilypondCodeStream <<
          "grace notes group elements list in '" <<
          graceNotesGroup->asString () <<
          "' is empty" <<
          ", line " << graceNotesGroup->getInputLineNumber ();

        lpsr2lilypondInternalError (
          gGlobalServiceRunData->getInputSourceName (),
          graceNotesGroup->getInputLineNumber (),
          __FILE__, __LINE__,
          s.str ());
      }

      if (++i == iEnd) break;
      fLilypondCodeStream <<
        ' ';
    } // for

    fLilypondCodeStream << "} ";
  }

  else {
    std::stringstream s;

    fLilypondCodeStream <<
      "grace notes group elements list in '" <<
      graceNotesGroup->asString () <<
      "' is empty" <<
      ", line " << graceNotesGroup->getInputLineNumber ();

    lpsr2lilypondInternalError (
      gGlobalServiceRunData->getInputSourceName (),
      graceNotesGroup->getInputLineNumber (),
      __FILE__, __LINE__,
      s.str ());
  }

  // force durations to be displayed explicitly
  // at the end of the grace notes
  fLastMetWholeNotes = Rational (0, 1);
}

//________________________________________________________________________
void lpsr2lilypondTranslator::visitStart (S_msrGraceNotesGroup& elt)
{
#ifdef TRACING_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gGlobalLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream s;

      s <<
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
        gLogStream << s.str ();
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << s.str ();
      }
    }
  }
#endif

  fOnGoingGraceNotesGroup = true;
}

void lpsr2lilypondTranslator::visitEnd (S_msrGraceNotesGroup& elt)
{
#ifdef TRACING_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gGlobalLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream s;

      s <<
        "% --> End visiting msrGraceNotesGroup " <<
        elt->asShortString () <<
        ", line " << elt->getInputLineNumber () <<
        std::endl;

      if (traceLpsrVisitors) {
        gLogStream << s.str ();
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << s.str ();
      }
    }
  }
#endif

  fOnGoingGraceNotesGroup = false;
}

//________________________________________________________________________
void lpsr2lilypondTranslator::visitStart (S_msrChordGraceNotesGroupLink& elt)
{
#ifdef TRACING_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gGlobalLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream s;

      s <<
        "% --> Start visiting msrChordGraceNotesGroupLink " <<
        elt->asShortString () <<
        ", line " << elt->getInputLineNumber () <<
        std::endl;

      if (traceLpsrVisitors) {
        gLogStream << s.str ();
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << s.str ();
      }
    }
  }
#endif

  fOnGoingChordGraceNotesGroupLink = true;
}

void lpsr2lilypondTranslator::visitEnd (S_msrChordGraceNotesGroupLink& elt)
{
#ifdef TRACING_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gGlobalLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream s;

      s <<
        "% --> End visiting msrChordGraceNotesGroupLink " <<
        elt->asShortString () <<
        ", line " << elt->getInputLineNumber () <<
        std::endl;

      if (traceLpsrVisitors) {
        gLogStream << s.str ();
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << s.str ();
      }
    }
  }
#endif

  fOnGoingChordGraceNotesGroupLink = false;
}

//________________________________________________________________________
void lpsr2lilypondTranslator::visitStart (S_msrAfterGraceNotesGroup& elt)
{
#ifdef TRACING_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gGlobalLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream s;

      s <<
        "% --> Start visiting msrAfterGraceNotesGroup " <<
        elt->asShortString () <<
        ", line " << elt->getInputLineNumber () <<
        std::endl;

      if (traceLpsrVisitors) {
        gLogStream << s.str ();
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << s.str ();
      }
    }
  }
#endif

 // JMI exists? if (elt->getGraceNotesGroupIsSlashed ()) {}
  fLilypondCodeStream <<
    "\\afterGrace { %{ visitStart (S_msrAfterGraceNotesGroup&) %} ";
}

void lpsr2lilypondTranslator::visitStart (S_msrAfterGraceNotesGroupContents& elt)
{
#ifdef TRACING_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gGlobalLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream s;

      s <<
        "% --> Start visiting msrAfterGraceNotesGroupContents " <<
        elt->asShortString () <<
        ", line " << elt->getInputLineNumber () <<
        std::endl;

      if (traceLpsrVisitors) {
        gLogStream << s.str ();
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << s.str ();
      }
    }
  }
#endif

  // write a first closing right bracket right now
  fLilypondCodeStream <<
    "} { ";

  // force durations to be displayed explicitly
  // at the beginning of the after grace notes contents
  fLastMetWholeNotes = Rational (0, 1);
}

void lpsr2lilypondTranslator::visitEnd (S_msrAfterGraceNotesGroupContents& elt)
{
#ifdef TRACING_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gGlobalLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream s;

      s <<
        "% --> End visiting msrAfterGraceNotesGroupContents " <<
        elt->asShortString () <<
        ", line " << elt->getInputLineNumber () <<
        std::endl;

      if (traceLpsrVisitors) {
        gLogStream << s.str ();
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << s.str ();
      }
    }
  }
#endif

  fLilypondCodeStream <<
    "} ";
}

void lpsr2lilypondTranslator::visitEnd (S_msrAfterGraceNotesGroup& elt)
{
#ifdef TRACING_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gGlobalLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream s;

      s <<
        "% --> End visiting msrAfterGraceNotesGroup " <<
        elt->asShortString () <<
        ", line " << elt->getInputLineNumber () <<
        std::endl;

      if (traceLpsrVisitors) {
        gLogStream << s.str ();
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << s.str ();
      }
    }
  }
#endif
}

//________________________________________________________________________
void lpsr2lilypondTranslator::generateBeforeNoteSpannersIfAny (
  const S_msrNote& note)
{
  const std::list<S_msrSpanner>&
    noteSpanners =
      note->getNoteSpanners ();

  if (noteSpanners.size ()) {
    std::list<S_msrSpanner>::const_iterator i;
    for (
      i=noteSpanners.begin ();
      i!=noteSpanners.end ();
      ++i
    ) {
      S_msrSpanner
        spanner = (*i);

      Bool doGenerateSpannerCode (true);

      switch (spanner->getSpannerKind ()) {
        case msrSpannerKind::kSpannerDashes:
          break;
        case msrSpannerKind::kSpannerWavyLine:
          if (spanner->getSpannerUpLinkToNote ()->getNoteTrillOrnament ()) {
            // don't generate anything, the trill will display the wavy line
            doGenerateSpannerCode = false;
          }
          break;
      } // switch

      if (doGenerateSpannerCode) {
        switch (spanner->getSpannerKind ()) {
          case msrSpannerKind::kSpannerDashes:
          case msrSpannerKind::kSpannerWavyLine:
            break;
        } // switch

        generateCodeForSpannerBeforeNote (spanner);
      }
    } // for
  }
}

//________________________________________________________________________
void lpsr2lilypondTranslator::generateAfterNoteSpannersIfAny (
  const S_msrNote& note)
{
  const std::list<S_msrSpanner>&
    noteSpanners =
      note->getNoteSpanners ();

  if (noteSpanners.size ()) {
    std::list<S_msrSpanner>::const_iterator i;
    for (
      i=noteSpanners.begin ();
      i!=noteSpanners.end ();
      ++i
    ) {
      S_msrSpanner
        spanner = (*i);

      Bool doGenerateSpannerCode (true);

      switch (spanner->getSpannerKind ()) {
        case msrSpannerKind::kSpannerDashes:
          break;
        case msrSpannerKind::kSpannerWavyLine:
          if (spanner->getSpannerUpLinkToNote ()->getNoteTrillOrnament ()) {
            // don't generate anything, the trill will display the wavy line
            doGenerateSpannerCode = false;
          }
          break;
      } // switch

      if (doGenerateSpannerCode) {
        switch (spanner->getSpannerKind ()) {
          case msrSpannerKind::kSpannerDashes:
          case msrSpannerKind::kSpannerWavyLine:
            break;
        } // switch

        generateCodeForSpannerAfterNote (spanner);
      }
    } // for
  }
}

//________________________________________________________________________
void lpsr2lilypondTranslator::visitStart (S_msrNote& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gGlobalLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || traceLpsrVisitors || generateMsrVisitingInformation) { // JMI
      std::stringstream s;

      s <<
        "% --> Start visiting note " <<
        elt->asShortString () <<
        ", fOnGoingChord: " <<
        fOnGoingChord <<
        ", fOnGoingGraceNotesGroup: " <<
        fOnGoingGraceNotesGroup <<
        ", fOnGoingChordGraceNotesGroupLink: " <<
        fOnGoingChordGraceNotesGroupLink <<
        ", fOnGoingMultipleFullBarRests: " <<
        fOnGoingMultipleFullBarRests <<
        ", line " << inputLineNumber <<
        std::endl;

      if (traceLpsrVisitors) {
        gLogStream << s.str ();
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << s.str ();
      }
    }
  }
#endif

  // register the note as on-going note
  fOnGoingNotesStack.push (elt); // will be popped in visitEnd (S_msrNote&)

  // is this note to be ignored?
  Bool noteIsToBeIgnored (false);

  if (fOnGoingGraceNotesGroup) { // JMI
    if (fOnGoingChord) {
      noteIsToBeIgnored = true;
    }
    else {
      switch (elt->getNoteKind ()) {
        case msrNoteKind::kNote_NO_:
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
        case msrNoteKind::kNoteSkipInGraceNotesGroup:
          {
          // don't generate code for the grace notes here, that's done thru
          // the note's noteGraceNotesGroupBefore and noteGraceNotesGroupAfter fields
            noteIsToBeIgnored = true;
/* JMI
            S_msrNote
              noteTheGraceNotesGroupIsAttachedTo =
                elt->getNoteDirectUpLinkToGraceNotesGroup ()->
                  getGraceNotesGroupUpLinkToNote ();

            // don't generate note if the notes it's grace notes group is attached to
            // has a ??? JMI
            if (
              noteTheGraceNotesGroupIsAttachedTo->getNoteGraceNotesGroupBefore ()
                ||
              noteTheGraceNotesGroupIsAttachedTo->getNoteGraceNotesGroupAfter ()
            ) {
              noteIsToBeIgnored = true;
            }
*/
          }
          break;

        // in chords in grace notes groups
        case msrNoteKind::kNoteInChordInGraceNotesGroup:
#ifdef TRACING_IS_ENABLED
          if (
            gGlobalLpsrOahGroup->getTraceLpsrVisitors ()
              ||
            gGlobalTracingOahGroup->getTraceGraceNotes ()
          ) {
            S_msrGraceNotesGroup
              upLinkToGraceNotesGroup =
                elt->getNoteDirectUpLinkToGraceNotesGroup ();

            gLogStream <<
              "% ==> upLinkToGraceNotesGroup: ";
            if (upLinkToGraceNotesGroup) {
              gLogStream <<
                upLinkToGraceNotesGroup->asString ();
            }
            else {
              gLogStream << "[NONE]";
            }
            gLogStream << std::endl;
          }
#endif
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

  if (fOnGoingMultipleFullBarRests) {
    switch (elt->getNoteKind ()) {
      case msrNoteKind::kNoteRestInMeasure:
        // don't handle multiple full-bar rests, that's done in visitEnd (S_msrMultipleFullBarRests&)
          /*
          if (elt->getNoteOccupiesAFullMeasure ()) {
            Bool inhibitMultipleFullBarRestsBrowsing =
              fVisitedLpsrScore->
                getMsrScore ()->
                  getInhibitMultipleFullBarRestsBrowsing ();

            if (inhibitMultipleFullBarRestsBrowsing) {
              if (
                gGlobalLpsrOahGroup->getTraceLpsrVisitors ()
                  ||
                gGlobalTracingOahGroup->getTraceMultipleFullBarRests ()) {
                gLogStream <<
                  "% ==> visiting multiple full-bar rests is ignored" <<
                  std::endl;
              }

  #ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceNotesDetails ()) {
      gLogStream <<
        "% ==> returning from visitStart (S_msrNote&)" <<
      std::endl;
    }
  #endif

            noteIsToBeIgnored = true;
            }
          }
          */

#ifdef TRACING_IS_ENABLED
          if (
            gGlobalLpsrOahGroup->getTraceLpsrVisitors ()
              ||
            gGlobalTracingOahGroup->getTraceMultipleFullBarRests ()
          ) {
            gLogStream <<
              "% ==> start visiting rest notes is ignored upon note " <<
              elt->asString () <<
              std::endl;
          }
#endif

          noteIsToBeIgnored = true;
        break;

      case msrNoteKind::kNoteSkipInMeasure:
        if (elt->getNoteDirectUpLinkToGraceNotesGroup ()) {
#ifdef TRACING_IS_ENABLED
          if (
            gGlobalLpsrOahGroup->getTraceLpsrVisitors ()
              ||
            gGlobalTracingOahGroup->getTraceNotes ()
          ) {
            gLogStream <<
              "% ==> start visiting skip notes is ignored upon note " <<
              elt->asString () <<
              std::endl;
          }
#endif

          noteIsToBeIgnored = true;
        }
        break;

      case msrNoteKind::kNoteRegularInGraceNotesGroup:
      case msrNoteKind::kNoteSkipInGraceNotesGroup:
#ifdef TRACING_IS_ENABLED
          if (
            gGlobalLpsrOahGroup->getTraceLpsrVisitors ()
              ||
            gGlobalTracingOahGroup->getTraceGraceNotes ()
          ) {
            gLogStream <<
              "% ==> start visiting grace notes is ignored upon note " <<
              elt->asString () <<
              std::endl;
          }
#endif

          noteIsToBeIgnored = true;
        break;

      case msrNoteKind::kNoteInChordInGraceNotesGroup:
#ifdef TRACING_IS_ENABLED
          if (
            gGlobalLpsrOahGroup->getTraceLpsrVisitors ()
              ||
            gGlobalTracingOahGroup->getTraceGraceNotes ()
          ) {
            gLogStream <<
              "% ==> start visiting chord grace notes is ignored upon note " <<
              elt->asString () <<
              std::endl;
          }
#endif

          noteIsToBeIgnored = true;
        break;

      default:
        ;
    } // switch
  }

  // is this note to be ignored?
  if (noteIsToBeIgnored) {
#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceNotes ()) {
      std::stringstream s;

      s <<
        "% ==> Start visiting notes is ignored inside grace notes groups " << // JMI BLARK
        elt->asShortString ();

      lpsr2lilypondInternalWarning (
        gGlobalServiceRunData->getInputSourceName (),
        inputLineNumber,
        s.str ());
    }
#endif

    return;
  }

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceNotes ()) {
    std::stringstream s;

    s <<
      "% --> Actually handling note " <<
      elt->asShortString () <<
      ", fOnGoingChord: " <<
      fOnGoingChord <<
      ", fOnGoingGraceNotesGroup: " <<
      fOnGoingGraceNotesGroup <<
      ", fOnGoingChordGraceNotesGroupLink: " <<
      fOnGoingChordGraceNotesGroupLink <<
      ", fOnGoingMultipleFullBarRests: " <<
      fOnGoingMultipleFullBarRests <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  // get the note's grace notes group after
  S_msrGraceNotesGroup
    noteGraceNotesGroupAfter =
      elt->getNoteGraceNotesGroupAfter ();

  // generate the note's grace notes group after opener if any
  if (noteGraceNotesGroupAfter) {
    fLilypondCodeStream <<
      "\\afterGrace { ";
  }

  // generate the note's grace notes group before if any,
  // unless the note belongs to a chord
  Bool doGenerateNoteGraceNotesGroupBefore (true);

  if (fOnGoingChord && fOnGoingGraceNotesGroup) {
    doGenerateNoteGraceNotesGroupBefore = false;
  }

  if (doGenerateNoteGraceNotesGroupBefore) {
    S_msrGraceNotesGroup
      noteGraceNotesGroupBefore =
        elt->getNoteGraceNotesGroupBefore ();

    if (noteGraceNotesGroupBefore) {
      generateGraceNotesGroup (
        noteGraceNotesGroupBefore);
    }
  }

  // generate the note scordaturas if any
  const std::list<S_msrScordatura>&
    noteScordaturas =
      elt->getNoteScordaturas ();

  if (noteScordaturas.size ()) {
    generateNoteScordaturas (elt);
  }

  // should the note actually be printed? // JMI
  msrPrintObjectKind
    notePrintObjectKind =
      elt->getNotePrintObjectKind ();

  if (notePrintObjectKind != fCurrentNotePrinObjectKind) {
    switch (notePrintObjectKind) {
      case msrPrintObjectKind::kPrintObjectNone:
        // JMI
        break;
      case msrPrintObjectKind::kPrintObjectYes:
        break;
      case msrPrintObjectKind::kPrintObjectNo:
        {
          // get the print notes head RGB color atom
          S_oahColorRGBAtom
            nonPrintNotesHeadColorRGBAtom =
              gGlobalLpsr2lilypondOahGroup->
                getNonPrintNotesHeadColorRGBAtom ();

          // has the note color been set? // JMI v0.9.65
          if (nonPrintNotesHeadColorRGBAtom->getSetByAnOption ()) {
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
//             if (gGlobalLpsr2lilypondOahGroup->getLilypondComments ()) {
//               fLilypondCodeStream <<
//                 "%{ " <<
//                 gGlobalServiceRunData->getInputSourceName () <<
//                 ":" <<
//                 inputLineNumber <<
//                 ": " <<
//                 "ignoring 'msrPrintObjectKind::kPrintObjectNo'" <<
//                 " %}" <<
//                 std::endl;
//             }

            fLilypondCodeStream <<
              "\\once\\hide NoteHead \\once\\hide Stem ";
          }
        }
        break;
    } // switch

    fCurrentNotePrinObjectKind = notePrintObjectKind;
  }

  // generate the note slashes if any
  const std::list<S_msrSlash>&
    noteSlashes =
      elt->getNoteSlashes ();

  if (noteSlashes.size ()) {
    generateNoteSlashes (elt);
  }

  // generate the note wedges circled tips if any
  const std::list<S_msrWedge>&
    noteWedges =
      elt->getNoteWedges ();

  if (noteWedges.size ()) {
    generateNoteWedges (elt);
  }

  // generate the note slurs line types if any,
  // unless the note is chord member
  if (! elt->getNoteBelongsToAChord ()) {
    const std::list<S_msrSlur>&
      noteSlurs =
        elt->getNoteSlurs ();

    if (noteSlurs.size ()) {
      generateNoteSlurLineTypes (elt);
    }
  }

  // generate the note glissandos styles if any
  const std::list<S_msrGlissando>&
    noteGlissandos =
      elt->getNoteGlissandos ();

  if (noteGlissandos.size ()) {
    generateNoteGlissandoStyles (elt);
  }

  // generate the note glissandos with text if any
  // just got noteGlissandos above
  if (noteGlissandos.size ()) {
    generateNoteGlissandosWithText (elt);
  }

  // generate the note slides line styles if any, implemented as glissandos
  const std::list<S_msrSlide>&
    noteSlides =
      elt->getNoteSlides ();

  if (noteSlides.size ()) {
    generateNoteSlideLineStyles (elt);
  }

  // generate the note slides with text if any
  // just got noteSlides above
  if (noteSlides.size ()) {
    generateNoteSlidesWithText (elt);
  }

  // should the note be parenthesized?
  msrNoteHeadParenthesesKind
    noteHeadParenthesesKind =
      elt->getNoteHeadParenthesesKind ();

  switch (noteHeadParenthesesKind) {
    case msrNoteHeadParenthesesKind::kNoteHeadParenthesesYes:
      fLilypondCodeStream << "\\parenthesize ";
      break;
    case msrNoteHeadParenthesesKind::kNoteHeadParenthesesNo:
      break;
  } // switch

  // generate the note technicals with std::string if any
  const std::list<S_msrTechnicalWithString>&
    noteTechnicalWithStrings =
      elt->getNoteTechnicalWithStrings ();

  if (noteTechnicalWithStrings.size ()) {
    generateNoteTechnicalsWithStrings (elt);
  }

  // is the note a cue note?
  switch (elt->getNoteIsACueNoteKind ()) {
    case msrNoteIsACueNoteKind::kNoteIsACueNoteYes:
      fLilypondCodeStream <<
        "\\once \\override NoteHead.font-size = -3 ";
      break;
    case msrNoteIsACueNoteKind::kNoteIsACueNoteNo:
      break;
  } // switch

  // has the note an octave shift up or down?
  if (! fOnGoingChord) {
    // the octave shift for the chords has already been generated
    S_msrOctaveShift
      noteOctaveShift =
        elt->
          getNoteOctaveShift ();

    if (noteOctaveShift) {
      generateCodeForOctaveShiftBeforeNote (
        noteOctaveShift);
    }
  }

  // generate things before the note
  generateCodeRightBeforeNote (elt);

  ////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////
  // generate the note itself as a LilyPond std::string
  ////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////

  generateCodeForNote (elt);

  // generate things after the note
  generateAfterNoteSpannersIfAny (elt);
  generateCodeRightAfterNote (elt);

  // has the note an octave shift up or down?
  if (! fOnGoingChord) {
    // the octave shift for the chords has already been generated
    S_msrOctaveShift
      noteOctaveShift =
        elt->
          getNoteOctaveShift ();

    if (noteOctaveShift) {
      generateCodeForOctaveShiftAfterNote (
        noteOctaveShift);
    }
  }

/* JMI
  // get the note's grace notes group after ??? JMI
  S_msrGraceNotesGroup
    noteGraceNotesGroupAfter =
      elt->getNoteGraceNotesGroupAfter ();

  // generate the note's grace notes group after opener if any
  if (noteGraceNotesGroupAfter) {
    fLilypondCodeStream <<
      "\\afterGrace { ";
  }
*/

  // generate the input line number as comment if relevant
  if (
    gGlobalLpsr2lilypondOahGroup->getInputLineNumbers ()
      ||
    gGlobalLpsr2lilypondOahGroup->getGenerateMeasurePositions ()
  ) {
    generateInputLineNumberAndOrMeasurePositionAsAComment (
      elt);
  }
}

void lpsr2lilypondTranslator::generateNoteScordaturas (
  const S_msrNote& note)
{
  int inputLineNumber =
    note->getInputLineNumber ();

  const std::list<S_msrScordatura>&
    noteScordaturas =
      note->getNoteScordaturas ();

  if (noteScordaturas.size ()) {
    fLilypondCodeStream <<
      " <<" <<
      std::endl;

    ++gIndenter;

    std::list<S_msrScordatura>::const_iterator
      iBegin = noteScordaturas.begin (),
      iEnd   = noteScordaturas.end (),
      i      = iBegin;
    for ( ; ; ) {
      S_msrScordatura
        scordatura = (*i);

      const std::list<S_msrStringTuning>&
        scordaturaStringTuningsList =
          scordatura->
            getScordaturaStringTuningsList ();

      ++gIndenter;

      fLilypondCodeStream <<
        "\\new Staff \\with { alignAboveContext = \"" <<
        note->
          fetchUpLinkToNoteToStaff ()->
            getStaffName () <<
        "\" } {" <<
        std::endl;

      ++gIndenter;

      fLilypondCodeStream <<
        "\\hide Staff.Stem" <<
        std::endl <<
        "\\hide Staff.TimeSignature" <<
        std::endl <<
        "\\small" <<
        std::endl <<
        "\\once \\override Score.RehearsalMark.self-alignment-X = #LEFT" <<
        std::endl <<
        "\\mark\\markup {\\small\\bold \"Scordatura\"}" <<
        std::endl <<
        "<";

      if (scordaturaStringTuningsList.size ()) {
        std::list<S_msrStringTuning>::const_iterator
          iBegin = scordaturaStringTuningsList.begin (),
          iEnd   = scordaturaStringTuningsList.end (),
          i      = iBegin;
        for ( ; ; ) {
          S_msrStringTuning
            stringTuning = (*i);

          fLilypondCodeStream <<
            stringTuningAsLilypondString (
              inputLineNumber,
              stringTuning);

          if (++i == iEnd) break;

          fLilypondCodeStream << ' ';
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

void lpsr2lilypondTranslator::generateNoteSlashes (
  const S_msrNote& note)
{
  const std::list<S_msrSlash>&
    noteSlashes =
      note->getNoteSlashes ();

  if (noteSlashes.size ()) {
    std::list<S_msrSlash>::const_iterator i;
    for (
      i=noteSlashes.begin ();
      i!=noteSlashes.end ();
      ++i
    ) {
      S_msrSlash slash = (*i);

      switch (slash->getSlashTypeKind ()) {
        case msrSlashTypeKind::kSlashType_NO_:
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
        case msrUseDotsKind::kUseDots_NO_:
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
        case msrSlashUseStemsKind::kSlashUseStems_NO_:
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
            "\\hide Staff.Stem " <<
            std::endl;
          break;
      } // switch
    } // for
  }
}

void lpsr2lilypondTranslator::generateNoteWedges (
  const S_msrNote& note)
{
  const std::list<S_msrWedge>&
    noteWedges =
      note->getNoteWedges ();

  if (noteWedges.size ()) {
    std::list<S_msrWedge>::const_iterator i;
    for (
      i=noteWedges.begin ();
      i!=noteWedges.end ();
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
  const std::list<S_msrSlur>&
    noteSlurs =
      note->getNoteSlurs ();

  if (noteSlurs.size ()) {
    std::list<S_msrSlur>::const_iterator i;
    for (
      i=noteSlurs.begin ();
      i!=noteSlurs.end ();
      ++i
    ) {
      S_msrSlur slur = (*i);

      /*
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
  const std::list<S_msrGlissando>&
    noteGlissandos =
      note->getNoteGlissandos ();

  if (noteGlissandos.size ()) {
    std::list<S_msrGlissando>::const_iterator i;
    for (
      i=noteGlissandos.begin ();
      i!=noteGlissandos.end ();
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

void lpsr2lilypondTranslator::generateNoteGlissandosWithText (
  const S_msrNote& note)
{
  const std::list<S_msrGlissando>&
    noteGlissandos =
      note->getNoteGlissandos ();

  if (noteGlissandos.size ()) {
    std::list<S_msrGlissando>::const_iterator i;
    for (
      i=noteGlissandos.begin ();
      i!=noteGlissandos.end ();
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

            if (glissandoTextValue.size ()) {
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
  const std::list<S_msrSlide>&
    noteSlides =
      note->getNoteSlides ();

  if (noteSlides.size ()) {

    std::list<S_msrSlide>::const_iterator i;
    for (
      i=noteSlides.begin ();
      i!=noteSlides.end ();
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

void lpsr2lilypondTranslator::generateNoteSlidesWithText (
  const S_msrNote& note)
{
  const std::list<S_msrSlide>&
    noteSlides =
      note->getNoteSlides ();

  if (noteSlides.size ()) {
    std::list<S_msrSlide>::const_iterator i;
    for (
      i=noteSlides.begin ();
      i!=noteSlides.end ();
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

            if (slideTextValue.size ()) {
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

void lpsr2lilypondTranslator::generateNoteTechnicalsWithStrings (
  const S_msrNote& note)
{
  int inputLineNumber =
    note->getInputLineNumber ();

  const std::list<S_msrTechnicalWithString>&
    noteTechnicalWithStrings =
      note->getNoteTechnicalWithStrings ();

  if (noteTechnicalWithStrings.size ()) {

    std::list<S_msrTechnicalWithString>::const_iterator i;
    for (
      i=noteTechnicalWithStrings.begin ();
      i!=noteTechnicalWithStrings.end ();
      ++i
    ) {
      S_msrTechnicalWithString technicalWithString = (*i);

      switch (technicalWithString->getTechnicalWithStringKind ()) {
        case msrTechnicalWithStringKind::kHammerOn:
          switch (technicalWithString->getTechnicalWithStringTypeKind ()) {
            case msrTechnicalTypeKind::kTechnicalTypeStart:
              {
                Rational
                  noteSoundingWholeNotes =
                    note->getMeasureElementSoundingWholeNotes ();

                Rational
                  halfWholeNotes =
                    noteSoundingWholeNotes /2;

                fLilypondCodeStream <<
                  "\\after " <<
                  wholeNotesAsLilypondString (
                    inputLineNumber,
                    halfWholeNotes) <<
                  " ^\"H\" ";
              }
              break;
            case msrTechnicalTypeKind::kTechnicalTypeStop:
              break;
            case msrTechnicalTypeKind::kTechnicalType_NO_:
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
                Rational
                  noteSoundingWholeNotes =
                    note->getMeasureElementSoundingWholeNotes ();

                Rational
                  halfWholeNotes =
                    noteSoundingWholeNotes /2;

                fLilypondCodeStream <<
                  "\\after " <<
                  wholeNotesAsLilypondString (
                    inputLineNumber,
                    halfWholeNotes) <<
                  " ^\"P\" ";
              }
              break;
            case msrTechnicalTypeKind::kTechnicalTypeStop:
              break;
            case msrTechnicalTypeKind::kTechnicalType_NO_:
              break;
          } // switch
          break;
      } // switch
    } // for
  }
}

void lpsr2lilypondTranslator::visitEnd (S_msrNote& elt)
{
#ifdef TRACING_IS_ENABLED
  int inputLineNumber =
    elt->getInputLineNumber ();

  {
    Bool
      traceLpsrVisitors =
        gGlobalLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream s;

      s <<
        "% --> End visiting note " <<
        elt->asShortString () <<
        ", line " << inputLineNumber <<
        std::endl;

      if (traceLpsrVisitors) {
        gLogStream << s.str ();
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << s.str ();
      }
    }
  }
#endif

  Bool noteIsToBeIgnored (false);

  // is this note to be ignored?
  if (fOnGoingChord && fOnGoingGraceNotesGroup) { // JMI
    noteIsToBeIgnored = true;
  }

  if (fOnGoingMultipleFullBarRests) {
    switch (elt->getNoteKind ()) {
      case msrNoteKind::kNoteRestInMeasure:
        // don't handle multiple full-bar restss, that's done in visitEnd (S_msrMultipleFullBarRests&)
          if (elt->getNoteOccupiesAFullMeasure ()) {
            Bool inhibitMultipleFullBarRestsBrowsing =
              fVisitedLpsrScore->
                getMsrScore ()->
                  getInhibitMultipleFullBarRestsBrowsing ();

            if (inhibitMultipleFullBarRestsBrowsing) {
#ifdef TRACING_IS_ENABLED
              if (
                gGlobalTracingOahGroup->getTraceNotes ()
                  ||
                gGlobalTracingOahGroup->getTraceMultipleFullBarRests ()
              ) {
                gLogStream <<
                  "% ==> end visiting multiple full-bar rests is ignored" <<
                  std::endl;
              }
#endif

#ifdef TRACING_IS_ENABLED
              if (gGlobalTracingOahGroup->getTraceNotesDetails ()) {
                gLogStream <<
                  "% ==> returning from visitEnd (S_msrNote&)" <<
                  std::endl;
              }
#endif

              noteIsToBeIgnored = true;
            }
          }
        break;

      case msrNoteKind::kNoteSkipInMeasure:
        if (elt->getNoteDirectUpLinkToGraceNotesGroup ()) {
#ifdef TRACING_IS_ENABLED
          if (
            gGlobalLpsrOahGroup->getTraceLpsrVisitors ()
              ||
            gGlobalTracingOahGroup->getTraceNotes ()
          ) {
            gLogStream <<
              "% ==> end visiting skip notes is ignored" <<
              std::endl;
          }
#endif

          noteIsToBeIgnored = true;
        }
        break;

      case msrNoteKind::kNoteRegularInGraceNotesGroup:
      case msrNoteKind::kNoteSkipInGraceNotesGroup:
#ifdef TRACING_IS_ENABLED
          if (
            gGlobalLpsrOahGroup->getTraceLpsrVisitors ()
              ||
            gGlobalTracingOahGroup->getTraceGraceNotes ()) {
            gLogStream <<
              "% ==> end visiting grace notes is ignored" <<
              std::endl;
          }
#endif

          noteIsToBeIgnored = true;
        break;

      default:
        ;
    } // switch
  }

  if (noteIsToBeIgnored) {
#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceNotes ()) {
      std::stringstream s;

      s <<
        "% ==> End visiting notes is ignored inside grace notes groups " << // JMI BLARK
        elt->asShortString ();

      lpsr2lilypondInternalWarning (
        gGlobalServiceRunData->getInputSourceName (),
        inputLineNumber,
        s.str ());
    }
#endif

    return;
  }

  // unregister the note as on-going note
  fOnGoingNotesStack.pop (); // was pushed in visitStart (S_msrNote&)

  S_msrSingleTremolo
    noteSingleTremolo =
      elt->getNoteSingleTremolo ();

  if (noteSingleTremolo) {
    // generate code for the single tremolo only
    // if note doesn't belong to a chord,
    // otherwise it will be generated for the chord itself
    if (! elt->getNoteBelongsToAChord ()) {
      fLilypondCodeStream <<
        singleTremoloDurationAsLilypondString (
          noteSingleTremolo);
    }
  }

  // generate the note words if any,
  // which should precede the articulations in LilyPond
  // generate code for the words only
  // if note doesn't belong to a chord,
  // otherwise it will be generated for the chord itself
  if (
 //   fOnGoingStanza // JMI ???
 //     &&
    ! elt->getNoteBelongsToAChord ()) {
    const std::list<S_msrWords>&
      noteWords =
        elt->getNoteWords ();

    if (noteWords.size ()) {
      generateCodeForNoteWords (noteWords);
    }
  }

/* TOO EARLY FOR ALL OF THEM??? JMI
  // generate the note articulations if any
  if (! fOnGoingChord) {
    const std::list<S_msrArticulation>&
      noteArticulations =
        elt->getNoteArticulations ();

    if (noteArticulations.size ()) {
      std::list<S_msrArticulation>::const_iterator i;
      for (
        i=noteArticulations.begin ();
        i!=noteArticulations.end ();
        ++i
      ) {
        const S_msrArticulation& articulation = (*i);
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
              std::stringstream s;

              s <<
                "note articulation '" <<
                articulation->asString () <<
                "' has 'fermata' kind, but is not of type S_msrFermata" <<
                ", line " << articulation->getInputLineNumber ();

              lpsr2lilypondInternalError (
                gGlobalServiceRunData->getInputSourceName (),
                articulation->getInputLineNumber (),
                __FILE__, __LINE__,
                s.str ());
            }
            break;

          default:
            generateNoteArticulation ((*i));
            fLilypondCodeStream <<
              ' ';
        } // switch
      } // for
    }
  }
  */

  // generate the note technicals if any
  const std::list<S_msrTechnical>&
    noteTechnicals =
      elt->getNoteTechnicals ();

  if (noteTechnicals.size ()) {
    std::list<S_msrTechnical>::const_iterator i;
    for (
      i=noteTechnicals.begin ();
      i!=noteTechnicals.end ();
      ++i
    ) {

      fLilypondCodeStream <<
        technicalAsLilypondString ((*i));

      switch ((*i)->getTechnicalPlacementKind ()) {
        case msrPlacementKind::kPlacement_NO_:
          break;
        case msrPlacementKind::kPlacementAbove:
          fLilypondCodeStream << "^";
          break;
        case msrPlacementKind::kPlacementBelow:
          fLilypondCodeStream << "_";
          break;
      } // switch

      fLilypondCodeStream << ' ';
    } // for
  }

  // generate the note technicals with integer if any,
  // but not for chord member notes strings:
  // they should appear after the chord itself
  switch (elt->getNoteKind ()) {
    case msrNoteKind::kNoteRegularInChord:
       break;

    default:
      {
        const std::list<S_msrTechnicalWithInteger>&
          noteTechnicalWithIntegers =
            elt->getNoteTechnicalWithIntegers ();

        if (noteTechnicalWithIntegers.size ()) {
          std::list<S_msrTechnicalWithInteger>::const_iterator i;
          for (
            i=noteTechnicalWithIntegers.begin ();
            i!=noteTechnicalWithIntegers.end ();
            ++i
          ) {

            S_msrTechnicalWithInteger
                technicalWithInteger = (*i);

            fLilypondCodeStream <<
              technicalWithIntegerAsLilypondString (
                technicalWithInteger);

            switch (technicalWithInteger->getTechnicalWithIntegerPlacementKind ()) {
              case msrPlacementKind::kPlacement_NO_:
                break;
              case msrPlacementKind::kPlacementAbove:
                fLilypondCodeStream << "^";
                break;
              case msrPlacementKind::kPlacementBelow:
                fLilypondCodeStream << "_";
                break;
            } // switch

            fLilypondCodeStream << ' ';
          } // for
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
        const std::list<S_msrTechnicalWithFloat>&
          noteTechnicalWithFloats =
            elt->getNoteTechnicalWithFloats ();

        if (noteTechnicalWithFloats.size ()) {
          std::list<S_msrTechnicalWithFloat>::const_iterator i;
          for (
            i=noteTechnicalWithFloats.begin ();
            i!=noteTechnicalWithFloats.end ();
            ++i
          ) {

            S_msrTechnicalWithFloat
                technicalWithFloat = (*i);

            fLilypondCodeStream <<
              technicalWithFloatAsLilypondString (
                technicalWithFloat);

            switch (technicalWithFloat->getTechnicalWithFloatPlacementKind ()) {
              case msrPlacementKind::kPlacement_NO_:
                break;
              case msrPlacementKind::kPlacementAbove:
                fLilypondCodeStream << "^";
                break;
              case msrPlacementKind::kPlacementBelow:
                fLilypondCodeStream << "_";
                break;
            } // switch

            fLilypondCodeStream << ' ';
          } // for
        }
      }
  } // switch

  // generate the note technicals with std::string if any
  const std::list<S_msrTechnicalWithString>&
    noteTechnicalWithStrings =
      elt->getNoteTechnicalWithStrings ();

  if (noteTechnicalWithStrings.size ()) {
    std::list<S_msrTechnicalWithString>::const_iterator i;
    for (
      i=noteTechnicalWithStrings.begin ();
      i!=noteTechnicalWithStrings.end ();
      ++i
    ) {

      fLilypondCodeStream <<
        technicalWithStringAsLilypondString ((*i));

      switch ((*i)->getTechnicalWithStringPlacementKind ()) {
        case msrPlacementKind::kPlacement_NO_:
          break;
        case msrPlacementKind::kPlacementAbove:
          fLilypondCodeStream << "^";
          break;
        case msrPlacementKind::kPlacementBelow:
          fLilypondCodeStream << "_";
          break;
      } // switch

      fLilypondCodeStream << ' ';
    } // for
  }

  // generate the note ornaments if any
  const std::list<S_msrOrnament>&
    noteOrnaments =
      elt->getNoteOrnaments ();

  if (noteOrnaments.size ()) {
    std::list<S_msrOrnament>::const_iterator i;
    for (
      i=noteOrnaments.begin ();
      i!=noteOrnaments.end ();
      ++i
    ) {
      S_msrOrnament
        ornament = (*i);

      generateOrnament (ornament); // some ornaments are not yet supported
    } // for
  }

  // generate the note dynamics if any
  if (! fOnGoingChord) {
    const std::list<S_msrDynamic>&
      noteDynamics =
        elt->getNoteDynamics ();

    if (noteDynamics.size ()) {
      std::list<S_msrDynamic>::const_iterator i;
      for (
        i=noteDynamics.begin ();
        i!=noteDynamics.end ();
        ++i
      ) {
        S_msrDynamic
          dynamic = (*i);

        switch (dynamic->getDynamicPlacementKind ()) {
          case msrPlacementKind::kPlacement_NO_:
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
          dynamicAsLilypondString (dynamic) << ' ';
      } // for
    }
  }

  // generate the note other dynamics if any
  if (! fOnGoingChord) {
    const std::list<S_msrOtherDynamic>&
      noteOtherDynamics =
        elt->getNoteOtherDynamics ();

    if (noteOtherDynamics.size ()) {
      std::list<S_msrOtherDynamic>::const_iterator i;
      for (
        i=noteOtherDynamics.begin ();
        i!=noteOtherDynamics.end ();
        ++i
      ) {
        S_msrOtherDynamic
          otherDynamic = (*i);

        switch (otherDynamic->getOtherDynamicPlacementKind ()) {
          case msrPlacementKind::kPlacement_NO_:
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
  }

  // generate the note beams if any,
  // unless the note is chord member
  Bool doGenerateBeams (true);
  if (elt->getNoteBelongsToAChord ()) {
    doGenerateBeams = false;
  }
  if (doGenerateBeams) {
    generateNoteBeams (elt);
  }

  // generate the note ligatures if any
  const std::list<S_msrLigature>&
    noteLigatures =
      elt->getNoteLigatures ();

  if (noteLigatures.size ()) {
    std::list<S_msrLigature>::const_iterator i;
    for (
      i=noteLigatures.begin ();
      i!=noteLigatures.end ();
      ++i
    ) {
      switch ((*i)->getLigatureKind ()) {
        case msrLigatureKind::kLigatureNone:
          break;
        case msrLigatureKind::kLigatureStart:
   // JMI       fLilypondCodeStream << "\\[ ";
          break;
        case msrLigatureKind::kLigatureContinue:
          break;
        case msrLigatureKind::kLigatureStop:
          fLilypondCodeStream << "\\] ";
          break;
      } // switch
    } // for
  }

  // generate the note wedges if any
  const std::list<S_msrWedge>&
    noteWedges =
      elt->getNoteWedges ();

  if (noteWedges.size ()) {
    std::list<S_msrWedge>::const_iterator i;
    for (
      i=noteWedges.begin ();
      i!=noteWedges.end ();
      ++i
    ) {
      S_msrWedge wedge = (*i);

      switch (wedge->getWedgeKind ()) {
        case msrWedgeKind::kWedgeKindNone:
          break;

        case msrWedgeKind::kWedgeCrescendo:
          switch (wedge->getWedgePlacementKind ()) {
            case msrPlacementKind::kPlacement_NO_:
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
            "\\< ";
          break;

        case msrWedgeKind::kWedgeDecrescendo:
          switch (wedge->getWedgePlacementKind ()) {
            case msrPlacementKind::kPlacement_NO_:
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
            "\\> ";
          break;

        case msrWedgeKind::kWedgeStop:
          fLilypondCodeStream <<
            "\\! ";
          break;
      } // switch
    } // for
  }

  // generate the note slurs if any,
  // unless the note is chord member
  Bool doGenerateSlurs (true);
  if (elt->getNoteBelongsToAChord ()) {
     doGenerateSlurs = false;
  }
  if (doGenerateSlurs) {
    generateNoteSlurs (elt);
  }

  // generate the note articulations if any,
  // which should follow the dynamics and wedges in LilyPond
  if (! fOnGoingChord) {
    const std::list<S_msrArticulation>&
      noteArticulations =
        elt->getNoteArticulations ();

    if (noteArticulations.size ()) {
      std::list<S_msrArticulation>::const_iterator i;
      for (
        i=noteArticulations.begin ();
        i!=noteArticulations.end ();
        ++i
      ) {
        const S_msrArticulation& articulation = (*i);

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
                    elt->getNoteOccupiesAFullMeasure ()
                      &&
                    elt->fetchNoteIsARest ()
                  ) {
//                     fLilypondCodeStream << "\\fermataMarkup ";
                    fLilypondCodeStream << "\\fermata "; // v0.9.63, since LilyPond 2.23
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
              std::stringstream s;

              s <<
                "note articulation '" <<
                articulation->asString () <<
                "' has 'fermata' kind, but is not of type S_msrFermata" <<
                ", line " << articulation->getInputLineNumber ();

              lpsr2lilypondInternalError (
                gGlobalServiceRunData->getInputSourceName (),
                articulation->getInputLineNumber (),
                __FILE__, __LINE__,
                s.str ());
            }
            break;

          default:
            generateNoteArticulation ((*i));
            fLilypondCodeStream <<
              ' ';
        } // switch
      } // for
    }
  }

  // generate the note glissandos if any
  const std::list<S_msrGlissando>&
    noteGlissandos =
      elt->getNoteGlissandos ();

  if (noteGlissandos.size ()) {
    std::list<S_msrGlissando>::const_iterator i;
    for (
      i=noteGlissandos.begin ();
      i!=noteGlissandos.end ();
      ++i
    ) {
      S_msrGlissando glissando = (*i);

      switch (glissando->getGlissandoTypeKind ()) {
        case msrGlissandoTypeKind::kGlissandoTypeNone:
          break;

        case msrGlissandoTypeKind::kGlissandoTypeStart:
          // generate the glissando itself
          fLilypondCodeStream <<
            "\\glissando " <<
            "\\glissandoTextOff ";
          break;

        case msrGlissandoTypeKind::kGlissandoTypeStop:
          break;
      } // switch
    } // for
  }

  // generate the note slides if any, implemented as glissandos
  const std::list<S_msrSlide>&
    noteSlides =
      elt->getNoteSlides ();

  if (noteSlides.size ()) {
    std::list<S_msrSlide>::const_iterator i;
    for (
      i=noteSlides.begin ();
      i!=noteSlides.end ();
      ++i
    ) {
      S_msrSlide slide = (*i);

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

  // the note spanners if any are handled in visitStart (S_msrNote&)

  // are there note scordaturas?
  const std::list<S_msrScordatura>&
    noteScordaturas =
      elt->getNoteScordaturas ();

  if (noteScordaturas.size ()) { // g_Indenter ??? JMI
    --gIndenter;

    fLilypondCodeStream <<
      std::endl <<
      '}' <<
      std::endl;

    --gIndenter;

    fLilypondCodeStream <<
      ">>" <<
      std::endl;
  }

  if (elt->getNoteIsFollowedByGraceNotesGroup ()) { // JMI
    if (! elt->fetchNoteIsARest ()) {
      fLilypondCodeStream <<
       " % noteIsFollowedByGraceNotesGroup" <<
        std::endl; // JMI ???
    }
  }

  // get the note's grace notes now,
  // unless the note belongs to a grace notes group link
  if (! fOnGoingChordGraceNotesGroupLink) {
    S_msrGraceNotesGroup
      noteGraceNotesGroupAfter =
        elt->getNoteGraceNotesGroupAfter ();

    // generate the note's grace notes after group closer if any
    if (noteGraceNotesGroupAfter) {
      fLilypondCodeStream <<
        "} ";
      generateGraceNotesGroup (
        noteGraceNotesGroupAfter);
    }
  }
}

//________________________________________________________________________
void lpsr2lilypondTranslator::visitStart (S_msrOctaveShift& elt)
{
#ifdef TRACING_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gGlobalLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream s;

      s <<
        "% --> Start visiting msrOctaveShift" <<
        ", line " << elt->getInputLineNumber () <<
        std::endl;

      if (traceLpsrVisitors) {
        gLogStream << s.str ();
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << s.str ();
      }
    }
  }
#endif
}

void lpsr2lilypondTranslator::visitEnd (S_msrOctaveShift& elt)
{
#ifdef TRACING_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gGlobalLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream s;

      s <<
        "% --> End visiting msrOctaveShift" <<
        ", line " << elt->getInputLineNumber () <<
        std::endl;

      if (traceLpsrVisitors) {
        gLogStream << s.str ();
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << s.str ();
      }
    }
  }
#endif
}

//________________________________________________________________________
void lpsr2lilypondTranslator::visitStart (S_msrAccordionRegistration& elt)
{
#ifdef TRACING_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gGlobalLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream s;

      s <<
        "% --> Start visiting msrAccordionRegistration" <<
        ", line " << elt->getInputLineNumber () <<
        std::endl;

      if (traceLpsrVisitors) {
        gLogStream << s.str ();
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << s.str ();
      }
    }
  }
#endif

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
#ifdef TRACING_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gGlobalLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream s;

      s <<
        "% --> Start visiting msrHarpPedalsTuning" <<
        ", line " << elt->getInputLineNumber () <<
        std::endl;

      if (traceLpsrVisitors) {
        gLogStream << s.str ();
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << s.str ();
      }
    }
  }
#endif

  std::map<msrDiatonicPitchKind, msrAlterationKind>
    harpPedalsAlterationKindsMap =
      elt->getHarpPedalsAlterationKindsMap ();

  if (harpPedalsAlterationKindsMap.size ()) {
    ++gIndenter;

    fLilypondCodeStream <<
      "_\\markup { \\harp-pedal #\"" <<
      harpPedalTuningAsLilypondString (
        harpPedalsAlterationKindsMap [msrDiatonicPitchKind::kDiatonicPitchD]) <<
      harpPedalTuningAsLilypondString (
        harpPedalsAlterationKindsMap [msrDiatonicPitchKind::kDiatonicPitchC]) <<
      harpPedalTuningAsLilypondString (
        harpPedalsAlterationKindsMap [msrDiatonicPitchKind::kDiatonicPitchB]) <<
      "|" <<
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
#ifdef TRACING_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gGlobalLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream s;

      s <<
        "% --> Start visiting msrStem" <<
        ", line " << elt->getInputLineNumber () <<
        std::endl;

      if (traceLpsrVisitors) {
        gLogStream << s.str ();
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << s.str ();
      }
    }
  }
#endif
}

void lpsr2lilypondTranslator::visitEnd (S_msrStem& elt)
{
#ifdef TRACING_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gGlobalLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream s;

      s <<
        "% --> End visiting msrStem" <<
        ", line " << elt->getInputLineNumber () <<
        std::endl;

      if (traceLpsrVisitors) {
        gLogStream << s.str ();
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << s.str ();
      }
    }
  }
#endif
}

//________________________________________________________________________
void lpsr2lilypondTranslator::visitStart (S_msrBeam& elt)
{
#ifdef TRACING_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gGlobalLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream s;

      s <<
        "% --> Start visiting msrBeam" <<
        ", line " << elt->getInputLineNumber () <<
        std::endl;

      if (traceLpsrVisitors) {
        gLogStream << s.str ();
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << s.str ();
      }
    }
  }
#endif
}

void lpsr2lilypondTranslator::visitEnd (S_msrBeam& elt)
{
#ifdef TRACING_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gGlobalLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream s;

      s <<
        "% --> End visiting msrBeam" <<
        ", line " << elt->getInputLineNumber () <<
        std::endl;

      if (traceLpsrVisitors) {
        gLogStream << s.str ();
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << s.str ();
      }
    }
  }
#endif
}

//________________________________________________________________________
void lpsr2lilypondTranslator::generateCodeForOctaveShiftBeforeNote (
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
        ' ';
      break;

    case msrOctaveShiftKind::kOctaveShiftDown:
      fLilypondCodeStream <<
        "\\ottava #" <<
        (octaveShiftSize - 1) / 7 << // 1 or 2
        ' ';
      break;

    case msrOctaveShiftKind::kOctaveShiftStop:
      break;

    case msrOctaveShiftKind::kOctaveShiftContinue:
      break;
  } // switch
}

void lpsr2lilypondTranslator::generateCodeForOctaveShiftAfterNote (
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
          fLilypondCodeStream <<
            "\\ottava #0 ";
      break;

    case msrOctaveShiftKind::kOctaveShiftContinue:
      break;
  } // switch
}

//________________________________________________________________________
void lpsr2lilypondTranslator::generateCodeAheadOfChordContents (
  const S_msrChord& chord)
{
  // generate the chord codas if any
  const std::list<S_msrCoda>&
    chordCodas =
      chord->getChordCodas ();

  if (chordCodas.size ()) {
    std::list<S_msrCoda>::const_iterator i;
    for (i=chordCodas.begin (); i!=chordCodas.end (); ++i) {
      // generate the coda
      S_msrCoda coda = (*i);

      generateCoda (coda);
    } // for
  }

  // generate the chord segnos if any
  const std::list<S_msrSegno>&
    chordSegnos =
      chord->getChordSegnos ();

  if (chordSegnos.size ()) {
    std::list<S_msrSegno>::const_iterator i;
    for (i=chordSegnos.begin (); i!=chordSegnos.end (); ++i) {
      // generate the segno
      generateSegno ((*i));
    } // for
  }
}

void lpsr2lilypondTranslator::generateCodeRightBeforeChordContents (
  const S_msrChord& chord)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceChords ()) {
    fLilypondCodeStream <<
      "% --> generateCodeRightBeforeChordContents() for chord " <<
      chord->asShortString () <<
      ", line " << chord->getInputLineNumber () <<
      std::endl;
  }
#endif

/* JMI
  gLogStream <<
    "% chordGraceNotesGroupBefore = ";
  if (chordGraceNotesGroupBefore) {
    gLogStream <<
      chordGraceNotesGroupBefore;
  }
  else {
    gLogStream <<
      "[NONE]";
  }
  gLogStream << std::endl;
*/

  // generate the chord's grace notes before if any,
  const S_msrChordGraceNotesGroupLink&
    chordGraceNotesGroupLinkBefore =
      chord->getChordGraceNotesGroupLinkBefore ();

  if (chordGraceNotesGroupLinkBefore) {
    generateGraceNotesGroup (
      chordGraceNotesGroupLinkBefore->
        getOriginalGraceNotesGroup ());
  }

  // get the chord glissandos
  const std::list<S_msrGlissando>&
    chordGlissandos =
      chord->getChordGlissandos ();

  // generate the chord glissandos styles if any
  if (chordGlissandos.size ()) {
    std::list<S_msrGlissando>::const_iterator i;
    for (
      i=chordGlissandos.begin ();
      i!=chordGlissandos.end ();
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

  // get the chord slides
  const std::list<S_msrSlide>&
    chordSlides =
      chord->getChordSlides ();

  // generate the chord slides styles if any, implemented as glissandos
  if (chordSlides.size ()) {
    std::list<S_msrSlide>::const_iterator i;
    for (
      i=chordSlides.begin ();
      i!=chordSlides.end ();
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

  // get the chord ligatures
  const std::list<S_msrLigature>&
    chordLigatures =
      chord->getChordLigatures ();

  // generate the chord ligatures if any
  if (chordLigatures.size ()) {
    std::list<S_msrLigature>::const_iterator i;
    for (
      i=chordLigatures.begin ();
      i!=chordLigatures.end ();
      ++i
    ) {

      switch ((*i)->getLigatureKind ()) {
        case msrLigatureKind::kLigatureNone:
          break;
        case msrLigatureKind::kLigatureStart:
          fLilypondCodeStream << "\\[ ";
          break;
        case msrLigatureKind::kLigatureContinue:
          break;
        case msrLigatureKind::kLigatureStop:
  // JMI        fLilypondCodeStream << "\\] ";
          break;
      } // switch
    } // for
  }

  // don't take the chord into account for line breaking ??? JMI

  // get the chord articulations
  const std::list<S_msrArticulation>&
    chordArticulations =
      chord->getChordArticulations ();

  // generate the chord arpeggios directions if any
  if (chordArticulations.size ()) {
    std::list<S_msrArticulation>::const_iterator i;
    for (
      i=chordArticulations.begin ();
      i!=chordArticulations.end ();
      ++i
    ) {
      const S_msrArticulation& articulation = (*i);

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

        fLilypondCodeStream << ' ';

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
            fLilypondCodeStream << " %{ \\kArticulationNonArpeggiatoTypeNone??? %}";
            break;
          case msrNonArpeggiatoTypeKind::kArticulationNonArpeggiatoTypeTop:
            fLilypondCodeStream << " %{ \\kArticulationNonArpeggiatoTypeTop??? %}";
            break;
          case msrNonArpeggiatoTypeKind::kArticulationNonArpeggiatoTypeBottom:
            fLilypondCodeStream << " %{ \\kArticulationNonArpeggiatoTypeBottom??? %}";
            break;
        } // switch

        fLilypondCodeStream << std::endl;
      }
   } // for
  }

  // should a stem direction command be generated?
  const std::list<S_msrStem>&
    chordStems =
      chord->getChordStems ();

  if (chordStems.size ()) {
    // only the first stem is used, since they all have the same kind
    S_msrStem firstChordStem = chordStems.front ();

    // sanity check
    mfAssert (
      __FILE__, __LINE__,
      firstChordStem != nullptr,
      "firstChordStem is null");

#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceStems ()) {
      fLilypondCodeStream <<
        "% --> generateCodeRightBeforeChordContents() for chord " <<
        chord->asShortString () <<
        ", firstChordStem: " << firstChordStem <<
        ", line " << chord->getInputLineNumber () <<
        std::endl;
    }
#endif

    generateStemIfNeededAndUpdateCurrentStemKind (firstChordStem);

    fLilypondCodeStream <<
      ' ';
  }

  // should an octave shift be generated?
  S_msrOctaveShift
    chordOctaveShift =
      chord->getChordOctaveShift ();

  if (chordOctaveShift) {
    generateCodeForOctaveShiftBeforeNote (
      chordOctaveShift);
  }
}

void lpsr2lilypondTranslator::generateCodeForChordInGraceNotesGroupContents (
  const S_msrChord& chord)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceChords ()) {
    fLilypondCodeStream <<
      "% --> generateCodeForChordInGraceNotesGroupContents() for chord " <<
      chord->asShortString () <<
      ", line " << chord->getInputLineNumber () <<
      std::endl;
  }
#endif

  // get the chord notes std::vector
  const std::vector<S_msrNote>&
    chordNotesVector =
      chord->getChordNotesVector ();

  // generate the chord notes KOF JMI
  if (chordNotesVector.size ()) {
    std::vector<S_msrNote>::const_iterator
      iBegin = chordNotesVector.begin (),
      iEnd   = chordNotesVector.end (),
      i      = iBegin;
    for ( ; ; ) {
      S_msrNote
        chordNote = (*i);

      // generate things before the chordNote
      generateCodeRightBeforeNote (chordNote);

      // generate the chordNote itself
      generateCodeForNote (chordNote);

      // generate things after the chordNote
      generateCodeRightAfterNote (chordNote);

      if (++i == iEnd) break;
      fLilypondCodeStream <<
        ' ';
    } // for
  }
}

void lpsr2lilypondTranslator::generateCodeRightAfterChordContents (
  const S_msrChord& chord)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceChords ()) {
    fLilypondCodeStream <<
      std::endl <<
      "% --> generateCodeRightAfterChordContents() for chord " <<
      chord->asShortString () <<
      ", line " << chord->getInputLineNumber () <<
      std::endl;
  }
#endif

  // get the chord notes std::vector
  const std::vector<S_msrNote>&
    chordNotesVector =
      chord->getChordNotesVector ();

  // if the preceding item is a chord, the first note of the chord
  // is used as the reference point for the octave placement
  // of a following note or chord
  switch (gGlobalLpsr2lilypondOahGroup->fetchOctaveEntryVariableValue ()) {
    case msrOctaveEntryKind::kOctaveEntryRelative:
      fCurrentOctaveEntryReference =
        chordNotesVector [0];
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
    int chordInputLineNumber =
      chord->getInputLineNumber ();

    // generate the chord duration
    fLilypondCodeStream <<
      durationAsLilypondString (
        chordInputLineNumber,
        chord->
          getChordDisplayWholeNotes ()); // JMI test wether chord is in a tuplet?
  }

/* JMI
  gLogStream <<
    "% chordGraceNotesGroupAfter = ";
  if (chordGraceNotesGroupAfter) {
    gLogStream <<
      chordGraceNotesGroupAfter;
  }
  else {
    gLogStream <<
      "[NONE]";
  }
  gLogStream << std::endl;

  if (chordGraceNotesGroupAfter) {
    generateGraceNotesGroup (
      chordGraceNotesGroupAfter);
  }
*/

  // generate the chord's grace notes after if any
  const S_msrChordGraceNotesGroupLink&
    chordGraceNotesGroupLinkAfter =
      chord->getChordGraceNotesGroupLinkAfter ();

  if (chordGraceNotesGroupLinkAfter) {
    generateGraceNotesGroup (
      chordGraceNotesGroupLinkAfter->
        getOriginalGraceNotesGroup ());
  }

  // are there pending chord member notes std::string numbers?
  if (fPendingChordMemberNotesStringNumbers.size ()) {
    std::list<int>::const_iterator
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
        ' ';
    } // for
    fLilypondCodeStream <<
      ' ';

    // forget about the pending std::string numbers
    fPendingChordMemberNotesStringNumbers.clear ();
  }

  // fetch the chord single tremolo
  S_msrSingleTremolo
    chordSingleTremolo =
      chord->getChordSingleTremolo ();

  if (chordSingleTremolo) {
    // generate code for the chord single tremolo
    fLilypondCodeStream <<
      singleTremoloDurationAsLilypondString (
        chordSingleTremolo);
  }

  fLilypondCodeStream <<
    ' ';

//   // get the chord articulations
//   const std::list<S_msrArticulation>&
//     chordArticulations =
//       chord->getChordArticulations ();
//
//   // generate the chord articulations if any
//   if (chordArticulations.size ()) {
//     std::list<S_msrArticulation>::const_iterator i;
//     for (
//       i=chordArticulations.begin ();
//       i!=chordArticulations.end ();
//       ++i
//     ) {
//       generateChordArticulation ((*i));
//
//       fLilypondCodeStream <<
//         ' ';
//     } // for
//   }
//
  // generate the chord technicals if any
  const std::list<S_msrTechnical>&
    chordTechnicals =
      chord->getChordTechnicals ();

  if (chordTechnicals.size ()) {
    std::list<S_msrTechnical>::const_iterator i;
    for (
      i=chordTechnicals.begin ();
      i!=chordTechnicals.end ();
      ++i
    ) {
      fLilypondCodeStream <<
        technicalAsLilypondString ((*i)) <<
        ' '; // JMI
    } // for
  }

  // generate the chord technicals with integer if any
  const std::list<S_msrTechnicalWithInteger>&
    chordTechnicalWithIntegers =
      chord->getChordTechnicalWithIntegers ();

  if (chordTechnicalWithIntegers.size ()) {
    std::list<S_msrTechnicalWithInteger>::const_iterator i;
    for (
      i=chordTechnicalWithIntegers.begin ();
      i!=chordTechnicalWithIntegers.end ();
      ++i
    ) {
      fLilypondCodeStream <<
        technicalWithIntegerAsLilypondString ((*i)) <<
        ' '; // JMI
    } // for
  }

  // generate the chord technicals with float if any
  const std::list<S_msrTechnicalWithFloat>&
    chordTechnicalWithFloats =
      chord->getChordTechnicalWithFloats ();

  if (chordTechnicalWithFloats.size ()) {
    std::list<S_msrTechnicalWithFloat>::const_iterator i;
    for (
      i=chordTechnicalWithFloats.begin ();
      i!=chordTechnicalWithFloats.end ();
      ++i
    ) {
      fLilypondCodeStream <<
        technicalWithFloatAsLilypondString ((*i)) <<
        ' '; // JMI
    } // for
  }

  // generate the chord technicals with std::string if any
  const std::list<S_msrTechnicalWithString>&
    chordTechnicalWithStrings =
      chord->getChordTechnicalWithStrings ();

  if (chordTechnicalWithStrings.size ()) {
    std::list<S_msrTechnicalWithString>::const_iterator i;
    for (
      i=chordTechnicalWithStrings.begin ();
      i!=chordTechnicalWithStrings.end ();
      ++i
    ) {
      fLilypondCodeStream <<
        technicalWithStringAsLilypondString ((*i)) <<
        ' '; // JMI
    } // for
  }

  // generate the chord ornaments if any
  const std::list<S_msrOrnament>&
    chordOrnaments =
      chord->getChordOrnaments ();

  if (chordOrnaments.size ()) {
    std::list<S_msrOrnament>::const_iterator i;
    for (
      i=chordOrnaments.begin ();
      i!=chordOrnaments.end ();
      ++i
    ) {
      S_msrOrnament
        ornament = (*i);

      switch (ornament->getOrnamentPlacementKind ()) {
        case msrPlacementKind::kPlacement_NO_:
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
  const std::list<S_msrDynamic>&
    chordDynamics =
      chord->getChordDynamics ();

  if (chordDynamics.size ()) {
    std::list<S_msrDynamic>::const_iterator i;
    for (
      i=chordDynamics.begin ();
      i!=chordDynamics.end ();
      ++i
    ) {
      S_msrDynamic
        dynamic = (*i);

      switch (dynamic->getDynamicPlacementKind ()) {
        case msrPlacementKind::kPlacement_NO_:
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
        dynamicAsLilypondString (dynamic) << ' ';
    } // for
  }

  // generate the chord other dynamics if any
  const std::list<S_msrOtherDynamic>&
    chordOtherDynamics =
      chord->getChordOtherDynamics ();

  if (chordOtherDynamics.size ()) {
    std::list<S_msrOtherDynamic>::const_iterator i;
    for (
      i=chordOtherDynamics.begin ();
      i!=chordOtherDynamics.end ();
      ++i
    ) {
      S_msrOtherDynamic
        otherDynamic = (*i);

      switch (otherDynamic->getOtherDynamicPlacementKind ()) {
        case msrPlacementKind::kPlacement_NO_:
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

  // JMI v0.9.61, only now after the various dynamics
  // get the chord articulations
  const std::list<S_msrArticulation>&
    chordArticulations =
      chord->getChordArticulations ();

  // generate the chord articulations if any
  if (chordArticulations.size ()) {
    std::list<S_msrArticulation>::const_iterator i;
    for (
      i=chordArticulations.begin ();
      i!=chordArticulations.end ();
      ++i
    ) {
      generateChordArticulation ((*i));

      fLilypondCodeStream <<
        ' ';
    } // for
  }

  // generate the chord words if any
  const std::list<S_msrWords>&
    chordWords =
      chord->getChordWords ();

  if (chordWords.size ()) {
    std::list<S_msrWords>::const_iterator i;
    for (
      i=chordWords.begin ();
      i!=chordWords.end ();
      ++i
    ) {

      msrPlacementKind
        wordsPlacementKind =
          (*i)->getWordsPlacementKind ();

      std::string wordsContents =
        (*i)->getWordsContents ();

      switch (wordsPlacementKind) {
        case msrPlacementKind::kPlacement_NO_:
          // should not occur
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

  // generate the chord beams links if any
  const std::list<S_msrChordBeamLink>&
    chordBeamLinks =
      chord->getChordBeamLinks ();

  if (chordBeamLinks.size ()) {
    std::list<S_msrChordBeamLink>::const_iterator i;
    for (
      i=chordBeamLinks.begin ();
      i!=chordBeamLinks.end ();
      ++i
    ) {
      S_msrChordBeamLink chordBeamLink = (*i);

      S_msrBeam originalBeam = chordBeamLink->getOriginalBeam ();

      // LilyPond will take care of multiple beams automatically,
      // so we need only generate code for the first number (level)
      switch (originalBeam->getBeamKind ()) {

        case msrBeamKind::kBeamBegin:
          if (originalBeam->getBeamNumber () == 1)
            fLilypondCodeStream << "[ ";
          break;

        case msrBeamKind::kBeamContinue:
          break;

        case msrBeamKind::kBeamEnd:
          if (originalBeam->getBeamNumber () == 1)
            fLilypondCodeStream << "] ";
          break;

        case msrBeamKind::kBeamForwardHook:
          break;

        case msrBeamKind::kBeamBackwardHook:
          break;

        case msrBeamKind::kBeam_NO_:
          break;
      } // switch
    } // for
  }

  // generate the chord slur links if any
  const std::list<S_msrChordSlurLink>&
    chordSlurLinks =
      chord->getChordSlurLinks ();

  if (chordSlurLinks.size ()) {
    std::list<S_msrChordSlurLink>::const_iterator i;
    for (
      i=chordSlurLinks.begin ();
      i!=chordSlurLinks.end ();
      ++i
    ) {
      S_msrChordSlurLink chordSlurLink = (*i);

      S_msrSlur originalSlur = chordSlurLink->getOriginalSlur ();

      switch (originalSlur->getSlurTypeKind ()) {
        case msrSlurTypeKind::kSlurType_NO_:
          break;

        case msrSlurTypeKind::kSlurTypeRegularStart:
          fLilypondCodeStream << "( ";
          break;
        case msrSlurTypeKind::kSlurTypeRegularContinue:
          break;
        case msrSlurTypeKind::kSlurTypeRegularStop:
          fLilypondCodeStream << ") ";
          break;

        case msrSlurTypeKind::kSlurTypePhrasingStart:
          fLilypondCodeStream << "\\( ";
          break;
        case msrSlurTypeKind::kSlurTypePhrasingContinue:
          break;
        case msrSlurTypeKind::kSlurTypePhrasingStop:
          fLilypondCodeStream << "\\) ";
          break;
      } // switch

      if (gGlobalLpsr2lilypondOahGroup->getInputLineNumbers ()) {
        // generate the input line number as a comment
        fLilypondCodeStream <<
          " %{ line " << originalSlur->getInputLineNumber () << " chord %} ";
      }
    } // for
  }

/* Don't print the chord ties, rely only on its notes's ties // JMI
  // thus using LilyPond's partially tied chords // JMI
  // generate the chord ties if any
  const std::list<S_msrTie>&
    chordTies =
      chord->getChordTies ();

  if (chordTies.size ()) {
    std::list<S_msrTie>::const_iterator i;
    for (
      i=chordTies.begin ();
      i!=chordTies.end ();
      ++i
    ) {
      fLilypondCodeStream <<
        "%{line " << inputLineNumber << "%}" <<
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
          "%{line " << inputLineNumber << "%}" <<
          "~ ";
      }
    }
  }
*/

  // get the chord ligatures
  const std::list<S_msrLigature>&
    chordLigatures =
      chord->getChordLigatures ();

  // generate the chord ligatures if any
  if (chordLigatures.size ()) {
    std::list<S_msrLigature>::const_iterator i;
    for (
      i=chordLigatures.begin ();
      i!=chordLigatures.end ();
      ++i
    ) {
      switch ((*i)->getLigatureKind ()) {
        case msrLigatureKind::kLigatureNone:
          break;
        case msrLigatureKind::kLigatureStart:
          fLilypondCodeStream << "\\[ ";
          break;
        case msrLigatureKind::kLigatureContinue:
          break;
        case msrLigatureKind::kLigatureStop:
          fLilypondCodeStream << "\\] ";
          break;
      } // switch
    } // for
  }

  // generate the chord wedges if any
  const std::list<S_msrWedge>&
    chordWedges =
      chord->getChordWedges ();

  if (chordWedges.size ()) {
    std::list<S_msrWedge>::const_iterator i;
    for (
      i=chordWedges.begin ();
      i!=chordWedges.end ();
      ++i
      ) {
      switch ((*i)->getWedgeKind ()) {
        case msrWedgeKind::kWedgeKindNone:
          break;
        case msrWedgeKind::kWedgeCrescendo:
          fLilypondCodeStream << "\\< ";
          break;
        case msrWedgeKind::kWedgeDecrescendo:
          fLilypondCodeStream << "\\> ";
          break;
        case msrWedgeKind::kWedgeStop:
          fLilypondCodeStream << "\\! ";
          break;
      } // switch
    } // for
  }

  // get the chord glissandos
  const std::list<S_msrGlissando>&
    chordGlissandos =
      chord->getChordGlissandos ();

  // generate the chord glissandos if any
  if (chordGlissandos.size ()) {
    std::list<S_msrGlissando>::const_iterator i;
    for (
      i=chordGlissandos.begin ();
      i!=chordGlissandos.end ();
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
  const std::list<S_msrSlide>&
    chordSlides =
      chord->getChordSlides ();

  // generate the chord slides if any, implemented as glissandos
  if (chordSlides.size ()) {
    std::list<S_msrSlide>::const_iterator i;
    for (
      i=chordSlides.begin ();
      i!=chordSlides.end ();
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
  const std::list<S_msrDalSegno>&
    chordDalSegnos =
      chord->getChordDalSegnos ();

  if (chordDalSegnos.size ()) {
    std::list<S_msrDalSegno>::const_iterator i;
    for (i=chordDalSegnos.begin (); i!=chordDalSegnos.end (); ++i) {
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
    generateCodeForOctaveShiftAfterNote (
      chordOctaveShift);
  }
}

void lpsr2lilypondTranslator::generateChordInGraceNotesGroup (const S_msrChord& chord)
{
  generateCodeRightBeforeChordContents (chord);

  // generate the start of the chord
  fLilypondCodeStream <<
    "<";

  generateCodeForChordInGraceNotesGroupContents (chord);

  // generate the end of the chord
  fLilypondCodeStream <<
    ">";

  generateCodeRightAfterChordContents (chord);
}

//________________________________________________________________________
void lpsr2lilypondTranslator::visitStart (S_msrChord& elt)
{
#ifdef TRACING_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gGlobalLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream s;

      s <<
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
        gLogStream << s.str ();
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << s.str ();
      }
    }
  }
#endif

#ifdef TRACING_IS_ENABLED
  if (fOnGoingGraceNotesGroup) {
    int chordInputLineNumber =
      elt->getInputLineNumber ();

    if (gGlobalTracingOahGroup->getTraceNotes ()) {
        std::stringstream s;

        s <<
          "% ==> Start visiting grace chords is ignored inside grace notes groups " <<
          elt->asShortString ();

      lpsr2lilypondInternalWarning (
        gGlobalServiceRunData->getInputSourceName (),
        chordInputLineNumber,
        s.str ());
    }

    return;
  }
#endif

  fOnGoingChord = true;
  fCurrentChord = elt;

  generateCodeAheadOfChordContents (elt);

  generateCodeRightBeforeChordContents (elt);

  // generate the start of the chord
  if (gGlobalLpsr2lilypondOahGroup->getInputLineNumbers ()) {
    fLilypondCodeStream <<
      " %{ line " << elt->getInputLineNumber () << " %} " <<
      std::endl;
  }

  fLilypondCodeStream <<
    "<";
}

void lpsr2lilypondTranslator::visitEnd (S_msrChord& elt)
{
#ifdef TRACING_IS_ENABLED
  int inputLineNumber =
    elt->getInputLineNumber ();

  {
    Bool
      traceLpsrVisitors =
        gGlobalLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream s;

      s <<
        "% --> End visiting msrChord " <<
        elt->asShortString () <<
        ", line " << inputLineNumber <<
        std::endl;

      if (traceLpsrVisitors) {
        gLogStream << s.str ();
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << s.str ();
      }
    }
  }

  if (fOnGoingGraceNotesGroup) {
    if (gGlobalTracingOahGroup->getTraceNotes ()) {
      lpsr2lilypondInternalWarning (
        gGlobalServiceRunData->getInputSourceName (),
        inputLineNumber,
        "% ==> End visiting grace chords is ignored inside grace notes groups");
    }

    return;
  }
#endif

  // generate the end of the chord
  fLilypondCodeStream <<
    ">";

  generateCodeRightAfterChordContents (elt);

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

  fOnGoingChord = false;
  fCurrentChord = nullptr;
}

//________________________________________________________________________
void lpsr2lilypondTranslator::visitStart (S_msrTuplet& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gGlobalLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream s;

      s <<
        "% --> Start visiting msrTuplet" <<
        elt->asShortString () <<
        ", fOnGoingChord: " <<
        fOnGoingChord <<
        ", fOnGoingGraceNotesGroup: " <<
        fOnGoingGraceNotesGroup <<
        ", fOnGoingChordGraceNotesGroupLink: " <<
        fOnGoingChordGraceNotesGroupLink <<
        ", line " << inputLineNumber <<
        std::endl;

      if (traceLpsrVisitors) {
        gLogStream << s.str ();
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << s.str ();
      }
    }
  }
#endif

  if (fOnGoingTupletsStack.size ()) {
    // elt is a nested tuplet

    S_msrTuplet
      containingTuplet =
        fOnGoingTupletsStack.top ();

    // unapply containing tuplet factor,
    // i.e 3/2 inside 5/4 becomes 15/8 in MusicXML...
    elt->
      unapplySoundingFactorToTupletMembers (
        containingTuplet->
          getTupletFactor ());
  }

  if (gGlobalLpsr2lilypondOahGroup->getIndentTuplets ()) {
    fLilypondCodeStream << std::endl;
  }

  if (gGlobalLpsr2lilypondOahGroup->getInputLineNumbers ()) {
    fLilypondCodeStream <<
      " %{ line " << inputLineNumber << " %} " <<
      std::endl;
  }

  // get bracket kind
  msrTupletBracketKind
    tupletBracketKind =
      elt->getTupletBracketKind ();

  switch (tupletBracketKind) {
    case msrTupletBracketKind::kTupletBracketYes:
    /* JMI
      fLilypondCodeStream <<
        "%{kTupletBracketYes%}" <<
        std::endl;
        */
      break;
    case msrTupletBracketKind::kTupletBracketNo:
      fLilypondCodeStream <<
        "\\once \\omit TupletBracket" <<
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
      fLilypondCodeStream <<
        "%{tupletShowNumberActual%}" <<
        std::endl;
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

  // get show type kind
  msrTupletShowTypeKind
    tupletShowTypeKind =
      elt->getTupletShowTypeKind ();

  Rational
    memberNoteDisplayWholeNotes =
      elt->getMemberNotesDisplayWholeNotes ();

  switch (tupletShowTypeKind) {
    case msrTupletShowTypeKind::kTupletShowTypeActual:
      fLilypondCodeStream <<
     // JMI ???   "\\once \\override TupletNumber.text = #(tuplet-number::append-note-wrapper tuplet-number::calc-fraction-text \"" <<
        "\\once \\override TupletNumber.text = #(tuplet-number::append-note-wrapper tuplet-number::calc-denominator-text \"" <<
        wholeNotesAsLilypondString (
          inputLineNumber,
          memberNoteDisplayWholeNotes) <<
        "\")" <<
        std::endl;
      break;
    case msrTupletShowTypeKind::kTupletShowTypeBoth:
      fLilypondCodeStream <<
        "\\once \\override TupletNumber.text = #(tuplet-number::fraction-with-notes" <<
        " #{ " <<
        wholeNotesAsLilypondString (
          inputLineNumber,
          memberNoteDisplayWholeNotes) <<
        " #} #{ " <<
        wholeNotesAsLilypondString (
          inputLineNumber,
          memberNoteDisplayWholeNotes) <<
        " #})" <<
        std::endl;
      break;
    case msrTupletShowTypeKind::kTupletShowTypeNone:
    /* JMI
      fLilypondCodeStream <<
        "%{tupletShowTypeNone%}" <<
        std::endl;
        */
      break;
  } // switch

  fLilypondCodeStream <<
    "\\tuplet " <<
	  tupletFactorAsLilypondString (elt->getTupletFactor ()) <<
    " {" <<
    std::endl;

  // push the tuplet on the tuplets std::stack
  fOnGoingTupletsStack.push (elt);

  ++gIndenter; // decremented in visitEnd (S_msrTuplet& elt)

  // force durations to be displayed explicitly
  // at the beginning of the tuplet
  fLastMetWholeNotes = Rational (0, 1);
}

void lpsr2lilypondTranslator::visitEnd (S_msrTuplet& elt)
{
#ifdef TRACING_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gGlobalLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream s;

      s <<
        "% --> End visiting msrTuplet" <<
        ", line " << elt->getInputLineNumber () <<
        std::endl;

      if (traceLpsrVisitors) {
        gLogStream << s.str ();
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << s.str ();
      }
    }
  }
#endif

  --gIndenter; // incremented in visitStart (S_msrTuplet& elt)

  if (gGlobalLpsr2lilypondOahGroup->getIndentTuplets ()) {
    fLilypondCodeStream << std::endl;
  }

  fLilypondCodeStream <<
    '}' <<
    std::endl;

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

  // pop the tuplet from the tuplets std::stack
  fOnGoingTupletsStack.pop ();

/* JMI
 ?????? fCurrentOctaveEntryReference = nullptr;
  // the first note after the tuplet will become the new reference
  */
}

//________________________________________________________________________
void lpsr2lilypondTranslator::visitStart (S_msrTie& elt)
{
#ifdef TRACING_IS_ENABLED
  int inputLineNumber =
    elt->getInputLineNumber ();

  {
    Bool
      traceLpsrVisitors =
        gGlobalLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream s;

      s <<
        "% --> Start visiting msrTie" <<
        ", line " << inputLineNumber <<
        std::endl;

      if (traceLpsrVisitors) {
        gLogStream << s.str ();
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << s.str ();
      }
    }
  }
#endif

  switch (elt->getTieKind ()) {
    case msrTieKind::kTieNone:
      break;
    case msrTieKind::kTieStart:
      if (fOnGoingNotesStack.size () > 0) {
        // this precludes generating for the chords' ties,
        // since the last of its notes sets fOnGoingNotesStack.size > 0 to false
        // after code has been generated for it
        fLilypondCodeStream <<
  // JMI        "%{line " << inputLineNumber << "%}" <<
          " ~ ";
      }
      break;
    case msrTieKind::kTieContinue:
      break;
    case msrTieKind::kTieStop:
      break;
   } // switch
}

void lpsr2lilypondTranslator::visitEnd (S_msrTie& elt)
{
#ifdef TRACING_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gGlobalLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream s;

      s <<
        "% --> End visiting msrTie" <<
        ", line " << elt->getInputLineNumber () <<
        std::endl;

      if (traceLpsrVisitors) {
        gLogStream << s.str ();
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << s.str ();
      }
    }
  }
#endif
}

//________________________________________________________________________
void lpsr2lilypondTranslator::visitStart (S_msrSegno& elt)
{
#ifdef TRACING_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gGlobalLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream s;

      s <<
        "% --> Start visiting msrSegno" <<
        ", line " << elt->getInputLineNumber () <<
        std::endl;

      if (traceLpsrVisitors) {
        gLogStream << s.str ();
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << s.str ();
      }
    }
  }
#endif
}

void lpsr2lilypondTranslator::visitStart (S_msrHiddenMeasureAndBarLine& elt)
{
#ifdef TRACING_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gGlobalLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream s;

      s <<
        "% --> Start visiting msrHiddenMeasureAndBarLine" <<
        ", line " << elt->getInputLineNumber () <<
        std::endl;

      if (traceLpsrVisitors) {
        gLogStream << s.str ();
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << s.str ();
      }
    }
  }
#endif

  fLilypondCodeStream <<
    "\\HiddenMeasureAndBarLine " <<
    "\\time 4/4";

  if (gGlobalLpsr2lilypondOahGroup->getInputLineNumbers ()) {
    fLilypondCodeStream <<
      " %{ " << elt->getInputLineNumber () << " %}";
  }

  fLilypondCodeStream << std::endl;
}

void lpsr2lilypondTranslator::visitStart (S_msrCoda& elt)
{
#ifdef TRACING_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gGlobalLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream s;

      s <<
        "% --> Start visiting msrCoda" <<
        ", line " << elt->getInputLineNumber () <<
        std::endl;

      if (traceLpsrVisitors) {
        gLogStream << s.str ();
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << s.str ();
      }
    }
  }
#endif
}

//________________________________________________________________________
void lpsr2lilypondTranslator::visitStart (S_msrEyeGlasses& elt)
{
#ifdef TRACING_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gGlobalLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream s;

      s <<
        "% --> Start visiting eyeGlasses" <<
        ", line " << elt->getInputLineNumber () <<
        std::endl;

      if (traceLpsrVisitors) {
        gLogStream << s.str ();
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << s.str ();
      }
    }
  }
#endif

  fLilypondCodeStream <<
    "^\\markup {\\eyeglasses} ";
}

void lpsr2lilypondTranslator::visitStart (S_msrScordatura& elt)
{
#ifdef TRACING_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gGlobalLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream s;

      s <<
        "% --> Start visiting scordatura" <<
        ", line " << elt->getInputLineNumber () <<
        std::endl;

      if (traceLpsrVisitors) {
        gLogStream << s.str ();
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << s.str ();
      }
    }
  }
#endif

/* JMI
  const std::list<S_msrStringTuning>&
    scordaturaStringTuningsList =
      elt->getScordaturaStringTuningsList ();

  ++gIndenter;

  fLilypondCodeStream <<
    "\\new Staff {" <<
    std::endl;

  ++gIndenter;

  fLilypondCodeStream <<
    "\\hide Staff.Stem" <<
    std::endl <<
    "\\hide Staff.TimeSignature" <<
    std::endl <<
    "\\small" <<
    std::endl <<
    "<";

  if (scordaturaStringTuningsList.size ()) {
    std::list<S_msrStringTuning>::const_iterator
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

      fLilypondCodeStream << ' ';
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
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gGlobalLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream s;

      s <<
        "% --> Start visiting pedal" <<
        ", line " << inputLineNumber <<
        std::endl;

      if (traceLpsrVisitors) {
        gLogStream << s.str ();
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << s.str ();
      }
    }
  }
#endif

  fLilypondCodeStream << std::endl;

  switch (elt->getPedalTypeKind ()) {
    case msrPedalTypeKind::kPedalType_NO_:
      {
        // should not occur
        std::stringstream s;

        s <<
          "msrPedal '" <<
          elt->asShortString () <<
          "' has no pedal type";

        lpsr2lilypondInternalError (
          gGlobalServiceRunData->getInputSourceName (),
          inputLineNumber,
          __FILE__, __LINE__,
          s.str ());
      }
      break;
    case msrPedalTypeKind::kPedalTypeStart:
      fLilypondCodeStream <<
        "\\sustainOn";
      break;
    case msrPedalTypeKind::kPedalTypeContinue:
      fLilypondCodeStream <<
        "\\sustainOff\\sustainOn"; // JMI
      break;
    case msrPedalTypeKind::kPedalTypeChange:
      fLilypondCodeStream <<
        "\\sustainOff\\sustainOn";
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
#ifdef TRACING_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gGlobalLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream s;

      s <<
        "% --> Start visiting damp" <<
        ", line " << elt->getInputLineNumber () <<
        std::endl;

      if (traceLpsrVisitors) {
        gLogStream << s.str ();
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << s.str ();
      }
    }
  }
#endif

  fLilypondCodeStream <<
    "^\\markup {\\damp} ";
}

void lpsr2lilypondTranslator::visitStart (S_msrDampAll& elt)
{
#ifdef TRACING_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gGlobalLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream s;

      s <<
        "% --> Start visiting dampAll" <<
        ", line " << elt->getInputLineNumber () <<
        std::endl;

      if (traceLpsrVisitors) {
        gLogStream << s.str ();
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << s.str ();
      }
    }
  }
#endif

  fLilypondCodeStream <<
    "^\\markup {\\dampAll} ";
}

//________________________________________________________________________
void lpsr2lilypondTranslator::visitStart (S_msrBarLine& elt)
{
#ifdef TRACING_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gGlobalLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream s;

      s <<
        std::endl <<
        "% --> Start visiting msrBarLine" <<
        ", line " << elt->getInputLineNumber () <<
        std::endl;

      if (traceLpsrVisitors) {
        gLogStream << s.str ();
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << s.str ();
      }
    }
  }
#endif

  int inputLineNumber =
    elt->getInputLineNumber ();

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

      if (gGlobalLpsr2lilypondOahGroup->getInputLineNumbers ()) {
        // generate the barLine line number as a comment
        fLilypondCodeStream <<
          " %{ line " << inputLineNumber << " %} ";
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
              " %{ line " << note->getInputLineNumber () << " %} ";
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
          if (fRepeatDescrsStack.size ()) {
            S_msrRepeat
              currentRepeat =
                fRepeatDescrsStack.back ()->getRepeat (),
              precedingRepeat =
                currentRepeat->getImmediatelyPrecedingRepeat ();

            if (precedingRepeat && ! precedingRepeat-> getRepeatEndings ().size ()) {
              // JMI
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
          if (fRepeatDescrsStack.size ()) {
            S_msrRepeat
              currentRepeat =
                fRepeatDescrsStack.back ()->getRepeat (),
              followingRepeat =
                currentRepeat->getImmediatelyPrecedingRepeat ();

      // JMI      if (followingRepeat && ! followingRepeat-> getRepeatEndings ().size ()) {
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

    case msrBarLineCategoryKind::kBarLineCategory_NO_:
      {
        std::stringstream s;

        s <<
          "barLine category has not been set" <<
          ", line " << elt->getInputLineNumber ();

  // JMI      lpsr2lilypondInternalError (
        lpsr2lilypondInternalWarning (
          gGlobalServiceRunData->getInputSourceName (),
          inputLineNumber,
  // JMI        __FILE__, __LINE__,
          s.str ());
      }
      break;
  } // switch

  if (gGlobalLpsr2lilypondOahGroup->getInputLineNumbers ()) {
    fLilypondCodeStream <<
      " %{ line " << inputLineNumber << " %}";
  }
}

void lpsr2lilypondTranslator::visitEnd (S_msrBarLine& elt)
{
#ifdef TRACING_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gGlobalLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream s;

      s <<
        std::endl <<
        "% --> End visiting msrBarLine" <<
        ", line " << elt->getInputLineNumber () <<
        std::endl;

      if (traceLpsrVisitors) {
        gLogStream << s.str ();
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << s.str ();
      }
    }
  }
#endif
}

//________________________________________________________________________
void lpsr2lilypondTranslator::visitStart (S_msrBarCheck& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gGlobalLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream s;

      s <<
        "% --> Start visiting msrBarCheck" <<
        ", nextBarNumber: " <<
        elt->getNextBarPuristNumber () <<
        ", line " << elt->getInputLineNumber () <<
        std::endl;

      if (traceLpsrVisitors) {
        gLogStream << s.str ();
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << s.str ();
      }
    }
  }
#endif

  int nextBarPuristNumber =
    elt->getNextBarPuristNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMultipleFullBarRests ()) {
    fLilypondCodeStream <<
      "% nextBarPuristNumber: " <<
      nextBarPuristNumber <<
      ", fOnGoingMultipleFullBarRests: " <<
      fOnGoingMultipleFullBarRests <<
      "fOnGoingVoiceCadenza: " <<
      fOnGoingVoiceCadenza <<
      ", nextBarPuristNumber: " <<
      nextBarPuristNumber <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  if (
    !
      (
        fOnGoingVoiceCadenza
            // should be tested in msr2lpsrTranslator.cpp JMI visitEnd (S_msrMeasure&)
            // MusicXML bar numbers cannot be relied upon for a LilyPond bar number check
          ||
        fOnGoingMultipleFullBarRests
      )
  ) {
    // don't generate a bar check before the end of measure 1 // JMI ???
    fLilypondCodeStream <<
      "| % " << nextBarPuristNumber;

    if (gGlobalLpsr2lilypondOahGroup->getInputLineNumbers ()) {
      // generate the line number as a comment
      fLilypondCodeStream <<
        " %{ line " << inputLineNumber << " %} ";
    }

    if (gGlobalLpsr2lilypondOahGroup->getOriginalMeasureNumbers ()) {
      // generate the original MusicXML measure number as a comment
      fLilypondCodeStream <<
        " % omn: " << elt->getNextBarOriginalNumber () << " %}";
    }

    fLilypondCodeStream <<
      std::endl;
  }
}

void lpsr2lilypondTranslator::visitEnd (S_msrBarCheck& elt)
{
#ifdef TRACING_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gGlobalLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream s;

      s <<
        "% --> End visiting msrBarCheck" <<
        ", line " << elt->getInputLineNumber () <<
        std::endl;

      if (traceLpsrVisitors) {
        gLogStream << s.str ();
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << s.str ();
      }
    }
  }
#endif
}

//________________________________________________________________________
void lpsr2lilypondTranslator::visitStart (S_msrBarNumberCheck& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gGlobalLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream s;

      s <<
        "% --> Start visiting msrBarNumberCheck" <<
        ", line " << inputLineNumber <<
        std::endl;

      if (traceLpsrVisitors) {
        gLogStream << s.str ();
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << s.str ();
      }
    }
  }
#endif

  if (gGlobalLpsr2lilypondOahGroup->getNoBarNumberChecks ()) {
    return;
  }

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMultipleFullBarRests ()) {
    fLilypondCodeStream <<
      "%, fOnGoingMultipleFullBarRests: " <<
      fOnGoingMultipleFullBarRests <<
      "% fOnGoingVoiceCadenza: " <<
      fOnGoingVoiceCadenza <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  if (
    !
      (
        fOnGoingVoiceCadenza
            // should be tested in msr2lpsrTranslator.cpp JMI visitEnd (S_msrMeasure&)
           // MusicXML bar numbers cannot be relied upon for a LilyPond bar number check
          ||
        fOnGoingMultipleFullBarRests
      )
  ) {
    int nextBarPuristNumber =
      elt->getNextBarPuristNumber ();

    fLilypondCodeStream <<
      "\\barNumberCheck #" <<
      nextBarPuristNumber;

    if (gGlobalLpsr2lilypondOahGroup->getInputLineNumbers ()) {
      // generate the line number as a comment
      fLilypondCodeStream <<
        " %{ line " << inputLineNumber << " %} ";
    }

    if (gGlobalLpsr2lilypondOahGroup->getOriginalMeasureNumbers ()) {
      fLilypondCodeStream <<
        " %{ nbon: " <<
        elt->getNextBarOriginalNumber () <<
        "%}";
    }

    fLilypondCodeStream <<
      std::endl;
  }
}

void lpsr2lilypondTranslator::visitEnd (S_msrBarNumberCheck& elt)
{
#ifdef TRACING_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gGlobalLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream s;

      s <<
        "% --> End visiting msrBarNumberCheck" <<
        ", line " << elt->getInputLineNumber () <<
        std::endl;

      if (traceLpsrVisitors) {
        gLogStream << s.str ();
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << s.str ();
      }
    }
  }
#endif
}

//________________________________________________________________________
void lpsr2lilypondTranslator::visitStart (S_msrLineBreak& elt)
{
#ifdef TRACING_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gGlobalLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream s;

      s <<
        "% --> Start visiting msrLineBreak" <<
        ", line " << elt->getInputLineNumber () <<
        std::endl;

      if (traceLpsrVisitors) {
        gLogStream << s.str ();
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << s.str ();
      }
    }
  }
#endif

  // enforce a page break here, not \myBreak // JMI
  fLilypondCodeStream <<
    "\\break | % " << elt->getNextBarNumber ();

  if (gGlobalLpsr2lilypondOahGroup->getInputLineNumbers ()) {
    // generate the line number as a comment
    fLilypondCodeStream <<
      " %{ line " << elt->getNextBarNumber () << " %} ";
  }

  if (gGlobalLpsr2lilypondOahGroup->getOriginalMeasureNumbers ()) {
    // generate the original MusicXML measure number as a comment
    fLilypondCodeStream <<
      " (omn: " << elt->getInputLineNumber () << ")";
  }

  // enforce a line break here, not using \myLineBreak,
  // if this is a user chosen line break
  switch (elt->getUserChosenLineBreakKind ()) {
    case msrUserChosenLineBreakKind::kUserChosenLineBreakYes:
      fLilypondCodeStream <<
        "\\lineBreak ";
      break;
    case msrUserChosenLineBreakKind::kUserChosenLineBreakNo:
      fLilypondCodeStream <<
        "\\myLineBreak ";
      break;
  } // switch

  if (gGlobalLpsr2lilypondOahGroup->getInputLineNumbers ()) {
    // generate the line number as a comment
    fLilypondCodeStream <<
      " %{ line " << elt->getInputLineNumber () << " %} ";
  }

  fLilypondCodeStream <<
    std::endl << std::endl;
}

void lpsr2lilypondTranslator::visitEnd (S_msrLineBreak& elt)
{
#ifdef TRACING_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gGlobalLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream s;

      s <<
        "% --> End visiting msrLineBreak" <<
        ", line " << elt->getInputLineNumber () <<
        std::endl;

      if (traceLpsrVisitors) {
        gLogStream << s.str ();
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << s.str ();
      }
    }
  }
#endif
}

//________________________________________________________________________
void lpsr2lilypondTranslator::visitStart (S_msrPageBreak& elt)
{
#ifdef TRACING_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gGlobalLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream s;

      s <<
        "% --> Start visiting msrPageBreak" <<
        ", line " << elt->getInputLineNumber () <<
        std::endl;

      if (traceLpsrVisitors) {
        gLogStream << s.str ();
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << s.str ();
      }
    }
  }
#endif

  // enforce a page break here, not using \myPageBreak,
  // if this is a user chosen page break
  switch (elt->getUserChosenPageBreakKind ()) {
    case msrUserChosenPageBreakKind::kUserChosenPageBreakYes:
      fLilypondCodeStream <<
        "\\pageBreak ";
      break;
    case msrUserChosenPageBreakKind::kUserChosenPageBreakNo:
      fLilypondCodeStream <<
        "\\myPageBreak ";
      break;
  } // switch

  if (gGlobalLpsr2lilypondOahGroup->getInputLineNumbers ()) {
    // generate the line number as a comment
    fLilypondCodeStream <<
      " %{ line " << elt->getInputLineNumber () << " %} ";
  }

  fLilypondCodeStream <<
    std::endl << std::endl;
}

void lpsr2lilypondTranslator::visitEnd (S_msrPageBreak& elt)
{
#ifdef TRACING_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gGlobalLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream s;

      s <<
        "% --> End visiting msrPageBreak" <<
        ", line " << elt->getInputLineNumber () <<
        std::endl;

      if (traceLpsrVisitors) {
        gLogStream << s.str ();
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << s.str ();
      }
    }
  }
#endif
}

//________________________________________________________________________
void lpsr2lilypondTranslator::visitStart (S_msrRepeat& elt)
{
#ifdef TRACING_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gGlobalLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream s;

      s <<
        "% --> Start visiting msrRepeat" <<
        ", line " << elt->getInputLineNumber () <<
        std::endl;

      if (traceLpsrVisitors) {
        gLogStream << s.str ();
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << s.str ();
      }
    }
  }
#endif

  int repeatEndingsNumber =
    elt->getRepeatEndings ().size ();

  if (repeatEndingsNumber == 0)
    repeatEndingsNumber = 2; // implicitly JMI ???

  fRepeatDescrsStack.push_back (
    lpsrRepeatDescr::create (
      elt,
      repeatEndingsNumber));

  int
    repeatTimes =
      elt->getRepeatTimes ();

  std::stringstream s;

  fLilypondCodeStream << std::endl;

  s <<
    "\\repeat volta " <<
    repeatTimes <<
// JMI    fRepeatDescrsStack.back ()->getRepeatEndingsNumber () <<
    " {";

  if (gGlobalLpsr2lilypondOahGroup->getLilypondComments ()) {
    fLilypondCodeStream << std::left <<
      std::setw (commentFieldWidth) <<
      s.str () << "% start of repeat";
  }
  else {
    fLilypondCodeStream <<
      s.str ();
  }

  fLilypondCodeStream << std::endl;

  ++gIndenter; // decremented in visitEnd (S_msrRepeat& elt)

  if (repeatTimes > 2) {
    fLilypondCodeStream <<
      "<>^\"" << repeatTimes << " times\"" << // JMI
      std::endl;
  }
}

void lpsr2lilypondTranslator::visitEnd (S_msrRepeat& elt)
{
#ifdef TRACING_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gGlobalLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream s;

      s <<
        "% --> End visiting msrRepeat" <<
        ", line " << elt->getInputLineNumber () <<
        std::endl;

      if (traceLpsrVisitors) {
        gLogStream << s.str ();
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << s.str ();
      }
    }
  }
#endif

  /*
    CAUTION:
      the end of the repeat has to be generated
      before the endings are handled
  */

  if (! fRepeatDescrsStack.back ()->getEndOfRepeatHasBeenGenerated ()) {
    // the end of the repeat has not been generated yet JMI

    --gIndenter;

    if (gGlobalLpsr2lilypondOahGroup->getLilypondComments ()) {
      fLilypondCodeStream << std::left <<
        std::setw (commentFieldWidth) <<
        '}' << "% end of repeat" <<
        std::endl;
    }
    else {
      fLilypondCodeStream <<
        std::endl <<
        '}' <<
        std::endl << std::endl;
    }
  }

  //   --gIndenter; // incremented in visitStart (S_msrRepeat& elt) JMI ???

  fRepeatDescrsStack.pop_back ();
}

//________________________________________________________________________
void lpsr2lilypondTranslator::visitStart (S_msrRepeatCommonPart& elt)
{
#ifdef TRACING_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gGlobalLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream s;

      s <<
        "% --> Start visiting msrRepeatCommonPart" <<
        ", line " << elt->getInputLineNumber () <<
        std::endl;

      if (traceLpsrVisitors) {
        gLogStream << s.str ();
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << s.str ();
      }
    }
  }
#endif
}

void lpsr2lilypondTranslator::visitEnd (S_msrRepeatCommonPart& elt)
{
#ifdef TRACING_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gGlobalLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream s;

      s <<
        "% --> Start visiting msrRepeatCommonPart" <<
        ", line " << elt->getInputLineNumber () <<
        std::endl;

      if (traceLpsrVisitors) {
        gLogStream << s.str ();
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << s.str ();
      }
    }
  }
#endif
}

//________________________________________________________________________
void lpsr2lilypondTranslator::visitStart (S_msrRepeatEnding& elt)
{
#ifdef TRACING_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gGlobalLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream s;

      s <<
        "% --> Start visiting msrRepeatEnding" <<
        ", line " << elt->getInputLineNumber () <<
        std::endl;

      if (traceLpsrVisitors) {
        gLogStream << s.str ();
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << s.str ();
      }
    }
  }
#endif

  fRepeatDescrsStack.back ()->
    incrementRepeatEndingsCounter ();

  int
    repeatEndingInternalNumber =
      elt->getRepeatEndingInternalNumber ();

  if (repeatEndingInternalNumber == 1) {
    --gIndenter;

    // first repeat ending is in charge of
    // outputting the end of the repeat
    if (gGlobalLpsr2lilypondOahGroup->getLilypondComments ()) {
      fLilypondCodeStream <<
        std::setw (commentFieldWidth) << std::left <<
        '}' << "% end of repeat" <<
        std::endl;
    }
    else {
      fLilypondCodeStream <<
        std::endl <<
        '}' <<
        std::endl;
    }
    fRepeatDescrsStack.back ()->
      setEndOfRepeatHasBeenGenerated ();

    // first repeat ending is in charge of
    // outputting the start of the alternative
    if (gGlobalLpsr2lilypondOahGroup->getLilypondComments ()) {
      fLilypondCodeStream << std::left <<
        std::endl <<
        std::setw (commentFieldWidth) <<
        "\\alternative {" <<
        "% start of alternative" <<
        std::endl;
    }
    else {
      fLilypondCodeStream <<
        std::endl <<
        "\\alternative {" <<
        std::endl;
    }

    ++gIndenter;
  }

  // output the start of the ending
  switch (elt->getRepeatEndingKind ()) {
    case msrRepeatEndingKind::kRepeatEndingHooked:
      if (gGlobalLpsr2lilypondOahGroup->getLilypondComments ()) {
        fLilypondCodeStream << std::left <<
          std::setw (commentFieldWidth) <<
          "{" << "% start of repeat hooked ending" <<
          std::endl;
      }
      else {
        fLilypondCodeStream <<
          "{" <<
          std::endl;
      }
      break;

    case msrRepeatEndingKind::kRepeatEndingHookless:
      if (gGlobalLpsr2lilypondOahGroup->getLilypondComments ()) {
        fLilypondCodeStream << std::left <<
          std::setw (commentFieldWidth) <<
          "{" << "% start of repeat hookless ending" <<
          std::endl;
      }
      else {
        fLilypondCodeStream <<
          "{" <<
          std::endl;
      }
      break;
  } // switch

  ++gIndenter; // incremented in visitEnd (S_msrRepeatEnding&)

  // generate the repeat ending number if any
  std::string
    repeatEndingNumber =
      elt->getRepeatEndingNumber ();

  if (repeatEndingNumber.size ()) {
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
#ifdef TRACING_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gGlobalLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream s;

      s <<
        "% --> End visiting msrRepeatEnding" <<
        ", line " << elt->getInputLineNumber () <<
        std::endl;

      if (traceLpsrVisitors) {
        gLogStream << s.str ();
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << s.str ();
      }
    }
  }
#endif

  --gIndenter;  // incremented in visitStart (S_msrRepeatEnding&)

  // output the end of the ending
  fLilypondCodeStream << std::endl;

  switch (elt->getRepeatEndingKind ()) {
    case msrRepeatEndingKind::kRepeatEndingHooked:
      if (gGlobalLpsr2lilypondOahGroup->getLilypondComments ()) {
        fLilypondCodeStream << std::left <<
          std::setw (commentFieldWidth) <<
          '}' << "% end of repeat hooked ending" <<
          std::endl;
      }
      else {
        fLilypondCodeStream <<
          '}' <<
          std::endl;
      }
      break;

    case msrRepeatEndingKind::kRepeatEndingHookless:
      if (gGlobalLpsr2lilypondOahGroup->getLilypondComments ())   {
        fLilypondCodeStream << std::left <<
          std::setw (commentFieldWidth) <<
          '}' << "% end of repeat hookless ending" <<
          std::endl;
      }
      else {
        fLilypondCodeStream <<
          '}' <<
          std::endl;
      }
      break;
  } // switch

/* JMI
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeats ()) {
    fLilypondCodeStream <<
      "% ===**** fRepeatDescrsStack.back () = '" <<
      fRepeatDescrsStack.back ()->asString () <<
      "'" <<
      std::endl;

      if (traceLpsrVisitors) {
        gLogStream << s.str ();
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << s.str ();
      }
    }
  }
#endif
*/

  if (
    fRepeatDescrsStack.back ()->getRepeatEndingsCounter ()
      ==
    fRepeatDescrsStack.back ()->getRepeatEndingsNumber ()) {

//    --gIn denter; // JMI ???

    // last repeat ending is in charge of
    // outputting the end of the alternative
    if (gGlobalLpsr2lilypondOahGroup->getLilypondComments ())   {
      fLilypondCodeStream << std::left <<
        std::setw (commentFieldWidth) <<
        '}' << "% end of alternative" <<
        std::endl;
    }
    else {
      fLilypondCodeStream <<
        '}' <<
        std::endl << std::endl;
    }
  }
}

//________________________________________________________________________
void lpsr2lilypondTranslator::visitStart (S_msrRehearsalMark& elt)
{
#ifdef TRACING_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gGlobalLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream s;

      s <<
        "% --> Start visiting msrRehearsalMark" <<
        ", line " << elt->getInputLineNumber () <<
        std::endl;

      if (traceLpsrVisitors) {
        gLogStream << s.str ();
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << s.str ();
      }
    }
  }
#endif

  fLilypondCodeStream << std::endl;

  switch (elt->getRehearsalMarkPlacementKind ()) {
    case msrPlacementKind::kPlacement_NO_:
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

  switch (elt->getRehearsalMarkKind ()) {
    case msrRehearsalMarkKind::kRehearsalMarkNone:
      fLilypondCodeStream <<
        "\\box"; // default value
      break;
    case msrRehearsalMarkKind::kRehearsalMarkRectangle:
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
        "%{ \\triangle??? %}";
      break;
    case msrRehearsalMarkKind::kRehearsalMarkDiamond:
      fLilypondCodeStream <<
        "%{ \\diamond??? %}";
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
#ifdef TRACING_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gGlobalLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream s;

      s <<
        "% --> End visiting msrRehearsalMark" <<
        ", line " << elt->getInputLineNumber () <<
        std::endl;

      if (traceLpsrVisitors) {
        gLogStream << s.str ();
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << s.str ();
      }
    }
  }
#endif
}

//________________________________________________________________________
void lpsr2lilypondTranslator::visitStart (S_msrMeasureRepeat& elt)
{
#ifdef TRACING_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gGlobalLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream s;

      s <<
        "% --> Start visiting msrMeasureRepeat" <<
        ", line " << elt->getInputLineNumber () <<
        std::endl;

      if (traceLpsrVisitors) {
        gLogStream << s.str ();
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << s.str ();
      }
    }
  }
#endif

#ifdef TRACING_IS_ENABLED
  int repeatReplicasMeasuresNumber =
    elt->fetchMeasureRepeatReplicasMeasuresNumber ();
#endif

  int replicasNumber =
    elt->fetchMeasureRepeatReplicasNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasureRepeats ()) {
    int repeatPatternMeasuresNumber =
      elt->fetchMeasureRepeatPatternMeasuresNumber ();

    fLilypondCodeStream <<
      "% measures repeat, line " << elt->getInputLineNumber () << ":" <<
      std::endl;

    int fieldWidth = 24;

    fLilypondCodeStream << std::left <<
      std::setw (fieldWidth) <<
      "% repeatPatternMeasuresNumber" << " = " << repeatPatternMeasuresNumber <<
      std::endl <<
      std::setw (fieldWidth) <<
      "% repeatReplicasMeasuresNumber" << " = " << repeatReplicasMeasuresNumber <<
      std::endl <<
      std::setw (fieldWidth) <<
      "% replicasNumber" << " = " << replicasNumber <<
      std::endl;
  }
#endif

  if (gGlobalLpsr2lilypondOahGroup->getLilypondComments ()) {
    fLilypondCodeStream << std::left <<
      std::setw (commentFieldWidth) <<
      "% start of measures repeat" <<
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
#ifdef TRACING_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gGlobalLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream s;

      s <<
        "% --> End visiting msrMeasureRepeat" <<
        ", line " << elt->getInputLineNumber () <<
        std::endl;

      if (traceLpsrVisitors) {
        gLogStream << s.str ();
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << s.str ();
      }
    }
  }
#endif

  --gIndenter; // incremented in visitStart (S_msrMeasureRepeat&)

  fLilypondCodeStream <<
    std::endl << std::endl <<
    " }" <<
    std::endl;

  if (gGlobalLpsr2lilypondOahGroup->getLilypondComments ()) {
    fLilypondCodeStream <<
      std::setw (commentFieldWidth) << std::left <<
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
#ifdef TRACING_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gGlobalLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream s;

      s <<
        "%--> Start visiting msrMeasureRepeatPattern" <<
        std::endl;

      if (traceLpsrVisitors) {
        gLogStream << s.str ();
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << s.str ();
      }
    }
  }
#endif
}

void lpsr2lilypondTranslator::visitEnd (S_msrMeasureRepeatPattern& elt)
{
#ifdef TRACING_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gGlobalLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream s;

      s <<
        "%--> End visiting msrMeasureRepeatPattern" <<
        std::endl;

      if (traceLpsrVisitors) {
        gLogStream << s.str ();
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << s.str ();
      }
    }
  }
#endif
}

void lpsr2lilypondTranslator::visitStart (S_msrMeasureRepeatReplicas& elt)
{
#ifdef TRACING_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gGlobalLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream s;

      s <<
        "%--> Start visiting msrMeasureRepeatReplicas" <<
        std::endl;

      if (traceLpsrVisitors) {
        gLogStream << s.str ();
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << s.str ();
      }
    }
  }
#endif

  // output the start of the ending
  if (gGlobalLpsr2lilypondOahGroup->getLilypondComments ()) {
    fLilypondCodeStream << std::left <<
      std::setw (commentFieldWidth) <<
      "{" << "% start of measures repeat replicas" <<
      std::endl;
  }
  else {
    fLilypondCodeStream <<
      std::endl <<
      "{" <<
      std::endl;
  }

  ++gIndenter; // decremented in visitEnd (S_msrMeasureRepeat&)
}

void lpsr2lilypondTranslator::visitEnd (S_msrMeasureRepeatReplicas& elt)
{
#ifdef TRACING_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gGlobalLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream s;

      s <<
        "%--> End visiting msrMeasureRepeatReplicas" <<
        std::endl;

      if (traceLpsrVisitors) {
        gLogStream << s.str ();
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << s.str ();
      }
    }
  }
#endif

  --gIndenter; // incremented in visitStart (S_msrMeasureRepeatReplicas&)
}

//________________________________________________________________________
void lpsr2lilypondTranslator::visitStart (S_msrMultipleFullBarRests& elt)
{
#ifdef TRACING_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gGlobalLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream s;

      s <<
        "% --> Start visiting msrMultipleFullBarRests" <<
        ", line " << elt->getInputLineNumber () <<
        std::endl;

      if (traceLpsrVisitors) {
        gLogStream << s.str ();
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << s.str ();
      }
    }
  }
#endif

  int inputLineNumber =
    elt->getInputLineNumber ();

  int multipleFullBarRestsNumber =
    elt->getMultipleFullBarRestsNumber ();

  if (gGlobalLpsr2lilypondOahGroup->getLilypondComments ()) {
    fLilypondCodeStream << std::left <<
      std::setw (commentFieldWidth) <<
      "% start of multiple full-bar rests" <<
      mfSingularOrPlural (
        multipleFullBarRestsNumber,
        "measure",
        "measures") <<
      ", line " << inputLineNumber <<
      std::endl << std::endl;

    ++gIndenter; // decremented in visitEnd (S_msrMultipleFullBarRests&)
  }

  if (
    gGlobalLpsr2lilypondOahGroup->
      getMultipleFullBarRestsExpandLimitAtom ()->getSetByAnOption ()
  ) {
    fLilypondCodeStream <<
      "\\override MultiMeasureRest.expand-limit = " <<
      gGlobalLpsr2lilypondOahGroup->getMultipleFullBarRestsExpandLimit () <<
      std::endl;
  }

  fLilypondCodeStream <<
    "\\compressMMRests { %{ CC %}" << // JMI v0.9.64
    std::endl;
  ++gIndenter;

  fOnGoingMultipleFullBarRests = true;
}

void lpsr2lilypondTranslator::visitEnd (S_msrMultipleFullBarRests& elt)
{
#ifdef TRACING_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gGlobalLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream s;

      s <<
        "% --> End visiting msrMultipleFullBarRests" <<
        ", line " << elt->getInputLineNumber () <<
        std::endl;

      if (traceLpsrVisitors) {
        gLogStream << s.str ();
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << s.str ();
      }
    }
  }
#endif

  int inputLineNumber =
    elt->getInputLineNumber ();

  int multipleFullBarRestsNumber =
    elt->getMultipleFullBarRestsNumber ();

  // start counting measures
  fRemainingMultipleFullBarRestsNumber =
    elt->getMultipleFullBarRestsNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMultipleFullBarRests ()) {
    gLogStream <<
      std::endl <<
      "--> fRemainingMultipleFullBarRestsNumber: " <<
      fRemainingMultipleFullBarRestsNumber <<
      std::endl;
  }
#endif

  // get multiple full-bar rests sounding notes JMI USELESS v0.9.63
  Rational
    multipleFullBarRestsMeasureSoundingNotes =
      elt->fetchMultipleFullBarRestsMeasureSoundingNotes ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMultipleFullBarRests ()) {
    gLogStream <<
      std::endl <<
      "--> multipleFullBarRestsMeasureSoundingNotes: " <<
      multipleFullBarRestsMeasureSoundingNotes <<
      std::endl;
  }
#endif

  // generate multiple full-bar rests only now, in case there are
  // clef, keys or times before them in the first measure
  std::string
    fullBarRestsWholeNoteAsLilypondString =
      multipleFullBarRestsWholeNoteAsLilypondString (
        inputLineNumber,
        multipleFullBarRestsMeasureSoundingNotes);

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMultipleFullBarRests ()) {
    gLogStream <<
      std::endl <<
      "--> fullBarRestsWholeNoteAsLilypondString: " <<
      fullBarRestsWholeNoteAsLilypondString <<
      std::endl;
  }
#endif

  fLilypondCodeStream <<
    "R" <<
    fullBarRestsWholeNoteAsLilypondString;

  if (gGlobalLpsr2lilypondOahGroup->getInputLineNumbers ()) {
    // generate information and line number as a comment
    fLilypondCodeStream <<
      " %{ line " <<
      elt->getInputLineNumber () <<
      " %}";
  }

  if (gGlobalLpsr2lilypondOahGroup->getNotesComments ()) {
    // generate information and line number as a comment
    fLilypondCodeStream <<
      "%{ multiple full-bar rest %}";
  }

  // wait until all measures have be visited
  // before the bar check is generated // JMI ???

  // now we can generate the bar check
  fLilypondCodeStream <<
    " | % " <<
    elt->getMultipleFullBarRestsLastMeasurePuristMeasureNumber () + 1;

/* TO BE FINALIZED JMI
    if (gGlobalLpsr2lilypondOahGroup->getOriginalMeasureNumbers ()) {
      // generate the original MusicXML measure number as a comment
      fLilypondCodeStream <<
        " (mxml3: " << measureElement->getInputLineNumber () << ")";
    }
*/

  fLilypondCodeStream <<
    std::endl;

  if (gGlobalLpsr2lilypondOahGroup->getLilypondComments ()) {
    --gIndenter; // incremented in visitStart (S_msrMultipleFullBarRests&)

    fLilypondCodeStream << std::left <<
      std::setw (commentFieldWidth) <<
      "% end of multiple full-bar rests" <<
      mfSingularOrPlural (
        multipleFullBarRestsNumber,
        "measure",
        "measures") <<
      ", line " << inputLineNumber <<
      std::endl << std::endl;
  }

  --gIndenter;

  fLilypondCodeStream <<
    "} %{ compressMMRests %}" <<
    std::endl;

  fOnGoingMultipleFullBarRests = false;
}

//________________________________________________________________________
void lpsr2lilypondTranslator::visitStart (S_msrMidiTempo& elt)
{
#ifdef TRACING_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gGlobalLpsrOahGroup->
          getTraceLpsrVisitors (),
      traceMidi =
        gGlobalTracingOahGroup->
          getTraceMidi (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || traceMidi || generateMsrVisitingInformation) {
      std::stringstream s;

      s <<
        "% --> Start visiting msrMidiTempo" <<
        ", line " << elt->getInputLineNumber () <<
        std::endl;

      if (traceLpsrVisitors) {
        gLogStream << s.str ();
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << s.str ();
      }
    }
  }
#endif

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
    elt->getMidiTempoDuration () << // BLARK
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
#ifdef TRACING_IS_ENABLED
  {
    Bool
      traceLpsrVisitors =
        gGlobalLpsrOahGroup->
          getTraceLpsrVisitors (),
      generateMsrVisitingInformation =
        gGlobalLpsr2lilypondOahGroup->
          getGenerateLpsrVisitingInformation ();

    if (traceLpsrVisitors || generateMsrVisitingInformation) {
      std::stringstream s;

      s <<
        "% --> End visiting msrMidiTempo" <<
        ", line " << elt->getInputLineNumber () <<
        std::endl;

      if (traceLpsrVisitors) {
        gLogStream << s.str ();
      }

      if (generateMsrVisitingInformation) {
        fLilypondCodeStream << s.str ();
      }
    }
  }
#endif
}


} // namespace


/* JMI
  Bool   commentOutLeftMargin (false);
  float  leftMarginValue = 0.0; // JMI
  std::string leftMarginUnitString =
           lengthUnitAsLilypondString (defaultLengthUnit);

  if (pageLayout) {
    S_msrMargin
      leftMargin =
        pageLayout->getSingleLeftMargin ();

    if (leftMargin) {
      leftMarginValue =
        leftMargin->getMarginLength ().getLengthValue ();

      leftMarginUnitString =
        lengthUnitAsLilypondString (
          leftMargin->getMarginLength ().getLengthUnitKind ());
    }
    else {
      commentOutLeftMargin = true;
    }
  }
  else {
    commentOutLeftMargin = true;
  }

  if (commentOutLeftMargin) {
    fLilypondCodeStream << "% ";
  }
  fLilypondCodeStream << std::left <<
    std::setw (fieldWidth) <<
    "left-margin" << " = " <<
    std::setprecision (3) << leftMarginValue <<
    leftMarginUnitString <<
    std::endl;

  // right margin
  Bool   commentOutRightMargin (false);
  float  rightMarginValue = 0.0; // JMI
  std::string rightMarginUnitString =
           lengthUnitAsLilypondString (defaultLengthUnit);

  if (pageLayout) {
    S_msrMargin
      rightMargin =
        pageLayout->getSingleRightMargin ();

    if (rightMargin) {
      rightMarginValue =
        rightMargin->getMarginLength ().getLengthValue ();

      rightMarginUnitString =
        lengthUnitAsLilypondString (
          rightMargin->getMarginLength ().getLengthUnitKind ());
    }
    else {
      commentOutRightMargin = true;
    }
  }
  else {
    commentOutRightMargin = true;
  }

  if (commentOutRightMargin) {
    fLilypondCodeStream << "% ";
  }
  fLilypondCodeStream << std::left <<
    std::setw (fieldWidth) <<
    "right-margin" << " = " <<
    std::setprecision (3) << rightMarginValue <<
    rightMarginUnitString <<
    std::endl;

  // top margin
  Bool   commentOutTopMargin (false);
  float  topMarginValue = 0.0; // JMI
  std::string topMarginUnitString =
           lengthUnitAsLilypondString (defaultLengthUnit);

  if (pageLayout) {
    S_msrMargin
      topMargin =
        pageLayout->getSingleTopMargin ();

    if (topMargin) {
      topMarginValue =
        topMargin->getMarginLength ().getLengthValue ();

      topMarginUnitString =
        lengthUnitAsLilypondString (
          topMargin->getMarginLength ().getLengthUnitKind ());
    }
    else {
      commentOutTopMargin = true;
    }
  }
  else {
    commentOutTopMargin = true;
  }

  if (commentOutTopMargin) {
    fLilypondCodeStream << "% ";
  }
  fLilypondCodeStream << std::left <<
    std::setw (fieldWidth) <<
    "top-margin" << " = " <<
    std::setprecision (3) << topMarginValue <<
    topMarginUnitString <<
    std::endl;

  // bottom margin
  Bool   commentOutBottomMargin (false);
  float  bottomMarginValue = 0.0; // JMI
  std::string bottomMarginUnitString =
           lengthUnitAsLilypondString (defaultLengthUnit);

  if (pageLayout) {
    S_msrMargin
      bottomMargin =
        pageLayout->getSingleBottomMargin ();

    if (bottomMargin) {
      bottomMarginValue =
        bottomMargin->getMarginLength ().getLengthValue ();

      bottomMarginUnitString =
        lengthUnitAsLilypondString (
          bottomMargin->getMarginLength ().getLengthUnitKind ());
    }
    else {
      commentOutBottomMargin = true;
    }
  }
  else {
    commentOutBottomMargin = true;
  }

  if (commentOutBottomMargin) {
    fLilypondCodeStream << "% ";
  }
  fLilypondCodeStream << std::left <<
    std::setw (fieldWidth) <<
    "bottom-margin" << " = " <<
    std::setprecision (3) << bottomMarginValue <<
    bottomMarginUnitString <<
    std::endl;
    */




