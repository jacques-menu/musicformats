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
#include <iomanip>      // std::setw, std::setprecision, ...

#include "visitor.h"

#include "oahEnableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "tracingOah.h"
#endif

#include "msrIdentification.h"

#include "oahOah.h"

#include "msrOah.h"


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
  const std::string& val)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceIdentification ()) {
    gLogStream <<
      "Setting workNumber to \"" << val << "\"" <<
      std::endl;
  }
#endif

  fIdentificationWorkNumber = val;
}

void msrIdentification::setIdentificationWorkCreditTypeTitle (
  int           inputLineNumber,
  const std::string& val)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceIdentification ()) {
    gLogStream <<
      "Setting workCreditTypeTitle to \"" << val << "\"" <<
      std::endl;
  }
#endif

  fIdentificationWorkCreditTypeTitle = val;
}

void msrIdentification::setIdentificationOpus (
  int           inputLineNumber,
  const std::string& val)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceIdentification ()) {
    gLogStream <<
      "Setting opus to \"" << val << "\"" <<
      std::endl;
  }
#endif

  fIdentificationOpus = val;
}

void msrIdentification::setIdentificationMovementNumber (
  int           inputLineNumber,
  const std::string& val)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceIdentification ()) {
    gLogStream <<
      "Setting movementNumber to \"" << val << "\"" <<
      std::endl;
  }
#endif

  fIdentificationMovementNumber = val;
}

void msrIdentification::setIdentificationMovementTitle (
  int           inputLineNumber,
  const std::string& val)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceIdentification ()) {
    gLogStream <<
      "Setting movementTitle to \"" << val << "\"" <<
      std::endl;
  }
#endif

  fIdentificationMovementTitle = val;
}

void msrIdentification::setIdentificationEncodingDate (
  int           inputLineNumber,
  const std::string& val)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceIdentification ()) {
    gLogStream <<
      "Setting encodingDate to \"" << val << "\"" <<
      std::endl;
  }
#endif

  fIdentificationEncodingDate = val;
}

void msrIdentification::setIdentificationMiscellaneousField (
  int           inputLineNumber,
  const std::string& val)
{
 #ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceIdentification ()) {
    gLogStream <<
      "Setting miscellaneousField to \"" << val << "\"" <<
      std::endl;
  }
#endif

  fIdentificationMiscellaneousField = val;
}

void msrIdentification::setIdentificationScoreInstrument (
  int           inputLineNumber,
  const std::string& val)
{
 #ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceIdentification ()) {
    gLogStream <<
      "Setting scoreInstrument to \"" << val << "\"" <<
      std::endl;
  }
#endif

  fIdentificationScoreInstrument = val;
}

void msrIdentification::appendComposer (
  int           inputLineNumber,
  const std::string& value)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceIdentification ()) {
    gLogStream <<
      "Appending composer \"" << value << "\"" <<
      std::endl;
  }
#endif

  fIdentificationComposersList.push_back (value);
}

void msrIdentification::appendLyricist (
  int           inputLineNumber,
  const std::string& value)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceIdentification ()) {
    gLogStream <<
      "Appending lyricist \"" << value << "\"" <<
      std::endl;
  }
#endif

  fIdentificationLyricistsList.push_back (value);
}

void msrIdentification::appendArranger (
  int           inputLineNumber,
  const std::string& value)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceIdentification ()) {
    gLogStream <<
      "Appending arranger \"" << value << "\"" <<
      std::endl;
  }
#endif

  fIdentificationArrangersList.push_back (value);
}

void msrIdentification::appendPoet (
  int           inputLineNumber,
  const std::string& value)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceIdentification ()) {
    gLogStream <<
      "Appending poet \"" << value << "\"" <<
      std::endl;
  }
#endif

  fIdentificationPoetsList.push_back (value);
}

void msrIdentification::appendTranslator (
  int           inputLineNumber,
  const std::string& value)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceIdentification ()) {
    gLogStream <<
      "Appending translator \"" << value << "\"" <<
      std::endl;
  }
#endif

  fIdentificationTranslatorsList.push_back (value);
}

void msrIdentification::appendArtist (
  int           inputLineNumber,
  const std::string& value)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceIdentification ()) {
    gLogStream <<
      "Appending artist \"" << value << "\"" <<
      std::endl;
  }
