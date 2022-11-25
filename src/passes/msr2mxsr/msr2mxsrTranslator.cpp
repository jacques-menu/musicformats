/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <climits>      // INT_MIN, INT_MAX
#include <sstream>
#include <iomanip>      // std::setw, std::setprecision, ...

#include "elements.h"
#include "factory.h"
#include "xml.h"
#include "xmlfile.h"

#include "mfAssert.h"

#include "oahEnableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "tracingOah.h"
#endif

#include "mfcLibraryComponent.h"

#include "mfServiceRunData.h"
#include "mfServiceRunData.h"

#include "msrArticulations.h"
#include "msrBarLines.h"
#include "msrTechnicals.h"
#include "msrTempos.h"

#include "msr2mxsrWae.h"

#include "oahOah.h"
#include "mxsrOah.h"
#include "msrOah.h"
#include "msr2mxsrOah.h"

#include "msr2mxsrComponent.h"

#include "msr2mxsrTranslator.h"

#include "msr2mxsrComponent.h"

#include "mxsrGeneration.h"

#include "msrBrowsers.h"


namespace MusicFormats
{
//________________________________________________________________________
msr2mxsrTranslator::msr2mxsrTranslator (
  const S_msrScore& visitedMsrScore)
{
  // the MSR score we're visiting
  fVisitedMsrScore = visitedMsrScore;

  // backup and forward handling
  fCurrentMeasurePosition = Rational (0, 1);

  fCurrentCumulatedSkipsDurations = Rational (0, 1);
  fCurrentCumulatedSkipsStaffNumber = -1;
  fCurrentCumulatedSkipsVoiceNumber = -1;
};

msr2mxsrTranslator::~msr2mxsrTranslator ()
{}

//________________________________________________________________________
Sxmlelement msr2mxsrTranslator::translateMsrToMxsr ()
{
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    fVisitedMsrScore != nullptr,
    "fVisitedMsrScore is null");

  // create the current score part-wise element
  fResultingMusicxmlelement =
    createMxmlScorePartWiseElement ();

  // create a msrScore browser
  msrBrowser<msrScore> browser (this);

  // browse the visited score with the browser
  browser.browse (*fVisitedMsrScore);

  return fResultingMusicxmlelement;
}

//________________________________________________________________________
int msr2mxsrTranslator::wholeNotesAsDivisions (
  int             inputLineNumber,
  const Rational& wholeNotes)
{
  Rational
    durationAsRational =
      wholeNotes
        /
      fPartShortestNoteDuration
        *
      fDivisionsMultiplyingFactor
        * 4; // divisions are per quarter note

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceDivisions ()) {
    gLogStream <<
      "--> durationAsRational: " <<
      durationAsRational <<
      "--> line " << inputLineNumber <<
      std::endl;
  }
#endif

  if (durationAsRational.getDenominator () != 1) {
    std::stringstream s;

    s <<
      "durationAsRational '" << durationAsRational <<
      "' is no integer number" <<
      ", line " << inputLineNumber;

    msr2mxsrInternalError (
      gGlobalServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }

  return durationAsRational.getNumerator ();
}

//______________________________________________________________________________
std::string msr2mxsrTranslator::msrLengthAsTenths (
  msrLength length)
{
  float lengthValue = length.getLengthValue ();

  // convert lengthValue to millimeters
  switch (length.getLengthUnitKind ()) {
    case msrLengthUnitKind::kUnitInch:
      lengthValue *= 25.4f;
      break;
    case msrLengthUnitKind::kUnitCentimeter:
      lengthValue *= 10.0f;
      break;
    case msrLengthUnitKind::kUnitMillimeter:
      break;
  } // switch

  // compute the number of tenths
  float tenths = lengthValue / fMillimeters * fTenths;

  // compute result
  std::stringstream s;

  s << tenths;

  return s.str ();
}

std::string msr2mxsrTranslator::S_msrLengthAsTenths (
  const S_msrLength& length)
{
  return msrLengthAsTenths (* length);
}

//______________________________________________________________________________
void msr2mxsrTranslator::populatePageMargins (
  Sxmlelement       elem,
  const S_msrMarginsGroup& marginsGroup)
{
  // left margin
  S_msrMargin leftMargin = marginsGroup->getLeftMargin ();

  if (leftMargin) {
    // append a left margin element to the page margins element
    msrLength
      leftMarginLength =
        leftMargin->getMarginLength ();

    elem->push (
      createMxmlelement (
        k_left_margin,
        msrLengthAsTenths (leftMarginLength)));
  }

  // right margin
  S_msrMargin rightMargin = marginsGroup->getRightMargin ();

  if (rightMargin) {
    // append a right margin element to the page margins element
    msrLength
      rightMarginLength =
        rightMargin->getMarginLength ();

    elem->push (
      createMxmlelement (
        k_right_margin,
        msrLengthAsTenths (rightMarginLength)));
  }

  // top margin
  S_msrMargin topMargin = marginsGroup->getTopMargin ();

  if (topMargin) {
    // append a top margin element to the page margins element
    msrLength
      topMarginLength =
        topMargin->getMarginLength ();

    elem->push (
      createMxmlelement (
        k_top_margin,
        msrLengthAsTenths (topMarginLength)));
  }

  // bottom margin
  S_msrMargin bottomMargin = marginsGroup->getBottomMargin ();

  if (bottomMargin) {
    // append a bottom margin element to the page margins element
    msrLength
      bottomMarginLength =
        bottomMargin->getMarginLength ();

    elem->push (
      createMxmlelement (
        k_bottom_margin,
        msrLengthAsTenths (bottomMarginLength)));
  }
}

void msr2mxsrTranslator::appendPageMarginsToScoreDefaultsPageLayout (
  const S_msrMarginsGroup& marginsGroup)
{
  // create a page margins element
  Sxmlelement
    pageMargins =
      createMxmlelement (k_page_margins, "");

  // set its "type" element
  pageMargins->add (
    createMxmlAttribute (
      "type",
      msrMarginTypeKindAsString (
        marginsGroup->getMarginsGroupTypeKind ())));

  // populate it
  populatePageMargins (
    pageMargins,
    marginsGroup);

  // append it to the defaults page layout element
  appendToScoreDefaultsPageLayout (
    pageMargins);
}

//______________________________________________________________________________
void msr2mxsrTranslator::populateSystemMargins (
  Sxmlelement       elem,
  const S_msrSystemLayout& systemLayout)
{
  // left margin
  S_msrMargin leftMargin = systemLayout->getLeftMargin ();

  if (leftMargin) {
    // append a left margin element to the page margins element
    msrLength
      leftMarginLength =
        leftMargin->getMarginLength ();

    elem->push (
      createMxmlelement (
        k_left_margin,
        msrLengthAsTenths (leftMarginLength)));
  }

  // right margin
  S_msrMargin rightMargin = systemLayout->getRightMargin ();

  if (rightMargin) {
    // append a right margin element to the page margins element
    msrLength
      rightMarginLength =
        rightMargin->getMarginLength ();

    elem->push (
      createMxmlelement (
        k_right_margin,
        msrLengthAsTenths (rightMarginLength)));
  }
}

void msr2mxsrTranslator::appendSystemMarginsToScoreDefaultsSystemLayout (
  const S_msrSystemLayout& systemLayout)
{
  // JMI
}

//________________________________________________________________________
std::string msr2mxsrTranslator::msrModeKindAsMusicXMLString (
  msrModeKind modeKind)
{
  std::string result;

  switch (modeKind) {
    case msrModeKind::kMode_NO_:
      result = "kMode_NO_";
      break;
    case msrModeKind::kModeMajor:
      result = "major";
      break;
    case msrModeKind::kModeMinor:
      result = "minor";
      break;
    case msrModeKind::kModeIonian:
      result = "ionian";
      break;
    case msrModeKind::kModeDorian:
      result = "dorian";
      break;
    case msrModeKind::kModePhrygian:
      result = "phrygian";
      break;
    case msrModeKind::kModeLydian:
      result = "lydian";
      break;
    case msrModeKind::kModeMixolydian:
      result = "mixolkModeMixolydianydian";
      break;
    case msrModeKind::kModeAeolian:
      result = "aeolian";
      break;
    case msrModeKind::kModeLocrian:
      result = "locrian";
      break;
  } // switch

  return result;
}

//______________________________________________________________________________
std::string msr2mxsrTranslator::msrPlacementKindAsMusicXMLString (
  msrPlacementKind placementKind)
{
  std::string result;

  switch (placementKind) {
    case msrPlacementKind::kPlacement_NO_:
      result = "";
      break;
    case msrPlacementKind::kPlacementAbove:
      result = "above";
      break;
    case msrPlacementKind::kPlacementBelow:
      result = "below";
      break;
  } // switch

  return result;
}

//______________________________________________________________________________
std::string msr2mxsrTranslator::msrSpannerTypeKindAsMusicXMLString (
  msrSpannerTypeKind spannerTypeKind)
{
  std::string result;

  switch (spannerTypeKind) {
    case msrSpannerTypeKind::kSpannerType_NO_:
      // should not occur
      break;
    case msrSpannerTypeKind::kSpannerTypeStart:
      result = "start";
      break;
    case msrSpannerTypeKind::kSpannerTypeStop:
      result = "stop";
      break;
    case msrSpannerTypeKind::kSpannerTypeContinue:
      result = "continue";
      break;
  } // switch

  return result;
}

//________________________________________________________________________
void msr2mxsrTranslator::appendToScoreWork (
  Sxmlelement elem)
{
  if (! fScoreWorkElement) {
    // create a work element
    fScoreWorkElement = createMxmlelement (k_work, "");
  }

  fScoreWorkElement->push (elem);
}

//________________________________________________________________________
void msr2mxsrTranslator::appendToScoreIdentification (
  Sxmlelement elem)
{
  if (! fScoreIdentificationElement) {
    // create an identification element
    fScoreIdentificationElement = createMxmlelement (k_identification, "");
  }

  fScoreIdentificationElement->push (elem);
}

void msr2mxsrTranslator::appendToScoreIdentificationEncoding (
  Sxmlelement elem)
{
  if (! fScoreIdentificationEncodingElement) {
    // create an encoding element
    fScoreIdentificationEncodingElement = createMxmlelement (k_encoding, "");
  }

  fScoreIdentificationEncodingElement->push (elem);
}

//________________________________________________________________________
void msr2mxsrTranslator::appendToScoreDefaults (
  Sxmlelement elem)
{
  if (! fScoreDefaultsElement) {
    // create a defaults element
    fScoreDefaultsElement = createMxmlelement (k_defaults, "");
  }

  fScoreDefaultsElement->push (elem);
}

void msr2mxsrTranslator::appendToScoreDefaultsPageLayout (
  Sxmlelement elem)
{
  if (! fScoreDefaultsPageLayoutElement) {
    // create a page layout element
    fScoreDefaultsPageLayoutElement = createMxmlelement (k_page_layout, "");
  }

  fScoreDefaultsPageLayoutElement->push (elem);
}

//________________________________________________________________________
void msr2mxsrTranslator::createMxmlAttributesElementAndAppendItToMeasure ()
{
#ifdef TRACING_IS_ENABLED
    if (gGlobalMxsrOahGroup->getTraceMxsr ()) {
      gLogStream <<
        "--> createMxmlAttributesElementAndAppendItToMeasure() 1" <<
        std::endl;
    }
#endif

  if (gGlobalMsr2mxsrOahGroup->getMusicXMLComments ()) {
    // create an attributes comment
    std::stringstream s;
    s <<
      " ===== " <<
      "Attributes " <<
      " ===== ";
    Sxmlelement comment = createMxmlelement (kComment, s.str ());

    // append it to the current measure element
    // maybe not if it not in the first measure seen with the given number??? JMI
    fCurrentMeasureElement->push (comment);
  }

  // create the attributes element
  Sxmlelement
    attributesElement =
      createMxmlelement (k_attributes, "");

  if (fDivisionsElement) {
    // append divisions to the current measure attributes element
    attributesElement->push (fDivisionsElement);
    fDivisionsElement = nullptr;
  }

  if (fKeyElement) {
    // append key to the current measure attributes element
#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceKeys ()) { // CAFE
      gLogStream <<
        "--> createMxmlAttributesElementAndAppendItToMeasure() 2" <<
        ", fKeyElement: ";
      printMxsr (fKeyElement, gLogStream);
      gLogStream << std::endl;
    }
#endif

    attributesElement->push (fKeyElement);
    fKeyElement = nullptr;
  }

  if (fTimeElement) {
#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceTimeSignatures ()) { // CAFE
      gLogStream <<
        "--> createMxmlAttributesElementAndAppendItToMeasure() 2" <<
        ", fTimeElement: ";
      printMxsr (fTimeElement, gLogStream);
      gLogStream << std::endl;
    }
#endif

    // append time to the current measure attributes element
    attributesElement->push (fTimeElement);
    fTimeElement = nullptr;
  }

  if (fStavesElement) {
    // append it the current measure attributes element
    attributesElement->push (fStavesElement);
    fStavesElement = nullptr;
  }

  if (fClefElementsList.size ()) {
    // append the clef  elements if any to the current measure element
    for (
      std::list<Sxmlelement>::const_iterator i =
        fClefElementsList.begin ();
      i!=fClefElementsList.end ();
      ++i
    ) {
      Sxmlelement clefElement = (*i);

#ifdef TRACING_IS_ENABLED
    if (gGlobalMxsrOahGroup->getTraceMxsr ()) {
      gLogStream <<
        "--> createMxmlAttributesElementAndAppendItToMeasure() 2" <<
        ", clefElement: ";
      printMxsr (clefElement, gLogStream);
      gLogStream << std::endl;
    }
#endif

      attributesElement->push (clefElement);
    } // for

    // forget about those clef elements
    fClefElementsList.clear ();
  }

  // append the attributes element to the current measure element
  fCurrentMeasureElement->push (attributesElement);
}

//________________________________________________________________________
void msr2mxsrTranslator::appendToMeasureDirection (
  Sxmlelement      elem,
  msrPlacementKind placementKind)
{
  if (gGlobalMsr2mxsrOahGroup->getMusicXMLComments ()) {
    // create a direction comment
    std::stringstream s;
    s <<
      " ===== " <<
      "Direction" <<
      " ===== ";
    Sxmlelement comment = createMxmlelement (kComment, s.str ());

    // append it to the current measure element
    fCurrentMeasureElement->push (comment);
  }

  // create a direction element
  Sxmlelement directionElement = createMxmlelement (k_direction, "");

  // set it's "placement" attribute if relevant
  std::string
    placementString =
      msrPlacementKindAsMusicXMLString (placementKind);

  if (placementString.size ()) {
    directionElement->add (createMxmlAttribute ("placement",  placementString));
  }

  // append the direction element to the current measure element
  fCurrentMeasureElement->push (directionElement);

  // create a direction type element
  Sxmlelement directionTypeElement = createMxmlelement (k_direction_type, "");

  // append it to the current direction element
  directionElement->push (directionTypeElement);

  // append elem to the direction type element
  directionTypeElement->push (elem);
}

