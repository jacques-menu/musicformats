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

#include "stringFilterWae.h"

#include "stringFilterBasicTypes.h"


using namespace MusicFormats;

%}


/// the Bison options
//_______________________________________________________________________________

%skeleton "lalr1.cc" // -*- C++ -*-
%require "3.8.1"
%defines

%define api.prefix {stringfilter}

%define api.token.raw

%define api.token.constructor
%define api.value.type variant
%define parse.assert

%code requires {
  #include <string>

  class stringFilterDriver;
}

// the parsing context
%param { stringFilterDriver& drv } // declaration, any parameter name is fine

%verbose // to produce stringFilterParser.output

%locations

// other Bison options
%define parse.trace
%define parse.error detailed
%define parse.lac full
// %define api.pure full

%printer { yyo << $$; } <*>;


%code {
  #include "stringFilterBasicTypes.h"
}


// the stringfilter tokens
//_______________________________________________________________________________

%define api.token.prefix {stringFilter_TOK_}

%token
  OR                "&"
  XOR               "^"
  AND               "|"
  NOT               "="

  LEFT_PARENTHESIS  "("
  RIGHT_PARENTHESIS ")"
;

%code {
  #include "stringFilterBasicTypes.h"
  #include "stringFilterDriver.h"
}

%token <std::string> WORD "word"

%token <std::string> SINGLE_QUOTED_STRING "single quoted string"
%token <std::string> DOUBLE_QUOTED_STRING "double quoted string"


// the stringfilter non-terminals
//_______________________________________________________________________________

%nterm <S_stringFilterNode>   StringFilterSpecification

%nterm <S_stringFilterNode>   Expression

%nterm <S_stringFilterNode>   Term

%nterm <S_stringFilterNode>   Factor

%nterm <S_stringFilterNode>   BasicFactor
%nterm <S_stringFilterNode>   SubExpression

%nterm <std::string>          SingleString
%nterm<std::string>           String


// the stringfilter axiom
//_______________________________________________________________________________

%start StringFilterSpecification



//_______________________________________________________________________________
%%
//_______________________________________________________________________________



//_______________________________________________________________________________
// the stringfilter non-terminals
//_______________________________________________________________________________


// the stringfilter axiom
//_______________________________________________________________________________

StringFilterSpecification
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
          stringFilterOr::create (
            $1,
            $3);
      }

  | Expression XOR Term
      {
        $$ =
          stringFilterXor::create (
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
          stringFilterAnd::create (
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
          stringFilterNot::create (
            $2);
      }
;


// BasicFactor
//_______________________________________________________________________________

BasicFactor
  : WORD
      {
        $$ =
          stringFilterString::create (
            $1);
      }
  | String
      {
        $$ =
          stringFilterString::create (
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
stringfilter::parser::error (const location_type& loc, const std::string& message)
{
  stringFilterError (
    message,
    loc);
}
