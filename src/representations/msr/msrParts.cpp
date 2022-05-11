/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <climits>      // INT_MIN, INT_MAX
#include <iomanip>      // setw, setprecision, ...
#include <algorithm>    // for_each

#include "visitor.h"

#include "mfAssert.h"

#include "mfServiceRunData.h"

#include "mfStringsHandling.h"

#include "msrWae.h"

#include "enableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "tracingOah.h"
#endif

#include "msrParts.h"

#include "oahOah.h"

#include "msrOah.h"

#include "msrBrowsers.h"


using namespace std;

namespace MusicFormats
{

//______________________________________________________________________________
// constants
const int msrPart::K_PART_HARMONIES_STAFF_NUMBER = 10;
const int msrPart::K_PART_HARMONIES_VOICE_NUMBER = 11;

const int msrPart::K_PART_FIGURED_BASS_STAFF_NUMBER = 20;
const int msrPart::K_PART_FIGURED_BASS_VOICE_NUMBER = 21;

// global variable
int msrPart::gPartsCounter = 0;

S_msrPart msrPart::create (
  int            inputLineNumber,
  const string&  partID,
  S_msrPartGroup partPartGroupUpLink)
{
  msrPart* o =
    new msrPart (
      inputLineNumber,
      partID,
      partPartGroupUpLink);
  assert (o != nullptr);
  return o;
}

msrPart::msrPart (
  int            inputLineNumber,
  const string&  partID,
  S_msrPartGroup partPartGroupUpLink)
    : msrPartGroupElement (inputLineNumber)
{
  // replace spaces in partID to set fPartID
  for_each (
    partID.begin (),
    partID.end (),
    mfStringSpaceReplacer (fPartID, '_'));

/* JMI
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    partPartGroupUpLink != nullptr,
    "partPartGroupUpLink is null");
    */

  // set part number
  fPartAbsoluteNumber = ++gPartsCounter;

  // set part's part group upLink
  fPartPartGroupUpLink = partPartGroupUpLink;

  // do other initializations
  initializePart ();
}

void msrPart::initializePart ()
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceParts ()) {
    gLogStream <<
      "Creating part \"" << asString () << "\"" <<
      endl;

    gGlobalMsrOahGroup->printMsrOahValues (40); // JMI
  }
#endif

  // is this part name in the part renaming map?
  map<string, string>::const_iterator
    it =
      gGlobalMsrOahGroup->getMsrPartsRenamingMap ().find (fPartID);

  if (it != gGlobalMsrOahGroup->getMsrPartsRenamingMap ().end ()) {
    // yes, rename the part accordinglingly
    string newMsrPartName = (*it).second;

    stringstream s;

    s <<
      "Renaming MSR part " <<
      getPartCombinedName () <<
      " to \"" <<
      newMsrPartName <<
      "\"";

    msrWarning (
      gGlobalServiceRunData->getInputSourceName (),
      fInputLineNumber,
      s.str ());

    fPartMsrName = newMsrPartName;
  }
  else {
    // coin the name from the argument
    fPartMsrName =
      "Part_"+mfStringNumbersToEnglishWords (fPartID);
  }

  // initialize part's number of measures
  fPartNumberOfMeasures = 0;

  // multiple full-bar rests
  fPartContainsMultipleFullBarRests = false;

  // current position in measure
  fPartCurrentPositionInMeasure = rational (0,0);

  // part shortest note duration
  fPartShortestNoteDuration = rational (INT_MAX, 1);

  // part shortest note tuplet factor
  fPartShortestNoteTupletFactor = rational (1, 1);

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceParts ()) {
    gLogStream <<
      "Creating part \"" << asString () << "\"" <<
      endl;
  }
#endif
}

msrPart::~msrPart ()
{}

S_msrScore  msrPart::fetchPartScoreUpLink () const
{
  S_msrScore result;

  if (fPartPartGroupUpLink) {
    result =
      fPartPartGroupUpLink->
        getPartGroupScoreUpLink ();
  }

  return result;
}

S_msrPart msrPart::createPartNewbornClone (S_msrPartGroup partGroupClone)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceParts ()) {
    gLogStream <<
      "Creating a newborn clone of part " <<
      getPartCombinedName () <<
      endl;
  }
#endif

  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    partGroupClone != nullptr,
    "partGroupClone is null");

  S_msrPart
    newbornClone =
      msrPart::create (
        fInputLineNumber,
        fPartID,
        partGroupClone);

  newbornClone->fPartMsrName =
    fPartMsrName;

  newbornClone->fPartName =
    fPartName;
  newbornClone->fPartNameDisplayText =
    fPartNameDisplayText;

  newbornClone->fPartAbbreviation =
    fPartAbbreviation;
  newbornClone->fPartAbbreviationDisplayText =
    fPartAbbreviationDisplayText;

  newbornClone->fPartInstrumentName =
    fPartInstrumentName;
  newbornClone->fPartInstrumentAbbreviation =
    fPartInstrumentAbbreviation;

  // set the newbornClone's number of measures
  newbornClone->
    setPartNumberOfMeasures (
      fPartNumberOfMeasures);

  return newbornClone;
}

void msrPart::registerStaffInPart (
  S_msrStaff staff)
{
  int inputLineNumber =
    staff->getInputLineNumber ();

  int staffNumber =
    staff->getStaffNumber ();

  msrStaffKind staffKind =
    staff->getStaffKind ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceStaves ()) {
    gLogStream <<
      "Registering staff \"" <<
      staff->getStaffName () <<
      ", \"" <<
      msrStaffKindAsString (staffKind) <<
      "\" under number " <<
      staffNumber <<
      " in part " <<
      getPartCombinedName () <<
      endl;
  }
#endif

  // register staff in the all staves list
  fPartAllStavesList.push_back (staff);

  // register its number in the staves numbers to staves map
  fPartStaveNumbersToStavesMap [staffNumber] = staff;

  switch (staffKind) {
    case msrStaffKind::kStaffKindRegular:
      // register staff in the regular staves list
      fPartRegularStavesList.push_back (staff);

      // register staff in the
      // non harmonies nor figured bass staves list
      fPartNonHarmoniesNorFiguredBassStavesList.push_back (staff);
      break;

    case msrStaffKind::kStaffKindTablature:
    case msrStaffKind::kStaffKindDrum:
    case msrStaffKind::kStaffKindRythmic:
      // register staff in the
      // non harmonies nor figured bass staves list
      fPartNonHarmoniesNorFiguredBassStavesList.push_back (staff);
      break;

    case msrStaffKind::kStaffKindHarmonies:
      // sanity check
      if (fPartHarmoniesStaff) {
        stringstream s;

        s <<
          "a harmonies staff already exists in part " <<
          getPartCombinedName () <<
          ", line " << inputLineNumber;

        msrInternalError ( // JMI ???
          gGlobalServiceRunData->getInputSourceName (),
          inputLineNumber,
          __FILE__, __LINE__,
          s.str ());
      }

      // register harmonies staff
      fPartHarmoniesStaff = staff;
      break;

    case msrStaffKind::kStaffKindFiguredBass:
      // sanity check
      if (fPartFiguredBassStaff) {
        stringstream s;

        s <<
          "a figured bass staff already exists in part " <<
          getPartCombinedName () <<
          ", line " << inputLineNumber;

        msrInternalError ( // JMI ???
          gGlobalServiceRunData->getInputSourceName (),
          inputLineNumber,
          __FILE__, __LINE__,
          s.str ());
      }

      // register figured bass staff
      fPartFiguredBassStaff = staff;
      break;
  } // switch
}

void msrPart::setPartCurrentPositionInMeasure (
  int             inputLineNumber,
  const rational& positionInMeasure)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTracePositionsInMeasures ()) {
    gLogStream <<
      "Setting part current position in measure to " <<
      positionInMeasure <<
      " in part " <<
      getPartCombinedName () <<
      endl;
  }
#endif

  if (positionInMeasure.getNumerator () < 0) {
    stringstream s;

    s <<
      "cannot set part current position in measure to " <<
      positionInMeasure <<
      " in part " <<
      getPartCombinedName () <<
      " since it is negative";

    msrInternalError (
      gGlobalServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }

  fPartCurrentPositionInMeasure =
    positionInMeasure;
}

void msrPart::incrementPartCurrentPositionInMeasure (
  int             inputLineNumber,
  const rational& duration)
{
  fPartCurrentPositionInMeasure += duration;
  fPartCurrentPositionInMeasure.rationalise ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTracePositionsInMeasures ()) {
    gLogStream <<
      "Incrementing part current position in measure by " <<
      duration <<
      " in part " <<
      getPartCombinedName () <<
      ", thus setting it to " <<
      fPartCurrentPositionInMeasure <<
      endl;
  }
#endif
}

void msrPart::decrementPartCurrentPositionInMeasure (
  int             inputLineNumber,
  const rational& duration)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTracePositionsInMeasures ()) {
    gLogStream <<
      "Decrementing part current position in measure by " <<
      duration <<
      " in part " <<
      getPartCombinedName () <<
      endl;
  }
#endif

  fPartCurrentPositionInMeasure -= duration;
  fPartCurrentPositionInMeasure.rationalise ();

  if (fPartCurrentPositionInMeasure.getNumerator () < 0) {
    stringstream s;

    s <<
      "cannot decrement part current position in measure by " <<
      duration <<
      " in part " <<
      getPartCombinedName () <<
      " since that sets it to " <<
      fPartCurrentPositionInMeasure <<
      ", which is negative ";

    msrInternalError (
      gGlobalServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTracePositionsInMeasures ()) {
    gLogStream <<
      "The new part current position in measure is " <<
      fPartCurrentPositionInMeasure <<
      " in part " <<
      getPartCombinedName () <<
      endl;
  }
#endif
}

