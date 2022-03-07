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

void msrHarmonyDegree::setHarmonyDegreeHarmonyUpLink (
  S_msrHarmony harmonyUpLink)
{
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    harmonyUpLink != nullptr,
     "harmonyUpLink is null");

  fHarmonyDegreeHarmonyUpLink =
    harmonyUpLink;
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

string msrHarmonyDegree::harmonyDegreeTypeKindAsString (
  msrHarmonyDegreeTypeKind harmonyDegreeTypeKind)
{
  string result;

  switch (harmonyDegreeTypeKind) {
    case msrHarmonyDegree::kHarmonyDegreeTypeAdd:
      result = "harmonyDegreeTypeAdd";
      break;
    case msrHarmonyDegree::kHarmonyDegreeTypeAlter:
      result = "harmonyDegreeTypeAlter";
      break;
    case msrHarmonyDegree::kHarmonyDegreeTypeSubstract:
      result = "harmonyDegreeTypeSubstract";
      break;
  } // switch

  return result;
}

string msrHarmonyDegree::harmonyDegreeKindAsShortString () const
{
  string result;

  switch (fHarmonyDegreeTypeKind) {
    case msrHarmonyDegree::kHarmonyDegreeTypeAdd:
      result = "degreeAdd";
      break;
    case msrHarmonyDegree::kHarmonyDegreeTypeAlter:
      result = "degreeAlter";
      break;
    case msrHarmonyDegree::kHarmonyDegreeTypeSubstract:
      result = "degreeSubtract";
      break;
  } // switch

  return result;
}

string msrHarmonyDegree::asString () const
{
  stringstream s;

  s <<
    "HarmonyDegree" <<
    ", type: " << harmonyDegreeKindAsShortString () <<
    ", value: " << fHarmonyDegreeValue <<
    ", alteration: " <<
    msrAlterationKindAsString (
      fHarmonyDegreeAlterationKind) <<
    ", line: " << fInputLineNumber;

  return s.str ();
}

void msrHarmonyDegree::print (ostream& os) const
{
  os << asString () << endl;
}

