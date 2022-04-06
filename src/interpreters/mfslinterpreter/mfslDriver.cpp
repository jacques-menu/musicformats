/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <sys/wait.h> // wait()
#include <unistd.h>   // sleep()

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

  fCaseStatementsNumber = 0;
}

void mfslDriver::setToolName (string toolName)
{
  if (fDisplayToolAndInput) {
    gLogStream <<
      "====> tool: " << toolName<<
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

void mfslDriver::setInputSouceName (string inputSouceName)
{
  if (fDisplayToolAndInput) {
    gLogStream <<
      "====> input: " << inputSouceName <<
      endl;
  }

 fInputSouceName = inputSouceName;
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

  ++fCaseStatementsNumber;

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

int mfslDriver::parseInput_Pass1 ()
{
  // initialize scanner location
  fScannerLocation.initialize (
    &fScriptSourceName);

  // begin scan
  scanBegin ();

  if (fScriptSourceName.empty () || fScriptSourceName == "-") {
    fScriptSourceName = "stdin"; // nicer for warning and error messages
  }

  // do the parsing
  yy::parser theParser (*this);

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
      "--> fToolName:      " << fToolName <<
      endl <<
      "--> fInputSouceName: " << fInputSouceName <<
      endl;
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

void mfslDriver::appendSelectLabelForToolLaunching (
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

    if (mfStringIsInStringSet (label, choiceLabelsSet)) {
      S_mfslOptionsBlock
        selectOptionsBlock =
          choice->
            getChoiceOptionsBlockForLabel (
              label,
              *this);

//         if (
//           false &&  // JMI
//           applySelectOptionIfPresent (
//             choice,
//             label)
//         ) {
//           // JMI
//         }

      Bool thisSelectStatmentHasToBeApplied (true);

      // has an option for the same choice label been used?
      if (
        gGlobalMfslInterpreterOahGroup->
          getSelectChoiceToLabelsMultiMapAtom ()->
            getSetByAnOption ()
      ) {
        // get the select choice to label multimap
        const multimap<string, string>&
          selectChoiceToLabelsMultiMap =
            gGlobalMfslInterpreterOahGroup->
              getSelectChoiceToLabelsMultiMap ();

        string labelInOption;

        if (
          mfKeyValuePairIsInStringToStringMultiMap (
            choiceName,
            selectChoiceToLabelsMultiMap,
            labelInOption)
        ) {
          thisSelectStatmentHasToBeApplied =
            labelInOption != label;
        }
      }

      if (thisSelectStatmentHasToBeApplied) {
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

      else {
        stringstream s;

        s <<
          "label \"" << label <<
          "\" of choice \"" <<
          choiceName <<
          "\" ignored in a 'select' statement, overridden by an option";

        mfslWarning (
          s.str (),
          fScannerLocation);
      }
    }

    else {
      stringstream s;

      s <<
        "label \"" << label <<
        "\" is no label of choice \"" <<
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

void mfslDriver::setEveryChoiceForToolLaunching (
  const string& choiceName)
{
  if (fTraceChoices) {
    gLogStream <<
      "====> setEveryChoiceForToolLaunching()" <<
      ", choiceName: " << choiceName <<
      endl;
  }

  // fetch the choice
  S_mfslChoice
    choice =
      fChoicesTable->
        fetchChoiceByNameNonConst (
          choiceName,
          *this);

  if (choice) {
    // is this choice name already selected by an option?
    S_oahStringAtom
      everyChoiceAtom =
        gGlobalMfslInterpreterOahGroup->
          getEveryChoiceAtom ();

    gLogStream <<
      "----------> everyChoiceAtom: " <<
      endl <<
      everyChoiceAtom <<
      endl;

    if (applyEveryOptionIfPresent (everyChoiceAtom)) {
    }

    else {
      // no, this 'every' statement is to be applied
      // register the choice to use for 'every' launching
      fChoiceToUseForEveryLaunching =
        choice;

      if (fTraceChoices) {
        gLogStream <<
          "====> setEveryChoiceForToolLaunching()" <<
          ", choice: " <<
          endl;
        ++gIndenter;
        gLogStream <<
          fChoiceToUseForEveryLaunching;
        --gIndenter;
      }

      if (fTraceChoices) {
        gLogStream <<
          "====> fChoiceToUseForEveryLaunching from script:" <<
          endl;
        ++gIndenter;
        gLogStream <<
          fChoiceToUseForEveryLaunching;
        --gIndenter;
      }
    }
  }

//   else {
//     stringstream s;
//
//     s <<
//       "name \"" << choiceName <<
//       "\" is no choice name, cannot be used in an 'every' statement";
//
//     mfslError (
//       s.str (),
//       fScannerLocation);
//   }
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
      fToolName <<
      " with the argument and option gathered from " <<
      fScriptSourceName <<
      endl;
  }

  // sanity checks
  mfAssert (
    __FILE__, __LINE__,
    fCaseStatementsStack.size () == 0,
    "fCaseStatementsStack should be empty after parsing");

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

//       // wait for child termination
//       int waitStatus;
//
//       pid_t
//         pid = wait (&waitStatus);

      // sleep for 1 second
      unsigned int
        sleepResult = sleep (1);
    } // for
  }

	return result;
}

// Bool mfslDriver::applySelectOptionIfPresent (
//   const S_mfslChoice choice,
//   const string&      label)
// {
//   Bool result;
//
//   if (fTraceChoices) {
//     gLogStream <<
//       "====> applySelectOptionIfPresent()" <<
//       ", label: " << label <<
//       ", choice:" <<
//       endl;
//
//     ++gIndenter;
//     gLogStream <<
//       choice;
//     --gIndenter;
//   }
//
//   // is this label already selected by an option?
//   S_oahStringToStringMultiMapElementAtom
//     selectChoiceToLabelsMultiMapAtom =
//       gGlobalMfslInterpreterOahGroup->
//         getSelectChoiceToLabelsMultiMapAtom ();
//
// //   gLogStream << JMI
// //     "----------> selectChoiceToLabelsMultiMapAtom: " <<
// //     endl <<
// //     selectChoiceToLabelsMultiMapAtom <<
// //     endl;
//
//   if (selectChoiceToLabelsMultiMapAtom->getSetByAnOption ()) {
//     // yes, this 'select' statement is superseeded by the option
//     const multimap<string, string>&
//       selectChoiceToLabelsMultiMap =
//         gGlobalMfslInterpreterOahGroup->
//           getSelectChoiceToLabelsMultiMap ();
//
//     if (fTraceChoices) {
//       mfDisplayStringToStringMultiMap (
//         "====> select option, selectChoiceToLabelsMultiMap",
//         selectChoiceToLabelsMultiMap,
//         gLogStream);
//     }
//
//     string
//       choiceName =
//         choice->getChoiceName ();
//
//     Bool
//       optionSuppliedLabelIsKnwonToTheChoice =
//         mfKeyValuePairIsInStringToStringMultiMap (
//           choiceName,
//           selectChoiceToLabelsMultiMap,
//           label);
//
//     if (optionSuppliedLabelIsKnwonToTheChoice) {
//       if (fTraceChoices) {
//         gLogStream <<
//           "====> label \"" <<
//           label <<
//           "\" for choice:" <<
//           endl;
//         ++gIndenter;
//         gLogStream <<
//           choice;
//         --gIndenter;
//
//         gLogStream <<
//           "is superseeded by \"" <<
//           label <<
//           "\" supplied by an option" <<
//           endl;
//       }
//
//       S_mfslOptionsBlock
//         selectedOptionsBlock =
//           choice->
//             getChoiceOptionsBlockForLabel (
//               label,
//               *this);
//
//       fSelectedOptionsBlocksList.push_back (
//         selectedOptionsBlock);
//
//       if (fTraceChoices) {
//         gLogStream <<
//           "====> selectedOptionsBlock by an option:" <<
//           endl;
//         ++gIndenter;
//         gLogStream <<
//           selectedOptionsBlock;
//         --gIndenter;
//       }
//     }
//
//     else {
//       stringstream s;
//
//       s <<
//         "label \"" << label <<
//         "\" is unknown to choice \"" <<
//         choiceName <<
//         "\"";
//
//       mfslError (
//         s.str (),
//         fScannerLocation);
//     }
//   }
//
//   return result;
// }

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
        "====> final select, selectChoiceToLabelsMultiMap",
        selectChoiceToLabelsMultiMap,
        gLogStream);
    }

//   if (selectChoiceToLabelsMultiMap.size () > 1) {
//     stringstream s;
//
//     s <<
//       "there are several 'select' options, only 1 can be used";
//
//     mfslInternalError (
//       s.str (),
//       fScannerLocation);
//   }

//   multimap<string, string>::const_iterator
//     it = selectChoiceToLabelsMultiMap.begin ();
  for (pair<string, string> thePair : selectChoiceToLabelsMultiMap) {
    string
      optionSuppliedChoiceName =
        thePair.first,
      optionSuppliedLabel =
        thePair.second;

      if (fTraceChoices) {
        gLogStream <<
          "====> optionSuppliedChoiceNamec, 'select', finally: \"" <<
          optionSuppliedChoiceName <<
          "\"" <<
          endl;
      }

    Bool
      optionSuppliedLabelIsKnwonToTheChoice =
        mfKeyValuePairIsInStringToStringMultiMap (
          optionSuppliedChoiceName,
          selectChoiceToLabelsMultiMap,
          optionSuppliedLabel);

    if (optionSuppliedLabelIsKnwonToTheChoice) {
      if (fTraceChoices) {
        gLogStream <<
          "====> option-supplied label \"" <<
          optionSuppliedLabel <<
          "\" for choice \"" <<
          optionSuppliedChoiceName <<
          "\" is being applied" <<
          endl;
      }

      S_mfslChoice
        optionSuppliedChoice =
          fChoicesTable->
            fetchChoiceByNameNonConst (
              optionSuppliedChoiceName,
              *this);

      S_mfslOptionsBlock
        selectOptionsBlock =
          optionSuppliedChoice->
            getChoiceOptionsBlockForLabel (
              optionSuppliedLabel,
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
        "label \"" <<
        optionSuppliedChoiceName <<
        "\" is unknown in choice \"" <<
        optionSuppliedChoiceName <<
        "\", 'select', finally";

      mfslError (
        s.str (),
        fScannerLocation);
    }
  } // for

  return result;
}

Bool mfslDriver::applyEveryOptionIfPresent (
  const S_oahStringAtom everyChoiceAtom)
{
  Bool result;

  if (everyChoiceAtom->getSetByAnOption ()) {
    // yes, this 'every' statement is superseeded by the option

    // get the option-supplied choice name
    string
      optionSuppliedChoiceName =
        gGlobalMfslInterpreterOahGroup->
          getEveryChoice ();

    if (fTraceChoices) {
      gLogStream <<
        "=====> optionSuppliedChoiceName, 'every' if present: " <<
        endl;
      ++gIndenter;
      gLogStream <<
        optionSuppliedChoiceName;
      --gIndenter;

      gLogStream <<
        "is superseeded by \"" <<
        optionSuppliedChoiceName <<
        "\" supplied by an option" <<
        endl;
    }

    // fetch the choice supplied by this option
    S_mfslChoice
      choiceSuppliedByAnOption =
        fChoicesTable->
          fetchChoiceByNameNonConst (
            optionSuppliedChoiceName,
            *this);

    if (choiceSuppliedByAnOption) { // JMI
      fChoiceToUseForEveryLaunching =
        choiceSuppliedByAnOption;

      if (fTraceChoices) {
        gLogStream <<
          "====> fChoiceToUseForEveryLaunching supplied by an option:" <<
          endl;
        ++gIndenter;
        gLogStream <<
          fChoiceToUseForEveryLaunching;
        --gIndenter;
      }
    }

    else {
      stringstream s;

      s <<
        "choice \"" << optionSuppliedChoiceName <<
        "\" is unknown in choices table";

      mfslError (
        s.str (),
        fScannerLocation);
    }
  }

  return result;
}

Bool mfslDriver::applyEveryOptionFinally ()
{
  Bool result;

  // get the option-supplied choice name
  string
    optionSuppliedChoiceName =
      gGlobalMfslInterpreterOahGroup->
        getEveryChoice ();

  if (fTraceChoices) {
    gLogStream <<
      "=====> optionSuppliedChoiceName, 'every, finally: " <<
      endl;
    ++gIndenter;
    gLogStream <<
      optionSuppliedChoiceName;
    --gIndenter;

    gLogStream <<
      "is superseeded by \"" <<
      optionSuppliedChoiceName <<
      "\" supplied by an option" <<
      endl;
  }

  // fetch the choice supplied by this option
  S_mfslChoice
    choiceSuppliedByAnOption =
      fChoicesTable->
        fetchChoiceByNameNonConst (
          optionSuppliedChoiceName,
          *this);

  if (choiceSuppliedByAnOption) { // JMI
    fChoiceToUseForEveryLaunching =
      choiceSuppliedByAnOption;

    if (fTraceChoices) {
      gLogStream <<
        "====> fChoiceToUseForEveryLaunching supplied by an option:" <<
        endl;
      ++gIndenter;
      gLogStream <<
        fChoiceToUseForEveryLaunching;
      --gIndenter;
    }
  }

  else {
    stringstream s;

    s <<
      "choice \"" << optionSuppliedChoiceName <<
      "\" is unknown in choices table";

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
      fScriptSourceName <<
      "\"";

    mfslWarning (
      s.str (),
      fScannerLocation);
  } // for

  // is there a pending 'select' option?
  S_oahStringToStringMultiMapElementAtom
    selectChoiceToLabelsMultiMapAtom =
      gGlobalMfslInterpreterOahGroup->
        getSelectChoiceToLabelsMultiMapAtom ();

  if (
    selectChoiceToLabelsMultiMapAtom->getSetByAnOption ()
//       &&
//     ! fChoiceToUseForEveryLaunching // JMI
  ) {
    applySelectOptionsFinally ();
  }

  // is there a pending 'every' option?
  S_oahStringAtom
    everyChoiceAtom =
      gGlobalMfslInterpreterOahGroup->
        getEveryChoiceAtom ();

  if (
    everyChoiceAtom->getSetByAnOption ()
//       &&
//     ! fOptionsBlockToUseForSelectLaunching // JMI
  ) {
    applyEveryOptionFinally ();
  }
}

void mfslDriver::populateTheCommandsList ()
{
  // the tool and input file source as string
  string
    toolAndInputAsString =
      fToolName +
      ' ' +
      fInputSouceName;

  // the main options block options as string
  string
    mainOptionsAsString;

  if (fMainOptionsBlock) {
    mainOptionsAsString =
      fMainOptionsBlock->
        asOptionsString ();
  }

  // what has been found in the script?
  if (fCaseStatementsNumber == 0) {
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

    if (fChoiceToUseForEveryLaunching) {
      // an 'every' statement has been supplied,
      // either in the script or by an option

      const set<string>&
        choiceLabelsSet =
          fChoiceToUseForEveryLaunching->
            getLabelsSet ();

      for (string label : choiceLabelsSet) {
        // fetch the options block
        S_mfslOptionsBlock
          optionsBlock =
            fChoiceToUseForEveryLaunching->
              getChoiceOptionsBlockForLabel (
                label,
                *this);

        string
          labelOptionsBlockAsString =
            optionsBlock->
              asOptionsString ();

        fCommandsList.push_back (
          toolAndInputAsString
            +
          ' '
            +
          mainOptionsAsString
            +
          ' '
            +
          labelOptionsBlockAsString);
      } // for
    }

    else if (fSelectedOptionsBlocksList.size ()) {
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
      // there are no 'select' nor 'every' statements,
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
          "there can be only 1 choice if there is no 'select' nor 'every' statement",
          fScannerLocation);
      }
    }
  }
}
