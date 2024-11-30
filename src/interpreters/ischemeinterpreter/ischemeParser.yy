%{

/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2024

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/


#include <iostream>

#include <string>

#include "mfIndentedTextOutput.h"

#include "oahBasicTypes.h"

#include "ischemeWae.h"


using namespace MusicFormats;

%}


/// the Bison options
//_______________________________________________________________________________

%skeleton "lalr1.cc" // -*- C++ -*-
%require "3.8.1"
%defines

%define api.prefix {stringMatcher}

%define api.token.raw

%define api.token.constructor
%define api.value.type variant
%define parse.assert

%code requires {
  # include <string>

  class ischemeDriver;
}

// the parsing context
%param { ischemeDriver& drv }

%verbose
%locations

// other Bison options
%define parse.trace
%define parse.error detailed
%define parse.lac full
// %define api.pure full

%printer { yyo << $$; } <*>;


%code {

#include "ischemeBasicTypes.h"

}


// the iScheme tokens
//_______________________________________________________________________________

%define api.token.prefix {ISCM_TOK_}
%token
  BAR        "|"
  AMPERSAND  "&"
  EQUAL      "="
  SEMICOLON  ";"
  COLON      ":"
  COMMA      ","

  SERVICE      "service"
  INPUT      "input"

  CHOICE    "choice"
  DEFAULT   "default"

  CASE       "case"

  SELECT     "select"
  ALL        "all"
;

%code {
# include "ischemeDriver.h"
}

%token <std::string> INTEGER "integer number"
%token <std::string> DOUBLE  "double number"

%token <std::string> SINGLE_QUOTED_STRING "single quoted_string"
%token <std::string> DOUBLE_QUOTED_STRING "double quoted_string"

%token <std::string> NAME "name"

%token <std::string> OPTION "option"


// the iScheme non-terminals
//_______________________________________________________________________________

%nterm <std::string> Number

%nterm <std::string> SingleString
%nterm <std::string> String

%nterm <std::string> OptionValue

%nterm <std::string> LabelName


// the iScheme axiom
//_______________________________________________________________________________

%start Script



//_______________________________________________________________________________
%%
//_______________________________________________________________________________



//_______________________________________________________________________________
// the iScheme non-terminals
//_______________________________________________________________________________


// the iScheme axiom
//_______________________________________________________________________________

Script :
  Service

  Input

      {
        ++gIndenter;

        // create the main options block
        S_ischemeOptionsBlock
          mainOptionsBlock =
            ischemeOptionsBlock::create (
              "Main options block");

        // push it onto the stack
        drv.optionsBlocksStackPush (
          mainOptionsBlock,
          "Creation of the main options block");

        --gIndenter;
      }

  OptionalScriptElementsSeq

      {
        ++gIndenter;

        // DON'T pop the current current options block from the stack, JMI
        // it contains the consolidated options and values

        --gIndenter;
      }

  OptionalSelectStatements
;


// numbers
//_______________________________________________________________________________

Number
  : INTEGER
  | DOUBLE
;


// strings
//_______________________________________________________________________________

SingleString
  : SINGLE_QUOTED_STRING
  | DOUBLE_QUOTED_STRING
;

String
  : SingleString
  | String SingleString
      {
        $$ = $1 + $2;
      }
;


// service statement
//_______________________________________________________________________________

Service
 : SERVICE COLON NAME SEMICOLON
      {
        drv.setService ($3);
      }
 ;


// input statement
//_______________________________________________________________________________

Input
  : INPUT COLON InputSourcesSeq SEMICOLON
;

InputSourcesSeq
  : InputSource
  | InputSourcesSeq AMPERSAND InputSource
;

InputSource
  : NAME
      {
        drv.appendInputSouce ($1);
      }

  | String
      {
        drv.appendInputSouce ($1);
      }
;


// contents
//_______________________________________________________________________________

OptionalScriptElementsSeq
  : ScriptElementsSeq
  |
;

ScriptElementsSeq
  : ScriptElement

  | ScriptElementsSeq ScriptElement
;

ScriptElement
  : Option
  | ChoiceDeclaration
  | CaseChoiceStatement
  | CaseInputStatement
;


// options
//_______________________________________________________________________________

