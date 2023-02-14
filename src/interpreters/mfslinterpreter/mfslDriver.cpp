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

#include "mfStaticSettings.h"

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

#include "oahEarlyOptions.h"

#include "mfslInterpreterOah.h"

#include "mfslDriver.h"
#include "mfslParser.h"

#include "mfslWae.h"

#include "waeHandlers.h"


//_______________________________________________________________________________
// constants
const std::string mfslDriver::K_ALL_PSEUDO_LABEL_NAME  = "all";

//______________________________________________________________________________
mfslDriver::mfslDriver ()
{
  // get the script source name
  fScriptName =
    gServiceRunData->
      getInputSourceName ();

  if (fScriptName == "-") {
    // MFSL data comes from standard input
#ifdef MF_TRACE_IS_ENABLED
    if (gEarlyOptions.getEarlyTraceOah ()) {
      gLog << "Reading standard input" << std::endl;
    }
#endif // MF_TRACE_IS_ENABLED
  }

  else {
    // MFSL data comes from a file
#ifdef MF_TRACE_IS_ENABLED
    if (gEarlyOptions.getEarlyTraceOah ()) {
	  	std::stringstream ss;

      ss <<
        "Reading file \"" << fScriptName << "\"" <<
        std::endl;

      gWaeHandler->waeTrace (
        __FILE__, __LINE__,
        ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED
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
    gLog <<
      "====> The known tools names are: " <<
      mfStringSetAsString (fKnownNames) <<
      std::endl;
  }

  // create the driver's choices table
  fChoicesTable = mfslChoicesTable::create ();

  // get the choice labels supplied by options
  fOptionsSuppliedChoicesLabelsMultiMap =
    gGlobalMfslInterpreterOahGroup->
      getSelectChoiceToLabelsMultiMap ();

  if (fTraceChoices) {
    gLog <<
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

mfslDriver::~mfslDriver ()
{}

void mfslDriver::setTool (std::string tool)
{
  if (fDisplayToolAndInput) {
    gLog <<
      "====> tool: " << tool<<
      std::endl;
  }

  // is this tool a known tool name?
  if (! mfStringIsInStringSet (tool, fKnownNames)) {
    gLog <<
      "### Unkown tool name \"" << tool << "\"" <<
      ", the known tool names are " <<
      mfStringSetAsString (fKnownNames) <<
      std::endl;
  }

  fTool = tool;
}

void mfslDriver::appendInputSouce (std::string inputSouce)
{
  if (fDisplayToolAndInput) {
    gLog <<
      "====> input: " << inputSouce <<
      std::endl;
  }

 fInputSoucesList.push_back (inputSouce);
}

void mfslDriver::optionsBlocksStackPush (
  const S_mfslOptionsBlock& optionsBlock,
  const std::string&      context)
{
  if (fTraceOptionsBlocks) {
    gLog <<
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

S_mfslOptionsBlock mfslDriver::optionsBlocksStackTop () const
{
#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    fOptionsBlocksStack.size (),
    "optionsBlocksStackTop(): fOptionsBlocksStack is empty");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  return fOptionsBlocksStack.front ();
}

void mfslDriver::registerOptionInCurrentOptionsBlock (
  const S_oahOption& option,
  mfslDriver& drv)
{
  S_mfslOptionsBlock
    currentOptionsBlock =
      fOptionsBlocksStack.front ();

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    currentOptionsBlock != nullptr,
    "currentOptionsBlock is null");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  if (fDisplayOptions) { // JMI
    gLog <<
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

void mfslDriver::registerOptionsSuppliedChoicesAsUsed (
  const std::string& choiceName)
{
  if (fDisplayOptions) { // JMI
    gLog <<
      "====> Registering option-supplied choice [" <<
      choiceName <<
      "] as used" <<
      std::endl;
  }

  fUnusedOptionsSuppliedChoicesSet.erase (
    choiceName);
}

void mfslDriver::registerOptionsSuppliedChoicesAsUnused (
  const std::string& choiceName)
{
  if (fDisplayOptions) { // JMI
    gLog <<
      "====> Registering option-supplied choice [" <<
      choiceName <<
      "] as used" <<
      std::endl;
  }

  fUnusedOptionsSuppliedChoicesSet.erase (
    choiceName);
}

void mfslDriver::optionsBlocksStackPop (
  const std::string& context)
{
#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    fOptionsBlocksStack.size () != 0,
    "optionsBlocksStackPop(): fOptionsBlocksStack is empty");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  if (fTraceOptionsBlocks) {
    gLog <<
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

void mfslDriver::displayOptionsBlocksStack (
  const std::string& context) const
{
  gLog <<
    "OptionsBlocks stack, context: " <<
    context <<
    ":" ;

  if (fOptionsBlocksStack.size ()) {
    gLog << std::endl;

    ++gIndenter;

    for (S_mfslOptionsBlock optionsBlock : fOptionsBlocksStack) {
      gLog << optionsBlock;
    } // for

    --gIndenter;
  }

  else {
    gLog << "[EMPTY]" << std::endl;
  }
}

void mfslDriver::caseChoiceStatementsStackPush (
  const S_mfslCaseChoiceStatement& caseChoiceStatement)
{
  if (fTraceCaseChoiceStatements) {
    gLog <<
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

S_mfslCaseChoiceStatement mfslDriver::caseChoiceStatementsStackTop () const
{
#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    fCaseChoiceStatementsStack.size (),
    "caseChoiceStatementsStackTop(): fCaseChoiceStatementsStack is empty");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  return fCaseChoiceStatementsStack.front ();
}

void mfslDriver::caseChoiceStatementsStackPop ()
{
#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    fCaseChoiceStatementsStack.size () != 0,
    "caseChoiceStatementsStackPop(): fCaseChoiceStatementsStack is empty");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  if (fTraceCaseChoiceStatements) {
    gLog <<
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

void mfslDriver::displayCaseChoiceStatementsStack (
  const std::string& context) const
{
  gLog <<
    "Case statements stack" <<
    ", context: " << context <<
    std::endl;

  if (fCaseChoiceStatementsStack.size ()) {
    ++gIndenter;

    for (S_mfslCaseChoiceStatement caseChoiceStatement : fCaseChoiceStatementsStack) {
      gLog << caseChoiceStatement;
    } // for

    --gIndenter;
  }

  else {
    gLog << "[EMPTY]" << std::endl;
  }
}

void mfslDriver::caseInputStatementsStackPush (
  const S_mfslCaseInputStatement& caseInputStatement)
{
  if (fTraceCaseInputStatements) {
    gLog <<
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

S_mfslCaseInputStatement mfslDriver::caseInputStatementsStackTop () const
{
#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    fCaseInputStatementsStack.size (),
    "caseInputStatementsStackTop(): fCaseInputStatementsStack is empty");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  return fCaseInputStatementsStack.front ();
}

void mfslDriver::caseInputStatementsStackPop ()
{
#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    fCaseInputStatementsStack.size () != 0,
    "caseInputStatementsStackPop(): fCaseInputStatementsStack is empty");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  if (fTraceCaseInputStatements) {
    gLog <<
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

void mfslDriver::displayCaseInputStatementsStack (
  const std::string& context) const
{
  gLog <<
    "Case statements stack" <<
    ", context: " << context <<
    std::endl;

  if (fCaseInputStatementsStack.size ()) {
    ++gIndenter;

    for (S_mfslCaseInputStatement caseInputStatement : fCaseInputStatementsStack) {
      gLog << caseInputStatement;
    } // for

    --gIndenter;
  }

  else {
    gLog << "[EMPTY]" << std::endl;
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
    gLog <<
      "--> parseResult:   " << parseResult <<
      std::endl;

    gLog <<
      "--> fTool:      " << fTool <<
      std::endl <<
      "--> fInputSoucesList: " <<
      std::endl;

    ++gIndenter;
    for (std::string inputSouce : fInputSoucesList ) {
      gLog <<
        inputSouce <<
        std::endl;
    } // for
    --gIndenter;
  }

  // print the options blocks stack if relevant
  if (gGlobalMfslInterpreterOahGroup->getTraceOptionsBlocks ()) {
    gLog <<
      "====> fOptionsBlocksStack:" <<
      std::endl;

    ++gIndenter;

    for (S_mfslOptionsBlock optionsBlock : fOptionsBlocksStack) {
      gLog <<
        optionsBlock <<
        std::endl;
    } // for

    --gIndenter;

    gLog;
  }

  // print the choices table if relevant
  if (
    gGlobalMfslInterpreterOahGroup->getTraceChoices ()
      ||
    gGlobalMfslInterpreterOahGroup->getNoLaunch ()
  ) {
    if (fChoicesTable->getChoicesMap ().size ()) {
      gLog <<
        "====> fChoicesTable:";

      if (fChoicesTable) {
        gLog << std::endl;

        ++gIndenter;

        gLog <<
          fChoicesTable <<
          std::endl;

        --gIndenter;
      }
      else {
        gLog << "[NONE]" << std::endl;
      }

      gLog << std::endl;
    }
  }

  // do the final semantics check
  finalSemanticsCheck ();

  return parseResult;
}

void mfslDriver::handleSelectLabel (
  const std::string& choiceName,
  const std::string& label)
{
  if (fTraceChoices) {
    gLog <<
      "====> appendSelectLabelForToolLaunching()" <<
      ", choiceName: " << choiceName <<
      ", label: " << label <<
      std::endl;
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
      gLog <<
        "====> appendSelectLabelForToolLaunching()" <<
        ", choice: " <<
        std::endl;
      ++gIndenter;
      gLog <<
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
        gLog);
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
      std::stringstream ss;

      ss <<
        "label \"" << label <<
        "\" is no label of choice nor 'all' \"" <<
        choiceName <<
        "\", cannot be used in a 'select' statement";

      mfslError (
        ss.str (),
        fScannerLocation);
    }
  }

  else {
    std::stringstream ss;

    ss <<
      "choice name \"" << choiceName <<
      "\" is unknown in the choices table, cannot be used in a 'select' statement";

    mfslError (
      ss.str (),
      fScannerLocation);
  }
}

void mfslDriver::appendSelectLabelForToolLaunching (
  const S_mfslChoice choice,
  const std::string&      label,
  Bool               allLabelSelected)
{
  std::string
    choiceName =
      choice->getChoiceName ();

  if (fTraceChoices) {
    gLog <<
      "====> appendSelectLabelForToolLaunching()" <<
      ", choiceName: " << choiceName <<
      ", label: " << label <<
      std::endl;
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
        getSelected ()
  ) {
    if (! overriddenMessageHasBeenIssued) {
      std::stringstream ss;

      ss <<
        "'select' label \"" <<
        label <<
        "\" for choice \"" <<
        choiceName <<
        "\" ignored, it is overridden by a '-select, -sel' option";

      mfslWarning (
        ss.str (),
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
      gLog <<
        "====> optionsBlock from script:" <<
        std::endl;
      ++gIndenter;
      gLog <<
        selectOptionsBlock;
      --gIndenter;
    }

    // this 'select' statement is to be applied
    fSelectedsBlocksList.push_back (
      selectOptionsBlock);
  }
}

mfMusicformatsErrorKind mfslDriver::launchMfslTool_Pass2 ()
{
  mfMusicformatsErrorKind
    result =
      mfMusicformatsErrorKind::kMusicformatsError_NONE;

  mfAssert (
    __FILE__, __LINE__,
    fOptionsBlocksStack.size () == 1,
    "fOptionsBlocksStack should contain only the main options block after parsing");

  if (fDisplayToolAndInput) {
    gLog <<
      "====> Launching " <<
      fTool <<
      " with the argument and option gathered from " <<
      fScriptName <<
      std::endl;
  }

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    fCaseChoiceStatementsStack.size () == 0,
    "fCaseChoiceStatementsStack should be empty after parsing");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  // populate the commands list with the options gathered in the script
  populateTheCommandsList ();

  // display the commands list
  if (fDisplayToolAndInput) {
    gLog <<
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
      gLog <<
        command <<
        std::endl;
    } // for

    --gIndenter;
  }

  // execute the commands in the list
  if (fNoLaunch) {
    gLog <<
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
        gLog <<
          "====> Running the tool with command: [" << command << ']' <<
          std::endl;
      }

      int
        commandExecutionResult =
          mfExecuteCommand (
            command,
            fDisplayToolAndInput);

      if (fDisplayToolAndInput) {
        gLog <<
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

Bool mfslDriver::applySelectOptionsFinally ()
{
  Bool result;

  if (fTraceChoices) {
    gLog <<
      "====> Finally applying 'select' options" <<
      std::endl;
  }

  const std::multimap<std::string, std::string>&
    selectChoiceToLabelsMultiMap =
      gGlobalMfslInterpreterOahGroup->
        getSelectChoiceToLabelsMultiMap ();

  if (fTraceChoices) {
    mfDisplayStringToStringMultiMap (
      "====> applySelectOptionsFinally(), selectChoiceToLabelsMultiMap",
      selectChoiceToLabelsMultiMap,
      gLog);
  }

  for (std::pair<std::string, std::string> thePair : selectChoiceToLabelsMultiMap) {
    std::string
      optionSuppliedChoiceName =
        thePair.first,
      optionSuppliedLabel =
        thePair.second;

    if (fTraceChoices) {
      gLog <<
        "====> applySelectOptionsFinally():" <<
        "optionSuppliedChoiceName\"" <<
        optionSuppliedChoiceName <<
        "\"" <<
        ", optionSuppliedLabel\"" <<
        optionSuppliedLabel <<
        "\"" <<
        std::endl;
    }

    S_mfslChoice
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
          gLog);
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

Bool mfslDriver::applySelectOption (
  const S_mfslChoice choice,
  const std::string&      label)
{
  Bool result;

  const std::string
    choiceName =
      choice->getChoiceName ();

  if (fTraceChoices) {
    gLog <<
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
      gLog);
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

    fSelectedsBlocksList.push_back (
      selectOptionsBlock);

    if (fTraceChoices) {
      gLog <<
        "====> selectOptionsBlock by an option:" <<
        std::endl;
      ++gIndenter;
      gLog <<
        selectOptionsBlock;
      --gIndenter;
    }
  }

  else {
    std::stringstream ss;

    ss <<
      "applySelectOption(): label \"" <<
      label <<
      "\" is unknown in choice \"" <<
      choiceName <<
      "\"";

    mfslError (
      ss.str (),
      fScannerLocation);
  }

  return result;
}

void mfslDriver::finalSemanticsCheck ()
{
  // have all the options supplied choices been used?
  for (std::string choiceName : fUnusedOptionsSuppliedChoicesSet) {
    std::stringstream ss;

    ss <<
      "option-supplied choice \"" <<
      choiceName <<
      "\" has not been used in script \"" <<
      fScriptName <<
      "\"";

    mfslWarning (
      ss.str (),
      fScannerLocation);
  } // for

  // are there pending 'select' options?
  S_oahStringToStringMultiMapElementAtom
    selectChoiceToLabelsMultiMapAtom =
      gGlobalMfslInterpreterOahGroup->
        getSelectChoiceToLabelsMultiMapAtom ();

  if (
    selectChoiceToLabelsMultiMapAtom->getSelected ()
  ) {
    applySelectOptionsFinally ();
  }
}

void mfslDriver::populateTheCommandsList ()
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

      if (fSelectedsBlocksList.size ()) {
        // 'select' statement have been supplied,
        // either in the script or by an option

        for (S_mfslOptionsBlock optionsBlock :fSelectedsBlocksList ) {
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
        const std::map<std::string, S_mfslChoice>&
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
          std::string
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
          mfslInternalError (
            "there can be only 1 choice if there is no 'select' statement",
            fScannerLocation);
        }
      }
    }
  } // for
}
