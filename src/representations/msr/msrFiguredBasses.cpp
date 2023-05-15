/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <iomanip>

#include "visitor.h"

#include "mfPreprocessorSettings.h"

#include "mfAssert.h"

#include "msrMeasureConstants.h"

#include "msrFiguredBasses.h"

#include "oahOah.h"

#include "msrOah.h"

#include "msrBrowsers.h"

#include "waeHandlers.h"


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
    case msrBassFigurePrefixKind::kBassFigurePrefix_UNKNOWN_:
      result = "kBassFigurePrefix_UNKNOWN_";
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
    case msrBassFigureSuffixKind::kBassFigureSuffix_UNKNOWN_:
      result = "kBassFigureSuffix_UNKNOWN_";
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
  msrBassFigure* obj =
    new msrBassFigure (
      inputLineNumber,
      figureUpLinkToPart,
      figurePrefixKind,
      figureNumber,
      figureSuffixKind);
  assert (obj != nullptr);
  return obj;
}

msrBassFigure::msrBassFigure (
  int                     inputLineNumber,
  const S_msrPart&        figureUpLinkToPart,
  msrBassFigurePrefixKind figurePrefixKind,
  int                     figureNumber,
  msrBassFigureSuffixKind figureSuffixKind)
    : msrElement (inputLineNumber)
{
#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    figureUpLinkToPart != nullptr,
    "figureUpLinkToPart is null");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  // set figured's part upLink
  fFigureUpLinkToPart =
    figureUpLinkToPart;

  fFigurePrefixKind = figurePrefixKind;
  fFigureNumber     = figureNumber;
  fFigureSuffixKind = figureSuffixKind;

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceFiguredBasses ()) {
    std::stringstream ss;

    ss <<
      "Creating bass figure " <<
      asString ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

msrBassFigure::~msrBassFigure ()
{}

S_msrBassFigure msrBassFigure::createFigureNewbornClone (
  const S_msrPart& containingPart)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceFiguredBasses ()) {
    std::stringstream ss;

    ss <<
      "Creating a newborn clone of bass figure " <<
      asString ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    containingPart != nullptr,
    "containingPart is null");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  S_msrBassFigure
    newbornClone =
      msrBassFigure::create (
        fInputStartLineNumber,
        containingPart,
        fFigurePrefixKind,
        fFigureNumber,
        fFigureSuffixKind);

  return newbornClone;
}

S_msrBassFigure msrBassFigure::createFigureDeepClone (
  const S_msrPart& containingPart)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceFiguredBasses ()) {
    std::stringstream ss;

    ss <<
      "Creating a deep clone of bass figure " <<
      asString ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    containingPart != nullptr,
    "containingPart is null");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  S_msrBassFigure
    deepClone =
      msrBassFigure::create (
        fInputStartLineNumber,
        containingPart,
        fFigurePrefixKind,
        fFigureNumber,
        fFigureSuffixKind);

  return deepClone;
}

// bool msrFiguredBass::compareFiguredBassesByIncreasingDuration (
//   const SMARTP<msrFiguredBass>& first,
//   const SMARTP<msrFiguredBass>& second)
// {
//   return
//     bool (
//       first->fFiguredBassWholeNotesDuration
//         <
//       second->fFiguredBassWholeNotesDuration
//     );
// }

