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

#include "stringMatcherWae.h"

#include "stringMatcherBasicTypes.h"


using namespace MusicFormats;

%}


/// the Bison options
//_______________________________________________________________________________

%skeleton "lalr1.cc" // -*- C++ -*-
%require "3.8.1"
%defines

%define api.prefix {smi}

%define api.token.raw

%define api.token.constructor
%define api.value.type variant
%define parse.assert

%code requires {
  #include <string>

  class stringMatcherDriver;
}

// the parsing context
%param { stringMatcherDriver& drv } // declaration, any parameter name is fine

%verbose // to produce stringMatcherParser.output

%locations

// other Bison options
%define parse.trace
%define parse.error detailed
%define parse.lac full
// %define api.pure full

%printer { yyo << $$; } <*>;


%code {
  #include "stringMatcherBasicTypes.h"
}


// the stringMatcher interpreter tokens
//_______________________________________________________________________________

%define api.token.prefix {smi_TOK_}

%token
  OR                "&"
  XOR               "^"
  AND               "|"
  NOT               "="

  LEFT_PARENTHESIS  "("
  RIGHT_PARENTHESIS ")"
;

%code {
  #include "stringMatcherBasicTypes.h"
  #include "stringMatcherDriver.h"
}

%token <std::string> WORD "word"

%token <std::string> SINGLE_QUOTED_STRING "single quoted string"
%token <std::string> DOUBLE_QUOTED_STRING "double quoted string"


// the stringMatcher interpreter non-terminals
//_______________________________________________________________________________

%nterm <S_stringMatcherNode>   StringMatcherSpecification

%nterm <S_stringMatcherNode>   Expression

%nterm <S_stringMatcherNode>   Term

%nterm <S_stringMatcherNode>   Factor

%nterm <S_stringMatcherNode>   BasicFactor
%nterm <S_stringMatcherNode>   SubExpression

%nterm <std::string>          SingleString
%nterm<std::string>           String


// the stringMatcher interpreter interpreter axiom
//_______________________________________________________________________________

%start StringMatcherSpecification



//_______________________________________________________________________________
%%
//_______________________________________________________________________________



//_______________________________________________________________________________
// the stringMatcher interpreter non-terminals
//_______________________________________________________________________________


// the stringMatcher interpreter axiom
//_______________________________________________________________________________

StringMatcherSpecification
  :
      {
      }

    Expression

      {
        $$ = $2;
      }
;


// Expression
//_______________________________________________________________________________

Expression
  : Term
      {
      }

  | Expression OR Term
      {
        $$ =
          stringMatcherOr::create (
            $1,
            $3);
      }

  | Expression XOR Term
      {
        $$ =
          stringMatcherXor::create (
            $1,
            $3);
      }
;


// Term
//_______________________________________________________________________________

Term
  : Factor
      {
      }

  | Term AND Factor
      {
        $$ =
          stringMatcherAnd::create (
            $1,
            $3);
      }
;


// Factor
//_______________________________________________________________________________

Factor
  : BasicFactor
      {
      }

  | NOT BasicFactor
      {
        $$ =
          stringMatcherNot::create (
            $2);
      }
;


// BasicFactor
//_______________________________________________________________________________

BasicFactor
  : WORD
      {
        $$ =
          stringMatcherString::create (
            $1);
      }
  | String
      {
        $$ =
          stringMatcherString::create (
            $1);
      }

  | SubExpression
      {
      }
;


// SubExpression
//_______________________________________________________________________________

SubExpression
  : LEFT_PARENTHESIS Expression RIGHT_PARENTHESIS
      {
        $$ = $2;
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
      {
      }
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
smi::parser::error (const location_type& loc, const std::string& message)
{
  stringMatcherError (
    message,
    loc);
}
