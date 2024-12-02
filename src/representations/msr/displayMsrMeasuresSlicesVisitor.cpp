/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2024

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <iomanip>      // std::setw, std::setprecision, ...

#include "mfServices.h"

#include "mfStringsHandling.h"

#include "msrTempos.h"

#include "oahOah.h"
#include "msrOah.h"

#include "displayMsrMeasuresSlicesVisitor.h"

#include "msrBrowsers.h"

#include "waeHandlers.h"


namespace MusicFormats
{

//________________________________________________________________________
displayMsrMeasuresSlicesVisitor::displayMsrMeasuresSlicesVisitor (
  const S_msrOahGroup& msrOpts)
{
  fMsrOahGroup = msrOpts;

  // create the
  fMeasuresSlicesSequence =
    msrMeasuresSlicesSequence::create (
      "measuresOrigin");

//   // part groups
//   fPartGroupsCounter = 0;
//
//   // parts
//   fPartsCounter = 0;
//
//   // staves
//   fStavesCounter = 0;
//   fOnGoingStaff = false;
//
//   // voices
//   fVoicesCounter = 0;
//
//   // stanzas
//   fStanzasCounter = 0;
//
//   // notes
//   fScoreUnpitchedNotesCounter     = 0;
//   fScoreRegularNotesCounter    = 0;
//   fScoreRestNotesCounter          = 0;
//   fScoreSkipNotesCounter          = 0;
//   fScoreDoubleTremoloNotesCounter = 0;
//   fScoreGraceNotesGroupCounter    = 0;
//
//   // chords
//   fScoreChordNotesCounter         = 0;
//   fScoreChordsCounter             = 0;
//
//   // tuplets
//   fScoreTupletNotesCounter        = 0;
//   fScoreTupletsCounter            = 0;
};

displayMsrMeasuresSlicesVisitor::~displayMsrMeasuresSlicesVisitor ()
{}

//________________________________________________________________________
void displayMsrMeasuresSlicesVisitor::printMsrScoreMeasuresSlices (
  const S_msrScore& score)
{
  if (score) {
    // set the parts browsing order
    score->
      setStavesBrowingOrderKind (
        msrStavesBrowingOrderKind::kStavesBrowingOrderHarmoniesRegularsFiguredBasses);

    // create a msrScore browser
    msrBrowser<msrScore> browser (this);

    // browse the score
    browser.browse (*score);
  }
}

//________________________________________________________________________
void displayMsrMeasuresSlicesVisitor::visitStart (S_msrScore& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting msrScore";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasuresSlices ()) {
    gLog <<
      "MSR measures slices of \"" <<
      gServiceRunData->getInputSourceName () <<
      "\":" <<
      std::endl << std::endl;

    ++gIndenter;

    S_msrMeasuresSlicesSequence
      scoreMeasuresSlicesSequence =
        elt->getScoreMeasuresSlicesSequence ();

    if (scoreMeasuresSlicesSequence) {
      scoreMeasuresSlicesSequence->
        printMeasuresSlicesVector (gLog);
    }
    else {
      gLog << "[NONE]" << std::endl;
    }
  }
#endif // MF_TRACE_IS_ENABLED
}

void displayMsrMeasuresSlicesVisitor::visitEnd (S_msrScore& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting msrScore";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasuresSlices ()) {
    --gIndenter;
  }
#endif // MF_TRACE_IS_ENABLED

  gLog <<
    "fMeasuresSlicesSequence:" <<
    std::endl;

  ++gIndenter;
  gLog << fMeasuresSlicesSequence;
  --gIndenter;

