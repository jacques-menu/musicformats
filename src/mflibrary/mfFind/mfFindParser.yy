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

#include "mfFindWae.h"


using namespace MusicFormats;

%}


/// the Bison options
//_______________________________________________________________________________

%skeleton "lalr1.cc" // -*- C++ -*-
%require "3.8.1"
%defines

%define api.prefix {mfFind}

%define api.token.raw

%define api.token.constructor
%define api.value.type variant
%define parse.assert

%code requires {
  #include <string>

  class mfFindDriver;
}

// the parsing context
%param { mfFindDriver& drv } // declaration, any parameter name is fine

%verbose // to produce mfFindParser.output

%locations

// other Bison options
%define parse.trace
%define parse.error detailed
%define parse.lac full
// %define api.pure full

%printer { yyo << $$; } <*>;


%code {
  #include "mfFindBasicTypes.h"
}


// the mfFind tokens
//_______________________________________________________________________________

%define api.token.prefix {mfFind_TOK_}

%token
  AND               "|"
  OR                "&"
  NOT               "="

  LEFT_PARENTHESIS  "("
  RIGHT_PARENTHESIS ")"
;

%code {
  #include "mfFindDriver.h"
}

%token <std::string> SINGLE_QUOTED_STRING "single quoted_string"
%token <std::string> DOUBLE_QUOTED_STRING "double quoted_string"


// the mfFind non-terminals
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


// the mfFind axiom
//_______________________________________________________________________________

%start Criterion



//_______________________________________________________________________________
%%
//_______________________________________________________________________________



//_______________________________________________________________________________
// the mfFind non-terminals
//_______________________________________________________________________________


// the mfFind axiom
//_______________________________________________________________________________

Criterion
  : Expression
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
mfFind::parser::error (const location_type& loc, const std::string& message)
{
  mfFindError (
    message,
    loc);
}