//________________________________________________________________________
void msr2mxsrTranslator::appendNoteToMeasure (
  Sxmlelement note,
  const S_msrNote&   theMsrNote)
{
#ifdef TRACING_IS_ENABLED
  int inputLineNumber =
    note->getInputLineNumber ();

  if (gGlobalTracingOahGroup->getTraceNotes ()) {
    gLogStream <<
      "--> appendNoteToMeasure(), note = " <<
      ", note: " << mxmlelementAsString (note) <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  // append the 'before spanner' elements if any
  appendNoteSpannersBeforeNote (theMsrNote);

  // append note to the current measure element
  fCurrentMeasureElement->push (note);

  // account for theMsrNote's whole notes duration in the measure
  fCurrentMeasurePosition +=
    theMsrNote->
      getMeasureElementSoundingWholeNotes ();

  // append the 'after spanner' elements if any
  appendNoteSpannersAfterNote (theMsrNote);
}

void msr2mxsrTranslator::appendOtherToMeasure (
  Sxmlelement elem)
{
#ifdef TRACING_IS_ENABLED
  int inputLineNumber =
    elem->getInputLineNumber ();

  if (gGlobalTracingOahGroup->getTraceNotes ()) {
    gLogStream <<
      "--> appendOtherToMeasure()" <<
      ", elem: " << mxmlelementAsString (elem) <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  // append elem to the current measure element
  fCurrentMeasureElement->push (elem);
}

//________________________________________________________________________
void msr2mxsrTranslator::appendToNoteNotations (
  Sxmlelement      elem,
  msrPlacementKind placementKind)
{
  if (! fCurrentNoteNotationsElement) {
    // create an notations element
    fCurrentNoteNotationsElement = createMxmlelement (k_notations, "");

    // append it to fCurrentNoteElement
    fCurrentNoteElement->push (fCurrentNoteNotationsElement);
  }

  // set elem's "placement" attribute if relevant
  std::string
    placementString =
      msrPlacementKindAsMusicXMLString (placementKind);

  if (placementString.size ()) {
    elem->add (createMxmlAttribute ("placement", placementString));
  }

  // append elem to the note notations element
  fCurrentNoteNotationsElement->push (elem);
}

void msr2mxsrTranslator::appendToNoteNotationsOrnaments (
  Sxmlelement      elem,
  msrPlacementKind placementKind)
{
  if (! fCurrentNoteNotationsOrnamentsElement) {
    // create an notations element
    fCurrentNoteNotationsOrnamentsElement = createMxmlelement (k_ornaments, "");

    // append it to fCurrentNoteNotationsElement
    appendToNoteNotations (
      fCurrentNoteNotationsOrnamentsElement,
      msrPlacementKind::kPlacement_NO_); // no placement for '<ornaments/>', only <trill-mark> has JMI ???
  }

  // set elem's "placement" attribute if relevant
  std::string
    placementString =
      msrPlacementKindAsMusicXMLString (placementKind);

  if (placementString.size ()) {
    elem->add (createMxmlAttribute ("placement", placementString));
  }

  // append elem to the note notations ornaments element
  fCurrentNoteNotationsOrnamentsElement->push (elem);
}

//________________________________________________________________________
void msr2mxsrTranslator::appendToNoteNotationsArticulations (
  Sxmlelement      elem,
  msrPlacementKind placementKind)
{
  if (! fCurrentNoteNotationsArticulationsElement) {
    // create the note notations articulations element
    fCurrentNoteNotationsArticulationsElement = createMxmlelement (k_articulations, "");

    // append it to fCurrentNoteNotationsArticulationsElement
    appendToNoteNotations (
      fCurrentNoteNotationsArticulationsElement,
      placementKind);
  }

  // set elem's "placement" attribute if relevant
  std::string
    placementString =
      msrPlacementKindAsMusicXMLString (placementKind);

  if (placementString.size ()) {
    elem->add (createMxmlAttribute ("placement", placementString));
  }

  // append elem to the note notations articulations element
  fCurrentNoteNotationsArticulationsElement->push (elem);
}

//________________________________________________________________________
void msr2mxsrTranslator::appendToNoteNotationsTechnicals (
  Sxmlelement      elem,
  msrPlacementKind placementKind)
{
  if (! fCurrentNoteNotationsTechnicalsElement) {
    // create an notations element
    fCurrentNoteNotationsTechnicalsElement = createMxmlelement (k_technical, "");

    // append it to fCurrentNoteNotationsElement
    appendToNoteNotations (
      fCurrentNoteNotationsTechnicalsElement,
      placementKind);
  }

  // set elem's "placement" attribute if relevant
  std::string
    placementString =
      msrPlacementKindAsMusicXMLString (placementKind);

  if (placementString.size ()) {
    elem->add (createMxmlAttribute ("placement", placementString));
  }

  // append elem to the note notations technicals element
  fCurrentNoteNotationsTechnicalsElement->push (elem);
}

//________________________________________________________________________
void msr2mxsrTranslator::visitStart (S_msrScore& elt)
{
#ifdef TRACING_IS_ENABLED
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrScore" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  // get the pass component
  S_mfcPassComponent
    passComponent =
      createMsr2mxsrComponent ();

  // get the pass component current version number and date
  std::string
    passComponentMostRecentVersionNumberAndDateAsString =
      getGlobalMusicFormatsVersionNumberAndDate ();

  // create the initial creation comment
  std::stringstream s;
  s <<
    std::endl <<
    "==================================================" <<
    std::endl <<

    "Generated by " <<
    gGlobalOahOahGroup->getOahOahGroupServiceName () <<
    ' ' <<
    passComponentMostRecentVersionNumberAndDateAsString <<
    std::endl <<

    "on " <<
    gGlobalServiceRunData->getRunDateFull () <<
    std::endl <<

    "from ";

  if (gGlobalServiceRunData->getInputSourceName () == "-") {
    s << "standard input";
  }
  else {
    s << "\"" << gGlobalServiceRunData->getInputSourceName () << "\"";
  }

  s <<
    std::endl <<

    "==================================================" <<
    std::endl;

  // append the initial creation comment to the score part wise element
  fResultingMusicxmlelement->push (createMxmlelement (kComment, s.str ()));

  // create a software element
  Sxmlelement
    softwareElement =
      createMxmlelement (
        k_software,
        gGlobalOahOahGroup->getOahOahGroupServiceName ()
          + ' '
          + passComponentMostRecentVersionNumberAndDateAsString +
          ", https://github.com/jacques-menu/musicformats");

  // append it to the identification encoding
  appendToScoreIdentificationEncoding (softwareElement);

  // create an encoding date element
  Sxmlelement
    encodingDateElement =
      createMxmlelement (
        k_encoding_date,
        gGlobalServiceRunData->getRunDateYYYYMMDD ());

  // append it to the identification encoding
  appendToScoreIdentificationEncoding (encodingDateElement);

  // create the part std::list element
  fScorePartListElement = createMxmlelement (k_part_list, "");
}

void msr2mxsrTranslator::visitEnd (S_msrScore& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrScore" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  // append the score work element if any to the score part wise element
  if (fScoreWorkElement) {
    fResultingMusicxmlelement->push (fScoreWorkElement);
  }

  // append the score movement number element if any to the score part wise element
  if (fScoreMovementNumberElement) {
    fResultingMusicxmlelement->push (fScoreMovementNumberElement);
  }

  // append the score movement title element if any to the score part wise element
  if (fScoreMovementTitleElement) {
    fResultingMusicxmlelement->push (fScoreMovementTitleElement);
  }

  // append the score identification element if any to the score part wise element
  if (fScoreIdentificationElement) {
    if (gGlobalMsr2mxsrOahGroup->getMusicXMLComments ()) {
      // create an identification comment
      std::stringstream s;
      s <<
        " ===== " <<
        "Identification" <<
        " ===== ";
      Sxmlelement comment = createMxmlelement (kComment, s.str ());

      // append it to the score partwise element
      fResultingMusicxmlelement->push (comment);
    }

    fResultingMusicxmlelement->push (fScoreIdentificationElement);
  }

  if (fScoreDefaultsScalingElement) {
    // append the scaling element to the score defaults element
    appendToScoreDefaults (fScoreDefaultsScalingElement);
  }

  if (fScoreDefaultsPageLayoutElement) {
    // append the page layout element to the score defaults element
    appendToScoreDefaults (fScoreDefaultsPageLayoutElement);
  }

  if (fScoreDefaultsSystemLayoutElement) {
    // append the system layout element to the score defaults element
    appendToScoreDefaults (fScoreDefaultsSystemLayoutElement);
  }

  // append the staff layout elements if any to the score defaults element
  for (
    std::list<Sxmlelement>::const_iterator i =
      fScoreDefaultsStaffLayoutElementsList.begin ();
    i!=fScoreDefaultsStaffLayoutElementsList.end ();
    ++i
  ) {
    Sxmlelement staffLayoutElement = (*i);

    appendToScoreDefaults (staffLayoutElement);
  } // for

  if (fScoreDefaultsAppearanceElement) {
    // append the appearance element to the score defaults element
    appendToScoreDefaults (fScoreDefaultsAppearanceElement);
  }

  // append the score defaults element if any to the score part wise element
  if (fScoreDefaultsElement) {
    if (gGlobalMsr2mxsrOahGroup->getMusicXMLComments ()) {
      // create an defaults comment
      std::stringstream s;
      s <<
        " ===== " <<
        "Defaults" <<
        " ===== ";
      Sxmlelement comment = createMxmlelement (kComment, s.str ());

      // append it to the score partwise element
      fResultingMusicxmlelement->push (comment);
    }

    fResultingMusicxmlelement->push (fScoreDefaultsElement);
  }

  // append the score credits element if any to the score part wise element
  size_t pendingScoreCreditElementsListSize =
    fPendingScoreCreditElementsList.size ();

  if (pendingScoreCreditElementsListSize) {
    if (gGlobalMsr2mxsrOahGroup->getMusicXMLComments ()) {
      std::stringstream s;
      s <<
        " ===== " <<
        mfSingularOrPlural (
          pendingScoreCreditElementsListSize, "Credit", "Credits") <<
        " ===== ";
      Sxmlelement comment = createMxmlelement (kComment, s.str ());

      // append it to the score partwise element
      fResultingMusicxmlelement->push (comment);
    }

    for (
      std::list<Sxmlelement>::const_iterator i =
        fPendingScoreCreditElementsList.begin ();
      i!=fPendingScoreCreditElementsList.end ();
      ++i
    ) {
      Sxmlelement creditElement = (*i);

      fResultingMusicxmlelement->push (creditElement);
    } // for
  }

  if (gGlobalMsr2mxsrOahGroup->getMusicXMLComments ()) {
    // create an part-std::list comment
    std::stringstream s;
    s <<
      " ===== " <<
      "PART-std::list" <<
      " ===== ";
    Sxmlelement comment = createMxmlelement (kComment, s.str ());

    // append it to the score partwise element
    fResultingMusicxmlelement->push (comment);
  }

  // append the part std::list element to the score part wise element
  fResultingMusicxmlelement->push (fScorePartListElement);

  // append the pending parts elements to the score part wise element
  for (
    std::list<Sxmlelement>::const_iterator i =
      fPendingPartElementsList.begin ();
    i!=fPendingPartElementsList.end ();
    ++i
  ) {
    Sxmlelement partElement = (*i);

    if (gGlobalMsr2mxsrOahGroup->getMusicXMLComments ()) {
      // create a part comment
      std::stringstream s;
      s <<
        " ============================ " <<
        "PART" <<
        " \"" << partElement->getAttributeValue ("id") << "\"" <<
        ", line " << inputLineNumber <<
        " ============================= ";
      Sxmlelement partComment = createMxmlelement (kComment, s.str ());

      // append it to the score part wise element
      fResultingMusicxmlelement->push (partComment);
    }

    // append the part element to the score part wise element
    fResultingMusicxmlelement->push (partElement);
  } // for
}

//________________________________________________________________________
void msr2mxsrTranslator::visitStart (S_msrIdentification& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrIdentification" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

  // work number
  std::string
    workNumber =
      elt->getIdentificationWorkNumber (),
    optionsWorkNumber =
      gGlobalMsr2mxsrOahGroup->getWorkNumber ();

  if (optionsWorkNumber.size ()) {
    workNumber = optionsWorkNumber;
  }

  // create the work number element
  Sxmlelement
    workNumberElement =
      createMxmlelement (
        k_work_number,
        workNumber);

  // append it to the score part wise element
  appendToScoreWork (workNumberElement);

  // work title
  std::string
    workCreditTypeTitle =
      elt->getIdentificationWorkCreditTypeTitle (),
    optionsWorkCreditTypeTitle =
      gGlobalMsr2mxsrOahGroup->getWorkCreditTypeTitle ();

  if (optionsWorkCreditTypeTitle.size ()) {
    workCreditTypeTitle = optionsWorkCreditTypeTitle;
  }

  // create the work title element
  Sxmlelement
    workCreditTypeTitleElement =
      createMxmlelement (
        k_work_title,
        workCreditTypeTitle);

  // append it to the score work element
  appendToScoreWork (workCreditTypeTitleElement);

  // opus
  std::string
    opus =
      elt->getIdentificationOpus ();

/* JMI <opus/> NOT YET USED ANYWAY?

https://forums.makemusic.com/viewtopic.php?f=12&t=2017&p=9869&hilit=opus&sid=fd2554e5ca94ddd842f10d40ae429772#p9869

Michael Good wrote:
Hi Mario,

I don't know if any distributed software is currently supporting the opus. However, we have used it for some internal programs here. Here is a short example from Schumann's Frauenliebe und Leben cycle. The opus file collects the 8 MusicXML score-partwise files for each individual song into a single opus:

<?xml version="1.0" standalone="no"?>
<!DOCTYPE opus PUBLIC "-//Recordare//DTD MusicXML 2.0 Opus//EN"
"http://www.musicxml.org/dtds/opus.dtd">
<opus version="2.0">
<title>Frauenliebe und Leben, Op. 42</title>
<score xlink:href="Frauenliebe1.xml"/>
<score xlink:href="Frauenliebe2.xml"/>
<score xlink:href="Frauenliebe3.xml"/>
<score xlink:href="Frauenliebe4.xml"/>
<score xlink:href="Frauenliebe5.xml"/>
<score xlink:href="Frauenliebe6.xml"/>
<score xlink:href="Frauenliebe7.xml"/>
<score xlink:href="Frauenliebe8.xml"/>
</opus>
...

  // create the opus element
  Sxmlelement
    opusElement =
      createMxmlelement (
        k_opus,
        opus);

    // append it to the score part wise element
    appendToScoreWork (opusElement);
*/

  // movement number
  std::string
    movementNumber =
      elt->getIdentificationMovementNumber (),
    optionsMovementNumber =
      gGlobalMsr2mxsrOahGroup->getMovementNumber ();

  if (optionsMovementNumber.size ()) {
    movementNumber = optionsMovementNumber;
  };

  // create the movement number element
  fScoreMovementNumberElement =
    createMxmlelement (
      k_movement_number,
      movementNumber);

  // don't append it at once to the score defaults element

  // movement title
  std::string
    movementTitle =
      elt->getIdentificationMovementTitle (),
    optionsMovementTitle =
      gGlobalMsr2mxsrOahGroup->getMovementTitle ();

  if (optionsMovementTitle.size ()) {
    movementTitle = optionsMovementTitle;
  };

  // create the movement title element
  fScoreMovementTitleElement =
    createMxmlelement (
      k_movement_title,
      movementTitle);

  // don't append it at once to the score defaults element

  // miscellaneous field
  std::string
    miscellaneousField =
      elt->getIdentificationMiscellaneousField ();

  // create the miscellaneous field element
  Sxmlelement
    miscellaneousFieldElement =
      createMxmlelement (
        k_miscellaneous_field,
        miscellaneousField);

  // set its "name" attribute
  miscellaneousFieldElement->add (
    createMxmlAttribute ("name", "description")); // ??? JMI sometines "comment"

  // create a miscellaneous element
  fIdentificationMiscellaneousElement = createMxmlelement (k_miscellaneous, "");

  // append the miscellaneous field element to it
  fIdentificationMiscellaneousElement->push (miscellaneousFieldElement);

  // score instrument
  std::string
    scoreInstrument =
      elt->getIdentificationScoreInstrument ();

  // create the score instrument element,
  // it will be appended to the score part element
  // when the latter is created
  fScoreInstrumentElement =
    createMxmlelement (
      k_score_instrument,
      "");
  // set it's "id" attribute later, when the partID is known!
  // create an instrument name element
  Sxmlelement
    scoreInstrumentNameElement =
      createMxmlelement (
        k_instrument_name,
        scoreInstrument);
  // append it to the score instrument element
  fScoreInstrumentElement->push (scoreInstrumentNameElement);

  // composers
  const std::list<std::string>&
    composersList =
      elt->getIdentificationComposersList ();

  for (
    std::list<std::string>::const_iterator i=composersList.begin ();
    i!=composersList.end ();
    ++i
  ) {
    std::string variableValue = (*i);

    // create a creator element
    Sxmlelement creatorElement = createMxmlelement (k_creator, variableValue);

    // set its "type" attribute
    creatorElement->add (createMxmlAttribute ("type", "composer"));

    // append it to the composers elements list
    fComposersElementsList.push_back (creatorElement);
  } // for

  // arrangers
  const std::list<std::string>&
    arrangersList =
      elt->getIdentificationArrangersList ();

  for (
    std::list<std::string>::const_iterator i=arrangersList.begin ();
    i!=arrangersList.end ();
    ++i
  ) {
    std::string variableValue = (*i);

    // create a creator element
    Sxmlelement creatorElement = createMxmlelement (k_creator, variableValue);

    // set its "type" attribute
    creatorElement->add (createMxmlAttribute ("type", "arranger"));

    // append it to the composers elements list
    fComposersElementsList.push_back (creatorElement);
  } // for

  // lyricists
  const std::list<std::string>&
    lyricistsList =
      elt->getIdentificationLyricistsList ();

  for (
    std::list<std::string>::const_iterator i=lyricistsList.begin ();
    i!=lyricistsList.end ();
    ++i
  ) {
    std::string variableValue = (*i);

    // append a creator element
    Sxmlelement creatorElement = createMxmlelement (k_creator, variableValue);

    // set its "type" attribute
    creatorElement->add (createMxmlAttribute ("type", "lyricist"));

    // append it to the composers elements list
    fComposersElementsList.push_back (creatorElement);
  } // for

  // poets
  const std::list<std::string>&
    poetsList =
      elt->getIdentificationPoetsList ();

  for (
    std::list<std::string>::const_iterator i=poetsList.begin ();
    i!=poetsList.end ();
    ++i
  ) {
    std::string variableValue = (*i);

    // append a creator element
    Sxmlelement creatorElement = createMxmlelement (k_creator, variableValue);

    // set its "type" attribute
    creatorElement->add (createMxmlAttribute ("type", "poet"));

    // append it to the composers elements list
    fComposersElementsList.push_back (creatorElement);
  } // for

  // translators
  const std::list<std::string>&
    translatorsList =
      elt->getIdentificationTranslatorsList ();

  for (
    std::list<std::string>::const_iterator i=translatorsList.begin ();
    i!=translatorsList.end ();
    ++i
  ) {
    std::string variableValue = (*i);

    // append a creator element
    Sxmlelement creatorElement = createMxmlelement (k_creator, variableValue);

    // set its "type" attribute
    creatorElement->add (createMxmlAttribute ("type", "translator"));

    // append it to the composers elements list
    fComposersElementsList.push_back (creatorElement);
  } // for

  // artists
  const std::list<std::string>&
    artistsList =
      elt->getIdentificationArtistsList ();

  for (
    std::list<std::string>::const_iterator i=artistsList.begin ();
    i!=artistsList.end ();
    ++i
  ) {
    std::string variableValue = (*i);

    // append a creator element
    Sxmlelement creatorElement = createMxmlelement (k_creator, variableValue);

    // set its "type" attribute
    creatorElement->add (createMxmlAttribute ("type", "artist"));

    // append it to the composers elements list
    fComposersElementsList.push_back (creatorElement);
  } // for

  // softwares
  const std::list<std::string>&
    softwaresList =
      elt->getIdentificationSoftwaresList ();

  for (
    std::list<std::string>::const_iterator i=softwaresList.begin ();
    i!=softwaresList.end ();
    ++i
  ) {
    std::string variableValue = (*i);

    // append a software element to the softwares elements list
    fSoftwaresElementsList.push_back (
      createMxmlelement (
      k_software,
      variableValue));
  } // for

  // rights
  const std::list<std::string>&
    rightsList =
      elt->getIdentificationRightsList ();

  for (
    std::list<std::string>::const_iterator i=rightsList.begin ();
    i!=rightsList.end ();
    ++i
  ) {
    std::string variableValue = (*i);

    // append a rights element to the rights elements list
    fRightsElementsList.push_back (
      createMxmlelement (
      k_rights,
      variableValue));
  } // for
}

void msr2mxsrTranslator::visitEnd (S_msrIdentification& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrIdentification" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

  if (fRightsElementsList.size ()) {
    // append the rights elements to the score identification element
    std::list<Sxmlelement>::const_iterator i;

    for (
      i=fRightsElementsList.begin ();
      i!=fRightsElementsList.end ();
      ++i) {
      Sxmlelement rightsElement = (*i);

      appendToScoreIdentification (rightsElement);
    } // for
  }

  if (fScoreIdentificationEncodingElement) {
    if (fSoftwaresElementsList.size ()) {
      // append the software elements to the score identification  encoding element
      for (
        std::list<Sxmlelement>::const_iterator i = fSoftwaresElementsList.begin ();
        i!=fSoftwaresElementsList.end ();
        ++i
      ) {
        Sxmlelement softwareElement = (*i);

        appendToScoreIdentificationEncoding (softwareElement);
      } // for
    }

    // append the score identification element to the score identification
    appendToScoreIdentification (fScoreIdentificationEncodingElement);
  }

  if (fIdentificationMiscellaneousElement) {
    // append the miscellaneous element to the score identification element
    appendToScoreIdentification (fIdentificationMiscellaneousElement);
  }
}

//________________________________________________________________________
void msr2mxsrTranslator::visitStart (S_msrScaling& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrScaling" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

  ++gIndenter;

  // get the values
  fMillimeters = elt->getMillimeters ();
  fTenths      = elt->getTenths ();

  if (fMillimeters > 0) {
    // create a scaling element
    fScoreDefaultsScalingElement = createMxmlelement (k_scaling, "");

    // append a millimeters sub-element to it
    {
      std::stringstream s;

      s << fMillimeters;

      fScoreDefaultsScalingElement->push (
        createMxmlelement (
          k_millimeters,
          s.str ()));
    }

    // append a tenths sub-element to it
    {
      std::stringstream s;

      s << fTenths;

      fScoreDefaultsScalingElement->push (
        createMxmlelement (
          k_tenths,
          s.str ()));
    }
  }
}

void msr2mxsrTranslator::visitEnd (S_msrScaling& elt)
{
  --gIndenter;

#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrScaling" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif
}

//________________________________________________________________________
void msr2mxsrTranslator::visitStart (S_msrPageLayout& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrPageLayout" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

  // page size
  S_msrLength pageHeight = elt->getPageHeight ();

  if (pageHeight) {
    // append a page height element to the defaults page layout element
    appendToScoreDefaultsPageLayout (
      createMxmlelement (
        k_page_height,
        S_msrLengthAsTenths (pageHeight)));
  }

  S_msrLength pageWidth = elt->getPageWidth ();

  if (pageWidth) {
    // append a page width element to the defaults page layout element
    appendToScoreDefaultsPageLayout (
      createMxmlelement (
        k_page_width,
        S_msrLengthAsTenths (pageWidth)));
  }

  // margins
  S_msrMarginsGroup oddMarginsGroup = elt->getOddMarginsGroup ();

  if (oddMarginsGroup) {
    // append an odd page margins element to the defaults page layout element
    appendPageMarginsToScoreDefaultsPageLayout (
      oddMarginsGroup);
  }

  S_msrMarginsGroup evenMarginsGroup = elt->getEvenMarginsGroup ();

  if (evenMarginsGroup) {
    // append an even page margins element to the defaults page layout element
    appendPageMarginsToScoreDefaultsPageLayout (
      evenMarginsGroup);
  }

  S_msrMarginsGroup bothMarginsGroup = elt->getBothMarginsGroup ();

  if (bothMarginsGroup) {
    // append a both page margins element to the defaults page layout element
    appendPageMarginsToScoreDefaultsPageLayout (
      bothMarginsGroup);
  }
}

void msr2mxsrTranslator::visitEnd (S_msrPageLayout& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrPageLayout" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif
}

//________________________________________________________________________
void msr2mxsrTranslator::visitStart (S_msrSystemLayout& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrSystemLayout" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

  // create a system layout element
  Sxmlelement
    systemLayoutElement =
      createMxmlelement (k_system_layout, "");

  if (fOnGoingMusicXMLPrintLayout) {
    // append it to the current print element
    fCurrentPrintElement->push (
      systemLayoutElement);
  }
  else {
    // don't append it at once to the score defaults element
    fScoreDefaultsSystemLayoutElement = systemLayoutElement;
  }

  // create a system margins element
  Sxmlelement
    systemMargins =
      createMxmlelement (k_system_margins, "");

  // populate it
  populateSystemMargins (
    systemMargins,
    elt);

  // append it to the system layout element
  systemLayoutElement->push (
    systemMargins);

  // distances
  S_msrLength systemDistance = elt->getSystemDistance ();

  if (systemDistance) {
    // create a system distance element
    Sxmlelement
      systemDistanceElement =
        createMxmlelement (
          k_system_distance,
          S_msrLengthAsTenths (systemDistance));

  // append it to the system layout element
  systemLayoutElement->push (
    systemDistanceElement);
  }

  S_msrLength topSystemDistance = elt->getTopSystemDistance ();

  if (topSystemDistance) {
    // create a top system distance element
    Sxmlelement
      topSystemDistanceElement =
        createMxmlelement (
          k_top_system_distance,
          S_msrLengthAsTenths (topSystemDistance));

  // append it to the system layout element
  systemLayoutElement->push (
    topSystemDistanceElement);
  }
}

void msr2mxsrTranslator::visitEnd (S_msrSystemLayout& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrSystemLayout" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif
}

//________________________________________________________________________
void msr2mxsrTranslator::visitStart (S_msrStaffLayout& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrStaffLayout" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

  // number
  int staffNumber = elt->getStaffNumber ();

  // create a staff layout element
  Sxmlelement
    staffLayoutElement =
      createMxmlelement (k_staff_layout, "");

  // set its "number" attribute if relevant
  // (it is 0 inside the <defaults /> element)
  if (staffNumber > 0) {
    staffLayoutElement->add (createMxmlIntegerAttribute ("number", staffNumber));
  }

  // distance
  S_msrLength staffDistance = elt->getStaffDistance ();

  if (staffDistance) {
    // create a staff distance element
    Sxmlelement
      staffDistanceElement =
        createMxmlelement (
          k_staff_distance,
          S_msrLengthAsTenths (staffDistance));

    // append it to the staff layout element
    staffLayoutElement->push (
      staffDistanceElement);
  }

  if (fOnGoingMusicXMLPrintLayout) {
    // append it to the current print element
    fCurrentPrintElement->push (
      staffLayoutElement);
  }
  else {
    // don't append it at once to the score defaults element
    fScoreDefaultsStaffLayoutElementsList.push_back (staffLayoutElement);
  }
}

void msr2mxsrTranslator::visitEnd (S_msrStaffLayout& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrStaffLayout" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif
}

//________________________________________________________________________
void msr2mxsrTranslator::visitStart (S_msrMeasureLayout& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrMeasureLayout" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

  if (fOnGoingMusicXMLPrintLayout) {
    // create a measure layout element
    Sxmlelement
      measureLayoutElement =
        createMxmlelement (k_measure_layout, "");

    // append it to the current print element
    fCurrentPrintElement->push (
      measureLayoutElement);

    // distance
    S_msrLength measureDistance = elt->getMeasureDistance ();

    if (measureDistance) {
      // create a measure distance element
      Sxmlelement
        measureDistanceElement =
          createMxmlelement (
            k_measure_distance,
            S_msrLengthAsTenths (measureDistance));

      // append it to the measure layout element
      measureLayoutElement->push (
        measureDistanceElement);
    }
  }

  else {
    // JMI
  }
}

void msr2mxsrTranslator::visitEnd (S_msrMeasureLayout& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrMeasureLayout" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif
}

//________________________________________________________________________
void msr2mxsrTranslator::populateAppearanceLineWidths (
  Sxmlelement                 appearanceElement,
  const std::list<S_msrLineWidth>& lineWidthsList)
{
  for (
    std::list<S_msrLineWidth>::const_iterator i =
      lineWidthsList.begin ();
    i!=lineWidthsList.end ();
    ++i
  ) {
    S_msrLineWidth lineWidth = (*i);

    // get line width type
    msrLineWidthTypeKind
      lineWidthTypeKind =
        lineWidth->getLineWidthTypeKind ();

    // get line width value
    S_msrLength
      lineWidthValue =
        lineWidth->getLineWidthValue ();

    // create a line width element
    Sxmlelement
      lineWidthElement =
        createMxmlelement (
          k_line_width,
          S_msrLengthAsTenths (lineWidthValue));

    // set its "type" attribute
    std::string lineWidthTypeString;

    switch (lineWidthTypeKind) {
      case msrLineWidthTypeKind::kLineWidthType_NO_:
        lineWidthTypeString = "kLineWidthType_NO_";
        break;
      case msrLineWidthTypeKind::kLineWidthTypeBeam:
        lineWidthTypeString = "beam";
        break;
      case msrLineWidthTypeKind::kLineWidthTypeBracket:
        lineWidthTypeString = "bracket";
        break;
      case msrLineWidthTypeKind::kLineWidthTypeDashes:
        lineWidthTypeString = "dashes";
        break;
      case msrLineWidthTypeKind::kLineWidthTypeEnclosure:
        lineWidthTypeString = "enclosure";
        break;
      case msrLineWidthTypeKind::kLineWidthTypeEnding:
        lineWidthTypeString = "ending";
        break;
      case msrLineWidthTypeKind::kLineWidthTypeExtend:
        lineWidthTypeString = "extend";
        break;
      case msrLineWidthTypeKind::kLineWidthTypeHeavyBarLine:
        lineWidthTypeString = "heavy barline";
        break;
      case msrLineWidthTypeKind::kLineWidthTypeHeavyLeger:
        lineWidthTypeString = "leger";
        break;
      case msrLineWidthTypeKind::kLineWidthTypeLightBarLine:
        lineWidthTypeString = "light barline";
        break;
      case msrLineWidthTypeKind::kLineWidthTypeOctaveShift:
        lineWidthTypeString = "octave shift";
        break;
      case msrLineWidthTypeKind::kLineWidthTypePedal:
        lineWidthTypeString = "pedal";
        break;
      case msrLineWidthTypeKind::kLineWidthTypeSlurMiddle:
        lineWidthTypeString = "slur middle";
        break;
      case msrLineWidthTypeKind::kLineWidthTypeSlurTip:
        lineWidthTypeString = "slur tip";
        break;
      case msrLineWidthTypeKind::kLineWidthTypeStaff:
        lineWidthTypeString = "staff";
        break;
      case msrLineWidthTypeKind::kLineWidthTypeStem:
        lineWidthTypeString = "stem";
        break;
      case msrLineWidthTypeKind::kLineWidthTypeTieMiddle:
        lineWidthTypeString = "tie middle";
        break;
      case msrLineWidthTypeKind::kLineWidthTypeTieTip:
        lineWidthTypeString = "tie tip";
        break;
      case msrLineWidthTypeKind::kLineWidthTypeTupletBracket:
        lineWidthTypeString = "tuplet bracket";
        break;
      case msrLineWidthTypeKind::kLineWidthTypeWedge:
        lineWidthTypeString = "wedge";
        break;
    } // switch

    lineWidthElement->add (createMxmlAttribute ("type", lineWidthTypeString));

    // append the line width element to the appearance element
    appearanceElement->push (
      lineWidthElement);
  } // for
}

void msr2mxsrTranslator::populateAppearanceNoteSizes (
  Sxmlelement                appearanceElement,
  const std::list<S_msrNoteSize>& noteSizesList)
{
  for (
    std::list<S_msrNoteSize>::const_iterator i =
      noteSizesList.begin ();
    i!=noteSizesList.end ();
    ++i
  ) {
    S_msrNoteSize noteSize = (*i);

    // get note size type
    msrNoteSizeTypeKind
      noteSizeTypeKind =
        noteSize->getNoteSizeTypeKind ();

    // get note size value
    float
      noteSizeValue =
        noteSize->getNoteSizeValue ();

    // create a note size element
    Sxmlelement
      noteSizeElement =
        createMxmlFloatElement (
          k_note_size,
          noteSizeValue);

    // set its "type" attribute
    std::string noteSizeTypeString;

    switch (noteSizeTypeKind) {
      case msrNoteSizeTypeKind::kNote_NO_SizeType:
        noteSizeTypeString = "msrNoteKind::kNote_NO_SizeType";
        break;
      case msrNoteSizeTypeKind::kNoteSizeTypeCue:
        noteSizeTypeString = "cue";
        break;
      case msrNoteSizeTypeKind::kNoteSizeTypeGrace:
        noteSizeTypeString = "grace";
        break;
      case msrNoteSizeTypeKind::kNoteSizeTypeLarge:
        noteSizeTypeString = "large";
        break;
    } // switch

    noteSizeElement->add (createMxmlAttribute ("type", noteSizeTypeString));

    // append the note size element to the appearance element
    appearanceElement->push (
      noteSizeElement);
  } // for
}

void msr2mxsrTranslator::populateAppearanceDistances (
  Sxmlelement                appearanceElement,
  const std::list<S_msrDistance>& distancesList)
{
  for (
    std::list<S_msrDistance>::const_iterator i =
      distancesList.begin ();
    i!=distancesList.end ();
    ++i
  ) {
    S_msrDistance distance = (*i);

    // get line width type
    msrDistanceTypeKind
      distanceTypeKind =
        distance->getDistanceTypeKind ();

    // get line width value
    S_msrLength
      distanceValue =
        distance->getDistanceValue ();

    // create a line width element
    Sxmlelement
      distanceElement =
        createMxmlelement (
          k_distance,
          S_msrLengthAsTenths (distanceValue));

    // set its "type" attribute
    std::string distanceTypeString;

    switch (distanceTypeKind) {
      case msrDistanceTypeKind::kDistanceType_NO_:
        distanceTypeString = "kDistanceType_NO_";
        break;
      case msrDistanceTypeKind::kDistanceTypeHyphen:
        distanceTypeString = "hyphen";
        break;
      case msrDistanceTypeKind::kDistanceTypeBeam:
        distanceTypeString = "beam";
        break;
    } // switch

    distanceElement->add (createMxmlAttribute ("type", distanceTypeString));

    // append the line width element to the appearance element
    appearanceElement->push (
      distanceElement);
  } // for
}

void msr2mxsrTranslator::populateAppearanceGlyphs (
  Sxmlelement             appearanceElement,
  const std::list<S_msrGlyph>& glyphsList)
{
  for (
    std::list<S_msrGlyph>::const_iterator i =
      glyphsList.begin ();
    i!=glyphsList.end ();
    ++i
  ) {
    S_msrGlyph glyph = (*i);

    // get line width type
    msrGlyphTypeKind
      glyphTypeKind =
        glyph->getGlyphTypeKind ();

    // get line width value
    std::string
      glyphValue =
        glyph->getGlyphValue ();

    // create a line width element
    Sxmlelement
      glyphElement =
        createMxmlelement (
          k_glyph,
          glyphValue);

    // set its "type" attribute
    std::string glyphTypeString;

    switch (glyphTypeKind) {
      case msrGlyphTypeKind::kGlyphType_NO_:
        glyphTypeString = "kGlyphType_NO_";
        break;
      case msrGlyphTypeKind::kGlyphTypeQuarterRest:
        glyphTypeString = "quarter-rest";
        break;
      case msrGlyphTypeKind::kGlyphTypeGClefOttavaBassa:
        glyphTypeString = "g-clef-ottava-bassa";
        break;
      case msrGlyphTypeKind::kGlyphTypeCClef:
        glyphTypeString = "c-clef";
        break;
      case msrGlyphTypeKind::kGlyphTypeFClef:
        glyphTypeString = "f-clef";
        break;
      case msrGlyphTypeKind::kGlyphTypePercussionClef:
        glyphTypeString = "percussion-clef";
        break;
      case msrGlyphTypeKind::kGlyphTypeOctaveShiftUp8:
        glyphTypeString = "octave-shift-up-8";
        break;
      case msrGlyphTypeKind::kGlyphTypeOctaveShiftDown8:
        glyphTypeString = "octave-shift-down-8";
        break;
      case msrGlyphTypeKind::kGlyphTypeOctaveShiftContinue8:
        glyphTypeString = "octave-shift-continue-8";
        break;
      case msrGlyphTypeKind::kGlyphTypeOctaveShiftDown15:
        glyphTypeString = "octave-shift-down-15";
        break;
      case msrGlyphTypeKind::kGlyphTypeOctaveShiftUp15:
        glyphTypeString = "octave-shift-up-15";
        break;
      case msrGlyphTypeKind::kGlyphTypeOctaveShiftContinue15:
        glyphTypeString = "octave-shift-continue-15";
        break;
      case msrGlyphTypeKind::kGlyphTypeOctaveShiftDown22:
        glyphTypeString = "octave-shift-down-22";
        break;
      case msrGlyphTypeKind::kGlyphTypeOctaveShiftUp22:
        glyphTypeString = "octave-shift-up-22";
        break;
      case msrGlyphTypeKind::kGlyphTypeOctaveShiftContinue22:
        glyphTypeString = "octave-shift-continue-22";
        break;
    } // switch

    glyphElement->add (createMxmlAttribute ("type", glyphTypeString));

    // append the line width element to the appearance element
    appearanceElement->push (
      glyphElement);
  } // for
}

void msr2mxsrTranslator::populateAppearanceOtherAppearances (
  Sxmlelement       appearanceElement,
  const std::list<S_msrOtherAppearance>&
                    otherAppearancesList)
{
  for (
    std::list<S_msrOtherAppearance>::const_iterator i =
      otherAppearancesList.begin ();
    i!=otherAppearancesList.end ();
    ++i
  ) {
    S_msrOtherAppearance otherAppearance = (*i);

    // get line width type
    msrOtherAppearanceTypeKind
      otherAppearanceTypeKind =
        otherAppearance->getOtherAppearanceTypeKind ();

    // get line width value
    std::string
      otherAppearanceValue =
        otherAppearance->getOtherAppearanceValue ();

    // create a line width element
    Sxmlelement
      otherAppearanceElement =
        createMxmlelement (
          k_other_appearance,
          otherAppearanceValue);

    // set its "type" attribute
    std::string otherAppearanceTypeString;

    switch (otherAppearanceTypeKind) {
      case msrOtherAppearanceTypeKind::kOtherAppearanceType_NO_:
        otherAppearanceTypeString = "kOtherAppearanceType_NO_";
        break;
    } // switch

    otherAppearanceElement->add (createMxmlAttribute (
      "type",
      otherAppearanceTypeString));

    // append the line width element to the appearance element
    appearanceElement->push (
      otherAppearanceElement);
  } // for
}

void msr2mxsrTranslator::visitStart (S_msrAppearance& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrAppearance" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

  // create an appearance element
  fScoreDefaultsAppearanceElement =
    createMxmlelement (k_appearance, "");

  // don't append it to the score defaults element yet,
  // this will be done in visitEnd (S_msrScore&)

  // append the line width elements if any to the appearance element
  const std::list<S_msrLineWidth>&
    lineWidthsList =
      elt->getLineWidthsList ();

  populateAppearanceLineWidths (
    fScoreDefaultsAppearanceElement,
    lineWidthsList);

  // append the note size elements if any to the appearance element
  const std::list<S_msrNoteSize>&
    noteSizesList =
      elt->getNoteSizesList ();

  populateAppearanceNoteSizes (
    fScoreDefaultsAppearanceElement,
    noteSizesList);

  // append the distance elements if any to the appearance element
  const std::list<S_msrDistance>&
    distancesList =
      elt->getDistancesList ();

  populateAppearanceDistances (
    fScoreDefaultsAppearanceElement,
    distancesList);

  // append the glyph elements if any to the appearance element
  const std::list<S_msrGlyph>&
    glyphsList =
      elt->getGlyphsList ();

  populateAppearanceGlyphs (
    fScoreDefaultsAppearanceElement,
    glyphsList);

  // append the other appearance elements if any to the appearance element
  const std::list<S_msrOtherAppearance>&
    otherAppearancesList =
      elt->getOtherAppearancesList ();

  populateAppearanceOtherAppearances (
    fScoreDefaultsAppearanceElement,
    otherAppearancesList);
}

void msr2mxsrTranslator::visitEnd (S_msrAppearance& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrAppearance" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif
}

//________________________________________________________________________
void msr2mxsrTranslator::visitStart (S_msrCredit& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrCredit" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

  // create a credit element
  fCurrentScoreCreditElement = createMxmlelement (k_credit, "");

  // set its "page" attribute
  fCurrentScoreCreditElement->add (
    createMxmlIntegerAttribute ("page", elt->getCreditPageNumber ()));

  // append the credit element to the credit elements pending std::list
  fPendingScoreCreditElementsList.push_back (fCurrentScoreCreditElement);
}

void msr2mxsrTranslator::visitEnd (S_msrCredit& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrCredit" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

  // forget about the current credit element
  fCurrentScoreCreditElement = nullptr;
}

void msr2mxsrTranslator::visitStart (S_msrCreditWords& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrCreditWords" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

  // create a credit words element
  Sxmlelement
    creditWordsElement =
      createMxmlelement (
        k_credit_words,
        elt->getCreditWordsContents ());

  // set its "default-x" attribute
  float
    creditWordsDefaultX =
      elt->getCreditWordsDefaultX ();
  if (creditWordsDefaultX > 0.0) {
    std::stringstream s;
    s << std::setprecision (8) << creditWordsDefaultX;
    creditWordsElement->add (createMxmlAttribute ("default-x", s.str ()));
  }

  // set its "default-y" attribute
  float
    creditWordsDefaultY =
      elt->getCreditWordsDefaultY ();
  if (creditWordsDefaultY > 0.0) {
    std::stringstream s;
    s << std::setprecision (8) << creditWordsDefaultY;
    creditWordsElement->add (createMxmlAttribute ("default-y", s.str ()));
  }

  // set its "font-family" attribute
  std::string
    creditWordsFontFamilyString =
      elt->getCreditWordsFontFamily ();

  if (creditWordsFontFamilyString.size ()) {
    creditWordsElement->add (
      createMxmlAttribute (
        "font-family",
        creditWordsFontFamilyString));
  }

  // set its "font-size" attribute
  float
    creditWordsFontSize =
      elt->getCreditWordsFontSize ();
  std::stringstream s;
  s << std::setprecision (2) << creditWordsFontSize;
  creditWordsElement->add (createMxmlAttribute ("font-size", s.str ()));

  // set its "font-weight" attribute
  std::string fontWeightString;

  switch (elt->getCreditWordsFontWeightKind ()) {
    case msrFontWeightKind::kFontWeightNone:
      break;
    case msrFontWeightKind::kFontWeightNormal:
      fontWeightString = "normal";
      break;
    case msrFontWeightKind::kFontWeightBold:
      fontWeightString = "bold";
      break;
    } // switch

  if (fontWeightString.size ()) {
    creditWordsElement->add (createMxmlAttribute ("font-weight", fontWeightString));
  }

  // set its "font-style" attribute
  std::string fontStyleString;

  switch (elt->getCreditWordsFontStyleKind ()) {
    case msrFontStyleKind::kFontStyleNone:
      break;
    case msrFontStyleKind::kFontStyleNormal:
      fontStyleString = "normal";
      break;
    case msrFontStyleKind::KFontStyleItalic:
      fontStyleString = "italic";
      break;
    } // switch

  if (fontStyleString.size ()) {
    creditWordsElement->add (createMxmlAttribute ("font-style", fontStyleString));
  }

  // set its "justify" attribute
  std::string justifyString;

  switch (elt->getCreditWordsJustifyKind ()) {
    case msrJustifyKind::kJustifyNone:
      break;
    case msrJustifyKind::kJustifyLeft:
      justifyString = "left";
      break;
    case msrJustifyKind::kJustifyCenter:
      justifyString = "center";
      break;
    case msrJustifyKind::kJustifyRight:
      justifyString = "right";
      break;
    } // switch

  if (justifyString.size ()) {
    creditWordsElement->add (createMxmlAttribute ("justify", justifyString));
  }

  // set its "halign" attribute
  std::string horizontalAlignmentString;

  switch (elt->getCreditWordsHorizontalAlignmentKind ()) {
    case msrHorizontalAlignmentKind::kHorizontalAlignmentNone:
      break;
    case msrHorizontalAlignmentKind::kHorizontalAlignmentLeft:
      horizontalAlignmentString = "left";
      break;
    case msrHorizontalAlignmentKind::kHorizontalAlignmentCenter:
      horizontalAlignmentString = "center";
      break;
    case msrHorizontalAlignmentKind::kHorizontalAlignmentRight:
      horizontalAlignmentString = "right";
      break;
    } // switch

  if (horizontalAlignmentString.size ()) {
    creditWordsElement->add (createMxmlAttribute ("halign", horizontalAlignmentString));
  }

  // set its "valign" attribute
  std::string verticalAlignmentString;

  switch (elt->getCreditWordsVerticalAlignmentKind ()) {
    case msrVerticalAlignmentKind::kVerticalAlignmentNone:
      break;
    case msrVerticalAlignmentKind::kVerticalAlignmentTop:
      verticalAlignmentString = "top";
      break;
    case msrVerticalAlignmentKind::kVerticalAlignmentMiddle:
      verticalAlignmentString = "middle";
      break;
    case msrVerticalAlignmentKind::kVerticalAlignmentBottom:
      verticalAlignmentString = "bottom";
      break;
    } // switch

  if (verticalAlignmentString.size ()) {
    creditWordsElement->add (createMxmlAttribute ("valign", verticalAlignmentString));
  }

  // set its "xml:lang" attribute
  creditWordsElement->add (
    createMxmlAttribute (
      "xml:lang",
      msrXMLLangKindAsString (elt->getCreditWordsXMLLang ())));

  // append it to the current credit element
  fCurrentScoreCreditElement->push (creditWordsElement);
}

void msr2mxsrTranslator::visitEnd (S_msrCreditWords& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrCreditWords" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif
}

//________________________________________________________________________
void msr2mxsrTranslator::visitStart (S_msrPartGroup& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrPartGroup " <<
      elt->getPartGroupCombinedName () <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  switch (elt->getPartGroupImplicitKind ()) {
    case msrPartGroupImplicitKind::kPartGroupImplicitYes:
      // this is an MSR concept, ignore this part group
      break;

    case msrPartGroupImplicitKind::kPartGroupImplicitNo:
      {
        if (gGlobalMsr2mxsrOahGroup->getMusicXMLComments ()) {
          // create a start comment
          std::stringstream s;
          s <<
            " ========== " <<
            elt->getPartGroupCombinedName () <<
            " START" <<
              ", line " << inputLineNumber <<
            " ========== ";
          Sxmlelement comment = createMxmlelement (kComment, s.str ());

          // append it to the current part std::list element
          fScorePartListElement->push (comment);
        }

        // create a part group element
        Sxmlelement scorePartGroupElement = createMxmlelement (k_part_group, "");

        // set it's "number" attribute
        scorePartGroupElement->add (
          createMxmlIntegerAttribute (
            "number", elt->getPartGroupNumber ()));

        // set it's "type" attribute
        scorePartGroupElement->add (createMxmlAttribute ("type", "start"));

        // create a group symbol element to the part group element if relevant
        std::string groupSymbolString;

        switch (elt->getPartGroupSymbolKind ()) {
          case msrPartGroupSymbolKind::kPartGroupSymbolNone:
            break;
          case msrPartGroupSymbolKind::kPartGroupSymbolBrace:
            groupSymbolString = "brace";
            break;
          case msrPartGroupSymbolKind::kPartGroupSymbolBracket:
            groupSymbolString = "bracket";
            break;
          case msrPartGroupSymbolKind::kPartGroupSymbolLine:
            groupSymbolString = "line";
            break;
          case msrPartGroupSymbolKind::kPartGroupSymbolSquare:
            groupSymbolString = "square";
            break;
        } // switch

        if (groupSymbolString.size ()) {
          Sxmlelement groupSymbolElement =
            createMxmlelement (
              k_group_symbol,
              groupSymbolString);

          // set its "default-x" attribute if relevant
          int
            partGroupSymbolDefaultX  =
              elt->getPartGroupSymbolDefaultX ();

          if (partGroupSymbolDefaultX != INT_MIN) { // JMI superfluous???
            groupSymbolElement->add (
              createMxmlIntegerAttribute (
                "default-x",
                partGroupSymbolDefaultX));
          }

          // append it to the part group element
          scorePartGroupElement->push (groupSymbolElement);
        }

        // append a group name element to the part group element
        std::string
          groupName = elt->getPartGroupName ();

        if (groupName.size ()) {
          scorePartGroupElement->push (
            createMxmlelement (
              k_group_name,
              groupName));
        }

        // append a group barLine element to the part group element
        std::string groupBarLineString;

        switch (elt->getPartGroupBarLineKind ()) {
          case msrPartGroupBarLineKind::kPartGroupBarLineYes:
            groupBarLineString = "yes";
            break;
          case msrPartGroupBarLineKind::kPartGroupBarLineNo:
            groupBarLineString = "no";
            break;
        } // switch

        scorePartGroupElement->push (
          createMxmlelement (
            k_group_barline,
            groupBarLineString));

        // append the part group element to the part std::list element
        fScorePartListElement->push (scorePartGroupElement);

        // push the part group element onto the std::stack
        fPartGroupElementsStack.push (scorePartGroupElement);
      }
      break;
  } // switch
}

void msr2mxsrTranslator::visitEnd (S_msrPartGroup& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrPartGroup " <<
      elt->getPartGroupCombinedName () <<
      ", line " << inputLineNumber <<
      std::endl;

    if (false) { // JMI TEMP
      gLogStream <<
        std::endl <<
        "--> partgroup elt:" <<
        std::endl <<
        elt <<
        std::endl << std::endl;
    }
  }
#endif

  switch (elt->getPartGroupImplicitKind ()) {
    case msrPartGroupImplicitKind::kPartGroupImplicitYes:
      // this is an MSR concept, ignore this part group
      break;

    case msrPartGroupImplicitKind::kPartGroupImplicitNo:
      {
        if (gGlobalMsr2mxsrOahGroup->getMusicXMLComments ()) {
          // create an end comment
          std::stringstream s;
          s <<
            " ========== " <<
            elt->getPartGroupCombinedName () <<
            " END" <<
              ", line " << inputLineNumber <<
            " ========== ";
          Sxmlelement comment = createMxmlelement (kComment, s.str ());

          // append it to the current part std::list element
          fScorePartListElement->push (comment);
        }

        // fetch the top-most part group element on the std::stack
        Sxmlelement
          partGroupElementsStackTop =
            fPartGroupElementsStack.top ();

/* JMI ???
        // sanity check
        if (elt != partGroupElementsStackTop) {
          s <<
            "elt " <<
            mxmlelementAsString (elt) <<
            " and partGroupElementsStackTop " <<
            mxmlelementAsString (partGroupElementsStackTop) <<
            " are different" <<
            ", line " << inputLineNumber;

          msr2mxsrInternalError (
            gGlobalServiceRunData->getInputSourceName (),
            inputLineNumber,
            __FILE__, __LINE__,
            s.str ());
        }
*/
        // create a part group element
        Sxmlelement scorePartGroupElement = createMxmlelement (k_part_group, "");

        // set it's "number" attribute
        scorePartGroupElement->add (
          createMxmlIntegerAttribute (
            "number", elt->getPartGroupNumber ()));

        // set it's "type" attribute
        scorePartGroupElement->add (createMxmlAttribute ("type", "stop"));

        // append the part group element to the part std::list element
        fScorePartListElement->push (scorePartGroupElement);

        // pop the part group element from the std::stack
        fPartGroupElementsStack.pop ();
      }
      break;
  } // switch
}

void msr2mxsrTranslator::visitStart (S_msrPart& elt)
{
#ifdef TRACING_IS_ENABLED
  int inputLineNumber =
    elt->getInputLineNumber ();
#endif

if (false) // JMI
  gLogStream << elt << std::endl;

  std::string
    partID =
      elt->getPartID (),
    partName =
      elt->getPartName (),
    partAbbreviation =
      elt->getPartAbbreviation (),
    partCombinedName =
      elt->getPartCombinedName ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrPart " <<
      partCombinedName <<
      ", line " << inputLineNumber <<
      std::endl;

    if (false) { // JMI TEMP
      gLogStream <<
        std::endl <<
        "--> part elt:" <<
        std::endl <<
        elt <<
        std::endl << std::endl;
    }
  }
#endif

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceParts ()) {
    gLogStream <<
      std::endl <<
      "<!--=== part \"" << partCombinedName << "\"" <<
      ", line " << inputLineNumber << " ===-->" <<
      std::endl;
  }
