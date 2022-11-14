/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <iomanip>      // std::setw()), set::precision(), ...
#include <iostream>
#include <sstream>

#include <regex>

#include "visitor.h"

#include "oahWae.h"
#include "msrWae.h"

#include "enableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "tracingOah.h"
#endif

#include "mfAssert.h"

#include "mfServiceRunData.h"
#include "mfStringsHandling.h"

#include "oahEarlyOptions.h"

#include "msrMeasures.h"

#include "msrTimeSignatures.h"

#include "oahOah.h"

#include "msrOah.h"


namespace MusicFormats
{

// time signatures
//______________________________________________________________________________
std::string msrTimeSignatureSymbolKindAsString (
  msrTimeSignatureSymbolKind timeSignatureSymbolKind)
{
  std::string result;

  switch (timeSignatureSymbolKind) {
    case msrTimeSignatureSymbolKind::kTimeSignatureSymbolCommon:
      result = "kTimeSignatureSymbolCommon";
      break;
    case msrTimeSignatureSymbolKind::kTimeSignatureSymbolCut:
      result = "kTimeSignatureSymbolCut";
      break;
    case msrTimeSignatureSymbolKind::kTimeSignatureSymbolNote:
      result = "kTimeSignatureSymbolNote";
      break;
    case msrTimeSignatureSymbolKind::kTimeSignatureSymbolDottedNote:
      result = "kTimeSignatureSymbolDottedNote";
      break;
    case msrTimeSignatureSymbolKind::kTimeSignatureSymbolSingleNumber:
      result = "kTimeSignatureSymbolSingleNumber";
      break;
    case msrTimeSignatureSymbolKind::kTimeSignatureSymbolSenzaMisura:
      result = "kTimeSignatureSymbolSenzaMisura";
      break;
    case msrTimeSignatureSymbolKind::kTimeSignatureSymbolNone:
      result = "kTimeSignatureSymbolNone";
      break;
  } // switch

  return result;
}

std::ostream& operator << (std::ostream& os, const msrTimeSignatureSymbolKind& elt)
{
  os << msrTimeSignatureSymbolKindAsString (elt);
  return os;
}

std::string msrTimeSignatureSeparatorKindAsString (
  msrTimeSignatureSeparatorKind timeSignatureSeparatorKind)
{
  std::string result;

  switch (timeSignatureSeparatorKind) {
    case msrTimeSignatureSeparatorKind::kTimeSignatureSeparatorNone:
      result = "kTimeSignatureSeparatorNone";
      break;
    case msrTimeSignatureSeparatorKind::kTimeSignatureSeparatorHorizontal:
      result = "kTimeSignatureSeparatorHorizontal";
      break;
    case msrTimeSignatureSeparatorKind::kTimeSignatureSeparatorDiagonal:
      result = "kTimeSignatureSeparatorDiagonal";
      break;
    case msrTimeSignatureSeparatorKind::kTimeSignatureSeparatorVertical:
      result = "kTimeSignatureSeparatorVertical";
      break;
    case msrTimeSignatureSeparatorKind::kTimeSignatureSeparatorAdjacent:
      result = "kTimeSignatureSeparatorAdjacent";
      break;
  } // switch

  return result;
}

std::ostream& operator << (std::ostream& os, const msrTimeSignatureSeparatorKind& elt)
{
  os << msrTimeSignatureSeparatorKindAsString (elt);
  return os;
}

std::string msrTimeSignatureRelationKindAsString (
  msrTimeSignatureRelationKind timeSignatureRelationKind)
{
  std::string result;

  switch (timeSignatureRelationKind) {
    case msrTimeSignatureRelationKind::kTimeSignatureRelationNone:
      result = "kTimeSignatureRelationNone";
      break;
    case msrTimeSignatureRelationKind::kTimeSignatureRelationParentheses:
      result = "kTimeSignatureRelationParentheses";
      break;
    case msrTimeSignatureRelationKind::kTimeSignatureRelationBracket:
      result = "kTimeSignatureRelationBracket";
      break;
    case msrTimeSignatureRelationKind::kTimeSignatureRelationEquals:
      result = "kTimeSignatureRelationEquals";
      break;
    case msrTimeSignatureRelationKind::kTimeSignatureRelationSlash:
      result = "kTimeSignatureRelationSlash";
      break;
    case msrTimeSignatureRelationKind::kTimeSignatureRelationSpace:
      result = "kTimeSignatureRelationSpace";
      break;
    case msrTimeSignatureRelationKind::kTimeSignatureRelationHyphen:
      result = "kTimeSignatureRelationHyphen";
      break;
  } // switch

  return result;
}

std::ostream& operator << (std::ostream& os, const msrTimeSignatureRelationKind& elt)
{
  os << msrTimeSignatureRelationKindAsString (elt);
  return os;
}

//______________________________________________________________________________
S_msrTimeSignatureItem msrTimeSignatureItem::create (
  int inputLineNumber)
{
  msrTimeSignatureItem* o =
    new msrTimeSignatureItem (
      inputLineNumber);
  assert (o != nullptr);

  return o;
}

msrTimeSignatureItem::msrTimeSignatureItem (
  int inputLineNumber)
    : msrElement (inputLineNumber)
{
  fTimeSignatureBeatValue = -1;

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceTimeSignatures ()) {
    gLogStream <<
      "Creating time signature item" <<
      ", line = " << inputLineNumber <<
      std::endl;
  }
#endif
}

