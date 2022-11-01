/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <iomanip>

#include "visitor.h"

#include "mfAssert.h"

#include "mfServiceRunData.h"

#include "msrWae.h"

#include "enableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "tracingOah.h"
#endif

#include "msrPitchesNames.h"

#include "msrMeasures.h"

#include "msrHarmonies.h"

#include "oahOah.h"

#include "msrOah.h"

#include "msrBrowsers.h"


using namespace std;

namespace MusicFormats
{

//______________________________________________________________________________
S_msrHarmonyDegree msrHarmonyDegree::create (
  int                      inputLineNumber,
  int                      harmonyDegreeValue,
  msrAlterationKind        harmonyDegreeAlterationKind,
  msrHarmonyDegreeTypeKind harmonyDegreeTypeKind)
{
  msrHarmonyDegree* o =
    new msrHarmonyDegree (
      inputLineNumber,
      harmonyDegreeValue,
      harmonyDegreeAlterationKind,
      harmonyDegreeTypeKind);
  assert (o != nullptr);

  return o;
}

msrHarmonyDegree::msrHarmonyDegree (
  int                      inputLineNumber,
  int                      harmonyDegreeValue,
  msrAlterationKind        harmonyDegreeAlterationKind,
  msrHarmonyDegreeTypeKind harmonyDegreeTypeKind)
    : msrElement (inputLineNumber)
{
  fHarmonyDegreeValue          = harmonyDegreeValue;
  fHarmonyDegreeAlterationKind = harmonyDegreeAlterationKind;
  fHarmonyDegreeTypeKind       = harmonyDegreeTypeKind;

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceHarmonies ()) {
    gLogStream <<
      "Creating harmony degree '" <<
      asString () <<
      "', line " << inputLineNumber <<
      endl;
  }
#endif
}

msrHarmonyDegree::~msrHarmonyDegree ()
{}

void msrHarmonyDegree::setHarmonyDegreeUpLinkToHarmony (
  S_msrHarmony UpLinkToHarmony)
{
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    UpLinkToHarmony != nullptr,
     "UpLinkToHarmony is null");

  fHarmonyDegreeUpLinkToHarmony =
    UpLinkToHarmony;
}

/* JMI ???
int msrHarmonyDegree::harmonyDegreeAsSemitones () const
{
/ *
  Kind indicates the type of chord. Degree elements
  can then add, subtract, or alter from these
  starting points.
* /

  int result = -1;

  // determine the value for an unaltered degree value
  switch (fHarmonyDegreeValue) {
    case 0:
      result = 0;
      break;
    case 1:
      result = 0;
      break;
    case 2:
      result = 0;
      break;
    case 3:
      result = 0;
      break;
    case 4:
      result = 0;
      break;
    case 5:
      result = 0;
      break;
    case 6:
      result = 0;
      break;
    case 7:
      result = 0;
      break;
    case 8:
      result = 0;
      break;
    case 9:
      result = 0;
      break;
    case 10:
      result = 0;
      break;
    case 11:
      result = 0;
      break;
    case 12:
      result = 0;
      break;
    case 13:
      result = 0;
      break;
  } // switch

  return result;
}
*/