void msrPart::setPartShortestNoteDuration (
  const rational& duration)
{
#ifdef TRACING_IS_ENABLED
  if (
    gGlobalTracingOahGroup->getTraceNotes ()
      ||
    gGlobalMsrOahGroup->getTraceMsrDurations ()
  ) {
    gLogStream <<
      "Setting the shortest note duration of part \"" <<
      fPartName <<
      "\" to " <<
      duration <<
      endl;
  }
#endif

  fPartShortestNoteDuration = duration;
}

void msrPart::setPartShortestNoteTupletFactor (
  const msrTupletFactor& noteTupletFactor)
{
#ifdef TRACING_IS_ENABLED
  if (
    gGlobalTracingOahGroup->getTraceNotes ()
      ||
    gGlobalMsrOahGroup->getTraceMsrDurations ()
  ) {
    gLogStream <<
      "Setting the shortest note tuplet factor of part " <<
      fPartName <<
      " to " <<
      noteTupletFactor <<
      endl;
  }
#endif

  fPartShortestNoteTupletFactor = noteTupletFactor;
}

void msrPart::assignSequentialNumbersToRegularVoicesInPart (
  int inputLineNumber)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceParts () || gGlobalTracingOahGroup->getTraceVoices ()) {
    gLogStream <<
      "Assigning sequential numbers to the staves in part \"" <<
      fPartID <<
      ", " <<
      fPartName <<
      "\"" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  for (S_msrStaff staff : fPartAllStavesList) {
    staff->
      assignSequentialNumbersToRegularVoicesInStaff (
        inputLineNumber);
  } // for
}

void msrPart::setPartMsrName (const string& partMsrName)
{
  // is this part name in the part renaming map?
  map<string, string>::const_iterator
    it =
      gGlobalMsrOahGroup->getMsrPartsRenamingMap ().find (fPartMsrName);

  if (it != gGlobalMsrOahGroup->getMsrPartsRenamingMap ().end ()) {
    // yes, rename the part accordinglingly
    fPartMsrName = (*it).second;

#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceParts ()) {
      gLogStream <<
        "Setting part name of " << getPartCombinedName () <<
        " to \"" << fPartMsrName << "\"" <<
         endl;
    }
#endif
  }
  else {
    // use the argument
    fPartMsrName = partMsrName;

#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceParts ()) {
      gLogStream <<
        "Keeping partID \"" << partMsrName <<
        "\" as part name  for " << getPartCombinedName () <<
      endl;
    }
#endif
  }
}

string msrPart::getPartCombinedName () const
{
  stringstream s;

  if (! fPartMsrName.size ()) {
    s <<
      "[empty name]";
  }
  else {
    s <<
     // JMI "\"" <<
      fPartMsrName
     // JMI << "\""
      ;
  }

  s <<
    " (partID \"" << fPartID << "\"";

  if (fPartName.size ()) {
    s <<
      ", partName \"" << fPartName << "\"";
  }

  s <<
    ")";

  return s.str ();
}

void msrPart::createAMeasureAndAppendItToPart (
  int           inputLineNumber,
  int           previousMeasureEndInputLineNumber,
  const string& measureNumber,
  msrMeasureImplicitKind
                measureImplicitKind)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasures ()) {
    gLogStream <<
      "Creating measure '" <<
      measureNumber <<
      "' and appending it to part " <<
      getPartCombinedName () <<
      "', line " << inputLineNumber <<
      endl;
  }
#endif

  ++gIndenter;

  // set part current measure number
  fPartCurrentMeasureNumber = measureNumber;

  // create and append measure in all the staves
  for (S_msrStaff staff : fPartAllStavesList) {
    staff->
      createAMeasureAndAppendItToStaff (
        inputLineNumber,
        previousMeasureEndInputLineNumber,
        measureNumber,
        measureImplicitKind);
  } // for

  --gIndenter;
}

void msrPart::setNextMeasureNumberInPart (
  int           inputLineNumber,
  const string& nextMeasureNumber)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasures ()) {
    gLogStream <<
      "Setting next measure number to '" <<
      nextMeasureNumber <<
      "' in part " <<
      getPartCombinedName () <<
      "', line " << inputLineNumber <<
      endl;
  }
#endif

  ++gIndenter;

  // set next measure number in all staves
  for (S_msrStaff staff : fPartAllStavesList) {
    staff->
      setNextMeasureNumberInStaff (
        inputLineNumber,
        nextMeasureNumber);
  } // for

  --gIndenter;
}

rational msrPart::fetchPartMeasuresWholeNotesDurationsVectorAt (
  int inputLineNumber,
  int indexValue) const
{
  rational result;

  size_t
    partMeasuresWholeNotesDurationsVectorSize =
      fPartMeasuresWholeNotesDurationsVector.size ();

#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceWholeNotes ()) {
      gLogStream <<
        "fetchPartMeasuresWholeNotesDurationsVectorAt() in part \"" <<
        getPartCombinedName () <<
        "\"" <<
        ", partMeasuresWholeNotesDurationsVectorSize: " <<
        partMeasuresWholeNotesDurationsVectorSize <<
        ", indexValue: " << indexValue <<
        endl;
    }
#endif

  // has this measureOrdinalNumber been registered already?
  try {
    rational
      currentValue =
        fPartMeasuresWholeNotesDurationsVector.at (indexValue);

    // yes
    result = currentValue;
  }

  catch (const out_of_range& e) {
    // no

    stringstream s;

    s <<
      "fetchPartMeasuresWholeNotesDurationsVectorAt() in part \"" <<
      getPartCombinedName () <<
      "\"" <<
      ", partMeasuresWholeNotesDurationsVectorSize: " <<
      partMeasuresWholeNotesDurationsVectorSize <<
      ", indexValue: " << indexValue << " is out of bounds" <<
      ", line " << inputLineNumber;

    msrInternalWarning (
//     msrInternalError (
      gGlobalServiceRunData->getInputSourceName (),
      inputLineNumber,
//       __FILE__, __LINE__,
      s.str ());

      result = rational (3, 4); // TEMP JMI v0.9.61
  }

  return result;
}

void msrPart::registerShortestNoteInPartIfRelevant (S_msrNote note)
{
  // is note the shortest one in this part?
  rational
    noteSoundingWholeNotes =
      note->
        getNoteSoundingWholeNotes ();

/* JMI
  rational
    noteDisplayWholeNotes =
      note->
        getNoteDisplayWholeNotes ();
      */

  if (noteSoundingWholeNotes < fPartShortestNoteDuration) {
    // set the part shortest note duration
    this ->
      setPartShortestNoteDuration (
        noteSoundingWholeNotes);

/* JMI
    // set the part shortest note tuplet factor // JMI ???
    fPartShortestNoteTupletFactor =
      note->
        getNoteTupletFactor ();

    this ->
      setPartShortestNoteTupletFactor (
        fVoiceShortestNoteTupletFactor);
*/

#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceNotes ()) {
      gLogStream <<
        "The new shortest note in part \"" << getPartCombinedName () << "\"" <<
        " becomes " << note->asString () <<
        endl;
    }
#endif
  }

/* JMI
  if (noteDisplayWholeNotes < fVoiceShortestNoteDuration) {
    fVoiceShortestNoteDuration = noteDisplayWholeNotes;
  }
  */
}

void msrPart::setPartNumberOfMeasures (size_t partNumberOfMeasures)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasures ()) {
    gLogStream <<
      "Setting the number of measures in part " <<
      getPartCombinedName () <<
      " to " <<
      partNumberOfMeasures <<
    endl;
  }
#endif

  fPartNumberOfMeasures = partNumberOfMeasures;

  size_t
    fPartMeasuresWholeNotesDurationsVectorSize =
      fPartMeasuresWholeNotesDurationsVector.size ();

  if (partNumberOfMeasures > fPartMeasuresWholeNotesDurationsVectorSize) {
#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceMeasures ()) {
      gLogStream <<
        "Resizing fPartMeasuresWholeNotesDurationsVector in part " << // JMI ???
        getPartCombinedName () <<
        " from " <<
        fPartMeasuresWholeNotesDurationsVectorSize <<
        " to " <<
        partNumberOfMeasures <<
        " measures" <<
        endl;
    }
#endif

    fPartMeasuresWholeNotesDurationsVector.clear ();
    fPartMeasuresWholeNotesDurationsVector.resize (
      fPartNumberOfMeasures,
      rational (0, 1));
  }
}

void msrPart::registerOrdinalMeasureNumberWholeNotesDuration (
  int             inputLineNumber,
  int             measureOrdinalNumber,
  const rational& wholeNotesDuration)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasures ()) {
    gLogStream <<
      "Registering the whole notes duration of the measure with ordinal number '" <<
      measureOrdinalNumber <<
      "' as " <<
      wholeNotesDuration <<
      " in part " << getPartCombinedName () <<
      ", measureOrdinalNumber: " << measureOrdinalNumber <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

#ifdef TRACING_IS_ENABLED
  if (
    gGlobalTracingOahGroup->getTraceMeasures ()
      ||
    gGlobalTracingOahGroup->getTracePositionsInMeasures ()
  ) {
    gLogStream <<
      "===> fPartMeasuresWholeNotesDurationsVector contents: " <<
      endl;
    for (rational rat : fPartMeasuresWholeNotesDurationsVector) {
      ++gIndenter;
      gLogStream <<
        rat <<
        endl;
      --gIndenter;
    } // for
    gLogStream <<
      "<==== end of fPartMeasuresWholeNotesDurationsVector contents " <<
      endl;
  }
#endif

  size_t index = measureOrdinalNumber - 1;

  // has this measureOrdinalNumber been registered already?
  try {
    rational
      currentValue =
        fPartMeasuresWholeNotesDurationsVector.at (index);

    // yes

    // allow for polymetrics in non-MusicXML contexts? JMI
#ifdef TRACING_IS_ENABLED
    if (
      gGlobalTracingOahGroup->getTraceMeasures ()
        ||
      gGlobalTracingOahGroup->getTracePositionsInMeasures ()
    ) {
      gLogStream <<
        "The measure with ordinal number " <<
        measureOrdinalNumber <<
        " was known with a whole notes duration of " <<
        currentValue <<
        ", now registering it with a duration of " <<
        wholeNotesDuration <<
        " in part " << getPartCombinedName () <<
        endl;
    }
#endif
  }

  catch (const out_of_range& e) {
    // no

    fPartMeasuresWholeNotesDurationsVector [index] =
      wholeNotesDuration;

#ifdef TRACING_IS_ENABLED
    if (true || gGlobalTracingOahGroup->getTraceMeasures ()) {
      gLogStream <<
        "The measure with ordinal number " <<
        measureOrdinalNumber <<
        " is now registered with a duration of " <<
        wholeNotesDuration <<
        " in part " << getPartCombinedName () <<
        ", fPartMeasuresWholeNotesDurationsVector.size () = " <<
        fPartMeasuresWholeNotesDurationsVector.size () <<
        endl;
    }
#endif
  }
}

