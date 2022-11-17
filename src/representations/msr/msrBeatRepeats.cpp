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

#include "msrBeatRepeats.h"

#include "oahOah.h"

#include "msrOah.h"

#include "msrBrowsers.h"


namespace MusicFormats
{

/* JMI
//______________________________________________________________________________
S_msrBeatRepeatElement msrBeatRepeatElement::create (
  int                 inputLineNumber,
  S_msrBeatRepeat upLinkToBeatRepeat)
{
  msrBeatRepeatElement* o =
    new msrBeatRepeatElement (
      inputLineNumber,
      upLinkToBeatRepeat);
  assert (o != nullptr);
  return o;
}

msrBeatRepeatElement::msrBeatRepeatElement (
  int                 inputLineNumber,
  S_msrBeatRepeat upLinkToBeatRepeat)
    : msrElement (inputLineNumber)
{
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    upLinkToBeatRepeat != nullptr,
    "upLinkToBeatRepeat is null");

  fBeatRepeatElementUpLinkToBeatRepeat = upLinkToBeatRepeat;
}

msrBeatRepeatElement::~msrBeatRepeatElement ()
{}

void msrBeatRepeatElement::appendSegmentToBeatRepeatElementsList ( // JMI ???
  int           inputLineNumber,
  const S_msrSegment&  segment,
  const std::string& context)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceVoices ()) {
    gLogStream <<
      "Appending segment '" << segment <<
      "' to beatRepeat element elements list '" << asString () <<
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

  fBeatRepeatElementsList.push_back (segment);
}

void msrBeatRepeatElement::appendBeatRepeatToBeatRepeatElementsList (
  int             inputLineNumber,
  S_msrBeatRepeat beatRepeat,
  const std::string&   context)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceVoices ()) {
    gLogStream <<
      "Appending beatRepeat '" << beatRepeat <<
      "' to beatRepeat element elements list '" << asString () <<
      "' (" << context << ")" <<
      ", line " << inputLineNumber <<
      std::endl;
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
  const S_msrVoiceElement& voiceElement,
  const std::string&     context)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceVoices ()) {
    gLogStream <<
      "Appending voice element '" << voiceElement <<
      "' to beatRepeat element elements list '" << asString () <<
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
    std::list<S_msrVoiceElement>::const_iterator
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

void msrBeatRepeatElement::acceptIn (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrBeatRepeatElement::acceptIn ()" <<
      std::endl;
  }

  if (visitor<S_msrBeatRepeatElement>*
    p =
      dynamic_cast<visitor<S_msrBeatRepeatElement>*> (v)) {
        S_msrBeatRepeatElement elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrBeatRepeatElement::visitStart ()" <<
            std::endl;
        }
        p->visitStart (elem);
  }
}

void msrBeatRepeatElement::acceptOut (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrBeatRepeatElement::acceptOut ()" <<
      std::endl;
  }

  if (visitor<S_msrBeatRepeatElement>*
    p =
      dynamic_cast<visitor<S_msrBeatRepeatElement>*> (v)) {
        S_msrBeatRepeatElement elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrBeatRepeatElement::visitEnd ()" <<
            std::endl;
        }
        p->visitEnd (elem);
  }
}

void msrBeatRepeatElement::browseData (basevisitor* v)
{
  // browse the elements
  if (fBeatRepeatElementsList.size ()) {
    for (
      std::list<S_msrVoiceElement>::const_iterator i = fBeatRepeatElementsList.begin ();
      i != fBeatRepeatElementsList.end ();
      ++i
  ) {
      // browse the element
      msrBrowser<msrVoiceElement> browser (v);
      browser.browse (*(*i));
    } // for
  }
}

std::string msrBeatRepeatElement::asString () const
{
  std::stringstream s;

  s <<
    "BeatRepeatElement" <<
    ", beatRepeat upLink: '" <<
    fBeatRepeatElementUpLinkToBeatRepeat->
      asShortString () <<
    "', line " << fInputLineNumber <<
    std::endl;

  return s.str ();
}

void msrBeatRepeatElement::print (std::ostream& os) const
{
  os <<
    "BeatRepeatElement" <<
    ", line " << fInputLineNumber <<
    std::endl;

  ++gIndenter;

  os <<
    "beatRepeat upLink: '" <<
    fBeatRepeatElementUpLinkToBeatRepeat->
      asShortString () <<
      "'" <<
    std::endl << std::endl;

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
    os << "[EMPTY]";
  }
  os << std::endl;

  if (elementsNumber) {
    os << std::endl;

    ++gIndenter;

    std::list<S_msrVoiceElement>::const_iterator
      iBegin = fBeatRepeatElementsList.begin (),
      iEnd   = fBeatRepeatElementsList.end (),
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

void msrBeatRepeatElement::printShort (std::ostream& os)
{
  os <<
    "BeatRepeatElement" <<
    ", line " << fInputLineNumber <<
    std::endl;

  ++gIndenter;

/ / * JMI
  os <<
    "beatRepeat upLink: '" <<
    fBeatRepeatElementUpLinkToBeatRepeat->
      asShortString () <<
      "'" <<
    std::endl;
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
    os << "[EMPTY]";
  }
  os << std::endl;

  if (elementsNumber) {
    os << std::endl;

    ++gIndenter;

    std::list<S_msrVoiceElement>::const_iterator
      iBegin = fBeatRepeatElementsList.begin (),
      iEnd   = fBeatRepeatElementsList.end (),
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

std::ostream& operator << (std::ostream& os, const S_msrBeatRepeatElement& elt)
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
S_msrBeatRepeatPattern msrBeatRepeatPattern::create (
  int                 inputLineNumber,
  S_msrBeatRepeat upLinkToBeatRepeat)
{
  msrBeatRepeatPattern* o =
    new msrBeatRepeatPattern (
      inputLineNumber,
      upLinkToBeatRepeat);
  assert (o != nullptr);
  return o;
}

msrBeatRepeatPattern::msrBeatRepeatPattern (
  int                 inputLineNumber,
  S_msrBeatRepeat upLinkToBeatRepeat)
    : msrElement (inputLineNumber)
{
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    upLinkToBeatRepeat != nullptr,
    "upLinkToBeatRepeat is null");

  fUpLinkToBeatRepeat = upLinkToBeatRepeat;
}

msrBeatRepeatPattern::~msrBeatRepeatPattern ()
{}

void msrBeatRepeatPattern::setBeatRepeatPatternSegment (
  const S_msrSegment& beatRepeatPatternSegment)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceBeatRepeats ()) {
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
      std::endl;
  }

  if (visitor<S_msrBeatRepeatPattern>*
    p =
      dynamic_cast<visitor<S_msrBeatRepeatPattern>*> (v)) {
        S_msrBeatRepeatPattern elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrBeatRepeatPattern::visitStart ()" <<
            std::endl;
        }
        p->visitStart (elem);
  }
}

void msrBeatRepeatPattern::acceptOut (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrBeatRepeatPattern::acceptOut ()" <<
      std::endl;
  }

  if (visitor<S_msrBeatRepeatPattern>*
    p =
      dynamic_cast<visitor<S_msrBeatRepeatPattern>*> (v)) {
        S_msrBeatRepeatPattern elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrBeatRepeatPattern::visitEnd ()" <<
            std::endl;
        }
        p->visitEnd (elem);
  }
}

void msrBeatRepeatPattern::browseData (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrBeatRepeatPattern::browseData ()" <<
      std::endl;
  }

  if (fBeatRepeatPatternSegment) {
    // browse the pattern segment
    msrBrowser<msrSegment> browser (v);
    browser.browse (*fBeatRepeatPatternSegment);
  }
}

std::string msrBeatRepeatPattern::asString () const
{
  std::stringstream s;

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

void msrBeatRepeatPattern::print (std::ostream& os) const
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

  if (! fBeatRepeatPatternSegment) {
    os <<
      " [NONE]" <<
      std::endl;
  }
  else {
    os << std::endl;

    ++gIndenter;

    os <<
      fBeatRepeatPatternSegment;

    --gIndenter;
  }

  --gIndenter;

  os << ']' << std::endl;
}

std::ostream& operator << (std::ostream& os, const S_msrBeatRepeatPattern& elt)
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
S_msrBeatRepeatReplicas msrBeatRepeatReplicas::create (
  int                 inputLineNumber,
  S_msrBeatRepeat upLinkToBeatRepeat)
{
  msrBeatRepeatReplicas* o =
    new msrBeatRepeatReplicas (
      inputLineNumber,
      upLinkToBeatRepeat);
  assert (o != nullptr);
  return o;
}

msrBeatRepeatReplicas::msrBeatRepeatReplicas (
  int                 inputLineNumber,
  S_msrBeatRepeat upLinkToBeatRepeat)
    : msrElement (inputLineNumber)
{
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    upLinkToBeatRepeat != nullptr,
    "upLinkToBeatRepeat is null");

  fUpLinkToBeatRepeat = upLinkToBeatRepeat;
}

msrBeatRepeatReplicas::~msrBeatRepeatReplicas ()
{}

void msrBeatRepeatReplicas::setBeatRepeatReplicasSegment (
  const S_msrSegment& beatRepeatReplicasSegment)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceBeatRepeats ()) {
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
      std::endl;
  }

  if (visitor<S_msrBeatRepeatReplicas>*
    p =
      dynamic_cast<visitor<S_msrBeatRepeatReplicas>*> (v)) {
        S_msrBeatRepeatReplicas elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrBeatRepeatReplicas::visitStart ()" <<
            std::endl;
        }
        p->visitStart (elem);
  }
}

void msrBeatRepeatReplicas::acceptOut (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrBeatRepeatReplicas::acceptOut ()" <<
      std::endl;
  }

  if (visitor<S_msrBeatRepeatReplicas>*
    p =
      dynamic_cast<visitor<S_msrBeatRepeatReplicas>*> (v)) {
        S_msrBeatRepeatReplicas elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrBeatRepeatReplicas::visitEnd ()" <<
            std::endl;
        }
        p->visitEnd (elem);
  }
}

void msrBeatRepeatReplicas::browseData (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrBeatRepeatReplicas::browseData ()" <<
      std::endl;
  }

  if (fBeatRepeatReplicasSegment) {
  // browse the replicas segment
    msrBrowser<msrSegment> browser (v);
    browser.browse (*fBeatRepeatReplicasSegment);
  }
}

std::string msrBeatRepeatReplicas::asString () const
{
  std::stringstream s;

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

void msrBeatRepeatReplicas::print (std::ostream& os) const
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

  if (! fBeatRepeatReplicasSegment) {
    os <<
      " [NONE]" <<
      std::endl;
  }
  else {
    os << std::endl;

    ++gIndenter;

    os <<
      fBeatRepeatReplicasSegment;

    --gIndenter;
  }

  --gIndenter;

  os << ']' << std::endl;
}

std::ostream& operator << (std::ostream& os, const S_msrBeatRepeatReplicas& elt)
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
S_msrBeatRepeat msrBeatRepeat::create (
  int        inputLineNumber,
  int        beatRepeatMeasuresNumber,
  int        beatRepeatSlashesNumber,
  const S_msrVoice& upLinkToVoice)
{
  msrBeatRepeat* o =
    new msrBeatRepeat (
      inputLineNumber,
      beatRepeatMeasuresNumber,
      beatRepeatSlashesNumber,
      upLinkToVoice);
  assert (o != nullptr);
  return o;
}

msrBeatRepeat::msrBeatRepeat (
  int        inputLineNumber,
  int        beatRepeatMeasuresNumber,
  int        beatRepeatSlashesNumber,
  const S_msrVoice& upLinkToVoice)
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

  fUpLinkToBeatRepeatToVoice = upLinkToVoice;

  // measures repeat build phase
  fCurrentBeatRepeatBuildPhaseKind =
    msrBeatRepeatBuildPhaseKind::kBeatRepeatBuildPhaseJustCreated;
}

msrBeatRepeat::~msrBeatRepeat ()
{}

S_msrBeatRepeat msrBeatRepeat::createBeatRepeatNewbornClone (
  const S_msrVoice& containingVoice)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceBeatRepeats ()) {
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
      std::endl;
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
    msrBeatRepeatBuildPhaseKind::kBeatRepeatBuildPhaseInPattern;
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
      std::endl;
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
    msrBeatRepeatBuildPhaseKind::kBeatRepeatBuildPhaseInReplicas;
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
      std::endl;
  }

  if (visitor<S_msrBeatRepeat>*
    p =
      dynamic_cast<visitor<S_msrBeatRepeat>*> (v)) {
        S_msrBeatRepeat elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrBeatRepeat::visitStart ()" <<
            std::endl;
        }
        p->visitStart (elem);
  }
}

void msrBeatRepeat::acceptOut (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrBeatRepeat::acceptOut ()" <<
      std::endl;
  }

  if (visitor<S_msrBeatRepeat>*
    p =
      dynamic_cast<visitor<S_msrBeatRepeat>*> (v)) {
        S_msrBeatRepeat elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrBeatRepeat::visitEnd ()" <<
            std::endl;
        }
        p->visitEnd (elem);
  }
}

void msrBeatRepeat::browseData (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrBeatRepeat::browseData ()" <<
      std::endl;
  }

  if (fBeatRepeatPattern) {
  // browse the measdure repeat pattern
    msrBrowser<msrBeatRepeatPattern> browser (v);
    browser.browse (*fBeatRepeatPattern);
  }

  // fetch the score
  S_msrScore
    score =
      fUpLinkToBeatRepeatToVoice->
        fetchVoiceUpLinkToScore ();

/* JMI
  Bool inhibitBeatRepeatReplicasBrowsing =
    score->getInhibitBeatRepeatReplicasBrowsing ();

  if (inhibitBeatRepeatReplicasBrowsing) {
#ifdef TRACING_IS_ENABLED
    if (gGlobalMsrOahGroup->getTraceMsrVisitors () || gGlobalTracingOahGroup->getTraceBeatRepeats ()) {
      gLogStream <<
        "% ==> visiting measures repeat replicas is inhibited" <<
        std::endl;
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
  const S_msrMeasureElement& elem)
{
  std::stringstream s;

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

std::string msrBeatRepeatKindAsString (
  msrBeatRepeatKind beatRepeatKind)
{
  std::string result;

  switch (beatRepeatKind) {
    case msrBeatRepeatKind::kBeatRepeat_NO_:
      result = "kBeatRepeat_NO_";
      break;
    case msrBeatRepeatKind::kBeatRepeatStart:
      result = "kBeatRepeatStart";
      break;
    case msrBeatRepeatKind::kBeatRepeatStop:
      result = "kBeatRepeatStop";
      break;
  } // switch

  return result;
}

std::ostream& operator << (std::ostream& os, const msrBeatRepeatKind& elt)
{
  os << msrBeatRepeatKindAsString (elt);
  return os;
}

std::string msrBeatRepeatBuildPhaseKindAsString (
  msrBeatRepeatBuildPhaseKind beatRepeatBuildPhaseKind)
{
  std::string result;

  switch (beatRepeatBuildPhaseKind) {
    case msrBeatRepeatBuildPhaseKind::kBeatRepeatBuildPhaseJustCreated:
      result = "kBeatRepeatBuildPhaseJustCreated";
      break;
    case msrBeatRepeatBuildPhaseKind::kBeatRepeatBuildPhaseInPattern:
      result = "kBeatRepeatBuildPhaseInPattern";
      break;
    case msrBeatRepeatBuildPhaseKind::kBeatRepeatBuildPhaseInReplicas:
      result = "kBeatRepeatBuildPhaseInReplicas";
      break;
    case msrBeatRepeatBuildPhaseKind::kBeatRepeatBuildPhaseCompleted:
      result = "kBeatRepeatBuildPhaseCompleted";
      break;
  } // switch

  return result;
}

std::ostream& operator << (std::ostream& os, const msrBeatRepeatBuildPhaseKind& elt)
{
  os << msrBeatRepeatBuildPhaseKindAsString (elt);
  return os;
}

std::string msrBeatRepeat::asString () const
{
  std::stringstream s;

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
    ']';

  return s.str ();
}

void msrBeatRepeat::displayBeatRepeat (
  int           inputLineNumber,
  const std::string& context)
{
  gLogStream <<
    std::endl <<
    "*********>> BeatRepeat " <<
    ", beatRepeatMeasuresNumber: '" <<
    fBeatRepeatMeasuresNumber <<
    ", beatRepeatSlashesNumber: '" <<
    fBeatRepeatSlashesNumber <<
    "', voice:" <<
    std::endl <<
    fUpLinkToBeatRepeatToVoice->getVoiceName () <<
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

void msrBeatRepeat::print (std::ostream& os) const
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
    std::endl;

  ++gIndenter;

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceBeatRepeats ()) {
    // print the current measures repeat build phase
    const int fieldWidth = 36;

    os <<
      std::setw (fieldWidth) <<
      "fCurrentBeatRepeatBuildPhaseKind" << " : " <<
      msrBeatRepeatBuildPhaseKindAsString (
        fCurrentBeatRepeatBuildPhaseKind) <<
      std::endl;
  }
#endif

  // print the measures repeat pattern
  if (! fBeatRepeatPattern) {
    os <<
      "measures repeat pattern: [NONE]" <<
      std::endl;
  }

  else {
    os <<
      fBeatRepeatPattern;
  }

  // print the measures repeat replicas
  if (! fBeatRepeatReplicas) {
    os <<
      "measures repeat replicas: [NONE]" <<
      std::endl;
  }

  else {
    os <<
      fBeatRepeatReplicas;
  }

  --gIndenter;

  os << ']' << std::endl;
}

void msrBeatRepeat::printShort (std::ostream& os) const
{
  print (os);
}

std::ostream& operator << (std::ostream& os, const S_msrBeatRepeat& elt)
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
