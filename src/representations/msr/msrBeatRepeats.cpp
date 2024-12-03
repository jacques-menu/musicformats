/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2024

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <iomanip>

#include "visitor.h"

#include "mfPreprocessorSettings.h"

#include "mfAssert.h"
#include "mfStringsHandling.h"

#include "msrWae.h"

#include "msrBeatRepeats.h"

#include "oahOah.h"

#include "msrOah.h"
#include "oahEarlyOptions.h"

#include "msrBrowsers.h"

#include "waeHandlers.h"


namespace MusicFormats
{

/* JMI
//______________________________________________________________________________
S_msrBeatRepeatElement msrBeatRepeatElement::create (
  int                 inputLineNumber,
  const S_msrBeatRepeat& upLinkToBeatRepeat)
{
  msrBeatRepeatElement* obj =
    new msrBeatRepeatElement (
      inputLineNumber,
      upLinkToBeatRepeat);
  assert (obj != nullptr);
  return obj;
}

msrBeatRepeatElement::msrBeatRepeatElement (
  int                 inputLineNumber,
  const S_msrBeatRepeat& upLinkToBeatRepeat)
    : msrElement (inputLineNumber)
{
#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    upLinkToBeatRepeat != nullptr,
    "upLinkToBeatRepeat is null");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  fBeatRepeatElementUpLinkToBeatRepeat = upLinkToBeatRepeat;
}

msrBeatRepeatElement::~msrBeatRepeatElement ()
{}

void msrBeatRepeatElement::appendSegmentToBeatRepeatElementsList ( // JMI ???
  int           inputLineNumber,
  const S_msrSegment&  segment,
  const std::string& context)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceVoices ()) {
    std::stringstream ss;

    ss <<
      "Appending segment '" << segment <<
      "' to beatRepeat element elements list '" << asString () <<
      "' (" << context << ")" <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    segment != nullptr,
    "segment is null");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  fBeatRepeatElementsList.push_back (segment);
}

void msrBeatRepeatElement::appendBeatRepeatToBeatRepeatElementsList (
  int             inputLineNumber,
  S_msrBeatRepeat beatRepeat,
  const std::string&   context)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceVoices ()) {
    std::stringstream ss;

    ss <<
      "Appending beatRepeat '" << beatRepeat <<
      "' to beatRepeat element elements list '" << asString () <<
      "' (" << context << ")" <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    beatRepeat != nullptr,
    "beatRepeat is null");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  fBeatRepeatElementsList.push_back (beatRepeat);
}

void msrBeatRepeatElement::appendVoiceElementToBeatRepeatElementsList (
  int               inputLineNumber,
  const S_msrVoiceElement& voiceElement,
  const std::string&     context)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceVoices ()) {
    std::stringstream ss;

    ss <<
      "Appending voice element '" << voiceElement <<
      "' to beatRepeat element elements list '" << asString () <<
      "' (" << context << ")" <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    voiceElement != nullptr,
    "voiceElement is null");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

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
    std::list <S_msrVoiceElement>::const_iterator
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
        std::stringstream ss;

        ss <<
          "tuplet first element should be a note, a chord or another tuplet, found instead '" <<
          element->asShortString () <<
          "'";

        msrInternalError (
          gServiceRunData->getInputSourceName (),
          fInputStartLineNumber,
          __FILE__, __LINE__,
          ss.str ());
      }

      if (++i == iEnd) break;
    } // for
  }

  return result;
}

void msrBeatRepeatElement::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrBeatRepeatElement::acceptIn ()";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msrBeatRepeatElement>*
    p =
      dynamic_cast<visitor<S_msrBeatRepeatElement>*> (v)) {
        S_msrBeatRepeatElement elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrBeatRepeatElement::visitStart ()";

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void msrBeatRepeatElement::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrBeatRepeatElement::acceptOut ()";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msrBeatRepeatElement>*
    p =
      dynamic_cast<visitor<S_msrBeatRepeatElement>*> (v)) {
        S_msrBeatRepeatElement elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrBeatRepeatElement::visitEnd ()";

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void msrBeatRepeatElement::browseData (basevisitor* v)
{
  // browse the elements
  if (fBeatRepeatElementsList.size ()) {
    for (
      std::list <S_msrVoiceElement>::const_iterator i = fBeatRepeatElementsList.begin ();
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
  std::stringstream ss;

  ss <<
    "BeatRepeatElement" <<
    ", beatRepeat upLink: '" <<
    fBeatRepeatElementUpLinkToBeatRepeat->
      asShortString () <<
    "', line " << fInputStartLineNumber <<
    std::endl;

  return ss.str ();
}

void msrBeatRepeatElement::printFull (std::ostream& os) const
{
  os <<
    "BeatRepeatElement" <<
    ", line " << fInputStartLineNumber <<
    std::endl;

  ++gIndenter;

  os <<
    "beatRepeat upLink: '" <<
    fBeatRepeatElementUpLinkToBeatRepeat->
      asShortString () <<
      '\'' <<
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

    std::list <S_msrVoiceElement>::const_iterator
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

void msrBeatRepeatElement::print (std::ostream& os)
{
  os <<
    "BeatRepeatElement" <<
    ", line " << fInputStartLineNumber <<
    std::endl;

  ++gIndenter;

/ / * JMI
  os <<
    "beatRepeat upLink: '" <<
    fBeatRepeatElementUpLinkToBeatRepeat->
      asShortString () <<
      '\'' <<
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

    std::list <S_msrVoiceElement>::const_iterator
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

std::ostream& operator << (std::ostream& os, const S_msrBeatRepeatElement& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NULL]" << std::endl;
  }

  return os;
}
*/

//______________________________________________________________________________
S_msrBeatRepeatPattern msrBeatRepeatPattern::create (
  int                 inputLineNumber,
  const S_msrBeatRepeat& upLinkToBeatRepeat)
{
  msrBeatRepeatPattern* obj =
    new msrBeatRepeatPattern (
      inputLineNumber,
      upLinkToBeatRepeat);
  assert (obj != nullptr);
  return obj;
}

msrBeatRepeatPattern::msrBeatRepeatPattern (
  int                 inputLineNumber,
  const S_msrBeatRepeat& upLinkToBeatRepeat)
    : msrElement (inputLineNumber)
{
#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    upLinkToBeatRepeat != nullptr,
    "upLinkToBeatRepeat is null");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  fUpLinkToBeatRepeat = upLinkToBeatRepeat;
}