msrTimeSignatureItem::~msrTimeSignatureItem ()
{}

Bool msrTimeSignatureItem::isEqualTo (S_msrTimeSignatureItem otherTimeSignatureItem) const
{
  if (! otherTimeSignatureItem) {
    return false;
  }

  if (
    ! (
        fTimeSignatureBeatValue == otherTimeSignatureItem->fTimeSignatureBeatValue
            &&
        fTimeSignatureBeatsNumbersVector.size ()
          ==
        otherTimeSignatureItem->fTimeSignatureBeatsNumbersVector.size ()
      )
    ) {
    return false;
  }

  for (size_t i = 0; i < fTimeSignatureBeatsNumbersVector.size (); ++i) {
    if (
      ! (
        fTimeSignatureBeatsNumbersVector [i]
          ==
        otherTimeSignatureItem->fTimeSignatureBeatsNumbersVector [i]
        )
      ) {
      return false;
    }
  } // for

  return true;
}

void msrTimeSignatureItem::appendBeatsNumber (int beatsNumber)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceTimeSignatures ()) {
    gLogStream <<
      "Appending beat number '" <<
      beatsNumber <<
      "' to time signature item '" <<
      asString () <<
      "'" <<
      std::endl;
    }
#endif

  fTimeSignatureBeatsNumbersVector.insert (
    fTimeSignatureBeatsNumbersVector.end (),
    beatsNumber);
}

void msrTimeSignatureItem::setTimeSignatureBeatValue (int timeSignatureBeatValue)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceTimeSignatures ()) {
    gLogStream <<
      "Setting beat value to '" <<
      timeSignatureBeatValue <<
      "' in time signature item '" <<
      asString () <<
      "'" <<
      std::endl;
    }
#endif

  fTimeSignatureBeatValue = timeSignatureBeatValue;
}

int msrTimeSignatureItem::getTimeSignatureBeatsNumber () const
{
  int result = 0;

  for (size_t i = 0; i < fTimeSignatureBeatsNumbersVector.size (); ++i) {
    result +=
      fTimeSignatureBeatsNumbersVector [i];
    } // for

  return result;
}

void msrTimeSignatureItem::acceptIn (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrTimeSignatureItem::acceptIn ()" <<
      std::endl;
  }

  if (visitor<S_msrTimeSignatureItem>*
    p =
      dynamic_cast<visitor<S_msrTimeSignatureItem>*> (v)) {
        S_msrTimeSignatureItem elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrTimeSignatureItem::visitStart ()" <<
            std::endl;
        }
        p->visitStart (elem);
  }
}

void msrTimeSignatureItem::acceptOut (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrTimeSignatureItem::acceptOut ()" <<
      std::endl;
  }

  if (visitor<S_msrTimeSignatureItem>*
    p =
      dynamic_cast<visitor<S_msrTimeSignatureItem>*> (v)) {
        S_msrTimeSignatureItem elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrTimeSignatureItem::visitEnd ()" <<
            std::endl;
        }
        p->visitEnd (elem);
  }
}

