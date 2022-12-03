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

#include "msrMeasureConstants.h"

#include "msrFiguredBasses.h"

#include "oahOah.h"

#include "msrOah.h"

#include "msrBrowsers.h"


namespace MusicFormats
{

//______________________________________________________________________________
std::string msrFiguredBassParenthesesKindAsString (
  msrFiguredBassParenthesesKind figuredBassParenthesesKind)
{
  std::string result;

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

std::ostream& operator << (std::ostream& os, const msrFiguredBassParenthesesKind& elt)
{
  os << msrFiguredBassParenthesesKindAsString (elt);
  return os;
}

std::string msrBassFigurePrefixKindAsString (
  msrBassFigurePrefixKind figurePrefixKind)
{
  std::string result;

  switch (figurePrefixKind) {
    case msrBassFigurePrefixKind::kBassFigurePrefix_NO_:
      result = "kBassFigurePrefix_NO_";
      break;
    case msrBassFigurePrefixKind::kBassFigurePrefixDoubleFlat:
      result = "kBassFigurePrefixDoubleFlat";
      break;
    case msrBassFigurePrefixKind::kBassFigurePrefixFlat:
      result = "kBassFigurePrefixFlat";
      break;
    case msrBassFigurePrefixKind::kBassFigurePrefixFlatFlat:
      result = "kBassFigurePrefixFlatFlat";
      break;
    case msrBassFigurePrefixKind::kBassFigurePrefixNatural:
      result = "kBassFigurePrefixNatural";
      break;
    case msrBassFigurePrefixKind::kBassFigurePrefixSharpSharp:
      result = "kBassFigurePrefixSharpSharp";
      break;
    case msrBassFigurePrefixKind::kBassFigurePrefixSharp:
      result = "kBassFigurePrefixSharp";
      break;
    case msrBassFigurePrefixKind::kBassFigurePrefixDoubleSharp:
      result = "kBassFigurePrefixDoubleSharp";
      break;
  } // switch

  return result;
}

std::ostream& operator << (std::ostream& os, const msrBassFigurePrefixKind& elt)
{
  os << msrBassFigurePrefixKindAsString (elt);
  return os;
}

std::string msrBassFigureSuffixKindAsString (
  msrBassFigureSuffixKind figureSuffixKind)
{
  std::string result;

  switch (figureSuffixKind) {
    case msrBassFigureSuffixKind::kBassFigureSuffix_NO_:
      result = "kBassFigureSuffix_NO_";
      break;
    case msrBassFigureSuffixKind::kBassFigureSuffixDoubleFlat:
      result = "kBassFigureSuffixDoubleFlat";
      break;
    case msrBassFigureSuffixKind::kBassFigureSuffixFlat:
      result = "kBassFigureSuffixFlat";
      break;
    case msrBassFigureSuffixKind::kBassFigureSuffixFlatFlat:
      result = "kBassFigureSuffixFlatFlat";
      break;
    case msrBassFigureSuffixKind::kBassFigureSuffixNatural:
      result = "kBassFigureSuffixNatural";
      break;
    case msrBassFigureSuffixKind::kBassFigureSuffixSharpSharp:
      result = "kBassFigureSuffixSharpSharp";
      break;
    case msrBassFigureSuffixKind::kBassFigureSuffixSharp:
      result = "kBassFigureSuffixSharp";
      break;
    case msrBassFigureSuffixKind::kBassFigureSuffixDoubleSharp:
      result = "kBassFigureSuffixDoubleSharp sharp";
      break;
    case msrBassFigureSuffixKind::kBassFigureSuffixSlash:
      result = "kBassFigureSuffixSlash";
      break;
  } // switch

  return result;
}

std::ostream& operator << (std::ostream& os, const msrBassFigureSuffixKind& elt)
{
  os << msrBassFigureSuffixKindAsString (elt);
  return os;
}

//______________________________________________________________________________
S_msrBassFigure msrBassFigure::create (
  int                     inputLineNumber,
  const S_msrPart&        figureUpLinkToPart,
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
  const S_msrPart&        figureUpLinkToPart,
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
      std::endl;
  }
#endif
}

msrBassFigure::~msrBassFigure ()
{}

S_msrBassFigure msrBassFigure::createFigureNewbornClone (
  const S_msrPart& containingPart)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceFiguredBass ()) {
    gLogStream <<
      "Creating a newborn clone of bass figure '" <<
      asString () <<
      "'" <<
      std::endl;
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
  const S_msrPart& containingPart)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceFiguredBass ()) {
    gLogStream <<
      "Creating a deep clone of bass figure '" <<
      asString () <<
      "'" <<
      std::endl;
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

void msrBassFigure::acceptIn (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrBassFigure::acceptIn ()" <<
      std::endl;
  }

  if (visitor<S_msrBassFigure>*
    p =
      dynamic_cast<visitor<S_msrBassFigure>*> (v)) {
        S_msrBassFigure elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrBassFigure::visitStart ()" <<
            std::endl;
        }
        p->visitStart (elem);
  }
}