msrBeatRepeatPattern::~msrBeatRepeatPattern ()
{}

void msrBeatRepeatPattern::setBeatRepeatPatternSegment (
  const S_msrSegment& beatRepeatPatternSegment)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceBeatRepeats ()) {
    std::stringstream ss;

    ss <<
      "Setting measures repeat pattern segment containing " <<
      mfSingularOrPlural (
        fetchMeasuresNumber (),
        "measure",
        "measures");

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    beatRepeatPatternSegment != nullptr,
    "beatRepeatPatternSegment is null");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

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
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrBeatRepeatPattern::acceptIn ()";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msrBeatRepeatPattern>*
    p =
      dynamic_cast<visitor<S_msrBeatRepeatPattern>*> (v)) {
        S_msrBeatRepeatPattern elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrBeatRepeatPattern::visitStart ()";

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void msrBeatRepeatPattern::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrBeatRepeatPattern::acceptOut ()";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msrBeatRepeatPattern>*
    p =
      dynamic_cast<visitor<S_msrBeatRepeatPattern>*> (v)) {
        S_msrBeatRepeatPattern elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrBeatRepeatPattern::visitEnd ()";

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void msrBeatRepeatPattern::browseData (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrBeatRepeatPattern::browseData ()";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (fBeatRepeatPatternSegment) {
    // browse the pattern segment
    msrBrowser<msrSegment> browser (v);
    browser.browse (*fBeatRepeatPatternSegment);
  }
}

std::string msrBeatRepeatPattern::asString () const
{
  std::stringstream ss;

  ss <<

    "BeatRepeatPattern" <<
    ", line " << fInputStartLineNumber <<
    " (" <<
    mfSingularOrPlural (
      fetchMeasuresNumber (),
      "repeated measure",
      "repeated measures") <<
    ")";

  return ss.str ();
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
    os << "[NULL]" << std::endl;
  }

  return os;
}

//______________________________________________________________________________
S_msrBeatRepeatReplicas msrBeatRepeatReplicas::create (
  int                 inputLineNumber,
  const S_msrBeatRepeat& upLinkToBeatRepeat)
{
  msrBeatRepeatReplicas* obj =
    new msrBeatRepeatReplicas (
      inputLineNumber,
      upLinkToBeatRepeat);
  assert (obj != nullptr);
  return obj;
}

msrBeatRepeatReplicas::msrBeatRepeatReplicas (
  int                 inputLineNumber,
  const S_msrBeatRepeat& upLinkToBeatRepeat)
    : msrElement (inputLineNumber)
{
#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    upLinkToBeatRepeat != nullptr,
    "upLinkToBeatRepeat is null");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  fUpLinkToBeatRepeat = upLinkToBeatRepeat;
}

