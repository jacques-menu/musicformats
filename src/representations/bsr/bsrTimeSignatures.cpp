/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <iostream>
#include <iomanip>      // setw()), set::precision(), ...
#include <sstream>

#include "visitor.h"

#include "enableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "tracingOah.h"
#endif

#include "mfServiceRunData.h"
#include "mfStringsHandling.h"

#include "bsrWae.h"

#include "bsrTimeSignatures.h"

#include "bsrNotes.h"
#include "bsrNumbers.h"

#include "oahOah.h"

#include "bsrOah.h"


using namespace std;

namespace MusicFormats
{

//______________________________________________________________________________
S_bsrTimeSignatureItem bsrTimeSignatureItem::create (
  int inputLineNumber)
{
  bsrTimeSignatureItem* o =
    new bsrTimeSignatureItem (
      inputLineNumber);
  assert (o != nullptr);

  return o;
}

bsrTimeSignatureItem::bsrTimeSignatureItem (
  int inputLineNumber)
    : bsrElement (inputLineNumber)
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

bsrTimeSignatureItem::~bsrTimeSignatureItem ()
{}

Bool bsrTimeSignatureItem::isEqualTo (S_bsrTimeSignatureItem otherTimeSignatureItem) const
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

  for (unsigned int i = 0; i < fTimeSignatureBeatsNumbersVector.size (); ++i) {
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

void bsrTimeSignatureItem::appendBeatsNumber (int beatsNumber)
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

void bsrTimeSignatureItem::setTimeSignatureBeatValue (int timeSignatureBeatValue)
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

int bsrTimeSignatureItem::getTimeSignatureBeatsNumber () const
{
  int result = 0;

  for (unsigned int i = 0; i < fTimeSignatureBeatsNumbersVector.size (); ++i) {
    result +=
      fTimeSignatureBeatsNumbersVector [i];
  } // for

  return result;
}

void bsrTimeSignatureItem::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
    gLogStream <<
      "% ==> bsrTimeSignatureItem::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_bsrTimeSignatureItem>*
    p =
      dynamic_cast<visitor<S_bsrTimeSignatureItem>*> (v)) {
        S_bsrTimeSignatureItem elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
          gLogStream <<
            "% ==> Launching bsrTimeSignatureItem::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void bsrTimeSignatureItem::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
    gLogStream <<
      "% ==> bsrTimeSignatureItem::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_bsrTimeSignatureItem>*
    p =
      dynamic_cast<visitor<S_bsrTimeSignatureItem>*> (v)) {
        S_bsrTimeSignatureItem elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
          gLogStream <<
            "% ==> Launching bsrTimeSignatureItem::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void bsrTimeSignatureItem::browseData (basevisitor* v)
{}

string bsrTimeSignatureItem::asString () const
{
  stringstream s;

  unsigned int timeSignatureBeatsNumbersVectorSize =
    fTimeSignatureBeatsNumbersVector.size ();

  s <<
    "TimeSignatureItem " <<
    ", " <<
    mfSingularOrPlural (
      timeSignatureBeatsNumbersVectorSize, "timeSignatureBeatNumber", "timeSignatureBeatNumbers");

  switch (timeSignatureBeatsNumbersVectorSize) {
    case 0:
    /* JMI
      msrInternalError (
        gGlobalServiceRunData->getInputSourceName (),
        fInputLineNumber,
        __FILE__, __LINE__,
        "time signature item beats numbers vector is empty");
        */
      break;

    case 1:
      s <<
        ", " <<
        fTimeSignatureBeatsNumbersVector [0] << "/" << fTimeSignatureBeatValue;
      break;

    default:
      s <<
        ", beats numbers: ";

      for (unsigned int i = 0; i < timeSignatureBeatsNumbersVectorSize; ++i) {
        s <<
          fTimeSignatureBeatsNumbersVector [i];

        if (i != timeSignatureBeatsNumbersVectorSize - 1) {
          s <<
            " ";
        }
      } // for

      s <<
        ", beat value: " << fTimeSignatureBeatValue;
  } // switch

  s <<
    ", line " << fInputLineNumber;

  return s.str ();
}

void bsrTimeSignatureItem::print (ostream& os) const
{
  os << asString () << endl;
}

ostream& operator<< (ostream& os, const S_bsrTimeSignatureItem& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_bsrTimeSignature bsrTimeSignature::create (
  int         inputLineNumber,
  bsrTimeSignatureKind timeKind)
{
  bsrTimeSignature* o =
    new bsrTimeSignature (
      inputLineNumber,
      timeKind);
  assert (o != nullptr);
  return o;
}

bsrTimeSignature::bsrTimeSignature (
  int         inputLineNumber,
  bsrTimeSignatureKind timeKind)
    : bsrLineContentsElement (inputLineNumber)
{
  fTimeKind = timeKind;

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceTimeSignatures ()) {
    gLogStream <<
      "Creating bsrTimeSignatures '" <<
      asString () <<
      "', line " <<
      fInputLineNumber <<
      endl;
  }
#endif
}

bsrTimeSignature::~bsrTimeSignature ()
{}

void bsrTimeSignature::appendTimeSignatureItem (S_bsrTimeSignatureItem timeSignatureItem)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceTimeSignatures ()) {
    gLogStream <<
      "Appending time signature item '" <<
      timeSignatureItem->asString () <<
      "' to time '" <<
      asString () <<
      "', line " <<
      fInputLineNumber <<
      endl;
  }
#endif

