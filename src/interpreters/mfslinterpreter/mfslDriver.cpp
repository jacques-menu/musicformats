/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <thread>         // std::this_thread::sleep_for
#include <chrono>         // std::chrono::seconds

/*
int main()
{
  std::cout << "countdown:\n";
  for (int i=10; i>0; --i) {
    std::cout << i << std::endl;
    std::this_thread::sleep_for (std::chrono::seconds(1));
  }
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

//_______________________________________________________________________________
// constants
const string mfslDriver::K_ALL_PSEUDO_LABEL_NAME  = "all";

//______________________________________________________________________________
mfslDriver::mfslDriver ()
{
  // get the script source name
  fScriptName =
    gGlobalServiceRunData->
      getInputSourceName ();

  if (fScriptName == "-") {
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
        "Reading file \"" << fScriptName << "\"" <<
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
  fTraceCaseChoiceStatements =
      gGlobalMfslInterpreterOahGroup->
        getTraceCaseChoiceStatements ().getValue ();

  fTraceInputs =
      gGlobalMfslInterpreterOahGroup->
        getTraceInputs ().getValue ();
  fTraceCaseInputStatements =
      gGlobalMfslInterpreterOahGroup->
        getTraceCaseInputStatements ().getValue ();

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
  fKnownNames.insert ("msdl");

  if (fDisplayToolAndInput) {
    gLogStream <<
      "====> The known tools names are: " <<
      mfStringSetAsString (fKnownNames) <<
      endl;
  }

  // create the driver's choices table
  fChoicesTable = mfslChoicesTable::create ();

  // get the choice labels supplied by options
  fOptionsSuppliedChoicesLabelsMultiMap =
    gGlobalMfslInterpreterOahGroup->
      getSelectChoiceToLabelsMultiMap ();

  if (fTraceChoices) {
    gLogStream <<
      "====> The choice labels set by options are: " <<
      mfStringToStringMultiMapAsString (
        fOptionsSuppliedChoicesLabelsMultiMap) <<
      endl;
  }

  // register all of them as unused
  for (pair<string, string> thePair : fOptionsSuppliedChoicesLabelsMultiMap) {
    string choiceName = thePair.first;

    registerOptionsSuppliedChoicesAsUnused (
      choiceName);
  } // for

  fCaseChoiceStatementsNumber = 0;
}

mfslDriver::~mfslDriver ()
{}

void mfslDriver::setTool (string tool)
{
  if (fDisplayToolAndInput) {
    gLogStream <<
      "====> tool: " << tool<<
      endl;
  }

  // is this tool a known tool name?
  if (! mfStringIsInStringSet (tool, fKnownNames)) {
    gLogStream <<
      "### Unkown tool name \"" << tool << "\"" <<
      ", the known tool names are " <<
      mfStringSetAsString (fKnownNames) <<
      endl;
  }

  fTool = tool;
}

void mfslDriver::appendInputSouce (string inputSouce)
{
  if (fDisplayToolAndInput) {
    gLogStream <<
      "====> input: " << inputSouce <<
      endl;
  }

 fInputSoucesList.push_back (inputSouce);
}

void mfslDriver::optionsBlocksStackPush (
  S_mfslOptionsBlock optionsBlock,
  const string&      context)
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
  S_oahOption option,
  mfslDriver& drv)
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
      option,
      drv);
}

void mfslDriver::registerOptionsSuppliedChoicesAsUsed (
  const string& choiceName)
{
  if (fDisplayOptions) { // JMI
    gLogStream <<
      "====> Registering option-supplied choice [" <<
      choiceName <<
      "] as used" <<
      endl;
  }

  fUnusedOptionsSuppliedChoicesSet.erase (
    choiceName);
}

void mfslDriver::registerOptionsSuppliedChoicesAsUnused (
  const string& choiceName)
{
  if (fDisplayOptions) { // JMI
    gLogStream <<
      "====> Registering option-supplied choice [" <<
      choiceName <<
      "] as used" <<
      endl;
  }

  fUnusedOptionsSuppliedChoicesSet.erase (
    choiceName);
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

void mfslDriver::caseChoiceStatementsStackPush (
  S_mfslCaseChoiceStatement caseChoiceStatement)
{
  if (fTraceCaseChoiceStatements) {
    gLogStream <<
      "====> Pushing [" <<
      caseChoiceStatement->asString () <<
      "] onto the case statements stack" <<
      endl;
  }

  ++fCaseChoiceStatementsNumber;

  fCaseChoiceStatementsStack.push_front (
    caseChoiceStatement);

  if (fTraceCaseChoiceStatements) {
    displayCaseChoiceStatementsStack (
      "after pushing a case statement");
  }
}

S_mfslCaseChoiceStatement mfslDriver::caseChoiceStatementsStackTop () const
{
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    fCaseChoiceStatementsStack.size (),
    "caseChoiceStatementsStackTop(): fCaseChoiceStatementsStack is empty");

  return fCaseChoiceStatementsStack.front ();
}

void mfslDriver::caseChoiceStatementsStackPop ()
{
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    fCaseChoiceStatementsStack.size () != 0,
    "caseChoiceStatementsStackPop(): fCaseChoiceStatementsStack is empty");

  if (fTraceCaseChoiceStatements) {
    gLogStream <<
      "====> Popping [" <<
      fCaseChoiceStatementsStack.front ()->asString () <<
      "] from the case statements stack" <<
      endl;
  }

  fCaseChoiceStatementsStack.pop_front ();

  if (fTraceCaseChoiceStatements) {
    displayCaseChoiceStatementsStack (
      "after popping a case statement");
  }
}

void mfslDriver::displayCaseChoiceStatementsStack (
  const string& context) const
{
  gLogStream <<
    "Case statements stack" <<
    ", context: " << context <<
    endl;

  if (fCaseChoiceStatementsStack.size ()) {
    ++gIndenter;

    for (S_mfslCaseChoiceStatement caseChoiceStatement : fCaseChoiceStatementsStack) {
      gLogStream << caseChoiceStatement;
    } // for

    --gIndenter;
  }

  else {
    gLogStream << "empty" << endl;
  }
}

void mfslDriver::caseInputStatementsStackPush (
  S_mfslCaseInputStatement caseInputStatement)
{
  if (fTraceCaseInputStatements) {
    gLogStream <<
      "====> Pushing [" <<
      caseInputStatement->asString () <<
      "] onto the case statements stack" <<
      endl;
  }

  ++fCaseInputStatementsNumber;

  fCaseInputStatementsStack.push_front (
    caseInputStatement);

  if (fTraceCaseInputStatements) {
    displayCaseInputStatementsStack (
      "after pushing a case statement");
  }
}

S_mfslCaseInputStatement mfslDriver::caseInputStatementsStackTop () const
{
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    fCaseInputStatementsStack.size (),
    "caseInputStatementsStackTop(): fCaseInputStatementsStack is empty");

  return fCaseInputStatementsStack.front ();
}

void mfslDriver::caseInputStatementsStackPop ()
{
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    fCaseInputStatementsStack.size () != 0,
    "caseInputStatementsStackPop(): fCaseInputStatementsStack is empty");

  if (fTraceCaseInputStatements) {
    gLogStream <<
      "====> Popping [" <<
      fCaseInputStatementsStack.front ()->asString () <<
      "] from the case statements stack" <<
      endl;
  }

  fCaseInputStatementsStack.pop_front ();

  if (fTraceCaseInputStatements) {
    displayCaseInputStatementsStack (
      "after popping a case statement");
  }
}

void mfslDriver::displayCaseInputStatementsStack (
  const string& context) const
{
  gLogStream <<
    "Case statements stack" <<
    ", context: " << context <<
    endl;

  if (fCaseInputStatementsStack.size ()) {
    ++gIndenter;

    for (S_mfslCaseInputStatement caseInputStatement : fCaseInputStatementsStack) {
      gLogStream << caseInputStatement;
    } // for

    --gIndenter;
  }

  else {
    gLogStream << "empty" << endl;
  }
}

int mfslDriver::parseInput_Pass1 ()
{
  // initialize scanner location
  fScannerLocation.initialize (
    &fScriptName);

  // begin scan
  scanBegin ();

  if (fScriptName.empty () || fScriptName == "-") {
    fScriptName = "stdin"; // nicer for warning and error messages
  }

  // do the parsing
  mfsl::parser theParser (*this);

  theParser.set_debug_level (
    fTraceParsing);

  int parseResult = theParser ();

  // end scan
  scanEnd ();

  // print the basic results
  if (fTraceParsing) {
    gLogStream <<
      "--> parseResult:   " << parseResult <<
      endl;

    gLogStream <<
      "--> fTool:      " << fTool <<
      endl <<
      "--> fInputSoucesList: " <<
      endl;

    ++gIndenter;
    for (string inputSouce : fInputSoucesList ) {
      gLogStream <<
        inputSouce <<
        endl;
    } // for
    --gIndenter;
  }

  // print the options blocks stack if relevant
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

  // print the choices table if relevant
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

  // do the final semantics check
  finalSemanticsCheck ();

  return parseResult;
}

void mfslDriver::handleSelectLabel (
  const string& choiceName,
  const string& label)
{
  if (fTraceChoices) {
    gLogStream <<
      "====> appendSelectLabelForToolLaunching()" <<
      ", choiceName: " << choiceName <<
      ", label: " << label <<
      endl;
  }

  // analyze this select command
  S_mfslChoice
    choice =
      fChoicesTable->
        fetchChoiceByNameNonConst (
          choiceName,
          *this);

  if (choice) {
    // register the options block to use for 'select' launching
    if (fTraceChoices) {
      gLogStream <<
        "====> appendSelectLabelForToolLaunching()" <<
        ", choice: " <<
        endl;
      ++gIndenter;
      gLogStream <<
        choice;
      --gIndenter;
    }

    const set<string>&
      choiceLabelsSet =
        choice->
          getLabelsSet ();

    if (fTraceChoices) {
      mfDisplayStringSet (
        "====> choiceLabelsSet",
        choiceLabelsSet,
        gLogStream);
    }

    if (label == K_ALL_PSEUDO_LABEL_NAME) {
      for (string choiceLabel : choiceLabelsSet) {
        appendSelectLabelForToolLaunching (
          choice,
          choiceLabel,
          true); // allLabelSelected
      }
    }

    else if (mfStringIsInStringSet (label, choiceLabelsSet)) {
      appendSelectLabelForToolLaunching (
        choice,
        label,
        false); // allLabelSelected
    }

    else {
      stringstream s;

      s <<
        "label \"" << label <<
        "\" is no label of choice nor 'all' \"" <<
        choiceName <<
        "\", cannot be used in a 'select' statement";

      mfslError (
        s.str (),
        fScannerLocation);
    }
  }

  else {
    stringstream s;

    s <<
      "choice name \"" << choiceName <<
      "\" is unknown in the choices table, cannot be used in a 'select' statement";

    mfslError (
      s.str (),
      fScannerLocation);
  }
}

void mfslDriver::appendSelectLabelForToolLaunching (
  const S_mfslChoice choice,
  const string&      label,
  Bool               allLabelSelected)
{
  string
    choiceName =
      choice->getChoiceName ();

  if (fTraceChoices) {
    gLogStream <<
      "====> appendSelectLabelForToolLaunching()" <<
      ", choiceName: " << choiceName <<
      ", label: " << label <<
      endl;
  }

  S_mfslOptionsBlock
    selectOptionsBlock =
      choice->
        getChoiceOptionsBlockForLabel (
          label,
          *this);

  // dont apply this select statement
  // if an option for the same choice label been used

  Bool overriddenMessageHasBeenIssued (false);

  if (
    gGlobalMfslInterpreterOahGroup->
      getSelectChoiceToLabelsMultiMapAtom ()->
        getSetByAnOption ()
  ) {
    if (! overriddenMessageHasBeenIssued) {
      stringstream s;

      s <<
        "'select' label \"" <<
        label <<
        "\" for choice \"" <<
        choiceName <<
        "\" ignored, it is overridden by a '-select, -sel' option";

      mfslWarning (
        s.str (),
        fScannerLocation);

      // issue above warning only one if the 'all' pseudo-label has be selected
      overriddenMessageHasBeenIssued = true;
    }
  }

  else {
    S_mfslOptionsBlock
      selectOptionsBlock =
        choice->
          getChoiceOptionsBlockForLabel (
            label,
            *this);

    if (fTraceChoices) {
      gLogStream <<
        "====> optionsBlock from script:" <<
        endl;
      ++gIndenter;
      gLogStream <<
        selectOptionsBlock;
      --gIndenter;
    }

    // this 'select' statement is to be applied
    fSelectedOptionsBlocksList.push_back (
      selectOptionsBlock);
  }
}

mfMusicformatsError mfslDriver::launchMfslTool_Pass2 ()
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
      "====> Launching " <<
      fTool <<
      " with the argument and option gathered from " <<
      fScriptName <<
      endl;
  }

  // sanity checks
  mfAssert (
    __FILE__, __LINE__,
    fCaseChoiceStatementsStack.size () == 0,
    "fCaseChoiceStatementsStack should be empty after parsing");

  // populate the commands list with the options gathered in the script
  populateTheCommandsList ();

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
      " *NOT* executed" <<
      endl;
  }

  else {
    for (string command : fCommandsList) {
      if (fDisplayToolAndInput) {
        gLogStream <<
          "====> Running the tool with command: [" << command << "]" <<
          endl;
      }

      int
        commandExecutionResult =
          mfExecuteCommand (
            command,
            fDisplayToolAndInput);

      if (fDisplayToolAndInput) {
        gLogStream <<
          "====> The execution result is: " <<
          commandExecutionResult <<
          endl;
      }

      if (commandExecutionResult) {
        result =
          mfMusicformatsError::kErrorInvalidFile;
      }

      // sleep for some milliseconds
          this_thread::sleep_for (chrono::milliseconds (100));
    } // for
  }

	return result;
}

Bool mfslDriver::applySelectOptionsFinally ()
{
  Bool result;

  if (fTraceChoices) {
    gLogStream <<
      "====> Finally applying 'select' options" <<
      endl;
  }

  const multimap<string, string>&
    selectChoiceToLabelsMultiMap =
      gGlobalMfslInterpreterOahGroup->
        getSelectChoiceToLabelsMultiMap ();

  if (fTraceChoices) {
    mfDisplayStringToStringMultiMap (
      "====> applySelectOptionsFinally(), selectChoiceToLabelsMultiMap",
      selectChoiceToLabelsMultiMap,
      gLogStream);
  }

  for (pair<string, string> thePair : selectChoiceToLabelsMultiMap) {
    string
      optionSuppliedChoiceName =
        thePair.first,
      optionSuppliedLabel =
        thePair.second;

    if (fTraceChoices) {
      gLogStream <<
        "====> applySelectOptionsFinally():" <<
        "optionSuppliedChoiceName\"" <<
        optionSuppliedChoiceName <<
        "\"" <<
        ", optionSuppliedLabel\"" <<
        optionSuppliedLabel <<
        "\"" <<
        endl;
    }

    S_mfslChoice
      choice =
        fChoicesTable->
          fetchChoiceByNameNonConst (
            optionSuppliedChoiceName,
            *this);

    if (optionSuppliedLabel == K_ALL_PSEUDO_LABEL_NAME) {
      const set<string>&
        choiceLabelsSet =
          choice->
            getLabelsSet ();

      if (fTraceChoices) {
        mfDisplayStringSet (
          "====> choiceLabelsSet",
          choiceLabelsSet,
          gLogStream);
      }

      for (string choiceLabel : choiceLabelsSet) {
        result =
          applySelectOption (
            choice,
            choiceLabel);
      } // for
    }

    else {
      result =
        applySelectOption (
          choice,
          optionSuppliedLabel);
    }
  } // for

  return result;
}

Bool mfslDriver::applySelectOption (
  const S_mfslChoice choice,
  const string&      label)
{
  Bool result;

  const string
    choiceName =
      choice->getChoiceName ();

  if (fTraceChoices) {
    gLogStream <<
      "====> option-supplied label \"" <<
      label <<
      "\" for choice \"" <<
      choiceName <<
      "\" is being applied" <<
      endl;
  }

  const set<string>&
    choiceLabelsSet =
      choice->
        getLabelsSet ();

  if (fTraceChoices) {
    mfDisplayStringSet (
      "====> choiceLabelsSet",
      choiceLabelsSet,
      gLogStream);
  }

  Bool
    labelIsKnwonToTheChoice =
      mfStringIsInStringSet (
        label,
        choiceLabelsSet);

  if (labelIsKnwonToTheChoice) {
    S_mfslChoice
      optionSuppliedChoice =
        fChoicesTable->
          fetchChoiceByNameNonConst (
            choiceName,
            *this);

    S_mfslOptionsBlock
      selectOptionsBlock =
        optionSuppliedChoice->
          getChoiceOptionsBlockForLabel (
            label,
            *this);

    fSelectedOptionsBlocksList.push_back (
      selectOptionsBlock);

    if (fTraceChoices) {
      gLogStream <<
        "====> selectOptionsBlock by an option:" <<
        endl;
      ++gIndenter;
      gLogStream <<
        selectOptionsBlock;
      --gIndenter;
    }
  }

  else {
    stringstream s;

    s <<
      "applySelectOption(): label \"" <<
      label <<
      "\" is unknown in choice \"" <<
      choiceName <<
      "\"";

    mfslError (
      s.str (),
      fScannerLocation);
  }

  return result;
}

void mfslDriver::finalSemanticsCheck ()
{
  // have all the options supplied choices been used?
  for (string choiceName : fUnusedOptionsSuppliedChoicesSet) {
    stringstream s;

    s <<
      "option-supplied choice \"" <<
      choiceName <<
      "\" has not been used in script \"" <<
      fScriptName <<
      "\"";

    mfslWarning (
      s.str (),
      fScannerLocation);
  } // for

  // are there pending 'select' options?
  S_oahStringToStringMultiMapElementAtom
    selectChoiceToLabelsMultiMapAtom =
      gGlobalMfslInterpreterOahGroup->
        getSelectChoiceToLabelsMultiMapAtom ();

  if (
    selectChoiceToLabelsMultiMapAtom->getSetByAnOption ()
  ) {
    applySelectOptionsFinally ();
  }
}

void mfslDriver::populateTheCommandsList ()
{
  for (string inputSouce : fInputSoucesList ) {
    // the tool and input file source as string
    string
      toolAndInputAsString =
        fTool +
        ' ' +
        inputSouce;

    // the main options block options as string
    string
      mainOptionsAsString;

    if (fMainOptionsBlock) {
      mainOptionsAsString =
        fMainOptionsBlock->
          asOptionsString ();
    }

    // what has been found in the script?
    if (fCaseChoiceStatementsNumber == 0) {
      // the options to be used are in the main options block alone
      fCommandsList.push_back (
        toolAndInputAsString
          +
        ' '
          +
        mainOptionsAsString);
    }

    else {
      // there are case statements

      if (fSelectedOptionsBlocksList.size ()) {
        // 'select' statement have been supplied,
        // either in the script or by an option

        for (S_mfslOptionsBlock optionsBlock :fSelectedOptionsBlocksList ) {
          // the 'select' choice options block options as string
          string
            selectChoiceOptionsAsString =
              optionsBlock->
                asOptionsString ();

          // append it to the commands list
          fCommandsList.push_back (
            toolAndInputAsString
              +
            ' '
              +
            mainOptionsAsString
              +
            ' '
              +
            selectChoiceOptionsAsString);
        } // for
      }

      else {
        // there are no 'select' statements,
        // check that there is only one choice in the script

        // get the choices table
        const map<string, S_mfslChoice>&
          choicesMultiMap =
            fChoicesTable->
              getChoicesMap ();

        int
          choicesNumber =
            choicesMultiMap.size ();

        if (choicesNumber == 1) {
          // and use this single choice's default label

          // grab the single choice
          S_mfslChoice
            singleChoice =
              (*( choicesMultiMap.begin ())).second;

          // get its default label
          string
            singleChoiceDefaultLabel =
              singleChoice->
                getChoiceDefaultLabel ();

          // get the options to be used
          S_mfslOptionsBlock
            optionsBlockToBeUsed =
              singleChoice->
                getChoiceOptionsBlockForLabel (
                  singleChoiceDefaultLabel,
                  *this);

          // fetch the options to be used as a string
          string
            optionsBlockToBeUsedAsString =
              optionsBlockToBeUsed->
                asOptionsString ();

          // append it to the commands list
          fCommandsList.push_back (
            toolAndInputAsString
              +
            ' '
              +
            mainOptionsAsString
              +
            ' '
              +
            optionsBlockToBeUsedAsString);
        }

        else {
          mfslInternalError (
            "there can be only 1 choice if there is no 'select' statement",
            fScannerLocation);
        }
      }
    }
  } // for
}
