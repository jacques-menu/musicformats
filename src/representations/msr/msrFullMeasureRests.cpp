/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include "visitor.h"

#include "mfAssert.h"

#include "mfServiceRunData.h"

#include "mfStringsHandling.h"

#include "msrWae.h"

#include "enableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "tracingOah.h"
#endif

#include "msrFullMeasureRests.h"

#include "oahOah.h"

#include "msrOah.h"

#include "msrBrowsers.h"


using namespace std;

namespace MusicFormats
{

//______________________________________________________________________________
S_msrFullMeasureRestsContents msrFullMeasureRestsContents::create (
  int                   inputLineNumber,
  S_msrFullMeasureRests fullMeasureRests)
{
  msrFullMeasureRestsContents* o =
    new msrFullMeasureRestsContents (
      inputLineNumber,
      fullMeasureRests);
  assert (o != nullptr);
  return o;
}

msrFullMeasureRestsContents::msrFullMeasureRestsContents (
  int                  inputLineNumber,
  S_msrFullMeasureRests fullMeasureRests)
    : msrElement (inputLineNumber)
{
  fFullMeasureRestsContentsFullMeasureRestsUpLink =
    fullMeasureRests;
}

msrFullMeasureRestsContents::~msrFullMeasureRestsContents ()
{}

S_msrFullMeasureRestsContents msrFullMeasureRestsContents::createFullMeasureRestsContentsNewbornClone (
  S_msrFullMeasureRests fullMeasureRests)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceFullMeasureRests ()) {
    gLogStream <<
      "Creating a newborn clone of a full measure rests contents" <<
      endl;
  }
#endif

  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    fullMeasureRests != nullptr,
    "fullMeasureRests is null");

  S_msrFullMeasureRestsContents
    newbornClone =
      msrFullMeasureRestsContents::create (
        fInputLineNumber,
        fullMeasureRests);

  return newbornClone;
}

void msrFullMeasureRestsContents::setFullMeasureRestsContentsSegment (
  int          inputLineNumber,
  S_msrSegment fullMeasureRestsContentsSegment)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceFullMeasureRests ()) {
    gLogStream <<
      "Setting full measure rests contents segment containing " <<
      mfSingularOrPlural (
        fullMeasureRestsContentsMeasuresNumber (),
        "measure",
        "measures") <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    fullMeasureRestsContentsSegment != nullptr,
    "fullMeasureRestsContentsSegment is null");

  fFullMeasureRestsContentsSegment =
    fullMeasureRestsContentsSegment;
}

int msrFullMeasureRestsContents::fullMeasureRestsContentsMeasuresNumber () const
{
  int result;

  if (fFullMeasureRestsContentsSegment) {
    result =
      fFullMeasureRestsContentsSegment->
      getSegmentMeasuresList ().size ();
  }
  else {
    result = 0;
  }

  return result;
}

