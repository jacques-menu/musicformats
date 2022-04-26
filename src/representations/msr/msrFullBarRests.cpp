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

#include "msrFullBarRests.h"

#include "oahOah.h"

#include "msrOah.h"

#include "msrBrowsers.h"


using namespace std;

namespace MusicFormats
{

//______________________________________________________________________________
S_msrFullBarRestsContents msrFullBarRestsContents::create (
  int                   inputLineNumber,
  S_msrFullBarRests fullBarRests)
{
  msrFullBarRestsContents* o =
    new msrFullBarRestsContents (
      inputLineNumber,
      fullBarRests);
  assert (o != nullptr);
  return o;
}

msrFullBarRestsContents::msrFullBarRestsContents (
  int                  inputLineNumber,
  S_msrFullBarRests fullBarRests)
    : msrElement (inputLineNumber)
{
  fFullBarRestsContentsFullBarRestsUpLink =
    fullBarRests;
}

msrFullBarRestsContents::~msrFullBarRestsContents ()
{}

S_msrFullBarRestsContents msrFullBarRestsContents::createFullBarRestsContentsNewbornClone (
  S_msrFullBarRests fullBarRests)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceFullBarRests ()) {
    gLogStream <<
      "Creating a newborn clone of a full-bar rests contents" <<
      endl;
  }
#endif

  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    fullBarRests != nullptr,
    "fullBarRests is null");

  S_msrFullBarRestsContents
    newbornClone =
      msrFullBarRestsContents::create (
        fInputLineNumber,
        fullBarRests);

  return newbornClone;
}

void msrFullBarRestsContents::setFullBarRestsContentsSegment (
  int          inputLineNumber,
  S_msrSegment fullBarRestsContentsSegment)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceFullBarRests ()) {
    gLogStream <<
      "Setting full-bar rests contents segment containing " <<
      mfSingularOrPlural (
        fullBarRestsContentsMeasuresNumber (),
        "measure",
        "measures") <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    fullBarRestsContentsSegment != nullptr,
    "fullBarRestsContentsSegment is null");

  fFullBarRestsContentsSegment =
    fullBarRestsContentsSegment;
}

int msrFullBarRestsContents::fullBarRestsContentsMeasuresNumber () const
{
  int result;

  if (fFullBarRestsContentsSegment) {
    result =
      fFullBarRestsContentsSegment->
      getSegmentMeasuresList ().size ();
  }
  else {
    result = 0;
  }

  return result;
}

