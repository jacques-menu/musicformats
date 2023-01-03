/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

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

#include "ischemeInterpreterOah.h"

#include "ischemeDriver.h"
#include "ischemeParser.h"

#include "ischemeWae.h"


//_______________________________________________________________________________
// constants
const std::string ischemeDriver::K_ALL_PSEUDO_LABEL_NAME  = "all";

//______________________________________________________________________________
ischemeDriver::ischemeDriver ()
{
  // get the script source name
  fScriptName =
    gGlobalServiceRunData->
      getInputSourceName ();

  if (fScriptName == "-") {
    // iScheme data comes from standard input
#ifdef OAH_TRACING_IS_ENABLED
    if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
      gLogStream << "Reading standard input" << std::endl;
    }
#endif
  }

  else {
    // iScheme data comes from a file
#ifdef OAH_TRACING_IS_ENABLED
    if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
      gLogStream <<
        "Reading file \"" << fScriptName << "\"" <<
        std::endl;
    }
#endif
  }

  // get the options values as bool,
  // since Bool is unknown to Flex and Bison-generated code
  fTraceScanning =
      gGlobalIschemeInterpreterOahGroup->
        getTraceScanning ().getValue ();

  fDisplayTokens =
      gGlobalIschemeInterpreterOahGroup->
        getDisplayTokens ().getValue ();

  fTraceParsing =
      gGlobalIschemeInterpreterOahGroup->
        getTraceParsing ().getValue ();

  fDisplayToolAndInput =
      gGlobalIschemeInterpreterOahGroup->
        getDisplayToolAndInput ().getValue ();

  fDisplayOptions =
      gGlobalIschemeInterpreterOahGroup->
        getDisplayOptions ().getValue ();

  fTraceChoices =
      gGlobalIschemeInterpreterOahGroup->
        getTraceChoices ().getValue ();
  fTraceCaseChoiceStatements =
      gGlobalIschemeInterpreterOahGroup->
        getTraceCaseChoiceStatements ().getValue ();

  fTraceInputs =
      gGlobalIschemeInterpreterOahGroup->
        getTraceInputs ().getValue ();
  fTraceCaseInputStatements =
      gGlobalIschemeInterpreterOahGroup->
        getTraceCaseInputStatements ().getValue ();

  fNoLaunch =
      gGlobalIschemeInterpreterOahGroup->
        getNoLaunch ().getValue ();

  fTraceOptionsBlocks =
      gGlobalIschemeInterpreterOahGroup->
        getTraceOptionsBlocks ().getValue ();

  fNoLaunch =
      gGlobalIschemeInterpreterOahGroup->
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
      std::endl;
  }

  // create the driver's choices table
  fChoicesTable = ischemeChoicesTable::create ();

  // get the choice labels supplied by options
  fOptionsSuppliedChoicesLabelsMultiMap =
    gGlobalIschemeInterpreterOahGroup->
      getSelectChoiceToLabelsMultiMap ();

  if (fTraceChoices) {
    gLogStream <<
      "====> The choice labels set by options are: " <<
      mfStringToStringMultiMapAsString (
        fOptionsSuppliedChoicesLabelsMultiMap) <<
      std::endl;
  }

  // register all of them as unused
  for (std::pair<std::string, std::string> thePair : fOptionsSuppliedChoicesLabelsMultiMap) {
    std::string choiceName = thePair.first;

    registerOptionsSuppliedChoicesAsUnused (
      choiceName);
  } // for

  fCaseChoiceStatementsNumber = 0;
}

ischemeDriver::~ischemeDriver ()
{}

void ischemeDriver::setTool (std::string tool)
{
  if (fDisplayToolAndInput) {
    gLogStream <<
      "====> tool: " << tool<<
      std::endl;
  }

  // is this tool a known tool name?
  if (! mfStringIsInStringSet (tool, fKnownNames)) {
    gLogStream <<
      "### Unkown tool name \"" << tool << "\"" <<
      ", the known tool names are " <<
      mfStringSetAsString (fKnownNames) <<
      std::endl;
  }

  fTool = tool;
}

