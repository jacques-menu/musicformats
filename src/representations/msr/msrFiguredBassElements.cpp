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

#include "msrFiguredBassElements.h"

#include "oahOah.h"

#include "msrOah.h"

#include "msrBrowsers.h"


using namespace std;

namespace MusicFormats
{

//______________________________________________________________________________
S_msrBassFigure msrBassFigure::create (
  int                 inputLineNumber,
  S_msrPart           figureUpLinkToPart,
  msrBassFigurePrefixKind figurePrefixKind,
  int                 figureNumber,
  msrBassFigureSuffixKind figureSuffixKind)
{
  msrBassFigure* o =
    new msrBassFigure (
      inputLineNumber,
      figureUpLinkToPart,
      figurePrefixKind,
      figureNumber,
      figureSuffixKind);
  assert (o != nullptr);

  return o;
}

msrBassFigure::msrBassFigure (
  int                 inputLineNumber,
  S_msrPart           figureUpLinkToPart,
  msrBassFigurePrefixKind figurePrefixKind,
  int                 figureNumber,
  msrBassFigureSuffixKind figureSuffixKind)
    : msrElement (inputLineNumber)
{
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    figureUpLinkToPart != nullptr,
    "figureUpLinkToPart is null");

  // set figured's part upLink
  fFigureUpLinkToPart =
    figureUpLinkToPart;

  fFigurePrefixKind = figurePrefixKind;
  fFigureNumber     = figureNumber;
  fFigureSuffixKind = figureSuffixKind;

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceFiguredBass ()) {
    gLogStream <<
      "Creating bass figure '" <<
      asString () <<
      "'" <<
      endl;
  }
#endif
}

msrBassFigure::~msrBassFigure ()
{}

S_msrBassFigure msrBassFigure::createFigureNewbornClone (
  S_msrPart containingPart)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceFiguredBass ()) {
    gLogStream <<
      "Creating a newborn clone of bass figure '" <<
      asString () <<
      "'" <<
      endl;
  }
#endif

  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    containingPart != nullptr,
    "containingPart is null");

  S_msrBassFigure
    newbornClone =
      msrBassFigure::create (
        fInputLineNumber,
        containingPart,
        fFigurePrefixKind,
        fFigureNumber,
        fFigureSuffixKind);

  return newbornClone;
}

S_msrBassFigure msrBassFigure::createFigureDeepClone (
  S_msrPart containingPart)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceFiguredBass ()) {
    gLogStream <<
      "Creating a deep clone of bass figure '" <<
      asString () <<
      "'" <<
      endl;
  }
#endif

  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    containingPart != nullptr,
    "containingPart is null");

  S_msrBassFigure
    figureDeepClone =
      msrBassFigure::create (
        fInputLineNumber,
        containingPart,
        fFigurePrefixKind,
        fFigureNumber,
        fFigureSuffixKind);

  return figureDeepClone;
}

string msrBassFigure::figurePrefixKindAsString (
  msrBassFigurePrefixKind figurePrefixKind)
{
  string result;

  switch (figurePrefixKind) {
    case msrBassFigure::k_NoFigurePrefix:
      result = "k_NoFigurePrefix";
      break;
    case msrBassFigure::kDoubleFlatPrefix:
      result = "kDoubleFlatPrefix";
      break;
    case msrBassFigure::kFlatPrefix:
      result = "kFlatPrefix";
      break;
    case msrBassFigure::kFlatFlatPrefix:
      result = "kFlatFlatPrefix";
      break;
    case msrBassFigure::kNaturalPrefix:
      result = "kNaturalPrefix";
      break;
    case msrBassFigure::kSharpSharpPrefix:
      result = "kSharpSharpPrefix";
      break;
    case msrBassFigure::kSharpPrefix:
      result = "kSharpPrefix";
      break;
    case msrBassFigure::kDoubleSharpPrefix:
      result = "kDoubleSharpPrefix";
      break;
  } // switch

  return result;
}

string msrBassFigure::figureSuffixKindAsString (
  msrBassFigureSuffixKind figureSuffixKind)
{
  string result;

  switch (figureSuffixKind) {
    case msrBassFigure::k_NoFigureSuffix:
      result = "k_NoFigureSuffix";
      break;
    case msrBassFigure::kDoubleFlatSuffix:
      result = "kDoubleFlatSuffix";
      break;
    case msrBassFigure::kFlatSuffix:
      result = "kFlatSuffix";
      break;
    case msrBassFigure::kFlatFlatSuffix:
      result = "kFlatFlatSuffix";
      break;
    case msrBassFigure::kNaturalSuffix:
      result = "kNaturalSuffix";
      break;
    case msrBassFigure::kSharpSharpSuffix:
      result = "kSharpSharpSuffix";
      break;
    case msrBassFigure::kSharpSuffix:
      result = "kSharpSuffix";
      break;
    case msrBassFigure::kDoubleSharpSuffix:
      result = "kDoubleSharpSuffix sharp";
      break;
    case msrBassFigure::kSlashSuffix:
      result = "kSlashSuffix";
      break;
  } // switch

  return result;
}