Option
  : OPTION
      {
        ++gIndenter;

        if (drv.getDisplayOptions ()) {
          gLog <<
            "====> option " << $1 <<
            ", line " << drv.getScannerLocation () <<
            endl;
        }

        drv.registerOptionInCurrentOptionsBlock (
          oahOption::create ($1, ""),
          drv);

        --gIndenter;
      }

  | OPTION OptionValue
      {
        ++gIndenter;

        if (drv.getDisplayOptions ()) {
          gLog <<
            "====> option " << $1 << ' ' << $2 <<
            ", line " << drv.getScannerLocation () <<
            endl;
        }

        drv.registerOptionInCurrentOptionsBlock (
          oahOption::create ($1, $2),
          drv);

        --gIndenter;
      }
;

OptionValue
  : NAME
  | Number
  | String
  | NAME EQUAL NAME
      {
        $$ = $1 + "=" + $3;
      }
  | NAME COLON NAME
      {
        $$ = $1 + ":" + $3;
      }
  | NAME EQUAL Number
      {
        $$ = $1 + "=" + $3;
      }
  | NAME COLON Number
      {
        $$ = $1 + ":" + $3;
      }
;


// Choice statement
//_______________________________________________________________________________

ChoiceDeclaration
  : CHOICE NAME COLON
      {
        ++gIndenter;

        std::string choiceName = $2;

        if (drv.getTraceCaseChoiceStatements ()) {
          gLog <<
            "====> choice " << choiceName << ": " << "..." <<
            ", line " << drv.getScannerLocation () <<
            endl;
        }

        // create a choice
        S_ischemeChoice
          choice =
             ischemeChoice::create (
               choiceName);

        // add it to the driver's choices table
        drv.getChoicesTable ()->
          addChoice (
            choice,
            drv);

        drv.setCurrentChoiceChoice (choice);
      }

    ChoiceLabels

    COMMA

    DEFAULT COLON NAME
      {
        std::string
          choiceName = $2,
          label      = $9;

        // fetch the voice in the choices table
        S_ischemeChoice
          choice =
            drv.getChoicesTable ()->
              fetchChoiceByName (
                choiceName,
                drv);

        // register label as the default label in this choice
        choice->
          registerChoiceDefaultLabel (
            label,
            drv);
      }

    SEMICOLON
      {
        if (drv.getTraceCaseChoiceStatements ()) {
          gLog <<
            "------------------------------------------------------------" <<
            endl;
        }

        --gIndenter;
      }
  ;

ChoiceLabels
  : NAME
      {
        ++gIndenter;

        std::string label = $1;

        drv.getCurrentChoiceChoice ()->
          addLabel (
            label,
            drv);

        --gIndenter;
      }

  | ChoiceLabels "|" NAME
      {
        ++gIndenter;

        std::string label = $3;

        drv.getCurrentChoiceChoice ()->
          addLabel (
            label,
            drv);

        --gIndenter;
      }
;


// case choice statement
//_______________________________________________________________________________

CaseChoiceLabel
  : NAME
      {
        ++gIndenter;

        std::string label = $1;

        // fetch case statement stack top
        S_ischemeCaseChoiceStatement
          currentCaseChoiceStatement =
            drv.caseChoiceStatementsStackTop ();

        // register this case label
        currentCaseChoiceStatement->
          registerCaseChoiceLabel (
            label,
            drv);

        --gIndenter;
      }
;

CaseChoiceLabelsSeq
  : CaseChoiceLabel
  | CaseChoiceLabelsSeq COMMA CaseChoiceLabel
;

