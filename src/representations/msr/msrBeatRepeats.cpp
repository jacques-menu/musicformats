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

#include "enableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "tracingOah.h"
#endif

#include "mfAssert.h"
#include "mfStringsHandling.h"

#include "msrWae.h"

#include "msrBeatRepeats.h"

#include "oahOah.h"

#include "msrOah.h"

#include "msrBrowsers.h"


using namespace std;

namespace MusicFormats
{

/* JMI
//______________________________________________________________________________
S_msrBeatRepeatElement msrBeatRepeatElement::create (
  int                 inputLineNumber,
  S_msrBeatRepeat beatRepeatUpLink)
{
  msrBeatRepeatElement* o =
    new msrBeatRepeatElement (
      inputLineNumber,
      beatRepeatUpLink);
  assert (o != nullptr);
  return o;
}

msrBeatRepeatElement::msrBeatRepeatElement (
  int                 inputLineNumber,
  S_msrBeatRepeat beatRepeatUpLink)
    : msrElement (inputLineNumber)
{
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    beatRepeatUpLink != nullptr,
    "beatRepeatUpLink is null");

  fBeatRepeatElementBeatRepeatUpLink = beatRepeatUpLink;
}

msrBeatRepeatElement::~msrBeatRepeatElement ()
{}

void msrBeatRepeatElement::appendSegmentToBeatRepeatElementsList ( // JMI ???
  int           inputLineNumber,
  S_msrSegment  segment,
  const string& context)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceVoices ()) {
    gLogStream <<
      "Appending segment '" << segment <<
      "' to beatRepeat element elements list '" << asString () <<
      "' (" << context << ")" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    segment != nullptr,
    "segment is null");

  fBeatRepeatElementsList.push_back (segment);
}

void msrBeatRepeatElement::appendBeatRepeatToBeatRepeatElementsList (
  int             inputLineNumber,
  S_msrBeatRepeat beatRepeat,
  const string&   context)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceVoices ()) {
    gLogStream <<
      "Appending beatRepeat '" << beatRepeat <<
      "' to beatRepeat element elements list '" << asString () <<
      "' (" << context << ")" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    beatRepeat != nullptr,
    "beatRepeat is null");

  fBeatRepeatElementsList.push_back (beatRepeat);
}

void msrBeatRepeatElement::appendVoiceElementToBeatRepeatElementsList (
  int               inputLineNumber,
  S_msrVoiceElement voiceElement,
  const string&     context)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceVoices ()) {
    gLogStream <<
      "Appending voice element '" << voiceElement <<
      "' to beatRepeat element elements list '" << asString () <<
      "' (" << context << ")" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    voiceElement != nullptr,
    "voiceElement is null");

  fBeatRepeatElementsList.push_back (voiceElement);
}

S_msrNote msrBeatRepeatElement::fetchBeatRepeatElementFirstNonGraceNote () const
{

  S_msrNote result;

  // fetch the first note in the first measure to which
  // a grace notes group can be attached
  // i.e. one not in a grace notes group itself,
  // possibly inside a chord or tuplet

  if (fBeatRepeatElementsList.size ()) {
    list<S_msrVoiceElement>::const_iterator
      iBegin = fBeatRepeatElementsList.begin (),
      iEnd   = fBeatRepeatElementsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      S_msrVoiceElement element = (*i);

      if (
        S_msrNote note = dynamic_cast<msrNote*>(&(*element))
        ) {
        result = note;
        break;
      }

      else if (
        S_msrChord chord = dynamic_cast<msrChord*>(&(*element))
        ) {
        // get the chord's first note
        result = chord->fetchChordFirstNonGraceNote ();
        break;
      }

      else if (
        S_msrTuplet tuplet = dynamic_cast<msrTuplet*>(&(*element))
        ) {
        // get the tuplet's first note
        result = tuplet->fetchTupletFirstNonGraceNote ();
        break;
      }

      else if (
        S_msrClef clef = dynamic_cast<msrClef*>(&(*element))
        ) {
        // ignore this clef
      }

      else if (
        S_msrKey key = dynamic_cast<msrKey*>(&(*element))
        ) {
        // ignore this key
      }

      else if (
        S_msrTimeSignature timeSignature = dynamic_cast<msrTimeSignature*>(&(*element))
        ) {
        // ignore this time
      }

      else {
        stringstream s;

        s <<
          "tuplet first element should be a note, a chord or another tuplet, found instead '" <<
          element->asShortString () <<
          "'";

        msrInternalError (
          gGlobalServiceRunData->getInputSourceName (),
          fInputLineNumber,
          __FILE__, __LINE__,
          s.str ());
      }

      if (++i == iEnd) break;
    } // for
  }

  return result;
}

void msrBeatRepeatElement::acceptIn (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrBeatRepeatElement::acceptIn ()" <<
      endl;
  }

  if (visitor<S_msrBeatRepeatElement>*
    p =
      dynamic_cast<visitor<S_msrBeatRepeatElement>*> (v)) {
        S_msrBeatRepeatElement elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrBeatRepeatElement::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrBeatRepeatElement::acceptOut (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrBeatRepeatElement::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrBeatRepeatElement>*
    p =
      dynamic_cast<visitor<S_msrBeatRepeatElement>*> (v)) {
        S_msrBeatRepeatElement elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrBeatRepeatElement::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrBeatRepeatElement::browseData (basevisitor* v)
{
  // browse the elements
  if (fBeatRepeatElementsList.size ()) {
    for (
      list<S_msrVoiceElement>::const_iterator i = fBeatRepeatElementsList.begin ();
      i != fBeatRepeatElementsList.end ();
      ++i
  ) {
      // browse the element
      msrBrowser<msrVoiceElement> browser (v);
      browser.browse (*(*i));
    } // for
  }
}

string msrBeatRepeatElement::asString () const
{
  stringstream s;

  s <<
    "BeatRepeatElement" <<
    ", beatRepeat upLink: '" <<
    fBeatRepeatElementBeatRepeatUpLink->
      asShortString () <<
    "', line " << fInputLineNumber <<
    endl;

  return s.str ();
}

void msrBeatRepeatElement::print (ostream& os) const
{
  os <<
    "BeatRepeatElement" <<
    ", line " << fInputLineNumber <<
    endl;

  ++gIndenter;

  os <<
    "beatRepeat upLink: '" <<
    fBeatRepeatElementBeatRepeatUpLink->
      asShortString () <<
      "'" <<
    endl << endl;

  // print the elements
  int elementsNumber =
    fBeatRepeatElementsList.size ();

  os <<
    "beatRepeatElementsList: ";
  if (elementsNumber) {
    os <<
      '(' <<
      mfSingularOrPlural (
        elementsNumber, "element", "elements") <<
      ")";
  }
  else {
    os << "empty";
  }
  os << endl;

  if (elementsNumber) {
    os << endl;

    ++gIndenter;

    list<S_msrVoiceElement>::const_iterator
      iBegin = fBeatRepeatElementsList.begin (),
      iEnd   = fBeatRepeatElementsList.end (),
      i      = iBegin;

    for ( ; ; ) {
      // print the element
      os << (*i);
      if (++i == iEnd) break;
      os << endl;
    } // for

    --gIndenter;
  }

  --gIndenter;
}

void msrBeatRepeatElement::printShort (ostream& os)
{
  os <<
    "BeatRepeatElement" <<
    ", line " << fInputLineNumber <<
    endl;

  ++gIndenter;

/ / * JMI
  os <<
    "beatRepeat upLink: '" <<
    fBeatRepeatElementBeatRepeatUpLink->
      asShortString () <<
      "'" <<
    endl;
* /

  // print the elements
  int elementsNumber =
    fBeatRepeatElementsList.size ();

  os <<
    "repeatElementsList: ";
  if (elementsNumber) {
    os <<
      '(' <<
      mfSingularOrPlural (
        elementsNumber, "element", "elements") <<
      ")";
  }
  else {
    os << "empty";
  }
  os << endl;

  if (elementsNumber) {
    os << endl;

    ++gIndenter;

    list<S_msrVoiceElement>::const_iterator
      iBegin = fBeatRepeatElementsList.begin (),
      iEnd   = fBeatRepeatElementsList.end (),
      i      = iBegin;

    for ( ; ; ) {
      // print the element
      (*i)->printShort (os);
      if (++i == iEnd) break;
      os << endl;
    } // for

    --gIndenter;
  }

  --gIndenter;
}

ostream& operator<< (ostream& os, const S_msrBeatRepeatElement& elt)
{
  elt->print (os);
  return os;
}
*/

//______________________________________________________________________________
S_msrBeatRepeatPattern msrBeatRepeatPattern::create (
  int                 inputLineNumber,
  S_msrBeatRepeat beatRepeatUpLink)
{
  msrBeatRepeatPattern* o =
    new msrBeatRepeatPattern (
      inputLineNumber,
      beatRepeatUpLink);
  assert (o != nullptr);
  return o;
}

msrBeatRepeatPattern::msrBeatRepeatPattern (
  int                 inputLineNumber,
  S_msrBeatRepeat beatRepeatUpLink)
    : msrElement (inputLineNumber)
{
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    beatRepeatUpLink != nullptr,
    "beatRepeatUpLink is null");