void msrTimeSignatureItem::browseData (basevisitor* v)
{}

std::string msrTimeSignatureItem::asString () const
{
  std::stringstream s;

  s <<
    "[TimeSignatureItem ";

  size_t vectorSize =
    fTimeSignatureBeatsNumbersVector.size ();

  switch (vectorSize) {
    case 0:
    /* JMI
      msrInternalError (
        gGlobalServiceRunData->getInputSourceName (),
        fInputLineNumber,
        __FILE__, __LINE__,
        "time signature item beats numbers std::vector is empty");
        */
      s <<
        "beats numbers: [NONE]";
      break;

    case 1:
      s <<
        fTimeSignatureBeatsNumbersVector [0] << '/' << fTimeSignatureBeatValue;
      break;

    default:
      s <<
        "beats numbers: ";

      for (size_t i = 0; i < vectorSize; ++i) {
        s <<
          fTimeSignatureBeatsNumbersVector [i];

        if (i != vectorSize - 1) {
          s <<
            ' ';
        }
      } // for

      s <<
        ", beat value: " << fTimeSignatureBeatValue;
  } // switch

  s <<
    ", line " << fInputLineNumber <<
    ']';

  return s.str ();
}

std::string msrTimeSignatureItem::asShortStringForMeasuresSlices () const
{
  std::stringstream s;

//   s <<
//     "TimeSignatureItem ";

  size_t vectorSize =
    fTimeSignatureBeatsNumbersVector.size ();

  switch (vectorSize) {
    case 0:
    /* JMI
      msrInternalError (
        gGlobalServiceRunData->getInputSourceName (),
        fInputLineNumber,
        __FILE__, __LINE__,
        "time signature item beats numbers std::vector is empty");
        */
      s <<
        "beats numbers: [NONE]";
      break;

    case 1:
      s <<
        fTimeSignatureBeatsNumbersVector [0] << '/' << fTimeSignatureBeatValue;
      break;

    default:
      s <<
        "beats numbers: ";

      for (size_t i = 0; i < vectorSize; ++i) {
        s <<
          fTimeSignatureBeatsNumbersVector [i];

        if (i != vectorSize - 1) {
          s <<
            ' ';
        }
      } // for

      s <<
        ", beat value: " << fTimeSignatureBeatValue;
  } // switch

/* JMI
  s <<
    ", line " << fInputLineNumber;
*/

  return s.str ();
}

void msrTimeSignatureItem::print (std::ostream& os) const
{
  os << asString () << std::endl;
}

std::ostream& operator << (std::ostream& os, const S_msrTimeSignatureItem& elt)
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
S_msrTimeSignature msrTimeSignature::create (
  int            inputLineNumber,
  S_msrMeasure&  upLinkToMeasure,
  msrTimeSignatureSymbolKind
                 timeSignatureSymbolKind)
{
  msrTimeSignature* o =
    new msrTimeSignature (
      inputLineNumber,
      upLinkToMeasure,
      timeSignatureSymbolKind);
  assert (o != nullptr);
  return o;
}

S_msrTimeSignature msrTimeSignature::create (
  int            inputLineNumber,
  msrTimeSignatureSymbolKind
                 timeSignatureSymbolKind)
{
  return
    msrTimeSignature::create (
      inputLineNumber,
      gNullMeasureSmartPointer, // set later in setMeasureElementUpLinkToMeasure()
      timeSignatureSymbolKind);
}

msrTimeSignature::msrTimeSignature (
  int                 inputLineNumber,
  const S_msrMeasure& upLinkToMeasure,
  msrTimeSignatureSymbolKind
                timeSignatureSymbolKind)
    : msrMeasureElementLambda (
      inputLineNumber,
      upLinkToMeasure)
{
  fTimeSignatureSymbolKind = timeSignatureSymbolKind;

  fTimeIsCompound = false;
}

