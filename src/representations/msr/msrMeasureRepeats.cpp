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

#include "oahEnableTracingIfDesired.h"
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


namespace MusicFormats
{

/* JMI
//______________________________________________________________________________
S_msrMeasureRepeatElement msrMeasureRepeatElement::create (
  int                 inputLineNumber,
  S_msrMeasureRepeat upLinkToMeasureRepeat)
{
  msrMeasureRepeatElement* o =
    new msrMeasureRepeatElement (
      inputLineNumber,
      upLinkToMeasureRepeat);
  assert (o != nullptr);
  return o;
}

msrMeasureRepeatElement::msrMeasureRepeatElement (
  int                 inputLineNumber,
  S_msrMeasureRepeat upLinkToMeasureRepeat)
    : msrElement (inputLineNumber)
{
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    upLinkToMeasureRepeat != nullptr,
    "upLinkToMeasureRepeat is null");

  fMeasureRepeatElementUpLinkToMeasureRepeat = upLinkToMeasureRepeat;
}

msrMeasureRepeatElement::~msrMeasureRepeatElement ()
{}

void msrMeasureRepeatElement::appendSegmentToMeasureRepeatElementsList ( // JMI ???
  int          inputLineNumber,
  S_msrSegment segment,
  std::string       context)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceVoices ()) {
    gLogStream <<
      "Appending segment '" << segment <<
      "' to measureRepeat element elements std::list '" << asString () <<
      "' (" << context << ")" <<
      ", line " << inputLineNumber <<
      std::endl;
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
  std::string          context)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceVoices ()) {
    gLogStream <<
      "Appending measureRepeat '" << measureRepeat <<
      "' to measureRepeat element elements std::list '" << asString () <<
      "' (" << context << ")" <<
      ", line " << inputLineNumber <<
      std::endl;
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
  std::string       context)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceVoices ()) {
    gLogStream <<
      "Appending voice element '" << voiceElement <<
      "' to measureRepeat element elements std::list '" << asString () <<
      "' (" << context << ")" <<
      ", line " << inputLineNumber <<
      std::endl;
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
    std::list<S_msrVoiceElement>::const_iterator
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
        std::stringstream s;

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
      std::endl;
  }

  if (visitor<S_msrMeasureRepeatElement>*
    p =
      dynamic_cast<visitor<S_msrMeasureRepeatElement>*> (v)) {
        S_msrMeasureRepeatElement elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrMeasureRepeatElement::visitStart ()" <<
            std::endl;
        }
        p->visitStart (elem);
  }
}

void msrMeasureRepeatElement::acceptOut (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrMeasureRepeatElement::acceptOut ()" <<
      std::endl;
  }

  if (visitor<S_msrMeasureRepeatElement>*
    p =
      dynamic_cast<visitor<S_msrMeasureRepeatElement>*> (v)) {
        S_msrMeasureRepeatElement elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrMeasureRepeatElement::visitEnd ()" <<
            std::endl;
        }
        p->visitEnd (elem);
  }
}

void msrMeasureRepeatElement::browseData (basevisitor* v)
{
  // browse the elements
  if (fMeasureRepeatElementsList.size ()) {
    for (
      std::list<S_msrVoiceElement>::const_iterator i = fMeasureRepeatElementsList.begin ();
      i != fMeasureRepeatElementsList.end ();
      ++i
  ) {
      // browse the element
      msrBrowser<msrVoiceElement> browser (v);
      browser.browse (*(*i));
    } // for
  }
}

std::string msrMeasureRepeatElement::asString () const
{
  std::stringstream s;

  s <<
    "MeasureRepeatElement" <<
    ", measureRepeat upLink: '" <<
    fMeasureRepeatElementUpLinkToMeasureRepeat->
      asShortString () <<
    "', line " << fInputLineNumber <<
    std::endl;

  return s.str ();
}

void msrMeasureRepeatElement::print (std::ostream& os) const
{
  os <<
    "MeasureRepeatElement" <<
    ", line " << fInputLineNumber <<
    std::endl;

  ++gIndenter;

  os <<
    "measureRepeat upLink: '" <<
    fMeasureRepeatElementUpLinkToMeasureRepeat->
      asShortString () <<
      "'" <<
    std::endl << std::endl;

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
    os << "[NONE]";
  }
  os << std::endl;

  if (elementsNumber) {
    os << std::endl;

    ++gIndenter;

    std::list<S_msrVoiceElement>::const_iterator
      iBegin = fMeasureRepeatElementsList.begin (),
      iEnd   = fMeasureRepeatElementsList.end (),
      i      = iBegin;

    for ( ; ; ) {
      // print the element
      os << (*i);
      if (++i == iEnd) break;
      os << std::endl;
    } // for

    --gIndenter;
  }

  --gIndenter;
}

void msrMeasureRepeatElement::printShort (std::ostream& os) const
{
  os <<
    "MeasureRepeatElement" <<
    ", line " << fInputLineNumber <<
    std::endl;

  ++gIndenter;

/ / * JMI
  os <<
    "measureRepeat upLink: '" <<
    fMeasureRepeatElementUpLinkToMeasureRepeat->
      asShortString () <<
      "'" <<
    std::endl;
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
    os << "[NONE]";
  }
  os << std::endl;

  if (elementsNumber) {
    os << std::endl;

    ++gIndenter;

    std::list<S_msrVoiceElement>::const_iterator
      iBegin = fMeasureRepeatElementsList.begin (),
      iEnd   = fMeasureRepeatElementsList.end (),
      i      = iBegin;

    for ( ; ; ) {
      // print the element
      (*i)->printShort (os);
      if (++i == iEnd) break;
      os << std::endl;
    } // for

    --gIndenter;
  }

  --gIndenter;
}

std::ostream& operator << (std::ostream& os, const S_msrMeasureRepeatElement& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NONE]" << std::endl;
  }

  return os;
}
*/

//______________________________________________________________________________
S_msrMeasureRepeatPattern msrMeasureRepeatPattern::create (
  int                 inputLineNumber,
  S_msrMeasureRepeat upLinkToMeasureRepeat)
{
  msrMeasureRepeatPattern* o =
    new msrMeasureRepeatPattern (
      inputLineNumber,
      upLinkToMeasureRepeat);
  assert (o != nullptr);
  return o;
}

msrMeasureRepeatPattern::msrMeasureRepeatPattern (
  int                 inputLineNumber,
  S_msrMeasureRepeat upLinkToMeasureRepeat)
    : msrElement (inputLineNumber)
{
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    upLinkToMeasureRepeat != nullptr,
    "upLinkToMeasureRepeat is null");