//   const int fieldWidth = 3;
//
//   gLog <<
//     std::setw (fieldWidth) <<
//     mfSingularOrPlural (
//       fPartGroupsCounter,
//       "part group",
//       "part groups") <<
//     std::endl <<
//
//     std::setw (fieldWidth) <<
//     mfSingularOrPlural (
//       fPartsCounter,
//       "part",
//       "parts") <<
//     std::endl <<
//
//     std::setw (fieldWidth) <<
//     mfSingularOrPlural (
//       fStavesCounter,
//       "staff",
//       "staves") <<
//     std::endl <<
//
//     std::setw (fieldWidth) <<
//     mfSingularOrPlural (
//       fVoicesCounter,
//       "voice",
//       "voices") <<
//     std::endl <<
//
//     std::setw (fieldWidth) <<
//     mfSingularOrPlural (
//       fStanzasCounter,
//       "stanza",
//       "stanzas") <<
//     std::endl <<
//
//     std::endl <<
//
//     std::setw (fieldWidth) <<
//     mfSingularOrPlural (
//       elt->getScoreMeasuresNumber (),
//       "measure",
//       "measures") <<
//     std::endl <<
//
//     std::endl <<
//
//     std::setw (fieldWidth) <<
//     mfSingularOrPlural (
//       fScoreRegularNotesCounter,
//       "standalone note",
//       "standalone notes") <<
//     std::endl <<
//
//     std::setw (fieldWidth) <<
//     mfSingularOrPlural (
//       fScoreRestNotesCounter,
//       "rest note",
//       "rest notes") <<
//     std::endl <<
//
//     std::setw (fieldWidth) <<
//     mfSingularOrPlural (
//       fScoreSkipNotesCounter,
//       "skip note",
//       "skip notes") <<
//     std::endl <<
//
//     std::setw (fieldWidth) <<
//     mfSingularOrPlural (
//       fScoreUnpitchedNotesCounter,
//       "unpitched note",
//       "unpitched notes") <<
//     std::endl <<
//
//     std::endl <<
//
//     std::setw (fieldWidth) <<
//     mfSingularOrPlural (
//       fScoreChordsCounter,
//       "chord",
//       "chords") <<
//     std::endl <<
//
//     std::setw (fieldWidth) <<
//     mfSingularOrPlural (
//       fScoreChordNotesCounter,
//       "chord note",
//       "chord notes") <<
//     std::endl <<
//
//     std::setw (fieldWidth) <<
//     mfSingularOrPlural (
//       fScoreTupletsCounter,
//       "tuplet",
//       "tuplets") <<
//     std::endl <<
//
//     std::setw (fieldWidth) <<
//     mfSingularOrPlural (
//       fScoreTupletNotesCounter,
//       "tuplet note",
//       "tuplet notes") <<
//     std::endl <<
//
//     std::endl <<
//
//     std::setw (fieldWidth) <<
//     mfSingularOrPlural (
//       fScoreGraceNotesGroupCounter,
//       "grace note group",
//       "grace notes groups") <<
//     std::endl <<
//
//     std::setw (fieldWidth) <<
//     mfSingularOrPlural (
//       fScoreDoubleTremoloNotesCounter,
//       "double tremolo note",
//       "double tremolo notes") <<
//     std::endl << std::endl;
//
//   --gIndenter;
}

//________________________________________________________________________
void displayMsrMeasuresSlicesVisitor::visitStart (S_msrPartGroup& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting msrPartGroup";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasuresSlices ()) {
    size_t partGroupElementsSize =
      elt->getPartGroupElementsList ().size ();

    gLog <<
      "PartGroup" << ' ' << elt->fetchPartGroupCombinedName () <<
      " contains " <<
      mfSingularOrPlural (
        partGroupElementsSize,
        " part or sub part group",
        " parts or sub part groups") <<
      std::endl;

    ++gIndenter;
  }
#endif // MF_TRACE_IS_ENABLED

//   ++fPartGroupsCounter;

//
//   const int fieldWidth = 24;
//
//   gLog << std::left <<
//     std::setw (fieldWidth) <<
//     "partGroupAbsoluteNumber" << ": \"" <<
//     elt->getPartGroupAbsoluteNumber  () << "\"" <<
//     std::endl <<
//
//     std::setw (fieldWidth) <<
//     "partGroupName" << ": \"" <<
//     elt->getPartGroupName  () << "\"" <<
//     std::endl <<
//
//     std::setw (fieldWidth) <<
//     "partGroupNameDisplayText" << ": \"" <<
//     elt->getPartGroupNameDisplayText  () << "\"" <<
//     std::endl <<
//
//     std::setw (fieldWidth) <<
//     "partGroupAccidentalText" << ": \"" <<
//     elt->getPartGroupAccidentalText  () << "\"" <<
//     std::endl <<
//
//     std::setw (fieldWidth) <<
//     "partGroupAbbrevation" << ": \"" <<
//     elt->getPartGroupAbbreviation  () << "\"" <<
//     std::endl <<
//
//     std::setw (fieldWidth) <<
//     "partGroupSymbolKind" << ": " <<
//     msrPartGroupSymbolKindAsString (
//       elt->getPartGroupSymbolKind  ()) <<
//     std::endl <<
//
//     std::setw (fieldWidth) <<
//     "partGroupSymbolDefaultX" << ": " <<
//     elt->getPartGroupSymbolDefaultX  () <<
//     std::endl <<
//
//     std::setw (fieldWidth) <<
//     "partGroupBarLine" << ": \"" <<
//     msrPartGroupBarLineKindAsString (
//       elt->getPartGroupBarLineKind ()) <<
//     "\"" <<
//     std::endl <<
//
//     std::setw (fieldWidth) <<
//     "partGroupInstrumentName" << ": \"" <<
//       elt->getPartGroupInstrumentName () << "\"" <<
//     std::endl << std::endl;
}

