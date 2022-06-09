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

#include "msrMeasureRepeats.h"

#include "oahOah.h"

#include "msrOah.h"

#include "msrBrowsers.h"


using namespace std;

namespace MusicFormats
{

/* JMI
//______________________________________________________________________________
S_msrMeasureRepeatElement msrMeasureRepeatElement::create (
  int                 inputLineNumber,
  S_msrMeasureRepeat measureRepeatUpLink)
{
  msrMeasureRepeatElement* o =
    new msrMeasureRepeatElement (
      inputLineNumber,
      measureRepeatUpLink);
  assert (o != nullptr);
  return o;
}

msrMeasureRepeatElement::msrMeasureRepeatElement (
  int                 inputLineNumber,
  S_msrMeasureRepeat measureRepeatUpLink)
    : msrElement (inputLineNumber)
{
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    measureRepeatUpLink != nullptr,
    "measureRepeatUpLink is null");

  fMeasureRepeatElementMeasureRepeatUpLink = measureRepeatUpLink;
}

msrMeasureRepeatElement::~msrMeasureRepeatElement ()
{}

void msrMeasureRepeatElement::appendSegmentToMeasureRepeatElementsList ( // JMI ???
  int          inputLineNumber,
  S_msrSegment segment,
  string       context)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceVoices ()) {
    gLogStream <<
      "Appending segment '" << segment <<
      "' to measureRepeat element elements list '" << asString () <<
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

  fMeasureRepeatElementsList.push_back (segment);
}

void msrMeasureRepeatElement::appendMeasureRepeatToMeasureRepeatElementsList (
  int                  inputLineNumber,
  S_msrMeasureRepeat  measureRepeat,
  string               context)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceVoices ()) {
    gLogStream <<
      "Appending measureRepeat '" << measureRepeat <<
      "' to measureRepeat element elements list '" << asString () <<
      "' (" << context << ")" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    measureRepeat != nullptr,
    "measureRepeat is null");

  fMeasureRepeatElementsList.push_back (measureRepeat);
}

void msrMeasureRepeatElement::appendVoiceElementToMeasureRepeatElementsList (
  int               inputLineNumber,
  S_msrVoiceElement voiceElement,
  string            context)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceVoices ()) {
    gLogStream <<
      "Appending voice element '" << voiceElement <<
      "' to measureRepeat element elements list '" << asString () <<
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

  fMeasureRepeatElementsList.push_back (voiceElement);
}

S_msrNote msrMeasureRepeatElement::fetchMeasureRepeatElementFirstNonGraceNote () const
{

  S_msrNote result;

  // fetch the first note in the first measure to which
  // a grace notes group can be attached
  // i.e. one not in a grace notes group itself,
  // possibly inside a chord or tuplet

  if (fMeasureRepeatElementsList.size ()) {
    list<S_msrVoiceElement>::const_iterator
      iBegin = fMeasureRepeatElementsList.begin (),
      iEnd   = fMeasureRepeatElementsList.end (),
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

void msrMeasureRepeatElement::acceptIn (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrMeasureRepeatElement::acceptIn ()" <<
      endl;
  }

  if (visitor<S_msrMeasureRepeatElement>*
    p =
      dynamic_cast<visitor<S_msrMeasureRepeatElement>*> (v)) {
        S_msrMeasureRepeatElement elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrMeasureRepeatElement::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrMeasureRepeatElement::acceptOut (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrMeasureRepeatElement::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrMeasureRepeatElement>*
    p =
      dynamic_cast<visitor<S_msrMeasureRepeatElement>*> (v)) {
        S_msrMeasureRepeatElement elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrMeasureRepeatElement::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrMeasureRepeatElement::browseData (basevisitor* v)
{
  // browse the elements
  if (fMeasureRepeatElementsList.size ()) {
    for (
      list<S_msrVoiceElement>::const_iterator i = fMeasureRepeatElementsList.begin ();
      i != fMeasureRepeatElementsList.end ();
      ++i
  ) {
      // browse the element
      msrBrowser<msrVoiceElement> browser (v);
      browser.browse (*(*i));
    } // for
  }
}

string msrMeasureRepeatElement::asString () const
{
  stringstream s;

  s <<
    "MeasureRepeatElement" <<
    ", measureRepeat upLink: '" <<
    fMeasureRepeatElementMeasureRepeatUpLink->
      asShortString () <<
    "', line " << fInputLineNumber <<
    endl;

  return s.str ();
}

void msrMeasureRepeatElement::print (ostream& os) const
{
  os <<
    "MeasureRepeatElement" <<
    ", line " << fInputLineNumber <<
    endl;

  ++gIndenter;

  os <<
    "measureRepeat upLink: '" <<
    fMeasureRepeatElementMeasureRepeatUpLink->
      asShortString () <<
      "'" <<
    endl << endl;

  // print the elements
  int elementsNumber =
    fMeasureRepeatElementsList.size ();

  os <<
    "measureRepeatElementsList: ";
  if (elementsNumber) {
    os <<
      '(' <<
      mfSingularOrPlural (
        elementsNumber, "element", "elements") <<
      ")";
  }
  else {
    os << "none";
  }
  os << endl;

  if (elementsNumber) {
    os << endl;

    ++gIndenter;

    list<S_msrVoiceElement>::const_iterator
      iBegin = fMeasureRepeatElementsList.begin (),
      iEnd   = fMeasureRepeatElementsList.end (),
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

void msrMeasureRepeatElement::printShort (ostream& os) const
{
  os <<
    "MeasureRepeatElement" <<
    ", line " << fInputLineNumber <<
    endl;

  ++gIndenter;

/ / * JMI
  os <<
    "measureRepeat upLink: '" <<
    fMeasureRepeatElementMeasureRepeatUpLink->
      asShortString () <<
      "'" <<
    endl;
* /

  // print the elements
  int elementsNumber =
    fMeasureRepeatElementsList.size ();

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
    os << "none";
  }
  os << endl;

  if (elementsNumber) {
    os << endl;

    ++gIndenter;

    list<S_msrVoiceElement>::const_iterator
      iBegin = fMeasureRepeatElementsList.begin (),
      iEnd   = fMeasureRepeatElementsList.end (),
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

ostream& operator<< (ostream& os, const S_msrMeasureRepeatElement& elt)
{
  elt->print (os);
  return os;
}
*/

//______________________________________________________________________________
S_msrMeasureRepeatPattern msrMeasureRepeatPattern::create (
  int                 inputLineNumber,
  S_msrMeasureRepeat measureRepeatUpLink)
{
  msrMeasureRepeatPattern* o =
    new msrMeasureRepeatPattern (
      inputLineNumber,
      measureRepeatUpLink);
  assert (o != nullptr);
  return o;
}

msrMeasureRepeatPattern::msrMeasureRepeatPattern (
  int                 inputLineNumber,
  S_msrMeasureRepeat measureRepeatUpLink)
    : msrElement (inputLineNumber)
{
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    measureRepeatUpLink != nullptr,
    "measureRepeatUpLink is null");