void msrFullBarRestsContents::acceptIn (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrFullBarRestsContents::acceptIn ()" <<
      endl;
  }

  if (visitor<S_msrFullBarRestsContents>*
    p =
      dynamic_cast<visitor<S_msrFullBarRestsContents>*> (v)) {
        S_msrFullBarRestsContents elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrFullBarRestsContents::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrFullBarRestsContents::acceptOut (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrFullBarRestsContents::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrFullBarRestsContents>*
    p =
      dynamic_cast<visitor<S_msrFullBarRestsContents>*> (v)) {
        S_msrFullBarRestsContents elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrFullBarRestsContents::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrFullBarRestsContents::browseData (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrFullBarRestsContents::browseData ()" <<
      endl;
  }

  if (fFullBarRestsContentsSegment) {
    // browse the pattern segment
    msrBrowser<msrSegment> browser (v);
    browser.browse (*fFullBarRestsContentsSegment);
  }
}

string msrFullBarRestsContents::asString () const
{
  stringstream s;

  s <<

    "FullBarRestsContents" <<
    ", line " << fInputLineNumber <<
    " (" <<
    mfSingularOrPlural (
      fullBarRestsContentsMeasuresNumber (),
      "contents measure",
      "contents measures") <<
    ")";

  return s.str ();
}

void msrFullBarRestsContents::print (ostream& os) const
{
  os <<
    endl <<
    '[' <<
    asString () <<
    endl;

  ++gIndenter;

  // print the pattern segment
  os <<
    "Contents segment:";

  if (! fFullBarRestsContentsSegment) {
    os <<
      " none" <<
      endl;
  }
  else {
    os << endl;

    ++gIndenter;

    os <<
      fFullBarRestsContentsSegment;

    --gIndenter;
  }

  --gIndenter;

  os << ']' << endl;
}

ostream& operator<< (ostream& os, const S_msrFullBarRestsContents& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_msrFullBarRests msrFullBarRests::create (
  int              inputLineNumber,
  const rational&  fullBarRestsMeasureSoundingNotes,
  int              fullBarRestsNumber,
  S_msrVoice       voiceUpLink)
{
  msrFullBarRests* o =
    new msrFullBarRests (
      inputLineNumber,
      fullBarRestsMeasureSoundingNotes,
      fullBarRestsNumber,
      voiceUpLink);
  assert (o != nullptr);
  return o;
}

S_msrFullBarRests msrFullBarRests::create (
  int          inputLineNumber,
  S_msrMeasure restMeasureClone,
  S_msrVoice   voiceUpLink)
{
  msrFullBarRests* o =
    new msrFullBarRests (
      inputLineNumber,
      restMeasureClone,
      voiceUpLink);
  assert (o != nullptr);
  return o;
}

msrFullBarRests::msrFullBarRests (
  int             inputLineNumber,
  const rational& fullBarRestsMeasureSoundingNotes,
  int             fullBarRestsNumber,
  S_msrVoice      voiceUpLink)
    : msrVoiceElement (inputLineNumber)
{
  fFullBarRestsVoiceUpLink = voiceUpLink;

  fFullBarRestsMeasureSoundingNotes = fullBarRestsMeasureSoundingNotes;

  fFullBarRestsNumber = fullBarRestsNumber;

  fFullBarRestsLastMeasurePuristNumber = -1;
}

msrFullBarRests::msrFullBarRests (
  int          inputLineNumber,
  S_msrMeasure restMeasureClone,
  S_msrVoice   voiceUpLink)
    : msrVoiceElement (inputLineNumber)
{
  fFullBarRestsVoiceUpLink = voiceUpLink;

  fFullBarRestsMeasureSoundingNotes =
    restMeasureClone->
      getFullMeasureWholeNotesDuration (); // JMI ???

  fFullBarRestsNumber = 1; // will evolve JMI

  fFullBarRestsLastMeasurePuristNumber = -1;

  // create the full-bar rests contents
  fFullBarRestsContents =
    msrFullBarRestsContents::create (
      inputLineNumber,
      this);
}

msrFullBarRests::~msrFullBarRests ()
{}

S_msrFullBarRests msrFullBarRests::createFullBarRestsNewbornClone (
  S_msrVoice containingVoice)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceFullBarRests ()) {
    gLogStream <<
      "Creating a newborn clone of full-bar rests '" <<
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

  S_msrFullBarRests
    newbornClone =
      msrFullBarRests::create (
        fInputLineNumber,
        fFullBarRestsMeasureSoundingNotes,
        fFullBarRestsNumber,
        containingVoice);

/* JMI
  newbornClone->fFullBarRestsNextMeasureNumber =
    fFullBarRestsNextMeasureNumber;
    */

  return newbornClone;
}

void msrFullBarRests::setFullBarRestsContents (
  S_msrFullBarRestsContents fullBarRestsContents)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceFullBarRests ()) {
    gLogStream <<
      "Setting full-bar rests contents containing " <<
      mfSingularOrPlural (
        fullBarRestsContents->
          fullBarRestsContentsMeasuresNumber (),
        "measure",
        "measures") <<
      endl;
  }
#endif

  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    fullBarRestsContents != nullptr,
    "fullBarRestsContents is null");

  fFullBarRestsContents = fullBarRestsContents;
}

void msrFullBarRests::setFullBarRestsNextMeasureNumber (
  const string& nextMeasureNumber)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceFullBarRests ()) {
    gLogStream <<
      "Setting full-bar rests next measure number to '" <<
      "' " <<
      nextMeasureNumber <<
      endl;
  }
#endif

  fFullBarRestsNextMeasureNumber =
    nextMeasureNumber;
}

void msrFullBarRests::setFullBarRestsLastMeasurePuristMeasureNumber (
  int inputLineNumber)
{
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    fFullBarRestsContents != nullptr,
    "fFullBarRestsContents is null");

  S_msrSegment
    fullBarRestsContentsSegment =
      fFullBarRestsContents->
        getFullBarRestsContentsSegment ();

  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    fullBarRestsContentsSegment != nullptr,
    "fullBarRestsContentsSegment is null");

  // get full-bar rests contents segment measures list
  const list<S_msrMeasure>&
    contentsSegmentMeasuresList =
      fullBarRestsContentsSegment->
        getSegmentMeasuresList ();

  // get full-bar rests contents last measure's purist number
  int lastMeasuresPuristNumber = -1;

  if (contentsSegmentMeasuresList.size ()) {
    lastMeasuresPuristNumber =
      contentsSegmentMeasuresList.back ()->
        getMeasurePuristNumber ();
  }
  else {
    stringstream s;

    s <<
      "cannot get full-bar rests contents last measure purist number" <<
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
  if (gGlobalTracingOahGroup->getTraceFullBarRests ()) {
    gLogStream <<
      "Setting full-bar rests last measure purist number to '" <<
      "' " <<
      lastMeasuresPuristNumber <<
      "', line " << inputLineNumber <<
      endl;
  }
#endif

  fFullBarRestsLastMeasurePuristNumber =
    lastMeasuresPuristNumber;
}