CaseChoiceStatement
  : CASE NAME COLON
      {
        ++gIndenter;

        std::string choiceName = $2;

        if (drv.getTraceCaseChoiceStatements ()) {
          gLog <<
            "====> case " << choiceName << ": ..." <<
            ", line " << drv.getScannerLocation () <<
            endl;
        }

        // create a new current case statement
        S_ischemeChoicesTable
          choicesTable =
            drv.getChoicesTable ();

        S_ischemeChoice
          choice =
            choicesTable->
              fetchChoiceByName (
                choiceName,
                drv);

        if (! choice) {
          std::stringstream ss;

          ss <<
            "name \"" << choiceName <<
            "\" is no choice name, cannot be used in a 'select' statement" <<
            ", line " << drv.getScannerLocation () <<
            endl;

          ischemeError (
            ss.str (),
            drv.getScannerLocation ());
        }

        S_ischemeCaseChoiceStatement
          caseChoiceStatement =
            ischemeCaseChoiceStatement::create (
              choice,
              drv);

        // push it onto the case statements stack
        drv.caseChoiceStatementsStackPush (
          caseChoiceStatement);

        // mark the choice as used
        choice->
          setChoiceIsUsedInCaseChoiceStatements ();
      }

    OptionalCaseChoiceAlternativesSeq SEMICOLON
      {
        // have all the label been used as labels?
        drv.caseChoiceStatementsStackTop ()->
          checkThatAllLabelsHaveBeenUsed (drv);

        // pop the current case statement from the case statements stack
        drv.caseChoiceStatementsStackPop ();

        if (drv.getTraceCaseChoiceStatements ()) {
          gLog <<
            "------------------------------------------------------------" <<
            endl;
        }

        --gIndenter;
      }
;

OptionalCaseChoiceAlternativesSeq
  : CaseChoiceAlternativesSeq
  |
;

CaseChoiceAlternativesSeq
  : CaseChoiceAlternative

  | CaseChoiceAlternativesSeq CaseChoiceAlternative
;

CaseChoiceAlternative
  :
      {
        ++gIndenter;

        S_ischemeCaseChoiceStatement
          currentCaseChoiceStatement =
            drv.caseChoiceStatementsStackTop ();

        // forget about previous case alternative if any
        currentCaseChoiceStatement->
          clearCaseCurrentLabelsList ();
      }

    CaseChoiceLabelsSeq COLON
      {
        S_ischemeCaseChoiceStatement
          currentCaseChoiceStatement =
            drv.caseChoiceStatementsStackTop ();

        // push a new current options block onto the stack
        std::stringstream ss;

        ss <<
          "Case alternative for " <<
          currentCaseChoiceStatement->
            currentLabelsListAsString () <<
          ", line " << drv.getScannerLocation ();

        std::string
          CaseChoiceAlternativeDescription =
            ss.str ();

        S_ischemeOptionsBlock
          CaseChoiceAlternativeOptionsBlock =
            ischemeOptionsBlock::create (
              CaseChoiceAlternativeDescription);

        drv.optionsBlocksStackPush (
          CaseChoiceAlternativeOptionsBlock,
          CaseChoiceAlternativeDescription);
      }

    OptionalScriptElementsSeq SEMICOLON
      {
        S_ischemeCaseChoiceStatement
          currentCaseChoiceStatement =
            drv.caseChoiceStatementsStackTop ();

        S_ischemeChoice
          currentCaseChoice =
            currentCaseChoiceStatement->
              getCaseChoice ();

        // handle the labels
        for (std::string label : currentCaseChoiceStatement->getCaseCurrentLabelsList ()) {
          // enrich the options block for label
          currentCaseChoice->
            enrichLabelOptionsBlock (
              label,
              drv.optionsBlocksStackTop (),
              drv);
        } // for

        // discard this case alternative
        std::stringstream ss;

        ss <<
          "Discarding case alternative options block for " <<
          currentCaseChoiceStatement->
            currentLabelsListAsString () <<
          ", line " << drv.getScannerLocation () <<
          endl;

        std::string context = ss.str ();

        drv.optionsBlocksStackPop (
          context);

        --gIndenter;
      }
;


// case input statementf
//_______________________________________________________________________________

CaseInputName
  : NAME
      {
        ++gIndenter;

        std::string label = $1;

        // fetch case input statement stack top
        S_ischemeCaseInputStatement
          currentCaseInputStatement =
            drv.caseInputStatementsStackTop ();

        // register this case input label
        currentCaseInputStatement->
          registerCaseInputName (
            label,
            drv);

        --gIndenter;
      }
;

CaseInputNamesSeq
  : CaseInputName
  | CaseInputNamesSeq COMMA CaseInputName
;

