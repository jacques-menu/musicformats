/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <iomanip>      // std::setw, std::setprecision, ...

#include "visitor.h"

#include "mfStaticSettings.h"

#include "lpsrHeaders.h"

#include "oahOah.h"

#include "lpsrOah.h"
#include "lpsr2lilypondOah.h"


namespace MusicFormats
{

//______________________________________________________________________________
S_lpsrHeader lpsrHeader::create (
  int inputLineNumber)
{
  lpsrHeader* o =
    new lpsrHeader (
      inputLineNumber);
  assert (o != nullptr);
  return o;
}

lpsrHeader::lpsrHeader (
  int inputLineNumber)
    : lpsrElement (inputLineNumber)
{}

lpsrHeader::~lpsrHeader ()
{}

size_t lpsrHeader::maxLilypondVariablesNamesLength ()
{
  size_t result = 0;

  // LilyPond informations

  if (fLilypondDedication.size () > result) {
      result = fLilypondDedication.size ();
  }

  if (fLilypondPiece.size () > result) {
      result = fLilypondPiece.size ();
  }

  if (fLilypondOpus.size () > result) {
      result = fLilypondOpus.size ();
  }

  if (fLilypondTitle.size () > result) {
      result = fLilypondTitle.size ();
  }

  if (fLilypondSubTitle.size () > result) {
      result = fLilypondSubTitle.size ();
  }

  if (fLilypondSubSubTitle.size () > result) {
      result = fLilypondSubSubTitle.size ();
  }

  if (fLilypondInstrument.size () > result) {
      result = fLilypondInstrument.size ();
  }

  if (fLilypondMeter.size () > result) {
      result = fLilypondMeter.size ();
  }

  if (fLilypondCopyright.size () > result) {
      result = fLilypondCopyright.size ();
  }

  if (fLilypondTagline.size () > result) {
      result = fLilypondTagline.size ();
  }

  return result;
}

void lpsrHeader::acceptIn (basevisitor* v)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalLpsrOahGroup->getTraceLpsrVisitors ()) {
    gLogStream <<
      "% ==> lpsrHeader::acceptIn ()" <<
      std::endl;
  }
#endif

  if (visitor<S_lpsrHeader>*
    p =
      dynamic_cast<visitor<S_lpsrHeader>*> (v)) {
        S_lpsrHeader elem = this;

#ifdef MF_TRACING_IS_ENABLED
        if (gGlobalLpsrOahGroup->getTraceLpsrVisitors ()) {
          gLogStream <<
            "% ==> Launching lpsrHeader::visitStart ()" <<
            std::endl;
        }
#endif
        p->visitStart (elem);
  }
}

void lpsrHeader::acceptOut (basevisitor* v)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalLpsrOahGroup->getTraceLpsrVisitors ()) {
    gLogStream <<
      "% ==> lpsrHeader::acceptOut ()" <<
      std::endl;
  }
