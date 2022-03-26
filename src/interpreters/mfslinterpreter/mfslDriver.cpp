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

#include "mfslWae.h"


using namespace std;

//______________________________________________________________________________
mfslDriver::~mfslDriver ()
{}

mfslDriver::mfslDriver ()
{
  // get the script source name
  fScriptSourceName =
    gGlobalServiceRunData->
      getInputSourceName ();

  if (fScriptSourceName == "-") {
    // MFSL data comes from standard input
#ifdef TRACING_IS_ENABLED
    if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
      gLogStream << "Reading standard input" << endl;
    }
#endif
	}
  else {
    // MFSL data comes from a file
#ifdef TRACING_IS_ENABLED
    if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
      gLogStream <<
        "Reading file \"" << fScriptSourceName << "\"" <<
        endl;
    }
#endif
	}

  // get the options values as bool,
  // since Bool is unknown to Flex and Bison-generated code
  fTraceScanning =
      gGlobalMfslInterpreterOahGroup->
        getTraceScanning ().getValue ();

  fDisplayTokens =
      gGlobalMfslInterpreterOahGroup->
        getDisplayTokens ().getValue ();

  fTraceParsing =
      gGlobalMfslInterpreterOahGroup->
        getTraceParsing ().getValue ();

  fDisplayToolAndInput =
      gGlobalMfslInterpreterOahGroup->
        getDisplayToolAndInput ().getValue ();

  fDisplayOptions =
      gGlobalMfslInterpreterOahGroup->
        getDisplayOptions ().getValue ();

  fTraceChoices =
      gGlobalMfslInterpreterOahGroup->
        getTraceChoices ().getValue ();
  fTraceChoiceStatements =
      gGlobalMfslInterpreterOahGroup->
        getTraceChoiceStatements ().getValue ();

  fTraceCaseStatements =
      gGlobalMfslInterpreterOahGroup->
        getTraceCaseStatements ().getValue ();

  fNoLaunch =
      gGlobalMfslInterpreterOahGroup->
        getNoLaunch ().getValue ();

  fTraceOptionsBlocks =
      gGlobalMfslInterpreterOahGroup->
        getTraceOptionsBlocks ().getValue ();

  fNoLaunch =
      gGlobalMfslInterpreterOahGroup->
        getNoLaunch ().getValue ();

  // register the known MusicFormats tools
  fKnownNames.insert ("xml2ly");
  fKnownNames.insert ("xml2brl");
  fKnownNames.insert ("xml2xml");
  fKnownNames.insert ("xml2gmn");
  fKnownNames.insert ("msdlconverter");

  if (fDisplayToolAndInput) {
    gLogStream <<
      "====> The known tools names are: " <<
      mfStringSetAsString (fKnownNames) <<
      "\"" <<
      endl;
  }

  // create the driver's choices table
  fChoicesTable = mfslChoicesTable::create ();

  // get the choice labels supplied by options
  fOptionsSuppliedChoicesLabelsMap =
    gGlobalMfslInterpreterOahGroup->
      getGenerateChoiceToLabelsMap ();

  if (fTraceChoices) {
    gLogStream <<
      "====> The choice labels set by options are: " <<
      mfStringToStringMapAsString (
        fOptionsSuppliedChoicesLabelsMap) <<
      endl;
  }

  // register all of them as unused
  for (pair<string, string> thePair : fOptionsSuppliedChoicesLabelsMap) {
    string choiceName = thePair.first;

    fUnusedOptionsSuppliedChoicesSet.insert (
      choiceName);
  } // for
}

void mfslDriver::setToolName (string toolName)
{
  if (fDisplayToolAndInput) {
    gLogStream <<
      "====> tool " << toolName<<
      endl;
  }

  // is this toolName a known tool name?
  if (! mfStringIsInStringSet (toolName, fKnownNames)) {
    gLogStream <<
      "### Unkown tool name \"" << toolName << "\"" <<
      ", the known tool names are " <<
      mfStringSetAsString (fKnownNames) <<
      endl;
  }

  fToolName = toolName;
}

void mfslDriver::setInputFileName (string inputFileName)
{
  if (fDisplayToolAndInput) {
    gLogStream <<
      "====> input: " << inputFileName <<
      endl;
  }

 fInputFileName = inputFileName;
}

void mfslDriver::optionsBlocksStackPush (
  S_mfslOptionsBlock optionsBlock,
  const string& context)
{
  if (fTraceOptionsBlocks) {
    gLogStream <<
      "====> Pushing [" <<
      optionsBlock->asString () <<
      "] onto the options blocks stack" <<
      ", context: " << context <<
      endl;
  }

  if (fOptionsBlocksStack.size () == 0) {
    // first push on the stack
    fMainOptionsBlock = optionsBlock;
  }

  fOptionsBlocksStack.push_front (
    optionsBlock);

  if (fTraceOptionsBlocks) {
    ++gIndenter;
    displayOptionsBlocksStack (context);
    --gIndenter;
  }
}

S_mfslOptionsBlock mfslDriver::optionsBlocksStackTop () const
{
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    fOptionsBlocksStack.size (),
    "optionsBlocksStackTop(): fOptionsBlocksStack is empty");

  return fOptionsBlocksStack.front ();
}

