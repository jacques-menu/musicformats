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

%locations

// other Bison options
%define parse.trace
%define parse.error detailed
%define parse.lac full

%printer { yyo << $$; } <*>;


%code {

#include "mfslBasicTypes.h"

S_mfslVariable pCurrentChoiceVariable;

}


// the MFSL tokens
//_______________________________________________________________________________

%define api.token.prefix {TOK_}
%token
  BAR  "|"
  SEMICOLON ";"
  COLON ":"
  EQUAL  "="

  SLASH  "/"
  COMMA  ","

  STAR  "*"

  LEFT_PARENTHESIS "("
  RIGHT_PARENTHESIS ")"

  TOOL    "tool"
  INPUT   "input"

  CHOICE  "choice"
  SET     "set"
  CASE    "case"
  ALL     "all"
;

%code {
# include "mfslDriver.h"
}

%token <string> INTEGER     "integer"
%token <string> DOUBLE     "double"

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
      {

        // push a new current scope onto the stack
        drv.pushScopeOntoStack (
          mfslScope::create ("Main scope"));
      }

  Tool

  Input

  ScriptElementsSequence
      {
        // store the resulting current scope in the driver
//         drv.setOptionsNamesAndValues (
//           drv.topOfScopesStack ());

        // pop the current current scope from the stack
//         drv.popScopeFromStack ();
      }
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


// tool
//_______________________________________________________________________________

Tool
 : TOOL ":" NAME
      {
        drv.setToolName ($3);
      }
 ;


// input
//_______________________________________________________________________________

Input
  : INPUT ":" NAME
      {
        drv.setInputFileName ($3);
      }

  | INPUT ":" String
      {
        drv.setInputFileName ($3);
     }
;


// contents
//_______________________________________________________________________________

ScriptElementsSequence
	: ScriptElement

	| ScriptElementsSequence ScriptElement
;

ScriptElement
  : Option
  | ChoiceDeclaration
  | ChoiceSetting
  | CaseStatement
;


// options
//_______________________________________________________________________________

Option
  : OPTION
      {
        if (drv.getDisplayNonTerminals ()) {
          gLogStream <<
            "====> option " << $1 <<
            endl;
        }

//         S_oahOptionsNamesAndValuesVector
//           optionsNamesAndValuesVector =
//             oahOptionsNamesAndValuesVector::create ();
//
//         optionsNamesAndValuesVector->
//           appendOptionNameAndValue ($1, "");

        drv.registerOptionNamesAndValuesInCurrentScope (
          oahOptionNameAndValue::create ($1, ""));
      }

  | OPTION OptionValue
      {
        if (drv.getDisplayNonTerminals ()) {
          gLogStream <<
            "====> option " << $1 << ' ' << $2 <<
            endl;
        }

//         S_oahOptionsNamesAndValuesVector
//           optionsNamesAndValuesVector =
//             oahOptionsNamesAndValuesVector::create ();
//
//         optionsNamesAndValuesVector->
//           appendOptionNameAndValue;

        drv.registerOptionNamesAndValuesInCurrentScope (
          oahOptionNameAndValue::create ($1, $2));
      }
;

OptionValue
  : NAME
  | Number
  | String
;


// choices
//_______________________________________________________________________________

ChoiceDeclaration
  : CHOICE NAME ":"
      {
        if (drv.getTraceSemantics ()) {
          gLogStream <<
            "====> ChoiceDeclaration " << $2 << " : " << "..." <<
            endl;
        }

        pCurrentChoiceVariable =
          drv.getVariablesTable ()->
            createAndInsertVariable ($2);
      }

    ChoicesPossibleValues SEMICOLON
      {
      }
  ;

ChoicesPossibleValues
  : NAME
      {
        pCurrentChoiceVariable->
          addPossibleValue ($1);
      }

  | ChoicesPossibleValues "|" NAME
      {
        pCurrentChoiceVariable->
          addPossibleValue ($3);
      }
;


ChoiceSetting
  : SET NAME "="
      {
        if (drv.getTraceSemantics ()) {
          gLogStream <<
            "====> ChoiceSetting, set " << $2 << " = " << "..." <<
            endl;
        }

        pCurrentChoiceVariable =
          drv.getVariablesTable () ->
            lookupVariableByName ($2);
      }

    NAME SEMICOLON
      {
        pCurrentChoiceVariable->
          setVariableValue ($5);
      }
;


// case
//_______________________________________________________________________________

