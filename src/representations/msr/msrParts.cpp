/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <climits>      // INT_MIN, INT_MAX
#include <iomanip>      // std::setw, std::setprecision, ...
#include <algorithm>    // for_each

#include "mfEnableSanityChecksSetting.h"

#include "visitor.h"

#include "mfAssert.h"

#include "mfServices.h"

#include "mfStringsHandling.h"

#include "msrWae.h"

#include "mfEnableTracingSetting.h"

#include "msrBarLines.h"
#include "msrBreaks.h"
#include "msrParts.h"
#include "msrRehearsalMarks.h"
#include "msrTempos.h"

#include "oahOah.h"

#include "msrOah.h"

#include "msrBrowsers.h"


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
  int                  inputLineNumber,
  const std::string&   partID,
  const S_msrPartGroup partUpLinkToPartGroup)
{
  msrPart* o =
    new msrPart (
      inputLineNumber,
      partID,
      partUpLinkToPartGroup);
  assert (o != nullptr);
  return o;
}

msrPart::msrPart (
  int                  inputLineNumber,
  const std::string&   partID,
  const S_msrPartGroup partUpLinkToPartGroup)
    : msrPartGroupElement (inputLineNumber)
{
  // replace spaces in partID to set fPartID
  for_each (
    partID.begin (),
    partID.end (),
    mfStringSpaceReplacer (fPartID, '_'));

/* JMI
#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    partUpLinkToPartGroup != nullptr,
    "partUpLinkToPartGroup is null");
#endif
*/

  // set part number
  fPartAbsoluteNumber = ++gPartsCounter;

  // set part's part group upLink
  fPartUpLinkToPartGroup = partUpLinkToPartGroup;

  // do other initializations
  initializePart ();
}

void msrPart::initializePart ()
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceParts ()) {
    gLogStream <<
      "Creating part \"" << asString () << "\"" <<
      std::endl;

    gGlobalMsrOahGroup->printMsrOahValues (40); // JMI
  }
#endif

  // is this part name in the part renaming map?
  std::map<std::string, std::string>::const_iterator
    it =
      gGlobalMsrOahGroup->getMsrPartsRenamingMap ().find (fPartID);

  if (it != gGlobalMsrOahGroup->getMsrPartsRenamingMap ().end ()) {
    // yes, rename the part accordinglingly
    std::string newMsrPartName = (*it).second;

    std::stringstream s;

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

  // current measure position
  fPartMeasurePosition = Rational (0, 1);

  // part shortest note duration
  fPartShortestNoteDuration = Rational (INT_MAX, 1);

  // part shortest note tuplet factor
  fPartShortestNoteTupletFactor = Rational (1, 1);

#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceParts ()) {
    gLogStream <<
      "Creating part \"" << asString () << "\"" <<
      std::endl;
  }
#endif
}

msrPart::~msrPart ()
{}

S_msrScore  msrPart::fetchPartUpLinkToScore () const
{
  S_msrScore result;

  if (fPartUpLinkToPartGroup) {
    result =
      fPartUpLinkToPartGroup->
        getPartGroupUpLinkToScore ();
  }

  return result;
}

S_msrPart msrPart::createPartNewbornClone (const S_msrPartGroup& partGroupClone)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceParts ()) {
    gLogStream <<
      "Creating a newborn clone of part " <<
      getPartCombinedName () <<
      std::endl;
  }
#endif

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    partGroupClone != nullptr,
    "partGroupClone is null");
#endif

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
  const S_msrStaff& staff)
{
  int inputLineNumber =
    staff->getInputLineNumber ();

  int staffNumber =
    staff->getStaffNumber ();

  msrStaffKind staffKind =
    staff->getStaffKind ();

#ifdef MF_TRACING_IS_ENABLED
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
      std::endl;
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
#ifdef MF_SANITY_CHECKS_ARE_ENABLED
      // sanity check
      if (fPartHarmoniesStaff) {
        std::stringstream s;

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
#endif

      // register harmonies staff
      fPartHarmoniesStaff = staff;
      break;

    case msrStaffKind::kStaffKindFiguredBass:
#ifdef MF_SANITY_CHECKS_ARE_ENABLED
      // sanity check
      if (fPartFiguredBassStaff) {
        std::stringstream s;

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
#endif

      // register figured bass staff
      fPartFiguredBassStaff = staff;
      break;
  } // switch
}

void msrPart::setPartMeasurePosition (
  int             inputLineNumber,
  const Rational& measurePosition)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasurePositions ()) {
    gLogStream <<
      "Setting part current measure position to " <<
      measurePosition <<
      " in part " <<
      getPartCombinedName () <<
      std::endl;
  }
#endif

  if (measurePosition.getNumerator () < 0) {
    std::stringstream s;

    s <<
      "cannot set part current measure position to " <<
      measurePosition <<
      " in part " <<
      getPartCombinedName () <<
      " since it is negative";

    msrInternalError (
      gGlobalServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }

  fPartMeasurePosition =
    measurePosition;
}

void msrPart::incrementPartMeasurePosition (
  int             inputLineNumber,
  const Rational& duration)
{
  fPartMeasurePosition += duration;

#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasurePositions ()) {
    gLogStream <<
      "Incrementing part current measure position by " <<
      duration <<
      " in part " <<
      getPartCombinedName () <<
      ", thus setting it to " <<
      fPartMeasurePosition <<
      std::endl;
  }
#endif
}

void msrPart::decrementPartMeasurePosition (
  int             inputLineNumber,
  const Rational& duration)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasurePositions ()) {
    gLogStream <<
      "Decrementing part current measure position by " <<
      duration <<
      " in part " <<
      getPartCombinedName () <<
      std::endl;
  }
#endif

  fPartMeasurePosition -= duration;

  if (fPartMeasurePosition.getNumerator () < 0) {
    std::stringstream s;

    s <<
      "cannot decrement part current measure position by " <<
      duration <<
      " in part " <<
      getPartCombinedName () <<
      " since that sets it to " <<
      fPartMeasurePosition <<
      ", which is negative ";

    msrInternalError (
      gGlobalServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }

#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasurePositions ()) {
    gLogStream <<
      "The new part current measure position is " <<
      fPartMeasurePosition <<
      " in part " <<
      getPartCombinedName () <<
      std::endl;
  }
#endif
}

void msrPart::setPartShortestNoteDuration (
  const Rational& duration)
{
#ifdef MF_TRACING_IS_ENABLED
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
      std::endl;
  }
#endif

  fPartShortestNoteDuration = duration;
}