void ischemeDriver::appendInputSouce (std::string inputSouce)
{
  if (fDisplayToolAndInput) {
    gLogStream <<
      "====> input: " << inputSouce <<
      std::endl;
  }

 fInputSoucesList.push_back (inputSouce);
}

void ischemeDriver::optionsBlocksStackPush (
  const S_ischemeOptionsBlock& optionsBlock,
  const std::string&      context)
{
  if (fTraceOptionsBlocks) {
    gLogStream <<
      "====> Pushing [" <<
      optionsBlock->asString () <<
      "] onto the options blocks stack" <<
      ", context: " << context <<
      std::endl;
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

S_ischemeOptionsBlock ischemeDriver::optionsBlocksStackTop () const
{
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    fOptionsBlocksStack.size (),
    "optionsBlocksStackTop(): fOptionsBlocksStack is empty");

  return fOptionsBlocksStack.front ();
}

void ischemeDriver::registerOptionInCurrentOptionsBlock (
  const S_oahOption& option,
  ischemeDriver& drv)
{
  const S_ischemeOptionsBlock&
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
      std::endl;
  }

  currentOptionsBlock->
    registerOptionsInOptionsBlock (
      option,
      drv);
}

void ischemeDriver::registerOptionsSuppliedChoicesAsUsed (
  const std::string& choiceName)
{
  if (fDisplayOptions) { // JMI
    gLogStream <<
      "====> Registering option-supplied choice [" <<
      choiceName <<
      "] as used" <<
      std::endl;
  }

  fUnusedOptionsSuppliedChoicesSet.erase (
    choiceName);
}

void ischemeDriver::registerOptionsSuppliedChoicesAsUnused (
  const std::string& choiceName)
{
  if (fDisplayOptions) { // JMI
    gLogStream <<
      "====> Registering option-supplied choice [" <<
      choiceName <<
      "] as used" <<
      std::endl;
  }

  fUnusedOptionsSuppliedChoicesSet.erase (
    choiceName);
}

void ischemeDriver::optionsBlocksStackPop (
  const std::string& context)
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
      std::endl;
  }

  fOptionsBlocksStack.pop_front ();

  if (fTraceOptionsBlocks) {
    ++gIndenter;
    displayOptionsBlocksStack (context);
    --gIndenter;
  }
}

void ischemeDriver::displayOptionsBlocksStack (
  const std::string& context) const
{
  gLogStream <<
    "OptionsBlocks stack, context: " <<
    context <<
    ":" ;

  if (fOptionsBlocksStack.size ()) {
    gLogStream << std::endl;

    ++gIndenter;

    for (S_ischemeOptionsBlock optionsBlock : fOptionsBlocksStack) {
      gLogStream << optionsBlock;
    } // for

    --gIndenter;
  }

  else {
    gLogStream << "[EMPTY]" << std::endl;
  }
}

void ischemeDriver::caseChoiceStatementsStackPush (
  const S_ischemeCaseChoiceStatement& caseChoiceStatement)
{
  if (fTraceCaseChoiceStatements) {
    gLogStream <<
      "====> Pushing [" <<
      caseChoiceStatement->asString () <<
      "] onto the case statements stack" <<
      std::endl;
  }

  ++fCaseChoiceStatementsNumber;

  fCaseChoiceStatementsStack.push_front (
    caseChoiceStatement);

  if (fTraceCaseChoiceStatements) {
    displayCaseChoiceStatementsStack (
      "after pushing a case statement");
  }
}

S_ischemeCaseChoiceStatement ischemeDriver::caseChoiceStatementsStackTop () const
{
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    fCaseChoiceStatementsStack.size (),
    "caseChoiceStatementsStackTop(): fCaseChoiceStatementsStack is empty");

  return fCaseChoiceStatementsStack.front ();
}

void ischemeDriver::caseChoiceStatementsStackPop ()
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
      std::endl;
  }

  fCaseChoiceStatementsStack.pop_front ();

  if (fTraceCaseChoiceStatements) {
    displayCaseChoiceStatementsStack (
      "after popping a case statement");
  }
}