void msrBassFigure::acceptOut (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrBassFigure::acceptOut ()" <<
      std::endl;
  }

  if (visitor<S_msrBassFigure>*
    p =
      dynamic_cast<visitor<S_msrBassFigure>*> (v)) {
        S_msrBassFigure elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrBassFigure::visitEnd ()" <<
            std::endl;
        }
        p->visitEnd (elem);
  }
}

void msrBassFigure::browseData (basevisitor* v)
{}

std::string msrBassFigure::asString () const
{
  std::stringstream s;

  s <<
    "[BassFigure" <<
    " '" << fFigureNumber <<
    "', prefix: " <<
    fFigurePrefixKind <<
    ", suffix: " <<
    fFigureSuffixKind <<
    ", line " << fInputLineNumber <<
    ']';

  return s.str ();
}

void msrBassFigure::print (std::ostream& os) const
{
  os << asString () << std::endl;

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

std::ostream& operator << (std::ostream& os, const S_msrBassFigure& elt)
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
S_msrFiguredBass msrFiguredBass::create (
  int                 inputLineNumber,
  const S_msrMeasure& upLinkToMeasure)
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
  int                 inputLineNumber)
{
  return
     msrFiguredBass::create (
      inputLineNumber,
      gGlobalNullMeasureSmartPointer); // set later in setMeasureElementUpLinkToMeasure()
}

S_msrFiguredBass msrFiguredBass::create (
  int                    inputLineNumber,
  const S_msrMeasure&    upLinkToMeasure,
  const Rational&        figuredBassSoundingWholeNotes,
  const Rational&        figuredBassDisplayWholeNotes,
  msrFiguredBassParenthesesKind
                         figuredBassParenthesesKind,
  const msrTupletFactor& figuredBassTupletFactor)
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

S_msrFiguredBass msrFiguredBass::create (
  int                    inputLineNumber,
  const Rational&        figuredBassSoundingWholeNotes,
  const Rational&        figuredBassDisplayWholeNotes,
  msrFiguredBassParenthesesKind
                         figuredBassParenthesesKind,
  const msrTupletFactor& figuredBassTupletFactor)
{
  return
    msrFiguredBass::create (
      inputLineNumber,
      gGlobalNullMeasureSmartPointer, // set later in setMeasureElementUpLinkToMeasure()
      figuredBassSoundingWholeNotes,
      figuredBassDisplayWholeNotes,
      figuredBassParenthesesKind,
      figuredBassTupletFactor);
}

