/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <iomanip>      // setw()), set::precision(), ...
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

#include "mfServiceRunData.h"

#include "mfStringsHandling.h"

#include "oahEarlyOptions.h"

#include "msrTimeSignatures.h"

#include "oahOah.h"

#include "msrOah.h"


using namespace std;

namespace MusicFormats
{

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
      endl;
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
      endl;
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
      endl;
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
      endl;
  }

  if (visitor<S_msrTimeSignatureItem>*
    p =
      dynamic_cast<visitor<S_msrTimeSignatureItem>*> (v)) {
        S_msrTimeSignatureItem elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrTimeSignatureItem::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrTimeSignatureItem::acceptOut (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrTimeSignatureItem::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrTimeSignatureItem>*
    p =
      dynamic_cast<visitor<S_msrTimeSignatureItem>*> (v)) {
        S_msrTimeSignatureItem elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrTimeSignatureItem::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrTimeSignatureItem::browseData (basevisitor* v)
{}

string msrTimeSignatureItem::asString () const
{
  stringstream s;

  s <<
    "TimeSignatureItem ";

  size_t vectorSize =
    fTimeSignatureBeatsNumbersVector.size ();

  switch (vectorSize) {
    case 0:
    /* JMI
      msrInternalError (
        gGlobalServiceRunData->getInputSourceName (),
        fInputLineNumber,
        __FILE__, __LINE__,
        "time signature item beats numbers vector is empty");
        */
      s <<
        "beats numbers: none";
      break;

    case 1:
      s <<
        fTimeSignatureBeatsNumbersVector [0] << "/" << fTimeSignatureBeatValue;
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
    ", line " << fInputLineNumber;

  return s.str ();
}

string msrTimeSignatureItem::asShortStringForMeasuresSlices () const
{
  stringstream s;

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
        "time signature item beats numbers vector is empty");
        */
      s <<
        "beats numbers: none";
      break;

    case 1:
      s <<
        fTimeSignatureBeatsNumbersVector [0] << "/" << fTimeSignatureBeatValue;
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

void msrTimeSignatureItem::print (ostream& os) const
{
  os << asString () << endl;
}

ostream& operator<< (ostream& os, const S_msrTimeSignatureItem& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_msrTimeSignature msrTimeSignature::create (
  int                        inputLineNumber,
  msrTimeSignatureSymbolKind timeSignatureSymbolKind)
{
  msrTimeSignature* o =
    new msrTimeSignature (
      inputLineNumber, timeSignatureSymbolKind);
  assert (o != nullptr);
  return o;
}

msrTimeSignature::msrTimeSignature (
  int                        inputLineNumber,
  msrTimeSignatureSymbolKind timeSignatureSymbolKind)
    : msrMeasureElement (inputLineNumber)
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
  const string& timeString)
{
  /*
    Handles timeString à la LilyPond, such as 3/4
  */

  S_msrTimeSignature result;

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceNotes ()) {
    gLogStream <<
      "Creating time from string \"" <<
      timeString <<
      "', line " << inputLineNumber <<
      endl;
  }
#endif

  string regularExpression (
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
      endl;
  }
#endif

  regex  e (regularExpression);
  smatch sm;

  regex_match (timeString, sm, e);

  size_t smSize = sm.size ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "There are " << smSize << " matches" <<
      " for time string \"" << timeString <<
      "\" with regex \"" << regularExpression <<
      "\":" <<
      endl;

    ++gIndenter;

    for (unsigned i = 0; i < smSize; ++i) {
      gLogStream <<
        i << ": " << "\"" << sm [i] << "\"" <<
        endl;
    } // for
    gLogStream << endl;

    --gIndenter;
  }
#endif

  // handling timeString à la LilyPond, such as "3/4"

  if (smSize != 3) {
    stringstream s;

    s <<
      "timeString \"" << timeString <<
      "\" is ill-formed";

    oahError (s.str ());
  }

  string
    beatsNumber   = sm [1],
    beatsDuration = sm [2];

#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "beatsNumber = \"" <<
      beatsNumber <<
      "\"" <<
      endl <<

      "beatsDuration = \"" <<
      beatsDuration <<
      "\"" <<
      endl;
  }
#endif

    int integerBeatsNumber;
    {
      stringstream s;
      s << beatsNumber;
      s >> integerBeatsNumber;
    }

    int integerValue;
    {
      stringstream s;
      s << beatsDuration;
      s >> integerValue;
    }

#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "integerBeatsNumber = " <<
      integerBeatsNumber <<
      endl <<
      "integerValue = " <<
      integerValue <<
      endl;
  }