void msrFullMeasureRestsContents::acceptIn (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrFullMeasureRestsContents::acceptIn ()" <<
      endl;
  }

  if (visitor<S_msrFullMeasureRestsContents>*
    p =
      dynamic_cast<visitor<S_msrFullMeasureRestsContents>*> (v)) {
        S_msrFullMeasureRestsContents elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrFullMeasureRestsContents::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrFullMeasureRestsContents::acceptOut (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrFullMeasureRestsContents::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrFullMeasureRestsContents>*
    p =
      dynamic_cast<visitor<S_msrFullMeasureRestsContents>*> (v)) {
        S_msrFullMeasureRestsContents elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrFullMeasureRestsContents::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrFullMeasureRestsContents::browseData (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrFullMeasureRestsContents::browseData ()" <<
      endl;
  }

  if (fFullMeasureRestsContentsSegment) {
    // browse the pattern segment
    msrBrowser<msrSegment> browser (v);
    browser.browse (*fFullMeasureRestsContentsSegment);
  }
}

string msrFullMeasureRestsContents::asString () const
{
  stringstream s;

  s <<

    "FullMeasureRestsContents" <<
    ", line " << fInputLineNumber <<
    " (" <<
    mfSingularOrPlural (
      fullMeasureRestsContentsMeasuresNumber (),
      "contents measure",
      "contents measures") <<
    ")";

  return s.str ();
}

void msrFullMeasureRestsContents::print (ostream& os) const
{
  os <<
    endl <<
    "[" <<
    asString () <<
    endl;

  ++gIndenter;

  // print the pattern segment
  os <<
    "Contents segment:";

  if (! fFullMeasureRestsContentsSegment) {
    os <<
      " none" <<
      endl;
  }
  else {
    os << endl;

    ++gIndenter;

    os <<
      fFullMeasureRestsContentsSegment;

    --gIndenter;
  }

  --gIndenter;

  os << "]" << endl;
}

ostream& operator<< (ostream& os, const S_msrFullMeasureRestsContents& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_msrFullMeasureRests msrFullMeasureRests::create (
  int              inputLineNumber,
  const rational&  fullMeasureRestsMeasureSoundingNotes,
  int              fullMeasureRestsNumber,
  S_msrVoice       voiceUpLink)
{
  msrFullMeasureRests* o =
    new msrFullMeasureRests (
      inputLineNumber,
      fullMeasureRestsMeasureSoundingNotes,
      fullMeasureRestsNumber,
      voiceUpLink);
  assert (o != nullptr);
  return o;
}

S_msrFullMeasureRests msrFullMeasureRests::create (
  int          inputLineNumber,
  S_msrMeasure restMeasureClone,
  S_msrVoice   voiceUpLink)
{
  msrFullMeasureRests* o =
    new msrFullMeasureRests (
      inputLineNumber,
      restMeasureClone,
      voiceUpLink);
  assert (o != nullptr);
  return o;
}

msrFullMeasureRests::msrFullMeasureRests (
  int             inputLineNumber,
  const rational& fullMeasureRestsMeasureSoundingNotes,
  int             fullMeasureRestsNumber,
  S_msrVoice      voiceUpLink)
    : msrVoiceElement (inputLineNumber)
{
  fFullMeasureRestsVoiceUpLink = voiceUpLink;

  fFullMeasureRestsMeasureSoundingNotes = fullMeasureRestsMeasureSoundingNotes;

  fFullMeasureRestsNumber = fullMeasureRestsNumber;

  fFullMeasureRestsLastMeasurePuristNumber = -1;
}

msrFullMeasureRests::msrFullMeasureRests (
  int          inputLineNumber,
  S_msrMeasure restMeasureClone,
  S_msrVoice   voiceUpLink)
    : msrVoiceElement (inputLineNumber)
{
  fFullMeasureRestsVoiceUpLink = voiceUpLink;

  fFullMeasureRestsMeasureSoundingNotes =
    restMeasureClone->
      getFullMeasureWholeNotesDuration (); // JMI ???

  fFullMeasureRestsNumber = 1; // will evolve JMI

  fFullMeasureRestsLastMeasurePuristNumber = -1;

  // create the full measure rests contents
  fFullMeasureRestsContents =
    msrFullMeasureRestsContents::create (
      inputLineNumber,
      this);
}

msrFullMeasureRests::~msrFullMeasureRests ()
{}

S_msrFullMeasureRests msrFullMeasureRests::createFullMeasureRestsNewbornClone (
  S_msrVoice containingVoice)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceFullMeasureRests ()) {
    gLogStream <<
      "Creating a newborn clone of full measure rests '" <<
      asString () <<
      "'" <<
      endl;
  }
#endif

  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    containingVoice != nullptr,
    "containingVoice is null");

  S_msrFullMeasureRests
    newbornClone =
      msrFullMeasureRests::create (
        fInputLineNumber,
        fFullMeasureRestsMeasureSoundingNotes,
        fFullMeasureRestsNumber,
        containingVoice);