#endif

  fCurrentMSRPart = elt;

  ++gIndenter;

  // create a score part element
  fScorePartElement = createMxmlelement (k_score_part, "");
  // set it's "id" attribute
  fScorePartElement->add (createMxmlAttribute ("id", partID));

  // append it to the part std::list element
  fScorePartListElement->push (fScorePartElement);

  // append a part name element to the score part element
  fScorePartElement->push (
    createMxmlelement (
      k_part_name,
      partName));

  // append a part abbreviation element to the score part element
  if (partAbbreviation.size ()) {
    fScorePartElement->push (
      createMxmlelement (
        k_part_abbreviation,
        partAbbreviation));
  }

  // append the score instrument element if any to the score part element
  if (fScoreInstrumentElement) {
    // set its id
    std::string
      partID = fCurrentMSRPart->getPartID ();
    fScoreInstrumentElement->add (createMxmlAttribute ("id", partID + "I1"));

    // append it to the score instrument element
    fScorePartElement->push (
      fScoreInstrumentElement);
  }

  // create a part element
  fCurrentPartElement = createMxmlelement (k_part, "");
  // set its "id" attribute
  fCurrentPartElement->add (createMxmlAttribute ("id", partID));

  // append it to the pending part elements list
  fPendingPartElementsList.push_back (fCurrentPartElement);

  // get the part shortest note's duration
  fPartShortestNoteDuration =
    elt->getPartShortestNoteDuration ();
    /* JMI ???
  fPartShortestNoteTupletFactor =
    elt->getPartShortestNoteTupletFactor ();

  // compute the divisions per quarter note
  if (fPartShortestNoteDuration > Rational (1, 4)) {
    // the shortest duration should be a quarter note at most
    fPartShortestNoteDuration = Rational (1, 4);
  }
*/

/*
  Rational
    partShortestNoteTupletFactorAsRational =
      fPartShortestNoteTupletFactor.asRational ();
*/

  Rational
    divisionsPerQuarterNoteAsRational =
      Rational (1, 4)
       /
      fPartShortestNoteDuration;

    /* JMI
  Rational
    rationalDivisionsPerQuarterNote =
      divisionsPerQuarterNoteAsRational
        *
      partShortestNoteTupletFactorAsRational;
      divisionsPerQuarterNoteAsRational.getNumerator ()
        *
      divisionsPerQuarterNoteAsRational.getDenominator ();
    */

  // compute divisions per quarter note and multiplying factor
  fDivisionsPerQuarterNote =
    divisionsPerQuarterNoteAsRational.getNumerator ();
  fDivisionsMultiplyingFactor =
    divisionsPerQuarterNoteAsRational.getDenominator ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceDivisions ()) {
    gLogStream <<
      "--> partShortestNoteDuration: " <<
      fPartShortestNoteDuration <<
      std::endl <<
      "--> divisionsPerQuarterNoteAsRational: " <<
      divisionsPerQuarterNoteAsRational <<
      std::endl <<
      /* JMI
      "--> partShortestNoteTupletFactor: " <<
      fPartShortestNoteTupletFactor.asString () <<
      std::endl <<
      "--> partShortestNoteTupletFactorAsRational: " <<
      partShortestNoteTupletFactorAsRational <<
      std::endl <<
      */
      "--> divisionsPerQuarterNote: " <<
      fDivisionsPerQuarterNote <<
      std::endl <<
      "--> divisionsMultiplyingFactor: " <<
      fDivisionsMultiplyingFactor <<
      std::endl;
  }
#endif

/* JMI SUPERFLUOUS ???
  if (fDivisionsMultiplyingFactor.getDenominator () != 1) {
    std::stringstream s;

    s <<
      "divisionsMultiplyingFactor '" << fDivisionsMultiplyingFactor <<
      "' is no integer number" <<
      ", line " << inputLineNumber;

    msr2mxsrInternalError (
// JMI    msrInternalWarning (
      gGlobalServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }
*/

  // a divisions element has to be appended for this part
  fPartDivisionsElementHasToBeAppended = true;

  // should a <staves /> element be created?
  int partStavesNumber =
    fCurrentMSRPart->getPartStaveNumbersToStavesMap ().size ();
  if (partStavesNumber > 1) {
    // create a staves element
    fStavesElement =
      createMxmlIntegerElement (
        k_staves,
        partStavesNumber);
  }
}

void msr2mxsrTranslator::visitEnd (S_msrPart& elt)
{
#ifdef TRACING_IS_ENABLED
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrPart " <<
      elt->getPartCombinedName () <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  --gIndenter;

  // forget about measure elements in the std::map
  fPartMeasureNumbersToElementsMap.clear ();

  // forget about the current part element
  fCurrentPartElement = nullptr;

  // forget about the current part clef
  fCurrentPartClef = nullptr;

  // forget about the current part key
  fCurrentPartKey = nullptr;

  // forget about the current part time
  fCurrentPartTimeSignature = nullptr;
}

//________________________________________________________________________
void msr2mxsrTranslator::visitStart (S_msrStaff& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrStaff \"" <<
      elt->getStaffName () << "\"" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }

    if (false) { // JMI TEMP
      gLogStream <<
        std::endl <<
        "--> staff elt:" <<
        std::endl <<
        elt <<
        std::endl << std::endl;
    }
#endif

/*
  ++gIndenter;

  switch (elt->getStaffKind ()) {
    case msrStaffKind::kStaffKindRegular:
    case msrStaffKind::kStaffKindTablature:
    case msrStaffKind::kStaffKindDrum:
    case msrStaffKind::kStaffKindRythmic:
      {
        // create a staff clone
        fCurrentStaffClone =
          elt->createStaffNewbornClone (
            fCurrentPartClone);

        // add it to the part clone
        fCurrentPartClone->
          addStaffToPartCloneByItsNumber (
            fCurrentStaffClone);

        // create a staff block
        fCurrentStaffBlock =
          lpsrStaffBlock::create (
            fCurrentStaffClone);

        std::string
          partName =
            fCurrentPartClone->getPartName (),
          partAbbreviation =
            fCurrentPartClone->getPartAbbreviation ();

        std::string staffBlockInstrumentName;
        std::string staffBlockShortInstrumentName;

        // don't set instrument name nor short instrument name // JMI
        // if the staff belongs to a piano part where they're already set
        if (! partName.size ()) {
          staffBlockInstrumentName = partName;
        }
        if (! partAbbreviation.size ()) {
          staffBlockShortInstrumentName = partAbbreviation;
        }

        if (staffBlockInstrumentName.size ()) {
          fCurrentStaffBlock->
            setStaffBlockInstrumentName (staffBlockInstrumentName);
        }

        if (staffBlockShortInstrumentName.size ()) {
          fCurrentStaffBlock->
            setStaffBlockShortInstrumentName (staffBlockShortInstrumentName);
        }

        // append the staff block to the current part block
        fCurrentPartBlock->
          appendStaffBlockToPartBlock (
            fCurrentStaffBlock);

        fOnGoingStaff = true;
      }
      break;

    case msrStaffKind::kStaffKindHarmonies:
      {
        // create a staff clone
        fCurrentStaffClone =
          elt->createStaffNewbornClone (
            fCurrentPartClone);

        // add it to the part clone
        fCurrentPartClone->
          addStaffToPartCloneByItsNumber (
            fCurrentStaffClone);

        fOnGoingStaff = true;
      }
      break;

    case msrStaffKind::kStaffKindFiguredBass:
      {
        // create a staff clone
        fCurrentStaffClone =
          elt->createStaffNewbornClone (
            fCurrentPartClone);

        // add it to the part clone
        fCurrentPartClone->
          addStaffToPartCloneByItsNumber (
            fCurrentStaffClone);

        // register it as the part figured bass staff
        fCurrentPartClone->
          setPartFiguredBassStaff (
            fCurrentStaffClone);

        fOnGoingStaff = true;
      }
      break;
  } // switch
  */
}

void msr2mxsrTranslator::visitEnd (S_msrStaff& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting S_msrStaff \"" <<
      elt->getStaffName () << "\"" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

/*
  --gIndenter;

  switch (elt->getStaffKind ()) {
    case msrStaffKind::kStaffKindRegular:
    case msrStaffKind::kStaffKindDrum:
    case msrStaffKind::kStaffKindRythmic:
      {
        fOnGoingStaff = false;
      }
      break;

    case msrStaffKind::kStaffKindTablature:
      // JMI
      break;

    case msrStaffKind::kStaffKindHarmonies:
      // JMI
      break;

    case msrStaffKind::kStaffKindFiguredBass:
      // JMI
      break;
  } // switch
  */
}

//________________________________________________________________________
void msr2mxsrTranslator::visitStart (S_msrVoice& elt)
{
#ifdef TRACING_IS_ENABLED
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrVoice \"" <<
      elt->asString () << "\"" <<
      ", line " << inputLineNumber <<
      std::endl;
  }

    if (false) { // JMI TEMP
      gLogStream <<
        std::endl <<
        "--> voice elt:" <<
        std::endl <<
        elt <<
        std::endl << std::endl;
    }
#endif

/*
  ++gIndenter;
*/
}

void msr2mxsrTranslator::visitEnd (S_msrVoice& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrVoice \"" <<
      elt->getVoiceName () << "\"" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

/*
  --gIndenter;

  switch (elt->getVoiceKind ()) {
    case msrVoiceKind::kVoiceKindRegular:
      // JMI
      break;

    case msrVoiceKind::kVoiceKindDynamics:
      break;

    case msrVoiceKind::kVoiceKindHarmonies:
      fOnGoingHarmoniesVoice = false;
      break;

    case msrVoiceKind::kVoiceKindFiguredBass:
      fOnGoingFiguredBassVoice = false;
      break;
  } // switch
  */
}

//________________________________________________________________________
void msr2mxsrTranslator::visitStart (S_msrSegment& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber () ;

#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrSegment '" <<
      elt->getSegmentAbsoluteNumber () << "'" <<
      ", line " << inputLineNumber <<
      std::endl;

    if (false) { // JMI TEMP
      gLogStream <<
        std::endl <<
        "--> segment elt:" <<
        std::endl <<
        elt <<
        std::endl << std::endl;
    }
  }
#endif

  if (gGlobalMsr2mxsrOahGroup->getMusicXMLComments ()) {
    // create a start comment
    std::stringstream s;
    s <<
      " ==================== " <<
      "Segment " <<
      elt->getSegmentAbsoluteNumber () <<
      " START" <<
        ", line " << inputLineNumber <<
      " ==================== ";
    Sxmlelement comment = createMxmlelement (kComment, s.str ());

    // append it to the current part element
    fCurrentPartElement->push (comment);
  }
}

void msr2mxsrTranslator::visitEnd (S_msrSegment& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrSegment '" <<
      elt->getSegmentAbsoluteNumber () << "'" <<
      ", line " << inputLineNumber <<
      std::endl;

    if (false) { // JMI TEMP
      gLogStream <<
        std::endl <<
        "--> segment elt:" <<
        std::endl <<
        elt <<
        std::endl << std::endl;
    }
  }
#endif

  if (gGlobalMsr2mxsrOahGroup->getMusicXMLComments ()) {
    // create an end comment
    std::stringstream s;
    s <<
      " ==================== " <<
      "Segment " <<
      elt->getSegmentAbsoluteNumber () <<
      " END" <<
        ", line " << inputLineNumber <<
      " ==================== ";
    Sxmlelement comment = createMxmlelement (kComment, s.str ());

    // append it to the current part element
    fCurrentPartElement->push (comment);
  }
}

//________________________________________________________________________
void msr2mxsrTranslator::visitStart (S_msrMeasure& elt)
{
  int
    inputLineNumber =
      elt->getInputLineNumber ();

  std::string
    measureNumber =
      elt->getMeasureNumber ();

#ifdef TRACING_IS_ENABLED
  int
    measurePuristNumber =
      elt->getMeasurePuristNumber ();

  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrMeasure '" <<
      measureNumber <<
      "', measurePuristNumber = '" <<
      measurePuristNumber <<
      "', line " << inputLineNumber <<
      std::endl;

    if (false) { // JMI TEMP
      gLogStream <<
        std::endl <<
        "--> measure elt:" <<
        std::endl <<
        elt <<
        std::endl << std::endl;
    }
  }
#endif

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasures ()) {
    gLogStream <<
      std::endl <<
      "<!--=== measure '" << measureNumber <<
//      "', voice \"" <<
//      fCurrentVoiceClone->getVoiceName () <<
      "\"" <<
      ", line " << inputLineNumber << " ===-->" <<
      std::endl;
  }
#endif

  // has a measure element for this measure number been created already?
  std::map<std::string, Sxmlelement>::iterator
    it =
      fPartMeasureNumbersToElementsMap.find (measureNumber);

  if (it != fPartMeasureNumbersToElementsMap.end ()) {
    // yes, use existing measure element
    fCurrentMeasureElement = (*it).second;
  }
  else {
    // no

    if (gGlobalMsr2mxsrOahGroup->getMusicXMLComments ()) {
      // create a comment
      std::stringstream s;
      s <<
        " ===== " <<
        "MEASURE " <<
        "ordinal number: " << elt->getMeasureOrdinalNumberInVoice () <<
        ", line " << inputLineNumber <<
        " ===== ";
      Sxmlelement comment = createMxmlelement (kComment, s.str ());

      // append it to the current part element
      fCurrentPartElement->push (comment);
    }

    // create a measure element
    fCurrentMeasureElement = createMxmlelement (k_measure, "");
    // set its "number" attribute
    fCurrentMeasureElement->add (createMxmlAttribute ("number", measureNumber));

    // append it to the current part element
    fCurrentPartElement->push (fCurrentMeasureElement);

    // register it in the part measures std::map
    fPartMeasureNumbersToElementsMap [measureNumber] = fCurrentMeasureElement;
  }

  // is there a print element to be appended?
  S_msrMusicXMLPrintLayout
    measureMusicXMLPrintLayout =
      elt->getMeasureMusicXMLPrintLayout ();

  if (measureMusicXMLPrintLayout) {
    if (gGlobalMsr2mxsrOahGroup->getMusicXMLComments ()) {
      // create a print comment
      std::stringstream s;
      s <<
        " ===== " <<
        "Print" <<
        ", line " << inputLineNumber <<
        " ===== ";
      Sxmlelement comment = createMxmlelement (kComment, s.str ());

      // append it to the current measure element
      fCurrentMeasureElement->push (comment);
    }

    // create a print element
    fCurrentPrintElement = createMxmlelement (k_print, "");

    // append it to the current measure element at once,
    // since is must be the first one in the measure
    fCurrentMeasureElement->push (fCurrentPrintElement);
  }

  // is there a divisions element to be appended?
  if (fPartDivisionsElementHasToBeAppended) {
    // append a divisions element to the attributes element
    fDivisionsElement =
      createMxmlIntegerElement (
        k_divisions,
        fDivisionsPerQuarterNote);
    fAnAttributeElementIsNeeded = true;

    fPartDivisionsElementHasToBeAppended = false;
  }

if (false) { // JMI
  // there's no previous MSR note yet in this measure
#ifdef TRACING_IS_ENABLED
    if (
      gGlobalMxsrOahGroup->getTraceBackup ()
        ||
      gGlobalMxsrOahGroup->getTraceForward ()
    ) {
      gLogStream <<
        "--> setting previous note  to nullptr at " <<
        elt->asString () <<
        " at beginning of measure '" <<
        elt->getMeasurePuristNumber () <<
        "', line " << inputLineNumber <<
        std::endl;
    }
#endif

  fPreviousMSRNote = nullptr;
}

  fANoteHasBeenMetInCurrentMeasure = false;
}

void msr2mxsrTranslator::visitEnd (S_msrMeasure& elt)
{
#ifdef TRACING_IS_ENABLED
  int inputLineNumber =
    elt->getInputLineNumber ();
#endif

  std::string
    measureNumber =
      elt->getMeasureNumber ();

  std::string
    nextMeasureNumber =
      elt->getNextMeasureNumber ();

#ifdef TRACING_IS_ENABLED
  int
    measurePuristNumber =
      elt->getMeasurePuristNumber ();

  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrMeasure '" <<
      measureNumber <<
      "', nextMeasureNumber = '" <<
      nextMeasureNumber <<
      "', measurePuristNumber = '" <<
      measurePuristNumber <<
      "', line " << inputLineNumber <<
      std::endl;
  }
#endif

  // forget about the current measure element
  fCurrentMeasureElement = nullptr;

  // reset the current position in the measure
  fCurrentMeasurePosition = Rational (0, 1);

  // reset the cumulated skip durations informations
  fCurrentCumulatedSkipsDurations = Rational (0, 1);
  fCurrentCumulatedSkipsStaffNumber = -1;
  fCurrentCumulatedSkipsVoiceNumber = -1;
}

//________________________________________________________________________
void msr2mxsrTranslator::visitStart (S_msrMusicXMLPrintLayout& elt)
{
#ifdef TRACING_IS_ENABLED
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrMusicXMLPrintLayout '" <<
      "', line " << inputLineNumber <<
      std::endl;
  }
#endif

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMusicXMLPrintLayouts ()) {
    gLogStream <<
      std::endl <<
//      "', voice \"" <<
//      fCurrentVoiceClone->getVoiceName () <<
      "\"" <<
      ", line " << inputLineNumber << " ===-->" <<
      std::endl;
  }
#endif

  // don't create a print element,
  // this has already been done in visitStart (S_msrMeasure&)

  // populate the current print element
  float staffSpacing = elt->getStaffSpacing ();
  if (staffSpacing > 0) {
    std::stringstream s;
    s << staffSpacing;
    fCurrentPrintElement->add (createMxmlAttribute ("staff-spacing", s.str ()));
  }

  Bool newSystem = elt->getNewSystem ();
  if (newSystem) {
    fCurrentPrintElement->add (createMxmlAttribute ("new-system", "yes"));
  }

  Bool newPage = elt->getNewPage ();
  if (newPage) {
    fCurrentPrintElement->add (createMxmlAttribute ("new-page", "yes"));
  }

  int blankPage = elt->getBlankPage ();
  if (blankPage > 0) {
    std::stringstream s;
    s << blankPage;
    fCurrentPrintElement->add (createMxmlAttribute ("blank-page", s.str ()));
  }

  int pageNumber = elt->getPageNumber ();
  if (pageNumber > 0) {
    std::stringstream s;
    s << pageNumber;
    fCurrentPrintElement->add (createMxmlAttribute ("page-number", s.str ()));
  }

  fOnGoingMusicXMLPrintLayout = true;
}

void msr2mxsrTranslator::visitEnd (S_msrMusicXMLPrintLayout& elt)
{
#ifdef TRACING_IS_ENABLED
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrMusicXMLPrintLayout '" <<
      "', line " << inputLineNumber <<
      std::endl;
  }
#endif

  // forget about the current print layout element
  fCurrentPrintElement = nullptr;

  fOnGoingMusicXMLPrintLayout = false;
}

//________________________________________________________________________
void msr2mxsrTranslator::visitStart (S_msrClef& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrClef " <<
      elt->asString () <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

  Bool doAppendAClefElementToTheMeasure (false);

  if (fCurrentPartClef) {
    // a clef element has already been created for the current part:
    // each voice in an MSR staff has its own clefs,
    // another one should be created only if it is different the current one
    doAppendAClefElementToTheMeasure =
      ! elt->isEqualTo (fCurrentPartClef);
  }
  else {
    // this the first clef met in the part
    doAppendAClefElementToTheMeasure = true;
  }

  if (doAppendAClefElementToTheMeasure) {
#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceClefs ()) {
      gLogStream <<
        "Creating an MXSR clef element for msrClef " <<
        elt->asString () <<
        ", line " << elt->getInputLineNumber () <<
        std::endl;
    }
#endif

    Sxmlelement clefElement = createMxmlelement (k_clef, "");

    // set clefElement's "number" attribute if relevant
    /*
      0 by default in MSR,
      1 by default in MusicXML:
        The optional  number attribute refers to staff numbers within the part,
        from top to bottom on the system.
        A value of 1 is assumed if not present.
    */

    int clefStaffNumber =
      elt->getClefStaffNumber ();

    if (clefStaffNumber > 1) {
      clefElement->add (
        createMxmlIntegerAttribute ("number", clefStaffNumber));
    }

    // populate clefElement
    switch (elt->getClefKind ()) {
      case msrClefKind::kClef_NO_:
        {
          clefElement->push (
            createMxmlelement (
              k_sign,
              "[NONE]"));
        }
        break;
      case msrClefKind::kClefTreble:
        {
          clefElement->push (
            createMxmlelement (
              k_sign,
              "G"));
          clefElement->push (
            createMxmlIntegerElement (
              k_line,
              2));
        }
        break;
      case msrClefKind::kClefSoprano:
        {
          clefElement->push (
            createMxmlelement (
              k_sign,
              "C"));
          clefElement->push (
            createMxmlIntegerElement (
              k_line,
              1));
        }
        break;
      case msrClefKind::kClefMezzoSoprano:
        {
          clefElement->push (
            createMxmlelement (
              k_sign,
              "C"));
          clefElement->push (
            createMxmlIntegerElement (
              k_line,
              2));
        }
        break;
      case msrClefKind::kClefAlto:
        {
          clefElement->push (
            createMxmlelement (
              k_sign,
              "C"));
          clefElement->push (
            createMxmlIntegerElement (
              k_line,
              3));
        }
        break;
      case msrClefKind::kClefTenor:
        {
          clefElement->push (
            createMxmlelement (
              k_sign,
              "C"));
          clefElement->push (
            createMxmlIntegerElement (
              k_line,
              4));
        }
        break;
      case msrClefKind::kClefBaritone:
        {
          clefElement->push (
            createMxmlelement (
              k_sign,
              "C"));
          clefElement->push (
            createMxmlIntegerElement (
              k_line,
              5));
        }
        break;
      case msrClefKind::kClefBass:
        {
          clefElement->push (
            createMxmlelement (
              k_sign,
              "F"));
          clefElement->push (
            createMxmlIntegerElement (
              k_line,
              4));
        }
        break;
      case msrClefKind::kClefTrebleLine1:
        {
          clefElement->push (
            createMxmlelement (
              k_sign,
              "G"));
          clefElement->push (
            createMxmlIntegerElement (
              k_line,
              1));
        }
        break;
      case msrClefKind::kClefTrebleMinus15:
        {
          clefElement->push (
            createMxmlelement (
              k_sign,
              "G"));
          clefElement->push (
            createMxmlIntegerElement (
              k_line,
              2));
          clefElement->push (
            createMxmlIntegerElement (
              k_clef_octave_change,
              -2));
        }
        break;
      case msrClefKind::kClefTrebleMinus8:
        {
          clefElement->push (
            createMxmlelement (
              k_sign,
              "G"));
          clefElement->push (
            createMxmlIntegerElement (
              k_line,
              2));
          clefElement->push (
            createMxmlIntegerElement (
              k_clef_octave_change,
              -1));
        }
        break;
      case msrClefKind::kClefTreblePlus8:
        {
          clefElement->push (
            createMxmlelement (
              k_sign,
              "G"));
          clefElement->push (
            createMxmlIntegerElement (
              k_line,
              2));
          clefElement->push (
            createMxmlIntegerElement (
              k_clef_octave_change,
              1));
        }
        break;
      case msrClefKind::kClefTreblePlus15:
        {
          clefElement->push (
            createMxmlelement (
              k_sign,
              "G"));
          clefElement->push (
            createMxmlIntegerElement (
              k_line,
              2));
          clefElement->push (
            createMxmlIntegerElement (
              k_clef_octave_change,
              2));
        }
        break;
      case msrClefKind::kClefBassMinus15:
        {
          clefElement->push (
            createMxmlelement (
              k_sign,
              "F"));
          clefElement->push (
            createMxmlIntegerElement (
              k_line,
              4));
          clefElement->push (
            createMxmlIntegerElement (
              k_clef_octave_change,
              -2));
        }
        break;
      case msrClefKind::kClefBassMinus8:
        {
          clefElement->push (
            createMxmlelement (
              k_sign,
              "F"));
          clefElement->push (
            createMxmlIntegerElement (
              k_line,
              4));
          clefElement->push (
            createMxmlIntegerElement (
              k_clef_octave_change,
              -1));
        }
        break;
      case msrClefKind::kClefBassPlus8:
        {
          clefElement->push (
            createMxmlelement (
              k_sign,
              "F"));
          clefElement->push (
            createMxmlIntegerElement (
              k_line,
              4));
          clefElement->push (
            createMxmlIntegerElement (
              k_clef_octave_change,
              1));
        }
        break;
      case msrClefKind::kClefBassPlus15:
        {
          clefElement->push (
            createMxmlelement (
              k_sign,
              "F"));
          clefElement->push (
            createMxmlIntegerElement (
              k_line,
              4));
          clefElement->push (
            createMxmlIntegerElement (
              k_clef_octave_change,
              2));
        }
        break;
      case msrClefKind::kClefVarbaritone:
        {
          clefElement->push (
            createMxmlelement (
              k_sign,
              "F"));
          clefElement->push (
            createMxmlIntegerElement (
              k_line,
              3));
        }
        break;

      case msrClefKind::kClefTablature4:
        {
          clefElement->push (
            createMxmlelement (
              k_sign,
              "tab"));
          clefElement->push (
            createMxmlIntegerElement (
              k_line,
              4));
        }
      case msrClefKind::kClefTablature5:
        {
          clefElement->push (
            createMxmlelement (
              k_sign,
              "tab"));
          clefElement->push (
            createMxmlIntegerElement (
              k_line,
              5));
        }
        break;
      case msrClefKind::kClefTablature6:
        {
          clefElement->push (
            createMxmlelement (
              k_sign,
              "tab"));
          clefElement->push (
            createMxmlIntegerElement (
              k_line,
              6));
        }
      case msrClefKind::kClefTablature7:
        {
          clefElement->push (
            createMxmlelement (
              k_sign,
              "tab"));
          clefElement->push (
            createMxmlIntegerElement (
              k_line,
              7));
        }
        break;

      case msrClefKind::kClefPercussion:
        {
          clefElement->push (
            createMxmlelement (
              k_sign,
              "percussion"));
        }
        break;

      case msrClefKind::kClefJianpu:
        {
          clefElement->push (
            createMxmlelement (
              k_sign,
              "jianpu"));
        }
        break;
    } // switch

    // append the clef element to the measure attributes element
    fClefElementsList.push_back (clefElement);
    fAnAttributeElementIsNeeded = true;

    // this clef becomes the new current part clef
    fCurrentPartClef = elt;
  }
}

void msr2mxsrTranslator::visitEnd (S_msrClef& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrClef" <<
      elt->asString () <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif
}