  fBeatRepeatUpLink = beatRepeatUpLink;
}

msrBeatRepeatPattern::~msrBeatRepeatPattern ()
{}

void msrBeatRepeatPattern::setBeatRepeatPatternSegment (
    S_msrSegment beatRepeatPatternSegment)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceBeatRepeats ()) {
    gLogStream <<
      "Setting measures repeat pattern segment containing " <<
      mfSingularOrPlural (
        fetchMeasuresNumber (),
        "measure",
        "measures") <<
      endl;
  }
#endif

  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    beatRepeatPatternSegment != nullptr,
    "beatRepeatPatternSegment is null");

  fBeatRepeatPatternSegment =
    beatRepeatPatternSegment;
}

int msrBeatRepeatPattern::fetchMeasuresNumber () const
{
  int result;

  if (fBeatRepeatPatternSegment) {
    result =
      fBeatRepeatPatternSegment->
        getSegmentElementsList ().size ();
  }
  else {
    result = 0;
  }

  return result;
}

void msrBeatRepeatPattern::acceptIn (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrBeatRepeatPattern::acceptIn ()" <<
      endl;
  }

  if (visitor<S_msrBeatRepeatPattern>*
    p =
      dynamic_cast<visitor<S_msrBeatRepeatPattern>*> (v)) {
        S_msrBeatRepeatPattern elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrBeatRepeatPattern::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrBeatRepeatPattern::acceptOut (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrBeatRepeatPattern::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrBeatRepeatPattern>*
    p =
      dynamic_cast<visitor<S_msrBeatRepeatPattern>*> (v)) {
        S_msrBeatRepeatPattern elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrBeatRepeatPattern::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrBeatRepeatPattern::browseData (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrBeatRepeatPattern::browseData ()" <<
      endl;
  }

  if (fBeatRepeatPatternSegment) {
    // browse the pattern segment
    msrBrowser<msrSegment> browser (v);
    browser.browse (*fBeatRepeatPatternSegment);
  }
}

string msrBeatRepeatPattern::asString () const
{
  stringstream s;

  s <<

    "BeatRepeatPattern" <<
    ", line " << fInputLineNumber <<
    " (" <<
    mfSingularOrPlural (
      fetchMeasuresNumber (),
      "repeated measure",
      "repeated measures") <<
    ")";

  return s.str ();
}

void msrBeatRepeatPattern::print (ostream& os) const
{
  os <<
    endl <<
    '[' <<
    asString () <<
    endl << endl;

  ++gIndenter;

  // print the pattern segment
  os <<
    "Pattern segment:";

  if (! fBeatRepeatPatternSegment) {
    os <<
      " none" <<
      endl;
  }
  else {
    os << endl;

    ++gIndenter;

    os <<
      fBeatRepeatPatternSegment;

    --gIndenter;
  }

  --gIndenter;

  os << ']' << endl;
}

ostream& operator<< (ostream& os, const S_msrBeatRepeatPattern& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_msrBeatRepeatReplicas msrBeatRepeatReplicas::create (
  int                 inputLineNumber,
  S_msrBeatRepeat beatRepeatUpLink)
{
  msrBeatRepeatReplicas* o =
    new msrBeatRepeatReplicas (
      inputLineNumber,
      beatRepeatUpLink);
  assert (o != nullptr);
  return o;
}

msrBeatRepeatReplicas::msrBeatRepeatReplicas (
  int                 inputLineNumber,
  S_msrBeatRepeat beatRepeatUpLink)
    : msrElement (inputLineNumber)
{
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    beatRepeatUpLink != nullptr,
    "beatRepeatUpLink is null");

  fBeatRepeatUpLink = beatRepeatUpLink;
}

msrBeatRepeatReplicas::~msrBeatRepeatReplicas ()
{}

void msrBeatRepeatReplicas::setBeatRepeatReplicasSegment (
  S_msrSegment beatRepeatReplicasSegment)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceBeatRepeats ()) {
    gLogStream <<
      "Setting measures repeat replicas segment containing " <<
      mfSingularOrPlural (
        fetchMeasuresNumber (),
        "measure",
        "measures") <<
      endl;
  }