/* JMI
  newbornClone->fFullMeasureRestsNextMeasureNumber =
    fFullMeasureRestsNextMeasureNumber;
    */

  return newbornClone;
}

void msrFullMeasureRests::setFullMeasureRestsContents (
  S_msrFullMeasureRestsContents fullMeasureRestsContents)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceFullMeasureRests ()) {
    gLogStream <<
      "Setting full measure rests contents containing " <<
      mfSingularOrPlural (
        fullMeasureRestsContents->
          fullMeasureRestsContentsMeasuresNumber (),
        "measure",
        "measures") <<
      endl;
  }
#endif

  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    fullMeasureRestsContents != nullptr,
    "fullMeasureRestsContents is null");

  fFullMeasureRestsContents = fullMeasureRestsContents;
}

void msrFullMeasureRests::setFullMeasureRestsNextMeasureNumber (
  const string& nextMeasureNumber)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceFullMeasureRests ()) {
    gLogStream <<
      "Setting full measure rests next measure number to '" <<
      "' " <<
      nextMeasureNumber <<
      endl;
  }
#endif

  fFullMeasureRestsNextMeasureNumber =
    nextMeasureNumber;
}

void msrFullMeasureRests::setFullMeasureRestsLastMeasurePuristMeasureNumber (
  int inputLineNumber)
{
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    fFullMeasureRestsContents != nullptr,
    "fFullMeasureRestsContents is null");

  S_msrSegment
    fullMeasureRestsContentsSegment =
      fFullMeasureRestsContents->
        getFullMeasureRestsContentsSegment ();

  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    fullMeasureRestsContentsSegment != nullptr,
    "fullMeasureRestsContentsSegment is null");

  // get full measure rests contents segment measures list
  const list<S_msrMeasure>&
    contentsSegmentMeasuresList =
      fullMeasureRestsContentsSegment->
        getSegmentMeasuresList ();

  // get full measure rests contents last measure's purist number
  int lastMeasuresPuristNumber = -1;

  if (contentsSegmentMeasuresList.size ()) {
    lastMeasuresPuristNumber =
      contentsSegmentMeasuresList.back ()->
        getMeasurePuristNumber ();
  }
  else {
    stringstream s;

    s <<
      "cannot get full measure rests contents last measure purist number" <<
      " because its measures list is empty" <<
      " in voice clone '" <<
      asShortString () <<
      "' ";

    msrInternalError (
      gGlobalServiceRunData->getInputSourceName (),
      fInputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceFullMeasureRests ()) {
    gLogStream <<
      "Setting full measure rests last measure purist number to '" <<
      "' " <<
      lastMeasuresPuristNumber <<
      "', line " << inputLineNumber <<
      endl;
  }
#endif

  fFullMeasureRestsLastMeasurePuristNumber =
    lastMeasuresPuristNumber;
}

void msrFullMeasureRests::appendMeasureCloneToFullMeasureRests (
  S_msrMeasure measureClone)
{
  fFullMeasureRestsContents->
    getFullMeasureRestsContentsSegment ()->
      appendMeasureToSegment (
        measureClone);
}