void msrPart::setPartShortestNoteTupletFactor (
  const msrTupletFactor& noteTupletFactor)
{
#ifdef MF_TRACING_IS_ENABLED
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
      std::endl;
  }
#endif

  fPartShortestNoteTupletFactor = noteTupletFactor;
}

void msrPart::assignSequentialNumbersToRegularVoicesInPart (
  int inputLineNumber)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceParts () || gGlobalTracingOahGroup->getTraceVoices ()) {
    gLogStream <<
      "Assigning sequential numbers to the staves in part \"" <<
      fPartID <<
      ", " <<
      fPartName <<
      "\"" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  for (S_msrStaff staff : fPartAllStavesList) {
    staff->
      assignSequentialNumbersToRegularVoicesInStaff (
        inputLineNumber);
  } // for
}

void msrPart::setPartMsrName (const std::string& partMsrName)
{
  // is this part name in the part renaming map?
  std::map<std::string, std::string>::const_iterator
    it =
      gGlobalMsrOahGroup->getMsrPartsRenamingMap ().find (fPartMsrName);

  if (it != gGlobalMsrOahGroup->getMsrPartsRenamingMap ().end ()) {
    // yes, rename the part accordinglingly
    fPartMsrName = (*it).second;

#ifdef MF_TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceParts ()) {
      gLogStream <<
        "Setting part name of " << getPartCombinedName () <<
        " to \"" << fPartMsrName << "\"" <<
         std::endl;
    }
#endif
  }
  else {
    // use the argument
    fPartMsrName = partMsrName;

#ifdef MF_TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceParts ()) {
      gLogStream <<
        "Keeping partID \"" << partMsrName <<
        "\" as part name  for " << getPartCombinedName () <<
      std::endl;
    }
#endif
  }
}

std::string msrPart::getPartCombinedName () const
{
  std::stringstream s;

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
  int                inputLineNumber,
  int                previousMeasureEndInputLineNumber,
  const std::string& measureNumber,
  msrMeasureImplicitKind
                     measureImplicitKind)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasures ()) {
    gLogStream <<
      "Creating measure '" <<
      measureNumber <<
      "' and appending it to part " <<
      getPartCombinedName () <<
      "', line " << inputLineNumber <<
      std::endl;
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
  int                inputLineNumber,
  const std::string& nextMeasureNumber)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasures ()) {
    gLogStream <<
      "Setting next measure number to '" <<
      nextMeasureNumber <<
      "' in part " <<
      getPartCombinedName () <<
      "', line " << inputLineNumber <<
      std::endl;
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

Rational msrPart::fetchPartMeasuresWholeNotesDurationsVectorAt (
  int inputLineNumber,
  int indexValue) const
{
  Rational result;

  size_t
    partMeasuresWholeNotesDurationsVectorSize =
      fPartMeasuresWholeNotesDurationsVector.size ();

#ifdef MF_TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceWholeNotes ()) {
      gLogStream <<
        "fetchPartMeasuresWholeNotesDurationsVectorAt() in part \"" <<
        getPartCombinedName () <<
        "\"" <<
        ", partMeasuresWholeNotesDurationsVectorSize: " <<
        partMeasuresWholeNotesDurationsVectorSize <<
        ", indexValue: " << indexValue <<
        std::endl;
    }
#endif

  // has this measureOrdinalNumber been registered already?
  try {
    Rational
      currentValue =
        fPartMeasuresWholeNotesDurationsVector.at (indexValue);

    // yes
    result = currentValue;
  }

  catch (const std::out_of_range& e) {
    // no

    std::stringstream s;

    s <<
      "fetchPartMeasuresWholeNotesDurationsVectorAt() in part \"" <<
      getPartCombinedName () <<
      "\"" <<
      ", partMeasuresWholeNotesDurationsVectorSize: " <<
      partMeasuresWholeNotesDurationsVectorSize <<
      ", indexValue: " << indexValue << " is out of bounds" <<
      ", line " << inputLineNumber;

//     msrInternalWarning (
     msrInternalError (
      gGlobalServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());

      result = Rational (15, 8); // TEMP JMI v0.9.61
  }

  return result;
}

