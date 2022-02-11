/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <climits>      // INT_MIN, INT_MAX

#include <vector>

#include "visitor.h"

#include "enableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "tracingOah.h"
#endif



#include "msdrMeasures.h"

#include "oahOah.h"

#include "msrOah.h"


using namespace std;

namespace MusicFormats
{

//______________________________________________________________________________
S_msdrMeasure msdrMeasure::create (
  int           inputLineNumber,
  const string& measureNumber)
{
  msdrMeasure* o =
    new msdrMeasure (
      inputLineNumber,
      measureNumber);
  assert (o != nullptr);

  return o;
}

msdrMeasure::msdrMeasure (
  int           inputLineNumber,
  const string& measureNumber)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceVoices ()) {
    gLogStream <<
      "Creating MSDR music \"" << asString () << "\"" <<
      endl;
  }
#endif

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
  S_msdrLayer layer)
{
}

void msdrMeasure::acceptIn (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msdrMeasure::acceptIn ()" <<
      endl;
  }

  if (visitor<S_msdrMeasure>*
    p =
      dynamic_cast<visitor<S_msdrMeasure>*> (v)) {
        S_msdrMeasure elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msdrMeasure::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msdrMeasure::acceptOut (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msdrMeasure::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msdrMeasure>*
    p =
      dynamic_cast<visitor<S_msdrMeasure>*> (v)) {
        S_msdrMeasure elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msdrMeasure::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msdrMeasure::browseData (basevisitor* v)
{
/* JMI
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msdrMeasure::browseData ()" <<
      endl;
  }

  // browse the voice initial elements
  if (fInitialMusicLayersList.size ()) {
    for (
      list<S_msdrMeasureElement>::const_iterator i = fInitialMusicLayersList.begin ();
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
      map<string, S_msrStanza>::const_iterator i = fMusicStanzasMap.begin ();
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
      "% <== msdrMeasure::browseData ()" <<
      endl;
  }
  */
}

string msdrMeasure::asShortString () const
{
  stringstream s;

  s <<
    "MSDR measure \"" << fMeasureNumber << "\"" <<
    ", line " << fInputLineNumber;

  return s.str ();
}

string msdrMeasure::asString () const
{
  stringstream s;

  s <<
    "MSDR measure \"" << fMeasureNumber << "\"" <<
    ", line " << fInputLineNumber;

  return s.str ();
}

void msdrMeasure::displayMeasure (
  int           inputLineNumber,
  const string& context) const
{
  gLogStream <<
    endl <<
    "*********>> Displaying MSDR measure \"" <<
    fMeasureNumber <<
    "\" (" << context << ")" <<
    ", line " << inputLineNumber <<
    " contains:" <<
    endl;

  ++gIndenter;
  print (gLogStream);
  --gIndenter;

  gLogStream <<
    " <<*********" <<
    endl << endl;
}

void msdrMeasure::print (ostream& os) const
{
  os <<
    "MSDR measure \"" << fMeasureNumber <<
    "\", line " << fInputLineNumber <<
    endl;

  ++gIndenter;

/* JMI
  os << left <<
    "(" <<
    mfSingularOrPlural (
      fVoiceActualHarmoniesCounter, "harmony", "harmonies") <<
     ", " <<
    mfSingularOrPlural (
      fVoiceActualFiguredBassElementsCounter, "figured bass", "figured bass elements") <<
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
    endl;

  const int fieldWidth = 41;

  os << left <<
    setw (fieldWidth) <<
    "staffUpLink" << " : " <<
    fVoiceStaffUpLink->getStaffName () <<
    endl <<

    setw (fieldWidth) <<
    "voiceCurrentMeasureNumber" << " : \"" <<
    fVoiceCurrentMeasureNumber <<
    "\"" <<
    endl <<

    setw (fieldWidth) <<
    "voiceCurrentMeasureOrdinalNumber" << " : " <<
    fVoiceCurrentMeasureOrdinalNumber <<
    endl <<

    setw (fieldWidth) <<
    "voiceCurrentMeasurePuristNumber" << " : " <<
    fVoiceCurrentMeasurePuristNumber <<
    endl <<

    setw (fieldWidth) <<
    "regularVoiceStaffSequentialNumber" << " : " <<
    regularVoiceStaffSequentialNumberAsString () <<
    endl;

#ifdef TRACING_IS_ENABLED
  // regular measure ends detection
  os << left <<
    setw (fieldWidth) <<
    "fWholeNotesSinceLastRegularMeasureEnd" << " : " <<
    fWholeNotesSinceLastRegularMeasureEnd <<
    endl;

  // incomplete measures after repeats detection
  os << left <<
    setw (fieldWidth) <<
    "fCurrentVoiceRepeatPhaseKind" << " : " <<
    voiceRepeatPhaseKindAsString (
      fCurrentVoiceRepeatPhaseKind) <<
    endl;

  }
*/

  --gIndenter;
}

void msdrMeasure::printShort (ostream& os) const
{
  os <<
    "MSDR measure number '" <<
    fMeasureNumber <<
    "', line " << fInputLineNumber <<
    endl;

  ++gIndenter;

/* JMI
  os << left <<
    "(" <<
    mfSingularOrPlural (
      fVoiceActualHarmoniesCounter, "harmony", "harmonies") <<
     ", " <<
    mfSingularOrPlural (
      fVoiceActualFiguredBassElementsCounter, "figured bass", "figured bass elements") <<
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
    endl;

  const int fieldWidth = 41;

#ifdef TRACING_IS_ENABLED
  // print the voice measures flat list
  unsigned int voiceMeasuresFlatListSize =
    fVoiceMeasuresFlatList.size ();

  os <<
    setw (fieldWidth) <<
    "voiceMeasuresFlatList";
  if (voiceMeasuresFlatListSize) {
    os <<
      " : " <<  voiceMeasuresFlatListSize << " elements";
  }
  else {
    os <<
      " : " << "empty";
  }
  os << endl;

  if (voiceMeasuresFlatListSize) {
    ++gIndenter;

    list<S_msrMeasure>::const_iterator
      iBegin = fVoiceMeasuresFlatList.begin (),
      iEnd   = fVoiceMeasuresFlatList.end (),
      i      = iBegin;

    for ( ; ; ) {
      // print the measure
      if (gGlobalTracingOahGroup->getTraceMeasures ()) {
        os << (*i)->asShortString ();
      }
      else {
        os << (*i)->getMeasureElementMeasureNumber ();
      }
      if (++i == iEnd) break;
      os << ' ';
    } // for
    os << endl;

    --gIndenter;
  }
#endif
*/

  --gIndenter;
}

ostream& operator<< (ostream& os, const S_msdrMeasure& elt)
{
  elt->print (os);
  return os;
}


}
