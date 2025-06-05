/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <iomanip>      // setw()), set::precision(), ...
#include <iostream>
#include <sstream>
#include <climits>      // INT_MIN, INT_MAX

#include <regex>

#include "visitor.h"

#include "mfPreprocessorSettings.h"

#include "mfAssert.h"
#include "mfServices.h"
#include "mfStringsHandling.h"

#include "oahEarlyOptions.h"

#include "msrMeasureConstants.h"

#include "msrClefKeyTimeSignatureGroups.h"

#include "oahOah.h"

#include "msrOah.h"

#include "oahWae.h"
#include "msrWae.h"

#include "waeHandlers.h"


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
  const mfInputLineNumber& inputLineNumber)
{
  msrTimeSignatureItem* obj =
    new msrTimeSignatureItem (
      inputLineNumber);
  assert (obj != nullptr);
  return obj;
}

msrTimeSignatureItem::msrTimeSignatureItem (
  const mfInputLineNumber& inputLineNumber)
    : msrElement (inputLineNumber)
{
  fTimeSignatureBeatValue = -1;

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceTimeSignatures ()) {
    std::stringstream ss;

    ss <<
      "Creating time signature item" <<
      ", line: " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

msrTimeSignatureItem::~msrTimeSignatureItem ()
{}

Bool msrTimeSignatureItem::isEqualTo (
  const S_msrTimeSignatureItem& otherTimeSignatureItem) const
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
        fTimeSignatureBeatsNumbersVector.at (i)
          ==
        otherTimeSignatureItem->fTimeSignatureBeatsNumbersVector.at (i)
        )
      ) {
      return false;
    }
  } // for

  return true;
}

void msrTimeSignatureItem::appendBeatsNumber (int beatsNumber)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceTimeSignatures ()) {
    std::stringstream ss;

    ss <<
      "Appending beat number '" <<
      beatsNumber <<
      "' to time signature item '" <<
      asString () <<
      '\'' <<
      std::endl;
    }
#endif // MF_TRACE_IS_ENABLED

  fTimeSignatureBeatsNumbersVector.insert (
    fTimeSignatureBeatsNumbersVector.end (),
    beatsNumber);
}

void msrTimeSignatureItem::setTimeSignatureBeatValue (int timeSignatureBeatValue)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceTimeSignatures ()) {
    std::stringstream ss;

    ss <<
      "Setting beat value to '" <<
      timeSignatureBeatValue <<
      "' in time signature item '" <<
      asString () <<
      '\'' <<
      std::endl;
    }
#endif // MF_TRACE_IS_ENABLED

  fTimeSignatureBeatValue = timeSignatureBeatValue;
}

int msrTimeSignatureItem::getTimeSignatureBeatsNumber () const
{
  int result = 0;

  for (size_t i = 0; i < fTimeSignatureBeatsNumbersVector.size (); ++i) {
    result +=
      fTimeSignatureBeatsNumbersVector.at (i);
    } // for

  return result;
}

