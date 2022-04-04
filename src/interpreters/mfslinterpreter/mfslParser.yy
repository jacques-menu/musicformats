%{

/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/


#include <iostream>

#include <string>

#include "mfIndentedTextOutput.h"

#include "oahBasicTypes.h"

#include "mfslWae.h"


using namespace MusicFormats;

%}


/// the Bison options
//_______________________________________________________________________________

%skeleton "lalr1.cc" // -*- C++ -*-
%require "3.8.1"
%defines

%define api.token.raw

%define api.token.constructor
%define api.value.type variant
%define parse.assert

%code requires {
  # include <string>

  class mfslDriver;
}

// the parsing context
%param { mfslDriver& drv }

%verbose
%locations

// other Bison options
%define parse.trace
%define parse.error detailed
%define parse.lac full
// %define api.pure full

%printer { yyo << $$; } <*>;


%code {

#include "mfslBasicTypes.h"

S_mfslChoice pCurrentChoiceChoice;

}


// the MFSL tokens
//_______________________________________________________________________________

%define api.token.prefix {TOK_}
%token
  BAR        "|"
  EQUAL      "="
  SEMICOLON  ";"
  COLON      ":"
  COMMA      ","

  TOOL      "tool"
  INPUT      "input"

  CHOICE    "choice"
  DEFAULT   "default"

  CASE       "case"

  SELECT     "select"
  EVERY      "every"
;

%code {
# include "mfslDriver.h"
}

%token <string> INTEGER "integer number"
%token <string> DOUBLE  "double number"

%token <string> SINGLE_QUOTED_STRING "single quoted_string"
%token <string> DOUBLE_QUOTED_STRING "double quoted_string"

%token <string> NAME "name"

%token <string> OPTION "option"


// the MFSL non-terminals
//_______________________________________________________________________________

%nterm <string> Number

%nterm <string> SingleString
%nterm <string> String

%nterm <string> OptionValue


// the MFSL axiom
//_______________________________________________________________________________

%start Script



//_______________________________________________________________________________
%%
//_______________________________________________________________________________



//_______________________________________________________________________________
// the MFSL non-terminals
//_______________________________________________________________________________


// the MFSL axiom
//_______________________________________________________________________________