void msrHarmonyDegree::acceptIn (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrHarmonyDegree::acceptIn ()" <<
      endl;
  }

  if (visitor<S_msrHarmonyDegree>*
    p =
      dynamic_cast<visitor<S_msrHarmonyDegree>*> (v)) {
        S_msrHarmonyDegree elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrHarmonyDegree::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrHarmonyDegree::acceptOut (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrHarmonyDegree::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrHarmonyDegree>*
    p =
      dynamic_cast<visitor<S_msrHarmonyDegree>*> (v)) {
        S_msrHarmonyDegree elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrHarmonyDegree::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrHarmonyDegree::browseData (basevisitor* v)
{}

string harmonyDegreeTypeKindAsString (
  msrHarmonyDegreeTypeKind harmonyDegreeTypeKind)
{
  string result;

  switch (harmonyDegreeTypeKind) {
    case msrHarmonyDegreeTypeKind::kHarmonyDegreeTypeAdd:
      result = "kHarmonyDegreeTypeAdd";
      break;
    case msrHarmonyDegreeTypeKind::kHarmonyDegreeTypeAlter:
      result = "kHarmonyDegreeTypeAlter";
      break;
    case msrHarmonyDegreeTypeKind::kHarmonyDegreeTypeSubstract:
      result = "kHarmonyDegreeTypeSubstract";
      break;
  } // switch

  return result;
}

string msrHarmonyDegree::harmonyDegreeKindAsShortString () const
{
  string result;

  switch (fHarmonyDegreeTypeKind) {
    case msrHarmonyDegreeTypeKind::kHarmonyDegreeTypeAdd:
      result = "kHarmonyDegreeTypeAdd";
      break;
    case msrHarmonyDegreeTypeKind::kHarmonyDegreeTypeAlter:
      result = "kHarmonyDegreeTypeAlter";
      break;
    case msrHarmonyDegreeTypeKind::kHarmonyDegreeTypeSubstract:
      result = "kHarmonyDegreeTypeSubstract";
      break;
  } // switch

  return result;
}

string msrHarmonyDegree::asString () const
{
  stringstream s;

  s <<
    "[HarmonyDegree" <<
    ", type: " << harmonyDegreeKindAsShortString () <<
    ", value: " << fHarmonyDegreeValue <<
    ", alteration: " <<
    msrAlterationKindAsString (
      fHarmonyDegreeAlterationKind) <<
    ", line: " << fInputLineNumber <<
    ']';

  return s.str ();
}

void msrHarmonyDegree::print (ostream& os) const
{
  os << asString () << endl;
}

ostream& operator << (ostream& os, const S_msrHarmonyDegree& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NONE]" << endl;
  }

  return os;
}

//______________________________________________________________________________
S_msrHarmony msrHarmony::create (
  int                      inputLineNumber,
  S_msrMeasure             upLinkToMeasure,
  msrQuarterTonesPitchKind harmonyRootQuarterTonesPitchKind,
  msrHarmonyKind           harmonyKind,
  const string&            harmonyKindText,
  int                      harmonyInversion,
  msrQuarterTonesPitchKind harmonyBassQuarterTonesPitchKind,
  const Rational&          harmonySoundingWholeNotes,
  const Rational&          harmonyDisplayWholeNotes,
  int                      harmoniesStaffNumber,
  msrTupletFactor          harmonyTupletFactor,
  const Rational&          harmonyWholeNotesOffset)
{
  msrHarmony* o =
    new msrHarmony (
      inputLineNumber,
      upLinkToMeasure,
      harmonyRootQuarterTonesPitchKind,
      harmonyKind,
      harmonyKindText,
      harmonyInversion,
      harmonyBassQuarterTonesPitchKind,
      harmonySoundingWholeNotes,
      harmonyDisplayWholeNotes,
      harmoniesStaffNumber,
      harmonyTupletFactor,
      harmonyWholeNotesOffset);
  assert (o != nullptr);

  return o;
}

