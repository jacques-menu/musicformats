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

#include "bsrWae.h"

#include "enableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "tracingOah.h"
#endif

#include "mfServiceRunData.h"

#include "msrBasicTypes.h"
#include "msrTempos.h"

#include "bsrTempos.h"

#include "bsrNotes.h"
#include "bsrNumbers.h"

#include "oahEarlyOptions.h"

#include "oahOah.h"
#include "waeOah.h"

#include "bsrOah.h"
#include "msr2bsrOah.h"


using namespace std;

namespace MusicFormats
{

//______________________________________________________________________________
string tempoKindAsDebugString (
  msrTempo::msrTempoKind tempoKind)
{
  string result;

  switch (tempoKind) {
    case msrTempo::k_NoTempoKind:
      result = "_";
      break;
    case msrTempo::kTempoBeatUnitsWordsOnly:
      result = "WordsOnly";
      break;
    case msrTempo::kTempoBeatUnitsPerMinute:
      result = "PerMinute";
      break;
    case msrTempo::kTempoBeatUnitsEquivalence:
      result = "Equivalence";
      break;
    case msrTempo::kTempoNotesRelationship:
      result = "NotesRelationShip";
      break;
  } // switch

  return result;
}

//______________________________________________________________________________
S_bsrTempo bsrTempo::create (
  int        inputLineNumber,
  S_msrTempo mTempo)
{
  bsrTempo* o =
    new bsrTempo (
      inputLineNumber, mTempo);
  assert (o != nullptr);
  return o;
}

bsrTempo::bsrTempo (
  int        inputLineNumber,
  S_msrTempo mTempo)
    : bsrLineContentsElement (inputLineNumber)
{
  fMsrTempo  = mTempo;

  fTempoCellsList = buildCellsList ();
}

bsrTempo::~bsrTempo ()
{}

S_bsrCellsList bsrTempo::buildCellsList () const
{
  S_bsrCellsList
    result =
      bsrCellsList::create (
        fInputLineNumber, kCellWordSign);

  if (! gGlobalMsr2bsrOahGroup->getNoTempos ()) {
    switch (fMsrTempo->getTempoKind ()) {
      case msrTempo::k_NoTempoKind:
        break;

      case msrTempo::kTempoBeatUnitsWordsOnly:
        break;

      case msrTempo::kTempoBeatUnitsPerMinute:
        {
          // fetch MSR tempo attributes
          const list<S_msrWords>&
            tempoWordsList =
              fMsrTempo->getTempoWordsList ();

          msrDottedDuration
            tempoDottedDuration =
              fMsrTempo->getTempoBeatUnit ();

          // handle tempo words
          unsigned int tempoWordsListSize = tempoWordsList.size ();

          if (tempoWordsListSize) {
            list<S_msrWords>::const_iterator
              iBegin = tempoWordsList.begin (),
              iEnd   = tempoWordsList.end (),
              i      = iBegin;

            for ( ; ; ) {
              S_msrWords words = (*i);

              // JMI ???

              if (++i == iEnd) break;
            } // for
          }

          // handle tempo duration and continuation dots number
          msrDurationKind
            durationKind =
              tempoDottedDuration.getDurationKind ();
          int
            dotsNumber =
              tempoDottedDuration.getDotsNumber ();

          // determine the note sign
          bsrNoteValueKind
            noteValueKind = bsrNoteValueKind::kNoteValueNone;

          switch (durationKind) {
            case msrDurationKind::k_NoDuration:
              break;

            case msrDurationKind::k1024th: // JMI
              break;
            case msrDurationKind::k512th: // JMI
              break;
            case msrDurationKind::k256th:
              noteValueKind = bsrNoteValueKind::kNoteValueC256th;
              break;
            case msrDurationKind::k128th:
              noteValueKind = bsrNoteValueKind::kNoteValueC128th;
              break;
            case msrDurationKind::k64th:
              noteValueKind = bsrNoteValueKind::kNoteValueC64th;
              break;
            case msrDurationKind::k32nd:
              noteValueKind = bsrNoteValueKind::kNoteValueC32nd;
              break;
            case msrDurationKind::k16th:
              noteValueKind = bsrNoteValueKind::kNoteValueC16th;
              break;
            case msrDurationKind::kEighth:
              noteValueKind = bsrNoteValueKind::kNoteValueC8th;
              break;
            case msrDurationKind::kQuarter:
              noteValueKind = bsrNoteValueKind::kNoteValueCQuarter;
              break;
            case msrDurationKind::kHalf:
              noteValueKind = bsrNoteValueKind::kNoteValueCHalf;
              break;
            case msrDurationKind::kWhole:
              noteValueKind = bsrNoteValueKind::kNoteValueCWhole;
              break;
            case msrDurationKind::kBreve:
              noteValueKind = bsrNoteValueKind::kNoteValueCBreve;
              break;
            case msrDurationKind::kLonga:
              break;
            case msrDurationKind::kMaxima:
              break;
          } // switch

          // create a note to represent the duration
          S_bsrNote
            bNote =
              bsrNote::create (
                fInputLineNumber,
                noteValueKind,
                dotsNumber,
                bsrNoteOctaveKind::kNoteOctaveNone,
                bsrNoteOctaveIsNeeded::kNoteOctaveIsNeededNo,
                bsrAccidentalKind::kAccidentalNone);

          // append its cells to result
          result->
            appendCellsListToCellsList (
              bNote->fetchCellsList ());

          // append an equals to result
          result->
            appendCellKindToCellsList (
              kCellTempoEquals);

          // handle per minute value
          string
            tempoPerMinuteString =
              fMsrTempo->getTempoPerMinute ();

          int
            perMinuteMin = -1,
            perMinuteMax = -1; // may be absent

          // decipher it to extract min and max values
          string regularExpression (
            "[[:space:]]*([[:digit:]]+)[[:space:]]*" // perMinuteMin
            "-"
            "[[:space:]]*([[:digit:]]+)[[:space:]]*" // perMinuteMax
            );

          regex e (regularExpression);
          smatch sm;

          regex_match (tempoPerMinuteString, sm, e);

          unsigned int smSize = sm.size ();

#ifdef TRACING_IS_ENABLED
          if (gGlobalTracingOahGroup->getTraceTempos () && ! gGlobalOahEarlyOptions.getEarlyQuietOption ()) {
            gLogStream <<
              "There are " << smSize << " matches" <<
              " for rational string '" << tempoPerMinuteString <<
              "' with regex '" << regularExpression <<
              "'" <<
              endl;

            for (unsigned i = 0; i < smSize; ++i) {
              gLogStream <<
                '[' << sm [i] << "] ";
            } // for

            gLogStream << endl;
          }
#endif

          if (smSize == 3) {
            {
              stringstream s;
              s << sm [1];
              s >> perMinuteMin;
            }
            {
              stringstream s;
              s << sm [2];
              s >> perMinuteMax;
            }
          }

          else {
            // decipher it to extract min (and only) values
            string regularExpression (
              "[[:space:]]*([[:digit:]]+)[[:space:]]*" // perMinuteMin
              );

            regex e (regularExpression);
            smatch sm;

            regex_match (tempoPerMinuteString, sm, e);

            unsigned int smSize = sm.size ();

#ifdef TRACING_IS_ENABLED
            if (gGlobalTracingOahGroup->getTraceTempos () && ! gGlobalOahEarlyOptions.getEarlyQuietOption ()) {
              gLogStream <<
                "There are " << smSize << " matches" <<
                " for rational string '" << tempoPerMinuteString <<
                "' with regex '" << regularExpression <<
                "'" <<
                endl;

              for (unsigned i = 0; i < smSize; ++i) {
                gLogStream <<
                  '[' << sm [i] << "] ";
              } // for

              gLogStream << endl;
            }
#endif

            if (smSize == 2) {
              stringstream s;
              s << sm [1];
              s >> perMinuteMin;
            }

            else {
              stringstream s;

              s <<
                "tempoPerMinuteString '" <<
                tempoPerMinuteString <<
                "' is ill-formed" <<
                ", line " << fInputLineNumber;

              bsrInternalError (
                gGlobalServiceRunData->getInputSourceName (),
                fInputLineNumber,
                __FILE__, __LINE__,
                s.str ());
            }
          }

/* JMI
          gLogStream << // JMI
            "% ==> bsrTempo::buildCellsList ()" <<
            ", tempoPerMinuteString = " << tempoPerMinuteString <<
            ", perMinuteMin = " << perMinuteMin <<
            ", perMinuteMax = " << perMinuteMax <<
            endl;
            */

          // create a number to represent perMinuteMin
          S_bsrNumber
            perMinuteNumber =
              bsrNumber::create (
                fInputLineNumber,
                perMinuteMin,
                bsrNumber::kNumberSignIsNeededYes);

          // append its cells to result
          result->
            appendCellsListToCellsList (
              perMinuteNumber->getNumberCellsList ());

          if (perMinuteMax > 0) {
            // append a hyphen to result
            result->
              appendCellKindToCellsList (
                kCellTempoHyphen);

            // create a number to represent perMinuteMax
            S_bsrNumber
              perMinuteNumber =
                bsrNumber::create (
                  fInputLineNumber,
                  perMinuteMax,
                  bsrNumber::kNumberSignIsNeededYes);

            // append its cells to result
            result->
              appendCellsListToCellsList (
                perMinuteNumber->getNumberCellsList ());
          }
        }
        break;

      case msrTempo::kTempoBeatUnitsEquivalence:
        break;

      case msrTempo::kTempoNotesRelationship:
        break;
    } // switch
  }

  return result;
}

int bsrTempo::fetchCellsNumber() const
{
  return fTempoCellsList->fetchCellsNumber();
}

void bsrTempo::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
    gLogStream <<
      "% ==> bsrTempo::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_bsrTempo>*
    p =
      dynamic_cast<visitor<S_bsrTempo>*> (v)) {
        S_bsrTempo elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
          gLogStream <<
            "% ==> Launching bsrTempo::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void bsrTempo::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
    gLogStream <<
      "% ==> bsrTempo::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_bsrTempo>*
    p =
      dynamic_cast<visitor<S_bsrTempo>*> (v)) {
        S_bsrTempo elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
          gLogStream <<
            "% ==> Launching bsrTempo::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void bsrTempo::browseData (basevisitor* v)
{}

string bsrTempo::asString () const
{
  stringstream s;

  s <<
    "Tempo" <<
    ", tempoKind: " <<
    msrTempo::tempoKindAsString (fMsrTempo->getTempoKind ()) <<
    ", tempoCellsList: " <<
    fTempoCellsList->asShortString () <<
    ", spacesBefore: " << fSpacesBefore <<
    ", line " << fInputLineNumber;

  return s.str ();
}

string bsrTempo::asDebugString () const
{
  stringstream s;

  s <<
    "[TEMPO ";

  switch (fMsrTempo->getTempoKind ()) {
    case msrTempo::k_NoTempoKind:
      s << "_";
      break;
    case msrTempo::kTempoBeatUnitsWordsOnly:
      s << "WordsOnly";
      break;
    case msrTempo::kTempoBeatUnitsPerMinute:
      {
        msrDottedDuration
          tempoBeatUnit =
            fMsrTempo->getTempoBeatUnit ();

        s <<
          tempoBeatUnit.asString () <<
          " = " <<
          fMsrTempo->getTempoPerMinute () <<
          "/mn";
      }
      break;
    case msrTempo::kTempoBeatUnitsEquivalence:
      s << "Equivalence";
      break;
    case msrTempo::kTempoNotesRelationship:
      s << "NotesRelationShip";
      break;
  } // switch
 // JMI   tempoKindAsDebugString (fMsrTempo->getTempoKind ());

  s << "]";

  return s.str ();
}

void bsrTempo::print (ostream& os) const
{
  os <<
    "Tempo" <<
    ", line " << fInputLineNumber <<
    endl;

  ++gIndenter;

  const int fieldWidth = 23;

  os <<
    setw (fieldWidth) <<
    "tempoKind" << " : " <<
    msrTempo::tempoKindAsString (fMsrTempo->getTempoKind ()) <<
    endl <<
    setw (fieldWidth) <<
    "tempoCellsList" << " : " <<
    fTempoCellsList->asShortString () <<
    endl <<
    setw (fieldWidth) <<
    "spacesBefore" << " : " << fSpacesBefore <<
    endl;

  --gIndenter;
}

ostream& operator<< (ostream& os, const S_bsrTempo& elt)
{
  elt->print (os);
  return os;
}


}