void msrPart::appendStaffDetailsToPart (
  S_msrStaffDetails staffDetails)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceStaffDetails ()) {
    gLogStream <<
      "Appending staff details\"" <<
      staffDetails->asShortString () <<
      "\" to part " << getPartCombinedName () <<
    endl;
  }
#endif

  // register staff details in part
  fCurrentPartStaffDetails = staffDetails;

  // append staff details to regular staves
  for (S_msrStaff staff : fPartAllStavesList) {
    staff->
      appendStaffDetailsToStaff (
        staffDetails);
  } // for
}

void msrPart::appendClefToPart (S_msrClef clef)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceClefs ()) {
    gLogStream <<
      "Appending clef '" <<
      clef->asString () <<
      "' to part " << getPartCombinedName () <<
    endl;
  }
#endif

  ++gIndenter;

  // append clef to part-level staves if any
  if (fPartHarmoniesStaff) {
    fPartHarmoniesStaff->
      appendClefToStaff (clef);
  }
  if (fPartFiguredBassStaff) {
    fPartFiguredBassStaff->
      appendClefToStaff (clef);
  }

  // append clef to regular staves
  for (S_msrStaff staff : fPartRegularStavesList) {
    staff->
      appendClefToStaff (clef);
  } // for

  --gIndenter;
}

void msrPart::appendKeyToPart (S_msrKey key)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceKeys ()) {
    gLogStream <<
      "Appending key " <<
      key->asString () <<
      " to part " << getPartCombinedName () <<
      endl;
  }
#endif

  // set part key
  fPartCurrentKey = key;

  ++gIndenter;

  // append key to part-level staves if any
  if (fPartHarmoniesStaff) {
    fPartHarmoniesStaff->
      appendKeyToStaff (key);
  }
  if (fPartFiguredBassStaff) {
    fPartFiguredBassStaff->
      appendKeyToStaff (key);
  }

  // append key to regular staves
  for (S_msrStaff staff : fPartRegularStavesList) {
    staff->
      appendKeyToStaff (key);
  } // for

  --gIndenter;
}

void msrPart::appendTimeSignatureToPart (S_msrTimeSignature timeSignature)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceTimeSignatures ()) {
    gLogStream <<
      "Appending time signature '" <<
      timeSignature->asString () <<
      "' to part " << getPartCombinedName () <<
      endl;
  }
#endif

  // set part time signature
  fPartCurrentTimeSignature = timeSignature;

  ++gIndenter;

  // append timeSignature to part-level staves if any
  if (fPartHarmoniesStaff) {
    fPartHarmoniesStaff->
      appendTimeSignatureToStaff (timeSignature);
  }
  if (fPartFiguredBassStaff) {
    fPartFiguredBassStaff->
      appendTimeSignatureToStaff (timeSignature);
  }

  // append timeSignature to regular staves
  for (S_msrStaff staff : fPartRegularStavesList) {
    staff->
      appendTimeSignatureToStaff (timeSignature);
  } // for

  --gIndenter;
}

void msrPart::appendTimeSignatureToPartClone (S_msrTimeSignature timeSignature)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceTimeSignatures ()) {
    gLogStream <<
      "Appending time signature '" <<
      timeSignature->asString () <<
      "' to part clone " << getPartCombinedName () <<
      endl;
  }
#endif

  // set part time signature
  fPartCurrentTimeSignature = timeSignature;

  ++gIndenter;

  // propagate it to regular staves
  for (S_msrStaff staff : fPartRegularStavesList) {
    staff->
      appendTimeSignatureToStaffClone (timeSignature);
  } // for

  --gIndenter;
}

void msrPart::appendTempoToPart (S_msrTempo tempo)
{
 #ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceTempos ()) {
    gLogStream <<
      "Appending tempo " << tempo->asString () <<
      " to part " <<
      getPartCombinedName () <<
      endl;
  }
#endif

 // append tempo to registered staves
  for (S_msrStaff staff : fPartAllStavesList) {
    staff->
      appendTempoToStaff (tempo);
  } // for
}

void msrPart::appendRehearsalMarkToPart (
  S_msrRehearsalMark rehearsalMark)
{
 #ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRehearsalMarks ()) {
    gLogStream <<
      "Appending rehearsal mark " << rehearsalMark->asString () <<
      " to part " <<
      getPartCombinedName () <<
      endl;
  }
#endif

 // append rehearsal mark to registered staves
  for (S_msrStaff staff : fPartAllStavesList) {
    staff->
      appendRehearsalMarkToStaff (rehearsalMark);
  } // for
}


void msrPart::appendLineBreakToPart (S_msrLineBreak lineBreak)
{
 #ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceLineBreaks ()) {
    gLogStream <<
      "Appending line break " << lineBreak->asString () <<
      " to part " <<
      getPartCombinedName () <<
      endl;
  }
#endif

 // append line break to registered staves
  for (S_msrStaff staff : fPartAllStavesList) {
    staff->
      appendLineBreakToStaff (lineBreak);
  } // for
}

void msrPart::appendPageBreakToPart (S_msrPageBreak pageBreak)
{
 #ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTracePageBreaks ()) {
    gLogStream <<
      "Appending page break " << pageBreak->asString () <<
      " to part " <<
      getPartCombinedName () <<
      endl;
  }
#endif

 // append page break to registered staves
  for (S_msrStaff staff : fPartAllStavesList) {
    staff->
      appendPageBreakToStaff (pageBreak);
  } // for
}

void msrPart::insertHiddenMeasureAndBarLineInPartClone (
  int             inputLineNumber,
  const rational& positionInMeasure)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceDalSegnos () || gGlobalTracingOahGroup->getTracePositionsInMeasures ()) {
    gLogStream <<
      "Inserting hidden measure and barLine at position " <<
      positionInMeasure <<
      "' in part clone " << getPartCombinedName () <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  ++gIndenter;

  // cascade it to all staves
  for (S_msrStaff staff : fPartAllStavesList) {
    staff->
      insertHiddenMeasureAndBarLineInStaffClone (
        inputLineNumber,
        positionInMeasure);
  } // for

  --gIndenter;
}

void msrPart::appendTransposeToPart (
  S_msrTranspose transpose)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceTranspositions ()) {
    gLogStream <<
      "Appending transpose \"" <<
      transpose->asString () <<
      "\" to part " << getPartCombinedName () <<
    endl;
  }
#endif

  // set part current transpose
  fPartCurrentTranspose = transpose;

  // cascade it to all staves
  for (S_msrStaff staff : fPartAllStavesList) {
    staff->
      appendTransposeToStaff (transpose);
  } // for
}

/* JMI
void msrPart::nestContentsIntoNewRepeatInPart (
  int inputLineNumber)
{
  for (S_msrStaff staff : fPartAllStavesList) {
    (*i)->
      nestContentsIntoNewRepeatInStaff (
        inputLineNumber);
  } // for
}
*/

void msrPart::handleRepeatStartInPart (
  int inputLineNumber)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeats ()) {
    gLogStream <<
      "Handling a repeat start in part \"" <<
      getPartCombinedName () <<
      "\", line " << inputLineNumber <<
      endl;
  }
#endif

  ++gIndenter;

  // cascade to all the staves
  for (S_msrStaff staff : fPartAllStavesList) {
    staff->
      handleRepeatStartInStaff (
        inputLineNumber);
  } // for

  --gIndenter;
}

void msrPart::handleRepeatEndInPart (
  int           inputLineNumber,
  const string& measureNumber,
  int           repeatTimes)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeats ()) {
    gLogStream <<
      "Handling a repeat end in part \"" <<
      getPartCombinedName () <<
      "\"" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  ++gIndenter;

  // cascade to all the staves
  for (S_msrStaff staff : fPartAllStavesList) {
    staff->
      handleRepeatEndInStaff (
        inputLineNumber,
        measureNumber,
        repeatTimes);
  } // for

  --gIndenter;
}

void msrPart::handleRepeatEndingStartInPart (
  int    inputLineNumber)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeats ()) {
    gLogStream <<
      "Handling a repeat ending start in part \"" <<
      getPartCombinedName () <<
      "\"" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  ++gIndenter;

  // cascade to all the staves
  for (S_msrStaff staff : fPartAllStavesList) {
    staff->
      handleRepeatEndingStartInStaff (
        inputLineNumber);
  } // for

  --gIndenter;
}

void msrPart::handleRepeatEndingEndInPart (
  int           inputLineNumber,
  const string& repeatEndingNumber, // may be "1, 2"
  msrRepeatEndingKind
                repeatEndingKind)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeats ()) {
    gLogStream <<
      "Handling a " <<
      msrRepeatEndingKindAsString (
        repeatEndingKind) <<
      " repeat ending end in part \"" <<
      getPartCombinedName () <<
      "\"" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  ++gIndenter;

  // cascade to all the staves
  for (S_msrStaff staff : fPartAllStavesList) {
    staff->
      handleRepeatEndingEndInStaff (
        inputLineNumber,
        repeatEndingNumber,
        repeatEndingKind);
  } // for

  --gIndenter;
}