void displayMsrMeasuresSlicesVisitor::visitEnd (S_msrPartGroup& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting msrPartGroup";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasuresSlices ()) {
    --gIndenter;
  }
#endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
void displayMsrMeasuresSlicesVisitor::visitStart (S_msrPart& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting msrPart";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasuresSlices ()) {
    size_t partStavesMapSize =
      elt->getPartStavesMap ().size ();

    gLog <<
      "Part " << elt->fetchPartCombinedName () <<
      " contains " <<
      mfSingularOrPlural (
        partStavesMapSize,
        "staff", "staves") <<
      std::endl;

    ++gIndenter;
  }
#endif // MF_TRACE_IS_ENABLED

//   ++fPartsCounter;

//   const int fieldWidth = 28;
//
//   gLog << std::left <<
//     std::setw (fieldWidth) <<
//     "partID" << ": \"" <<
//     elt->getPartID () << "\"" <<
//     std::endl <<
//
//     std::setw (fieldWidth) <<
//     "partMsrName" << ": \"" <<
//     elt->getPartMsrName () << "\"" <<
//     std::endl <<
//
//     std::setw (fieldWidth) <<
//     "partName" << ": \"" <<
//     elt->getPartName  () << "\"" <<
//     std::endl <<
//
//     std::setw (fieldWidth) <<
//     "partAbbrevation" << ": \"" <<
//     elt->getPartAbbreviation () << "\"" <<
//     std::endl <<
//
//     std::setw (fieldWidth) <<
//     "partInstrumentName" << ": \"" <<
//     elt->getPartInstrumentName () << "\"" <<
//     std::endl <<
//
//     std::setw (fieldWidth) <<
//     "partInstrumentAbbreviation" << ": \"" <<
//     elt->getPartInstrumentAbbreviation () << "\"" <<
//     std::endl <<
//
//     std::setw (fieldWidth) <<
//     "partNumberOfMeasures" << ": " <<
//     elt->getPartNumberOfMeasures () <<
//     std::endl << std::endl;
}

void displayMsrMeasuresSlicesVisitor::visitEnd (S_msrPart& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting msrPart";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasuresSlices ()) {
    --gIndenter;
  }
#endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
void displayMsrMeasuresSlicesVisitor::visitStart (S_msrStaff& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting msrStaff";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasuresSlices ()) {
    size_t staffVoicesVectorSize =
      elt->getStaffAllVoicesList ().size ();

    gLog <<
      "Staff" << ' ' << elt->getStaffName () <<
      " contains " <<
      mfSingularOrPlural (
        staffVoicesVectorSize,
        "voice", "voices") <<
      std::endl;

    ++gIndenter;
  }
#endif // MF_TRACE_IS_ENABLED

//   ++fStavesCounter;

//
//   const int fieldWidth = 27;
//
//   // print the staff number
//   gLog << std::left <<
//     std::setw (fieldWidth) <<
//     "staffNumber" << ": " <<
//     elt->getStaffNumber () <<
//     std::endl <<
//
//     std::setw (fieldWidth) <<
//    "staffInstrumentName" << ": \"" <<
//     elt->getStaffInstrumentName () << "\"" <<
//     std::endl <<
//
//     std::setw (fieldWidth) <<
//    "staffInstrumentAbbreviation" << ": \"" <<
//     elt->getStaffInstrumentAbbreviation () << "\"" <<
//     std::endl;
//
//   gLog << std::endl;

//   fOnGoingStaff = true;
}

void displayMsrMeasuresSlicesVisitor::visitEnd (S_msrStaff& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting msrStaff";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasuresSlices ()) {
    --gIndenter;
  }
#endif // MF_TRACE_IS_ENABLED

//   fOnGoingStaff = false;
}

//________________________________________________________________________
void displayMsrMeasuresSlicesVisitor::visitStart (S_msrVoice& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting msrVoice";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasuresSlices ()) {
    gLog <<
      "Voice " << elt->getVoiceNumber () << ":" <<
      std::endl;

    ++gIndenter;
  }
#endif // MF_TRACE_IS_ENABLED

