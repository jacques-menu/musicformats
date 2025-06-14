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
#include <regex>

#include "visitor.h"

#include "bsrWae.h"

#include "mfPreprocessorSettings.h"

#include "mfServices.h"

#include "msrTempos.h"

#include "bsrTempos.h"

#include "bsrNotes.h"
#include "bsrNumbers.h"

#include "oahEarlyOptions.h"

#include "oahOah.h"
#include "waeOah.h"

#include "bsrOah.h"
#include "msr2bsrOah.h"

#include "waeHandlers.h"


namespace MusicFormats
{

//______________________________________________________________________________
// std::string tempoKindAsDebugString ( // JMI 0.9.66
//   msrTempoKBeatUnitsKind tempoKind)
// {
//   std::string result;
//
//   switch (tempoKind) {
//     case msrTempoKBeatUnitsKind::kTempoBeatUnits_UNKNOWN_:
//       result = "_";
//       break;
//     case msrTempoKBeatUnitsKind::kTempoBeatUnitsWordsOnly:
//       result = "WordsOnly";
//       break;
//     case msrTempoKBeatUnitsKind::kTempoBeatUnitsPerMinute:
//       result = "PerMinute";
//       break;
//     case msrTempoKBeatUnitsKind::kTempoBeatUnitsEquivalence:
//       result = "Equivalence";
//       break;
//     case msrTempoKBeatUnitsKind::kTempoNotesRelationship:
//       result = "NotesRelationShip";
//       break;
//   } // switch
//
//   return result;
// }

//______________________________________________________________________________
S_bsrTempo bsrTempo::create (
  const mfInputLineNumber& inputLineNumber,
  const S_msrTempo& mTempo)
{
  bsrTempo* obj =
    new bsrTempo (
      inputLineNumber, mTempo);
  assert (obj != nullptr);
  return obj;
}

bsrTempo::bsrTempo (
  const mfInputLineNumber& inputLineNumber,
  const S_msrTempo& mTempo)
    : bsrLineContentsElement (inputLineNumber)
{
  fMsrTempo = mTempo;

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
      case msrTempoKBeatUnitsKind::kTempoBeatUnits_UNKNOWN_:
        break;

      case msrTempoKBeatUnitsKind::kTempoBeatUnitsWordsOnly:
        break;

      case msrTempoKBeatUnitsKind::kTempoBeatUnitsPerMinute:
        {
          // fetch MSR tempo attributes
          const std::list <S_msrWords>&
            tempoWordsList =
              fMsrTempo->getTempoWordsList ();

          mfDottedNotesDuration
            tempoDottedNotesDuration =
              fMsrTempo->getTempoBeatUnit ();

          // handle tempo words
          size_t tempoWordsListSize = tempoWordsList.size ();

          if (tempoWordsListSize) {
            std::list <S_msrWords>::const_iterator
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
          mfDurationKind
            notesDurationKind =
              tempoDottedNotesDuration.getNotesDurationKind ();
          int
            dotsNumber =
              tempoDottedNotesDuration.getDotsNumber ();

          // determine the note sign
          bsrNoteValueKind
            noteValueKind = bsrNoteValueKind::kNoteValueNone;

          switch (notesDurationKind) {
            case mfDurationKind::kDuration_UNKNOWN_:
              break;

            case mfDurationKind::kDuration1024th: // JMI
              break;
            case mfDurationKind::kDuration512th: // JMI
              break;
            case mfDurationKind::kDuration256th:
              noteValueKind = bsrNoteValueKind::kNoteValueC256th;
              break;
            case mfDurationKind::kDuration128th:
              noteValueKind = bsrNoteValueKind::kNoteValueC128th;
              break;
            case mfDurationKind::kDuration64th:
              noteValueKind = bsrNoteValueKind::kNoteValueC64th;
              break;
            case mfDurationKind::kDuration32nd:
              noteValueKind = bsrNoteValueKind::kNoteValueC32nd;
              break;
            case mfDurationKind::kDuration16th:
              noteValueKind = bsrNoteValueKind::kNoteValueC16th;
              break;
            case mfDurationKind::kDurationEighth:
              noteValueKind = bsrNoteValueKind::kNoteValueC8th;
              break;
            case mfDurationKind::kDurationQuarter:
              noteValueKind = bsrNoteValueKind::kNoteValueCQuarter;
              break;
            case mfDurationKind::kDurationHalf:
              noteValueKind = bsrNoteValueKind::kNoteValueCHalf;
              break;
            case mfDurationKind::kDurationWhole:
              noteValueKind = bsrNoteValueKind::kNoteValueCWhole;
              break;
            case mfDurationKind::kDurationBreve:
              noteValueKind = bsrNoteValueKind::kNoteValueCBreve;
              break;
            case mfDurationKind::kDurationLonga:
              break;
            case mfDurationKind::kDurationMaxima:
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
          std::string
            tempoPerMinuteString =
              fMsrTempo->getTempoPerMinute ();

          int
            perMinuteMin = -1,
            perMinuteMax = -1; // may be absent

          // decipher it to extract min and max values
          std::string regularExpression (
            "[[:space:]]*([[:digit:]]+)[[:space:]]*" // perMinuteMin
            "-"
            "[[:space:]]*([[:digit:]]+)[[:space:]]*" // perMinuteMax
            );

          std::regex e (regularExpression);
          std::smatch sm;

          regex_match (tempoPerMinuteString, sm, e);

          size_t smSize = sm.size ();

#ifdef MF_TRACE_IS_ENABLED
          if (gTraceOahGroup->getTraceTempos () && ! gEarlyOptions.getEarlyQuietOption ()) {
            gLog <<
              "There are " << smSize << " matches" <<
              " for mfRational string '" << tempoPerMinuteString <<
              "' with std::regex '" << regularExpression <<
              '\'' <<
              std::endl;

            for (unsigned i = 0; i < smSize; ++i) {
              gLog <<
                '[' << sm [i] << "] ";
            } // for

            gLog << std::endl;
          }
#endif // MF_TRACE_IS_ENABLED

          if (smSize == 3) {
            {
              std::stringstream ss;
              ss << sm [1];
              ss >> perMinuteMin;
            }
            {
              std::stringstream ss;
              ss << sm [2];
              ss >> perMinuteMax;
            }
          }

          else {
            // decipher it to extract min (and only) values
            std::string regularExpression (
              "[[:space:]]*([[:digit:]]+)[[:space:]]*" // perMinuteMin
              );

            std::regex e (regularExpression);
            std::smatch sm;

            regex_match (tempoPerMinuteString, sm, e);

            size_t smSize = sm.size ();

#ifdef MF_TRACE_IS_ENABLED
            if (gTraceOahGroup->getTraceTempos () && ! gEarlyOptions.getEarlyQuietOption ()) {
              gLog <<
                "There are " << smSize << " matches" <<
                " for mfRational string '" << tempoPerMinuteString <<
                "' with std::regex '" << regularExpression <<
                '\'' <<
                std::endl;

              for (unsigned i = 0; i < smSize; ++i) {
                gLog <<
                  '[' << sm [i] << "] ";
              } // for

              gLog << std::endl;
            }
#endif // MF_TRACE_IS_ENABLED

            if (smSize == 2) {
              std::stringstream ss;
              ss << sm [1];
              ss >> perMinuteMin;
            }

            else {
              std::stringstream ss;

              ss <<
                "tempoPerMinuteString '" <<
                tempoPerMinuteString <<
                "' is ill-formed" <<
                ", line " << fInputLineNumber;

              bsrInternalError (
                gServiceRunData->getInputSourceName (),
                fInputLineNumber,
                __FILE__, mfInputLineNumber (__LINE__),
                ss.str ());
            }
          }

/* JMI
          gLog << // JMI
            "% ==> bsrTempo::buildCellsList ()" <<
            ", tempoPerMinuteString: " << tempoPerMinuteString <<
            ", perMinuteMin: " << perMinuteMin <<
            ", perMinuteMax: " << perMinuteMax <<
            std::endl;
            */

          // create a number to represent perMinuteMin
          S_bsrNumber
            perMinuteNumber =
              bsrNumber::create (
                fInputLineNumber,
                perMinuteMin,
                bsrNumberSignIsNeededKind::kNumberSignIsNeededYes);

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
                  bsrNumberSignIsNeededKind::kNumberSignIsNeededYes);

            // append its cells to result
            result->
              appendCellsListToCellsList (
                perMinuteNumber->getNumberCellsList ());
          }
        }
        break;

      case msrTempoKBeatUnitsKind::kTempoBeatUnitsEquivalence:
        break;

      case msrTempoKBeatUnitsKind::kTempoNotesRelationship:
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
#ifdef MF_TRACE_IS_ENABLED
  if (gBsrOahGroup->getTraceBsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% --> End visiting bsrTranscriptionNotesElement" <<
      "% ==> bsrTempo::acceptIn ()";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_bsrTempo>*
    p =
      dynamic_cast<visitor<S_bsrTempo>*> (v)) {
        S_bsrTempo elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gBsrOahGroup->getTraceBsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching bsrTempo::visitStart ()";

          gWaeHandler->waeTrace (
            __FILE__, mfInputLineNumber (__LINE__),
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void bsrTempo::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gBsrOahGroup->getTraceBsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% --> End visiting bsrTranscriptionNotesElement" <<
      "% ==> bsrTempo::acceptOut ()";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_bsrTempo>*
    p =
      dynamic_cast<visitor<S_bsrTempo>*> (v)) {
        S_bsrTempo elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gBsrOahGroup->getTraceBsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching bsrTempo::visitEnd ()";

          gWaeHandler->waeTrace (
            __FILE__, mfInputLineNumber (__LINE__),
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void bsrTempo::browseData (basevisitor* v)
{}

std::string bsrTempo::asString () const
{
  std::stringstream ss;

  ss <<
    "Tempo" <<
    ", tempoKind: " <<
    msrTempoBeatUnitsKindAsString (fMsrTempo->getTempoKind ()) <<
    ", tempoCellsList: " <<
    fTempoCellsList->asShortString () <<
    ", spacesBefore: " << fSpacesBefore <<
    ", line " << fInputLineNumber;

  return ss.str ();
}

std::string bsrTempo::asDebugString () const
{
  std::stringstream ss;

  ss <<
    "[TEMPO ";

  switch (fMsrTempo->getTempoKind ()) {
    case msrTempoKBeatUnitsKind::kTempoBeatUnits_UNKNOWN_:
      ss << "_";
      break;
    case msrTempoKBeatUnitsKind::kTempoBeatUnitsWordsOnly:
      ss << "WordsOnly";
      break;
    case msrTempoKBeatUnitsKind::kTempoBeatUnitsPerMinute:
      {
        mfDottedNotesDuration
          tempoBeatUnit =
            fMsrTempo->getTempoBeatUnit ();

        ss <<
          tempoBeatUnit.asString () <<
          ": " <<
          fMsrTempo->getTempoPerMinute () <<
          "/mn";
      }
      break;
    case msrTempoKBeatUnitsKind::kTempoBeatUnitsEquivalence:
      ss << "Equivalence";
      break;
    case msrTempoKBeatUnitsKind::kTempoNotesRelationship:
      ss << "NotesRelationShip";
      break;
  } // switch

  ss << ']';

  return ss.str ();
}

void bsrTempo::print (std::ostream& os) const
{
  os <<
    "Tempo" <<
    ", line " << fInputLineNumber <<
    std::endl;

  ++gIndenter;

  constexpr int fieldWidth = 23;

  os <<
    std::setw (fieldWidth) <<
    "tempoKind" << ": " <<
    msrTempoBeatUnitsKindAsString (fMsrTempo->getTempoKind ()) <<
    std::endl <<
    std::setw (fieldWidth) <<
    "tempoCellsList" << ": " <<
    fTempoCellsList->asShortString () <<
    std::endl <<
    std::setw (fieldWidth) <<
    "spacesBefore" << ": " << fSpacesBefore <<
    std::endl;

  --gIndenter;
}

std::ostream& operator << (std::ostream& os, const S_bsrTempo& elt)
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