  fTimeSignatureItemsVector.push_back (timeSignatureItem);
}

S_bsrCellsList bsrTimeSignature::buildCellsList () const
{
  S_bsrCellsList
    result =
      bsrCellsList::create (fInputLineNumber);

  switch (fTimeKind) {
    case bsrTimeSignature::kTimeNone:
      break;

    case bsrTimeSignature::kTimeCommon:
      result->appendCellKindToCellsList (kCellNumberSign);
      result->appendCellKindToCellsList (kCell4);
      result->appendCellKindToCellsList (kCellLower4);
      break;

    case bsrTimeSignature::kTimeCut:
      result->appendCellKindToCellsList (kCellNumberSign);
      result->appendCellKindToCellsList (kCell2);
      result->appendCellKindToCellsList (kCellLower2);
      break;

    case bsrTimeSignature::kTimeNumerical: // JMI ???
    case bsrTimeSignature::kTimeNote:
    case bsrTimeSignature::kTimeDottedNote:
      {
        if (fTimeSignatureItemsVector.size ()) {
          vector<S_bsrTimeSignatureItem>::const_iterator
            iBegin = fTimeSignatureItemsVector.begin (),
        // JMI    iEnd   = fTimeSignatureItemsVector.end (),
            i      = iBegin;

          S_bsrTimeSignatureItem theBsrTimeSignatureItem = (*i);

          const vector<int>&
            timeSignatureBeatsNumbersVector =
              theBsrTimeSignatureItem->getTimeSignatureBeatsNumbersVector ();

          int theBsrTimeSignatureBeatValue =
              theBsrTimeSignatureItem->getTimeSignatureBeatValue ();

          unsigned int vectorSize =
            timeSignatureBeatsNumbersVector.size ();

          switch (vectorSize) {
            case 0:
            /* JMI
              msrInternalError (
                gGlobalServiceRunData->getInputSourceName (),
                fInputLineNumber,
                __FILE__, __LINE__,
                "time signature item beats numbers vector is empty");
                */
              break;

            case 1:
              {
                // determine the beats number
                int beatsNumberToBeUsed = timeSignatureBeatsNumbersVector [0];

                switch (fTimeKind) {
                  case bsrTimeSignature::kTimeNote:
                    break;
                  case bsrTimeSignature::kTimeDottedNote:
                    beatsNumberToBeUsed /= 3;
                    break;
                  default:
                    ;
                } // switch

                // create the beats number
                S_bsrNumber
                  beatsNumber =
                    bsrNumber::create (
                      fInputLineNumber,
                      beatsNumberToBeUsed,
                      bsrNumber::kNumberSignIsNeededYes);

                // append it to result
                result->appendCellsListToCellsList (
                  beatsNumber->fetchCellsList ());

                // create music code indicator
                S_bsrCellsList
                  musicCodeIndicator =
                    bsrCellsList::create (
                      fInputLineNumber,
                      bsrCellKind::kDots6, bsrCellKind::kDots3);

                // append it to result
                result->appendCellsListToCellsList (
                  musicCodeIndicator);

                // determine the beat value to be used
                int beatValueToBeUsed = theBsrTimeSignatureBeatValue;

                switch (fTimeKind) {
                  case bsrTimeSignature::kTimeNote:
                    break;
                  case bsrTimeSignature::kTimeDottedNote:
                    beatValueToBeUsed /= 2;
                    break;
                  default:
                    ;
                } // switch

                // append the beat value to result
                switch (beatValueToBeUsed) {
                  case 1:
                    result->appendCellsListToCellsList (
                      noteValueKindAsCellsList (
                        fInputLineNumber,
                        bsrNoteValueKind::kNoteValueCWhole));
                    break;
                  case 2:
                    result->appendCellsListToCellsList (
                      noteValueKindAsCellsList (
                        fInputLineNumber,
                        bsrNoteValueKind::kNoteValueCHalf));
                    break;
                  case 4:
                    result->appendCellsListToCellsList (
                      noteValueKindAsCellsList (
                        fInputLineNumber,
                        bsrNoteValueKind::kNoteValueCQuarter));
                    break;
                  case 8:
                    result->appendCellsListToCellsList (
                      noteValueKindAsCellsList (
                        fInputLineNumber,
                        bsrNoteValueKind::kNoteValueC8th));
                    break;
                  case 16:
                    result->appendCellsListToCellsList (
                      noteValueKindAsCellsList (
                        fInputLineNumber,
                        bsrNoteValueKind::kNoteValueC16th));
                    break;
                  case 32:
                    result->appendCellsListToCellsList (
                      noteValueKindAsCellsList (
                        fInputLineNumber,
                        bsrNoteValueKind::kNoteValueC32nd));
                    break;
                  case 64:
                    result->appendCellsListToCellsList (
                      noteValueKindAsCellsList (
                        fInputLineNumber,
                        bsrNoteValueKind::kNoteValueC64th));
                    break;
                  case 128:
                    result->appendCellsListToCellsList (
                      noteValueKindAsCellsList (
                        fInputLineNumber,
                        bsrNoteValueKind::kNoteValueC128th));
                    break;
                  case 256:
                    result->appendCellsListToCellsList (
                      noteValueKindAsCellsList (
                        fInputLineNumber,
                        bsrNoteValueKind::kNoteValueC256th));
                    break;
                  default:
                    {
                      stringstream s;

                      s <<
                        "MSR time beat value '" <<
                        theBsrTimeSignatureBeatValue <<
                        "' is not supported in Braille";

                      bsrWarning (
                        gGlobalServiceRunData->getInputSourceName (),
                        fInputLineNumber,
                        s.str ());
                    }
                } // switch

                // append a dot to the beat number if needed
                switch (fTimeKind) {
                  case bsrTimeSignature::kTimeNote:
                    break;
                  case bsrTimeSignature::kTimeDottedNote:
                    result->appendCellKindToCellsList (
                      kCellAugmentationDot);
                    break;
                  default:
                    ;
                } // switch
              }
              break;

            default:
              ;
            /*
              s <<
                "beats numbers: ";

              for (int i = 0; i < vectorSize; ++i) {
                s <<
                  timeSignatureBeatsNumbersVector [i];

                if (i != vectorSize - 1) {
                  s <<
                    " ";
                }
              } // for

              s <<
                ", beat value: " << fTimeSignatureBeatValue;
                */
          } // switch
        }
      }
      break;

    case bsrTimeSignature::kTimeSingleNumber:
      {
        if (fTimeSignatureItemsVector.size ()) {
          vector<S_bsrTimeSignatureItem>::const_iterator
            iBegin = fTimeSignatureItemsVector.begin (),
   // JMI         iEnd   = fTimeSignatureItemsVector.end (),
            i      = iBegin;

          S_bsrTimeSignatureItem theBsrTimeSignatureItem = (*i);

          const vector<int>&
            timeSignatureBeatsNumbersVector =
              theBsrTimeSignatureItem->getTimeSignatureBeatsNumbersVector ();

          int theBsrTimeSignatureBeatValue =
            theBsrTimeSignatureItem->getTimeSignatureBeatValue ();

          unsigned int vectorSize =
            timeSignatureBeatsNumbersVector.size ();

          switch (vectorSize) {
            case 0:
            /* JMI
              msrInternalError (
                gGlobalServiceRunData->getInputSourceName (),
                fInputLineNumber,
                __FILE__, __LINE__,
                "time signature item beats numbers vector is empty");
                */
              break;

            case 1:
              {
                // create the beats number
                S_bsrNumber
                  beatsNumber =
                    bsrNumber::create (
                      fInputLineNumber,
                      timeSignatureBeatsNumbersVector [0],
                      bsrNumber::kNumberSignIsNeededYes);

                // append it to result
                result->appendCellsListToCellsList (
                  beatsNumber->fetchCellsList ());

                // append the beat value sign to result
                switch (theBsrTimeSignatureBeatValue) {
                  case 1:
                    result->appendCellKindToCellsList (
                      kCellLower1);
                    break;
                  case 2:
                    result->appendCellKindToCellsList (
                      kCellLower2);
                    break;
                  case 4:
                    result->appendCellKindToCellsList (
                      kCellLower4);
                    break;
                  case 8:
                    result->appendCellKindToCellsList (
                      kCellLower8);
                    break;
                  case 16:
                    result->appendCellKindToCellsList (
                      kCellLower1);
                    result->appendCellKindToCellsList (
                      kCellLower6);
                    break;
                  case 32:
                    result->appendCellKindToCellsList (
                      kCellLower3);
                    result->appendCellKindToCellsList (
                      kCellLower2);
                    break;
                  default:
                    {
                      stringstream s;

                      s <<
                        "MSR time beat value '" <<
                        theBsrTimeSignatureBeatValue <<
                        "' is not supported in Braille";

                      bsrWarning (
                        gGlobalServiceRunData->getInputSourceName (),
                        fInputLineNumber,
                        s.str ());
                    }
                } // switch
              }
              break;

            default:
              ;
            /*
              s <<
                "beats numbers: ";

              for (int i = 0; i < vectorSize; ++i) {
                s <<
                  timeSignatureBeatsNumbersVector [i];

                if (i != vectorSize - 1) {
                  s <<
                    " ";
                }
              } // for

              s <<
                ", beat value: " << fTimeSignatureBeatValue;
                */
          } // switch
        }
      }
      break;

    case bsrTimeSignature::kTimeSenzaMisura:
      break;
  } // switch

  return result;
}