void msrPart::registerShortestNoteInPartIfRelevant (const S_msrNote& note)
{
  // is note the shortest one in this part?
  Rational
    noteSoundingWholeNotes =
      note->
        getMeasureElementSoundingWholeNotes ();

/* JMI
  Rational
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

#ifdef MF_TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceNotes ()) {
      gLogStream <<
        "The new shortest note in part \"" << getPartCombinedName () << "\"" <<
        " becomes " << note->asString () <<
        std::endl;
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
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasures ()) {
    gLogStream <<
      "Setting the number of measures in part " <<
      getPartCombinedName () <<
      " to " <<
      partNumberOfMeasures <<
    std::endl;
  }
#endif

  fPartNumberOfMeasures = partNumberOfMeasures;

  size_t
    fPartMeasuresWholeNotesDurationsVectorSize =
      fPartMeasuresWholeNotesDurationsVector.size ();

  if (partNumberOfMeasures > fPartMeasuresWholeNotesDurationsVectorSize) {
#ifdef MF_TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceMeasures ()) {
      gLogStream <<
        "Resizing fPartMeasuresWholeNotesDurationsVector in part " << // JMI ???
        getPartCombinedName () <<
        " from " <<
        fPartMeasuresWholeNotesDurationsVectorSize <<
        " to " <<
        partNumberOfMeasures <<
        " measures" <<
        std::endl;
    }
#endif

    fPartMeasuresWholeNotesDurationsVector.clear ();
    fPartMeasuresWholeNotesDurationsVector.resize (
      fPartNumberOfMeasures,
      Rational (0, 1));
  }
}

void msrPart::registerOrdinalMeasureNumberWholeNotesDuration (
  int             inputLineNumber,
  int             measureOrdinalNumber,
  const Rational& wholeNotesDuration)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasures ()) {
    gLogStream <<
      "Registering the whole notes duration of the measure with ordinal number '" <<
      measureOrdinalNumber <<
      "' as " <<
      wholeNotesDuration <<
      " in part " << getPartCombinedName () <<
      ", measureOrdinalNumber: " << measureOrdinalNumber <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasures ()) {
    gLogStream <<
      "===> fPartMeasuresWholeNotesDurationsVector contents: " <<
      std::endl;
    for (Rational rat : fPartMeasuresWholeNotesDurationsVector) {
      ++gIndenter;
      gLogStream <<
        rat <<
        std::endl;
      --gIndenter;
    } // for
    gLogStream <<
      "<==== end of fPartMeasuresWholeNotesDurationsVector contents " <<
      std::endl;
  }
#endif

  size_t index = measureOrdinalNumber - 1;

  // has this measureOrdinalNumber been registered already?
  try {
    Rational
      currentValue =
        fPartMeasuresWholeNotesDurationsVector.at (index);

    // yes

    // allow for polymetrics in non-MusicXML contexts? JMI
#ifdef MF_TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceMeasures ()) {
      gLogStream <<
        "The measure with ordinal number " <<
        measureOrdinalNumber <<
        " was known with a whole notes duration of " <<
        currentValue <<
        ", now registering it with a duration of " <<
        wholeNotesDuration <<
        " in part " << getPartCombinedName () <<
        std::endl;
    }
#endif
  }

  catch (const std::out_of_range& e) {
    // no

    fPartMeasuresWholeNotesDurationsVector [index] =
      wholeNotesDuration;

#ifdef MF_TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceMeasures ()) {
      gLogStream <<
        "The measure with ordinal number " <<
        measureOrdinalNumber <<
        " is now registered with a duration of " <<
        wholeNotesDuration <<
        " in part " << getPartCombinedName () <<
        ", fPartMeasuresWholeNotesDurationsVector.size () = " <<
        fPartMeasuresWholeNotesDurationsVector.size () <<
        std::endl;
    }
#endif
  }
}

void msrPart::appendStaffDetailsToPart (
  const S_msrStaffDetails& staffDetails)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceStaffDetails ()) {
    gLogStream <<
      "Appending staff details\"" <<
      staffDetails->asShortString () <<
      "\" to part " << getPartCombinedName () <<
    std::endl;
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

void msrPart::appendClefToPart (
  const S_msrClef& clef)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceClefs ()) {
    gLogStream <<
      "Appending clef '" <<
      clef->asString () <<
      "' to part " << getPartCombinedName () <<
    std::endl;
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

void msrPart::appendKeyToPart (
  const S_msrKey& key)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceKeys ()) {
    gLogStream <<
      "Appending key " <<
      key->asString () <<
      " to part " << getPartCombinedName () <<
      std::endl;
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

void msrPart::appendTimeSignatureToPart (
  const S_msrTimeSignature& timeSignature)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceTimeSignatures ()) {
    gLogStream <<
      "Appending time signature '" <<
      timeSignature->asString () <<
      "' to part " << getPartCombinedName () <<
      std::endl;
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

void msrPart::appendTimeSignatureToPartClone (
  const S_msrTimeSignature& timeSignature)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceTimeSignatures ()) {
    gLogStream <<
      "Appending time signature '" <<
      timeSignature->asString () <<
      "' to part clone " << getPartCombinedName () <<
      std::endl;
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

void msrPart::appendTempoToPart (
  const S_msrTempo& tempo)
{
 #ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceTempos ()) {
    gLogStream <<
      "Appending tempo " << tempo->asString () <<
      " to part " <<
      getPartCombinedName () <<
      std::endl;
  }
#endif

 // append tempo to registered staves
  for (S_msrStaff staff : fPartAllStavesList) {
    staff->
      appendTempoToStaff (tempo);
  } // for
}

void msrPart::appendRehearsalMarkToPart (
  const S_msrRehearsalMark& rehearsalMark)
{
 #ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRehearsalMarks ()) {
    gLogStream <<
      "Appending rehearsal mark " << rehearsalMark->asString () <<
      " to part " <<
      getPartCombinedName () <<
      std::endl;
  }
#endif

 // append rehearsal mark to registered staves
  for (S_msrStaff staff : fPartAllStavesList) {
    staff->
      appendRehearsalMarkToStaff (rehearsalMark);
  } // for
}


void msrPart::appendLineBreakToPart (
  const S_msrLineBreak& lineBreak)
{
 #ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceLineBreaks ()) {
    gLogStream <<
      "Appending line break " << lineBreak->asString () <<
      " to part " <<
      getPartCombinedName () <<
      std::endl;
  }
#endif

 // append line break to registered staves
  for (S_msrStaff staff : fPartAllStavesList) {
    staff->
      appendLineBreakToStaff (lineBreak);
  } // for
}

void msrPart::appendPageBreakToPart (
  const S_msrPageBreak& pageBreak)
{
 #ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTracePageBreaks ()) {
    gLogStream <<
      "Appending page break " << pageBreak->asString () <<
      " to part " <<
      getPartCombinedName () <<
      std::endl;
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
  const Rational& measurePosition)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasures ()) {
    gLogStream <<
      "Inserting hidden measure and barLine at position " <<
      measurePosition <<
      "' in part clone " << getPartCombinedName () <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  ++gIndenter;

  // cascade it to all staves
  for (S_msrStaff staff : fPartAllStavesList) {
    staff->
      insertHiddenMeasureAndBarLineInStaffClone (
        inputLineNumber,
        measurePosition);
  } // for

  --gIndenter;
}

void msrPart::appendTranspositionToPart (
  const S_msrTransposition& transposition)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceTranspositions ()) {
    gLogStream <<
      "Appending transposition \"" <<
      transposition->asString () <<
      "\" to part " << getPartCombinedName () <<
    std::endl;
  }
#endif

  // set part current transposition
  fPartCurrentTransposition = transposition;

  // cascade it to all staves
  for (S_msrStaff staff : fPartAllStavesList) {
    staff->
      appendTranspositionToStaff (transposition);
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
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeats ()) {
    gLogStream <<
      "Handling a repeat start in part \"" <<
      getPartCombinedName () <<
      "\", line " << inputLineNumber <<
      std::endl;
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
  int                inputLineNumber,
  const std::string& measureNumber,
  int                repeatTimes)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeats ()) {
    gLogStream <<
      "Handling a repeat end in part \"" <<
      getPartCombinedName () <<
      "\"" <<
      ", line " << inputLineNumber <<
      std::endl;
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
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeats ()) {
    gLogStream <<
      "Handling a repeat ending start in part \"" <<
      getPartCombinedName () <<
      "\"" <<
      ", line " << inputLineNumber <<
      std::endl;
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
  int                 inputLineNumber,
  const std::string&  repeatEndingNumber, // may be "1, 2"
  msrRepeatEndingKind repeatEndingKind)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeats ()) {
    gLogStream <<
      "Handling a " <<
      msrRepeatEndingKindAsString (
        repeatEndingKind) <<
      " repeat ending end in part \"" <<
      getPartCombinedName () <<
      "\"" <<
      ", line " << inputLineNumber <<
      std::endl;
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
  const std::string& measureNumber,
  int    repeatTimes)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeats ()) {
    gLogStream <<
      "Finalizing a repeat upon its end in part \"" <<
      getPartCombinedName () <<
      "\"" <<
      ", line " << inputLineNumber <<
      std::endl;
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
  int                inputLineNumber,
  const S_msrRepeat& repeatCLone)
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
  const S_msrRepeatEnding& repeatEndingClone)
{
  for (S_msrStaff staff : fPartAllStavesList) {
    staff->
      appendRepeatEndingCloneToStaff (repeatEndingClone);
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

void msrPart::appendMultipleFullBarRestsToPart (
  int inputLineNumber,
  int multipleFullBarRestsMeasuresNumber)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMultipleFullBarRests ()) {
    gLogStream <<
      "Appending a multiple full-bar rest for " <<
      mfSingularOrPlural (
        multipleFullBarRestsMeasuresNumber, "measure", "measures") <<
      " to part " <<
      getPartCombinedName () <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  fPartContainsMultipleFullBarRests = true;

  // create multiple rest in all staves
  for (S_msrStaff staff : fPartAllStavesList) {
    staff->
      appendMultipleFullBarRestsToStaff (
        inputLineNumber,
        multipleFullBarRestsMeasuresNumber);
  } // for
}

void msrPart::replicateLastAppendedMeasureInPart (
  int inputLineNumber,
  int replicatasNumber)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMultipleFullBarRests ()) {
    gLogStream <<
      "Replicating last appended measure in part " <<
      getPartCombinedName () <<
      std::endl;
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
  int                inputLineNumber,
  const std::string& previousMeasureNumber,
  int                multipleFullBarRestsNumber)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMultipleFullBarRests ()) {
    gLogStream <<
      "Adding " <<
      mfSingularOrPlural (
        multipleFullBarRestsNumber, "multiple full-bar rest", "multiple full-bar rests") <<
      " to part " <<
      getPartCombinedName () <<
      ", " <<
      std::endl;
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
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMultipleFullBarRests ()) {
    gLogStream <<
      "Appending the pending multiple rest to part " <<
      getPartCombinedName () <<
      std::endl;
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
  int                              inputLineNumber,
  const S_msrMultipleFullBarRests& multipleFullBarRests)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMultipleFullBarRests ()) {
    gLogStream <<
      "Appending multiple rest '" <<
      multipleFullBarRests->asString () <<
      "' to part clone " <<
      getPartCombinedName () <<
      std::endl;
  }
#endif

  for (S_msrStaff staff : fPartAllStavesList) {
    staff->
      appendMultipleFullBarRestsCloneToStaff (
        inputLineNumber,
        multipleFullBarRests);
  } // for
}

void msrPart::appendBarLineToPart (
  const S_msrBarLine& barLine)
{
 #ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceBarLines ()) {
    gLogStream <<
      "Appending barLine " << barLine->asString () <<
      " to part " <<
      getPartCombinedName () <<
      std::endl;
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
    std::stringstream s;

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

#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceStaves ()) {
    gLogStream <<
      "Adding " <<
      msrStaffKindAsString (staffKind) <<
      " staff " << staffNumber <<
      " to part " << getPartCombinedName () <<
      ", line " << inputLineNumber <<
      std::endl;
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
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceStaves ()) {
    gLogStream <<
      "Adding harmonies staff " <<
      " to part " << getPartCombinedName () <<
      ", line " << inputLineNumber <<
      std::endl;
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
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceStaves ()) {
    gLogStream <<
      "Adding figured bass staff " <<
      " to part " << getPartCombinedName () <<
      ", line " << inputLineNumber <<
      std::endl;
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

void msrPart::addStaffToPartCloneByItsNumber (const S_msrStaff& staff)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceStaves ()) {
    gLogStream <<
      "Adding staff \"" << staff->getStaffName () <<
      "\" to part clone " << getPartCombinedName () <<
      std::endl;
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
  const S_msrVoice& voice)
{
  // register voice in this staff
  fPartAllVoicesList.push_back (voice);

  // register it in the partgroup uplink
  fPartUpLinkToPartGroup->
    registerVoiceInPartGroupAllVoicesList (voice);
}

S_msrVoice msrPart::createPartHarmoniesVoice (
  int                inputLineNumber,
  const std::string& currentMeasureNumber)
{
  if (fPartHarmoniesVoice) {
    std::stringstream s;

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
#ifdef MF_TRACING_IS_ENABLED
  int partHarmoniesStaffNumber =
    msrPart::K_PART_HARMONIES_STAFF_NUMBER;

  if (gGlobalTracingOahGroup->getTraceHarmonies ()) {
    gLogStream <<
      "Creating harmonies staff for part \"" <<
      getPartCombinedName () <<
      "\" with staff number " <<
      partHarmoniesStaffNumber <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  fPartHarmoniesStaff =
    addHarmoniesStaffToPart (
      inputLineNumber);

  // create the part harmonies voice
  int partHarmoniesVoiceNumber =
    msrPart::K_PART_HARMONIES_VOICE_NUMBER;

#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceHarmonies ()) {
    gLogStream <<
      "Creating harmonies voice for part \"" <<
      getPartCombinedName () <<
      "\" with voice number " <<
      partHarmoniesVoiceNumber <<
      ", line " << inputLineNumber <<
      std::endl;
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

//* JMI v0.9.66
  // set backward link
 // ->
//     setHarmoniesVoicePartBackwardLink (fPartHarmoniesVoice);
//*/

  return fPartHarmoniesVoice;
}