CaseStatement
  : CASE NAME ":"
      {
        if (drv.getTraceSemantics ()) {
          gLogStream <<
            "====> CaseStatement: " << $2 << ":" << " ..." <<
            endl;
        }

        // push a new current case statement descr
        drv.pushCaseStatementOntoStack (
          mfslCaseStatement::create (
            drv.getVariablesTable () ->
              checkCaseVariableByName ($2)));
      }

    CaseAlternativesSequence SEMICOLON
      {
        if (drv.getTraceSemantics ()) {
//           S_oahOptionsNamesAndValuesVector
//             optionsNamesAndValuesVector=
//               drv.topOfOptionNamesAndValuesStack ();

//           gLogStream <<
//             "====> CaseStatement: ";
//
//           if (optionsNamesAndValuesVector) {
//             gLogStream << optionsNamesAndValuesVector;
//           }
//           else {
//             gLogStream << " none";
//           }
//
//           gLogStream <<
//             endl;
        }

        // have all the possible value been used as labels?
        drv.topOfCaseStatementStack ()->
          checkThatAllPossibleValuesHaveBeenUsed ();

        // pop the current case statement descr from the stack
        drv.popCaseStatementFromStack ();
      }
;

CaseAlternativesSequence
  : CaseAlternative

  | CaseAlternativesSequence CaseAlternative
;

CaseAlternative :
  NAME ":"
      {
        if (drv.getTraceSemantics ()) {
          gLogStream <<
            "====> CaseAlternative \"" <<
            $1 <<
            endl;
        }

        // push a new current options names and values
        // for this case alternative
//         drv.registerOptionNamesAndValuesInCurrentScope (
//           oahOptionsNamesAndValuesVector::create ());
      }

  ScriptElementsSequence SEMICOLON
      {
        string label = $1;

//         S_oahOptionsNamesAndValuesVector
//           optionsNamesAndValuesVector =
//             drv.topOfOptionNamesAndValuesStack ();

//         if (drv.getTraceSemantics ()) {
//           gLogStream <<
//             "====> CaseAlternative \"" <<
//             label <<
//             "\", optionsNamesAndValuesVector: ";
//
//           if (optionsNamesAndValuesVector) {
//             gLogStream << optionsNamesAndValuesVector;
//           }
//           else {
//             gLogStream << " none";
//           }
//
//           gLogStream <<
//             endl;
//         }

        S_mfslCaseStatement
          currentCaseStatement =
            drv.topOfCaseStatementStack ();

        S_mfslVariable
          currentCaseVariable =
            currentCaseStatement->
              getCaseVariable ();

        // register this case label value
        currentCaseStatement->
          registerCaseLabelValue (label);

        // is label equal to case variable value?
        if (currentCaseVariable->getVariableValue () == label) {
          // this is the case alternative to keep
          if (drv.getTraceSemantics ()) {
            gLogStream <<
              "====> Keeping caseAlternative \"" <<
              label <<
              "\", optionsNamesAndValuesVector: ";

  // optionsNameAndValueVectorsPlusEquals


//             if (optionsNamesAndValuesVector) {
//               gLogStream << optionsNamesAndValuesVector;
//             }
//             else {
//               gLogStream << " none";
//             }
//
//             gLogStream <<
//               endl;
          }

//           drv.registerOptionNamesAndValuesInCurrentScope (
//             optionsNamesAndValuesVector);
        }
        else {
          // discard this case alternative
          if (drv.getTraceSemantics ()) {
            gLogStream <<
              "====> Discarding caseAlternative \"" <<
              label <<
              "\", optionsNamesAndValuesVector: ";

//             if (optionsNamesAndValuesVector) {
//               gLogStream << optionsNamesAndValuesVector;
//             }
//             else {
//               gLogStream << " none";
//             }
//
//             gLogStream <<
//               endl;
          }
        }

        // pop the current options names and values from the stack
//         drv.popOptionNamesAndValuesFromStack ();
      }
;



//_______________________________________________________________________________
%%
//_______________________________________________________________________________


// other service code
//_______________________________________________________________________________


void
yy::parser::error (const location_type& l, const string& m)
{
  cerr << l << ": " << m << '\n';
}

//         if (drv.getTraceSemantics ()) {
//           S_oahOptionsNamesAndValuesVector
//             optionsNamesAndValuesVector =
//               drv.topOfOptionNamesAndValuesStack ();
//
//           gLogStream <<
//             "====> Script optionsNamesAndValuesVector: ";
//
//           if (optionsNamesAndValuesVector) {
//             gLogStream << optionsNamesAndValuesVector;
//           }
//           else {
//             gLogStream << " none";
//           }
//
//           gLogStream <<
//             endl;
//         }

//         for (S_oahOptionsNamesAndValuesVector ptionsNamesAndValues: drv.getOptionsNamesAndValuesStack) {
//           gLogStream << ptionsNamesAndValues << endl;
//         }
