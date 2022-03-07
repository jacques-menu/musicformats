/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <iostream>
#include <sstream>
#include <iomanip>      // setw, setprecision, ...

#include "visitor.h"

#include "enableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "tracingOah.h"
#endif

#include "msrIdentification.h"

#include "oahOah.h"

#include "msrOah.h"


using namespace std;

namespace MusicFormats
{

//______________________________________________________________________________
S_msrIdentification msrIdentification::create (
  int inputLineNumber)
{
  msrIdentification* o =
    new msrIdentification (
      inputLineNumber);
  assert (o != nullptr);
  return o;
}

msrIdentification::msrIdentification (
  int inputLineNumber)
    : msrElement (inputLineNumber)
{}

msrIdentification::~msrIdentification ()
{}

void msrIdentification::setIdentificationWorkNumber (
  int           inputLineNumber,
  const string& val)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceIdentification ()) {
    gLogStream <<
      "Setting workNumber to \"" << val << "\"" <<
      endl;
  }
#endif

  fIdentificationWorkNumber = val;
}

void msrIdentification::setIdentificationWorkTitle (
  int           inputLineNumber,
  const string& val)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceIdentification ()) {
    gLogStream <<
      "Setting workTitle to \"" << val << "\"" <<
      endl;
  }
#endif

  fIdentificationWorkTitle = val;
}

void msrIdentification::setIdentificationOpus (
  int           inputLineNumber,
  const string& val)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceIdentification ()) {
    gLogStream <<
      "Setting opus to \"" << val << "\"" <<
      endl;
  }
#endif

  fIdentificationOpus = val;
}

void msrIdentification::setIdentificationMovementNumber (
  int           inputLineNumber,
  const string& val)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceIdentification ()) {
    gLogStream <<
      "Setting movementNumber to \"" << val << "\"" <<
      endl;
  }
#endif

  fIdentificationMovementNumber = val;
}

void msrIdentification::setIdentificationMovementTitle (
  int           inputLineNumber,
  const string& val)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceIdentification ()) {
    gLogStream <<
      "Setting movementTitle to \"" << val << "\"" <<
      endl;
  }
#endif

  fIdentificationMovementTitle = val;
}

void msrIdentification::setIdentificationEncodingDate (
  int           inputLineNumber,
  const string& val)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceIdentification ()) {
    gLogStream <<
      "Setting encodingDate to \"" << val << "\"" <<
      endl;
  }
#endif

  fIdentificationEncodingDate = val;
}

void msrIdentification::setIdentificationMiscellaneousField (
  int           inputLineNumber,
  const string& val)
{
 #ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceIdentification ()) {
    gLogStream <<
      "Setting miscellaneousField to \"" << val << "\"" <<
      endl;
  }
#endif

  fIdentificationMiscellaneousField = val;
}

void msrIdentification::setIdentificationScoreInstrument (
  int           inputLineNumber,
  const string& val)
{
 #ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceIdentification ()) {
    gLogStream <<
      "Setting scoreInstrument to \"" << val << "\"" <<
      endl;
  }
#endif

  fIdentificationScoreInstrument = val;
}

void msrIdentification::appendComposer (
  int           inputLineNumber,
  const string& value)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceIdentification ()) {
    gLogStream <<
      "Appending composer \"" << value << "\"" <<
      endl;
  }
#endif

  fIdentificationComposersList.push_back (value);
}

void msrIdentification::appendLyricist (
  int           inputLineNumber,
  const string& value)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceIdentification ()) {
    gLogStream <<
      "Appending lyricist \"" << value << "\"" <<
      endl;
  }
#endif

  fIdentificationLyricistsList.push_back (value);
}

void msrIdentification::appendArranger (
  int           inputLineNumber,
  const string& value)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceIdentification ()) {
    gLogStream <<
      "Appending arranger \"" << value << "\"" <<
      endl;
  }
#endif

  fIdentificationArrangersList.push_back (value);
}

void msrIdentification::appendPoet (
  int           inputLineNumber,
  const string& value)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceIdentification ()) {
    gLogStream <<
      "Appending poet \"" << value << "\"" <<
      endl;
  }
#endif

  fIdentificationPoetsList.push_back (value);
}

void msrIdentification::appendTranslator (
  int           inputLineNumber,
  const string& value)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceIdentification ()) {
    gLogStream <<
      "Appending translator \"" << value << "\"" <<
      endl;
  }