#endif

  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    beatRepeatReplicasSegment != nullptr,
    "beatRepeatReplicasSegment is null");

  fBeatRepeatReplicasSegment =
    beatRepeatReplicasSegment;
}

int msrBeatRepeatReplicas::fetchMeasuresNumber () const
{
  int result;

  if (fBeatRepeatReplicasSegment) {
    result =
      fBeatRepeatReplicasSegment->
        getSegmentElementsList ().size ();
  }
  else {
    result = 0;
  }

  return result;
}

void msrBeatRepeatReplicas::acceptIn (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrBeatRepeatReplicas::acceptIn ()" <<
      endl;
  }

  if (visitor<S_msrBeatRepeatReplicas>*
    p =
      dynamic_cast<visitor<S_msrBeatRepeatReplicas>*> (v)) {
        S_msrBeatRepeatReplicas elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrBeatRepeatReplicas::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrBeatRepeatReplicas::acceptOut (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrBeatRepeatReplicas::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrBeatRepeatReplicas>*
    p =
      dynamic_cast<visitor<S_msrBeatRepeatReplicas>*> (v)) {
        S_msrBeatRepeatReplicas elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrBeatRepeatReplicas::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrBeatRepeatReplicas::browseData (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrBeatRepeatReplicas::browseData ()" <<
      endl;
  }

  if (fBeatRepeatReplicasSegment) {
  // browse the replicas segment
    msrBrowser<msrSegment> browser (v);
    browser.browse (*fBeatRepeatReplicasSegment);
  }
}

string msrBeatRepeatReplicas::asString () const
{
  stringstream s;

  s <<

    "BeatRepeatReplicas" <<
    ", line " << fInputLineNumber <<
    " (" <<
    mfSingularOrPlural (
      fetchMeasuresNumber (),
      "replicas measure",
      "replicas measures") <<
    ")";

  return s.str ();
}

void msrBeatRepeatReplicas::print (ostream& os) const
{
  os <<
    endl <<
    '[' <<
    asString () <<
    endl << endl;

  ++gIndenter;

  // print the replicas segment
  os <<
    "Replicas segment:";

  if (! fBeatRepeatReplicasSegment) {
    os <<
      " none" <<
      endl;
  }
  else {
    os << endl;

    ++gIndenter;

    os <<
      fBeatRepeatReplicasSegment;

    --gIndenter;
  }

  --gIndenter;

  os << ']' << endl;
}

ostream& operator<< (ostream& os, const S_msrBeatRepeatReplicas& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_msrBeatRepeat msrBeatRepeat::create (
  int        inputLineNumber,
  int        beatRepeatMeasuresNumber,
  int        beatRepeatSlashesNumber,
  S_msrVoice voiceUpLink)
{
  msrBeatRepeat* o =
    new msrBeatRepeat (
      inputLineNumber,
      beatRepeatMeasuresNumber,
      beatRepeatSlashesNumber,
      voiceUpLink);
  assert (o != nullptr);
  return o;
}

msrBeatRepeat::msrBeatRepeat (
  int        inputLineNumber,
  int        beatRepeatMeasuresNumber,
  int        beatRepeatSlashesNumber,
  S_msrVoice voiceUpLink)
    : msrSegmentElement (inputLineNumber)
{
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    beatRepeatMeasuresNumber > 0,
    "beatRepeatMeasuresNumber is not positive");

  fBeatRepeatMeasuresNumber = beatRepeatMeasuresNumber;

  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    beatRepeatSlashesNumber > 0,
    "beatRepeatSlashesNumber is not positive");

  fBeatRepeatSlashesNumber  = beatRepeatSlashesNumber;

  fBeatRepeatVoiceUpLink = voiceUpLink;

  // measures repeat build phase
  fCurrentBeatRepeatBuildPhaseKind =
    msrBeatRepeat::kBeatRepeatBuildPhaseJustCreated;
}

