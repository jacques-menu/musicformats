/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <iomanip>

#include "mfStaticSettings.h"

#include "visitor.h"

#include "mfStaticSettings.h"

#include "mfAssert.h"
#include "mfStringsHandling.h"

#include "msrWae.h"

#include "msrMeasureRepeats.h"

#include "oahOah.h"

#include "msrOah.h"

#include "msrBrowsers.h"

#include "waeHandlers.h"


namespace MusicFormats
{

/* JMI
//______________________________________________________________________________
S_msrMeasureRepeatElement msrMeasureRepeatElement::create (
  int                 inputLineNumber,
  const S_msrMeasureRepeat& upLinkToMeasureRepeat)
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
  const S_msrMeasureRepeat& upLinkToMeasureRepeat)
    : msrElement (inputLineNumber)
{
#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    upLinkToMeasureRepeat != nullptr,
    "upLinkToMeasureRepeat is null");
#endif

  fMeasureRepeatElementUpLinkToMeasureRepeat = upLinkToMeasureRepeat;
}

msrMeasureRepeatElement::~msrMeasureRepeatElement ()
{}

void msrMeasureRepeatElement::appendSegmentToMeasureRepeatElementsList ( // JMI ???
  int          inputLineNumber,
  const S_msrSegment& segment,
  std::string       context)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceVoices ()) {
		std::stringstream ss;

    ss <<
      "Appending segment '" << segment <<
      "' to measureRepeat element elements list '" << asString () <<
      "' (" << context << ")" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    segment != nullptr,
    "segment is null");
#endif

  fMeasureRepeatElementsList.push_back (segment);
}

void msrMeasureRepeatElement::appendMeasureRepeatToMeasureRepeatElementsList (
  int                  inputLineNumber,
  S_msrMeasureRepeat  measureRepeat,
  std::string          context)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceVoices ()) {
		std::stringstream ss;

    ss <<
      "Appending measureRepeat '" << measureRepeat <<
      "' to measureRepeat element elements list '" << asString () <<
      "' (" << context << ")" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    measureRepeat != nullptr,
    "measureRepeat is null");
#endif

  fMeasureRepeatElementsList.push_back (measureRepeat);
}

void msrMeasureRepeatElement::appendVoiceElementToMeasureRepeatElementsList (
  int               inputLineNumber,
  const S_msrVoiceElement& voiceElement,
  std::string       context)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceVoices ()) {
		std::stringstream ss;

    ss <<
      "Appending voice element '" << voiceElement <<
      "' to measureRepeat element elements list '" << asString () <<
      "' (" << context << ")" <<
      ", line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    voiceElement != nullptr,
    "voiceElement is null");
#endif

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
        std::stringstream ss;

        ss <<
          "tuplet first element should be a note, a chord or another tuplet, found instead '" <<
          element->asShortString () <<
          "'";

        msrInternalError (
          gServiceRunData->getInputSourceName (),
          fInputLineNumber,
          __FILE__, __LINE__,
          ss.str ());
      }

      if (++i == iEnd) break;
    } // for
  }

  return result;
}

void msrMeasureRepeatElement::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrMeasureRepeatElement::acceptIn ()" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif

  if (visitor<S_msrMeasureRepeatElement>*
    p =
      dynamic_cast<visitor<S_msrMeasureRepeatElement>*> (v)) {
        S_msrMeasureRepeatElement elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrMeasureRepeatElement::visitStart ()" <<
            std::endl;

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif
        p->visitStart (elem);
  }
}

void msrMeasureRepeatElement::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrMeasureRepeatElement::acceptOut ()" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif

  if (visitor<S_msrMeasureRepeatElement>*
    p =
      dynamic_cast<visitor<S_msrMeasureRepeatElement>*> (v)) {
        S_msrMeasureRepeatElement elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrMeasureRepeatElement::visitEnd ()" <<
            std::endl;

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif
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
  std::stringstream ss;

  ss <<
    "MeasureRepeatElement" <<
    ", measureRepeat upLink: '" <<
    fMeasureRepeatElementUpLinkToMeasureRepeat->
      asShortString () <<
    "', line " << fInputLineNumber <<
    std::endl;

  return ss.str ();
}

void msrMeasureRepeatElement::printFull (std::ostream& os) const
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

void msrMeasureRepeatElement::print (std::ostream& os) const
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
      os << (*i);
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
  const S_msrMeasureRepeat& upLinkToMeasureRepeat)
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
  const S_msrMeasureRepeat& upLinkToMeasureRepeat)
    : msrElement (inputLineNumber)
{
#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    upLinkToMeasureRepeat != nullptr,
    "upLinkToMeasureRepeat is null");
#endif

  fUpLinkToMeasureRepeat = upLinkToMeasureRepeat;
}

msrMeasureRepeatPattern::~msrMeasureRepeatPattern ()
{}

void msrMeasureRepeatPattern::setMeasureRepeatPatternSegment (
  const S_msrSegment& measureRepeatPatternSegment)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceMeasureRepeats ()) {
		std::stringstream ss;

    ss <<
      "Setting measures repeat pattern segment containing " <<
      mfSingularOrPlural (
        fetchMeasuresNumber (),
        "measure",
        "measures") <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    measureRepeatPatternSegment != nullptr,
    "measureRepeatPatternSegment is null");
#endif

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
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrMeasureRepeatPattern::acceptIn ()" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif

  if (visitor<S_msrMeasureRepeatPattern>*
    p =
      dynamic_cast<visitor<S_msrMeasureRepeatPattern>*> (v)) {
        S_msrMeasureRepeatPattern elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrMeasureRepeatPattern::visitStart ()" <<
            std::endl;

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif
        p->visitStart (elem);
  }
}

void msrMeasureRepeatPattern::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrMeasureRepeatPattern::acceptOut ()" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif

  if (visitor<S_msrMeasureRepeatPattern>*
    p =
      dynamic_cast<visitor<S_msrMeasureRepeatPattern>*> (v)) {
        S_msrMeasureRepeatPattern elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrMeasureRepeatPattern::visitEnd ()" <<
            std::endl;

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif
        p->visitEnd (elem);
  }
}

void msrMeasureRepeatPattern::browseData (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrMeasureRepeatPattern::browseData ()" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif

  if (fMeasureRepeatPatternSegment) {
    // browse the pattern segment
    msrBrowser<msrSegment> browser (v);
    browser.browse (*fMeasureRepeatPatternSegment);
  }
}

std::string msrMeasureRepeatPattern::asString () const
{
  std::stringstream ss;

  ss <<

    "MeasureRepeatPattern" <<
    ", line " << fInputLineNumber <<
    " (" <<
    mfSingularOrPlural (
      fetchMeasuresNumber (),
      "repeated measure",
      "repeated measures") <<
    ")";

  return ss.str ();
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
  const S_msrMeasureRepeat& upLinkToMeasureRepeat)
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
  const S_msrMeasureRepeat& upLinkToMeasureRepeat)
    : msrElement (inputLineNumber)
{
#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    upLinkToMeasureRepeat != nullptr,
    "upLinkToMeasureRepeat is null");
#endif

  fUpLinkToMeasureRepeat = upLinkToMeasureRepeat;
}

msrMeasureRepeatReplicas::~msrMeasureRepeatReplicas ()
{}

void msrMeasureRepeatReplicas::setMeasureRepeatReplicasSegment (
  const S_msrSegment& measureRepeatReplicasSegment)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceMeasureRepeats ()) {
		std::stringstream ss;

    ss <<
      "Setting measures repeat replicas segment containing " <<
      mfSingularOrPlural (
        fetchMeasuresNumber (),
        "measure",
        "measures") <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    measureRepeatReplicasSegment != nullptr,
    "measureRepeatReplicasSegment is null");
#endif

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
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrMeasureRepeatReplicas::acceptIn ()" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif

  if (visitor<S_msrMeasureRepeatReplicas>*
    p =
      dynamic_cast<visitor<S_msrMeasureRepeatReplicas>*> (v)) {
        S_msrMeasureRepeatReplicas elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrMeasureRepeatReplicas::visitStart ()" <<
            std::endl;

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif
        p->visitStart (elem);
  }
}

void msrMeasureRepeatReplicas::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrMeasureRepeatReplicas::acceptOut ()" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif

  if (visitor<S_msrMeasureRepeatReplicas>*
    p =
      dynamic_cast<visitor<S_msrMeasureRepeatReplicas>*> (v)) {
        S_msrMeasureRepeatReplicas elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrMeasureRepeatReplicas::visitEnd ()" <<
            std::endl;

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif
        p->visitEnd (elem);
  }
}

void msrMeasureRepeatReplicas::browseData (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrMeasureRepeatReplicas::browseData ()" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif

  if (fMeasureRepeatReplicasSegment) {
  // browse the replicas segment
    msrBrowser<msrSegment> browser (v);
    browser.browse (*fMeasureRepeatReplicasSegment);
  }
}

std::string msrMeasureRepeatReplicas::asString () const
{
  std::stringstream ss;

  ss <<

    "MeasureRepeatReplicas" <<
    ", line " << fInputLineNumber <<
    " (" <<
    mfSingularOrPlural (
      fetchMeasuresNumber (),
      "replicas measure",
      "replicas measures") <<
    ")";

  return ss.str ();
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
  const S_msrVoice& upLinkToVoice)
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
  const S_msrVoice& upLinkToVoice)
    : msrSegmentElement (inputLineNumber)
{
#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    measureRepeatMeasuresNumber > 0,
    "measureRepeatMeasuresNumber is not positive");
#endif

  fMeasureRepeatMeasuresNumber = measureRepeatMeasuresNumber;

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    measureRepeatSlashesNumber > 0,
    "measureRepeatSlashesNumber is not positive");
#endif

  fMeasureRepeatSlashesNumber  = measureRepeatSlashesNumber;

  fUpLinkToMeasureRepeatToVoice = upLinkToVoice;

  // measures repeat build phase
  fCurrentMeasureRepeatBuildPhaseKind =
    msrMeasureRepeatBuildPhaseKind::kMeasureRepeatBuildPhaseJustCreated;
}

msrMeasureRepeat::~msrMeasureRepeat ()
{}

S_msrMeasureRepeat msrMeasureRepeat::createMeasureRepeatNewbornClone (
  const S_msrVoice& containingVoice)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceMeasureRepeats ()) {
		std::stringstream ss;

    ss <<
      "Creating a newborn clone of measures repeat '" <<
      asString () <<
      "'" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    containingVoice != nullptr,
    "containingVoice is null");
#endif

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
  const S_msrMeasureRepeatPattern& measureRepeatPattern)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceMeasureRepeats ()) {
		std::stringstream ss;

    ss <<
      "Setting measures repeat pattern containing " <<
      mfSingularOrPlural (
        measureRepeatPattern->
          fetchMeasuresNumber (),
        "measure",
        "measures") <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    measureRepeatPattern != nullptr,
    "measureRepeatPattern is null");
#endif

  fMeasureRepeatPattern = measureRepeatPattern;

  // set currentRepeat's build phase
  fCurrentMeasureRepeatBuildPhaseKind =
    msrMeasureRepeatBuildPhaseKind::kMeasureRepeatBuildPhaseInPattern;
}

void msrMeasureRepeat::setMeasureRepeatReplicas (
  const S_msrMeasureRepeatReplicas& measureRepeatReplicas)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceMeasureRepeats ()) {
		std::stringstream ss;

    ss <<
      "Setting measures repeat replicas containing " <<
      mfSingularOrPlural (
        measureRepeatReplicas->
          fetchMeasuresNumber (),
        "measure",
        "measures") <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    measureRepeatReplicas != nullptr,
    "measureRepeatReplicas is null");
#endif

  fMeasureRepeatReplicas = measureRepeatReplicas;

  // set currentRepeat's build phase
  fCurrentMeasureRepeatBuildPhaseKind =
    msrMeasureRepeatBuildPhaseKind::kMeasureRepeatBuildPhaseInReplicas;
}

int msrMeasureRepeat::fetchMeasureRepeatReplicasNumber () const
{
  int patternMeasuresNumber =
    fetchMeasureRepeatPatternMeasuresNumber ();

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    patternMeasuresNumber > 0,
    "patternMeasuresNumber is not positive");
#endif

  return
    fetchMeasureRepeatReplicasMeasuresNumber ()
      / // JMI ???
    patternMeasuresNumber;
}

int msrMeasureRepeat::fetchMeasureRepeatPatternMeasuresNumber () const
{
#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    fMeasureRepeatPattern != nullptr,
    "fMeasureRepeatPattern is null");
#endif

  return
    fMeasureRepeatPattern->
      fetchMeasuresNumber ();
}

int msrMeasureRepeat::fetchMeasureRepeatReplicasMeasuresNumber () const
{
#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    fMeasureRepeatReplicas != nullptr,
    "fMeasureRepeatReplicas is null");
#endif

  return
    fMeasureRepeatReplicas->
      fetchMeasuresNumber ();
}

void msrMeasureRepeat::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrMeasureRepeat::acceptIn ()" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif

  if (visitor<S_msrMeasureRepeat>*
    p =
      dynamic_cast<visitor<S_msrMeasureRepeat>*> (v)) {
        S_msrMeasureRepeat elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrMeasureRepeat::visitStart ()" <<
            std::endl;

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif
        p->visitStart (elem);
  }
}

void msrMeasureRepeat::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrMeasureRepeat::acceptOut ()" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif

  if (visitor<S_msrMeasureRepeat>*
    p =
      dynamic_cast<visitor<S_msrMeasureRepeat>*> (v)) {
        S_msrMeasureRepeat elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrMeasureRepeat::visitEnd ()" <<
            std::endl;

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif
        p->visitEnd (elem);
  }
}

void msrMeasureRepeat::browseData (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrMeasureRepeat::browseData ()" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif

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
#ifdef MF_TRACE_IS_ENABLED
    if (
      gGlobalMsrOahGroup->getTraceMsrVisitors ()
        ||
      gGlobalTraceOahGroup->getTraceMeasureRepeats ()
    ) {
      std::stringstream ss;

      ss <<
        "% ==> visiting measures repeat replicas is inhibited" <<
        std::endl;

      gWaeHandler->waeTrace (
        __FILE__, __LINE__,
        ss.str ());
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
  const S_msrMeasureElement& elem)
{
  std::stringstream ss;

  ss <<
    "cannot append measure element " <<
    elem->asShortString () <<
    " to measure repeat " <<
    asShortString ();

  msrInternalError (
    gServiceRunData->getInputSourceName (),
    fInputLineNumber,
    __FILE__, __LINE__,
    ss.str ());
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
  std::stringstream ss;

  ss <<

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

  return ss.str ();
}

void msrMeasureRepeat::displayMeasureRepeat (
  int           inputLineNumber,
  const std::string& context)
{
  gLog <<
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
  print (gLog);
  --gIndenter;

  gLog <<
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

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceMeasureRepeats ()) {
    // print the current measures repeat build phase
    const int fieldWidth = 36;

    os <<
      std::setw (fieldWidth) <<
      "fCurrentMeasureRepeatBuildPhaseKind" << ": " <<
      measureRepeatBuildPhaseKindAsString (
        fCurrentMeasureRepeatBuildPhaseKind) <<
      std::endl;
  }
#endif

  // print the measures repeat pattern
  if (! fMeasureRepeatPattern) {
    os <<
      "fMeasureRepeatPattern: [NONE]" <<
      std::endl;
  }

  else {
    os <<
      fMeasureRepeatPattern;
  }

  // print the measures repeat replicas
  if (! fMeasureRepeatReplicas) {
    os <<
      "fMeasureRepeatReplicas: [NONE]" <<
      std::endl;
  }

  else {
    os <<
      fMeasureRepeatReplicas;
  }

  --gIndenter;

  os << ']' << std::endl;
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