//     ++fVoicesCounter;
//
//   size_t voiceStanzasMapSize = elt->getVoiceStanzasMap ().size ();
//
//   gLog <<
//     "Voice" << ' ' << elt->getVoiceName () <<
//     " has " <<
//     mfSingularOrPlural (
//       voiceStanzasMapSize,
//       "stanza", "stanzas") <<
//     std::endl;
//
//   ++gIndenter;
//
//   const int fieldWidth = 34;
//
//   gLog << std::left <<
//     std::setw (fieldWidth) <<
//     "voiceNumber" << ": " <<
//     elt->getVoiceNumber () <<
//     std::endl <<
//     std::setw (fieldWidth) <<
//     "regularVoiceStaffSequentialNumber" << ": " <<
//     elt->getRegularVoiceStaffSequentialNumber () <<
//     std::endl <<
//     std::setw (fieldWidth) <<
//     "voiceActualNotesCounter" << ": " <<
//     elt->getVoiceActualNotesCounter () <<
//     std::endl <<
//     std::setw (fieldWidth) <<
//     "voiceActualHarmoniesCounter" << ": " <<
//     elt->getVoiceActualHarmoniesCounter () <<
//     std::endl <<
//     std::setw (fieldWidth) <<
//     "measureIsMusicallyEmpty" << ": " <<
//     elt->getMeasureIsMusicallyEmpty () <<
//     std::endl;
//
//   gLog << std::endl;
//
//   // don't show fVoiceMuteStanza in the summary
//
//   const std::list<S_msrMeasure>&
//     voiceMeasuresFlatList =
//       elt->getVoiceMeasuresFlatList ();
//
//   for (S_msrMeasure measure : voiceMeasuresFlatList) {
//   } // for
}

void displayMsrMeasuresSlicesVisitor::visitEnd (S_msrVoice& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting msrVoice";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasuresSlices ()) {
    --gIndenter;
  }
#endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
void displayMsrMeasuresSlicesVisitor::visitStart (S_msrClef& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting msrClef";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

//   if (! fOnGoingStaff) {
//     gLog <<
//       elt->asString () <<
//       std::endl;
//   }
}