S_msrVoice msrPart::createPartFiguredBassVoice (
  int                inputLineNumber,
  const std::string& currentMeasureNumber)
{
  if (fPartFiguredBassVoice) {
    std::stringstream s;

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
#ifdef MF_TRACING_IS_ENABLED
  int partFiguredBassStaffNumber =
    msrPart::K_PART_FIGURED_BASS_STAFF_NUMBER;

  if (gGlobalTracingOahGroup->getTraceFiguredBasses ()) {
    gLogStream <<
      "Creating figured bass staff for part \"" <<
      getPartCombinedName () <<
      "\" with staff number " <<
      partFiguredBassStaffNumber <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  fPartFiguredBassStaff =
    addHFiguredBassStaffToPart (
      inputLineNumber);

  // create the figured bass voice
  int partFiguredBassVoiceNumber =
    msrPart::K_PART_FIGURED_BASS_VOICE_NUMBER;

#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceFiguredBasses ()) {
    gLogStream <<
      "Creating figured bass voice for part \"" <<
      getPartCombinedName () <<
      "\" with voice number " <<
      partFiguredBassVoiceNumber <<
      ", line " << inputLineNumber <<
      std::endl;
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

///* JMI v0.9.66
  // set backward link
//   ->
//     setFiguredBassVoicePartBackwardLink (fPartFiguredBassVoice);
//*/

  return fPartFiguredBassVoice;
}

void msrPart::appendFiguredBassToPart (
  const S_msrVoice&       figuredBassSupplierVoice,
  const S_msrFiguredBass& figuredBass)
{
  int inputLineNumber =
    figuredBass->getInputLineNumber ();

  ++gIndenter;

  switch (figuredBassSupplierVoice->getVoiceKind ()) {
    case msrVoiceKind::kVoiceKindRegular:
      // append the figured bass to the part figured bass voice
#ifdef MF_TRACING_IS_ENABLED
      if (gGlobalTracingOahGroup->getTraceFiguredBasses ()) {
        gLogStream <<
          "Appending figured bass " <<
          figuredBass->asString () <<
          " to part " <<
          getPartCombinedName () <<
          ", line " << inputLineNumber <<
          std::endl;
      }
#endif

      fPartFiguredBassVoice->
        appendFiguredBassToVoice (figuredBass);
      break;

    case msrVoiceKind::kVoiceKindDynamics:
      break;

    case msrVoiceKind::kVoiceKindHarmonies:
    case msrVoiceKind::kVoiceKindFiguredBass:
      {
        std::stringstream s;

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

void msrPart::appendFiguredBassToPartClone (
  const S_msrVoice&       figuredBassSupplierVoice,
  const S_msrFiguredBass& figuredBass)
{
  int inputLineNumber =
    figuredBass->getInputLineNumber ();

  ++gIndenter;

  switch (figuredBassSupplierVoice->getVoiceKind ()) {
    case msrVoiceKind::kVoiceKindFiguredBass:
      // append the figured bass to the part figured bass voice
#ifdef MF_TRACING_IS_ENABLED
      if (gGlobalTracingOahGroup->getTraceFiguredBasses ()) {
        gLogStream <<
          "Appending figured bass " <<
          figuredBass->asString () <<
          " to part clone " <<
          getPartCombinedName () <<
          ", line " << inputLineNumber <<
          std::endl;
      }
#endif

      fPartFiguredBassVoice->
        appendFiguredBassToVoiceClone (figuredBass);
      break;

    case msrVoiceKind::kVoiceKindDynamics:
      break;

    case msrVoiceKind::kVoiceKindRegular:
    case msrVoiceKind::kVoiceKindHarmonies:
      {
        std::stringstream s;

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
  const S_msrScordatura& scordatura)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceParts ()) {
    gLogStream <<
      "Appending scordatura '" <<
      scordatura->asString () <<
      "' to part " <<
      getPartCombinedName () <<
      std::endl;
  }
#endif

  for (S_msrStaff staff : fPartRegularStavesList) {
    staff->
      appendScordaturaToStaff (
        scordatura);
  } // for
}

void msrPart::appendAccordionRegistrationToPart (
  const S_msrAccordionRegistration&
    accordionRegistration)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceParts ()) {
    gLogStream <<
      "Appending accordion registration '" <<
      accordionRegistration->asString () <<
      "' to part " <<
      getPartCombinedName () <<
      std::endl;
  }
#endif

  for (S_msrStaff staff : fPartRegularStavesList) {
    staff->
      appendAccordionRegistrationToStaff (
        accordionRegistration);
  } // for
}

void msrPart::appendHarpPedalsTuningToPart (
  const S_msrHarpPedalsTuning&
    harpPedalsTuning)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceParts ()) {
    gLogStream <<
      "Appending harp pedals tuning '" <<
      harpPedalsTuning->asString () <<
      "' to part " <<
      getPartCombinedName () <<
      std::endl;
  }
#endif

  for (S_msrStaff staff : fPartRegularStavesList) {
    staff->
      appendHarpPedalsTuningToStaff (
        harpPedalsTuning);
  } // for
}

void msrPart::addSkipGraceNotesGroupAheadOfVoicesClonesIfNeeded (
  const S_msrVoice&           graceNotesGroupOriginVoice,
  const S_msrGraceNotesGroup& skipGraceNotesGroup)
{
#ifdef MF_TRACING_IS_ENABLED
  int inputLineNumber =
    skipGraceNotesGroup->getInputLineNumber ();
#endif

#ifdef MF_TRACING_IS_ENABLED
  if (
    gGlobalTracingOahGroup->getTraceMeasures ()
      ||
    gGlobalTracingOahGroup->getTraceGraceNotes ()
      ||
    gGlobalTracingOahGroup->getTraceParts ()
    ) {
    gLogStream <<
      "addSkipGraceNotesGroupAheadOfVoicesClonesIfNeeded () in " <<
      getPartCombinedName () <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  for (S_msrStaff staff : fPartAllStavesList) {
    std::map<int, S_msrVoice>
      staffAllVoicesMap =
        staff->
          getStaffVoiceNumbersToAllVoicesMap ();

    for (std::pair<int, S_msrVoice> thePair : staffAllVoicesMap) {
      const S_msrVoice& voice = thePair.second;

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
  const Rational& backupStepLength)
{
  // account for backup in part
  decrementPartMeasurePosition (
    inputLineNumber,
    backupStepLength);
}

void msrPart::finalizeLastAppendedMeasureInPart (
  int    inputLineNumber)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasures ()) {
    gLogStream <<
      "Finalizing last appended measure in part " <<
      getPartCombinedName () <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  ++gIndenter;

  // reset current measure position
  fPartMeasurePosition = Rational (0, 1);

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
      fPartUpLinkToPartGroup->
        getPartGroupUpLinkToScore ();

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

bool msrPart::compareStavesToHaveFiguredBassesBelowCorrespondingPart (
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
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceParts ()) {
    gLogStream <<
      "Finalizing part " <<
      getPartCombinedName () <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  ++gIndenter;

  if (! fPartAllStavesList.size ()) {
    std::stringstream s;

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
      compareStavesToHaveFiguredBassesBelowCorrespondingPart);

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
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceParts ()) {
    gLogStream <<
      "Finalizing part clone " <<
      getPartCombinedName () <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

// JMI CAFE ???
    // sort the staves to have harmonies above and figured bass below the part
    fPartAllStavesList.sort (
      compareStavesToHaveFiguredBassesBelowCorrespondingPart);

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

#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceParts ()) {
    gLogStream <<
      "Finalizing all the measures of part \"" <<
      getPartCombinedName () <<
      "\", line " << inputLineNumber <<
      std::endl;
  }
#endif

  for (const S_msrVoice& voice : fPartAllVoicesList) {
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
#ifdef MF_TRACING_IS_ENABLED
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
      std::endl;
  }
#endif

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    fPartMeasuresSlicesSequence == nullptr,
    "fPartMeasuresSlicesSequence is not null");
#endif

  ++gIndenter;

  // create the part measures slices sequence
  fPartMeasuresSlicesSequence =
    msrMeasuresSlicesSequence::create (
      fPartName); // origin

  // populate it
  for (S_msrStaff staff : fPartAllStavesList) {
#ifdef MF_TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceMeasuresSlices ()) {
      gLogStream <<
        "---> staff \"" <<
        staff->getStaffName () <<
        "\":" <<
        std::endl;
    }
#endif

    ++gIndenter;

    S_msrMeasuresSlicesSequence
      staffMeasuresSlicesSequence =
        staff->
          getStaffMeasuresSlicesSequence ();

    if (! staffMeasuresSlicesSequence) {
      std::stringstream s;

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

#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasuresSlices ()) {
    gLogStream <<
      "fPartMeasuresSlicesSequence:" <<
      std::endl;

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
      std::endl;
  }

  if (visitor<S_msrPart>*
    p =
      dynamic_cast<visitor<S_msrPart>*> (v)) {
        S_msrPart elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrPart::visitStart ()" <<
            std::endl;
        }
        p->visitStart (elem);
  }
}

void msrPart::acceptOut (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrPart::acceptOut ()" <<
      std::endl;
  }

  if (visitor<S_msrPart>*
    p =
      dynamic_cast<visitor<S_msrPart>*> (v)) {
        S_msrPart elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrPart::visitEnd ()" <<
            std::endl;
        }
        p->visitEnd (elem);
  }
}

