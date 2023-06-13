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

#include "mfslInterpreterOah.h"

#include "mfslDriver.h"
#include "mfslParser.h"

#include "mfslWae.h"


using namespace std;

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
      gLog << "Reading standard input" << endl;
    }
#endif // MF_TRACE_IS_ENABLED
  }

  else {
    // MFSL data comes from a file
#ifdef MF_TRACE_IS_ENABLED
    if (gEarlyOptions.getEarlyTraceOah ()) {
      gLog <<
        "Reading file \"" << fScriptName << "\"" <<
        endl;
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

  fDisplayServiceAndInput =
      gGlobalMfslInterpreterOahGroup->
        getDisplayServiceAndInput ().getValue ();

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

mfslDriver::~mfslDriver ()
{}

void mfslDriver::setService (std::string service)
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

void mfslDriver::appendInputSouce (std::string inputSouce)
{
  if (fDisplayServiceAndInput) {
    gLog <<
      "====> input: " << inputSouce <<
      endl;
  }

 fInputSoucesList.push_back (inputSouce);
}

void mfslDriver::optionsBlocksStackPush (
  S_mfslOptionsBlock optionsBlock,
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

void mfslDriver::registerOptionsSuppliedChoicesAsUsed (
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

void mfslDriver::registerOptionsSuppliedChoicesAsUnused (
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

void mfslDriver::optionsBlocksStackPop (
  const std::string& context)
{
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    fOptionsBlocksStack.size () != 0,
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

void mfslDriver::displayOptionsBlocksStack (
  const std::string& context) const
{
  gLog <<
    "OptionsBlocks stack, context: " <<
    context <<
    ":" ;

  if (fOptionsBlocksStack.size ()) {
    gLog << endl;

    ++gIndenter;

    for (S_mfslOptionsBlock optionsBlock : fOptionsBlocksStack) {
      gLog << optionsBlock;
    } // for

    --gIndenter;
  }

  else {
    gLog << "empty" << endl;
  }
}

void mfslDriver::caseChoiceStatementsStackPush (
  S_mfslCaseChoiceStatement caseChoiceStatement)
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

void mfslDriver::displayCaseChoiceStatementsStack (
  const std::string& context) const
{
  gLog <<
    "Case statements stack" <<
    ", context: " << context <<
    endl;

  if (fCaseChoiceStatementsStack.size ()) {
    ++gIndenter;

    for (S_mfslCaseChoiceStatement caseChoiceStatement : fCaseChoiceStatementsStack) {
      gLog << caseChoiceStatement;
    } // for

    --gIndenter;
  }

  else {
    gLog << "empty" << endl;
  }
}

void mfslDriver::caseInputStatementsStackPush (
  S_mfslCaseInputStatement caseInputStatement)
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

void mfslDriver::displayCaseInputStatementsStack (
  const std::string& context) const
{
  gLog <<
    "Case statements stack" <<
    ", context: " << context <<
    endl;

  if (fCaseInputStatementsStack.size ()) {
    ++gIndenter;

    for (S_mfslCaseInputStatement caseInputStatement : fCaseInputStatementsStack) {
      gLog << caseInputStatement;
    } // for

    --gIndenter;
  }

  else {
    gLog << "empty" << endl;
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
  if (gGlobalMfslInterpreterOahGroup->getTraceOptionsBlocks ()) {
    gLog <<
      "====> fOptionsBlocksStack:" <<
      endl;

    ++gIndenter;

    for (S_mfslOptionsBlock optionsBlock : fOptionsBlocksStack) {
      gLog <<
        optionsBlock <<
        endl;
    } // for

    --gIndenter;

    gLog << std::endl;
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

void mfslDriver::handleSelectLabel (
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
        "====> handleSelectLabel()" <<
        ", choice: " <<
        endl;
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

void mfslDriver::appendSelectLabelForServiceLaunching (
  const S_mfslChoice choice,
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

mfMusicformatsErrorKind mfslDriver::launchMfslService_Pass2 ()
{
  mfMusicformatsErrorKind
    result =
      mfMusicformatsErrorKind::kMusicformatsError_NONE;

  mfAssert (
    __FILE__, __LINE__,
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
    __FILE__, __LINE__,
    fCaseChoiceStatementsStack.size () == 0,
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

Bool mfslDriver::applySelectOptionsFinally ()
{
  Bool result;

  if (fTraceChoices) {
    gLog <<
      "====> Finally applying 'select' options" <<
      endl;
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
  for (std::string choiceName : fUnusedOptionsSuppliedChoicesSet) {
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
    selectChoiceToLabelsMultiMapAtom->getSelected ()
  ) {
    applySelectOptionsFinally ();
  }
}

void mfslDriver::populateTheCommandsList ()
{
  for (std::string inputSouce : fInputSoucesList ) {
    // the service and input file source as std::string
    std::string
      serviceAndInputAsString =
        fService +
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

        for (S_mfslOptionsBlock optionsBlock :fSelectedOptionsBlocksList ) {
          // the 'select' choice options block options as std::string
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
        const map<std::string, S_mfslChoice>&
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
          mfslInternalError (
            "there can be only 1 choice if there is no 'select' statement",
            fScannerLocation);
        }
      }
    }
  } // for
}