  fMeasureRepeatUpLink = measureRepeatUpLink;
}

msrMeasureRepeatPattern::~msrMeasureRepeatPattern ()
{}

void msrMeasureRepeatPattern::setMeasureRepeatPatternSegment (
    S_msrSegment measureRepeatPatternSegment)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasureRepeats ()) {
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
    measureRepeatPatternSegment != nullptr,
    "measureRepeatPatternSegment is null");

  fMeasureRepeatPatternSegment =
    measureRepeatPatternSegment;
}

int msrMeasureRepeatPattern::fetchMeasuresNumber () const
{
  int result;

  if (fMeasureRepeatPatternSegment) {
    result =
      fMeasureRepeatPatternSegment->
        getSegmentElementsList ().size ();
  }
  else {
    result = 0;
  }

  return result;
}

void msrMeasureRepeatPattern::acceptIn (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrMeasureRepeatPattern::acceptIn ()" <<
      endl;
  }

  if (visitor<S_msrMeasureRepeatPattern>*
    p =
      dynamic_cast<visitor<S_msrMeasureRepeatPattern>*> (v)) {
        S_msrMeasureRepeatPattern elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrMeasureRepeatPattern::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrMeasureRepeatPattern::acceptOut (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrMeasureRepeatPattern::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrMeasureRepeatPattern>*
    p =
      dynamic_cast<visitor<S_msrMeasureRepeatPattern>*> (v)) {
        S_msrMeasureRepeatPattern elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrMeasureRepeatPattern::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrMeasureRepeatPattern::browseData (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrMeasureRepeatPattern::browseData ()" <<
      endl;
  }

  if (fMeasureRepeatPatternSegment) {
    // browse the pattern segment
    msrBrowser<msrSegment> browser (v);
    browser.browse (*fMeasureRepeatPatternSegment);
  }
}

string msrMeasureRepeatPattern::asString () const
{
  stringstream s;

  s <<

    "MeasureRepeatPattern" <<
    ", line " << fInputLineNumber <<
    " (" <<
    mfSingularOrPlural (
      fetchMeasuresNumber (),
      "repeated measure",
      "repeated measures") <<
    ")";

  return s.str ();
}

void msrMeasureRepeatPattern::print (ostream& os) const
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

  if (! fMeasureRepeatPatternSegment) {
    os <<
      " none" <<
      endl;
  }
  else {
    os << endl;

    ++gIndenter;

    os <<
      fMeasureRepeatPatternSegment;

    --gIndenter;
  }

  --gIndenter;

  os << ']' << endl;
}