void msrPart::browseData (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrPart::browseData ()" <<
      std::endl;
  }

#ifdef MF_TRACING_IS_ENABLED // JMI
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) { // JMI TEMP
    gLogStream <<
      "++++++++ fPartAllStavesList.size(): " <<
      fPartAllStavesList.size () <<
      std::endl;

    if (fPartAllStavesList.size ()) {
      for (S_msrStaff staff : fPartAllStavesList) {
        gLogStream <<
          std::endl <<
          "+++++++++ staff: ++++++++" <<
          " \"" << staff->getStaffName () << "\"" <<
          std::endl;
      } // for
    }

    gLogStream <<
      "++++++++ fPartNonHarmoniesNorFiguredBassStavesList.size(): " <<
      fPartNonHarmoniesNorFiguredBassStavesList.size () <<
      std::endl;

    if (fPartNonHarmoniesNorFiguredBassStavesList.size ()) {
      for (S_msrStaff staff : fPartNonHarmoniesNorFiguredBassStavesList) {
        gLogStream <<
          std::endl <<
          "+++++++++ staff: ++++++++" <<
          " \"" << staff->getStaffName () << "\"" <<
          std::endl;
      } // for
    }
  }
#endif

  /* don't enforce any order here, leave it to the client thru sorting ??? JMI v0.9.66 */

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