//________________________________________________________________________
void msr2mxsrTranslator::visitStart (S_msrKey& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrKey " <<
      elt->asString () <<
      ", fCurrentPartKey: ";

    if (fCurrentPartKey) {
      gLogStream <<
        fCurrentPartKey;
    }
    else {
      gLogStream << "[NONE]";
    }

    gLogStream <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  Bool doAppendAKeyElementToTheMeasure (false);

  if (fCurrentPartKey) {
    // a key element has already been created for the current part:
    // each voice in an MSR staff has its own keys,
    // another one should be created only if it is different the current one
    doAppendAKeyElementToTheMeasure =
      ! elt->isEqualTo (fCurrentPartKey);
  }
  else {
    // this the first key met in the part
    doAppendAKeyElementToTheMeasure = true;
  }

  if (doAppendAKeyElementToTheMeasure) {
#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceKeys ()) {
      gLogStream <<
        "Creating an MXSR key element for msrKey " <<
        elt->asString () <<
        ", line " << elt->getInputLineNumber () <<
        std::endl;
    }
#endif

    // create the key element
    fKeyElement = createMxmlelement (k_key, "");

    // populate it
    switch (elt->getKeyKind ()) {
      case msrKeyKind::kKeyTraditional:
        {
          // compute the number of fifths
          const int K_NO_FIFTHS_NUMBER = -99;
          int       fifthsNumber = K_NO_FIFTHS_NUMBER;

          msrQuarterTonesPitchKind
            keyTonicQuarterTonesPitchKind =
              elt->getKeyTonicQuarterTonesPitchKind ();

          switch (keyTonicQuarterTonesPitchKind) {
            case msrQuarterTonesPitchKind::kQTP_NO_:
            case msrQuarterTonesPitchKind::kQTP_Rest:
            case msrQuarterTonesPitchKind::kQTP_Skip:
              // should not occur
              break;

            case msrQuarterTonesPitchKind::kQTP_C_Natural:
              fifthsNumber = 0;
              break;
            case msrQuarterTonesPitchKind::kQTP_G_Natural:
              fifthsNumber = 1;
              break;
            case msrQuarterTonesPitchKind::kQTP_D_Natural:
              fifthsNumber = 2;
              break;
            case msrQuarterTonesPitchKind::kQTP_A_Natural:
              fifthsNumber = 3;
              break;
            case msrQuarterTonesPitchKind::kQTP_E_Natural:
              fifthsNumber = 4;
              break;
            case msrQuarterTonesPitchKind::kQTP_B_Natural:
              fifthsNumber = 5;
              break;
            case msrQuarterTonesPitchKind::kQTP_F_Sharp:
              fifthsNumber = 6;
              break;
            case msrQuarterTonesPitchKind::kQTP_C_Sharp:
              fifthsNumber = 7;
              break;

            case msrQuarterTonesPitchKind::kQTP_G_Sharp: // JMI
              fifthsNumber = 8;
              break;
            case msrQuarterTonesPitchKind::kQTP_D_Sharp:
              fifthsNumber = 9;
              break;
            case msrQuarterTonesPitchKind::kQTP_A_Sharp:
              fifthsNumber = 10;
              break;
            case msrQuarterTonesPitchKind::kQTP_E_Sharp:
              fifthsNumber = 11;
              break;

            case msrQuarterTonesPitchKind::kQTP_F_Natural:
              fifthsNumber = -1;
              break;
            case msrQuarterTonesPitchKind::kQTP_B_Flat:
              fifthsNumber = -2;
              break;
            case msrQuarterTonesPitchKind::kQTP_E_Flat:
              fifthsNumber = -3;
              break;
            case msrQuarterTonesPitchKind::kQTP_A_Flat:
              fifthsNumber = -4;
              break;
            case msrQuarterTonesPitchKind::kQTP_D_Flat:
              fifthsNumber = -5;
              break;
            case msrQuarterTonesPitchKind::kQTP_G_Flat:
              fifthsNumber = -6;
              break;
            case msrQuarterTonesPitchKind::kQTP_C_Flat:
              fifthsNumber = -7;
              break;

            case msrQuarterTonesPitchKind::kQTP_F_Flat: // JMI
              fifthsNumber = -8;
              break;
            case msrQuarterTonesPitchKind::kQTP_B_DoubleFlat:
              fifthsNumber = -9;
              break;
            case msrQuarterTonesPitchKind::kQTP_E_DoubleFlat:
              fifthsNumber = -10;
              break;
            case msrQuarterTonesPitchKind::kQTP_A_DoubleFlat:
              fifthsNumber = -11;
              break;

            default:
              // should not occur
              break;
          } // switch

          if (fifthsNumber != K_NO_FIFTHS_NUMBER) {
            // populate the key element
            fKeyElement->push (
              createMxmlIntegerElement (
                k_fifths,
                fifthsNumber));

            msrModeKind
              modeKind =
                elt->getModeKind (); // JMI should be major by default???

            switch (modeKind) {
              case msrModeKind::kMode_NO_:
                break;
              default:
                fKeyElement->push (
                  createMxmlelement (
                    k_mode,
                    msrModeKindAsMusicXMLString (modeKind)));
            } // switch
          }

          else {
            std::stringstream s;

            s <<
              "key fifthsNumber not specified for key '" <<
              elt->asShortString ();

            msr2mxsrInternalError (
              gGlobalServiceRunData->getInputSourceName (),
              inputLineNumber,
              __FILE__, __LINE__,
              s.str ());
          }
        }
        break;

      case msrKeyKind::kKeyHumdrumScot:
        {
          // JMI
        }
        break;
    } // switch

    // append the key element to the measure attributes element
    fAnAttributeElementIsNeeded = true;

    // this key becomes the new current part key
    fCurrentPartKey = elt;
  }
}

void msr2mxsrTranslator::visitEnd (S_msrKey& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrKey" <<
      elt->asString () <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif
}

//________________________________________________________________________
void msr2mxsrTranslator::visitStart (S_msrTimeSignature& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrTimeSignature " <<
      elt->asString () <<
      ", fCurrentPartKey: ";

    if (fCurrentPartTimeSignature) {
      gLogStream <<
        fCurrentPartTimeSignature;
    }
    else {
      gLogStream << "[NONE]";
    }

    gLogStream <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

  Bool doAppendATimeElementToTheMeasure (false);

  if (fCurrentPartTimeSignature) {
    // a time element has already been created for the current part:
    // each voice in an MSR staff has its own times,
    // another one should be created only if it is different the current one
    doAppendATimeElementToTheMeasure =
      ! elt->isEqualTo (fCurrentPartTimeSignature);
  }
  else {
    // this the first time met in the part
    doAppendATimeElementToTheMeasure = true;
  }

  if (doAppendATimeElementToTheMeasure) {
#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceTimeSignatures ()) {
      gLogStream <<
        "Creating an MXSR time element for msrTimeSignature " <<
        elt->asString () <<
        ", line " << elt->getInputLineNumber () <<
        std::endl;
    }
#endif

    // create the time element
    fTimeElement = createMxmlelement (k_time, "");

    // populate it
    switch (elt->getTimeSignatureSymbolKind ()) {
      case msrTimeSignatureSymbolKind::kTimeSignatureSymbolCommon:
        {
          fTimeElement->add (createMxmlAttribute ("symbol", "common"));

          fTimeElement->push (
            createMxmlIntegerElement (
              k_beats,
              4));
          fTimeElement->push (
            createMxmlIntegerElement (
              k_beat_type,
              4));
        }
        break;

      case msrTimeSignatureSymbolKind::kTimeSignatureSymbolCut:
         {
          fTimeElement->add (createMxmlAttribute ("symbol", "cut"));

          fTimeElement->push (
            createMxmlIntegerElement (
              k_beats,
              2));
          fTimeElement->push (
            createMxmlIntegerElement (
              k_beat_type,
              2));
        }
       break;

      case msrTimeSignatureSymbolKind::kTimeSignatureSymbolNote:
        break;

      case msrTimeSignatureSymbolKind::kTimeSignatureSymbolDottedNote:
        break;

      case msrTimeSignatureSymbolKind::kTimeSignatureSymbolSingleNumber:
        break;

      case msrTimeSignatureSymbolKind::kTimeSignatureSymbolSenzaMisura:
        break;

      case msrTimeSignatureSymbolKind::kTimeSignatureSymbolNone:
        {
          const std::vector<S_msrTimeSignatureItem>&
            timeSignatureItemsVector =
              elt->getTimeSignatureItemsVector ();

          if (! elt->getTimeIsCompound ()) {
            // simple time
            // \time "3/4" for 3/4
            // or senza misura

            S_msrTimeSignatureItem
              timeSignatureItem =
                timeSignatureItemsVector [0]; // the only element;

            // fetch the time signature item beat numbers std::vector
            const std::vector<int>&
              beatsNumbersVector =
                timeSignatureItem->
                  getTimeSignatureBeatsNumbersVector ();

            fTimeElement->push (
              createMxmlIntegerElement (
                k_beats,
                beatsNumbersVector [0])); // the only element
            fTimeElement->push (
              createMxmlIntegerElement (
                k_beat_type,
                timeSignatureItem->getTimeSignatureBeatValue ()));
          }

          else {
            // compound time
            // \compoundMeter #'(3 2 8) for 3+2/8
            // \compoundMeter #'((3 8) (2 8) (3 4)) for 3/8+2/8+3/4
            // \compoundMeter #'((3 2 8) (3 4)) for 3+2/8+3/4

          }
        }
        break;
    } // switch

    // append the time element to the measure attributes element
    fAnAttributeElementIsNeeded = true;

    // this time becomes the new current part time
    fCurrentPartTimeSignature = elt;
  }
}

void msr2mxsrTranslator::visitEnd (S_msrTimeSignature& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrTimeSignature" <<
      elt->asString () <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif
}

//________________________________________________________________________
void msr2mxsrTranslator::visitStart (S_msrTempo& elt)
{
#ifdef TRACING_IS_ENABLED
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrTempo" <<
      elt->asString () <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

/*
  const std::list<S_msrWords>&
    tempoWordsList =
      elt->getTempoWordsList ();

 size_t tempoWordsListSize = tempoWordsList.size ();
*/

  msrDottedDuration tempoBeatUnit  = elt->getTempoBeatUnit ();
  std::string       tempoPerMinute = elt->getTempoPerMinute ();

  msrTempoParenthesizedKind
    tempoParenthesizedKind =
      elt->getTempoParenthesizedKind ();

  msrPlacementKind
    tempoPlacementKind =
      elt->getTempoPlacementKind ();

  switch (elt->getTempoKind ()) {
    case msrTempoKBeatUnitsKind::kTempoBeatUnits_NO_:
      break;

    case msrTempoKBeatUnitsKind::kTempoBeatUnitsWordsOnly:
      {
    /*
        fOutputStream <<
          "\\tempo ";

        if (tempoWordsListSize) {
          std::list<S_msrWords>::const_iterator
            iBegin = tempoWordsList.begin (),
            iEnd   = tempoWordsList.end (),
            i      = iBegin;

          for ( ; ; ) {
            S_msrWords words = (*i);

            fOutputStream <<
              "\"" << words->getWordsContents () << "\"";

            if (++i == iEnd) break;

            fOutputStream <<
              ' ';
          } // for

          fOutputStream <<
            std::endl;
        }
        */
        }
      break;

    case msrTempoKBeatUnitsKind::kTempoBeatUnitsPerMinute:
      switch (tempoParenthesizedKind) {
        case msrTempoParenthesizedKind::kTempoParenthesizedYes:
          {
          // create the metronome element
          Sxmlelement metronomeElement = createMxmlelement (k_metronome, "");

          // set its "parentheses" attribute
          metronomeElement->add (createMxmlAttribute ("parentheses", "yes"));

          // append the beat unit element to the metronome elements
          msrDurationKind
            durationKind =
              tempoBeatUnit.getDurationKind ();
              /* JMI
          int
            dotsNumber =
              tempoBeatUnit.getDotsNumber ();
              */

          metronomeElement-> push (
            createMxmlelement (
              k_beat_unit,
              msrDurationKindAsMusicXMLType (durationKind)));

          // append the per minute element to the metronome elements
          metronomeElement-> push (createMxmlelement (k_per_minute, tempoPerMinute));

          // append the metronome element to the current measure element
          appendToMeasureDirection (
            metronomeElement,
            tempoPlacementKind);

  /*
          if (tempoWordsListSize) {
            std::list<S_msrWords>::const_iterator
              iBegin = tempoWordsList.begin (),
              iEnd   = tempoWordsList.end (),
              i      = iBegin;

            for ( ; ; ) {
              S_msrWords words = (*i);

              fOutputStream <<
                "\"" << words->getWordsContents () << "\"";

              if (++i == iEnd) break;

              fOutputStream <<
                ' ';
            } // for
          }
          */
        }
        break;

      case msrTempoParenthesizedKind::kTempoParenthesizedNo:
        {
          // create the metronome element
          Sxmlelement metronomeElement = createMxmlelement (k_metronome, "");

          // append the beat unit element to the metronome elements
          msrDurationKind
            durationKind =
              tempoBeatUnit.getDurationKind ();
              /*
          int
            dotsNumber =
              tempoBeatUnit.getDotsNumber ();
              */

          metronomeElement-> push (
            createMxmlelement (
              k_beat_unit,
              msrDurationKindAsMusicXMLType (durationKind)));

          // append the per minute element to the metronome elements
          metronomeElement-> push (createMxmlelement (k_per_minute, tempoPerMinute));

          // append the metronome element to the current measure element
          appendToMeasureDirection (
            metronomeElement,
            tempoPlacementKind);

  /*
          if (tempoWordsListSize) {
            std::list<S_msrWords>::const_iterator
              iBegin = tempoWordsList.begin (),
              iEnd   = tempoWordsList.end (),
              i      = iBegin;

            for ( ; ; ) {
              S_msrWords words = (*i);

              fOutputStream <<
                "\"" << words->getWordsContents () << "\"";

              if (++i == iEnd) break;

              fOutputStream <<
                ' ';
            } // for
          }
          */
        }
          break;
        } // switch
      break;

    case msrTempoKBeatUnitsKind::kTempoBeatUnitsEquivalence:
      {
    /*
      fOutputStream <<
        "\\tempo ";

      if (tempoWordsListSize) {
        std::list<S_msrWords>::const_iterator
          iBegin = tempoWordsList.begin (),
          iEnd   = tempoWordsList.end (),
          i      = iBegin;

        for ( ; ; ) {
          S_msrWords words = (*i);

          fOutputStream <<
            "\"" << words->getWordsContents () << "\"";

          if (++i == iEnd) break;

          fOutputStream <<
            ' ';
        } // for
      }

      fOutputStream <<
        ' ' <<
        "\\markup {" <<
        std::endl;

      ++gIndenter;

      fOutputStream <<
        "\\concat {" <<
        std::endl;

      ++gIndenter;

      switch (tempoParenthesizedKind) {
        case msrTempoParenthesizedKind::kTempoParenthesizedYes:
          fOutputStream <<
            '(' <<
            std::endl;
          break;
        case msrTempoParenthesizedKind::kTempoParenthesizedNo:
          break;
      } // switch

      ++gIndenter;

      if (versionNumberGreaterThanOrEqualTo ("2.20")) {
        fOutputStream <<
          " \\smaller \\general-align #Y #DOWN \\note {";
      }
      else {
        fOutputStream <<
          " \\smaller \\general-align #Y #DOWN \\note #\"";
      }

      fOutputStream <<
        dottedDurationAsLilypondStringWithoutBackSlash (
          inputLineNumber,
          tempoBeatUnit);

      if (versionNumberGreaterThanOrEqualTo ("2.20")) {
        fOutputStream <<
          "} #UP";
      }
      else {
        fOutputStream <<
          "\" #UP";
      }

      fOutputStream <<
        std::endl <<
        "\" = \"" <<
        std::endl;

      fOutputStream <<
        "(";
      if (versionNumberGreaterThanOrEqualTo ("2.20")) {
        fOutputStream <<
          " \\smaller \\general-align #Y #DOWN \\note {";
      }
      else {
        fOutputStream <<
          " \\smaller \\general-align #Y #DOWN \\note #\"";
      }

      fOutputStream <<
        dottedDurationAsLilypondStringWithoutBackSlash (
          inputLineNumber,
          elt->getTempoEquivalentBeatUnit ());

      if (versionNumberGreaterThanOrEqualTo ("2.20")) {
        fOutputStream <<
          "} #UP";
      }
      else {
        fOutputStream <<
          "\" #UP";
      }

      fOutputStream << std::endl;

      --gIndenter;

      switch (tempoParenthesizedKind) {
        case msrTempoParenthesizedKind::kTempoParenthesizedYes:
          fOutputStream <<
            ")" <<
            std::endl;
          break;
        case msrTempoParenthesizedKind::kTempoParenthesizedNo:
          break;
      } // switch

      --gIndenter;

      fOutputStream <<
        '}' <<
        std::endl;

      --gIndenter;

      fOutputStream <<
        '}' <<
        std::endl;
        */
        }
      break;

    case msrTempoKBeatUnitsKind::kTempoNotesRelationship:
      {
    /*
      fOutputStream <<
        "\\tempoNotesRelationship #\"";

      if (tempoWordsListSize) {
        std::list<S_msrWords>::const_iterator
          iBegin = tempoWordsList.begin (),
          iEnd   = tempoWordsList.end (),
          i      = iBegin;

        for ( ; ; ) {
          S_msrWords words = (*i);

          fOutputStream <<
     // JMI       "\"" <<
            words->getWordsContents (); // JMI <<
      // JMI      "\"";

          if (++i == iEnd) break;

          fOutputStream <<
            ' ';
        } // for
      }

      fOutputStream <<
        "\"";

      switch (tempoParenthesizedKind) {
        case msrTempoParenthesizedKind::kTempoParenthesizedYes:
          fOutputStream <<
            " ##t";
          break;
        case msrTempoParenthesizedKind::kTempoParenthesizedNo:
          fOutputStream <<
            " ##f";
          break;
      } // switch

      fOutputStream << std::endl;
      */
      }
      break;
  } // switch
}

void msr2mxsrTranslator::visitEnd (S_msrTempo& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrTempo" <<
      elt->asString () <<
      std::endl;
  }
#endif
}

//________________________________________________________________________
void msr2mxsrTranslator::visitStart (S_msrChord& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrChord" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  if (gGlobalMsr2mxsrOahGroup->getMusicXMLComments ()) {
    // create a chord start comment
    std::stringstream s;
    s <<
      " ===== " <<
      "Chord start " <<
      ", chordSoundingWholeNotes: " <<
      elt->getMeasureElementSoundingWholeNotes () <<
      ", " <<
      elt->getChordNotesVector ().size () <<
      " elements" <<
      ", line " << inputLineNumber <<
      " ===== ";
    fPendingChordStartCommentElement = createMxmlelement (kComment, s.str ());
  }

  // append it to the current measure element
  fCurrentMeasureElement->push (fPendingChordStartCommentElement);

  fOnGoingChord = true;
}

void msr2mxsrTranslator::visitEnd (S_msrChord& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrChord" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

  if (gGlobalMsr2mxsrOahGroup->getMusicXMLComments ()) {
    // create a chord end comment
    std::stringstream s;
    s <<
      " ===== " <<
      "Chord end " <<
      ", chordSoundingWholeNotes: " <<
      elt->getMeasureElementSoundingWholeNotes () <<
      ", " <<
      elt->getChordNotesVector ().size () <<
      " elements" <<
      ", line " << inputLineNumber <<
      " ===== ";
    Sxmlelement comment = createMxmlelement (kComment, s.str ());

    // append it to the current measure element
    fCurrentMeasureElement->push (comment);
  }

  // forget about the pending chord start comment
  fPendingChordStartCommentElement = nullptr;

  fOnGoingChord = false;
}

//________________________________________________________________________
void msr2mxsrTranslator::visitStart (S_msrTuplet& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrTuplet" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  if (gGlobalMsr2mxsrOahGroup->getMusicXMLComments ()) {
    // create a tuplet start comment
    std::stringstream s;
    s <<
      " ===== " <<
      "Tuplet start " <<
      ", tupletFactor: " <<
      elt->getTupletFactor ().asRational () <<
      ", " <<
      elt->getTupletElementsList ().size () << " elements" <<
      ", line " << inputLineNumber <<
      " ===== ";
    Sxmlelement comment = createMxmlelement (kComment, s.str ());

    // append it to the current measure element
    fCurrentMeasureElement->push (comment);
  }
}

void msr2mxsrTranslator::visitEnd (S_msrTuplet& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrTuplet" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

  if (gGlobalMsr2mxsrOahGroup->getMusicXMLComments ()) {
    // create a tuplet end comment
    std::stringstream s;
    s <<
      " ===== " <<
      "Tuplet end " <<
      ", tupletFactor: " <<
      elt->getTupletFactor ().asRational () <<
      ", tupletElementsList: " <<
      elt->getTupletElementsList ().size () << " elements" <<
      ", line " << inputLineNumber <<
      " ===== ";
    Sxmlelement comment = createMxmlelement (kComment, s.str ());

    // append it to the current measure element
    fCurrentMeasureElement->push (comment);
  }
}

//________________________________________________________________________
void msr2mxsrTranslator:: appendNoteWedges (
  const S_msrNote& theMsrNote)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceWedges ()) {
    gLogStream <<
      "--> appendNoteWedges, theMsrNote = " <<
      theMsrNote->asShortString () <<
      std::endl;
  }
#endif

  const std::list<S_msrWedge>&
    noteWedges =
      theMsrNote->getNoteWedges () ;

  if (noteWedges.size ()) {
    std::list<S_msrWedge>::const_iterator i;

    for (i=noteWedges.begin (); i!=noteWedges.end (); ++i) {
      S_msrWedge wedge = (*i);

      msrWedgeKind wedgeKind = wedge->getWedgeKind ();

      std::string typeString;

      switch (wedgeKind) {
        case msrWedgeKind::kWedgeKindNone:
          // should not occur
          break;
        case msrWedgeKind::kWedgeCrescendo:
          typeString = "crescendo";
          break;
        case msrWedgeKind::kWedgeDecrescendo:
          typeString = "diminuendo";
          break;
        case msrWedgeKind::kWedgeStop:
          typeString = "stop";
          break;
      } // switch

      // create the wedge element
      Sxmlelement wedgeElement = createMxmlelement (k_wedge, "");

      // set its "type" attribute
      wedgeElement->add (createMxmlAttribute ("type", typeString));

      // append the wedge element to the current measure element
      appendToMeasureDirection (
        wedgeElement,
        wedge->getWedgePlacementKind ());
    } // for
  }
}

//________________________________________________________________________
void msr2mxsrTranslator::appendNoteDynamics (
  const S_msrNote& theMsrNote)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceDynamics ()) {
    gLogStream <<
      "--> appendNoteDynamics, theMsrNote = " <<
      theMsrNote->asShortString () <<
      std::endl;
  }
#endif

  const std::list<S_msrDynamic>&
    noteDynamics =
      theMsrNote->getNoteDynamics () ;

  if (noteDynamics.size ()) {
    std::list<S_msrDynamic>::const_iterator i;

    for (i=noteDynamics.begin (); i!=noteDynamics.end (); ++i) {
      S_msrDynamic dynamic = (*i);

      // create the dynamics element
      Sxmlelement dynamicsElement = createMxmlelement (k_dynamics, "");

      // create the dynamics specific sub-element
      int subElementID = -1;

      switch (dynamic->getDynamicKind ()) {
        case msrDynamicKind::kDynamicF:
          subElementID = k_f;
          break;
        case msrDynamicKind::kDynamicFF:
          subElementID = k_ff;
          break;
        case msrDynamicKind::kDynamicFFF:
          subElementID = k_fff;
          break;
        case msrDynamicKind::kDynamicFFFF:
          subElementID = k_ffff;
          break;
        case msrDynamicKind::kDynamicFFFFF:
          subElementID = k_fffff;
          break;
        case msrDynamicKind::kDynamicFFFFFF:
          subElementID = k_ffffff;
          break;

        case msrDynamicKind::kDynamicP:
          subElementID = k_p;
          break;
        case msrDynamicKind::kDynamicPP:
          subElementID = k_pp;
          break;
        case msrDynamicKind::kDynamicPPP:
          subElementID = k_ppp;
          break;
        case msrDynamicKind::kDynamicPPPP:
          subElementID = k_pppp;
          break;
        case msrDynamicKind::kDynamicPPPPP:
          subElementID = k_ppppp;
          break;
        case msrDynamicKind::kDynamicPPPPPP:
          subElementID = k_pppppp;
          break;

        case msrDynamicKind::kDynamicMF:
          subElementID = k_mf;
          break;
        case msrDynamicKind::kDynamicMP:
          subElementID = k_mp;
          break;
        case msrDynamicKind::kDynamicFP:
          subElementID = k_fp;
          break;
        case msrDynamicKind::kDynamicFZ:
          subElementID = k_fz;
          break;
        case msrDynamicKind::kDynamicPF:
          subElementID = k_pf;
          break;
        case msrDynamicKind::kDynamicRF:
          subElementID = k_rf;
          break;
        case msrDynamicKind::kDynamicSF:
          subElementID = k_sf;
          break;

        case msrDynamicKind::kDynamicRFZ:
          subElementID = k_rfz;
          break;
        case msrDynamicKind::kDynamicSFZ:
          subElementID = k_sfz;
          break;
        case msrDynamicKind::kDynamicSFP:
          subElementID = k_sfp;
          break;
        case msrDynamicKind::kDynamicSFPP:
          subElementID = k_sfpp;
          break;
        case msrDynamicKind::kDynamicSFFZ:
          subElementID = k_sffz;
          break;
        case msrDynamicKind::kDynamicSFZP:
          subElementID = k_sfzp;
          break;
        case msrDynamicKind::kDynamicN:
          subElementID = k_n;
          break;
        case msrDynamicKind::kDynamic_NO_:
          ; // should not occur
          break;
      } // switch

      Sxmlelement dynamicsElementSubElement =
        createMxmlelement (subElementID, "");

      // append the dynamics sub-element to the dynamics elements
      dynamicsElement-> push (dynamicsElementSubElement);

      // append the dynamics element to the current measure element
      appendToMeasureDirection (
        dynamicsElement,
        dynamic->getDynamicPlacementKind ());
    } // for
  }
}

//________________________________________________________________________
void msr2mxsrTranslator::appendABackupToMeasure (
  const S_msrNote& theMsrNote)
{
  int inputLineNumber =
    theMsrNote->getInputLineNumber ();

  // fetch the backup duration divisions
  Rational
    previousNoteMeasureElementMeasurePosition =
      fPreviousMSRNote->getMeasureElementMeasurePosition (),
    previousNoteSoundingWholeNotes =
      fPreviousMSRNote->getMeasureElementSoundingWholeNotes (),

    theMsrNoteMeasureElementMeasurePosition =
      fPreviousMSRNote->getMeasureElementMeasurePosition (),
    theMsrNoteSoundingWholeNotes =
      theMsrNote->getMeasureElementSoundingWholeNotes ();

  Rational
    backupDuration =
      previousNoteMeasureElementMeasurePosition
        +
      previousNoteSoundingWholeNotes
        -
      theMsrNoteMeasureElementMeasurePosition;

  int
    backupDurationDivisions =
      wholeNotesAsDivisions (
        inputLineNumber,
        backupDuration);

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceBackup ()) {
    gLogStream <<
      "Creating a backup element, theMsrNote = " <<
      theMsrNote->asShortString () <<

      ", previousNoteMeasureElementMeasurePosition: " <<
      previousNoteMeasureElementMeasurePosition <<
      ", previousNoteSoundingWholeNotes: " <<
      previousNoteSoundingWholeNotes <<

      ", theMsrNoteMeasureElementMeasurePosition: " <<
      theMsrNoteMeasureElementMeasurePosition <<
      ", theMsrNoteSoundingWholeNotes: " <<
      theMsrNoteSoundingWholeNotes <<

      ", backupDuration: " << backupDuration <<
      ", backupDurationDivisions: " << backupDurationDivisions <<

      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  if (gGlobalMsr2mxsrOahGroup->getMusicXMLComments ()) {
    S_msrVoice
      noteVoice =
        theMsrNote->fetchNoteUpLinkToVoice ();

    int
      noteStaffNumber =
        theMsrNote->
          fetchUpLinkToNoteToStaff ()->
            getStaffNumber (),
      previousMSRNoteStaffNumber =
        fPreviousMSRNoteStaff->
          getStaffNumber ();

    // create a backup comment
    std::stringstream s;
    s <<
      " ===== " <<
      "Backup" <<

      ", previousNoteMeasureElementMeasurePosition: " <<
      previousNoteMeasureElementMeasurePosition <<
      ", previousNoteSoundingWholeNotes: " <<
      previousNoteSoundingWholeNotes <<

      ", theMsrNoteMeasureElementMeasurePosition: " <<
      theMsrNoteMeasureElementMeasurePosition <<
      ", theMsrNoteSoundingWholeNotes: " <<
      theMsrNoteSoundingWholeNotes <<

      ", backupDuration: " << backupDuration <<
      ", backupDurationDivisions: " << backupDurationDivisions <<

      ", from staff: " << previousMSRNoteStaffNumber <<
      ", to staff: " << noteStaffNumber <<
      ", line " << inputLineNumber <<
      " ===== ";
    Sxmlelement comment = createMxmlelement (kComment, s.str ());

    // append it to the current measure element
    appendOtherToMeasure (comment);
  }

  // create a backup element
  Sxmlelement backupElement = createMxmlelement (k_backup, "");

  // append a duration sub-element to it
  backupElement->push (
    createMxmlIntegerElement (k_duration, backupDurationDivisions));

  // append it to the current measure element
  appendOtherToMeasure (backupElement);

  // reset the cumulated skip durations informations
  fCurrentCumulatedSkipsDurations = Rational (0, 1);
  fCurrentCumulatedSkipsStaffNumber = -1;
  fCurrentCumulatedSkipsVoiceNumber = -1;
}

//________________________________________________________________________
void msr2mxsrTranslator:: appendAForwardToMeasure (
  const S_msrNote& theMsrNote)
{
  int inputLineNumber =
     theMsrNote->getInputLineNumber ();

  /*
    <forward>
      <duration>16</duration>
      <voice>1</voice>
      <staff>1</staff>
    </forward>
  */

  // fetch the forward duration divisions
  int
    forwardDurationDivisions =
      wholeNotesAsDivisions (
        inputLineNumber,
        fCurrentCumulatedSkipsDurations);

#ifdef TRACING_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceForward ()) {
    gLogStream <<
      "Creating a forward element, note = " <<
      theMsrNote->asShortString () <<
      ", forwardDurationDivisions: " << forwardDurationDivisions <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  int
    noteStaffNumber =
      theMsrNote->
        fetchUpLinkToNoteToStaff ()->
          getStaffNumber (),
    noteVoiceNumber =
      theMsrNote->
        fetchNoteUpLinkToVoice ()->
          getVoiceNumber ();

  if (gGlobalMsr2mxsrOahGroup->getMusicXMLComments ()) {
    int
      previousMSRNoteStaffNumber =
        fPreviousMSRNoteStaff->
          getStaffNumber (),
      previousMSRNoteVoiceNumber =
        fPreviousMSRNoteVoice->
          getVoiceNumber ();

    // create a forward comment
    S_msrVoice
      noteVoice =
        theMsrNote->fetchNoteUpLinkToVoice ();

    std::stringstream s;
    s <<
      " ===== " <<
      "Forward" <<
      ", forwardDurationDivisions: " << forwardDurationDivisions <<
      ", in staff: " << previousMSRNoteStaffNumber <<
      ", in voice: " << previousMSRNoteVoiceNumber <<
      ", line " << inputLineNumber <<
      " ===== ";
    Sxmlelement comment = createMxmlelement (kComment, s.str ());

    // append it to the current measure element
    appendOtherToMeasure (comment);
  }

  // create a forward element
  Sxmlelement forwardElement = createMxmlelement (k_forward, "");

  // append a duration sub-element to it
  forwardElement->push (
    createMxmlIntegerElement (k_duration, forwardDurationDivisions));

  // append a voice sub-element to it
  forwardElement->push (
    createMxmlIntegerElement (k_voice, noteVoiceNumber));

  // append a staff sub-element to it
  forwardElement->push (
    createMxmlIntegerElement (k_staff, noteStaffNumber));

  // append it to the current measure element
  appendOtherToMeasure (forwardElement);

  // reset the cumulated skip durations informations
  fCurrentCumulatedSkipsDurations = Rational (0, 1);
  fCurrentCumulatedSkipsStaffNumber = -1;
  fCurrentCumulatedSkipsVoiceNumber = -1;
}