ostream& operator<< (ostream& os, const S_msrMeasureRepeatPattern& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_msrMeasureRepeatReplicas msrMeasureRepeatReplicas::create (
  int                 inputLineNumber,
  S_msrMeasureRepeat measureRepeatUpLink)
{
  msrMeasureRepeatReplicas* o =
    new msrMeasureRepeatReplicas (
      inputLineNumber,
      measureRepeatUpLink);
  assert (o != nullptr);
  return o;
}

msrMeasureRepeatReplicas::msrMeasureRepeatReplicas (
  int                 inputLineNumber,
  S_msrMeasureRepeat measureRepeatUpLink)
    : msrElement (inputLineNumber)
{
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    measureRepeatUpLink != nullptr,
    "measureRepeatUpLink is null");

  fMeasureRepeatUpLink = measureRepeatUpLink;
}

msrMeasureRepeatReplicas::~msrMeasureRepeatReplicas ()
{}

void msrMeasureRepeatReplicas::setMeasureRepeatReplicasSegment (
  S_msrSegment measureRepeatReplicasSegment)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasureRepeats ()) {
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
    measureRepeatReplicasSegment != nullptr,
    "measureRepeatReplicasSegment is null");

  fMeasureRepeatReplicasSegment =
    measureRepeatReplicasSegment;
}

int msrMeasureRepeatReplicas::fetchMeasuresNumber () const
{
  int result;

  if (fMeasureRepeatReplicasSegment) {
    result =
      fMeasureRepeatReplicasSegment->
        getSegmentElementsList ().size ();
  }
  else {
    result = 0;
  }

  return result;
}