void msrBassFigure::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrBassFigure::acceptIn ()";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msrBassFigure>*
    p =
      dynamic_cast<visitor<S_msrBassFigure>*> (v)) {
        S_msrBassFigure elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrBassFigure::visitStart ()";

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void msrBassFigure::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrBassFigure::acceptOut ()";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msrBassFigure>*
    p =
      dynamic_cast<visitor<S_msrBassFigure>*> (v)) {
        S_msrBassFigure elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrBassFigure::visitEnd ()";

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void msrBassFigure::browseData (basevisitor* v)
{}

std::string msrBassFigure::asString () const
{
  std::stringstream ss;

  ss <<
    "[BassFigure" <<
    " '" << fFigureNumber <<
    "', prefix: " <<
    fFigurePrefixKind <<
    ", suffix: " <<
    fFigureSuffixKind <<
    ", line " << fInputStartLineNumber <<
    ']';

  return ss.str ();
}

void msrBassFigure::print (std::ostream& os) const
{
  os << asString () << std::endl;

/* JMI
  if (fFigureUpLinkToPart) { // JMI ???
    ss <<
      ":" <<
      wholeNotesAsMsrString (
        fInputStartLineNumber,
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
    os << "[NULL]" << std::endl;
  }

  return os;
}

//______________________________________________________________________________
S_msrFiguredBass msrFiguredBass::create (
  int                 inputLineNumber,
  const S_msrMeasure& upLinkToMeasure)
{
  msrFiguredBass* obj =
    new msrFiguredBass (
      inputLineNumber,
      upLinkToMeasure,
      msrWholeNotes (0, 1),           // figuredBassSoundingWholeNotes
      msrWholeNotes (0, 1),           // figuredBassDisplayWholeNotes
//       msrWholeNotes (0, 1),           // fFiguredBassWholeNotesDuration
      msrFiguredBassParenthesesKind::kFiguredBassParenthesesNo,
      msrTupletFactor (1, 1));
  assert (obj != nullptr);
  return obj;
}

S_msrFiguredBass msrFiguredBass::create (
  int inputLineNumber)
{
  return
     msrFiguredBass::create (
      inputLineNumber,
      gNullMeasure); // set later in setMeasureElementUpLinkToMeasure()
}

S_msrFiguredBass msrFiguredBass::create (
  int                    inputLineNumber,
  const S_msrMeasure&    upLinkToMeasure,
  const msrWholeNotes&   figuredBassSoundingWholeNotes,
  const msrWholeNotes&   figuredBassDisplayWholeNotes,
//   const msrWholeNotes&   figuredBassWholeNotesDuration,
  msrFiguredBassParenthesesKind
                         figuredBassParenthesesKind,
  const msrTupletFactor& figuredBassTupletFactor)
{
  msrFiguredBass* obj =
    new msrFiguredBass (
      inputLineNumber,
      upLinkToMeasure,
      figuredBassSoundingWholeNotes,
      figuredBassDisplayWholeNotes,
//       figuredBassWholeNotesDuration,
      figuredBassParenthesesKind,
      figuredBassTupletFactor);
  assert (obj != nullptr);
  return obj;
}

S_msrFiguredBass msrFiguredBass::create (
  int                    inputLineNumber,
  const msrWholeNotes&   figuredBassSoundingWholeNotes,
  const msrWholeNotes&   figuredBassDisplayWholeNotes,
//   const msrWholeNotes&   figuredBassWholeNotesDuration,
  msrFiguredBassParenthesesKind
                         figuredBassParenthesesKind,
  const msrTupletFactor& figuredBassTupletFactor)
{
  return
    msrFiguredBass::create (
      inputLineNumber,
      gNullMeasure, // set later in setMeasureElementUpLinkToMeasure()
      figuredBassSoundingWholeNotes,
      figuredBassDisplayWholeNotes,
//       figuredBassWholeNotesDuration,
      figuredBassParenthesesKind,
      figuredBassTupletFactor);
}

msrFiguredBass::msrFiguredBass (
  int                    inputLineNumber,
  const S_msrMeasure&    upLinkToMeasure,
  const msrWholeNotes&   figuredBassSoundingWholeNotes,
  const msrWholeNotes&   figuredBassDisplayWholeNotes,
//   const msrWholeNotes&   figuredBassWholeNotesDuration,
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

  doSetSoundingWholeNotes (
    figuredBassSoundingWholeNotes,
    "msrFiguredBass::msrFiguredBass()");

  fFiguredBassDisplayWholeNotes =
    figuredBassDisplayWholeNotes;

//   fFiguredBassWholeNotesDuration =
//     figuredBassWholeNotesDuration;

  fFiguredBassParenthesesKind =
    figuredBassParenthesesKind;

  // a figured bass element is considered to be at the beginning of the measure
  // until this is computed in msrMeasure::finalizeTheFiguredBassesInAFiguredBassMeasure()
  fMeasurePosition = msrWholeNotes (0, 1);

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceFiguredBasses ()) {
    std::stringstream ss;

    ss <<
      "Creating figuredBass " <<
      asString ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

msrFiguredBass::~msrFiguredBass ()
{}

S_msrFiguredBass msrFiguredBass::createFiguredBassNewbornClone (
  const S_msrVoice& containingVoice)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceFiguredBasses ()) {
    std::stringstream ss;

    ss <<
      "Creating a newborn clone of figured bass " <<
      asShortString () <<
      ", line " << fInputStartLineNumber;

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

  S_msrFiguredBass
    newbornClone =
      msrFiguredBass::create (
        fInputStartLineNumber,
        gNullMeasure, // set later in setMeasureElementUpLinkToMeasure()
        fSoundingWholeNotes, // JMI v0.9.67
        fFiguredBassDisplayWholeNotes,
//         fFiguredBassWholeNotesDuration,
        fFiguredBassParenthesesKind,
        fFiguredBassTupletFactor);

  newbornClone->setFiguredBassUpLinkToVoice (
    containingVoice);

  return newbornClone;
}

S_msrFiguredBass msrFiguredBass::createFiguredBassDeepClone ()
 // S_msrPart containingPart) // JMI v0.9.66
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceFiguredBasses ()) {
    std::stringstream ss;

    ss <<
      "Creating a deep clone of figuredBass " <<
      asString () <<
      ", line " << fInputStartLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

// #ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
//   mfAssert (
//     __FILE__, __LINE__,
//     containingPart != nullptr,
//     "containingPart is null");
// ?#endif // MF_SANITY_CHECKS_ARE_ENABLED

  S_msrFiguredBass
    figuredBassDeepClone =
      msrFiguredBass::create (
        fInputStartLineNumber,
        gNullMeasure, // set later in setMeasureElementUpLinkToMeasure()
        fSoundingWholeNotes, // JMI v0.9.67
        fFiguredBassDisplayWholeNotes,
//         fFiguredBassWholeNotesDuration,
        fFiguredBassParenthesesKind,
        fFiguredBassTupletFactor);

//   figuredBassDeepClone->setFiguredBassUpLinkToVoice (
//     containingVoice);

  return figuredBassDeepClone;
}

void msrFiguredBass::setFiguredBassUpLinkToNote (
  const S_msrNote& note)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceFiguredBasses ()) {
    std::stringstream ss;

    ss <<
      "Setting the uplink to note of figured bass " <<
      asString () <<
      " to note " << note->asString ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    note != nullptr,
    "note is null");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  fFiguredBassUpLinkToNote = note;
}

void msrFiguredBass::setFiguredBassUpLinkToMeasure (
  const S_msrMeasure& measure)
{
#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    measure != nullptr,
    "measure is null");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceFiguredBasses ()) {
    ++gIndenter;

    gLog <<
      "Setting the uplink to measure of figured bass " <<
      asString () <<
      " to measure " << measure->asString () <<
      "' in measure '" <<
      measure->asString () <<
      std::endl;

    --gIndenter;
  }
#endif // MF_TRACE_IS_ENABLED

  fFiguredBassUpLinkToMeasure = measure;
}

void msrFiguredBass::appendFigureToFiguredBass (
  const S_msrBassFigure& bassFigure)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceFiguredBasses ()) {
    std::stringstream ss;

    ss <<
      "Appending bass figure " << bassFigure->asString () <<
      " to figured-bass element '" <<
      asString () <<
      "'";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fFiguredBassFiguresList.push_back (bassFigure);
}

void msrFiguredBass::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrFiguredBass::acceptIn ()";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msrFiguredBass>*
    p =
      dynamic_cast<visitor<S_msrFiguredBass>*> (v)) {
        S_msrFiguredBass elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrFiguredBass::visitStart ()";

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void msrFiguredBass::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrFiguredBass::acceptOut ()";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msrFiguredBass>*
    p =
      dynamic_cast<visitor<S_msrFiguredBass>*> (v)) {
        S_msrFiguredBass elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrFiguredBass::visitEnd ()";

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
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
  std::stringstream ss;

  ss <<
    "[FiguredBass" <<
    ", " << std::hex << std::showbase << this << std::dec <<
    ", fMeasurePosition: " <<
    fMeasurePosition.asString () <<
    ", fSoundingWholeNotes: " <<
    wholeNotesAsMsrString (
      fInputStartLineNumber,
      fSoundingWholeNotes) <<
    ", fFiguredBassDisplayWholeNotes: " <<
    wholeNotesAsMsrString (
      fInputStartLineNumber,
      fFiguredBassDisplayWholeNotes);

  ss <<
    ", fFiguredBassUpLinkToNote: ";
    if (fFiguredBassUpLinkToNote) {
      ss <<
        fFiguredBassUpLinkToNote->asString ();
    }
    else {
      ss << "[NULL]";
    }

  ss <<
    ", fFiguredBassUpLinkToMeasure: ";
    if (fFiguredBassUpLinkToMeasure) {
      ss <<
        fFiguredBassUpLinkToMeasure->getMeasureNumber ();
    }
    else {
      ss << "[NULL]";
    }

  ss <<
    ", fFiguredBassUpLinkToVoice" << ": ";
  if (fFiguredBassUpLinkToVoice) {
    ss << fFiguredBassUpLinkToVoice->asString ();
  }
  else {
    ss << "[NULL]";
  }
  ss << std::endl;

  ss <<
    ", fFiguredBassParenthesesKind: " <<
    fFiguredBassParenthesesKind <<

    ", fFiguredBassTupletFactor: " <<
    fFiguredBassTupletFactor.asString ();

  if (fFiguredBassFiguresList.size ()) {
    ss << ", fFiguredBassFiguresList: [";

    std::list<S_msrBassFigure>::const_iterator
      iBegin = fFiguredBassFiguresList.begin (),
      iEnd   = fFiguredBassFiguresList.end (),
      i      = iBegin;

    for ( ; ; ) {
      ss << (*i)->asString ();
      if (++i == iEnd) break;
      ss << ' ';
    } // for
  ss << ']';
  }

  // print the figured bass measure position
  ss <<
    ", measurePosition: " << fMeasurePosition;

/* JMI
  if (fFiguredBassUpLinkToPart) { // JMI ???
    ss <<
      ":" <<
      wholeNotesAsMsrString (
        fInputStartLineNumber,
        fFiguredBassSoundingWholeNotes);
  }
*/

  // print the figured bass voice position
//   ss <<
//     ", voicePosition: " << fVoicePosition;

  ss <<
    ", line " << fInputStartLineNumber <<
    ']';

  return ss.str ();
}

std::string msrFiguredBass::asShortStringForMeasuresSlices () const
{
  std::stringstream ss;

  ss <<
    "[FiguredBass" <<
    ", " << std::hex << std::showbase << this << std::dec <<
    ", fMeasurePosition: " <<
    fMeasurePosition.asString () <<
    ", fSoundingWholeNotes: " <<
    wholeNotesAsMsrString (
      fInputStartLineNumber,
      fSoundingWholeNotes) <<
    ", fFiguredBassDisplayWholeNotes: " <<
    wholeNotesAsMsrString (
      fInputStartLineNumber,
      fFiguredBassDisplayWholeNotes);

  ss <<
    ", fFiguredBassUpLinkToNote: ";
    if (fFiguredBassUpLinkToNote) {
      ss <<
        fFiguredBassUpLinkToNote->asString ();
    }
    else {
      ss << "[NULL]";
    }

  ss <<
    ", fFiguredBassUpLinkToMeasure: ";
    if (fFiguredBassUpLinkToMeasure) {
      ss <<
        fFiguredBassUpLinkToMeasure->getMeasureNumber ();
    }
    else {
      ss << "[NULL]";
    }

  ss <<
    ", fFiguredBassUpLinkToVoice" << ": ";
  if (fFiguredBassUpLinkToVoice) {
    ss << fFiguredBassUpLinkToVoice->asString ();
  }
  else {
    ss << "[NULL]";
  }
  ss << std::endl;

  ss <<
    ", fFiguredBassParenthesesKind: " <<
    fFiguredBassParenthesesKind <<

    ", fFiguredBassTupletFactor: " <<
    fFiguredBassTupletFactor.asString ();

  if (fFiguredBassFiguresList.size ()) {
    ss << ", fFiguredBassFiguresList: [";

    std::list<S_msrBassFigure>::const_iterator
      iBegin = fFiguredBassFiguresList.begin (),
      iEnd   = fFiguredBassFiguresList.end (),
      i      = iBegin;

    for ( ; ; ) {
      ss << (*i)->asString ();
      if (++i == iEnd) break;
      ss << ' ';
    } // for
  }

  // print the figured bass measure position
  ss <<
    ", measurePosition: " << fMeasurePosition;

/* JMI
  if (fFiguredBassUpLinkToPart) { // JMI ???
    ss <<
      ":" <<
      wholeNotesAsMsrString (
        fInputStartLineNumber,
        fFiguredBassSoundingWholeNotes);
  }
*/

  // print the figured bass voice position
//   ss <<
//     ", voicePosition: " << fVoicePosition;

  ss <<
    ", line " << fInputStartLineNumber <<
    ']';

  return ss.str ();
}

void msrFiguredBass::print (std::ostream& os) const
{
  os <<
    "[FiguredBass" <<
    ", line " << fInputStartLineNumber <<
    std::endl;

  ++gIndenter;

  const int fieldWidth = 36;

  os << std::left <<
    std::setw (fieldWidth) <<
    "fMeasurePosition" << ": " <<
    fMeasurePosition.asString () <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fSoundingWholeNotes" << ": " <<
    fSoundingWholeNotes.asString () <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fFiguredBassDisplayWholeNotes" << ": " <<
    fFiguredBassDisplayWholeNotes.asString () <<
    std::endl;

  os <<
    std::setw (fieldWidth) <<
    "fFiguredBassUpLinkToNote" << ": ";
  if (fFiguredBassUpLinkToNote) {
    os << fFiguredBassUpLinkToNote->asString ();
  }
  else {
    os << "[NULL]";
  }
  os << std::endl;

  os <<
    std::setw (fieldWidth) <<
    "fFiguredBassUpLinkToMeasure" << ": ";
  if (fFiguredBassUpLinkToMeasure) {
    os << fFiguredBassUpLinkToMeasure->asString ();
  }
  else {
    os << "[NULL]";
  }
  os << std::endl;

  os <<
    std::setw (fieldWidth) <<
    "fFiguredBassUpLinkToVoice" << ": ";
  if (fFiguredBassUpLinkToVoice) {
    os << fFiguredBassUpLinkToVoice->asString ();
  }
  else {
    os << "[NULL]";
  }
  os << std::endl;

  os << std::left <<
    std::setw (fieldWidth) <<
    "fFiguredBassParenthesesKind" << ": " <<
    fFiguredBassParenthesesKind <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fFiguredBassTupletFactor" << ": " <<
    fFiguredBassTupletFactor.asString () <<
    std::endl;

  os << std::left <<
    std::setw (fieldWidth) <<
    "fFiguredBassFiguresList" << ": ";
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
    "fMeasurePosition" << ": " << fMeasurePosition.asString () <<
    std::endl;

//   // print the figured bass voice position
//   os <<
//     std::setw (fieldWidth) <<
//     "fVoicePosition" << ": " << fVoicePosition <<
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
    os << "[NULL]" << std::endl;
  }

  return os;
}


}