#endif

  fIdentificationArtistsList.push_back (value);
}

void msrIdentification::appendSoftware (
  int           inputLineNumber,
  const std::string& value)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceIdentification ()) {
    gLogStream <<
      "Appending software \"" << value << "\"" <<
      std::endl;
  }
#endif

  fIdentificationSoftwaresList.push_back (value);
}

void msrIdentification::appendRight (
  int           inputLineNumber,
  const std::string& value)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceIdentification ()) {
    gLogStream <<
      "Appending right \"" << value << "\"" <<
      std::endl;
  }
#endif

  fIdentificationRightsList.push_back (value);
}

void msrIdentification::appendSource (
  int           inputLineNumber,
  const std::string& value)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceIdentification ()) {
    gLogStream <<
      "Appendings source \"" << value << "\"" <<
      std::endl;
  }
#endif

  fIdentificationSourcesList.push_back (value);
}

void msrIdentification::appendRelation (
  int           inputLineNumber,
  const std::string& value)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceIdentification ()) {
    gLogStream <<
      "Appending relation \"" << value << "\"" <<
      std::endl;
  }
#endif

  fIdentificationRelationsList.push_back (value);
}

size_t msrIdentification::maxIdentificationNamesLength ()
{
  size_t result = 0;

  if (fIdentificationWorkNumber.size () > result) {
    result = fIdentificationWorkNumber.size ();
  }

  if (fIdentificationWorkCreditTypeTitle.size () > result) {
    result = fIdentificationWorkCreditTypeTitle.size ();
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
    std::list<std::string>::const_iterator i;

    for (i=fIdentificationComposersList.begin (); i!=fIdentificationComposersList.end (); ++i) {
      size_t length = (*i).size ();
      if (length > result){
        result = length;
      }
    } // for
  }

  if (fIdentificationLyricistsList.size ()) {
    std::list<std::string>::const_iterator i;

    for (i=fIdentificationLyricistsList.begin (); i!=fIdentificationLyricistsList.end (); ++i) {
      size_t length = (*i).size ();
      if (length > result) {
        result = length;
      }
    } // for
  }

  if (fIdentificationArrangersList.size ()) {
    std::list<std::string>::const_iterator i;

    for (i=fIdentificationArrangersList.begin (); i!=fIdentificationArrangersList.end (); ++i) {
      size_t length = (*i).size ();
      if (length > result) {
        result = length;
      }
    } // for
  }

  if (fIdentificationPoetsList.size ()) {
    std::list<std::string>::const_iterator i;

    for (i=fIdentificationPoetsList.begin (); i!=fIdentificationPoetsList.end (); ++i) {
      size_t length = (*i).size ();
      if (length > result) {
        result = length;
      }
    } // for
  }

  if (fIdentificationTranslatorsList.size ()) {
    std::list<std::string>::const_iterator i;

    for (i=fIdentificationTranslatorsList.begin (); i!=fIdentificationTranslatorsList.end (); ++i) {
      size_t length = (*i).size ();
      if (length > result) {
        result = length;
      }
    } // for
  }

  if (fIdentificationArtistsList.size ()) {
    std::list<std::string>::const_iterator i;

    for (i=fIdentificationArtistsList.begin (); i!=fIdentificationArtistsList.end (); ++i) {
      size_t length = (*i).size ();
      if (length > result) {
        result = length;
      }
    } // for
  }

  if (fIdentificationSoftwaresList.size ()) {
    std::list<std::string>::const_iterator i;

    for (i=fIdentificationSoftwaresList.begin (); i!=fIdentificationSoftwaresList.end (); ++i) {
      size_t length = (*i).size ();
      if (length > result) {
        result = length;
      }
    } // for
  }

  if (fIdentificationRightsList.size ()) {
    std::list<std::string>::const_iterator i;

    for (i=fIdentificationRightsList.begin (); i!=fIdentificationRightsList.end (); ++i) {
      size_t length = (*i).size ();
      if (length > result) {
        result = length;
      }
    } // for
  }

  if (fIdentificationSourcesList.size ()) {
    std::list<std::string>::const_iterator i;

    for (i=fIdentificationSourcesList.begin (); i!=fIdentificationSourcesList.end (); ++i) {
      size_t length = (*i).size ();
      if (length > result) {
        result = length;
      }
    } // for
  }

  if (fIdentificationRelationsList.size ()) {
    std::list<std::string>::const_iterator i;

    for (i=fIdentificationRelationsList.begin (); i!=fIdentificationRelationsList.end (); ++i) {
      size_t length = (*i).size ();
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
      std::endl;
  }

  if (visitor<S_msrIdentification>*
    p =
      dynamic_cast<visitor<S_msrIdentification>*> (v)) {
        S_msrIdentification elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrIdentification::visitStart ()" <<
            std::endl;
        }
        p->visitStart (elem);
  }
}