  fUpLinkToMeasureRepeat = upLinkToMeasureRepeat;
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
      std::endl;
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
      std::endl;
  }

  if (visitor<S_msrMeasureRepeatPattern>*
    p =
      dynamic_cast<visitor<S_msrMeasureRepeatPattern>*> (v)) {
        S_msrMeasureRepeatPattern elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrMeasureRepeatPattern::visitStart ()" <<
            std::endl;
        }
        p->visitStart (elem);
  }
}

void msrMeasureRepeatPattern::acceptOut (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrMeasureRepeatPattern::acceptOut ()" <<
      std::endl;
  }

  if (visitor<S_msrMeasureRepeatPattern>*
    p =
      dynamic_cast<visitor<S_msrMeasureRepeatPattern>*> (v)) {
        S_msrMeasureRepeatPattern elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrMeasureRepeatPattern::visitEnd ()" <<
            std::endl;
        }
        p->visitEnd (elem);
  }
}

void msrMeasureRepeatPattern::browseData (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrMeasureRepeatPattern::browseData ()" <<
      std::endl;
  }

  if (fMeasureRepeatPatternSegment) {
    // browse the pattern segment
    msrBrowser<msrSegment> browser (v);
    browser.browse (*fMeasureRepeatPatternSegment);
  }
}

std::string msrMeasureRepeatPattern::asString () const
{
  std::stringstream s;

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

void msrMeasureRepeatPattern::print (std::ostream& os) const
{
  os <<
    std::endl <<
    '[' <<
    asString () <<
    std::endl << std::endl;

  ++gIndenter;

  // print the pattern segment
  os <<
    "Pattern segment:";

  if (! fMeasureRepeatPatternSegment) {
    os <<
      " [NONE]" <<
      std::endl;
  }
  else {
    os << std::endl;

    ++gIndenter;

    os <<
      fMeasureRepeatPatternSegment;

    --gIndenter;
  }

  --gIndenter;

  os << ']' << std::endl;
}

std::ostream& operator << (std::ostream& os, const S_msrMeasureRepeatPattern& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NONE]" << std::endl;
  }

  return os;
}

//______________________________________________________________________________
S_msrMeasureRepeatReplicas msrMeasureRepeatReplicas::create (
  int                 inputLineNumber,
  S_msrMeasureRepeat upLinkToMeasureRepeat)
{
  msrMeasureRepeatReplicas* o =
    new msrMeasureRepeatReplicas (
      inputLineNumber,
      upLinkToMeasureRepeat);
  assert (o != nullptr);
  return o;
}