msrBeatRepeat::~msrBeatRepeat ()
{}

S_msrBeatRepeat msrBeatRepeat::createBeatRepeatNewbornClone (
  S_msrVoice containingVoice)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceBeatRepeats ()) {
    gLogStream <<
      "Creating a newborn clone of measures repeat '" <<
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

  S_msrBeatRepeat
    newbornClone =
      msrBeatRepeat::create (
        fInputLineNumber,
        fBeatRepeatMeasuresNumber,
        fBeatRepeatSlashesNumber,
        containingVoice);

  return newbornClone;
}

void msrBeatRepeat::setBeatRepeatPattern (
  S_msrBeatRepeatPattern beatRepeatPattern)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceBeatRepeats ()) {
    gLogStream <<
      "Setting measures repeat pattern containing " <<
      mfSingularOrPlural (
        beatRepeatPattern->
          fetchMeasuresNumber (),
        "measure",
        "measures") <<
      endl;
  }
#endif

  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    beatRepeatPattern != nullptr,
    "beatRepeatPattern is null");

  fBeatRepeatPattern = beatRepeatPattern;

  // set currentRepeat's build phase
  fCurrentBeatRepeatBuildPhaseKind =
    msrBeatRepeat::kBeatRepeatBuildPhaseInPattern;
}