msrHarmony::msrHarmony (
  int                      inputLineNumber,
  S_msrMeasure             upLinkToMeasure,
  msrQuarterTonesPitchKind harmonyRootQuarterTonesPitchKind,
  msrHarmonyKind           harmonyKind,
  const string&            harmonyKindText,
  int                      harmonyInversion,
  msrQuarterTonesPitchKind harmonyBassQuarterTonesPitchKind,
  const Rational&          harmonySoundingWholeNotes,
  const Rational&          harmonyDisplayWholeNotes,
  int                      harmoniesStaffNumber,
  msrTupletFactor          harmonyTupletFactor,
  const Rational&          harmonyWholeNotesOffset)
    : msrMeasureElement (
        inputLineNumber,
        upLinkToMeasure),
      fHarmonyTupletFactor (
        harmonyTupletFactor)
{
  fHarmonyRootQuarterTonesPitchKind =
    harmonyRootQuarterTonesPitchKind;

  fHarmonyKind     = harmonyKind;
  fHarmonyKindText = harmonyKindText;

  fHarmonyInversion = harmonyInversion;

  fHarmonyBassQuarterTonesPitchKind =
    harmonyBassQuarterTonesPitchKind;

  doSetMeasureElementSoundingWholeNotes (
    harmonySoundingWholeNotes,
    "msrHarmony::msrHarmony()");

  fHarmonyDisplayWholeNotes =
    harmonyDisplayWholeNotes;

  // a harmony is considered to be at the beginning of the measure
  // until this is computed in msrMeasure::finalizeHarmonyInHarmoniesMeasure()
  fMeasureElementMeasurePosition = Rational (0, 1);

  fHarmoniesStaffNumber = harmoniesStaffNumber;

  fHarmonyWholeNotesOffset = harmonyWholeNotesOffset;

  // handle harmony inversion if any
  /*
    Inversion is a number indicating which inversion is used:
    0 for root position, 1 for first inversion, etc.
  */
  if (fHarmonyInversion > 0) {
    // fetch the harmony intervals
    S_msrHarmonyStructure
      harmonyStructure =
        msrHarmonyStructure::create (
  // JMI        inputLineNumber,
          fHarmonyKind);

    // fetch the bass harmony item for the inversion
    S_msrHarmonyInterval
      bassHarmonyInterval =
        harmonyStructure->
          bassHarmonyIntervalForHarmonyInversion (
            inputLineNumber,
            fHarmonyInversion);

    // fetch the inverted harmony bass semitones pitch
    msrQuarterTonesPitchKind
      invertedHarmonyBassQuarterTonesPitchKind =
        noteAtIntervalFromQuarterTonesPitch (
          inputLineNumber,
          bassHarmonyInterval->getHarmonyIntervalIntervalKind (),
          fHarmonyRootQuarterTonesPitchKind);

    // is this compatible with bass quartertones pitch if specified?
    if (fHarmonyBassQuarterTonesPitchKind != msrQuarterTonesPitchKind::k_NoQuarterTonesPitch) {
      if (
        invertedHarmonyBassQuarterTonesPitchKind
          !=
        fHarmonyBassQuarterTonesPitchKind
        ) {
        stringstream s;

        s <<
          "inversion '" <<
          fHarmonyInversion <<
          "' is not compatible with bass quaternotes pitch '" <<
          msrQuarterTonesPitchKindAsStringInLanguage (
            fHarmonyBassQuarterTonesPitchKind,
            gGlobalMsrOahGroup->
              getMsrQuarterTonesPitchesLanguageKind ()) <<
          "'";

        msrError (
          gGlobalServiceRunData->getInputSourceName (),
          inputLineNumber,
          __FILE__, __LINE__,
          s.str ());
      }
    }

    // set the bass quartertones pitch according to the inversion
    fHarmonyBassQuarterTonesPitchKind =
      invertedHarmonyBassQuarterTonesPitchKind;
  }

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceHarmonies ()) {
    gLogStream <<
      "Creating harmony " <<
      asString () <<
      endl;
  }
#endif
}

msrHarmony::~msrHarmony ()
{}

S_msrHarmony msrHarmony::createHarmonyNewbornClone (
  S_msrVoice containingVoice)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceHarmonies ()) {
    gLogStream <<
      "Creating a newborn clone of harmony " <<
      asShortString () <<
      ", line " << fInputLineNumber <<
      endl;
  }
#endif

  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    containingVoice != nullptr,
    "containingVoice is null");

  S_msrHarmony
    newbornClone =
      msrHarmony::create (
        fInputLineNumber,
        nullptr, // will be set when harmony is appended to a measure JMI v0.9.66 PIM
        fHarmonyRootQuarterTonesPitchKind,
        fHarmonyKind,
        fHarmonyKindText,
        fHarmonyInversion,
        fHarmonyBassQuarterTonesPitchKind,
        fMeasureElementSoundingWholeNotes,
        fHarmonyDisplayWholeNotes,
        fHarmoniesStaffNumber,
        fHarmonyTupletFactor,
        fHarmonyWholeNotesOffset);

  newbornClone->setHarmoniesUpLinkToVoice (
    containingVoice);

  // frame JMI ???
  newbornClone->fHarmonyFrame =
    fHarmonyFrame;

  return newbornClone;
}

S_msrHarmony msrHarmony::createHarmonyDeepClone (
  S_msrVoice containingVoice)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceHarmonies ()) {
    gLogStream <<
      "Creating a deep clone of harmony " <<
      asShortString () <<
      ", line " << fInputLineNumber <<
      endl;
  }
#endif

  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    containingVoice != nullptr,
    "containingVoice is null");

  S_msrHarmony
    harmonyDeepClone =
      msrHarmony::create (
        fInputLineNumber,
        nullptr, // will be set when harmony is appended to a measure JMI v0.9.66 PIM
        fHarmonyRootQuarterTonesPitchKind,
        fHarmonyKind, fHarmonyKindText,
        fHarmonyInversion,
        fHarmonyBassQuarterTonesPitchKind,
// JMI        fHarmonySoundingWholeNotes,
        fMeasureElementSoundingWholeNotes,
        fHarmonyDisplayWholeNotes,
        fHarmoniesStaffNumber,
        fHarmonyTupletFactor,
        fHarmonyWholeNotesOffset);

  harmonyDeepClone->setHarmoniesUpLinkToVoice (
    containingVoice);

  // JMI popoulate! v0.9.66

  return harmonyDeepClone;
}