void msrMeasureRepeatReplicas::acceptIn (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrMeasureRepeatReplicas::acceptIn ()" <<
      endl;
  }

  if (visitor<S_msrMeasureRepeatReplicas>*
    p =
      dynamic_cast<visitor<S_msrMeasureRepeatReplicas>*> (v)) {
        S_msrMeasureRepeatReplicas elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrMeasureRepeatReplicas::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrMeasureRepeatReplicas::acceptOut (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrMeasureRepeatReplicas::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrMeasureRepeatReplicas>*
    p =
      dynamic_cast<visitor<S_msrMeasureRepeatReplicas>*> (v)) {
        S_msrMeasureRepeatReplicas elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrMeasureRepeatReplicas::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrMeasureRepeatReplicas::browseData (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrMeasureRepeatReplicas::browseData ()" <<
      endl;
  }

  if (fMeasureRepeatReplicasSegment) {
  // browse the replicas segment
    msrBrowser<msrSegment> browser (v);
    browser.browse (*fMeasureRepeatReplicasSegment);
  }
}

string msrMeasureRepeatReplicas::asString () const
{
  stringstream s;

  s <<

    "MeasureRepeatReplicas" <<
    ", line " << fInputLineNumber <<
    " (" <<
    mfSingularOrPlural (
      fetchMeasuresNumber (),
      "replicas measure",
      "replicas measures") <<
    ")";

  return s.str ();
}

void msrMeasureRepeatReplicas::print (ostream& os) const
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

  if (! fMeasureRepeatReplicasSegment) {
    os <<
      " none" <<
      endl;
  }
  else {
    os << endl;

    ++gIndenter;

    os <<
      fMeasureRepeatReplicasSegment;

    --gIndenter;
  }

  --gIndenter;

  os << ']' << endl;
}

ostream& operator<< (ostream& os, const S_msrMeasureRepeatReplicas& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_msrMeasureRepeat msrMeasureRepeat::create (
  int        inputLineNumber,
  int        measureRepeatMeasuresNumber,
  int        measureRepeatSlashesNumber,
  S_msrVoice voiceUpLink)
{
  msrMeasureRepeat* o =
    new msrMeasureRepeat (
      inputLineNumber,
      measureRepeatMeasuresNumber,
      measureRepeatSlashesNumber,
      voiceUpLink);
  assert (o != nullptr);
  return o;
}

msrMeasureRepeat::msrMeasureRepeat (
  int        inputLineNumber,
  int        measureRepeatMeasuresNumber,
  int        measureRepeatSlashesNumber,
  S_msrVoice voiceUpLink)
    : msrSegmentElement (inputLineNumber)
{
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    measureRepeatMeasuresNumber > 0,
    "measureRepeatMeasuresNumber is not positive");

  fMeasureRepeatMeasuresNumber = measureRepeatMeasuresNumber;

  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    measureRepeatSlashesNumber > 0,
    "measureRepeatSlashesNumber is not positive");

  fMeasureRepeatSlashesNumber  = measureRepeatSlashesNumber;

  fMeasureRepeatVoiceUpLink = voiceUpLink;

  // measures repeat build phase
  fCurrentMeasureRepeatBuildPhaseKind =
    msrMeasureRepeat::kMeasureRepeatBuildPhaseJustCreated;
}

msrMeasureRepeat::~msrMeasureRepeat ()
{}

S_msrMeasureRepeat msrMeasureRepeat::createMeasureRepeatNewbornClone (
  S_msrVoice containingVoice)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasureRepeats ()) {
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

  S_msrMeasureRepeat
    newbornClone =
      msrMeasureRepeat::create (
        fInputLineNumber,
        fMeasureRepeatMeasuresNumber,
        fMeasureRepeatSlashesNumber,
        containingVoice);

  return newbornClone;
}

void msrMeasureRepeat::setMeasureRepeatPattern (
  S_msrMeasureRepeatPattern measureRepeatPattern)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasureRepeats ()) {
    gLogStream <<
      "Setting measures repeat pattern containing " <<
      mfSingularOrPlural (
        measureRepeatPattern->
          fetchMeasuresNumber (),
        "measure",
        "measures") <<
      endl;
  }
#endif

  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    measureRepeatPattern != nullptr,
    "measureRepeatPattern is null");

  fMeasureRepeatPattern = measureRepeatPattern;

  // set currentRepeat's build phase
  fCurrentMeasureRepeatBuildPhaseKind =
    msrMeasureRepeat::kMeasureRepeatBuildPhaseInPattern;
}