msrFiguredBass::msrFiguredBass (
  int                    inputLineNumber,
  const S_msrMeasure&    upLinkToMeasure,
  const Rational&        figuredBassSoundingWholeNotes,
  const Rational&        figuredBassDisplayWholeNotes,
  msrFiguredBassParenthesesKind
                         figuredBassParenthesesKind,
  const msrTupletFactor& figuredBassTupletFactor)
    : msrMeasureElement (
        inputLineNumber),
      fFiguredBassTupletFactor (
        figuredBassTupletFactor)
{
//   // sanity check JMI v0.9.66
//   mfAssert (
//     __FILE__, __LINE__,
//     upLinkToMeasure != nullptr,
//     "upLinkToMeasure is null");

  fFiguredBassUpLinkToMeasure = upLinkToMeasure;

  /* JMI
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
      std::endl;
  }
#endif
}

msrFiguredBass::~msrFiguredBass ()
{}

S_msrFiguredBass msrFiguredBass::createFiguredBassNewbornClone (
  const S_msrVoice& containingVoice)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceFiguredBass ()) {
    gLogStream <<
      "Creating a newborn clone of figured bass '" <<
      asShortString () <<
      "'" <<
      std::endl;
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
	      gGlobalNullMeasureSmartPointer, // set later in setMeasureElementUpLinkToMeasure()
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
      std::endl;
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
	      gGlobalNullMeasureSmartPointer, // set later in setMeasureElementUpLinkToMeasure()
        fMeasureElementSoundingWholeNotes,
        fFiguredBassDisplayWholeNotes,
        fFiguredBassParenthesesKind,
        fFiguredBassTupletFactor);

  return figuredBassDeepClone;
}

void msrFiguredBass::setFiguredBassUpLinkToNote (
  const S_msrNote& note)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceFiguredBass ()) {
    gLogStream <<
      "==> Setting the uplink to note of figured bass " <<
      asString () <<
      " to note " << note->asString () <<
      std::endl;
  }
#endif
  fFiguredBassUpLinkToNote = note;
}

void msrFiguredBass::setFiguredBassUpLinkToMeasure (
  const S_msrMeasure& measure)
{
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    measure != nullptr,
    "measure is null");

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceFiguredBass ()) {
    ++gIndenter;

    gLogStream <<
      "==> Setting the uplink to measure of figured bass " <<
      asString () <<
      " to measure " << measure->asString () <<
      "' in measure '" <<
      measure->asString () <<
      std::endl;

    --gIndenter;
  }
#endif

  fFiguredBassUpLinkToMeasure = measure;
}

void msrFiguredBass::appendFigureToFiguredBass (
  const S_msrBassFigure& bassFigure)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceFiguredBass ()) {
    gLogStream <<
      "Appending bass figure " << bassFigure->asString () <<
      " to figured-bass element '" <<
      asString () <<
      "'" <<
      std::endl;
  }
#endif

  fFiguredBassFiguresList.push_back (bassFigure);
}

void msrFiguredBass::acceptIn (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrFiguredBass::acceptIn ()" <<
      std::endl;
  }

  if (visitor<S_msrFiguredBass>*
    p =
      dynamic_cast<visitor<S_msrFiguredBass>*> (v)) {
        S_msrFiguredBass elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrFiguredBass::visitStart ()" <<
            std::endl;
        }
        p->visitStart (elem);
  }
}

void msrFiguredBass::acceptOut (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrFiguredBass::acceptOut ()" <<
      std::endl;
  }

  if (visitor<S_msrFiguredBass>*
    p =
      dynamic_cast<visitor<S_msrFiguredBass>*> (v)) {
        S_msrFiguredBass elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrFiguredBass::visitEnd ()" <<
            std::endl;
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

std::string msrFiguredBass::asString () const
{
  std::stringstream s;

  s <<
    "[FiguredBass" <<
    ", fMeasureElementMeasurePosition: " <<
    fMeasureElementMeasurePosition <<
    ", fMeasureElementSoundingWholeNotes: " <<
    wholeNotesAsMsrString (
      fInputLineNumber,
      fMeasureElementSoundingWholeNotes) <<
    ", fFiguredBassDisplayWholeNotes: " <<
    wholeNotesAsMsrString (
      fInputLineNumber,
      fFiguredBassDisplayWholeNotes) <<

    ", fFiguredBassParenthesesKind: " <<
    fFiguredBassParenthesesKind <<

    ", fFiguredBassTupletFactor: " <<
    fFiguredBassTupletFactor.asString ();

  if (fFiguredBassFiguresList.size ()) {
    s << ", fFiguredBassFiguresList: [";

    std::list<S_msrBassFigure>::const_iterator
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

  // print the figured bass measure position
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

  // print the figured bass voice position
//   s <<
//     ", voicePosition: " << fMeasureElementVoicePosition;

  s <<
    ", line " << fInputLineNumber <<
    ']';

  return s.str ();
}

void msrFiguredBass::print (std::ostream& os) const
{
  os <<
    "[FiguredBass" <<
    ", line " << fInputLineNumber <<
    std::endl;

  ++gIndenter;

  const int fieldWidth = 36;

  os << std::left <<
    std::setw (fieldWidth) <<
    "fMeasureElementMeasurePosition" << " : " <<
    fMeasureElementMeasurePosition <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fMeasureElementSoundingWholeNotes" << " : " <<
    fMeasureElementSoundingWholeNotes <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fFiguredBassDisplayWholeNotes" << " : " <<
    fFiguredBassDisplayWholeNotes <<
    std::endl;

  os <<
    std::setw (fieldWidth) <<
    "fFiguredBassUpLinkToNote" << " : ";
  if (fFiguredBassUpLinkToNote) {
    os << fFiguredBassUpLinkToNote->asString ();
  }
  else {
    os << "[NONE]";
  }
  os << std::endl;

  os <<
    std::setw (fieldWidth) <<
    "fFiguredBassUpLinkToVoice" << " : ";
  if (fFiguredBassUpLinkToVoice) {
    os << fFiguredBassUpLinkToVoice->asString ();
  }
  else {
    os << "[NONE]";
  }
  os << std::endl;

  os << std::left <<
    std::setw (fieldWidth) <<
    "fFiguredBassParenthesesKind" << " : " <<
    fFiguredBassParenthesesKind <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fFiguredBassTupletFactor" << " : " <<
    fFiguredBassTupletFactor.asString () <<
    std::endl;

  os << std::left <<
    std::setw (fieldWidth) <<
    "fFiguredBassFiguresList" << " : ";
  if (fFiguredBassFiguresList.size ()) {
    os << std::endl;
    ++gIndenter;

    for (S_msrBassFigure bassFigure : fFiguredBassFiguresList) {
      os << bassFigure;
    } // for

    --gIndenter;
  }
  else {
    os << "[EMPTY]" << std::endl;
  }

  // print the figured bass measure position
  os <<
    std::setw (fieldWidth) <<
    "fMeasureElementMeasurePosition" << " : " << fMeasureElementMeasurePosition <<
    std::endl;

//   // print the figured bass voice position
//   os <<
//     std::setw (fieldWidth) <<
//     "fMeasureElementVoicePosition" << " : " << fMeasureElementVoicePosition <<
//     std::endl;

  --gIndenter;

  os << ']' << std::endl;
}

std::ostream& operator << (std::ostream& os, const S_msrFiguredBass& elt)
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