void ischemeDriver::displayCaseChoiceStatementsStack (
  const std::string& context) const
{
  gLogStream <<
    "Case statements stack" <<
    ", context: " << context <<
    std::endl;

  if (fCaseChoiceStatementsStack.size ()) {
    ++gIndenter;

    for (S_ischemeCaseChoiceStatement caseChoiceStatement : fCaseChoiceStatementsStack) {
      gLogStream << caseChoiceStatement;
    } // for

    --gIndenter;
  }

  else {
    gLogStream << "[EMPTY]" << std::endl;
  }
}

void ischemeDriver::caseInputStatementsStackPush (
  const S_ischemeCaseInputStatement& caseInputStatement)
{
  if (fTraceCaseInputStatements) {
    gLogStream <<
      "====> Pushing [" <<
      caseInputStatement->asString () <<
      "] onto the case statements stack" <<
      std::endl;
  }

  ++fCaseInputStatementsNumber;

  fCaseInputStatementsStack.push_front (
    caseInputStatement);

  if (fTraceCaseInputStatements) {
    displayCaseInputStatementsStack (
      "after pushing a case statement");
  }
}

S_ischemeCaseInputStatement ischemeDriver::caseInputStatementsStackTop () const
{
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    fCaseInputStatementsStack.size (),
    "caseInputStatementsStackTop(): fCaseInputStatementsStack is empty");

  return fCaseInputStatementsStack.front ();
}

void ischemeDriver::caseInputStatementsStackPop ()
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
      std::endl;
  }

  fCaseInputStatementsStack.pop_front ();

  if (fTraceCaseInputStatements) {
    displayCaseInputStatementsStack (
      "after popping a case statement");
  }
}

void ischemeDriver::displayCaseInputStatementsStack (
  const std::string& context) const
{
  gLogStream <<
    "Case statements stack" <<
    ", context: " << context <<
    std::endl;

  if (fCaseInputStatementsStack.size ()) {
    ++gIndenter;

    for (S_ischemeCaseInputStatement caseInputStatement : fCaseInputStatementsStack) {
      gLogStream << caseInputStatement;
    } // for

    --gIndenter;
  }

  else {
    gLogStream << "[EMPTY]" << std::endl;
  }
}

int ischemeDriver::parseInput_Pass1 ()
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
  iscm::parser theParser (*this);

  theParser.set_debug_level (
    fTraceParsing);

  int parseResult = theParser ();

  // end scan
  scanEnd ();

  // print the basic results
  if (fTraceParsing) {
    gLogStream <<
      "--> parseResult:   " << parseResult <<
      std::endl;

    gLogStream <<
      "--> fTool:      " << fTool <<
      std::endl <<
      "--> fInputSoucesList: " <<
      std::endl;

    ++gIndenter;
    for (std::string inputSouce : fInputSoucesList ) {
      gLogStream <<
        inputSouce <<
        std::endl;
    } // for
    --gIndenter;
  }

  // print the options blocks stack if relevant
  if (gGlobalIschemeInterpreterOahGroup->getTraceOptionsBlocks ()) {
    gLogStream <<
      "====> fOptionsBlocksStack:" <<
      std::endl;

    ++gIndenter;

    for (S_ischemeOptionsBlock optionsBlock : fOptionsBlocksStack) {
      gLogStream <<
        optionsBlock <<
        std::endl;
    } // for

    --gIndenter;

    gLogStream;
  }

  // print the choices table if relevant
  if (
    gGlobalIschemeInterpreterOahGroup->getTraceChoices ()
      ||
    gGlobalIschemeInterpreterOahGroup->getNoLaunch ()
  ) {
    if (fChoicesTable->getChoicesMap ().size ()) {
      gLogStream <<
        "====> fChoicesTable:";

      if (fChoicesTable) {
        gLogStream << std::endl;

        ++gIndenter;

        gLogStream <<
          fChoicesTable <<
          std::endl;

        --gIndenter;
      }
      else {
        gLogStream << "[NONE]" << std::endl;
      }

      gLogStream << std::endl;
    }
  }

  // do the final semantics check
  finalSemanticsCheck ();

  return parseResult;
}