msrBeatRepeatReplicas::~msrBeatRepeatReplicas ()
{}

void msrBeatRepeatReplicas::setBeatRepeatReplicasSegment (
  const S_msrSegment& beatRepeatReplicasSegment)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceBeatRepeats ()) {
    std::stringstream ss;

    ss <<
      "Setting measures repeat replicas segment containing " <<
      mfSingularOrPlural (
        fetchMeasuresNumber (),
        "measure",
        "measures");

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    beatRepeatReplicasSegment != nullptr,
    "beatRepeatReplicasSegment is null");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

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
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrBeatRepeatReplicas::acceptIn ()";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msrBeatRepeatReplicas>*
    p =
      dynamic_cast<visitor<S_msrBeatRepeatReplicas>*> (v)) {
        S_msrBeatRepeatReplicas elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrBeatRepeatReplicas::visitStart ()";

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void msrBeatRepeatReplicas::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrBeatRepeatReplicas::acceptOut ()";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msrBeatRepeatReplicas>*
    p =
      dynamic_cast<visitor<S_msrBeatRepeatReplicas>*> (v)) {
        S_msrBeatRepeatReplicas elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrBeatRepeatReplicas::visitEnd ()";

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void msrBeatRepeatReplicas::browseData (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrBeatRepeatReplicas::browseData ()";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (fBeatRepeatReplicasSegment) {
  // browse the replicas segment
    msrBrowser<msrSegment> browser (v);
    browser.browse (*fBeatRepeatReplicasSegment);
  }
}

std::string msrBeatRepeatReplicas::asString () const
{
  std::stringstream ss;

  ss <<

    "BeatRepeatReplicas" <<
    ", line " << fInputStartLineNumber <<
    " (" <<
    mfSingularOrPlural (
      fetchMeasuresNumber (),
      "replicas measure",
      "replicas measures") <<
    ")";

  return ss.str ();
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
    os << "[NULL]" << std::endl;
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
  msrBeatRepeat* obj =
    new msrBeatRepeat (
      inputLineNumber,
      beatRepeatMeasuresNumber,
      beatRepeatSlashesNumber,
      upLinkToVoice);
  assert (obj != nullptr);
  return obj;
}

msrBeatRepeat::msrBeatRepeat (
  int        inputLineNumber,
  int        beatRepeatMeasuresNumber,
  int        beatRepeatSlashesNumber,
  const S_msrVoice& upLinkToVoice)
    : msrSegmentElement (inputLineNumber)
{
#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    beatRepeatMeasuresNumber > 0,
    "beatRepeatMeasuresNumber is not positive");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  fBeatRepeatMeasuresNumber = beatRepeatMeasuresNumber;

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    beatRepeatSlashesNumber > 0,
    "beatRepeatSlashesNumber is not positive");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

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
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceBeatRepeats ()) {
    std::stringstream ss;

    ss <<
      "Creating a newborn clone of measures repeat '" <<
      asString () <<
      "'";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    containingVoice != nullptr,
    "containingVoice is null");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  S_msrBeatRepeat
    newbornClone =
      msrBeatRepeat::create (
        fInputStartLineNumber,
        fBeatRepeatMeasuresNumber,
        fBeatRepeatSlashesNumber,
        containingVoice);

  return newbornClone;
}

void msrBeatRepeat::setBeatRepeatPattern (
  const S_msrBeatRepeatPattern& beatRepeatPattern)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceBeatRepeats ()) {
    std::stringstream ss;

    ss <<
      "Setting measures repeat pattern containing " <<
      mfSingularOrPlural (
        beatRepeatPattern->
          fetchMeasuresNumber (),
        "measure",
        "measures");

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    beatRepeatPattern != nullptr,
    "beatRepeatPattern is null");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  fBeatRepeatPattern = beatRepeatPattern;

  // set currentRepeat's build phase
  fCurrentBeatRepeatBuildPhaseKind =
    msrBeatRepeatBuildPhaseKind::kBeatRepeatBuildPhaseInPattern;
}

void msrBeatRepeat::setBeatRepeatReplicas (
  const S_msrBeatRepeatReplicas& beatRepeatReplicas)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceBeatRepeats ()) {
    std::stringstream ss;

    ss <<
      "Setting measures repeat replicas containing " <<
      mfSingularOrPlural (
        beatRepeatReplicas->
          fetchMeasuresNumber (),
        "measure",
        "measures");

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    beatRepeatReplicas != nullptr,
    "beatRepeatReplicas is null");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  fBeatRepeatReplicas = beatRepeatReplicas;

  // set currentRepeat's build phase
  fCurrentBeatRepeatBuildPhaseKind =
    msrBeatRepeatBuildPhaseKind::kBeatRepeatBuildPhaseInReplicas;
}