/* JMI
void msrPart::finalizeRepeatEndInPart (
  int    inputLineNumber,
  const string& measureNumber,
  int    repeatTimes)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeats ()) {
    gLogStream <<
      "Finalizing a repeat upon its end in part \"" <<
      getPartCombinedName () <<
      "\"" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  ++gIndenter;

  // create repeat and append it to registered staves
  for (S_msrStaff staff : fPartAllStavesList) {
    staff->
      finalizeRepeatEndInStaff (
        inputLineNumber,
        measureNumber,
        repeatTimes);
  } // for

  --gIndenter;
}
*/

void msrPart::appendRepeatCloneToPart (
  int         inputLineNumber,
  S_msrRepeat repeatCLone)
{
  for (S_msrStaff staff : fPartAllStavesList) {
    switch (staff->getStaffKind ()) {
      case msrStaffKind::kStaffKindRegular:
        staff->appendRepeatCloneToStaff (
          inputLineNumber, repeatCLone);
        break;

      case msrStaffKind::kStaffKindTablature:
        break;

      case msrStaffKind::kStaffKindHarmonies:
        break;

      case msrStaffKind::kStaffKindFiguredBass:
        break;

      case msrStaffKind::kStaffKindDrum:
        staff->appendRepeatCloneToStaff (
          inputLineNumber, repeatCLone);
        break;

      case msrStaffKind::kStaffKindRythmic:
        staff->appendRepeatCloneToStaff (
          inputLineNumber, repeatCLone);
        break;
    } // switch
  } // for
}

void msrPart::appendRepeatEndingCloneToPart (
  S_msrRepeatEnding repeatEndingCLone)
{
  for (S_msrStaff staff : fPartAllStavesList) {
    staff->
      appendRepeatEndingCloneToStaff (repeatEndingCLone);
  } // for
}

void msrPart::createMeasureRepeatFromItsFirstMeasuresInPart (
  int inputLineNumber,
  int measureRepeatMeasuresNumber,
  int measureRepeatSlashes)
{
  // create measures repeat from its first measure in all staves
  for (S_msrStaff staff : fPartAllStavesList) {
    staff->
      createMeasureRepeatFromItsFirstMeasuresInStaff (
        inputLineNumber,
        measureRepeatMeasuresNumber,
        measureRepeatSlashes);
  } // for
}

void msrPart::appendPendingMeasureRepeatToPart (
  int inputLineNumber)
{
  // append pending measures repeat to all staves
  for (S_msrStaff staff : fPartAllStavesList) {
    staff->
      appendPendingMeasureRepeatToStaff (
        inputLineNumber);
  } // for
}

void msrPart::createMultipleFullBarRestsInPart (
  int inputLineNumber,
  int multipleMultipleFullBarRestsMeasuresNumber)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMultipleFullBarRests ()) {
    gLogStream <<
      "Creating " <<
      mfSingularOrPlural (
        multipleMultipleFullBarRestsMeasuresNumber, "multiple full-bar rest", "multiple full-bar rests") <<
      " in part " <<
      getPartCombinedName () <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  fPartContainsMultipleFullBarRests = true;

  // create multiple rest in all staves
  for (S_msrStaff staff : fPartAllStavesList) {
    staff->
      createMultipleFullBarRestsInStaff (
        inputLineNumber,
        multipleMultipleFullBarRestsMeasuresNumber);
  } // for
}

void msrPart::replicateLastAppendedMeasureInPart (
  int inputLineNumber,
  int replicatasNumber)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMultipleFullBarRests ()) {
    gLogStream <<
      "Replicating last appended measure in part " <<
      getPartCombinedName () <<
      endl;
  }
#endif

  // add multiple rest to all staves
  for (S_msrStaff staff : fPartAllStavesList) {
    staff->
      replicateLastAppendedMeasureInStaff (
        inputLineNumber,
        replicatasNumber);
  } // for
}

void msrPart::addEmptyMeasuresToPart (
  int           inputLineNumber,
  const string& previousMeasureNumber,
  int           multipleFullBarRestsNumber)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMultipleFullBarRests ()) {
    gLogStream <<
      "Adding " <<
      mfSingularOrPlural (
        multipleFullBarRestsNumber, "multiple full-bar rest", "multiple full-bar rests") <<
      " to part " <<
      getPartCombinedName () <<
      ", " <<
      endl;
  }
#endif

  fPartContainsMultipleFullBarRests = true;

  // add multiple rest to all staves
  for (S_msrStaff staff : fPartAllStavesList) {
    staff->
      addEmptyMeasuresToStaff (
        inputLineNumber,
        previousMeasureNumber,
        multipleFullBarRestsNumber);
  } // for
}

void msrPart::appendPendingMultipleFullBarRestsToPart (
  int inputLineNumber)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMultipleFullBarRests ()) {
    gLogStream <<
      "Appending the pending multiple rest to part " <<
      getPartCombinedName () <<
      endl;
  }
#endif

  // append pending multiple rest to all staves
  for (S_msrStaff staff : fPartAllStavesList) {
    staff->
      appendPendingMultipleFullBarRestsToStaff (
        inputLineNumber);
  } // for
}

void msrPart::appendMultipleFullBarRestsCloneToPart (
  int               inputLineNumber,
  S_msrMultipleFullBarRests multipleMultipleFullBarRests)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMultipleFullBarRests ()) {
    gLogStream <<
      "Appending multiple rest '" <<
      multipleMultipleFullBarRests->asString () <<
      "' to part clone " <<
      getPartCombinedName () <<
      endl;
  }
#endif

  for (S_msrStaff staff : fPartAllStavesList) {
    staff->
      appendMultipleFullBarRestsCloneToStaff (
        inputLineNumber,
        multipleMultipleFullBarRests);
  } // for
}

void msrPart::appendBarLineToPart (S_msrBarLine barLine)
{
 #ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceBarLines ()) {
    gLogStream <<
      "Appending barLine " << barLine->asString () <<
      " to part " <<
      getPartCombinedName () <<
      endl;
  }
#endif

 // append barLine to registered staves
  for (S_msrStaff staff : fPartAllStavesList) {
    staff->
      appendBarLineToStaff (barLine);
  } // for
}

S_msrStaff msrPart::addStaffToPartByItsNumber (
  int          inputLineNumber,
  msrStaffKind staffKind,
  int          staffNumber)
{
  if (fPartStaveNumbersToStavesMap.count (staffNumber)) {
    stringstream s;

    s <<
      "staffNumber " << staffNumber <<
      " already exists in part " << getPartCombinedName () <<
      ", line " << inputLineNumber;

    msrInternalError ( // JMI ???
      gGlobalServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());

    return fPartStaveNumbersToStavesMap [staffNumber];
  }

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceStaves ()) {
    gLogStream <<
      "Adding " <<
      msrStaffKindAsString (staffKind) <<
      " staff " << staffNumber <<
      " to part " << getPartCombinedName () <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  // create the staff
  S_msrStaff
    staff =
      msrStaff::create (
        inputLineNumber,
        staffKind,
        staffNumber,
        this);

  // register staff in this part
  registerStaffInPart (
    staff);

  // initialize staff current clef and key if relevant // JMI
  switch (staffKind) {
    case msrStaffKind::kStaffKindRegular:
    case msrStaffKind::kStaffKindTablature:
    case msrStaffKind::kStaffKindHarmonies:
    case msrStaffKind::kStaffKindFiguredBass:
    case msrStaffKind::kStaffKindDrum:
    case msrStaffKind::kStaffKindRythmic:
      staff->setStaffCurrentClef (fPartCurrentClef);
      staff->setStaffCurrentKey (fPartCurrentKey);
      break;
  } // switch

  return staff;
}

S_msrStaff msrPart::addHarmoniesStaffToPart (
  int inputLineNumber)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceStaves ()) {
    gLogStream <<
      "Adding harmonies staff " <<
      " to part " << getPartCombinedName () <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  // create the staff
  int partHarmoniesStaffNumber =
    msrPart::K_PART_HARMONIES_STAFF_NUMBER;

  S_msrStaff
    staff =
      msrStaff::create (
        inputLineNumber,
        msrStaffKind::kStaffKindHarmonies,
        partHarmoniesStaffNumber,
        this);

  // register staff in this part
  registerStaffInPart (
    staff);

/* JMI
  // initialize staff current clef and key if relevant // JMI
  switch (staffKind) {
    case msrStaffKind::kStaffKindRegular:
    case msrStaffKind::kStaffKindTablature:
    case msrStaffKind::kStaffKindHarmonies:
    case msrStaffKind::kStaffKindFiguredBass:
    case msrStaffKind::kStaffKindDrum:
    case msrStaffKind::kStaffKindRythmic:
      staff->setStaffCurrentClef (fPartCurrentClef);
      staff->setStaffCurrentKey (fPartCurrentKey);
      break;
  } // switch
*/

  return staff;
}

S_msrStaff msrPart::addHFiguredBassStaffToPart (
  int inputLineNumber)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceStaves ()) {
    gLogStream <<
      "Adding figured bass staff " <<
      " to part " << getPartCombinedName () <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  // create the staff
  int partFiguredBassStaffNumber =
    msrPart::K_PART_FIGURED_BASS_STAFF_NUMBER;

  S_msrStaff
    staff =
      msrStaff::create (
        inputLineNumber,
        msrStaffKind::kStaffKindFiguredBass,
        partFiguredBassStaffNumber,
        this);

  // register staff in this part
  registerStaffInPart (
   staff);

/* JMI
  // initialize staff current clef and key if relevant // JMI
  switch (staffKind) {
    case msrStaffKind::kStaffKindRegular:
    case msrStaffKind::kStaffKindTablature:
    case msrStaffKind::kStaffKindHarmonies:
    case msrStaffKind::kStaffKindFiguredBass:
    case msrStaffKind::kStaffKindDrum:
    case msrStaffKind::kStaffKindRythmic:
      staff->setStaffCurrentClef (fPartCurrentClef);
      staff->setStaffCurrentKey (fPartCurrentKey);
      break;
  } // switch
*/

  return staff;
}

