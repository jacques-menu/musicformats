/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include "mfAssert.h"
#include "mfIndentedTextOutput.h"
#include "mfSystemInterface.h"

#include "mfslInterpreterOah.h"

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
  bool displayNonTerminals,
  bool traceSemantics)
{
  fTraceParsing  = traceScanning;
  fTraceScanning = tTraceParsing;

  fDisplayTokens       = displayTokens;
  fDisplayNonTerminals = displayNonTerminals;

  fTraceSemantics = traceSemantics;

  fVariablesTable = mfslVariablesTable::create ();
}

void mfslDriver::setToolName (string value)
{
  if (fDisplayNonTerminals) {
    gLogStream <<
      "====> tool" << value<<
      endl;
  }

  fToolName = value;
}

void mfslDriver::setInputFileName (string value)
{
  if (fDisplayNonTerminals) {
    gLogStream <<
      "====> input: " << value <<
      endl;
  }

 fInputFileName = value;
}

void mfslDriver::pushScopeOntoStack (
  S_mfslScope scope)
{
  if (fTraceSemantics) {
    gLogStream <<
      "====> Pushing [" <<
      scope <<
      "] onto scopes descrs stack" <<
      endl << endl;
  }

  fScopesStack.push_front (
    scope);
}

void mfslDriver::registerOptionNamesAndValuesInCurrentScope (
  S_oahOptionNameAndValue
    optionNameAndValue)
{
  if (fTraceSemantics) {
    gLogStream <<
      "====> Registering [" <<
      optionNameAndValue <<
      "] in current scope" <<
      endl;
  }

  fScopesStack.front ()->
    registerOptionNamesAndValuesInScope (
      optionNameAndValue);
}

void mfslDriver::pushCaseStatementOntoStack (
  S_mfslCaseStatement caseStatement)
{
  if (fTraceSemantics) {
    gLogStream <<
      "====> Pushing [" <<
      caseStatement <<
      "] onto case statements stack" <<
      endl << endl;
  }

  fCaseStatementsStack.push_front (
    caseStatement);
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

  // print the scopes stack
  if (gGlobalMfslInterpreterOahGroup->getTraceSemantics ()) {
    gLogStream <<
      "====> fScopesStack:" <<
      endl;

    ++gIndenter;

    for (S_mfslScope scope : fScopesStack) {
      gLogStream <<
        scope <<
        endl;
    } // for

    --gIndenter;

    gLogStream << endl;
  }

  // print the variables table
  if (gGlobalMfslInterpreterOahGroup->getTraceSemantics ()) {
    gLogStream <<
      "====> fVariablesTable:";

    if (fVariablesTable) {
      gLogStream << endl;

      ++gIndenter;

      gLogStream <<
        fVariablesTable <<
        endl;

      --gIndenter;
    }
    else {
      gLogStream << "none" << endl;
    }

    gLogStream << endl;
  }

  return result;
}

mfMusicformatsError mfslDriver::launchMfslTool ()
{
  mfMusicformatsError
    result =
      mfMusicformatsError::k_NoError;

  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    fScopesStack.size () == 1,
    "fScopesStack should contain a single element after parsing");

  S_mfslScope
    mainScope = fScopesStack.front ();

  // compose the command line
  stringstream s;

  s <<
    fToolName <<
    ' ' <<
    fInputFileName <<
    ' ';

  for (
    S_oahOptionNameAndValue
      optionNameAndValue :
        mainScope->getScopeOptionsNamesAndValuesVector ()
  ) {
    s <<
      optionNameAndValue->asStringForCommandLine () <<
      ' ';
  } // for

//     ' ' <<
//     fOptionsNamesAndValues->
//       asCommandLineOptionsString ();

  string command = s.str ();

  // launch the tool with the options gathered from the script
  if (fTraceSemantics) {
    gLogStream <<
      "--> executing command: [" << command << "]" <<
      endl << endl;;
  }

  if (! mfExecuteCommand (command, fTraceSemantics)) {
    result =
      mfMusicformatsError::kErrorInvalidFile;
  }

	return result;
}