void msrBassFigure::acceptIn (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrBassFigure::acceptIn ()" <<
      endl;
  }

  if (visitor<S_msrBassFigure>*
    p =
      dynamic_cast<visitor<S_msrBassFigure>*> (v)) {
        S_msrBassFigure elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrBassFigure::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrBassFigure::acceptOut (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrBassFigure::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrBassFigure>*
    p =
      dynamic_cast<visitor<S_msrBassFigure>*> (v)) {
        S_msrBassFigure elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrBassFigure::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrBassFigure::browseData (basevisitor* v)
{}

string msrBassFigure::asString () const
{
  stringstream s;

  s <<
    "BassFigure" <<
      "'" << fFigureNumber <<
    "', prefix: " <<
    figurePrefixKindAsString (
      fFigurePrefixKind) <<
    ", suffix: " <<
    figureSuffixKindAsString (
      fFigureSuffixKind) <<
    ", line " << fInputLineNumber <<
    endl;

  return s.str ();
}

void msrBassFigure::print (ostream& os) const
{
  os << asString () << endl;

/* JMI
  if (fFigureUpLinkToPart) { // JMI ???
    s <<
      ":" <<
      wholeNotesAsMsrString (
        fInputLineNumber,
        fFigureSoundingWholeNotes);
  }
*/
}

ostream& operator << (ostream& os, const S_msrBassFigure& elt)
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
S_msrFiguredBassElement msrFiguredBassElement::create (
  // no figuredBassElementUpLinkToVoice yet
  int inputLineNumber) // ,
// JMI  S_msrPart figuredBassElementUpLinkToPart)
{
  msrFiguredBassElement* o =
    new msrFiguredBassElement (
      inputLineNumber,
//    figuredBassElementUpLinkToPart,
      Rational (0, 1),           // figuredBassElementSoundingWholeNotes
      Rational (0, 1),           // figuredBassElementDisplayWholeNotes
      kFiguredBassElementParenthesesNo,
      msrTupletFactor (1, 1));
  assert (o != nullptr);

  return o;
}

S_msrFiguredBassElement msrFiguredBassElement::create (
  int             inputLineNumber,
  const Rational& figuredBassElementSoundingWholeNotes,
  const Rational& figuredBassElementDisplayWholeNotes,
  msrFiguredBassElementParenthesesKind
                  figuredBassElementParenthesesKind,
  msrTupletFactor figuredBassElementTupletFactor)
{
  msrFiguredBassElement* o =
    new msrFiguredBassElement (
      inputLineNumber,
 //     figuredBassElementUpLinkToPart,
      figuredBassElementSoundingWholeNotes,
      figuredBassElementDisplayWholeNotes,
      figuredBassElementParenthesesKind,
      figuredBassElementTupletFactor);
  assert (o != nullptr);

  return o;
}

msrFiguredBassElement::msrFiguredBassElement (
  int             inputLineNumber,
  const Rational& figuredBassElementSoundingWholeNotes,
  const Rational& figuredBassElementDisplayWholeNotes,
  msrFiguredBassElementParenthesesKind
                  figuredBassElementParenthesesKind,
  msrTupletFactor figuredBassElementTupletFactor)
    : msrMeasureElement (inputLineNumber),
      fFiguredBassElementTupletFactor (
        figuredBassElementTupletFactor)
{
  /* JMI
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    figuredBassElementUpLinkToPart != nullptr,
    "figuredBassElementUpLinkToPart is null");

  // set figuredBassElement's part upLink
  fFiguredBassElementUpLinkToPart =
    figuredBassElementUpLinkToPart;
    */

  doSetMeasureElementSoundingWholeNotes (
    figuredBassElementSoundingWholeNotes,
    "msrFiguredBassElement::msrFiguredBassElement()");

  fFiguredBassElementDisplayWholeNotes =
    figuredBassElementDisplayWholeNotes;

  fFiguredBassElementParenthesesKind =
    figuredBassElementParenthesesKind;

  // a figured bass element is considered to be at the beginning of the measure
  // until this is computed in msrMeasure::finalizeFiguredBassElementsInFiguredBassMeasure()
  fMeasureElementPositionInMeasure = Rational (0, 1);

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceFiguredBass ()) {
    gLogStream <<
      "Creating figuredBassElement " <<
      asString () <<
      endl;
  }
#endif
}

msrFiguredBassElement::~msrFiguredBassElement ()
{}

S_msrFiguredBassElement msrFiguredBassElement::createFiguredBassElementNewbornClone (
  S_msrVoice containingVoice)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceFiguredBass ()) {
    gLogStream <<
      "Creating a newborn clone of figured bass '" <<
      asShortString () <<
      "'" <<
      endl;
  }