void msrPart::addStaffToPartCloneByItsNumber (S_msrStaff staff)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceStaves ()) {
    gLogStream <<
      "Adding staff \"" << staff->getStaffName () <<
      "\" to part clone " << getPartCombinedName () <<
      endl;
  }
#endif

  // register staff in this part
  registerStaffInPart (
    staff);
}

S_msrStaff msrPart::fetchStaffFromPart (
  int staffNumber)
{
  S_msrStaff result;

  if (fPartStaveNumbersToStavesMap.count (staffNumber)) {
    result =
      fPartStaveNumbersToStavesMap [staffNumber];
  }

  return result;
}

void msrPart::registerVoiceInPartAllVoicesList (
  S_msrVoice voice)
{
  // register voice in this staff
  fPartAllVoicesList.push_back (voice);

  // register it in the partgroup uplink
  fPartPartGroupUpLink->
    registerVoiceInPartGroupAllVoicesList (voice);
}

S_msrVoice msrPart::createPartHarmoniesVoice (
  int           inputLineNumber,
  const string& currentMeasureNumber)
{
  if (fPartHarmoniesVoice) {
    stringstream s;

    s <<
      "Part \"" <<
      getPartCombinedName () <<
      "\" already has a harmonies voice";

    msrInternalError (
      gGlobalServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }

  // create the part harmonies staff
#ifdef TRACING_IS_ENABLED
  int partHarmoniesStaffNumber =
    msrPart::K_PART_HARMONIES_STAFF_NUMBER;

  if (gGlobalTracingOahGroup->getTraceHarmonies ()) {
    gLogStream <<
      "Creating harmonies staff for part \"" <<
      getPartCombinedName () <<
      "\" with staff number " <<
      partHarmoniesStaffNumber <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  fPartHarmoniesStaff =
    addHarmoniesStaffToPart (
      inputLineNumber);

  // create the part harmonies voice
  int partHarmoniesVoiceNumber =
    msrPart::K_PART_HARMONIES_VOICE_NUMBER;

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceHarmonies ()) {
    gLogStream <<
      "Creating harmonies voice for part \"" <<
      getPartCombinedName () <<
      "\" with voice number " <<
      partHarmoniesVoiceNumber <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  fPartHarmoniesVoice =
    msrVoice::create (
      inputLineNumber,
      msrVoiceKind::kVoiceKindHarmonies,
      partHarmoniesVoiceNumber,
      msrVoiceCreateInitialLastSegmentKind::kCreateInitialLastSegmentYes,
      fPartHarmoniesStaff);

  // register the part harmonies voice in part harmonies staff
  fPartHarmoniesStaff->
    registerVoiceInStaff (
      inputLineNumber,
      fPartHarmoniesVoice);

/* JMI
  // set backward link
  fPartHarmoniesVoice->
    setHarmoniesVoicePartBackwardLink (this);
*/

  return fPartHarmoniesVoice;
}

void msrPart::appendHarmonyToPart ( // UNUSED JMI
  S_msrVoice   harmonySupplierVoice,
  S_msrHarmony harmony)
{
  int inputLineNumber =
    harmony->getInputLineNumber ();

  ++gIndenter;

  switch (harmonySupplierVoice->getVoiceKind ()) {
    case msrVoiceKind::kVoiceKindRegular:
      // append the harmony to the part harmonies voice
#ifdef TRACING_IS_ENABLED
      if (gGlobalTracingOahGroup->getTraceHarmonies ()) {
        gLogStream <<
          "Appending harmony " <<
          harmony->asString () <<
          " to part " <<
          getPartCombinedName () <<
          ", line " << inputLineNumber <<
          endl;
      }
#endif

      fPartHarmoniesVoice->
        appendHarmonyToVoice (harmony);
      break;

    case msrVoiceKind::kVoiceKindDynamics:
      break;

    case msrVoiceKind::kVoiceKindHarmonies:
    case msrVoiceKind::kVoiceKindFiguredBass:
      {
        stringstream s;

        s <<
          "harmony cannot be appended to part by " <<
          msrVoiceKindAsString (
            harmonySupplierVoice->getVoiceKind ()) <<
          " voice \" " <<
          harmonySupplierVoice->getVoiceName () <<
          "\"";

        msrInternalError (
          gGlobalServiceRunData->getInputSourceName (),
          inputLineNumber,
          __FILE__, __LINE__,
          s.str ());
      }
      break;
  } // switch

  --gIndenter;
}

void msrPart::appendHarmonyToPartClone (
  S_msrVoice   harmonySupplierVoice,
  S_msrHarmony harmony)
{
  int inputLineNumber =
    harmony->getInputLineNumber ();

  ++gIndenter;

  switch (harmonySupplierVoice->getVoiceKind ()) {
    case msrVoiceKind::kVoiceKindFiguredBass:
      // append the harmony to the part harmonies voice
#ifdef TRACING_IS_ENABLED
      if (gGlobalTracingOahGroup->getTraceHarmonies ()) {
        gLogStream <<
          "Appending harmony " <<
          harmony->asString () <<
          " to part clone " <<
          getPartCombinedName () <<
          ", line " << inputLineNumber <<
          endl;
      }
#endif

      fPartHarmoniesVoice->
        appendHarmonyToVoiceClone (harmony);
      break;

    case msrVoiceKind::kVoiceKindDynamics:
      break;

    case msrVoiceKind::kVoiceKindRegular:
    case msrVoiceKind::kVoiceKindHarmonies:
      {
        stringstream s;

        s <<
          "harmony cannot by appended to part clone by " <<
          msrVoiceKindAsString (
            harmonySupplierVoice->getVoiceKind ()) <<
          " voice \" " <<
          harmonySupplierVoice->getVoiceName () <<
          "\"";

        msrInternalError (
          gGlobalServiceRunData->getInputSourceName (),
          inputLineNumber,
          __FILE__, __LINE__,
          s.str ());
      }
      break;
  } // switch

  --gIndenter;
}

S_msrVoice msrPart::createPartFiguredBassVoice (
  int           inputLineNumber,
  const string& currentMeasureNumber)
{
  if (fPartFiguredBassVoice) {
    stringstream s;

    s <<
      "Part \"" <<
      getPartCombinedName () <<
      "\" already has a figured bass voice";

    msrInternalError (
      gGlobalServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }

  // create the part figured bass staff
#ifdef TRACING_IS_ENABLED
  int partFiguredBassStaffNumber =
    msrPart::K_PART_FIGURED_BASS_STAFF_NUMBER;

  if (gGlobalTracingOahGroup->getTraceFiguredBass ()) {
    gLogStream <<
      "Creating figured bass staff for part \"" <<
      getPartCombinedName () <<
      "\" with staff number " <<
      partFiguredBassStaffNumber <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  fPartFiguredBassStaff =
    addHFiguredBassStaffToPart (
      inputLineNumber);

  // create the figured bass voice
  int partFiguredBassVoiceNumber =
    msrPart::K_PART_FIGURED_BASS_VOICE_NUMBER;

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceFiguredBass ()) {
    gLogStream <<
      "Creating figured bass voice for part \"" <<
      getPartCombinedName () <<
      "\" with voice number " <<
      partFiguredBassVoiceNumber <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  fPartFiguredBassVoice =
    msrVoice::create (
      inputLineNumber,
      msrVoiceKind::kVoiceKindFiguredBass,
      partFiguredBassVoiceNumber,
      msrVoiceCreateInitialLastSegmentKind::kCreateInitialLastSegmentYes,
      fPartFiguredBassStaff);

  // register the figured bass voice in the part figured bass staff
  fPartFiguredBassStaff->
    registerVoiceInStaff (
      inputLineNumber,
      fPartFiguredBassVoice);

/* JMI
  // set backward link
  fPartFiguredBassVoice->
    setFiguredBassVoicePartBackwardLink (this);
*/

  return fPartFiguredBassVoice;
}

void msrPart::appendFiguredBassElementToPart (
  S_msrVoice              figuredBassSupplierVoice,
  S_msrFiguredBassElement figuredBassElement)
{
  int inputLineNumber =
    figuredBassElement->getInputLineNumber ();

  ++gIndenter;

  switch (figuredBassSupplierVoice->getVoiceKind ()) {
    case msrVoiceKind::kVoiceKindRegular:
      // append the figured bass to the part figured bass voice
#ifdef TRACING_IS_ENABLED
      if (gGlobalTracingOahGroup->getTraceFiguredBass ()) {
        gLogStream <<
          "Appending figured bass " <<
          figuredBassElement->asString () <<
          " to part " <<
          getPartCombinedName () <<
          ", line " << inputLineNumber <<
          endl;
      }
#endif

      fPartFiguredBassVoice->
        appendFiguredBassElementToVoice (figuredBassElement);
      break;

    case msrVoiceKind::kVoiceKindDynamics:
      break;

    case msrVoiceKind::kVoiceKindHarmonies:
    case msrVoiceKind::kVoiceKindFiguredBass:
      {
        stringstream s;

        s <<
          "figured bass cannot by appended to part by " <<
          msrVoiceKindAsString (
            figuredBassSupplierVoice->getVoiceKind ()) <<
          " voice \" " <<
          figuredBassSupplierVoice->getVoiceName () <<
          "\"";

        msrInternalError (
          gGlobalServiceRunData->getInputSourceName (),
          inputLineNumber,
          __FILE__, __LINE__,
          s.str ());
      }
      break;
  } // switch

  --gIndenter;
}

void msrPart::appendFiguredBassElementToPartClone (
  S_msrVoice              figuredBassSupplierVoice,
  S_msrFiguredBassElement figuredBassElement)
{
  int inputLineNumber =
    figuredBassElement->getInputLineNumber ();

  ++gIndenter;

  switch (figuredBassSupplierVoice->getVoiceKind ()) {
    case msrVoiceKind::kVoiceKindFiguredBass:
      // append the figured bass to the part figured bass voice
#ifdef TRACING_IS_ENABLED
      if (gGlobalTracingOahGroup->getTraceFiguredBass ()) {
        gLogStream <<
          "Appending figured bass " <<
          figuredBassElement->asString () <<
          " to part clone " <<
          getPartCombinedName () <<
          ", line " << inputLineNumber <<
          endl;
      }
#endif

      fPartFiguredBassVoice->
        appendFiguredBassElementToVoiceClone (figuredBassElement);
      break;

    case msrVoiceKind::kVoiceKindDynamics:
      break;

    case msrVoiceKind::kVoiceKindRegular:
    case msrVoiceKind::kVoiceKindHarmonies:
      {
        stringstream s;

        s <<
          "figured bass cannot by appended to part clone by " <<
          msrVoiceKindAsString (
            figuredBassSupplierVoice->getVoiceKind ()) <<
          " voice \" " <<
          figuredBassSupplierVoice->getVoiceName () <<
          "\"";

        msrInternalError (
          gGlobalServiceRunData->getInputSourceName (),
          inputLineNumber,
          __FILE__, __LINE__,
          s.str ());
      }
      break;
  } // switch

  --gIndenter;
}

void msrPart::appendScordaturaToPart (
  S_msrScordatura scordatura)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceParts ()) {
    gLogStream <<
      "Appending scordatura '" <<
      scordatura->asString () <<
      "' to part " <<
      getPartCombinedName () <<
      endl;
  }
#endif

  for (S_msrStaff staff : fPartRegularStavesList) {
    staff->
      appendScordaturaToStaff (
        scordatura);
  } // for
}

void msrPart::appendAccordionRegistrationToPart (
  S_msrAccordionRegistration
    accordionRegistration)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceParts ()) {
    gLogStream <<
      "Appending accordion registration '" <<
      accordionRegistration->asString () <<
      "' to part " <<
      getPartCombinedName () <<
      endl;
  }
#endif

  for (S_msrStaff staff : fPartRegularStavesList) {
    staff->
      appendAccordionRegistrationToStaff (
        accordionRegistration);
  } // for
}