CaseInputStatement
  : CASE INPUT COLON
      {
        ++gIndenter;

        std::string inputName = "$2 INPUT";

        if (drv.getTraceCaseInputStatements ()) {
          gLog <<
            "====> case input " << inputName << ": ..." <<
            ", line " << drv.getScannerLocation () <<
            endl;
        }

        // create a new current case input statement
        S_ischemeInputsTable
          inputsTable =
            drv.getInputsTable ();

        S_ischemeInput
          input =
            inputsTable->
              fetchInputByName (
                inputName,
                drv);

        if (! input) {
          std::stringstream ss;

          ss <<
            "name \"" << inputName <<
            "\" is no input name, cannot be used in a 'select' statement" <<
            ", line " << drv.getScannerLocation () <<
            endl;

          ischemeError (
            ss.str (),
            drv.getScannerLocation ());
        }

        S_ischemeCaseInputStatement
          caseInputStatement =
            ischemeCaseInputStatement::create (
              input,
              drv);

        // push it onto the case input statements stack
        drv.caseInputStatementsStackPush (
          caseInputStatement);

        // mark the input as used
        input->
          setInputIsUsedInCaseInputStatements ();
      }

    OptionalCaseInputAlternativesSeq SEMICOLON
      {
        // have all the name been used as names?
        drv.caseInputStatementsStackTop ()->
          clearCaseInputCurrentNamesList (); // drv ??? JMI

        // pop the current case input statement from the case input statements stack
        drv.caseInputStatementsStackPop ();

        if (drv.getTraceCaseInputStatements ()) {
          gLog <<
            "------------------------------------------------------------" <<
            endl;
        }

        --gIndenter;
      }
;

OptionalCaseInputAlternativesSeq
  : CaseInputAlternativesSeq
  |
;

CaseInputAlternativesSeq
  : CaseInputAlternative

  | CaseInputAlternativesSeq CaseInputAlternative
;

CaseInputAlternative
  :
      {
        ++gIndenter;

        S_ischemeCaseInputStatement
          currentCaseInputStatement =
            drv.caseInputStatementsStackTop ();

        // forget about previous case input alternative if any
        currentCaseInputStatement->
          clearCaseInputCurrentNamesList ();
      }

    CaseInputNamesSeq COLON
      {
        S_ischemeCaseInputStatement
          currentCaseInputStatement =
            drv.caseInputStatementsStackTop ();

        // push a new current options block onto the stack
        std::stringstream ss;

        ss <<
          "CaseInput alternative for " <<
          currentCaseInputStatement->
            currentNamesListAsString () <<
          ", line " << drv.getScannerLocation ();

        std::string
          caseInputAlternativeDescription =
            ss.str ();

        S_ischemeOptionsBlock
          caseInputAlternativeOptionsBlock =
            ischemeOptionsBlock::create (
              caseInputAlternativeDescription);

        drv.optionsBlocksStackPush (
          caseInputAlternativeOptionsBlock,
          caseInputAlternativeDescription);
      }

    OptionalScriptElementsSeq SEMICOLON
      {
        S_ischemeCaseInputStatement
          currentCaseInputStatement =
            drv.caseInputStatementsStackTop ();

        S_ischemeInput
          currentCaseInputInput =
            currentCaseInputStatement->
              getCaseInputInput ();

        // handle the names
        for (std::string name : currentCaseInputStatement->getCaseInputCurrentNamesList ()) {
          // enrich the options block for name
          currentCaseInputInput->
            enrichNameOptionsBlock (
              name,
              drv.optionsBlocksStackTop (),
              drv);
        } // for

        // discard this case input alternative
        std::stringstream ss;

        ss <<
          "Discarding case input alternative options block for " <<
          currentCaseInputStatement->
            currentNamesListAsString () <<
          ", line " << drv.getScannerLocation () <<
          endl;

        std::string context = ss.str ();

        drv.optionsBlocksStackPop (
          context);

        --gIndenter;
      }
;


// select statement
//_______________________________________________________________________________

OptionalSelectStatements
  : SelectStatementSeq
  |
;

SelectStatementSeq
  : SelectStatement
  | SelectStatementSeq SelectStatement
;

LabelName
  : NAME
  | ALL
      { $$ = ischemeDriver::K_ALL_PSEUDO_LABEL_NAME; }
;

SelectStatement
  : SELECT NAME COLON LabelName SEMICOLON
      {
        std::string
          choiceName = $2,
          label = $4;

        drv.handleSelectLabel (
          choiceName,
          label);
      }


//_______________________________________________________________________________
%%
//_______________________________________________________________________________


// other service code
//_______________________________________________________________________________


void
stringMatcher::parser::error (const location_type& loc, const std::string& message)
{
  ischemeError (
    message,
    loc);
}