void msrBeatRepeat::setBeatRepeatReplicas (
  S_msrBeatRepeatReplicas beatRepeatReplicas)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceBeatRepeats ()) {
    gLogStream <<
      "Setting measures repeat replicas containing " <<
      mfSingularOrPlural (
        beatRepeatReplicas->
          fetchMeasuresNumber (),
        "measure",
        "measures") <<
      endl;
  }
#endif

  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    beatRepeatReplicas != nullptr,
    "beatRepeatReplicas is null");

  fBeatRepeatReplicas = beatRepeatReplicas;

  // set currentRepeat's build phase
  fCurrentBeatRepeatBuildPhaseKind =
    msrBeatRepeat::kBeatRepeatBuildPhaseInReplicas;
}

int msrBeatRepeat::fetchBeatRepeatReplicasNumber () const
{
  int patternMeasuresNumber =
    fetchBeatRepeatPatternMeasuresNumber ();

  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    patternMeasuresNumber > 0,
    "patternMeasuresNumber is not positive");

  return
    fetchBeatRepeatReplicasMeasuresNumber ()
      / // JMI ???
    patternMeasuresNumber;
}

int msrBeatRepeat::fetchBeatRepeatPatternMeasuresNumber () const
{
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    fBeatRepeatPattern != nullptr,
    "fBeatRepeatPattern is null");

  return
    fBeatRepeatPattern->
      fetchMeasuresNumber ();
}

int msrBeatRepeat::fetchBeatRepeatReplicasMeasuresNumber () const
{
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    fBeatRepeatReplicas != nullptr,
    "fBeatRepeatReplicas is null");

  return
    fBeatRepeatReplicas->
      fetchMeasuresNumber ();
}