void msrPart::appendHarpPedalsTuningToPart (
  S_msrHarpPedalsTuning
    harpPedalsTuning)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceParts ()) {
    gLogStream <<
      "Appending harp pedals tuning '" <<
      harpPedalsTuning->asString () <<
      "' to part " <<
      getPartCombinedName () <<
      endl;
  }
#endif

  for (S_msrStaff staff : fPartRegularStavesList) {
    staff->
      appendHarpPedalsTuningToStaff (
        harpPedalsTuning);
  } // for
}

void msrPart::addSkipGraceNotesGroupBeforeAheadOfVoicesClonesIfNeeded (
  S_msrVoice           graceNotesGroupOriginVoice,
  S_msrGraceNotesGroup skipGraceNotesGroup)
{
#ifdef TRACING_IS_ENABLED
  int inputLineNumber =
    skipGraceNotesGroup->getInputLineNumber ();
#endif

#ifdef TRACING_IS_ENABLED
  if (
    gGlobalTracingOahGroup->getTraceMeasures ()
      ||
    gGlobalTracingOahGroup->getTraceGraceNotes ()
      ||
    gGlobalTracingOahGroup->getTraceParts ()
    ) {
    gLogStream <<
      "addSkipGraceNotesGroupBeforeAheadOfVoicesClonesIfNeeded () in " <<
      getPartCombinedName () <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  for (S_msrStaff staff : fPartAllStavesList) {
    map<int, S_msrVoice>
      staffAllVoicesMap =
        staff->
          getStaffVoiceNumbersToAllVoicesMap ();

    for (pair<int, S_msrVoice> thePair : staffAllVoicesMap) {
      S_msrVoice voice = thePair.second;

      if (voice != graceNotesGroupOriginVoice) {
        // add skip grace notes group ahead of voice
        voice->
          addGraceNotesGroupBeforeAheadOfVoiceIfNeeded (
            skipGraceNotesGroup);
      }
    } // for
  } // for
}

void msrPart::handleBackupInPart (
  int             inputLineNumber,
  const rational& backupStepLength)
{
  // account for backup in part
  decrementPartCurrentPositionInMeasure (
    inputLineNumber,
    backupStepLength);
}

void msrPart::finalizeLastAppendedMeasureInPart (
  int    inputLineNumber)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasures ()) {
    gLogStream <<
      "Finalizing last appended measure in part " <<
      getPartCombinedName () <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  ++gIndenter;

  // reset current position in measure
  fPartCurrentPositionInMeasure = rational (0,0);

  // finalize current measure in all staves
  for (S_msrStaff staff : fPartAllStavesList) {
    staff->
      finalizeLastAppendedMeasureInStaff (
        inputLineNumber);
  } // for

  --gIndenter;
}

void msrPart::setPartInstrumentNamesMaxLengthes ()
{
  S_msrScore
    score =
      fPartPartGroupUpLink->
        getPartGroupScoreUpLink ();

  size_t partInstrumentNameLength =
    fPartInstrumentName.size ();

  if (
    partInstrumentNameLength
      >
    score->getScoreInstrumentNamesMaxLength ()
  ) {
    score->
      setIdentificationScoreInstrumentNamesMaxLength (
        partInstrumentNameLength);
  }

  size_t partInstrumentAbbreviationLength =
    fPartInstrumentAbbreviation.size ();

  if (
    partInstrumentAbbreviationLength
      >
    score->getScoreInstrumentAbbreviationsMaxLength ()
  ) {
    score->
      setIdentificationScoreInstrumentAbbreviationsMaxLength (
        partInstrumentAbbreviationLength);
  }
}

bool msrPart::compareStavesToHaveFiguredBassElementsBelowCorrespondingPart (
  const S_msrStaff& first,
  const S_msrStaff& second)
{
  int
    firstStaffNumber =
      first->getStaffNumber (),
    secondStaffNumber =
      second->getStaffNumber ();

  if (firstStaffNumber > msrPart::K_PART_FIGURED_BASS_STAFF_NUMBER) {
    firstStaffNumber -= msrPart::K_PART_FIGURED_BASS_STAFF_NUMBER + 1;
  }
  if (secondStaffNumber > msrPart::K_PART_FIGURED_BASS_STAFF_NUMBER) {
    secondStaffNumber -= msrPart::K_PART_FIGURED_BASS_STAFF_NUMBER + 1;
  }

  bool result =
    firstStaffNumber > secondStaffNumber;

  return result;
}

void msrPart::finalizePart (
  int inputLineNumber)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceParts ()) {
    gLogStream <<
      "Finalizing part " <<
      getPartCombinedName () <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  ++gIndenter;

  if (! fPartAllStavesList.size ()) {
    stringstream s;

    s <<
      "Part " <<
      getPartCombinedName () <<
      " appears in the part list, but doesn't contain any stave";

    msrWarning (
      gGlobalServiceRunData->getInputSourceName (),
      inputLineNumber,
      s.str ());
  }

  else {
    // sort the staves to have harmonies above and
    // figured bass below the part
    fPartAllStavesList.sort (
      compareStavesToHaveFiguredBassElementsBelowCorrespondingPart);

    // finalize all the staves
  for (S_msrStaff staff : fPartAllStavesList) {
    staff->
      finalizeStaff (
        inputLineNumber);
    } // for
  }

  // set score instrument names max lengthes if relevant
  setPartInstrumentNamesMaxLengthes ();

  // collect the part measures slices from the staves
  collectPartMeasuresSlices (
    inputLineNumber);

  --gIndenter;
}

void msrPart::finalizePartClone (
  int inputLineNumber)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceParts ()) {
    gLogStream <<
      "Finalizing part clone " <<
      getPartCombinedName () <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

// JMI CAFE ???
    // sort the staves to have harmonies above and figured bass below the part
    fPartAllStavesList.sort (
      compareStavesToHaveFiguredBassElementsBelowCorrespondingPart);

    // finalize all the staves
  for (S_msrStaff staff : fPartAllStavesList) {
    staff->
      finalizeStaff (
        inputLineNumber);
    } // for

  // set score instrument names max lengthes if relevant
  setPartInstrumentNamesMaxLengthes ();

  // collect the part measures slices from the staves
  collectPartMeasuresSlices (
    inputLineNumber);
}

void msrPart::finalizePartAndAllItsMeasures (
  int inputLineNumber)
{
  return; // JMI TEMP NOEL

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceParts ()) {
    gLogStream <<
      "Finalizing all the measures of part \"" <<
      getPartCombinedName () <<
      "\", line " << inputLineNumber <<
      endl;
  }
#endif

  for (S_msrVoice voice : fPartAllVoicesList) {
    voice->
      finalizeVoiceAndAllItsMeasures (
        inputLineNumber);
  } // for

  // collect the part measures slices from the staves
  collectPartMeasuresSlices (
    inputLineNumber);
}

void msrPart::collectPartMeasuresSlices (
  int inputLineNumber)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasuresSlices ()) {
    size_t
      partAllStavesListSize =
        fPartAllStavesList.size ();

    gLogStream <<
      "Collecting the measures slices of part \"" <<
      getPartCombinedName () <<
      "\", " <<
      mfSingularOrPluralWithoutNumber (
        partAllStavesListSize, "there is", "there are") <<
      ' ' <<
      mfSingularOrPlural (
        partAllStavesListSize, "voice", "voices") <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    fPartMeasuresSlicesSequence == nullptr,
    "fPartMeasuresSlicesSequence is not null");

  ++gIndenter;

  // create the part measures slices sequence
  fPartMeasuresSlicesSequence =
    msrMeasuresSlicesSequence::create (
      fPartName); // origin

  // populate it
  for (S_msrStaff staff : fPartAllStavesList) {
#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceMeasuresSlices ()) {
      gLogStream <<
        "---> staff \"" <<
        staff->getStaffName () <<
        "\":" <<
        endl;
    }