std::string msrPart::asString () const
{
  std::stringstream s;

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
  std::ostream& os,
  int           fieldWidth) const
{
  os << std::left <<
    std::setw (fieldWidth) <<
    "fPartMeasuresWholeNotesDurationsVector" << ": " ;

  if (fPartNumberOfMeasures == 0) {
    os << "[EMPTY]" << std::endl;
  }
  else {
    os << std::endl;

    ++gIndenter;

    for (size_t i = 0; i < fPartNumberOfMeasures; ++i) {
      int j = i + 1; // indices start at 0

      os << std::left <<
        "ordinal number " <<
        std::setw (3) << std::right <<
        j << ": " <<
        std::setw (4) <<
        fPartMeasuresWholeNotesDurationsVector [ i ].toString () <<
        std::endl;
    } // for

    --gIndenter;
  }
}

void msrPart::printFull (std::ostream& os) const
{
  os <<
    "[Part" << ' ' << fPartMsrName <<
    " (" <<
    mfSingularOrPlural (
      fPartAllStavesList.size (), "staff", "staves") <<
    ")" <<
    ", line " << fInputLineNumber <<
    std::endl;

  ++gIndenter;

  const int fieldWidth = 37;

  os << std::left <<
    std::setw (fieldWidth) <<
    "fUpLinkToPartGroup" << ": ";
  if (fPartUpLinkToPartGroup) {
    // it may be empty
    os <<
      fPartUpLinkToPartGroup->getPartGroupCombinedName ();
  }
  else {
    os << "[NONE]";
  }
  os << std::endl;

  os << std::left <<
    std::setw (fieldWidth) <<
    "fPartID" << ": \"" <<
    fPartID << "\"" <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fPartMsrName" << ": \"" <<
    fPartMsrName << "\"" <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fPartName" << ": \"" <<
    fPartName << "\"" <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fPartNameDisplayText" << ": \"" <<
    fPartNameDisplayText << "\"" <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fPartAbbrevation" << ": \"" <<
    fPartAbbreviation << "\"" <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fPartAbbreviationDisplayText" << ": \"" <<
    fPartAbbreviationDisplayText << "\"" <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fPartAbsoluteNumber" << ": " <<
    fPartAbsoluteNumber <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fPartInstrumentName" << ": \"" <<
    fPartInstrumentName << "\"" <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fPartInstrumentAbbreviation" << ": \"" <<
    fPartInstrumentAbbreviation << "\"" <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fPartNumberOfMeasures" << ": " <<
    fPartNumberOfMeasures <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fPartContainsMultipleFullBarRests" << ": " <<
    fPartContainsMultipleFullBarRests <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fPartMeasurePosition" << ": " <<
    fPartMeasurePosition <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fPartHarmoniesStaff" << ": \"" <<
    fPartHarmoniesStaff->asShortString () << "\"" <<
    std::endl;

  os << std::left <<
    std::setw (fieldWidth) <<
    "fPartHarmoniesStaff" << ": ";
    if (fPartHarmoniesStaff) {
      os <<
        fPartHarmoniesStaff->asShortString ();
    }
    else {
      os << "[NONE]";
    }
  os << std::endl;

  os << std::left <<
    std::setw (fieldWidth) <<
    "fPartHarmoniesVoice" << ": ";
    if (fPartHarmoniesVoice) {
      os <<
        fPartHarmoniesVoice->asShortString ();
    }
    else {
      os << "[NONE]";
    }
  os << std::endl;

  os << std::left <<
    std::setw (fieldWidth) <<
    "fPartFiguredBassStaff" << ": ";
    if (fPartFiguredBassStaff) {
      os <<
        fPartFiguredBassStaff->asShortString ();
    }
    else {
      os << "[NONE]";
    }
  os << std::endl;

  os << std::left <<
    std::setw (fieldWidth) <<
    "fPartFiguredBassVoice" << ": ";
    if (fPartFiguredBassVoice) {
      os <<
        fPartFiguredBassVoice->asShortString ();
    }
    else {
      os << "[NONE]";
    }
  os << std::endl;

  // print current the part clef if any
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceClefs ()) {
    os << std::left <<
      std::setw (fieldWidth) <<
      "fPartCurrentClef" << ": ";

    if (fPartCurrentClef) {
      os <<
        "'" <<
        fPartCurrentClef->asShortString () <<
        "'";
    }
    else {
      os <<
        "[NONE]";
    }

    os << std::endl;
  }
