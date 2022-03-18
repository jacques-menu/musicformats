/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include "mfAssert.h"
#include "mfStringsHandling.h"
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

  fKnownToolNames.insert ("xml2ly");
  fKnownToolNames.insert ("xml2brl");
  fKnownToolNames.insert ("xml2xml");
  fKnownToolNames.insert ("xml2gmn");

  fKnownToolNames.insert ("msdlconverter");

  fVariablesTable = mfslVariablesTable::create ();
}

void mfslDriver::setToolName (string value)
{
  if (fDisplayNonTerminals) {
    gLogStream <<
      "====> tool" << value<<
      endl;
  }

  // is this value a known tool name?
  if (! mfStringIsInStringSet (value, fKnownToolNames)) {
    gLogStream <<
      "### Unkown tool name \"" << value << "\"" <<
      ", the known tool names are " <<
      mfStringSetAsString (fKnownToolNames) <<
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
      "] onto scopes stack" <<
      endl << endl;
  }

  fScopesStack.push_front (
    scope);
}

void mfslDriver::popScopeFromStack ()
{
  // fetch the top scope in the stack
  list<S_mfslScope>::iterator
    topIt = fScopesStack.begin ();

  S_mfslScope
    topScope = (*topIt);

  if (fCaseStatementsStack.size () > 1) {
    // fetch the next to top scope in the stack
    list<S_mfslScope>::iterator
      nextToTopIt = next (topIt);

    S_mfslScope
      nextToTopScope = (*nextToTopIt);

    // merge the top scope into the one immediately below it in the stack
    if (fTraceSemantics) {
      gLogStream <<
        "====> Inserting elements from [" <<
        topScope->getScopeName () <<
        "] into [" <<
        nextToTopScope->getScopeName () <<
        endl << endl;
    }

    vector<S_oahOption>&
      optionsVector1 =
        topScope->
          getScopeOptionsVectorToModify ();

    const vector<S_oahOption>&
      optionsVector2 =
        nextToTopScope->
          getScopeOptionsVector ();

    optionsNameAndValueVectorsPlusEquals (
      optionsVector1,
      optionsVector2);
  }

  if (fTraceSemantics) {
    gLogStream <<
      "====> Popping [" <<
      topScope->getScopeName () <<
      "] from the scopes stack" <<
      endl;
  }

  fScopesStack.pop_front ();
}

void mfslDriver::registerOptionInCurrentScope (
  S_oahOption option)
{
  S_mfslScope
    currentScope =
      fScopesStack.front ();

  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    currentScope != nullptr,
    "currentScope is null");

  if (fTraceSemantics) {
    gLogStream <<
      "====> Registering [" <<
      option <<
      "] in (current) \"" <<
      currentScope->getScopeName () <<
      "\" scope" <<
      endl;
  }

  currentScope->
    registerOptionsInScope (
      option);
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

void mfslDriver::popCaseStatementFromStack ()
{
  fCaseStatementsStack.pop_front ();
}

void mfslDriver::appendAllStatementToList (
  S_mfslAllStatement allStatement)
{
  if (fTraceSemantics) {
    gLogStream <<
      "====> Pushing [" <<
      allStatement <<
      "] onto all statements stack" <<
      endl << endl;
  }

  fAllStatementsList.push_back (
    allStatement);
}

// void mfslDriver::popAllStatementFromList ()
// {
//   fAllStatementsList.pop_back ();
// }

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
    S_oahOption
      option :
        mainScope->getScopeOptionsVector ()
  ) {
    s <<
      option->asStringForCommandLine () <<
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