#endif

    ++gIndenter;

    S_msrMeasuresSlicesSequence
      staffMeasuresSlicesSequence =
        staff->
          getStaffMeasuresSlicesSequence ();

    if (! staffMeasuresSlicesSequence) {
      stringstream s;

      s <<
        "The staffMeasuresSlicesSequence of staff \"" <<
        staff->getStaffName () <<
        "\" is null";

      msrWarning (
        gGlobalServiceRunData->getInputSourceName (),
        inputLineNumber,
        s.str ());
    }
    else {
      fPartMeasuresSlicesSequence->
        mergeWithMeasuresSlicesSequence (
          inputLineNumber,
          getPartCombinedName (),
          staffMeasuresSlicesSequence);
    }

    --gIndenter;
  } // for

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasuresSlices ()) {
    gLogStream <<
      "fPartMeasuresSlicesSequence:" <<
      endl;

    ++gIndenter;
    gLogStream <<
      fPartMeasuresSlicesSequence;
    --gIndenter;
  }
#endif

  --gIndenter;
}

void msrPart::acceptIn (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrPart::acceptIn ()" <<
      endl;
  }

  if (visitor<S_msrPart>*
    p =
      dynamic_cast<visitor<S_msrPart>*> (v)) {
        S_msrPart elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrPart::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrPart::acceptOut (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrPart::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrPart>*
    p =
      dynamic_cast<visitor<S_msrPart>*> (v)) {
        S_msrPart elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrPart::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrPart::browseData (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrPart::browseData ()" <<
      endl;
  }

#ifdef TRACING_IS_ENABLED // JMI
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) { // JMI TEMP
    gLogStream <<
      "++++++++ fPartAllStavesList.size(): " <<
      fPartAllStavesList.size () <<
      endl;

    if (fPartAllStavesList.size ()) {
      for (S_msrStaff staff : fPartAllStavesList) {
        gLogStream <<
          endl <<
          "+++++++++ staff: ++++++++" <<
          " \"" << staff->getStaffName () << "\"" <<
          endl;
      } // for
    }

    gLogStream <<
      "++++++++ fPartNonHarmoniesNorFiguredBassStavesList.size(): " <<
      fPartNonHarmoniesNorFiguredBassStavesList.size () <<
      endl;

    if (fPartNonHarmoniesNorFiguredBassStavesList.size ()) {
      for (S_msrStaff staff : fPartNonHarmoniesNorFiguredBassStavesList) {
        gLogStream <<
          endl <<
          "+++++++++ staff: ++++++++" <<
          " \"" << staff->getStaffName () << "\"" <<
          endl;
      } // for
    }
  }
#endif

  /* don't enforce any order here, leave it to the client thru sorting ??? JMI */

  // browse the part harmonies staff if any right now, JMI
  // to place it before the corresponding part
  if (fPartHarmoniesStaff) {
    msrBrowser<msrStaff> browser (v);
    browser.browse (*fPartHarmoniesStaff);
  }

  // browse all non harmonies and non figured bass staves
  for (S_msrStaff staff : fPartNonHarmoniesNorFiguredBassStavesList) {
    // browse the staff
    msrBrowser<msrStaff> browser (v);
    browser.browse (*staff);
  } // for

  // browse the part figured bass staff if any only now, JMI
  // to place it after the corresponding part
  if (fPartFiguredBassStaff) {
    msrBrowser<msrStaff> browser (v);
    browser.browse (*fPartFiguredBassStaff);
  }

//   // browse all the part staves JMI ---> this leads to 4 HelloWorld scores... v0.9.62
//   for (S_msrStaff staff : fPartAllStavesList) {
//     if (staff != fPartHarmoniesStaff && staff != fPartFiguredBassStaff) {
//       // browse the staff
//       msrBrowser<msrStaff> browser (v);
//       browser.browse (*staff);
//     }
//   } // for
}

string msrPart::asString () const
{
  stringstream s;

  s <<
    "Part" <<
    ", partID = \"" <<
    fPartID <<
    "\", partName = \"" <<
    fPartName <<
    "\", partMsrName: " << fPartMsrName <<
    fPartName <<
    "\", partStaves: " << fPartAllStavesList.size () <<
    ", line " << fInputLineNumber;

  return s.str ();
}

void msrPart::printPartMeasuresWholeNotesDurationsVector (
  ostream& os,
  int      fieldWidth) const
{
  os << left <<
    setw (fieldWidth) <<
    "fPartMeasuresWholeNotesDurationsVector" << " : " ;

  if (fPartNumberOfMeasures == 0) {
    os << "empty" << endl;
  }
  else {
    os << endl;

    ++gIndenter;

    for (size_t i = 0; i < fPartNumberOfMeasures; ++i) {
      int j = i + 1; // indices start at 0

      os << left <<
        "ordinal number " <<
        setw (3) << right <<
        j << " : " <<
        setw (4) <<
        fPartMeasuresWholeNotesDurationsVector [ i ].toString ();

      if (i % 3 == 1) {
        os << endl;
      }

      os << endl;
    } // for

    --gIndenter;
  }
}

void msrPart::print (ostream& os) const
{
  os <<
    "[Part" << ' ' << fPartMsrName <<
    " (" <<
    mfSingularOrPlural (
      fPartAllStavesList.size (), "staff", "staves") <<
    ")" <<
    ", line " << fInputLineNumber <<
    endl;

  ++gIndenter;

  const int fieldWidth = 37;

  os << left <<
    setw (fieldWidth) <<
    "fPartGroupUpLink" << " : ";
  if (fPartPartGroupUpLink) {
    // it may be empty
    os <<
      fPartPartGroupUpLink->getPartGroupCombinedName ();
  }
  else {
    os << "none";
  }
  os << endl;

  os << left <<
    setw (fieldWidth) <<
    "fPartID" << " : \"" <<
    fPartID << "\"" <<
    endl <<

    setw (fieldWidth) <<
    "fPartMsrName" << " : \"" <<
    fPartMsrName << "\"" <<
    endl <<

    setw (fieldWidth) <<
    "fPartAbsoluteNumber" << " : " <<
    fPartAbsoluteNumber <<
    endl <<

    setw (fieldWidth) <<
    "fPartName" << " : \"" <<
    fPartName << "\"" <<
    endl <<
    setw (fieldWidth) <<
    "fPartNameDisplayText" << " : \"" <<
    fPartNameDisplayText << "\"" <<
    endl <<

    setw (fieldWidth) <<
    "fPartAbbrevation" << " : \"" <<
    fPartAbbreviation << "\"" <<
    endl <<
    setw (fieldWidth) <<
    "fPartAbbreviationDisplayText" << " : \"" <<
    fPartAbbreviationDisplayText << "\"" <<
    endl <<

    setw (fieldWidth) <<
    "fPartInstrumentName" << " : \"" <<
    fPartInstrumentName << "\"" <<
    endl <<
    setw (fieldWidth) <<
    "fPartInstrumentAbbreviation" << " : \"" <<
    fPartInstrumentAbbreviation << "\"" <<
    endl <<

    setw (fieldWidth) <<
    "fPartNumberOfMeasures" << " : " <<
    fPartNumberOfMeasures <<
    endl <<

    setw (fieldWidth) <<
    "fPartContainsMultipleFullBarRests" << " : " <<
    fPartContainsMultipleFullBarRests <<
    endl <<

    setw (fieldWidth) <<
    "fPartCurrentPositionInMeasure" << " : " <<
    fPartCurrentPositionInMeasure <<
    endl;

  // print current the part clef if any
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceClefs ()) {
    os << left <<
      setw (fieldWidth) <<
      "fPartCurrentClef" << " : ";

    if (fPartCurrentClef) {
      os <<
        "'" <<
        fPartCurrentClef->asShortString () <<
        "'";
    }
    else {
      os <<
        "none";
    }

    os << endl;
  }
#endif

  // print the current part key if any
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceKeys ()) {
    os << left <<
      setw (fieldWidth) <<
      "fPartCurrentKey" << " : ";

    if (fPartCurrentKey) {
      os <<
        "'" <<
        fPartCurrentKey->asShortString () <<
        "'";
    }
    else {
      os <<
        "none";
    }

    os << endl;
  }
#endif

  // print the current part time if any
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceTimeSignatures ()) {
    os << left <<
      setw (fieldWidth) <<
      "fPartCurrentTimeSignature" << " : ";

    if (fPartCurrentTimeSignature) {
      os <<
        "'" <<
        fPartCurrentTimeSignature->asShortString () <<
        "'";
    }
    else {
      os << "none";
    }

    os << endl;
  }