#endif

  if (visitor<S_lpsrHeader>*
    p =
      dynamic_cast<visitor<S_lpsrHeader>*> (v)) {
        S_lpsrHeader elem = this;

#ifdef MF_TRACING_IS_ENABLED
        if (gGlobalLpsrOahGroup->getTraceLpsrVisitors ()) {
          gLogStream <<
            "% ==> Launching lpsrHeader::visitEnd ()" <<
            std::endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void lpsrHeader::browseData (basevisitor* v)
{}

std::string lpsrHeader::asString () const
{
  std::stringstream s;

  s <<
    "[LpsrHeader" <<
    ", lilypondTitle: \" " <<  fLilypondTitle << "\"" <<
//    ", workNumber: \" " <<  fWorkNumber << "\"" <<
//    ", workCreditTypeTitle: \" " <<  fWorkCreditTypeTitle << "\"" <<
    ", lilypondOpus: \" " <<  fLilypondOpus << "\"" <<
//    ", movementNumber: \" " <<  fMovementNumber << "\"" <<
//    ", movementTitle: \" " <<  fMovementTitle << "\"" <<
    ", ..." <<
    ']';

  return s.str ();
}

void lpsrHeader::print (std::ostream& os) const
{
  os <<
    "Header" <<
    std::endl;

  Bool emptyHeader (true);

  ++gIndenter;

  const int fieldWidth = 16;

  // identification

  os <<
    "HeaderIdentification:";

  if (fHeaderIdentification) {
    os << std::endl;
    ++gIndenter;
    os <<
      fHeaderIdentification;
    --gIndenter;
  }
  else {
    os << " [NONE]" << std::endl;
  }

  // LilyPond informations

  if (fLilypondDedication.size ()) {
    os << std::left <<
      std::setw (fieldWidth) <<
      "fLilypondDedication" << ": " <<
      fLilypondDedication <<
      std::endl;

    emptyHeader = false;
  }

  if (fLilypondPiece.size ()) {
    os << std::left <<
      std::setw (fieldWidth) <<
      "fLilypondPiece" << ": " <<
      fLilypondPiece <<
      std::endl;

    emptyHeader = false;
  }

  if (fLilypondOpus.size ()) {
    os << std::left <<
      std::setw (fieldWidth) <<
      "fLilypondOpus" << ": " <<
      fLilypondOpus <<
      std::endl;

    emptyHeader = false;
  }

  if (fLilypondTitle.size ()) {
    os << std::left <<
      std::setw (fieldWidth) <<
      "fLilypondTitle" << ": " <<
      fLilypondTitle <<
      std::endl;

    emptyHeader = false;
  }

  if (fLilypondSubTitle.size ()) {
    os << std::left <<
      std::setw (fieldWidth) <<
      "fLilypondSubTitle" << ": " <<
      fLilypondSubTitle <<
      std::endl;

    emptyHeader = false;
  }

  if (fLilypondSubSubTitle.size ()) {
    os << std::left <<
      std::setw (fieldWidth) <<
      "fLilypondSubSubTitle" << ": " <<
      fLilypondSubSubTitle <<
      std::endl;

    emptyHeader = false;
  }

  if (fLilypondInstrument.size ()) {
    os << std::left <<
      std::setw (fieldWidth) <<
      "fLilypondInstrument" << ": " <<
      fLilypondInstrument <<
      std::endl;

    emptyHeader = false;
  }

  if (fLilypondMeter.size ()) {
    os << std::left <<
      std::setw (fieldWidth) <<
      "fLilypondMeter" << ": " <<
      fLilypondMeter <<
      std::endl;

    emptyHeader = false;
  }

  if (fLilypondCopyright.size ()) {
    os << std::left <<
      std::setw (fieldWidth) <<
      "fLilypondCopyright" << ": " <<
      fLilypondCopyright <<
      std::endl;

    emptyHeader = false;
  }

  if (fLilypondTagline.size ()) {
    os << std::left <<
      std::setw (fieldWidth) <<
      "fLilypondTagline" << ": " <<
      fLilypondTagline <<
      std::endl;

    emptyHeader = false;
  }

  if (emptyHeader) {
    os <<
      ' ' << "nothing specified" <<
      std::endl << std::endl; // JMI
  }

  --gIndenter;
}

std::ostream& operator << (std::ostream& os, const S_lpsrHeader& elt)
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

/*
void lpsrHeader::appendRight (
  int    inputLineNumber,
  std::string value)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceIdentification ()) {
    gLogStream <<
      "Appending right \"" <<
      value <<
      "\" to lpsrHeader" <<
      ", line " << fInputLineNumber <<
      std::endl;
  }
#endif

  fRightsList.push_back (value);
}

void lpsrHeader::appendComposer (
  int    inputLineNumber,
  std::string value)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceIdentification ()) {
    gLogStream <<
      "Appending composer \"" <<
      value <<
      "\" to lpsrHeader" <<
      ", line " << fInputLineNumber <<
      std::endl;
  }
#endif

  fComposersList.push_back (value);
}

void lpsrHeader::appendArranger (
  int    inputLineNumber,
  std::string value)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceIdentification ()) {
    gLogStream <<
      "Appending arranger \"" <<
      value <<
      "\" to lpsrHeader" <<
      ", line " << fInputLineNumber <<
      std::endl;
  }
#endif

  fArrangersList.push_back (value);
}

void lpsrHeader::appendLyricist (
  int    inputLineNumber,
  std::string value)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceIdentification ()) {
    gLogStream <<
      "Appending lyricist \"" <<
      value <<
      "\" to lpsrHeader" <<
      ", line " << fInputLineNumber <<
      std::endl;
  }
#endif

  fLyricistsList.push_back (value);
}

void lpsrHeader::appendPoet (
  int    inputLineNumber,
  std::string value)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceIdentification ()) {
    gLogStream <<
      "Appending poet \"" <<
      value <<
      "\" to lpsrHeader" <<
      ", line " << fInputLineNumber <<
      std::endl;
  }
#endif

  fPoetsList.push_back (value);
}

void lpsrHeader::removeAllPoets (
  int    inputLineNumber)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceIdentification ()) {
    gLogStream <<
      "Removing all poets from lpsrHeader" <<
      ", line " << fInputLineNumber <<
      std::endl;
  }
#endif

  fPoetsList.clear ();
}

void lpsrHeader::appendTranslator (
  int    inputLineNumber,
  std::string value)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceIdentification ()) {
    gLogStream <<
      "Appending translator \"" <<
      value <<
      "\" to lpsrHeader" <<
      ", line " << fInputLineNumber <<
      std::endl;
  }
#endif

  fTranslatorsList.push_back (value);
}

void lpsrHeader::appendArtist (
  int    inputLineNumber,
  std::string value)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceIdentification ()) {
    gLogStream <<
      "Appending artist \"" <<
      value <<
      "\" to lpsrHeader" <<
      ", line " << fInputLineNumber <<
      std::endl;
  }
#endif

  fArtistsList.push_back (value);
}

void lpsrHeader::appendSoftware (
  int    inputLineNumber,
  std::string value)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceIdentification ()) {
    gLogStream <<
      "Appending software \"" <<
      value <<
      "\" to lpsrHeader" <<
      ", line " << fInputLineNumber <<
      std::endl;
  }
#endif

  fSoftwaresList.push_back (value);
}

*/
