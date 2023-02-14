/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

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

#include "mxsrOah.h"

#include "msrOah.h"

#include "waeHandlers.h"


namespace MusicFormats
{

//______________________________________________________________________________
S_msrDivisions msrDivisions::create (
  int inputLineNumber,
  int divisionsPerQuarterNote)
{
  msrDivisions* o =
    new msrDivisions (
      inputLineNumber, divisionsPerQuarterNote);
  assert (o != nullptr);
  return o;
}

S_msrDivisions msrDivisions::createDivisionsNewbornClone ()
{
#ifdef MF_TRACE_IS_ENABLED
 if (gGlobalMxsrOahGroup->getTraceDivisions ()) {
		std::stringstream ss;

    ss <<
      "Creating a newborn clone of divisions '" <<
      asString () <<
      "'" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
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
  int inputLineNumber,
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
    gGlobalMxsrOahGroup->getTraceDivisions ()
      &&
   ! gEarlyOptions.getEarlyQuietOption ()
  ) {
		std::stringstream ss;

    ss <<
      "Initializing divisions" <<
      ", divisionsPerQuarterNote: " << fDivisionsPerQuarterNote <<
      ", line " << fInputLineNumber <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  ++gIndenter;

  // forget fDurationsToDivisions's contents
  fDurationKindsToDivisions.clear ();

  // positive powers of 2 of a quarter note
  int bigDivisions = fDivisionsPerQuarterNote;

  for (int i = (int) msrDurationKind::kDurationQuarter; i >= (int) msrDurationKind::kDurationMaxima; --i) {
    /*
    gLog <<
      msrDurationAsString (msrDuration (i)) <<
      " -> " <<
      bigDivisions <<
      std::endl;
    */

    fDurationKindsToDivisions.push_front (
      std::make_pair (
        msrDurationKind (i), bigDivisions));

    bigDivisions *= 2;
  } // for

  if (fDivisionsPerQuarterNote > 1) {
    // negative powers of 2 of a quarter note
    int
      smallDivisions =
        fDivisionsPerQuarterNote / 2;
    msrDurationKind
      currentDurationKind =
        msrDurationKind::kDurationEighth;

    while (smallDivisions >= 1) {
      /*
      gLog <<
        msrDurationAsString (currentDuration) <<
        " % --> " <<
        smallDivisions <<
        std::endl;
      */

      fDurationKindsToDivisions.push_back (
        std::make_pair (currentDurationKind, smallDivisions));

      currentDurationKind =
        msrDurationKind (
          (int) currentDurationKind + 1);
      smallDivisions /= 2;
    } // while
  }

  // print the durations divisions if needed
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceDivisions ()) {
    printDurationKindsDivisions (gLog);
  }
#endif // MF_TRACE_IS_ENABLED

  --gIndenter;
}

int msrDivisions::durationKindAsDivisions (
  int             inputLineNumber,
  msrDurationKind durationKind)
{
  for (
    std::list<std::pair<msrDurationKind, int> >::const_iterator i =
      fDurationKindsToDivisions.begin ();
    i != fDurationKindsToDivisions.end ();
    ++i
  ) {
    if ((*i).first == durationKind)
      return
        (*i).second;
  } // for

  std::stringstream ss;

  ss <<
    "duration " << msrDurationKindAsString (durationKind) <<
    " cannot be converted to divisions with " <<
    fDivisionsPerQuarterNote << " divisions per quarter note" <<
    std::endl;

  printDurationKindsDivisions (ss);

  msrInternalError (
    gServiceRunData->getInputSourceName (),
    inputLineNumber,
    __FILE__, __LINE__,
    ss.str ());

  return -1; // never reached
}

void msrDivisions::printDurationKindsDivisions (std::ostream& os)
{
  os <<
    "The mapping of durations to divisions with " <<
    fDivisionsPerQuarterNote << " divisions per quarter note" <<
    " is:" <<
    std::endl;

  ++gIndenter;

  if (fDurationKindsToDivisions.size ()) {
    std::list<std::pair<msrDurationKind, int> >::const_iterator
      iBegin = fDurationKindsToDivisions.begin (),
      iEnd   = fDurationKindsToDivisions.end (),
      i      = iBegin;

    for ( ; ; ) {
      os <<
        std::setw (6) << std::left <<
        msrDurationKindAsString (
          msrDurationKind ((*i).first)) <<
        ": " <<
        std::setw (5) << std::right <<
        (*i).second;

      if (++i == iEnd) break;

      os << std::endl;
    } // for

/* JMI

    for (
      std::list<std::pair<msrDuration, int> >::const_iterator i =
        fDurationsToDivisions.begin ();
      i != fDurationsToDivisions.end ();
      ++i
  ) {
      os <<
        std::setw (6) << std::left <<
        msrDurationAsString (msrDuration((*i).first)) <<
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

std::string msrDivisions::divisionsAsMsrString (
  int  inputLineNumber,
  int  divisions,
  int& numberOfDotsNeeded)
{
  std::string result;

  // the result is a base duration, followed by a suffix made of
  // either a sequence of dots or a multiplication factor

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMxsrOahGroup->getTraceDivisions ()) {
    const int fieldWidth = 16;

		std::stringstream ss;

    ss <<
     "--> divisionsAsMsrString ():" <<
      std::endl <<
      gTab << std::setw (fieldWidth) <<
      "inputLineNumber" << ": " << inputLineNumber <<
      std::endl <<
      gTab << std::setw (fieldWidth) <<
      "divisions" << ": " << divisions <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  msrDurationKind baseDurationKind          = msrDurationKind::kDuration1024th;
  int             baseDurationDivisions = -1;

  // search fDurationsToDivisions in longer to shortest order
  std::list<std::pair<msrDurationKind, int> >::const_iterator
    iBegin = fDurationKindsToDivisions.begin (),
    iEnd   = fDurationKindsToDivisions.end (),
    i      = iBegin;

  for ( ; ; ) {
    if (i == iEnd) {
      std::stringstream ss;

      ss <<
        "divisions " << divisions <<
        " could not be handled by divisionsAsMsrString () with:" <<
        std::endl;

      printDurationKindsDivisions (gLog);

      msrInternalError (
        gServiceRunData->getInputSourceName (),
        inputLineNumber,
        __FILE__, __LINE__,
        ss.str ());
      break;
    }

    if ((*i).second <= divisions) {
      // found base duration in list
      baseDurationKind      = (*i).first;
      baseDurationDivisions = (*i).second;

      result =
        msrDurationKindAsString (baseDurationKind);

#ifdef MF_TRACE_IS_ENABLED
      if (gGlobalMxsrOahGroup->getTraceDivisions ()) {
        const int fieldWidth = 22;

        gLog <<
            gTab << std::setw (fieldWidth) <<
          "divisions" << ": " << divisions <<
          std::endl << std::endl <<
            gTab << std::setw (fieldWidth) <<
          "baseDurationDivisions" << ": " << baseDurationDivisions <<
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

  if (divisions > baseDurationDivisions) {
    // divisions is not a power of 2 of a quarter note

    // the next element in the list is half as long as (*i)
    int remainingDivisions =
      divisions - baseDurationDivisions;
    int nextDivisionsInList =
      baseDurationDivisions / 2;

#ifdef MF_TRACE_IS_ENABLED
    if (gGlobalMxsrOahGroup->getTraceDivisions ()) {
      const int fieldWidth = 22;

      gLog <<
        gTab << std::setw (fieldWidth) <<
        "divisions" << ": " << divisions <<
        std::endl <<
        gTab << std::setw (fieldWidth) <<
      "baseDurationDivisions" << ": " << baseDurationDivisions <<
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
      Rational r (
        divisions,
        baseDurationDivisions);

#ifdef MF_TRACE_IS_ENABLED
      if (gGlobalMxsrOahGroup->getTraceDivisions ()) {
        const int fieldWidth = 22;

        gLog <<
          gTab << std::setw (fieldWidth) <<
          "divisions" << ": " << divisions <<
          std::endl <<
          gTab << std::setw (fieldWidth) <<
          "baseDurationDivisions" << ": " << baseDurationDivisions <<
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
        if (gGlobalMxsrOahGroup->getTraceDivisions ()) {
          const int fieldWidth = 22;

          gLog <<
            gTab << std::setw (fieldWidth) <<
            "divisions" << ": " << divisions <<
            std::endl <<
            gTab << std::setw (fieldWidth) <<
            "baseDurationDivisions" << ": " << baseDurationDivisions <<
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
      if (gGlobalMxsrOahGroup->getTraceDivisions ()) {
        const int fieldWidth = 24;

        gLog <<
          gTab << std::setw (fieldWidth) <<
          "divisions" << ": " << divisions <<
          std::endl <<
          gTab << std::setw (fieldWidth) <<
          "baseDurationDivisions" << ": " << baseDurationDivisions <<
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
  if (gGlobalMxsrOahGroup->getTraceDivisions ()) {
		std::stringstream ss;

    ss <<
      "<-- divisionsAsMsrString (): returns " << result <<
      std::endl << std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  return result;
}

std::string msrDivisions::divisionsAsMsrString (
  int  inputLineNumber,
  int  divisions)
{
  int numberOfDots; // to be ignored

  return
    divisionsAsMsrString (
      inputLineNumber,
      divisions,
      numberOfDots);
}

std::string msrDivisions::tupletDivisionsAsMsrString (
  int inputLineNumber,
  int divisions,
  int actualNotes,
  int normalNotes)
{
  return
    divisionsAsMsrString (
      inputLineNumber,
      divisions * actualNotes / normalNotes);
}

std::string tupletWholeNotesAsMsrString (
  int             inputLineNumber,
  const Rational& wholeNotes,
  int             actualNotes,
  int             normalNotes)
{
  return
    wholeNotesAsMsrString (
      inputLineNumber,
      wholeNotes
        *
      Rational (actualNotes, normalNotes));
}

void msrDivisions::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrDivisions::acceptIn ()" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msrDivisions>*
    p =
      dynamic_cast<visitor<S_msrDivisions>*> (v)) {
        S_msrDivisions elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrDivisions::visitStart ()" <<
            std::endl;

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void msrDivisions::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrDivisions::acceptOut ()" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msrDivisions>*
    p =
      dynamic_cast<visitor<S_msrDivisions>*> (v)) {
        S_msrDivisions elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrDivisions::visitEnd ()" <<
            std::endl;

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
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
    os << "[NONE]" << std::endl;
  }

  return os;
}


}