Bool msrTimeSignature::isEqualTo (S_msrTimeSignature otherTimeSignature) const
{
  if (! otherTimeSignature) {
    return false;
  }

  if (
    ! (
        fTimeSignatureSymbolKind == otherTimeSignature->fTimeSignatureSymbolKind
          &&
        fTimeIsCompound == otherTimeSignature->fTimeIsCompound
          &&
        fTimeSignatureItemsVector.size () == otherTimeSignature->fTimeSignatureItemsVector.size ()
      )
    ) {
    return false;
  }

  for (size_t i = 0; i < fTimeSignatureItemsVector.size (); ++i) {
    if (
      ! (
        fTimeSignatureItemsVector [i]->isEqualTo (
          otherTimeSignature->fTimeSignatureItemsVector [i])
        )
      ) {
      return false;
    }
  } // for

  return true;
}

S_msrTimeSignature msrTimeSignature::createTwoEightsTime (
  int inputLineNumber)
{
  // create the time
  S_msrTimeSignature
    timeSignature =
      msrTimeSignature::create (
        inputLineNumber,
        gNullMeasureSmartPointer, // set later in setMeasureElementUpLinkToMeasure()
        msrTimeSignatureSymbolKind::kTimeSignatureSymbolNone);

  // create a four quarters time signature item
  S_msrTimeSignatureItem
    timeSignatureItem =
      msrTimeSignatureItem::create (
        inputLineNumber);

  timeSignatureItem->
    appendBeatsNumber (2);
  timeSignatureItem->
    setTimeSignatureBeatValue (8);

  // append the time signature item to the time
  timeSignature->
    appendTimeSignatureItem (timeSignatureItem);

  // return the time
  return timeSignature;
}

S_msrTimeSignature msrTimeSignature::createThreeEightsTime (
  int inputLineNumber)
{
  // create the time
  S_msrTimeSignature
    timeSignature =
      msrTimeSignature::create (
        inputLineNumber,
        gNullMeasureSmartPointer, // set later in setMeasureElementUpLinkToMeasure()
        msrTimeSignatureSymbolKind::kTimeSignatureSymbolNone);

  // create a four quarters time signature item
  S_msrTimeSignatureItem
    timeSignatureItem =
      msrTimeSignatureItem::create (
        inputLineNumber);

  timeSignatureItem->
    appendBeatsNumber (3);
  timeSignatureItem->
    setTimeSignatureBeatValue (8);

  // append the time signature item to the time
  timeSignature->
    appendTimeSignatureItem (timeSignatureItem);

  // return the timeSignature
  return timeSignature;
}

S_msrTimeSignature msrTimeSignature::createSixEightsTime (
  int inputLineNumber)
{
  // create the time
  S_msrTimeSignature
    timeSignature =
      msrTimeSignature::create (
        inputLineNumber,
        gNullMeasureSmartPointer, // set later in setMeasureElementUpLinkToMeasure()
        msrTimeSignatureSymbolKind::kTimeSignatureSymbolNone);

  // create a four quarters time signature item
  S_msrTimeSignatureItem
    timeSignatureItem =
      msrTimeSignatureItem::create (
        inputLineNumber);

  timeSignatureItem->
    appendBeatsNumber (6);
  timeSignatureItem->
    setTimeSignatureBeatValue (8);

  // append the time signature item to the time
  timeSignature->
    appendTimeSignatureItem (timeSignatureItem);

  // return the timeSignature
  return timeSignature;
}

S_msrTimeSignature msrTimeSignature::createTwoQuartersTime (
  int inputLineNumber)
{
  // create the time
  S_msrTimeSignature
    timeSignature =
      msrTimeSignature::create (
        inputLineNumber,
        gNullMeasureSmartPointer, // set later in setMeasureElementUpLinkToMeasure()
        msrTimeSignatureSymbolKind::kTimeSignatureSymbolNone);

  // create a four quarters time signature item
  S_msrTimeSignatureItem
    timeSignatureItem =
      msrTimeSignatureItem::create (
        inputLineNumber);

  timeSignatureItem->
    appendBeatsNumber (2);
  timeSignatureItem->
    setTimeSignatureBeatValue (4);

  // append the time signature item to the timeSignature
  timeSignature->
    appendTimeSignatureItem (timeSignatureItem);

  // return the timeSignature
  return timeSignature;
}