#endif

  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    containingVoice != nullptr,
    "containingVoice is null");

  S_msrFiguredBassElement
    newbornClone =
      msrFiguredBassElement::create (
        fInputLineNumber,
 //       containingPart,
// JMI        fFiguredBassElementSoundingWholeNotes,
        fMeasureElementSoundingWholeNotes,
        fFiguredBassElementDisplayWholeNotes,
        fFiguredBassElementParenthesesKind,
        fFiguredBassElementTupletFactor);

  return newbornClone;
}

S_msrFiguredBassElement msrFiguredBassElement::createFiguredBassElementDeepClone ()
 // S_msrPart containingPart)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceFiguredBass ()) {
    gLogStream <<
      "Creating a deep clone of figuredBassElement '" <<
      asString () <<
      "'" <<
      endl;
  }
#endif

/* JMi
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    containingPart != nullptr,
    "containingPart is null");
    */

  S_msrFiguredBassElement
    figuredBassElementDeepClone =
      msrFiguredBassElement::create (
        fInputLineNumber,
   //     containingPart,
// JMI        fFiguredBassElementSoundingWholeNotes,
        fMeasureElementSoundingWholeNotes,
        fFiguredBassElementDisplayWholeNotes,
        fFiguredBassElementParenthesesKind,
        fFiguredBassElementTupletFactor);

  return figuredBassElementDeepClone;
}

void msrFiguredBassElement::setFiguredBassElementPositionInMeasure (
  const S_msrMeasure measure,
  const Rational&    positionInMeasure,
  const string&      context)
{
  // set the figured bass position in measure

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTracePositionsInMeasures ()) {
    gLogStream <<
      "Setting figured bass element's position in measure of " << asString () <<
      " to " <<
      positionInMeasure <<
      " (was " <<
      fMeasureElementPositionInMeasure <<
      ") in measure " <<
      measure->asShortString () <<
      " (fMeasureElementMeasureNumber: " <<
      fMeasureElementMeasureNumber <<
      "), context: \"" <<
      context <<
      "\"" <<
      endl;
  }
#endif

  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    positionInMeasure != msrMoment::K_NO_POSITION,
    "positionInMeasure == msrMoment::K_NO_POSITION");

  // set figured bass element's position in measure
  fMeasureElementPositionInMeasure = positionInMeasure;

  // compute figured bass's position in voice
  if (false) { // JMI CAFE v0.9.66
  Rational
    positionFromBeginningOfVoice =
      measure->
        getMeasurePositionFromBeginningOfVoice ()
        +
      positionInMeasure;

  // set figured bass's position in voice
  setMeasureElementPositionFromBeginningOfVoice (
    positionFromBeginningOfVoice,
    context);
}

  // update current position in voice
  S_msrVoice
    voice =
      measure->
        fetchMeasureUpLinkToVoice ();

  voice->
    incrementCurrentPositionFromBeginningOfVoice (
      fFiguredBassElementUpLinkToNote->
        getMeasureElementSoundingWholeNotes ());
}

void msrFiguredBassElement::appendFigureToFiguredBass (
  S_msrBassFigure bassFigure)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceFiguredBass ()) {
    gLogStream <<
      "Appending bass figure '" << bassFigure->asString () <<
      "' to figured-bass element '" <<
      asString () <<
      "'" <<
      endl;
  }
#endif

  fFiguredBassElementFiguresList.push_back (bassFigure);
}

