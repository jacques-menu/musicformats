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

#include "msrMeasures.h"

#include "msrFiguredBasses.h"

#include "oahOah.h"

#include "msrOah.h"

#include "msrBrowsers.h"


using namespace std;

namespace MusicFormats
{

//______________________________________________________________________________
S_msrBassFigure msrBassFigure::create (
  int                     inputLineNumber,
  S_msrPart               figureUpLinkToPart,
  msrBassFigurePrefixKind figurePrefixKind,
  int                     figureNumber,
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
  int                     inputLineNumber,
  S_msrPart               figureUpLinkToPart,
  msrBassFigurePrefixKind figurePrefixKind,
  int                     figureNumber,
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

string msrBassFigurePrefixKindAsString (
  msrBassFigurePrefixKind figurePrefixKind)
{
  string result;

  switch (figurePrefixKind) {
    case msrBassFigurePrefixKind::k_NoBassFigurePrefix:
      result = "k_NoBassFigurePrefix";
      break;
    case msrBassFigurePrefixKind::kPrefixDoubleFlat:
      result = "kPrefixDoubleFlat";
      break;
    case msrBassFigurePrefixKind::kPrefixFlat:
      result = "kPrefixFlat";
      break;
    case msrBassFigurePrefixKind::kPrefixFlatFlat:
      result = "kPrefixFlatFlat";
      break;
    case msrBassFigurePrefixKind::kPrefixNatural:
      result = "kPrefixNatural";
      break;
    case msrBassFigurePrefixKind::kPrefixSharpSharp:
      result = "kPrefixSharpSharp";
      break;
    case msrBassFigurePrefixKind::kPrefixSharp:
      result = "kPrefixSharp";
      break;
    case msrBassFigurePrefixKind::kPrefixDoubleSharp:
      result = "kPrefixDoubleSharp";
      break;
  } // switch

  return result;
}

ostream& operator << (ostream& os, const msrBassFigurePrefixKind& elt)
{
  os << msrBassFigurePrefixKindAsString (elt);
  return os;
}

string msrBassFigureSuffixKindAsString (
  msrBassFigureSuffixKind figureSuffixKind)
{
  string result;

  switch (figureSuffixKind) {
    case msrBassFigureSuffixKind::k_NoBassFigureSuffix:
      result = "k_NoBassFigureSuffix";
      break;
    case msrBassFigureSuffixKind::kSuffixDoubleFlat:
      result = "kSuffixDoubleFlat";
      break;
    case msrBassFigureSuffixKind::kSuffixFlat:
      result = "kSuffixFlat";
      break;
    case msrBassFigureSuffixKind::kSuffixFlatFlat:
      result = "kSuffixFlatFlat";
      break;
    case msrBassFigureSuffixKind::kSuffixNatural:
      result = "kSuffixNatural";
      break;
    case msrBassFigureSuffixKind::kSuffixSharpSharp:
      result = "kSuffixSharpSharp";
      break;
    case msrBassFigureSuffixKind::kSuffixSharp:
      result = "kSuffixSharp";
      break;
    case msrBassFigureSuffixKind::kSuffixDoubleSharp:
      result = "kSuffixDoubleSharp sharp";
      break;
    case msrBassFigureSuffixKind::kSuffixSlash:
      result = "kSuffixSlash";
      break;
  } // switch

  return result;
}

ostream& operator << (ostream& os, const msrBassFigureSuffixKind& elt)
{
  os << msrBassFigureSuffixKindAsString (elt);
  return os;
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
    "[BassFigure" <<
    " '" << fFigureNumber <<
    "', prefix: " <<
    msrBassFigurePrefixKindAsString (
      fFigurePrefixKind) <<
    ", suffix: " <<
    msrBassFigureSuffixKindAsString (
      fFigureSuffixKind) <<
    ", line " << fInputLineNumber <<
    ']';

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
S_msrFiguredBass msrFiguredBass::create (
  int          inputLineNumber,
  S_msrMeasure upLinkToMeasure)
{
  msrFiguredBass* o =
    new msrFiguredBass (
      inputLineNumber,
      upLinkToMeasure,
      Rational (0, 1),           // figuredBassSoundingWholeNotes
      Rational (0, 1),           // figuredBassDisplayWholeNotes
      msrFiguredBassParenthesesKind::kFiguredBassParenthesesNo,
      msrTupletFactor (1, 1));
  assert (o != nullptr);

  return o;
}

S_msrFiguredBass msrFiguredBass::create (
  int             inputLineNumber,
  S_msrMeasure    upLinkToMeasure,
  const Rational& figuredBassSoundingWholeNotes,
  const Rational& figuredBassDisplayWholeNotes,
  msrFiguredBassParenthesesKind
                  figuredBassParenthesesKind,
  msrTupletFactor figuredBassTupletFactor)
{
  msrFiguredBass* o =
    new msrFiguredBass (
      inputLineNumber,
      upLinkToMeasure,
      figuredBassSoundingWholeNotes,
      figuredBassDisplayWholeNotes,
      figuredBassParenthesesKind,
      figuredBassTupletFactor);
  assert (o != nullptr);

  return o;
}

msrFiguredBass::msrFiguredBass (
  int             inputLineNumber,
  S_msrMeasure    upLinkToMeasure,
  const Rational& figuredBassSoundingWholeNotes,
  const Rational& figuredBassDisplayWholeNotes,
  msrFiguredBassParenthesesKind
                  figuredBassParenthesesKind,
  msrTupletFactor figuredBassTupletFactor)
    : msrMeasureElement (
        inputLineNumber,
        upLinkToMeasure),
      fFiguredBassTupletFactor (
        figuredBassTupletFactor)
{
  /* JMI
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    figuredBassUpLinkToPart != nullptr,
    "figuredBassUpLinkToPart is null");

  // set figuredBass's part upLink
  fFiguredBassUpLinkToPart =
    figuredBassUpLinkToPart;
    */

  doSetMeasureElementSoundingWholeNotes (
    figuredBassSoundingWholeNotes,
    "msrFiguredBass::msrFiguredBass()");

  fFiguredBassDisplayWholeNotes =
    figuredBassDisplayWholeNotes;

  fFiguredBassParenthesesKind =
    figuredBassParenthesesKind;

  // a figured bass element is considered to be at the beginning of the measure
  // until this is computed in msrMeasure::finalizeFiguredBassesInFiguredBassMeasure()
  fMeasureElementMeasurePosition = Rational (0, 1);

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceFiguredBass ()) {
    gLogStream <<
      "Creating figuredBass " <<
      asString () <<
      endl;
  }
#endif
}

msrFiguredBass::~msrFiguredBass ()
{}

S_msrFiguredBass msrFiguredBass::createFiguredBassNewbornClone (
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

  S_msrFiguredBass
    newbornClone =
      msrFiguredBass::create (
        fInputLineNumber,
        nullptr, // will be set when figured bass is appended to a measure JMI v0.9.66 PIM
        fMeasureElementSoundingWholeNotes,
        fFiguredBassDisplayWholeNotes,
        fFiguredBassParenthesesKind,
        fFiguredBassTupletFactor);

  return newbornClone;
}

S_msrFiguredBass msrFiguredBass::createFiguredBassDeepClone ()
 // S_msrPart containingPart)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceFiguredBass ()) {
    gLogStream <<
      "Creating a deep clone of figuredBass '" <<
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

  S_msrFiguredBass
    figuredBassDeepClone =
      msrFiguredBass::create (
        fInputLineNumber,
        nullptr, // will be set when figured bass is appended to a measure JMI v0.9.66 PIM
        fMeasureElementSoundingWholeNotes,
        fFiguredBassDisplayWholeNotes,
        fFiguredBassParenthesesKind,
        fFiguredBassTupletFactor);

  return figuredBassDeepClone;
}

void msrFiguredBass::setFiguredBassMeasurePosition (
  const S_msrMeasure measure,
  const Rational&    measurePosition,
  const string&      context)
{
  // set the figured bass position in measure

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasurePositions ()) {
    gLogStream <<
      "Setting figured bass element's position in measure of " << asString () <<
      " to " <<
      measurePosition <<
      " (was " <<
      fMeasureElementMeasurePosition <<
      ") in measure " <<
      measure->asShortString () <<
      " (measureElementMeasureNumber: " <<
      fetchMeasureElementMeasureNumber () <<
      "), context: \"" <<
      context <<
      "\"" <<
      endl;
  }
#endif

  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    measurePosition != msrMoment::K_NO_POSITION,
    "measurePosition == msrMoment::K_NO_POSITION");

  // set figured bass element's position in measure
  fMeasureElementMeasurePosition = measurePosition;

  // compute figured bass's position in voice
  if (false) { // JMI CAFE v0.9.66
  Rational
    voicePosition =
      measure->
        getMeasureVoicePosition ()
        +
      measurePosition;

  // set figured bass's position in voice
  setMeasureElementVoicePosition (
    voicePosition,
    context);
}

  // update current position in voice
  S_msrVoice
    voice =
      measure->
        fetchMeasureUpLinkToVoice ();

  voice->
    incrementCurrentVoicePosition (
      fFiguredBassUpLinkToNote->
        getMeasureElementSoundingWholeNotes ());
}