int bsrTimeSignature::fetchCellsNumber() const
{
  // time signature items may have been appended after construction
  return fetchCellsList ()->fetchCellsNumber ();
}

void bsrTimeSignature::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
    gLogStream <<
      "% ==> bsrTimeSignature::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_bsrTimeSignature>*
    p =
      dynamic_cast<visitor<S_bsrTimeSignature>*> (v)) {
        S_bsrTimeSignature elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
          gLogStream <<
            "% ==> Launching bsrTimeSignature::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void bsrTimeSignature::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
    gLogStream <<
      "% ==> bsrTimeSignature::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_bsrTimeSignature>*
    p =
      dynamic_cast<visitor<S_bsrTimeSignature>*> (v)) {
        S_bsrTimeSignature elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
          gLogStream <<
            "% ==> Launching bsrTimeSignature::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void bsrTimeSignature::browseData (basevisitor* v)
{}

string bsrTimeSignature::timeKindAsString (
  bsrTimeSignatureKind timeKind)
{
  string result;

  switch (timeKind) {
    case bsrTimeSignature::kTimeNone:
      result = "timeNone";
      break;

    case bsrTimeSignature::kTimeCommon:
      result = "timeCommon";
      break;
    case bsrTimeSignature::kTimeCut:
      result = "timeCut";
      break;
    case bsrTimeSignature::kTimeNumerical:
      result = "timeNumerical";
      break;
    case bsrTimeSignature::kTimeNote:
      result = "timeNote";
      break;
    case bsrTimeSignature::kTimeDottedNote:
      result = "timeDottedNote";
      break;
    case bsrTimeSignature::kTimeSingleNumber:
      result = "timeSingleNumber";
      break;
    case bsrTimeSignature::kTimeSenzaMisura:
      result = "timeSenzaMisura";
      break;
  } // switch

  return result;
}

string bsrTimeSignature::timeKindAsDebugString (
  bsrTimeSignatureKind timeKind)
{
  string result;

  switch (timeKind) {
    case bsrTimeSignature::kTimeNone:
      result = "_";
      break;

    case bsrTimeSignature::kTimeCommon:
      result = "C";
      break;
    case bsrTimeSignature::kTimeCut:
      result = "C/";
      break;
    case bsrTimeSignature::kTimeNumerical:
      result = "Num";
      break;
    case bsrTimeSignature::kTimeNote:
      result = "Note";
      break;
    case bsrTimeSignature::kTimeDottedNote:
      result = "DottedNote";
      break;
    case bsrTimeSignature::kTimeSingleNumber:
      result = "SingleNumber";
      break;
    case bsrTimeSignature::kTimeSenzaMisura:
      result = "SenzaMisura";
      break;
  } // switch

  return result;
}

string bsrTimeSignature::asString () const
{
  stringstream s;

  s <<
    "Time" <<
    ", timeKind " << " : " <<
    timeKindAsString (fTimeKind) <<
    ", timeCellsList: " << fetchCellsList ()->asString () <<
    ", spacesBefore: " << fSpacesBefore <<
    ", line " << fInputLineNumber;

  return s.str ();
}

string bsrTimeSignature::asDebugString () const
{
  stringstream s;

  s <<
    "[TIME " <<
    timeKindAsDebugString (fTimeKind) <<
    "]";

  return s.str ();
}

void bsrTimeSignature::print (ostream& os) const
{
  unsigned int timeSignatureItemsVectorSize = fTimeSignatureItemsVector.size ();

  os <<
    "Time" <<
    ", " <<
    mfSingularOrPlural (
      timeSignatureItemsVectorSize, "item", "items") <<
    ", line " << fInputLineNumber <<
    ":" <<
    endl;

  ++gIndenter;

  const int fieldWidth = 16;

  os << left <<
    setw (fieldWidth) <<
    "timeKind " << " : " <<
    timeKindAsString (fTimeKind) <<
    endl <<
    setw (fieldWidth) <<
    "timeSignatureItemsVector" << " : ";

  if (timeSignatureItemsVectorSize) {
    os << endl;

    ++gIndenter;

    vector<S_bsrTimeSignatureItem>::const_iterator
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

  os <<
    setw (fieldWidth) <<
    "timeCellsList" << " : " << fetchCellsList ()->asString () <<
    endl <<
    setw (fieldWidth) <<
    "spacesBefore" << " : " << fSpacesBefore <<
   endl;

  --gIndenter;
}

ostream& operator<< (ostream& os, const S_bsrTimeSignature& elt)
{
  elt->print (os);
  return os;
}


}