void mfslDriver::registerOptionInCurrentOptionsBlock (
  S_oahOption option)
{
  S_mfslOptionsBlock
    currentOptionsBlock =
      fOptionsBlocksStack.front ();

  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    currentOptionsBlock != nullptr,
    "currentOptionsBlock is null");

  if (fDisplayOptions) { // JMI
    gLogStream <<
      "====> Registering option [" <<
      option->asString () <<
      "] in (current) \"" <<
      currentOptionsBlock->getOptionsBlockName () <<
      "\" options block" <<
      endl;
  }

  currentOptionsBlock->
    registerOptionsInOptionsBlock (
      option);
}

void mfslDriver::optionsBlocksStackPop (
  const string& context)
{
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    fOptionsBlocksStack.size () != 0,
    "optionsBlocksStackPop(): fOptionsBlocksStack is empty");

  if (fTraceOptionsBlocks) {
    gLogStream <<
      "====> Popping [" <<
      fOptionsBlocksStack.front ()->asString () <<
      "] from the options blocks stack" <<
      ", context: " << context <<
      endl;
  }

  fOptionsBlocksStack.pop_front ();

  if (fTraceOptionsBlocks) {
    ++gIndenter;
    displayOptionsBlocksStack (context);
    --gIndenter;
  }
}

void mfslDriver::displayOptionsBlocksStack (
  const string& context) const
{
  gLogStream <<
    "OptionsBlocks stack, context: " <<
    context <<
    ":" ;

  if (fOptionsBlocksStack.size ()) {
    gLogStream << endl;

    ++gIndenter;

    for (S_mfslOptionsBlock optionsBlock : fOptionsBlocksStack) {
      gLogStream << optionsBlock;
    } // for

    --gIndenter;
  }

  else {
    gLogStream << "empty" << endl;
  }
}

void mfslDriver::caseStatementsStackPush (
  S_mfslCaseStatement caseStatement)
{
  if (fTraceCaseStatements) {
    gLogStream <<
      "====> Pushing [" <<
      caseStatement->asString () <<
      "] onto the case statements stack" <<
      endl;
  }

  fCaseStatementsStack.push_front (
    caseStatement);

  if (fTraceCaseStatements) {
    displayCaseStatementsStack (
      "after pushing a case statement");
  }
}

S_mfslCaseStatement mfslDriver::caseStatementsStackTop () const
{
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    fCaseStatementsStack.size (),
    "caseStatementsStackTop(): fCaseStatementsStack is empty");

  return fCaseStatementsStack.front ();
}

void mfslDriver::caseStatementsStackPop ()
{
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    fCaseStatementsStack.size () != 0,
    "caseStatementsStackPop(): fCaseStatementsStack is empty");

  if (fTraceCaseStatements) {
    gLogStream <<
      "====> Popping [" <<
      fCaseStatementsStack.front ()->asString () <<
      "] from the case statements stack" <<
      endl;
  }

  fCaseStatementsStack.pop_front ();

  if (fTraceCaseStatements) {
    displayCaseStatementsStack (
      "after popping a case statement");
  }
}

void mfslDriver::displayCaseStatementsStack (
  const string& context) const
{
  gLogStream <<
    "Case statements stack" <<
    ", context: " << context <<
    endl;

  if (fCaseStatementsStack.size ()) {
    ++gIndenter;

    for (S_mfslCaseStatement caseStatement : fCaseStatementsStack) {
      gLogStream << caseStatement;
    } // for

    --gIndenter;
  }

  else {
    gLogStream << "empty" << endl;
  }
}

int mfslDriver::parseInput ()
{
  // initialize scanner location
  fScannerLocation.initialize (
    &fScriptSourceName);

  // begin scan
  scanBegin ();

  // do the parsing
  yy::parser theParser (*this);

  theParser.set_debug_level (
    fTraceParsing);

  int result = theParser ();

  // end scan
  scanEnd ();

  // print the options blocks stack
  if (gGlobalMfslInterpreterOahGroup->getTraceOptionsBlocks ()) {
    gLogStream <<
      "====> fOptionsBlocksStack:" <<
      endl;

    ++gIndenter;

    for (S_mfslOptionsBlock optionsBlock : fOptionsBlocksStack) {
      gLogStream <<
        optionsBlock <<
        endl;
    } // for

    --gIndenter;

    gLogStream;
  }

  // print the choices table
  if (
    gGlobalMfslInterpreterOahGroup->getTraceChoices ()
      ||
    gGlobalMfslInterpreterOahGroup->getNoLaunch ()
  ) {
    if (fChoicesTable->getChoicesMap ().size ()) {
      gLogStream <<
        "====> fChoicesTable:";

      if (fChoicesTable) {
        gLogStream << endl;

        ++gIndenter;

        gLogStream <<
          fChoicesTable <<
          endl;

        --gIndenter;
      }
      else {
        gLogStream << "none" << endl;
      }

      gLogStream << endl;
    }
  }

  // have all the options supplied choices been used?
  for (string choiceName : fUnusedOptionsSuppliedChoicesSet) {
    stringstream s;

    s <<
      "option supplied choice \"" <<
      choiceName <<
      "\" has not been used in script \"" <<
      fScriptSourceName <<
      "\"";

    mfslWarning (
      s.str (),
      yy::location (
        yy::position (),
        yy::position ())); // JMI
  } // for

  return result;
}