void msrFiguredBass::appendFigureToFiguredBass (
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

  fFiguredBassFiguresList.push_back (bassFigure);
}

void msrFiguredBass::acceptIn (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrFiguredBass::acceptIn ()" <<
      endl;
  }

  if (visitor<S_msrFiguredBass>*
    p =
      dynamic_cast<visitor<S_msrFiguredBass>*> (v)) {
        S_msrFiguredBass elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrFiguredBass::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrFiguredBass::acceptOut (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrFiguredBass::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrFiguredBass>*
    p =
      dynamic_cast<visitor<S_msrFiguredBass>*> (v)) {
        S_msrFiguredBass elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrFiguredBass::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrFiguredBass::browseData (basevisitor* v)
{
  for (S_msrBassFigure bassFigure : fFiguredBassFiguresList) {
    // browse the bass figure
    msrBrowser<msrBassFigure> browser (v);
    browser.browse (*(bassFigure));
  } // for
}

string msrFiguredBassParenthesesKindAsString (
  msrFiguredBassParenthesesKind figuredBassParenthesesKind)
{
  string result;

  switch (figuredBassParenthesesKind) {
    case msrFiguredBassParenthesesKind::kFiguredBassParenthesesYes:
      result = "kFiguredBassParenthesesYes";
      break;
    case msrFiguredBassParenthesesKind::kFiguredBassParenthesesNo:
      result = "kFiguredBassParenthesesNo";
      break;
  } // switch

  return result;
}

string msrFiguredBass::asString () const
{
  stringstream s;

  s <<
    "[FiguredBass" <<
    ", fMeasureElementSoundingWholeNotes: " <<
    wholeNotesAsMsrString (
      fInputLineNumber,
      fMeasureElementSoundingWholeNotes) <<
    ", fFiguredBassDisplayWholeNotes: " <<
    wholeNotesAsMsrString (
      fInputLineNumber,
      fFiguredBassDisplayWholeNotes) <<

    ", fFiguredBassParenthesesKind: " <<
    msrFiguredBassParenthesesKindAsString (
      fFiguredBassParenthesesKind) <<

    ", fFiguredBassTupletFactor: " <<
    fFiguredBassTupletFactor.asString ();

  if (fFiguredBassFiguresList.size ()) {
    s << ", fFiguredBassFiguresList: [";

    list<S_msrBassFigure>::const_iterator
      iBegin = fFiguredBassFiguresList.begin (),
      iEnd   = fFiguredBassFiguresList.end (),
      i      = iBegin;

    for ( ; ; ) {
      s << (*i)->asString ();
      if (++i == iEnd) break;
      s << ' ';
    } // for
  s << ']';
  }

  // print the figured bass position in measure
  s <<
    ", measurePosition: " << fMeasureElementMeasurePosition;

/* JMI
  if (fFiguredBassUpLinkToPart) { // JMI ???
    s <<
      ":" <<
      wholeNotesAsMsrString (
        fInputLineNumber,
        fFiguredBassSoundingWholeNotes);
  }
*/

  // print the figured bass position in voice
//   s <<
//     ", voicePosition: " << fMeasureElementVoicePosition;

  s <<
    ", line " << fInputLineNumber <<
    ']';

  return s.str ();
}

void msrFiguredBass::print (ostream& os) const
{
  os <<
    "[FiguredBass" <<
    ", line " << fInputLineNumber <<
    endl;

  ++gIndenter;

  const int fieldWidth = 36;

  os <<
    setw (fieldWidth) <<
    "fFiguredBassUpLinkToNote" << " : ";
  if (fFiguredBassUpLinkToNote) {
    os << fFiguredBassUpLinkToNote->asString ();
  }
  else {
    os << "[NONE]";
  }
  os << endl;

  os <<
    setw (fieldWidth) <<
    "fFiguredBassUpLinkToVoice" << " : ";
  if (fFiguredBassUpLinkToVoice) {
    os << fFiguredBassUpLinkToVoice->asString ();
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
    "fFiguredBassDisplayWholeNotes" << " : " <<
    fFiguredBassDisplayWholeNotes <<
    endl <<

    setw (fieldWidth) <<
    "fFiguredBassParenthesesKind" << " : " <<
    msrFiguredBassParenthesesKindAsString (
      fFiguredBassParenthesesKind) <<
    endl <<

    setw (fieldWidth) <<
    "fFiguredBassTupletFactor" << " : " <<
    fFiguredBassTupletFactor.asString () <<
    endl;

  os << left <<
    setw (fieldWidth) <<
    "fFiguredBassFiguresList" << " : ";
  if (fFiguredBassFiguresList.size ()) {
    os << endl;
    ++gIndenter;

    for (S_msrBassFigure bassFigure : fFiguredBassFiguresList) {
      os << bassFigure;
    } // for

    --gIndenter;
  }
  else {
    os << "[EMPTY]" << endl;
  }

  // print the figured bass position in measure
  os <<
    setw (fieldWidth) <<
    "fMeasureElementMeasurePosition" << " : " << fMeasureElementMeasurePosition <<
    endl;

//   // print the figured bass position in voice
//   os <<
//     setw (fieldWidth) <<
//     "fMeasureElementVoicePosition" << " : " << fMeasureElementVoicePosition <<
//     endl;

  --gIndenter;

  os << ']' << endl;
}

ostream& operator << (ostream& os, const S_msrFiguredBass& elt)
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