#endif

  fIdentificationTranslatorsList.push_back (value);
}

void msrIdentification::appendArtist (
  int           inputLineNumber,
  const string& value)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceIdentification ()) {
    gLogStream <<
      "Appending artist \"" << value << "\"" <<
      endl;
  }
#endif

  fIdentificationArtistsList.push_back (value);
}

void msrIdentification::appendSoftware (
  int           inputLineNumber,
  const string& value)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceIdentification ()) {
    gLogStream <<
      "Appending software \"" << value << "\"" <<
      endl;
  }
#endif

  fIdentificationSoftwaresList.push_back (value);
}

void msrIdentification::appendRight (
  int           inputLineNumber,
  const string& value)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceIdentification ()) {
    gLogStream <<
      "Appending right \"" << value << "\"" <<
      endl;
  }
#endif

  fIdentificationRightsList.push_back (value);
}

void msrIdentification::appendSource (
  int           inputLineNumber,
  const string& value)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceIdentification ()) {
    gLogStream <<
      "Appendings source \"" << value << "\"" <<
      endl;
  }
#endif

  fIdentificationSourcesList.push_back (value);
}

void msrIdentification::appendRelation (
  int           inputLineNumber,
  const string& value)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceIdentification ()) {
    gLogStream <<
      "Appending relation \"" << value << "\"" <<
      endl;
  }
#endif

  fIdentificationRelationsList.push_back (value);
}

unsigned int msrIdentification::maxIdentificationNamesLength ()
{
  unsigned int result = 0;

  if (fIdentificationWorkNumber.size () > result) {
    result = fIdentificationWorkNumber.size ();
  }

  if (fIdentificationWorkTitle.size () > result) {
    result = fIdentificationWorkTitle.size ();
  }

  if (fIdentificationOpus.size () > result) {
    result = fIdentificationOpus.size ();
  }

  if (fIdentificationMovementNumber.size () > result) {
    result = fIdentificationMovementNumber.size ();
  }

  if (fIdentificationMovementTitle.size () > result) {
    result = fIdentificationMovementTitle.size ();
  }

  if (fIdentificationEncodingDate.size () > result) {
    result = fIdentificationEncodingDate.size ();
  }

  if (fIdentificationMiscellaneousField.size () > result) {
    result = fIdentificationMiscellaneousField.size ();
  }

  if (fIdentificationScoreInstrument.size () > result) {
    result = fIdentificationScoreInstrument.size ();
  }

  if (fIdentificationComposersList.size ()) {
    list<string>::const_iterator i;

    for (i=fIdentificationComposersList.begin (); i!=fIdentificationComposersList.end (); ++i) {
      unsigned int length = (*i).size ();
      if (length > result){
        result = length;
      }
    } // for
  }

  if (fIdentificationLyricistsList.size ()) {
    list<string>::const_iterator i;

    for (i=fIdentificationLyricistsList.begin (); i!=fIdentificationLyricistsList.end (); ++i) {
      unsigned int length = (*i).size ();
      if (length > result) {
        result = length;
      }
    } // for
  }

  if (fIdentificationArrangersList.size ()) {
    list<string>::const_iterator i;

    for (i=fIdentificationArrangersList.begin (); i!=fIdentificationArrangersList.end (); ++i) {
      unsigned int length = (*i).size ();
      if (length > result) {
        result = length;
      }
    } // for
  }

  if (fIdentificationPoetsList.size ()) {
    list<string>::const_iterator i;

    for (i=fIdentificationPoetsList.begin (); i!=fIdentificationPoetsList.end (); ++i) {
      unsigned int length = (*i).size ();
      if (length > result) {
        result = length;
      }
    } // for
  }

  if (fIdentificationTranslatorsList.size ()) {
    list<string>::const_iterator i;

    for (i=fIdentificationTranslatorsList.begin (); i!=fIdentificationTranslatorsList.end (); ++i) {
      unsigned int length = (*i).size ();
      if (length > result) {
        result = length;
      }
    } // for
  }

  if (fIdentificationArtistsList.size ()) {
    list<string>::const_iterator i;

    for (i=fIdentificationArtistsList.begin (); i!=fIdentificationArtistsList.end (); ++i) {
      unsigned int length = (*i).size ();
      if (length > result) {
        result = length;
      }
    } // for
  }

  if (fIdentificationSoftwaresList.size ()) {
    list<string>::const_iterator i;

    for (i=fIdentificationSoftwaresList.begin (); i!=fIdentificationSoftwaresList.end (); ++i) {
      unsigned int length = (*i).size ();
      if (length > result) {
        result = length;
      }
    } // for
  }

  if (fIdentificationRightsList.size ()) {
    list<string>::const_iterator i;

    for (i=fIdentificationRightsList.begin (); i!=fIdentificationRightsList.end (); ++i) {
      unsigned int length = (*i).size ();
      if (length > result) {
        result = length;
      }
    } // for
  }

  if (fIdentificationSourcesList.size ()) {
    list<string>::const_iterator i;

    for (i=fIdentificationSourcesList.begin (); i!=fIdentificationSourcesList.end (); ++i) {
      unsigned int length = (*i).size ();
      if (length > result) {
        result = length;
      }
    } // for
  }

  if (fIdentificationRelationsList.size ()) {
    list<string>::const_iterator i;

    for (i=fIdentificationRelationsList.begin (); i!=fIdentificationRelationsList.end (); ++i) {
      unsigned int length = (*i).size ();
      if (length > result) {
        result = length;
      }
    } // for
  }

  return result;
}