void msrTimeSignatureItem::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrTimeSignatureItem::acceptIn ()";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msrTimeSignatureItem>*
    p =
      dynamic_cast<visitor<S_msrTimeSignatureItem>*> (v)) {
        S_msrTimeSignatureItem elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrTimeSignatureItem::visitStart ()";

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void msrTimeSignatureItem::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrTimeSignatureItem::acceptOut ()";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msrTimeSignatureItem>*
    p =
      dynamic_cast<visitor<S_msrTimeSignatureItem>*> (v)) {
        S_msrTimeSignatureItem elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrTimeSignatureItem::visitEnd ()";

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void msrTimeSignatureItem::browseData (basevisitor* v)
{}

std::string msrTimeSignatureItem::asString () const
{
  std::stringstream ss;

  ss <<
    "[TimeSignatureItem ";

  size_t vectorSize =
    fTimeSignatureBeatsNumbersVector.size ();

  switch (vectorSize) {
    case 0:
    /* JMI
      msrInternalError (
        gServiceRunData->getInputSourceName (),
        fInputLineNumber,
        __FILE__, __LINE__,
        "time signature item beats numbers vector is empty");
        */
      ss <<
        "beats numbers: [NONE]";
      break;

    case 1:
      ss <<
       fTimeSignatureBeatsNumbersVector.at (0) << '/' << fTimeSignatureBeatValue;
      break;

    default:
      ss <<
        "beats numbers: ";

      for (size_t i = 0; i < vectorSize; ++i) {
        ss <<
          fTimeSignatureBeatsNumbersVector.at (i);

        if (i != vectorSize - 1) {
          ss <<
            ' ';
        }
      } // for

      ss <<
        ", beat value: " << fTimeSignatureBeatValue;
  } // switch

  ss <<
    ", line " << fInputLineNumber <<
    ']';

  return ss.str ();
}

// std::string msrTimeSignatureItem::asStringForMeasuresSlices () const
// {
//   std::stringstream ss;
//
// //   ss <<
// //     "TimeSignatureItem ";
//
//   size_t vectorSize =
//     fTimeSignatureBeatsNumbersVector.size ();
//
//   switch (vectorSize) {
//     case 0:
//     /* JMI
//       msrInternalError (
//         gServiceRunData->getInputSourceName (),
//         fInputLineNumber,
//         __FILE__, __LINE__,
//         "time signature item beats numbers vector is empty");
//         */
//       ss <<
//         "beats numbers: [NONE]";
//       break;
//
//     case 1:
//       ss <<
//        fTimeSignatureBeatsNumbersVector.at (0) << '/' << fTimeSignatureBeatValue;
//       break;
//
//     default:
//       ss <<
//         "beats numbers: ";
//
//       for (size_t i = 0; i < vectorSize; ++i) {
//         ss <<
//           fTimeSignatureBeatsNumbersVector.at (i);
//
//         if (i != vectorSize - 1) {
//           ss <<
//             ' ';
//         }
//       } // for
//
//       ss <<
//         ", beat value: " << fTimeSignatureBeatValue;
//   } // switch
//
// /* JMI
//   ss <<
//     ", line " << fInputLineNumber;
// */
//
//   return ss.str ();
// }

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
    os << "[NULL]" << std::endl;
  }

  return os;
}

//______________________________________________________________________________
S_msrTimeSignature msrTimeSignature::create (
  const mfInputLineNumber& inputLineNumber,
  const S_msrMeasure&        upLinkToMeasure,
  msrTimeSignatureSymbolKind timeSignatureSymbolKind)
{
  msrTimeSignature* obj =
    new msrTimeSignature (
      inputLineNumber,
      upLinkToMeasure,
      timeSignatureSymbolKind);
  assert (obj != nullptr);
  return obj;
}

S_msrTimeSignature msrTimeSignature::create (
  const mfInputLineNumber& inputLineNumber,
  msrTimeSignatureSymbolKind timeSignatureSymbolKind)
{
  return
    msrTimeSignature::create (
      inputLineNumber,
      gNullMeasure, // set later in setMeasureElementUpLinkToMeasure()
      timeSignatureSymbolKind);
}

msrTimeSignature::msrTimeSignature (
  const mfInputLineNumber& inputLineNumber,
  const S_msrMeasure&        upLinkToMeasure,
  msrTimeSignatureSymbolKind timeSignatureSymbolKind)
    : msrMeasureElement (
      inputLineNumber)
{
  fMeasureElementUpLinkToMeasure = upLinkToMeasure;

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
        fTimeSignatureItemsVector.at (i)->isEqualTo (
          otherTimeSignature->fTimeSignatureItemsVector.at (i))
        )
      ) {
      return false;
    }
  } // for

  return true;
}

