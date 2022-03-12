/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include "mfslDriver.h"
#include "mfslParser.h"

using namespace std;


//______________________________________________________________________________
mfslDriver::~mfslDriver ()
{}

mfslDriver::mfslDriver (
  bool traceScanning,
  bool tTraceParsing,
  bool displayTokens,
  bool displayNonTerminals)
{
  fTraceParsing  = traceScanning;
  fTraceScanning = tTraceParsing;

  fDisplayTokens       = displayTokens;
  fDisplayNonTerminals = displayNonTerminals;

  fScannerVariables ["one"] = 1;
  fScannerVariables ["two"] = 2;
}

int mfslDriver::parseFile (const string &inputFileName)
{
  // input file name
  fInputFileName = inputFileName;

  // initialize scanner location
  fScannerLocation.initialize (
    &fInputFileName);

  // begin scan
  scanBegin ();

  // do the parsing
  yy::parser theParser (*this);

  theParser.set_debug_level (
    fTraceParsing);

  int result = theParser ();

  // end scan
  scanEnd ();

  return result;
}