msrMeasureRepeatReplicas::msrMeasureRepeatReplicas (
  int                 inputLineNumber,
  S_msrMeasureRepeat upLinkToMeasureRepeat)
    : msrElement (inputLineNumber)
{
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    upLinkToMeasureRepeat != nullptr,
    "upLinkToMeasureRepeat is null");

  fUpLinkToMeasureRepeat = upLinkToMeasureRepeat;
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
      std::endl;
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
      std::endl;
  }

  if (visitor<S_msrMeasureRepeatReplicas>*
    p =
      dynamic_cast<visitor<S_msrMeasureRepeatReplicas>*> (v)) {
        S_msrMeasureRepeatReplicas elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrMeasureRepeatReplicas::visitStart ()" <<
            std::endl;
        }
        p->visitStart (elem);
  }
}

void msrMeasureRepeatReplicas::acceptOut (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrMeasureRepeatReplicas::acceptOut ()" <<
      std::endl;
  }

  if (visitor<S_msrMeasureRepeatReplicas>*
    p =
      dynamic_cast<visitor<S_msrMeasureRepeatReplicas>*> (v)) {
        S_msrMeasureRepeatReplicas elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrMeasureRepeatReplicas::visitEnd ()" <<
            std::endl;
        }
        p->visitEnd (elem);
  }
}

void msrMeasureRepeatReplicas::browseData (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrMeasureRepeatReplicas::browseData ()" <<
      std::endl;
  }

  if (fMeasureRepeatReplicasSegment) {
  // browse the replicas segment
    msrBrowser<msrSegment> browser (v);
    browser.browse (*fMeasureRepeatReplicasSegment);
  }
}

std::string msrMeasureRepeatReplicas::asString () const
{
  std::stringstream s;

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

void msrMeasureRepeatReplicas::print (std::ostream& os) const
{
  os <<
    std::endl <<
    '[' <<
    asString () <<
    std::endl << std::endl;

  ++gIndenter;

  // print the replicas segment
  os <<
    "Replicas segment:";

  if (! fMeasureRepeatReplicasSegment) {
    os <<
      " [NONE]" <<
      std::endl;
  }
  else {
    os << std::endl;

    ++gIndenter;

    os <<
      fMeasureRepeatReplicasSegment;

    --gIndenter;
  }

  --gIndenter;

  os << ']' << std::endl;
}

std::ostream& operator << (std::ostream& os, const S_msrMeasureRepeatReplicas& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NONE]" << std::endl;
  }

  return os;
}

//______________________________________________________________________________
S_msrMeasureRepeat msrMeasureRepeat::create (
  int        inputLineNumber,
  int        measureRepeatMeasuresNumber,
  int        measureRepeatSlashesNumber,
  S_msrVoice upLinkToVoice)
{
  msrMeasureRepeat* o =
    new msrMeasureRepeat (
      inputLineNumber,
      measureRepeatMeasuresNumber,
      measureRepeatSlashesNumber,
      upLinkToVoice);
  assert (o != nullptr);
  return o;
}

msrMeasureRepeat::msrMeasureRepeat (
  int        inputLineNumber,
  int        measureRepeatMeasuresNumber,
  int        measureRepeatSlashesNumber,
  S_msrVoice upLinkToVoice)
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

  fUpLinkToMeasureRepeatToVoice = upLinkToVoice;

  // measures repeat build phase
  fCurrentMeasureRepeatBuildPhaseKind =
    msrMeasureRepeatBuildPhaseKind::kMeasureRepeatBuildPhaseJustCreated;
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
      std::endl;
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
      std::endl;
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
    msrMeasureRepeatBuildPhaseKind::kMeasureRepeatBuildPhaseInPattern;
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
      std::endl;
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
    msrMeasureRepeatBuildPhaseKind::kMeasureRepeatBuildPhaseInReplicas;
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
      std::endl;
  }

  if (visitor<S_msrMeasureRepeat>*
    p =
      dynamic_cast<visitor<S_msrMeasureRepeat>*> (v)) {
        S_msrMeasureRepeat elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrMeasureRepeat::visitStart ()" <<
            std::endl;
        }
        p->visitStart (elem);
  }
}

void msrMeasureRepeat::acceptOut (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrMeasureRepeat::acceptOut ()" <<
      std::endl;
  }

  if (visitor<S_msrMeasureRepeat>*
    p =
      dynamic_cast<visitor<S_msrMeasureRepeat>*> (v)) {
        S_msrMeasureRepeat elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrMeasureRepeat::visitEnd ()" <<
            std::endl;
        }
        p->visitEnd (elem);
  }
}

