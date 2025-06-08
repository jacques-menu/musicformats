/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <iostream>
#include <sstream>
#include <iomanip>      // std::setw, std::setprecision, ...

#include "visitor.h"

#include "mfServices.h"

#include "msrWae.h"

#include "msrDivisions.h"

#include "oahEarlyOptions.h"

#include "oahOah.h"

#include "mxsr2msrOah.h"

#include "msrOah.h"

#include "waeHandlers.h"


namespace MusicFormats
{

//______________________________________________________________________________
S_msrDivisions msrDivisions::create (
  const mfInputLineNumber& inputLineNumber,
  int divisionsPerQuarterNote)
{
  msrDivisions* obj =
    new msrDivisions (
      inputLineNumber, divisionsPerQuarterNote);
  assert (obj != nullptr);
  return obj;
}

S_msrDivisions msrDivisions::createDivisionsNewbornClone ()
{
#ifdef MF_TRACE_IS_ENABLED
 if (gGlobalMxsr2msrOahGroup->getTraceDivisions ()) {
    std::stringstream ss;

    ss <<
      "Creating a newborn clone of divisions '" <<
      asString () <<
      "'";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  S_msrDivisions
    newbornClone =
      msrDivisions::create (
        fInputLineNumber,
        fDivisionsPerQuarterNote);

  return newbornClone;
}

msrDivisions::msrDivisions (
  const mfInputLineNumber& inputLineNumber,
  int divisionsPerQuarterNote)
    : msrElement (inputLineNumber)
{
  // set divisions per quarter note
  fDivisionsPerQuarterNote = divisionsPerQuarterNote;

  // initialize the divisions
  initializeDivisions ();
}

msrDivisions::~msrDivisions ()
{}

void msrDivisions::initializeDivisions ()
{
#ifdef MF_TRACE_IS_ENABLED
  if (
    gGlobalMxsr2msrOahGroup->getTraceDivisions ()
      &&
   ! gEarlyOptions.getEarlyQuietOption ()
  ) {
    std::stringstream ss;

    ss <<
      "Initializing divisions" <<
      ", divisionsPerQuarterNote: " << fDivisionsPerQuarterNote <<
      ", line " << fInputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  ++gIndenter;

  // forget fNotesDurationsToDivisions's contents
  fNotesDurationKindsToDivisions.clear ();

  // positive powers of 2 of a quarter note
  int bigDivisions = fDivisionsPerQuarterNote;

  for (int i = (int) mfDurationKind::kDurationQuarter; i >= (int) mfDurationKind::kDurationMaxima; --i) {
    /*
    gLog <<
      msrNotesDurationAsString (msrNotesDuration (i)) <<
      " -> " <<
      bigDivisions <<
      std::endl;
    */

    fNotesDurationKindsToDivisions.push_front (
      std::make_pair (
        mfDurationKind (i), bigDivisions));

    bigDivisions *= 2;
  } // for

  if (fDivisionsPerQuarterNote > 1) {
    // negative powers of 2 of a quarter note
    int
      smallDivisions =
        fDivisionsPerQuarterNote / 2;
    mfDurationKind
      currentNotesDurationKind =
        mfDurationKind::kDurationEighth;

    while (smallDivisions >= 1) {
      /*
      gLog <<
        msrNotesDurationAsString (currentNotesDuration) <<
        " % --> " <<
        smallDivisions <<
        std::endl;
      */

      fNotesDurationKindsToDivisions.push_back (
        std::make_pair (currentNotesDurationKind, smallDivisions));

      currentNotesDurationKind =
        mfDurationKind (
          (int) currentNotesDurationKind + 1);
      smallDivisions /= 2;
    } // while
  }

  // print the durations divisions if needed
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceDivisions ()) {
    printNotesDurationKindsDivisions (gLog);
  }
#endif // MF_TRACE_IS_ENABLED

  --gIndenter;
}

int msrDivisions::notesDurationKindAsDivisions (
  const mfInputLineNumber& inputLineNumber,
  mfDurationKind notesDurationKind)
{
  for (
    std::list <std::pair <mfDurationKind, int>>::const_iterator i =
      fNotesDurationKindsToDivisions.begin ();
    i != fNotesDurationKindsToDivisions.end ();
    ++i
  ) {
    if ((*i).first == notesDurationKind)
      return
        (*i).second;
  } // for

  std::stringstream ss;

  ss <<
    "duration " << mfDurationKindAsString (notesDurationKind) <<
    " cannot be converted to divisions with " <<
    fDivisionsPerQuarterNote << " divisions per quarter note" <<
    std::endl;

  printNotesDurationKindsDivisions (ss);

  msrInternalError (
    gServiceRunData->getInputSourceName (),
    inputLineNumber,
    __FILE__, mfInputLineNumber (__LINE__),
    ss.str ());

  return -1; // never reached
}

void msrDivisions::printNotesDurationKindsDivisions (std::ostream& os)
{
  os <<
    "The mapping of durations to divisions with " <<
    fDivisionsPerQuarterNote << " divisions per quarter note" <<
    " is:" <<
    std::endl;

  ++gIndenter;

  if (fNotesDurationKindsToDivisions.size ()) {
    std::list <std::pair <mfDurationKind, int>>::const_iterator
      iBegin = fNotesDurationKindsToDivisions.begin (),
      iEnd   = fNotesDurationKindsToDivisions.end (),
      i      = iBegin;

    for ( ; ; ) {
      os <<
        std::setw (6) << std::left <<
        mfDurationKindAsString (
          mfDurationKind ((*i).first)) <<
        ": " <<
        std::setw (5) << std::right <<
        (*i).second;

      if (++i == iEnd) break;

      os << std::endl;
    } // for

/* JMI

    for (
      std::list <std::pair <msrNotesDuration, int>>::const_iterator i =
        fNotesDurationsToDivisions.begin ();
      i != fNotesDurationsToDivisions.end ();
      ++i
  ) {
      os <<
        std::setw (6) << std::left <<
        msrNotesDurationAsString (msrNotesDuration((*i).first)) <<
        ": " <<
        std::setw (4) << std::right <<
        (*i).second <<
        std::endl;
    } // for
*/
  }

  else
    os <<
      "an empty list";

  os << std::endl;

  --gIndenter;
}

std::string msrDivisions::divisionspitchAndOctaveAsString (
  const mfInputLineNumber& inputLineNumber,
  int  divisions,
  int& numberOfDotsNeeded)
{
  std::string result;

  // the result is a base duration, followed by a suffix made of
  // either a sequence of dots or a multiplication factor

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceDivisions ()) {
    constexpr int fieldWidth = 16;

    std::stringstream ss;

    ss <<
     "--> divisionspitchAndOctaveAsString ():" <<
      std::endl <<
      gTab << std::setw (fieldWidth) <<
      "inputLineNumber" << ": " << inputLineNumber <<
      std::endl <<
      gTab << std::setw (fieldWidth) <<
      "divisions" << ": " << divisions;

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  mfDurationKind baseNotesDurationKind = mfDurationKind::kDuration1024th;
  int            baseNotesDurationDivisions = -1;

  // search fNotesDurationsToDivisions in longer to shortest order
  std::list <std::pair <mfDurationKind, int>>::const_iterator
    iBegin = fNotesDurationKindsToDivisions.begin (),
    iEnd   = fNotesDurationKindsToDivisions.end (),
    i      = iBegin;

  for ( ; ; ) {
    if (i == iEnd) {
      std::stringstream ss;

      ss <<
        "divisions " << divisions <<
        " could not be handled by divisionspitchAndOctaveAsString () with:" <<
        std::endl;

      printNotesDurationKindsDivisions (gLog);

      msrInternalError (
        gServiceRunData->getInputSourceName (),
        inputLineNumber,
        __FILE__, mfInputLineNumber (__LINE__),
        ss.str ());
      break;
    }

    if ((*i).second <= divisions) {
      // found base duration in list
      baseNotesDurationKind = (*i).first;
      baseNotesDurationDivisions = (*i).second;

      result =
        mfDurationKindAsString (baseNotesDurationKind);

#ifdef MF_TRACE_IS_ENABLED
      if (gGlobalMxsr2msrOahGroup->getTraceDivisions ()) {
        constexpr int fieldWidth = 22;

        gLog <<
            gTab << std::setw (fieldWidth) <<
          "divisions" << ": " << divisions <<
          std::endl << std::endl <<
            gTab << std::setw (fieldWidth) <<
          "baseNotesDurationDivisions" << ": " << baseNotesDurationDivisions <<
          std::endl <<
            gTab << std::setw (fieldWidth) <<
          "result" << ": " << result <<
          std::endl << std::endl;
      }
#endif // MF_TRACE_IS_ENABLED

      break;
    }

    // next please!
    ++i;
  } // for

  int dotsNumber = 0;

  if (divisions > baseNotesDurationDivisions) {
    // divisions is not a power of 2 of a quarter note

    // the next element in the list is half as long as (*i)
    int remainingDivisions =
      divisions - baseNotesDurationDivisions;
    int nextDivisionsInList =
      baseNotesDurationDivisions / 2;

#ifdef MF_TRACE_IS_ENABLED
    if (gGlobalMxsr2msrOahGroup->getTraceDivisions ()) {
      constexpr int fieldWidth = 22;

      gLog <<
        gTab << std::setw (fieldWidth) <<
        "divisions" << ": " << divisions <<
        std::endl <<
        gTab << std::setw (fieldWidth) <<
      "baseNotesDurationDivisions" << ": " << baseNotesDurationDivisions <<
        std::endl <<
        gTab << std::setw (fieldWidth) <<
      "nextDivisionsInList" << ": " << nextDivisionsInList <<
        std::endl <<
        gTab << std::setw (fieldWidth) <<
      "remainingDivisions" << ": " << remainingDivisions <<
        std::endl << std::endl;
    }
#endif // MF_TRACE_IS_ENABLED

    if (remainingDivisions < nextDivisionsInList) {
      // the suffix is a multiplication factor
      mfRational
        r (
          divisions,
          baseNotesDurationDivisions);

#ifdef MF_TRACE_IS_ENABLED
      if (gGlobalMxsr2msrOahGroup->getTraceDivisions ()) {
        constexpr int fieldWidth = 22;

        gLog <<
          gTab << std::setw (fieldWidth) <<
          "divisions" << ": " << divisions <<
          std::endl <<
          gTab << std::setw (fieldWidth) <<
          "baseNotesDurationDivisions" << ": " << baseNotesDurationDivisions <<
          std::endl <<
          gTab << std::setw (fieldWidth) <<
          "r" << ": " << r <<
          std::endl << std::endl;
      }
#endif // MF_TRACE_IS_ENABLED

      result +=
        "*" + r.toString ();
    }

    else {
      dotsNumber = 1; // account for next element in the list

      while (remainingDivisions > nextDivisionsInList) {
        ++dotsNumber;
        remainingDivisions -= nextDivisionsInList;
        nextDivisionsInList /= 2;

#ifdef MF_TRACE_IS_ENABLED
        if (gGlobalMxsr2msrOahGroup->getTraceDivisions ()) {
          constexpr int fieldWidth = 22;

          gLog <<
            gTab << std::setw (fieldWidth) <<
            "divisions" << ": " << divisions <<
            std::endl <<
            gTab << std::setw (fieldWidth) <<
            "baseNotesDurationDivisions" << ": " << baseNotesDurationDivisions <<
            std::endl <<
            gTab << std::setw (fieldWidth) <<
            "nextDivisionsInList" << ": " << nextDivisionsInList <<
            std::endl <<
            gTab << std::setw (fieldWidth) <<
            "remainingDivisions" << ": " << remainingDivisions <<
            std::endl <<
            gTab << std::setw (fieldWidth) <<
            "dotsNumber" << ": " << dotsNumber <<
            std::endl << std::endl;
        }
#endif // MF_TRACE_IS_ENABLED

        if (dotsNumber > 5 ) {
          break; // JMI
        }
      } // while

#ifdef MF_TRACE_IS_ENABLED
      if (gGlobalMxsr2msrOahGroup->getTraceDivisions ()) {
        constexpr int fieldWidth = 24;

        gLog <<
          gTab << std::setw (fieldWidth) <<
          "divisions" << ": " << divisions <<
          std::endl <<
          gTab << std::setw (fieldWidth) <<
          "baseNotesDurationDivisions" << ": " << baseNotesDurationDivisions <<
          std::endl <<
          gTab << std::setw (fieldWidth) <<
          "nextDivisionsInList" << ": " << nextDivisionsInList <<
          std::endl <<
          gTab << std::setw (fieldWidth) <<
          "remainingDivisions" << ": " << remainingDivisions <<
          std::endl <<
          gTab << std::setw (fieldWidth) <<
          "dotsNumber" << ": " << dotsNumber <<
          std::endl << std::endl;
      }
#endif // MF_TRACE_IS_ENABLED

      if (remainingDivisions - nextDivisionsInList == 0) {
        // the suffix is composed of dots
      for (int k = 0; k < dotsNumber; ++k)
        result += ".";
      }
    }
  }

  numberOfDotsNeeded = dotsNumber;

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsr2msrOahGroup->getTraceDivisions ()) {
    std::stringstream ss;

    ss <<
      "<-- divisionspitchAndOctaveAsString (): returns " << result <<
      std::endl << std::endl;

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  return result;
}

std::string msrDivisions::divisionspitchAndOctaveAsString (
  const mfInputLineNumber& inputLineNumber,
  int  divisions)
{
  int numberOfDots; // to be ignored

  return
    divisionspitchAndOctaveAsString (
      inputLineNumber,
      divisions,
      numberOfDots);
}

std::string msrDivisions::tupletDivisionspitchAndOctaveAsString (
  const mfInputLineNumber& inputLineNumber,
  int divisions,
  int actualNotes,
  int normalNotes)
{
  return
    divisionspitchAndOctaveAsString (
      inputLineNumber,
      divisions * actualNotes / normalNotes);
}

std::string tupletWholeNotesPitchAndOctaveAsString (
  const mfInputLineNumber& inputLineNumber,
  const mfWholeNotes& wholeNotes,
  int             actualNotes,
  int             normalNotes)
{
  return
    wholeNotesPitchAndOctaveAsString (
      inputLineNumber,
      wholeNotes
        *
      mfRational (actualNotes, normalNotes));
}

void msrDivisions::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrDivisions::acceptIn ()";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msrDivisions>*
    p =
      dynamic_cast<visitor<S_msrDivisions>*> (v)) {
        S_msrDivisions elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrDivisions::visitStart ()";

          gWaeHandler->waeTrace (
            __FILE__, mfInputLineNumber (__LINE__),
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void msrDivisions::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrDivisions::acceptOut ()";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msrDivisions>*
    p =
      dynamic_cast<visitor<S_msrDivisions>*> (v)) {
        S_msrDivisions elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrDivisions::visitEnd ()";

          gWaeHandler->waeTrace (
            __FILE__, mfInputLineNumber (__LINE__),
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void msrDivisions::browseData (basevisitor* v)
{}

std::string msrDivisions::asString () const
{
  std::stringstream ss;

  ss <<
    "Divisions" <<
    ", " << fDivisionsPerQuarterNote <<
    " per quarter note";

  return ss.str ();
}

void msrDivisions::print (std::ostream& os) const
{
  os << asString () << std::endl;
}

std::ostream& operator << (std::ostream& os, const S_msrDivisions& elt)
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
