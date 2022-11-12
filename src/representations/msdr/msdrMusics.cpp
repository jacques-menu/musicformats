/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <climits>      // INT_MIN, INT_MAX

#include "visitor.h"

#include "enableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "tracingOah.h"
#endif



#include "msdrMusics.h"

#include "msrOah.h"


namespace MusicFormats
{

//______________________________________________________________________________
S_msdrMusic msdrMusic::create (
  int           inputLineNumber,
  const std::string& musicName)
{
  msdrMusic* o =
    new msdrMusic (
      inputLineNumber,
      musicName);
  assert (o != nullptr);

  return o;
}

msdrMusic::msdrMusic (
  int           inputLineNumber,
  const std::string& musicName)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceVoices ()) {
    gLogStream <<
      "Creating MSDR music \"" << asString () << "\"" <<
      std::endl;
  }
#endif

  fMusicName = musicName;

  fInputLineNumber = inputLineNumber;
}

void msdrMusic::initializeMusic ()
{
  // JMI
}

msdrMusic::~msdrMusic ()
{}

void msdrMusic::addMeasureToMusic (
  int           inputLineNumber,
  int           measureNumber,
  S_msdrMeasure measure)
{
}

void msdrMusic::acceptIn (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msdrMusic::acceptIn ()" <<
      std::endl;
  }

  if (visitor<S_msdrMusic>*
    p =
      dynamic_cast<visitor<S_msdrMusic>*> (v)) {
        S_msdrMusic elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msdrMusic::visitStart ()" <<
            std::endl;
        }
        p->visitStart (elem);
  }
}

void msdrMusic::acceptOut (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msdrMusic::acceptOut ()" <<
      std::endl;
  }

  if (visitor<S_msdrMusic>*
    p =
      dynamic_cast<visitor<S_msdrMusic>*> (v)) {
        S_msdrMusic elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msdrMusic::visitEnd ()" <<
            std::endl;
        }
        p->visitEnd (elem);
  }
}

void msdrMusic::browseData (basevisitor* v)
{
/* JMI
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msdrMusic::browseData ()" <<
      std::endl;
  }

  // browse the voice initial elements
  if (fInitialMusicElementsList.size ()) {
    for (
      std::list<S_msdrMusicElement>::const_iterator i = fInitialMusicElementsList.begin ();
      i != fInitialMusicElementsList.end ();
      ++i
    ) {
      // browse the element
      msrBrowser<msdrMusicElement> browser (v);
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
      S_msrStanza stanza = (*i).second;

      if (stanza->getStanzaTextPresent ()) {
        // browse the stanza
        msrBrowser<msrStanza> browser (v);
        browser.browse (*(stanza));
      }
    } // for
  }

  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% <== msdrMusic::browseData ()" <<
      std::endl;
  }
  */
}

std::string msdrMusic::asShortString () const
{
  std::stringstream s;

  s <<
    "MSDR voice \"" << fMusicName << "\", " <<
    ", line " << fInputLineNumber;

  return s.str ();
}

std::string msdrMusic::asString () const
{
  std::stringstream s;

  s <<
    "MSDR music \"" << fMusicName << "\", " <<
    ", line " << fInputLineNumber;

  return s.str ();
}

void msdrMusic::displayMusic (
  int           inputLineNumber,
  const std::string& context) const
{
  gLogStream <<
    std::endl <<
    "*********>> Displaying MSDR music \"" <<
    fMusicName <<
    "\" (" << context << ")" <<
    ", line " << inputLineNumber <<
    " contains:" <<
    std::endl;

  ++gIndenter;
  print (gLogStream);
  --gIndenter;

  gLogStream <<
    " <<*********" <<
    std::endl << std::endl;
}

void msdrMusic::print (std::ostream& os) const
{
  os <<
    "MSDR music \"" << fMusicName <<
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
    "fVoiceUpLinkToStaff" << " : " <<
    fVoiceUpLinkToStaff->getStaffName () <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fVoiceCurrentMeasureNumber" << " : \"" <<
    fVoiceCurrentMeasureNumber <<
    "\"" <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fVoiceCurrentMeasureOrdinalNumber" << " : " <<
    fVoiceCurrentMeasureOrdinalNumber <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fVoiceCurrentMeasurePuristNumber" << " : " <<
    fVoiceCurrentMeasurePuristNumber <<
    std::endl <<

    std::setw (fieldWidth) <<
    "regularVoiceStaffSequentialNumber" << " : " <<
    regularVoiceStaffSequentialNumberAsString () <<
    std::endl;

#ifdef TRACING_IS_ENABLED
  // regular measure ends detection
  os << std::left <<
    std::setw (fieldWidth) <<
    "fWholeNotesSinceLastRegularMeasureEnd" << " : " <<
    fWholeNotesSinceLastRegularMeasureEnd <<
    std::endl;

  // incomplete measures after repeats detection
  os << std::left <<
    std::setw (fieldWidth) <<
    "fCurrentVoiceRepeatPhaseKind" << " : " <<
    msrVoiceRepeatPhaseKindAsString (
      fCurrentVoiceRepeatPhaseKind) <<
    std::endl;

  }
*/

  --gIndenter;
}

void msdrMusic::printShort (std::ostream& os) const
{
  os <<
    "MSDR music \"" << fMusicName <<
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

#ifdef TRACING_IS_ENABLED
  // print the voice measures flat std::list
  size_t voiceMeasuresFlatListSize =
    fVoiceMeasuresFlatList.size ();

  os <<
    std::setw (fieldWidth) <<
    "voiceMeasuresFlatList";
  if (voiceMeasuresFlatListSize) {
    os <<
      " : " <<  voiceMeasuresFlatListSize << " elements";
  }
  else {
    os <<
      " : " << "[EMPTY]";
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
      if (gGlobalTracingOahGroup->getTraceMeasures ()) {
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
#endif
*/

  --gIndenter;
}

std::ostream& operator << (std::ostream& os, const S_msdrMusic& elt)
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