//________________________________________________________________________
void msr2mxsrTranslator:: appendABackupOrForwardToMeasureIfNeeded (
  const S_msrNote& theMsrNote)
{
#ifdef TRACING_IS_ENABLED
  int inputLineNumber =
     theMsrNote->getInputLineNumber ();

  if (
    gGlobalMxsrOahGroup->getTraceBackup ()
      ||
    gGlobalMxsrOahGroup->getTraceForward ()
  ) {
    gLogStream <<
      "Appending a backup or forward to measure if needed, theMsrNote = " <<
      theMsrNote->asShortString () <<
      ", fCurrentCumulatedSkipsDurations: " << fCurrentCumulatedSkipsDurations <<
      ", previousMSRNote: ";
    if (fPreviousMSRNote) {
      gLogStream <<
        fPreviousMSRNote->asShortString ();
    }
    else {
      gLogStream << "[NONE]";
    }

    gLogStream <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  int
    noteStaffNumber =
      theMsrNote->
        fetchUpLinkToNoteToStaff ()->
          getStaffNumber (),
    noteVoiceNumber =
      theMsrNote->
        fetchNoteUpLinkToVoice ()->
          getVoiceNumber (),
    previousMSRNoteStaffNumber =
      fPreviousMSRNoteStaff
        ? fPreviousMSRNoteStaff->getStaffNumber ()
        : 0,
    previousMSRNoteVoiceNumber =
      fPreviousMSRNoteVoice
        ? fPreviousMSRNoteVoice->getVoiceNumber ()
        : 0;

#ifdef TRACING_IS_ENABLED
  if (
    gGlobalMxsrOahGroup->getTraceBackup ()
      ||
    gGlobalMxsrOahGroup->getTraceForward ()
  ) {
    gLogStream <<
      "--> appendABackupOrForwardToMeasureIfNeeded(1), theMsrNote = " <<
      theMsrNote->asShortString () <<
      ", fCurrentCumulatedSkipsDurations: " << fCurrentCumulatedSkipsDurations <<
      ", noteStaffNumber: " << noteStaffNumber <<
      ", noteVoiceNumber: " << noteVoiceNumber <<
      ", previousMSRNote: ";
    if (fPreviousMSRNote) {
      gLogStream <<
        fPreviousMSRNote->asShortString ();
    }
    else {
      gLogStream << "[NONE]";
    }

    gLogStream <<
      ", previousMSRNoteStaffNumber: " << previousMSRNoteStaffNumber <<
      ", previousMSRNoteVoiceNumber: " << previousMSRNoteVoiceNumber <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

/*
  A <backup /> is needed when a note is in another voice as the previous one.
  A <forward /> is needed when a note follows one or more skip(s) in the same voice.
  Consecutive skips are not created by mxsr2msr from MusicXML data,
  but this may happen if the  MSR API is used freely

  A skip in MSR may have been created when there was a <backup />
  to a position after the beginning of the measure,
  in which case a <backu> /? should be created in the MusicXML output,
  and not a <forward />
*/

  if (fOnGoingChord) { // JMI ???
  }

/*
fCurrentCumulatedSkipsStaffNumber
fCurrentCumulatedSkipsVoiceNumber
*/

  if (fPreviousMSRNote) {
    // compare the staff and voice numbers of note and fPreviousMSRNote

    if (noteStaffNumber == previousMSRNoteStaffNumber) {
      if (noteVoiceNumber == previousMSRNoteVoiceNumber) {
        // same staff, same voice

        // is a <forward /> or <backup /> element needed?
        if (fCurrentCumulatedSkipsDurations.getNumerator () != 0) {
          // a skip may have been created due to a <backup /> to a position
          // that is not at the beginning of the measure
          Rational
            noteMeasurePosition =
              theMsrNote->getMeasureElementMeasurePosition ();

          Rational
            positionAfterNoteInMeasure =
              noteMeasurePosition +
                theMsrNote->getMeasureElementSoundingWholeNotes ();

#ifdef TRACING_IS_ENABLED
          if (
            gGlobalMxsrOahGroup->getTraceBackup ()
              ||
            gGlobalMxsrOahGroup->getTraceForward ()
          ) {
            gLogStream <<
              "--> appendABackupOrForwardToMeasureIfNeeded(2), note = " <<
              theMsrNote->asShortString () <<
              ", noteMeasurePosition: " << noteMeasurePosition <<
              ", positionAfterNoteInMeasure: " << positionAfterNoteInMeasure <<
              ", fCurrentMeasurePosition: " << fCurrentMeasurePosition <<
              ", line " << inputLineNumber <<
              std::endl;
          }
#endif

          if (positionAfterNoteInMeasure < fCurrentMeasurePosition) { // JMI TEST
            appendABackupToMeasure (theMsrNote);
          }
          else if (positionAfterNoteInMeasure > fCurrentMeasurePosition) {
            appendAForwardToMeasure (theMsrNote);
          }
        }
      }

      else {
        // same staff, different voice
        appendABackupToMeasure (theMsrNote);
      }
    }

    else {
      // there is a staff change
      appendABackupToMeasure (theMsrNote);

      if (noteVoiceNumber == previousMSRNoteVoiceNumber) {
        // JMI
      }
      else {
        // JMI
      }
    }
  }
}

//________________________________________________________________________
void msr2mxsrTranslator:: populateNoteDirections (
  const S_msrNote& theMsrNote)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceNotes ()) {
    gLogStream <<
      "--> populateNoteDirections, theMsrNote = " <<
      theMsrNote->asShortString () <<
      std::endl;
  }
#endif

/*
<!ELEMENT direction-type (rehearsalMark+ | segno+ | coda+ |
  (words | symbol)+ | wedge | dynamics+ | dashes |
  bracket | pedal | metronome | octave-shift | harp-pedals |
  damp | damp-all | eyeglasses | std::string-mute |
  scordatura | image | principal-voice | percussion+ |
  accordion-registration | staff-divide | other-direction)>
<!ATTLIST direction-type
    %optional-unique-id;
>
*/

  // append the wedges elements if any
  appendNoteWedges (theMsrNote);

  // append the dynamics elements if any
  appendNoteDynamics (theMsrNote);
}

//________________________________________________________________________
void msr2mxsrTranslator:: appendNoteOrnaments (
  const S_msrNote& theMsrNote)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceOrnaments ()) {
    gLogStream <<
      "--> appendNoteOrnaments, theMsrNote = " <<
      theMsrNote->asShortString () <<
      std::endl;
  }
#endif

/* JMI
<!ELEMENT ornaments
  (((trill-mark
  | turn | delayed-turn | inverted-turn |
     delayed-inverted-turn | vertical-turn |
     inverted-vertical-turn | shake | wavy-line |
     mordent | inverted-mordent | schleifer | tremolo |
     haydn | other-ornament), accidental-mark*)*)>
<!ATTLIST ornaments
    %optional-unique-id;
>
<!ELEMENT trill-mark EMPTY>
<!ATTLIST trill-mark
    %print-style;
    %placement;
    %trill-sound;
>
    */

  // append the ornament elements if any
  const std::list<S_msrOrnament>&
    noteOrnaments =
      theMsrNote->getNoteOrnaments () ;

  if (noteOrnaments.size ()) {
    for (S_msrOrnament ornament : noteOrnaments) {
      msrOrnamentKind
        ornamentKind =
          ornament->getOrnamentKind ();

      int ornamentType = kComment; // JMI ??? v0.9.66

      switch (ornamentKind) {
        case msrOrnamentKind::kOrnamentTrill:
          ornamentType = k_trill_mark;
          break;
        case msrOrnamentKind::kOrnamentTurn:
          ornamentType = k_turn;
          break;
        case msrOrnamentKind::kOrnamentInvertedTurn:
          ornamentType = k_inverted_turn;
          break;
        case msrOrnamentKind::kOrnamentDelayedTurn:
          ornamentType = k_delayed_turn;
          break;
        case msrOrnamentKind::kOrnamentDelayedInvertedTurn:
          ornamentType = k_delayed_inverted_turn;
          break;
        case msrOrnamentKind::kOrnamentVerticalTurn:
          ornamentType = k_vertical_turn;
          break;
        case msrOrnamentKind::kOrnamentMordent:
          ornamentType = k_mordent;
          break;
        case msrOrnamentKind::kOrnamentInvertedMordent:
          ornamentType = k_inverted_mordent;
          break;
        case msrOrnamentKind::kOrnamentSchleifer:
          ornamentType = k_schleifer;
          break;
        case msrOrnamentKind::kOrnamentShake:
          ornamentType = k_shake;
          break;
        case msrOrnamentKind::kOrnamentAccidentalKind:
          ornamentType = k_accidental_mark;
          break;
      } // switch

      // append the note ornament element to the current note element
      switch (ornamentKind) {
        case msrOrnamentKind::kOrnamentAccidentalKind:
          {
            Sxmlelement ornamentElement =
              createMxmlelement (
                ornamentType,
                msrAccidentalKindAsMusicXMLString (
                  ornament->getOrnamentAccidentalKind ()));

            appendToNoteNotations (
              ornamentElement,
              ornament->getOrnamentPlacementKind ());
          }
          break;

        default:
          {
            Sxmlelement ornamentElement =
              createMxmlelement (
                ornamentType,
                "");

            appendToNoteNotationsOrnaments (
              ornamentElement,
              ornament->getOrnamentPlacementKind ());
          }
      } // switch
    } // for
  }
}

//________________________________________________________________________
void msr2mxsrTranslator:: appendNoteTechnicals (
  const S_msrNote& theMsrNote)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceTechnicals ()) {
    gLogStream <<
      "--> appendNoteTechnicals, theMsrNote = " <<
      theMsrNote->asShortString () <<
      std::endl;
  }
#endif

/* JMI
<!ELEMENT technical
  ((up-bow | down-bow | harmonic | open-std::string |
    thumb-position | fingering | pluck | double-tongue |
    triple-tongue | stopped | snap-pizzicato | fret |
    std::string | hammer-on | pull-off | bend | tap | heel |
    toe | fingernails | hole | arrow | handbell |
    brass-bend | flip | smear | open | half-muted |
    harmon-mute | golpe | other-technical)*)>
<!ATTLIST technical
    %optional-unique-id;

    */

  // append the technical elements if any
  const std::list<S_msrTechnical>&
    noteTechnicals =
      theMsrNote->getNoteTechnicals () ;

  if (noteTechnicals.size ()) {
    std::list<S_msrTechnical>::const_iterator i;

    for (i=noteTechnicals.begin (); i!=noteTechnicals.end (); ++i) {
      S_msrTechnical
        technical = (*i);

      msrTechnicalKind
        technicalKind =
          technical->getTechnicalKind ();

      int technicalType = kComment; // JMI

      switch (technicalKind) {
        case msrTechnicalKind::kTechnicalArrow:
          technicalType = k_arrow;
          break;
        case msrTechnicalKind::kTechnicalDoubleTongue:
          technicalType = k_double_tongue;
          break;
        case msrTechnicalKind::kTechnicalDownBow:
          technicalType = k_down_bow;
          break;
        case msrTechnicalKind::kTechnicalFingernails:
          technicalType = k_fingernails;
          break;
        case msrTechnicalKind::kTechnicalHarmonic:
          technicalType = k_harmonic;
          break;
        case msrTechnicalKind::kTechnicalHeel:
          technicalType = k_heel;
          break;
        case msrTechnicalKind::kTechnicalHole:
          technicalType = k_hole;
          break;
        case msrTechnicalKind::kTechnicalOpenString:
          technicalType = k_open_string;
          break;
        case msrTechnicalKind::kTechnicalSnapPizzicato:
          technicalType = k_snap_pizzicato;
          break;
        case msrTechnicalKind::kTechnicalStopped:
          technicalType = k_stopped;
          break;
        case msrTechnicalKind::kTechnicalTap:
          technicalType = k_tap;
          break;
        case msrTechnicalKind::kTechnicalThumbPosition:
          technicalType = k_thumb_position;
          break;
        case msrTechnicalKind::kTechnicalToe:
          technicalType = k_toe;
          break;
        case msrTechnicalKind::kTechnicalTripleTongue:
          technicalType = k_triple_tongue;
          break;
        case msrTechnicalKind::kTechnicalUpBow:
          technicalType = k_up_bow;
          break;
      } // switch

      // create the technical element
      Sxmlelement technicalElement = createMxmlelement (technicalType, "");

      // append the note technicals element to the current note element
      appendToNoteNotationsTechnicals (
        technicalElement,
        technical->getTechnicalPlacementKind ());
    } // for
  }
}

//________________________________________________________________________
void msr2mxsrTranslator:: appendNoteTechnicalWithIntegers (
  const S_msrNote& theMsrNote)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceTechnicals ()) {
    gLogStream <<
      "--> appendNoteTechnicalWithIntegers, theMsrNote = " <<
      theMsrNote->asShortString () <<
      std::endl;
  }
#endif

/* JMI
<!ELEMENT technical
  ((up-bow | down-bow | harmonic | open-std::string |
    thumb-position | fingering | pluck | double-tongue |
    triple-tongue | stopped | snap-pizzicato | fret |
    std::string | hammer-on | pull-off | bend | tap | heel |
    toe | fingernails | hole | arrow | handbell |
    brass-bend | flip | smear | open | half-muted |
    harmon-mute | golpe | other-technical)*)>
<!ATTLIST technical
    %optional-unique-id;

    */

  // append the technicalWithInteger elements if any
  const std::list<S_msrTechnicalWithInteger>&
    noteTechnicalWithIntegers =
      theMsrNote->getNoteTechnicalWithIntegers () ;

  if (noteTechnicalWithIntegers.size ()) {
    std::list<S_msrTechnicalWithInteger>::const_iterator i;

    for (i=noteTechnicalWithIntegers.begin (); i!=noteTechnicalWithIntegers.end (); ++i) {
      S_msrTechnicalWithInteger
        technicalWithInteger = (*i);

      msrTechnicalWithIntegerKind
        technicalWithIntegerKind =
          technicalWithInteger->getTechnicalWithIntegerKind ();

      int technicalWithIntegerType = kComment; // JMI

      switch (technicalWithIntegerKind) {
        case msrTechnicalWithIntegerKind::kFingering:
          technicalWithIntegerType = k_fingering;
          break;
        case msrTechnicalWithIntegerKind::kFret:
          technicalWithIntegerType = k_fret;
          break;
        case msrTechnicalWithIntegerKind::kString:
          technicalWithIntegerType = k_string;
          break;
      } // switch

      // create the technicalWithInteger element
      int technicalWithIntegerValue =
        technicalWithInteger->getTechnicalWithIntegerValue ();

      Sxmlelement technicalWithIntegerElement =
        technicalWithIntegerValue > 0
          ? createMxmlIntegerElement (
              technicalWithIntegerType,
              technicalWithIntegerValue)
          : createMxmlelement (
              technicalWithIntegerType,
              "");

      // append the note technicalWithIntegers element to the current note element
      appendToNoteNotationsTechnicals (
        technicalWithIntegerElement,
        technicalWithInteger->getTechnicalWithIntegerPlacementKind ());
    } // for
  }
}

//________________________________________________________________________
void msr2mxsrTranslator:: appendNoteTechnicalWithFloats (
  const S_msrNote& theMsrNote)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceTechnicals ()) {
    gLogStream <<
      "--> appendNoteTechnicalWithFloats, theMsrNote = " <<
      theMsrNote->asShortString () <<
      std::endl;
  }
#endif

/* JMI
<!ELEMENT technical
  ((up-bow | down-bow | harmonic | open-std::string |
    thumb-position | fingering | pluck | double-tongue |
    triple-tongue | stopped | snap-pizzicato | fret |
    std::string | hammer-on | pull-off | bend | tap | heel |
    toe | fingernails | hole | arrow | handbell |
    brass-bend | flip | smear | open | half-muted |
    harmon-mute | golpe | other-technical)*)>
<!ATTLIST technical
    %optional-unique-id;

    */

  // append the technicalWithFloat elements if any
  const std::list<S_msrTechnicalWithFloat>&
    noteTechnicalWithFloats =
      theMsrNote->getNoteTechnicalWithFloats () ;

  if (noteTechnicalWithFloats.size ()) {
    std::list<S_msrTechnicalWithFloat>::const_iterator i;

    for (i=noteTechnicalWithFloats.begin (); i!=noteTechnicalWithFloats.end (); ++i) {
      S_msrTechnicalWithFloat
        technicalWithFloat = (*i);

      msrTechnicalWithFloatKind
        technicalWithFloatKind =
          technicalWithFloat->getTechnicalWithFloatKind ();

      int technicalWithFloatType = kComment; // JMI

      switch (technicalWithFloatKind) {
        case msrTechnicalWithFloatKind::kTechnicalWithFloatBend:
          technicalWithFloatType = k_bend;
          break;
      } // switch

      // create the technicalWithFloat element
      std::stringstream s;

      s << technicalWithFloat->getTechnicalWithFloatValue ();

      Sxmlelement technicalWithFloatElement =
        createMxmlelement (
          technicalWithFloatType,
          s.str ());

      // append the note technicalWithFloats element to the current note element
      appendToNoteNotationsTechnicals (
        technicalWithFloatElement,
        technicalWithFloat->getTechnicalWithFloatPlacementKind ());
    } // for
  }
}

//________________________________________________________________________
void msr2mxsrTranslator:: appendNoteTechnicalWithStrings (
  const S_msrNote& theMsrNote)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceTechnicals ()) {
    gLogStream <<
      "--> appendNoteTechnicalWithStrings, theMsrNote = " <<
      theMsrNote->asShortString () <<
      std::endl;
  }
#endif

/* JMI
<!ELEMENT technical
  ((up-bow | down-bow | harmonic | open-std::string |
    thumb-position | fingering | pluck | double-tongue |
    triple-tongue | stopped | snap-pizzicato | fret |
    std::string | hammer-on | pull-off | bend | tap | heel |
    toe | fingernails | hole | arrow | handbell |
    brass-bend | flip | smear | open | half-muted |
    harmon-mute | golpe | other-technical)*)>
<!ATTLIST technical
    %optional-unique-id;

    */

  // append the technicalWithString elements if any
  const std::list<S_msrTechnicalWithString>&
    noteTechnicalWithStrings =
      theMsrNote->getNoteTechnicalWithStrings () ;

  if (noteTechnicalWithStrings.size ()) {
    std::list<S_msrTechnicalWithString>::const_iterator i;

    for (i=noteTechnicalWithStrings.begin (); i!=noteTechnicalWithStrings.end (); ++i) {
      S_msrTechnicalWithString
        technicalWithString = (*i);

      msrTechnicalWithStringKind
        technicalWithStringKind =
          technicalWithString->getTechnicalWithStringKind ();

      int technicalWithStringType = kComment; // JMI

      switch (technicalWithStringKind) {
        case msrTechnicalWithStringKind::kHammerOn:
          technicalWithStringType = k_hammer_on;
          break;
        case msrTechnicalWithStringKind::kHandbell:
          technicalWithStringType = k_handbell;
          break;
        case msrTechnicalWithStringKind::kOtherTechnical:
          technicalWithStringType = k_other_technical;
          break;
        case msrTechnicalWithStringKind::kPluck:
          technicalWithStringType = k_pluck;
          break;
        case msrTechnicalWithStringKind::kPullOff:
          technicalWithStringType = k_pull_off;
          break;
      } // switch

      // create the technicalWithString element
      Sxmlelement technicalWithStringElement =
        createMxmlelement (
          technicalWithStringType,
          technicalWithString->getTechnicalWithStringValue ());

      // append the note technicalWithStrings element to the current note element
      appendToNoteNotationsTechnicals (
        technicalWithStringElement,
        technicalWithString->getTechnicalWithStringPlacementKind ());
    } // for
  }
}

//________________________________________________________________________
void msr2mxsrTranslator:: appendNoteArticulations (
  const S_msrNote& theMsrNote)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceArticulations ()) {
    gLogStream <<
      "--> appendNoteArticulations, theMsrNote = " <<
      theMsrNote->asShortString () <<
      std::endl;
  }
#endif

  // append the articulation elements if any
  const std::list<S_msrArticulation>&
    noteArticulations =
      theMsrNote->getNoteArticulations () ;

  if (noteArticulations.size ()) {
    std::list<S_msrArticulation>::const_iterator i;

    for (i=noteArticulations.begin (); i!=noteArticulations.end (); ++i) {
      const S_msrArticulation& articulation = (*i);

      msrArticulationKind
        articulationKind =
          articulation->getArticulationKind ();

      int articulationType = kComment; // JMI

/* JMI
  ((accent | strong-accent | staccato | tenuto |
    detached-legato | staccatissimo | spiccato |
    scoop | plop | doit | falloff | breath-mark |
    caesura | stress | unstress | soft-accent |
    other-articulation)*)>
    */

      switch (articulationKind) {
        case msrArticulationKind::kArticulation_NO_:
          // JMI ???
          break;

        case msrArticulationKind::kArticulationAccent:
          articulationType = k_accent;
          break;
        case msrArticulationKind::kArticulationBreathMark:
          articulationType = k_breath_mark;
          break;
        case msrArticulationKind::kArticulationCaesura:
          articulationType = k_caesura;
          break;
        case msrArticulationKind::kArticulationSpiccato:
          articulationType = k_spiccato;
          break;
        case msrArticulationKind::kArticulationStaccato:
          articulationType = k_staccato;
          break;
        case msrArticulationKind::kArticulationStaccatissimo:
          articulationType = k_staccatissimo;
          break;
        case msrArticulationKind::kArticulationStress:
          articulationType = k_stress;
          break;
        case msrArticulationKind::kArticulationUnstress:
          articulationType = k_unstress;
          break;
        case msrArticulationKind::kArticulationDetachedLegato:
          articulationType = k_detached_legato;
          break;
        case msrArticulationKind::kArticulationStrongAccent:
          articulationType = k_strong_accent;
          break;
        case msrArticulationKind::kArticulationTenuto:
          articulationType = k_tenuto;
          break;
        case msrArticulationKind::kArticulationFermata:
          articulationType = k_fermata;
          break;
        case msrArticulationKind::kArticulationArpeggiato:
          articulationType = k_arpeggiate;
          break;
        case msrArticulationKind::kArticulationNonArpeggiato:
          articulationType = k_non_arpeggiate;
          break;
        case msrArticulationKind::kArticulationDoit:
          articulationType = k_doit;
          break;
        case msrArticulationKind::kArticulationFalloff:
          articulationType = k_falloff;
          break;
        case msrArticulationKind::kArticulationPlop:
          articulationType = k_plop;
          break;
        case msrArticulationKind::kArticulationScoop:
          articulationType = k_scoop;
          break;
      } // switch

      // create the articulation element
      Sxmlelement articulationElement = createMxmlelement (articulationType, "");

      // append it to the current note notations articulations element
      switch (articulationKind) {
        case msrArticulationKind::kArticulation_NO_:
          // JMI ???
          break;

        case msrArticulationKind::kArticulationAccent:
        case msrArticulationKind::kArticulationBreathMark:
        case msrArticulationKind::kArticulationCaesura:
        case msrArticulationKind::kArticulationSpiccato:
        case msrArticulationKind::kArticulationStaccato:
        case msrArticulationKind::kArticulationStaccatissimo:
        case msrArticulationKind::kArticulationStress:
        case msrArticulationKind::kArticulationUnstress:
        case msrArticulationKind::kArticulationDetachedLegato:
        case msrArticulationKind::kArticulationStrongAccent:
        case msrArticulationKind::kArticulationTenuto:
        case msrArticulationKind::kArticulationDoit:
        case msrArticulationKind::kArticulationFalloff:
        case msrArticulationKind::kArticulationPlop:
        case msrArticulationKind::kArticulationScoop:
        case msrArticulationKind::kArticulationFermata:
          appendToNoteNotationsArticulations (
            articulationElement,
            articulation->getArticulationPlacementKind ());
          break;

        case msrArticulationKind::kArticulationArpeggiato:
        case msrArticulationKind::kArticulationNonArpeggiato:
          appendToNoteNotationsArticulations (
            articulationElement,
            articulation->getArticulationPlacementKind ());
          break;
      } // switch

        /*
        */
    } // for
  }
}

//________________________________________________________________________
void msr2mxsrTranslator:: appendNoteTieIfAny (
  const S_msrNote& theMsrNote)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceTies ()) {
    gLogStream <<
      "--> appendNoteTieIfAny, theMsrNote = " <<
      theMsrNote->asShortString () <<
      std::endl;
  }
#endif

  // append the tie element if any
  S_msrTie noteTie = theMsrNote->getNoteTie ();

  if (noteTie) {
    // create the tied element
    Sxmlelement tiedElement = createMxmlelement (k_tied, "");

    // set its "type" attribute if any
    std::string typeString;

    switch (noteTie->getTieKind ()) {
      case msrTieKind::kTieNone:
        break;
      case msrTieKind::kTieStart:
        typeString = "start";
        break;
      case msrTieKind::kTieContinue:
        typeString = "continue";
        break;
      case msrTieKind::kTieStop:
        typeString = "stop";
        break;
    } // switch

    if (typeString.size ()) {
      tiedElement->add (
        createMxmlAttribute ("type", typeString));
    }

    // append it to the current note notations element
    appendToNoteNotations (
      tiedElement,
      noteTie->getTiePlacementKind ());
  }
}

//________________________________________________________________________
void msr2mxsrTranslator:: appendNoteSlursIfAny (
  const S_msrNote& theMsrNote)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceSlurs ()) {
    gLogStream <<
      "--> appendNoteSlursIfAny, theMsrNote = " <<
      theMsrNote->asShortString () <<
      std::endl;
  }
#endif

  // append the slur elements if any
  const std::list<S_msrSlur>&
    noteSlurs =
      theMsrNote->getNoteSlurs () ;

  if (noteSlurs.size ()) {
    std::list<S_msrSlur>::const_iterator i;

    for (i=noteSlurs.begin (); i!=noteSlurs.end (); ++i) {
      S_msrSlur
        slur = (*i);

      // create the slur element
      Sxmlelement slurElement = createMxmlelement (k_slur, "");

      // create the slur number attribute
      int slurNumber = slur->getSlurNumber ();

      if (slurNumber > 0) {
        slurElement->add (createMxmlIntegerAttribute ("number", slurNumber));
      }

      // create the slur type attribute
      msrSlurTypeKind
        slurTypeKind =
          slur->getSlurTypeKind ();

      std::string slurTypeString;

      switch (slurTypeKind) {
        case msrSlurTypeKind::kSlurType_NO_:
          break;
        case msrSlurTypeKind::kSlurTypeRegularStart:
          slurTypeString = "start";
          break;
        case msrSlurTypeKind::kSlurTypeRegularContinue:
          slurTypeString = "continue";
          break;
        case msrSlurTypeKind::kSlurTypeRegularStop:
          slurTypeString = "stop";
          break;

        case msrSlurTypeKind::kSlurTypePhrasingStart:
          slurTypeString = "start";
          break;
        case msrSlurTypeKind::kSlurTypePhrasingContinue:
          slurTypeString = "continue";
          break;
        case msrSlurTypeKind::kSlurTypePhrasingStop:
          slurTypeString = "stop";
          break;
      } // switch

      if (slurTypeString.size ()) {
        slurElement->add (createMxmlAttribute ("type", slurTypeString));
      }

      // get the slur placement kind
      msrPlacementKind
        slurPlacementKind =
          slur->getSlurPlacementKind ();

      // append the slur element to the current note note notations element
      appendToNoteNotations (
        slurElement,
        slurPlacementKind);
    } // for
  }
}

//________________________________________________________________________
void msr2mxsrTranslator:: appendNoteTupletIfRelevant (
  const S_msrNote& theMsrNote)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceTuplets ()) {
    gLogStream <<
      "--> appendNoteTupletIfRelevant, theMsrNote = " <<
      theMsrNote->asShortString () <<
      std::endl;
  }
#endif

  switch (theMsrNote->getNoteKind ()) {
    case msrNoteKind::kNote_NO_:
    case msrNoteKind::kNoteRestInMeasure:
    case msrNoteKind::kNoteSkipInMeasure:
    case msrNoteKind::kNoteUnpitchedInMeasure:
    case msrNoteKind::kNoteRegularInMeasure:
    case msrNoteKind::kNoteRegularInChord:
      break;

    case msrNoteKind::kNoteRegularInTuplet:
    case msrNoteKind::kNoteRestInTuplet:
      {
        // get theMsrNote's uplink to tuplet
        S_msrTuplet
          noteDirectUpLinkToTuplet =
            theMsrNote->
              getNoteDirectUpLinkToTuplet ();

        // get theMsrNote's position in tuplet
        size_t
          notePositionInTuplet =
            theMsrNote->
              getPositionInTuplet (); // JMI v0.9.66

        // compute the type std::string if relevant
        std::string typeString;

        if (notePositionInTuplet == 1) {
          typeString = "start";
        }
        else if (
          notePositionInTuplet
            ==
          noteDirectUpLinkToTuplet->getTupletElementsList ().size ()
        ) {
          typeString = "stop";
        }

        if (typeString.size ()) {
          // create a tuplet element
          Sxmlelement tupletElement = createMxmlelement (k_tuplet, "");

          // set its "number" attribute
          tupletElement->add (
            createMxmlIntegerAttribute (
              "number",
              noteDirectUpLinkToTuplet->getTupletNumber ()));

          // set its "type" attribute
          tupletElement->add (
            createMxmlAttribute (
              "type",
              typeString));

          // append it to the current note notations elements
          appendToNoteNotations (
            tupletElement,
            msrPlacementKind::kPlacement_NO_);
        }
      }
      break;

    case msrNoteKind::kNoteInDoubleTremolo:
    case msrNoteKind::kNoteRegularInGraceNotesGroup:
    case msrNoteKind::kNoteSkipInGraceNotesGroup:
      break;

    case msrNoteKind::kNoteInChordInGraceNotesGroup:
      break;

    case msrNoteKind::kNoteInTupletInGraceNotesGroup:
      break;

    case msrNoteKind::kNoteUnpitchedInTuplet:
      break;
  } // switch
}

//________________________________________________________________________
void msr2mxsrTranslator:: appendNoteSpannersBeforeNote (
  const S_msrNote& theMsrNote)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceSpanners ()) {
    gLogStream <<
      "--> appendNoteSpannersBeforeNote, theMsrNote = " <<
      theMsrNote->asShortString () <<
      std::endl;
  }
#endif

  // append the spanner elements if any
  const std::list<S_msrSpanner>&
    noteSpanners =
      theMsrNote->
        getNoteSpanners () ;

  if (noteSpanners.size ()) {
    std::list<S_msrSpanner>::const_iterator i;

    for (i=noteSpanners.begin (); i!=noteSpanners.end (); ++i) {
      S_msrSpanner spanner = (*i);

      msrSpannerTypeKind
        spannerTypeKind =
          spanner->
            getSpannerTypeKind ();

      // should we handle this spanner at all?
      Bool doHandleSpanner (true);

      switch (spannerTypeKind) {
        case msrSpannerTypeKind::kSpannerTypeStart:
          break;
        case msrSpannerTypeKind::kSpannerTypeStop:
          doHandleSpanner = false;
          break;
        case msrSpannerTypeKind::kSpannerTypeContinue:
          break;
        case msrSpannerTypeKind::kSpannerType_NO_:
          // should not occur
          break;
      } // switch

      if (doHandleSpanner) {
        msrSpannerKind // twice ??? JMI v0.9.66
          spannerKind =
            spanner->
              getSpannerKind ();

        int spannerType = kComment; // JMI

        Sxmlelement containingElement;

        switch (spannerKind) {
          case msrSpannerKind::kSpannerDashes:
            // dashes go into the measure direction element
            spannerType = k_dashes;
            break;
          case msrSpannerKind::kSpannerWavyLine:
            // wavy lines go into the note notations ornaments
            spannerType = k_wavy_line;
            break;
        } // switch

        // create the spanner element
        Sxmlelement spannerElement = createMxmlelement (spannerType, "");

        // set spannerElement's "number" attribute if relevant
        int
          spannerNumber =
            spanner->getSpannerNumber ();

        if (spannerNumber > 0) {
          spannerElement->add (createMxmlIntegerAttribute ("number", spannerNumber));
        }

        // set spannerElement's "type" attribute if relevant
        std::string
          typeString =
            msrSpannerTypeKindAsMusicXMLString (spannerTypeKind);

        if (typeString.size ()) {
          spannerElement->add (createMxmlAttribute ("type", typeString));
        }

        switch (spannerKind) {
          case msrSpannerKind::kSpannerDashes:
            // dashes go into the measure direction element
            appendToMeasureDirection (
              spannerElement,
              spanner->getSpannerPlacementKind ());
            break;
          case msrSpannerKind::kSpannerWavyLine:
            // wavy lines go into the note notations ornaments
            appendToNoteNotationsOrnaments (
              spannerElement,
              spanner->getSpannerPlacementKind ());
              break;
        } // switch
      }
    } // for
  }
}

//________________________________________________________________________
void msr2mxsrTranslator:: appendNoteSpannersAfterNote (
  const S_msrNote& theMsrNote)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceSpanners ()) {
    gLogStream <<
      "--> appendNoteSpannersAfterNote, theMsrNote = " <<
      theMsrNote->asShortString () <<
      std::endl;
  }
#endif

  // append the spanner elements if any
  const std::list<S_msrSpanner>&
    noteSpanners =
      theMsrNote->getNoteSpanners () ;

  if (noteSpanners.size ()) {
    std::list<S_msrSpanner>::const_iterator i;

    for (i=noteSpanners.begin (); i!=noteSpanners.end (); ++i) {
      S_msrSpanner spanner = (*i);

      msrSpannerTypeKind
        spannerTypeKind =
          spanner->getSpannerTypeKind ();

      // should we handle this spanner at all?
      Bool doHandleSpanner (true);

      switch (spannerTypeKind) {
        case msrSpannerTypeKind::kSpannerTypeStart:
          doHandleSpanner = false;
          break;
        case msrSpannerTypeKind::kSpannerTypeStop:
          break;
        case msrSpannerTypeKind::kSpannerTypeContinue:
          break;
        case msrSpannerTypeKind::kSpannerType_NO_:
          // should not occur
          break;
      } // switch

      if (doHandleSpanner) {
        msrSpannerKind
          spannerKind =
            spanner->getSpannerKind ();

        int spannerType = kComment; // JMI

        Sxmlelement containingElement;

        switch (spannerKind) {
          case msrSpannerKind::kSpannerDashes:
            // dashes go into the measure direction element
            spannerType = k_dashes;
            break;
          case msrSpannerKind::kSpannerWavyLine:
            // wavy lines go into the note notations ornaments
            spannerType = k_wavy_line;
            break;
        } // switch

        // create the spanner element
        Sxmlelement spannerElement = createMxmlelement (spannerType, "");

        // set spannerElement's "number" attribute if relevant
        int
          spannerNumber =
            spanner->getSpannerNumber ();

        if (spannerNumber > 1) {
          spannerElement->add (createMxmlIntegerAttribute ("number", spannerNumber));
        }

        // set spannerElement's "type" attribute if relevant
        std::string
          typeString =
            msrSpannerTypeKindAsMusicXMLString (spannerTypeKind);

        if (typeString.size ()) {
          spannerElement->add (createMxmlAttribute ("type", typeString));
        }

        switch (spannerKind) {
          case msrSpannerKind::kSpannerDashes:
            // dashes go into the measure direction element
            appendToMeasureDirection (
              spannerElement,
              spanner->getSpannerPlacementKind ());
            break;
          case msrSpannerKind::kSpannerWavyLine:
            // wavy lines go into the note notations ornaments
            appendToNoteNotationsOrnaments (
              spannerElement,
              spanner->getSpannerPlacementKind ());
              break;
        } // switch
      }
    } // for
  }
}