#endif

  // print the current part key if any
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceKeys ()) {
    os << std::left <<
      std::setw (fieldWidth) <<
      "fPartCurrentKey" << ": ";

    if (fPartCurrentKey) {
      os <<
        "'" <<
        fPartCurrentKey->asShortString () <<
        "'";
    }
    else {
      os <<
        "[NONE]";
    }

    os << std::endl;
  }
#endif

  // print the current part time if any
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceTimeSignatures ()) {
    os << std::left <<
      std::setw (fieldWidth) <<
      "fPartCurrentTimeSignature" << ": ";

    if (fPartCurrentTimeSignature) {
      os <<
        "'" <<
        fPartCurrentTimeSignature->asShortString () <<
        "'";
    }
    else {
      os << "[NONE]";
    }

    os << std::endl;
  }
#endif

  os << std::left <<
    std::setw (fieldWidth) << "fPartShortestNoteDuration" << ": " <<
    fPartShortestNoteDuration <<
    std::endl;

  os << std::left <<
    std::setw (fieldWidth) << "fPartShortestNoteTupletFactor" << ": " <<
    std::endl;

  ++gIndenter;
  os << fPartShortestNoteTupletFactor << std::endl;
  --gIndenter;

  // print the staves list
  size_t partAllStavesListSize =
    fPartAllStavesList.size ();

  os << std::left <<
    std::setw (fieldWidth) <<
    "fPartAllStavesList" << ": ";

  if (partAllStavesListSize) {
    os << std::endl;
    ++gIndenter;

    for (S_msrStaff staff : fPartAllStavesList) {
      os << "\"" << staff->getStaffName () << "\"" << std::endl;
    } // for
    os << std::endl;

    --gIndenter;
  }
  else {
    os << "[NONE]" << std::endl;
  }

  // print the regular staves list
  size_t partRegularStavesListSize =
    fPartRegularStavesList.size ();

  os << std::left <<
    std::setw (fieldWidth) <<
    "fPartRegularStavesList" << ": ";

  if (partRegularStavesListSize) {
    os << std::endl;
    ++gIndenter;

    for (S_msrStaff staff : fPartRegularStavesList) {
      os << "\"" << staff->getStaffName () << "\"" << std::endl;
    } // for
    os << std::endl;

    --gIndenter;
  }
  else {
    os << "[NONE]" << std::endl;
  }

  // print the non harmonies nor figured bass staves list
  size_t partNonHarmoniesNorFiguredBassStavesListSize =
    fPartNonHarmoniesNorFiguredBassStavesList.size ();

  os << std::left <<
    std::setw (fieldWidth) <<
    "fPartNonHarmoniesNorFiguredBassStavesList" << ": ";

  if (partNonHarmoniesNorFiguredBassStavesListSize) {
    os << std::endl;
    ++gIndenter;

    for (S_msrStaff staff : fPartNonHarmoniesNorFiguredBassStavesList) {
      os << "\"" << staff->getStaffName () << "\"" << std::endl;
    } // for
    os << std::endl;

    --gIndenter;
  }
  else {
    os << "[EMPTY]" << std::endl;
  }

  // print part part all voices
  size_t partAllVoicesListSize =
    fPartAllVoicesList.size ();

  os << std::left <<
    std::setw (fieldWidth) <<
    "Voice names in fPartAllVoicesList" << ": ";

  if (partAllVoicesListSize) {
    os << std::endl;
    ++gIndenter;

    for (const S_msrVoice& voice : fPartAllVoicesList) {
      os << "\"" << voice->getVoiceName () << "\"" << std::endl;
    } // for

    --gIndenter;
  }
  else {
    os << "[EMPTY]" << std::endl;
  }

  os << std::endl;

  // print the part harmonies staff if any
  os << std::left <<
    std::setw (fieldWidth) <<
    "fPartHarmoniesStaff" << ": ";
  if (fPartHarmoniesStaff) {
    os << std::endl;
    ++gIndenter;
    os << fPartHarmoniesStaff;
    --gIndenter;
  }
  else {
    os << "[NONE]" << std::endl;
  }

  os << std::endl;

  // print the part figured bass staff if any
  os << std::left <<
    std::setw (fieldWidth) <<
    "fPartFiguredBassStaff" << ": ";
  if (fPartFiguredBassStaff) {
    os << std::endl;

    ++gIndenter;
    os << fPartFiguredBassStaff;
    --gIndenter;
  }
  else {
    os << "[NONE]" << std::endl;
  }

  os << std::endl;

  // print the part measures whole notes durations vector
  printPartMeasuresWholeNotesDurationsVector (
    os,
    fieldWidth);

  os << std::endl;

  // print all the staves
  if (fPartAllStavesList.size ()) {
    std::list<S_msrStaff>::const_iterator
      iBegin = fPartAllStavesList.begin (),
      iEnd   = fPartAllStavesList.end (),
      i      = iBegin;

    for ( ; ; ) {
      S_msrStaff staff = (*i);

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
      // sanity check
      mfAssert (
        __FILE__, __LINE__,
        staff != nullptr,
        "staff is null");
#endif

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

      os << std::endl;
    } // for
  }

  os << std::endl;

  // print the part measures slices sequence
  os <<
    std::setw (fieldWidth) <<
    "fPartMeasuresSlicesSequence" << ": ";

  if (fPartMeasuresSlicesSequence) {
    os << std::endl;
    ++gIndenter;
    os << fPartMeasuresSlicesSequence;
    --gIndenter;
  }
  else {
    os << "[NONE]" << std::endl;
  }

  --gIndenter;

  os << ']' << std::endl;
}