void msrFullMeasureRests::acceptIn (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrFullMeasureRests::acceptIn ()" <<
      endl;
  }

  if (visitor<S_msrFullMeasureRests>*
    p =
      dynamic_cast<visitor<S_msrFullMeasureRests>*> (v)) {
        S_msrFullMeasureRests elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrFullMeasureRests::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrFullMeasureRests::acceptOut (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrFullMeasureRests::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrFullMeasureRests>*
    p =
      dynamic_cast<visitor<S_msrFullMeasureRests>*> (v)) {
        S_msrFullMeasureRests elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrFullMeasureRests::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrFullMeasureRests::browseData (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrFullMeasureRests::browseData ()" <<
      endl;
  }

/* JMI
  // get the part upLink
  S_msrPart
    partUpLink =
      fFullMeasureRestsVoiceUpLink->
        fetchVoicePartUpLink ();

  // get the part group upLink
  S_msrPartGroup
    partGroupUpLink =
      partUpLink->
        getPartPartGroupUpLink ();

  // get the score upLink
  S_msrScore
    scoreUpLink =
      partGroupUpLink->
        getPartGroupScoreUpLink ();

  Bool inhibitFullMeasureRestsBrowsing =
    scoreUpLink->
      getInhibitFullMeasureRestsBrowsing ();

  if (inhibitFullMeasureRestsBrowsing) {
    if (gGlobalMsrOahGroup->getTraceMsrVisitors () || gGlobalTracingOahGroup->getTraceFullMeasureRests ()) {
      gLogStream <<
        "% ==> visiting full measure rests is inhibited" <<
        endl;
    }
  }
*/

  if (fFullMeasureRestsContents) {
 // JMI   if (! inhibitFullMeasureRestsBrowsing) {
      // browse the full measure rests contents
      msrBrowser<msrFullMeasureRestsContents> browser (v);
      browser.browse (*fFullMeasureRestsContents);
//    }
  }
}

string msrFullMeasureRests::asString () const
{
  stringstream s;

  s <<
    "FullMeasureRests" <<
    ", fullMeasureRestsLastMeasurePuristNumber: '" <<
    fFullMeasureRestsLastMeasurePuristNumber <<
    "'" <<
    ", fullMeasureRestsMeasureSoundingNotes: " <<
    fFullMeasureRestsMeasureSoundingNotes <<
    ", " <<
    mfSingularOrPlural (
      fFullMeasureRestsNumber,
        "rest measure",
        "full measure rests") <<
    ", fullMeasureRestsNextMeasureNumber: '" <<
    fFullMeasureRestsNextMeasureNumber <<
    "'" <<
    ", line " << fInputLineNumber;

  return s.str ();
}

void msrFullMeasureRests::displayFullMeasureRests (
  int           inputLineNumber,
  const string& context)
{
  gLogStream <<
    endl <<
    "*********>> Full measure rests " << context << " \"" <<
 // JMI   getVoiceName () <<
    "\"" <<
    ", line " << inputLineNumber <<
    " contains:" <<
    endl;

  ++gIndenter;
  print (gLogStream);
  --gIndenter;

  gLogStream <<
    " <<*********" <<
    endl << endl;
}

void msrFullMeasureRests::print (ostream& os) const
{
  os <<
    "FullMeasureRests" <<
    ", line " << fInputLineNumber <<
    endl;

  ++gIndenter;

  const int fieldWidth = 36;

  os << left <<
    setw (fieldWidth) <<
    "fullMeasureRestsLastMeasurePuristNumber" << " : " <<
    fFullMeasureRestsLastMeasurePuristNumber <<
    endl <<
    setw (fieldWidth) <<
    "fullMeasureRestsMeasureSoundingNotes" << " : " <<
    fFullMeasureRestsMeasureSoundingNotes <<
    endl <<
    setw (fieldWidth) <<
    "fullMeasureRestsNumber" << " : " <<
    fFullMeasureRestsNumber <<
    endl <<
    setw (fieldWidth) <<
    "fullMeasureRestsNextMeasureNumber" << " : '" <<
    fFullMeasureRestsNextMeasureNumber <<
    "'" <<
    endl;

  // print the voice upLink
  os << left <<
    setw (fieldWidth) <<
    "fullMeasureRestsVoiceUpLink" << " : " <<
    "\"" <<
    fFullMeasureRestsVoiceUpLink->getVoiceName () <<
    "\"" <<
    endl;

  // print the rests contents
  if (! fFullMeasureRestsContents) {
    os << left <<
      setw (fieldWidth) <<
      "fullMeasureRestsContents" << " : " << "none" <<
      endl;
  }

  else {
    os <<
      fFullMeasureRestsContents;
  }

  --gIndenter;
}

ostream& operator<< (ostream& os, const S_msrFullMeasureRests& elt)
{
  elt->print (os);
  return os;
}


}