void msrMeasureRepeat::setMeasureRepeatReplicas (
  S_msrMeasureRepeatReplicas measureRepeatReplicas)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasureRepeats ()) {
    gLogStream <<
      "Setting measures repeat replicas containing " <<
      mfSingularOrPlural (
        measureRepeatReplicas->
          fetchMeasuresNumber (),
        "measure",
        "measures") <<
      endl;
  }
#endif

  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    measureRepeatReplicas != nullptr,
    "measureRepeatReplicas is null");

  fMeasureRepeatReplicas = measureRepeatReplicas;

  // set currentRepeat's build phase
  fCurrentMeasureRepeatBuildPhaseKind =
    msrMeasureRepeat::kMeasureRepeatBuildPhaseInReplicas;
}

int msrMeasureRepeat::fetchMeasureRepeatReplicasNumber () const
{
  int patternMeasuresNumber =
    fetchMeasureRepeatPatternMeasuresNumber ();

  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    patternMeasuresNumber > 0,
    "patternMeasuresNumber is not positive");

  return
    fetchMeasureRepeatReplicasMeasuresNumber ()
      / // JMI ???
    patternMeasuresNumber;
}

int msrMeasureRepeat::fetchMeasureRepeatPatternMeasuresNumber () const
{
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    fMeasureRepeatPattern != nullptr,
    "fMeasureRepeatPattern is null");

  return
    fMeasureRepeatPattern->
      fetchMeasuresNumber ();
}

int msrMeasureRepeat::fetchMeasureRepeatReplicasMeasuresNumber () const
{
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    fMeasureRepeatReplicas != nullptr,
    "fMeasureRepeatReplicas is null");

  return
    fMeasureRepeatReplicas->
      fetchMeasuresNumber ();
}