void msrFiguredBassElement::acceptIn (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrFiguredBassElement::acceptIn ()" <<
      endl;
  }

  if (visitor<S_msrFiguredBassElement>*
    p =
      dynamic_cast<visitor<S_msrFiguredBassElement>*> (v)) {
        S_msrFiguredBassElement elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrFiguredBassElement::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrFiguredBassElement::acceptOut (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrFiguredBassElement::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrFiguredBassElement>*
    p =
      dynamic_cast<visitor<S_msrFiguredBassElement>*> (v)) {
        S_msrFiguredBassElement elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrFiguredBassElement::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrFiguredBassElement::browseData (basevisitor* v)
{
  for (
    list<S_msrBassFigure>::const_iterator i =
      fFiguredBassElementFiguresList.begin ();
    i != fFiguredBassElementFiguresList.end ();
    ++i
  ) {
    // browse the bass figure
    msrBrowser<msrBassFigure> browser (v);
    browser.browse (*(*i));
  } // for
}

string msrFiguredBassElement::figuredBassElementParenthesesKindAsString (
  msrFiguredBassElementParenthesesKind figuredBassElementParenthesesKind)
{
  string result;

  switch (figuredBassElementParenthesesKind) {
    case msrFiguredBassElement::kFiguredBassElementParenthesesYes:
      result = "figuredBassElementParenthesesYes";
      break;
    case msrFiguredBassElement::kFiguredBassElementParenthesesNo:
      result = "figuredBassElementParenthesesNo";
      break;
  } // switch

  return result;
}

string msrFiguredBassElement::asString () const
{
  stringstream s;

  s <<
    "[Figured-bass element" <<
    ", measureElementSoundingWholeNotes: " <<
    wholeNotesAsMsrString (
      fInputLineNumber,
      fMeasureElementSoundingWholeNotes) <<
    ", figuredBassElementDisplayWholeNotes: " <<
    wholeNotesAsMsrString (
      fInputLineNumber,
      fFiguredBassElementDisplayWholeNotes) <<

    ", figuredBassElementParenthesesKind: " <<
    figuredBassElementParenthesesKindAsString (
      fFiguredBassElementParenthesesKind) <<

    ", figuredBassElementTupletFactor: " <<
    fFiguredBassElementTupletFactor.asString ();

  if (fFiguredBassElementFiguresList.size ()) {
    s << ", figuredBassElementFiguresList: [";

    list<S_msrBassFigure>::const_iterator
      iBegin = fFiguredBassElementFiguresList.begin (),
      iEnd   = fFiguredBassElementFiguresList.end (),
      i      = iBegin;

    for ( ; ; ) {
      s << (*i)->asString ();
      if (++i == iEnd) break;
      s << ' ';
    } // for
  s << "]";
  }

  // print the figured bass position in measure
  s <<
    ", positionInMeasure: " << fMeasureElementPositionInMeasure;

/* JMI
  if (fFiguredBassElementUpLinkToPart) { // JMI ???
    s <<
      ":" <<
      wholeNotesAsMsrString (
        fInputLineNumber,
        fFiguredBassElementSoundingWholeNotes);
  }
*/

  // print the figured bass position in voice
  s <<
    ", positionFromBeginningOfVoice: " << fMeasureElementPositionFromBeginningOfVoice;

  s <<
    ", line " << fInputLineNumber <<
    "]";

  return s.str ();
}

void msrFiguredBassElement::print (ostream& os) const
{
  os <<
    "[FiguredBassElement" <<
    ", line " << fInputLineNumber <<
    endl;

  ++gIndenter;

  const int fieldWidth = 33;

  os <<
    setw (fieldWidth) <<
    "figuredBassElementUpLinkToNote" << " : ";
  if (fFiguredBassElementUpLinkToNote) {
    os << fFiguredBassElementUpLinkToNote->asString ();
  }
  else {
    os << "[NONE]";
  }
  os << endl;

  os <<
    setw (fieldWidth) <<
    "figuredBassElementUpLinkToVoice" << " : ";
  if (fFiguredBassElementUpLinkToVoice) {
    os << fFiguredBassElementUpLinkToVoice->asString ();
  }
  else {
    os << "[NONE]";
  }
  os << endl;

  os << left <<
    setw (fieldWidth) <<
    "fMeasureElementSoundingWholeNotes" << " : " <<
    fMeasureElementSoundingWholeNotes <<
    endl <<

    setw (fieldWidth) <<
    "fFiguredBassElementDisplayWholeNotes" << " : " <<
    fFiguredBassElementDisplayWholeNotes <<
    endl <<

    setw (fieldWidth) <<
    "fFiguredBassElementParenthesesKind" << " : " <<
    figuredBassElementParenthesesKindAsString (
      fFiguredBassElementParenthesesKind) <<
    endl <<

    setw (fieldWidth) <<
    "fFiguredBassElementTupletFactor" << " : " <<
    fFiguredBassElementTupletFactor.asString () <<
    endl;

  if (fFiguredBassElementFiguresList.size ()) {
    ++gIndenter;

    list<S_msrBassFigure>::const_iterator
      iBegin = fFiguredBassElementFiguresList.begin (),
      iEnd   = fFiguredBassElementFiguresList.end (),
      i      = iBegin;

    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
 // JMI     os << endl;
    } // for

    --gIndenter;
  }

  // print the figured bass position in measure
  os <<
    setw (fieldWidth) <<
    "positionInMeasure" << " : " << fMeasureElementPositionInMeasure <<
    endl;

  // print the figured bass position in voice
  os <<
    setw (fieldWidth) <<
    "positionFromBeginningOfVoice" << " : " << fMeasureElementPositionFromBeginningOfVoice <<
    endl;

  --gIndenter;

  os << ']' << endl;
}

ostream& operator << (ostream& os, const S_msrFiguredBassElement& elt)
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