#endif

  // create the time
  result =
    msrTimeSignature::create (
      inputLineNumber,
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
      endl;
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

  // append the time signature item to the vector
  fTimeSignatureItemsVector.insert (
    fTimeSignatureItemsVector.end (), timeSignatureItem);
}

rational msrTimeSignature::wholeNotesPerMeasure () const
{
  rational result (0, 1); // addition neutral element

  size_t vectorSize = fTimeSignatureItemsVector.size ();

  if (vectorSize) {
    /* JMI
    // start with first item
    result =
      rational (
        fTimeSignatureItemsVector [0]->getTimeSignatureBeatsNumber (),
        fTimeSignatureItemsVector [0]->getTimeSignatureBeatValue ());
*/

/* JMI
    gLogStream <<
      endl << endl <<
      "result1 = " <<
      result.getNumerator () <<
      "/" <<
      result.getDenominator () <<
      endl << endl;
*/

    // iterate over the others
    for (size_t i = 0; i < vectorSize; ++i) {
      result +=
        rational (
          fTimeSignatureItemsVector [i]->getTimeSignatureBeatsNumber (),
          fTimeSignatureItemsVector [i]->getTimeSignatureBeatValue ());

/* JMI
      gLogStream <<
        endl << endl <<
        "result2 = " <<
        result.getNumerator () <<
        "/" <<
        result.getDenominator () <<
        endl << endl;
        */

    } // for
  }

  else {
    msrInternalError (
      gGlobalServiceRunData->getInputSourceName (),
      fInputLineNumber,
      __FILE__, __LINE__,
      "time signature items vector is empty");
  }

  // rationalize result
  result.rationalise ();

  // return it
  return result;
}

void msrTimeSignature::acceptIn (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrTimeSignature::acceptIn ()" <<
      endl;
  }

  if (visitor<S_msrTimeSignature>*
    p =
      dynamic_cast<visitor<S_msrTimeSignature>*> (v)) {
        S_msrTimeSignature elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrTimeSignature::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrTimeSignature::acceptOut (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrTimeSignature::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrTimeSignature>*
    p =
      dynamic_cast<visitor<S_msrTimeSignature>*> (v)) {
        S_msrTimeSignature elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrTimeSignature::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrTimeSignature::browseData (basevisitor* v)
{}

string msrTimeSignature::asString () const
{
  stringstream s;

  s <<
    "[Time " <<
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

    vector<S_msrTimeSignatureItem>::const_iterator
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
        "time  items vector is empty");
    }
  }

  s << "]";

  return s.str ();
}

string msrTimeSignature::asShortString () const
{
  /* JMI
  stringstream s;

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

string msrTimeSignature::asShortStringForMeasuresSlices () const
{
  stringstream s;

  s <<
    "[";

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

    vector<S_msrTimeSignatureItem>::const_iterator
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
        "time  items vector is empty");
    }
  }

  s << "]";

  return s.str ();
}

void msrTimeSignature::print (ostream& os) const
{
  os <<
    "[Time" <<
    ", line "  << fInputLineNumber <<
    endl;

  ++gIndenter;

  const int fieldWidth = 23;

  os << left <<
    setw (fieldWidth) <<
    "timeSignatureSymbolKind" << " : " <<
    msrTimeSignatureSymbolKindAsString (fTimeSignatureSymbolKind) <<
    endl <<
    setw (fieldWidth) <<
    "fTimeIsCompound" << " : " <<
    fTimeIsCompound <<
    endl <<
    setw (fieldWidth) <<
    "timeSignatureItemsVector.size()" << " : " <<
    mfSingularOrPlural (
      fTimeSignatureItemsVector.size (), "item", "items") <<
    endl <<
    setw (fieldWidth) <<
    "fTimeSignatureItemsVector" << " : ";

  if (fTimeSignatureItemsVector.size ()) {
    os << endl;

    ++gIndenter;

    vector<S_msrTimeSignatureItem>::const_iterator
      iBegin = fTimeSignatureItemsVector.begin (),
      iEnd   = fTimeSignatureItemsVector.end (),
      i      = iBegin;

    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
 // JMI     os << endl;
    } // for

    --gIndenter;
  }

  else {
    os <<
      " none" <<
      endl;
  }

  --gIndenter;

  os << ']' << endl;
}

ostream& operator<< (ostream& os, const S_msrTimeSignature& elt)
{
  elt->print (os);
  return os;
}


}