//________________________________________________________________________
void msr2mxsrTranslator:: appendStemToNote (
  const S_msrNote& theMsrNote)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceStems ()) {
    gLogStream <<
      "--> appendStemToNote, theMsrNote = " <<
      theMsrNote->asShortString () <<
      std::endl;
  }
#endif

  S_msrStem stem = theMsrNote->getNoteStem ();

  if (stem) {
    msrStemKind
      stemKind = stem->getStemKind ();

    std::string stemString;

    switch (stemKind) {
      case msrStemKind::kStemNeutral:
        stemString = "[NONE]";
        break;
      case msrStemKind::kStemUp:
        stemString = "up";
        break;
      case msrStemKind::kStemDown:
        stemString = "down";
        break;
      case msrStemKind::kStemDouble:
        stemString = "double";
        break;
    } // switch

    // append a slur element to the current note element
    fCurrentNoteElement->push (
      createMxmlelement (
        k_stem,
        stemString));
  }
}

//________________________________________________________________________
void msr2mxsrTranslator::appendBeamsToNote (
  const S_msrNote& theMsrNote)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceBeams ()) {
    gLogStream <<
      "--> appendBeamsToNote, theMsrNote = " <<
      theMsrNote->asShortString () <<
      std::endl;
  }
#endif

  // append the beam elements if any
  const std::list<S_msrBeam>&
    noteBeams =
      theMsrNote->getNoteBeams () ;

  if (noteBeams.size ()) {
    std::list<S_msrBeam>::const_iterator i;

    for (i=noteBeams.begin (); i!=noteBeams.end (); ++i) {
      S_msrBeam beam = (*i);

      msrBeamKind
        beamKind =
          beam->getBeamKind ();

      // create a beam element
      std::string beamString;

      switch (beamKind) {
        case msrBeamKind::kBeam_NO_:
          break;
        case msrBeamKind::kBeamBegin:
          beamString = "begin";
          break;
        case msrBeamKind::kBeamContinue:
          beamString = "continue";
          break;
        case msrBeamKind::kBeamEnd:
          beamString = "end";
          break;
        case msrBeamKind::kBeamForwardHook:
          beamString = "forward hook";
          break;
        case msrBeamKind::kBeamBackwardHook:
          beamString = "backward hook";
          break;
      } // switch

      Sxmlelement beamElement = createMxmlelement (k_beam, beamString);

      // set its "number" attribute
      beamElement->add (createMxmlIntegerAttribute ("number", beam->getBeamNumber ()));

      // append the beam element to the current note element
      fCurrentNoteElement->push (beamElement);
    } // for
  }
}

//________________________________________________________________________
void msr2mxsrTranslator:: appendStaffToNoteIfRelevant (
  const S_msrNote& theMsrNote)
{
#ifdef TRACING_IS_ENABLED
  int inputLineNumber =
    theMsrNote->getInputLineNumber ();

  if (gGlobalTracingOahGroup->getTraceNotes ()) {
    gLogStream <<
      "--> appendStaffToNoteIfRelevant(), theMsrNote = " <<
      theMsrNote->asShortString () <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  // fetch theMsrNote staff
  S_msrStaff
    noteStaff =
      theMsrNote->
        fetchNoteUpLinkToVoice ()->
          getVoiceUpLinkToStaff ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceNotesDetails ()) {
    gLogStream <<
      std::endl <<
      "--> noteStaff: ";
    if (noteStaff) {
      gLogStream <<
        noteStaff;
    }
    else {
      gLogStream <<
        "[NONE]";
    }
  }
#endif

  // append the staff attribute if relevant
  if (noteStaff) {
    int
      noteStaffNumber =
        noteStaff->
          getStaffNumber ();

    if (true || noteStaffNumber != 1) { // options ? JMI
      fCurrentNoteElement->push (
        createMxmlIntegerElement (
          k_staff,
          noteStaffNumber));
    }
  }
}

//________________________________________________________________________
void msr2mxsrTranslator::appendVoiceToNoteIfRelevant (
  const S_msrNote& theMsrNote)
{
#ifdef TRACING_IS_ENABLED
  int inputLineNumber =
    theMsrNote->getInputLineNumber ();

  if (gGlobalTracingOahGroup->getTraceNotes ()) {
    gLogStream <<
      "--> appendVoiceToNoteIfRelevant(), theMsrNote = " <<
      theMsrNote->asShortString () <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  // fetch theMsrNote voice
  S_msrVoice
    noteVoice =
      theMsrNote->fetchNoteUpLinkToVoice ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceNotesDetails ()) {
    gLogStream <<
      std::endl <<
      "--> noteVoice: ";
    if (noteVoice) {
      gLogStream <<
        noteVoice;
    }
    else {
      gLogStream <<
        "[NONE]";
    }
  }
#endif

  // append the voice attribute if relevant
  if (noteVoice) {
    int
      noteVoiceNumber =
        noteVoice->
          getVoiceNumber ();

    if (true || noteVoiceNumber != 1) { // options ? JMI
      fCurrentNoteElement->push (
        createMxmlIntegerElement (
          k_voice,
          noteVoiceNumber));
    }
  }
}

//________________________________________________________________________
void msr2mxsrTranslator:: appendNoteNotationsToNote (
                                const S_msrNote& theMsrNote)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceNotes ()) {
    gLogStream <<
      "--> appendNoteNotationsToNote, theMsrNote = " <<
      theMsrNote->asShortString () <<
      std::endl;
  }
#endif

/*
<!ELEMENT notations
  (%editorial;,
   (tied | slur | tuplet | glissando | slide |
    ornaments | technical | articulations | dynamics |
    fermata | arpeggiate | non-arpeggiate |
    accidental-mark | other-notation)*)>
<!ATTLIST notations
    %print-object;
    %optional-unique-id;
>
*/

  // append the ornaments elements if any
  appendNoteOrnaments (theMsrNote);

  // append the articulation elements if any
  appendNoteArticulations (theMsrNote);

  // append the technical elements if any
  appendNoteTechnicals (theMsrNote);
  appendNoteTechnicalWithIntegers (theMsrNote);
  appendNoteTechnicalWithFloats (theMsrNote);
  appendNoteTechnicalWithStrings (theMsrNote);

  // append the tie element if any
  appendNoteTieIfAny (theMsrNote);

  // append the slur elements if any
  appendNoteSlursIfAny (theMsrNote);

  // append the tuplet elements if relevant
  appendNoteTupletIfRelevant (theMsrNote);
}

//________________________________________________________________________
void msr2mxsrTranslator:: appendNoteLyricsToNote (
  const S_msrNote& theMsrNote)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceLyrics ()) {
    gLogStream <<
      "--> appendNoteLyricsToNote, theMsrNote = " <<
      theMsrNote->asShortString () <<
      std::endl;
  }
#endif

/*
<!ELEMENT lyric
  ((((syllabic?, text),
     (elision?, syllabic?, text)*, extend?) |
     extend | laughing | humming),
    end-line?, end-paragraph?, %editorial;)>
<!ATTLIST lyric
    number NMTOKEN #IMPLIED
    name CDATA #IMPLIED
    %justify;
    %position;
    %placement;
    %color;
    %print-object;
>
*/

  // append the lyric elements if any
  const std::list<S_msrSyllable>&
    noteSyllables =
      theMsrNote->getNoteSyllables () ;

  if (noteSyllables.size ()) {
    std::list<S_msrSyllable>::const_iterator i;

    for (i=noteSyllables.begin (); i!=noteSyllables.end (); ++i) {
      S_msrSyllable syllable = (*i);

      // create and append a lyric element if relevant
      Bool   doCreateALyricElement (false);
      std::string syllabicString;

      switch ( syllable->getSyllableKind ()) {
        case msrSyllableKind::kSyllableNone:
          // should not occur
          break;

        case msrSyllableKind::kSyllableSingle:
          doCreateALyricElement = true;
          syllabicString = "single";
          break;

        case msrSyllableKind::kSyllableBegin:
          doCreateALyricElement = true;
          syllabicString = "begin";
          break;
        case msrSyllableKind::kSyllableMiddle:
          doCreateALyricElement = true;
          syllabicString = "middle";
          break;
        case msrSyllableKind::kSyllableEnd:
          doCreateALyricElement = true;
          syllabicString = "end";
          break;

        case msrSyllableKind::kSyllableOnRestNote:
          break;

        case msrSyllableKind::kSyllableSkipRestNote:
          break;
        case msrSyllableKind::kSyllableSkipNonRestNote:
          break;

        case msrSyllableKind::kSyllableMeasureEnd:
          break;

        case msrSyllableKind::kSyllableLineBreak:
          break;
        case msrSyllableKind::kSyllablePageBreak:
          break;
      } // switch

      if (doCreateALyricElement) {
        // create the lyric element
        Sxmlelement lyricElement = createMxmlelement (k_lyric, "");

        // set its "number" attribute
        lyricElement->add (
          createMxmlAttribute (
            "number",
            syllable->getSyllableStanzaNumber ()));

        // append a syllabic element to the lyric element if relevant
        if (syllabicString.size ()) {
          lyricElement->push (
            createMxmlelement (k_syllabic, syllabicString));
        }

        // append a text elements to the lyric element if relevant
        const std::list<std::string>&
          syllableTextsList =
            syllable->getSyllableTextsList ();

        for (
          std::list<std::string>::const_iterator i = syllableTextsList.begin ();
          i!=syllableTextsList.end ();
          ++i
        ) {
          std::string text = (*i);

          lyricElement->push (
            createMxmlelement (k_text, text));
        } // for

        // append the extend element to the lyric element if relevant
        std::string extendTypeString;
        Bool   doCreateAnExtendElement (true);

        switch (syllable->getSyllableExtendKind ()) {
          case msrSyllableExtendKind::kSyllableExtendNone:
          doCreateAnExtendElement = false;
            break;
          case msrSyllableExtendKind::kSyllableExtendEmpty:
            break;
          case msrSyllableExtendKind::kSyllableExtendSingle:
            extendTypeString = "single";
            break;
          case msrSyllableExtendKind::kSyllableExtendStart:
            extendTypeString = "start";
            break;
          case msrSyllableExtendKind::kSyllableExtendContinue:
            extendTypeString = "continue";
            break;
          case msrSyllableExtendKind::kSyllableExtendStop:
            extendTypeString = "stop";
            break;
        } // switch

        if (doCreateAnExtendElement) {
          // create an extend element
          Sxmlelement extendElement = createMxmlelement (k_extend, "");

          if (extendTypeString.size ()) {
            // set its "type" attribute
            extendElement->add (createMxmlAttribute ("type", extendTypeString));
          }

          // append the extend element to the lyric element
          lyricElement->push (extendElement);
        }

        // append a syllabic element to the lyric element if relevant

        // append the lyric element to the current note element
        fCurrentNoteElement->push (lyricElement);
      }
    } // for
  }
}

//________________________________________________________________________
void msr2mxsrTranslator::appendBasicsToNote (
  const S_msrNote& theMsrNote)
{
  int inputLineNumber =
    theMsrNote->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceNotes ()) {
    gLogStream <<
      "--> appendBasicsToNote(), theMsrNote = " <<
      theMsrNote->asShortString () <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  // grab theMsrNote's informations
  msrNoteKind
    noteKind =
      theMsrNote->getNoteKind ();

  msrQuarterTonesPitchKind
    noteQuarterTonesPitchKind =
      theMsrNote->getNoteQuarterTonesPitchKind ();

  msrDiatonicPitchKind noteDiatonicPitchKind;
  msrAlterationKind    noteAlterationKind;

  fetchDiatonicPitchKindAndAlterationKindFromQuarterTonesPitchKind (
    inputLineNumber,
    noteQuarterTonesPitchKind,
    noteDiatonicPitchKind,
    noteAlterationKind);

  msrOctaveKind
    noteOctaveKind =
      theMsrNote->getNoteOctaveKind ();

  float
    noteMusicXMLAlter =
      msrMusicXMLAlterFromAlterationKind (
        noteAlterationKind);

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceNotes ()) {
    gLogStream <<
      "-->  noteKind: " << msrNoteKindAsString (noteKind) <<
      "-->  noteOctaveKind: " <<
      msrOctaveKindAsString (noteOctaveKind) <<
      "-->  noteDiatonicPitchKind: " <<
      msrDiatonicPitchKindAsString (noteDiatonicPitchKind) <<
      std::endl;
  }
#endif

  // append the chord sub element if relevant
  switch (noteKind) {
    case msrNoteKind::kNoteRegularInChord:
      if (! theMsrNote->getNoteIsAChordsFirstMemberNote ()) {
        fCurrentNoteElement->push (createMxmlelement (k_chord, ""));
      }
      break;
    default:
      ;
  } // switch

  // append the grace sub element if relevant
  switch (noteKind) {
    case msrNoteKind::kNoteRegularInGraceNotesGroup:
    case msrNoteKind::kNoteSkipInGraceNotesGroup:
      fCurrentNoteElement->push (createMxmlelement (k_grace, ""));
      break;
    default:
      ;
  } // switch

  // append the step and pitch or rest sub elements
  switch (noteKind) {
    case msrNoteKind::kNote_NO_:
      break;

    case msrNoteKind::kNoteRestInMeasure:
      fCurrentNoteElement->push (createMxmlelement (k_rest, ""));
      break;

    case msrNoteKind::kNoteSkipInMeasure:
      break;

    case msrNoteKind::kNoteUnpitchedInMeasure:
      break;

    case msrNoteKind::kNoteRegularInMeasure:
    case msrNoteKind::kNoteRegularInChord:
    case msrNoteKind::kNoteRegularInTuplet:
      {
        // create the pitch element
        Sxmlelement pitchElement = createMxmlelement (k_pitch, "");

        // append the step element
        pitchElement->push (
          createMxmlelement (
            k_step,
            msrDiatonicPitchKindAsString (noteDiatonicPitchKind)));

        if (noteMusicXMLAlter != 0.0) {
          // append the alter element
          std::stringstream s;
          s << std::setprecision (2) << noteMusicXMLAlter;
          pitchElement->push (
            createMxmlelement (
              k_alter,
              s.str ()));
        }

        // append the octave element
        pitchElement->push (
          createMxmlIntegerElement (
            k_octave,
            octaveNumberFromOctaveKind (noteOctaveKind)));

        fCurrentNoteElement->push (pitchElement);
      }
      break;

    case msrNoteKind::kNoteRestInTuplet:
      fCurrentNoteElement->push (createMxmlelement (k_rest, ""));
      break;

    case msrNoteKind::kNoteInDoubleTremolo:
      break;

    case msrNoteKind::kNoteRegularInGraceNotesGroup:
    case msrNoteKind::kNoteSkipInGraceNotesGroup:
      {
        // create the pitch element
        Sxmlelement pitchElement = createMxmlelement (k_pitch, "");

        // append the step element
        pitchElement->push (
          createMxmlelement (
            k_step,
            msrDiatonicPitchKindAsString (noteDiatonicPitchKind)));

        if (noteMusicXMLAlter != 0.0) {
          // append the alter element
          std::stringstream s;
          s << std::setprecision (2) << noteMusicXMLAlter;
          pitchElement->push (
            createMxmlelement (
              k_alter,
              s.str ()));
        }

        // append the octave element
        pitchElement->push (
          createMxmlIntegerElement (
            k_octave,
            octaveNumberFromOctaveKind (noteOctaveKind)));

        // append the pitch element to the current note
        fCurrentNoteElement->push (pitchElement);
      }
      break;

    case msrNoteKind::kNoteInChordInGraceNotesGroup:
      break;

    case msrNoteKind::kNoteInTupletInGraceNotesGroup:
      break;

    case msrNoteKind::kNoteUnpitchedInTuplet:
      break;
  } // switch
}

//________________________________________________________________________
void msr2mxsrTranslator::appendDurationToNoteIfRelevant (
  const S_msrNote& theMsrNote)
{
  int inputLineNumber =
    theMsrNote->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceNotes ()) {
    gLogStream <<
      "--> appendDurationToNoteIfRelevant(1), theMsrNote = " <<
      theMsrNote->asShortString () <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  // grab theMsrNote's informations
  msrNoteKind
    noteKind =
      theMsrNote->getNoteKind ();

  Rational
    noteSoundingWholeNotes =
      theMsrNote->getMeasureElementSoundingWholeNotes ();

#ifdef TRACING_IS_ENABLED
  Rational
    noteDisplayWholeNotes =
      theMsrNote->getNoteDisplayWholeNotes ();

  if (gGlobalTracingOahGroup->getTraceNotes ()) {
    gLogStream <<
      "---> noteKind: " << msrNoteKindAsString (noteKind) <<
      ", noteSoundingWholeNotes: " << noteSoundingWholeNotes <<
      ", noteDisplayWholeNotes: " << noteDisplayWholeNotes <<
      ", noteTupletFactor: " << theMsrNote->getNoteTupletFactor ().asRational () <<
      ", divisionsPerQuarterNote: " << fDivisionsPerQuarterNote <<
      ", partShortestNoteDuration: " << fPartShortestNoteDuration <<
      ", fDivisionsMultiplyingFactor: " << fDivisionsMultiplyingFactor <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  Bool doAppendDuration (false);

  switch (noteKind) {
    case msrNoteKind::kNote_NO_:
      break;

    case msrNoteKind::kNoteSkipInMeasure:
      // nothing is generated at once for a skip note
      break;

    case msrNoteKind::kNoteUnpitchedInMeasure:
      break;

    case msrNoteKind::kNoteRestInMeasure:
    case msrNoteKind::kNoteRegularInMeasure:
    case msrNoteKind::kNoteRegularInChord:
      doAppendDuration = true;
      break;

    case msrNoteKind::kNoteRegularInTuplet:
    case msrNoteKind::kNoteRestInTuplet:
      doAppendDuration = true;
      break;


    case msrNoteKind::kNoteInDoubleTremolo:
      break;

    case msrNoteKind::kNoteRegularInGraceNotesGroup:
    case msrNoteKind::kNoteSkipInGraceNotesGroup:
      break;

    case msrNoteKind::kNoteInChordInGraceNotesGroup:
      break;

    case msrNoteKind::kNoteInTupletInGraceNotesGroup:
      break;

    case msrNoteKind::kNoteUnpitchedInTuplet:
      break;
  } // switch

  if (doAppendDuration) {
    Rational
      soundingDurationAsRational =
        noteSoundingWholeNotes
          /
        fPartShortestNoteDuration
          *
        fDivisionsMultiplyingFactor;

#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceNotes ()) {
      gLogStream <<
        "---> soundingDurationAsRational: " <<
        soundingDurationAsRational <<
        "--> line " << inputLineNumber <<
        std::endl;
    }
#endif

    if (soundingDurationAsRational.getDenominator () != 1) {
      std::stringstream s;

      s <<
        "soundingDurationAsRational '" << soundingDurationAsRational <<
        "' is no integer number" <<
        ", line " << inputLineNumber;

      msr2mxsrInternalError (
        gGlobalServiceRunData->getInputSourceName (),
        inputLineNumber,
        __FILE__, __LINE__,
        s.str ());
    }

    fCurrentNoteElement->push (
      createMxmlIntegerElement (
        k_duration,
        soundingDurationAsRational.getNumerator ()));

#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceNotesDetails ()) {
      gLogStream <<
        std::endl <<
        "--> appendDurationToNoteIfRelevant(2): " <<
        theMsrNote <<
        std::endl;
    }
#endif
  }
}

void msr2mxsrTranslator::appendTimeModificationToNoteIfRelevant (
  const S_msrNote& theMsrNote)
{
#ifdef TRACING_IS_ENABLED
  int inputLineNumber =
    theMsrNote->getInputLineNumber ();

  if (gGlobalTracingOahGroup->getTraceNotes ()) {
    gLogStream <<
      "--> appendTimeModificationToNoteIfRelevant(), theMsrNote = " <<
      theMsrNote->asShortString () <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  msrNoteKind
    noteKind =
      theMsrNote->getNoteKind ();

  // append the time modification if relevant
  switch (noteKind) {
    case msrNoteKind::kNote_NO_:
      break;

    case msrNoteKind::kNoteRestInMeasure:
      break;

    case msrNoteKind::kNoteSkipInMeasure:
      break;

    case msrNoteKind::kNoteUnpitchedInMeasure:
      break;

    case msrNoteKind::kNoteRegularInMeasure:
      break;

    case msrNoteKind::kNoteInDoubleTremolo:
      break;

    case msrNoteKind::kNoteRegularInGraceNotesGroup:
    case msrNoteKind::kNoteSkipInGraceNotesGroup:
      break;

    case msrNoteKind::kNoteInChordInGraceNotesGroup:
      break;

    case msrNoteKind::kNoteRegularInChord:
      break;

    case msrNoteKind::kNoteRegularInTuplet:
    case msrNoteKind::kNoteRestInTuplet:
      {
        Sxmlelement
          timeModificationElement = createMxmlelement (k_time_modification, "");

        timeModificationElement->push (
          createMxmlIntegerElement (
            k_actual_notes,
            theMsrNote->getNoteTupletFactor ().getTupletActualNotes ()));
        timeModificationElement->push (
          createMxmlIntegerElement (
            k_normal_notes,
            theMsrNote->getNoteTupletFactor ().getTupletNormalNotes ()));

        fCurrentNoteElement->push (timeModificationElement);
      }
      break;

    case msrNoteKind::kNoteInTupletInGraceNotesGroup:
      break;

    case msrNoteKind::kNoteUnpitchedInTuplet:
      break;
  } // switch
}

//________________________________________________________________________
void msr2mxsrTranslator::appendMsrNoteToMesureIfRelevant (
  const S_msrNote& theMsrNote)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceNotes ()) {
    gLogStream <<
      "--> appendMsrNoteToMesureIfRelevant, theMsrNote = " <<
      theMsrNote->asShortString () <<
      std::endl;
  }
#endif

  int inputLineNumber =
    theMsrNote->getInputLineNumber ();

  // should a note sub-element be generated?
  Bool doGenerateNote (true);

  switch (theMsrNote->getNoteKind ()) {
    case msrNoteKind::kNote_NO_:
      break;
    case msrNoteKind::kNoteRestInMeasure:
      break;
    case msrNoteKind::kNoteSkipInMeasure:
      doGenerateNote = false;
      // cumulating the skip notes durations for <forward /> elements generation
      fCurrentCumulatedSkipsDurations +=
        theMsrNote->getMeasureElementSoundingWholeNotes ();
      break;
    case msrNoteKind::kNoteUnpitchedInMeasure:
      break;
    case msrNoteKind::kNoteRegularInMeasure:
      break;

    case msrNoteKind::kNoteRegularInChord:
      if (theMsrNote->getNoteIsAChordsFirstMemberNote ()) {
        if (false && fPendingChordStartCommentElement) { // JMI
          // append the pending chord start comment to the current part element
          fCurrentPartElement->push (fPendingChordStartCommentElement);
        }
      }
      break;

    case msrNoteKind::kNoteRegularInTuplet:
    case msrNoteKind::kNoteRestInTuplet:
      break;

    case msrNoteKind::kNoteInDoubleTremolo:
    case msrNoteKind::kNoteRegularInGraceNotesGroup:
    case msrNoteKind::kNoteSkipInGraceNotesGroup:
      break;

    case msrNoteKind::kNoteInChordInGraceNotesGroup:
      break;

    case msrNoteKind::kNoteInTupletInGraceNotesGroup:
      break;

    case msrNoteKind::kNoteUnpitchedInTuplet:
      break;
  } // switch

  if (doGenerateNote) {
    // create a note element
    fCurrentNoteElement = createMxmlelement (k_note, "");

    // append theMsrNote basic sub-elements
    appendBasicsToNote (theMsrNote);

    // append the duration sub-element if relevant
    appendDurationToNoteIfRelevant (theMsrNote);

    // append the voice sub-element if relevant
    appendVoiceToNoteIfRelevant (theMsrNote);

    // append the type sub-element if relevant
    Bool doGenerateType (true);

    switch (theMsrNote->getNoteKind ()) {
      case msrNoteKind::kNote_NO_:
        break;

      case msrNoteKind::kNoteRestInMeasure:
        doGenerateType = false;
        break;

      case msrNoteKind::kNoteSkipInMeasure:
        break;

      case msrNoteKind::kNoteUnpitchedInMeasure:
        break;

      case msrNoteKind::kNoteRegularInMeasure:
        break;

      case msrNoteKind::kNoteRegularInChord:
        break;

      case msrNoteKind::kNoteRegularInTuplet:
      case msrNoteKind::kNoteRestInTuplet:
        break;

      case msrNoteKind::kNoteInDoubleTremolo:
        break;

      case msrNoteKind::kNoteRegularInGraceNotesGroup:
      case msrNoteKind::kNoteSkipInGraceNotesGroup:
        break;

      case msrNoteKind::kNoteInChordInGraceNotesGroup:
        break;

      case msrNoteKind::kNoteInTupletInGraceNotesGroup:
        break;

      case msrNoteKind::kNoteUnpitchedInTuplet:
        break;
    } // switch

    if (doGenerateType) {
      // append a type element containing the graphic duration
      // as a MusicXML std::string
      fCurrentNoteElement->push (
        createMxmlelement (
          k_type,
          theMsrNote->noteGraphicDurationAsMusicXMLString ()));
    }

    // append the time-modification sub-element if relevant
    appendTimeModificationToNoteIfRelevant (theMsrNote);

    // append the dots sub-element if relevant
    int
      noteDotsNumber =
        theMsrNote->getNoteDotsNumber ();

#ifdef TRACING_IS_ENABLED
    if (gGlobalTracingOahGroup->getTraceNotes ()) {
      gLogStream <<
        "-->  noteDotsNumber: " << noteDotsNumber <<
        "--> line " << inputLineNumber <<
        std::endl;
    }
#endif

    for (int i = 0; i < noteDotsNumber; ++i) {
      fCurrentNoteElement->push (
        createMxmlelement (
          k_dot, ""));
    } // for

    // append the accidental if any
    msrAccidentalKind
      accidentalKind =
        theMsrNote->getNoteAccidentalKind ();

    std::string
      accidentalString =
        msrAccidentalKindAsMusicXMLString (
          accidentalKind);

    if (accidentalString.size ()) {
      fCurrentNoteElement->push (
        createMxmlelement (
          k_accidental,
          accidentalString));
    }

    // append the stem if any
    appendStemToNote (theMsrNote);

    // append the staff if any
    appendStaffToNoteIfRelevant (theMsrNote);

    // append the beams if any
    appendBeamsToNote (theMsrNote);

    // append the articulations if any
    appendNoteNotationsToNote (theMsrNote);

    // append the lyrics if any
    appendNoteLyricsToNote (theMsrNote);

    // append theMsrNote element to the current measure element right now,
    // unless it contains a grace notes group
    S_msrGraceNotesGroup
      noteGraceNotesGroupBefore =
        theMsrNote->
          getNoteGraceNotesGroupBefore (),
      noteGraceNotesGroupAfter =
        theMsrNote->
          getNoteGraceNotesGroupAfter ();

    if (! (noteGraceNotesGroupBefore || noteGraceNotesGroupAfter)) {
      if (gGlobalMsr2mxsrOahGroup->getMusicXMLComments ()) {
        // create a note comment
        S_msrVoice
          noteVoice =
            theMsrNote->fetchNoteUpLinkToVoice ();

        std::stringstream s;
        s <<
          " ===== " <<
          "Note " <<
          theMsrNote->notePitchAndSoundingWholeNotesAsString () <<
          ", staffNumber: " <<
            noteVoice->getVoiceUpLinkToStaff ()->getStaffNumber () <<
          ", voiceNumber: " <<
            noteVoice->getVoiceNumber () <<
          ", measureElementMeasurePosition: " <<
            theMsrNote->getMeasureElementMeasurePosition () <<
          ", noteSoundingWholeNotes: " <<
            theMsrNote->getMeasureElementSoundingWholeNotes () <<
          ", line " << inputLineNumber <<
          " ===== ";
        Sxmlelement comment = createMxmlelement (kComment, s.str ());

        // append it to the current measure element
        appendOtherToMeasure (comment);
      }

      // append theMsrNote to the current measure element
      appendNoteToMeasure (
        fCurrentNoteElement,
        theMsrNote);
    }
    else {
      fCurrentNoteAwaitsGraceNotes = true;
      fPendingMsrNoteAwaitingGraceNotes = theMsrNote;
      fPendingNoteElement = fCurrentNoteElement;
    }
  }
}

//________________________________________________________________________
void msr2mxsrTranslator::visitStart (S_msrGraceNotesGroup& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber () ;

#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrGraceNotesGroup" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  if (gGlobalMsr2mxsrOahGroup->getMusicXMLComments ()) {
    // create a start comment
    std::stringstream s;
    s <<
      " ==================== " <<
      "Grace notes group " <<
      inputLineNumber <<
      " START" <<
        ", line " << inputLineNumber <<
      " ==================== ";
    Sxmlelement comment = createMxmlelement (kComment, s.str ());

    // append it to the current measure element
    fCurrentMeasureElement->push (comment);
  }
}

void msr2mxsrTranslator::visitEnd (S_msrGraceNotesGroup& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber () ;

#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrGraceNotesGroup" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  if (gGlobalMsr2mxsrOahGroup->getMusicXMLComments ()) {
    // create an end comment
    std::stringstream s;
    s <<
      " ==================== " <<
      "Grace notes group " <<
      inputLineNumber <<
      " END" <<
        ", line " << inputLineNumber <<
      " ==================== ";
    Sxmlelement comment = createMxmlelement (kComment, s.str ());

    // append it to the current measure element
    fCurrentMeasureElement->push (comment);
  }

  // append the note element to the current measure element only now
  // if it contains a grace notes group
  /*
  S_msrGraceNotesGroup
    noteGraceNotesGroupBefore =
      theMsrNote->getNoteGraceNotesGroupBefore (),
    noteGraceNotesGroupAfter =
      theMsrNote->getNoteGraceNotesGroupAfter ();
*/
  if (fCurrentNoteAwaitsGraceNotes) {
    appendNoteToMeasure (
      fPendingNoteElement,
      fPendingMsrNoteAwaitingGraceNotes);

    // forget about these after the pending grace notes
    fCurrentNoteAwaitsGraceNotes = false;
    fPendingMsrNoteAwaitingGraceNotes = nullptr;
    fPendingNoteElement = nullptr;
  }
}

/*
<!ELEMENT note
  (((grace, ((%full-note;, (tie, tie?)?) | (cue, %full-note;))) |
    (cue, %full-note;, duration) |
    (%full-note;, duration, (tie, tie?)?)),
   instrument?, %editorial-voice;, type?, dot*,
   accidental?, time-modification?, stem?, notehead?,
   notehead-text?, staff?, beam*, notations*, lyric*, play?)>
*/

//________________________________________________________________________
void msr2mxsrTranslator::visitStart (S_msrNote& elt)
{
#ifdef TRACING_IS_ENABLED
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrNote '" <<
      elt->asString () <<
      "'" <<
      ", line " << inputLineNumber <<
      std::endl;

    if (false) { // JMI TEMP
      gLogStream <<
        std::endl <<
        "--> note elt:" <<
        std::endl <<
        elt <<
        std::endl << std::endl;
    }
  }
#endif

  // append a backup or forward sub-element if needed
  appendABackupOrForwardToMeasureIfNeeded (elt);

  // populate the measure attributes if needed
  if (fAnAttributeElementIsNeeded) {
    createMxmlAttributesElementAndAppendItToMeasure ();

    fAnAttributeElementIsNeeded = false;
  }

  // append the note directions to the note element
  populateNoteDirections (elt);

  // append the note element to the measure element if relevant
  appendMsrNoteToMesureIfRelevant (elt);

  fANoteHasBeenMetInCurrentMeasure = true;
}

