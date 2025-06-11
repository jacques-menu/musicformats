/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

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
  for (int i = 10; i>0; --i) {
    std::cout << i << std::endl;
    std::this_thread::sleep_for (std::chrono::seconds(1));
  }
*/

#include "mfAssert.h"
#include "mfStringsHandling.h"
#include "mfIndentedTextOutput.h"
#include "mfSystemInterface.h"

#include "oahEarlyOptions.h"

#include "mnx2mnxsrInterpreterOah.h"

#include "mnx2mnxsrDriver.h"
#include "mnx2mnxsrParser.h"

#include "mnx2mnxsrWae.h"


using namespace std;

//_______________________________________________________________________________
// constants
const std::string mnx2mnxsrDriver::K_ALL_PSEUDO_LABEL_NAME = "all";

//______________________________________________________________________________
mnx2mnxsrDriver::mnx2mnxsrDriver ()
{
  // get the script source name
  fScriptName =
    gServiceRunData->
      getInputSourceName ();

  if (fScriptName == "-") {
    // MNX2MNXSR data comes from standard input
#ifdef MF_TRACE_IS_ENABLED
    if (gEarlyOptions.getEarlyTraceOah ()) {
      gLog << "Reading standard input" << endl;
    }
#endif // MF_TRACE_IS_ENABLED
  }

  else {
    // MNX2MNXSR data comes from a file
#ifdef MF_TRACE_IS_ENABLED
    if (gEarlyOptions.getEarlyTraceOah ()) {
      gLog <<
        "Reading file \"" << fScriptName << "\"" <<
        endl;
    }
#endif // MF_TRACE_IS_ENABLED
  }

  // get the options values as Bool,
  // since Bool is unknown to Flex and Bison-generated code
  fTraceScanning =
      gGlobalMnx2mnxsrInterpreterOahGroup->
        getTraceScanning ().getBareValue ();

  fDisplayTokens =
      gGlobalMnx2mnxsrInterpreterOahGroup->
        getDisplayTokens ().getBareValue ();

  fTraceParsing =
      gGlobalMnx2mnxsrInterpreterOahGroup->
        getTraceParsing ().getBareValue ();

  fDisplayServiceAndInput =
      gGlobalMnx2mnxsrInterpreterOahGroup->
        getDisplayServiceAndInput ().getBareValue ();

  fDisplayOptions =
      gGlobalMnx2mnxsrInterpreterOahGroup->
        getDisplayOptions ().getBareValue ();

  fTraceChoices =
      gGlobalMnx2mnxsrInterpreterOahGroup->
        getTraceChoices ().getBareValue ();
  fTraceCaseChoiceStatements =
      gGlobalMnx2mnxsrInterpreterOahGroup->
        getTraceCaseChoiceStatements ().getBareValue ();

  fTraceInputs =
      gGlobalMnx2mnxsrInterpreterOahGroup->
        getTraceInputs ().getBareValue ();
  fTraceCaseInputStatements =
      gGlobalMnx2mnxsrInterpreterOahGroup->
        getTraceCaseInputStatements ().getBareValue ();

  fNoLaunch =
      gGlobalMnx2mnxsrInterpreterOahGroup->
        getNoLaunch ().getBareValue ();

  fTraceOptionsBlocks =
      gGlobalMnx2mnxsrInterpreterOahGroup->
        getTraceOptionsBlocks ().getBareValue ();

  fNoLaunch =
      gGlobalMnx2mnxsrInterpreterOahGroup->
        getNoLaunch ().getBareValue ();

  // register the known MusicFormats services
  fKnownNames.insert ("xml2ly");
  fKnownNames.insert ("xml2brl");
  fKnownNames.insert ("xml2xml");
  fKnownNames.insert ("xml2gmn");
  fKnownNames.insert ("msdl");

  if (fDisplayServiceAndInput) {
    gLog <<
      "====> The known services names are: " <<
      mfStringSetAsString (fKnownNames) <<
      endl;
  }

  // create the driver's choices table
  fChoicesTable = mnx2mnxsrChoicesTable::create ();

  // get the choice labels supplied by options
  fOptionsSuppliedChoicesLabelsMultiMap =
    gGlobalMnx2mnxsrInterpreterOahGroup->
      getSelectChoiceToLabelsMultiMap ();

  if (fTraceChoices) {
    gLog <<
      "====> The choice labels set by options are: " <<
      mfStringToStringMultiMapAsString (
        fOptionsSuppliedChoicesLabelsMultiMap) <<
      endl;
  }

  // register all of them as unused
  for (pair<std::string, std::string> thePair : fOptionsSuppliedChoicesLabelsMultiMap) {
    std::string choiceName = thePair.first;

    registerOptionsSuppliedChoicesAsUnused (
      choiceName);
  } // for

  fCaseChoiceStatementsNumber = 0;
}