S_msrTimeSignature msrTimeSignature::createThreeQuartersTime (
  int inputLineNumber)
{
  // create the time
  S_msrTimeSignature
    timeSignature =
      msrTimeSignature::create (
        inputLineNumber,
        gNullMeasureSmartPointer, // set later in setMeasureElementUpLinkToMeasure()
        msrTimeSignatureSymbolKind::kTimeSignatureSymbolNone);

  // create a four quarters time signature item
  S_msrTimeSignatureItem
    timeSignatureItem =
      msrTimeSignatureItem::create (
        inputLineNumber);

  timeSignatureItem->
    appendBeatsNumber (3);
  timeSignatureItem->
    setTimeSignatureBeatValue (4);

  // append the time signature item to the timeSignature
  timeSignature->
    appendTimeSignatureItem (timeSignatureItem);

  // return the timeSignature
  return timeSignature;
}

S_msrTimeSignature msrTimeSignature::createFourQuartersTime (
  int inputLineNumber)
{
  // create the time
  S_msrTimeSignature
    timeSignature =
      msrTimeSignature::create (
        inputLineNumber,
        gNullMeasureSmartPointer, // set later in setMeasureElementUpLinkToMeasure()
        msrTimeSignatureSymbolKind::kTimeSignatureSymbolNone);

  // create a four quarters time signature item
  S_msrTimeSignatureItem
    timeSignatureItem =
      msrTimeSignatureItem::create (
        inputLineNumber);

  timeSignatureItem->
    appendBeatsNumber (4);
  timeSignatureItem->
    setTimeSignatureBeatValue (4);

  // append the time signature item to the timeSignature
  timeSignature->
    appendTimeSignatureItem (timeSignatureItem);

  // return the timeSignature
  return timeSignature;
}

S_msrTimeSignature msrTimeSignature::createFiveQuartersTime (
  int inputLineNumber)
{
  // create the time
  S_msrTimeSignature
    timeSignature =
      msrTimeSignature::create (
        inputLineNumber,
        gNullMeasureSmartPointer, // set later in setMeasureElementUpLinkToMeasure()
        msrTimeSignatureSymbolKind::kTimeSignatureSymbolNone);

  // create a four quarters time signature item
  S_msrTimeSignatureItem
    timeSignatureItem =
      msrTimeSignatureItem::create (
        inputLineNumber);

  timeSignatureItem->
    appendBeatsNumber (5);
  timeSignatureItem->
    setTimeSignatureBeatValue (4);

  // append the time signature item to the timeSignature
  timeSignature->
    appendTimeSignatureItem (timeSignatureItem);

  // return the timeSignature
  return timeSignature;
}

S_msrTimeSignature msrTimeSignature::createTwoHalvesTime (
  int inputLineNumber)
{
  // create the time
  S_msrTimeSignature
    timeSignature =
      msrTimeSignature::create (
        inputLineNumber,
        gNullMeasureSmartPointer, // set later in setMeasureElementUpLinkToMeasure()
        msrTimeSignatureSymbolKind::kTimeSignatureSymbolNone);

  // create a four quarters time signature item
  S_msrTimeSignatureItem
    timeSignatureItem =
      msrTimeSignatureItem::create (
        inputLineNumber);

  timeSignatureItem->
    appendBeatsNumber (2);
  timeSignatureItem->
    setTimeSignatureBeatValue (2);

  // append the time signature item to the timeSignature
  timeSignature->
    appendTimeSignatureItem (timeSignatureItem);

  // return the timeSignature
  return timeSignature;
}

S_msrTimeSignature msrTimeSignature::createThreeHalvesTime (
  int inputLineNumber)
{
  // create the time
  S_msrTimeSignature
    timeSignature =
      msrTimeSignature::create (
        inputLineNumber,
        gNullMeasureSmartPointer, // set later in setMeasureElementUpLinkToMeasure()
        msrTimeSignatureSymbolKind::kTimeSignatureSymbolNone);

  // create a four quarters time signature item
  S_msrTimeSignatureItem
    timeSignatureItem =
      msrTimeSignatureItem::create (
        inputLineNumber);

  timeSignatureItem->
    appendBeatsNumber (3);
  timeSignatureItem->
    setTimeSignatureBeatValue (2);

  // append the time signature item to the timeSignature
  timeSignature->
    appendTimeSignatureItem (timeSignatureItem);

  // return the timeSignature
  return timeSignature;
}