void msr2mxsrTranslator::visitEnd (S_msrNote& elt)
{
#ifdef TRACING_IS_ENABLED
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrNote " <<
      elt->asString () <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  // remember this note and its voice and staff if relevant
  // skips are just taken into account in fCurrentCumulatedSkipsDurations,
  // and a <backup /> or <forward /> is generated upon the next non-skip note

  Bool doRememberThisNote (false);

  switch (elt->getNoteKind ()) {
    case msrNoteKind::kNote_NO_:
      break;

    case msrNoteKind::kNoteRestInMeasure:
      doRememberThisNote = true;
      break;

    case msrNoteKind::kNoteSkipInMeasure:
      break;

    case msrNoteKind::kNoteUnpitchedInMeasure:
      doRememberThisNote = true;
      break;

    case msrNoteKind::kNoteRegularInMeasure:
    case msrNoteKind::kNoteRegularInChord:
    case msrNoteKind::kNoteRegularInTuplet:
    case msrNoteKind::kNoteRestInTuplet:
      doRememberThisNote = true;
      break;

    case msrNoteKind::kNoteInDoubleTremolo:
      doRememberThisNote = true; // JMI ???
      break;

    case msrNoteKind::kNoteRegularInGraceNotesGroup:
    case msrNoteKind::kNoteSkipInGraceNotesGroup:
      break;

    case msrNoteKind::kNoteInChordInGraceNotesGroup:
      break;

    case msrNoteKind::kNoteInTupletInGraceNotesGroup:
      break;

    case msrNoteKind::kNoteUnpitchedInTuplet:
      doRememberThisNote = true;
      break;
  } // switch

  if (doRememberThisNote) {
#ifdef TRACING_IS_ENABLED
    if (
      gGlobalMxsrOahGroup->getTraceBackup ()
        ||
      gGlobalMxsrOahGroup->getTraceForward ()
    ) {
      gLogStream <<
        "--> remembering note " <<
        elt->asString () <<
        " as previous note" <<
        ", line " << inputLineNumber <<
        std::endl;
    }
#endif

    fPreviousMSRNote = elt;
    fPreviousMSRNoteVoice = fPreviousMSRNote->fetchNoteUpLinkToVoice ();
    fPreviousMSRNoteStaff = fPreviousMSRNoteVoice->getVoiceUpLinkToStaff ();
  }

  // forget about the note element
  fCurrentNoteElement = nullptr;

  // forget about the note notations element
  fCurrentNoteNotationsElement              = nullptr;
  fCurrentNoteNotationsOrnamentsElement     = nullptr;
  fCurrentNoteNotationsArticulationsElement = nullptr;
  fCurrentNoteNotationsTechnicalsElement    = nullptr;
}

//________________________________________________________________________
void msr2mxsrTranslator::visitStart (S_msrBarLine& elt)
{
#ifdef TRACING_IS_ENABLED
  int inputLineNumber =
    elt->getInputLineNumber ();
#endif

#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrBarLine" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  std::string barLStyleString;

  switch (elt->getBarLineStyleKind ()) {
    case msrBarLineStyleKind::kBarLineStyleNone:
      break;
    case msrBarLineStyleKind::kBarLineStyleRegular:
      barLStyleString = "regular";
      break;
    case msrBarLineStyleKind::kBarLineStyleDotted:
      barLStyleString = "dotted";
      break;
    case msrBarLineStyleKind::kBarLineStyleDashed:
      barLStyleString = "dashed";
      break;
    case msrBarLineStyleKind::kBarLineStyleHeavy:
      barLStyleString = "heavy";
      break;
    case msrBarLineStyleKind::kBarLineStyleLightLight:
      barLStyleString = "light-light";
      break;
    case msrBarLineStyleKind::kBarLineStyleLightHeavy:
      barLStyleString = "light-heavy";
      break;
    case msrBarLineStyleKind::kBarLineStyleHeavyLight:
      barLStyleString = "heavy-light";
      break;
    case msrBarLineStyleKind::kBarLineStyleHeavyHeavy:
      barLStyleString = "heavy-heavy";
      break;
    case msrBarLineStyleKind::kBarLineStyleTick:
      barLStyleString = "tick";
      break;
    case msrBarLineStyleKind::kBarLineStyleShort:
      barLStyleString = "short";
      break;
  } // switch

  if (barLStyleString.size ()) {
    // create the bar style element
    Sxmlelement barStyleElement = createMxmlelement (k_bar_style, barLStyleString);

    // create the barLine element
    Sxmlelement barLineElement = createMxmlelement (k_barline, "");

    // set its "location" attribute if any
    msrBarLineLocationKind
      barLineLocationKind =
        elt->getLocationKind ();

    std::string barLineLocationString;

    switch (barLineLocationKind) {
      case msrBarLineLocationKind::kBarLineLocationNone:
        break;
      case msrBarLineLocationKind::kBarLineLocationLeft:
        barLineLocationString = "left";
        break;
      case msrBarLineLocationKind::kBarLineLocationMiddle:
        barLineLocationString = "middle";
        break;
      case msrBarLineLocationKind::kBarLineLocationRight:
        barLineLocationString = "right";
        break;
    } // switch

    if (barLineLocationString.size ()) {
      barLineElement->add (createMxmlAttribute ("location", barLineLocationString));
    }

    // append the barLine element to the barLine element
    barLineElement->push (barStyleElement);

    // append the barLine element to the measure element
    appendOtherToMeasure (barLineElement);
  }
}

void msr2mxsrTranslator::visitEnd (S_msrBarLine& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrBarLine" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif
}


/*
//________________________________________________________________________
void msr2mxsrTranslator::visitStart (S_msrStaffLinesNumber& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrStaffLinesNumber" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  // create a staff lines number clone
  fCurrentStaffLinesNumberClone =
    elt->
      createMxmlStaffLinesNumberNewbornClone ();
}

//________________________________________________________________________
void msr2mxsrTranslator::visitStart (S_msrStaffTuning& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrStaffTuning" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

  // create a staff tuning clone
  fCurrentStaffTuningClone =
    elt->
      createMxmlStaffTuningNewbornClone ();
}

//________________________________________________________________________
void msr2mxsrTranslator::visitStart (S_msrStaffDetails& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrStaffDetails" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

  fCurrentStaffTuningClone = nullptr;
}

void msr2mxsrTranslator::visitEnd (S_msrStaffDetails& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrStaffDetails" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

  // append the staff details to the current voice clone
  fCurrentVoiceClone->
    appendStaffDetailsToVoice (
      elt);
}
*/

/*
//________________________________________________________________________
void msr2mxsrTranslator::visitStart (S_msrVoiceStaffChange& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrVoiceStaffChange '" <<
      elt->asString () << "'" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

  // create a voice staff change clone
  S_msrVoiceStaffChange
    voiceStaffChangeClone =
      elt->
        createMxmlStaffChangeNewbornClone ();

  // append it to the current voice clone
  fCurrentVoiceClone->
    appendVoiceStaffChangeToVoice (
      voiceStaffChangeClone);
}

//________________________________________________________________________
void msr2mxsrTranslator::visitStart (S_msrHarmony& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrHarmony '" <<
      elt->asString () <<
      ", fOnGoingNonGraceNote: " << fOnGoingNonGraceNote <<
      ", fOnGoingHarmoniesVoice: " << fOnGoingHarmoniesVoice <<
      ", fOnGoingHarmony: " << fOnGoingHarmony <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

  // create a harmony new born clone
  fCurrentHarmonyClone =
    elt->
      createMxmlHarmonyNewbornClone (
        fCurrentVoiceClone);

  if (fOnGoingNonGraceNote) {
    // register this note as the harmony note upLink
    harmony->
      setHarmonyUpLinkToNote (fOnGoingNonGraceNote);

    // register the harmony in the current non-grace note clone
    fCurrentNonGraceNoteClone->
      appendHarmonyToNoteHarmoniesList (
        fCurrentHarmonyClone);

    // don't append the harmony to the part harmony,
    // this has been done in pass2b // JMI ???
  }

  else if (fOnGoingHarmoniesVoice) {
  / * JMI
    // get the harmony whole notes offset
    Rational
      harmonyWholeNotesOffset =
        elt->getHarmonyWholeNotesOffset ();

    // is harmonyWholeNotesOffset not equal to 0?
    if (harmonyWholeNotesOffset.getNumerator () != 0) {
      // create skip with duration harmonyWholeNotesOffset
      S_msrNote
        skip =
          msrNote::createSkipNote (
            elt->                getInputLineNumber (),
            "666", // JMI elt->                getHarmoniesMeasureNumber (),
            elt->                getHarmonyDisplayWholeNotes (), // would be 0/1 otherwise JMI
            elt->                getHarmonyDisplayWholeNotes (),
            0, // JMI elt->                getHarmonyDotsNumber (),
            fCurrentVoiceClone-> getRegularVoiceStaffSequentialNumber (), // JMI
            fCurrentVoiceClone-> getVoiceNumber ());

      // append it to the current voice clone
      // to 'push' the harmony aside
      fCurrentVoiceClone->
        appendNoteToVoice (skip);
    }
* /

    // append the harmony to the current voice clone
    fCurrentVoiceClone->
      appendHarmonyToVoiceClone (
        fCurrentHarmonyClone);
  }

  else {
    std::stringstream s;

    s <<
      "harmony '" << elt->asShortString () <<
      "' is out of context, cannot be handled";

    msr2mxsrInternalError (
      gGlobalServiceRunData->getInputSourceName (),
      elt->getInputLineNumber (),
      __FILE__, __LINE__,
      s.str ());
  }

  fOnGoingHarmony = true;
}

void msr2mxsrTranslator::visitStart (S_msrHarmonyDegree& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting S_msrHarmonyDegree '" <<
      elt->asString () <<
      ", fOnGoingNonGraceNote: " << fOnGoingNonGraceNote <<
      ", fOnGoingHarmoniesVoice: " << fOnGoingHarmoniesVoice <<
      ", fOnGoingHarmony: " << fOnGoingHarmony <<
      "', line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

  // append the harmony degree to the current harmony clone
  fCurrentHarmonyClone->
    appendHarmonyDegreeToHarmony (
      elt);
}

void msr2mxsrTranslator::visitEnd (S_msrHarmony& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrHarmony '" <<
      elt->asString () <<
      "'" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

  fCurrentHarmonyClone = nullptr;
  fOnGoingHarmony = false;
}

//________________________________________________________________________
void msr2mxsrTranslator::visitStart (S_msrFrame& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrFrame '" <<
      elt->asString () <<
      "'" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

  if (fOnGoingHarmony) {
    // register the frame in the current non-grace note clone
    fCurrentHarmonyClone->
      setHarmonyFrame (elt);
  }

  else {
    std::stringstream s;

    s <<
      "frame '" << elt->asShortString () <<
      "' is out of context, cannot be appendd";

    msr2mxsrInternalError (
      gGlobalServiceRunData->getInputSourceName (),
      elt->getInputLineNumber (),
      __FILE__, __LINE__,
      s.str ());
  }
}

//________________________________________________________________________
void msr2mxsrTranslator::visitStart (S_msrFiguredBass& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrFiguredBass '" <<
      elt->asString () <<
      "'" <<
      ", fOnGoingFiguredBassVoice = " << fOnGoingFiguredBassVoice <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

  // create a figured bass new born clone
  fCurrentFiguredBassClone =
    elt->
      createMxmlFiguredBassNewbornClone (
        fCurrentVoiceClone);

  if (fOnGoingNonGraceNote) {
    // append the figured bass to the current non-grace note clone
    fCurrentNonGraceNoteClone->
      appendFiguredBassToNoteFiguredBassesList (
        fCurrentFiguredBassClone);

    // don't append the figured bass to the part figured bass,  JMI ???
    // this will be done below
  }

  else if (fOnGoingFiguredBassVoice) { // JMI
    / *
    // register the figured bass in the part clone figured bass
    fCurrentPartClone->
      appendFiguredBassToPartClone (
        fCurrentVoiceClone,
        fCurrentFiguredBassClone);
        * /
    // append the figured bass to the current voice clone
    fCurrentVoiceClone->
      appendFiguredBassToVoiceClone (
        fCurrentFiguredBassClone);
  }

  else {
    std::stringstream s;

    s <<
      "figured bass '" << elt->asShortString () <<
      "' is out of context, cannot be append";

    msr2mxsrInternalError (
      gGlobalServiceRunData->getInputSourceName (),
      elt->getInputLineNumber (),
      __FILE__, __LINE__,
      s.str ());
  }
}

void msr2mxsrTranslator::visitStart (S_msrBassFigure& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrBassFigure '" <<
      elt->asString () <<
      "'" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

  // append the bass figure to the current figured bass
  fCurrentFiguredBassClone->
    appendFigureToFiguredBass (
      elt);
}

void msr2mxsrTranslator::visitEnd (S_msrFiguredBass& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrFiguredBass '" <<
      elt->asString () <<
      "'" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

  fCurrentFiguredBassClone = nullptr;
}
*/

/*
//________________________________________________________________________
void msr2mxsrTranslator::visitStart (S_msrStanza& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrStanza \"" <<
      elt->getStanzaName () <<
      "\"" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

  ++gIndenter;

//  if (elt->getStanzaTextPresent ()) { // JMI
    fCurrentStanzaClone =
      elt->createStanzaNewbornClone (
        fCurrentVoiceClone);

    // append the stanza clone to the LPSR score elements list
    fResultingMusicxmlelement->
      appendStanzaToLpsrScoreElementsList (
        fCurrentStanzaClone);

    // append a use of the stanza to the current staff block
    fCurrentStaffBlock ->
      appendLyricsUseToStaffBlock (
        fCurrentStanzaClone);
//  }
//  else
  //  fCurrentStanzaClone = 0; // JMI

  fOnGoingStanza = true;
}

void msr2mxsrTranslator::visitEnd (S_msrStanza& elt)
{
  --gIndenter;

#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrStanza \"" <<
      elt->getStanzaName () <<
      "\"" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

  // forget about this stanza
  fCurrentStanzaClone = nullptr;

  fOnGoingStanza = false;
}

//________________________________________________________________________
void msr2mxsrTranslator::visitStart (S_msrSyllable& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrSyllable" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  // create the syllable clone
  fCurrentSyllableClone =
    elt->createSyllableNewbornClone (
      fCurrentPartClone);

  // add it to the current stanza clone or current note clone
  if (fOnGoingStanza) { // fCurrentStanzaClone JM
    // visiting a syllable as a stanza member
    fCurrentStanzaClone->
      appendSyllableToStanza (
        fCurrentSyllableClone);
  }

  else if (fOnGoingNonGraceNote) { // JMI
    // visiting a syllable as attached to the current non-grace note
    fCurrentSyllableClone->
      appendSyllableToNoteAndSetItsUpLinkToNote (
        fCurrentNonGraceNoteClone);

    if (gGlobalLpsrOahGroup->getAddMsrWordsFromTheMusicXMLLyrics ()) {
      // get the syllable texts std::list
      const std::list<std::string>&
        syllableTextsList =
          elt->getSyllableTextsList ();

      if (syllableTextsList.size ()) {
        // build a single words value from the texts std::list
        // JMI create an msrWords instance for each???
        std::string wordsValue =
          elt->syllableTextsListAsString();

        // create the words
#ifdef TRACING_IS_ENABLED
        if (gGlobalTracingOahGroup->getTraceLyrics ()) {
          gLogStream <<
            "Changing lyrics '" <<
            wordsValue <<
            "' into words for note '" <<
            fCurrentNonGraceNoteClone->asShortString () <<
            "'" <<
      // JMI      fCurrentSyllableClone->asString () <<
            std::endl;
        }
#endif

        S_msrWords
          words =
            msrWords::create (
              inputLineNumber,
              msrPlacementKind::kPlacement_NO_,                // default value
              wordsValue,
              msrJustifyKind::kJustifyNone,                  // default value
              msrHorizontalAlignmentKind::kHorizontalAlignmentNone,      // default value
              msrVerticalAlignmentKind::kVerticalAlignmentNone,        // default value
              msrFontStyleKind::kFontStyleNone,                // default value
              msrFontSize::create (
                msrFontSizeKind::kFontSizeNone), // default value
              msrFontWeightKind::kFontWeightNone,               // default value
              msrXMLLangKind::kXMLLangIt,                    // default value
              elt->getSyllableUpLinkToNote ()->getNoteStaffNumber ());

        // append it to the current non-grace note
#ifdef TRACING_IS_ENABLED
        if (gGlobalTracingOahGroup->getTraceWords ()) {
          gLogStream <<
            "Appending words '" <<
            words->asShortString () <<
            "' to note '" <<
            fCurrentNonGraceNoteClone->asShortString () <<
            "'" <<
            std::endl;
        }
#endif
        fCurrentNonGraceNoteClone->
          appendWordsToNote (
            words);
      }
    }
  }
  else {
    std::stringstream s;

    s <<
      "syllable '" << elt->asShortString () <<
      "' is out of context, cannot be append";

    msr2mxsrInternalError (
      gGlobalServiceRunData->getInputSourceName (),
      elt->getInputLineNumber (),
      __FILE__, __LINE__,
      s.str ());
  }

  // a syllable ends the sysllable extend range if any
  if (fOnGoingSyllableExtend) {
    / * JMI ???
    // create melisma end command
    S_lpsrMelismaCommand
      melismaCommand =
        lpsrMelismaCommand::create (
          inputLineNumber,
          lpsrMelismaCommand::kMelismaEnd);

    // append it to current voice clone
    fCurrentVoiceClone->
      appendOtherElementToVoice (melismaCommand);
* /

    fOnGoingSyllableExtend = false;
  }
}

void msr2mxsrTranslator::visitEnd (S_msrSyllable& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrSyllable" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif
}

//________________________________________________________________________
void msr2mxsrTranslator::visitStart (S_msrTransposition& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrTransposition" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

  // append transpose to voice clone
  fCurrentVoiceClone->
    appendTranspositionToVoice (elt);
}

void msr2mxsrTranslator::visitEnd (S_msrTransposition& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrTransposition" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif
}

//________________________________________________________________________
void msr2mxsrTranslator::visitStart (S_msrRehearsalMark& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrRehearsalMark" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

  fCurrentVoiceClone->
    appendRehearsalMarkToVoice (elt);
}

void msr2mxsrTranslator::visitEnd (S_msrRehearsalMark& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrRehearsalMark" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif
}

//________________________________________________________________________
void msr2mxsrTranslator::visitStart (S_msrFermata& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrFermata" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

  // a fermata is an articulation

  if (fOnGoingNonGraceNote) {
    fCurrentNonGraceNoteClone->
      appendArticulationToNote (elt);
  }
  else {
    std::stringstream s;

    s <<
      "fermata '" << elt->asShortString () <<
      "' is out of context, cannot be append";

    msr2mxsrInternalError (
      gGlobalServiceRunData->getInputSourceName (),
      elt->getInputLineNumber (),
      __FILE__, __LINE__,
      s.str ());
  }
}

//________________________________________________________________________
void msr2mxsrTranslator::visitStart (S_msrArpeggiato& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrArpeggiato" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

  // an arpeggiato is an articulation

  if (fOnGoingNonGraceNote) {
    fCurrentNonGraceNoteClone->
      appendArticulationToNote (elt); // addArpeggiatoToNote ??? JMI
  }
  else {
    std::stringstream s;

    s <<
      "arpeggiato '" << elt->asShortString () <<
      "' is out of context, cannot be append";

    msr2mxsrInternalError (
      gGlobalServiceRunData->getInputSourceName (),
      elt->getInputLineNumber (),
      __FILE__, __LINE__,
      s.str ());
  }
}

//________________________________________________________________________
void msr2mxsrTranslator::visitStart (S_msrNonArpeggiato& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrNonArpeggiato" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

  // an nonArpeggiato is an articulation

  if (fOnGoingNonGraceNote) {
    fCurrentNonGraceNoteClone->
      appendArticulationToNote (elt); // addArpeggiatoToNote ??? JMI
  }
  else if (fOnGoingChord) {
    fCurrentChordClone->
      appendArticulationToChord (elt);
  }
  else {
    std::stringstream s;

    s <<
      "nonArpeggiato '" << elt->asShortString () <<
      "' is out of context, cannot be append";

    msr2mxsrInternalError (
      gGlobalServiceRunData->getInputSourceName (),
      elt->getInputLineNumber (),
      __FILE__, __LINE__,
      s.str ());
  }
}

//________________________________________________________________________
void msr2mxsrTranslator::visitStart (S_msrTechnical& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrTechnical" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

  if (fOnGoingNonGraceNote) {
    fCurrentNonGraceNoteClone->
      appendTechnicalToNote (elt);
  }
  else if (fOnGoingChord) {
    fCurrentChordClone->
      appendTechnicalToChord (elt);
  }
  else {
    std::stringstream s;

    s <<
      "technical '" << elt->asShortString () <<
      "' is out of context, cannot be append";

    msr2mxsrInternalError (
      gGlobalServiceRunData->getInputSourceName (),
      elt->getInputLineNumber (),
      __FILE__, __LINE__,
      s.str ());
  }

  // doest the score need the 'tongue' function?
  switch (elt->getTechnicalKind ()) {
    case msrTechnicalKind::kTechnicalArrow:
      break;
    case msrTechnicalKind::kTechnicalDoubleTongue:
      fResultingMusicxmlelement->
        // this score needs the 'tongue' Scheme function
        setTongueSchemeFunctionIsNeeded ();
      break;
    case msrTechnicalKind::kTechnicalDownBow:
      break;
    case msrTechnicalKind::kTechnicalFingernails:
      break;
    case msrTechnicalKind::kTechnicalHarmonic:
      break;
    case msrTechnicalKind::kTechnicalHeel:
      break;
    case msrTechnicalKind::kTechnicalHole:
      break;
    case msrTechnicalKind::kTechnicalOpenString:
      break;
    case msrTechnicalKind::kTechnicalSnapPizzicato:
      break;
    case msrTechnicalKind::kTechnicalStopped:
      break;
    case msrTechnicalKind::kTechnicalTap:
      break;
    case msrTechnicalKind::kTechnicalThumbPosition:
      break;
    case msrTechnicalKind::kTechnicalToe:
      break;
    case msrTechnicalKind::kTechnicalTripleTongue:
      fResultingMusicxmlelement->
        // this score needs the 'tongue' Scheme function
        setTongueSchemeFunctionIsNeeded ();
      break;
    case msrTechnicalKind::kTechnicalUpBow:
      break;
  } // switch
}

void msr2mxsrTranslator::visitEnd (S_msrTechnical& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrTechnical" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif
}

//________________________________________________________________________
void msr2mxsrTranslator::visitStart (S_msrTechnicalWithInteger& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrTechnicalWithInteger" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

  if (fOnGoingNonGraceNote) {
    fCurrentNonGraceNoteClone->
      appendTechnicalWithIntegerToNote (elt);
  }
  else if (fOnGoingChord) {
    fCurrentChordClone->
      appendTechnicalWithIntegerToChord (elt);
  }
  else {
    std::stringstream s;

    s <<
      "technicalWithInteger '" << elt->asShortString () <<
      "' is out of context, cannot be append";

    msr2mxsrInternalError (
      gGlobalServiceRunData->getInputSourceName (),
      elt->getInputLineNumber (),
      __FILE__, __LINE__,
      s.str ());
  }
}

void msr2mxsrTranslator::visitEnd (S_msrTechnicalWithInteger& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrTechnicalWithInteger" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif
}

//________________________________________________________________________
void msr2mxsrTranslator::visitStart (S_msrTechnicalWithFloat& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrTechnicalWithFloat" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

  if (fOnGoingNonGraceNote) {
    fCurrentNonGraceNoteClone->
      appendTechnicalWithFloatToNote (elt);
  }
  else if (fOnGoingChord) {
    fCurrentChordClone->
      appendTechnicalWithFloatToChord (elt);
  }
  else {
    std::stringstream s;

    s <<
      "technicalWithFloat '" << elt->asShortString () <<
      "' is out of context, cannot be append";

    msr2mxsrInternalError (
      gGlobalServiceRunData->getInputSourceName (),
      elt->getInputLineNumber (),
      __FILE__, __LINE__,
      s.str ());
  }
}

void msr2mxsrTranslator::visitEnd (S_msrTechnicalWithFloat& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrTechnicalWithFloat" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif
}

//________________________________________________________________________
void msr2mxsrTranslator::visitStart (S_msrTechnicalWithString& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrTechnicalWithString" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

  if (fOnGoingNonGraceNote) {
    fCurrentNonGraceNoteClone->
      appendTechnicalWithStringToNote (elt);
  }
  else if (fOnGoingChord) {
    fCurrentChordClone->
      appendTechnicalWithStringToChord (elt);
  }
  else {
    std::stringstream s;

    s <<
      "technicalWithString '" << elt->asShortString () <<
      "' is out of context, cannot be append";

    msr2mxsrInternalError (
      gGlobalServiceRunData->getInputSourceName (),
      elt->getInputLineNumber (),
      __FILE__, __LINE__,
      s.str ());
  }

  switch (elt->getTechnicalWithStringKind ()) {
    case msrTechnicalWithStringKind::kHammerOn:
    case msrTechnicalWithStringKind::kPullOff:
      // this score needs the 'after' Scheme function
      fResultingMusicxmlelement->
        setAfterSchemeFunctionIsNeeded ();
      break;
    default:
      ;
  } // switch
}

void msr2mxsrTranslator::visitEnd (S_msrTechnicalWithString& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrTechnicalWithString" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif
}

//________________________________________________________________________
void msr2mxsrTranslator::visitStart (S_msrOrnament& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrOrnament" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

  if (fOnGoingNonGraceNote) {
    fCurrentNonGraceNoteClone->
      appendOrnamentToNote (elt);
  }
  else if (fOnGoingChord) {
    fCurrentChordClone->
      appendOrnamentToChord (elt);
  }
  else {
    std::stringstream s;

    s <<
      "ornament '" << elt->asShortString () <<
      "' is out of context, cannot be append";

    msr2mxsrInternalError (
      gGlobalServiceRunData->getInputSourceName (),
      elt->getInputLineNumber (),
      __FILE__, __LINE__,
      s.str ());
  }
}

void msr2mxsrTranslator::visitEnd (S_msrOrnament& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrOrnament" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif
}

//________________________________________________________________________
void msr2mxsrTranslator::visitStart (S_msrGlissando& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrGlissando" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

  if (fOnGoingNonGraceNote) {
    fCurrentNonGraceNoteClone->
      appendGlissandoToNote (elt);
  }
  else if (fOnGoingChord) {
    fCurrentChordClone->
      appendGlissandoToChord (elt);
  }
  else {
    std::stringstream s;

    s <<
      "glissando '" << elt->asShortString () <<
      "' is out of context, cannot be append";

    msr2mxsrInternalError (
      gGlobalServiceRunData->getInputSourceName (),
      elt->getInputLineNumber (),
      __FILE__, __LINE__,
      s.str ());
  }

  if (elt->getGlissandoTextValue ().size ()) {
    fResultingMusicxmlelement->
      // this score needs the 'glissandoWithText' Scheme functions
      addGlissandoWithTextSchemeFunctionsToScore ();
  }
}

void msr2mxsrTranslator::visitEnd (S_msrGlissando& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrGlissando" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif
}

//________________________________________________________________________
void msr2mxsrTranslator::visitStart (S_msrSlide& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrSlide" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

  if (fOnGoingNonGraceNote) {
    fCurrentNonGraceNoteClone->
      appendSlideToNote (elt);
  }
  else if (fOnGoingChord) {
    fCurrentChordClone->
      appendSlideToChord (elt);
  }
  else {
    std::stringstream s;

    s <<
      "slide '" << elt->asShortString () <<
      "' is out of context, cannot be append";

    msr2mxsrInternalError (
      gGlobalServiceRunData->getInputSourceName (),
      elt->getInputLineNumber (),
      __FILE__, __LINE__,
      s.str ());
  }
}

void msr2mxsrTranslator::visitEnd (S_msrSlide& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrSlide" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif
}

//________________________________________________________________________
void msr2mxsrTranslator::visitStart (S_msrSingleTremolo& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrSingleTremolo" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

  if (fOnGoingNonGraceNote) {
    fCurrentNonGraceNoteClone->
      setNoteSingleTremolo (elt);
  }
  else if (fOnGoingChord) {
    fCurrentChordClone->
      setChordSingleTremolo (elt);
  }
  else {
    std::stringstream s;

    s <<
      "singleTremolo '" << elt->asShortString () <<
      "' is out of context, cannot be append";

    msr2mxsrInternalError (
      gGlobalServiceRunData->getInputSourceName (),
      elt->getInputLineNumber (),
      __FILE__, __LINE__,
      s.str ());
  }
}

void msr2mxsrTranslator::visitEnd (S_msrSingleTremolo& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrSingleTremolo" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif
}

//________________________________________________________________________
void msr2mxsrTranslator::visitStart (S_msrDoubleTremolo& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrDoubleTremolo" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

  // create a double tremolo clone from the two elements
  fCurrentDoubleTremoloClone = elt; // JMI FIX THAT
/ * JMI
    elt->createDoubleTremoloNewbornClone (
      elt->getDoubleTremoloFirstElement ()->
        createMxmlNewBornClone (),
      elt->getDoubleTremoloSecondElement ()
        createMxmlNewBornClone ());
        * /

  fOnGoingDoubleTremolo = true;
}

void msr2mxsrTranslator::visitEnd (S_msrDoubleTremolo& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrSingleTremolo" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

  // append the current double tremolo clone to the current voice clone
  fCurrentVoiceClone->
    appendDoubleTremoloToVoice (
      fCurrentDoubleTremoloClone);

  // forget about it
  fCurrentDoubleTremoloClone = nullptr;

  fOnGoingDoubleTremolo = false;
}

//________________________________________________________________________
void msr2mxsrTranslator::visitStart (S_msrDynamic& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrDynamic" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

  if (fOnGoingNonGraceNote) {
    fCurrentNonGraceNoteClone->
      appendDynamicToNote (elt);

    // is this a non LilyPond native dynamics?
    Bool knownToLilyPondNatively (true);

    switch (elt->getDynamicKind ()) {
      case msrDynamicKind::kDynamicFFFFF:
      case msrDynamicKind::kDynamicFFFFFF:
      case msrDynamicKind::kDynamicPPPPP:
      case msrDynamicKind::kDynamicPPPPPP:
      case msrDynamicKind::kDynamicRF:
      case msrDynamicKind::kDynamicSFPP:
      case msrDynamicKind::kDynamicSFFZ:
      case msrDynamicKind::kDynamicSFZP:
      case msrDynamicKind::kDynamicN:
      case msrDynamicKind::kDynamic_NO_:
        knownToLilyPondNatively = false;

      default:
        ;
    } // switch

    if (! knownToLilyPondNatively) {
      // this score needs the 'dynamics' Scheme function
      fResultingMusicxmlelement->
        setDynamicsSchemeFunctionIsNeeded ();
    }
  }

  else if (fOnGoingChord) {
    fCurrentChordClone->
      appendDynamicToChord (elt);
  }

  else {
    std::stringstream s;

    s <<
      "dynamics '" << elt->asShortString () <<
      "' is out of context, cannot be append";

    msr2mxsrInternalError (
      gGlobalServiceRunData->getInputSourceName (),
      elt->getInputLineNumber (),
      __FILE__, __LINE__,
      s.str ());
  }
}

void msr2mxsrTranslator::visitEnd (S_msrDynamic& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrDynamic" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif
}

//________________________________________________________________________
void msr2mxsrTranslator::visitStart (S_msrOtherDynamic& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrOtherDynamic" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

  if (fOnGoingNonGraceNote) {
    fCurrentNonGraceNoteClone->
      appendOtherDynamicToNote (elt);
  }
  else if (fOnGoingChord) {
    fCurrentChordClone->
      appendOtherDynamicToChord (elt);
  }
  else {
    std::stringstream s;

    s <<
      "otherDynamic '" << elt->asShortString () <<
      "' is out of context, cannot be append";

    msr2mxsrInternalError (
      gGlobalServiceRunData->getInputSourceName (),
      elt->getInputLineNumber (),
      __FILE__, __LINE__,
      s.str ());
  }

  fResultingMusicxmlelement->
    // this score needs the 'otherDynamic' Scheme function
    setOtherDynamicSchemeFunctionIsNeeded ();
}

void msr2mxsrTranslator::visitEnd (S_msrOtherDynamic& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrOtherDynamic" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif
}

//________________________________________________________________________
void msr2mxsrTranslator::visitStart (S_msrWords& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrWords" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  if (fOnGoingNonGraceNote || fOnGoingChord) {
    Bool wordsHasBeenAppended (false);

    if (gGlobalLpsrOahGroup->getConvertLpsrWordsToTempo ()) {
      // create a tempo containing elt
      S_msrTempo
        tempo =
          msrTempo::create (
            inputLineNumber,
            elt);

#ifdef TRACING_IS_ENABLED
      if (gGlobalTracingOahGroup->getTraceWords ()) {
        gLogStream <<
          "Converting words '" <<
          elt->asShortString () <<
          "' to tempo '" <<
          tempo->asShortString () <<
          "'" <<
          std::endl;
      }
#endif

      // append the tempo to the current voice clone
      fCurrentVoiceClone->
        appendTempoToVoice (tempo);

      wordsHasBeenAppended = true;
    }

    else if (gGlobalLpsrOahGroup->getConvertWordsToRehearsalMarks ()) {
      // create a rehearsal mark containing elt's words contents
      S_msrRehearsalMark
        rehearsalMark =
          msrRehearsalMark::create (
            inputLineNumber,
            msrRehearsalMarkKind::kRehearsalMarkNone,
            elt->getWordsContents (),
            elt->getWordsPlacementKind ()); // above ??? JMI

#ifdef TRACING_IS_ENABLED
      if (gGlobalTracingOahGroup->getTraceWords ()) {
        gLogStream <<
          "Converting words '" <<
          elt->asShortString () <<
          "' to rehearsal mark '" <<
          rehearsalMark->asShortString () <<
          "'" <<
          std::endl;
      }
#endif

      // append the rehearsalMark to the current voice clone
      fCurrentVoiceClone->
        appendRehearsalMarkToVoice (rehearsalMark);

      wordsHasBeenAppended = true;
    }

    else {
    / * JMI
      std::string wordsContents = elt->getWordsContents ();

      // is this words contents in the std::string to dal segno kind std::map?
      const std::map<std::string, msrDalSegno::msrDalSegnoKind>&
        converStringToDalSegnoMap =
          gGlobalMxsr2msrOahGroup->
            getStringToDalSegnoKindMap ();

      std::map<std::string, msrDalSegno::msrDalSegnoKind>::const_iterator
        it =
          converStringToDalSegnoMap.find (wordsContents);

      if (it != converStringToDalSegnoMap.end ()) {
        // yes
        msrDalSegno::msrDalSegnoKind
          dalSegnoKind =
            (*it).second;

        // create a dal segno element containing elt's words contents
        S_msrDalSegno
          dalSegno =
            msrDalSegno::create (
              inputLineNumber,
              dalSegnoKind,
              wordsContents,
              elt->getWordsStaffNumber ());

#ifdef TRACING_IS_ENABLED
        if (gGlobalTracingOahGroup->getTraceWords ()) {
          gLogStream <<
            "Converting words '" <<
            elt->asShortString () <<
            "' to dal segno element '" <<
            dalSegno->asShortString () <<
            "'" <<
            std::endl;
        }
#endif

        if (fOnGoingNonGraceNote) {
          fCurrentNonGraceNoteClone->
            appendDalSegnoToNote (dalSegno);
        }
        else if (fOnGoingChord) {
          fCurrentChordClone->
            appendDalSegnoToChord (dalSegno);
        }

      wordsHasBeenAppended = true;
      }
      * /
    }

    if (! wordsHasBeenAppended) {
      if (fOnGoingNonGraceNote) {
        fCurrentNonGraceNoteClone->
          appendWordsToNote (elt);
      }
      else if (fOnGoingChord) {
        fCurrentChordClone->
          appendWordsToChord (elt);
      }
    }
  }

  else {
    std::stringstream s;

    s <<
      "words '" << elt->asShortString () <<
      "' is out of context, cannot be handled";

    msr2mxsrInternalError (
      gGlobalServiceRunData->getInputSourceName (),
      elt->getInputLineNumber (),
      __FILE__, __LINE__,
      s.str ());
  }
}

void msr2mxsrTranslator::visitEnd (S_msrWords& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrWords" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif
}

//________________________________________________________________________
void msr2mxsrTranslator::visitStart (S_msrSlur& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrSlur" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

  / *
    Only the  first note of the chord should get the slur notation.
    Some applications print out the slur for all notes,
    i.e. a stop and a start in sequence:
    these should be ignored
  * /

  if (fOnGoingNonGraceNote) {
    // don't add slurs to chord member notes except the first one
    switch (fCurrentNonGraceNoteClone->getNoteKind ()) {
      case msrNoteKind::kNoteRegularInChord:
        if (fCurrentNonGraceNoteClone->getNoteIsAChordsFirstMemberNote ()) {
          fCurrentNonGraceNoteClone->
            appendSlurToNote (elt);
        }
        break;

      default:
        fCurrentNonGraceNoteClone->
          appendSlurToNote (elt);
    } // switch
  }

  else if (fOnGoingChord) {
    fCurrentChordClone->
      appendSlurToChord (elt);
  }

  else {
    displayCurrentOnGoingValues ();

    std::stringstream s;

    s <<
      "slur '" << elt->asShortString () <<
      "' is out of context, cannot be handled";

    msr2mxsrInternalError (
      gGlobalServiceRunData->getInputSourceName (),
      elt->getInputLineNumber (),
      __FILE__, __LINE__,
      s.str ());
  }
}

void msr2mxsrTranslator::visitEnd (S_msrSlur& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrSlur" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif
}

//________________________________________________________________________
void msr2mxsrTranslator::visitStart (S_msrLigature& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrLigature" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

  if (fOnGoingNonGraceNote) {
    fCurrentNonGraceNoteClone->
      appendLigatureToNote (elt);
  }
  else if (fOnGoingChord) {
    fCurrentChordClone->
      appendLigatureToChord (elt);
  }
  else {
    std::stringstream s;

    s <<
      "ligature '" << elt->asShortString () <<
      "' is out of context, cannot be handled";

    msr2mxsrInternalError (
      gGlobalServiceRunData->getInputSourceName (),
      elt->getInputLineNumber (),
      __FILE__, __LINE__,
      s.str ());
  }
}

void msr2mxsrTranslator::visitEnd (S_msrLigature& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrLigature" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif
}

//________________________________________________________________________
void msr2mxsrTranslator::visitStart (S_msrSlash& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrSlash" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

  if (fOnGoingNonGraceNote) {
    fCurrentNonGraceNoteClone->
      appendSlashToNote (elt);
  }
  else if (fOnGoingChord) {
    fCurrentChordClone->
      appendSlashToChord (elt);
  }
  else {
    std::stringstream s;

    s <<
      "slash '" << elt->asShortString () <<
      "' is out of context, cannot be handled";

    msr2mxsrInternalError (
      gGlobalServiceRunData->getInputSourceName (),
      elt->getInputLineNumber (),
      __FILE__, __LINE__,
      s.str ());
  }
}

//________________________________________________________________________
void msr2mxsrTranslator::visitStart (S_msrCrescDecresc& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrCrescDecresc" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

  if (fOnGoingNonGraceNote) {
    fCurrentNonGraceNoteClone->
      appendCrescDecrescToNote (elt);
  }
  else if (fOnGoingChord) {
    fCurrentChordClone->
      appendWedgeToChord (elt);
  }
  else {
    std::stringstream s;

    s <<
      "wedge '" << elt->asShortString () <<
      "' is out of context, cannot be handled";

    msr2mxsrInternalError (
      gGlobalServiceRunData->getInputSourceName (),
      elt->getInputLineNumber (),
      __FILE__, __LINE__,
      s.str ());
  }
}

void msr2mxsrTranslator::visitEnd (S_msrCrescDecresc& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrCrescDecresc" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif
}

//________________________________________________________________________
void msr2mxsrTranslator::visitStart (S_msrWedge& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrWedge" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

  if (fOnGoingNonGraceNote) {
    fCurrentNonGraceNoteClone->
      appendWedgeToNote (elt);
  }
  else if (fOnGoingChord) {
    fCurrentChordClone->
      appendWedgeToChord (elt);
  }
  else {
    std::stringstream s;

    s <<
      "wedge '" << elt->asShortString () <<
      "' is out of context, cannot be handled";

    msr2mxsrInternalError (
      gGlobalServiceRunData->getInputSourceName (),
      elt->getInputLineNumber (),
      __FILE__, __LINE__,
      s.str ());
  }
}

void msr2mxsrTranslator::visitEnd (S_msrWedge& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrWedge" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif
}

//________________________________________________________________________
void msr2mxsrTranslator::visitStart (S_msrWedge& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrWedge" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

  if (fOnGoingNonGraceNote) {
    fCurrentNonGraceNoteClone->
      appendWedgeToNote (elt);
  }
  else if (fOnGoingChord) {
    fCurrentChordClone->
      appendWedgeToChord (elt);
  }
  else {
    std::stringstream s;

    s <<
      "wedge '" << elt->asShortString () <<
      "' is out of context, cannot be handled";

    msr2mxsrInternalError (
      gGlobalServiceRunData->getInputSourceName (),
      elt->getInputLineNumber (),
      __FILE__, __LINE__,
      s.str ());
  }
}

void msr2mxsrTranslator::visitEnd (S_msrWedge& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrWedge" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif
}

*/