void displayMsrMeasuresSlicesVisitor::visitEnd (S_msrClef& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting msrClef";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
void displayMsrMeasuresSlicesVisitor::visitStart (S_msrKey& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting msrKey";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

//   if (! fOnGoingStaff) {
//     gLog <<
//       elt->asString () <<
//       std::endl;
//   }
}

void displayMsrMeasuresSlicesVisitor::visitEnd (S_msrKey& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting msrKey";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
void displayMsrMeasuresSlicesVisitor::visitStart (S_msrTimeSignature& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting msrTimeSignature";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

//   if (! fOnGoingStaff) {
//     gLog <<
//       elt->asString () <<
//       std::endl;
//   }
}

void displayMsrMeasuresSlicesVisitor::visitEnd (S_msrTimeSignature& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting msrTimeSignature" <<
    std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
void displayMsrMeasuresSlicesVisitor::visitStart (S_msrSegment& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting msrSegment";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

void displayMsrMeasuresSlicesVisitor::visitEnd (S_msrSegment& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting msrSegment";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
void displayMsrMeasuresSlicesVisitor::visitStart (S_msrMeasure& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting msrMeasure";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasuresSlices ()) {
    gLog <<
      "Measure " << elt->getMeasureNumber () << ":" <<
      std::endl;

    ++gIndenter;

    gLog << gTab;
  }
#endif // MF_TRACE_IS_ENABLED

  // create a msrMeasuresSlice
  fCurrentMeasuresSlice =
    msrMeasuresSlice::create (
      elt->getMeasureNumber (),
      elt->getMeasurePuristNumber ());

  // add it to fMeasuresSlicesSequence
  fMeasuresSlicesSequence->
    appendMeasuresSliceToSequence (
      fCurrentMeasuresSlice);

  // append elt to fCurrentMeasuresSlice
  fCurrentMeasuresSlice->
    appendMeasureToMeasureSlice (
      elt);
}

void displayMsrMeasuresSlicesVisitor::visitEnd (S_msrMeasure& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting msrMeasure";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasuresSlices ()) {
    --gIndenter;

    gLog << std::endl;
  }
#endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
void displayMsrMeasuresSlicesVisitor::visitStart (S_msrNote& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting " <<
      msrNoteKindAsString (
        elt->getNoteKind ()) <<
      " msrNote";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

//   // print only 'playable' notes
//   switch (elt->getNoteKind ()) {
//     case msrNoteKind::kNote_UNKNOWN_:
//       break;
//
//     case msrNoteKind::kNoteRestInMeasure:
//       break;
//     case msrNoteKind::kNoteSkipInMeasure:
//       break;
//     case msrNoteKind::kNoteUnpitchedInMeasure:
//       break;
//
//     case msrNoteKind::kNoteRegularInMeasure:
//       gLog <<
//         elt->noteCoreAsString () << ' ';
//       break;
//
//     case msrNoteKind::kNoteInDoubleTremolo:
//       break;
//
//     case msrNoteKind::kNoteRegularInGraceNotesGroup:
//     case msrNoteKind::kNoteSkipInGraceNotesGroup:
//     case msrNoteKind::kNoteInChordInGraceNotesGroup:
//       break;
//
//     case msrNoteKind::kNoteRegularInChord:
//       gLog <<
//         elt->noteCoreAsString () << ' ';
//       break;
//
//     case msrNoteKind::kNoteRegularInTuplet:
//     case msrNoteKind::kNoteRestInTuplet:
//       gLog <<
//         elt->noteCoreAsString () << ' ';
//      break;
//
//     case msrNoteKind::kNoteInTupletInGraceNotesGroup:
//       break;
//
//     case msrNoteKind::kNoteUnpitchedInTuplet:
//       break;
//   } // switch
}

void displayMsrMeasuresSlicesVisitor::visitEnd (S_msrNote& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting " <<
      msrNoteKindAsString (
        elt->getNoteKind ()) <<
      " msrNote";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

//   switch (elt->getNoteKind ()) { // JMI
//     case msrNoteKind::kNote_UNKNOWN_:
//       break;
//     case msrNoteKind::kNoteRestInMeasure:
//       break;
//     case msrNoteKind::kNoteSkipInMeasure:
//       break;
//     case msrNoteKind::kNoteUnpitchedInMeasure:
//       break;
//     case msrNoteKind::kNoteRegularInMeasure:
//       break;
//     case msrNoteKind::kNoteInDoubleTremolo:
//       break;
//     case msrNoteKind::kNoteRegularInGraceNotesGroup:
//     case msrNoteKind::kNoteSkipInGraceNotesGroup:
//     case msrNoteKind::kNoteInChordInGraceNotesGroup:
//       break;
//     case msrNoteKind::kNoteRegularInChord:
//       break;
//     case msrNoteKind::kNoteRegularInTuplet:
//     case msrNoteKind::kNoteRestInTuplet:
//     case msrNoteKind::kNoteInTupletInGraceNotesGroup:
//     case msrNoteKind::kNoteUnpitchedInTuplet:
//       break;
//   } // switch
}

//________________________________________________________________________
void displayMsrMeasuresSlicesVisitor::visitStart (S_msrChord& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting msrChord";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
//
//   gLog <<
//     elt->asShortString () <<
//     "< ";

//   ++fScoreChordsCounter;
}

void displayMsrMeasuresSlicesVisitor::visitEnd (S_msrChord& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting msrChord";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

//   gLog << " > ";
}

//________________________________________________________________________
void displayMsrMeasuresSlicesVisitor::visitStart (S_msrTuplet& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting msrTuplet";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

//   gLog << "{ ";

//   ++fScoreTupletsCounter;
}

void displayMsrMeasuresSlicesVisitor::visitEnd (S_msrTuplet& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting msrTuplet";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

//   gLog << " } ";
}

//________________________________________________________________________
void displayMsrMeasuresSlicesVisitor::visitStart (S_msrBarLine& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting msrBarLine";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

void displayMsrMeasuresSlicesVisitor::visitEnd (S_msrBarLine& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting msrBarLine";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
void displayMsrMeasuresSlicesVisitor::visitStart (S_msrBarNumberCheck& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting msrBarNumberCheck";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

void displayMsrMeasuresSlicesVisitor::visitEnd (S_msrBarNumberCheck& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting msrBarNumberCheck";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
void displayMsrMeasuresSlicesVisitor::visitStart (S_msrLineBreak& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting msrLineBreak";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

void displayMsrMeasuresSlicesVisitor::visitEnd (S_msrLineBreak& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting msrLineBreak";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
void displayMsrMeasuresSlicesVisitor::visitStart (S_msrRepeat& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> Start visiting msrRepeat";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

void displayMsrMeasuresSlicesVisitor::visitEnd (S_msrRepeat& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "--> End visiting msrRepeat";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}


} // namespace