Script :
  Tool

  Input

      {
        ++gIndenter;

        // create the main options block
        S_mfslOptionsBlock
          mainOptionsBlock =
            mfslOptionsBlock::create (
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

  OptionalSelectOrEveryStatement
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


// tool statement
//_______________________________________________________________________________

Tool
 : TOOL COLON NAME
      {
        drv.setToolName ($3);
      }
 ;


// input statement
//_______________________________________________________________________________

Input
  : INPUT COLON NAME
      {
        drv.setInputSouceName ($3);
      }

  | INPUT COLON String
      {
        drv.setInputSouceName ($3);
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
  | CaseStatement
;


// options
//_______________________________________________________________________________

Option
  : OPTION
      {
        ++gIndenter;

        if (drv.getDisplayOptions ()) {
          gLogStream <<
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
          gLogStream <<
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

        string choiceName = $2;

        if (drv.getTraceChoiceStatements ()) {
          gLogStream <<
            "====> choice " << choiceName << " : " << "..." <<
            ", line " << drv.getScannerLocation () <<
            endl;
        }

        // create a choice
        S_mfslChoice
          choice =
             mfslChoice::create (
               choiceName);

        // add it to the driver's choices table
        drv.getChoicesTable ()->
          addChoice (
            choice,
            drv);

        pCurrentChoiceChoice = choice;
      }

    ChoiceLabels

    COMMA

    DEFAULT COLON NAME
      {
        string
          choiceName = $2,
          label      = $9;

        // fetch the voice in the choices table
        S_mfslChoice
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
        if (drv.getTraceChoiceStatements ()) {
          gLogStream <<
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

        string label = $1;

        pCurrentChoiceChoice->
          addLabel (
            label,
            drv);

        --gIndenter;
      }

  | ChoiceLabels "|" NAME
      {
        ++gIndenter;

        string label = $3;

        pCurrentChoiceChoice->
          addLabel (
            label,
            drv);

        --gIndenter;
      }
;


// case statement
//_______________________________________________________________________________

CaseLabel
  : NAME
      {
        ++gIndenter;

        string label = $1;

        // fetch case statement stack top
        S_mfslCaseStatement
          currentCaseStatement =
            drv.caseStatementsStackTop ();

        // register this case label
        currentCaseStatement->
          registerCaseLabel (
            label,
            drv);

        --gIndenter;
      }
;

CaseLabelsSeq
  : CaseLabel
  | CaseLabelsSeq COMMA CaseLabel
;

CaseStatement
  : CASE NAME COLON
      {
        ++gIndenter;

        string choiceName = $2;

        if (drv.getTraceCaseStatements ()) {
          gLogStream <<
            "====> case " << choiceName << ": ..." <<
            ", line " << drv.getScannerLocation () <<
            endl;
        }

        // create a new current case statement
        S_mfslChoicesTable
          choicesTable =
            drv.getChoicesTable ();

        S_mfslChoice
          choice =
            choicesTable->
              fetchChoiceByName (
                choiceName,
                drv);

        if (! choice) {
          stringstream s;

          s <<
            "name \"" << choiceName <<
            "\" is no choice name, cannot be used in a 'select' statement" <<
            ", line " << drv.getScannerLocation () <<
            endl;

          mfslError (
            s.str (),
            drv.getScannerLocation ());
        }

        S_mfslCaseStatement
          caseStatement =
            mfslCaseStatement::create (
              choice,
              drv);

        // push it onto the case statements stack
        drv.caseStatementsStackPush (
          caseStatement);

        // mark the choice as used
        choice->
          setChoiceIsUsedInCaseStatements ();
      }

    OptionalCaseAlternativesSeq SEMICOLON
      {
        // have all the label been used as labels?
        drv.caseStatementsStackTop ()->
          checkThatAllLabelsHaveBeenUsed (drv);

        // pop the current case statement from the case statements stack
        drv.caseStatementsStackPop ();

        if (drv.getTraceCaseStatements ()) {
          gLogStream <<
            "------------------------------------------------------------" <<
            endl;
        }

        --gIndenter;
      }
;

OptionalCaseAlternativesSeq
  : CaseAlternativesSeq
  |
;

CaseAlternativesSeq
  : CaseAlternative

  | CaseAlternativesSeq CaseAlternative
;

CaseAlternative
  :
      {
        ++gIndenter;

        S_mfslCaseStatement
          currentCaseStatement =
            drv.caseStatementsStackTop ();

        // forget about previous case alternative if any
        currentCaseStatement->
          clearCaseCurrentLabelsList ();
      }

    CaseLabelsSeq COLON
      {
        S_mfslCaseStatement
          currentCaseStatement =
            drv.caseStatementsStackTop ();

        // push a new current options block onto the stack
        stringstream s;

        s <<
          "Case alternative for " <<
          currentCaseStatement->
            currentLabelsListAsString () <<
          ", line " << drv.getScannerLocation ();

        string
          caseAlternativeDescription =
            s.str ();

        S_mfslOptionsBlock
          caseAlternativeOptionsBlock =
            mfslOptionsBlock::create (
              caseAlternativeDescription);

        drv.optionsBlocksStackPush (
          caseAlternativeOptionsBlock,
          caseAlternativeDescription);
      }

    OptionalScriptElementsSeq SEMICOLON
      {
        S_mfslCaseStatement
          currentCaseStatement =
            drv.caseStatementsStackTop ();

        S_mfslChoice
          currentCaseChoice =
            currentCaseStatement->
              getCaseChoice ();

        // handle the labels
        for (string label : currentCaseStatement->getCaseCurrentLabelsList ()) {
          // enrich the options block for label
          currentCaseChoice->
            enrichLabelOptionsBlock (
              label,
              drv.optionsBlocksStackTop (),
              drv);
        } // for

        // discard this case alternative
        stringstream s;

        s <<
          "Discarding case alternative options block for " <<
          currentCaseStatement->
            currentLabelsListAsString () <<
          ", line " << drv.getScannerLocation () <<
          endl;

        string context = s.str ();

        drv.optionsBlocksStackPop (
          context);

        --gIndenter;
      }
;


// select or every statement
//_______________________________________________________________________________

OptionalSelectOrEveryStatement
  : SelectStatement
  | EveryStatement
  |
;

SelectStatement
  : SELECT NAME COLON NAME SEMICOLON
      {
        string
          choiceName = $2,
          label = $4;

        drv.setSelectLabelForToolLaunching (
          choiceName,
          label);
      }

EveryStatement
  : EVERY NAME SEMICOLON
      {
        string
          choiceName = $2;

        drv.setEveryChoiceForToolLaunching (
          choiceName);
      }


//_______________________________________________________________________________
%%
//_______________________________________________________________________________


// other service code
//_______________________________________________________________________________


void
yy::parser::error (const location_type& loc, const string& message)
{
  mfslError (
    message,
    loc);
}
