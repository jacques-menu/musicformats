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

#include "bsrMusicHeadings.h"

#include "oahOah.h"

#include "bsrOah.h"
#include "brailleGenerationOah.h"

#include "msrBrowsers.h"


using namespace std;

namespace MusicFormats
{

//______________________________________________________________________________
S_bsrMusicHeading bsrMusicHeading::create (
  int inputLineNumber)
{
  bsrMusicHeading* o =
    new bsrMusicHeading (
      inputLineNumber);
  assert (o != nullptr);
  return o;
}

bsrMusicHeading::bsrMusicHeading (
  int inputLineNumber)
    : bsrLine (
        inputLineNumber,
        0, // JMI ???
        gGlobalBrailleGenerationOahGroup->getCellsPerLine ())
{}

bsrMusicHeading::~bsrMusicHeading ()
{}

S_bsrCellsList bsrMusicHeading::buildCellsList () const
{
  S_bsrCellsList
    result =
      bsrCellsList::create (fInputLineNumber);

  // append the tempo to result
  if (fMusicHeadingTempo) {
    result->appendCellsListToCellsList (
      fMusicHeadingTempo->fetchCellsList ());
  }

  // append 1 space to result if needed
  if (fMusicHeadingTempo) {
    result->appendCellsListToCellsList (
      bsrSpaces::create (
        fInputLineNumber, 1)->
          fetchCellsList ());
  }

  // append the key to result if any
  if (fMusicHeadingKey) {
    result->appendCellsListToCellsList (
      fMusicHeadingKey->fetchCellsList ());
  }

  // append the time to result if any
  if (fMusicHeadingTimeSignatureSignature) {
    result->appendCellsListToCellsList (
      fMusicHeadingTimeSignatureSignature->fetchCellsList ());
  }

  return result;
}

void bsrMusicHeading::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
    gLogStream <<
      "% ==> bsrMusicHeading::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_bsrMusicHeading>*
    p =
      dynamic_cast<visitor<S_bsrMusicHeading>*> (v)) {
        S_bsrMusicHeading elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
          gLogStream <<
            "% ==> Launching bsrMusicHeading::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void bsrMusicHeading::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
    gLogStream <<
      "% ==> bsrMusicHeading::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_bsrMusicHeading>*
    p =
      dynamic_cast<visitor<S_bsrMusicHeading>*> (v)) {
        S_bsrMusicHeading elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
          gLogStream <<
            "% ==> Launching bsrMusicHeading::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void bsrMusicHeading::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
    gLogStream <<
      "% ==> bsrScore::browseData ()" <<
      endl;
  }
#endif

  if (fMusicHeadingTempo) {
    // browse the tempo
    msrBrowser<bsrTempo> browser (v);
    browser.browse (*fMusicHeadingTempo);
  }

  if (fMusicHeadingKey) {
    // browse the key
    msrBrowser<bsrKey> browser (v);
    browser.browse (*fMusicHeadingKey);
  }

  if (fMusicHeadingTimeSignatureSignature) {
    // browse the time
    msrBrowser<bsrTimeSignature> browser (v);
    browser.browse (*fMusicHeadingTimeSignatureSignature);
  }

#ifdef TRACING_IS_ENABLED
  if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
    gLogStream <<
      "% <== bsrScore::browseData ()" <<
      endl;
  }
#endif
}

string bsrMusicHeading::asString () const
{
  stringstream s;

  s <<
    "MusicHeading" <<
    ", fMusicHeadingTempo: ";

  if (fMusicHeadingTempo) {
    s <<
      fMusicHeadingTempo->asShortString ();
  }
  else {
    s <<
      "[NONE]";
  }

  s <<
      ", fMusicHeadingTempo: ";

  if (fMusicHeadingTempo) {
    s <<
      fMusicHeadingTempo->asShortString ();
  }
  else {
    s <<
      "[NONE]";
  }

  s <<
      ", fMusicHeadingTimeSignatureSignature: ";

  if (fMusicHeadingTimeSignatureSignature) {
    s <<
      fMusicHeadingTimeSignatureSignature->asShortString ();
  }
  else {
    s <<
      "[NONE]";
  }

  s <<
    ", line " << fInputLineNumber;

  return s.str ();
}

string bsrMusicHeading::asDebugString () const
{
  stringstream s;

  if (fMusicHeadingTempo) {
    s <<
      fMusicHeadingTempo->asDebugString ();
  }

  if (fMusicHeadingTempo) {
    s <<
      fMusicHeadingTempo->asDebugString ();
  }

  if (fMusicHeadingTimeSignatureSignature) {
    s <<
      fMusicHeadingTimeSignatureSignature->asDebugString ();
  }

  return s.str ();
}

void bsrMusicHeading::print (ostream& os) const
{
  os <<
    "MusicHeading" <<
    ", line " << fInputLineNumber <<
    endl;

  ++gIndenter;

// JMI  const int fieldWidth = 22;

  os <<
    "musicHeadingTempo: ";
  if (fMusicHeadingTempo) {
    os << endl;

    ++gIndenter;

    os <<
      fMusicHeadingTempo;

    --gIndenter;
  }
  else {
    os <<
      "[NONE]" <<
      endl;
  }

  os <<
    "musicHeadingKey: ";
  if (fMusicHeadingKey) {
    os << endl;

    ++gIndenter;

    os <<
      fMusicHeadingKey;

    --gIndenter;
  }
  else {
    os <<
      "[NONE]" <<
      endl;
  }

  os <<
    "musicHeadingTimeSignatureSignature: ";
  if (fMusicHeadingTimeSignatureSignature) {
    os << endl;

    ++gIndenter;

    os <<
      fMusicHeadingTimeSignatureSignature;

    --gIndenter;
  }
  else {
    os <<
      "[NONE]" <<
      endl;
  }

  os <<
    "musicHeadingCellsList: " <<
    endl;
  ++gIndenter;
  os <<
    buildCellsList () <<
    endl;
  --gIndenter;

  --gIndenter;
}

ostream& operator << (ostream& os, const S_bsrMusicHeading& elt)
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