int msrBeatRepeat::fetchBeatRepeatReplicasNumber () const
{
  int patternMeasuresNumber =
    fetchBeatRepeatPatternMeasuresNumber ();

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    patternMeasuresNumber > 0,
    "patternMeasuresNumber is not positive");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  return
    fetchBeatRepeatReplicasMeasuresNumber ()
      / // JMI ???
    patternMeasuresNumber;
}

int msrBeatRepeat::fetchBeatRepeatPatternMeasuresNumber () const
{
#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    fBeatRepeatPattern != nullptr,
    "fBeatRepeatPattern is null");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  return
    fBeatRepeatPattern->
      fetchMeasuresNumber ();
}

int msrBeatRepeat::fetchBeatRepeatReplicasMeasuresNumber () const
{
#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    fBeatRepeatReplicas != nullptr,
    "fBeatRepeatReplicas is null");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  return
    fBeatRepeatReplicas->
      fetchMeasuresNumber ();
}

void msrBeatRepeat::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrBeatRepeat::acceptIn ()";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msrBeatRepeat>*
    p =
      dynamic_cast<visitor<S_msrBeatRepeat>*> (v)) {
        S_msrBeatRepeat elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrBeatRepeat::visitStart ()";

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void msrBeatRepeat::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrBeatRepeat::acceptOut ()";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msrBeatRepeat>*
    p =
      dynamic_cast<visitor<S_msrBeatRepeat>*> (v)) {
        S_msrBeatRepeat elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrBeatRepeat::visitEnd ()";

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void msrBeatRepeat::browseData (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrBeatRepeat::browseData ()";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

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
#ifdef MF_TRACE_IS_ENABLED
    if (gMsrOahGroup->getTraceMsrVisitors () || gTraceOahGroup->getTraceBeatRepeats ()) {
      gLog <<
        "% ==> visiting measures repeat replicas is inhibited";

      gWaeHandler->waeTrace (
        __FILE__, __LINE__,
        ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED
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

void msrBeatRepeat::appendMeasureElementToSegmentElement (
  const S_msrMeasureElement& elem)
{
  std::stringstream ss;

  ss <<
    "cannot append measure element " <<
    elem->asShortString () <<
    " to beat repeat " <<
    asShortString ();

  msrInternalError (
    gServiceRunData->getInputSourceName (),
    fInputStartLineNumber,
    __FILE__, __LINE__,
    ss.str ());
}

std::string msrBeatRepeatKindAsString (
  msrBeatRepeatKind beatRepeatKind)
{
  std::string result;

  switch (beatRepeatKind) {
    case msrBeatRepeatKind::kBeatRepeat_UNKNOWN_:
      result = "kBeatRepeat_UNKNOWN_";
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
  std::stringstream ss;

  ss <<

    "[BeatRepeat" <<
    ", line " << fInputStartLineNumber <<
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

  return ss.str ();
}

void msrBeatRepeat::displayBeatRepeat (
  int           inputLineNumber,
  const std::string& context)
{
  gLog <<
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
  print (gLog);
  --gIndenter;

  gLog <<
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
    ", line " << fInputStartLineNumber <<
    std::endl;

  ++gIndenter;

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceBeatRepeats ()) {
    // print the current measures repeat build phase
    const int fieldWidth = 36;

    os <<
      std::setw (fieldWidth) <<
      "fCurrentBeatRepeatBuildPhaseKind" << ": " <<
      msrBeatRepeatBuildPhaseKindAsString (
        fCurrentBeatRepeatBuildPhaseKind) <<
      std::endl;
  }
#endif // MF_TRACE_IS_ENABLED

  // print the beat repeat pattern
  if (! fBeatRepeatPattern) {
    os <<
      "fBeatRepeatPattern: [NONE]" <<
      std::endl;
  }

  else {
    os <<
      fBeatRepeatPattern;
  }

  // print the beat repeat replicas
  if (! fBeatRepeatReplicas) {
    os <<
      "fBeatRepeatReplicas: [NONE]" <<
      std::endl;
  }

  else {
    os <<
      fBeatRepeatReplicas;
  }

  --gIndenter;

  os << ']' << std::endl;
}

std::ostream& operator << (std::ostream& os, const S_msrBeatRepeat& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NULL]" << std::endl;
  }

  return os;
}


}
