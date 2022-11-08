/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <iomanip>      // setw, setprecision, ...

#include "visitor.h"

#include "enableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "tracingOah.h"
#endif

#include "lpsrHeaders.h"

#include "oahOah.h"

#include "lpsrOah.h"
#include "lpsr2lilypondOah.h"


using namespace std;

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
#ifdef TRACING_IS_ENABLED
  if (gGlobalLpsrOahGroup->getTraceLpsrVisitors ()) {
    gLogStream <<
      "% ==> lpsrHeader::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_lpsrHeader>*
    p =
      dynamic_cast<visitor<S_lpsrHeader>*> (v)) {
        S_lpsrHeader elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalLpsrOahGroup->getTraceLpsrVisitors ()) {
          gLogStream <<
            "% ==> Launching lpsrHeader::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void lpsrHeader::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalLpsrOahGroup->getTraceLpsrVisitors ()) {
    gLogStream <<
      "% ==> lpsrHeader::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_lpsrHeader>*
    p =
      dynamic_cast<visitor<S_lpsrHeader>*> (v)) {
        S_lpsrHeader elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalLpsrOahGroup->getTraceLpsrVisitors ()) {
          gLogStream <<
            "% ==> Launching lpsrHeader::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void lpsrHeader::browseData (basevisitor* v)
{}

string lpsrHeader::asString () const
{
  stringstream s;

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

void lpsrHeader::print (ostream& os) const
{
  os <<
    "Header" <<
    endl;

  Bool emptyHeader (true);

  ++gIndenter;

  const int fieldWidth = 16;

  // identification

  os <<
    "HeaderIdentification:";

  if (fHeaderIdentification) {
    os << endl;
    ++gIndenter;
    os <<
      fHeaderIdentification;
    --gIndenter;
  }
  else {
    os << " [NONE]" << endl;
  }

  // LilyPond informations

  if (fLilypondDedication.size ()) {
    os << left <<
      setw (fieldWidth) <<
      "fLilypondDedication" << " : " <<
      fLilypondDedication <<
      endl;

    emptyHeader = false;
  }

  if (fLilypondPiece.size ()) {
    os << left <<
      setw (fieldWidth) <<
      "fLilypondPiece" << " : " <<
      fLilypondPiece <<
      endl;

    emptyHeader = false;
  }

  if (fLilypondOpus.size ()) {
    os << left <<
      setw (fieldWidth) <<
      "fLilypondOpus" << " : " <<
      fLilypondOpus <<
      endl;

    emptyHeader = false;
  }

  if (fLilypondTitle.size ()) {
    os << left <<
      setw (fieldWidth) <<
      "fLilypondTitle" << " : " <<
      fLilypondTitle <<
      endl;

    emptyHeader = false;
  }

  if (fLilypondSubTitle.size ()) {
    os << left <<
      setw (fieldWidth) <<
      "fLilypondSubTitle" << " : " <<
      fLilypondSubTitle <<
      endl;

    emptyHeader = false;
  }

  if (fLilypondSubSubTitle.size ()) {
    os << left <<
      setw (fieldWidth) <<
      "fLilypondSubSubTitle" << " : " <<
      fLilypondSubSubTitle <<
      endl;

    emptyHeader = false;
  }

  if (fLilypondInstrument.size ()) {
    os << left <<
      setw (fieldWidth) <<
      "fLilypondInstrument" << " : " <<
      fLilypondInstrument <<
      endl;

    emptyHeader = false;
  }

  if (fLilypondMeter.size ()) {
    os << left <<
      setw (fieldWidth) <<
      "fLilypondMeter" << " : " <<
      fLilypondMeter <<
      endl;

    emptyHeader = false;
  }

  if (fLilypondCopyright.size ()) {
    os << left <<
      setw (fieldWidth) <<
      "fLilypondCopyright" << " : " <<
      fLilypondCopyright <<
      endl;

    emptyHeader = false;
  }

  if (fLilypondTagline.size ()) {
    os << left <<
      setw (fieldWidth) <<
      "fLilypondTagline" << " : " <<
      fLilypondTagline <<
      endl;

    emptyHeader = false;
  }

  if (emptyHeader) {
    os <<
      ' ' << "nothing specified" <<
      endl << endl; // JMI
  }

  --gIndenter;
}

void lpsrHeader::printShort (ostream& os) const
{
  print (os); // JMI
}

ostream& operator << (ostream& os, const S_lpsrHeader& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NONE]" << endl;
  }
  
  return os;
}


}

/*
void lpsrHeader::appendRight (
  int    inputLineNumber,
  string value)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceIdentification ()) {
    gLogStream <<
      "Appending right \"" <<
      value <<
      "\" to lpsrHeader" <<
      ", line " << fInputLineNumber <<
      endl;
  }
#endif

  fRightsList.push_back (value);
}

void lpsrHeader::appendComposer (
  int    inputLineNumber,
  string value)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceIdentification ()) {
    gLogStream <<
      "Appending composer \"" <<
      value <<
      "\" to lpsrHeader" <<
      ", line " << fInputLineNumber <<
      endl;
  }
#endif

  fComposersList.push_back (value);
}

void lpsrHeader::appendArranger (
  int    inputLineNumber,
  string value)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceIdentification ()) {
    gLogStream <<
      "Appending arranger \"" <<
      value <<
      "\" to lpsrHeader" <<
      ", line " << fInputLineNumber <<
      endl;
  }
#endif

  fArrangersList.push_back (value);
}

void lpsrHeader::appendLyricist (
  int    inputLineNumber,
  string value)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceIdentification ()) {
    gLogStream <<
      "Appending lyricist \"" <<
      value <<
      "\" to lpsrHeader" <<
      ", line " << fInputLineNumber <<
      endl;
  }
#endif

  fLyricistsList.push_back (value);
}

void lpsrHeader::appendPoet (
  int    inputLineNumber,
  string value)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceIdentification ()) {
    gLogStream <<
      "Appending poet \"" <<
      value <<
      "\" to lpsrHeader" <<
      ", line " << fInputLineNumber <<
      endl;
  }
#endif

  fPoetsList.push_back (value);
}

void lpsrHeader::removeAllPoets (
  int    inputLineNumber)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceIdentification ()) {
    gLogStream <<
      "Removing all poets from lpsrHeader" <<
      ", line " << fInputLineNumber <<
      endl;
  }
#endif

  fPoetsList.clear ();
}

void lpsrHeader::appendTranslator (
  int    inputLineNumber,
  string value)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceIdentification ()) {
    gLogStream <<
      "Appending translator \"" <<
      value <<
      "\" to lpsrHeader" <<
      ", line " << fInputLineNumber <<
      endl;
  }
#endif

  fTranslatorsList.push_back (value);
}

void lpsrHeader::appendArtist (
  int    inputLineNumber,
  string value)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceIdentification ()) {
    gLogStream <<
      "Appending artist \"" <<
      value <<
      "\" to lpsrHeader" <<
      ", line " << fInputLineNumber <<
      endl;
  }
#endif

  fArtistsList.push_back (value);
}

void lpsrHeader::appendSoftware (
  int    inputLineNumber,
  string value)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceIdentification ()) {
    gLogStream <<
      "Appending software \"" <<
      value <<
      "\" to lpsrHeader" <<
      ", line " << fInputLineNumber <<
      endl;
  }
#endif

  fSoftwaresList.push_back (value);
}

*/