void msrHarmony::setHarmonyTupletFactor (
  msrTupletFactor tupletFactor)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceTuplets ()) {
    gLogStream <<
      "Setting the tuplet factor of harmony " <<
      asShortString () <<
      " to " <<
      tupletFactor.asString () <<
      ", line " << fInputLineNumber <<
      endl;
  }
#endif

  fHarmonyTupletFactor = tupletFactor;
}

void msrHarmony::setHarmonyUpLinkToNote (S_msrNote note)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceHarmonies ()) {
    gLogStream <<
      "Setting harmony " << asShortString ()  <<
      " note uplink to " << note->asString () <<
      endl;
  }
#endif

  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    note != nullptr,
    "note is null");

  fHarmonyUpLinkToNote = note;
}

void msrHarmony::setHarmonyMeasurePosition (
  const S_msrMeasure measure,
  const Rational&    measurePosition,
  const string&      context)
{
  // set the harmony position in measure, taking it's offset into account

  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    measure != nullptr,
     "setHarmonyMeasurePosition(): measure is null");

  // the offset can be negative, so we merely add it to measurePosition
  // to obtain the harmony's actual measurePosition
  // this overwrites it with the same value if fHarmonyWholeNotesOffset is null JMI ???
  Rational
    actualMeasurePosition =
      measurePosition
        +
      fHarmonyWholeNotesOffset;

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasurePositions ()) {
    gLogStream <<
      "Setting harmony's position in measure of " << asString () <<
      " to " <<
      measurePosition <<
      " (was " <<
      fMeasureElementMeasurePosition <<
      ") in measure " <<
      measure->asShortString () <<
      " (measureElementMeasureNumber: " <<
      fetchMeasureElementMeasureNumber () <<
      "), context: \"" <<
      context <<
      "\"" <<
      "', harmonyWholeNotesOffset = " <<
      fHarmonyWholeNotesOffset <<
      endl;
  }
#endif

  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    fHarmonyUpLinkToNote != nullptr,
    "fHarmonyUpLinkToNote is null");

  // compute harmony's position in voice
  Rational
    voicePosition =
      measure->
        getMeasureVoicePosition ()
        +
      actualMeasurePosition;

  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    measurePosition != msrMoment::K_NO_POSITION,
    "measurePosition == msrMoment::K_NO_POSITION");

  // set harmony's position in measure
  fMeasureElementMeasurePosition = measurePosition;

  // update current position in voice
  S_msrVoice
    voice =
      measure->
        fetchMeasureUpLinkToVoice ();

  voice->
    incrementCurrentVoicePosition (
      fHarmonyUpLinkToNote->
        getMeasureElementSoundingWholeNotes ());
}

void msrHarmony::setHarmonyFrame (S_msrFrame frame)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceFrames ()) {
    gLogStream <<
      "Setting harmony " << asShortString ()  <<
      " frame to " << frame->asString () <<
      endl;
  }
#endif

  fHarmonyFrame = frame;
}

void msrHarmony::incrementHarmonySoundingWholeNotesDuration (
  int             inputLineNumber,
  const Rational& wholeNotesDelta)
{
  // compute currentHarmony's future sounding whole notes
  Rational
    augmentedSoundingWholeNotes =
      fMeasureElementSoundingWholeNotes
        +
      wholeNotesDelta;

  // extend currentHarmony's sounding whole notes
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceHarmonies ()) {
    gLogStream <<
      "Extending the sounding whole notes of harmony " <<
      asString () <<
      " from " <<
      fMeasureElementSoundingWholeNotes <<
      " to " <<
      augmentedSoundingWholeNotes <<
      " to fill measure " <<
      this->asShortString () <<
      " in voice \"" <<
      fHarmoniesUpLinkToVoice->getVoiceName () <<
      "\", line " << inputLineNumber <<
      endl;
  }
#endif

  setMeasureElementSoundingWholeNotes (
    augmentedSoundingWholeNotes,
    "incrementHarmonySoundingWholeNotesDuration()");
}