S_msrTimeSignature msrTimeSignature::createTwoEightsTime (
  const mfInputLineNumber& inputLineNumber)
{
  // create the time
  S_msrTimeSignature
    timeSignature =
      msrTimeSignature::create (
        inputLineNumber,
        gNullMeasure, // set later in setMeasureElementUpLinkToMeasure()
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
  const mfInputLineNumber& inputLineNumber)
{
  // create the time
  S_msrTimeSignature
    timeSignature =
      msrTimeSignature::create (
        inputLineNumber,
        gNullMeasure, // set later in setMeasureElementUpLinkToMeasure()
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
  const mfInputLineNumber& inputLineNumber)
{
  // create the time
  S_msrTimeSignature
    timeSignature =
      msrTimeSignature::create (
        inputLineNumber,
        gNullMeasure, // set later in setMeasureElementUpLinkToMeasure()
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
  const mfInputLineNumber& inputLineNumber)
{
  // create the time
  S_msrTimeSignature
    timeSignature =
      msrTimeSignature::create (
        inputLineNumber,
        gNullMeasure, // set later in setMeasureElementUpLinkToMeasure()
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
  const mfInputLineNumber& inputLineNumber)
{
  // create the time
  S_msrTimeSignature
    timeSignature =
      msrTimeSignature::create (
        inputLineNumber,
        gNullMeasure, // set later in setMeasureElementUpLinkToMeasure()
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
  const mfInputLineNumber& inputLineNumber)
{
  // create the time
  S_msrTimeSignature
    timeSignature =
      msrTimeSignature::create (
        inputLineNumber,
        gNullMeasure, // set later in setMeasureElementUpLinkToMeasure()
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
  const mfInputLineNumber& inputLineNumber)
{
  // create the time
  S_msrTimeSignature
    timeSignature =
      msrTimeSignature::create (
        inputLineNumber,
        gNullMeasure, // set later in setMeasureElementUpLinkToMeasure()
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
  const mfInputLineNumber& inputLineNumber)
{
  // create the time
  S_msrTimeSignature
    timeSignature =
      msrTimeSignature::create (
        inputLineNumber,
        gNullMeasure, // set later in setMeasureElementUpLinkToMeasure()
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
  const mfInputLineNumber& inputLineNumber)
{
  // create the time
  S_msrTimeSignature
    timeSignature =
      msrTimeSignature::create (
        inputLineNumber,
        gNullMeasure, // set later in setMeasureElementUpLinkToMeasure()
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
  const mfInputLineNumber& inputLineNumber)
{
  // create the time
  S_msrTimeSignature
    timeSignature =
      msrTimeSignature::create (
        inputLineNumber,
        gNullMeasure, // set later in setMeasureElementUpLinkToMeasure()
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
  const mfInputLineNumber& inputLineNumber,
  const std::string& timeString)
{
  /*
    Handles timeString à la LilyPond, such as 3/4
  */

  S_msrTimeSignature result;

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceNotes ()) {
    std::stringstream ss;

    ss <<
      "Creating time from string \"" <<
      timeString <<
      "', line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  std::string regularExpression (
    "[[:space:]]*"
    "([[:digit:]]+)" // beatsNumber
    "[[:space:]]*"
    "/"
    "[[:space:]]*"
    "([[:digit:]]+)" // beatsNotesDuration
    "[[:space:]]*"
    );

#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "regularExpression: " <<
      regularExpression;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  std::regex  e (regularExpression);
  std::smatch sm;

  regex_match (timeString, sm, e);

  size_t smSize = sm.size ();

#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "There are " << smSize << " matches" <<
      " for time string \"" << timeString <<
      "\" with std::regex \"" << regularExpression <<
      "\":" <<
      std::endl;

    ++gIndenter;

    for (unsigned i = 0; i < smSize; ++i) {
      gLog <<
        i << ": " << "\"" << sm [i] << "\"" <<
        std::endl;
    } // for
    gLog << std::endl;

    --gIndenter;
  }
#endif // MF_TRACE_IS_ENABLED

  // handling timeString à la LilyPond, such as "3/4"

  if (smSize != 3) {
    std::stringstream ss;

    ss <<
      "timeString \"" << timeString <<
      "\" is ill-formed";

    oahError (ss.str ());
  }

  std::string
    beatsNumber = sm [1],
    beatsNotesDuration = sm [2];

#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "beatsNumber: \"" <<
      beatsNumber <<
      "\"" <<
      std::endl <<

      "beatsNotesDuration: \"" <<
      beatsNotesDuration <<
      "\"";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

    int integerBeatsNumber;
    {
      std::stringstream ss;
      ss << beatsNumber;
      ss >> integerBeatsNumber;
    }

    int integerValue;
    {
      std::stringstream ss;
      ss << beatsNotesDuration;
      ss >> integerValue;
    }

#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "integerBeatsNumber: " <<
      integerBeatsNumber <<
      std::endl <<
      "integerValue: " <<
      integerValue;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // create the time signature
  result =
    msrTimeSignature::create (
      inputLineNumber,
        gNullMeasure, // set later in setMeasureElementUpLinkToMeasure()
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

// void msrTimeSignature::setTimeSignatureUpLinkToMeasure (
//   const S_msrMeasure& measure)
// {
// #ifdef MF_SANITY_CHECKS_ARE_ENABLED
//   // sanity check
//   mfAssert (
//     __FILE__, __LINE__,
//     measure != nullptr,
//     "measure is NULL");
// #endif // MF_SANITY_CHECKS_ARE_ENABLED
//
// #ifdef MF_TRACE_IS_ENABLED
//   if (gTraceOahGroup->getTraceTimeSignatures ()) {
//     ++gIndenter;
//
//     gLog <<
//       "Setting the uplink to measure of time signature " <<
//       asString () <<
//       " to measure " << measure->asString () <<
//       "' in measure '" <<
//       measure->asString () <<
//       std::endl;
//
//     --gIndenter;
//   }
// #endif // MF_TRACE_IS_ENABLED
//
//   fTimeSignatureUpLinkToMeasure = measure;
// }

void msrTimeSignature::appendTimeSignatureItem (
  const S_msrTimeSignatureItem& timeSignatureItem)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceTimeSignatures ()) {
    std::stringstream ss;

    ss <<
      "Append item '" <<
      timeSignatureItem->asString () <<
      "' to time '" <<
      "JMI" <<
      '\'' <<
      std::endl;
    }
#endif // MF_TRACE_IS_ENABLED

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

  // append the time signature item to the vector
  fTimeSignatureItemsVector.insert (
    fTimeSignatureItemsVector.end (), timeSignatureItem);
}

mfWholeNotes msrTimeSignature::timeSignatureWholeNotesPerMeasure () const
{
  mfWholeNotes result (0, 1); // addition neutral element

  switch (fTimeSignatureSymbolKind) {
    case msrTimeSignatureSymbolKind::kTimeSignatureSymbolSenzaMisura:
      // senza misura, no measure length limitation JMI ??? 0.9.72
      result = mfWholeNotes (INT_MAX, 1);
      break;

    default:
      // regular time signature
      {
        size_t vectorSize = fTimeSignatureItemsVector.size ();

      #ifdef MF_TRACE_IS_ENABLED
          if (gTraceOahGroup->getTraceTimeSignatures ()) {
            gLog <<
              "--> timeSignatureWholeNotesPerMeasure() 1 for " <<
              asString () <<
              ", line " << fInputLineNumber <<
            std::endl;
          }
      #endif // MF_TRACE_IS_ENABLED

        if (vectorSize) {
          /* JMI
          // start with first item
          result =
            mfWholeNotes (
              fTimeSignatureItems.at (0)->getTimeSignatureBeatsNumber (),
              fTimeSignatureItems.at (0)->getTimeSignatureBeatValue ());
      */

      /* JMI
          gLog <<
            std::endl << std::endl <<
            "result1: " <<
            result.getNumerator () <<
            '/' <<
            result.getDenominator () <<
            std::endl << std::endl;
      */

          // iterate over the others
          for (size_t i = 0; i < vectorSize; ++i) {
            result +=
              mfWholeNotes (
                fTimeSignatureItemsVector.at (i)->getTimeSignatureBeatsNumber (),
                fTimeSignatureItemsVector.at (i)->getTimeSignatureBeatValue ());

      /* JMI
            gLog <<
              std::endl << std::endl <<
              "result2: " <<
              result.getNumerator () <<
              '/' <<
              result.getDenominator () <<
              std::endl << std::endl;
              */

          } // for
        }

        else {
          msrInternalError (
            gServiceRunData->getInputSourceName (),
            fInputLineNumber,
            __FILE__, __LINE__,
            "time signature items vector is empty");
        }
      }
  } // switch

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceTimeSignatures ()) {
    gLog <<
      "--> timeSignatureWholeNotesPerMeasure() 2, result = " <<
      result <<
      ", line " << fInputLineNumber <<
    std::endl;
  }
#endif // MF_TRACE_IS_ENABLED

  // return result
  return result;
}

void msrTimeSignature::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrTimeSignature::acceptIn ()";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msrTimeSignature>*
    p =
      dynamic_cast<visitor<S_msrTimeSignature>*> (v)) {
        S_msrTimeSignature elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrTimeSignature::visitStart ()";

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void msrTimeSignature::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrTimeSignature::acceptOut ()";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msrTimeSignature>*
    p =
      dynamic_cast<visitor<S_msrTimeSignature>*> (v)) {
        S_msrTimeSignature elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrTimeSignature::visitEnd ()";

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void msrTimeSignature::browseData (basevisitor* v)
{}

std::string msrTimeSignature::asString () const
{
  std::stringstream ss;

  ss <<
    "[TimeSignature" <<
    ", fTimeSignatureSymbolKind: " <<
    fTimeSignatureSymbolKind;

  switch (fTimeSignatureSymbolKind) {
    case msrTimeSignatureSymbolKind::kTimeSignatureSymbolSenzaMisura:
      break;

    default:
      // regular time signature
      {
        ss <<
          ", fTimeIsCompound: " <<
          fTimeIsCompound <<
          ", fTimeSignatureItemsVector.size(): " <<
          mfSingularOrPlural (
            fTimeSignatureItemsVector.size (), "item", "items");

        if (fTimeSignatureItemsVector.size ()) {
          ss << ", ";

          std::vector <S_msrTimeSignatureItem>::const_iterator
            iBegin = fTimeSignatureItemsVector.begin (),
            iEnd   = fTimeSignatureItemsVector.end (),
            i      = iBegin;

          for ( ; ; ) {
            ss << (*i)->asString ();
            if (++i == iEnd) break;
            ss << ", ";
          } // for
        }
        else {
          msrInternalError (
            gServiceRunData->getInputSourceName (),
            fInputLineNumber,
            __FILE__, __LINE__,
            "time  items vector is empty");
        }
      }
  } // switch

  ss <<
    ", line " << fInputLineNumber <<
    ']';

  return ss.str ();
}

std::string msrTimeSignature::asShortString () const
{
  /* JMI
  std::stringstream ss;

  ss <<
    "TimeSignature, " <<
    ", fTimeSignatureSymbolKind: " <<
    fTimeSignatureSymbolKind <<
    ", fTimeIsCompound " <<
    fTimeIsCompound <<
    ", " <<
    mfSingularOrPlural (
      fTimeSignatureItemsVector.size (), "item", "items") <<
    ", line " << fInputLineNumber;

  return ss.str ();
  */

  return asString ();
}

// std::string msrTimeSignature::asStringForMeasuresSlices () const
// {
//   std::stringstream ss;
//
//   ss <<
//     '[';
//
// /* JMI
//     ", fTimeSignatureSymbolKind: " <<
//     fTimeSignatureSymbolKind <<
//     ", fTimeIsCompound: " <<
//     fTimeIsCompound;
//     ", fTimeSignatureItemsVector.size(): " <<
//     mfSingularOrPlural (
//       fTimeSignatureItemsVector.size (), "item", "items") <<
//     mfSingularOrPlural (
//       fTimeSignatureItemsVector.size (), "item", "items") <<
//     ", line " << fInputLineNumber;
// */
//
//   if (fTimeSignatureItemsVector.size ()) {
// //     ss <<
// //       ", ";
//
//     std::vector <S_msrTimeSignatureItem>::const_iterator
//       iBegin = fTimeSignatureItemsVector.begin (),
//       iEnd   = fTimeSignatureItemsVector.end (),
//       i      = iBegin;
//
//     for ( ; ; ) {
//       S_msrTimeSignatureItem timeSignatureItem = (*i);
//
//       ss << timeSignatureItem->asStringForMeasuresSlices ();
//       if (++i == iEnd) break;
//       ss << ", ";
//     } // for
//   }
//   else {
//     if (
//       fTimeSignatureSymbolKind != msrTimeSignatureSymbolKind::kTimeSignatureSymbolSenzaMisura
//     ) {
//       msrInternalError (
//         gServiceRunData->getInputSourceName (),
//         fInputLineNumber,
//         __FILE__, __LINE__,
//         "time  items vector is empty");
//     }
//   }
//
//   ss << ']';
//
//   return ss.str ();
// }

void msrTimeSignature::print (std::ostream& os) const
{
  os <<
    "[TimeSignature" <<
    ", line "  << fInputLineNumber <<
    std::endl;

  ++gIndenter;

  constexpr int fieldWidth = 33;

  os << std::left <<
    std::setw (fieldWidth) <<
    "fTimeSignatureSymbolKind" << ": " <<
    fTimeSignatureSymbolKind <<
    std::endl;

  switch (fTimeSignatureSymbolKind) {
    case msrTimeSignatureSymbolKind::kTimeSignatureSymbolSenzaMisura:
      break;

    default:
      // regular time signature
      {
        os <<
          std::setw (fieldWidth) <<
          "fTimeIsCompound" << ": " <<
          fTimeIsCompound <<
          std::endl <<
          std::setw (fieldWidth) <<
          "fTimeSignatureItemsVector.size()" << ": " <<
          mfSingularOrPlural (
            fTimeSignatureItemsVector.size (), "item", "items") <<
          std::endl <<
          std::setw (fieldWidth) <<
          "fTimeSignatureItemsVector" << ": ";

        if (fTimeSignatureItemsVector.size ()) {
          os << std::endl;

          ++gIndenter;

          for (S_msrTimeSignatureItem timeSignatureItem : fTimeSignatureItemsVector ) {
            os << timeSignatureItem;
          } // for

          --gIndenter;
        }
        else {
          os <<
            " [EMPTY]" <<
            std::endl;
        }
      }
  } // switch

  --gIndenter;

  os << ']' << std::endl;
}

std::ostream& operator << (std::ostream& os, const S_msrTimeSignature& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NULL]" << std::endl;
  }

  return os;
}

EXP mfIndentedStringStream& operator << (
  mfIndentedStringStream& iss, const S_msrTimeSignature& elt)
{
  iss << elt->asString ();
  return iss;
}


}