S_msrTimeSignature msrTimeSignature::createFourHalvesTime (
  int inputLineNumber)
{
  // create the time
  S_msrTimeSignature
    timeSignature =
      msrTimeSignature::create (
        inputLineNumber,
        gNullMeasureSmartPointer, // set later in setMeasureElementUpLinkToMeasure()
        msrTimeSignatureSymbolKind::kTimeSignatureSymbolNone);

  // create a four quarters time signature item
  S_msrTimeSignatureItem
    timeSignatureItem =
      msrTimeSignatureItem::create (
        inputLineNumber);

  timeSignatureItem->
    appendBeatsNumber (4);
  timeSignatureItem->
    setTimeSignatureBeatValue (2);

  // append the time signature item to the timeSignature
  timeSignature->
    appendTimeSignatureItem (timeSignatureItem);

  // return the timeSignature
  return timeSignature;
}

//________________________________________________________________________
S_msrTimeSignature msrTimeSignature::createTimeFromString (
  int           inputLineNumber,
  const std::string& timeString)
{
  /*
    Handles timeString à la LilyPond, such as 3/4
  */

  S_msrTimeSignature result;

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceNotes ()) {
    gLogStream <<
      "Creating time from std::string \"" <<
      timeString <<
      "', line " << inputLineNumber <<
      std::endl;
  }
#endif

  std::string regularExpression (
    "[[:space:]]*"
    "([[:digit:]]+)" // beatsNumber
    "[[:space:]]*"
    "/"
    "[[:space:]]*"
    "([[:digit:]]+)" // beatsDuration
    "[[:space:]]*"
    );

#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "regularExpression = " <<
      regularExpression <<
      std::endl;
  }
#endif

  std::regex  e (regularExpression);
  std::smatch sm;

  regex_match (timeString, sm, e);

  size_t smSize = sm.size ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "There are " << smSize << " matches" <<
      " for time std::string \"" << timeString <<
      "\" with std::regex \"" << regularExpression <<
      "\":" <<
      std::endl;

    ++gIndenter;

    for (unsigned i = 0; i < smSize; ++i) {
      gLogStream <<
        i << ": " << "\"" << sm [i] << "\"" <<
        std::endl;
    } // for
    gLogStream << std::endl;

    --gIndenter;
  }
#endif

  // handling timeString à la LilyPond, such as "3/4"

  if (smSize != 3) {
    std::stringstream s;

    s <<
      "timeString \"" << timeString <<
      "\" is ill-formed";

    oahError (s.str ());
  }

  std::string
    beatsNumber   = sm [1],
    beatsDuration = sm [2];

#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "beatsNumber = \"" <<
      beatsNumber <<
      "\"" <<
      std::endl <<

      "beatsDuration = \"" <<
      beatsDuration <<
      "\"" <<
      std::endl;
  }
#endif

    int integerBeatsNumber;
    {
      std::stringstream s;
      s << beatsNumber;
      s >> integerBeatsNumber;
    }

    int integerValue;
    {
      std::stringstream s;
      s << beatsDuration;
      s >> integerValue;
    }

#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "integerBeatsNumber = " <<
      integerBeatsNumber <<
      std::endl <<
      "integerValue = " <<
      integerValue <<
      std::endl;
  }
#endif

  // create the time signature
  result =
    msrTimeSignature::create (
      inputLineNumber,
        gNullMeasureSmartPointer, // set later in setMeasureElementUpLinkToMeasure()
      msrTimeSignatureSymbolKind::kTimeSignatureSymbolNone);

  // create a four quarters time signature item
  S_msrTimeSignatureItem
    timeSignatureItem =
      msrTimeSignatureItem::create (
        inputLineNumber);

  timeSignatureItem->
    appendBeatsNumber (integerBeatsNumber);
  timeSignatureItem->
    setTimeSignatureBeatValue (integerValue);

  // append the time signature item to the timeSignature
  result->
    appendTimeSignatureItem (timeSignatureItem);

  return result;
}

msrTimeSignature::~msrTimeSignature ()
{}

void msrTimeSignature::setMeasureElementMeasurePosition (
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasurePositions ()) {
    gLogStream <<
      "Setting time signature's position in measure of " <<
      asString () <<
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
      std::endl;
  }
