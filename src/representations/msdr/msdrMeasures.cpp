/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <climits>      // INT_MIN, INT_MAX

#include <vector>

#include "visitor.h"

#include "mfStaticSettings.h"

#include "msdrMeasures.h"

#include "oahOah.h"

#include "msrOah.h"

#include "waeHandlers.h"


namespace MusicFormats
{

//______________________________________________________________________________
S_msdrMeasure msdrMeasure::create (
  int           inputLineNumber,
  const std::string& measureNumber)
{
  msdrMeasure* obj =
    new msdrMeasure (
      inputLineNumber,
      measureNumber);
  assert (obj != nullptr);
  return obj;
}

msdrMeasure::msdrMeasure (
  int           inputLineNumber,
  const std::string& measureNumber)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceVoices ()) {
    std::stringstream ss;

    ss <<
      "Creating MSDR music \"" << asString () << "\"" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fMeasureNumber = measureNumber;

  fInputLineNumber = inputLineNumber;
}

void msdrMeasure::initializeMeasure ()
{
  // JMI
}

msdrMeasure::~msdrMeasure ()
{}

void msdrMeasure::addLayerToMeasure (
  int         inputLineNumber,
  int         layerNumber,
  const S_msdrLayer& layer)
{
}

void msdrMeasure::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msdrMeasure::acceptIn ()" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msdrMeasure>*
    p =
      dynamic_cast<visitor<S_msdrMeasure>*> (v)) {
        S_msdrMeasure elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msdrMeasure::visitStart ()" <<
            std::endl;

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void msdrMeasure::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msdrMeasure::acceptOut ()" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msdrMeasure>*
    p =
      dynamic_cast<visitor<S_msdrMeasure>*> (v)) {
        S_msdrMeasure elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msdrMeasure::visitEnd ()" <<
            std::endl;

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void msdrMeasure::browseData (basevisitor* v)
{
/* JMI
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msdrMeasure::browseData ()" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // browse the voice initial elements
  if (fInitialMusicLayersList.size ()) {
    for (
      std::list<S_msdrMeasureElement>::const_iterator i = fInitialMusicLayersList.begin ();
      i != fInitialMusicLayersList.end ();
      ++i
    ) {
      // browse the element
      msrBrowser<msrMusicElement> browser (v);
      browser.browse (*(*i));
    } // for
  }

  if (fMusicLastSegment) {
    // browse the voice last segment
    msrBrowser<msrSegment> browser (v);
    browser.browse (*fMusicLastSegment);
  }

  // browse the voice stanzas
  if (fMusicStanzasMap.size ()) {
    for (
      std::map<std::string, S_msrStanza>::const_iterator i = fMusicStanzasMap.begin ();
      i != fMusicStanzasMap.end ();
      ++i
    ) {
      const S_msrStanza& stanza = (*i).second;

      if (stanza->getStanzaTextPresent ()) {
        // browse the stanza
        msrBrowser<msrStanza> browser (v);
        browser.browse (*(stanza));
      }
    } // for
  }

#ifdef MF_TRACING_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% <== msdrMeasure::browseData ()" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
  */
}

std::string msdrMeasure::asShortString () const
{
  std::stringstream ss;

  ss <<
    "MSDR measure \"" << fMeasureNumber << "\"" <<
    ", line " << fInputLineNumber;

  return ss.str ();
}

std::string msdrMeasure::asString () const
{
  std::stringstream ss;

  ss <<
    "MSDR measure \"" << fMeasureNumber << "\"" <<
    ", line " << fInputLineNumber;

  return ss.str ();
}

void msdrMeasure::displayMeasure (
  int           inputLineNumber,
  const std::string& context) const
{
  gLog <<
    std::endl <<
    "*********>> Displaying MSDR measure \"" <<
    fMeasureNumber <<
    "\" (" << context << ")" <<
    ", line " << inputLineNumber <<
    " contains:" <<
    std::endl;

  ++gIndenter;
  print (gLog);
  --gIndenter;

  gLog <<
    " <<*********" <<
    std::endl << std::endl;
}

void msdrMeasure::printFull (std::ostream& os) const
{
  os <<
    "MSDR measure \"" << fMeasureNumber <<
    "\", line " << fInputLineNumber <<
    std::endl;

  ++gIndenter;

/* JMI
  os << std::left <<
    '(' <<
    mfSingularOrPlural (
      fVoiceActualHarmoniesCounter, "harmony", "harmonies") <<
     ", " <<
    mfSingularOrPlural (
      fVoiceActualFiguredBassesCounter, "figured bass", "figured bass elements") <<
     ", " <<
    mfSingularOrPlural (
      fVoiceActualNotesCounter, "actual note", "actual notes") <<
     ", " <<
    mfSingularOrPlural (
      fVoiceRestsCounter, "rest", "rests") <<
     ", " <<
    mfSingularOrPlural (
      fVoiceSkipsCounter, "skip", "skips") <<
     ", " <<
    mfSingularOrPlural (
      fVoiceStanzasMap.size (), "stanza", "stanzas") <<
    ")" <<
    std::endl;

  const int fieldWidth = 41;

  os << std::left <<
    std::setw (fieldWidth) <<
    "upLinkToStaff" << ": " <<
    fVoiceUpLinkToStaff->getStaffName () <<
    std::endl <<

    std::setw (fieldWidth) <<
    "voiceCurrentMeasureNumber" << ": \"" <<
    fVoiceCurrentMeasureNumber <<
    "\"" <<
    std::endl <<

    std::setw (fieldWidth) <<
    "voiceCurrentMeasureOrdinalNumber" << ": " <<
    fVoiceCurrentMeasureOrdinalNumber <<
    std::endl <<

    std::setw (fieldWidth) <<
    "voiceCurrentMeasurePuristNumber" << ": " <<
    fVoiceCurrentMeasurePuristNumber <<
    std::endl <<

    std::setw (fieldWidth) <<
    "regularVoiceStaffSequentialNumber" << ": " <<
    regularVoiceStaffSequentialNumberAsString () <<
    std::endl;

#ifdef MF_TRACE_IS_ENABLED
  // regular measure ends detection
  os << std::left <<
    std::setw (fieldWidth) <<
    "fWholeNotesSinceLastRegularMeasureEnd" << ": " <<
    fWholeNotesSinceLastRegularMeasureEnd <<
    std::endl;

  // incomplete measures after repeats detection
  os << std::left <<
    std::setw (fieldWidth) <<
    "fCurrentVoiceRepeatPhaseKind" << ": " <<
    msrVoiceRepeatPhaseKindAsString (
      fCurrentVoiceRepeatPhaseKind) <<
    std::endl;

  }
*/

  --gIndenter;
}

void msdrMeasure::print (std::ostream& os) const
{
  os <<
    "MSDR measure number '" <<
    fMeasureNumber <<
    "', line " << fInputLineNumber <<
    std::endl;

  ++gIndenter;

/* JMI
  os << std::left <<
    '(' <<
    mfSingularOrPlural (
      fVoiceActualHarmoniesCounter, "harmony", "harmonies") <<
     ", " <<
    mfSingularOrPlural (
      fVoiceActualFiguredBassesCounter, "figured bass", "figured bass elements") <<
     ", " <<
    mfSingularOrPlural (
      fVoiceActualNotesCounter, "actual note", "actual notes") <<
     ", " <<
    mfSingularOrPlural (
      fVoiceRestsCounter, "rest", "rests") <<
     ", " <<
    mfSingularOrPlural (
      fVoiceSkipsCounter, "skip", "skips") <<
     ", " <<
    mfSingularOrPlural (
      fVoiceStanzasMap.size (), "stanza", "stanzas") <<
    ")" <<
    std::endl;

  const int fieldWidth = 41;

#ifdef MF_TRACE_IS_ENABLED
  // print the voice measures flat list
  size_t voiceMeasuresFlatListSize =
    fVoiceMeasuresFlatList.size ();

  os <<
    std::setw (fieldWidth) <<
    "voiceMeasuresFlatList";
  if (voiceMeasuresFlatListSize) {
    os <<
      ": " <<  voiceMeasuresFlatListSize << " elements";
  }
  else {
    os <<
      ": " << "[EMPTY]";
  }
  os << std::endl;

  if (voiceMeasuresFlatListSize) {
    ++gIndenter;

    std::list<S_msrMeasure>::const_iterator
      iBegin = fVoiceMeasuresFlatList.begin (),
      iEnd   = fVoiceMeasuresFlatList.end (),
      i      = iBegin;

    for ( ; ; ) {
      // print the measure
      if (gTraceOahGroup->getTraceMeasures ()) {
        os << (*i)->asShortString ();
      }
      else {
        os << (*i)->getMeasureNumber ();
      }
      if (++i == iEnd) break;
      os << ' ';
    } // for
    os << std::endl;

    --gIndenter;
  }
#endif // MF_TRACE_IS_ENABLED
*/

  --gIndenter;
}

std::ostream& operator << (std::ostream& os, const S_msdrMeasure& elt)
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