mnx2mnxsrDriver::~mnx2mnxsrDriver ()
{}

//_______________________________________________________________________________
void mnx2mnxsrDriver::setService (std::string service)
{
  if (fDisplayServiceAndInput) {
    gLog <<
      "====> service: " << service<<
      endl;
  }

  // is this service a known service name?
  if (! mfStringIsInStringSet (service, fKnownNames)) {
    gLog <<
      "### Unkown service name \"" << service << "\"" <<
      ", the known service names are " <<
      mfStringSetAsString (fKnownNames) <<
      endl;
  }

  fService = service;
}

//_______________________________________________________________________________
void mnx2mnxsrDriver::appendInputSouce (std::string inputSouce)
{
  if (fDisplayServiceAndInput) {
    gLog <<
      "====> input: " << inputSouce <<
      endl;
  }

 fInputSoucesList.push_back (inputSouce);
}

//_______________________________________________________________________________
void mnx2mnxsrDriver::optionsBlocksStackPush (
  S_mnx2mnxsrOptionsBlock optionsBlock,
  const std::string& context)
{
  if (fTraceOptionsBlocks) {
    gLog <<
      "====> Pushing [" <<
      optionsBlock->asString () <<
      "] onto the options blocks stack" <<
      ", context: " << context <<
      endl;
  }

  if (fOptionsBlocksStack.empty ()) {
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

S_mnx2mnxsrOptionsBlock mnx2mnxsrDriver::optionsBlocksStackTop () const
{
  // sanity check
  mfAssert (
    __FILE__, mfInputLineNumber (__LINE__),
    fOptionsBlocksStack.size (),
    "optionsBlocksStackTop(): fOptionsBlocksStack is empty");

  return fOptionsBlocksStack.front ();
}

void mnx2mnxsrDriver::optionsBlocksStackPop (
  const std::string& context)
{
  // sanity check
  mfAssert (
    __FILE__, mfInputLineNumber (__LINE__),
    ! fOptionsBlocksStack.empty (),
    "optionsBlocksStackPop(): fOptionsBlocksStack is empty");

  if (fTraceOptionsBlocks) {
    gLog <<
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

void mnx2mnxsrDriver::displayOptionsBlocksStack (
  const std::string& context) const
{
  gLog <<
    "OptionsBlocks stack, context: " <<
    context <<
    ":";

  if (fOptionsBlocksStack.size ()) {
    gLog << endl;

    ++gIndenter;

    for (S_mnx2mnxsrOptionsBlock optionsBlock : fOptionsBlocksStack) {
      gLog << optionsBlock;
    } // for

    --gIndenter;
  }

  else {
    gLog << "empty" << endl;
  }
}

//_______________________________________________________________________________
void mnx2mnxsrDriver::registerOptionInCurrentOptionsBlock (
  S_oahOption option,
  mnx2mnxsrDriver& drv)
{
  S_mnx2mnxsrOptionsBlock
    currentOptionsBlock =
      fOptionsBlocksStack.front ();

  // sanity check
  mfAssert (
    __FILE__, mfInputLineNumber (__LINE__),
    currentOptionsBlock != nullptr,
    "currentOptionsBlock is NULL");

  if (fDisplayOptions) { // JMI
    gLog <<
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

void mnx2mnxsrDriver::registerOptionsSuppliedChoicesAsUsed (
  const std::string& choiceName)
{
  if (fDisplayOptions) { // JMI
    gLog <<
      "====> Registering option-supplied choice [" <<
      choiceName <<
      "] as used" <<
      endl;
  }

  fUnusedOptionsSuppliedChoicesSet.erase (
    choiceName);
}

void mnx2mnxsrDriver::registerOptionsSuppliedChoicesAsUnused (
  const std::string& choiceName)
{
  if (fDisplayOptions) { // JMI
    gLog <<
      "====> Registering option-supplied choice [" <<
      choiceName <<
      "] as used" <<
      endl;
  }

  fUnusedOptionsSuppliedChoicesSet.erase (
    choiceName);
}

//_______________________________________________________________________________
void mnx2mnxsrDriver::caseChoiceStatementsStackPush (
  S_mnx2mnxsrCaseChoiceStatement caseChoiceStatement)
{
  if (fTraceCaseChoiceStatements) {
    gLog <<
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

S_mnx2mnxsrCaseChoiceStatement mnx2mnxsrDriver::caseChoiceStatementsStackTop () const
{
  // sanity check
  mfAssert (
    __FILE__, mfInputLineNumber (__LINE__),
    fCaseChoiceStatementsStack.size (),
    "caseChoiceStatementsStackTop(): fCaseChoiceStatementsStack is empty");

  return fCaseChoiceStatementsStack.front ();
}

void mnx2mnxsrDriver::caseChoiceStatementsStackPop ()
{
  // sanity check
  mfAssert (
    __FILE__, mfInputLineNumber (__LINE__),
    ! fCaseChoiceStatementsStack.empty (),
    "caseChoiceStatementsStackPop(): fCaseChoiceStatementsStack is empty");

  if (fTraceCaseChoiceStatements) {
    gLog <<
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

void mnx2mnxsrDriver::displayCaseChoiceStatementsStack (
  const std::string& context) const
{
  gLog <<
    "Case statements stack" <<
    ", context: " << context <<
    endl;

  if (fCaseChoiceStatementsStack.size ()) {
    ++gIndenter;

    for (S_mnx2mnxsrCaseChoiceStatement caseChoiceStatement : fCaseChoiceStatementsStack) {
      gLog << caseChoiceStatement;
    } // for

    --gIndenter;
  }

  else {
    gLog << "empty" << endl;
  }
}

void mnx2mnxsrDriver::caseInputStatementsStackPush (
  S_mnx2mnxsrCaseInputStatement caseInputStatement)
{
  if (fTraceCaseInputStatements) {
    gLog <<
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

S_mnx2mnxsrCaseInputStatement mnx2mnxsrDriver::caseInputStatementsStackTop () const
{
  // sanity check
  mfAssert (
    __FILE__, mfInputLineNumber (__LINE__),
    fCaseInputStatementsStack.size (),
    "caseInputStatementsStackTop(): fCaseInputStatementsStack is empty");

  return fCaseInputStatementsStack.front ();
}

void mnx2mnxsrDriver::caseInputStatementsStackPop ()
{
  // sanity check
  mfAssert (
    __FILE__, mfInputLineNumber (__LINE__),
    ! fCaseInputStatementsStack.empty (),
    "caseInputStatementsStackPop(): fCaseInputStatementsStack is empty");

  if (fTraceCaseInputStatements) {
    gLog <<
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

void mnx2mnxsrDriver::displayCaseInputStatementsStack (
  const std::string& context) const
{
  gLog <<
    "Case statements stack" <<
    ", context: " << context <<
    endl;

  if (fCaseInputStatementsStack.size ()) {
    ++gIndenter;

    for (S_mnx2mnxsrCaseInputStatement caseInputStatement : fCaseInputStatementsStack) {
      gLog << caseInputStatement;
    } // for

    --gIndenter;
  }

  else {
    gLog << "empty" << endl;
  }
}

//_______________________________________________________________________________
int mnx2mnxsrDriver::parseInput_Pass1 ()
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
  mnx2mnxsr::parser theParser (*this);

  theParser.set_debug_level (
    fTraceParsing);

  int parseResult = theParser ();

  // end scan
  scanEnd ();

  // print the basic results
  if (fTraceParsing) {
    gLog <<
      "--> parseResult:   " << parseResult <<
      endl;

    gLog <<
      "--> fService:      " << fService <<
      endl <<
      "--> fInputSoucesList: " <<
      endl;

    ++gIndenter;
    for (std::string inputSouce : fInputSoucesList ) {
      gLog <<
        inputSouce <<
        endl;
    } // for
    --gIndenter;
  }

  // print the options blocks stack if relevant
  if (gGlobalMnx2mnxsrInterpreterOahGroup->getTraceOptionsBlocks ()) {
    gLog <<
      "====> fOptionsBlocksStack:" <<
      endl;

    ++gIndenter;

    for (S_mnx2mnxsrOptionsBlock optionsBlock : fOptionsBlocksStack) {
      gLog <<
        optionsBlock <<
        endl;
    } // for

    --gIndenter;

    gLog << std::endl;
  }

  // print the choices table if relevant
  if (
    gGlobalMnx2mnxsrInterpreterOahGroup->getTraceChoices ()
      ||
    gGlobalMnx2mnxsrInterpreterOahGroup->getNoLaunch ()
  ) {
    if (fChoicesTable->getChoicesMap ().size ()) {
      gLog <<
        "====> fChoicesTable:";

      if (fChoicesTable) {
        gLog << endl;

        ++gIndenter;

        gLog <<
          fChoicesTable <<
          endl;

        --gIndenter;
      }
      else {
        gLog << "none" << endl;
      }

      gLog << endl;
    }
  }

  // do the final semantics check
  finalSemanticsCheck ();

  return parseResult;
}

//_______________________________________________________________________________
void mnx2mnxsrDriver::handleSelectLabel (
  const std::string& choiceName,
  const std::string& label)
{
  if (fTraceChoices) {
    gLog <<
      "====> handleSelectLabel()" <<
      ", choiceName: " << choiceName <<
      ", label: " << label <<
      endl;
  }

  // analyze this select command
  S_mnx2mnxsrChoice
    choice =
      fChoicesTable->
        fetchChoiceByNameNonConst (
          choiceName,
          *this);

  if (choice) {
    // register the options block to use for 'select' launching
    if (fTraceChoices) {
      gLog <<
        "====> handleSelectLabel()" <<
        ", choice: " <<
        endl;
      ++gIndenter;
      gLog <<
        choice;
      --gIndenter;
    }

    const std::set <std::string>&
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
        appendSelectLabelForServiceLaunching (
          choice,
          choiceLabel,
          true); // allLabelSelected
      }
    }

    else if (mfStringIsInStringSet (label, choiceLabelsSet)) {
      appendSelectLabelForServiceLaunching (
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

      mnx2mnxsrError (
        ss.str (),
        fScannerLocation);
    }
  }

  else {
    std::stringstream ss;

    ss <<
      "choice name \"" << choiceName <<
      "\" is unknown in the choices table, cannot be used in a 'select' statement";

    mnx2mnxsrError (
      ss.str (),
      fScannerLocation);
  }
}

void mnx2mnxsrDriver::appendSelectLabelForServiceLaunching (
  const S_mnx2mnxsrChoice choice,
  const std::string& label,
  Bool               allLabelSelected)
{
  std::string
    choiceName =
      choice->getChoiceName ();

  if (fTraceChoices) {
    gLog <<
      "====> appendSelectLabelForServiceLaunching()" <<
      ", choiceName: " << choiceName <<
      ", label: " << label <<
      endl;
  }

  S_mnx2mnxsrOptionsBlock
    selectOptionsBlock =
      choice->
        getChoiceOptionsBlockForLabel (
          label,
          *this);

  // dont apply this select statement
  // if an option for the same choice label been used

  Bool overriddenMessageHasBeenIssued (false);

  if (
    gGlobalMnx2mnxsrInterpreterOahGroup->
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

      mnx2mnxsrWarning (
        ss.str (),
        fScannerLocation);

      // issue above warning only one if the 'all' pseudo-label has be selected
      overriddenMessageHasBeenIssued = true;
    }
  }

  else {
    S_mnx2mnxsrOptionsBlock
      selectOptionsBlock =
        choice->
          getChoiceOptionsBlockForLabel (
            label,
            *this);

    if (fTraceChoices) {
      gLog <<
        "====> optionsBlock from script:" <<
        endl;
      ++gIndenter;
      gLog <<
        selectOptionsBlock;
      --gIndenter;
    }

    // this 'select' statement is to be applied
    fSelectedOptionsBlocksList.push_back (
      selectOptionsBlock);
  }
}

//_______________________________________________________________________________
mfMusicformatsErrorKind mnx2mnxsrDriver::launchMnx2mnxsrService_Pass2 ()
{
  mfMusicformatsErrorKind
    result =
      mfMusicformatsErrorKind::kMusicformatsError_NONE;

  mfAssert (
    __FILE__, mfInputLineNumber (__LINE__),
    fOptionsBlocksStack.size () == 1,
    "fOptionsBlocksStack should contain only the main options block after parsing");

  if (fDisplayServiceAndInput) {
    gLog <<
      "====> Launching " <<
      fService <<
      " with the argument and option gathered from " <<
      fScriptName <<
      endl;
  }

  // sanity checks
  mfAssert (
    __FILE__, mfInputLineNumber (__LINE__),
    fCaseChoiceStatementsStack.empty (),
    "fCaseChoiceStatementsStack should be empty after parsing");

  // populate the commands list with the options gathered in the script
  populateTheCommandsList ();

  // display the commands list
  if (fDisplayServiceAndInput) {
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
      endl;

    ++gIndenter;

    for (std::string command : fCommandsList) {
      gLog <<
        command <<
        endl;
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
      endl;
  }

  else {
    for (std::string command : fCommandsList) {
      if (fDisplayServiceAndInput) {
        gLog <<
          "====> Running the service with command: [" << command << ']' <<
          endl;
      }

      int
        commandExecutionResult =
          mfExecuteCommand (
            command,
            fDisplayServiceAndInput);

      if (fDisplayServiceAndInput) {
        gLog <<
          "====> The execution result is: " <<
          commandExecutionResult <<
          endl;
      }

      if (commandExecutionResult) {
        result =
          mfMusicformatsErrorKind::kMusicformatsErrorInvalidFile;
      }

      // sleep for some milliseconds
          this_thread::sleep_for (chrono::milliseconds (100));
    } // for
  }

  return result;
}

//_______________________________________________________________________________
Bool mnx2mnxsrDriver::applySelectOptionsFinally ()
{
  Bool result;

  if (fTraceChoices) {
    gLog <<
      "====> Finally applying 'select' options" <<
      endl;
  }

  const std::multimap <std::string, std::string>&
    selectChoiceToLabelsMultiMap =
      gGlobalMnx2mnxsrInterpreterOahGroup->
        getSelectChoiceToLabelsMultiMap ();

  if (fTraceChoices) {
    mfDisplayStringToStringMultiMap (
      "====> applySelectOptionsFinally(), selectChoiceToLabelsMultiMap",
      selectChoiceToLabelsMultiMap,
      gLog);
  }

  for (pair<std::string, std::string> thePair : selectChoiceToLabelsMultiMap) {
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
        endl;
    }

    S_mnx2mnxsrChoice
      choice =
        fChoicesTable->
          fetchChoiceByNameNonConst (
            optionSuppliedChoiceName,
            *this);

    if (optionSuppliedLabel == K_ALL_PSEUDO_LABEL_NAME) {
      const std::set <std::string>&
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

Bool mnx2mnxsrDriver::applySelectOption (
  const S_mnx2mnxsrChoice choice,
  const std::string& label)
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
      endl;
  }

  const std::set <std::string>&
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
    S_mnx2mnxsrChoice
      optionSuppliedChoice =
        fChoicesTable->
          fetchChoiceByNameNonConst (
            choiceName,
            *this);

    S_mnx2mnxsrOptionsBlock
      selectOptionsBlock =
        optionSuppliedChoice->
          getChoiceOptionsBlockForLabel (
            label,
            *this);

    fSelectedOptionsBlocksList.push_back (
      selectOptionsBlock);

    if (fTraceChoices) {
      gLog <<
        "====> selectOptionsBlock by an option:" <<
        endl;
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

    mnx2mnxsrError (
      ss.str (),
      fScannerLocation);
  }

  return result;
}

//_______________________________________________________________________________
void mnx2mnxsrDriver::finalSemanticsCheck ()
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

    mnx2mnxsrWarning (
      ss.str (),
      fScannerLocation);
  } // for

  // are there pending 'select' options?
  S_oahStringToStringMultiMapAtom
    selectChoiceToLabelsMultiMapAtom =
      gGlobalMnx2mnxsrInterpreterOahGroup->
        getSelectChoiceToLabelsMultiMapAtom ();

  if (
    selectChoiceToLabelsMultiMapAtom->getSelected ()
  ) {
    applySelectOptionsFinally ();
  }
}

void mnx2mnxsrDriver::populateTheCommandsList ()
{
  for (std::string inputSouce : fInputSoucesList ) {
    // the service and input file source as string
    std::string
      serviceAndInputAsString =
        fService +
        ' ' +
        inputSouce;

    // the main options block options as string
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
        serviceAndInputAsString
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

        for (S_mnx2mnxsrOptionsBlock optionsBlock :fSelectedOptionsBlocksList ) {
          // the 'select' choice options block options as string
          std::string
            selectChoiceOptionsAsString =
              optionsBlock->
                asOptionsString ();

          // append it to the commands list
          fCommandsList.push_back (
            serviceAndInputAsString
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
        const map<std::string, S_mnx2mnxsrChoice>&
          choicesMultiMap =
            fChoicesTable->
              getChoicesMap ();

        int
          choicesNumber =
            choicesMultiMap.size ();

        if (choicesNumber == 1) {
          // and use this single choice's default label

          // grab the single choice
          S_mnx2mnxsrChoice
            singleChoice =
              (*( choicesMultiMap.begin ())).second;

          // get its default label
          std::string
            singleChoiceDefaultLabel =
              singleChoice->
                getChoiceDefaultLabel ();

          // get the options to be used
          S_mnx2mnxsrOptionsBlock
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
            serviceAndInputAsString
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
          mnx2mnxsrInternalError (
            "there can be only 1 choice if there is no 'select' statement",
            fScannerLocation);
        }
      }
    }
  } // for
}