void msrPart::print (std::ostream& os) const
{
  os <<
    "[Part" << ' ' << fPartMsrName <<
    " (" <<
    mfSingularOrPlural (
      fPartAllStavesList.size (), "staff", "staves") <<
    ")" <<
    ", line " << fInputLineNumber <<
    std::endl;

  ++gIndenter;

  const int fieldWidth = 39;

/*
  os << std::left <<
    std::setw (fieldWidth) <<
    "fUpLinkToPartGroup" << ": ";
  if (fPartUpLinkToPartGroup) {
    // it may be empty
    os <<
      fPartUpLinkToPartGroup->getPartGroupCombinedName ();
  }
  else {
    os << "[NONE]";
  }
  os << std::endl;
*/

  os << std::left <<
    std::setw (fieldWidth) <<
    "fPartID" << ": \"" <<
    fPartID << "\"" <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fPartMsrName" << ": \"" <<
    fPartMsrName << "\"" <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fPartAbsoluteNumber" << ": " <<
    fPartAbsoluteNumber <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fPartName" << ": \"" <<
    fPartName << "\"" <<
    std::endl;

  os << std::left <<
    std::setw (fieldWidth) <<
    "fPartHarmoniesStaff" << ": ";
    if (fPartHarmoniesStaff) {
      os <<
        fPartHarmoniesStaff->asShortString ();
    }
    else {
      os << "[NONE]";
    }
  os << std::endl;

  os << std::left <<
    std::setw (fieldWidth) <<
    "fPartHarmoniesVoice" << ": ";
    if (fPartHarmoniesVoice) {
      os <<
        fPartHarmoniesVoice->asShortString ();
    }
    else {
      os << "[NONE]";
    }
  os << std::endl;

  os << std::left <<
    std::setw (fieldWidth) <<
    "fPartFiguredBassStaff" << ": ";
    if (fPartFiguredBassStaff) {
      os <<
        fPartFiguredBassStaff->asShortString ();
    }
    else {
      os << "[NONE]";
    }
  os << std::endl;

  os << std::left <<
    std::setw (fieldWidth) <<
    "fPartFiguredBassVoice" << ": ";
    if (fPartFiguredBassVoice) {
      os <<
        fPartFiguredBassVoice->asShortString ();
    }
    else {
      os << "[NONE]";
    }
  os << std::endl;

  // print the part measure' whole notes durations vector
  printPartMeasuresWholeNotesDurationsVector (
    os,
    fieldWidth);

  // print all the staves
  if (fPartAllStavesList.size ()) {
    os << std::endl;

    std::list<S_msrStaff>::const_iterator
      iBegin = fPartAllStavesList.begin (),
      iEnd   = fPartAllStavesList.end (),
      i      = iBegin;

    for ( ; ; ) {
      S_msrStaff staff = (*i);

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
      // sanity check
      mfAssert (
        __FILE__, __LINE__,
        staff != nullptr,
        "staff is null");
#endif

      msrStaffKind
        staffKind =
          staff->getStaffKind ();

      switch (staffKind) { // JMI v0.9.66
        case msrStaffKind::kStaffKindRegular:
          os << staff;
          break;

        case msrStaffKind::kStaffKindTablature:
          os << staff;
          break;

        case msrStaffKind::kStaffKindHarmonies:
          os << staff;
          break;

        case msrStaffKind::kStaffKindFiguredBass:
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

      os << std::endl;
    } // for
  }

  --gIndenter;

  os << ']' << std::endl;
}

void msrPart::printSummary (std::ostream& os) const
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
    std::endl;

  ++gIndenter;

  const int fieldWidth = 28;

  os << std::left <<
    std::setw (fieldWidth) <<
    "fPartID" << ": \"" <<
    fPartID << "\"" <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fPartMsrName" << ": \"" <<
    fPartMsrName << "\"" <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fPartName" << ": \"" <<
    fPartName << "\"" <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fPartAbsoluteNumber" << ": " <<
    fPartAbsoluteNumber <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fPartNameDisplayText" << ": \"" <<
    fPartNameDisplayText << "\"" <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fPartAbbrevation" << ": \"" <<
    fPartAbbreviation << "\"" <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fPartAbbreviationDisplayText" << ": \"" <<
    fPartAbbreviationDisplayText << "\"" <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fPartInstrumentName" << ": \"" <<
    fPartInstrumentName << "\"" <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fPartInstrumentAbbreviation" << ": \"" <<
    fPartInstrumentAbbreviation << "\"" <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fPartNumberOfMeasures" << ": " <<
    fPartNumberOfMeasures <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fPartContainsMultipleFullBarRests" << ": " <<
    fPartContainsMultipleFullBarRests <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fPartMeasurePosition" << ": " <<
    fPartMeasurePosition <<
    std::endl;

  // print all the staves
  if (fPartAllStavesList.size ()) {
    os <<
      std::setw (fieldWidth) <<
      "fPartAllStavesList" << ": " <<
      std::endl;

    ++gIndenter;

    for (S_msrStaff staff : fPartAllStavesList) {
      staff->printSummary (os);
    } // for

    --gIndenter;
  }

  // print the harmonies staff if any
  if (fPartHarmoniesStaff) {
    os <<
      std::setw (fieldWidth) <<
      "fPartHarmoniesStaff" << ": " <<
      std::endl;

    ++gIndenter;
    os <<
      fPartHarmoniesStaff <<
      std::endl;
    --gIndenter;
  }

  // print the figured bass staff if any
  if (fPartFiguredBassStaff) {
    os <<
      std::setw (fieldWidth) <<
      "fPartFiguredBassStaff" << ": " <<
      std::endl;

    ++gIndenter;
    os <<
      fPartFiguredBassStaff <<
      std::endl;
    --gIndenter;
  }

  --gIndenter;

  os << ']' << std::endl;
}

void msrPart::printSlices (std::ostream& os) const
{
  os << "MSR part slices";

  os << std::endl << std::endl;

  ++gIndenter;

  if (fPartAllStavesList.size ()) {
    std::list<S_msrStaff>::const_iterator
      iBegin = fPartAllStavesList.begin (),
      iEnd   = fPartAllStavesList.end (),
      i      = iBegin;
    for ( ; ; ) {
      (*i)->
        printSlices (os);
      if (++i == iEnd) break;
      os << std::endl;
    } // for
  }

  --gIndenter;
}

std::ostream& operator << (std::ostream& os, const S_msrPart& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NONE]" << std::endl;
  }

  return os;
}


}