void mfslDriver::setOnlyLabelForToolLaunching (
  const string& choiceName,
  const string& label)
{
  S_mfslChoice
    choice =
      fChoicesTable->
        lookupChoiceByName (
          choiceName);

  if (choice) {
    // register the options block to use for 'only' launching
    fOptionsBlockToUseForOnlyLaunching =
      choice->
        getChoiceOptionsBlockForLabel (
          label);
  }

  else {
    stringstream s;

    s <<
      "choiceName \"" << choiceName <<
      "\" is no choice name";

    mfslError (
      s.str (),
      fScannerLocation);
  }
}

void mfslDriver::setAllChoicesOptionsBlockForToolLaunching (
  const string& choiceName)
{
  S_mfslChoice
    choice =
      fChoicesTable->
        lookupChoiceByName (
          choiceName);

  if (choice) {
    // register the choice to use for 'all' launching
    fChoiceToUseForAllLaunching =
      choice;
  }

  else {
    stringstream s;

    s <<
      "choiceName \"" << choiceName <<
      "\" is no choice name";

    mfslError (
      s.str (),
      fScannerLocation);
  }
}

mfMusicformatsError mfslDriver::launchMfslTool ()
{
  mfMusicformatsError
    result =
      mfMusicformatsError::k_NoError;

  mfAssert (
    __FILE__, __LINE__,
    fOptionsBlocksStack.size () == 1,
    "fOptionsBlocksStack should contain only the main options block after parsing");

  if (fDisplayToolAndInput) {
    gLogStream <<
      "====> Launching  tool " << fToolName <<
      endl;
  }

  // sanity checks
  mfAssert (
    __FILE__, __LINE__,
    fCaseStatementsStack.size () == 0,
    "fCaseStatementsStack should be empty after parsing");

  // the tool and input file source as string
  string
    toolAndInputAsString =
      fToolName +
      ' ' +
      fInputFileName;

  // the main options block options as string
  string
    mainOptionsAsString;

  if (fMainOptionsBlock) {
    mainOptionsAsString =
      fMainOptionsBlock->
        asOptionsString ();
  }

  // populate the commands list with the options gathered in the script
  if (fChoiceToUseForAllLaunching) {
    // an 'all' statement has been supplied,
    // either in the script or by an option

    const set<string>&
      labelsSet =
        fChoiceToUseForAllLaunching->
          getLabelsSet ();

    for (string label : labelsSet) {
      // fetch the options block
      S_mfslOptionsBlock
        optionsBlock =
          fChoiceToUseForAllLaunching->
            getChoiceOptionsBlockForLabel (
              label);

      string
        labelOptionsBlockAsString =
          optionsBlock->
            asOptionsString ();

      fCommandsList.push_back (
        toolAndInputAsString
          +
        ' '
          +
        labelOptionsBlockAsString);
    } // for
  }

  else if (fOptionsBlockToUseForOnlyLaunching) {
    // an 'only' statement has been supplied,
    // either in the script or by an option

    // the only choice options block options as string
    string
      onlyChoiceOptionsAsString =
        fOptionsBlockToUseForOnlyLaunching->
          asOptionsString ();

    fCommandsList.push_back (
      toolAndInputAsString
        +
      ' '
        +
      onlyChoiceOptionsAsString);
  }

  else {
    // the options to be uses are in the main options block alone
    fCommandsList.push_back (
      toolAndInputAsString
        +
      ' '
        +
      mainOptionsAsString);
  }

  // display the commands list
  if (fDisplayToolAndInput) {
    gLogStream <<
      "====> The " <<
      mfSingularOrPluralWithoutNumber (
        fCommandsList.size (),
        "command", "commands") <<
      " to be executed " <<
      mfSingularOrPluralWithoutNumber (
        fCommandsList.size (),
        "is", "are") <<
      ":" <<
      endl;

    ++gIndenter;

    for (string command : fCommandsList) {
      gLogStream <<
        command <<
        endl;
    } // for

    --gIndenter;
  }

  // execute the commands in the list
  if (fNoLaunch) {
    gLogStream <<
      "====> The " <<
      mfSingularOrPluralWithoutNumber (
        fCommandsList.size (),
        "command", "commands") <<
      " above " <<
      mfSingularOrPluralWithoutNumber (
        fCommandsList.size (),
        "is", "are") <<
      " *not* executed" <<
      endl;
  }

  else {
    for (string command : fCommandsList) {
      if (fDisplayToolAndInput) {
        gLogStream <<
          "====> Executing command: [" << command << "]" <<
          endl;
      }

      if (! mfExecuteCommand (command, fDisplayToolAndInput)) {
        result =
          mfMusicformatsError::kErrorInvalidFile;
      }
    } // for
  }

	return result;
}
