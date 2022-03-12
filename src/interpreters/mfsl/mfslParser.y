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

// #include <string>

#include "mfIndentedTextOutput.h"


using namespace std;

using namespace MusicFormats;

// #define gLogStream        *MusicFormats::gGlobalLogIndentedOstream

%}


/* the Bison options */
/* ----------------- */

%skeleton "lalr1.cc" // -*- C++ -*-
%require "3.8.1"
%defines

%printer { yyo << $$; } <*>;

%define api.token.raw

%define api.token.constructor
%define api.value.type variant
%define parse.assert

%code requires {
  #include <string>

  class mfslDriver;
}

// The parsing context
%param { mfslDriver& drv } // declaration, any parameter name is fine

%locations

%verbose // to produce mfslParser.output

%define parse.trace
%define parse.error detailed
%define parse.lac full

//%define parse.report
//%define parse.verbose

%code {
# include "mfslDriver.h"
}


/* the MFSL tokens */
/* ----------- --- */

%define api.token.prefix {TOK_}
%token
  BAR  "|"
  SEMICOLON ";"
  COLON ":"
  EQUAL  "="

  SLASH  "/"
  COMMA  ","

  STAR  "*"

  LEFT_PARENTHESIS  "("
  RIGHT_PARENTHESIS ")"

  TOOL    "tool"
  INPUT   "input"
  CHOICE  "choice"
  SET     "set"
  CASE    "case"
;

%token <std::string> INTEGER "integer"
%token <std::string> DOUBLE  "double"

%token <std::string> SINGLE_QUOTED_STRING "single quoted_string"
%token <std::string> DOUBLE_QUOTED_STRING "double quoted_string"

%token <std::string> NAME "name"

%token <std::string> OPTION "option"


/* the MFSL non-terminals */
/* ---------------------- */

%nterm <std::string> Number
%nterm <std::string> String
%nterm <std::string> OptionValue



/* the MFSL axiom */
/* -------------- */

%start script




%%




/* ---------------------- */
/* the MFSL non-terminals */
/* ---------------------- */

// the MFSL axiom
//_______________________________________________________________________________

script :
  Tool
  Input
  ContentsElements
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
	    if (drv.getDisplayNonTerminals ()) {
        gLogStream <<
          "  ==> tool: " << $3 <<
          endl << endl;
			}

			drv.setToolName ($3);
    }
 ;


// input
//_______________________________________________________________________________

Input
  : INPUT ":" NAME
    {
	    if (drv.getDisplayNonTerminals ()) {
        gLogStream <<
          "  ==> input: " << $3 <<
          endl << endl;
			}

			drv.setInputFileName ($3);
   }

  | INPUT ":" String
    {
	    if (drv.getDisplayNonTerminals ()) {
        gLogStream <<
          "  ==> input" << $3 <<
          endl << endl;
			}

			drv.setInputFileName ($3);
   }
 ;


// contents
//_______________________________________________________________________________

ContentsElements
	: ContentsElement
	| ContentsElements ContentsElement
;

ContentsElement
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
          "  ==> option " << $1 <<
          endl << endl;
			}
    }

  | OPTION OptionValue
    {
	    if (drv.getDisplayNonTerminals ()) {
        gLogStream <<
          "  ==> option " << $1 << ' ' << $2 <<
          endl << endl;
			}
    }

OptionValue
  : NAME
  | Number
  | String
;

// choices
//_______________________________________________________________________________

ChoiceDeclaration
  : CHOICE NAME ":" Choices SEMICOLON
    {
	    if (drv.getDisplayNonTerminals ()) {
        gLogStream <<
          "  ==> ChoiceDeclaration " << $2 << " : ..." <<
          endl << endl;
			}
    }
  ;

Choices
  : NAME
  | Choices "|" NAME
;


ChoiceSetting
  : SET NAME "=" NAME SEMICOLON
    {
	    if (drv.getDisplayNonTerminals ()) {
        gLogStream <<
          "  ==> ChoiceSetting, set " << $2 << " = " << $4 <<
          endl << endl;
			}
    }
  ;

CaseStatement
  : CASE NAME ":" Cases SEMICOLON
    {
	    if (drv.getDisplayNonTerminals ()) {
        gLogStream <<
          "  ==> CaseStatement, " << $2 << " : ..." <<
          endl << endl;
			}
    }
;

Cases
  : Case
  | Cases Case
;

Case
  : NAME ":" ContentsElements SEMICOLON
    {
	    if (drv.getDisplayNonTerminals ()) {
        gLogStream <<
          "  ==> Case " << $1 << " : ..." <<
          endl << endl;
			}
    }
;



// ----------------------------------- */
%%
// ----------------------------------- */



/* other service code */
// ----------------------------------- */


void yy::parser::error (const location_type& l, const string& m)
{
  std::cerr << l << ": " << m << '\n';
}