ostream& operator<< (ostream& os, const S_msrHarmonyDegree& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_msrHarmony msrHarmony::createWithoutVoiceUplink (
  int                      inputLineNumber,
  // no harmoniesVoiceUpLink yet
  msrQuarterTonesPitchKind harmonyRootQuarterTonesPitchKind,
  msrHarmonyKind           harmonyKind,
  const string&            harmonyKindText,
  int                      harmonyInversion,
  msrQuarterTonesPitchKind harmonyBassQuarterTonesPitchKind,
  const rational&          harmonySoundingWholeNotes,
  const rational&          harmonyDisplayWholeNotes,
  int                      harmoniesStaffNumber,
  msrTupletFactor          harmonyTupletFactor,
  const rational&          harmonyWholeNotesOffset)
{
  msrHarmony* o =
    new msrHarmony (
      inputLineNumber,
      nullptr, // will be set afterwards
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

S_msrHarmony msrHarmony::createWithVoiceUplink (
  int                      inputLineNumber,
  S_msrVoice               harmoniesVoiceUpLink,
  msrQuarterTonesPitchKind harmonyRootQuarterTonesPitchKind,
  msrHarmonyKind           harmonyKind,
  const string&            harmonyKindText,
  int                      harmonyInversion,
  msrQuarterTonesPitchKind harmonyBassQuarterTonesPitchKind,
  const rational&          harmonySoundingWholeNotes,
  const rational&          harmonyDisplayWholeNotes,
  int                      harmoniesStaffNumber,
  msrTupletFactor          harmonyTupletFactor,
  const rational&          harmonyWholeNotesOffset)
{
  msrHarmony* o =
    new msrHarmony (
      inputLineNumber,
      harmoniesVoiceUpLink,
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
  S_msrVoice               harmoniesVoiceUpLink,
  msrQuarterTonesPitchKind harmonyRootQuarterTonesPitchKind,
  msrHarmonyKind           harmonyKind,
  const string&            harmonyKindText,
  int                      harmonyInversion,
  msrQuarterTonesPitchKind harmonyBassQuarterTonesPitchKind,
  const rational&          harmonySoundingWholeNotes,
  const rational&          harmonyDisplayWholeNotes,
  int                      harmoniesStaffNumber,
  msrTupletFactor          harmonyTupletFactor,
  const rational&          harmonyWholeNotesOffset)
    : msrMeasureElement (
        inputLineNumber),
      fHarmonyTupletFactor (
        harmonyTupletFactor)
{
  /* JMI
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    harmoniesVoiceUpLink != nullptr,
     "harmoniesVoiceUpLink is null");
     */

  // set harmony's voice upLink
  fHarmoniesVoiceUpLink =
    harmoniesVoiceUpLink;

  fHarmonyRootQuarterTonesPitchKind =
    harmonyRootQuarterTonesPitchKind;

  fHarmonyKind     = harmonyKind;
  fHarmonyKindText = harmonyKindText;

  fHarmonyInversion = harmonyInversion;

  fHarmonyBassQuarterTonesPitchKind =
    harmonyBassQuarterTonesPitchKind;

// JMI  fHarmonySoundingWholeNotes =
  fMeasureElementSoundingWholeNotes =
    harmonySoundingWholeNotes;
  fHarmonyDisplayWholeNotes =
    harmonyDisplayWholeNotes;

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
      msrHarmony::createWithVoiceUplink (
        fInputLineNumber,
        containingVoice,
        fHarmonyRootQuarterTonesPitchKind,
        fHarmonyKind,
        fHarmonyKindText,
        fHarmonyInversion,
        fHarmonyBassQuarterTonesPitchKind,
// JMI        fHarmonySoundingWholeNotes,
        fMeasureElementSoundingWholeNotes,
        fHarmonyDisplayWholeNotes,
        fHarmoniesStaffNumber,
        fHarmonyTupletFactor,
        fHarmonyWholeNotesOffset);

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
      msrHarmony::createWithVoiceUplink (
        fInputLineNumber,
        containingVoice,
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

void msrHarmony::setHarmonyFrame (S_msrFrame frame)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceFrames ()) {
    gLogStream <<
      "Setting harmony '" << asShortString ()  << "'" <<
      " frame to '" << frame->asString () << "'" <<
      endl;
  }
#endif

  fHarmonyFrame = frame;
}

void msrHarmony::setHarmonyPositionInMeasure (
  const rational& positionInMeasure)
{
  // set the harmony position in measure, taking it's offset into account

  // the offset can be negative, so we merely add it to positionInMeasure
  // to obtain the harmony's actual positionInMeasure
  // this overwrites it with the same value if fHarmonyWholeNotesOffset is null JMI ???
  rational
    actualPositionInMeasure =
      positionInMeasure
        +
      fHarmonyWholeNotesOffset;
  actualPositionInMeasure.rationalise ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTracePositionsInMeasures ()) {
    gLogStream <<
      "Setting actual harmony position in measure of " << asString () <<
      " to '" <<
      actualPositionInMeasure <<
      "', positionInMeasure = " <<
      positionInMeasure <<
      "', harmonyWholeNotesOffset = " <<
      fHarmonyWholeNotesOffset <<
      endl;
  }
#endif

  string context =
    "setHarmonyPositionInMeasure()";

  setMeasureElementPositionInMeasure (
    actualPositionInMeasure,
    context);

  // compute harmony's position in voice
  S_msrMeasure
    measure =
      fHarmonyNoteUpLink->
        getNoteDirectMeasureUpLink ();

  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    measure != nullptr,
     "setHarmonyPositionInMeasure(): measure is null");

  rational
    positionInVoice =
      measure->
        getMeasurePositionInVoice ()
        +
      actualPositionInMeasure;
  positionInVoice.rationalise ();

  // set harmony's position in voice
  setMeasureElementPositionInVoice (
    positionInVoice,
    context);

  // update current position in voice
  S_msrVoice
    voice =
      measure->
        fetchMeasureVoiceUpLink ();

  voice->
    incrementCurrentPositionInVoice (
      fHarmonyNoteUpLink->
        getNoteSoundingWholeNotes ());
}

void msrHarmony::incrementHarmonySoundingWholeNotesDuration (
  int             inputLineNumber,
  const rational& wholeNotesDelta)
{
  // compute currentHarmony's future sounding whole notes
  rational
    augmentedSoundingWholeNotes =
      fMeasureElementSoundingWholeNotes
        +
      wholeNotesDelta;
  augmentedSoundingWholeNotes.rationalise ();

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
      fHarmoniesVoiceUpLink->getVoiceName () <<
      "\", line " << inputLineNumber <<
      endl;
  }
#endif

  setHarmonySoundingWholeNotes (
    augmentedSoundingWholeNotes);
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
    ", fMeasureElementPositionInMeasure: " <<
    fMeasureElementPositionInMeasure <<

    ", fHarmonyKindText: \"" <<
    fHarmonyKindText << "\"";

  s << ", fHarmonyInversion: ";
  if (fHarmonyInversion == K_HARMONY_NO_INVERSION) {
    s << "none";
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
    ", harmoniesStaffNumber: ";
  if (fHarmoniesStaffNumber == msrStaff::K_NO_STAFF_NUMBER)
    s << "none";
  else
    s << fHarmoniesStaffNumber;

  // print the harmony tuplet factor
  s <<
    ", harmonyTupletFactor: " <<
    fHarmonyTupletFactor.asString ();

  // print the harmony frame
  s << ", harmonyFrame: ";
  if (fHarmonyFrame) {
    s << fHarmonyFrame;
  }
  else {
    s << "none";
  }

  s <<
    ", line " << fInputLineNumber <<
    "]";

  return s.str ();
}

void msrHarmony::print (ostream& os) const
{
  os <<
    "[Harmony" <<
     ", line " << fInputLineNumber <<
    endl;

  ++gIndenter;

  const int fieldWidth = 26;

  os << left <<
    setw (fieldWidth) <<
    "harmonyRoot" << " : " <<
    msrQuarterTonesPitchKindAsStringInLanguage (
      fHarmonyRootQuarterTonesPitchKind,
      gGlobalMsrOahGroup->
        getMsrQuarterTonesPitchesLanguageKind ()) <<
    endl <<
    setw (fieldWidth) <<
    "harmonyKind" << " : " <<
    msrHarmonyKindAsString (fHarmonyKind) <<
    endl <<

    setw (fieldWidth) <<
    "harmonySoundingWholeNotes" << " : " <<
    fMeasureElementSoundingWholeNotes <<
    endl <<
    setw (fieldWidth) <<
    "harmonyDisplayWholeNotes" << " : " <<
    fHarmonyDisplayWholeNotes <<
    endl;

  // print the harmony whole notes offset
  os <<
    setw (fieldWidth) <<
    "harmonyWholeNotesOffset" << " : " << fHarmonyWholeNotesOffset <<
    endl;

  // print the harmony position in measure
  os <<
    setw (fieldWidth) <<
    "positionInMeasure" << " : " << fMeasureElementPositionInMeasure <<
    endl;

  // print the harmony bass position in voice
  os <<
    setw (fieldWidth) <<
    "positionInVoice" << " : " << fMeasureElementPositionInVoice <<
    endl;

  os <<
    setw (fieldWidth) <<
    "harmonyKindText" << " : \"" <<
    fHarmonyKindText <<
    "\"" <<
    endl <<

    setw (fieldWidth) <<
    "harmonyBass" << " : " <<
    msrQuarterTonesPitchKindAsStringInLanguage (
      fHarmonyBassQuarterTonesPitchKind,
      gGlobalMsrOahGroup->
        getMsrQuarterTonesPitchesLanguageKind ()) <<
    endl;

  os <<
    setw (fieldWidth) <<
    "harmonyInversion" << " : ";
  if (fHarmonyInversion == K_HARMONY_NO_INVERSION) {
    os << "none";
  }
  else {
    os << fHarmonyInversion;
  }
  os << endl;

  // print harmony degrees if any
  os <<
    setw (fieldWidth) <<
    "harmonyDegrees";

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
      "none" <<
      endl;
  }

  // print the harmonies staff number
  os <<
    setw (fieldWidth) <<
    "harmoniesStaffNumber" << " : ";
  if (fHarmoniesStaffNumber == msrStaff::K_NO_STAFF_NUMBER) {
    os << "none";
  }
  else {
    os << fHarmoniesStaffNumber;
  }
  os << endl;

  // print the harmony tuplet factor
  os <<
    setw (fieldWidth) <<
    "harmonyTupletFactor" << " : " << fHarmonyTupletFactor.asString () <<
    endl;

  // print the harmony frame
  os <<
    setw (fieldWidth) <<
    "harmonyFrame" << " : ";
  if (fHarmonyFrame) {
    os << fHarmonyFrame;
  }
  else {
    os << "none";
  }
  os << endl;

  // print the harmony note uplink
  os <<
    setw (fieldWidth) <<
    "harmonyNoteUpLink" << " : ";
  if (fHarmonyNoteUpLink) {
    os <<
      endl <<
      gTab << fHarmonyNoteUpLink->asString ();
  }
  else {
    os << "none";
  }
  os << endl;

  --gIndenter;

  os << ']' << endl;
}

ostream& operator<< (ostream& os, const S_msrHarmony& elt)
{
  elt->print (os);
  return os;
}


}