void msrFullBarRests::appendMeasureCloneToFullBarRests (
  S_msrMeasure measureClone)
{
  fFullBarRestsContents->
    getFullBarRestsContentsSegment ()->
      appendMeasureToSegment (
        measureClone);
}

void msrFullBarRests::acceptIn (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrFullBarRests::acceptIn ()" <<
      endl;
  }

  if (visitor<S_msrFullBarRests>*
    p =
      dynamic_cast<visitor<S_msrFullBarRests>*> (v)) {
        S_msrFullBarRests elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrFullBarRests::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrFullBarRests::acceptOut (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrFullBarRests::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrFullBarRests>*
    p =
      dynamic_cast<visitor<S_msrFullBarRests>*> (v)) {
        S_msrFullBarRests elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrFullBarRests::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrFullBarRests::browseData (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrFullBarRests::browseData ()" <<
      endl;
  }

/* JMI
  // get the part upLink
  S_msrPart
    partUpLink =
      fFullBarRestsVoiceUpLink->
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

  Bool inhibitFullBarRestsBrowsing =
    scoreUpLink->
      getInhibitFullBarRestsBrowsing ();

  if (inhibitFullBarRestsBrowsing) {
    if (gGlobalMsrOahGroup->getTraceMsrVisitors () || gGlobalTracingOahGroup->getTraceFullBarRests ()) {
      gLogStream <<
        "% ==> visiting full-bar rests is inhibited" <<
        endl;
    }
  }
*/

  if (fFullBarRestsContents) {
 // JMI   if (! inhibitFullBarRestsBrowsing) {
      // browse the full-bar rests contents
      msrBrowser<msrFullBarRestsContents> browser (v);
      browser.browse (*fFullBarRestsContents);
//    }
  }
}

string msrFullBarRests::asString () const
{
  stringstream s;

  s <<
    "FullBarRests" <<
    ", fullBarRestsLastMeasurePuristNumber: '" <<
    fFullBarRestsLastMeasurePuristNumber <<
    "'" <<
    ", fullBarRestsMeasureSoundingNotes: " <<
    fFullBarRestsMeasureSoundingNotes <<
    ", " <<
    mfSingularOrPlural (
      fFullBarRestsNumber,
        "rest measure",
        "full-bar rests") <<
    ", fullBarRestsNextMeasureNumber: '" <<
    fFullBarRestsNextMeasureNumber <<
    "'" <<
    ", line " << fInputLineNumber;

  return s.str ();
}

void msrFullBarRests::displayFullBarRests (
  int           inputLineNumber,
  const string& context)
{
  gLogStream <<
    endl <<
    "*********>> Full-bar rests " << context << " \"" <<
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

void msrFullBarRests::print (ostream& os) const
{
  os <<
    "FullBarRests" <<
    ", line " << fInputLineNumber <<
    endl;

  ++gIndenter;

  const int fieldWidth = 36;

  os << left <<
    setw (fieldWidth) <<
    "fullBarRestsLastMeasurePuristNumber" << " : " <<
    fFullBarRestsLastMeasurePuristNumber <<
    endl <<
    setw (fieldWidth) <<
    "fullBarRestsMeasureSoundingNotes" << " : " <<
    fFullBarRestsMeasureSoundingNotes <<
    endl <<
    setw (fieldWidth) <<
    "fullBarRestsNumber" << " : " <<
    fFullBarRestsNumber <<
    endl <<
    setw (fieldWidth) <<
    "fullBarRestsNextMeasureNumber" << " : '" <<
    fFullBarRestsNextMeasureNumber <<
    "'" <<
    endl;

  // print the voice upLink
  os << left <<
    setw (fieldWidth) <<
    "fullBarRestsVoiceUpLink" << " : " <<
    "\"" <<
    fFullBarRestsVoiceUpLink->getVoiceName () <<
    "\"" <<
    endl;

  // print the rests contents
  if (! fFullBarRestsContents) {
    os << left <<
      setw (fieldWidth) <<
      "fullBarRestsContents" << " : " << "none" <<
      endl;
  }

  else {
    os <<
      fFullBarRestsContents;
  }

  --gIndenter;
}

ostream& operator<< (ostream& os, const S_msrFullBarRests& elt)
{
  elt->print (os);
  return os;
}


}