void msrMeasureRepeat::acceptIn (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrMeasureRepeat::acceptIn ()" <<
      endl;
  }

  if (visitor<S_msrMeasureRepeat>*
    p =
      dynamic_cast<visitor<S_msrMeasureRepeat>*> (v)) {
        S_msrMeasureRepeat elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrMeasureRepeat::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrMeasureRepeat::acceptOut (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrMeasureRepeat::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrMeasureRepeat>*
    p =
      dynamic_cast<visitor<S_msrMeasureRepeat>*> (v)) {
        S_msrMeasureRepeat elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrMeasureRepeat::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrMeasureRepeat::browseData (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrMeasureRepeat::browseData ()" <<
      endl;
  }

  if (fMeasureRepeatPattern) {
  // browse the measdure repeat pattern
    msrBrowser<msrMeasureRepeatPattern> browser (v);
    browser.browse (*fMeasureRepeatPattern);
  }

  // fetch the score
  S_msrScore
    score =
      fMeasureRepeatVoiceUpLink->
        fetchVoiceScoreUpLink ();

  Bool inhibitMeasureRepeatReplicasBrowsing =
    score->getInhibitMeasureRepeatReplicasBrowsing ();

  if (inhibitMeasureRepeatReplicasBrowsing) {
#ifdef TRACING_IS_ENABLED
    if (gGlobalMsrOahGroup->getTraceMsrVisitors () || gGlobalTracingOahGroup->getTraceMeasureRepeats ()) {
      gLogStream <<
        "% ==> visiting measures repeat replicas is inhibited" <<
        endl;
    }
#endif
  }

  if (fMeasureRepeatReplicas) {
    if (! inhibitMeasureRepeatReplicasBrowsing) {
      // browse the measures repeat replicas
      msrBrowser<msrMeasureRepeatReplicas> browser (v);
      browser.browse (*fMeasureRepeatReplicas);
    }
  }
}

void msrMeasureRepeat:: appendMeasureElementToSegmentElement (
  S_msrMeasureElement elem)
{
  stringstream s;

  s <<
    "cannot append measure element " <<
    elem->asShortString () <<
    " to measure repeat " <<
    asShortString ();

  msrInternalError (
    gGlobalServiceRunData->getInputSourceName (),
    fInputLineNumber,
    __FILE__, __LINE__,
    s.str ());
}

string msrMeasureRepeat::measureRepeatBuildPhaseKindAsString (
  msrMeasureRepeatBuildPhaseKind measureRepeatBuildPhaseKind)
{
  string result;

  switch (measureRepeatBuildPhaseKind) {
    case msrMeasureRepeat::kMeasureRepeatBuildPhaseJustCreated:
      result = "measureRepeatBuildPhaseJustCreated";
      break;
    case msrMeasureRepeat::kMeasureRepeatBuildPhaseInPattern:
      result = "measureRepeatBuildPhaseInPattern";
      break;
    case msrMeasureRepeat::kMeasureRepeatBuildPhaseInReplicas:
      result = "measureRepeatBuildPhaseInReplicas";
      break;
    case msrMeasureRepeat::kMeasureRepeatBuildPhaseCompleted:
      result = "measureRepeatBuildPhaseCompleted";
      break;
  } // switch

  return result;
}

string msrMeasureRepeat::asString () const
{
  stringstream s;

  s <<

    "'MeasureRepeat" <<
    ", line " << fInputLineNumber <<
    " (" <<
    mfSingularOrPlural (
      fetchMeasureRepeatPatternMeasuresNumber (),
      "repeated measure",
      "repeated measures") <<
    ", " <<
    mfSingularOrPlural (
      fetchMeasureRepeatReplicasMeasuresNumber (),
      "replicas measure",
      "replicas measures") <<
    ", " <<
    fetchMeasureRepeatReplicasNumber () << " replicas" <<
    ")" <<
    "]";

  return s.str ();
}

void msrMeasureRepeat::displayMeasureRepeat (
  int           inputLineNumber,
  const string& context)
{
  gLogStream <<
    endl <<
    "*********>> MeasureRepeat " <<
    ", measureRepeatMeasuresNumber: '" <<
    fMeasureRepeatMeasuresNumber <<
    ", measureRepeatSlashesNumber: '" <<
    fMeasureRepeatSlashesNumber <<
    "', voice:" <<
    endl <<
    fMeasureRepeatVoiceUpLink->getVoiceName () <<
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

void msrMeasureRepeat::print (ostream& os) const
{
  os <<
    "[MeasureRepeat" <<
    " (" <<
    mfSingularOrPlural (
      fMeasureRepeatPattern
        ? fetchMeasureRepeatPatternMeasuresNumber ()
        : 0,
      "pattern measure",
      "pattern measures") <<
    ", " <<
    mfSingularOrPlural (
      fMeasureRepeatReplicas
        ? fetchMeasureRepeatReplicasMeasuresNumber ()
        : 0,
      "replica measure",
      "replicas measures") <<
    ", line " << fInputLineNumber <<
    endl;

  ++gIndenter;

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasureRepeats ()) {
    // print the current measures repeat build phase
    const int fieldWidth = 36;

    os <<
      setw (fieldWidth) <<
      "fCurrentMeasureRepeatBuildPhaseKind" << " : " <<
      measureRepeatBuildPhaseKindAsString (
        fCurrentMeasureRepeatBuildPhaseKind) <<
      endl;
  }
#endif

  // print the measures repeat pattern
  if (! fMeasureRepeatPattern) {
    os <<
      "measures repeat pattern: none" <<
      endl;
  }

  else {
    os <<
      fMeasureRepeatPattern;
  }

  // print the measures repeat replicas
  if (! fMeasureRepeatReplicas) {
    os <<
      "measures repeat replicas: none" <<
      endl;
  }

  else {
    os <<
      fMeasureRepeatReplicas;
  }

  --gIndenter;

  os << ']' << endl;
}

void msrMeasureRepeat::printShort (ostream& os) const
{
  print (os);
}

ostream& operator<< (ostream& os, const S_msrMeasureRepeat& elt)
{
  elt->print (os);
  return os;
}


}