void msrBeatRepeat::acceptIn (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrBeatRepeat::acceptIn ()" <<
      endl;
  }

  if (visitor<S_msrBeatRepeat>*
    p =
      dynamic_cast<visitor<S_msrBeatRepeat>*> (v)) {
        S_msrBeatRepeat elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrBeatRepeat::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrBeatRepeat::acceptOut (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrBeatRepeat::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrBeatRepeat>*
    p =
      dynamic_cast<visitor<S_msrBeatRepeat>*> (v)) {
        S_msrBeatRepeat elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrBeatRepeat::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrBeatRepeat::browseData (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrBeatRepeat::browseData ()" <<
      endl;
  }

  if (fBeatRepeatPattern) {
  // browse the measdure repeat pattern
    msrBrowser<msrBeatRepeatPattern> browser (v);
    browser.browse (*fBeatRepeatPattern);
  }

  // fetch the score
  S_msrScore
    score =
      fBeatRepeatVoiceUpLink->
        fetchVoiceScoreUpLink ();

/* JMI
  Bool inhibitBeatRepeatReplicasBrowsing =
    score->getInhibitBeatRepeatReplicasBrowsing ();

  if (inhibitBeatRepeatReplicasBrowsing) {
#ifdef TRACING_IS_ENABLED
    if (gGlobalMsrOahGroup->getTraceMsrVisitors () || gGlobalTracingOahGroup->getTraceBeatRepeats ()) {
      gLogStream <<
        "% ==> visiting measures repeat replicas is inhibited" <<
        endl;
    }
#endif
  }

  if (fBeatRepeatReplicas) {
    if (! inhibitBeatRepeatReplicasBrowsing) {
      // browse the measures repeat replicas
      msrBrowser<msrBeatRepeatReplicas> browser (v);
      browser.browse (*fBeatRepeatReplicas);
    }
  }
  */
}

void msrBeatRepeat:: appendMeasureElementToSegmentElement (
  S_msrMeasureElement elem)
{
  stringstream s;

  s <<
    "cannot append measure element " <<
    elem->asShortString () <<
    " to beat repeat " <<
    asShortString ();

  msrInternalError (
    gGlobalServiceRunData->getInputSourceName (),
    fInputLineNumber,
    __FILE__, __LINE__,
    s.str ());
}

string msrBeatRepeat::beatRepeatBuildPhaseKindAsString (
  msrBeatRepeatBuildPhaseKind beatRepeatBuildPhaseKind)
{
  string result;

  switch (beatRepeatBuildPhaseKind) {
    case msrBeatRepeat::kBeatRepeatBuildPhaseJustCreated:
      result = "beatRepeatBuildPhaseJustCreated";
      break;
    case msrBeatRepeat::kBeatRepeatBuildPhaseInPattern:
      result = "beatRepeatBuildPhaseInPattern";
      break;
    case msrBeatRepeat::kBeatRepeatBuildPhaseInReplicas:
      result = "beatRepeatBuildPhaseInReplicas";
      break;
    case msrBeatRepeat::kBeatRepeatBuildPhaseCompleted:
      result = "beatRepeatBuildPhaseCompleted";
      break;
  } // switch

  return result;
}

string msrBeatRepeat::asString () const
{
  stringstream s;

  s <<

    "[BeatRepeat" <<
    ", line " << fInputLineNumber <<
    " (" <<
    mfSingularOrPlural (
      fetchBeatRepeatPatternMeasuresNumber (),
      "repeated measure",
      "repeated measures") <<
    ", " <<
    mfSingularOrPlural (
      fetchBeatRepeatReplicasMeasuresNumber (),
      "replicas measure",
      "replicas measures") <<
    ", " <<
    fetchBeatRepeatReplicasNumber () << " replicas" <<
    ")" <<
    "]";

  return s.str ();
}

void msrBeatRepeat::displayBeatRepeat (
  int           inputLineNumber,
  const string& context)
{
  gLogStream <<
    endl <<
    "*********>> BeatRepeat " <<
    ", beatRepeatMeasuresNumber: '" <<
    fBeatRepeatMeasuresNumber <<
    ", beatRepeatSlashesNumber: '" <<
    fBeatRepeatSlashesNumber <<
    "', voice:" <<
    endl <<
    fBeatRepeatVoiceUpLink->getVoiceName () <<
    " (" << context << ")" <<
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

void msrBeatRepeat::print (ostream& os) const
{
  os <<
    "[BeatRepeat" <<
    " (" <<
    mfSingularOrPlural (
      fBeatRepeatPattern
        ? fetchBeatRepeatPatternMeasuresNumber ()
        : 0,
      "pattern measure",
      "pattern measures") <<
    ", " <<
    mfSingularOrPlural (
      fBeatRepeatReplicas
        ? fetchBeatRepeatReplicasMeasuresNumber ()
        : 0,
      "replica measure",
      "replicas measures") <<
    ", line " << fInputLineNumber <<
    endl;

  ++gIndenter;

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceBeatRepeats ()) {
    // print the current measures repeat build phase
    const int fieldWidth = 36;

    os <<
      setw (fieldWidth) <<
      "fCurrentBeatRepeatBuildPhaseKind" << " : " <<
      beatRepeatBuildPhaseKindAsString (
        fCurrentBeatRepeatBuildPhaseKind) <<
      endl;
  }
#endif

  // print the measures repeat pattern
  if (! fBeatRepeatPattern) {
    os <<
      "measures repeat pattern: none" <<
      endl;
  }

  else {
    os <<
      fBeatRepeatPattern;
  }

  // print the measures repeat replicas
  if (! fBeatRepeatReplicas) {
    os <<
      "measures repeat replicas: none" <<
      endl;
  }

  else {
    os <<
      fBeatRepeatReplicas;
  }

  --gIndenter;

  os << ']' << endl;
}

void msrBeatRepeat::printShort (ostream& os) const
{
  print (os);
}

ostream& operator<< (ostream& os, const S_msrBeatRepeat& elt)
{
  elt->print (os);
  return os;
}


}