/*
//________________________________________________________________________
void msr2mxsrTranslator::visitStart (S_msrOctaveShift& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrOctaveShift" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

  if (fOnGoingNonGraceNote) {
    fCurrentNonGraceNoteClone->
      setNoteOctaveShift (elt);
  }
  else if (fOnGoingChord) {
    fCurrentChordClone->
      setChordOctaveShift (elt);
  }
  else {
    std::stringstream s;

    s <<
      "octaveShift '" << elt->asShortString () <<
      "' is out of context, cannot be handled";

    msr2mxsrInternalError (
      gGlobalServiceRunData->getInputSourceName (),
      elt->getInputLineNumber (),
      __FILE__, __LINE__,
      s.str ());
  }
}

void msr2mxsrTranslator::visitEnd (S_msrOctaveShift& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrOctaveShift" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif
}

//________________________________________________________________________
void msr2mxsrTranslator::visitStart (S_msrAccordionRegistration& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrAccordionRegistration" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

  // append the accordion registration to the voice clone
  fCurrentVoiceClone->
    appendAccordionRegistrationToVoice (elt);

  // the generated code needs modules scm and accreg
  fResultingMusicxmlelement->
    setScmAndAccregSchemeModulesAreNeeded ();
}

//________________________________________________________________________
void msr2mxsrTranslator::visitStart (S_msrHarpPedalsTuning& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrHarpPedalsTuning" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

  // append the harp pedals tuning to the voice clone
  fCurrentVoiceClone->
    appendHarpPedalsTuningToVoice (elt);
}

//________________________________________________________________________
void msr2mxsrTranslator::visitStart (S_msrStem& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrStem" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

  if (fOnGoingNonGraceNote) {
    fCurrentNonGraceNoteClone->
      setNoteStem (elt);
  }
  else if (fOnGoingChord) {
    fCurrentChordClone->
      appendStemToChord (elt);
  }
  else {
    std::stringstream s;

    s <<
      "stem '" << elt->asShortString () <<
      "' is out of context, cannot be handled";

    msr2mxsrInternalError (
      gGlobalServiceRunData->getInputSourceName (),
      elt->getInputLineNumber (),
      __FILE__, __LINE__,
      s.str ());
  }
}

void msr2mxsrTranslator::visitEnd (S_msrStem& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrStem" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif
}

//________________________________________________________________________
void msr2mxsrTranslator::visitStart (S_msrBeam& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrBeam" <<
      ", line " << elt->getInputLineNumber () <<
// JMI      ", fOnGoingNonGraceNote = " << fOnGoingNonGraceNote <<
// JMI      ", fOnGoingChord = " << fOnGoingChord <<
      std::endl;
  }
#endif

  // a beam may be present at the same time
  // in a note or grace note and the chord the latter belongs to

  if (fOnGoingGraceNotesGroup) {
    fCurrentGraceNoteClone->
      appendBeamToNote (elt);
  }
  else if (fOnGoingNonGraceNote) {
    fCurrentNonGraceNoteClone->
      appendBeamToNote (elt);
  }

  if (fOnGoingChord) { // else ??? JMI
    fCurrentChordClone->
      appendBeamToChord (elt);
  }
}

void msr2mxsrTranslator::visitEnd (S_msrBeam& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrBeam" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif
}

//________________________________________________________________________
void msr2mxsrTranslator::visitStart (S_msrTie& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrTie" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

  if (fOnGoingNonGraceNote) {
    fCurrentNonGraceNoteClone->
      setNoteTie (elt);
  }
  else if (fOnGoingChord) {
    fCurrentChordClone->
      appendTieToChord (elt);
  }
  else {
    std::stringstream s;

    s <<
      "tie '" << elt->asShortString () <<
      "' is out of context, cannot be handled";

    msr2mxsrInternalError (
      gGlobalServiceRunData->getInputSourceName (),
      elt->getInputLineNumber (),
      __FILE__, __LINE__,
      s.str ());
  }
}

void msr2mxsrTranslator::visitEnd (S_msrTie& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrTie" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif
}

//________________________________________________________________________
void msr2mxsrTranslator::visitStart (S_msrSegno& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrSegno" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

  if (fOnGoingChord || fOnGoingNonGraceNote) {
    if (fOnGoingChord) {
      fCurrentChordClone->
        appendSegnoToChord (elt);
    }
    if (fOnGoingNonGraceNote) {
      fCurrentNonGraceNoteClone->
        appendSegnoToNote (elt);
    }
  }
  else {
    std::stringstream s;

    s <<
      "segno '" << elt->asShortString () <<
      "' is out of context, cannot be handled";

    msr2mxsrInternalError (
      gGlobalServiceRunData->getInputSourceName (),
      elt->getInputLineNumber (),
      __FILE__, __LINE__,
      s.str ());
  }
}

void msr2mxsrTranslator::visitStart (S_msrDalSegno& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrDalSegno" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  if (fOnGoingChord || fOnGoingNonGraceNote) {
    if (fOnGoingChord) {
      fCurrentChordClone->
        appendDalSegnoToChord (elt);
    }
    if (fOnGoingNonGraceNote) {
      fCurrentNonGraceNoteClone->
        appendDalSegnoToNote (elt);
    }
  }
  else {
    std::stringstream s;

    s <<
      "dal segno '" << elt->asShortString () <<
      "' is out of context, cannot be handled";

    msr2mxsrInternalError (
      gGlobalServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }
}

void msr2mxsrTranslator::visitStart (S_msrCoda& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrCoda" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

  if (fOnGoingChord || fOnGoingNonGraceNote) {
    if (fOnGoingChord) {
      fCurrentChordClone->
        appendCodaToChord (elt);
    }
    if (fOnGoingNonGraceNote) {
      fCurrentNonGraceNoteClone->
        appendCodaToNote (elt);
    }
  }
  else {
    std::stringstream s;

    s <<
      "coda '" << elt->asShortString () <<
      "' is out of context, cannot be handled";

    msr2mxsrInternalError (
      gGlobalServiceRunData->getInputSourceName (),
      elt->getInputLineNumber (),
      __FILE__, __LINE__,
      s.str ());
  }
}

//________________________________________________________________________
void msr2mxsrTranslator::visitStart (S_msrEyeGlasses& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting eyeGlasses" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

  fCurrentNonGraceNoteClone->
    appendEyeGlassesToNote (elt);
}

void msr2mxsrTranslator::visitStart (S_msrScordatura& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting scordatura" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

  fCurrentNonGraceNoteClone->
    appendScordaturaToNote (elt);
}

void msr2mxsrTranslator::visitStart (S_msrPedal& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting pedal" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

  fCurrentNonGraceNoteClone->
    appendPedalToNote (elt);
}

void msr2mxsrTranslator::visitStart (S_msrDamp& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting damp" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

  fCurrentNonGraceNoteClone->
    appendDampToNote (elt);

  fResultingMusicxmlelement->
    // this score needs the 'custom short barLine' Scheme function
    setDampMarkupIsNeeded ();
}

void msr2mxsrTranslator::visitStart (S_msrDampAll& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting dampAll" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

  fCurrentNonGraceNoteClone->
    appendDampAllToNote (elt);

  fResultingMusicxmlelement->
    // this score needs the 'custom short barLine' Scheme function
    setDampAllMarkupIsNeeded ();
}

//________________________________________________________________________
void msr2mxsrTranslator::visitStart (S_msrBarCheck& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrBarCheck" <<
      ", nextBarNumber: " <<
      elt->getNextBarPuristNumber () <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

  fCurrentVoiceClone->
    appendBarCheckToVoice (elt);
}

void msr2mxsrTranslator::visitEnd (S_msrBarCheck& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrBarCheck" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif
}

//________________________________________________________________________
void msr2mxsrTranslator::visitStart (S_msrBarNumberCheck& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrBarNumberCheck" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

  fCurrentVoiceClone->
    appendBarNumberCheckToVoice (elt);
}

void msr2mxsrTranslator::visitEnd (S_msrBarNumberCheck& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrBarNumberCheck" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif
}

//________________________________________________________________________
void msr2mxsrTranslator::visitStart (S_msrLineBreak& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrLineBreak" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

  fCurrentVoiceClone->
    appendLineBreakToVoice (elt);
}

void msr2mxsrTranslator::visitEnd (S_msrLineBreak& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrLineBreak" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif
}

//________________________________________________________________________
void msr2mxsrTranslator::visitStart (S_msrPageBreak& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrPageBreak" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif

  fCurrentVoiceClone->
    appendPageBreakToVoice (elt);
}

void msr2mxsrTranslator::visitEnd (S_msrPageBreak& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrPageBreak" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif
}

//________________________________________________________________________
void msr2mxsrTranslator::visitStart (S_msrRepeat& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrRepeat" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeats ()) {
    gLogStream <<
      "Handling repeat start in voice clone \"" <<
      fCurrentVoiceClone->getVoiceName () <<
      "\"" <<
      std::endl;
  }
#endif

  fCurrentVoiceClone->
    handleRepeatStartInVoiceClone (
      inputLineNumber,
      elt);
}

void msr2mxsrTranslator::visitEnd (S_msrRepeat& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrRepeat" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeats ()) {
    gLogStream <<
      "Handling repeat end in voice clone \"" <<
      fCurrentVoiceClone->getVoiceName () <<
//      "\" in part \"" <<
//      fCurrentPartClone->getPartCombinedName () <<
      "\"" <<
      std::endl;
  }
#endif

  fCurrentVoiceClone->
    handleRepeatEndInVoiceClone (
      inputLineNumber);
}

//________________________________________________________________________
void msr2mxsrTranslator::visitStart (S_msrRepeatCommonPart& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrRepeatCommonPart" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  fCurrentVoiceClone->
    handleRepeatCommonPartStartInVoiceClone (
      inputLineNumber);
}

void msr2mxsrTranslator::visitEnd (S_msrRepeatCommonPart& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrRepeatCommonPart" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  fCurrentVoiceClone->
    handleRepeatCommonPartEndInVoiceClone (
      inputLineNumber);
}

//________________________________________________________________________
void msr2mxsrTranslator::visitStart (S_msrRepeatEnding& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrRepeatEnding" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  // handle the repeat ending start in the voice clone
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeats ()) {
    gLogStream <<
      "Handling a repeat ending start in voice clone \"" <<
      fCurrentVoiceClone->getVoiceName () <<
      "\"" <<
      std::endl;
  }
#endif

  fCurrentVoiceClone->
    handleRepeatEndingStartInVoiceClone (
      inputLineNumber,
      elt->getRepeatEndingKind (),
      elt->getRepeatEndingNumber ());
}

void msr2mxsrTranslator::visitEnd (S_msrRepeatEnding& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrRepeatEnding" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  // handle the repeat ending end in the voice clone
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeats ()) {
    gLogStream <<
      "Handling a repeat ending end in voice clone \"" <<
      fCurrentVoiceClone->getVoiceName () <<
      "\"" <<
      std::endl;
  }
#endif

  fCurrentVoiceClone->
    handleRepeatEndingEndInVoiceClone (
      inputLineNumber,
      elt->getRepeatEndingNumber (),
      elt->getRepeatEndingKind ());
}

//________________________________________________________________________
void msr2mxsrTranslator::visitStart (S_msrMultipleFullBarRests& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrMultipleFullBarRests" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  ++gIndenter;

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMultipleFullBarRests ()) {
    gLogStream <<
      "Handling multiple rest start in voice clone \"" <<
      fCurrentVoiceClone->getVoiceName () <<
      "\"" <<
      std::endl;
  }
#endif

  fCurrentVoiceClone->
    handleMultipleFullBarRestsStartInVoiceClone (
      inputLineNumber,
      elt);
}

void msr2mxsrTranslator::visitEnd (S_msrMultipleFullBarRests& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrMultipleFullBarRests" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  --gIndenter;

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMultipleFullBarRests ()) {
    gLogStream <<
      "Handling multiple rest start in voice clone \"" <<
      fCurrentVoiceClone->getVoiceName () <<
      "\"" <<
      std::endl;
  }
#endif

  fCurrentVoiceClone->
    handleMultipleFullBarRestsEndInVoiceClone (
      inputLineNumber);
}

//________________________________________________________________________
void msr2mxsrTranslator::visitStart (S_msrMeasureRepeat& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrMeasureRepeat" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  ++gIndenter;

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasureRepeats ()) {
    gLogStream <<
      "Handling measures repeat start in voice clone \"" <<
      fCurrentVoiceClone->getVoiceName () <<
      "\"" <<
      std::endl;
  }
#endif

  fCurrentVoiceClone->
    handleMeasureRepeatStartInVoiceClone (
      inputLineNumber,
      elt);
}

void msr2mxsrTranslator::visitEnd (S_msrMeasureRepeat& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrMeasureRepeat" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  --gIndenter;

/ * JMI
  // set last segment as the measures repeat pattern segment
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasureRepeats ()) {
    gLogStream <<
      "Setting current last segment as measures repeat pattern segment in voice \"" <<
      fCurrentVoiceClone->getVoiceName () <<
      "\"" <<
      std::endl;
  }
#endif
* /

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasureRepeats ()) {
    gLogStream <<
      "Handling measures repeat end in voice clone \"" <<
      fCurrentVoiceClone->getVoiceName () <<
      "\"" <<
      std::endl;
  }
#endif

  fCurrentVoiceClone->
    handleMeasureRepeatEndInVoiceClone (
      inputLineNumber);
}

//________________________________________________________________________
void msr2mxsrTranslator::visitStart (S_msrMeasureRepeatPattern& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrMeasureRepeatPattern" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  ++gIndenter;

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasureRepeats ()) {
    fCurrentVoiceClone->
      displayVoice (
        inputLineNumber,
        "Upon visitStart (S_msrMeasureRepeatPattern&)");
  }
#endif

  fCurrentVoiceClone->
    handleMeasureRepeatPatternStartInVoiceClone (
      inputLineNumber);
}

void msr2mxsrTranslator::visitEnd (S_msrMeasureRepeatPattern& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrMeasureRepeatPattern" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  --gIndenter;

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasureRepeats ()) {
    fCurrentVoiceClone->
      displayVoice (
        inputLineNumber,
        "Upon visitEnd (S_msrMeasureRepeatPattern&) 1");
  }
#endif

  fCurrentVoiceClone->
    handleMeasureRepeatPatternEndInVoiceClone (
      inputLineNumber);
}

//________________________________________________________________________
void msr2mxsrTranslator::visitStart (S_msrMeasureRepeatReplicas& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrMeasureRepeatReplicas" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  ++gIndenter;

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasureRepeats ()) {
    fCurrentVoiceClone->
      displayVoice (
        inputLineNumber,
        "Upon visitStart (S_msrMeasureRepeatReplicas&)");
  }
#endif

  fCurrentVoiceClone->
    handleMeasureRepeatReplicasStartInVoiceClone (
      inputLineNumber);
}

void msr2mxsrTranslator::visitEnd (S_msrMeasureRepeatReplicas& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting S_msrMeasureRepeatReplicas" <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  --gIndenter;


#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasureRepeats ()) {
    fCurrentVoiceClone->
      displayVoice (
        inputLineNumber,
        "Upon visitEnd (S_msrMeasureRepeatReplicas&) 1");
  }
#endif

  fCurrentVoiceClone->
    handleMeasureRepeatReplicasEndInVoiceClone (
      inputLineNumber);
}

//________________________________________________________________________
void msr2mxsrTranslator::visitStart (S_msrMidiTempo& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrMidiTempo" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif
}

void msr2mxsrTranslator::visitEnd (S_msrMidiTempo& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrMidiTempo" <<
      ", line " << elt->getInputLineNumber () <<
      std::endl;
  }
#endif
}
*/

} // namespace


/*
//________________________________________________________________________
void msr2mxsrTranslator::displayCurrentOnGoingValues ()
{
  gLogStream <<
    "Ongoing value:" <<
    std::endl;

  ++gIndenter;

  const int fieldWidth = 25;

  gLogStream <<
    std::setw (fieldWidth) <<
    "fOnGoingStaff" << ": " << fOnGoingStaff <<
    std::endl<<
    std::setw (fieldWidth) <<
    "fOnGoingHarmoniesVoice" << ": " << fOnGoingHarmoniesVoice <<
    std::endl<<
    std::setw (fieldWidth) <<
    "fOnGoingHarmony" << ": " << fOnGoingHarmony <<
    std::endl<<
    std::setw (fieldWidth) <<
    "fOnGoingFiguredBassVoice" << ": " << fOnGoingFiguredBassVoice <<
    std::endl<<
    std::setw (fieldWidth) <<
    "fOnGoingNonGraceNote" << ": " << fOnGoingNonGraceNote <<
    std::endl<<
    std::setw (fieldWidth) <<
    "fOnGoingDoubleTremolo" << ": " << fOnGoingDoubleTremolo <<
    std::endl<<
    std::setw (fieldWidth) <<
    "fOnGoingGraceNotesGroup" << ": " << fOnGoingGraceNotesGroup <<
    std::endl<<
    std::setw (fieldWidth) <<
    "fOnGoingChord" << ": " << fOnGoingChord <<
    std::endl<<
    std::setw (fieldWidth) <<
    "fOnGoingStanza" << ": " << fOnGoingStanza <<
    std::endl<<
    std::setw (fieldWidth) <<
    "fOnGoingSyllableExtend" << ": " << fOnGoingSyllableExtend <<
    std::endl;

  --gIndenter;
}
*/

/* JMI
//________________________________________________________________________
// a comparison class   to sort elements
class   musicxmlOrder
{
  public:

    // constructors/destructor
    // ------------------------------------------------------

    musicxmlOrder (
      std::map<int,int>& order, Sxmlelement container);

    virtual  ~musicxmlOrder ();

  public:

    // public services
    // ------------------------------------------------------

    Bool                  operator()  (Sxmlelement a, Sxmlelement b);

  private:

    // private fields
    // ------------------------------------------------------

    std::map<int,int>&          fOrder;
    Sxmlelement            fContainer;
};

musicxmlOrder::musicxmlOrder (
  std::map<int,int>& order, Sxmlelement container)
  : fOrder (order)
{
  fContainer = container;
}

musicxmlOrder::~musicxmlOrder ()
{}

Bool musicxmlOrder::operator() (Sxmlelement a, Sxmlelement b)
{
  int aIndex = fOrder [a->getType ()];
  int bIndex = fOrder [b->getType ()];

  if (aIndex == 0) return false; // wrong a element: reject to end of std::list
  if (bIndex == 0) return true;   // wrong b element: reject to end of std::list

  return aIndex < bIndex;
}
*/


/*
  if       (timeSignatureSymbol == "common") {
    fCurrentTimeSignatureSymbolKind = msrTimeSignatureSymbolKind::kTimeSignatureSymbolCommon;
  }
  else  if (timeSignatureSymbol == "cut") {
    fCurrentTimeSignatureSymbolKind = msrTimeSignatureSymbolKind::kTimeSignatureSymbolCut;
  }
  else  if (timeSignatureSymbol == "note") {
    fCurrentTimeSignatureSymbolKind = msrTimeSignatureSymbolKind::kTimeSignatureSymbolNote;
  }
  else  if (timeSignatureSymbol == "dotted-note") {
    fCurrentTimeSignatureSymbolKind = msrTimeSignatureSymbolKind::kTimeSignatureSymbolDottedNote;
  }
  else  if (timeSignatureSymbol == "single-number") {
    fCurrentTimeSignatureSymbolKind = msrTimeSignatureSymbolKind::kTimeSignatureSymbolSingleNumber;
  }

  // handle the time
  if (timeSignatureSymbolKind == msrTimeSignatureSymbolKind::kTimeSignatureSymbolSenzaMisura) {
    // senza misura time

    fVoiceIsCurrentlySenzaMisura = true;
  }

  else {
    // con misura time

    int timesItemsNumber =
      timeSignatureItemsVector.size ();

    if (timesItemsNumber) {
      // should there be a single number?
      switch (timeSignatureSymbolKind) {
        case msrTimeSignatureSymbolKind::kTimeSignatureSymbolCommon:
          break;
        case msrTimeSignatureSymbolKind::kTimeSignatureSymbolCut:
          break;
        case msrTimeSignatureSymbolKind::kTimeSignatureSymbolNote:
          break;
        case msrTimeSignatureSymbolKind::kTimeSignatureSymbolDottedNote:
          break;
        case msrTimeSignatureSymbolKind::kTimeSignatureSymbolSingleNumber:
          fOutputStream <<
            "\\once\\override Staff.TimeSignature.style = #'single-digit" <<
            std::endl;
          break;
        case msrTimeSignatureSymbolKind::kTimeSignatureSymbolSenzaMisura:
          break;
        case msrTimeSignatureSymbolKind::kTimeSignatureSymbolNone:
          break;
      } // switch

      if (! elt->getTimeIsCompound ()) {
        // simple time
        // \time "3/4" for 3/4
        // or senza misura

        S_msrTimeSignatureItem
          timeSignatureItem =
            timeSignatureItemsVector [0]; // the only element;

        // fetch the time signature item beat numbers std::vector
        const std::vector<int>&
          beatsNumbersVector =
            timeSignatureItem->
              getTimeSignatureBeatsNumbersVector ();

        // should the time be numeric?
        if (
          timeSignatureSymbolKind == msrTimeSignatureSymbolKind::kTimeSignatureSymbolNone
            ||
          gGlobalLpsr2lilypondOahGroup->getNumericalTime ()) {
          fOutputStream <<
            "\\numericTimeSignature ";
        }

        fOutputStream <<
          "\\time " <<
          beatsNumbersVector [0] << // the only element
          '/' <<
          timeSignatureItem->getTimeSignatureBeatValue () <<
          std::endl;
      }

      else {
        // compound time
        // \compoundMeter #'(3 2 8) for 3+2/8
        // \compoundMeter #'((3 8) (2 8) (3 4)) for 3/8+2/8+3/4
        // \compoundMeter #'((3 2 8) (3 4)) for 3+2/8+3/4

        fOutputStream <<
          "\\compoundMeter #`(";

        // handle all the time signature items in the std::vector
        for (int i = 0; i < timesItemsNumber; ++i) {
          S_msrTimeSignatureItem
            timeSignatureItem =
              timeSignatureItemsVector [i];

          // fetch the time signature item beat numbers std::vector
          const std::vector<int>&
            beatsNumbersVector =
              timeSignatureItem->
                getTimeSignatureBeatsNumbersVector ();

          int beatsNumbersNumber =
            beatsNumbersVector.size ();

          // first generate the opening parenthesis
          fOutputStream <<
            "(";

          // then generate all beats numbers in the std::vector
          for (int j = 0; j < beatsNumbersNumber; ++j) {
            fOutputStream <<
              beatsNumbersVector [j] <<
              ' ';
          } // for

          // then generate the beat type
          fOutputStream <<
            timeSignatureItem->getTimeSignatureBeatValue ();

          // and finally generate the closing parenthesis
          fOutputStream <<
            ")";

          if (i != timesItemsNumber - 1) {
            fOutputStream <<
              ' ';
          }
        } // for

      fOutputStream <<
        ")" <<
        std::endl;
      }
    }

    else {
      // there are no time signature items
      if (timeSignatureSymbolKind != msrTimeSignatureSymbolKind::kTimeSignatureSymbolSenzaMisura) {
        msr2mxsrInternalError (
          gGlobalServiceRunData->getInputSourceName (),
          elt->getInputLineNumber (),
          __FILE__, __LINE__,
          "time signature items std::vector is empty");
      }
    }
  }
*/