#endif

  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    measurePosition != msrMoment::K_NO_POSITION,
    "measurePosition == msrMoment::K_NO_POSITION");

  // set time signature's position in measure
  fMeasureElementMeasurePosition = measurePosition;
}

void msrTimeSignature::appendTimeSignatureItem (
  S_msrTimeSignatureItem timeSignatureItem)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceTimeSignatures ()) {
    gLogStream <<
      "Append item '" <<
      timeSignatureItem->asString () <<
      "' to time '" <<
      "JMI" <<
      "'" <<
      std::endl;
    }
#endif

  if (! fTimeSignatureItemsVector.size ()) {
    // this is the first item inserted
    if (
      timeSignatureItem->
        getTimeSignatureBeatsNumbersVector (). size ()
        >
      1) {
      // this time is compound
      fTimeIsCompound = true;
    }
  }

  else {
    // there are several time signature items, this time is compound
    fTimeIsCompound = true;
  }

  // append the time signature item to the std::vector
  fTimeSignatureItemsVector.insert (
    fTimeSignatureItemsVector.end (), timeSignatureItem);
}

Rational msrTimeSignature::wholeNotesDurationPerMeasure () const
{
  Rational result (0, 1); // addition neutral element

  size_t vectorSize = fTimeSignatureItemsVector.size ();

  if (vectorSize) {
    /* JMI
    // start with first item
    result =
      Rational (
        fTimeSignatureItemsVector [0]->getTimeSignatureBeatsNumber (),
        fTimeSignatureItemsVector [0]->getTimeSignatureBeatValue ());
*/

/* JMI
    gLogStream <<
      std::endl << std::endl <<
      "result1 = " <<
      result.getNumerator () <<
      '/' <<
      result.getDenominator () <<
      std::endl << std::endl;
*/

    // iterate over the others
    for (size_t i = 0; i < vectorSize; ++i) {
      result +=
        Rational (
          fTimeSignatureItemsVector [i]->getTimeSignatureBeatsNumber (),
          fTimeSignatureItemsVector [i]->getTimeSignatureBeatValue ());

/* JMI
      gLogStream <<
        std::endl << std::endl <<
        "result2 = " <<
        result.getNumerator () <<
        '/' <<
        result.getDenominator () <<
        std::endl << std::endl;
        */

    } // for
  }

  else {
    msrInternalError (
      gGlobalServiceRunData->getInputSourceName (),
      fInputLineNumber,
      __FILE__, __LINE__,
      "time signature items std::vector is empty");
  }

  // return result
  return result;
}

void msrTimeSignature::acceptIn (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrTimeSignature::acceptIn ()" <<
      std::endl;
  }

  if (visitor<S_msrTimeSignature>*
    p =
      dynamic_cast<visitor<S_msrTimeSignature>*> (v)) {
        S_msrTimeSignature elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrTimeSignature::visitStart ()" <<
            std::endl;
        }
        p->visitStart (elem);
  }
}

void msrTimeSignature::acceptOut (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrTimeSignature::acceptOut ()" <<
      std::endl;
  }

  if (visitor<S_msrTimeSignature>*
    p =
      dynamic_cast<visitor<S_msrTimeSignature>*> (v)) {
        S_msrTimeSignature elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrTimeSignature::visitEnd ()" <<
            std::endl;
        }
        p->visitEnd (elem);
  }
}

void msrTimeSignature::browseData (basevisitor* v)
{}

std::string msrTimeSignature::asString () const
{
  std::stringstream s;

  s <<
    "[Time" <<
    ", timeSignatureSymbolKind: " <<
    msrTimeSignatureSymbolKindAsString (fTimeSignatureSymbolKind) <<
    ", fTimeIsCompound: " <<
    fTimeIsCompound <<
    ", fTimeSignatureItemsVector.size(): " <<
    mfSingularOrPlural (
      fTimeSignatureItemsVector.size (), "item", "items") <<
    mfSingularOrPlural (
      fTimeSignatureItemsVector.size (), "item", "items") <<
    ", line " << fInputLineNumber;

  if (fTimeSignatureItemsVector.size ()) {
    s <<
      ", ";

    std::vector<S_msrTimeSignatureItem>::const_iterator
      iBegin = fTimeSignatureItemsVector.begin (),
      iEnd   = fTimeSignatureItemsVector.end (),
      i      = iBegin;

    for ( ; ; ) {
      s << (*i)->asString ();
      if (++i == iEnd) break;
      s << ", ";
    } // for
  }
  else {
    if (fTimeSignatureSymbolKind != msrTimeSignatureSymbolKind::kTimeSignatureSymbolSenzaMisura) {
      msrInternalError (
        gGlobalServiceRunData->getInputSourceName (),
        fInputLineNumber,
        __FILE__, __LINE__,
        "time  items std::vector is empty");
    }
  }

  s << ']';

  return s.str ();
}