#endif

  os << left <<
    setw (fieldWidth) << "fPartShortestNoteDuration" << " : " <<
    fPartShortestNoteDuration <<
    endl;

  os << left <<
    setw (fieldWidth) << "fPartShortestNoteTupletFactor" << " : " <<
    endl;

  ++gIndenter;
  os << fPartShortestNoteTupletFactor << endl;
  --gIndenter;

  // print all the staves if any
  size_t partAllStavesListSize =
    fPartAllStavesList.size ();

  os << left <<
    setw (fieldWidth) <<
    "Staff names in fPartAllStavesList" << " : ";

  if (partAllStavesListSize) {
    os << endl;
    ++gIndenter;

    for (S_msrStaff staff : fPartAllStavesList) {
      os << "\"" << staff->getStaffName () << "\"" << endl;
    } // for
    os << endl;

    --gIndenter;
  }
  else {
    os << "none" << endl;
  }

  size_t partNonHarmoniesNorFiguredBassStavesListSize =
    fPartNonHarmoniesNorFiguredBassStavesList.size ();

  os << left <<
    setw (fieldWidth) <<
    "Staff names in fPartNonHarmoniesNorFiguredBassStavesList" << " : ";

  if (partNonHarmoniesNorFiguredBassStavesListSize) {
    os << endl;
    ++gIndenter;

    for (S_msrStaff staff : fPartNonHarmoniesNorFiguredBassStavesList) {
      os << "\"" << staff->getStaffName () << "\"" << endl;
    } // for
    os << endl;

    --gIndenter;
  }
  else {
    os << "empty" << endl;
  }

  // print all the voices names if any
  size_t partAllVoicesListSize =
    fPartAllVoicesList.size ();

  os << left <<
    setw (fieldWidth) <<
    "Voice names in fPartAllVoicesList" << " : ";

  if (partAllVoicesListSize) {
    os << endl;
    ++gIndenter;

    for (S_msrVoice voice : fPartAllVoicesList) {
      os << "\"" << voice->getVoiceName () << "\"" << endl;
    } // for

    --gIndenter;
  }
  else {
    os << "empty" << endl;
  }

  os << endl;

  // print the part harmonies staff if any
  os << left <<
    setw (fieldWidth) <<
    "fPartHarmoniesStaff" << " : ";
  if (fPartHarmoniesStaff) {
    os << endl;

    ++gIndenter;
    os << fPartHarmoniesStaff;
    --gIndenter;
  }
  else {
    os << "none" << endl;
  }

  os << endl;

  // print the part figured bass staff if any
  os << left <<
    setw (fieldWidth) <<
    "fPartFiguredBassStaff" << " : ";
  if (fPartFiguredBassStaff) {
    os << endl;

    ++gIndenter;
    os << fPartFiguredBassStaff;
    --gIndenter;
  }
  else {
    os << "none" << endl;
  }

  os << endl;

  // print the part measure' whole notes durations vector
  printPartMeasuresWholeNotesDurationsVector (
    os,
    fieldWidth);

  os << endl;

  // print all the staves
  if (fPartAllStavesList.size ()) {
    list<S_msrStaff>::const_iterator
      iBegin = fPartAllStavesList.begin (),
      iEnd   = fPartAllStavesList.end (),
      i      = iBegin;

    for ( ; ; ) {
      S_msrStaff staff = (*i);

      // sanity check
      mfAssert (
        __FILE__, __LINE__,
        staff != nullptr,
        "staff is null");

      msrStaffKind
        staffKind =
          staff->getStaffKind ();

      switch (staffKind) { // JMI
        case msrStaffKind::kStaffKindRegular:
          os << staff;
          break;

        case msrStaffKind::kStaffKindTablature:
          os << staff;
          break;

        case msrStaffKind::kStaffKindHarmonies:
    // JMI      if (gGlobalMsrOahGroup->getShowHarmoniesVoices ()) {}
          os << staff;
          break;

        case msrStaffKind::kStaffKindFiguredBass:
    // JMI      if (gGlobalMsrOahGroup->getShowFiguredBassVoices ()) {}
          os << staff;
          break;

        case msrStaffKind::kStaffKindDrum:
          os << staff;
          break;

        case msrStaffKind::kStaffKindRythmic:
          os << staff;
          break;
      } // switch

      if (++i == iEnd) break;

      os << endl;
    } // for
  }

  os << endl;

  // print the part measures slices sequence
  os <<
    setw (fieldWidth) <<
    "fPartMeasuresSlicesSequence" << " : ";

  if (fPartMeasuresSlicesSequence) {
    os << endl;
    ++gIndenter;
    os << fPartMeasuresSlicesSequence;
    --gIndenter;
  }
  else {
    os << "none" << endl;
  }

  --gIndenter;

  os << ']' << endl;
}

void msrPart::printShort (ostream& os) const
{
  os <<
    "[Part" << ' ' << fPartMsrName <<
    " (" <<
    mfSingularOrPlural (
      fPartAllStavesList.size (), "staff", "staves") <<
    ")" <<
    ", line " << fInputLineNumber <<
    endl;

  ++gIndenter;

  const int fieldWidth = 39;

/*
  os << left <<
    setw (fieldWidth) <<
    "fPartGroupUpLink" << " : ";
  if (fPartPartGroupUpLink) {
    // it may be empty
    os <<
      fPartPartGroupUpLink->getPartGroupCombinedName ();
  }
  else {
    os << "none";
  }
  os << endl;
*/

  os << left <<
    setw (fieldWidth) <<
    "fPartID" << " : \"" <<
    fPartID << "\"" <<
    endl <<

    setw (fieldWidth) <<
    "fPartMsrName" << " : \"" <<
    fPartMsrName << "\"" <<
    endl <<

    setw (fieldWidth) <<
    "fPartAbsoluteNumber" << " : " <<
    fPartAbsoluteNumber <<
    endl <<

    setw (fieldWidth) <<
    "fPartName" << " : \"" <<
    fPartName << "\"" <<
    endl;

  // print the part measure' whole notes durations vector
  printPartMeasuresWholeNotesDurationsVector (
    os,
    fieldWidth);

  // print all the staves
  if (fPartAllStavesList.size ()) {
    os << endl;

    list<S_msrStaff>::const_iterator
      iBegin = fPartAllStavesList.begin (),
      iEnd   = fPartAllStavesList.end (),
      i      = iBegin;

    for ( ; ; ) {
      S_msrStaff staff = (*i);

      // sanity check
      mfAssert (
        __FILE__, __LINE__,
        staff != nullptr,
        "staff is null");

      msrStaffKind
        staffKind =
          staff->getStaffKind ();

      switch (staffKind) { // JMI
        case msrStaffKind::kStaffKindRegular:
          staff->printShort (os);
          break;

        case msrStaffKind::kStaffKindTablature:
          staff->printShort (os);
          break;

        case msrStaffKind::kStaffKindHarmonies:
          staff->printShort (os);
          break;

        case msrStaffKind::kStaffKindFiguredBass:
          staff->printShort (os);
          break;

        case msrStaffKind::kStaffKindDrum:
          staff->printShort (os);
          break;

        case msrStaffKind::kStaffKindRythmic:
          staff->printShort (os);
          break;
      } // switch

      if (++i == iEnd) break;

      os << endl;
    } // for
  }

  --gIndenter;

  os << ']' << endl;
}

void msrPart::printSummary (ostream& os) const
{
  os <<
    "[Part" << ' ' << fPartMsrName <<
    " (" <<
    mfSingularOrPlural (
      fPartAllStavesList.size (), "staff", "staves") <<
    ", " <<
    mfSingularOrPlural (
      fPartNumberOfMeasures, "measure", "measure") <<
    ")" <<
    endl;

  ++gIndenter;

  const int fieldWidth = 28;

  os << left <<
    setw (fieldWidth) <<
    "fPartID" << " : \"" <<
    fPartID << "\"" <<
    endl <<

    setw (fieldWidth) <<
    "fPartMsrName" << " : \"" <<
    fPartMsrName << "\"" <<
    endl <<

    setw (fieldWidth) <<
    "fPartName" << " : \"" <<
    fPartName << "\"" <<
    endl <<

    setw (fieldWidth) <<
    "fPartAbsoluteNumber" << " : " <<
    fPartAbsoluteNumber <<
    endl <<

    setw (fieldWidth) <<
    "fPartNameDisplayText" << " : \"" <<
    fPartNameDisplayText << "\"" <<
    endl <<

    setw (fieldWidth) <<
    "fPartAbbrevation" << " : \"" <<
    fPartAbbreviation << "\"" <<
    endl <<
    setw (fieldWidth) <<
    "fPartAbbreviationDisplayText" << " : \"" <<
    fPartAbbreviationDisplayText << "\"" <<
    endl <<

    setw (fieldWidth) <<
    "fPartInstrumentName" << " : \"" <<
    fPartInstrumentName << "\"" <<
    endl <<
    setw (fieldWidth) <<
    "fPartInstrumentAbbreviation" << " : \"" <<
    fPartInstrumentAbbreviation << "\"" <<
    endl <<

    setw (fieldWidth) <<
    "fPartNumberOfMeasures" << " : " <<
    fPartNumberOfMeasures <<
    endl <<

    setw (fieldWidth) <<
    "fPartContainsMultipleFullBarRests" << " : " <<
    fPartContainsMultipleFullBarRests <<
    endl <<

    setw (fieldWidth) <<
    "fPartCurrentPositionInMeasure" << " : " <<
    fPartCurrentPositionInMeasure <<
    endl;

  // print all the staves
  if (fPartAllStavesList.size ()) {
    os <<
      setw (fieldWidth) <<
      "fPartAllStavesList" << " : " <<
      endl;

    ++gIndenter;

    for (S_msrStaff staff : fPartAllStavesList) {
      staff->printSummary (os);
    } // for

    --gIndenter;
  }

  // print the harmonies staff if any
  if (fPartHarmoniesStaff) {
    os <<
      setw (fieldWidth) <<
      "fPartHarmoniesStaff" << " : " <<
      endl;

    ++gIndenter;
    os <<
      fPartHarmoniesStaff <<
      endl;
    --gIndenter;
  }

  // print the figured bass staff if any
  if (fPartFiguredBassStaff) {
    os <<
      setw (fieldWidth) <<
      "fPartFiguredBassStaff" << " : " <<
      endl;

    ++gIndenter;
    os <<
      fPartFiguredBassStaff <<
      endl;
    --gIndenter;
  }

  --gIndenter;

  os << ']' << endl;
}

void msrPart::printSlices (ostream& os) const
{
  os << "MSR part slices";

  os << endl << endl;

  ++gIndenter;

  if (fPartAllStavesList.size ()) {
    list<S_msrStaff>::const_iterator
      iBegin = fPartAllStavesList.begin (),
      iEnd   = fPartAllStavesList.end (),
      i      = iBegin;
    for ( ; ; ) {
      (*i)->
        printSlices (os);
      if (++i == iEnd) break;
      os << endl;
    } // for
  }

  --gIndenter;
}

ostream& operator<< (ostream& os, const S_msrPart& elt)
{
  elt->print (os);
  return os;
}


}
