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

#include "mfFilterWae.h"


using namespace MusicFormats;

%}


/// the Bison options
//_______________________________________________________________________________

%skeleton "lalr1.cc" // -*- C++ -*-
%require "3.8.1"
%defines

%define api.prefix {mfFilter}

%define api.token.raw

%define api.token.constructor
%define api.value.type variant
%define parse.assert

%code requires {
  #include <string>

  class mfFilterDriver;
}

// the parsing context
%param { mfFilterDriver& drv } // declaration, any parameter name is fine

%verbose // to produce mfFilterParser.output

%locations

// other Bison options
%define parse.trace
%define parse.error detailed
%define parse.lac full
// %define api.pure full

%printer { yyo << $$; } <*>;


%code {
  #include "mfFilterBasicTypes.h"
}


// the mfFilter tokens
//_______________________________________________________________________________

%define api.token.prefix {mfFilter_TOK_}

%token
  AND               "|"
  OR                "&"
  NOT               "="

  LEFT_PARENTHESIS  "("
  RIGHT_PARENTHESIS ")"
;

%code {
  #include "mfFilterDriver.h"
}

%token <std::string> SINGLE_QUOTED_STRING "single quoted_string"
%token <std::string> DOUBLE_QUOTED_STRING "double quoted_string"


// the mfFilter non-terminals
//_______________________________________________________________________________

%nterm <std::string> Criterion

%nterm <std::string> Expression
%nterm               Or

%nterm <std::string> Term
%nterm               And

%nterm <std::string> Factor

%nterm <std::string> SubExpression

%nterm <std::string> SingleString
%nterm <std::string> String


// the mfFilter axiom
//_______________________________________________________________________________

%start Criterion



//_______________________________________________________________________________
%%
//_______________________________________________________________________________



//_______________________________________________________________________________
// the mfFilter non-terminals
//_______________________________________________________________________________


// the mfFilter axiom
//_______________________________________________________________________________

Criterion
  :
      {
        ++gIndenter;

        // create the main expression
        S_mfFilterExpression
          mainExpression =
            mfFilterExpression::create (
              "Main expression");

        // push it onto the stack
        drv.expressionsStackPush (
          mainExpression,
          "Creation of the main expression");

        --gIndenter;
      }

    Expression

      {
        $$ = $2;
      }
;


// Expression
//_______________________________________________________________________________

Or
  : OR
//  | OROR
;

Expression
  : Term
      {
      }
  | Expression Or Term
      {
        $$ = $1 + " or " + $3;
      }
;


// Term
//_______________________________________________________________________________

And
  : AND
//  | ANDAND
;

Term
  : Factor
      {
      }
  | Term And Factor
      {
        $$ = $1 + " and " + $3;
      }
;


// Factor
//_______________________________________________________________________________

Factor
  : String
      {
      }
  | NOT String
      {
        $$ = "not " + $2;
      }
  | SubExpression
;


// SubExpression
//_______________________________________________________________________________

SubExpression
  : LEFT_PARENTHESIS Expression RIGHT_PARENTHESIS
      {
        $$ = "( " + $2 + " )";
      }
;


// String
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


//_______________________________________________________________________________
%%
//_______________________________________________________________________________


// other service code
//_______________________________________________________________________________


void
mfFilter::parser::error (const location_type& loc, const std::string& message)
{
  mfFilterError (
    message,
    loc);
}
