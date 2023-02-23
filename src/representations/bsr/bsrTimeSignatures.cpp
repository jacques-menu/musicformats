/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <iostream>
#include <iomanip>      // setw()), set::precision(), ...
#include <sstream>

#include "visitor.h"

#include "mfStaticSettings.h"

#include "mfServices.h"
#include "mfStringsHandling.h"

#include "bsrWae.h"

#include "bsrTimeSignatures.h"

#include "bsrNotes.h"
#include "bsrNumbers.h"

#include "oahOah.h"

#include "bsrOah.h"

#include "waeHandlers.h"


namespace MusicFormats
{

//______________________________________________________________________________
std::string bsrTimeSignatureKindAsString (
  bsrTimeSignatureKind timeKind)
{
  std::string result;

  switch (timeKind) {
    case bsrTimeSignatureKind::kTimeSignatureNone:
      result = "kTimeNone";
      break;

    case bsrTimeSignatureKind::kTimeSignatureCommon:
      result = "kTimeCommon";
      break;
    case bsrTimeSignatureKind::kTimeSignatureCut:
      result = "kTimeCut";
      break;
    case bsrTimeSignatureKind::kTimeSignatureNumerical:
      result = "kTimeNumerical";
      break;
    case bsrTimeSignatureKind::kTimeSignatureNote:
      result = "kTimeNote";
      break;
    case bsrTimeSignatureKind::kTimeSignatureDottedNote:
      result = "kTimeNote";
      break;
    case bsrTimeSignatureKind::kTimeSignatureSingleNumber:
      result = "kTimeSingleNumber";
      break;
    case bsrTimeSignatureKind::kTimeSignatureSenzaMisura:
      result = "kTimeSenzaMisura";
      break;
  } // switch

  return result;
}

std::ostream& operator << (std::ostream& os, const bsrTimeSignatureKind& elt)
{
  os << bsrTimeSignatureKindAsString (elt);
  return os;
}

std::string bsrTimeKindAsDebugString (
  bsrTimeSignatureKind timeKind)
{
  std::string result;

  switch (timeKind) {
    case bsrTimeSignatureKind::kTimeSignatureNone:
      result = "_";
      break;

    case bsrTimeSignatureKind::kTimeSignatureCommon:
      result = "C";
      break;
    case bsrTimeSignatureKind::kTimeSignatureCut:
      result = "C/";
      break;
    case bsrTimeSignatureKind::kTimeSignatureNumerical:
      result = "Num";
      break;
    case bsrTimeSignatureKind::kTimeSignatureNote:
      result = "Note";
      break;
    case bsrTimeSignatureKind::kTimeSignatureDottedNote:
      result = "DottedNote";
      break;
    case bsrTimeSignatureKind::kTimeSignatureSingleNumber:
      result = "SingleNumber";
      break;
    case bsrTimeSignatureKind::kTimeSignatureSenzaMisura:
      result = "SenzaMisura";
      break;
  } // switch

  return result;
}

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

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceTimeSignatures ()) {
    std::stringstream ss;

    ss <<
      "Creating time signature item" <<
      ", line: " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
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

void bsrTimeSignatureItem::appendBeatsNumber (int beatsNumber)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceTimeSignatures ()) {
    std::stringstream ss;

    ss <<
      "Appending beat number '" <<
      beatsNumber <<
      "' to time signature item '" <<
      asString () <<
      "'" <<
      std::endl;
    }
#endif // MF_TRACE_IS_ENABLED

  fTimeSignatureBeatsNumbersVector.insert (
    fTimeSignatureBeatsNumbersVector.end (),
    beatsNumber);
}

void bsrTimeSignatureItem::setTimeSignatureBeatValue (int timeSignatureBeatValue)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceTimeSignatures ()) {
    std::stringstream ss;

    ss <<
      "Setting beat value to '" <<
      timeSignatureBeatValue <<
      "' in time signature item '" <<
      asString () <<
      "'" <<
      std::endl;
    }