void msrHarmony::acceptIn (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrHarmony::acceptIn ()" <<
      endl;
  }

  if (visitor<S_msrHarmony>*
    p =
      dynamic_cast<visitor<S_msrHarmony>*> (v)) {
        S_msrHarmony elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrHarmony::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrHarmony::acceptOut (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrHarmony::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrHarmony>*
    p =
      dynamic_cast<visitor<S_msrHarmony>*> (v)) {
        S_msrHarmony elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrHarmony::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrHarmony::browseData (basevisitor* v)
{
  // browse harmony degrees if any
  if (fHarmonyDegreesList.size ()) {
    for (
      list<S_msrHarmonyDegree>::const_iterator i = fHarmonyDegreesList.begin ();
      i != fHarmonyDegreesList.end ();
      ++i
    ) {
      // browse the harmony degree
      msrBrowser<msrHarmonyDegree> browser (v);
      browser.browse (*(*i));
    } // for
  }

  if (fHarmonyFrame) {
    // browse the frame
    msrBrowser<msrFrame> browser (v);
    browser.browse (*fHarmonyFrame);
  }
}

string msrHarmony::asString () const
{
  stringstream s;

  s <<
    "[Harmony" <<
    ", fHarmonyRootQuarterTonesPitchKind: " <<
    msrQuarterTonesPitchKindAsStringInLanguage (
      fHarmonyRootQuarterTonesPitchKind,
      gGlobalMsrOahGroup->
        getMsrQuarterTonesPitchesLanguageKind ()) <<
    ", fHarmonyKind: " <<
    msrHarmonyKindAsShortString (fHarmonyKind) <<

    ", fMeasureElementSoundingWholeNotes: " <<
    fMeasureElementSoundingWholeNotes <<
    ", fHarmonyDisplayWholeNotes: " <<
    fHarmonyDisplayWholeNotes <<

    ", fHarmonyWholeNotesOffset: " <<
    fHarmonyWholeNotesOffset <<
    ", fMeasureElementMeasurePosition: " <<
    fMeasureElementMeasurePosition <<

    ", fHarmonyKindText: \"" <<
    fHarmonyKindText << "\"";

  s << ", fHarmonyInversion: ";
  if (fHarmonyInversion == K_HARMONY_NO_INVERSION) {
    s << "[NONE]";
  }
  else {
    s << fHarmonyInversion;
  }

  if (fHarmonyBassQuarterTonesPitchKind != msrQuarterTonesPitchKind::k_NoQuarterTonesPitch) {
    s <<
      ", fHarmonyBassQuarterTonesPitchKind: " <<
    msrQuarterTonesPitchKindAsStringInLanguage (
      fHarmonyBassQuarterTonesPitchKind,
      gGlobalMsrOahGroup->
        getMsrQuarterTonesPitchesLanguageKind ());
  }

  if (fHarmonyDegreesList.size ()) {
    s <<
      ", fHarmonyDegreesList: [";

    list<S_msrHarmonyDegree>::const_iterator
      iBegin = fHarmonyDegreesList.begin (),
      iEnd   = fHarmonyDegreesList.end (),
      i      = iBegin;

    for ( ; ; ) {
      s << (*i)->asString ();
      if (++i == iEnd) break;
      s << ' ';
    } // for
  }

  // print the harmonies staff number
  s <<
    ", fHarmoniesStaffNumber: ";
  if (fHarmoniesStaffNumber == msrStaff::K_NO_STAFF_NUMBER)
    s << "[NONE]";
  else
    s << fHarmoniesStaffNumber;

  // print the harmony tuplet factor
  s <<
    ", fHarmonyTupletFactor: " <<
    fHarmonyTupletFactor.asString ();

  // print the harmony frame
  s << ", fHarmonyFrame: ";
  if (fHarmonyFrame) {
    s << fHarmonyFrame;
  }
  else {
    s << "[NONE]";
  }

  // print the harmony note uplink
  s << ", fHarmonyUpLinkToNote: ";
  if (fHarmonyUpLinkToNote) {
    s << fHarmonyUpLinkToNote;
  }
  else {
    s << "[NONE]";
  }

  s <<
    ", line " << fInputLineNumber <<
    ']';

  return s.str ();
}

void msrHarmony::print (ostream& os) const
{
  os <<
    "[Harmony" <<
     ", line " << fInputLineNumber <<
    endl;

  ++gIndenter;

  const int fieldWidth = 44;

  os << left <<
    setw (fieldWidth) <<
    "harmonyRoot" << " : " <<
    msrQuarterTonesPitchKindAsStringInLanguage (
      fHarmonyRootQuarterTonesPitchKind,
      gGlobalMsrOahGroup->
        getMsrQuarterTonesPitchesLanguageKind ()) <<
    endl <<
    setw (fieldWidth) <<
    "fHarmonyKind" << " : " <<
    msrHarmonyKindAsString (fHarmonyKind) <<
    endl <<

    setw (fieldWidth) <<
    "fMeasureElementSoundingWholeNotes" << " : " <<
    fMeasureElementSoundingWholeNotes <<
    endl <<
    setw (fieldWidth) <<
    "fHarmonyDisplayWholeNotes" << " : " <<
    fHarmonyDisplayWholeNotes <<
    endl;

  // print the harmony whole notes offset
  os <<
    setw (fieldWidth) <<
    "fHarmonyWholeNotesOffset" << " : " << fHarmonyWholeNotesOffset <<
    endl;

  os <<
    setw (fieldWidth) <<
    "fHarmonyKindText" << " : \"" <<
    fHarmonyKindText <<
    "\"" <<
    endl <<

    setw (fieldWidth) <<
    "fHarmonyBassQuarterTonesPitchKind" << " : " <<
    msrQuarterTonesPitchKindAsStringInLanguage (
      fHarmonyBassQuarterTonesPitchKind,
      gGlobalMsrOahGroup->
        getMsrQuarterTonesPitchesLanguageKind ()) <<
    endl;

  os <<
    setw (fieldWidth) <<
    "fHarmonyInversion" << " : ";
  if (fHarmonyInversion == K_HARMONY_NO_INVERSION) {
    os << "[NONE]";
  }
  else {
    os << fHarmonyInversion;
  }
  os << endl;

  // print harmony degrees if any
  os <<
    setw (fieldWidth) <<
    "fHarmonyDegreesList";

  if (fHarmonyDegreesList.size ()) {
    os << endl;

    ++gIndenter;

    list<S_msrHarmonyDegree>::const_iterator
      iBegin = fHarmonyDegreesList.begin (),
      iEnd   = fHarmonyDegreesList.end (),
      i      = iBegin;

    for ( ; ; ) {
      os <<
        (*i)->asString ();
      if (++i == iEnd) break;
      os << endl;
    } // for

    os << endl;

    --gIndenter;
  }
  else {
    os <<
      " : " <<
      "[NONE]" <<
      endl;
  }

  // print the harmonies staff number
  os <<
    setw (fieldWidth) <<
    "fHarmoniesStaffNumber" << " : ";
  if (fHarmoniesStaffNumber == msrStaff::K_NO_STAFF_NUMBER) {
    os << "[NONE]";
  }
  else {
    os << fHarmoniesStaffNumber;
  }
  os << endl;

  // print the harmony tuplet factor
  os <<
    setw (fieldWidth) <<
    "fHarmonyTupletFactor" << " : " << fHarmonyTupletFactor.asString () <<
    endl;

  // print the harmony frame
  os <<
    setw (fieldWidth) <<
    "fHarmonyFrame" << " : ";
  if (fHarmonyFrame) {
    os << fHarmonyFrame;
  }
  else {
    os << "[NONE]";
  }
  os << endl;

  // print the harmony measure number
  os <<
    setw (fieldWidth) <<
    "measureElementMeasureNumber" << " : " << fetchMeasureElementMeasureNumber () <<
    endl;

  // print the harmony position in measure
  os <<
    setw (fieldWidth) <<
    "fMeasureElementMeasurePosition" << " : " << fMeasureElementMeasurePosition <<
    endl;

  // print the harmony bass position in voice
//   os <<
//     setw (fieldWidth) <<
//     "fMeasureElementVoicePosition" << " : " << fMeasureElementVoicePosition <<
//     endl;

  // print the harmony note uplink
  os <<
    setw (fieldWidth) <<
    "fHarmonyUpLinkToNote" << " : ";
  if (fHarmonyUpLinkToNote) {
    os <<
      endl <<
      gTab << fHarmonyUpLinkToNote->asString ();
  }
  else {
    os << "[NONE]";
  }
  os << endl;

  --gIndenter;

  os << ']' << endl;
}

ostream& operator << (ostream& os, const S_msrHarmony& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NONE]" << endl;
  }

  return os;
}


}