std::string msrTimeSignature::asShortString () const
{
  /* JMI
  std::stringstream s;

  s <<
    "TimeSignature, " <<
    ", timeSignatureSymbolKind: " <<
    msrTimeSignatureSymbolKindAsString (fTimeSignatureSymbolKind) <<
    ", fTimeIsCompound " <<
    fTimeIsCompound <<
    ", " <<
    mfSingularOrPlural (
      fTimeSignatureItemsVector.size (), "item", "items") <<
    ", line " << fInputLineNumber;

  return s.str ();
  */

  return asString ();
}

std::string msrTimeSignature::asShortStringForMeasuresSlices () const
{
  std::stringstream s;

  s <<
    '[';

/* JMI
    ", timeSignatureSymbolKind: " <<
    msrTimeSignatureSymbolKindAsString (fTimeSignatureSymbolKind) <<
    ", fTimeIsCompound: " <<
    fTimeIsCompound;
    ", fTimeSignatureItemsVector.size(): " <<
    mfSingularOrPlural (
      fTimeSignatureItemsVector.size (), "item", "items") <<
    mfSingularOrPlural (
      fTimeSignatureItemsVector.size (), "item", "items") <<
    ", line " << fInputLineNumber;
*/

  if (fTimeSignatureItemsVector.size ()) {
//     s <<
//       ", ";

    std::vector<S_msrTimeSignatureItem>::const_iterator
      iBegin = fTimeSignatureItemsVector.begin (),
      iEnd   = fTimeSignatureItemsVector.end (),
      i      = iBegin;

    for ( ; ; ) {
      S_msrTimeSignatureItem timeSignatureItem = (*i);

      s << timeSignatureItem->asShortStringForMeasuresSlices ();
      if (++i == iEnd) break;
      s << ", ";
    } // for
  }
  else {
    if (fTimeSignatureSymbolKind != msrTimeSignatureSymbolKind::kTimeSignatureSymbolSenzaMisura) {
      msrInternalError (
        gGlobalServiceRunData->getInputSourceName (),
        fInputLineNumber,
        __FILE__, __LINE__,
        "time  items std::vector is empty");
    }
  }

  s << ']';

  return s.str ();
}

void msrTimeSignature::print (std::ostream& os) const
{
  os <<
    "[Time" <<
    ", line "  << fInputLineNumber <<
    std::endl;

  ++gIndenter;

  const int fieldWidth = 31;

  os << std::left <<
    std::setw (fieldWidth) <<
    "timeSignatureSymbolKind" << " : " <<
    msrTimeSignatureSymbolKindAsString (fTimeSignatureSymbolKind) <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fTimeIsCompound" << " : " <<
    fTimeIsCompound <<
    std::endl <<
    std::setw (fieldWidth) <<
    "timeSignatureItemsVector.size()" << " : " <<
    mfSingularOrPlural (
      fTimeSignatureItemsVector.size (), "item", "items") <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fTimeSignatureItemsVector" << " : ";

  if (fTimeSignatureItemsVector.size ()) {
    os << std::endl;

    ++gIndenter;

    std::vector<S_msrTimeSignatureItem>::const_iterator
      iBegin = fTimeSignatureItemsVector.begin (),
      iEnd   = fTimeSignatureItemsVector.end (),
      i      = iBegin;

    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
 // JMI     os << std::endl;
    } // for

    --gIndenter;
  }

  else {
    os <<
      " [NONE]" <<
      std::endl;
  }

  --gIndenter;

  os << ']' << std::endl;
}

std::ostream& operator << (std::ostream& os, const S_msrTimeSignature& elt)
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