void msrIdentification::acceptIn (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrIdentification::acceptIn ()" <<
      endl;
  }

  if (visitor<S_msrIdentification>*
    p =
      dynamic_cast<visitor<S_msrIdentification>*> (v)) {
        S_msrIdentification elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrIdentification::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrIdentification::acceptOut (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrIdentification::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrIdentification>*
    p =
      dynamic_cast<visitor<S_msrIdentification>*> (v)) {
        S_msrIdentification elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrIdentification::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrIdentification::browseData (basevisitor* v)
{}

string msrIdentification::asString () const
{
  stringstream s;

  s <<
    "[ Identification" <<
    ", workNumber: \" " <<  fIdentificationWorkNumber << "\"" <<
    ", workTitle: \" " <<  fIdentificationWorkTitle << "\"" <<
    ", opus: \" " <<  fIdentificationOpus << "\"" <<
    ", movementNumber: \" " <<  fIdentificationMovementNumber << "\"" <<
    ", movementTitle: \" " <<  fIdentificationMovementTitle << "\"" <<
    ", ..." <<
    "]";

  return s.str ();
}

void msrIdentification::print (ostream& os) const
{
  os <<
    "[Identification" <<
    endl;

  const int fieldWidth = 20;

  Bool emptyIdentification (true);

  ++gIndenter;

  if (fIdentificationWorkNumber.size ()) {
    os << left <<
      setw (fieldWidth) <<
      "fIdentificationWorkNumber" << " : \"" <<
      fIdentificationWorkNumber <<
      "\"" <<
      endl;

    emptyIdentification = false;
  }

  if (fIdentificationWorkTitle.size ()) {
    os << left <<
      setw (fieldWidth) <<
      "fIdentificationWorkTitle" << " : \"" <<
      fIdentificationWorkTitle <<
      "\"" <<
      endl;

    emptyIdentification = false;
  }

  if (fIdentificationOpus.size ()) {
    os << left <<
      setw (fieldWidth) <<
      "fIdentificationOpus" << " : \"" <<
      fIdentificationOpus <<
      "\"" <<
      endl;

    emptyIdentification = false;
  }

  if (fIdentificationMovementNumber.size ()) {
    os << left <<
      setw (fieldWidth) <<
      "fIdentificationMovementNumber" << " : \"" <<
      fIdentificationMovementNumber <<
      "\"" <<
      endl;

    emptyIdentification = false;
  }

  if (fIdentificationMovementTitle.size ()) {
    os << left <<
      setw (fieldWidth) <<
      "fIdentificationMovementTitle" << " : \"" <<
      fIdentificationMovementTitle <<
      "\"" <<
      endl;

    emptyIdentification = false;
  }

  if (fIdentificationEncodingDate.size ()) {
    os << left <<
      setw (fieldWidth) <<
      "fIdentificationEncodingDate" << " : \"" <<
      fIdentificationEncodingDate <<
      "\"" <<
      endl;

    emptyIdentification = false;
  }

  if (fIdentificationMiscellaneousField.size ()) {
    os << left <<
      setw (fieldWidth) <<
      "fIdentificationMiscellaneousField" << " : \"" <<
      fIdentificationMiscellaneousField <<
      "\"" <<
      endl;

    emptyIdentification = false;
  }

  if (fIdentificationComposersList.size ()) {
    os << left <<
      setw (fieldWidth) <<
      "fIdentificationComposersList" << " : " <<
      endl;

    for (
      list<string>::const_iterator i = fIdentificationComposersList.begin ();
      i != fIdentificationComposersList.end ();
      ++i
    ) {
      os <<
        gTab << "\"" << (*i) << "\"" <<
        endl;
    } // for

    emptyIdentification = false;
  }

  if (fIdentificationLyricistsList.size ()) {
    os << left <<
      setw (fieldWidth) <<
      "fIdentificationLyricistsList" << " : " <<
      endl;

    for (
      list<string>::const_iterator i = fIdentificationLyricistsList.begin ();
      i != fIdentificationLyricistsList.end ();
      ++i
    ) {
      os <<
        gTab << "\"" << (*i) << "\"" <<
        endl;
    } // for

    emptyIdentification = false;
  }

  if (fIdentificationArrangersList.size ()) {
    os << left <<
      setw (fieldWidth) <<
      "fIdentificationArrangersList" << " : " <<
      endl;

    for (
      list<string>::const_iterator i = fIdentificationArrangersList.begin ();
      i != fIdentificationArrangersList.end ();
      ++i
    ) {
      os <<
        gTab << "\"" << (*i) << "\"" <<
        endl;
    } // for

    emptyIdentification = false;
  }

  if (fIdentificationPoetsList.size ()) {
    os << left <<
      setw (fieldWidth) <<
      "fIdentificationPoetsList" << " : " <<
      endl;

    for (
      list<string>::const_iterator i = fIdentificationPoetsList.begin ();
      i != fIdentificationPoetsList.end ();
      ++i
    ) {
      os <<
        gTab << "\"" << (*i) << "\"" <<
        endl;
    } // for

    emptyIdentification = false;
  }

  if (fIdentificationTranslatorsList.size ()) {
    os << left <<
      setw (fieldWidth) <<
      "fIdentificationTranslatorsList" << " : " <<
      endl;

    for (
      list<string>::const_iterator i = fIdentificationTranslatorsList.begin ();
      i != fIdentificationTranslatorsList.end ();
      ++i
    ) {
      os <<
        gTab << "\"" << (*i) << "\"" <<
        endl;
    } // for

    emptyIdentification = false;
  }

  if (fIdentificationSoftwaresList.size ()) {
    os << left <<
      setw (fieldWidth) <<
      "fIdentificationSoftwaresList" << " : " <<
      endl;

    for (
      list<string>::const_iterator i = fIdentificationSoftwaresList.begin ();
      i != fIdentificationSoftwaresList.end ();
      ++i
    ) {
      os <<
        gTab << "\"" << (*i) << "\"" <<
        endl;
    } // for

    emptyIdentification = false;
  }

  if (fIdentificationRightsList.size ()) {
    os << left <<
      setw (fieldWidth) <<
      "fIdentificationRightsList" << " : " <<
      endl;

    for (
      list<string>::const_iterator i = fIdentificationRightsList.begin ();
      i != fIdentificationRightsList.end ();
      ++i
    ) {
      os <<
        gTab << "\"" << (*i) << "\"" <<
        endl;
    } // for

    emptyIdentification = false;
  }

  if (fIdentificationSourcesList.size ()) {
    os << left <<
      setw (fieldWidth) <<
      "fIdentificationSourcesList" << " : " <<
      endl;

    for (
      list<string>::const_iterator i = fIdentificationSourcesList.begin ();
      i != fIdentificationSourcesList.end ();
      ++i
    ) {
      os <<
        gTab << "\"" << (*i) << "\"" <<
        endl;
    } // for

    emptyIdentification = false;
  }

  if (fIdentificationRelationsList.size ()) {
    os << left <<
      setw (fieldWidth) <<
      "fIdentificationRelationsList" << " : " <<
      endl;

    for (
      list<string>::const_iterator i = fIdentificationRelationsList.begin ();
      i != fIdentificationRelationsList.end ();
      ++i
    ) {
      os <<
        gTab << "\"" << (*i) << "\"" <<
        endl;
    } // for

    emptyIdentification = false;
  }

  if (emptyIdentification) {
    os <<
      ' ' << "nothing specified" <<
      endl;
  }

  --gIndenter;

  os << ']' << endl;
}

ostream& operator<< (ostream& os, const S_msrIdentification& elt)
{
  elt->print (os);
  return os;
}


}