void msrMeasureRepeat::browseData (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrMeasureRepeat::browseData ()" <<
      std::endl;
  }

  if (fMeasureRepeatPattern) {
  // browse the measdure repeat pattern
    msrBrowser<msrMeasureRepeatPattern> browser (v);
    browser.browse (*fMeasureRepeatPattern);
  }

  // fetch the score
  S_msrScore
    score =
      fUpLinkToMeasureRepeatToVoice->
        fetchVoiceUpLinkToScore ();

  Bool inhibitMeasureRepeatReplicasBrowsing =
    score->getInhibitMeasureRepeatReplicasBrowsing ();

  if (inhibitMeasureRepeatReplicasBrowsing) {
#ifdef TRACING_IS_ENABLED
    if (gGlobalMsrOahGroup->getTraceMsrVisitors () || gGlobalTracingOahGroup->getTraceMeasureRepeats ()) {
      gLogStream <<
        "% ==> visiting measures repeat replicas is inhibited" <<
        std::endl;
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
  std::stringstream s;

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

std::string measureRepeatBuildPhaseKindAsString (
  msrMeasureRepeatBuildPhaseKind measureRepeatBuildPhaseKind)
{
  std::string result;

  switch (measureRepeatBuildPhaseKind) {
    case msrMeasureRepeatBuildPhaseKind::kMeasureRepeatBuildPhaseJustCreated:
      result = "measureRepeatBuildPhaseJustCreated";
      break;
    case msrMeasureRepeatBuildPhaseKind::kMeasureRepeatBuildPhaseInPattern:
      result = "measureRepeatBuildPhaseInPattern";
      break;
    case msrMeasureRepeatBuildPhaseKind::kMeasureRepeatBuildPhaseInReplicas:
      result = "measureRepeatBuildPhaseInReplicas";
      break;
    case msrMeasureRepeatBuildPhaseKind::kMeasureRepeatBuildPhaseCompleted:
      result = "measureRepeatBuildPhaseCompleted";
      break;
  } // switch

  return result;
}

std::ostream& operator << (std::ostream& os, const msrMeasureRepeatBuildPhaseKind& elt)
{
  os << measureRepeatBuildPhaseKindAsString (elt);
  return os;
}

std::string msrMeasureRepeat::asString () const
{
  std::stringstream s;

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
    ']';

  return s.str ();
}

void msrMeasureRepeat::displayMeasureRepeat (
  int           inputLineNumber,
  const std::string& context)
{
  gLogStream <<
    std::endl <<
    "*********>> MeasureRepeat " <<
    ", measureRepeatMeasuresNumber: '" <<
    fMeasureRepeatMeasuresNumber <<
    ", measureRepeatSlashesNumber: '" <<
    fMeasureRepeatSlashesNumber <<
    "', voice:" <<
    std::endl <<
    fUpLinkToMeasureRepeatToVoice->getVoiceName () <<
    " (" << context << ")" <<
    ", line " << inputLineNumber <<
    " contains:" <<
    std::endl;

  ++gIndenter;
  print (gLogStream);
  --gIndenter;

  gLogStream <<
    " <<*********" <<
    std::endl << std::endl;
}

void msrMeasureRepeat::print (std::ostream& os) const
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
    std::endl;

  ++gIndenter;

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasureRepeats ()) {
    // print the current measures repeat build phase
    const int fieldWidth = 36;

    os <<
      std::setw (fieldWidth) <<
      "fCurrentMeasureRepeatBuildPhaseKind" << " : " <<
      measureRepeatBuildPhaseKindAsString (
        fCurrentMeasureRepeatBuildPhaseKind) <<
      std::endl;
  }
#endif

  // print the measures repeat pattern
  if (! fMeasureRepeatPattern) {
    os <<
      "measures repeat pattern: [NONE]" <<
      std::endl;
  }

  else {
    os <<
      fMeasureRepeatPattern;
  }

  // print the measures repeat replicas
  if (! fMeasureRepeatReplicas) {
    os <<
      "measures repeat replicas: [NONE]" <<
      std::endl;
  }

  else {
    os <<
      fMeasureRepeatReplicas;
  }

  --gIndenter;

  os << ']' << std::endl;
}

void msrMeasureRepeat::printShort (std::ostream& os) const
{
  print (os);
}

std::ostream& operator << (std::ostream& os, const S_msrMeasureRepeat& elt)
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