void ischemeDriver::handleSelectLabel (
  const std::string& choiceName,
  const std::string& label)
{
  if (fTraceChoices) {
    gLogStream <<
      "====> appendSelectLabelForToolLaunching()" <<
      ", choiceName: " << choiceName <<
      ", label: " << label <<
      std::endl;
  }

  // analyze this select command
  S_ischemeChoice
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
        std::endl;
      ++gIndenter;
      gLogStream <<
        choice;
      --gIndenter;
    }

    const std::set<std::string>&
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
      for (std::string choiceLabel : choiceLabelsSet) {
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
      std::stringstream s;

      s <<
        "label \"" << label <<
        "\" is no label of choice nor 'all' \"" <<
        choiceName <<
        "\", cannot be used in a 'select' statement";

      ischemeError (
        s.str (),
        fScannerLocation);
    }
  }

  else {
    std::stringstream s;

    s <<
      "choice name \"" << choiceName <<
      "\" is unknown in the choices table, cannot be used in a 'select' statement";

    ischemeError (
      s.str (),
      fScannerLocation);
  }
}

void ischemeDriver::appendSelectLabelForToolLaunching (
  const S_ischemeChoice choice,
  const std::string&      label,
  Bool               allLabelSelected)
{
  std::string
    choiceName =
      choice->getChoiceName ();

  if (fTraceChoices) {
    gLogStream <<
      "====> appendSelectLabelForToolLaunching()" <<
      ", choiceName: " << choiceName <<
      ", label: " << label <<
      std::endl;
  }

  S_ischemeOptionsBlock
    selectOptionsBlock =
      choice->
        getChoiceOptionsBlockForLabel (
          label,
          *this);

  // dont apply this select statement
  // if an option for the same choice label been used

  Bool overriddenMessageHasBeenIssued (false);

  if (
    gGlobalIschemeInterpreterOahGroup->
      getSelectChoiceToLabelsMultiMapAtom ()->
        getSetByAnOption ()
  ) {
    if (! overriddenMessageHasBeenIssued) {
      std::stringstream s;

      s <<
        "'select' label \"" <<
        label <<
        "\" for choice \"" <<
        choiceName <<
        "\" ignored, it is overridden by a '-select, -sel' option";

      ischemeWarning (
        s.str (),
        fScannerLocation);

      // issue above warning only one if the 'all' pseudo-label has be selected
      overriddenMessageHasBeenIssued = true;
    }
  }

  else {
    const S_ischemeOptionsBlock&
      selectOptionsBlock =
        choice->
          getChoiceOptionsBlockForLabel (
            label,
            *this);

    if (fTraceChoices) {
      gLogStream <<
        "====> optionsBlock from script:" <<
        std::endl;
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

mfMusicformatsErrorKind ischemeDriver::launchIschemeTool_Pass2 ()
{
  mfMusicformatsErrorKind
    result =
      mfMusicformatsErrorKind::kMusicformatsError_NONE;

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
      std::endl;
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
      std::endl;

    ++gIndenter;

    for (std::string command : fCommandsList) {
      gLogStream <<
        command <<
        std::endl;
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
      std::endl;
  }

  else {
    for (std::string command : fCommandsList) {
      if (fDisplayToolAndInput) {
        gLogStream <<
          "====> Running the tool with command: [" << command << ']' <<
          std::endl;
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
          std::endl;
      }

      if (commandExecutionResult) {
        result =
          mfMusicformatsErrorKind::kMusicformatsErrorInvalidFile;
      }

      // sleep for some milliseconds
          std::this_thread::sleep_for (std::chrono::milliseconds (100));
    } // for
  }

  return result;
}

Bool ischemeDriver::applySelectOptionsFinally ()
{
  Bool result;

  if (fTraceChoices) {
    gLogStream <<
      "====> Finally applying 'select' options" <<
      std::endl;
  }

  const std::multimap<std::string, std::string>&
    selectChoiceToLabelsMultiMap =
      gGlobalIschemeInterpreterOahGroup->
        getSelectChoiceToLabelsMultiMap ();

  if (fTraceChoices) {
    mfDisplayStringToStringMultiMap (
      "====> applySelectOptionsFinally(), selectChoiceToLabelsMultiMap",
      selectChoiceToLabelsMultiMap,
      gLogStream);
  }

  for (std::pair<std::string, std::string> thePair : selectChoiceToLabelsMultiMap) {
    std::string
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
        std::endl;
    }

    S_ischemeChoice
      choice =
        fChoicesTable->
          fetchChoiceByNameNonConst (
            optionSuppliedChoiceName,
            *this);

    if (optionSuppliedLabel == K_ALL_PSEUDO_LABEL_NAME) {
      const std::set<std::string>&
        choiceLabelsSet =
          choice->
            getLabelsSet ();

      if (fTraceChoices) {
        mfDisplayStringSet (
          "====> choiceLabelsSet",
          choiceLabelsSet,
          gLogStream);
      }

      for (std::string choiceLabel : choiceLabelsSet) {
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

Bool ischemeDriver::applySelectOption (
  const S_ischemeChoice choice,
  const std::string&      label)
{
  Bool result;

  const std::string
    choiceName =
      choice->getChoiceName ();

  if (fTraceChoices) {
    gLogStream <<
      "====> option-supplied label \"" <<
      label <<
      "\" for choice \"" <<
      choiceName <<
      "\" is being applied" <<
      std::endl;
  }

  const std::set<std::string>&
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
    S_ischemeChoice
      optionSuppliedChoice =
        fChoicesTable->
          fetchChoiceByNameNonConst (
            choiceName,
            *this);

    const S_ischemeOptionsBlock&
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
        std::endl;
      ++gIndenter;
      gLogStream <<
        selectOptionsBlock;
      --gIndenter;
    }
  }

  else {
    std::stringstream s;

    s <<
      "applySelectOption(): label \"" <<
      label <<
      "\" is unknown in choice \"" <<
      choiceName <<
      "\"";

    ischemeError (
      s.str (),
      fScannerLocation);
  }

  return result;
}

void ischemeDriver::finalSemanticsCheck ()
{
  // have all the options supplied choices been used?
  for (std::string choiceName : fUnusedOptionsSuppliedChoicesSet) {
    std::stringstream s;

    s <<
      "option-supplied choice \"" <<
      choiceName <<
      "\" has not been used in script \"" <<
      fScriptName <<
      "\"";

    ischemeWarning (
      s.str (),
      fScannerLocation);
  } // for

  // are there pending 'select' options?
  S_oahStringToStringMultiMapElementAtom
    selectChoiceToLabelsMultiMapAtom =
      gGlobalIschemeInterpreterOahGroup->
        getSelectChoiceToLabelsMultiMapAtom ();

  if (
    selectChoiceToLabelsMultiMapAtom->getSetByAnOption ()
  ) {
    applySelectOptionsFinally ();
  }
}

void ischemeDriver::populateTheCommandsList ()
{
  for (std::string inputSouce : fInputSoucesList ) {
    // the tool and input file source as std::string
    std::string
      toolAndInputAsString =
        fTool +
        ' ' +
        inputSouce;

    // the main options block options as std::string
    std::string
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

        for (S_ischemeOptionsBlock optionsBlock :fSelectedOptionsBlocksList ) {
          // the 'select' choice options block options as std::string
          std::string
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
        const std::map<std::string, S_ischemeChoice>&
          choicesMultiMap =
            fChoicesTable->
              getChoicesMap ();

        int
          choicesNumber =
            choicesMultiMap.size ();

        if (choicesNumber == 1) {
          // and use this single choice's default label

          // grab the single choice
          S_ischemeChoice
            singleChoice =
              (*( choicesMultiMap.begin ())).second;

          // get its default label
          std::string
            singleChoiceDefaultLabel =
              singleChoice->
                getChoiceDefaultLabel ();

          // get the options to be used
          const S_ischemeOptionsBlock&
            optionsBlockToBeUsed =
              singleChoice->
                getChoiceOptionsBlockForLabel (
                  singleChoiceDefaultLabel,
                  *this);

          // fetch the options to be used as a std::string
          std::string
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
          ischemeInternalError (
            "there can be only 1 choice if there is no 'select' statement",
            fScannerLocation);
        }
      }
    }
  } // for
}