#endif // MF_TRACE_IS_ENABLED

  fTimeSignatureBeatValue = timeSignatureBeatValue;
}

int bsrTimeSignatureItem::getTimeSignatureBeatsNumber () const
{
  int result = 0;

  for (size_t i = 0; i < fTimeSignatureBeatsNumbersVector.size (); ++i) {
    result +=
      fTimeSignatureBeatsNumbersVector [i];
  } // for

  return result;
}

void bsrTimeSignatureItem::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gBsrOahGroup->getTraceBsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% --> End visiting bsrTranscriptionNotesElement" <<
      "% ==> bsrTimeSignatureItem::acceptIn ()" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_bsrTimeSignatureItem>*
    p =
      dynamic_cast<visitor<S_bsrTimeSignatureItem>*> (v)) {
        S_bsrTimeSignatureItem elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gBsrOahGroup->getTraceBsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching bsrTimeSignatureItem::visitStart ()" <<
            std::endl;

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void bsrTimeSignatureItem::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gBsrOahGroup->getTraceBsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% --> End visiting bsrTranscriptionNotesElement" <<
      "% ==> bsrTimeSignatureItem::acceptOut ()" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_bsrTimeSignatureItem>*
    p =
      dynamic_cast<visitor<S_bsrTimeSignatureItem>*> (v)) {
        S_bsrTimeSignatureItem elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gBsrOahGroup->getTraceBsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching bsrTimeSignatureItem::visitEnd ()" <<
            std::endl;

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void bsrTimeSignatureItem::browseData (basevisitor* v)
{}

std::string bsrTimeSignatureItem::asString () const
{
  std::stringstream ss;

  size_t timeSignatureBeatsNumbersVectorSize =
    fTimeSignatureBeatsNumbersVector.size ();

  ss <<
    "[TimeSignatureItem " <<
    ", " <<
    mfSingularOrPlural (
      timeSignatureBeatsNumbersVectorSize, "timeSignatureBeatNumber", "timeSignatureBeatNumbers");

  switch (timeSignatureBeatsNumbersVectorSize) {
    case 0:
    /* JMI
      msrInternalError (
        gServiceRunData->getInputSourceName (),
        fInputLineNumber,
        __FILE__, __LINE__,
        "time signature item beats numbers vector is empty");
        */
      break;

    case 1:
      ss <<
        ", " <<
        fTimeSignatureBeatsNumbersVector [0] << '/' << fTimeSignatureBeatValue;
      break;

    default:
      ss <<
        ", beats numbers: ";

      for (size_t i = 0; i < timeSignatureBeatsNumbersVectorSize; ++i) {
        ss <<
          fTimeSignatureBeatsNumbersVector [i];

        if (i != timeSignatureBeatsNumbersVectorSize - 1) {
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

void bsrTimeSignatureItem::print (std::ostream& os) const
{
  os << asString () << std::endl;
}

std::ostream& operator << (std::ostream& os, const S_bsrTimeSignatureItem& elt)
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

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceTimeSignatures ()) {
    std::stringstream ss;

    ss <<
      "Creating bsrTimeSignatures '" <<
      asString () <<
      "', line " <<
      fInputLineNumber <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

bsrTimeSignature::~bsrTimeSignature ()
{}

void bsrTimeSignature::appendTimeSignatureItem (S_bsrTimeSignatureItem timeSignatureItem)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceTimeSignatures ()) {
    std::stringstream ss;

    ss <<
      "Appending time signature item '" <<
      timeSignatureItem->asString () <<
      "' to time '" <<
      asString () <<
      "', line " <<
      fInputLineNumber <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fTimeSignatureItemsVector.push_back (timeSignatureItem);
}

S_bsrCellsList bsrTimeSignature::buildCellsList () const
{
  S_bsrCellsList
    result =
      bsrCellsList::create (fInputLineNumber);

  switch (fTimeKind) {
    case bsrTimeSignatureKind::kTimeSignatureNone:
      break;

    case bsrTimeSignatureKind::kTimeSignatureCommon:
      result->appendCellKindToCellsList (kCellNumberSign);
      result->appendCellKindToCellsList (kCell4);
      result->appendCellKindToCellsList (kCellLower4);
      break;

    case bsrTimeSignatureKind::kTimeSignatureCut:
      result->appendCellKindToCellsList (kCellNumberSign);
      result->appendCellKindToCellsList (kCell2);
      result->appendCellKindToCellsList (kCellLower2);
      break;

    case bsrTimeSignatureKind::kTimeSignatureNumerical: // JMI ???
    case bsrTimeSignatureKind::kTimeSignatureNote:
    case bsrTimeSignatureKind::kTimeSignatureDottedNote:
      {
        if (fTimeSignatureItemsVector.size ()) {
          std::vector<S_bsrTimeSignatureItem>::const_iterator
            iBegin = fTimeSignatureItemsVector.begin (),
        // JMI    iEnd   = fTimeSignatureItemsVector.end (),
            i      = iBegin;

          S_bsrTimeSignatureItem theBsrTimeSignatureItem = (*i);

          const std::vector<int>&
            timeSignatureBeatsNumbersVector =
              theBsrTimeSignatureItem->getTimeSignatureBeatsNumbersVector ();

          int theBsrTimeSignatureBeatValue =
              theBsrTimeSignatureItem->getTimeSignatureBeatValue ();

          size_t vectorSize =
            timeSignatureBeatsNumbersVector.size ();

          switch (vectorSize) {
            case 0:
            /* JMI
              msrInternalError (
                gServiceRunData->getInputSourceName (),
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
                  case bsrTimeSignatureKind::kTimeSignatureNote:
                    break;
                  case bsrTimeSignatureKind::kTimeSignatureDottedNote:
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
                      bsrNumberSignIsNeededKind::kNumberSignIsNeededYes);

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
                  case bsrTimeSignatureKind::kTimeSignatureNote:
                    break;
                  case bsrTimeSignatureKind::kTimeSignatureDottedNote:
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
                      std::stringstream ss;

                      ss <<
                        "MSR time beat value '" <<
                        theBsrTimeSignatureBeatValue <<
                        "' is not supported in Braille";

                      bsrWarning (
                        gServiceRunData->getInputSourceName (),
                        fInputLineNumber,
                        ss.str ());
                    }
                } // switch

                // append a dot to the beat number if needed
                switch (fTimeKind) {
                  case bsrTimeSignatureKind::kTimeSignatureNote:
                    break;
                  case bsrTimeSignatureKind::kTimeSignatureDottedNote:
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
              ss <<
                "beats numbers: ";

              for (int i = 0; i < vectorSize; ++i) {
                ss <<
                  timeSignatureBeatsNumbersVector [i];

                if (i != vectorSize - 1) {
                  ss <<
                    ' ';
                }
              } // for

              ss <<
                ", beat value: " << fTimeSignatureBeatValue;
                */
          } // switch
        }
      }
      break;

    case bsrTimeSignatureKind::kTimeSignatureSingleNumber:
      {
        if (fTimeSignatureItemsVector.size ()) {
          std::vector<S_bsrTimeSignatureItem>::const_iterator
            iBegin = fTimeSignatureItemsVector.begin (),
   // JMI         iEnd   = fTimeSignatureItemsVector.end (),
            i      = iBegin;

          S_bsrTimeSignatureItem theBsrTimeSignatureItem = (*i);

          const std::vector<int>&
            timeSignatureBeatsNumbersVector =
              theBsrTimeSignatureItem->getTimeSignatureBeatsNumbersVector ();

          int theBsrTimeSignatureBeatValue =
            theBsrTimeSignatureItem->getTimeSignatureBeatValue ();

          size_t vectorSize =
            timeSignatureBeatsNumbersVector.size ();

          switch (vectorSize) {
            case 0:
            /* JMI
              msrInternalError (
                gServiceRunData->getInputSourceName (),
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
                      bsrNumberSignIsNeededKind::kNumberSignIsNeededYes);

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
                      std::stringstream ss;

                      ss <<
                        "MSR time beat value '" <<
                        theBsrTimeSignatureBeatValue <<
                        "' is not supported in Braille";

                      bsrWarning (
                        gServiceRunData->getInputSourceName (),
                        fInputLineNumber,
                        ss.str ());
                    }
                } // switch
              }
              break;

            default:
              ;
            /*
              ss <<
                "beats numbers: ";

              for (int i = 0; i < vectorSize; ++i) {
                ss <<
                  timeSignatureBeatsNumbersVector [i];

                if (i != vectorSize - 1) {
                  ss <<
                    ' ';
                }
              } // for

              ss <<
                ", beat value: " << fTimeSignatureBeatValue;
                */
          } // switch
        }
      }
      break;

    case bsrTimeSignatureKind::kTimeSignatureSenzaMisura:
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
#ifdef MF_TRACE_IS_ENABLED
  if (gBsrOahGroup->getTraceBsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% --> End visiting bsrTranscriptionNotesElement" <<
      "% ==> bsrTimeSignature::acceptIn ()" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_bsrTimeSignature>*
    p =
      dynamic_cast<visitor<S_bsrTimeSignature>*> (v)) {
        S_bsrTimeSignature elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gBsrOahGroup->getTraceBsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching bsrTimeSignature::visitStart ()" <<
            std::endl;

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void bsrTimeSignature::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gBsrOahGroup->getTraceBsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% --> End visiting bsrTranscriptionNotesElement" <<
      "% ==> bsrTimeSignature::acceptOut ()" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_bsrTimeSignature>*
    p =
      dynamic_cast<visitor<S_bsrTimeSignature>*> (v)) {
        S_bsrTimeSignature elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gBsrOahGroup->getTraceBsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching bsrTimeSignature::visitEnd ()" <<
            std::endl;

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void bsrTimeSignature::browseData (basevisitor* v)
{}

std::string bsrTimeSignature::asString () const
{
  std::stringstream ss;

  ss <<
    "Time" <<
    ", timeKind " << ": " <<
    bsrTimeSignatureKindAsString (fTimeKind) <<
    ", timeCellsList: " << fetchCellsList ()->asString () <<
    ", spacesBefore: " << fSpacesBefore <<
    ", line " << fInputLineNumber;

  return ss.str ();
}

std::string bsrTimeSignature::asDebugString () const
{
  std::stringstream ss;

  ss <<
    "[TIME " <<
    fTimeKind <<
    ']';

  return ss.str ();
}

void bsrTimeSignature::print (std::ostream& os) const
{
  size_t timeSignatureItemsVectorSize = fTimeSignatureItemsVector.size ();

  os <<
    "Time" <<
    ", " <<
    mfSingularOrPlural (
      timeSignatureItemsVectorSize, "item", "items") <<
    ", line " << fInputLineNumber <<
    ":" <<
    std::endl;

  ++gIndenter;

  const int fieldWidth = 16;

  os << std::left <<
    std::setw (fieldWidth) <<
    "timeKind " << ": " <<
    bsrTimeSignatureKindAsString (fTimeKind) <<
    std::endl <<
    std::setw (fieldWidth) <<
    "timeSignatureItemsVector" << ": ";

  if (timeSignatureItemsVectorSize) {
    os << std::endl;

    ++gIndenter;

    std::vector<S_bsrTimeSignatureItem>::const_iterator
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

  os <<
    std::setw (fieldWidth) <<
    "timeCellsList" << ": " << fetchCellsList ()->asString () <<
    std::endl <<
    std::setw (fieldWidth) <<
    "spacesBefore" << ": " << fSpacesBefore <<
   std::endl;

  --gIndenter;
}

std::ostream& operator << (std::ostream& os, const S_bsrTimeSignature& elt)
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
