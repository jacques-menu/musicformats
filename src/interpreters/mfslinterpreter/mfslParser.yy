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
//   SELECT    "select"

  CASE       "case"

  ONLY       "only"
  ALL        "all"
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
              "Main optionsBlock");

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

  OptionalOnlyOrAllStatement
;


// numbers
//_______________________________________________________________________________

Number
  : INTEGER
  | DOUBLE
;


// strings
//_______________________________________________________________________________

String
  : SINGLE_QUOTED_STRING
  | DOUBLE_QUOTED_STRING
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
        drv.setInputFileName ($3);
      }

  | INPUT COLON String
      {
        drv.setInputFileName ($3);
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
//   | ChoiceSelection
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
          oahOption::create ($1, ""));

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
          oahOption::create ($1, $2));

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

        S_mfslChoice
          choice =
            drv.getChoicesTable ()->
              createAndInsertChoice (
                choiceName,
                mfslChoiceKind::kChoiceChoice);

        pCurrentChoiceChoice = choice;
      }

    ChoiceLabels SEMICOLON
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
          ChoicesTable =
            drv.getChoicesTable ();

        S_mfslChoice
          choice =
            ChoicesTable->
              lookupChoiceByName (
                choiceName);

        if (! choice) {
          stringstream s;

          s <<
            "choiceName \"" << choiceName <<
            "\" is no choice name";

          mfslError (
            s.str (),
            drv.getScannerLocation ());
        }

        S_mfslCaseStatement
          caseStatement =
            mfslCaseStatement::create (
              choice);

        // push it onto the case statements stack
        drv.caseStatementsStackPush (
          caseStatement);
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

        drv.optionsBlocksStackPush (
          mfslOptionsBlock::create (
            caseAlternativeDescription),
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
              drv.optionsBlocksStackTop ());
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


// only or all statement
//_______________________________________________________________________________

OptionalOnlyOrAllStatement
  : OnlyStatement
  | AllStatement
  |
;

OnlyStatement
  : ONLY NAME COLON NAME SEMICOLON
      {
        string
          choiceName = $2,
          label = $4;

        drv.setOnlyLabelForToolLaunching (
          choiceName,
          label);
      }

AllStatement
  : ALL NAME SEMICOLON
      {
        string
          choiceName = $2;

        drv.setAllChoicesOptionsBlockForToolLaunching (
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