void msrIdentification::acceptOut (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrIdentification::acceptOut ()" <<
      std::endl;
  }

  if (visitor<S_msrIdentification>*
    p =
      dynamic_cast<visitor<S_msrIdentification>*> (v)) {
        S_msrIdentification elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrIdentification::visitEnd ()" <<
            std::endl;
        }
        p->visitEnd (elem);
  }
}

void msrIdentification::browseData (basevisitor* v)
{}

std::string msrIdentification::asString () const
{
  std::stringstream s;

  s <<
    "[ Identification" <<
    ", workNumber: \" " <<  fIdentificationWorkNumber << "\"" <<
    ", workCreditTypeTitle: \" " <<  fIdentificationWorkCreditTypeTitle << "\"" <<
    ", opus: \" " <<  fIdentificationOpus << "\"" <<
    ", movementNumber: \" " <<  fIdentificationMovementNumber << "\"" <<
    ", movementTitle: \" " <<  fIdentificationMovementTitle << "\"" <<
    ", ..." <<
    ']';

  return s.str ();
}

void msrIdentification::print (std::ostream& os) const
{
  os <<
    "[Identification" <<
    std::endl;

  const int fieldWidth = 20;

  Bool emptyIdentification (true);

  ++gIndenter;

  if (fIdentificationWorkNumber.size ()) {
    os << std::left <<
      std::setw (fieldWidth) <<
      "fIdentificationWorkNumber" << " : \"" <<
      fIdentificationWorkNumber <<
      "\"" <<
      std::endl;

    emptyIdentification = false;
  }

  if (fIdentificationWorkCreditTypeTitle.size ()) {
    os << std::left <<
      std::setw (fieldWidth) <<
      "fIdentificationWorkCreditTypeTitle" << " : \"" <<
      fIdentificationWorkCreditTypeTitle <<
      "\"" <<
      std::endl;

    emptyIdentification = false;
  }

  if (fIdentificationOpus.size ()) {
    os << std::left <<
      std::setw (fieldWidth) <<
      "fIdentificationOpus" << " : \"" <<
      fIdentificationOpus <<
      "\"" <<
      std::endl;

    emptyIdentification = false;
  }

  if (fIdentificationMovementNumber.size ()) {
    os << std::left <<
      std::setw (fieldWidth) <<
      "fIdentificationMovementNumber" << " : \"" <<
      fIdentificationMovementNumber <<
      "\"" <<
      std::endl;

    emptyIdentification = false;
  }

  if (fIdentificationMovementTitle.size ()) {
    os << std::left <<
      std::setw (fieldWidth) <<
      "fIdentificationMovementTitle" << " : \"" <<
      fIdentificationMovementTitle <<
      "\"" <<
      std::endl;

    emptyIdentification = false;
  }

  if (fIdentificationEncodingDate.size ()) {
    os << std::left <<
      std::setw (fieldWidth) <<
      "fIdentificationEncodingDate" << " : \"" <<
      fIdentificationEncodingDate <<
      "\"" <<
      std::endl;

    emptyIdentification = false;
  }

  if (fIdentificationMiscellaneousField.size ()) {
    os << std::left <<
      std::setw (fieldWidth) <<
      "fIdentificationMiscellaneousField" << " : \"" <<
      fIdentificationMiscellaneousField <<
      "\"" <<
      std::endl;

    emptyIdentification = false;
  }

  if (fIdentificationComposersList.size ()) {
    os << std::left <<
      std::setw (fieldWidth) <<
      "fIdentificationComposersList" << " : " <<
      std::endl;

    for (
      std::list<std::string>::const_iterator i = fIdentificationComposersList.begin ();
      i != fIdentificationComposersList.end ();
      ++i
    ) {
      os <<
        gTab << "\"" << (*i) << "\"" <<
        std::endl;
    } // for

    emptyIdentification = false;
  }

  if (fIdentificationLyricistsList.size ()) {
    os << std::left <<
      std::setw (fieldWidth) <<
      "fIdentificationLyricistsList" << " : " <<
      std::endl;

    for (
      std::list<std::string>::const_iterator i = fIdentificationLyricistsList.begin ();
      i != fIdentificationLyricistsList.end ();
      ++i
    ) {
      os <<
        gTab << "\"" << (*i) << "\"" <<
        std::endl;
    } // for

    emptyIdentification = false;
  }

  if (fIdentificationArrangersList.size ()) {
    os << std::left <<
      std::setw (fieldWidth) <<
      "fIdentificationArrangersList" << " : " <<
      std::endl;

    for (
      std::list<std::string>::const_iterator i = fIdentificationArrangersList.begin ();
      i != fIdentificationArrangersList.end ();
      ++i
    ) {
      os <<
        gTab << "\"" << (*i) << "\"" <<
        std::endl;
    } // for

    emptyIdentification = false;
  }

  if (fIdentificationPoetsList.size ()) {
    os << std::left <<
      std::setw (fieldWidth) <<
      "fIdentificationPoetsList" << " : " <<
      std::endl;

    for (
      std::list<std::string>::const_iterator i = fIdentificationPoetsList.begin ();
      i != fIdentificationPoetsList.end ();
      ++i
    ) {
      os <<
        gTab << "\"" << (*i) << "\"" <<
        std::endl;
    } // for

    emptyIdentification = false;
  }

  if (fIdentificationTranslatorsList.size ()) {
    os << std::left <<
      std::setw (fieldWidth) <<
      "fIdentificationTranslatorsList" << " : " <<
      std::endl;

    for (
      std::list<std::string>::const_iterator i = fIdentificationTranslatorsList.begin ();
      i != fIdentificationTranslatorsList.end ();
      ++i
    ) {
      os <<
        gTab << "\"" << (*i) << "\"" <<
        std::endl;
    } // for

    emptyIdentification = false;
  }

  if (fIdentificationSoftwaresList.size ()) {
    os << std::left <<
      std::setw (fieldWidth) <<
      "fIdentificationSoftwaresList" << " : " <<
      std::endl;

    for (
      std::list<std::string>::const_iterator i = fIdentificationSoftwaresList.begin ();
      i != fIdentificationSoftwaresList.end ();
      ++i
    ) {
      os <<
        gTab << "\"" << (*i) << "\"" <<
        std::endl;
    } // for

    emptyIdentification = false;
  }

  if (fIdentificationRightsList.size ()) {
    os << std::left <<
      std::setw (fieldWidth) <<
      "fIdentificationRightsList" << " : " <<
      std::endl;

    for (
      std::list<std::string>::const_iterator i = fIdentificationRightsList.begin ();
      i != fIdentificationRightsList.end ();
      ++i
    ) {
      os <<
        gTab << "\"" << (*i) << "\"" <<
        std::endl;
    } // for

    emptyIdentification = false;
  }

  if (fIdentificationSourcesList.size ()) {
    os << std::left <<
      std::setw (fieldWidth) <<
      "fIdentificationSourcesList" << " : " <<
      std::endl;

    for (
      std::list<std::string>::const_iterator i = fIdentificationSourcesList.begin ();
      i != fIdentificationSourcesList.end ();
      ++i
    ) {
      os <<
        gTab << "\"" << (*i) << "\"" <<
        std::endl;
    } // for

    emptyIdentification = false;
  }

  if (fIdentificationRelationsList.size ()) {
    os << std::left <<
      std::setw (fieldWidth) <<
      "fIdentificationRelationsList" << " : " <<
      std::endl;

    for (
      std::list<std::string>::const_iterator i = fIdentificationRelationsList.begin ();
      i != fIdentificationRelationsList.end ();
      ++i
    ) {
      os <<
        gTab << "\"" << (*i) << "\"" <<
        std::endl;
    } // for

    emptyIdentification = false;
  }

  if (emptyIdentification) {
    os <<
      ' ' << "nothing specified" <<
      std::endl;
  }

  --gIndenter;

  os << ']' << std::endl;
}

std::ostream& operator << (std::ostream& os, const S_msrIdentification& elt)
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
